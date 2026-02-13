/* SPDX-License-Identifier: GPL-2.0-only */

#ifndef _NF_CONNTRACK_QOS_H
#define _NF_CONNTRACK_QOS_H
#include <net/net_namespace.h>
#include <linux/netfilter/nf_conntrack_common.h>
#include <linux/netfilter/nf_conntrack_tuple_common.h>
#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_extend.h>

struct nf_conn_tos {
	u8		value;
	atomic64_t	counter;
};

struct nf_conn_qos {
	struct nf_conn_tos	tos[IP_CT_DIR_MAX];
};

static inline
struct nf_conn_qos *nf_conn_qos_find(const struct nf_conn *ct)
{
	return nf_ct_ext_find(ct, NF_CT_EXT_QOS);
}

static inline
struct nf_conn_qos *nf_ct_qos_ext_add(struct nf_conn *ct, gfp_t gfp)
{
#if IS_ENABLED(CONFIG_NF_CONNTRACK)
	struct net *net = nf_ct_net(ct);
	struct nf_conn_qos *qos;

	if (!net->ct.sysctl_qos)
		return NULL;

	qos = nf_ct_ext_add(ct, NF_CT_EXT_QOS, gfp);
	if (!qos)
		pr_debug("failed to add qos extension area");

	memset(qos, 0, sizeof(struct nf_conn_qos));

	return qos;
#else
	return NULL;
#endif
}

static inline void nf_ct_qos_clear(struct nf_conn *ct)
{
#if IS_ENABLED(CONFIG_NF_CONNTRACK)
	struct nf_conn_qos *qos;

	qos = nf_ct_ext_find(ct, NF_CT_EXT_QOS);
	if (qos)
		memset(qos, 0, sizeof(struct nf_conn_qos));
#endif
}

void nf_conntrack_qos_pernet_init(struct net *net);

#endif /* _NF_CONNTRACK_QOS_H */
