// SPDX-License-Identifier: GPL-2.0-only
/* Copyright (C) 2020 Felix Fietkau <nbd@nbd.name> */

#include <linux/kernel.h>
#include <linux/debugfs.h>
#include "mtk_eth_soc.h"

struct mtk_flow_addr_info
{
	void *src, *dest;
	u16 *src_port, *dest_port;
	bool ipv6;
};

static const char *mtk_foe_entry_state_str(int state)
{
	static const char * const state_str[] = {
		[MTK_FOE_STATE_INVALID] = "INV",
		[MTK_FOE_STATE_UNBIND] = "UNB",
		[MTK_FOE_STATE_BIND] = "BND",
		[MTK_FOE_STATE_FIN] = "FIN",
	};

	if (state >= ARRAY_SIZE(state_str) || !state_str[state])
		return "UNK";

	return state_str[state];
}

static const char *mtk_foe_pkt_type_str(int type)
{
	static const char * const type_str[] = {
		[MTK_PPE_PKT_TYPE_IPV4_HNAPT] = "IPv4 5T",
		[MTK_PPE_PKT_TYPE_IPV4_ROUTE] = "IPv4 3T",
		[MTK_PPE_PKT_TYPE_IPV4_DSLITE] = "DS-LITE",
		[MTK_PPE_PKT_TYPE_IPV6_ROUTE_3T] = "IPv6 3T",
		[MTK_PPE_PKT_TYPE_IPV6_ROUTE_5T] = "IPv6 5T",
		[MTK_PPE_PKT_TYPE_IPV6_6RD] = "6RD",
	};

	if (type >= ARRAY_SIZE(type_str) || !type_str[type])
		return "UNKNOWN";

	return type_str[type];
}

static void
mtk_print_addr(struct seq_file *m, u32 *addr, bool ipv6)
{
	__be32 n_addr[4];
	int i;

	if (!ipv6) {
		seq_printf(m, "%pI4h", addr);
		return;
	}

	for (i = 0; i < ARRAY_SIZE(n_addr); i++)
		n_addr[i] = htonl(addr[i]);
	seq_printf(m, "%pI6", n_addr);
}

static void
mtk_print_addr_info(struct seq_file *m, struct mtk_flow_addr_info *ai)
{
	mtk_print_addr(m, ai->src, ai->ipv6);
	if (ai->src_port)
		seq_printf(m, ":%d", *ai->src_port);
	seq_printf(m, "->");
	mtk_print_addr(m, ai->dest, ai->ipv6);
	if (ai->dest_port)
		seq_printf(m, ":%d", *ai->dest_port);
}

static int
mtk_ppe_debugfs_foe_show(struct seq_file *m, void *private, bool bind)
{
	struct mtk_ppe *ppe = m->private;
	int i;

	for (i = 0; i < MTK_PPE_ENTRIES; i++) {
		struct mtk_foe_entry *entry = mtk_foe_get_entry(ppe, i);
		struct mtk_foe_mac_info *l2;
		struct mtk_flow_addr_info ai = {};
		struct mtk_foe_accounting *acct;
		unsigned char h_source[ETH_ALEN];
		unsigned char h_dest[ETH_ALEN];
		int type, state;
		u32 ib2;


		state = FIELD_GET(MTK_FOE_IB1_STATE, entry->ib1);
		if (!state)
			continue;

		if (bind && state != MTK_FOE_STATE_BIND)
			continue;

		acct = mtk_ppe_mib_entry_read(ppe, i);

		type = mtk_get_ib1_pkt_type(ppe->eth, entry->ib1);
		seq_printf(m, "%05x %s %7s", i,
			   mtk_foe_entry_state_str(state),
			   mtk_foe_pkt_type_str(type));

		switch (type) {
		case MTK_PPE_PKT_TYPE_IPV4_HNAPT:
		case MTK_PPE_PKT_TYPE_IPV4_DSLITE:
			ai.src_port = &entry->ipv4.orig.src_port;
			ai.dest_port = &entry->ipv4.orig.dest_port;
			fallthrough;
		case MTK_PPE_PKT_TYPE_IPV4_ROUTE:
			ai.src = &entry->ipv4.orig.src_ip;
			ai.dest = &entry->ipv4.orig.dest_ip;
			break;
		case MTK_PPE_PKT_TYPE_IPV6_ROUTE_5T:
			ai.src_port = &entry->ipv6.src_port;
			ai.dest_port = &entry->ipv6.dest_port;
			fallthrough;
		case MTK_PPE_PKT_TYPE_IPV6_ROUTE_3T:
		case MTK_PPE_PKT_TYPE_IPV6_6RD:
			ai.src = &entry->ipv6.src_ip;
			ai.dest = &entry->ipv6.dest_ip;
			ai.ipv6 = true;
			break;
		}

		seq_printf(m, " ppe=%d", ppe->index);

		seq_printf(m, " orig=");
		mtk_print_addr_info(m, &ai);

		switch (type) {
		case MTK_PPE_PKT_TYPE_IPV4_HNAPT:
		case MTK_PPE_PKT_TYPE_IPV4_DSLITE:
			ai.src_port = &entry->ipv4.new.src_port;
			ai.dest_port = &entry->ipv4.new.dest_port;
			fallthrough;
		case MTK_PPE_PKT_TYPE_IPV4_ROUTE:
			ai.src = &entry->ipv4.new.src_ip;
			ai.dest = &entry->ipv4.new.dest_ip;
			seq_printf(m, " new=");
			mtk_print_addr_info(m, &ai);
			break;
		}

		if (type >= MTK_PPE_PKT_TYPE_IPV4_DSLITE) {
			l2 = &entry->ipv6.l2;
			ib2 = entry->ipv6.ib2;
		} else {
			l2 = &entry->ipv4.l2;
			ib2 = entry->ipv4.ib2;
		}

		*((__be32 *)h_source) = htonl(l2->src_mac_hi);
		*((__be16 *)&h_source[4]) = htons(l2->src_mac_lo);
		*((__be32 *)h_dest) = htonl(l2->dest_mac_hi);
		*((__be16 *)&h_dest[4]) = htons(l2->dest_mac_lo);

		seq_printf(m, " eth=%pM->%pM etype=%04x"
			      " vlan=%d,%d ib1=%08x ib2=%08x"
			      " packets=%llu bytes=%llu\n",
			   h_source, h_dest, ntohs(l2->etype),
			   l2->vlan1, l2->vlan2, entry->ib1, ib2,
			   acct ? acct->packets : 0, acct ? acct->bytes : 0);
	}

	return 0;
}

static int
mtk_ppe_debugfs_foe_all_show(struct seq_file *m, void *private)
{
	return mtk_ppe_debugfs_foe_show(m, private, false);
}
DEFINE_SHOW_ATTRIBUTE(mtk_ppe_debugfs_foe_all);

static int
mtk_ppe_debugfs_foe_bind_show(struct seq_file *m, void *private)
{
	return mtk_ppe_debugfs_foe_show(m, private, true);
}
DEFINE_SHOW_ATTRIBUTE(mtk_ppe_debugfs_foe_bind);

int mtk_ppe_debugfs_init(struct mtk_ppe *ppe, int index)
{
	struct dentry *root;

	snprintf(ppe->dirname, sizeof(ppe->dirname), "ppe%d", index);

	root = debugfs_create_dir(ppe->dirname, NULL);
	debugfs_create_file("entries", S_IRUGO, root, ppe, &mtk_ppe_debugfs_foe_all_fops);
	debugfs_create_file("bind", S_IRUGO, root, ppe, &mtk_ppe_debugfs_foe_bind_fops);

	return 0;
}

static int
mtk_ppe_internal_debugfs_read_dscp(struct seq_file *m, void *private)
{
	struct mtk_eth *eth = m->private;

	if (eth->dscp_toggle == 0)
		pr_info("Keep DSCP mode is disabled now!\n");
	else if (eth->dscp_toggle == 1)
		pr_info("Keep DSCP mode is enabled now!\n");

	return 0;
}

static int mtk_ppe_internal_debugfs_open_dscp(struct inode *inode, struct file *file)
{
	return single_open(file, mtk_ppe_internal_debugfs_read_dscp,
			   inode->i_private);
}

static ssize_t
mtk_ppe_internal_debugfs_write_dscp(struct file *file, const char __user *buffer,
				    size_t count, loff_t *data)
{
	struct seq_file *m = file->private_data;
	struct mtk_eth *eth = m->private;
	char buf[8];
	int len = count;

	if ((len > 8) || copy_from_user(buf, buffer, len))
		return -EFAULT;

	if (buf[0] == '0') {
		pr_info("Keep DSCP mode is going to be disabled !\n");
		eth->dscp_toggle = 0;
	} else if (buf[0] == '1') {
		pr_info("Keep DSCP mode is going to be enabled !\n");
		eth->dscp_toggle = 1;
	}

	return len;
}

static const struct file_operations mtk_ppe_internal_debugfs_dscp_fops = {
	.open = mtk_ppe_internal_debugfs_open_dscp,
	.read = seq_read,
	.llseek = seq_lseek,
	.write = mtk_ppe_internal_debugfs_write_dscp,
	.release = single_release,
};

static int
mtk_ppe_internal_debugfs_read_qos(struct seq_file *m, void *private)
{
	struct mtk_eth *eth = m->private;

	if (eth->qos_toggle == 0)
		pr_info("HQoS is disabled now!\n");
	else if (eth->qos_toggle == 1)
		pr_info("HQoS is enabled now!\n");
	else if (eth->qos_toggle == 2)
		pr_info("Per-port-per-queue mode is enabled!\n");
	else if (eth->qos_toggle == 3)
		pr_info("Adaptive Per-port-per-queue mode is enabled!\n");

	return 0;
}

static int mtk_ppe_internal_debugfs_open_qos(struct inode *inode, struct file *file)
{
	return single_open(file, mtk_ppe_internal_debugfs_read_qos,
			   inode->i_private);
}

static ssize_t
mtk_ppe_internal_debugfs_write_qos(struct file *file, const char __user *buffer,
				   size_t count, loff_t *data)
{
	struct seq_file *m = file->private_data;
	struct mtk_eth *eth = m->private;
	char buf[8];
	int len = count;

	if ((len > 8) || copy_from_user(buf, buffer, len))
		return -EFAULT;

	if (buf[0] == '0') {
		pr_info("HQoS is going to be disabled !\n");
		eth->qos_toggle = 0;
	} else if (buf[0] == '1') {
		pr_info("HQoS mode is going to be enabled !\n");
		eth->qos_toggle = 1;
	} else if (buf[0] == '2') {
		pr_info("Per-port-per-queue mode is going to be enabled !\n");
		pr_info("PPPQ use qid 3~14 (scheduler 0).\n");
		eth->qos_toggle = 2;
	} else if (buf[0] == '3') {
		pr_info("Adaptive Per-port-per-queue mode is going to be enabled !\n");
		pr_info("PPPQ use qid 3~14 (scheduler 0).\n");
		eth->qos_toggle = 3;
		eth->qdma_shaper.threshold = 6000;
	}

	return len;
}

static const struct file_operations mtk_ppe_internal_debugfs_qos_fops = {
	.open = mtk_ppe_internal_debugfs_open_qos,
	.read = seq_read,
	.llseek = seq_lseek,
	.write = mtk_ppe_internal_debugfs_write_qos,
	.release = single_release,
};

static int
mtk_ppe_internal_debugfs_level_show(struct seq_file *m, void *private)
{
	struct mtk_eth *eth = m->private;

	seq_printf(m, "PPE debug level=%d\n", eth->debug_level);

	return 0;
}

static int
mtk_ppe_internal_debugfs_level_open(struct inode *inode, struct file *file)
{
	return single_open(file, mtk_ppe_internal_debugfs_level_show,
			   inode->i_private);
}

static ssize_t
mtk_ppe_internal_debugfs_level_write(struct file *file, const char __user *buf,
				     size_t count, loff_t *offset)
{
	struct seq_file *m = file->private_data;
	struct mtk_eth *eth = (struct mtk_eth *)m->private;
	char tmp[8] = {0};
	u32 level;

	if ((count > sizeof(tmp)))
		return -EFAULT;

	if (copy_from_user(tmp, buf, count))
		return -EFAULT;

	if (sscanf(tmp, "%d", &level) != 1)
		return -EFAULT;

	if (level < 0 || level > 7) {
		pr_warn("The input debug level is invalid, "
			"it should range from 0 to 7.\n");
		return -EINVAL;
	}

	eth->debug_level = level;

	return count;
}

static const struct file_operations mtk_ppe_internal_debugfs_level_fops = {
	.open = mtk_ppe_internal_debugfs_level_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.write = mtk_ppe_internal_debugfs_level_write,
	.release = single_release,
};

static int
mtk_ppe_internal_debugfs_foe_all_show(struct seq_file *m, void *private)
{
	struct mtk_eth *eth = m->private;
	void *backup;
	int i;

	backup = m->private;
	for (i = 0; i < eth->soc->ppe_num; i++) {
		if (!eth->ppe[i])
			continue;

		m->private = eth->ppe[i];
		mtk_ppe_debugfs_foe_show(m, private, false);
	}
	m->private = backup;

	return 0;
}
DEFINE_SHOW_ATTRIBUTE(mtk_ppe_internal_debugfs_foe_all);

static int
mtk_ppe_internal_debugfs_foe_bind_show(struct seq_file *m, void *private)
{
	struct mtk_eth *eth = m->private;
	void *backup;
	int i;

	backup = m->private;
	for (i = 0; i < eth->soc->ppe_num; i++) {
		if (!eth->ppe[i])
			continue;

		m->private = eth->ppe[i];
		mtk_ppe_debugfs_foe_show(m, private, true);
	}
	m->private = backup;

	return 0;
}
DEFINE_SHOW_ATTRIBUTE(mtk_ppe_internal_debugfs_foe_bind);

int mtk_ppe_internal_debugfs_init(struct mtk_eth *eth)
{
	struct dentry *root;
	char name[16], name_symlink[48];
	long i;
	int ret = 0;

	root = debugfs_create_dir("mtk_ppe", NULL);
	if (!root)
		return -ENOMEM;

	debugfs_create_file("entries", S_IRUGO, root, eth,
			    &mtk_ppe_internal_debugfs_foe_all_fops);
	debugfs_create_file("bind", S_IRUGO, root, eth,
			    &mtk_ppe_internal_debugfs_foe_bind_fops);
	debugfs_create_file("dscp_toggle", S_IRUGO, root, eth,
			    &mtk_ppe_internal_debugfs_dscp_fops);
	debugfs_create_file("qos_toggle", S_IRUGO, root, eth,
			    &mtk_ppe_internal_debugfs_qos_fops);
	debugfs_create_file("debug_level", S_IRUGO, root, eth,
			    &mtk_ppe_internal_debugfs_level_fops);

	for (i = 0; i < (mtk_is_netsys_v2_or_greater(eth) ? 4 : 2); i++) {
		ret = snprintf(name, sizeof(name), "qdma_sch%ld", i);
		if (ret != strlen(name)) {
			ret = -ENOMEM;
			goto err;
		}
		ret = snprintf(name_symlink, sizeof(name_symlink),
			       "/sys/kernel/debug/mtketh/qdma_sch%ld", i);
		if (ret != strlen(name_symlink)) {
			ret = -ENOMEM;
			goto err;
		}
		debugfs_create_symlink(name, root, name_symlink);
	}

	for (i = 0; i < MTK_QDMA_NUM_QUEUES; i++) {
		ret = snprintf(name, sizeof(name), "qdma_txq%ld", i);
		if (ret != strlen(name)) {
			ret = -ENOMEM;
			goto err;
		}
		ret = snprintf(name_symlink, sizeof(name_symlink),
			       "/sys/kernel/debug/mtketh/qdma_txq%ld", i);
		if (ret != strlen(name_symlink)) {
			ret = -ENOMEM;
			goto err;
		}
		debugfs_create_symlink(name, root, name_symlink);
	}

	return 0;

err:
	return ret;
}
