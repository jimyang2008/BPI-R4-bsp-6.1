/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2025 MediaTek Inc.
 *
 * Author: Weijie Gao <weijie.gao@mediatek.com>
 */

#ifndef _ARM64_MEMDUMP_H_
#define _ARM64_MEMDUMP_H_

struct pt_regs;

void arm64_atf_memory_dump(int reboot_timeout);
void arm64_atf_memory_dump_save_regs(struct pt_regs *regs);

#endif /* _ARM64_MEMDUMP_H_ */
