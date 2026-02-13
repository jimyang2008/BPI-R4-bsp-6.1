// SPDX-License-Identifier: GPL-2.0-only
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/netlink.h>
#include <linux/netfilter.h>
#include <linux/workqueue.h>
#include <linux/spinlock.h>
#include <linux/netfilter/nf_conntrack_common.h>
#include <linux/netfilter/nf_tables.h>
#include <net/ip.h> /* for ipv4 options. */
#include <net/netfilter/nf_tables.h>
#include <net/netfilter/nf_tables_core.h>
#include <net/netfilter/nf_conntrack_qos.h>
#include <net/netfilter/nf_conntrack_core.h>
#include <net/netfilter/nf_conntrack_extend.h>
#include <net/netfilter/nf_flow_table.h>
#include "../bridge/br_private.h"

struct nft_flow_offload {
	struct nft_flowtable	*flowtable;
};

static void
nft_flow_offload_dscp_init(struct flow_offload *flow, const struct nf_conn *ct,
			   enum ip_conntrack_dir dir)
{
	struct nf_conn_qos *qos;

	qos = nf_conn_qos_find(ct);
	if (qos) {
		flow->tuplehash[dir].tuple.tos = qos->tos[dir].value;
		flow->tuplehash[!dir].tuple.tos = qos->tos[!dir].value;
	}
}

static enum flow_offload_xmit_type nft_xmit_type(struct dst_entry *dst)
{
	if (dst_xfrm(dst))
		return FLOW_OFFLOAD_XMIT_XFRM;

	return FLOW_OFFLOAD_XMIT_NEIGH;
}

static void nft_default_forward_path(struct nf_flow_route *route,
				     struct dst_entry *dst_cache,
				     enum ip_conntrack_dir dir)
{
	route->tuple[!dir].in.ifindex	= dst_cache->dev->ifindex;
	route->tuple[dir].dst		= dst_cache;
	route->tuple[dir].xmit_type	= nft_xmit_type(dst_cache);
}

static bool nft_is_valid_ether_device(const struct net_device *dev)
{
	if (!dev || (dev->flags & IFF_LOOPBACK) || dev->type != ARPHRD_ETHER ||
	    dev->addr_len != ETH_ALEN || !is_valid_ether_addr(dev->dev_addr))
		return false;

	return true;
}

static u16 nft_flow_offload_get_vlan_id(struct net_bridge_port *port, struct sk_buff *skb)
{
	u16 vlan_id = 0;

	if (!port || !br_opt_get(port->br, BROPT_VLAN_ENABLED))
		return 0;

	if (skb_vlan_tag_present(skb))
		vlan_id = skb_vlan_tag_get_id(skb);
	else
		br_vlan_get_pvid_rcu(skb->dev, &vlan_id);

	return vlan_id;
}

static bool nft_flow_offload_is_bridging(struct sk_buff *skb)
{
	struct net_bridge_port *port;
	unsigned char *dmac = eth_hdr(skb)->h_dest;
	bool bridging = false;
	u16 vlan_id;

	if (!netif_is_bridge_port(skb->dev))
		return false;

	rcu_read_lock();
	port = br_port_get_rcu(skb->dev);
	if (port) {
		vlan_id = nft_flow_offload_get_vlan_id(port, skb);

		/* lookup fdb entry */
		if (br_fdb_find_rcu(port->br, dmac, vlan_id))
			bridging = true;
	}
	rcu_read_unlock();

	return bridging;
}

static int nft_dev_fill_forward_path(struct net_device_path_ctx *ctx,
				     const struct nf_flow_route *route,
				     const struct dst_entry *dst_cache,
				     const struct nf_conn *ct,
				     enum ip_conntrack_dir dir, u8 *ha,
				     struct net_device_path_stack *stack)
{
	const void *daddr = &ct->tuplehash[!dir].tuple.src.u3;
	struct net_device *dev = dst_cache->dev;
	struct neighbour *n;
	u8 nud_state;

	if (!nft_is_valid_ether_device(dev))
		goto out;

	if (!is_zero_ether_addr(ha))
		goto out;

	n = dst_neigh_lookup(dst_cache, daddr);
	if (!n)
		return -1;

	read_lock_bh(&n->lock);
	nud_state = n->nud_state;
	ether_addr_copy(ha, n->ha);
	read_unlock_bh(&n->lock);
	neigh_release(n);

	if (!(nud_state & NUD_VALID))
		return -1;

out:
	return __dev_fill_forward_path(ctx, ha, stack);
}

static void nft_br_vlan_dev_fill_forward_path(const struct nft_pktinfo *pkt,
					      struct net_device_path_ctx *ctx)
{
	struct net_bridge_port *port;
	u16 vlan_id;

	rcu_read_lock();
	port = br_port_get_rcu(pkt->skb->dev);
	if (port) {
		vlan_id = nft_flow_offload_get_vlan_id(port, pkt->skb);
		if (vlan_id) {
			ctx->num_vlans = 1;
			ctx->vlan[0].id = vlan_id;
			ctx->vlan[0].proto = port->br->vlan_proto;
		}
	}
	rcu_read_unlock();
}

struct nft_forward_info {
	const struct net_device *indev;
	const struct net_device *outdev;
	const struct net_device *hw_outdev;
	struct id {
		__u16	id;
		__be16	proto;
	} encap[NF_FLOW_TABLE_ENCAP_MAX];
	u8 num_encaps;
	u8 ingress_vlans;
	u8 h_source[ETH_ALEN];
	u8 h_dest[ETH_ALEN];
	enum flow_offload_xmit_type xmit_type;
};

static void nft_dev_path_info(const struct net_device_path_stack *stack,
			      struct nft_forward_info *info,
			      unsigned char *ha, struct nf_flowtable *flowtable)
{
	const struct net_device_path *path;
	int i;

	memcpy(info->h_dest, ha, ETH_ALEN);

	for (i = 0; i < stack->num_paths; i++) {
		path = &stack->path[i];
		info->indev = path->dev;
		switch (path->type) {
		case DEV_PATH_ETHERNET:
		case DEV_PATH_DSA:
		case DEV_PATH_VLAN:
		case DEV_PATH_PPPOE:
			if (is_zero_ether_addr(info->h_source))
				memcpy(info->h_source, path->dev->dev_addr, ETH_ALEN);

			if (path->type == DEV_PATH_ETHERNET)
				break;
			if (path->type == DEV_PATH_DSA) {
				i = stack->num_paths;
				break;
			}

			/* DEV_PATH_VLAN and DEV_PATH_PPPOE */
			if (info->num_encaps >= NF_FLOW_TABLE_ENCAP_MAX) {
				info->indev = NULL;
				break;
			}
			if (!info->outdev)
				info->outdev = path->dev;
			info->encap[info->num_encaps].id = path->encap.id;
			info->encap[info->num_encaps].proto = path->encap.proto;
			info->num_encaps++;
			if (path->type == DEV_PATH_PPPOE)
				memcpy(info->h_dest, path->encap.h_dest, ETH_ALEN);
			break;
		case DEV_PATH_BRIDGE:
			if (is_zero_ether_addr(info->h_source))
				memcpy(info->h_source, path->dev->dev_addr, ETH_ALEN);

			switch (path->bridge.vlan_mode) {
			case DEV_PATH_BR_VLAN_UNTAG_HW:
				info->ingress_vlans |= BIT(info->num_encaps - 1);
				break;
			case DEV_PATH_BR_VLAN_TAG:
				info->encap[info->num_encaps].id = path->bridge.vlan_id;
				info->encap[info->num_encaps].proto = path->bridge.vlan_proto;
				info->num_encaps++;
				break;
			case DEV_PATH_BR_VLAN_UNTAG:
				if (info->num_encaps > 0)
					info->num_encaps--;
				break;
			case DEV_PATH_BR_VLAN_KEEP:
				break;
			}
			info->xmit_type = FLOW_OFFLOAD_XMIT_DIRECT;
			break;
		case DEV_PATH_MTK_WDMA:
			if (is_zero_ether_addr(info->h_source))
				memcpy(info->h_source, path->dev->dev_addr, ETH_ALEN);
			break;
		default:
			break;
		}
	}
	if (!info->outdev)
		info->outdev = info->indev;

	info->hw_outdev = info->indev;

	if (nf_flowtable_hw_offload(flowtable) &&
	    nft_is_valid_ether_device(info->indev))
		info->xmit_type = FLOW_OFFLOAD_XMIT_DIRECT;
}

static bool nft_flowtable_find_dev(const struct net_device *dev,
				   struct nft_flowtable *ft)
{
	struct nft_hook *hook;
	bool found = false;

	list_for_each_entry_rcu(hook, &ft->hook_list, list) {
		if (hook->ops.dev != dev)
			continue;

		found = true;
		break;
	}

	return found;
}

static int nft_dev_forward_path(const struct nft_pktinfo *pkt,
				struct nf_flow_route *route,
				const struct nf_conn *ct,
				enum ip_conntrack_dir dir,
				struct nft_flowtable *ft)
{
	const struct dst_entry *dst = route->tuple[dir].dst;
	struct net_device_path_stack stack;
	struct net_device_path_ctx ctx = {
		.dev	= dst->dev,
	};
	struct nft_forward_info info = {};
	struct ethhdr *eth;
	enum ip_conntrack_dir skb_dir;
	unsigned char ha[ETH_ALEN];
	int i;

	memset(ha, 0, sizeof(ha));

	if (nft_flow_offload_is_bridging(pkt->skb) && skb_mac_header_was_set(pkt->skb)) {
		eth = eth_hdr(pkt->skb);
		skb_dir = CTINFO2DIR(skb_get_nfct(pkt->skb) & NFCT_INFOMASK);
		if (skb_dir != dir) {
			memcpy(ha, eth->h_source, ETH_ALEN);
			memcpy(info.h_source, eth->h_dest, ETH_ALEN);
		} else {
			memcpy(ha, eth->h_dest, ETH_ALEN);
			memcpy(info.h_source, eth->h_source, ETH_ALEN);
		}

		nft_br_vlan_dev_fill_forward_path(pkt, &ctx);
	}

	if (nft_dev_fill_forward_path(&ctx, route, dst, ct, dir, ha, &stack) >= 0)
		nft_dev_path_info(&stack, &info, ha, &ft->data);

	if (!info.indev || !nft_flowtable_find_dev(info.indev, ft))
		return -ENOENT;

	route->tuple[!dir].in.ifindex = info.indev->ifindex;
	for (i = 0; i < info.num_encaps; i++) {
		route->tuple[!dir].in.encap[i].id = info.encap[i].id;
		route->tuple[!dir].in.encap[i].proto = info.encap[i].proto;
	}
	route->tuple[!dir].in.num_encaps = info.num_encaps;
	route->tuple[!dir].in.ingress_vlans = info.ingress_vlans;

	if (info.xmit_type == FLOW_OFFLOAD_XMIT_DIRECT) {
		memcpy(route->tuple[dir].out.h_source, info.h_source, ETH_ALEN);
		memcpy(route->tuple[dir].out.h_dest, info.h_dest, ETH_ALEN);
		route->tuple[dir].out.ifindex = info.outdev->ifindex;
		route->tuple[dir].out.hw_ifindex = info.hw_outdev->ifindex;
		route->tuple[dir].xmit_type = info.xmit_type;
	}

	return 0;
}

static int nft_flow_route_routing(const struct nft_pktinfo *pkt,
				  const struct nf_conn *ct,
				  struct nf_flow_route *route,
				  enum ip_conntrack_dir dir,
				  struct nft_flowtable *ft)
{
	struct dst_entry *this_dst = skb_dst(pkt->skb);
	struct dst_entry *other_dst = NULL;
	struct flowi fl;

	memset(&fl, 0, sizeof(fl));
	switch (nft_pf(pkt)) {
	case NFPROTO_IPV4:
		fl.u.ip4.daddr = ct->tuplehash[dir].tuple.src.u3.ip;
		fl.u.ip4.saddr = ct->tuplehash[!dir].tuple.src.u3.ip;
		fl.u.ip4.flowi4_oif = nft_in(pkt)->ifindex;
		fl.u.ip4.flowi4_iif = this_dst->dev->ifindex;
		fl.u.ip4.flowi4_tos = RT_TOS(ip_hdr(pkt->skb)->tos);
		fl.u.ip4.flowi4_mark = pkt->skb->mark;
		fl.u.ip4.flowi4_flags = FLOWI_FLAG_ANYSRC;
		break;
	case NFPROTO_IPV6:
		fl.u.ip6.daddr = ct->tuplehash[dir].tuple.src.u3.in6;
		fl.u.ip6.saddr = ct->tuplehash[!dir].tuple.src.u3.in6;
		fl.u.ip6.flowi6_oif = nft_in(pkt)->ifindex;
		fl.u.ip6.flowi6_iif = this_dst->dev->ifindex;
		fl.u.ip6.flowlabel = ip6_flowinfo(ipv6_hdr(pkt->skb));
		fl.u.ip6.flowi6_mark = pkt->skb->mark;
		fl.u.ip6.flowi6_flags = FLOWI_FLAG_ANYSRC;
		break;
	}

	if (!dst_hold_safe(this_dst))
		return -ENOENT;

	nf_route(nft_net(pkt), &other_dst, &fl, false, nft_pf(pkt));
	if (!other_dst) {
		dst_release(this_dst);
		return -ENOENT;
	}

	nft_default_forward_path(route, this_dst, dir);
	nft_default_forward_path(route, other_dst, !dir);

	if (route->tuple[dir].xmit_type	== FLOW_OFFLOAD_XMIT_NEIGH &&
	    route->tuple[!dir].xmit_type == FLOW_OFFLOAD_XMIT_NEIGH) {
		if (nft_dev_forward_path(pkt, route, ct, dir, ft) ||
		    nft_dev_forward_path(pkt, route, ct, !dir, ft)) {
			dst_release(route->tuple[dir].dst);
			dst_release(route->tuple[!dir].dst);
			return -ENOENT;
		}
	}

	return 0;
}

static int
nft_flow_route_dir(const struct nft_pktinfo *pkt,
		   const struct nf_conn *ct,
		   struct nf_flow_route *route,
		   enum ip_conntrack_dir dir,
		   int ifindex)
{
	struct dst_entry *other_dst = NULL;
	struct flowi fl;

	memset(&fl, 0, sizeof(fl));
	switch (nft_pf(pkt)) {
	case NFPROTO_IPV4:
		fl.u.ip4.daddr = ct->tuplehash[!dir].tuple.src.u3.ip;
		fl.u.ip4.flowi4_oif = ifindex;
		fl.u.ip4.flowi4_tos = RT_TOS(ip_hdr(pkt->skb)->tos);
		fl.u.ip4.flowi4_mark = pkt->skb->mark;
		fl.u.ip4.flowi4_flags = FLOWI_FLAG_ANYSRC;
		break;
	case NFPROTO_IPV6:
		fl.u.ip6.saddr = ct->tuplehash[!dir].tuple.dst.u3.in6;
		fl.u.ip6.daddr = ct->tuplehash[!dir].tuple.src.u3.in6;
		fl.u.ip6.flowi6_oif = ifindex;
		fl.u.ip6.flowlabel = ip6_flowinfo(ipv6_hdr(pkt->skb));
		fl.u.ip6.flowi6_mark = pkt->skb->mark;
		fl.u.ip6.flowi6_flags = FLOWI_FLAG_ANYSRC;
		break;
	}

	nf_route(nft_net(pkt), &other_dst, &fl, false, nft_pf(pkt));
	if (!other_dst)
		return -ENOENT;

	nft_default_forward_path(route, other_dst, dir);

	return 0;
}

static int
nft_flow_route_bridging(const struct nft_pktinfo *pkt,
			const struct nf_conn *ct,
			struct nf_flow_route *route,
			enum ip_conntrack_dir dir,
			struct nft_flowtable *ft)
{
	int ret;

	ret = nft_flow_route_dir(pkt, ct, route, dir,
				 nft_out(pkt)->ifindex);
	if (ret)
		return ret;

	ret = nft_flow_route_dir(pkt, ct, route, !dir,
				 nft_in(pkt)->ifindex);
	if (ret)
		goto err_route_dir1;

	if (route->tuple[dir].xmit_type == FLOW_OFFLOAD_XMIT_NEIGH &&
	    route->tuple[!dir].xmit_type == FLOW_OFFLOAD_XMIT_NEIGH) {
		if (nft_dev_forward_path(pkt, route, ct, dir, ft) ||
		    nft_dev_forward_path(pkt, route, ct, !dir, ft)) {
			ret = -ENOENT;
			goto err_route_dir2;
		}
	}

	return 0;

err_route_dir2:
	dst_release(route->tuple[!dir].dst);
err_route_dir1:
	dst_release(route->tuple[dir].dst);
	return ret;
}

static bool nft_flow_offload_skip(struct sk_buff *skb, int family)
{
	if (skb_sec_path(skb))
		return true;

	if (family == NFPROTO_IPV4) {
		const struct ip_options *opt;

		opt = &(IPCB(skb)->opt);

		if (unlikely(opt->optlen))
			return true;
	}

	return false;
}

static void flow_offload_ct_tcp(struct nf_conn *ct)
{
	/* conntrack will not see all packets, disable tcp window validation. */
	spin_lock_bh(&ct->lock);
	ct->proto.tcp.seen[0].flags |= IP_CT_TCP_FLAG_BE_LIBERAL;
	ct->proto.tcp.seen[1].flags |= IP_CT_TCP_FLAG_BE_LIBERAL;
	spin_unlock_bh(&ct->lock);
}

static void nft_flow_offload_eval(const struct nft_expr *expr,
				  struct nft_regs *regs,
				  const struct nft_pktinfo *pkt)
{
	struct nft_flow_offload *priv = nft_expr_priv(expr);
	struct nf_flowtable *flowtable = &priv->flowtable->data;
	struct tcphdr _tcph, *tcph = NULL;
	struct nf_flow_route route = {};
	enum ip_conntrack_info ctinfo;
	struct flow_offload *flow;
	enum ip_conntrack_dir dir;
	struct nf_conn *ct;
	struct nf_conn_qos *qos;
	struct ipv6hdr *ip6h;
	struct iphdr *iph;
	u32 offset = 0;
	u8 tos = 0;
	int ret;

	if (nft_flow_offload_skip(pkt->skb, nft_pf(pkt)))
		goto out;

	ct = nf_ct_get(pkt->skb, &ctinfo);
	if (!ct)
		goto out;

	switch (ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple.dst.protonum) {
	case IPPROTO_TCP:
		if (ct->proto.tcp.state != TCP_CONNTRACK_ESTABLISHED)
			goto out;

		tcph = skb_header_pointer(pkt->skb, nft_thoff(pkt),
					  sizeof(_tcph), &_tcph);
		if (unlikely(!tcph || tcph->fin || tcph->rst))
			goto out;
		break;
	case IPPROTO_UDP:
		break;
#ifdef CONFIG_NF_CT_PROTO_GRE
	case IPPROTO_GRE: {
		struct nf_conntrack_tuple *tuple;

		if (ct->status & IPS_NAT_MASK)
			goto out;
		tuple = &ct->tuplehash[IP_CT_DIR_ORIGINAL].tuple;
		/* No support for GRE v1 */
		if (tuple->src.u.gre.key || tuple->dst.u.gre.key)
			goto out;
		break;
	}
#endif
	default:
		goto out;
	}

	if (nf_ct_ext_exist(ct, NF_CT_EXT_HELPER) ||
	    ct->status & (IPS_SEQ_ADJUST | IPS_NAT_CLASH))
		goto out;

	if (!nf_ct_is_confirmed(ct))
		goto out;

	dir = CTINFO2DIR(ctinfo);

	qos = nf_conn_qos_find(ct);
	if (qos) {
		struct net *net = nf_ct_net(ct);

		switch (nft_pf(pkt)) {
		case NFPROTO_IPV4:
			iph = (struct iphdr *)(skb_network_header(pkt->skb) + offset);
			tos = iph->tos;
			break;
		case NFPROTO_IPV6:
			ip6h = (struct ipv6hdr *)(skb_network_header(pkt->skb) + offset);
			tos = ipv6_get_dsfield(ip6h);
			break;
		}

		qos->tos[dir].value = tos;
		atomic64_add(1, &qos->tos[dir].counter);

		if (atomic64_read(&qos->tos[dir].counter) < net->ct.sysctl_qos)
			goto out;
	}

	if (test_and_set_bit(IPS_OFFLOAD_BIT, &ct->status))
		goto out;

	if (nft_flow_offload_is_bridging(pkt->skb)) {
		if (nft_flow_route_bridging(pkt, ct, &route, dir, priv->flowtable) < 0)
			goto err_flow_route;
	} else {
		if (nft_flow_route_routing(pkt, ct, &route, dir, priv->flowtable) < 0)
			goto err_flow_route;
	}

	flow = flow_offload_alloc(ct);
	if (!flow)
		goto err_flow_alloc;

	flow_offload_route_init(flow, &route);
	if (tcph)
		flow_offload_ct_tcp(ct);

	nft_flow_offload_dscp_init(flow, ct, dir);

	__set_bit(NF_FLOW_HW_BIDIRECTIONAL, &flow->flags);
	ret = flow_offload_add(flowtable, flow);
	if (ret < 0)
		goto err_flow_add;

	dst_release(route.tuple[dir].dst);
	dst_release(route.tuple[!dir].dst);

	return;

err_flow_add:
	flow_offload_free(flow);
err_flow_alloc:
	dst_release(route.tuple[dir].dst);
	dst_release(route.tuple[!dir].dst);
err_flow_route:
	clear_bit(IPS_OFFLOAD_BIT, &ct->status);
out:
	regs->verdict.code = NFT_BREAK;
}

static int nft_flow_offload_validate(const struct nft_ctx *ctx,
				     const struct nft_expr *expr,
				     const struct nft_data **data)
{
	unsigned int hook_mask = (1 << NF_INET_FORWARD);

	if (ctx->family != NFPROTO_IPV4 &&
	    ctx->family != NFPROTO_IPV6 &&
	    ctx->family != NFPROTO_INET)
		return -EOPNOTSUPP;

	return nft_chain_validate_hooks(ctx->chain, hook_mask);
}

static const struct nla_policy nft_flow_offload_policy[NFTA_FLOW_MAX + 1] = {
	[NFTA_FLOW_TABLE_NAME]	= { .type = NLA_STRING,
				    .len = NFT_NAME_MAXLEN - 1 },
};

static int nft_flow_offload_init(const struct nft_ctx *ctx,
				 const struct nft_expr *expr,
				 const struct nlattr * const tb[])
{
	struct nft_flow_offload *priv = nft_expr_priv(expr);
	u8 genmask = nft_genmask_next(ctx->net);
	struct nft_flowtable *flowtable;

	if (!tb[NFTA_FLOW_TABLE_NAME])
		return -EINVAL;

	flowtable = nft_flowtable_lookup(ctx->table, tb[NFTA_FLOW_TABLE_NAME],
					 genmask);
	if (IS_ERR(flowtable))
		return PTR_ERR(flowtable);

	if (!nft_use_inc(&flowtable->use))
		return -EMFILE;

	priv->flowtable = flowtable;

	return nf_ct_netns_get(ctx->net, ctx->family);
}

static void nft_flow_offload_deactivate(const struct nft_ctx *ctx,
					const struct nft_expr *expr,
					enum nft_trans_phase phase)
{
	struct nft_flow_offload *priv = nft_expr_priv(expr);

	nf_tables_deactivate_flowtable(ctx, priv->flowtable, phase);
}

static void nft_flow_offload_activate(const struct nft_ctx *ctx,
				      const struct nft_expr *expr)
{
	struct nft_flow_offload *priv = nft_expr_priv(expr);

	nft_use_inc_restore(&priv->flowtable->use);
}

static void nft_flow_offload_destroy(const struct nft_ctx *ctx,
				     const struct nft_expr *expr)
{
	nf_ct_netns_put(ctx->net, ctx->family);
}

static int nft_flow_offload_dump(struct sk_buff *skb,
				 const struct nft_expr *expr, bool reset)
{
	struct nft_flow_offload *priv = nft_expr_priv(expr);

	if (nla_put_string(skb, NFTA_FLOW_TABLE_NAME, priv->flowtable->name))
		goto nla_put_failure;

	return 0;

nla_put_failure:
	return -1;
}

static struct nft_expr_type nft_flow_offload_type;
static const struct nft_expr_ops nft_flow_offload_ops = {
	.type		= &nft_flow_offload_type,
	.size		= NFT_EXPR_SIZE(sizeof(struct nft_flow_offload)),
	.eval		= nft_flow_offload_eval,
	.init		= nft_flow_offload_init,
	.activate	= nft_flow_offload_activate,
	.deactivate	= nft_flow_offload_deactivate,
	.destroy	= nft_flow_offload_destroy,
	.validate	= nft_flow_offload_validate,
	.dump		= nft_flow_offload_dump,
	.reduce		= NFT_REDUCE_READONLY,
};

static struct nft_expr_type nft_flow_offload_type __read_mostly = {
	.name		= "flow_offload",
	.ops		= &nft_flow_offload_ops,
	.policy		= nft_flow_offload_policy,
	.maxattr	= NFTA_FLOW_MAX,
	.owner		= THIS_MODULE,
};

static int __init nft_flow_offload_module_init(void)
{
	return nft_register_expr(&nft_flow_offload_type);
}

static void __exit nft_flow_offload_module_exit(void)
{
	nft_unregister_expr(&nft_flow_offload_type);
}

module_init(nft_flow_offload_module_init);
module_exit(nft_flow_offload_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Pablo Neira Ayuso <pablo@netfilter.org>");
MODULE_ALIAS_NFT_EXPR("flow_offload");
MODULE_DESCRIPTION("nftables hardware flow offload module");
