// SPDX-License-Identifier: GPL-2.0-only
/* QoS handling for netfilter. */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/netfilter.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/export.h>

#include <net/netfilter/nf_conntrack.h>
#include <net/netfilter/nf_conntrack_extend.h>
#include <net/netfilter/nf_conntrack_qos.h>

static unsigned short nf_ct_qos __read_mostly = 3;

module_param_named(qos, nf_ct_qos, ushort, 0644);
MODULE_PARM_DESC(qos, "Enable connection tracking qos.");

void nf_conntrack_qos_pernet_init(struct net *net)
{
	net->ct.sysctl_qos = nf_ct_qos;
}

