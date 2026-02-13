// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2025 MediaTek Inc.
 *
 * Author: Weijie Gao <weijie.gao@mediatek.com>
 */

#include <linux/kernel.h>
#include <linux/arm-smccc.h>
#include <linux/printk.h>
#include <linux/crash_core.h>
#include <linux/ioport.h>
#include <linux/crc32.h>

#define CONFIG_KEXEC_CORE
#include <linux/kexec.h>
#include <asm/kexec.h>
#include <asm/cacheflush.h>

/*
 * MTK_SIP_EMERG_MEM_DUMP - Do emergency memory dump thru. ethernet
 *
 * parameters
 * @x1:		reboot after memory dump
 *
 * no return
 */
#define MTK_SIP_EMERG_MEM_DUMP			0xC2000540

/* core dump info */
#define MEMDUMP_CORE_VER			2

struct memdump_core_info {
	u32 crc;
	u32 len;
	u32 ver;
	u32 nr_cpus;
	u32 nr_mem_ranges;
	u32 panic_cpu;			/* v2 */
	uintptr_t note_buf_pa;
	size_t note_buf_size;
	uintptr_t vmcoreinfo_note_pa;
	size_t vmcoreinfo_note_size;
	uintptr_t kernel_va;
	uintptr_t kernel_pa;
	size_t kernel_size;
	uintptr_t mem_ranges_pa;
	uintptr_t panic_regs_pa;	/* v2 */
	u32 panic_regs_valid;		/* v2 */
	u32 unused;			/* v2 */
};

struct memdump_mem_range {
	u64 va_start;
	u64 pa_start;
	u64 pa_end;
};

struct crash_mem_priv {
	unsigned int nr_ranges;
	struct memdump_mem_range *ranges;
};

static struct memdump_core_info *mdump_core_info;
static struct pt_regs *mdump_panic_regs;

static int get_nr_ram_ranges_callback(struct resource *res, void *arg)
{
	unsigned int *nr_ranges = arg;

	(*nr_ranges)++;
	return 0;
}

static int log_ram_headers_callback(struct resource *res, void *arg)
{
	struct crash_mem_priv *cmem = arg;

	cmem->ranges[cmem->nr_ranges].va_start = (u64)__va(res->start);
	cmem->ranges[cmem->nr_ranges].pa_start = res->start;
	cmem->ranges[cmem->nr_ranges].pa_end = res->end;
	cmem->nr_ranges++;

	return 0;
}

static void prepare_mdump_core_info(void)
{
	struct memdump_mem_range *mem_ranges;
	struct crash_mem_priv cmpriv;
	uint32_t cpu, nr_ranges = 0;
	uintptr_t *note_buf;
	size_t len;

	walk_system_ram_res(0, -1, &nr_ranges, get_nr_ram_ranges_callback);

	mdump_core_info->ver = MEMDUMP_CORE_VER;
	mdump_core_info->nr_cpus = NR_CPUS;
	mdump_core_info->panic_cpu = smp_processor_id();

	note_buf = (uintptr_t *)((uintptr_t)mdump_core_info + sizeof(*mdump_core_info));
	mdump_core_info->note_buf_pa = __pa(note_buf);
	mdump_core_info->note_buf_size = sizeof(note_buf_t);

	mdump_core_info->vmcoreinfo_note_pa = paddr_vmcoreinfo_note();
	mdump_core_info->vmcoreinfo_note_size = VMCOREINFO_NOTE_SIZE;

	mdump_core_info->kernel_va = (uintptr_t)_text;
	mdump_core_info->kernel_pa = __pa_symbol(_text);
	mdump_core_info->kernel_size = _end - _text;

	for_each_present_cpu(cpu) {
		note_buf[cpu] = per_cpu_ptr_to_phys(per_cpu_ptr(crash_notes, cpu));
	}

	mem_ranges = (struct memdump_mem_range *)((uintptr_t)note_buf + NR_CPUS * sizeof(uintptr_t));
	mdump_core_info->mem_ranges_pa = __pa(mem_ranges);

	cmpriv.nr_ranges = 0;
	cmpriv.ranges = mem_ranges;

	walk_system_ram_res(0, -1, &cmpriv, log_ram_headers_callback);

	mdump_core_info->nr_mem_ranges = cmpriv.nr_ranges;

	mdump_core_info->panic_regs_pa = __pa(mdump_panic_regs);

	len = sizeof(*mdump_core_info) +
	      NR_CPUS * sizeof(uintptr_t) +
	      cmpriv.nr_ranges * sizeof(struct memdump_mem_range);

	mdump_core_info->len = len;

	mdump_core_info->crc = crc32(0xffffffff, mdump_core_info, len) ^ 0xffffffff;

	dcache_clean_inval_poc((unsigned long)mdump_core_info, len);
}

void arm64_atf_memory_dump(int reboot_timeout)
{
	struct arm_smccc_res res;
	struct pt_regs regs;

	crash_setup_regs(&regs, NULL);
	crash_save_vmcoreinfo();

	/* for crashing cpu */
	crash_save_cpu(&regs, smp_processor_id());

	prepare_mdump_core_info();

	pr_emerg("Starting Memory dump SMCC\n");

	arm_smccc_smc(MTK_SIP_EMERG_MEM_DUMP, reboot_timeout,
		      __pa(mdump_core_info), 0, 0, 0, 0, 0, &res);

	pr_err("Memory dump SMCC failed\n");
}

void arm64_atf_memory_dump_save_regs(struct pt_regs *regs)
{
	mdump_core_info->panic_regs_valid = true;

	memcpy(mdump_panic_regs, regs, sizeof(*regs));

	dcache_clean_inval_poc((unsigned long)mdump_panic_regs, sizeof(*regs));
}

/* dummy functions */
void machine_kexec_cleanup(struct kimage *kimage)
{
}

void machine_crash_shutdown(struct pt_regs *regs)
{
}

void machine_kexec(struct kimage *kimage)
{
	BUG();
}

static int __init memdump_core_info_init(void)
{
	mdump_core_info = (struct memdump_core_info *)get_zeroed_page(GFP_KERNEL);
	if (!mdump_core_info) {
		pr_warn("Memory allocation for memdump_core_info failed\n");
		return -ENOMEM;
	}

	mdump_panic_regs = (struct pt_regs *)get_zeroed_page(GFP_KERNEL);
	if (!mdump_panic_regs) {
		pr_warn("Memory allocation for pt_regs failed\n");
		free_page((unsigned long)mdump_core_info);
		return -ENOMEM;
	}

	return 0;
}

subsys_initcall(memdump_core_info_init);
