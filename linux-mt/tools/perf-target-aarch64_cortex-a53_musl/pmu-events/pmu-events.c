
#include <pmu-events/pmu-events.h>
#include "util/header.h"
#include "util/pmu.h"
#include <string.h>
#include <stddef.h>

struct compact_pmu_event {
        int offset;
};

struct pmu_table_entry {
        const struct compact_pmu_event *entries;
        uint32_t num_entries;
        struct compact_pmu_event pmu_name;
};

static const char *const big_c_string =
/* offset=0 */ "default_core\000"
/* offset=13 */ "br_immed_spec\000branch\000Branch speculatively executed, immediate branch\000event=0x78\000\00000\000Branch speculatively executed, immediate branch\000"
/* offset=145 */ "br_return_spec\000branch\000Branch speculatively executed, procedure return\000event=0x79\000\00000\000Branch speculatively executed, procedure return\000"
/* offset=278 */ "br_indirect_spec\000branch\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000"
/* offset=411 */ "br_mis_pred\000branch\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000Mispredicted or not predicted branch speculatively executed\000"
/* offset=565 */ "br_pred\000branch\000Predictable branch speculatively executed\000event=0x12\000\00000\000Predictable branch speculatively executed\000"
/* offset=679 */ "cpu_cycles\000bus\000Cycle\000event=0x11\000\00000\000Cycle\000"
/* offset=721 */ "bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000Bus cycle\000"
/* offset=771 */ "bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Bus access read\000"
/* offset=836 */ "bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Bus access write\000"
/* offset=903 */ "bus_access_shared\000bus\000Bus access, Normal, Cacheable, Shareable\000event=0x62\000\00000\000Bus access, Normal, Cacheable, Shareable\000"
/* offset=1022 */ "bus_access_not_shared\000bus\000Bus access, not Normal, Cacheable, Shareable\000event=0x63\000\00000\000Bus access, not Normal, Cacheable, Shareable\000"
/* offset=1153 */ "bus_access_normal\000bus\000Bus access, Normal\000event=0x64\000\00000\000Bus access, Normal\000"
/* offset=1228 */ "bus_access_periph\000bus\000Bus access, peripheral\000event=0x65\000\00000\000Bus access, peripheral\000"
/* offset=1311 */ "bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000Attributable Bus access\000"
/* offset=1389 */ "cnt_cycles\000bus\000Constant frequency cycles\000event=0x4004\000\00000\000Constant frequency cycles.  The counter increments at a constant frequency equal to the rate of increment of the system counter, CNTPCT_EL0\000"
/* offset=1587 */ "l1d_cache_rd\000cache\000L1D cache access, read\000event=0x40\000\00000\000Attributable Level 1 data cache access, read\000"
/* offset=1689 */ "l1d_cache_wr\000cache\000L1D cache access, write\000event=0x41\000\00000\000Attributable Level 1 data cache access, write\000"
/* offset=1793 */ "l1d_cache_refill_rd\000cache\000L1D cache refill, read\000event=0x42\000\00000\000Attributable Level 1 data cache refill, read\000"
/* offset=1902 */ "l1d_cache_inval\000cache\000L1D cache invalidate\000event=0x48\000\00000\000Attributable Level 1 data cache invalidate\000"
/* offset=2003 */ "l1d_tlb_refill_rd\000cache\000L1D tlb refill, read\000event=0x4c\000\00000\000Attributable Level 1 data TLB refill, read\000"
/* offset=2106 */ "l1d_tlb_refill_wr\000cache\000L1D tlb refill, write\000event=0x4d\000\00000\000Attributable Level 1 data TLB refill, write\000"
/* offset=2211 */ "l2d_cache_rd\000cache\000L2D cache access, read\000event=0x50\000\00000\000Attributable Level 2 data cache access, read\000"
/* offset=2313 */ "l2d_cache_wr\000cache\000L2D cache access, write\000event=0x51\000\00000\000Attributable Level 2 data cache access, write\000"
/* offset=2417 */ "l2d_cache_refill_rd\000cache\000L2D cache refill, read\000event=0x52\000\00000\000Attributable Level 2 data cache refill, read\000"
/* offset=2526 */ "l2d_cache_refill_wr\000cache\000L2D cache refill, write\000event=0x53\000\00000\000Attributable Level 2 data cache refill, write\000"
/* offset=2637 */ "l2d_cache_wb_victim\000cache\000L2D cache Write-Back, victim\000event=0x56\000\00000\000Attributable Level 2 data cache Write-Back, victim\000"
/* offset=2758 */ "l2d_cache_wb_clean\000cache\000L2D cache Write-Back, cleaning and coherency\000event=0x57\000\00000\000Level 2 data cache Write-Back, cleaning and coherency\000"
/* offset=2897 */ "l2d_cache_inval\000cache\000L2D cache invalidate\000event=0x58\000\00000\000Attributable Level 2 data cache invalidate\000"
/* offset=2998 */ "l1i_cache_refill\000cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Level 1 instruction cache refill\000"
/* offset=3101 */ "l1i_tlb_refill\000cache\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Attributable Level 1 instruction TLB refill\000"
/* offset=3224 */ "l1d_cache_refill\000cache\000Level 1 data cache refill\000event=0x3\000\00000\000Level 1 data cache refill\000"
/* offset=3313 */ "l1d_cache\000cache\000Level 1 data cache access\000event=0x4\000\00000\000Level 1 data cache access\000"
/* offset=3395 */ "l1d_tlb_refill\000cache\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Attributable Level 1 data TLB refill\000"
/* offset=3504 */ "l1i_cache\000cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Attributable Level 1 instruction cache access\000"
/* offset=3627 */ "l2d_cache\000cache\000Level 2 data cache access\000event=0x16\000\00000\000Level 2 data cache access\000"
/* offset=3710 */ "l2d_cache_refill\000cache\000Level 2 data refill\000event=0x17\000\00000\000Level 2 data refill\000"
/* offset=3788 */ "l2d_cache_wb\000cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Attributable Level 2 data cache write-back\000"
/* offset=3908 */ "l1d_tlb\000cache\000Attributable Level 1 data or unified TLB access\000event=0x25\000\00000\000Attributable Level 1 data or unified TLB access\000"
/* offset=4033 */ "l1i_tlb\000cache\000Attributable Level 1 instruction TLB access\000event=0x26\000\00000\000Attributable Level 1 instruction TLB access\000"
/* offset=4150 */ "l2d_tlb_refill\000cache\000Attributable Level 2 data TLB refill\000event=0x2d\000\00000\000Attributable Level 2 data TLB refill\000"
/* offset=4260 */ "l2i_tlb_refill\000cache\000Attributable Level 2 instruction TLB refill\000event=0x2e\000\00000\000Attributable Level 2 instruction TLB refill\000"
/* offset=4384 */ "l2d_tlb\000cache\000Attributable Level 2 data or unified TLB access\000event=0x2f\000\00000\000Attributable Level 2 data or unified TLB access\000"
/* offset=4509 */ "l2i_tlb\000cache\000Attributable Level 2 instruction TLB access\000event=0x30\000\00000\000Attributable Level 2 instruction TLB access\000"
/* offset=4626 */ "dtlb_walk\000cache\000Access to data TLB causes a translation table walk\000event=0x34\000\00000\000Access to data TLB causes a translation table walk\000"
/* offset=4759 */ "itlb_walk\000cache\000Access to instruction TLB that causes a translation table walk\000event=0x35\000\00000\000Access to instruction TLB that causes a translation table walk\000"
/* offset=4916 */ "l1d_cache_lmiss_rd\000cache\000Level 1 data cache long-latency read miss\000event=0x39\000\00000\000Level 1 data cache long-latency read miss.  The counter counts each memory read access counted by L1D_CACHE that incurs additional latency because it returns data from outside the Level 1 data or unified cache of this processing element\000"
/* offset=5235 */ "l1i_cache_lmiss\000cache\000Level 1 instruction cache long-latency read miss\000event=0x4006\000\00000\000Level 1 instruction cache long-latency read miss.  If the L1I_CACHE_RD event is implemented, the counter counts each access counted by L1I_CACHE_RD that incurs additional latency because it returns instructions from outside of the Level 1 instruction cache of this PE.  If the L1I_CACHE_RD event is not implemented, the counter counts each access counted by L1I_CACHE that incurs additional latency because it returns instructions from outside the Level 1 instruction cache of this PE.  The event indicates to software that the access missed in the Level 1 instruction cache and might have a significant performance impact due to the additional latency, compared to the latency of an access that hits in the Level 1 instruction cache\000"
/* offset=6057 */ "l2d_cache_lmiss_rd\000cache\000Level 2 data cache long-latency read miss\000event=0x4009\000\00000\000Level 2 data cache long-latency read miss.  The counter counts each memory read access counted by L2D_CACHE that incurs additional latency because it returns data from outside the Level 2 data or unified cache of this processing element.  The event indicates to software that the access missed in the Level 2 data or unified cache and might have a significant performance impact compared to the latency of an access that hits in the Level 2 data or unified cache\000"
/* offset=6604 */ "l2_prefetch_refill\000core imp def\000Level 2 prefetch requests, refilled to L2 cache\000event=0x10a\000\00000\000Level 2 prefetch requests, refilled to L2 cache\000"
/* offset=6748 */ "l2_prefetch_upgrade\000core imp def\000Level 2 prefetch requests, late\000event=0x10b\000\00000\000Level 2 prefetch requests, late\000"
/* offset=6861 */ "bpu_hit_btb\000core imp def\000Predictable branch speculatively executed that hit any level of BTB\000event=0x110\000\00000\000Predictable branch speculatively executed that hit any level of BTB\000"
/* offset=7038 */ "bpu_conditional_branch_hit_btb\000core imp def\000Predictable conditional branch speculatively executed that hit any level of BTB\000event=0x111\000\00000\000Predictable conditional branch speculatively executed that hit any level of BTB\000"
/* offset=7258 */ "bpu_hit_indirect_predictor\000core imp def\000Predictable taken branch speculatively executed that hit any level of BTB that access the indirect predictor\000event=0x112\000\00000\000Predictable taken branch speculatively executed that hit any level of BTB that access the indirect predictor\000"
/* offset=7532 */ "bpu_hit_rsb\000core imp def\000Predictable taken branch speculatively executed that hit any level of BTB that access the return predictor\000event=0x113\000\00000\000Predictable taken branch speculatively executed that hit any level of BTB that access the return predictor\000"
/* offset=7787 */ "bpu_unconditional_branch_miss_btb\000core imp def\000Predictable unconditional branch speculatively executed that did not hit any level of BTB\000event=0x114\000\00000\000Predictable unconditional branch speculatively executed that did not hit any level of BTB\000"
/* offset=8030 */ "bpu_branch_no_hit\000core imp def\000Predictable branch speculatively executed, unpredicted\000event=0x115\000\00000\000Predictable branch speculatively executed, unpredicted\000"
/* offset=8187 */ "bpu_hit_btb_and_mispredict\000core imp def\000Predictable branch speculatively executed that hit any level of BTB that mispredict\000event=0x116\000\00000\000Predictable branch speculatively executed that hit any level of BTB that mispredict\000"
/* offset=8411 */ "bpu_conditional_branch_hit_btb_and_mispredict\000core imp def\000Predictable conditional branch speculatively executed that hit any level of BTB that (direction) mispredict\000event=0x117\000\00000\000Predictable conditional branch speculatively executed that hit any level of BTB that (direction) mispredict\000"
/* offset=8702 */ "bpu_indirect_branch_hit_btb_and_mispredict\000core imp def\000Predictable taken branch speculatively executed that hit any level of BTB that access the indirect predictor that mispredict\000event=0x118\000\00000\000Predictable taken branch speculatively executed that hit any level of BTB that access the indirect predictor that mispredict\000"
/* offset=9024 */ "bpu_hit_rsb_and_mispredict\000core imp def\000Predictable taken branch speculatively executed that hit any level of BTB that access the return predictor that mispredict\000event=0x119\000\00000\000Predictable taken branch speculatively executed that hit any level of BTB that access the return predictor that mispredict\000"
/* offset=9326 */ "bpu_miss_rsb_and_mispredict\000core imp def\000Predictable taken branch speculatively executed that hit any level of BTB that access the overflow/underflow return predictor that mispredict\000event=0x11a\000\00000\000Predictable taken branch speculatively executed that hit any level of BTB that access the overflow/underflow return predictor that mispredict\000"
/* offset=9667 */ "bpu_no_prediction_mispredict\000core imp def\000Predictable branch speculatively executed, unpredicted, that mispredict\000event=0x11b\000\00000\000Predictable branch speculatively executed, unpredicted, that mispredict\000"
/* offset=9869 */ "bpu_btb_update\000core imp def\000Predictable branch speculatively executed, unpredicted, that mispredict\000event=0x11c\000\00000\000Predictable branch speculatively executed, unpredicted, that mispredict\000"
/* offset=10057 */ "bpu_rsb_full_stall\000core imp def\000Count predict pipe stalls due to speculative return address predictor full\000event=0x11d\000\00000\000Count predict pipe stalls due to speculative return address predictor full\000"
/* offset=10255 */ "icf_inst_spec_decode\000core imp def\000Macro-ops speculatively decoded\000event=0x11f\000\00000\000Macro-ops speculatively decoded\000"
/* offset=10369 */ "gpc_flush\000core imp def\000Flushes\000event=0x120\000\00000\000Flushes\000"
/* offset=10424 */ "gpc_flush_mem_fault\000core imp def\000Flushes due to memory hazards\000event=0x121\000\00000\000Flushes due to memory hazards\000"
/* offset=10533 */ "msc_etm_extout0\000core imp def\000ETM extout bit 0\000event=0x141\000\00000\000ETM extout bit 0\000"
/* offset=10612 */ "msc_etm_extout1\000core imp def\000ETM extout bit 1\000event=0x142\000\00000\000ETM extout bit 1\000"
/* offset=10691 */ "msc_etm_extout2\000core imp def\000ETM extout bit 2\000event=0x143\000\00000\000ETM extout bit 2\000"
/* offset=10770 */ "msc_etm_extout3\000core imp def\000ETM extout bit 3\000event=0x144\000\00000\000ETM extout bit 3\000"
/* offset=10849 */ "l2c_snoop\000core imp def\000Bus request sn\000event=0x156\000\00000\000Bus request sn\000"
/* offset=10918 */ "l2c_dat_crd_stall\000core imp def\000L2 TXDAT LCRD blocked\000event=0x169\000\00000\000L2 TXDAT LCRD blocked\000"
/* offset=11009 */ "l2c_rsp_crd_stall\000core imp def\000L2 TXRSP LCRD blocked\000event=0x16a\000\00000\000L2 TXRSP LCRD blocked\000"
/* offset=11100 */ "l2c_req_crd_stall\000core imp def\000L2 TXREQ LCRD blocked\000event=0x16b\000\00000\000L2 TXREQ LCRD blocked\000"
/* offset=11191 */ "icf_early_mis_pred\000core imp def\000Early mispredict\000event=0xd100\000\00000\000Early mispredict\000"
/* offset=11274 */ "icf_feq_full\000core imp def\000FEQ full cycles\000event=0xd101\000\00000\000FEQ full cycles\000"
/* offset=11349 */ "icf_inst_fifo_full\000core imp def\000Instruction FIFO Full\000event=0xd102\000\00000\000Instruction FIFO Full\000"
/* offset=11442 */ "l1i_tlb_miss\000core imp def\000L1I TLB miss\000event=0xd103\000\00000\000L1I TLB miss\000"
/* offset=11511 */ "icf_stall\000core imp def\000ICF sent 0 instructions to IDR this cycle\000event=0xd104\000\00000\000ICF sent 0 instructions to IDR this cycle\000"
/* offset=11635 */ "icf_pc_fifo_full\000core imp def\000PC FIFO Full\000event=0xd105\000\00000\000PC FIFO Full\000"
/* offset=11708 */ "idr_stall_bob_id\000core imp def\000Stall due to BOB ID\000event=0xd200\000\00000\000Stall due to BOB ID\000"
/* offset=11795 */ "idr_stall_lob_id\000core imp def\000Dispatch stall due to LOB entries\000event=0xd201\000\00000\000Dispatch stall due to LOB entries\000"
/* offset=11910 */ "idr_stall_sob_id\000core imp def\000Dispatch stall due to SOB entries\000event=0xd202\000\00000\000Dispatch stall due to SOB entries\000"
/* offset=12025 */ "idr_stall_ixu_sched\000core imp def\000Dispatch stall due to IXU scheduler entries\000event=0xd203\000\00000\000Dispatch stall due to IXU scheduler entries\000"
/* offset=12163 */ "idr_stall_fsu_sched\000core imp def\000Dispatch stall due to FSU scheduler entries\000event=0xd204\000\00000\000Dispatch stall due to FSU scheduler entries\000"
/* offset=12301 */ "idr_stall_rob_id\000core imp def\000Dispatch stall due to ROB entries\000event=0xd205\000\00000\000Dispatch stall due to ROB entries\000"
/* offset=12416 */ "idr_stall_flush\000core imp def\000Dispatch stall due to flush (6 cycles)\000event=0xd206\000\00000\000Dispatch stall due to flush (6 cycles)\000"
/* offset=12540 */ "idr_stall_wfi\000core imp def\000Dispatch stall due to WFI\000event=0xd207\000\00000\000Dispatch stall due to WFI\000"
/* offset=12636 */ "idr_stall_swob_timeout\000core imp def\000Number of SWOB drains triggered by timeout\000event=0xd208\000\00000\000Number of SWOB drains triggered by timeout\000"
/* offset=12775 */ "idr_stall_swob_raw\000core imp def\000Number of SWOB drains triggered by system register or special-purpose register read-after-write or specific special-purpose register writes that cause SWOB drain\000event=0xd209\000\00000\000Number of SWOB drains triggered by system register or special-purpose register read-after-write or specific special-purpose register writes that cause SWOB drain\000"
/* offset=13148 */ "idr_stall_swob_full\000core imp def\000Number of SWOB drains triggered by system register write when SWOB full\000event=0xd20a\000\00000\000Number of SWOB drains triggered by system register write when SWOB full\000"
/* offset=13342 */ "stall_frontend_cache\000core imp def\000Dispatch stall due to L1 instruction cache miss\000event=0xd20b\000\00000\000Dispatch stall due to L1 instruction cache miss\000"
/* offset=13489 */ "stall_frontend_tlb\000core imp def\000Dispatch stall due to L1 instruction TLB miss\000event=0xd20c\000\00000\000Dispatch stall due to L1 instruction TLB miss\000"
/* offset=13630 */ "stall_backend_cache\000core imp def\000Dispatch stall due to L1 data cache miss\000event=0xd20d\000\00000\000Dispatch stall due to L1 data cache miss\000"
/* offset=13762 */ "stall_backend_tlb\000core imp def\000Dispatch stall due to L1 data TLB miss\000event=0xd20e\000\00000\000Dispatch stall due to L1 data TLB miss\000"
/* offset=13888 */ "stall_backend_resource\000core imp def\000Dispatch stall due to lack of any core resource\000event=0xd20f\000\00000\000Dispatch stall due to lack of any core resource\000"
/* offset=14037 */ "ixu_num_uops_issued\000core imp def\000Instructions issued by the scheduler\000event=0xd300\000\00000\000Instructions issued by the scheduler\000"
/* offset=14161 */ "ixu_issue_cancel\000core imp def\000Any uop issued was canceled for any reason\000event=0xd301\000\00000\000Any uop issued was canceled for any reason\000"
/* offset=14294 */ "ixu_load_cancel\000core imp def\000A load wakeup to the scheduler has been cancelled\000event=0xd302\000\00000\000A load wakeup to the scheduler has been cancelled\000"
/* offset=14440 */ "ixu_slow_cancel\000core imp def\000The scheduler had to cancel one slow Uop due to resource conflict\000event=0xd303\000\00000\000The scheduler had to cancel one slow Uop due to resource conflict\000"
/* offset=14618 */ "ixu_ixa_issued\000core imp def\000Uops issued by the scheduler on IXA\000event=0xd304\000\00000\000Uops issued by the scheduler on IXA\000"
/* offset=14735 */ "ixu_ixa_par0_issued\000core imp def\000Uops issued by the scheduler on IXA Par 0\000event=0xd305\000\00000\000Uops issued by the scheduler on IXA Par 0\000"
/* offset=14869 */ "ixu_ixa_par1_issued\000core imp def\000Uops issued by the scheduler on IXA Par 1\000event=0xd306\000\00000\000Uops issued by the scheduler on IXA Par 1\000"
/* offset=15003 */ "ixu_ixb_issued\000core imp def\000Uops issued by the scheduler on IXB\000event=0xd307\000\00000\000Uops issued by the scheduler on IXB\000"
/* offset=15120 */ "ixu_ixb_par0_issued\000core imp def\000Uops issued by the scheduler on IXB Par 0\000event=0xd308\000\00000\000Uops issued by the scheduler on IXB Par 0\000"
/* offset=15254 */ "ixu_ixb_par1_issued\000core imp def\000Uops issued by the scheduler on IXB Par 1\000event=0xd309\000\00000\000Uops issued by the scheduler on IXB Par 1\000"
/* offset=15388 */ "ixu_ixc_issued\000core imp def\000Uops issued by the scheduler on IXC\000event=0xd30a\000\00000\000Uops issued by the scheduler on IXC\000"
/* offset=15505 */ "ixu_ixc_par0_issued\000core imp def\000Uops issued by the scheduler on IXC Par 0\000event=0xd30b\000\00000\000Uops issued by the scheduler on IXC Par 0\000"
/* offset=15639 */ "ixu_ixc_par1_issued\000core imp def\000Uops issued by the scheduler on IXC Par 1\000event=0xd30c\000\00000\000Uops issued by the scheduler on IXC Par 1\000"
/* offset=15773 */ "ixu_ixd_issued\000core imp def\000Uops issued by the scheduler on IXD\000event=0xd30d\000\00000\000Uops issued by the scheduler on IXD\000"
/* offset=15890 */ "ixu_ixd_par0_issued\000core imp def\000Uops issued by the scheduler on IXD Par 0\000event=0xd30e\000\00000\000Uops issued by the scheduler on IXD Par 0\000"
/* offset=16024 */ "ixu_ixd_par1_issued\000core imp def\000Uops issued by the scheduler on IXD Par 1\000event=0xd30f\000\00000\000Uops issued by the scheduler on IXD Par 1\000"
/* offset=16158 */ "fsu_issued\000core imp def\000Uops issued by the FSU scheduler\000event=0xd400\000\00000\000Uops issued by the FSU scheduler\000"
/* offset=16265 */ "fsu_fsx_issued\000core imp def\000Uops issued by the scheduler on pipe X\000event=0xd401\000\00000\000Uops issued by the scheduler on pipe X\000"
/* offset=16388 */ "fsu_fsy_issued\000core imp def\000Uops issued by the scheduler on pipe Y\000event=0xd402\000\00000\000Uops issued by the scheduler on pipe Y\000"
/* offset=16511 */ "fsu_fsz_issued\000core imp def\000Uops issued by the scheduler on pipe Z\000event=0xd403\000\00000\000Uops issued by the scheduler on pipe Z\000"
/* offset=16634 */ "fsu_cancel\000core imp def\000Uops canceled (load cancels)\000event=0xd404\000\00000\000Uops canceled (load cancels)\000"
/* offset=16733 */ "fsu_div_sqrt_stall\000core imp def\000Count scheduler stalls due to divide/sqrt\000event=0xd405\000\00000\000Count scheduler stalls due to divide/sqrt\000"
/* offset=16866 */ "gpc_swob_drain\000core imp def\000Number of SWOB drains\000event=0xd500\000\00000\000Number of SWOB drains\000"
/* offset=16955 */ "breakpoint_match\000core imp def\000GPC detected a Breakpoint instruction match\000event=0xd501\000\00000\000GPC detected a Breakpoint instruction match\000"
/* offset=17090 */ "l1d_tlb_miss\000core imp def\000L1D TLB miss\000event=0xd600\000\00000\000L1D TLB miss\000"
/* offset=17159 */ "ofb_full\000core imp def\000OFB full cycles\000event=0xd601\000\00000\000OFB full cycles\000"
/* offset=17230 */ "ld_from_st_fwd\000core imp def\000Load satisified from store forwarded data\000event=0xd605\000\00000\000Load satisified from store forwarded data\000"
/* offset=17359 */ "l1_pfetch_ld_gen\000core imp def\000L1 prefetcher, load prefetch requests generated\000event=0xd606\000\00000\000L1 prefetcher, load prefetch requests generated\000"
/* offset=17502 */ "l1_pfetch_ld_fill\000core imp def\000L1 prefetcher, load prefetch fills into the L1 cache\000event=0xd607\000\00000\000L1 prefetcher, load prefetch fills into the L1 cache\000"
/* offset=17656 */ "l1_pfetch_l2_req\000core imp def\000L1 prefetcher, load prefetch to L2 generated\000event=0xd608\000\00000\000L1 prefetcher, load prefetch to L2 generated\000"
/* offset=17793 */ "l1_pfetch_dist_rst\000core imp def\000L1 prefetcher, distance was reset\000event=0xd609\000\00000\000L1 prefetcher, distance was reset\000"
/* offset=17910 */ "l1_pfetch_dist_inc\000core imp def\000L1 prefetcher, distance was increased\000event=0xd60a\000\00000\000L1 prefetcher, distance was increased\000"
/* offset=18035 */ "l1_pfetch_entry_trained\000core imp def\000L1 prefetcher, table entry is trained\000event=0xd60b\000\00000\000L1 prefetcher, table entry is trained\000"
/* offset=18165 */ "lsu_st_retire_stall\000core imp def\000Store retirement pipe stall\000event=0xd60c\000\00000\000Store retirement pipe stall\000"
/* offset=18271 */ "watchpoint_match\000core imp def\000LSU detected a Watchpoint data match\000event=0xd60d\000\00000\000LSU detected a Watchpoint data match\000"
/* offset=18392 */ "l2c_pipe_replay\000core imp def\000L2 pipeline replay\000event=0xd700\000\00000\000L2 pipeline replay\000"
/* offset=18476 */ "l2c_inst_refill\000core imp def\000L2 refill from I-side miss\000event=0xd701\000\00000\000L2 refill from I-side miss\000"
/* offset=18576 */ "l2c_data_refill\000core imp def\000L2 refill from D-side miss\000event=0xd702\000\00000\000L2 refill from D-side miss\000"
/* offset=18676 */ "l2_prefetch_req\000core imp def\000L2 prefetcher, load prefetch requests generated\000event=0xd703\000\00000\000L2 prefetcher, load prefetch requests generated\000"
/* offset=18818 */ "mmu_d_otb_alloc\000core imp def\000L2D OTB allocate\000event=0xd800\000\00000\000L2D OTB allocate\000"
/* offset=18898 */ "mmu_d_s1_walk_fault\000core imp def\000D-side Stage1 tablewalk fault\000event=0xd80b\000\00000\000D-side Stage1 tablewalk fault\000"
/* offset=19008 */ "mmu_d_s2_walk_fault\000core imp def\000D-side Stage2 tablewalk fault\000event=0xd80c\000\00000\000D-side Stage2 tablewalk fault\000"
/* offset=19118 */ "mmu_d_walk_steps\000core imp def\000D-side Tablewalk steps or descriptor fetches\000event=0xd80d\000\00000\000D-side Tablewalk steps or descriptor fetches\000"
/* offset=19255 */ "mmu_i_otb_alloc\000core imp def\000L2I OTB allocate\000event=0xd900\000\00000\000L2I OTB allocate\000"
/* offset=19335 */ "mmu_i_s1_walk_fault\000core imp def\000I-side Stage1 tablewalk fault\000event=0xd90b\000\00000\000I-side Stage1 tablewalk fault\000"
/* offset=19445 */ "mmu_i_s2_walk_fault\000core imp def\000I-side Stage2 tablewalk fault\000event=0xd90c\000\00000\000I-side Stage2 tablewalk fault\000"
/* offset=19555 */ "mmu_i_walk_steps\000core imp def\000I-side Tablewalk steps or descriptor fetches\000event=0xd90d\000\00000\000I-side Tablewalk steps or descriptor fetches\000"
/* offset=19692 */ "exc_undef\000exception\000Exception taken, Other synchronous\000event=0x81\000\00000\000Exception taken, Other synchronous\000"
/* offset=19797 */ "exc_svc\000exception\000Exception taken, Supervisor Call\000event=0x82\000\00000\000Exception taken, Supervisor Call\000"
/* offset=19896 */ "exc_pabort\000exception\000Exception taken, Instruction Abort\000event=0x83\000\00000\000Exception taken, Instruction Abort\000"
/* offset=20002 */ "exc_dabort\000exception\000Exception taken, Data Abort and SError\000event=0x84\000\00000\000Exception taken, Data Abort and SError\000"
/* offset=20116 */ "exc_irq\000exception\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000"
/* offset=20191 */ "exc_fiq\000exception\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000"
/* offset=20266 */ "exc_hvc\000exception\000Exception taken, Hypervisor Call\000event=0x8a\000\00000\000Exception taken, Hypervisor Call\000"
/* offset=20365 */ "exc_trap_pabort\000exception\000Exception taken, Instruction Abort not taken locally\000event=0x8b\000\00000\000Exception taken, Instruction Abort not taken locally\000"
/* offset=20512 */ "exc_trap_dabort\000exception\000Exception taken, Data Abort or SError not taken locally\000event=0x8c\000\00000\000Exception taken, Data Abort or SError not taken locally\000"
/* offset=20665 */ "exc_trap_other\000exception\000Exception taken, Other traps not taken locally\000event=0x8d\000\00000\000Exception taken, Other traps not taken locally\000"
/* offset=20799 */ "exc_trap_irq\000exception\000Exception taken, IRQ not taken locally\000event=0x8e\000\00000\000Exception taken, IRQ not taken locally\000"
/* offset=20915 */ "exc_trap_fiq\000exception\000Exception taken, FIQ not taken locally\000event=0x8f\000\00000\000Exception taken, FIQ not taken locally\000"
/* offset=21031 */ "exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Exception taken\000"
/* offset=21097 */ "exc_return\000exception\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Instruction architecturally executed, condition check pass, exception return\000"
/* offset=21286 */ "sw_incr\000instruction\000Instruction architecturally executed, Condition code check pass, software increment\000event=0\000\00000\000Instruction architecturally executed, Condition code check pass, software increment\000"
/* offset=21486 */ "st_retired\000instruction\000Instruction architecturally executed, condition code check pass, store\000event=0x7\000\00000\000Instruction architecturally executed, condition code check pass, store\000"
/* offset=21665 */ "op_spec\000instruction\000Micro-operation speculatively executed\000event=0x3b\000\00000\000Micro-operation speculatively executed.  The counter counts the number of operations executed by the processing element, including those that are executed speculatively and would not be executed in a simple sequential execution of the program\000"
/* offset=21982 */ "ld_spec\000instruction\000Operation speculatively executed, load\000event=0x70\000\00000\000Operation speculatively executed, load\000"
/* offset=22095 */ "st_spec\000instruction\000Operation speculatively executed, store\000event=0x71\000\00000\000Operation speculatively executed, store\000"
/* offset=22210 */ "ldst_spec\000instruction\000Operation speculatively executed, load or store\000event=0x72\000\00000\000Operation speculatively executed, load or store\000"
/* offset=22343 */ "dp_spec\000instruction\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Operation speculatively executed, integer data processing\000"
/* offset=22494 */ "ase_spec\000instruction\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Operation speculatively executed, Advanced SIMD instruction\000"
/* offset=22650 */ "vfp_spec\000instruction\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Operation speculatively executed, floating-point instruction\000"
/* offset=22808 */ "pc_write_spec\000instruction\000Operation speculatively executed, software change of the PC\000event=0x76\000\00000\000Operation speculatively executed, software change of the PC\000"
/* offset=22969 */ "br_immed_retired\000instruction\000Instruction architecturally executed, immediate branch\000event=0xd\000\00000\000Instruction architecturally executed, immediate branch\000"
/* offset=23122 */ "br_return_retired\000instruction\000Instruction architecturally executed, condition code check pass, procedure return\000event=0xe\000\00000\000Instruction architecturally executed, condition code check pass, procedure return\000"
/* offset=23330 */ "crypto_spec\000instruction\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Operation speculatively executed, Cryptographic instruction\000"
/* offset=23489 */ "isb_spec\000instruction\000Barrier speculatively executed, ISB\000event=0x7c\000\00000\000Barrier speculatively executed, ISB\000"
/* offset=23597 */ "dsb_spec\000instruction\000Barrier speculatively executed, DSB\000event=0x7d\000\00000\000Barrier speculatively executed, DSB\000"
/* offset=23705 */ "dmb_spec\000instruction\000Barrier speculatively executed, DMB\000event=0x7e\000\00000\000Barrier speculatively executed, DMB\000"
/* offset=23813 */ "rc_ld_spec\000instruction\000Release consistency operation speculatively executed, Load-Acquire\000event=0x90\000\00000\000Release consistency operation speculatively executed, Load-Acquire\000"
/* offset=23985 */ "rc_st_spec\000instruction\000Release consistency operation speculatively executed, Store-Release\000event=0x91\000\00000\000Release consistency operation speculatively executed, Store-Release\000"
/* offset=24159 */ "inst_retired\000instruction\000Instruction architecturally executed\000event=0x8\000\00000\000Instruction architecturally executed\000"
/* offset=24272 */ "cid_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000"
/* offset=24486 */ "pc_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, software change of the PC\000event=0xc\000\00000\000Instruction architecturally executed, condition code check pass, software change of the PC\000"
/* offset=24711 */ "inst_spec\000instruction\000Operation speculatively executed\000event=0x1b\000\00000\000Operation speculatively executed\000"
/* offset=24814 */ "ttbr_write_retired\000instruction\000Instruction architecturally executed, Condition code check pass, write to TTBR\000event=0x1c\000\00000\000Instruction architecturally executed, Condition code check pass, write to TTBR\000"
/* offset=25018 */ "br_retired\000instruction\000Instruction architecturally executed, branch\000event=0x21\000\00000\000Instruction architecturally executed, branch\000"
/* offset=25146 */ "br_mis_pred_retired\000instruction\000Instruction architecturally executed, mispredicted branch\000event=0x22\000\00000\000Instruction architecturally executed, mispredicted branch\000"
/* offset=25309 */ "op_retired\000instruction\000Micro-operation architecturally executed\000event=0x3a\000\00000\000Micro-operation architecturally executed.  The counter counts each operation counted by OP_SPEC that would be executed in a simple sequential execution of the program\000"
/* offset=25555 */ "nop_spec\000instruction\000Speculatively executed, NOP\000event=0x100\000\00000\000Operation speculatively executed, NOP\000"
/* offset=25658 */ "ldrex_spec\000intrinsic\000Exclusive operation speculatively executed, LDREX or LDX\000event=0x6c\000\00000\000Exclusive operation speculatively executed, LDREX or LDX\000"
/* offset=25808 */ "strex_pass_spec\000intrinsic\000Exclusive operation speculatively executed, STREX or STX pass\000event=0x6d\000\00000\000Exclusive operation speculatively executed, STREX or STX pass\000"
/* offset=25973 */ "strex_fail_spec\000intrinsic\000Exclusive operation speculatively executed, STREX or STX fail\000event=0x6e\000\00000\000Exclusive operation speculatively executed, STREX or STX fail\000"
/* offset=26138 */ "strex_spec\000intrinsic\000Exclusive operation speculatively executed, STREX or STX\000event=0x6f\000\00000\000Exclusive operation speculatively executed, STREX or STX\000"
/* offset=26288 */ "ld_retired\000memory\000Instruction architecturally executed, condition code check pass, load\000event=0x6\000\00000\000Instruction architecturally executed, condition code check pass, load\000"
/* offset=26460 */ "mem_access_rd\000memory\000Data memory access, read\000event=0x66\000\00000\000Data memory access, read\000"
/* offset=26546 */ "mem_access_wr\000memory\000Data memory access, write\000event=0x67\000\00000\000Data memory access, write\000"
/* offset=26634 */ "unaligned_ld_spec\000memory\000Unaligned access, read\000event=0x68\000\00000\000Unaligned access, read\000"
/* offset=26720 */ "unaligned_st_spec\000memory\000Unaligned access, write\000event=0x69\000\00000\000Unaligned access, write\000"
/* offset=26808 */ "unaligned_ldst_spec\000memory\000Unaligned access\000event=0x6a\000\00000\000Unaligned access\000"
/* offset=26884 */ "ld_align_lat\000memory\000Load with additional latency from alignment\000event=0x4021\000\00000\000Load with additional latency from alignment\000"
/* offset=27009 */ "st_align_lat\000memory\000Store with additional latency from alignment\000event=0x4022\000\00000\000Store with additional latency from alignment\000"
/* offset=27136 */ "mem_access\000memory\000Data memory access\000event=0x13\000\00000\000Data memory access\000"
/* offset=27207 */ "memory_error\000memory\000Local memory error\000event=0x1a\000\00000\000Local memory error\000"
/* offset=27280 */ "ldst_align_lat\000memory\000Access with additional latency from alignment\000event=0x4020\000\00000\000Access with additional latency from alignment\000"
/* offset=27411 */ "mem_access_checked\000memory\000Checked data memory access\000event=0x4024\000\00000\000Checked data memory access\000"
/* offset=27508 */ "mem_access_checked_rd\000memory\000Checked data memory access, read\000event=0x4025\000\00000\000Checked data memory access, read\000"
/* offset=27620 */ "mem_access_checked_wr\000memory\000Checked data memory access, write\000event=0x4026\000\00000\000Checked data memory access, write\000"
/* offset=27734 */ "stall_frontend\000pipeline\000Impacted by errata, use metrics instead -  Spec update: Errata AC03_CPU_29\000event=0x23\000\00000\000No operation issued because of the frontend\000"
/* offset=27892 */ "stall_backend\000pipeline\000No operation issued due to the backend\000event=0x24\000\00000\000No operation issued due to the backend\000"
/* offset=28008 */ "stall\000pipeline\000Impacted by errata, use metrics instead -  Spec update: Errata AC03_CPU_29\000event=0x3c\000\00000\000No operation sent for execution.  The counter counts every attributable cycle on which no attributable instruction or operation was sent for execution on this processing element\000"
/* offset=28291 */ "stall_slot_backend\000pipeline\000No operation sent for execution on a slot due to the backend\000event=0x3d\000\00000\000No operation sent for execution on a slot due to the backend.  Counts each slot counted by STALL_SLOT where no attributable instruction or operation was sent for execution because the backend is unable to accept it\000"
/* offset=28610 */ "stall_slot_frontend\000pipeline\000Impacted by errata, use metrics instead -  Spec update: Errata AC03_CPU_29\000event=0x3e\000\00000\000No operation sent for execution on a slot due to the frontend.  Counts each slot counted by STALL_SLOT where no attributable instruction or operation was sent for execution because there was no attributable instruction or operation available to issue from the processing element from the frontend for the slot\000"
/* offset=29039 */ "stall_slot\000pipeline\000No operation sent for execution on a slot\000event=0x3f\000\00000\000No operation sent for execution on a slot.  The counter counts on each attributable cycle the number of instruction or operation slots that were not occupied by an instruction or operation attributable to the processing element\000"
/* offset=29344 */ "stall_backend_mem\000pipeline\000Memory stall cycles\000event=0x4005\000\00000\000Memory stall cycles.  The counter counts each cycle counted by STALL_BACKEND where there is a cache miss in the last level of cache within the processing element clock domain\000"
/* offset=29583 */ "sample_pop\000spe\000Sample Population\000event=0x4000\000\00000\000Sample Population\000"
/* offset=29651 */ "sample_feed\000spe\000Sample Taken\000event=0x4001\000\00000\000Sample Taken\000"
/* offset=29710 */ "sample_filtrate\000spe\000Sample Taken and not removed by filtering\000event=0x4002\000\00000\000Sample Taken and not removed by filtering\000"
/* offset=29831 */ "sample_collision\000spe\000Sample collided with previous sample\000event=0x4003\000\00000\000Sample collided with previous sample\000"
/* offset=29943 */ "br_mis_pred\000branch\000Branch mispredicted\000event=0x10\000\00000\000Mispredicted or not predicted branch speculatively executed\000"
/* offset=30057 */ "br_pred\000branch\000Predictable branch\000event=0x12\000\00000\000Predictable branch speculatively executed\000"
/* offset=30148 */ "l1d_tlb\000cache\000L1D TLB access\000event=0x25\000\00000\000This event counts any load or store operation which accesses the data L1 TLB\000"
/* offset=30269 */ "l1i_tlb\000cache\000Attributable Level 1 instruction TLB access\000event=0x26\000\00000\000This event counts any instruction fetch which accesses the instruction L1 TLB\000"
/* offset=30420 */ "l2d_tlb_access\000cache\000L2D TLB access\000event=0x34\000\00000\000Level 2 access to data TLB that caused a page table walk. This event counts on any data access which causes L2D_TLB_REFILL to count\000"
/* offset=30603 */ "l2i_tlb_access\000cache\000L2I TLB access\000event=0x35\000\00000\000Level 2 access to instruciton TLB that caused a page table walk. This event counts on any instruciton access which causes L2I_TLB_REFILL to count\000"
/* offset=30800 */ "btb_mis_pred\000cache\000BTB misprediction\000event=0x102\000\00000\000Branch target buffer misprediction\000"
/* offset=30888 */ "itb_miss\000cache\000ITB miss\000event=0x103\000\00000\000ITB miss\000"
/* offset=30937 */ "dtb_miss\000cache\000DTB miss\000event=0x104\000\00000\000DTB miss\000"
/* offset=30986 */ "l1d_cache_late_miss\000cache\000L1D cache late miss\000event=0x105\000\00000\000Level 1 data cache late miss\000"
/* offset=31077 */ "l1d_cache_prefetch\000cache\000L1D cache prefetch\000event=0x106\000\00000\000Level 1 data cache prefetch request\000"
/* offset=31173 */ "l2d_cache_prefetch\000cache\000L2D cache prefetch\000event=0x107\000\00000\000Level 2 data cache prefetch request\000"
/* offset=31269 */ "l1_stage2_tlb_refill\000cache\000L1 stage 2 TLB refill\000event=0x111\000\00000\000Level 1 stage 2 TLB refill\000"
/* offset=31361 */ "page_walk_l0_stage1_hit\000cache\000Page walk, L0 stage-1 hit\000event=0x112\000\00000\000Page walk cache level-0 stage-1 hit\000"
/* offset=31469 */ "page_walk_l1_stage1_hit\000cache\000Page walk, L1 stage-1 hit\000event=0x113\000\00000\000Page walk cache level-1 stage-1 hit\000"
/* offset=31577 */ "page_walk_l2_stage1_hit\000cache\000Page walk, L2 stage-1 hit\000event=0x114\000\00000\000Page walk cache level-2 stage-1 hit\000"
/* offset=31685 */ "page_walk_l1_stage2_hit\000cache\000Page walk, L1 stage-2 hit\000event=0x115\000\00000\000Page walk cache level-1 stage-2 hit\000"
/* offset=31793 */ "page_walk_l2_stage2_hit\000cache\000Page walk, L2 stage-2 hit\000event=0x116\000\00000\000Page walk cache level-2 stage-2 hit\000"
/* offset=31901 */ "cpu_cycles\000clock\000Cycle\000event=0x11\000\00000\000The number of core clock cycles\000"
/* offset=31971 */ "fsu_clock_off_cycles\000clock\000FSU clocking gated off cycle\000event=0x101\000\00000\000FSU clocking gated off cycle\000"
/* offset=32072 */ "wait_cycles\000clock\000Wait state cycle\000event=0x110\000\00000\000Wait state cycle\000"
/* offset=32140 */ "sw_incr\000instruction\000Software increment\000event=0\000\00000\000Instruction architecturally executed, software increment\000"
/* offset=32248 */ "cid_write_retired\000instruction\000Write to CONTEXTIDR\000event=0xb\000\00000\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000"
/* offset=32397 */ "br_retired\000instruction\000Instruction architecturally executed, branch\000event=0x21\000\00000\000This event counts all branches, taken or not. This excludes exception entries, debug entries and CCFAIL branches\000"
/* offset=32593 */ "br_mis_pred_retired\000instruction\000Instruction architecturally executed, mispredicted branch\000event=0x22\000\00000\000This event counts any branch counted by BR_RETIRED which is not correctly predicted and causes a pipeline flush\000"
/* offset=32810 */ "memory_error\000memory\000Local memory error\000event=0x1a\000\00000\000This event counts any correctable or uncorrectable memory error (ECC or parity) in the protected core RAMs\000"
/* offset=32971 */ "decode_stall\000pipeline\000Decode starved\000event=0x108\000\00000\000Decode starved for instruction cycle\000"
/* offset=33061 */ "dispatch_stall\000pipeline\000Dispatch stalled\000event=0x109\000\00000\000Op dispatch stalled cycle\000"
/* offset=33144 */ "ixa_stall\000pipeline\000IXA stalled\000event=0x10a\000\00000\000IXA Op non-issue\000"
/* offset=33208 */ "ixb_stall\000pipeline\000IXB stalled\000event=0x10b\000\00000\000IXB Op non-issue\000"
/* offset=33272 */ "bx_stall\000pipeline\000BX stalled\000event=0x10c\000\00000\000BX Op non-issue\000"
/* offset=33333 */ "lx_stall\000pipeline\000LX stalled\000event=0x10d\000\00000\000LX Op non-issue\000"
/* offset=33394 */ "sx_stall\000pipeline\000SX stalled\000event=0x10e\000\00000\000SX Op non-issue\000"
/* offset=33455 */ "fx_stall\000pipeline\000FX stalled\000event=0x10f\000\00000\000FX Op non-issue\000"
/* offset=33516 */ "l1d_cache_wb\000cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000Attributable Level 1 data cache write-back\000"
/* offset=33636 */ "memory_error\000exception\000Local memory error\000event=0x1a\000\00000\000Local memory error\000"
/* offset=33712 */ "ld_retired\000instruction\000Instruction architecturally executed, condition code check pass, load\000event=0x6\000\00000\000Instruction architecturally executed, condition code check pass, load\000"
/* offset=33889 */ "exc_return\000instruction\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Instruction architecturally executed, condition check pass, exception return\000"
/* offset=34080 */ "unaligned_ldst_retired\000memory\000Instruction architecturally executed, condition code check pass, unaligned\000event=0xf\000\00000\000Instruction architecturally executed, condition code check pass, unaligned\000"
/* offset=34274 */ "br_cond_pred\000branch\000Predicted conditional branch executed. This event counts when any branch that the conditional predictor can predict is retired. This event still counts when branch prediction is disabled due to the Memory Management Unit (MMU) being off\000event=0xc9\000\00000\000Predicted conditional branch executed. This event counts when any branch that the conditional predictor can predict is retired. This event still counts when branch prediction is disabled due to the Memory Management Unit (MMU) being off\000"
/* offset=34783 */ "br_indirect_mis_pred\000branch\000Indirect branch mispredicted. This event counts when any indirect branch that the Branch Target Address Cache (BTAC) can predict is retired and has mispredicted either the condition or the address. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xca\000\00000\000Indirect branch mispredicted. This event counts when any indirect branch that the Branch Target Address Cache (BTAC) can predict is retired and has mispredicted either the condition or the address. This event still counts when branch prediction is disabled due to the MMU being off\000"
/* offset=35390 */ "br_indirect_addr_mis_pred\000branch\000Indirect branch mispredicted due to address miscompare. This event counts when any indirect branch that the BTAC can predict is retired, was taken, correctly predicted the condition, and has mispredicted the address. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xcb\000\00000\000Indirect branch mispredicted due to address miscompare. This event counts when any indirect branch that the BTAC can predict is retired, was taken, correctly predicted the condition, and has mispredicted the address. This event still counts when branch prediction is disabled due to the MMU being off\000"
/* offset=36040 */ "br_cond_mis_pred\000branch\000Conditional branch mispredicted. This event counts when any branch that the conditional predictor can predict is retired and has mispredicted the condition. This event still counts when branch prediction is disabled due to the MMU being off. Conditional indirect branches that correctly predict the condition but mispredict the address do not count\000event=0xcc\000\00000\000Conditional branch mispredicted. This event counts when any branch that the conditional predictor can predict is retired and has mispredicted the condition. This event still counts when branch prediction is disabled due to the MMU being off. Conditional indirect branches that correctly predict the condition but mispredict the address do not count\000"
/* offset=36777 */ "br_indirect_addr_pred\000branch\000Indirect branch with predicted address executed. This event counts when any indirect branch that the BTAC can predict is retired, was taken, and correctly predicted the condition. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xcd\000\00000\000Indirect branch with predicted address executed. This event counts when any indirect branch that the BTAC can predict is retired, was taken, and correctly predicted the condition. This event still counts when branch prediction is disabled due to the MMU being off\000"
/* offset=37349 */ "br_return_addr_pred\000branch\000Procedure return with predicted address executed. This event counts when any procedure return that the call-return stack can predict is retired, was taken, and correctly predicted the condition. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xce\000\00000\000Procedure return with predicted address executed. This event counts when any procedure return that the call-return stack can predict is retired, was taken, and correctly predicted the condition. This event still counts when branch prediction is disabled due to the MMU being off\000"
/* offset=37949 */ "br_return_addr_mis_pred\000branch\000Procedure return mispredicted due to address miscompare. This event counts when any procedure return that the call-return stack can predict is retired, was taken, correctly predicted the condition, and has mispredicted the address. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xcf\000\00000\000Procedure return mispredicted due to address miscompare. This event counts when any procedure return that the call-return stack can predict is retired, was taken, correctly predicted the condition, and has mispredicted the address. This event still counts when branch prediction is disabled due to the MMU being off\000"
/* offset=38627 */ "l2d_cache_allocate\000cache\000Attributable Level 2 data cache allocation without refill\000event=0x20\000\00000\000Attributable Level 2 data cache allocation without refill\000"
/* offset=38783 */ "l3d_cache\000cache\000Attributable Level 3 data cache access\000event=0x2b\000\00000\000Attributable Level 3 data cache access\000"
/* offset=38892 */ "ll_cache_rd\000cache\000Attributable Last level cache memory read\000event=0x36\000\00000\000Attributable Last level cache memory read\000"
/* offset=39009 */ "ll_cache_miss_rd\000cache\000Last level cache miss, read\000event=0x37\000\00000\000Last level cache miss, read\000"
/* offset=39103 */ "l1d_cache_refill_wr\000cache\000L1D cache refill, write\000event=0x43\000\00000\000Attributable Level 1 data cache refill, write\000"
/* offset=39214 */ "l1d_cache_refill_inner\000cache\000L1D cache refill, inner\000event=0x44\000\00000\000Attributable Level 1 data cache refill, inner\000"
/* offset=39328 */ "l1d_cache_refill_outer\000cache\000L1D cache refill, outer\000event=0x45\000\00000\000Attributable Level 1 data cache refill, outer\000"
/* offset=39442 */ "l3d_cache_rd\000cache\000Attributable Level 3 data or unified cache access, read\000event=0xa0\000\00000\000Attributable Level 3 data or unified cache access, read\000"
/* offset=39588 */ "l3d_cache_refill_rd\000cache\000Attributable Level 3 data or unified cache refill, read\000event=0xa2\000\00000\000Attributable Level 3 data or unified cache refill, read\000"
/* offset=39741 */ "l2d_cache_refill_prefetch\000cache\000L2 cache refill due to prefetch. If the complex is configured with a per-complex L2 cache, this event does not count. If the complex is configured without a per-complex L2 cache, this event counts the cluster cache event, as defined by L3D_CACHE_REFILL_PREFETCH. If neither a per-complex cache or a cluster cache is configured, this event is not implemented\000event=0xc1\000\00000\000L2 cache refill due to prefetch. If the complex is configured with a per-complex L2 cache, this event does not count. If the complex is configured without a per-complex L2 cache, this event counts the cluster cache event, as defined by L3D_CACHE_REFILL_PREFETCH. If neither a per-complex cache or a cluster cache is configured, this event is not implemented\000"
/* offset=40504 */ "l1d_cache_refill_prefetch\000cache\000L1 data cache refill due to prefetch. This event counts any linefills from the prefetcher that cause an allocation into the L1 data cache\000event=0xc2\000\00000\000L1 data cache refill due to prefetch. This event counts any linefills from the prefetcher that cause an allocation into the L1 data cache\000"
/* offset=40827 */ "l2d_ws_mode\000cache\000L2 cache write streaming mode. This event counts for each cycle where the core is in write streaming mode and is not allocating writes into the L2 cache\000event=0xc3\000\00000\000L2 cache write streaming mode. This event counts for each cycle where the core is in write streaming mode and is not allocating writes into the L2 cache\000"
/* offset=41166 */ "l1d_ws_mode_entry\000cache\000L1 data cache entering write streaming mode. This event counts for each entry into write streaming mode\000event=0xc4\000\00000\000L1 data cache entering write streaming mode. This event counts for each entry into write streaming mode\000"
/* offset=41413 */ "l1d_ws_mode\000cache\000L1 data cache write streaming mode. This event counts for each cycle where the core is in write streaming mode and is not allocating writes into the L1 data cache\000event=0xc5\000\00000\000L1 data cache write streaming mode. This event counts for each cycle where the core is in write streaming mode and is not allocating writes into the L1 data cache\000"
/* offset=41772 */ "l3d_ws_mode\000cache\000L3 cache write streaming mode. This event counts for each cycle where the core is in write streaming mode and is not allocating writes into the L3 cache\000event=0xc7\000\00000\000L3 cache write streaming mode. This event counts for each cycle where the core is in write streaming mode and is not allocating writes into the L3 cache\000"
/* offset=42111 */ "ll_ws_mode\000cache\000Last level cache write streaming mode. This event counts for each cycle where the core is in write streaming mode and is not allocating writes into the system cache\000event=0xc8\000\00000\000Last level cache write streaming mode. This event counts for each cycle where the core is in write streaming mode and is not allocating writes into the system cache\000"
/* offset=42473 */ "l2d_walk_tlb\000cache\000L2 TLB walk cache access. This event does not count if the MMU is disabled\000event=0xd0\000\00000\000L2 TLB walk cache access. This event does not count if the MMU is disabled\000"
/* offset=42657 */ "l2d_walk_tlb_refill\000cache\000L2 TLB walk cache refill. This event does not count if the MMU is disabled\000event=0xd1\000\00000\000L2 TLB walk cache refill. This event does not count if the MMU is disabled\000"
/* offset=42848 */ "l2d_s2_tlb\000cache\000L2 TLB IPA cache access. This event counts on each access to the IPA cache. If a single translation table walk needs to make multiple accesses to the IPA cache, each access is counted. If stage 2 translation is disabled, this event does not count\000event=0xd4\000\00000\000L2 TLB IPA cache access. This event counts on each access to the IPA cache. If a single translation table walk needs to make multiple accesses to the IPA cache, each access is counted. If stage 2 translation is disabled, this event does not count\000"
/* offset=43374 */ "l2d_s2_tlb_refill\000cache\000L2 TLB IPA cache refill. This event counts on each refill of the IPA cache. If a single translation table walk needs to make multiple accesses to the IPA cache, each access that causes a refill is counted. If stage 2 translation is disabled, this event does not count\000event=0xd5\000\00000\000L2 TLB IPA cache refill. This event counts on each refill of the IPA cache. If a single translation table walk needs to make multiple accesses to the IPA cache, each access that causes a refill is counted. If stage 2 translation is disabled, this event does not count\000"
/* offset=43949 */ "l2d_cache_stash_dropped\000cache\000L2 cache stash dropped. This event counts on each stash request that is received from the interconnect or the Accelerator Coherency Port (ACP), that targets L2 cache and is dropped due to lack of buffer space to hold the request\000event=0xd6\000\00000\000L2 cache stash dropped. This event counts on each stash request that is received from the interconnect or the Accelerator Coherency Port (ACP), that targets L2 cache and is dropped due to lack of buffer space to hold the request\000"
/* offset=44452 */ "l3d_cache_lmiss_rd\000cache\000Level 3 data cache long-latency read miss\000event=0x400b\000\00000\000Level 3 data cache long-latency read miss.  The counter counts each memory read access counted by L3D_CACHE that incurs additional latency because it returns data from outside the Level 3 data or unified cache of this processing element.  The event indicates to software that the access missed in the Level 3 data or unified cache and might have a significant performance impact compared to the latency of an access that hits in the Level 3 data or unified cache\000"
/* offset=44999 */ "sve_inst_retired\000instruction\000Instruction architecturally executed, SVE\000event=0x8002\000\00000\000Instruction architecturally executed, SVE\000"
/* offset=45129 */ "sve_inst_spec\000instruction\000SVE operations speculatively executed\000event=0x8006\000\00000\000SVE operations speculatively executed\000"
/* offset=45248 */ "fp_hp_spec\000instruction\000Floating-point half-precision operations speculatively executed\000event=0x8014\000\00000\000Floating-point half-precision operations speculatively executed\000"
/* offset=45416 */ "fp_sp_spec\000instruction\000Floating-point single-precision operations speculatively executed\000event=0x8018\000\00000\000Floating-point single-precision operations speculatively executed\000"
/* offset=45588 */ "fp_dp_spec\000instruction\000Floating-point double-precision operations speculatively executed\000event=0x801c\000\00000\000Floating-point double-precision operations speculatively executed\000"
/* offset=45760 */ "ase_sve_int8_spec\000instruction\000Advanced SIMD and SVE 8-bit integer operations speculatively executed\000event=0x80e3\000\00000\000Advanced SIMD and SVE 8-bit integer operations speculatively executed\000"
/* offset=45947 */ "ase_sve_int16_spec\000instruction\000Advanced SIMD and SVE 16-bit integer operations speculatively executed\000event=0x80e7\000\00000\000Advanced SIMD and SVE 16-bit integer operations speculatively executed\000"
/* offset=46137 */ "ase_sve_int32_spec\000instruction\000Advanced SIMD and SVE 32-bit integer operations speculatively executed\000event=0x80eb\000\00000\000Advanced SIMD and SVE 32-bit integer operations speculatively executed\000"
/* offset=46327 */ "ase_sve_int64_spec\000instruction\000Advanced SIMD and SVE 64-bit integer operations speculatively executed\000event=0x80ef\000\00000\000Advanced SIMD and SVE 64-bit integer operations speculatively executed\000"
/* offset=46517 */ "remote_access_rd\000memory\000Attributable memory read access to another socket in a multi-socket system\000event=0x38\000\00000\000Attributable memory read access to another socket in a multi-socket system\000"
/* offset=46706 */ "stall_frontend\000pipeline\000No operation issued because of the frontend\000event=0x23\000\00000\000No operation issued because of the frontend\000"
/* offset=46833 */ "stall\000pipeline\000No operation sent for execution\000event=0x3c\000\00000\000No operation sent for execution.  The counter counts every attributable cycle on which no attributable instruction or operation was sent for execution on this processing element\000"
/* offset=47073 */ "stall_slot_frontend\000pipeline\000No operation sent for execution on a slot due to the frontend\000event=0x3e\000\00000\000No operation sent for execution on a slot due to the frontend.  Counts each slot counted by STALL_SLOT where no attributable instruction or operation was sent for execution because there was no attributable instruction or operation available to issue from the processing element from the frontend for the slot\000"
/* offset=47489 */ "stall_frontend_cache\000pipeline\000No operation issued due to the frontend, cache miss. This event counts every cycle that the Data Processing Unit (DPU) instruction queue is empty and there is an instruction cache miss being processed\000event=0xe1\000\00000\000No operation issued due to the frontend, cache miss. This event counts every cycle that the Data Processing Unit (DPU) instruction queue is empty and there is an instruction cache miss being processed\000"
/* offset=47936 */ "stall_frontend_tlb\000pipeline\000No operation issued due to the frontend, TLB miss. This event counts every cycle that the DPU instruction queue is empty and there is an instruction L1 TLB miss being processed\000event=0xe2\000\00000\000No operation issued due to the frontend, TLB miss. This event counts every cycle that the DPU instruction queue is empty and there is an instruction L1 TLB miss being processed\000"
/* offset=48333 */ "stall_frontend_pderr\000pipeline\000No operation issued due to the frontend, pre-decode error\000event=0xe3\000\00000\000No operation issued due to the frontend, pre-decode error\000"
/* offset=48494 */ "stall_backend_ilock\000pipeline\000No operation issued due to the backend interlock. This event counts every cycle where the issue of an operation is stalled and there is an interlock. Stall cycles due to a stall in the Wr stage are excluded\000event=0xe4\000\00000\000No operation issued due to the backend interlock. This event counts every cycle where the issue of an operation is stalled and there is an interlock. Stall cycles due to a stall in the Wr stage are excluded\000"
/* offset=48952 */ "stall_backend_ilock_addr\000pipeline\000No operation issued due to the backend, address interlock. This event counts every cycle where the issue of an operation is stalled and there is an interlock on an address operand. This type of interlock is caused by a load/store instruction waiting for data to calculate the address. Stall cycles due to a stall in the Wr stage are excluded\000event=0xe5\000\00000\000No operation issued due to the backend, address interlock. This event counts every cycle where the issue of an operation is stalled and there is an interlock on an address operand. This type of interlock is caused by a load/store instruction waiting for data to calculate the address. Stall cycles due to a stall in the Wr stage are excluded\000"
/* offset=49685 */ "stall_backend_ilock_vpu\000pipeline\000No operation issued due to the backend, interlock, or the Vector Processing Unit (VPU). This event counts every cycle where there is a stall or an interlock that is caused by a VPU instruction. Stall cycles due to a stall in the Wr stage are excluded\000event=0xe6\000\00000\000No operation issued due to the backend, interlock, or the Vector Processing Unit (VPU). This event counts every cycle where there is a stall or an interlock that is caused by a VPU instruction. Stall cycles due to a stall in the Wr stage are excluded\000"
/* offset=50235 */ "stall_backend_ld\000pipeline\000No operation issued due to the backend, load. This event counts every cycle where there is a stall in the Wr stage due to a load\000event=0xe7\000\00000\000No operation issued due to the backend, load. This event counts every cycle where there is a stall in the Wr stage due to a load\000"
/* offset=50534 */ "stall_backend_st\000pipeline\000No operation issued due to the backend, store. This event counts every cycle where there is a stall in the Wr stage due to a store\000event=0xe8\000\00000\000No operation issued due to the backend, store. This event counts every cycle where there is a stall in the Wr stage due to a store\000"
/* offset=50837 */ "stall_backend_ld_cache\000pipeline\000No operation issued due to the backend, load, cache miss. This event counts every cycle where there is a stall in the Wr stage due to a load that is waiting on data. The event counts for stalls that are caused by missing the cache or where the data is Non-cacheable\000event=0xe9\000\00000\000No operation issued due to the backend, load, cache miss. This event counts every cycle where there is a stall in the Wr stage due to a load that is waiting on data. The event counts for stalls that are caused by missing the cache or where the data is Non-cacheable\000"
/* offset=51416 */ "stall_backend_ld_tlb\000pipeline\000No operation issued due to the backend, load, TLB miss. This event counts every cycle where there is a stall in the Wr stage due to a load that misses in the L1 TLB\000event=0xea\000\00000\000No operation issued due to the backend, load, TLB miss. This event counts every cycle where there is a stall in the Wr stage due to a load that misses in the L1 TLB\000"
/* offset=51791 */ "stall_backend_st_stb\000pipeline\000No operation issued due to the backend, store, Store Buffer (STB) full. This event counts every cycle where there is a stall in the Wr stage because of a store operation that is waiting due to the STB being full\000event=0xeb\000\00000\000No operation issued due to the backend, store, Store Buffer (STB) full. This event counts every cycle where there is a stall in the Wr stage because of a store operation that is waiting due to the STB being full\000"
/* offset=52260 */ "stall_backend_st_tlb\000pipeline\000No operation issued due to the backend, store, TLB miss. This event counts every cycle where there is a stall in the Wr stage because of a store operation that has missed in the L1 TLB\000event=0xec\000\00000\000No operation issued due to the backend, store, TLB miss. This event counts every cycle where there is a stall in the Wr stage because of a store operation that has missed in the L1 TLB\000"
/* offset=52675 */ "stall_backend_vpu_hazard\000pipeline\000No operation issued due to the backend, VPU hazard. This event counts every cycle where the core stalls due to contention for the VPU with the other core\000event=0xed\000\00000\000No operation issued due to the backend, VPU hazard. This event counts every cycle where the core stalls due to contention for the VPU with the other core\000"
/* offset=53032 */ "stall_slot_backend_ilock\000pipeline\000Issue slot not issued due to interlock. For each cycle, this event counts each dispatch slot that does not issue due to an interlock\000event=0xee\000\00000\000Issue slot not issued due to interlock. For each cycle, this event counts each dispatch slot that does not issue due to an interlock\000"
/* offset=53347 */ "pmu_ovfs\000pmu\000PMU overflow, counters accessible to EL1 and EL0\000event=0x400d\000\00000\000PMU overflow, counters accessible to EL1 and EL0\000"
/* offset=53475 */ "pmu_hovfs\000pmu\000PMU overflow, counters reserved for use by EL2\000event=0x400f\000\00000\000PMU overflow, counters reserved for use by EL2\000"
/* offset=53600 */ "trb_wrap\000trace\000Trace buffer current write pointer wrapped\000event=0x400c\000\00000\000Trace buffer current write pointer wrapped\000"
/* offset=53718 */ "trb_trig\000trace\000Trace buffer Trigger Event\000event=0x400e\000\00000\000Trace buffer Trigger Event\000"
/* offset=53804 */ "trcextout0\000trace\000PE Trace Unit external output 0\000event=0x4010\000\00000\000PE Trace Unit external output 0\000"
/* offset=53902 */ "trcextout1\000trace\000PE Trace Unit external output 1\000event=0x4011\000\00000\000PE Trace Unit external output 1\000"
/* offset=54000 */ "trcextout2\000trace\000PE Trace Unit external output 2\000event=0x4012\000\00000\000PE Trace Unit external output 2\000"
/* offset=54098 */ "trcextout3\000trace\000PE Trace Unit external output 3\000event=0x4013\000\00000\000PE Trace Unit external output 3\000"
/* offset=54196 */ "cti_trigout4\000trace\000Cross-trigger Interface output trigger 4\000event=0x4018\000\00000\000Cross-trigger Interface output trigger 4\000"
/* offset=54314 */ "cti_trigout5\000trace\000Cross-trigger Interface output trigger 5 \000event=0x4019\000\00000\000Cross-trigger Interface output trigger 5 \000"
/* offset=54434 */ "cti_trigout6\000trace\000Cross-trigger Interface output trigger 6\000event=0x401a\000\00000\000Cross-trigger Interface output trigger 6\000"
/* offset=54552 */ "cti_trigout7\000trace\000Cross-trigger Interface output trigger 7\000event=0x401b\000\00000\000Cross-trigger Interface output trigger 7\000"
/* offset=54670 */ "br_cond\000branch\000Conditional branch executed\000event=0xc9\000\00000\000\000"
/* offset=54729 */ "br_indirect_mispred\000branch\000Indirect branch mispredicted\000event=0xca\000\00000\000\000"
/* offset=54801 */ "br_indirect_mispred_addr\000branch\000Indirect branch mispredicted because of address miscompare\000event=0xcb\000\00000\000\000"
/* offset=54908 */ "br_cond_mispred\000branch\000Conditional branch mispredicted\000event=0xcc\000\00000\000\000"
/* offset=54979 */ "prefetch_linefill\000cache\000Linefill because of prefetch\000event=0xc2\000\00000\000\000"
/* offset=55048 */ "prefetch_linefill_drop\000cache\000Instruction Cache Throttle occurred\000event=0xc3\000\00000\000\000"
/* offset=55129 */ "read_alloc_enter\000cache\000Entering read allocate mode\000event=0xc4\000\00000\000\000"
/* offset=55196 */ "read_alloc\000cache\000Read allocate mode\000event=0xc5\000\00000\000\000"
/* offset=55248 */ "ext_snoop\000cache\000SCU Snooped data from another CPU for this CPU\000event=0xc8\000\00000\000\000"
/* offset=55327 */ "ext_mem_req\000memory\000External memory request\000event=0xc0\000\00000\000\000"
/* offset=55386 */ "ext_mem_req_nc\000memory\000Non-cacheable external memory request\000event=0xc1\000\00000\000\000"
/* offset=55462 */ "exc_irq\000other\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000"
/* offset=55533 */ "exc_fiq\000other\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000"
/* offset=55604 */ "pre_decode_err\000other\000Pre-decode error\000event=0xc6\000\00000\000\000"
/* offset=55658 */ "l1i_cache_err\000other\000L1 Instruction Cache (data or tag) memory error\000event=0xd0\000\00000\000\000"
/* offset=55742 */ "l1d_cache_err\000other\000L1 Data Cache (data, tag or dirty) memory error, correctable or non-correctable\000event=0xd1\000\00000\000\000"
/* offset=55858 */ "tlb_err\000other\000TLB memory error\000event=0xd2\000\00000\000\000"
/* offset=55905 */ "stall_sb_full\000pipeline\000Data Write operation that stalls the pipeline because the store buffer is full\000event=0xc7\000\00000\000\000"
/* offset=56023 */ "other_iq_dep_stall\000pipeline\000Cycles that the DPU IQ is empty and that is not because of a recent micro-TLB miss, instruction cache miss or pre-decode error\000event=0xe0\000\00000\000\000"
/* offset=56194 */ "ic_dep_stall\000pipeline\000Cycles the DPU IQ is empty and there is an instruction cache miss being processed\000event=0xe1\000\00000\000\000"
/* offset=56314 */ "iutlb_dep_stall\000pipeline\000Cycles the DPU IQ is empty and there is an instruction micro-TLB miss being processed\000event=0xe2\000\00000\000\000"
/* offset=56441 */ "decode_dep_stall\000pipeline\000Cycles the DPU IQ is empty and there is a pre-decode error being processed\000event=0xe3\000\00000\000\000"
/* offset=56558 */ "other_interlock_stall\000pipeline\000Cycles there is an interlock other than  Advanced SIMD/Floating-point instructions or load/store instruction\000event=0xe4\000\00000\000\000"
/* offset=56714 */ "agu_dep_stall\000pipeline\000Cycles there is an interlock for a load/store instruction waiting for data to calculate the address in the AGU\000event=0xe5\000\00000\000\000"
/* offset=56864 */ "simd_dep_stall\000pipeline\000Cycles there is an interlock for an Advanced SIMD/Floating-point operation\000event=0xe6\000\00000\000\000"
/* offset=56979 */ "ld_dep_stall\000pipeline\000Cycles there is a stall in the Wr stage because of a load miss\000event=0xe7\000\00000\000\000"
/* offset=57080 */ "st_dep_stall\000pipeline\000Cycles there is a stall in the Wr stage because of a store\000event=0xe8\000\00000\000\000"
/* offset=57177 */ "br_cond_pred\000branch\000Predicted conditional branch executed.This event counts when any branch which can be predicted by the conditional predictor is retired. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xc9\000\00000\000Predicted conditional branch executed.This event counts when any branch which can be predicted by the conditional predictor is retired. This event still counts when branch prediction is disabled due to the MMU being off\000"
/* offset=57652 */ "br_indirect_mis_pred\000branch\000Indirect branch mis-predicted.This event counts when any indirect branch which can be predicted by the BTAC is retired, and has mispredicted for either the condition or the address. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xca\000\00000\000Indirect branch mis-predicted.This event counts when any indirect branch which can be predicted by the BTAC is retired, and has mispredicted for either the condition or the address. This event still counts when branch prediction is disabled due to the MMU being off\000"
/* offset=58227 */ "br_indirect_addr_mis_pred\000branch\000Indirect branch mis-predicted due to address mis-compare.This event counts when any indirect branch which can be predicted by the BTAC is retired, was taken and correctly predicted the condition, and has mispredicted the address. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xcb\000\00000\000Indirect branch mis-predicted due to address mis-compare.This event counts when any indirect branch which can be predicted by the BTAC is retired, was taken and correctly predicted the condition, and has mispredicted the address. This event still counts when branch prediction is disabled due to the MMU being off\000"
/* offset=58903 */ "br_cond_mis_pred\000branch\000Conditional branch mis-predicted.This event counts when any branch which can be predicted by the conditional predictor is retired, and has mis-predicted the condition. This event still counts when branch prediction is disabled due to the MMU being off. Conditional indirect branches which correctly predicted the condition but mis-predicted on the address do not count this event\000event=0xcc\000\00000\000Conditional branch mis-predicted.This event counts when any branch which can be predicted by the conditional predictor is retired, and has mis-predicted the condition. This event still counts when branch prediction is disabled due to the MMU being off. Conditional indirect branches which correctly predicted the condition but mis-predicted on the address do not count this event\000"
/* offset=59702 */ "br_indirect_addr_pred\000branch\000Indirect branch with predicted address executed.This event counts when any indirect branch which can be predicted by the BTAC is retired, was taken and correctly predicted the condition. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xcd\000\00000\000Indirect branch with predicted address executed.This event counts when any indirect branch which can be predicted by the BTAC is retired, was taken and correctly predicted the condition. This event still counts when branch prediction is disabled due to the MMU being off\000"
/* offset=60288 */ "br_return_addr_pred\000branch\000Procedure return with predicted address executed.This event counts when any procedure return which can be predicted by the CRS is retired, was taken and correctly predicted the condition. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xce\000\00000\000Procedure return with predicted address executed.This event counts when any procedure return which can be predicted by the CRS is retired, was taken and correctly predicted the condition. This event still counts when branch prediction is disabled due to the MMU being off\000"
/* offset=60874 */ "br_return_addr_mis_pred\000branch\000Procedure return mis-predicted due to address mis-compare.This event counts when any procedure return which can be predicted by the CRS is retired, was taken and correctly predicted the condition, and has mispredicted the address. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xcf\000\00000\000Procedure return mis-predicted due to address mis-compare.This event counts when any procedure return which can be predicted by the CRS is retired, was taken and correctly predicted the condition, and has mispredicted the address. This event still counts when branch prediction is disabled due to the MMU being off\000"
/* offset=61550 */ "l3d_cache_allocate\000cache\000Attributable Level 3 data cache allocation without refill\000event=0x29\000\00000\000Attributable Level 3 data cache allocation without refill\000"
/* offset=61706 */ "l3d_cache_refill\000cache\000Attributable Level 3 data cache refill\000event=0x2a\000\00000\000Attributable Level 3 data cache refill\000"
/* offset=61822 */ "l3d_cache_refill_prefetch\000cache\000Level 3 cache refill due to prefetch. This event counts any linefills from the hardware prefetcher which cause an allocation into the L3 cache. Note It might not be possible to both distinguish hardware vs software prefetches and also which prefetches cause an allocation. If so, only hardware prefetches should be counted, regardless of whether they allocate. If either the core is configured without a per-core L2 or the cluster is configured without an L3 cache, this event is not implemented\000event=0xc0\000\00000\000Level 3 cache refill due to prefetch. This event counts any linefills from the hardware prefetcher which cause an allocation into the L3 cache. Note It might not be possible to both distinguish hardware vs software prefetches and also which prefetches cause an allocation. If so, only hardware prefetches should be counted, regardless of whether they allocate. If either the core is configured without a per-core L2 or the cluster is configured without an L3 cache, this event is not implemented\000"
/* offset=62861 */ "l2d_cache_refill_prefetch\000cache\000Level 2 cache refill due to prefetch. +//0 If the core is configured with a per-core L2 cache: This event does not count. +//0 If the core is configured without a per-core L2 cache: This event counts the cluster cache event, as defined by L3D_CACHE_REFILL_PREFETCH. +//0 If there is neither a per-core cache nor a cluster cache configured, this event is not implemented\000event=0xc1\000\00000\000Level 2 cache refill due to prefetch. +//0 If the core is configured with a per-core L2 cache: This event does not count. +//0 If the core is configured without a per-core L2 cache: This event counts the cluster cache event, as defined by L3D_CACHE_REFILL_PREFETCH. +//0 If there is neither a per-core cache nor a cluster cache configured, this event is not implemented\000"
/* offset=63648 */ "l1d_cache_refill_prefetch\000cache\000Level 1 data cache refill due to prefetch. This event counts any linefills from the prefetcher which cause an allocation into the L1 D-cache\000event=0xc2\000\00000\000Level 1 data cache refill due to prefetch. This event counts any linefills from the prefetcher which cause an allocation into the L1 D-cache\000"
/* offset=63977 */ "l2d_ws_mode\000cache\000Level 2 cache write streaming mode. This event counts for each cycle where the core is in write-streaming mode and not allocating writes into the L2 cache\000event=0xc3\000\00000\000Level 2 cache write streaming mode. This event counts for each cycle where the core is in write-streaming mode and not allocating writes into the L2 cache\000"
/* offset=64320 */ "l1d_ws_mode_entry\000cache\000Level 1 data cache entering write streaming mode.This event counts for each entry into write-streaming mode\000event=0xc4\000\00000\000Level 1 data cache entering write streaming mode.This event counts for each entry into write-streaming mode\000"
/* offset=64575 */ "l1d_ws_mode\000cache\000Level 1 data cache write streaming mode.This event counts for each cycle where the core is in write-streaming mode and not allocating writes into the L1 D-cache\000event=0xc5\000\00000\000Level 1 data cache write streaming mode.This event counts for each cycle where the core is in write-streaming mode and not allocating writes into the L1 D-cache\000"
/* offset=64930 */ "l3d_ws_mode\000cache\000Level 3 cache write streaming mode.This event counts for each cycle where the core is in write-streaming mode and not allocating writes into the L3 cache\000event=0xc7\000\00000\000Level 3 cache write streaming mode.This event counts for each cycle where the core is in write-streaming mode and not allocating writes into the L3 cache\000"
/* offset=65271 */ "l2d_llwalk_tlb\000cache\000Level 2 TLB last-level walk cache access.This event does not count if the MMU is disabled\000event=0xd0\000\00000\000Level 2 TLB last-level walk cache access.This event does not count if the MMU is disabled\000"
/* offset=65487 */ "l2d_llwalk_tlb_refill\000cache\000Level 2 TLB last-level walk cache refill.This event does not count if the MMU is disabled\000event=0xd1\000\00000\000Level 2 TLB last-level walk cache refill.This event does not count if the MMU is disabled\000"
/* offset=65710 */ "l2d_l2walk_tlb\000cache\000Level 2 TLB level-2 walk cache access.This event counts accesses to the level-2 walk cache where the last-level walk cache has missed. The event only counts when the translation regime of the pagewalk uses level 2 descriptors. This event does not count if the MMU is disabled\000event=0xd2\000\00000\000Level 2 TLB level-2 walk cache access.This event counts accesses to the level-2 walk cache where the last-level walk cache has missed. The event only counts when the translation regime of the pagewalk uses level 2 descriptors. This event does not count if the MMU is disabled\000"
/* offset=66298 */ "l2d_l2walk_tlb_refill\000cache\000Level 2 TLB level-2 walk cache refill.This event does not count if the MMU is disabled\000event=0xd3\000\00000\000Level 2 TLB level-2 walk cache refill.This event does not count if the MMU is disabled\000"
/* offset=66515 */ "l2d_s2_tlb\000cache\000Level 2 TLB IPA cache access. This event counts on each access to the IPA cache. +//0 If a single pagewalk needs to make multiple accesses to the IPA cache, each access is counted. +//0 If stage 2 translation is disabled, this event does not count\000event=0xd4\000\00000\000Level 2 TLB IPA cache access. This event counts on each access to the IPA cache. +//0 If a single pagewalk needs to make multiple accesses to the IPA cache, each access is counted. +//0 If stage 2 translation is disabled, this event does not count\000"
/* offset=67043 */ "l2d_s2_tlb_refill\000cache\000Level 2 TLB IPA cache refill. This event counts on each refill of the IPA cache. +//0 If a single pagewalk needs to make multiple accesses to the IPA cache, each access which causes a refill is counted. +//0 If stage 2 translation is disabled, this event does not count\000event=0xd5\000\00000\000Level 2 TLB IPA cache refill. This event counts on each refill of the IPA cache. +//0 If a single pagewalk needs to make multiple accesses to the IPA cache, each access which causes a refill is counted. +//0 If stage 2 translation is disabled, this event does not count\000"
/* offset=67622 */ "l2d_cache_stash_dropped\000cache\000Level 2 cache stash dropped.This event counts on each stash request received from the interconnect or ACP, that is targeting L2 and gets dropped due to lack of buffer space to hold the request\000event=0xd6\000\00000\000Level 2 cache stash dropped.This event counts on each stash request received from the interconnect or ACP, that is targeting L2 and gets dropped due to lack of buffer space to hold the request\000"
/* offset=68053 */ "predecode_error\000exception\000Predecode error\000event=0xc6\000\00000\000Predecode error\000"
/* offset=68126 */ "stall_frontend_cache\000pipeline\000No operation issued due to the frontend, cache miss.This event counts every cycle the DPU IQ is empty and there is an instruction cache miss being processed\000event=0xe1\000\00000\000No operation issued due to the frontend, cache miss.This event counts every cycle the DPU IQ is empty and there is an instruction cache miss being processed\000"
/* offset=68485 */ "stall_frontend_tlb\000pipeline\000No operation issued due to the frontend, TLB miss.This event counts every cycle the DPU IQ is empty and there is an instruction L1 TLB miss being processed\000event=0xe2\000\00000\000No operation issued due to the frontend, TLB miss.This event counts every cycle the DPU IQ is empty and there is an instruction L1 TLB miss being processed\000"
/* offset=68840 */ "stall_frontend_pderr\000pipeline\000No operation issued due to the frontend, pre-decode error.This event counts every cycle the DPU IQ is empty and there is a pre-decode error being processed\000event=0xe3\000\00000\000No operation issued due to the frontend, pre-decode error.This event counts every cycle the DPU IQ is empty and there is a pre-decode error being processed\000"
/* offset=69197 */ "stall_backend_ilock\000pipeline\000No operation issued due to the backend interlock.This event counts every cycle that issue is stalled and there is an interlock. Stall cycles due to a stall in Wr (typically awaiting load data) are excluded\000event=0xe4\000\00000\000No operation issued due to the backend interlock.This event counts every cycle that issue is stalled and there is an interlock. Stall cycles due to a stall in Wr (typically awaiting load data) are excluded\000"
/* offset=69653 */ "stall_backend_ilock_agu\000pipeline\000No operation issued due to the backend, interlock, AGU.This event counts every cycle that issue is stalled and there is an interlock that is due to a load/store instruction waiting for data to calculate the address in the AGU. Stall cycles due to a stall in Wr (typically awaiting load data) are excluded\000event=0xe5\000\00000\000No operation issued due to the backend, interlock, AGU.This event counts every cycle that issue is stalled and there is an interlock that is due to a load/store instruction waiting for data to calculate the address in the AGU. Stall cycles due to a stall in Wr (typically awaiting load data) are excluded\000"
/* offset=70311 */ "stall_backend_ilock_fpu\000pipeline\000No operation issued due to the backend, interlock, FPU.This event counts every cycle that issue is stalled and there is an interlock that is due to an FPU/NEON instruction. Stall cycles due to a stall in the Wr stage (typically awaiting load data) are excluded\000event=0xe6\000\00000\000No operation issued due to the backend, interlock, FPU.This event counts every cycle that issue is stalled and there is an interlock that is due to an FPU/NEON instruction. Stall cycles due to a stall in the Wr stage (typically awaiting load data) are excluded\000"
/* offset=70881 */ "stall_backend_ld\000pipeline\000No operation issued due to the backend, load.This event counts every cycle there is a stall in the Wr stage due to a load\000event=0xe7\000\00000\000No operation issued due to the backend, load.This event counts every cycle there is a stall in the Wr stage due to a load\000"
/* offset=71166 */ "stall_backend_st\000pipeline\000No operation issued due to the backend, store.This event counts every cycle there is a stall in the Wr stage due to a store\000event=0xe8\000\00000\000No operation issued due to the backend, store.This event counts every cycle there is a stall in the Wr stage due to a store\000"
/* offset=71455 */ "stall_backend_ld_cache\000pipeline\000No operation issued due to the backend, load, cache miss.This event counts every cycle there is a stall in the Wr stage due to a load which is waiting on data (due to missing the cache or being non-cacheable)\000event=0xe9\000\00000\000No operation issued due to the backend, load, cache miss.This event counts every cycle there is a stall in the Wr stage due to a load which is waiting on data (due to missing the cache or being non-cacheable)\000"
/* offset=71920 */ "stall_backend_ld_tlb\000pipeline\000No operation issued due to the backend, load, TLB miss.This event counts every cycle there is a stall in the Wr stage due to a load which has missed in the L1 TLB\000event=0xea\000\00000\000No operation issued due to the backend, load, TLB miss.This event counts every cycle there is a stall in the Wr stage due to a load which has missed in the L1 TLB\000"
/* offset=72291 */ "stall_backend_st_stb\000pipeline\000No operation issued due to the backend, store, STB full.This event counts every cycle there is a stall in the Wr stage due to a store which is waiting due to the STB being full\000event=0xeb\000\00000\000No operation issued due to the backend, store, STB full.This event counts every cycle there is a stall in the Wr stage due to a store which is waiting due to the STB being full\000"
/* offset=72690 */ "stall_backend_st_tlb\000pipeline\000No operation issued due to the backend, store, TLB miss.This event counts every cycle there is a stall in the Wr stage due to a store which has missed in the L1 TLB\000event=0xec\000\00000\000No operation issued due to the backend, store, TLB miss.This event counts every cycle there is a stall in the Wr stage due to a store which has missed in the L1 TLB\000"
/* offset=73065 */ "l1d_cache_wb_victim\000cache\000L1D cache Write-Back, victim\000event=0x46\000\00000\000Attributable Level 1 data cache Write-Back, victim\000"
/* offset=73186 */ "l1d_cache_wb_clean\000cache\000L1D cache Write-Back, cleaning and coherency\000event=0x47\000\00000\000Level 1 data cache Write-Back, cleaning and coherency\000"
/* offset=73325 */ "exc_smc\000exception\000Exception taken, Secure Monitor Call\000event=0x88\000\00000\000Exception taken, Secure Monitor Call\000"
/* offset=73432 */ "ldrex_spec\000instruction\000Exclusive operation speculatively executed, LDREX or LDX\000event=0x6c\000\00000\000Exclusive operation speculatively executed, LDREX or LDX\000"
/* offset=73584 */ "strex_pass_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX pass\000event=0x6d\000\00000\000Exclusive operation speculatively executed, STREX or STX pass\000"
/* offset=73751 */ "strex_fail_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX fail\000event=0x6e\000\00000\000Exclusive operation speculatively executed, STREX or STX fail\000"
/* offset=73918 */ "stb_stall\000cache\000Merge in the store buffer\000event=0xc0\000\00000\000Merge in the store buffer\000"
/* offset=74001 */ "l1d_pref_line_fill\000cache\000Level 1 data cache refill started due to prefetch. Counts any linefills from the prefetcher which cause an allocation into the L1 D-cache\000event=0xc3\000\00000\000Level 1 data cache refill started due to prefetch. Counts any linefills from the prefetcher which cause an allocation into the L1 D-cache\000"
/* offset=74317 */ "l2d_pref_line_fill\000cache\000Level 2 cache refill due to prefetch. +//0 If the core is configured with a per-core L2 cache: This event does not count. +//0 If the core is configured without a per-core L2 cache: This event counts the cluster cache event, as defined by L3_PREF_LINE_FILL. +//0 If there is neither a per-core cache nor a cluster cache configured, this event is not implemented\000event=0xc4\000\00000\000Level 2 cache refill due to prefetch. +//0 If the core is configured with a per-core L2 cache: This event does not count. +//0 If the core is configured without a per-core L2 cache: This event counts the cluster cache event, as defined by L3_PREF_LINE_FILL. +//0 If there is neither a per-core cache nor a cluster cache configured, this event is not implemented\000"
/* offset=75081 */ "l3_pref_line_fill\000cache\000Level 3 cache refill due to prefetch. This event counts any linefills from the hardware prefetcher which cause an allocation into the L3 cache. Note It might not be possible to distinguish between both hardware and software prefetches and also which prefetches cause an allocation. If so, only hardware prefetches should be counted, regardless of whether they allocate. If either the core is configured without a per-core L2 or the cluster is configured without an L3 cache, this event is not implemented\000event=0xc5\000\00000\000Level 3 cache refill due to prefetch. This event counts any linefills from the hardware prefetcher which cause an allocation into the L3 cache. Note It might not be possible to distinguish between both hardware and software prefetches and also which prefetches cause an allocation. If so, only hardware prefetches should be counted, regardless of whether they allocate. If either the core is configured without a per-core L2 or the cluster is configured without an L3 cache, this event is not implemented\000"
/* offset=76130 */ "l1d_ws_mode_enter\000cache\000L1D entering write stream mode\000event=0xc6\000\00000\000L1D entering write stream mode\000"
/* offset=76231 */ "l1d_ws_mode\000cache\000L1D is in write stream mode\000event=0xc7\000\00000\000L1D is in write stream mode\000"
/* offset=76320 */ "l2d_ws_mode\000cache\000Level 2 cache write streaming mode. This event counts for each cycle where the core is in write-streaming mode and not allocating writes into the L2 cache\000event=0xc8\000\00000\000Level 2 cache write streaming mode. This event counts for each cycle where the core is in write-streaming mode and not allocating writes into the L2 cache\000"
/* offset=76663 */ "l3d_ws_mode\000cache\000Level 3 cache write streaming mode. This event counts for each cycle where the core is in write-streaming mode and not allocating writes into the L3 cache\000event=0xc9\000\00000\000Level 3 cache write streaming mode. This event counts for each cycle where the core is in write-streaming mode and not allocating writes into the L3 cache\000"
/* offset=77006 */ "tlb_l2tlb_llwalk_access\000cache\000Level 2 TLB last-level walk cache access. This event does not count if the MMU is disabled\000event=0xca\000\00000\000Level 2 TLB last-level walk cache access. This event does not count if the MMU is disabled\000"
/* offset=77233 */ "tlb_l2tlb_llwalk_refill\000cache\000Level 2 TLB last-level walk cache refill. This event does not count if the MMU is disabled\000event=0xcb\000\00000\000Level 2 TLB last-level walk cache refill. This event does not count if the MMU is disabled\000"
/* offset=77460 */ "tlb_l2tlb_l2walk_access\000cache\000Level 2 TLB level-2 walk cache access. This event counts accesses to the level-2 walk cache where the last-level walk cache has missed. The event only counts when the translation regime of the pagewalk uses level 2 descriptors. This event does not count if the MMU is disabled\000event=0xcc\000\00000\000Level 2 TLB level-2 walk cache access. This event counts accesses to the level-2 walk cache where the last-level walk cache has missed. The event only counts when the translation regime of the pagewalk uses level 2 descriptors. This event does not count if the MMU is disabled\000"
/* offset=78059 */ "tlb_l2tlb_l2walk_refill\000cache\000Level 2 TLB level-2 walk cache refill. This event does not count if the MMU is disabled\000event=0xcd\000\00000\000Level 2 TLB level-2 walk cache refill. This event does not count if the MMU is disabled\000"
/* offset=78280 */ "tlb_l2tlb_s2_access\000cache\000Level 2 TLB IPA cache access. This event counts on each access to the IPA cache. +//0 If a single pagewalk needs to make multiple accesses to the IPA cache, each access is counted. +//0 If stage 2 translation is disabled, this event does not count\000event=0xce\000\00000\000Level 2 TLB IPA cache access. This event counts on each access to the IPA cache. +//0 If a single pagewalk needs to make multiple accesses to the IPA cache, each access is counted. +//0 If stage 2 translation is disabled, this event does not count\000"
/* offset=78817 */ "tlb_l2tlb_s2_refill\000cache\000Level 2 TLB IPA cache refill. This event counts on each refill of the IPA cache. +//0 If a single pagewalk needs to make multiple accesses to the IPA cache, each access which causes a refill is counted. +//0 If stage 2 translation is disabled, this event does not count\000event=0xcf\000\00000\000Level 2 TLB IPA cache refill. This event counts on each refill of the IPA cache. +//0 If a single pagewalk needs to make multiple accesses to the IPA cache, each access which causes a refill is counted. +//0 If stage 2 translation is disabled, this event does not count\000"
/* offset=79398 */ "l2_l1d_cache_wb_unatt\000cache\000Unattributable Level 1 data cache write-back. This event occurs when a requestor outside the PE makes a coherency request that results in writeback\000event=0xf0\000\00000\000Unattributable Level 1 data cache write-back. This event occurs when a requestor outside the PE makes a coherency request that results in writeback\000"
/* offset=79737 */ "l2_l2d_cache_unatt\000cache\000Unattributable Level 2 data cache access. This event occurs when a requestor outside the PE makes a coherency request that results in level 2 data cache access\000event=0xf1\000\00000\000Unattributable Level 2 data cache access. This event occurs when a requestor outside the PE makes a coherency request that results in level 2 data cache access\000"
/* offset=80097 */ "l2_l2d_cache_rd_unatt\000cache\000Unattributable Level 2 data cache access, read. This event occurs when a requestor outside the PE makes a coherency request that results in level 2 data cache read access\000event=0xf2\000\00000\000Unattributable Level 2 data cache access, read. This event occurs when a requestor outside the PE makes a coherency request that results in level 2 data cache read access\000"
/* offset=80482 */ "l2_l3d_cache_unatt\000cache\000Unattributable Level 3 data cache access. This event occurs when a requestor outside the PE makes a coherency request that results in level 3 data cache read access\000event=0xf3\000\00000\000Unattributable Level 3 data cache access. This event occurs when a requestor outside the PE makes a coherency request that results in level 3 data cache read access\000"
/* offset=80852 */ "l2_l3d_cache_rd_unatt\000cache\000Unattributable Level 3 data cache access, read. This event occurs when a requestor outside the PE makes a coherency request that results in level 3 data cache read access\000event=0xf4\000\00000\000Unattributable Level 3 data cache access, read. This event occurs when a requestor outside the PE makes a coherency request that results in level 3 data cache read access\000"
/* offset=81237 */ "l2_l3d_cache_alloc_unatt\000cache\000Unattributable Level 3 data or unified cache allocation without refill. This event occurs when a requestor outside the PE makes a coherency request that results in level 3 cache allocate without refill\000event=0xf5\000\00000\000Unattributable Level 3 data or unified cache allocation without refill. This event occurs when a requestor outside the PE makes a coherency request that results in level 3 cache allocate without refill\000"
/* offset=81687 */ "l2_l3d_cache_refill_unatt\000cache\000Unattributable Level 3 data or unified cache refill. This event occurs when a requestor outside the PE makes a coherency request that results in level 3 cache refill\000event=0xf6\000\00000\000Unattributable Level 3 data or unified cache refill. This event occurs when a requestor outside the PE makes a coherency request that results in level 3 cache refill\000"
/* offset=82066 */ "l2d_cache_stash_dropped\000cache\000Level 2 cache stash dropped. This event counts on each stash request received from the interconnect or ACP, that is targeting L2 and gets dropped due to lack of buffer space to hold the request. L2 and L3 cache events (L2D_CACHE*, L3D_CACHE*) The behavior of these events depends on the configuration of the core. If the private L2 cache is present, the L2D_CACHE* events count the activity in the private L2 cache, and the L3D_CACHE* events count the activity in the DSU L3 cache (if present). If the private L2 cache is not present but the DSU L3 cache is present, the L2D_CACHE* events count activity in the DSU L3 cache and the L3D_CACHE* events do not count. The L2D_CACHE_WB, L2D_CACHE_WR and L2D_CACHE_REFILL_WR events do not count in this configuration. If neither the private L2 cache nor the DSU L3 cache are present, neither the L2D_CACHE* or L3D_CACHE* events will count\000event=0xf7\000\00000\000Level 2 cache stash dropped. This event counts on each stash request received from the interconnect or ACP, that is targeting L2 and gets dropped due to lack of buffer space to hold the request. L2 and L3 cache events (L2D_CACHE*, L3D_CACHE*) The behavior of these events depends on the configuration of the core. If the private L2 cache is present, the L2D_CACHE* events count the activity in the private L2 cache, and the L3D_CACHE* events count the activity in the DSU L3 cache (if present). If the private L2 cache is not present but the DSU L3 cache is present, the L2D_CACHE* events count activity in the DSU L3 cache and the L3D_CACHE* events do not count. The L2D_CACHE_WB, L2D_CACHE_WR and L2D_CACHE_REFILL_WR events do not count in this configuration. If neither the private L2 cache nor the DSU L3 cache are present, neither the L2D_CACHE* or L3D_CACHE* events will count\000"
/* offset=83877 */ "dpu_br_ind_mis\000dpu\000Instruction retired, indirect branch, mispredicted\000event=0xe9\000\00000\000Instruction retired, indirect branch, mispredicted\000"
/* offset=84013 */ "dpu_br_cond_mis\000dpu\000Instruction retired, conditional branch, mispredicted\000event=0xea\000\00000\000Instruction retired, conditional branch, mispredicted\000"
/* offset=84156 */ "dpu_mem_err_ifu\000dpu\000Memory error (any type) from IFU\000event=0xeb\000\00000\000Memory error (any type) from IFU\000"
/* offset=84257 */ "dpu_mem_err_dcu\000dpu\000Memory error (any type) from DCU\000event=0xec\000\00000\000Memory error (any type) from DCU\000"
/* offset=84358 */ "dpu_mem_err_tlb\000dpu\000Memory error (any type) from TLB\000event=0xed\000\00000\000Memory error (any type) from TLB\000"
/* offset=84459 */ "ifu_ic_miss_wait\000ifu\000I-Cache miss on an access from the prefetch block\000event=0xd0\000\00000\000I-Cache miss on an access from the prefetch block\000"
/* offset=84595 */ "ifu_iutlb_miss_wait\000ifu\000Counts the cycles spent on a request for Level 2 TLB lookup after a Level 1l ITLB miss\000event=0xd1\000\00000\000Counts the cycles spent on a request for Level 2 TLB lookup after a Level 1l ITLB miss\000"
/* offset=84808 */ "ifu_micro_cond_mispred\000ifu\000Micro-predictor conditional/direction mispredict, with respect to. if3/if4 predictor\000event=0xd2\000\00000\000Micro-predictor conditional/direction mispredict, with respect to. if3/if4 predictor\000"
/* offset=85020 */ "ifu_micro_caddr_mispred\000ifu\000Micro-predictor address mispredict, with respect to if3/if4 predictor\000event=0xd3\000\00000\000Micro-predictor address mispredict, with respect to if3/if4 predictor\000"
/* offset=85203 */ "ifu_micro_hit\000ifu\000Micro-predictor hit with immediate redirect\000event=0xd4\000\00000\000Micro-predictor hit with immediate redirect\000"
/* offset=85324 */ "ifu_micro_neg_hit\000ifu\000Micro-predictor negative cache hit\000event=0xd6\000\00000\000Micro-predictor negative cache hit\000"
/* offset=85431 */ "ifu_micro_correction\000ifu\000Micro-predictor correction\000event=0xd7\000\00000\000Micro-predictor correction\000"
/* offset=85525 */ "ifu_micro_no_instr1\000ifu\000A 2nd instruction could have been pushed but was not because it was nonsequential\000event=0xd8\000\00000\000A 2nd instruction could have been pushed but was not because it was nonsequential\000"
/* offset=85728 */ "ifu_micro_no_pred\000ifu\000Micro-predictor miss\000event=0xd9\000\00000\000Micro-predictor miss\000"
/* offset=85807 */ "ifu_flushed_tlb_miss\000ifu\000Thread flushed due to TLB miss\000event=0xda\000\00000\000Thread flushed due to TLB miss\000"
/* offset=85909 */ "ifu_flushed_excl_tlb_miss\000ifu\000Thread flushed due to reasons other than TLB miss\000event=0xdb\000\00000\000Thread flushed due to reasons other than TLB miss\000"
/* offset=86054 */ "ifu_all_thrds_rdy\000ifu\000This thread and the other thread both ready for scheduling in if0\000event=0xdc\000\00000\000This thread and the other thread both ready for scheduling in if0\000"
/* offset=86223 */ "ifu_win_arb_other_rdy\000ifu\000This thread was arbitrated when the other thread was also ready for scheduling\000event=0xdd\000\00000\000This thread was arbitrated when the other thread was also ready for scheduling\000"
/* offset=86422 */ "ifu_win_arb_other_act\000ifu\000This thread was arbitrated when the other thread was also active, but not necessarily ready. For example, waiting for I-Cache or TLB\000event=0xde\000\00000\000This thread was arbitrated when the other thread was also active, but not necessarily ready. For example, waiting for I-Cache or TLB\000"
/* offset=86729 */ "ifu_not_rdy_for_arb\000ifu\000This thread was not arbitrated because it was not ready for scheduling. For example, due to a cache miss or TLB miss\000event=0xdf\000\00000\000This thread was not arbitrated because it was not ready for scheduling. For example, due to a cache miss or TLB miss\000"
/* offset=87002 */ "ifu_goto_idle\000ifu\000The thread moved from an active state to an inactive state (long-term sleep state, causing deallocation of some resources)\000event=0xe0\000\00000\000The thread moved from an active state to an inactive state (long-term sleep state, causing deallocation of some resources)\000"
/* offset=87281 */ "ifu_ic_lookup_under_miss\000ifu\000I-Cache lookup under miss from other thread\000event=0xe1\000\00000\000I-Cache lookup under miss from other thread\000"
/* offset=87413 */ "ifu_ic_miss_under_miss\000ifu\000I-Cache miss under miss from other thread\000event=0xe2\000\00000\000I-Cache miss under miss from other thread\000"
/* offset=87539 */ "ifu_instr_pushed\000ifu\000This thread pushed an instruction into the IQ\000event=0xe3\000\00000\000This thread pushed an instruction into the IQ\000"
/* offset=87667 */ "ifu_ic_lf_sp\000ifu\000I-Cache Speculative line fill\000event=0xe4\000\00000\000I-Cache Speculative line fill\000"
/* offset=87759 */ "dpu_br_cond_retired\000instruction\000Instruction retired, conditional branch\000event=0xe8\000\00000\000Instruction retired, conditional branch\000"
/* offset=87886 */ "biu_ext_mem_req\000memory\000External memory request\000event=0xc1\000\00000\000External memory request\000"
/* offset=87972 */ "biu_ext_mem_req_nc\000memory\000External memory request to non-cacheable memory\000event=0xc2\000\00000\000External memory request to non-cacheable memory\000"
/* offset=88109 */ "l1d_tlb_rd\000cache\000L1D tlb access, read\000event=0x4e\000\00000\000Attributable Level 1 data or unified TLB access, read\000"
/* offset=88216 */ "l1d_tlb_wr\000cache\000L1D tlb access, write\000event=0x4f\000\00000\000Attributable Level 1 data or unified TLB access, write\000"
/* offset=88325 */ "l2d_tlb_refill_rd\000cache\000L2D cache refill, read\000event=0x5c\000\00000\000Attributable Level 2 data or unified TLB refill, read\000"
/* offset=88441 */ "l2d_tlb_refill_wr\000cache\000L2D cache refill, write\000event=0x5d\000\00000\000Attributable Level 2 data or unified TLB refill, write\000"
/* offset=88559 */ "l2d_tlb_rd\000cache\000L2D cache access, read\000event=0x5e\000\00000\000Attributable Level 2 data or unified TLB access, read\000"
/* offset=88668 */ "l2d_tlb_wr\000cache\000L2D cache access, write\000event=0x5f\000\00000\000Attributable Level 2 data or unified TLB access, write\000"
/* offset=88779 */ "strex_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX\000event=0x6f\000\00000\000Exclusive operation speculatively executed, STREX or STX\000"
/* offset=88931 */ "ase_inst_spec\000instruction\000ASE operations speculatively executed\000event=0x8005\000\00000\000ASE operations speculatively executed\000"
/* offset=89050 */ "sve_pred_spec\000instruction\000SVE predicated Operations speculatively executed\000event=0x8074\000\00000\000SVE predicated Operations speculatively executed\000"
/* offset=89191 */ "sve_pred_empty_spec\000instruction\000SVE predicated operations with no active predicates speculatively executed\000event=0x8075\000\00000\000SVE predicated operations with no active predicates speculatively executed\000"
/* offset=89390 */ "sve_pred_full_spec\000instruction\000SVE predicated operations speculatively executed with all active predicates\000event=0x8076\000\00000\000SVE predicated operations speculatively executed with all active predicates\000"
/* offset=89590 */ "sve_pred_partial_spec\000instruction\000SVE predicated operations speculatively executed with partially active predicates\000event=0x8077\000\00000\000SVE predicated operations speculatively executed with partially active predicates\000"
/* offset=89805 */ "sve_pred_not_full_spec\000instruction\000SVE predicated operations with empty or partially active predicates\000event=0x8079\000\00000\000SVE predicated operations with empty or partially active predicates\000"
/* offset=89993 */ "sve_ldff_spec\000instruction\000SVE First-fault load Operations speculatively executed\000event=0x80bc\000\00000\000SVE First-fault load Operations speculatively executed\000"
/* offset=90146 */ "sve_ldff_fault_spec\000instruction\000SVE first-fault load operations speculatively executed which set FFR bit to 0\000event=0x80bd\000\00000\000SVE first-fault load operations speculatively executed which set FFR bit to 0\000"
/* offset=90351 */ "fp_scale_ops_spec\000instruction\000Scalable floating-point element Operations speculatively executed\000event=0x80c0\000\00000\000Scalable floating-point element Operations speculatively executed\000"
/* offset=90530 */ "fp_fixed_ops_spec\000instruction\000Non-scalable floating-point element Operations speculatively executed\000event=0x80c1\000\00000\000Non-scalable floating-point element Operations speculatively executed\000"
/* offset=90717 */ "remote_access\000memory\000Access to another socket in a multi-socket system\000event=0x31\000\00000\000Access to another socket in a multi-socket system\000"
/* offset=90853 */ "i_tag_ram_rd\000cache\000Number of ways read in the instruction cache - Tag RAM\000event=0xc2\000\00000\000Number of ways read in the instruction cache - Tag RAM\000"
/* offset=90997 */ "i_data_ram_rd\000cache\000Number of ways read in the instruction cache - Data RAM\000event=0xc3\000\00000\000Number of ways read in the instruction cache - Data RAM\000"
/* offset=91144 */ "i_btac_ram_rd\000cache\000Number of ways read in the instruction BTAC RAM\000event=0xc4\000\00000\000Number of ways read in the instruction BTAC RAM\000"
/* offset=91275 */ "pld_utlb_refill\000cache\000Level 1 PLD TLB refill\000event=0xe7\000\00000\000Level 1 PLD TLB refill\000"
/* offset=91358 */ "cp15_utlb_refill\000cache\000Level 1 CP15 TLB refill\000event=0xe8\000\00000\000Level 1 CP15 TLB refill\000"
/* offset=91444 */ "utlb_flush\000cache\000Level 1 TLB flush\000event=0xe9\000\00000\000Level 1 TLB flush\000"
/* offset=91512 */ "tlb_access\000cache\000Level 2 TLB access\000event=0xea\000\00000\000Level 2 TLB access\000"
/* offset=91582 */ "tlb_miss\000cache\000Level 2 TLB miss\000event=0xeb\000\00000\000Level 2 TLB miss\000"
/* offset=91646 */ "dcache_self_hit_vipt\000cache\000Data cache hit in itself due to VIPT aliasing\000event=0xec\000\00000\000Data cache hit in itself due to VIPT aliasing\000"
/* offset=91780 */ "etm_ext_out0\000etm\000ETM trace unit output 0\000event=0xde\000\00000\000ETM trace unit output 0\000"
/* offset=91860 */ "etm_ext_out1\000etm\000ETM trace unit output 1\000event=0xdf\000\00000\000ETM trace unit output 1\000"
/* offset=91940 */ "exc_trap_hyp\000exception\000Number of Traps to hypervisor\000event=0xdc\000\00000\000Number of Traps to hypervisor\000"
/* offset=92038 */ "mmu_ptw\000mmu\000Duration of a translation table walk handled by the MMU\000event=0xe0\000\00000\000Duration of a translation table walk handled by the MMU\000"
/* offset=92177 */ "mmu_ptw_st1\000mmu\000Duration of a Stage 1 translation table walk handled by the MMU\000event=0xe1\000\00000\000Duration of a Stage 1 translation table walk handled by the MMU\000"
/* offset=92336 */ "mmu_ptw_st2\000mmu\000Duration of a Stage 2 translation table walk handled by the MMU\000event=0xe2\000\00000\000Duration of a Stage 2 translation table walk handled by the MMU\000"
/* offset=92495 */ "mmu_ptw_lsu\000mmu\000Duration of a translation table walk requested by the LSU\000event=0xe3\000\00000\000Duration of a translation table walk requested by the LSU\000"
/* offset=92642 */ "mmu_ptw_iside\000mmu\000Duration of a translation table walk requested by the Instruction Side\000event=0xe4\000\00000\000Duration of a translation table walk requested by the Instruction Side\000"
/* offset=92817 */ "mmu_ptw_pld\000mmu\000Duration of a translation table walk requested by a Preload instruction or Prefetch request\000event=0xe5\000\00000\000Duration of a translation table walk requested by a Preload instruction or Prefetch request\000"
/* offset=93032 */ "mmu_ptw_cp15\000mmu\000Duration of a translation table walk requested by a CP15 operation (maintenance by MVA and VA to PA operations)\000event=0xe6\000\00000\000Duration of a translation table walk requested by a CP15 operation (maintenance by MVA and VA to PA operations)\000"
/* offset=93288 */ "lf_stall\000pipeline\000A linefill caused an instruction side stall\000event=0xc0\000\00000\000A linefill caused an instruction side stall\000"
/* offset=93409 */ "ptw_stall\000pipeline\000A translation table walk caused an instruction side stall\000event=0xc1\000\00000\000A translation table walk caused an instruction side stall\000"
/* offset=93559 */ "d_lsu_slot_full\000pipeline\000Duration for which all slots in the Load-Store Unit are busy\000event=0xd3\000\00000\000Duration for which all slots in the Load-Store Unit are busy\000"
/* offset=93721 */ "ls_iq_full\000pipeline\000Duration for which all slots in the load-store issue queue are busy\000event=0xd8\000\00000\000Duration for which all slots in the load-store issue queue are busy\000"
/* offset=93892 */ "dp_iq_full\000pipeline\000Duration for which all slots in the data processing issue queue are busy\000event=0xd9\000\00000\000Duration for which all slots in the data processing issue queue are busy\000"
/* offset=94073 */ "de_iq_full\000pipeline\000Duration for which all slots in the Data Engine issue queue are busy\000event=0xda\000\00000\000Duration for which all slots in the Data Engine issue queue are busy\000"
/* offset=94246 */ "l1d_cache_allocate\000cache\000Level 1 data cache allocation without refill\000event=0x1f\000\00000\000Level 1 data cache allocation without refill\000"
/* offset=94376 */ "l1pld_tlb_refill\000cache\000Level 1 PLD TLB refill\000event=0xe7\000\00000\000Level 1 PLD TLB refill\000"
/* offset=94460 */ "l2pld_tlb\000cache\000Level 2 preload and MMU prefetcher TLB access. This event only counts software and hardware prefetches at Level 2\000event=0xe8\000\00000\000Level 2 preload and MMU prefetcher TLB access. This event only counts software and hardware prefetches at Level 2\000"
/* offset=94719 */ "l1pld_tlb\000cache\000Level 1 preload TLB access. This event only counts software and hardware prefetches at Level 1. This event counts all accesses to the preload data micro TLB, that is L1 prefetcher and preload instructions. This event does not take into account whether the MMU is enabled or not\000event=0xeb\000\00000\000Level 1 preload TLB access. This event only counts software and hardware prefetches at Level 1. This event counts all accesses to the preload data micro TLB, that is L1 prefetcher and preload instructions. This event does not take into account whether the MMU is enabled or not\000"
/* offset=95306 */ "pldtlb_walk\000cache\000Prefetch access to unified TLB that caused a page table walk. This event counts software and hardware prefetches\000event=0xec\000\00000\000Prefetch access to unified TLB that caused a page table walk. This event counts software and hardware prefetches\000"
/* offset=95565 */ "exc_trap_hyp\000exception\000Number of traps to hypervisor. This event counts the number of exception traps taken to EL2, excluding HVC instructions. This event is set every time that an exception is executed because of a decoded trap to the hypervisor. CCFAIL exceptions and traps caused by HVC instructions are excluded. This event is not counted when it is accessible from Non-secure EL0 or EL1\000event=0xdc\000\00000\000Number of traps to hypervisor. This event counts the number of exception traps taken to EL2, excluding HVC instructions. This event is set every time that an exception is executed because of a decoded trap to the hypervisor. CCFAIL exceptions and traps caused by HVC instructions are excluded. This event is not counted when it is accessible from Non-secure EL0 or EL1\000"
/* offset=96341 */ "mmu_ptw_st1\000mmu\000Duration of a Stage 1 translation table walk handled by the MMU. This event is not counted when it is accessible from Non-secure EL0 or EL1\000event=0xe1\000\00000\000Duration of a Stage 1 translation table walk handled by the MMU. This event is not counted when it is accessible from Non-secure EL0 or EL1\000"
/* offset=96652 */ "mmu_ptw_st2\000mmu\000Duration of a Stage 2 translation table walk handled by the MMU. This event is not counted when it is accessible from Non-secure EL0 or EL1\000event=0xe2\000\00000\000Duration of a Stage 2 translation table walk handled by the MMU. This event is not counted when it is accessible from Non-secure EL0 or EL1\000"
/* offset=96963 */ "mmu_ptw_iside\000mmu\000Duration of a translation table walk requested by the instruction side\000event=0xe4\000\00000\000Duration of a translation table walk requested by the instruction side\000"
/* offset=97138 */ "mmu_ptw_cp15\000mmu\000Duration of a translation table walk requested by an address translation operation\000event=0xe6\000\00000\000Duration of a translation table walk requested by an address translation operation\000"
/* offset=97336 */ "d_lsu_slot_full\000pipeline\000Duration for which all slots in the Load-Store Unit (LSU) are busy\000event=0xd3\000\00000\000Duration for which all slots in the Load-Store Unit (LSU) are busy\000"
/* offset=97510 */ "ls_iq_full\000pipeline\000Duration for which all slots in the load-store issue queue are busy. This event counts the cycles where all slots in the LS IQs are full with micro-operations waiting for issuing, and the dispatch stage is not empty\000event=0xd8\000\00000\000Duration for which all slots in the load-store issue queue are busy. This event counts the cycles where all slots in the LS IQs are full with micro-operations waiting for issuing, and the dispatch stage is not empty\000"
/* offset=97977 */ "dp_iq_full\000pipeline\000Duration for which all slots in the data processing issue queue are busy. This event counts the cycles where all slots in the DP0 and DP1 IQs are full with micro-operations waiting for issuing, and the despatch stage is not empty\000event=0xd9\000\00000\000Duration for which all slots in the data processing issue queue are busy. This event counts the cycles where all slots in the DP0 and DP1 IQs are full with micro-operations waiting for issuing, and the despatch stage is not empty\000"
/* offset=98472 */ "de_iq_full\000pipeline\000Duration for which all slots in the data engine issue queue are busy. This event is set every time that the data engine rename has at least one valid instruction, excluding No Operations (NOPs), that cannot move to the issue stage because accpt_instr is LOW\000event=0xda\000\00000\000Duration for which all slots in the data engine issue queue are busy. This event is set every time that the data engine rename has at least one valid instruction, excluding No Operations (NOPs), that cannot move to the issue stage because accpt_instr is LOW\000"
/* offset=99023 */ "br_mis_pred\000branch\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000This event counts any predictable branch instruction which is mispredicted either due to dynamic misprediction or because the MMU is off and the branches are statically predicted not taken\000"
/* offset=99306 */ "br_pred\000branch\000Predictable branch speculatively executed\000event=0x12\000\00000\000This event counts all predictable branches\000"
/* offset=99421 */ "cpu_cycles\000bus\000The number of core clock cycles\000event=0x11\000\00000\000The number of core clock cycles\000"
/* offset=99515 */ "bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000This event counts for every beat of data transferred over the data channels between the core and the SCU. If both read and write data beats are transferred on a given cycle, this event is counted twice on that cycle. This event counts the sum of BUS_ACCESS_RD and BUS_ACCESS_WR\000"
/* offset=99847 */ "bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000This event duplicates CPU_CYCLES\000"
/* offset=99920 */ "l1i_cache_refill\000cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000This event counts any instruction fetch which misses in the cache\000"
/* offset=100056 */ "l1i_tlb_refill\000cache\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000This event counts any refill of the instruction L1 TLB from the L2 TLB. This includes refills that result in a translation fault\000"
/* offset=100264 */ "l1d_cache_refill\000cache\000Level 1 data cache refill\000event=0x3\000\00000\000This event counts any load or store operation or page table walk access which causes data to be read from outside the L1, including accesses which do not allocate into L1\000"
/* offset=100498 */ "l1d_cache\000cache\000Level 1 data cache access\000event=0x4\000\00000\000This event counts any load or store operation or page table walk access which looks up in the L1 data cache. In particular, any access which could count the L1D_CACHE_REFILL event causes this event to count\000"
/* offset=100761 */ "l1d_tlb_refill\000cache\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000This event counts any refill of the data L1 TLB from the L2 TLB. This includes refills that result in a translation fault\000"
/* offset=100955 */ "l1i_cache\000cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Level 1 instruction cache access or Level 0 Macro-op cache access. This event counts any instruction fetch which accesses the L1 instruction cache or L0 Macro-op cache\000"
/* offset=101200 */ "l1d_cache_wb\000cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000This event counts any write-back of data from the L1 data cache to L2 or L3. This counts both victim line evictions and snoops, including cache maintenance operations\000"
/* offset=101444 */ "l2d_cache\000cache\000Level 2 data cache access\000event=0x16\000\00000\000This event counts any transaction from L1 which looks up in the L2 cache, and any write-back from the L1 to the L2. Snoops from outside the core and cache maintenance operations are not counted\000"
/* offset=101695 */ "l2d_cache_refill\000cache\000Level 2 data refill\000event=0x17\000\00000\000L2 data cache refill. This event counts any cacheable transaction from L1 which causes data to be read from outside the core. L2 refills caused by stashes into L2 should not be counted\000"
/* offset=101938 */ "l2d_cache_wb\000cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000This event counts any write-back of data from the L2 cache to outside the core. This includes snoops to the L2 which return data, regardless of whether they cause an invalidation. Invalidations from the L2 which do not write data outside of the core and snoops which return data from the L1 are not counted\000"
/* offset=102322 */ "l2d_cache_allocate\000cache\000Attributable Level 2 data cache allocation without refill\000event=0x20\000\00000\000This event counts any full cache line write into the L2 cache which does not cause a linefill, including write-backs from L1 to L2 and full-line writes which do not allocate into L1\000"
/* offset=102602 */ "l1d_tlb\000cache\000Level 1 data TLB access\000event=0x25\000\00000\000This event counts any load or store operation which accesses the data L1 TLB. If both a load and a store are executed on a cycle, this event counts twice. This event counts regardless of whether the MMU is enabled\000"
/* offset=102869 */ "l1i_tlb\000cache\000Level 1 instruction TLB access\000event=0x26\000\00000\000This event counts any instruction fetch which accesses the instruction L1 TLB.This event counts regardless of whether the MMU is enabled\000"
/* offset=103066 */ "l3d_cache_allocate\000cache\000Allocation without refill\000event=0x29\000\00000\000This event counts any full cache line write into the L3 cache which does not cause a linefill, including write-backs from L2 to L3 and full-line writes which do not allocate into L2\000"
/* offset=103314 */ "l3d_cache_refill\000cache\000Attributable Level 3 unified cache refill\000event=0x2a\000\00000\000This event counts for any cacheable read transaction returning datafrom the SCU for which the data source was outside the cluster. Transactions such as ReadUnique are counted here as 'read' transactions, even though they can be generated by store instructions\000"
/* offset=103654 */ "l3d_cache\000cache\000Attributable Level 3 unified cache access\000event=0x2b\000\00000\000This event counts for any cacheable read transaction returning datafrom the SCU, or for any cacheable write to the SCU\000"
/* offset=103846 */ "l2d_tlb_refill\000cache\000Attributable L2 data or unified TLB refill\000event=0x2d\000\00000\000This event counts on anyrefill of the L2 TLB, caused by either an instruction or data access.This event does not count if the MMU is disabled\000"
/* offset=104067 */ "l2d_tlb\000cache\000Attributable Level 2 data or unified TLB access\000event=0x2f\000\00000\000This event counts on any access to the L2 TLB (caused by a refill of any of the L1 TLBs). This event does not count if the MMU is disabled\000"
/* offset=104283 */ "dtlb_walk\000cache\000Access to data TLB causes a translation table walk\000event=0x34\000\00000\000This event counts on any data access which causes L2D_TLB_REFILL to count\000"
/* offset=104439 */ "itlb_walk\000cache\000Access to instruction TLB that causes a translation table walk\000event=0x35\000\00000\000This event counts on any instruction access which causes L2D_TLB_REFILL to count\000"
/* offset=104614 */ "memory_error\000exception\000Local memory error\000event=0x1a\000\00000\000This event counts any correctable or uncorrectable memory error (ECC or parity) in the protected core RAMs\000"
/* offset=104778 */ "inst_retired\000instruction\000Instruction architecturally executed\000event=0x8\000\00000\000This event counts all retired instructions, including those that fail their condition check\000"
/* offset=104946 */ "cid_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000This event only counts writes to CONTEXTIDR in AArch32 state, and via the CONTEXTIDR_EL1 mnemonic in AArch64 state\000"
/* offset=105190 */ "ttbr_write_retired\000instruction\000Instruction architecturally executed, Condition code check pass, write to TTBR\000event=0x1c\000\00000\000This event only counts writes to TTBR0/TTBR1 in AArch32 state and TTBR0_EL1/TTBR1_EL1 in AArch64 state\000"
/* offset=105418 */ "br_immed_spec\000instruction\000Branch speculatively executed, immediate branch\000event=0x78\000\00000\000Branch speculatively executed, immediate branch\000"
/* offset=105555 */ "br_indirect_spec\000instruction\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000"
/* offset=105693 */ "br_return_spec\000instruction\000Branch speculatively executed, procedure return\000event=0x79\000\00000\000Branch speculatively executed, procedure return\000"
/* offset=105831 */ "mem_access\000memory\000Data memory access\000event=0x13\000\00000\000This event counts memory accesses due to load or store instructions. This event counts the sum of MEM_ACCESS_RD and MEM_ACCESS_WR\000"
/* offset=106013 */ "stall_frontend\000pipeline\000No operation issued because of the frontend\000event=0x23\000\00000\000The counter counts on any cycle when there are no fetched instructions available to dispatch\000"
/* offset=106189 */ "stall_backend\000pipeline\000No operation issued due to the backend\000event=0x24\000\00000\000The counter counts on any cycle fetched instructions are not dispatched due to resource constraints\000"
/* offset=106366 */ "bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000Counts memory transactions issued by the CPU to the external bus, including snoop requests and snoop responses. Each beat of data is counted individually\000"
/* offset=106574 */ "bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000Counts bus cycles in the CPU. Bus cycles represent a clock cycle in which a transaction could be sent or received on the interface from the CPU to the external bus. Since that interface is driven at the same clock speed as the CPU, this event is a duplicate of CPU_CYCLES\000"
/* offset=106886 */ "bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Counts memory read transactions seen on the external bus. Each beat of data is counted individually\000"
/* offset=107035 */ "bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Counts memory write transactions seen on the external bus. Each beat of data is counted individually\000"
/* offset=107186 */ "exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Counts any taken architecturally visible exceptions such as IRQ, FIQ, SError, and other synchronous exceptions. Exceptions are counted whether or not they are taken locally\000"
/* offset=107409 */ "exc_return\000exception\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Counts any architecturally executed exception return instructions. Eg: AArch64: ERET\000"
/* offset=107606 */ "exc_undef\000exception\000Exception taken, Other synchronous\000event=0x81\000\00000\000Counts the number of synchronous exceptions which are taken locally that are due to attempting to execute an instruction that is UNDEFINED. Attempting to execute instruction bit patterns that have not been allocated. Attempting to execute instructions when they are disabled. Attempting to execute instructions at an inappropriate Exception level. Attempting to execute an instruction when the value of PSTATE.IL is 1\000"
/* offset=108094 */ "exc_svc\000exception\000Exception taken, Supervisor Call\000event=0x82\000\00000\000Counts SVC exceptions taken locally\000"
/* offset=108196 */ "exc_pabort\000exception\000Exception taken, Instruction Abort\000event=0x83\000\00000\000Counts synchronous exceptions that are taken locally and caused by Instruction Aborts\000"
/* offset=108353 */ "exc_dabort\000exception\000Exception taken, Data Abort and SError\000event=0x84\000\00000\000Counts exceptions that are taken locally and are caused by data aborts or SErrors. Conditions that could cause those exceptions are attempting to read or write memory where the MMU generates a fault, attempting to read or write memory with a misaligned address, interrupts from the nSEI inputs and internally generated SErrors\000"
/* offset=108755 */ "exc_irq\000exception\000Exception taken, IRQ\000event=0x86\000\00000\000Counts IRQ exceptions including the virtual IRQs that are taken locally\000"
/* offset=108881 */ "exc_fiq\000exception\000Exception taken, FIQ\000event=0x87\000\00000\000Counts FIQ exceptions including the virtual FIQs that are taken locally\000"
/* offset=109007 */ "exc_smc\000exception\000Exception taken, Secure Monitor Call\000event=0x88\000\00000\000Counts SMC exceptions take to EL3\000"
/* offset=109111 */ "exc_hvc\000exception\000Exception taken, Hypervisor Call\000event=0x8a\000\00000\000Counts HVC exceptions taken to EL2\000"
/* offset=109212 */ "exc_trap_pabort\000exception\000Exception taken, Instruction Abort not taken locally\000event=0x8b\000\00000\000Counts exceptions which are traps not taken locally and are caused by Instruction Aborts. For example, attempting to execute an instruction with a misaligned PC\000"
/* offset=109467 */ "exc_trap_dabort\000exception\000Exception taken, Data Abort or SError not taken locally\000event=0x8c\000\00000\000Counts exceptions which are traps not taken locally and are caused by Data Aborts or SError interrupts. Conditions that could cause those exceptions are:\n\n1. Attempting to read or write memory where the MMU generates a fault,\n2. Attempting to read or write memory with a misaligned address,\n3. Interrupts from the SEI input.\n4. internally generated SErrors\000"
/* offset=109921 */ "exc_trap_other\000exception\000Exception taken, Other traps not taken locally\000event=0x8d\000\00000\000Counts the number of synchronous trap exceptions which are not taken locally and are not SVC, SMC, HVC, data aborts, Instruction Aborts, or interrupts\000"
/* offset=110159 */ "exc_trap_irq\000exception\000Exception taken, IRQ not taken locally\000event=0x8e\000\00000\000Counts IRQ exceptions including the virtual IRQs that are not taken locally\000"
/* offset=110312 */ "exc_trap_fiq\000exception\000Exception taken, FIQ not taken locally\000event=0x8f\000\00000\000Counts FIQs which are not taken locally but taken from EL0, EL1,\n or EL2 to EL3 (which would be the normal behavior for FIQs when not executing\n in EL3)\000"
/* offset=110542 */ "cpu_cycles\000general\000Cycle\000event=0x11\000\00000\000Counts CPU clock cycles (not timer cycles). The clock measured by this event is defined as the physical clock driving the CPU logic\000"
/* offset=110714 */ "l1d_cache_refill\000l1d_cache\000Level 1 data cache refill\000event=0x3\000\00000\000Counts level 1 data cache refills caused by speculatively executed load or store operations that missed in the level 1 data cache. This event only counts one event per cache line. This event does not count cache line allocations from preload instructions or from hardware cache prefetching\000"
/* offset=111071 */ "l1d_cache\000l1d_cache\000Level 1 data cache access\000event=0x4\000\00000\000Counts level 1 data cache accesses from any load/store operations. Atomic operations that resolve in the CPUs caches (near atomic operations) counts as both a write access and read access. Each access to a cache line is counted including the multiple accesses caused by single instructions such as LDM or STM. Each access to other level 1 data or unified memory structures, for example refill buffers, write buffers, and write-back buffers, are also counted\000"
/* offset=111589 */ "l1d_cache_wb\000l1d_cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000Counts write-backs of dirty data from the L1 data cache to the L2 cache. This occurs when either a dirty cache line is evicted from L1 data cache and allocated in the L2 cache or dirty data is written to the L2 and possibly to the next level of cache. This event counts both victim cache line evictions and cache write-backs from snoops or cache maintenance operations. The following cache operations are not counted:\n\n1. Invalidations which do not result in data being transferred out of the L1 (such as evictions of clean data),\n2. Full line writes which write to L2 without writing L1, such as write streaming mode\000"
/* offset=112288 */ "l1d_cache_rd\000l1d_cache\000L1D cache access, read\000event=0x40\000\00000\000Counts level 1 data cache accesses from any load operation. Atomic load operations that resolve in the CPUs caches counts as both a write access and read access\000"
/* offset=112510 */ "l1d_cache_wr\000l1d_cache\000L1D cache access, write\000event=0x41\000\00000\000Counts level 1 data cache accesses generated by store operations. This event also counts accesses caused by a DC ZVA (data cache zero, specified by virtual address) instruction. Near atomic operations that resolve in the CPUs caches count as a write access and read access\000"
/* offset=112845 */ "l1d_cache_refill_rd\000l1d_cache\000L1D cache refill, read\000event=0x42\000\00000\000Counts level 1 data cache refills caused by speculatively executed load instructions where the memory read operation misses in the level 1 data cache. This event only counts one event per cache line\000"
/* offset=113112 */ "l1d_cache_refill_wr\000l1d_cache\000L1D cache refill, write\000event=0x43\000\00000\000Counts level 1 data cache refills caused by speculatively executed store instructions where the memory write operation misses in the level 1 data cache. This event only counts one event per cache line\000"
/* offset=113382 */ "l1d_cache_refill_inner\000l1d_cache\000L1D cache refill, inner\000event=0x44\000\00000\000Counts level 1 data cache refills where the cache line data came from caches inside the immediate cluster of the core\000"
/* offset=113572 */ "l1d_cache_refill_outer\000l1d_cache\000L1D cache refill, outer\000event=0x45\000\00000\000Counts level 1 data cache refills for which the cache line data came from outside the immediate cluster of the core, like an SLC in the system interconnect or DRAM\000"
/* offset=113808 */ "l1d_cache_wb_victim\000l1d_cache\000L1D cache Write-Back, victim\000event=0x46\000\00000\000Counts dirty cache line evictions from the level 1 data cache caused by a new cache line allocation. This event does not count evictions caused by cache maintenance operations\000"
/* offset=114058 */ "l1d_cache_wb_clean\000l1d_cache\000L1D cache Write-Back, cleaning and coherency\000event=0x47\000\00000\000Counts write-backs from the level 1 data cache that are a result of a coherency operation made by another CPU. Event count includes cache maintenance operations\000"
/* offset=114308 */ "l1d_cache_inval\000l1d_cache\000L1D cache invalidate\000event=0x48\000\00000\000Counts each explicit invalidation of a cache line in the level 1 data cache caused by:\n\n- Cache Maintenance Operations (CMO) that operate by a virtual address.\n- Broadcast cache coherency operations from another CPU in the system.\n\nThis event does not count for the following conditions:\n\n1. A cache refill invalidates a cache line.\n2. A CMO which is executed on that CPU and invalidates a cache line specified by set/way.\n\nNote that CMOs that operate by set/way cannot be broadcast from one CPU to another\000"
/* offset=114877 */ "l1i_cache_refill\000l1i_cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Counts cache line refills in the level 1 instruction cache caused by a missed instruction fetch. Instruction fetches may include accessing multiple instructions, but the single cache line allocation is counted once\000"
/* offset=115166 */ "l1i_cache\000l1i_cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Counts instruction fetches which access the level 1 instruction cache. Instruction cache accesses caused by cache maintenance operations are not counted\000"
/* offset=115400 */ "l2d_cache\000l2_cache\000Level 2 data cache access\000event=0x16\000\00000\000Counts level 2 cache accesses. level 2 cache is a unified cache for data and instruction accesses. Accesses are for misses in the first level caches or translation resolutions due to accesses. This event also counts write back of dirty data from level 1 data cache to the L2 cache\000"
/* offset=115741 */ "l2d_cache_refill\000l2_cache\000Level 2 data refill\000event=0x17\000\00000\000Counts cache line refills into the level 2 cache. level 2 cache is a unified cache for data and instruction accesses. Accesses are for misses in the level 1 caches or translation resolutions due to accesses\000"
/* offset=116009 */ "l2d_cache_wb\000l2_cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Counts write-backs of data from the L2 cache to outside the CPU. This includes snoops to the L2 (from other CPUs) which return data even if the snoops cause an invalidation. L2 cache line invalidations which do not write data outside the CPU and snoops which return data from an L1 cache are not counted. Data would not be written outside the cache when invalidating a clean cache line\000"
/* offset=116475 */ "l2d_cache_allocate\000l2_cache\000Attributable Level 2 data cache allocation without refill\000event=0x20\000\00000\000TBD\000"
/* offset=116580 */ "l2d_cache_rd\000l2_cache\000L2D cache access, read\000event=0x50\000\00000\000Counts level 2 cache accesses due to memory read operations. level 2 cache is a unified cache for data and instruction accesses, accesses are for misses in the level 1 caches or translation resolutions due to accesses\000"
/* offset=116858 */ "l2d_cache_wr\000l2_cache\000L2D cache access, write\000event=0x51\000\00000\000Counts level 2 cache accesses due to memory write operations. level 2 cache is a unified cache for data and instruction accesses, accesses are for misses in the level 1 caches or translation resolutions due to accesses\000"
/* offset=117138 */ "l2d_cache_refill_rd\000l2_cache\000L2D cache refill, read\000event=0x52\000\00000\000Counts refills for memory accesses due to memory read operation counted by L2D_CACHE_RD. level 2 cache is a unified cache for data and instruction accesses, accesses are for misses in the level 1 caches or translation resolutions due to accesses\000"
/* offset=117451 */ "l2d_cache_refill_wr\000l2_cache\000L2D cache refill, write\000event=0x53\000\00000\000Counts refills for memory accesses due to memory write operation counted by L2D_CACHE_WR. level 2 cache is a unified cache for data and instruction accesses, accesses are for misses in the level 1 caches or translation resolutions due to accesses\000"
/* offset=117766 */ "l2d_cache_wb_victim\000l2_cache\000L2D cache Write-Back, victim\000event=0x56\000\00000\000Counts evictions from the level 2 cache because of a line being allocated into the L2 cache\000"
/* offset=117931 */ "l2d_cache_wb_clean\000l2_cache\000L2D cache Write-Back, cleaning and coherency\000event=0x57\000\00000\000Counts write-backs from the level 2 cache that are a result of either:\n\n1. Cache maintenance operations,\n\n2. Snoop responses or,\n\n3. Direct cache transfers to another CPU due to a forwarding snoop request\000"
/* offset=118224 */ "l2d_cache_inval\000l2_cache\000L2D cache invalidate\000event=0x58\000\00000\000Counts each explicit invalidation of a cache line in the level 2 cache by cache maintenance operations that operate by a virtual address, or by external coherency operations. This event does not count if either:\n\n1. A cache refill invalidates a cache line or,\n2. A Cache Maintenance Operation (CMO), which invalidates a cache line specified by set/way, is executed on that CPU.\n\nCMOs that operate by set/way cannot be broadcast from one CPU to another\000"
/* offset=118737 */ "l3d_cache_allocate\000l3_cache\000Attributable Level 3 data cache allocation without refill\000event=0x29\000\00000\000Counts level 3 cache line allocates that do not fetch data from outside the level 3 data or unified cache. For example, allocates due to streaming stores\000"
/* offset=118992 */ "l3d_cache_refill\000l3_cache\000Attributable Level 3 data cache refill\000event=0x2a\000\00000\000Counts level 3 accesses that receive data from outside the L3 cache\000"
/* offset=119140 */ "l3d_cache\000l3_cache\000Attributable Level 3 data cache access\000event=0x2b\000\00000\000Counts level 3 cache accesses. level 3 cache is a unified cache for data and instruction accesses. Accesses are for misses in the lower level caches or translation resolutions due to accesses\000"
/* offset=119405 */ "l3d_cache_rd\000l3_cache\000Attributable Level 3 data or unified cache access, read\000event=0xa0\000\00000\000TBD\000"
/* offset=119502 */ "ll_cache_rd\000ll_cache\000Attributable Last level cache memory read\000event=0x36\000\00000\000Counts read transactions that were returned from outside the core cluster. This event counts when the system register CPUECTLR.EXTLLC bit is set. This event counts read transactions returned from outside the core if those transactions are either hit in the system level cache or missed in the SLC and are returned from any other external sources\000"
/* offset=119926 */ "ll_cache_miss_rd\000ll_cache\000Last level cache miss, read\000event=0x37\000\00000\000Counts read transactions that were returned from outside the core cluster but missed in the system level cache. This event counts when the system register CPUECTLR.EXTLLC bit is set. This event counts read transactions returned from outside the core if those transactions are missed in the System level Cache. The data source of the transaction is indicated by a field in the CHI transaction returning to the CPU. This event does not count reads caused by cache maintenance operations\000"
/* offset=120480 */ "mem_access\000memory\000Data memory access\000event=0x13\000\00000\000Counts memory accesses issued by the CPU load store unit, where those accesses are issued due to load or store operations. This event counts memory accesses no matter whether the data is received from any level of cache hierarchy or external memory. If memory accesses are broken up into smaller transactions than what were specified in the load or store instructions, then the event counts those smaller memory transactions\000"
/* offset=120957 */ "memory_error\000memory\000Local memory error\000event=0x1a\000\00000\000Counts any detected correctable or uncorrectable physical memory errors (ECC or parity) in protected CPUs RAMs. On the core, this event counts errors in the caches (including data and tag rams). Any detected memory error (from either a speculative and abandoned access, or an architecturally executed access) is counted. Note that errors are only detected when the actual protected memory is accessed by an operation\000"
/* offset=121428 */ "remote_access\000memory\000Access to another socket in a multi-socket system\000event=0x31\000\00000\000Counts accesses to another chip, which is implemented as a different CMN mesh in the system. If the CHI bus response back to the core indicates that the data source is from another chip (mesh), then the counter is updated. If no data is returned, even if the system snoops another chip/mesh, then the counter is not updated\000"
/* offset=121838 */ "mem_access_rd\000memory\000Data memory access, read\000event=0x66\000\00000\000Counts memory accesses issued by the CPU due to load operations. The event counts any memory load access, no matter whether the data is received from any level of cache hierarchy or external memory. The event also counts atomic load operations. If memory accesses are broken up by the load/store unit into smaller transactions that are issued by the bus interface, then the event counts those smaller transactions\000"
/* offset=122313 */ "mem_access_wr\000memory\000Data memory access, write\000event=0x67\000\00000\000Counts memory accesses issued by the CPU due to store operations. The event counts any memory store access, no matter whether the data is located in any level of cache or external memory. The event also counts atomic load and store operations. If memory accesses are broken up by the load/store unit into smaller transactions that are issued by the bus interface, then the event counts those smaller transactions\000"
/* offset=122788 */ "sw_incr\000retired\000Instruction architecturally executed, Condition code check pass, software increment\000event=0\000\00000\000Counts software writes to the PMSWINC_EL0 (software PMU increment) register. The PMSWINC_EL0 register is a manually updated counter for use by application software.\n\nThis event could be used to measure any user program event, such as accesses to a particular data structure (by writing to the PMSWINC_EL0 register each time the data structure is accessed).\n\nTo use the PMSWINC_EL0 register and event, developers must insert instructions that write to the PMSWINC_EL0 register into the source code.\n\nSince the SW_INCR event records writes to the PMSWINC_EL0 register, there is no need to do a read/increment/write sequence to the PMSWINC_EL0 register\000"
/* offset=123550 */ "inst_retired\000retired\000Instruction architecturally executed\000event=0x8\000\00000\000Counts instructions that have been architecturally executed\000"
/* offset=123682 */ "cid_write_retired\000retired\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000Counts architecturally executed writes to the CONTEXTIDR register, which usually contain the kernel PID and can be output with hardware trace\000"
/* offset=123949 */ "ttbr_write_retired\000retired\000Instruction architecturally executed, Condition code check pass, write to TTBR\000event=0x1c\000\00000\000Counts architectural writes to TTBR0/1_EL1. If virtualization host extensions are enabled (by setting the HCR_EL2.E2H bit to 1), then accesses to TTBR0/1_EL1 that are redirected to TTBR0/1_EL2, or accesses to TTBR0/1_EL12, are counted. TTBRn registers are typically updated when the kernel is swapping user-space threads or applications\000"
/* offset=124407 */ "br_retired\000retired\000Instruction architecturally executed, branch\000event=0x21\000\00000\000Counts architecturally executed branches, whether the branch is taken or not. Instructions that explicitly write to the PC are also counted\000"
/* offset=124626 */ "br_mis_pred_retired\000retired\000Instruction architecturally executed, mispredicted branch\000event=0x22\000\00000\000Counts branches counted by BR_RETIRED which were mispredicted and caused a pipeline flush\000"
/* offset=124817 */ "sample_pop\000spe\000Sample Population\000event=0x4000\000\00000\000Counts statistical profiling sample population, the count of all operations that could be sampled but may or may not be chosen for sampling\000"
/* offset=125007 */ "sample_feed\000spe\000Sample Taken\000event=0x4001\000\00000\000Counts statistical profiling samples taken for sampling\000"
/* offset=125109 */ "sample_filtrate\000spe\000Sample Taken and not removed by filtering\000event=0x4002\000\00000\000Counts statistical profiling samples taken which are not removed by filtering\000"
/* offset=125266 */ "sample_collision\000spe\000Sample collided with previous sample\000event=0x4003\000\00000\000Counts statistical profiling samples that have collided with a previous sample and so therefore not taken\000"
/* offset=125447 */ "br_mis_pred\000spec_operation\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000Counts branches which are speculatively executed and mispredicted\000"
/* offset=125615 */ "br_pred\000spec_operation\000Predictable branch speculatively executed\000event=0x12\000\00000\000Counts branches speculatively executed and were predicted right\000"
/* offset=125759 */ "inst_spec\000spec_operation\000Operation speculatively executed\000event=0x1b\000\00000\000Counts operations that have been speculatively executed\000"
/* offset=125888 */ "unaligned_ld_spec\000spec_operation\000Unaligned access, read\000event=0x68\000\00000\000Counts unaligned memory read operations issued by the CPU. This event counts unaligned accesses (as defined by the actual instruction), even if they are subsequently issued as multiple aligned accesses. The event does not count preload operations (PLD, PLI)\000"
/* offset=126217 */ "unaligned_st_spec\000spec_operation\000Unaligned access, write\000event=0x69\000\00000\000Counts unaligned memory write operations issued by the CPU. This event counts unaligned accesses (as defined by the actual instruction), even if they are subsequently issued as multiple aligned accesses\000"
/* offset=126492 */ "unaligned_ldst_spec\000spec_operation\000Unaligned access\000event=0x6a\000\00000\000Counts unaligned memory operations issued by the CPU. This event counts unaligned accesses (as defined by the actual instruction), even if they are subsequently issued as multiple aligned accesses\000"
/* offset=126756 */ "ldrex_spec\000spec_operation\000Exclusive operation speculatively executed, LDREX or LDX\000event=0x6c\000\00000\000Counts Load-Exclusive operations that have been speculatively executed. Eg: LDREX, LDX\000"
/* offset=126941 */ "strex_pass_spec\000spec_operation\000Exclusive operation speculatively executed, STREX or STX pass\000event=0x6d\000\00000\000Counts store-exclusive operations that have been speculatively executed and have successfully completed the store operation\000"
/* offset=127173 */ "strex_fail_spec\000spec_operation\000Exclusive operation speculatively executed, STREX or STX fail\000event=0x6e\000\00000\000Counts store-exclusive operations that have been speculatively executed and have not successfully completed the store operation\000"
/* offset=127409 */ "strex_spec\000spec_operation\000Exclusive operation speculatively executed, STREX or STX\000event=0x6f\000\00000\000Counts store-exclusive operations that have been speculatively executed\000"
/* offset=127579 */ "ld_spec\000spec_operation\000Operation speculatively executed, load\000event=0x70\000\00000\000Counts speculatively executed load operations including Single Instruction Multiple Data (SIMD) load operations\000"
/* offset=127768 */ "st_spec\000spec_operation\000Operation speculatively executed, store\000event=0x71\000\00000\000Counts speculatively executed store operations including Single Instruction Multiple Data (SIMD) store operations\000"
/* offset=127960 */ "dp_spec\000spec_operation\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Counts speculatively executed logical or arithmetic instructions such as MOV/MVN operations\000"
/* offset=128148 */ "ase_spec\000spec_operation\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Counts speculatively executed Advanced SIMD operations excluding load, store and move micro-operations that move data to or from SIMD (vector) registers\000"
/* offset=128400 */ "vfp_spec\000spec_operation\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Counts speculatively executed floating point operations. This event does not count operations that move data to or from floating point (vector) registers\000"
/* offset=128654 */ "pc_write_spec\000spec_operation\000Operation speculatively executed, software change of the PC\000event=0x76\000\00000\000Counts speculatively executed operations which cause software changes of the PC. Those operations include all taken branch operations\000"
/* offset=128892 */ "crypto_spec\000spec_operation\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Counts speculatively executed cryptographic operations except for PMULL and VMULL operations\000"
/* offset=129087 */ "br_immed_spec\000spec_operation\000Branch speculatively executed, immediate branch\000event=0x78\000\00000\000Counts immediate branch operations which are speculatively executed\000"
/* offset=129247 */ "br_return_spec\000spec_operation\000Branch speculatively executed, procedure return\000event=0x79\000\00000\000Counts procedure return operations (RET) which are speculatively executed\000"
/* offset=129414 */ "br_indirect_spec\000spec_operation\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Counts indirect branch operations including procedure returns, which are speculatively executed. This includes operations that force a software change of the PC, other than exception-generating operations.  Eg: BR Xn, RET\000"
/* offset=129730 */ "isb_spec\000spec_operation\000Barrier speculatively executed, ISB\000event=0x7c\000\00000\000Counts ISB operations that are executed\000"
/* offset=129845 */ "dsb_spec\000spec_operation\000Barrier speculatively executed, DSB\000event=0x7d\000\00000\000Counts DSB operations that are speculatively issued to Load/Store unit in the CPU\000"
/* offset=130002 */ "dmb_spec\000spec_operation\000Barrier speculatively executed, DMB\000event=0x7e\000\00000\000Counts DMB operations that are speculatively issued to the Load/Store unit in the CPU. This event does not count implied barriers from load acquire/store release operations\000"
/* offset=130250 */ "rc_ld_spec\000spec_operation\000Release consistency operation speculatively executed, Load-Acquire\000event=0x90\000\00000\000Counts any load acquire operations that are speculatively executed. Eg: LDAR, LDARH, LDARB\000"
/* offset=130449 */ "rc_st_spec\000spec_operation\000Release consistency operation speculatively executed, Store-Release\000event=0x91\000\00000\000Counts any store release operations that are speculatively executed. Eg: STLR, STLRH, STLRB'\000"
/* offset=130651 */ "stall_frontend\000stall\000No operation issued because of the frontend\000event=0x23\000\00000\000Counts cycles when frontend could not send any micro-operations to the rename stage because of frontend resource stalls caused by fetch memory latency or branch prediction flow stalls. All the frontend slots were empty during the cycle when this event counts\000"
/* offset=130990 */ "stall_backend\000stall\000No operation issued due to the backend\000event=0x24\000\00000\000Counts cycles whenever the rename unit is unable to send any micro-operations to the backend of the pipeline because of backend resource constraints. Backend resource constraints can include issue stage fullness, execution stage fullness, or other internal pipeline resource fullness. All the backend slots were empty during the cycle when this event counts\000"
/* offset=131422 */ "l1i_tlb_refill\000tlb\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Counts level 1 instruction TLB refills from any Instruction fetch. If there are multiple misses in the TLB that are resolved by the refill, then this event only counts once. This event will not count if the translation table walk results in a fault (such as a translation or access fault), since there is no new translation created for the TLB\000"
/* offset=131843 */ "l1d_tlb_refill\000tlb\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Counts level 1 data TLB accesses that resulted in TLB refills. If there are multiple misses in the TLB that are resolved by the refill, then this event only counts once. This event counts for refills caused by preload instructions or hardware prefetch accesses. This event counts regardless of whether the miss hits in L2 or results in a translation table walk. This event will not count if the translation table walk results in a fault (such as a translation or access fault), since there is no new translation created for the TLB. This event will not count on an access from an AT(address translation) instruction\000"
/* offset=132529 */ "l1d_tlb\000tlb\000Attributable Level 1 data or unified TLB access\000event=0x25\000\00000\000Counts level 1 data TLB accesses caused by any memory load or store operation. Note that load or store instructions can be broken up into multiple memory operations. This event does not count TLB maintenance operations\000"
/* offset=132823 */ "l1i_tlb\000tlb\000Attributable Level 1 instruction TLB access\000event=0x26\000\00000\000Counts level 1 instruction TLB accesses, whether the access hits or misses in the TLB. This event counts both demand accesses and prefetch or preload generated accesses\000"
/* offset=133063 */ "l2d_tlb_refill\000tlb\000Attributable Level 2 data TLB refill\000event=0x2d\000\00000\000Counts level 2 TLB refills caused by memory operations from both data and instruction fetch, except for those caused by TLB maintenance operations and hardware prefetches\000"
/* offset=133305 */ "l2d_tlb\000tlb\000Attributable Level 2 data or unified TLB access\000event=0x2f\000\00000\000Counts level 2 TLB accesses except those caused by TLB maintenance operations\000"
/* offset=133458 */ "dtlb_walk\000tlb\000Access to data TLB causes a translation table walk\000event=0x34\000\00000\000Counts data memory translation table walks caused by a miss in the L2 TLB driven by a memory access. Note that partial translations that also cause a table walk are counted. This event does not count table walks caused by TLB maintenance operations\000"
/* offset=133787 */ "itlb_walk\000tlb\000Access to instruction TLB that causes a translation table walk\000event=0x35\000\00000\000Counts instruction memory translation table walks caused by a miss in the L2 TLB driven by a memory access. Partial translations that also cause a table walk are counted. This event does not count table walks caused by TLB maintenance operations\000"
/* offset=134125 */ "l1d_tlb_refill_rd\000tlb\000L1D tlb refill, read\000event=0x4c\000\00000\000Counts level 1 data TLB refills caused by memory read operations. If there are multiple misses in the TLB that are resolved by the refill, then this event only counts once. This event counts for refills caused by preload instructions or hardware prefetch accesses. This event counts regardless of whether the miss hits in L2 or results in a translation table walk. This event will not count if the translation table walk results in a fault (such as a translation or access fault), since there is no new translation created for the TLB. This event will not count on an access from an Address Translation (AT) instruction\000"
/* offset=134803 */ "l1d_tlb_refill_wr\000tlb\000L1D tlb refill, write\000event=0x4d\000\00000\000Counts level 1 data TLB refills caused by data side memory write operations. If there are multiple misses in the TLB that are resolved by the refill, then this event only counts once. This event counts for refills caused by preload instructions or hardware prefetch accesses. This event counts regardless of whether the miss hits in L2 or results in a translation table walk. This event will not count if the table walk results in a fault (such as a translation or access fault), since there is no new translation created for the TLB. This event will not count with an access from an Address Translation (AT) instruction\000"
/* offset=135483 */ "l1d_tlb_rd\000tlb\000L1D tlb access, read\000event=0x4e\000\00000\000Counts level 1 data TLB accesses caused by memory read operations. This event counts whether the access hits or misses in the TLB. This event does not count TLB maintenance operations\000"
/* offset=135718 */ "l1d_tlb_wr\000tlb\000L1D tlb access, write\000event=0x4f\000\00000\000Counts any L1 data side TLB accesses caused by memory write operations. This event counts whether the access hits or misses in the TLB. This event does not count TLB maintenance operations\000"
/* offset=135959 */ "l2d_tlb_refill_rd\000tlb\000L2D cache refill, read\000event=0x5c\000\00000\000Counts level 2 TLB refills caused by memory read operations from both data and instruction fetch except for those caused by TLB maintenance operations or hardware prefetches\000"
/* offset=136193 */ "l2d_tlb_refill_wr\000tlb\000L2D cache refill, write\000event=0x5d\000\00000\000Counts level 2 TLB refills caused by memory write operations from both data and instruction fetch except for those caused by TLB maintenance operations\000"
/* offset=136406 */ "l2d_tlb_rd\000tlb\000L2D cache access, read\000event=0x5e\000\00000\000Counts level 2 TLB accesses caused by memory read operations from both data and instruction fetch except for those caused by TLB maintenance operations\000"
/* offset=136611 */ "l2d_tlb_wr\000tlb\000L2D cache access, write\000event=0x5f\000\00000\000Counts level 2 TLB accesses caused by memory write operations from both data and instruction fetch except for those caused by TLB maintenance operations\000"
/* offset=136818 */ "fp_hp_spec\000fp_operation\000Floating-point half-precision operations speculatively executed\000event=0x8014\000\00000\000Counts speculatively executed half precision floating point operations\000"
/* offset=136994 */ "fp_sp_spec\000fp_operation\000Floating-point single-precision operations speculatively executed\000event=0x8018\000\00000\000Counts speculatively executed single precision floating point operations\000"
/* offset=137174 */ "fp_dp_spec\000fp_operation\000Floating-point double-precision operations speculatively executed\000event=0x801c\000\00000\000Counts speculatively executed double precision floating point operations\000"
/* offset=137354 */ "fp_scale_ops_spec\000fp_operation\000Scalable floating-point element Operations speculatively executed\000event=0x80c0\000\00000\000Counts speculatively executed scalable single precision floating point operations\000"
/* offset=137550 */ "fp_fixed_ops_spec\000fp_operation\000Non-scalable floating-point element Operations speculatively executed\000event=0x80c1\000\00000\000Counts speculatively executed non-scalable single precision floating point operations\000"
/* offset=137754 */ "cnt_cycles\000general\000Constant frequency cycles\000event=0x4004\000\00000\000Counts constant frequency cycles\000"
/* offset=137849 */ "l1d_cache\000l1d_cache\000Level 1 data cache access\000event=0x4\000\00000\000Counts level 1 data cache accesses from any load/store operations. Atomic operations that resolve in the CPUs caches (near atomic operations) count as both a write access and read access. Each access to a cache line is counted including the multiple accesses caused by single instructions such as LDM or STM. Each access to other level 1 data or unified memory structures, for example refill buffers, write buffers, and write-back buffers, are also counted\000"
/* offset=138366 */ "l1d_cache_lmiss_rd\000l1d_cache\000Level 1 data cache long-latency read miss\000event=0x39\000\00000\000Counts cache line refills into the level 1 data cache from any memory read operations, that incurred additional latency\000"
/* offset=138572 */ "l1d_cache_rd\000l1d_cache\000L1D cache access, read\000event=0x40\000\00000\000Counts level 1 data cache accesses from any load operation. Atomic load operations that resolve in the CPUs caches count as both a write access and read access\000"
/* offset=138793 */ "l1i_cache_lmiss\000l1i_cache\000Level 1 instruction cache long-latency read miss\000event=0x4006\000\00000\000Counts cache line refills into the level 1 instruction cache, that incurred additional latency\000"
/* offset=138980 */ "l2d_cache_lmiss_rd\000l2_cache\000Level 2 data cache long-latency read miss\000event=0x4009\000\00000\000Counts cache line refills into the level 2 unified cache from any memory read operations that incurred additional latency\000"
/* offset=139189 */ "l3d_cache_lmiss_rd\000l3_cache\000Level 3 data cache long-latency read miss\000event=0x400b\000\00000\000Counts any cache line refill into the level 3 cache from memory read operations that incurred additional latency\000"
/* offset=139389 */ "ldst_align_lat\000memory\000Access with additional latency from alignment\000event=0x4020\000\00000\000Counts the number of memory read and write accesses in a cycle that incurred additional latency, due to the alignment of the address and the size of data being accessed, which results in store crossing a single cache line\000"
/* offset=139696 */ "ld_align_lat\000memory\000Load with additional latency from alignment\000event=0x4021\000\00000\000Counts the number of memory read accesses in a cycle that incurred additional latency, due to the alignment of the address and size of data being accessed, which results in load crossing a single cache line\000"
/* offset=139984 */ "st_align_lat\000memory\000Store with additional latency from alignment\000event=0x4022\000\00000\000Counts the number of memory write access in a cycle that incurred additional latency, due to the alignment of the address and size of data being accessed incurred additional latency\000"
/* offset=140248 */ "mem_access_checked\000memory\000Checked data memory access\000event=0x4024\000\00000\000Counts the number of memory read and write accesses in a cycle that are tag checked by the Memory Tagging Extension (MTE)\000"
/* offset=140440 */ "mem_access_checked_rd\000memory\000Checked data memory access, read\000event=0x4025\000\00000\000Counts the number of memory read accesses in a cycle that are tag checked by the Memory Tagging Extension (MTE)\000"
/* offset=140631 */ "mem_access_checked_wr\000memory\000Checked data memory access, write\000event=0x4026\000\00000\000Counts the number of memory write accesses in a cycle that is tag checked by the Memory Tagging Extension (MTE)\000"
/* offset=140823 */ "op_retired\000retired\000Micro-operation architecturally executed\000event=0x3a\000\00000\000Counts micro-operations that are architecturally executed. This is a count of number of micro-operations retired from the commit queue in a single cycle\000"
/* offset=141051 */ "op_spec\000spec_operation\000Micro-operation speculatively executed\000event=0x3b\000\00000\000Counts micro-operations speculatively executed. This is the count of the number of micro-operations dispatched in a cycle\000"
/* offset=141250 */ "ase_inst_spec\000spec_operation\000ASE operations speculatively executed\000event=0x8005\000\00000\000Counts speculatively executed Advanced SIMD operations\000"
/* offset=141389 */ "stall\000stall\000No operation sent for execution\000event=0x3c\000\00000\000Counts cycles when no operations are sent to the rename unit from the frontend or from the rename unit to the backend for any reason (either frontend or backend stall)\000"
/* offset=141616 */ "stall_slot_backend\000stall\000No operation sent for execution on a slot due to the backend\000event=0x3d\000\00000\000Counts slots per cycle in which no operations are sent from the rename unit to the backend due to backend resource constraints\000"
/* offset=141844 */ "stall_slot_frontend\000stall\000No operation sent for execution on a slot due to the frontend\000event=0x3e\000\00000\000Counts slots per cycle in which no operations are sent to the rename unit from the frontend due to frontend resource constraints\000"
/* offset=142076 */ "stall_slot\000stall\000No operation sent for execution on a slot\000event=0x3f\000\00000\000Counts slots per cycle in which no operations are sent to the rename unit from the frontend or from the rename unit to the backend for any reason (either frontend or backend stall)\000"
/* offset=142331 */ "stall_backend_mem\000stall\000Memory stall cycles\000event=0x4005\000\00000\000Counts cycles when the backend is stalled because there is a pending demand load request in progress in the last level core cache\000"
/* offset=142522 */ "sve_inst_spec\000sve\000SVE operations speculatively executed\000event=0x8006\000\00000\000Counts speculatively executed operations that are SVE operations\000"
/* offset=142660 */ "sve_pred_spec\000sve\000SVE predicated Operations speculatively executed\000event=0x8074\000\00000\000Counts speculatively executed predicated SVE operations\000"
/* offset=142800 */ "sve_pred_empty_spec\000sve\000SVE predicated operations with no active predicates speculatively executed\000event=0x8075\000\00000\000Counts speculatively executed predicated SVE operations with no active predicate elements\000"
/* offset=143006 */ "sve_pred_full_spec\000sve\000SVE predicated operations speculatively executed with all active predicates\000event=0x8076\000\00000\000Counts speculatively executed predicated SVE operations with all predicate elements active\000"
/* offset=143213 */ "sve_pred_partial_spec\000sve\000SVE predicated operations speculatively executed with partially active predicates\000event=0x8077\000\00000\000Counts speculatively executed predicated SVE operations with at least one but not all active predicate elements\000"
/* offset=143450 */ "sve_pred_not_full_spec\000sve\000SVE predicated operations with empty or partially active predicates\000event=0x8079\000\00000\000Counts speculatively executed predicated SVE operations with at least one non active predicate elements\000"
/* offset=143666 */ "sve_ldff_spec\000sve\000SVE First-fault load Operations speculatively executed\000event=0x80bc\000\00000\000Counts speculatively executed SVE first fault or non-fault load operations\000"
/* offset=143831 */ "sve_ldff_fault_spec\000sve\000SVE first-fault load operations speculatively executed which set FFR bit to 0\000event=0x80bd\000\00000\000Counts speculatively executed SVE first fault or non-fault load operations that clear at least one bit in the FFR\000"
/* offset=144064 */ "ase_sve_int8_spec\000sve\000Advanced SIMD and SVE 8-bit integer operations speculatively executed\000event=0x80e3\000\00000\000Counts speculatively executed Advanced SIMD or SVE integer operations with the largest data type an 8-bit integer\000"
/* offset=144287 */ "ase_sve_int16_spec\000sve\000Advanced SIMD and SVE 16-bit integer operations speculatively executed\000event=0x80e7\000\00000\000Counts speculatively executed Advanced SIMD or SVE integer operations with the largest data type a 16-bit integer\000"
/* offset=144512 */ "ase_sve_int32_spec\000sve\000Advanced SIMD and SVE 32-bit integer operations speculatively executed\000event=0x80eb\000\00000\000Counts speculatively executed Advanced SIMD or SVE integer operations with the largest data type a 32-bit integer\000"
/* offset=144737 */ "ase_sve_int64_spec\000sve\000Advanced SIMD and SVE 64-bit integer operations speculatively executed\000event=0x80ef\000\00000\000Counts speculatively executed Advanced SIMD or SVE integer operations with the largest data type a 64-bit integer\000"
/* offset=144962 */ "trb_wrap\000trace\000Trace buffer current write pointer wrapped\000event=0x400c\000\00000\000This event is generated each time the current write pointer is wrapped to the base pointer\000"
/* offset=145128 */ "trcextout0\000trace\000PE Trace Unit external output 0\000event=0x4010\000\00000\000This event is generated each time an event is signaled by ETE external event 0\000"
/* offset=145273 */ "trcextout1\000trace\000PE Trace Unit external output 1\000event=0x4011\000\00000\000This event is generated each time an event is signaled by ETE external event 1\000"
/* offset=145418 */ "trcextout2\000trace\000PE Trace Unit external output 2\000event=0x4012\000\00000\000This event is generated each time an event is signaled by ETE external event 2\000"
/* offset=145563 */ "trcextout3\000trace\000PE Trace Unit external output 3\000event=0x4013\000\00000\000This event is generated each time an event is signaled by ETE external event 3\000"
/* offset=145708 */ "cti_trigout4\000trace\000Cross-trigger Interface output trigger 4\000event=0x4018\000\00000\000This event is generated each time an event is signaled on CTI output trigger 4\000"
/* offset=145864 */ "cti_trigout5\000trace\000Cross-trigger Interface output trigger 5 \000event=0x4019\000\00000\000This event is generated each time an event is signaled on CTI output trigger 5\000"
/* offset=146021 */ "cti_trigout6\000trace\000Cross-trigger Interface output trigger 6\000event=0x401a\000\00000\000This event is generated each time an event is signaled on CTI output trigger 6\000"
/* offset=146177 */ "cti_trigout7\000trace\000Cross-trigger Interface output trigger 7\000event=0x401b\000\00000\000This event is generated each time an event is signaled on CTI output trigger 7\000"
/* offset=146333 */ "l1d_cache_rd\000core imp def\000L1D cache access, read\000event=0x40\000\00000\000Attributable Level 1 data cache access, read\000"
/* offset=146442 */ "l1d_cache_wr\000core imp def\000L1D cache access, write\000event=0x41\000\00000\000Attributable Level 1 data cache access, write\000"
/* offset=146553 */ "l1d_cache_refill_rd\000core imp def\000L1D cache refill, read\000event=0x42\000\00000\000Attributable Level 1 data cache refill, read\000"
/* offset=146669 */ "l1d_cache_refill_wr\000core imp def\000L1D cache refill, write\000event=0x43\000\00000\000Attributable Level 1 data cache refill, write\000"
/* offset=146787 */ "l1d_cache_refill_inner\000core imp def\000L1D cache refill, inner\000event=0x44\000\00000\000Attributable Level 1 data cache refill, inner\000"
/* offset=146908 */ "l1d_cache_refill_outer\000core imp def\000L1D cache refill, outer\000event=0x45\000\00000\000Attributable Level 1 data cache refill, outer\000"
/* offset=147029 */ "l1d_cache_wb_victim\000core imp def\000L1D cache Write-Back, victim\000event=0x46\000\00000\000Attributable Level 1 data cache Write-Back, victim\000"
/* offset=147157 */ "l1d_cache_wb_clean\000core imp def\000L1D cache Write-Back, cleaning and coherency\000event=0x47\000\00000\000Level 1 data cache Write-Back, cleaning and coherency\000"
/* offset=147303 */ "l1d_cache_inval\000core imp def\000L1D cache invalidate\000event=0x48\000\00000\000Attributable Level 1 data cache invalidate\000"
/* offset=147411 */ "l1d_tlb_refill_rd\000core imp def\000L1D tlb refill, read\000event=0x4c\000\00000\000Attributable Level 1 data TLB refill, read\000"
/* offset=147521 */ "l1d_tlb_refill_wr\000core imp def\000L1D tlb refill, write\000event=0x4d\000\00000\000Attributable Level 1 data TLB refill, write\000"
/* offset=147633 */ "l1d_tlb_rd\000core imp def\000L1D tlb access, read\000event=0x4e\000\00000\000Attributable Level 1 data or unified TLB access, read\000"
/* offset=147747 */ "l1d_tlb_wr\000core imp def\000L1D tlb access, write\000event=0x4f\000\00000\000Attributable Level 1 data or unified TLB access, write\000"
/* offset=147863 */ "l2d_tlb_refill_rd\000core imp def\000L2D cache refill, read\000event=0x5c\000\00000\000Attributable Level 2 data or unified TLB refill, read\000"
/* offset=147986 */ "l2d_tlb_refill_wr\000core imp def\000L2D cache refill, write\000event=0x5d\000\00000\000Attributable Level 2 data or unified TLB refill, write\000"
/* offset=148111 */ "l2d_tlb_rd\000core imp def\000L2D cache access, read\000event=0x5e\000\00000\000Attributable Level 2 data or unified TLB access, read\000"
/* offset=148227 */ "l2d_tlb_wr\000core imp def\000L2D cache access, write\000event=0x5f\000\00000\000Attributable Level 2 data or unified TLB access, write\000"
/* offset=148345 */ "bus_access_rd\000core imp def\000Bus access read\000event=0x60\000\00000\000Bus access read\000"
/* offset=148419 */ "bus_access_wr\000core imp def\000Bus access write\000event=0x61\000\00000\000Bus access write\000"
/* offset=148495 */ "mem_access_rd\000core imp def\000Data memory access, read\000event=0x66\000\00000\000Data memory access, read\000"
/* offset=148587 */ "mem_access_wr\000core imp def\000Data memory access, write\000event=0x67\000\00000\000Data memory access, write\000"
/* offset=148681 */ "unaligned_ld_spec\000core imp def\000Unaligned access, read\000event=0x68\000\00000\000Unaligned access, read\000"
/* offset=148773 */ "unaligned_st_spec\000core imp def\000Unaligned access, write\000event=0x69\000\00000\000Unaligned access, write\000"
/* offset=148867 */ "unaligned_ldst_spec\000core imp def\000Unaligned access\000event=0x6a\000\00000\000Unaligned access\000"
/* offset=148949 */ "exc_undef\000core imp def\000Exception taken, Other synchronous\000event=0x81\000\00000\000Exception taken, Other synchronous\000"
/* offset=149057 */ "exc_svc\000core imp def\000Exception taken, Supervisor Call\000event=0x82\000\00000\000Exception taken, Supervisor Call\000"
/* offset=149159 */ "exc_pabort\000core imp def\000Exception taken, Instruction Abort\000event=0x83\000\00000\000Exception taken, Instruction Abort\000"
/* offset=149268 */ "exc_dabort\000core imp def\000Exception taken, Data Abort and SError\000event=0x84\000\00000\000Exception taken, Data Abort and SError\000"
/* offset=149385 */ "exc_irq\000core imp def\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000"
/* offset=149463 */ "exc_fiq\000core imp def\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000"
/* offset=149541 */ "exc_smc\000core imp def\000Exception taken, Secure Monitor Call\000event=0x88\000\00000\000Exception taken, Secure Monitor Call\000"
/* offset=149651 */ "exc_hvc\000core imp def\000Exception taken, Hypervisor Call\000event=0x8a\000\00000\000Exception taken, Hypervisor Call\000"
/* offset=149753 */ "exc_trap_pabort\000core imp def\000Exception taken, Instruction Abort not taken locally\000event=0x8b\000\00000\000Exception taken, Instruction Abort not taken locally\000"
/* offset=149903 */ "exc_trap_dabort\000core imp def\000Exception taken, Data Abort or SError not taken locally\000event=0x8c\000\00000\000Exception taken, Data Abort or SError not taken locally\000"
/* offset=150059 */ "exc_trap_other\000core imp def\000Exception taken, Other traps not taken locally\000event=0x8d\000\00000\000Exception taken, Other traps not taken locally\000"
/* offset=150196 */ "exc_trap_irq\000core imp def\000Exception taken, IRQ not taken locally\000event=0x8e\000\00000\000Exception taken, IRQ not taken locally\000"
/* offset=150315 */ "exc_trap_fiq\000core imp def\000Exception taken, FIQ not taken locally\000event=0x8f\000\00000\000Exception taken, FIQ not taken locally\000"
/* offset=150434 */ "imx8_ddr\000"
/* offset=150443 */ "imx8mm_ddr.cycles\000ddrc\000ddr cycles event\000event=0\000i.MX8MM\00000\000\000"
/* offset=150503 */ "imx8mm_ddr.read_cycles\000ddrc\000ddr read-cycles event\000event=0x2a\000i.MX8MM\00000\000\000"
/* offset=150576 */ "imx8mm_ddr.write_cycles\000ddrc\000ddr write-cycles event\000event=0x2b\000i.MX8MM\00000\000\000"
/* offset=150651 */ "imx8mm_ddr.read\000ddrc\000ddr read event\000event=0x35\000i.MX8MM\00000\000\000"
/* offset=150710 */ "imx8mm_ddr.write\000ddrc\000ddr write event\000event=0x38\000i.MX8MM\00000\000\000"
/* offset=150771 */ "imx8mn_ddr.cycles\000ddrc\000ddr cycles event\000event=0\000i.MX8MN\00000\000\000"
/* offset=150831 */ "imx8mn_ddr.read_cycles\000ddrc\000ddr read-cycles event\000event=0x2a\000i.MX8MN\00000\000\000"
/* offset=150904 */ "imx8mn_ddr.write_cycles\000ddrc\000ddr write-cycles event\000event=0x2b\000i.MX8MN\00000\000\000"
/* offset=150979 */ "imx8mn_ddr.read\000ddrc\000ddr read event\000event=0x35\000i.MX8MN\00000\000\000"
/* offset=151038 */ "imx8mn_ddr.write\000ddrc\000ddr write event\000event=0x38\000i.MX8MN\00000\000\000"
/* offset=151099 */ "imx8mp_ddr.cycles\000ddrc\000ddr cycles event\000event=0\000i.MX8MP\00000\000\000"
/* offset=151159 */ "imx8mp_ddr.read_cycles\000ddrc\000ddr read-cycles event\000event=0x2a\000i.MX8MP\00000\000\000"
/* offset=151232 */ "imx8mp_ddr.write_cycles\000ddrc\000ddr write-cycles event\000event=0x2b\000i.MX8MP\00000\000\000"
/* offset=151307 */ "imx8mp_ddr.read\000ddrc\000ddr read event\000event=0x35\000i.MX8MP\00000\000\000"
/* offset=151366 */ "imx8mp_ddr.write\000ddrc\000ddr write event\000event=0x38\000i.MX8MP\00000\000\000"
/* offset=151427 */ "imx8mq_ddr.cycles\000ddrc\000ddr cycles event\000event=0\000i.MX8MQ\00000\000\000"
/* offset=151487 */ "imx8mq_ddr.read_cycles\000ddrc\000ddr read-cycles event\000event=0x2a\000i.MX8MQ\00000\000\000"
/* offset=151560 */ "imx8mq_ddr.write_cycles\000ddrc\000ddr write-cycles event\000event=0x2b\000i.MX8MQ\00000\000\000"
/* offset=151635 */ "imx8mq_ddr.read\000ddrc\000ddr read event\000event=0x35\000i.MX8MQ\00000\000\000"
/* offset=151694 */ "imx8mq_ddr.write\000ddrc\000ddr write event\000event=0x38\000i.MX8MQ\00000\000\000"
/* offset=151755 */ "ali_drw\000"
/* offset=151763 */ "hif_rd_or_wr\000ali_drw\000A Write or Read Op at HIF interface. The unit is 64B\000config=0\000ali_drw_pmu\00000\000\000"
/* offset=151862 */ "hif_wr\000ali_drw\000A Write Op at HIF interface. The unit is 64B\000config=0x1\000ali_drw_pmu\00000\000\000"
/* offset=151949 */ "hif_rd\000ali_drw\000A Read Op at HIF interface. The unit is 64B\000config=0x2\000ali_drw_pmu\00000\000\000"
/* offset=152035 */ "hif_rmw\000ali_drw\000A Read-Modify-Write Op at HIF interface. The unit is 64B\000config=0x3\000ali_drw_pmu\00000\000\000"
/* offset=152135 */ "hif_hi_pri_rd\000ali_drw\000A high priority Read at HIF interface. The unit is 64B\000config=0x4\000ali_drw_pmu\00000\000\000"
/* offset=152239 */ "dfi_wr_data_cycles\000ali_drw\000A write data cycle at DFI interface (to DRAM)\000config=0x7\000ali_drw_pmu\00000\000\000"
/* offset=152339 */ "dfi_rd_data_cycles\000ali_drw\000A read data cycle at DFI interface (to DRAM)\000config=0x8\000ali_drw_pmu\00000\000\000"
/* offset=152438 */ "hpr_xact_when_critical\000ali_drw\000A high priority read becomes critical\000config=0x9\000ali_drw_pmu\00000\000\000"
/* offset=152534 */ "lpr_xact_when_critical\000ali_drw\000A low priority read becomes critical\000config=0xa\000ali_drw_pmu\00000\000\000"
/* offset=152629 */ "wr_xact_when_critical\000ali_drw\000A write becomes critical\000config=0xb\000ali_drw_pmu\00000\000\000"
/* offset=152711 */ "op_is_activate\000ali_drw\000An Activate(ACT) command to DRAM\000config=0xc\000ali_drw_pmu\00000\000\000"
/* offset=152794 */ "op_is_rd_or_wr\000ali_drw\000A Read or Write CAS command to DRAM\000config=0xd\000ali_drw_pmu\00000\000\000"
/* offset=152880 */ "op_is_rd_activate\000ali_drw\000An Activate(ACT) command for read to DRAM\000config=0xe\000ali_drw_pmu\00000\000\000"
/* offset=152975 */ "op_is_rd\000ali_drw\000A Read CAS command to DRAM\000config=0xf\000ali_drw_pmu\00000\000\000"
/* offset=153046 */ "op_is_wr\000ali_drw\000A Write CAS command to DRAM\000config=0x10\000ali_drw_pmu\00000\000\000"
/* offset=153119 */ "op_is_mwr\000ali_drw\000A Masked Write command to DRAM\000config=0x11\000ali_drw_pmu\00000\000\000"
/* offset=153196 */ "op_is_precharge\000ali_drw\000A Precharge(PRE) command to DRAM\000config=0x12\000ali_drw_pmu\00000\000\000"
/* offset=153281 */ "precharge_for_rdwr\000ali_drw\000A Precharge(PRE) required by read or write\000config=0x13\000ali_drw_pmu\00000\000\000"
/* offset=153379 */ "precharge_for_other\000ali_drw\000A Precharge(PRE) required by other conditions\000config=0x14\000ali_drw_pmu\00000\000\000"
/* offset=153481 */ "rdwr_transitions\000ali_drw\000A read-write turnaround\000config=0x15\000ali_drw_pmu\00000\000\000"
/* offset=153558 */ "write_combine\000ali_drw\000A write combine(merge) in write data buffer\000config=0x16\000ali_drw_pmu\00000\000\000"
/* offset=153652 */ "war_hazard\000ali_drw\000A Write-After-Read hazard\000config=0x17\000ali_drw_pmu\00000\000\000"
/* offset=153725 */ "raw_hazard\000ali_drw\000A Read-After-Write hazard\000config=0x18\000ali_drw_pmu\00000\000\000"
/* offset=153798 */ "waw_hazard\000ali_drw\000A Write-After-Write hazard\000config=0x19\000ali_drw_pmu\00000\000\000"
/* offset=153872 */ "op_is_enter_selfref_rk0\000ali_drw\000Rank0 enters self-refresh(SRE)\000config=0x1a\000ali_drw_pmu\00000\000\000"
/* offset=153963 */ "op_is_enter_selfref_rk1\000ali_drw\000Rank1 enters self-refresh(SRE)\000config=0x1b\000ali_drw_pmu\00000\000\000"
/* offset=154054 */ "op_is_enter_selfref_rk2\000ali_drw\000Rank2 enters self-refresh(SRE)\000config=0x1c\000ali_drw_pmu\00000\000\000"
/* offset=154145 */ "op_is_enter_selfref_rk3\000ali_drw\000Rank3 enters self-refresh(SRE)\000config=0x1d\000ali_drw_pmu\00000\000\000"
/* offset=154236 */ "op_is_enter_powerdown_rk0\000ali_drw\000Rank0 enters power-down(PDE)\000config=0x1e\000ali_drw_pmu\00000\000\000"
/* offset=154327 */ "op_is_enter_powerdown_rk1\000ali_drw\000Rank1 enters power-down(PDE)\000config=0x1f\000ali_drw_pmu\00000\000\000"
/* offset=154418 */ "op_is_enter_powerdown_rk2\000ali_drw\000Rank2 enters power-down(PDE)\000config=0x20\000ali_drw_pmu\00000\000\000"
/* offset=154509 */ "op_is_enter_powerdown_rk3\000ali_drw\000Rank3 enters power-down(PDE)\000config=0x21\000ali_drw_pmu\00000\000\000"
/* offset=154600 */ "selfref_mode_rk0\000ali_drw\000A cycle that Rank0 stays in self-refresh mode\000config=0x26\000ali_drw_pmu\00000\000\000"
/* offset=154699 */ "selfref_mode_rk1\000ali_drw\000A cycle that Rank1 stays in self-refresh mode\000config=0x27\000ali_drw_pmu\00000\000\000"
/* offset=154798 */ "selfref_mode_rk2\000ali_drw\000A cycle that Rank2 stays in self-refresh mode\000config=0x28\000ali_drw_pmu\00000\000\000"
/* offset=154897 */ "selfref_mode_rk3\000ali_drw\000A cycle that Rank3 stays in self-refresh mode\000config=0x29\000ali_drw_pmu\00000\000\000"
/* offset=154996 */ "op_is_refresh\000ali_drw\000An auto-refresh(REF) command to DRAM\000config=0x2a\000ali_drw_pmu\00000\000\000"
/* offset=155083 */ "op_is_crit_ref\000ali_drw\000A critical auto-refresh(REF) command to DRAM\000config=0x2b\000ali_drw_pmu\00000\000\000"
/* offset=155179 */ "op_is_load_mode\000ali_drw\000An MRR or MRW command to DRAM\000config=0x2d\000ali_drw_pmu\00000\000\000"
/* offset=155261 */ "op_is_zqcl\000ali_drw\000A ZQCal command to DRAM\000config=0x2e\000ali_drw_pmu\00000\000\000"
/* offset=155332 */ "visible_window_limit_reached_rd\000ali_drw\000At least one entry in read queue reaches the visible window limit\000config=0x30\000ali_drw_pmu\00000\000\000"
/* offset=155466 */ "visible_window_limit_reached_wr\000ali_drw\000At least one entry in write queue reaches the visible window limit\000config=0x31\000ali_drw_pmu\00000\000\000"
/* offset=155601 */ "op_is_dqsosc_mpc\000ali_drw\000A DQS Oscillator MPC command to DRAM\000config=0x34\000ali_drw_pmu\00000\000\000"
/* offset=155691 */ "op_is_dqsosc_mrr\000ali_drw\000A DQS Oscillator MRR command to DRAM\000config=0x35\000ali_drw_pmu\00000\000\000"
/* offset=155781 */ "op_is_tcr_mrr\000ali_drw\000A Temperature Compensated Refresh(TCR) MRR command to DRAM\000config=0x36\000ali_drw_pmu\00000\000\000"
/* offset=155890 */ "op_is_zqstart\000ali_drw\000A ZQCal Start command to DRAM\000config=0x37\000ali_drw_pmu\00000\000\000"
/* offset=155970 */ "op_is_zqlatch\000ali_drw\000A ZQCal Latch command to DRAM\000config=0x38\000ali_drw_pmu\00000\000\000"
/* offset=156050 */ "chi_txreq\000ali_drw\000A packet at CHI TXREQ interface (request)\000config=0x39\000ali_drw_pmu\00000\000\000"
/* offset=156138 */ "chi_txdat\000ali_drw\000A packet at CHI TXDAT interface (read data)\000config=0x3a\000ali_drw_pmu\00000\000\000"
/* offset=156228 */ "chi_rxdat\000ali_drw\000A packet at CHI RXDAT interface (write data)\000config=0x3b\000ali_drw_pmu\00000\000\000"
/* offset=156319 */ "chi_rxrsp\000ali_drw\000A packet at CHI RXRSP interface\000config=0x3c\000ali_drw_pmu\00000\000\000"
/* offset=156397 */ "tsz_vio\000ali_drw\000A violation detected in TZC\000config=0x3d\000ali_drw_pmu\00000\000\000"
/* offset=156469 */ "ddr_cycles\000ali_drw\000The ddr cycles\000config=0x80\000ali_drw_pmu\00000\000\000"
/* offset=156531 */ "bus_read_total_tofu\000bus\000This event counts read transactions from tofu controller to measured CMG\000event=0x314\000\00000\000This event counts read transactions from tofu controller to measured CMG\000"
/* offset=156717 */ "bus_read_total_pci\000bus\000This event counts read transactions from PCI controller to measured CMG\000event=0x315\000\00000\000This event counts read transactions from PCI controller to measured CMG\000"
/* offset=156900 */ "bus_read_total_mem\000bus\000This event counts read transactions from measured CMG local memory to measured CMG\000event=0x316\000\00000\000This event counts read transactions from measured CMG local memory to measured CMG\000"
/* offset=157105 */ "bus_write_total_cmg0\000bus\000This event counts write transactions from measured CMG to CMG0, if measured CMG is not CMG0\000event=0x318\000\00000\000This event counts write transactions from measured CMG to CMG0, if measured CMG is not CMG0\000"
/* offset=157330 */ "bus_write_total_cmg1\000bus\000This event counts write transactions from measured CMG to CMG1, if measured CMG is not CMG1\000event=0x319\000\00000\000This event counts write transactions from measured CMG to CMG1, if measured CMG is not CMG1\000"
/* offset=157555 */ "bus_write_total_cmg2\000bus\000This event counts write transactions from measured CMG to CMG2, if measured CMG is not CMG2\000event=0x31a\000\00000\000This event counts write transactions from measured CMG to CMG2, if measured CMG is not CMG2\000"
/* offset=157780 */ "bus_write_total_cmg3\000bus\000This event counts write transactions from measured CMG to CMG3, if measured CMG is not CMG3\000event=0x31b\000\00000\000This event counts write transactions from measured CMG to CMG3, if measured CMG is not CMG3\000"
/* offset=158005 */ "bus_write_total_tofu\000bus\000This event counts write transactions from measured CMG to tofu controller\000event=0x31c\000\00000\000This event counts write transactions from measured CMG to tofu controller\000"
/* offset=158194 */ "bus_write_total_pci\000bus\000This event counts write transactions from measured CMG to PCI controller\000event=0x31d\000\00000\000This event counts write transactions from measured CMG to PCI controller\000"
/* offset=158380 */ "bus_write_total_mem\000bus\000This event counts write transactions from measured CMG to measured CMG local memory\000event=0x31e\000\00000\000This event counts write transactions from measured CMG to measured CMG local memory\000"
/* offset=158588 */ "l1d_cache_refill_prf\000cache\000This event counts L1D_CACHE_REFILL caused by software or hardware prefetch\000event=0x49\000\00000\000This event counts L1D_CACHE_REFILL caused by software or hardware prefetch\000"
/* offset=158780 */ "l2d_cache_refill_prf\000cache\000This event counts L2D_CACHE_REFILL caused by software or hardware prefetch\000event=0x59\000\00000\000This event counts L2D_CACHE_REFILL caused by software or hardware prefetch\000"
/* offset=158972 */ "l1d_cache_refill_dm\000cache\000This event counts L1D_CACHE_REFILL caused by demand access\000event=0x200\000\00000\000This event counts L1D_CACHE_REFILL caused by demand access\000"
/* offset=159132 */ "l1d_cache_refill_hwprf\000cache\000This event counts L1D_CACHE_REFILL caused by hardware prefetch\000event=0x202\000\00000\000This event counts L1D_CACHE_REFILL caused by hardware prefetch\000"
/* offset=159303 */ "l1_miss_wait\000cache\000This event counts outstanding L1D cache miss requests per cycle\000event=0x208\000\00000\000This event counts outstanding L1D cache miss requests per cycle\000"
/* offset=159466 */ "l1i_miss_wait\000cache\000This event counts outstanding L1I cache miss requests per cycle\000event=0x209\000\00000\000This event counts outstanding L1I cache miss requests per cycle\000"
/* offset=159630 */ "l2d_cache_refill_dm\000cache\000This event counts L2D_CACHE_REFILL caused by demand access\000event=0x300\000\00000\000This event counts L2D_CACHE_REFILL caused by demand access\000"
/* offset=159790 */ "l2d_cache_refill_hwprf\000cache\000This event counts L2D_CACHE_REFILL caused by hardware prefetch\000event=0x302\000\00000\000This event counts L2D_CACHE_REFILL caused by hardware prefetch\000"
/* offset=159961 */ "l2_miss_wait\000cache\000This event counts outstanding L2 cache miss requests per cycle\000event=0x308\000\00000\000This event counts outstanding L2 cache miss requests per cycle\000"
/* offset=160122 */ "l2_miss_count\000cache\000This event counts the number of times of L2 cache miss\000event=0x309\000\00000\000This event counts the number of times of L2 cache miss\000"
/* offset=160268 */ "l2d_swap_dm\000cache\000This event counts operations where demand access hits an L2 cache refill buffer allocated by software or hardware prefetch\000event=0x325\000\00000\000This event counts operations where demand access hits an L2 cache refill buffer allocated by software or hardware prefetch\000"
/* offset=160548 */ "l2d_cache_mibmch_prf\000cache\000This event counts operations where software or hardware prefetch hits an L2 cache refill buffer allocated by demand access\000event=0x326\000\00000\000This event counts operations where software or hardware prefetch hits an L2 cache refill buffer allocated by demand access\000"
/* offset=160837 */ "l2d_cache_swap_local\000cache\000This event counts operations where demand access hits an L2 cache refill buffer allocated by software or hardware prefetch\000event=0x396\000\00000\000This event counts operations where demand access hits an L2 cache refill buffer allocated by software or hardware prefetch\000"
/* offset=161126 */ "ea_l2\000cache\000This event counts energy consumption per cycle of L2 cache\000event=0x3e0\000\00000\000This event counts energy consumption per cycle of L2 cache\000"
/* offset=161272 */ "cpu_cycles\000cycle\000Cycle\000event=0x11\000\00000\000Cycle\000"
/* offset=161316 */ "dczva_spec\000instruction\000This event counts architecturally executed zero blocking operations due to the 'DC ZVA' instruction\000event=0x9f\000\00000\000This event counts architecturally executed zero blocking operations due to the 'DC ZVA' instruction\000"
/* offset=161554 */ "fp_mv_spec\000instruction\000This event counts architecturally executed floating-point move operations\000event=0x105\000\00000\000This event counts architecturally executed floating-point move operations\000"
/* offset=161741 */ "prd_spec\000instruction\000This event counts architecturally executed operations that using predicate register\000event=0x108\000\00000\000This event counts architecturally executed operations that using predicate register\000"
/* offset=161946 */ "iel_spec\000instruction\000This event counts architecturally executed inter-element manipulation operations\000event=0x109\000\00000\000This event counts architecturally executed inter-element manipulation operations\000"
/* offset=162145 */ "ireg_spec\000instruction\000This event counts architecturally executed inter-register manipulation operations\000event=0x10a\000\00000\000This event counts architecturally executed inter-register manipulation operations\000"
/* offset=162347 */ "fp_ld_spec\000instruction\000This event counts architecturally executed NOSIMD load operations that using SIMD&FP registers\000event=0x112\000\00000\000This event counts architecturally executed NOSIMD load operations that using SIMD&FP registers\000"
/* offset=162576 */ "fp_st_spec\000instruction\000This event counts architecturally executed NOSIMD store operations that using SIMD&FP registers\000event=0x113\000\00000\000This event counts architecturally executed NOSIMD store operations that using SIMD&FP registers\000"
/* offset=162807 */ "bc_ld_spec\000instruction\000This event counts architecturally executed SIMD broadcast floating-point load operations\000event=0x11a\000\00000\000This event counts architecturally executed SIMD broadcast floating-point load operations\000"
/* offset=163024 */ "effective_inst_spec\000instruction\000This event counts architecturally executed instructions, excluding the MOVPRFX instruction\000event=0x121\000\00000\000This event counts architecturally executed instructions, excluding the MOVPRFX instruction\000"
/* offset=163254 */ "pre_index_spec\000instruction\000This event counts architecturally executed operations that uses 'pre-index' as its addressing mode\000event=0x123\000\00000\000This event counts architecturally executed operations that uses 'pre-index' as its addressing mode\000"
/* offset=163495 */ "post_index_spec\000instruction\000This event counts architecturally executed operations that uses 'post-index' as its addressing mode\000event=0x124\000\00000\000This event counts architecturally executed operations that uses 'post-index' as its addressing mode\000"
/* offset=163739 */ "ea_memory\000memory\000This event counts energy consumption per cycle of CMG local memory\000event=0x3e8\000\00000\000This event counts energy consumption per cycle of CMG local memory\000"
/* offset=163906 */ "uop_split\000other\000This event counts the occurrence count of the micro-operation split\000event=0x139\000\00000\000This event counts the occurrence count of the micro-operation split\000"
/* offset=164074 */ "ld_comp_wait_l2_miss\000other\000This event counts every cycle that no operation was committed because the oldest and uncommitted load/store/prefetch operation waits for memory access\000event=0x180\000\00000\000This event counts every cycle that no operation was committed because the oldest and uncommitted load/store/prefetch operation waits for memory access\000"
/* offset=164419 */ "ld_comp_wait_l2_miss_ex\000other\000This event counts every cycle that no instruction was committed because the oldest and uncommitted integer load operation waits for memory access\000event=0x181\000\00000\000This event counts every cycle that no instruction was committed because the oldest and uncommitted integer load operation waits for memory access\000"
/* offset=164757 */ "ld_comp_wait_l1_miss\000other\000This event counts every cycle that no instruction was committed because the oldest and uncommitted load/store/prefetch operation waits for L2 cache access\000event=0x182\000\00000\000This event counts every cycle that no instruction was committed because the oldest and uncommitted load/store/prefetch operation waits for L2 cache access\000"
/* offset=165110 */ "ld_comp_wait_l1_miss_ex\000other\000This event counts every cycle that no instruction was committed because the oldest and uncommitted integer load operation waits for L2 cache access\000event=0x183\000\00000\000This event counts every cycle that no instruction was committed because the oldest and uncommitted integer load operation waits for L2 cache access\000"
/* offset=165452 */ "ld_comp_wait\000other\000This event counts every cycle that no instruction was committed because the oldest and uncommitted load/store/prefetch operation waits for L1D cache, L2 cache and memory access\000event=0x184\000\00000\000This event counts every cycle that no instruction was committed because the oldest and uncommitted load/store/prefetch operation waits for L1D cache, L2 cache and memory access\000"
/* offset=165841 */ "ld_comp_wait_ex\000other\000This event counts every cycle that no instruction was committed because the oldest and uncommitted integer load operation waits for L1D cache, L2 cache and memory access\000event=0x185\000\00000\000This event counts every cycle that no instruction was committed because the oldest and uncommitted integer load operation waits for L1D cache, L2 cache and memory access\000"
/* offset=166219 */ "ld_comp_wait_pfp_busy\000other\000This event counts every cycle that no instruction was committed due to the lack of an available prefetch port\000event=0x186\000\00000\000This event counts every cycle that no instruction was committed due to the lack of an available prefetch port\000"
/* offset=166483 */ "ld_comp_wait_pfp_busy_ex\000other\000This event counts the LD_COMP_WAIT_PFP_BUSY caused by an integer load operation\000event=0x187\000\00000\000This event counts the LD_COMP_WAIT_PFP_BUSY caused by an integer load operation\000"
/* offset=166690 */ "ld_comp_wait_pfp_busy_swpf\000other\000This event counts the LD_COMP_WAIT_PFP_BUSY caused by a software prefetch instruction\000event=0x188\000\00000\000This event counts the LD_COMP_WAIT_PFP_BUSY caused by a software prefetch instruction\000"
/* offset=166911 */ "eu_comp_wait\000other\000This event counts every cycle that no instruction was committed and the oldest and uncommitted instruction is an integer or floating-point/SIMD instruction\000event=0x189\000\00000\000This event counts every cycle that no instruction was committed and the oldest and uncommitted instruction is an integer or floating-point/SIMD instruction\000"
/* offset=167258 */ "fl_comp_wait\000other\000This event counts every cycle that no instruction was committed and the oldest and uncommitted instruction is a floating-point/SIMD instruction\000event=0x18a\000\00000\000This event counts every cycle that no instruction was committed and the oldest and uncommitted instruction is a floating-point/SIMD instruction\000"
/* offset=167581 */ "br_comp_wait\000other\000This event counts every cycle that no instruction was committed and the oldest and uncommitted instruction is a branch instruction\000event=0x18b\000\00000\000This event counts every cycle that no instruction was committed and the oldest and uncommitted instruction is a branch instruction\000"
/* offset=167878 */ "rob_empty\000other\000This event counts every cycle that no instruction was committed because the CSE is empty\000event=0x18c\000\00000\000This event counts every cycle that no instruction was committed because the CSE is empty\000"
/* offset=168088 */ "rob_empty_stq_busy\000other\000This event counts every cycle that no instruction was committed because the CSE is empty and the store port (SP) is full\000event=0x18d\000\00000\000This event counts every cycle that no instruction was committed because the CSE is empty and the store port (SP) is full\000"
/* offset=168371 */ "wfe_wfi_cycle\000other\000This event counts every cycle that the instruction unit is halted by the WFE/WFI instruction\000event=0x18e\000\00000\000This event counts every cycle that the instruction unit is halted by the WFE/WFI instruction\000"
/* offset=168593 */ "_0inst_commit\000other\000This event counts every cycle that no instruction was committed, but counts at the time when commits MOVPRFX only\000event=0x190\000\00000\000This event counts every cycle that no instruction was committed, but counts at the time when commits MOVPRFX only\000"
/* offset=168857 */ "_1inst_commit\000other\000This event counts every cycle that one instruction is committed\000event=0x191\000\00000\000This event counts every cycle that one instruction is committed\000"
/* offset=169021 */ "_2inst_commit\000other\000This event counts every cycle that two instructions are committed\000event=0x192\000\00000\000This event counts every cycle that two instructions are committed\000"
/* offset=169189 */ "_3inst_commit\000other\000This event counts every cycle that three instructions are committed\000event=0x193\000\00000\000This event counts every cycle that three instructions are committed\000"
/* offset=169361 */ "_4inst_commit\000other\000This event counts every cycle that four instructions are committed\000event=0x194\000\00000\000This event counts every cycle that four instructions are committed\000"
/* offset=169531 */ "uop_only_commit\000other\000This event counts every cycle that only any micro-operations are committed\000event=0x198\000\00000\000This event counts every cycle that only any micro-operations are committed\000"
/* offset=169719 */ "single_movprfx_commit\000other\000This event counts every cycle that only the MOVPRFX instruction is committed\000event=0x199\000\00000\000This event counts every cycle that only the MOVPRFX instruction is committed\000"
/* offset=169917 */ "ea_core\000other\000This event counts energy consumption per cycle of core\000event=0x1e0\000\00000\000This event counts energy consumption per cycle of core\000"
/* offset=170057 */ "l1hwpf_stream_pf\000other\000This event counts streaming prefetch requests to L1D cache generated by hardware prefetcher\000event=0x230\000\00000\000This event counts streaming prefetch requests to L1D cache generated by hardware prefetcher\000"
/* offset=170280 */ "l1hwpf_inj_alloc_pf\000other\000This event counts allocation type prefetch injection requests to L1D cache generated by hardware prefetcher\000event=0x231\000\00000\000This event counts allocation type prefetch injection requests to L1D cache generated by hardware prefetcher\000"
/* offset=170538 */ "l1hwpf_inj_noalloc_pf\000other\000This event counts non-allocation type prefetch injection requests to L1D cache generated by hardware prefetcher\000event=0x232\000\00000\000This event counts non-allocation type prefetch injection requests to L1D cache generated by hardware prefetcher\000"
/* offset=170806 */ "l2hwpf_stream_pf\000other\000This event counts streaming prefetch requests to L2 cache generated by hardware prefecher\000event=0x233\000\00000\000This event counts streaming prefetch requests to L2 cache generated by hardware prefecher\000"
/* offset=171025 */ "l2hwpf_inj_alloc_pf\000other\000This event counts allocation type prefetch injection requests to L2 cache generated by hardware prefetcher\000event=0x234\000\00000\000This event counts allocation type prefetch injection requests to L2 cache generated by hardware prefetcher\000"
/* offset=171281 */ "l2hwpf_inj_noalloc_pf\000other\000This event counts non-allocation type prefetch injection requests to L2 cache generated by hardware prefetcher\000event=0x235\000\00000\000This event counts non-allocation type prefetch injection requests to L2 cache generated by hardware prefetcher\000"
/* offset=171547 */ "l2hwpf_other\000other\000This event counts prefetch requests to L2 cache generated by the other causes\000event=0x236\000\00000\000This event counts prefetch requests to L2 cache generated by the other causes\000"
/* offset=171738 */ "eaga_val\000pipeline\000This event counts valid cycles of EAGA pipeline\000event=0x1a0\000\00000\000This event counts valid cycles of EAGA pipeline\000"
/* offset=171868 */ "eagb_val\000pipeline\000This event counts valid cycles of EAGB pipeline\000event=0x1a1\000\00000\000This event counts valid cycles of EAGB pipeline\000"
/* offset=171998 */ "exa_val\000pipeline\000This event counts valid cycles of EXA pipeline\000event=0x1a2\000\00000\000This event counts valid cycles of EXA pipeline\000"
/* offset=172125 */ "exb_val\000pipeline\000This event counts valid cycles of EXB pipeline\000event=0x1a3\000\00000\000This event counts valid cycles of EXB pipeline\000"
/* offset=172252 */ "fla_val\000pipeline\000This event counts valid cycles of FLA pipeline\000event=0x1a4\000\00000\000This event counts valid cycles of FLA pipeline\000"
/* offset=172379 */ "flb_val\000pipeline\000This event counts valid cycles of FLB pipeline\000event=0x1a5\000\00000\000This event counts valid cycles of FLB pipeline\000"
/* offset=172506 */ "prx_val\000pipeline\000This event counts valid cycles of PRX pipeline\000event=0x1a6\000\00000\000This event counts valid cycles of PRX pipeline\000"
/* offset=172633 */ "fla_val_prd_cnt\000pipeline\000This event counts the number of 1's in the predicate bits of request in FLA pipeline, where it is corrected so that it becomes 16 when all bits are 1\000event=0x1b4\000\00000\000This event counts the number of 1's in the predicate bits of request in FLA pipeline, where it is corrected so that it becomes 16 when all bits are 1\000"
/* offset=172974 */ "flb_val_prd_cnt\000pipeline\000This event counts the number of 1's in the predicate bits of request in FLB pipeline, where it is corrected so that it becomes 16 when all bits are 1\000event=0x1b5\000\00000\000This event counts the number of 1's in the predicate bits of request in FLB pipeline, where it is corrected so that it becomes 16 when all bits are 1\000"
/* offset=173315 */ "l1_pipe0_val\000pipeline\000This event counts valid cycles of L1D cache pipeline#0\000event=0x240\000\00000\000This event counts valid cycles of L1D cache pipeline#0\000"
/* offset=173463 */ "l1_pipe1_val\000pipeline\000This event counts valid cycles of L1D cache pipeline#1\000event=0x241\000\00000\000This event counts valid cycles of L1D cache pipeline#1\000"
/* offset=173611 */ "l1_pipe0_val_iu_tag_adrs_sce\000pipeline\000This event counts requests in L1D cache pipeline#0 that its sce bit of tagged address is 1\000event=0x250\000\00000\000This event counts requests in L1D cache pipeline#0 that its sce bit of tagged address is 1\000"
/* offset=173847 */ "l1_pipe0_val_iu_tag_adrs_pfe\000pipeline\000This event counts requests in L1D cache pipeline#0 that its pfe bit of tagged address is 1\000event=0x251\000\00000\000This event counts requests in L1D cache pipeline#0 that its pfe bit of tagged address is 1\000"
/* offset=174083 */ "l1_pipe1_val_iu_tag_adrs_sce\000pipeline\000This event counts requests in L1D cache pipeline#1 that its sce bit of tagged address is 1\000event=0x252\000\00000\000This event counts requests in L1D cache pipeline#1 that its sce bit of tagged address is 1\000"
/* offset=174319 */ "l1_pipe1_val_iu_tag_adrs_pfe\000pipeline\000This event counts requests in L1D cache pipeline#1 that its pfe bit of tagged address is 1\000event=0x253\000\00000\000This event counts requests in L1D cache pipeline#1 that its pfe bit of tagged address is 1\000"
/* offset=174555 */ "l1_pipe0_comp\000pipeline\000This event counts completed requests in L1D cache pipeline#0\000event=0x260\000\00000\000This event counts completed requests in L1D cache pipeline#0\000"
/* offset=174716 */ "l1_pipe1_comp\000pipeline\000This event counts completed requests in L1D cache pipeline#1\000event=0x261\000\00000\000This event counts completed requests in L1D cache pipeline#1\000"
/* offset=174877 */ "l1i_pipe_comp\000pipeline\000This event counts completed requests in L1I cache pipeline\000event=0x268\000\00000\000This event counts completed requests in L1I cache pipeline\000"
/* offset=175034 */ "l1i_pipe_val\000pipeline\000This event counts valid cycles of L1I cache pipeline\000event=0x269\000\00000\000This event counts valid cycles of L1I cache pipeline\000"
/* offset=175178 */ "l1_pipe_abort_stld_intlk\000pipeline\000This event counts aborted requests in L1D pipelines that due to store-load interlock\000event=0x274\000\00000\000This event counts aborted requests in L1D pipelines that due to store-load interlock\000"
/* offset=175398 */ "l1_pipe0_val_iu_not_sec0\000pipeline\000This event counts requests in L1D cache pipeline#0 that its sector cache ID is not 0\000event=0x2a0\000\00000\000This event counts requests in L1D cache pipeline#0 that its sector cache ID is not 0\000"
/* offset=175618 */ "l1_pipe1_val_iu_not_sec0\000pipeline\000This event counts requests in L1D cache pipeline#1 that its sector cache ID is not 0\000event=0x2a1\000\00000\000This event counts requests in L1D cache pipeline#1 that its sector cache ID is not 0\000"
/* offset=175838 */ "l1_pipe_comp_gather_2flow\000pipeline\000This event counts the number of times where 2 elements of the gather instructions became 2 flows because 2 elements could not be combined\000event=0x2b0\000\00000\000This event counts the number of times where 2 elements of the gather instructions became 2 flows because 2 elements could not be combined\000"
/* offset=176165 */ "l1_pipe_comp_gather_1flow\000pipeline\000This event counts the number of times where 2 elements of the gather instructions became 1 flow because 2 elements could be combined\000event=0x2b1\000\00000\000This event counts the number of times where 2 elements of the gather instructions became 1 flow because 2 elements could be combined\000"
/* offset=176482 */ "l1_pipe_comp_gather_0flow\000pipeline\000This event counts the number of times where 2 elements of the gather instructions became 0 flow because both predicate values are 0\000event=0x2b2\000\00000\000This event counts the number of times where 2 elements of the gather instructions became 0 flow because both predicate values are 0\000"
/* offset=176797 */ "l1_pipe_comp_scatter_1flow\000pipeline\000This event counts the number of flows of the scatter instructions\000event=0x2b3\000\00000\000This event counts the number of flows of the scatter instructions\000"
/* offset=176981 */ "l1_pipe0_comp_prd_cnt\000pipeline\000This event counts the number of 1's in the predicate bits of request in L1D cache pipeline#0, where it is corrected so that it becomes 16 when all bits are 1\000event=0x2b8\000\00000\000This event counts the number of 1's in the predicate bits of request in L1D cache pipeline#0, where it is corrected so that it becomes 16 when all bits are 1\000"
/* offset=177344 */ "l1_pipe1_comp_prd_cnt\000pipeline\000This event counts the number of 1's in the predicate bits of request in L1D cache pipeline#1, where it is corrected so that it becomes 16 when all bits are 1\000event=0x2b9\000\00000\000This event counts the number of 1's in the predicate bits of request in L1D cache pipeline#1, where it is corrected so that it becomes 16 when all bits are 1\000"
/* offset=177707 */ "l2_pipe_val\000pipeline\000This event counts valid cycles of L2 cache pipeline\000event=0x330\000\00000\000This event counts valid cycles of L2 cache pipeline\000"
/* offset=177848 */ "l2_pipe_comp_all\000pipeline\000This event counts completed requests in L2 cache pipeline\000event=0x350\000\00000\000This event counts completed requests in L2 cache pipeline\000"
/* offset=178006 */ "l2_pipe_comp_pf_l2mib_mch\000pipeline\000This event counts operations where software or hardware prefetch hits an L2 cache refill buffer allocated by demand access\000event=0x370\000\00000\000This event counts operations where software or hardware prefetch hits an L2 cache refill buffer allocated by demand access\000"
/* offset=178303 */ "simd_inst_retired\000sve\000SIMD Instruction architecturally executed\000event=0x8000\000\00000\000SIMD Instruction architecturally executed\000"
/* offset=178426 */ "sve_inst_retired\000sve\000Instruction architecturally executed, SVE\000event=0x8002\000\00000\000Instruction architecturally executed, SVE\000"
/* offset=178548 */ "uop_spec\000sve\000Microarchitectural operation, Operations speculatively executed\000event=0x8008\000\00000\000Microarchitectural operation, Operations speculatively executed\000"
/* offset=178706 */ "sve_math_spec\000sve\000SVE Math accelerator Operations speculatively executed\000event=0x800e\000\00000\000SVE Math accelerator Operations speculatively executed\000"
/* offset=178851 */ "fp_spec\000sve\000Floating-point Operations speculatively executed\000event=0x8010\000\00000\000Floating-point Operations speculatively executed\000"
/* offset=178978 */ "fp_fma_spec\000sve\000Floating-point FMA Operations speculatively executed\000event=0x8028\000\00000\000Floating-point FMA Operations speculatively executed\000"
/* offset=179117 */ "fp_recpe_spec\000sve\000Floating-point reciprocal estimate Operations speculatively executed\000event=0x8034\000\00000\000Floating-point reciprocal estimate Operations speculatively executed\000"
/* offset=179290 */ "fp_cvt_spec\000sve\000floating-point convert Operations speculatively executed\000event=0x8038\000\00000\000floating-point convert Operations speculatively executed\000"
/* offset=179437 */ "ase_sve_int_spec\000sve\000Advanced SIMD and SVE integer Operations speculatively executed\000event=0x8043\000\00000\000Advanced SIMD and SVE integer Operations speculatively executed\000"
/* offset=179603 */ "sve_pred_spec\000sve\000SVE predicated Operations speculatively executed\000event=0x8074\000\00000\000SVE predicated Operations speculatively executed\000"
/* offset=179736 */ "sve_movprfx_spec\000sve\000SVE MOVPRFX Operations speculatively executed\000event=0x807c\000\00000\000SVE MOVPRFX Operations speculatively executed\000"
/* offset=179866 */ "sve_movprfx_u_spec\000sve\000SVE MOVPRFX unfused Operations speculatively executed\000event=0x807f\000\00000\000SVE MOVPRFX unfused Operations speculatively executed\000"
/* offset=180014 */ "ase_sve_ld_spec\000sve\000Advanced SIMD and SVE load Operations speculatively executed\000event=0x8085\000\00000\000Advanced SIMD and SVE load Operations speculatively executed\000"
/* offset=180173 */ "ase_sve_st_spec\000sve\000Advanced SIMD and SVE store Operations speculatively executed\000event=0x8086\000\00000\000Advanced SIMD and SVE store Operations speculatively executed\000"
/* offset=180334 */ "prf_spec\000sve\000Prefetch Operations speculatively executed\000event=0x8087\000\00000\000Prefetch Operations speculatively executed\000"
/* offset=180450 */ "base_ld_reg_spec\000sve\000General-purpose register load Operations speculatively executed\000event=0x8089\000\00000\000General-purpose register load Operations speculatively executed\000"
/* offset=180616 */ "base_st_reg_spec\000sve\000General-purpose register store Operations speculatively executed\000event=0x808a\000\00000\000General-purpose register store Operations speculatively executed\000"
/* offset=180784 */ "sve_ldr_reg_spec\000sve\000SVE unpredicated load register Operations speculatively executed\000event=0x8091\000\00000\000SVE unpredicated load register Operations speculatively executed\000"
/* offset=180952 */ "sve_str_reg_spec\000sve\000SVE unpredicated store register Operations speculatively executed\000event=0x8092\000\00000\000SVE unpredicated store register Operations speculatively executed\000"
/* offset=181122 */ "sve_ldr_preg_spec\000sve\000SVE load predicate register Operations speculatively executed\000event=0x8095\000\00000\000SVE load predicate register Operations speculatively executed\000"
/* offset=181285 */ "sve_str_preg_spec\000sve\000SVE store predicate register Operations speculatively executed\000event=0x8096\000\00000\000SVE store predicate register Operations speculatively executed\000"
/* offset=181450 */ "sve_prf_contig_spec\000sve\000SVE contiguous prefetch element Operations speculatively executed\000event=0x809f\000\00000\000SVE contiguous prefetch element Operations speculatively executed\000"
/* offset=181623 */ "ase_sve_ld_multi_spec\000sve\000Advanced SIMD and SVE contiguous load multiple vector Operations speculatively executed\000event=0x80a5\000\00000\000Advanced SIMD and SVE contiguous load multiple vector Operations speculatively executed\000"
/* offset=181842 */ "ase_sve_st_multi_spec\000sve\000Advanced SIMD and SVE contiguous store multiple vector Operations speculatively executed\000event=0x80a6\000\00000\000Advanced SIMD and SVE contiguous store multiple vector Operations speculatively executed\000"
/* offset=182063 */ "sve_ld_gather_spec\000sve\000SVE gather-load Operations speculatively executed\000event=0x80ad\000\00000\000SVE gather-load Operations speculatively executed\000"
/* offset=182203 */ "sve_st_scatter_spec\000sve\000SVE scatter-store Operations speculatively executed\000event=0x80ae\000\00000\000SVE scatter-store Operations speculatively executed\000"
/* offset=182348 */ "sve_prf_gather_spec\000sve\000SVE gather-prefetch Operations speculatively executed\000event=0x80af\000\00000\000SVE gather-prefetch Operations speculatively executed\000"
/* offset=182497 */ "sve_ldff_spec\000sve\000SVE First-fault load Operations speculatively executed\000event=0x80bc\000\00000\000SVE First-fault load Operations speculatively executed\000"
/* offset=182642 */ "fp_scale_ops_spec\000sve\000Scalable floating-point element Operations speculatively executed\000event=0x80c0\000\00000\000Scalable floating-point element Operations speculatively executed\000"
/* offset=182813 */ "fp_fixed_ops_spec\000sve\000Non-scalable floating-point element Operations speculatively executed\000event=0x80c1\000\00000\000Non-scalable floating-point element Operations speculatively executed\000"
/* offset=182992 */ "fp_hp_scale_ops_spec\000sve\000Scalable half-precision floating-point element Operations speculatively executed\000event=0x80c2\000\00000\000Scalable half-precision floating-point element Operations speculatively executed\000"
/* offset=183196 */ "fp_hp_fixed_ops_spec\000sve\000Non-scalable half-precision floating-point element Operations speculatively executed\000event=0x80c3\000\00000\000Non-scalable half-precision floating-point element Operations speculatively executed\000"
/* offset=183408 */ "fp_sp_scale_ops_spec\000sve\000Scalable single-precision floating-point element Operations speculatively executed\000event=0x80c4\000\00000\000Scalable single-precision floating-point element Operations speculatively executed\000"
/* offset=183616 */ "fp_sp_fixed_ops_spec\000sve\000Non-scalable single-precision floating-point element Operations speculatively executed\000event=0x80c5\000\00000\000Non-scalable single-precision floating-point element Operations speculatively executed\000"
/* offset=183832 */ "fp_dp_scale_ops_spec\000sve\000Scalable double-precision floating-point element Operations speculatively executed\000event=0x80c6\000\00000\000Scalable double-precision floating-point element Operations speculatively executed\000"
/* offset=184040 */ "fp_dp_fixed_ops_spec\000sve\000Non-scalable double-precision floating-point element Operations speculatively executed\000event=0x80c7\000\00000\000Non-scalable double-precision floating-point element Operations speculatively executed\000"
/* offset=184256 */ "l2d_cache_rd\000core imp def\000L2D cache access, read\000event=0x50\000\00000\000Attributable Level 2 data cache access, read\000"
/* offset=184365 */ "l2d_cache_wr\000core imp def\000L2D cache access, write\000event=0x51\000\00000\000Attributable Level 2 data cache access, write\000"
/* offset=184476 */ "l2d_cache_refill_rd\000core imp def\000L2D cache refill, read\000event=0x52\000\00000\000Attributable Level 2 data cache refill, read\000"
/* offset=184592 */ "l2d_cache_refill_wr\000core imp def\000L2D cache refill, write\000event=0x53\000\00000\000Attributable Level 2 data cache refill, write\000"
/* offset=184710 */ "l2d_cache_wb_victim\000core imp def\000L2D cache Write-Back, victim\000event=0x56\000\00000\000Attributable Level 2 data cache Write-Back, victim\000"
/* offset=184838 */ "l2d_cache_wb_clean\000core imp def\000L2D cache Write-Back, cleaning and coherency\000event=0x57\000\00000\000Level 2 data cache Write-Back, cleaning and coherency\000"
/* offset=184984 */ "l2d_cache_inval\000core imp def\000L2D cache invalidate\000event=0x58\000\00000\000Attributable Level 2 data cache invalidate\000"
/* offset=185092 */ "l1i_cache_prf\000core imp def\000L1I cache prefetch access count\000event=0x102e\000\00000\000Level 1 instruction cache prefetch access count\000"
/* offset=185216 */ "l1i_cache_prf_refill\000core imp def\000L1I cache miss due to prefetch access count\000event=0x102f\000\00000\000Level 1 instruction cache miss due to prefetch access count\000"
/* offset=185371 */ "iq_is_empty\000core imp def\000Instruction queue is empty\000event=0x1043\000\00000\000Instruction queue is empty\000"
/* offset=185467 */ "if_is_stall\000core imp def\000Instruction fetch stall cycles\000event=0x1044\000\00000\000Instruction fetch stall cycles\000"
/* offset=185571 */ "fetch_bubble\000core imp def\000Instructions can receive, but not send\000event=0x2014\000\00000\000Instructions can receive, but not send\000"
/* offset=185692 */ "prf_req\000core imp def\000Prefetch request from LSU\000event=0x6013\000\00000\000Prefetch request from LSU\000"
/* offset=185782 */ "hit_on_prf\000core imp def\000Hit on prefetched data\000event=0x6014\000\00000\000Hit on prefetched data\000"
/* offset=185869 */ "exe_stall_cycle\000core imp def\000Cycles of that the number of issue ups are less than 4\000event=0x7001\000\00000\000Cycles of that the number of issuing micro operations are less than 4\000"
/* offset=186040 */ "mem_stall_anyload\000core imp def\000No any micro operation is issued and meanwhile any load operation is not resolved\000event=0x7004\000\00000\000No any micro operation is issued and meanwhile any load operation is not resolved\000"
/* offset=186252 */ "mem_stall_l1miss\000core imp def\000No any micro operation is issued and meanwhile there is any load operation missing L1 cache and pending data refill\000event=0x7006\000\00000\000No any micro operation is issued and meanwhile there is any load operation missing L1 cache and pending data refill\000"
/* offset=186531 */ "mem_stall_l2miss\000core imp def\000No any micro operation is issued and meanwhile there is any load operation missing both L1 and L2 cache and pending data refill from L3 cache\000event=0x7007\000\00000\000No any micro operation is issued and meanwhile there is any load operation missing both L1 and L2 cache and pending data refill from L3 cache\000"
/* offset=186862 */ "hisi_sccl,ddrc\000"
/* offset=186877 */ "flux_wr\000uncore ddrc\000DDRC total write operations\000config=0\000\00000\000DDRC total write operations\000"
/* offset=186966 */ "flux_rd\000uncore ddrc\000DDRC total read operations\000config=0x1\000\00000\000DDRC total read operations\000"
/* offset=187055 */ "flux_wcmd\000uncore ddrc\000DDRC write commands\000config=0x2\000\00000\000DDRC write commands\000"
/* offset=187132 */ "flux_rcmd\000uncore ddrc\000DDRC read commands\000config=0x3\000\00000\000DDRC read commands\000"
/* offset=187207 */ "pre_cmd\000uncore ddrc\000DDRC precharge commands\000config=0x4\000\00000\000DDRC precharge commands\000"
/* offset=187290 */ "act_cmd\000uncore ddrc\000DDRC active commands\000config=0x5\000\00000\000DDRC active commands\000"
/* offset=187367 */ "rnk_chg\000uncore ddrc\000DDRC rank commands\000config=0x6\000\00000\000DDRC rank commands\000"
/* offset=187440 */ "rw_chg\000uncore ddrc\000DDRC read and write changes\000config=0x7\000\00000\000DDRC read and write changes\000"
/* offset=187530 */ "hisi_sccl,hha\000"
/* offset=187544 */ "rx_ops_num\000uncore hha\000The number of all operations received by the HHA\000config=0\000\00000\000The number of all operations received by the HHA\000"
/* offset=187677 */ "rx_outer\000uncore hha\000The number of all operations received by the HHA from another socket\000config=0x1\000\00000\000The number of all operations received by the HHA from another socket\000"
/* offset=187850 */ "rx_sccl\000uncore hha\000The number of all operations received by the HHA from another SCCL in this socket\000config=0x2\000\00000\000The number of all operations received by the HHA from another SCCL in this socket\000"
/* offset=188048 */ "rx_ccix\000uncore hha\000Count of the number of operations that HHA has received from CCIX\000config=0x3\000\00000\000Count of the number of operations that HHA has received from CCIX\000"
/* offset=188214 */ "rx_wbi\000uncore hha\000\000config=0x4\000\00000\000\000"
/* offset=188249 */ "rx_wbip\000uncore hha\000\000config=0x5\000\00000\000\000"
/* offset=188285 */ "rx_wtistash\000uncore hha\000\000config=0x11\000\00000\000\000"
/* offset=188326 */ "rd_ddr_64b\000uncore hha\000The number of read operations sent by HHA to DDRC which size is 64 bytes\000config=0x1c\000\00000\000The number of read operations sent by HHA to DDRC which size is 64bytes\000"
/* offset=188509 */ "wr_ddr_64b\000uncore hha\000The number of write operations sent by HHA to DDRC which size is 64 bytes\000config=0x1d\000\00000\000The number of write operations sent by HHA to DDRC which size is 64 bytes\000"
/* offset=188695 */ "rd_ddr_128b\000uncore hha\000The number of read operations sent by HHA to DDRC which size is 128 bytes\000config=0x1e\000\00000\000The number of read operations sent by HHA to DDRC which size is 128 bytes\000"
/* offset=188882 */ "wr_ddr_128b\000uncore hha\000The number of write operations sent by HHA to DDRC which size is 128 bytes\000config=0x1f\000\00000\000The number of write operations sent by HHA to DDRC which size is 128 bytes\000"
/* offset=189071 */ "spill_num\000uncore hha\000Count of the number of spill operations that the HHA has sent\000config=0x20\000\00000\000Count of the number of spill operations that the HHA has sent\000"
/* offset=189232 */ "spill_success\000uncore hha\000Count of the number of successful spill operations that the HHA has sent\000config=0x21\000\00000\000Count of the number of successful spill operations that the HHA has sent\000"
/* offset=189419 */ "bi_num\000uncore hha\000\000config=0x23\000\00000\000\000"
/* offset=189455 */ "mediated_num\000uncore hha\000\000config=0x32\000\00000\000\000"
/* offset=189497 */ "tx_snp_num\000uncore hha\000\000config=0x33\000\00000\000\000"
/* offset=189537 */ "tx_snp_outer\000uncore hha\000\000config=0x34\000\00000\000\000"
/* offset=189579 */ "tx_snp_ccix\000uncore hha\000\000config=0x35\000\00000\000\000"
/* offset=189620 */ "rx_snprspdata\000uncore hha\000\000config=0x38\000\00000\000\000"
/* offset=189663 */ "rx_snprsp_outer\000uncore hha\000\000config=0x3c\000\00000\000\000"
/* offset=189708 */ "sdir-lookup\000uncore hha\000\000config=0x40\000\00000\000\000"
/* offset=189749 */ "edir-lookup\000uncore hha\000\000config=0x41\000\00000\000\000"
/* offset=189790 */ "sdir-hit\000uncore hha\000\000config=0x42\000\00000\000\000"
/* offset=189828 */ "edir-hit\000uncore hha\000\000config=0x43\000\00000\000\000"
/* offset=189866 */ "sdir-home-migrate\000uncore hha\000\000config=0x4c\000\00000\000\000"
/* offset=189913 */ "edir-home-migrate\000uncore hha\000\000config=0x4d\000\00000\000\000"
/* offset=189960 */ "hisi_sccl,l3c\000"
/* offset=189974 */ "rd_cpipe\000uncore l3c\000Total read accesses\000config=0\000\00000\000Total read accesses\000"
/* offset=190047 */ "wr_cpipe\000uncore l3c\000Total write accesses\000config=0x1\000\00000\000Total write accesses\000"
/* offset=190124 */ "rd_hit_cpipe\000uncore l3c\000Total read hits\000config=0x2\000\00000\000Total read hits\000"
/* offset=190195 */ "wr_hit_cpipe\000uncore l3c\000Total write hits\000config=0x3\000\00000\000Total write hits\000"
/* offset=190268 */ "victim_num\000uncore l3c\000l3c precharge commands\000config=0x4\000\00000\000l3c precharge commands\000"
/* offset=190351 */ "rd_spipe\000uncore l3c\000Count of the number of read lines that come from this cluster of CPU core in spipe\000config=0x20\000\00000\000Count of the number of read lines that come from this cluster of CPU core in spipe\000"
/* offset=190553 */ "wr_spipe\000uncore l3c\000Count of the number of write lines that come from this cluster of CPU core in spipe\000config=0x21\000\00000\000Count of the number of write lines that come from this cluster of CPU core in spipe\000"
/* offset=190757 */ "rd_hit_spipe\000uncore l3c\000Count of the number of read lines that hits in spipe of this L3C\000config=0x22\000\00000\000Count of the number of read lines that hits in spipe of this L3C\000"
/* offset=190927 */ "wr_hit_spipe\000uncore l3c\000Count of the number of write lines that hits in spipe of this L3C\000config=0x23\000\00000\000Count of the number of write lines that hits in spipe of this L3C\000"
/* offset=191099 */ "back_invalid\000uncore l3c\000Count of the number of L3C back invalid operations\000config=0x29\000\00000\000Count of the number of L3C back invalid operations\000"
/* offset=191241 */ "retry_cpu\000uncore l3c\000Count of the number of retry that L3C suppresses the CPU operations\000config=0x40\000\00000\000Count of the number of retry that L3C suppresses the CPU operations\000"
/* offset=191414 */ "retry_ring\000uncore l3c\000Count of the number of retry that L3C suppresses the ring operations\000config=0x41\000\00000\000Count of the number of retry that L3C suppresses the ring operations\000"
/* offset=191590 */ "prefetch_drop\000uncore l3c\000Count of the number of prefetch drops from this L3C\000config=0x42\000\00000\000Count of the number of prefetch drops from this L3C\000"
/* offset=191735 */ "hisi_sicl,cpa\000"
/* offset=191749 */ "cpa_cycles\000uncore cpa\000count of CPA cycles\000config=0\0000x00000030\00000\000\000"
/* offset=191815 */ "cpa_p1_wr_dat\000uncore cpa\000Number of write ops transmitted by the P1 port\000config=0x61\0000x00000030\00000\000\000"
/* offset=191914 */ "cpa_p1_rd_dat\000uncore cpa\000Number of read ops transmitted by the P1 port\000config=0x62\0000x00000030\00000\000\000"
/* offset=192012 */ "cpa_p1_rd_dat_64b\000uncore cpa\000Number of read ops transmitted by the P1 port which size is 64 bytes\000config=0x3\0000x00000030\00000\000\000"
/* offset=192136 */ "cpa_p1_rd_dat_32b\000uncore cpa\000Number of read ops transmitted by the P1 port which size is 32 bytes\000config=0x4\0000x00000030\00000\000\000"
/* offset=192260 */ "cpa_p0_wr_dat\000uncore cpa\000Number of write ops transmitted by the P0 port\000config=0xe1\0000x00000030\00000\000\000"
/* offset=192359 */ "cpa_p0_rd_dat\000uncore cpa\000Number of read ops transmitted by the P0 port\000config=0xe2\0000x00000030\00000\000\000"
/* offset=192457 */ "cpa_p0_rd_dat_64b\000uncore cpa\000Number of read ops transmitted by the P0 port which size is 64 bytes\000config=0x83\0000x00000030\00000\000\000"
/* offset=192582 */ "cpa_p0_rd_dat_32b\000uncore cpa\000Number of read ops transmitted by the P0 port which size is 32 bytes\000config=0x84\0000x00000030\00000\000\000"
/* offset=192707 */ "bp_l1_btb_correct\000branch\000L1 BTB Correction\000event=0x8a\000\00000\000\000"
/* offset=192766 */ "bp_l2_btb_correct\000branch\000L2 BTB Correction\000event=0x8b\000\00000\000\000"
/* offset=192825 */ "l3_cache_rd\000cache\000L3 cache access, read\000event=0x40\000\00000\000Attributable Level 3 cache access, read\000"
/* offset=192920 */ "segment_reg_loads.any\000other\000Number of segment register loads\000event=0x6,period=200000,umask=0x80\000\00000\000\000"
/* offset=193021 */ "dispatch_blocked.any\000other\000Memory cluster signals to block micro-op dispatch for any reason\000event=0x9,period=200000,umask=0x20\000\00000\000\000"
/* offset=193153 */ "eist_trans\000other\000Number of Enhanced Intel SpeedStep(R) Technology (EIST) transitions\000event=0x3a,period=200000,umask=0x0\000\00000\000\000"
/* offset=193278 */ "uncore_hisi_ddrc.flux_wcmd\000uncore\000DDRC write commands\000event=0x2\000\00000\000DDRC write commands\000"
/* offset=193366 */ "uncore_cbox\000"
/* offset=193378 */ "unc_cbo_xsnp_response.miss_eviction\000uncore\000A cross-core snoop resulted from L3 Eviction which misses in some processor core\000event=0x22,umask=0x81\000\00000\000A cross-core snoop resulted from L3 Eviction which misses in some processor core\000"
/* offset=193609 */ "event-hyphen\000uncore\000UNC_CBO_HYPHEN\000event=0xe0,umask=0x00\000\00000\000UNC_CBO_HYPHEN\000"
/* offset=193685 */ "event-two-hyph\000uncore\000UNC_CBO_TWO_HYPH\000event=0xc0,umask=0x00\000\00000\000UNC_CBO_TWO_HYPH\000"
/* offset=193767 */ "uncore_hisi_l3c.rd_hit_cpipe\000uncore\000Total read hits\000event=0x7\000\00000\000Total read hits\000"
/* offset=193849 */ "uncore_imc_free_running\000"
/* offset=193873 */ "uncore_imc_free_running.cache_miss\000uncore\000Total cache misses\000event=0x12\000\00000\000Total cache misses\000"
/* offset=193968 */ "uncore_imc\000"
/* offset=193979 */ "uncore_imc.cache_hits\000uncore\000Total cache hits\000event=0x34\000\00000\000Total cache hits\000"
/* offset=194057 */ "uncore_sys_ddr_pmu\000"
/* offset=194076 */ "sys_ddr_pmu.write_cycles\000uncore\000ddr write-cycles event\000event=0x2b\000v8\00000\000\000"
/* offset=194149 */ "uncore_sys_ccn_pmu\000"
/* offset=194168 */ "sys_ccn_pmu.read_cycles\000uncore\000ccn read-cycles event\000config=0x2c\0000x01\00000\000\000"
/* offset=194242 */ "branch_miss_pred_rate\000branch\000BR_MIS_PRED / BR_PRED\000\000Branch predictor misprediction rate. May not count branches that are never resolved because they are in the misprediction shadow of an earlier branch\000\000100%\000\000\000\00000"
/* offset=194455 */ "bus_utilization\000Bus\000BUS_ACCESS / BUS_CYCLES * 100\000\000Core-to-uncore bus utilization\000\0001percent of bus cycles\000\000\000\00000"
/* offset=194566 */ "l1d_cache_miss_ratio\000Miss_Ratio;L1D_Cache_Effectiveness\000L1D_CACHE_REFILL / L1D_CACHE\000\000This metric measures the ratio of level 1 data cache accesses missed to the total number of level 1 data cache accesses. This gives an indication of the effectiveness of the level 1 data cache\000\0001per cache access\000\000\000\00000"
/* offset=194869 */ "l1i_cache_miss_ratio\000Miss_Ratio;L1I_Cache_Effectiveness\000L1I_CACHE_REFILL / L1I_CACHE\000\000This metric measures the ratio of level 1 instruction cache accesses missed to the total number of level 1 instruction cache accesses. This gives an indication of the effectiveness of the level 1 instruction cache\000\0001per cache access\000\000\000\00000"
/* offset=195193 */ "Miss_Ratio;l1d_cache_read_miss\000Cache\000L1D_CACHE_LMISS_RD / L1D_CACHE_RD\000\000L1D cache read miss rate\000\0001per cache read access\000\000\000\00000"
/* offset=195319 */ "l2_cache_miss_ratio\000Miss_Ratio;L2_Cache_Effectiveness\000L2D_CACHE_REFILL / L2D_CACHE\000\000This metric measures the ratio of level 2 cache accesses missed to the total number of level 2 cache accesses. This gives an indication of the effectiveness of the level 2 cache, which is a unified cache that stores both data and instruction. Note that cache accesses in this cache are either data memory access or instruction fetch as this is a unified cache\000\0001per cache access\000\000\000\00000"
/* offset=195787 */ "l1i_cache_read_miss_rate\000Cache\000L1I_CACHE_LMISS / L1I_CACHE\000\000L1I cache read miss rate\000\0001per cache access\000\000\000\00000"
/* offset=195896 */ "l2d_cache_read_miss_rate\000Cache\000L2D_CACHE_LMISS_RD / L2D_CACHE_RD\000\000L2 cache read miss rate\000\0001per cache read access\000\000\000\00000"
/* offset=196015 */ "l1d_cache_miss_mpki\000Cache\000L1D_CACHE_LMISS_RD * 1e3 / INST_RETIRED\000\000Misses per thousand instructions (data)\000\0001MPKI\000\000\000\00000"
/* offset=196134 */ "l1i_cache_miss_mpki\000Cache\000L1I_CACHE_LMISS * 1e3 / INST_RETIRED\000\000Misses per thousand instructions (instruction)\000\0001MPKI\000\000\000\00000"
/* offset=196257 */ "simd_percentage\000Operation_Mix\000ASE_SPEC / INST_SPEC * 100\000\000This metric measures advanced SIMD operations as a percentage of total operations speculatively executed\000\0001percent of operations\000\000\000\00000"
/* offset=196449 */ "crypto_percentage\000Operation_Mix\000CRYPTO_SPEC / INST_SPEC * 100\000\000This metric measures crypto operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000"
/* offset=196633 */ "gflops\000InstructionMix\000VFP_SPEC / (duration_time * 1e9)\000\000Giga-floating point operations per second\000\000\000\000\000\00000"
/* offset=196738 */ "integer_dp_percentage\000Operation_Mix\000DP_SPEC / INST_SPEC * 100\000\000This metric measures scalar integer operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000"
/* offset=196930 */ "ipc\000General\000INST_RETIRED / CPU_CYCLES\000\000This metric measures the number of instructions retired per cycle\000\0001per cycle\000\000\000\00000"
/* offset=197052 */ "load_percentage\000Operation_Mix\000LD_SPEC / INST_SPEC * 100\000\000This metric measures load operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000"
/* offset=197228 */ "load_store_spec_rate\000Operation_Mix\000LDST_SPEC / INST_SPEC * 100\000\000The rate of load or store instructions speculatively executed to overall instructions speclatively executed\000\0001percent of operations\000\000\000\00000"
/* offset=197429 */ "retired_mips\000InstructionMix\000INST_RETIRED / (duration_time * 1e6)\000\000Millions of instructions per second\000\000\000\000\000\00000"
/* offset=197538 */ "spec_utilization_mips\000PEutilization\000INST_SPEC / (duration_time * 1e6)\000\000Millions of instructions per second\000\000\000\000\000\00000"
/* offset=197652 */ "pc_write_spec_rate\000Operation_Mix\000PC_WRITE_SPEC / INST_SPEC * 100\000\000The rate of software change of the PC speculatively executed to overall instructions speclatively executed\000\0001percent of operations\000\000\000\00000"
/* offset=197854 */ "store_percentage\000Operation_Mix\000ST_SPEC / INST_SPEC * 100\000\000This metric measures store operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000"
/* offset=198032 */ "scalar_fp_percentage\000Operation_Mix\000VFP_SPEC / INST_SPEC * 100\000\000This metric measures scalar floating point operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000"
/* offset=198231 */ "retired_rate\000General\000OP_RETIRED / OP_SPEC\000\000Of all the micro-operations issued, what percentage are retired(committed)\000\000100%\000\000\000\00000"
/* offset=198360 */ "wasted\000General\0001 - OP_RETIRED / (CPU_CYCLES * #slots)\000\000Of all the micro-operations issued, what proportion are lost\000\000100%\000\000\000\00000"
/* offset=198487 */ "wasted_rate\000General\0001 - OP_RETIRED / OP_SPEC\000\000Of all the micro-operations issued, what percentage are not retired(committed)\000\000100%\000\000\000\00000"
/* offset=198623 */ "stall_backend_cache_rate\000Stall\000STALL_BACKEND_CACHE / CPU_CYCLES * 100\000\000Proportion of cycles stalled and no operations issued to backend and cache miss\000\0001percent of cycles\000\000\000\00000"
/* offset=198799 */ "stall_backend_resource_rate\000Stall\000STALL_BACKEND_RESOURCE / CPU_CYCLES * 100\000\000Proportion of cycles stalled and no operations issued to backend and resource full\000\0001percent of cycles\000\000\000\00000"
/* offset=198984 */ "stall_backend_tlb_rate\000Stall\000STALL_BACKEND_TLB / CPU_CYCLES * 100\000\000Proportion of cycles stalled and no operations issued to backend and TLB miss\000\0001percent of cycles\000\000\000\00000"
/* offset=199154 */ "stall_frontend_cache_rate\000Stall\000STALL_FRONTEND_CACHE / CPU_CYCLES * 100\000\000Proportion of cycles stalled and no ops delivered from frontend and cache miss\000\0001percent of cycles\000\000\000\00000"
/* offset=199331 */ "stall_frontend_tlb_rate\000Stall\000STALL_FRONTEND_TLB / CPU_CYCLES * 100\000\000Proportion of cycles stalled and no ops delivered from frontend and TLB miss\000\0001percent of cycles\000\000\000\00000"
/* offset=199502 */ "dtlb_walk_ratio\000Miss_Ratio;DTLB_Effectiveness\000DTLB_WALK / L1D_TLB\000\000This metric measures the ratio of data TLB Walks to the total number of data TLB accesses. This gives an indication of the effectiveness of the data TLB accesses\000\0001per TLB access\000\000\000\00000"
/* offset=199753 */ "itlb_walk_ratio\000Miss_Ratio;ITLB_Effectiveness\000ITLB_WALK / L1I_TLB\000\000This metric measures the ratio of instruction TLB Walks to the total number of instruction TLB accesses. This gives an indication of the effectiveness of the instruction TLB accesses\000\0001per TLB access\000\000\000\00000"
/* offset=200025 */ "backend_bound\000Default;TopdownL1\000100 * (stall_slot_backend / (#slots * cpu_cycles))\000\000This metric is the percentage of total slots that were stalled due to resource constraints in the backend of the processor\000\0001percent of slots\000\000\000TopdownL1\00000"
/* offset=200265 */ "frontend_bound\000Default;TopdownL1\000100 - (retired_fraction + slots_lost_misspeculation_fraction + backend_bound)\000\000This metric is the percentage of total slots that were stalled due to resource constraints in the frontend of the processor\000\0001percent of slots\000\000\000TopdownL1\00000"
/* offset=200534 */ "slots_lost_misspeculation_fraction\000Default;TopdownL1\000100 * ((OP_SPEC - OP_RETIRED) / (CPU_CYCLES * #slots))\000\000Fraction of slots lost due to misspeculation\000\0001percent of slots\000\000\000TopdownL1\00000"
/* offset=200721 */ "retired_fraction\000Default;TopdownL1\000100 * (OP_RETIRED / (CPU_CYCLES * #slots))\000\000Fraction of slots retiring, useful work\000\0001percent of slots\000\000\000TopdownL1\00000"
/* offset=200873 */ "backend_core\000TopdownL2\000backend_bound / 100 - backend_memory\000\000Fraction of slots the CPU was stalled due to backend non-memory subsystem issues\000\000100%\000\000\000\00000"
/* offset=201026 */ "backend_memory\000TopdownL2\000(STALL_BACKEND_TLB + STALL_BACKEND_CACHE) / CPU_CYCLES\000\000Fraction of slots the CPU was stalled due to backend memory subsystem issues (cache/tlb miss)\000\000100%\000\000\000\00000"
/* offset=201212 */ "branch_mispredict\000TopdownL2\000BR_MIS_PRED_RETIRED / GPC_FLUSH * slots_lost_misspeculation_fraction\000\000Fraction of slots lost due to branch misprediciton\000\0001percent of slots\000\000\000\00000"
/* offset=201385 */ "frontend_bandwidth\000TopdownL2\000frontend_bound - frontend_latency\000\000Fraction of slots the CPU did not dispatch at full bandwidth - able to dispatch partial slots only (1, 2, or 3 uops)\000\0001percent of slots\000\000\000\00000"
/* offset=201590 */ "frontend_latency\000TopdownL2\000(STALL_FRONTEND - (STALL_SLOT_FRONTEND - frontend_bound / 100 * CPU_CYCLES * #slots) / #slots) / CPU_CYCLES * 100\000\000Fraction of slots the CPU was stalled due to frontend latency issues (cache/tlb miss); nothing to dispatch\000\0001percent of slots\000\000\000\00000"
/* offset=201863 */ "other_miss_pred\000TopdownL2\000slots_lost_misspeculation_fraction - branch_mispredict\000\000Fraction of slots lost due to other/non-branch misprediction misspeculation\000\0001percent of slots\000\000\000\00000"
/* offset=202045 */ "pipe_utilization\000TopdownL2\000100 * ((IXU_NUM_UOPS_ISSUED + FSU_ISSUED) / (CPU_CYCLES * 6))\000\000Fraction of execute slots utilized\000\0001percent of slots\000\000\000\00000"
/* offset=202194 */ "d_cache_l2_miss_rate\000TopdownL3\000STALL_BACKEND_MEM / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled due to data L2 cache miss\000\0001percent of cycles\000\000\000\00000"
/* offset=202353 */ "d_cache_miss_rate\000TopdownL3\000STALL_BACKEND_CACHE / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled due to data cache miss\000\0001percent of cycles\000\000\000\00000"
/* offset=202508 */ "d_tlb_miss_rate\000TopdownL3\000STALL_BACKEND_TLB / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled due to data TLB miss\000\0001percent of cycles\000\000\000\00000"
/* offset=202657 */ "fsu_pipe_utilization\000TopdownL3\000FSU_ISSUED / (CPU_CYCLES * 2) * 100\000\000Fraction of FSU execute slots utilized\000\0001percent of slots\000\000\000\00000"
/* offset=202788 */ "i_cache_miss_rate\000TopdownL3\000STALL_FRONTEND_CACHE / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled due to instruction cache miss\000\0001percent of slots\000\000\000\00000"
/* offset=202950 */ "i_tlb_miss_rate\000TopdownL3\000STALL_FRONTEND_TLB / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled due to instruction TLB miss\000\0001percent of slots\000\000\000\00000"
/* offset=203106 */ "ixu_pipe_utilization\000TopdownL3\000IXU_NUM_UOPS_ISSUED / (CPU_CYCLES * #slots) * 100\000\000Fraction of IXU execute slots utilized\000\0001percent of slots\000\000\000\00000"
/* offset=203251 */ "stall_recovery_rate\000TopdownL3\000IDR_STALL_FLUSH / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled due to flush recovery\000\0001percent of slots\000\000\000\00000"
/* offset=203402 */ "stall_fsu_sched_rate\000TopdownL4\000IDR_STALL_FSU_SCHED / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled and FSU was full\000\0001percent of cycles\000\000\000\00000"
/* offset=203554 */ "stall_ixu_sched_rate\000TopdownL4\000IDR_STALL_IXU_SCHED / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled and IXU was full\000\0001percent of cycles\000\000\000\00000"
/* offset=203706 */ "stall_lob_id_rate\000TopdownL4\000IDR_STALL_LOB_ID / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled and LOB was full\000\0001percent of cycles\000\000\000\00000"
/* offset=203852 */ "stall_rob_id_rate\000TopdownL4\000IDR_STALL_ROB_ID / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled and ROB was full\000\0001percent of cycles\000\000\000\00000"
/* offset=203998 */ "stall_sob_id_rate\000TopdownL4\000IDR_STALL_SOB_ID / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled and SOB was full\000\0001percent of cycles\000\000\000\00000"
/* offset=204144 */ "backend_stalled_cycles\000Cycle_Accounting\000STALL_BACKEND / CPU_CYCLES * 100\000\000This metric is the percentage of cycles that were stalled due to resource constraints in the backend unit of the processor\000\0001percent of cycles\000\000\000\00000"
/* offset=204366 */ "branch_misprediction_ratio\000Miss_Ratio;Branch_Effectiveness\000BR_MIS_PRED_RETIRED / BR_RETIRED\000\000This metric measures the ratio of branches mispredicted to the total number of branches architecturally executed. This gives an indication of the effectiveness of the branch prediction unit\000\0001per branch\000\000\000\00000"
/* offset=204667 */ "branch_mpki\000MPKI;Branch_Effectiveness\000BR_MIS_PRED_RETIRED / INST_RETIRED * 1e3\000\000This metric measures the number of branch mispredictions per thousand instructions executed\000\0001MPKI\000\000\000\00000"
/* offset=204851 */ "branch_percentage\000Operation_Mix\000(BR_IMMED_SPEC + BR_INDIRECT_SPEC) / INST_SPEC * 100\000\000This metric measures branch operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000"
/* offset=205058 */ "dtlb_mpki\000MPKI;DTLB_Effectiveness\000DTLB_WALK / INST_RETIRED * 1e3\000\000This metric measures the number of data TLB Walks per thousand instructions executed\000\0001MPKI\000\000\000\00000"
/* offset=205221 */ "dtlb_walk_ratio\000Miss_Ratio;DTLB_Effectiveness\000DTLB_WALK / L1D_TLB\000\000This metric measures the ratio of instruction TLB Walks to the total number of data TLB accesses. This gives an indication of the effectiveness of the data TLB accesses\000\0001per TLB access\000\000\000\00000"
/* offset=205479 */ "frontend_stalled_cycles\000Cycle_Accounting\000STALL_FRONTEND / CPU_CYCLES * 100\000\000This metric is the percentage of cycles that were stalled due to resource constraints in the frontend unit of the processor\000\0001percent of cycles\000\000\000\00000"
/* offset=205704 */ "itlb_mpki\000MPKI;ITLB_Effectiveness\000ITLB_WALK / INST_RETIRED * 1e3\000\000This metric measures the number of instruction TLB Walks per thousand instructions executed\000\0001MPKI\000\000\000\00000"
/* offset=205874 */ "l1d_cache_mpki\000MPKI;L1D_Cache_Effectiveness\000L1D_CACHE_REFILL / INST_RETIRED * 1e3\000\000This metric measures the number of level 1 data cache accesses missed per thousand instructions executed\000\0001MPKI\000\000\000\00000"
/* offset=206074 */ "l1d_tlb_miss_ratio\000Miss_Ratio;DTLB_Effectiveness\000L1D_TLB_REFILL / L1D_TLB\000\000This metric measures the ratio of level 1 data TLB accesses missed to the total number of level 1 data TLB accesses. This gives an indication of the effectiveness of the level 1 data TLB\000\0001per TLB access\000\000\000\00000"
/* offset=206358 */ "l1d_tlb_mpki\000MPKI;DTLB_Effectiveness\000L1D_TLB_REFILL / INST_RETIRED * 1e3\000\000This metric measures the number of level 1 instruction TLB accesses missed per thousand instructions executed\000\0001MPKI\000\000\000\00000"
/* offset=206554 */ "l1i_cache_mpki\000MPKI;L1I_Cache_Effectiveness\000L1I_CACHE_REFILL / INST_RETIRED * 1e3\000\000This metric measures the number of level 1 instruction cache accesses missed per thousand instructions executed\000\0001MPKI\000\000\000\00000"
/* offset=206761 */ "l1i_tlb_miss_ratio\000Miss_Ratio;ITLB_Effectiveness\000L1I_TLB_REFILL / L1I_TLB\000\000This metric measures the ratio of level 1 instruction TLB accesses missed to the total number of level 1 instruction TLB accesses. This gives an indication of the effectiveness of the level 1 instruction TLB\000\0001per TLB access\000\000\000\00000"
/* offset=207066 */ "l1i_tlb_mpki\000MPKI;ITLB_Effectiveness\000L1I_TLB_REFILL / INST_RETIRED * 1e3\000\000This metric measures the number of level 1 instruction TLB accesses missed per thousand instructions executed\000\0001MPKI\000\000\000\00000"
/* offset=207262 */ "l2_cache_mpki\000MPKI;L2_Cache_Effectiveness\000L2D_CACHE_REFILL / INST_RETIRED * 1e3\000\000This metric measures the number of level 2 unified cache accesses missed per thousand instructions executed. Note that cache accesses in this cache are either data memory access or instruction fetch as this is a unified cache\000\0001MPKI\000\000\000\00000"
/* offset=207581 */ "l2_tlb_miss_ratio\000Miss_Ratio;ITLB_Effectiveness;DTLB_Effectiveness\000L2D_TLB_REFILL / L2D_TLB\000\000This metric measures the ratio of level 2 unified TLB accesses missed to the total number of level 2 unified TLB accesses. This gives an indication of the effectiveness of the level 2 TLB\000\0001per TLB access\000\000\000\00000"
/* offset=207884 */ "l2_tlb_mpki\000MPKI;ITLB_Effectiveness;DTLB_Effectiveness\000L2D_TLB_REFILL / INST_RETIRED * 1e3\000\000This metric measures the number of level 2 unified TLB accesses missed per thousand instructions executed\000\0001MPKI\000\000\000\00000"
/* offset=208094 */ "ll_cache_read_hit_ratio\000LL_Cache_Effectiveness\000(LL_CACHE_RD - LL_CACHE_MISS_RD) / LL_CACHE_RD\000\000This metric measures the ratio of last level cache read accesses hit in the cache to the total number of last level cache accesses. This gives an indication of the effectiveness of the last level cache for read traffic. Note that cache accesses in this cache are either data memory access or instruction fetch as this is a system level cache\000\0001per cache access\000\000\000\00000"
/* offset=208555 */ "ll_cache_read_miss_ratio\000Miss_Ratio;LL_Cache_Effectiveness\000LL_CACHE_MISS_RD / LL_CACHE_RD\000\000This metric measures the ratio of last level cache read accesses missed to the total number of last level cache accesses. This gives an indication of the effectiveness of the last level cache for read traffic. Note that cache accesses in this cache are either data memory access or instruction fetch as this is a system level cache\000\0001per cache access\000\000\000\00000"
/* offset=209002 */ "ll_cache_read_mpki\000MPKI;LL_Cache_Effectiveness\000LL_CACHE_MISS_RD / INST_RETIRED * 1e3\000\000This metric measures the number of last level cache read accesses missed per thousand instructions executed\000\0001MPKI\000\000\000\00000"
/* offset=209208 */ "backend_bound\000Default;TopdownL1\000100 * (STALL_SLOT_BACKEND / (CPU_CYCLES * #slots) - BR_MIS_PRED * 3 / CPU_CYCLES)\000\000This metric is the percentage of total slots that were stalled due to resource constraints in the backend of the processor\000\0001percent of slots\000\000\000TopdownL1\00000"
/* offset=209479 */ "bad_speculation\000Default;TopdownL1\000100 * ((1 - OP_RETIRED / OP_SPEC) * (1 - (STALL_SLOT if strcmp_cpuid_str(0x410fd493) | strcmp_cpuid_str(0x410fd490) ^ 1 else STALL_SLOT - CPU_CYCLES) / (CPU_CYCLES * #slots)) + BR_MIS_PRED * 4 / CPU_CYCLES)\000\000This metric is the percentage of total slots that executed operations and didn't retire due to a pipeline flush.\nThis indicates cycles that were utilized but inefficiently\000\0001percent of slots\000\000\000TopdownL1\00000"
/* offset=209926 */ "frontend_bound\000Default;TopdownL1\000100 * ((STALL_SLOT_FRONTEND if strcmp_cpuid_str(0x410fd493) | strcmp_cpuid_str(0x410fd490) ^ 1 else STALL_SLOT_FRONTEND - CPU_CYCLES) / (CPU_CYCLES * #slots) - BR_MIS_PRED / CPU_CYCLES)\000\000This metric is the percentage of total slots that were stalled due to resource constraints in the frontend of the processor\000\0001percent of slots\000\000\000TopdownL1\00000"
/* offset=210303 */ "retiring\000Default;TopdownL1\000100 * (OP_RETIRED / OP_SPEC * (1 - (STALL_SLOT if strcmp_cpuid_str(0x410fd493) | strcmp_cpuid_str(0x410fd490) ^ 1 else STALL_SLOT - CPU_CYCLES) / (CPU_CYCLES * #slots)))\000\000This metric is the percentage of total slots that retired operations, which indicates cycles that were utilized efficiently\000\0001percent of slots\000\000\000TopdownL1\00000"
/* offset=210658 */ "l3d_cache_mpki\000MPKI;L3_Cache_Effectiveness\000L3D_CACHE_REFILL / INST_RETIRED * 1e3\000\000The rate of L3 D-Cache misses per kilo instructions\000\0001MPKI\000\000\000\00000"
/* offset=210804 */ "l3d_cache_miss_rate\000Miss_Ratio;L3_Cache_Effectiveness\000L3D_CACHE_REFILL / L3D_CACHE\000\000The rate of L3 D-Cache misses to the overall L3 D-Cache\000\000100%\000\000\000\00000"
/* offset=210955 */ "branch_pki\000MPKI;Branch_Effectiveness\000BR_RETIRED / INST_RETIRED * 1e3\000\000The rate of branches retired per kilo instructions\000\0001PKI\000\000\000\00000"
/* offset=211087 */ "ipc_rate\000General\000ipc / #slots\000\000IPC percentage of peak. The peak of IPC is the number of slots\000\000100%\000\000\000\00000"
/* offset=211192 */ "spec_ipc\000General\000INST_SPEC / CPU_CYCLES\000\000Speculatively executed Instructions Per Cycle (IPC)\000\000\000\000\000\00000"
/* offset=211292 */ "branch_immed_spec_rate\000Operation_Mix\000BR_IMMED_SPEC / INST_SPEC\000\000The rate of branch immediate instructions speculatively executed to overall instructions speculatively executed\000\000100%\000\000\000\00000"
/* offset=211479 */ "branch_return_spec_rate\000Operation_Mix\000BR_RETURN_SPEC / INST_SPEC\000\000The rate of procedure return instructions speculatively executed to overall instructions speculatively executed\000\000100%\000\000\000\00000"
/* offset=211668 */ "branch_indirect_spec_rate\000Operation_Mix\000BR_INDIRECT_SPEC / INST_SPEC\000\000The rate of indirect branch instructions speculatively executed to overall instructions speculatively executed\000\000100%\000\000\000\00000"
/* offset=211860 */ "imx8mm_ddr_read.all\000\000imx8mm_ddr.read_cycles * 4 * 4\000\000bytes all masters read from ddr based on read-cycles event\000\0009.765625e-4KB\000i.MX8MM\000\000\00000"
/* offset=211999 */ "imx8mm_ddr_write.all\000\000imx8mm_ddr.write_cycles * 4 * 4\000\000bytes all masters write to ddr based on write-cycles event\000\0009.765625e-4KB\000i.MX8MM\000\000\00000"
/* offset=212140 */ "imx8mn_ddr_read.all\000\000imx8mn_ddr.read_cycles * 4 * 2\000\000bytes all masters read from ddr based on read-cycles event\000\0009.765625e-4KB\000i.MX8MN\000\000\00000"
/* offset=212279 */ "imx8mn_ddr_write.all\000\000imx8mn_ddr.write_cycles * 4 * 2\000\000bytes all masters write to ddr based on write-cycles event\000\0009.765625e-4KB\000i.MX8MN\000\000\00000"
/* offset=212420 */ "imx8mp_ddr_read.all\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0xffff\\,axi_id\\=0x0000@\000\000bytes of all masters read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=212560 */ "imx8mp_ddr_write.all\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0xffff\\,axi_id\\=0x0000@\000\000bytes of all masters write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=212701 */ "imx8mp_ddr_read.a53\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0000@\000\000bytes of a53 core read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=212838 */ "imx8mp_ddr_write.a53\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0000@\000\000bytes of a53 core write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=212976 */ "imx8mp_ddr_read.supermix\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x000f\\,axi_id\\=0x0020@\000\000bytes of supermix(m7) core read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=213127 */ "imx8mp_ddr_write.supermix\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x000f\\,axi_id\\=0x0020@\000\000bytes of supermix(m7) write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=213274 */ "imx8mp_ddr_read.3d\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0070@\000\000bytes of gpu 3d read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=213408 */ "imx8mp_ddr_write.3d\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0070@\000\000bytes of gpu 3d write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=213543 */ "imx8mp_ddr_read.2d\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0071@\000\000bytes of gpu 2d read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=213677 */ "imx8mp_ddr_write.2d\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0071@\000\000bytes of gpu 2d write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=213812 */ "imx8mp_ddr_read.lcdif1\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0068@\000\000bytes of display lcdif1 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=213958 */ "imx8mp_ddr_write.lcdif1\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0068@\000\000bytes of display lcdif1 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=214105 */ "imx8mp_ddr_read.lcdif2\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0069@\000\000bytes of display lcdif2 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=214251 */ "imx8mp_ddr_write.lcdif2\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0069@\000\000bytes of display lcdif2 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=214398 */ "imx8mp_ddr_read.isi1\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x006a@\000\000bytes of display isi1 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=214540 */ "imx8mp_ddr_write.isi1\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x006a@\000\000bytes of display isi1 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=214683 */ "imx8mp_ddr_read.isi2\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x006b@\000\000bytes of display isi2 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=214825 */ "imx8mp_ddr_write.isi2\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x006b@\000\000bytes of display isi2 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=214968 */ "imx8mp_ddr_read.isi3\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x006c@\000\000bytes of display isi3 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=215110 */ "imx8mp_ddr_write.isi3\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x006c@\000\000bytes of display isi3 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=215253 */ "imx8mp_ddr_read.isp1\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x006d@\000\000bytes of display isp1 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=215395 */ "imx8mp_ddr_write.isp1\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x006d@\000\000bytes of display isp1 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=215538 */ "imx8mp_ddr_read.isp2\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x006e@\000\000bytes of display isp2 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=215680 */ "imx8mp_ddr_write.isp2\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x006e@\000\000bytes of display isp2 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=215823 */ "imx8mp_ddr_read.dewarp\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x006f@\000\000bytes of display dewarp read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=215969 */ "imx8mp_ddr_write.dewarp\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x006f@\000\000bytes of display dewarp write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=216116 */ "imx8mp_ddr_read.vpu1\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x007c@\000\000bytes of vpu1 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=216250 */ "imx8mp_ddr_write.vpu1\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x007c@\000\000bytes of vpu1 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=216385 */ "imx8mp_ddr_read.vpu2\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x007d@\000\000bytes of vpu2 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=216519 */ "imx8mp_ddr_write.vpu2\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x007d@\000\000bytes of vpu2 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=216654 */ "imx8mp_ddr_read.vpu3\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x007e@\000\000bytes of vpu3 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=216788 */ "imx8mp_ddr_write.vpu3\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x007e@\000\000bytes of vpu3 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=216923 */ "imx8mp_ddr_read.npu\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0073@\000\000bytes of npu read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=217055 */ "imx8mp_ddr_write.npu\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0073@\000\000bytes of npu write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=217188 */ "imx8mp_ddr_read.usb1\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0078@\000\000bytes of hsio usb1 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=217327 */ "imx8mp_ddr_write.usb1\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0078@\000\000bytes of hsio usb1 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=217467 */ "imx8mp_ddr_read.usb2\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0079@\000\000bytes of hsio usb2 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=217606 */ "imx8mp_ddr_write.usb2\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0079@\000\000bytes of hsio usb2 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=217746 */ "imx8mp_ddr_read.pci\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x007a@\000\000bytes of hsio pci read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=217883 */ "imx8mp_ddr_write.pci\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x007a@\000\000bytes of hsio pci write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=218021 */ "imx8mp_ddr_read.hdmi_hrv_mwr\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0074@\000\000bytes of hdmi_tx hrv_mwr read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=218174 */ "imx8mp_ddr_write.hdmi_hrv_mwr\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0074@\000\000bytes of hdmi_tx hrv_mwr write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=218328 */ "imx8mp_ddr_read.hdmi_lcdif\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0075@\000\000bytes of hdmi_tx lcdif read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=218477 */ "imx8mp_ddr_write.hdmi_lcdif\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0075@\000\000bytes of hdmi_tx lcdif write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=218627 */ "imx8mp_ddr_read.hdmi_hdcp\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0076@\000\000bytes of hdmi_tx tx_hdcp read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=218777 */ "imx8mp_ddr_write.hdmi_hdcp\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0076@\000\000bytes of hdmi_tx tx_hdcp write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=218928 */ "imx8mp_ddr_read.audio_dsp\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0041@\000\000bytes of audio dsp read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=219072 */ "imx8mp_ddr_write.audio_dsp\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0041@\000\000bytes of audio dsp write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=219217 */ "imx8mp_ddr_read.audio_sdma2_per\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0062@\000\000bytes of audio sdma2_per read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=219373 */ "imx8mp_ddr_write.audio_sdma2_per\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0062@\000\000bytes of audio sdma2_per write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=219530 */ "imx8mp_ddr_read.audio_sdma2_burst\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0063@\000\000bytes of audio sdma2_burst read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=219690 */ "imx8mp_ddr_write.audio_sdma2_burst\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0063@\000\000bytes of audio sdma2_burst write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=219851 */ "imx8mp_ddr_read.audio_sdma3_per\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0064@\000\000bytes of audio sdma3_per read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=220007 */ "imx8mp_ddr_write.audio_sdma3_per\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0064@\000\000bytes of audio sdma3_per write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=220164 */ "imx8mp_ddr_read.audio_sdma3_burst\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0065@\000\000bytes of audio sdma3_burst read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=220324 */ "imx8mp_ddr_write.audio_sdma3_burst\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0065@\000\000bytes of audio sdma3_burst write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=220485 */ "imx8mp_ddr_read.audio_sdma_pif\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0066@\000\000bytes of audio sdma_pif read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=220639 */ "imx8mp_ddr_write.audio_sdma_pif\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0066@\000\000bytes of audio sdma_pif write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000"
/* offset=220794 */ "imx8mq_ddr_read.all\000\000imx8mq_ddr.read_cycles * 4 * 4\000\000bytes all masters read from ddr based on read-cycles event\000\0009.765625e-4KB\000i.MX8MQ\000\000\00000"
/* offset=220933 */ "imx8mq_ddr_write.all\000\000imx8mq_ddr.write_cycles * 4 * 4\000\000bytes all masters write to ddr based on write-cycles event\000\0009.765625e-4KB\000i.MX8MQ\000\000\00000"
/* offset=221074 */ "ddr_read_bandwidth.all\000ali_drw\000hif_rd * 64 / 1e6 / duration_time\000\000The ddr read bandwidth(MB/s)\000\0001MB/s\000ali_drw_pmu\000\000\00000"
/* offset=221192 */ "ddr_write_bandwidth.all\000ali_drw\000(hif_wr + hif_rmw) * 64 / 1e6 / duration_time\000\000The ddr write bandwidth(MB/s)\000\0001MB/s\000ali_drw_pmu\000\000\00000"
/* offset=221324 */ "frontend_bound\000Default;TopDownL1\000FETCH_BUBBLE / (4 * CPU_CYCLES)\000\000Frontend bound L1 topdown metric\000Frontend bound L1 topdown metric\000\000\000\000TopDownL1\00000"
/* offset=221471 */ "bad_speculation\000Default;TopDownL1\000(INST_SPEC - INST_RETIRED) / (4 * CPU_CYCLES)\000\000Bad Speculation L1 topdown metric\000Bad Speculation L1 topdown metric\000\000\000\000TopDownL1\00000"
/* offset=221635 */ "retiring\000Default;TopDownL1\000INST_RETIRED / (CPU_CYCLES * 4)\000\000Retiring L1 topdown metric\000Retiring L1 topdown metric\000\000\000\000TopDownL1\00000"
/* offset=221764 */ "backend_bound\000Default;TopDownL1\0001 - (frontend_bound + bad_speculation + retiring)\000\000Backend Bound L1 topdown metric\000Backend Bound L1 topdown metric\000\000\000\000TopDownL1\00000"
/* offset=221926 */ "fetch_latency_bound\000TopDownL2\000armv8_pmuv3_0@event\\=0x201d@ / CPU_CYCLES\000\000Fetch latency bound L2 topdown metric\000Fetch latency bound L2 topdown metric\000\000\000\000\00000"
/* offset=222081 */ "fetch_bandwidth_bound\000TopDownL2\000frontend_bound - fetch_latency_bound\000\000Fetch bandwidth bound L2 topdown metric\000Fetch bandwidth bound L2 topdown metric\000\000\000\000\00000"
/* offset=222237 */ "branch_mispredicts\000TopDownL2\000bad_speculation * BR_MIS_PRED / (BR_MIS_PRED + armv8_pmuv3_0@event\\=0x2013@)\000\000Branch mispredicts L2 topdown metric\000Branch mispredicts L2 topdown metric\000\000\000\000\00000"
/* offset=222424 */ "machine_clears\000TopDownL2\000bad_speculation - branch_mispredicts\000\000Machine clears L2 topdown metric\000Machine clears L2 topdown metric\000\000\000\000\00000"
/* offset=222559 */ "core_bound\000TopDownL2\000(EXE_STALL_CYCLE - (MEM_STALL_ANYLOAD + armv8_pmuv3_0@event\\=0x7005@)) / CPU_CYCLES\000\000Core bound L2 topdown metric\000Core bound L2 topdown metric\000\000\000\000\00000"
/* offset=222729 */ "memory_bound\000TopDownL2\000(MEM_STALL_ANYLOAD + armv8_pmuv3_0@event\\=0x7005@) / CPU_CYCLES\000\000Memory bound L2 topdown metric\000Memory bound L2 topdown metric\000\000\000\000\00000"
/* offset=222885 */ "idle_by_itlb_miss\000TopDownL3\000((L2I_TLB - L2I_TLB_REFILL) * 15 + L2I_TLB_REFILL * 100) / CPU_CYCLES\000\000Idle by itlb miss L3 topdown metric\000Idle by itlb miss L3 topdown metric\000\000\000\000\00000"
/* offset=223062 */ "idle_by_icache_miss\000TopDownL3\000((L2I_CACHE - L2I_CACHE_REFILL) * 15 + L2I_CACHE_REFILL * 100) / CPU_CYCLES\000\000Idle by icache miss L3 topdown metric\000Idle by icache miss L3 topdown metric\000\000\000\000\00000"
/* offset=223251 */ "bp_misp_flush\000TopDownL3\000BR_MIS_PRED * 5 / CPU_CYCLES\000\000BP misp flush L3 topdown metric\000BP misp flush L3 topdown metric\000\000\000\000\00000"
/* offset=223375 */ "ooo_flush\000TopDownL3\000armv8_pmuv3_0@event\\=0x2013@ * 5 / CPU_CYCLES\000\000OOO flush L3 topdown metric\000OOO flush L3 topdown metric\000\000\000\000\00000"
/* offset=223504 */ "sp_flush\000TopDownL3\000armv8_pmuv3_0@event\\=0x1001@ * 5 / CPU_CYCLES\000\000Static predictor flush L3 topdown metric\000Static predictor flush L3 topdown metric\000\000\000\000\00000"
/* offset=223658 */ "indirect_branch\000TopDownL3\000armv8_pmuv3_0@event\\=0x1010@ / BR_MIS_PRED\000\000Indirect branch L3 topdown metric\000Indirect branch L3 topdown metric\000\000\000\000\00000"
/* offset=223802 */ "push_branch\000TopDownL3\000(armv8_pmuv3_0@event\\=0x1013@ + armv8_pmuv3_0@event\\=0x1016@) / BR_MIS_PRED\000\000Push branch L3 topdown metric\000Push branch L3 topdown metric\000\000\000\000\00000"
/* offset=223967 */ "pop_branch\000TopDownL3\000armv8_pmuv3_0@event\\=0x100d@ / BR_MIS_PRED\000\000Pop branch L3 topdown metric\000Pop branch L3 topdown metric\000\000\000\000\00000"
/* offset=224096 */ "other_branch\000TopDownL3\000(BR_MIS_PRED - armv8_pmuv3_0@event\\=0x1010@ - armv8_pmuv3_0@event\\=0x1013@ - armv8_pmuv3_0@event\\=0x1016@ - armv8_pmuv3_0@event\\=0x100d@) / BR_MIS_PRED\000\000Other branch L3 topdown metric\000Other branch L3 topdown metric\000\000\000\000\00000"
/* offset=224340 */ "nuke_flush\000TopDownL3\000armv8_pmuv3_0@event\\=0x2012@ / armv8_pmuv3_0@event\\=0x2013@\000\000Nuke flush L3 topdown metric\000Nuke flush L3 topdown metric\000\000\000\000\00000"
/* offset=224486 */ "other_flush\000TopDownL3\0001 - nuke_flush\000\000Other flush L3 topdown metric\000Other flush L3 topdown metric\000\000\000\000\00000"
/* offset=224590 */ "sync_stall\000TopDownL3\000armv8_pmuv3_0@event\\=0x2010@ / CPU_CYCLES\000\000Sync stall L3 topdown metric\000Sync stall L3 topdown metric\000\000\000\000\00000"
/* offset=224718 */ "rob_stall\000TopDownL3\000armv8_pmuv3_0@event\\=0x2004@ / CPU_CYCLES\000\000Rob stall L3 topdown metric\000Rob stall L3 topdown metric\000\000\000\000\00000"
/* offset=224843 */ "ptag_stall\000TopDownL3\000(armv8_pmuv3_0@event\\=0x2006@ + armv8_pmuv3_0@event\\=0x2007@ + armv8_pmuv3_0@event\\=0x2008@) / CPU_CYCLES\000\000Ptag stall L3 topdown metric\000Ptag stall L3 topdown metric\000\000\000\000\00000"
/* offset=225035 */ "saveopq_stall\000TopDownL3\000armv8_pmuv3_0@event\\=0x201e@ / CPU_CYCLES\000\000SaveOpQ stall L3 topdown metric\000SaveOpQ stall L3 topdown metric\000\000\000\000\00000"
/* offset=225172 */ "pc_buffer_stall\000TopDownL3\000armv8_pmuv3_0@event\\=0x2005@ / CPU_CYCLES\000\000PC buffer stall L3 topdown metric\000PC buffer stall L3 topdown metric\000\000\000\000\00000"
/* offset=225315 */ "divider\000TopDownL3\000armv8_pmuv3_0@event\\=0x7002@ / CPU_CYCLES\000\000Divider L3 topdown metric\000Divider L3 topdown metric\000\000\000\000\00000"
/* offset=225434 */ "fsu_stall\000TopDownL3\000armv8_pmuv3_0@event\\=0x7003@ / CPU_CYCLES\000\000FSU stall L3 topdown metric\000FSU stall L3 topdown metric\000\000\000\000\00000"
/* offset=225559 */ "exe_ports_util\000TopDownL3\000core_bound - divider - fsu_stall\000\000EXE ports util L3 topdown metric\000EXE ports util L3 topdown metric\000\000\000\000\00000"
/* offset=225690 */ "l1_bound\000TopDownL3\000(MEM_STALL_ANYLOAD - MEM_STALL_L1MISS) / CPU_CYCLES\000\000L1 bound L3 topdown metric\000L1 bound L3 topdown metric\000\000\000\000\00000"
/* offset=225822 */ "l2_bound\000TopDownL3\000(MEM_STALL_L1MISS - MEM_STALL_L2MISS) / CPU_CYCLES\000\000L2 bound L3 topdown metric\000L2 bound L3 topdown metric\000\000\000\000\00000"
/* offset=225953 */ "mem_bound\000TopDownL3\000MEM_STALL_L2MISS / CPU_CYCLES\000\000Mem bound L3 topdown metric\000Mem bound L3 topdown metric\000\000\000\000\00000"
/* offset=226066 */ "store_bound\000TopDownL3\000armv8_pmuv3_0@event\\=0x7005@ / CPU_CYCLES\000\000Store bound L3 topdown metric\000Store bound L3 topdown metric\000\000\000\000\00000"
/* offset=226197 */ "cpa_p1_avg_bw\000CPA\000(cpa_p1_wr_dat * 64 + cpa_p1_rd_dat_64b * 64 + cpa_p1_rd_dat_32b * 32) / cpa_cycles\000\000Average bandwidth of CPA Port 1\000\000\0000x00000030\000\000\00000"
/* offset=226349 */ "cpa_p0_avg_bw\000CPA\000(cpa_p0_wr_dat * 64 + cpa_p0_rd_dat_64b * 64 + cpa_p0_rd_dat_32b * 32) / cpa_cycles\000\000Average bandwidth of CPA Port 0\000\000\0000x00000030\000\000\00000"
/* offset=226501 */ "CPI\000\0001 / IPC\000\000\000\000\000\000\000\00000"
/* offset=226523 */ "IPC\000group1\000inst_retired.any / cpu_clk_unhalted.thread\000\000\000\000\000\000\000\00000"
/* offset=226586 */ "Frontend_Bound_SMT\000\000idq_uops_not_delivered.core / (4 * (cpu_clk_unhalted.thread / 2 * (1 + cpu_clk_unhalted.one_thread_active / cpu_clk_unhalted.ref_xclk)))\000\000\000\000\000\000\000\00000"
/* offset=226752 */ "dcache_miss_cpi\000\000l1d\\-loads\\-misses / inst_retired.any\000\000\000\000\000\000\000\00000"
/* offset=226816 */ "icache_miss_cycles\000\000l1i\\-loads\\-misses / inst_retired.any\000\000\000\000\000\000\000\00000"
/* offset=226883 */ "cache_miss_cycles\000group1\000dcache_miss_cpi + icache_miss_cycles\000\000\000\000\000\000\000\00000"
/* offset=226954 */ "DCache_L2_All_Hits\000\000l2_rqsts.demand_data_rd_hit + l2_rqsts.pf_hit + l2_rqsts.rfo_hit\000\000\000\000\000\000\000\00000"
/* offset=227048 */ "DCache_L2_All_Miss\000\000max(l2_rqsts.all_demand_data_rd - l2_rqsts.demand_data_rd_hit, 0) + l2_rqsts.pf_miss + l2_rqsts.rfo_miss\000\000\000\000\000\000\000\00000"
/* offset=227182 */ "DCache_L2_All\000\000DCache_L2_All_Hits + DCache_L2_All_Miss\000\000\000\000\000\000\000\00000"
/* offset=227246 */ "DCache_L2_Hits\000\000d_ratio(DCache_L2_All_Hits, DCache_L2_All)\000\000\000\000\000\000\000\00000"
/* offset=227314 */ "DCache_L2_Misses\000\000d_ratio(DCache_L2_All_Miss, DCache_L2_All)\000\000\000\000\000\000\000\00000"
/* offset=227384 */ "M1\000\000ipc + M2\000\000\000\000\000\000\000\00000"
/* offset=227406 */ "M2\000\000ipc + M1\000\000\000\000\000\000\000\00000"
/* offset=227428 */ "M3\000\0001 / M3\000\000\000\000\000\000\000\00000"
/* offset=227448 */ "L1D_Cache_Fill_BW\000\00064 * l1d.replacement / 1e9 / duration_time\000\000\000\000\000\000\000\00000"
;

static const struct compact_pmu_event pmu_events__ampere_ampereone_default_core[] = {
{ 22494 }, /* ase_spec\000instruction\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Operation speculatively executed, Advanced SIMD instruction\000 */
{ 8030 }, /* bpu_branch_no_hit\000core imp def\000Predictable branch speculatively executed, unpredicted\000event=0x115\000\00000\000Predictable branch speculatively executed, unpredicted\000 */
{ 9869 }, /* bpu_btb_update\000core imp def\000Predictable branch speculatively executed, unpredicted, that mispredict\000event=0x11c\000\00000\000Predictable branch speculatively executed, unpredicted, that mispredict\000 */
{ 7038 }, /* bpu_conditional_branch_hit_btb\000core imp def\000Predictable conditional branch speculatively executed that hit any level of BTB\000event=0x111\000\00000\000Predictable conditional branch speculatively executed that hit any level of BTB\000 */
{ 8411 }, /* bpu_conditional_branch_hit_btb_and_mispredict\000core imp def\000Predictable conditional branch speculatively executed that hit any level of BTB that (direction) mispredict\000event=0x117\000\00000\000Predictable conditional branch speculatively executed that hit any level of BTB that (direction) mispredict\000 */
{ 6861 }, /* bpu_hit_btb\000core imp def\000Predictable branch speculatively executed that hit any level of BTB\000event=0x110\000\00000\000Predictable branch speculatively executed that hit any level of BTB\000 */
{ 8187 }, /* bpu_hit_btb_and_mispredict\000core imp def\000Predictable branch speculatively executed that hit any level of BTB that mispredict\000event=0x116\000\00000\000Predictable branch speculatively executed that hit any level of BTB that mispredict\000 */
{ 7258 }, /* bpu_hit_indirect_predictor\000core imp def\000Predictable taken branch speculatively executed that hit any level of BTB that access the indirect predictor\000event=0x112\000\00000\000Predictable taken branch speculatively executed that hit any level of BTB that access the indirect predictor\000 */
{ 7532 }, /* bpu_hit_rsb\000core imp def\000Predictable taken branch speculatively executed that hit any level of BTB that access the return predictor\000event=0x113\000\00000\000Predictable taken branch speculatively executed that hit any level of BTB that access the return predictor\000 */
{ 9024 }, /* bpu_hit_rsb_and_mispredict\000core imp def\000Predictable taken branch speculatively executed that hit any level of BTB that access the return predictor that mispredict\000event=0x119\000\00000\000Predictable taken branch speculatively executed that hit any level of BTB that access the return predictor that mispredict\000 */
{ 8702 }, /* bpu_indirect_branch_hit_btb_and_mispredict\000core imp def\000Predictable taken branch speculatively executed that hit any level of BTB that access the indirect predictor that mispredict\000event=0x118\000\00000\000Predictable taken branch speculatively executed that hit any level of BTB that access the indirect predictor that mispredict\000 */
{ 9326 }, /* bpu_miss_rsb_and_mispredict\000core imp def\000Predictable taken branch speculatively executed that hit any level of BTB that access the overflow/underflow return predictor that mispredict\000event=0x11a\000\00000\000Predictable taken branch speculatively executed that hit any level of BTB that access the overflow/underflow return predictor that mispredict\000 */
{ 9667 }, /* bpu_no_prediction_mispredict\000core imp def\000Predictable branch speculatively executed, unpredicted, that mispredict\000event=0x11b\000\00000\000Predictable branch speculatively executed, unpredicted, that mispredict\000 */
{ 10057 }, /* bpu_rsb_full_stall\000core imp def\000Count predict pipe stalls due to speculative return address predictor full\000event=0x11d\000\00000\000Count predict pipe stalls due to speculative return address predictor full\000 */
{ 7787 }, /* bpu_unconditional_branch_miss_btb\000core imp def\000Predictable unconditional branch speculatively executed that did not hit any level of BTB\000event=0x114\000\00000\000Predictable unconditional branch speculatively executed that did not hit any level of BTB\000 */
{ 22969 }, /* br_immed_retired\000instruction\000Instruction architecturally executed, immediate branch\000event=0xd\000\00000\000Instruction architecturally executed, immediate branch\000 */
{ 13 }, /* br_immed_spec\000branch\000Branch speculatively executed, immediate branch\000event=0x78\000\00000\000Branch speculatively executed, immediate branch\000 */
{ 278 }, /* br_indirect_spec\000branch\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000 */
{ 411 }, /* br_mis_pred\000branch\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000Mispredicted or not predicted branch speculatively executed\000 */
{ 25146 }, /* br_mis_pred_retired\000instruction\000Instruction architecturally executed, mispredicted branch\000event=0x22\000\00000\000Instruction architecturally executed, mispredicted branch\000 */
{ 565 }, /* br_pred\000branch\000Predictable branch speculatively executed\000event=0x12\000\00000\000Predictable branch speculatively executed\000 */
{ 25018 }, /* br_retired\000instruction\000Instruction architecturally executed, branch\000event=0x21\000\00000\000Instruction architecturally executed, branch\000 */
{ 23122 }, /* br_return_retired\000instruction\000Instruction architecturally executed, condition code check pass, procedure return\000event=0xe\000\00000\000Instruction architecturally executed, condition code check pass, procedure return\000 */
{ 145 }, /* br_return_spec\000branch\000Branch speculatively executed, procedure return\000event=0x79\000\00000\000Branch speculatively executed, procedure return\000 */
{ 16955 }, /* breakpoint_match\000core imp def\000GPC detected a Breakpoint instruction match\000event=0xd501\000\00000\000GPC detected a Breakpoint instruction match\000 */
{ 1311 }, /* bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000Attributable Bus access\000 */
{ 1153 }, /* bus_access_normal\000bus\000Bus access, Normal\000event=0x64\000\00000\000Bus access, Normal\000 */
{ 1022 }, /* bus_access_not_shared\000bus\000Bus access, not Normal, Cacheable, Shareable\000event=0x63\000\00000\000Bus access, not Normal, Cacheable, Shareable\000 */
{ 1228 }, /* bus_access_periph\000bus\000Bus access, peripheral\000event=0x65\000\00000\000Bus access, peripheral\000 */
{ 771 }, /* bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Bus access read\000 */
{ 903 }, /* bus_access_shared\000bus\000Bus access, Normal, Cacheable, Shareable\000event=0x62\000\00000\000Bus access, Normal, Cacheable, Shareable\000 */
{ 836 }, /* bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Bus access write\000 */
{ 721 }, /* bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000Bus cycle\000 */
{ 24272 }, /* cid_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000 */
{ 1389 }, /* cnt_cycles\000bus\000Constant frequency cycles\000event=0x4004\000\00000\000Constant frequency cycles.  The counter increments at a constant frequency equal to the rate of increment of the system counter, CNTPCT_EL0\000 */
{ 679 }, /* cpu_cycles\000bus\000Cycle\000event=0x11\000\00000\000Cycle\000 */
{ 23330 }, /* crypto_spec\000instruction\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Operation speculatively executed, Cryptographic instruction\000 */
{ 23705 }, /* dmb_spec\000instruction\000Barrier speculatively executed, DMB\000event=0x7e\000\00000\000Barrier speculatively executed, DMB\000 */
{ 22343 }, /* dp_spec\000instruction\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Operation speculatively executed, integer data processing\000 */
{ 23597 }, /* dsb_spec\000instruction\000Barrier speculatively executed, DSB\000event=0x7d\000\00000\000Barrier speculatively executed, DSB\000 */
{ 4626 }, /* dtlb_walk\000cache\000Access to data TLB causes a translation table walk\000event=0x34\000\00000\000Access to data TLB causes a translation table walk\000 */
{ 20002 }, /* exc_dabort\000exception\000Exception taken, Data Abort and SError\000event=0x84\000\00000\000Exception taken, Data Abort and SError\000 */
{ 20191 }, /* exc_fiq\000exception\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000 */
{ 20266 }, /* exc_hvc\000exception\000Exception taken, Hypervisor Call\000event=0x8a\000\00000\000Exception taken, Hypervisor Call\000 */
{ 20116 }, /* exc_irq\000exception\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000 */
{ 19896 }, /* exc_pabort\000exception\000Exception taken, Instruction Abort\000event=0x83\000\00000\000Exception taken, Instruction Abort\000 */
{ 21097 }, /* exc_return\000exception\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Instruction architecturally executed, condition check pass, exception return\000 */
{ 19797 }, /* exc_svc\000exception\000Exception taken, Supervisor Call\000event=0x82\000\00000\000Exception taken, Supervisor Call\000 */
{ 21031 }, /* exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Exception taken\000 */
{ 20512 }, /* exc_trap_dabort\000exception\000Exception taken, Data Abort or SError not taken locally\000event=0x8c\000\00000\000Exception taken, Data Abort or SError not taken locally\000 */
{ 20915 }, /* exc_trap_fiq\000exception\000Exception taken, FIQ not taken locally\000event=0x8f\000\00000\000Exception taken, FIQ not taken locally\000 */
{ 20799 }, /* exc_trap_irq\000exception\000Exception taken, IRQ not taken locally\000event=0x8e\000\00000\000Exception taken, IRQ not taken locally\000 */
{ 20665 }, /* exc_trap_other\000exception\000Exception taken, Other traps not taken locally\000event=0x8d\000\00000\000Exception taken, Other traps not taken locally\000 */
{ 20365 }, /* exc_trap_pabort\000exception\000Exception taken, Instruction Abort not taken locally\000event=0x8b\000\00000\000Exception taken, Instruction Abort not taken locally\000 */
{ 19692 }, /* exc_undef\000exception\000Exception taken, Other synchronous\000event=0x81\000\00000\000Exception taken, Other synchronous\000 */
{ 16634 }, /* fsu_cancel\000core imp def\000Uops canceled (load cancels)\000event=0xd404\000\00000\000Uops canceled (load cancels)\000 */
{ 16733 }, /* fsu_div_sqrt_stall\000core imp def\000Count scheduler stalls due to divide/sqrt\000event=0xd405\000\00000\000Count scheduler stalls due to divide/sqrt\000 */
{ 16265 }, /* fsu_fsx_issued\000core imp def\000Uops issued by the scheduler on pipe X\000event=0xd401\000\00000\000Uops issued by the scheduler on pipe X\000 */
{ 16388 }, /* fsu_fsy_issued\000core imp def\000Uops issued by the scheduler on pipe Y\000event=0xd402\000\00000\000Uops issued by the scheduler on pipe Y\000 */
{ 16511 }, /* fsu_fsz_issued\000core imp def\000Uops issued by the scheduler on pipe Z\000event=0xd403\000\00000\000Uops issued by the scheduler on pipe Z\000 */
{ 16158 }, /* fsu_issued\000core imp def\000Uops issued by the FSU scheduler\000event=0xd400\000\00000\000Uops issued by the FSU scheduler\000 */
{ 10369 }, /* gpc_flush\000core imp def\000Flushes\000event=0x120\000\00000\000Flushes\000 */
{ 10424 }, /* gpc_flush_mem_fault\000core imp def\000Flushes due to memory hazards\000event=0x121\000\00000\000Flushes due to memory hazards\000 */
{ 16866 }, /* gpc_swob_drain\000core imp def\000Number of SWOB drains\000event=0xd500\000\00000\000Number of SWOB drains\000 */
{ 11191 }, /* icf_early_mis_pred\000core imp def\000Early mispredict\000event=0xd100\000\00000\000Early mispredict\000 */
{ 11274 }, /* icf_feq_full\000core imp def\000FEQ full cycles\000event=0xd101\000\00000\000FEQ full cycles\000 */
{ 11349 }, /* icf_inst_fifo_full\000core imp def\000Instruction FIFO Full\000event=0xd102\000\00000\000Instruction FIFO Full\000 */
{ 10255 }, /* icf_inst_spec_decode\000core imp def\000Macro-ops speculatively decoded\000event=0x11f\000\00000\000Macro-ops speculatively decoded\000 */
{ 11635 }, /* icf_pc_fifo_full\000core imp def\000PC FIFO Full\000event=0xd105\000\00000\000PC FIFO Full\000 */
{ 11511 }, /* icf_stall\000core imp def\000ICF sent 0 instructions to IDR this cycle\000event=0xd104\000\00000\000ICF sent 0 instructions to IDR this cycle\000 */
{ 11708 }, /* idr_stall_bob_id\000core imp def\000Stall due to BOB ID\000event=0xd200\000\00000\000Stall due to BOB ID\000 */
{ 12416 }, /* idr_stall_flush\000core imp def\000Dispatch stall due to flush (6 cycles)\000event=0xd206\000\00000\000Dispatch stall due to flush (6 cycles)\000 */
{ 12163 }, /* idr_stall_fsu_sched\000core imp def\000Dispatch stall due to FSU scheduler entries\000event=0xd204\000\00000\000Dispatch stall due to FSU scheduler entries\000 */
{ 12025 }, /* idr_stall_ixu_sched\000core imp def\000Dispatch stall due to IXU scheduler entries\000event=0xd203\000\00000\000Dispatch stall due to IXU scheduler entries\000 */
{ 11795 }, /* idr_stall_lob_id\000core imp def\000Dispatch stall due to LOB entries\000event=0xd201\000\00000\000Dispatch stall due to LOB entries\000 */
{ 12301 }, /* idr_stall_rob_id\000core imp def\000Dispatch stall due to ROB entries\000event=0xd205\000\00000\000Dispatch stall due to ROB entries\000 */
{ 11910 }, /* idr_stall_sob_id\000core imp def\000Dispatch stall due to SOB entries\000event=0xd202\000\00000\000Dispatch stall due to SOB entries\000 */
{ 13148 }, /* idr_stall_swob_full\000core imp def\000Number of SWOB drains triggered by system register write when SWOB full\000event=0xd20a\000\00000\000Number of SWOB drains triggered by system register write when SWOB full\000 */
{ 12775 }, /* idr_stall_swob_raw\000core imp def\000Number of SWOB drains triggered by system register or special-purpose register read-after-write or specific special-purpose register writes that cause SWOB drain\000event=0xd209\000\00000\000Number of SWOB drains triggered by system register or special-purpose register read-after-write or specific special-purpose register writes that cause SWOB drain\000 */
{ 12636 }, /* idr_stall_swob_timeout\000core imp def\000Number of SWOB drains triggered by timeout\000event=0xd208\000\00000\000Number of SWOB drains triggered by timeout\000 */
{ 12540 }, /* idr_stall_wfi\000core imp def\000Dispatch stall due to WFI\000event=0xd207\000\00000\000Dispatch stall due to WFI\000 */
{ 24159 }, /* inst_retired\000instruction\000Instruction architecturally executed\000event=0x8\000\00000\000Instruction architecturally executed\000 */
{ 24711 }, /* inst_spec\000instruction\000Operation speculatively executed\000event=0x1b\000\00000\000Operation speculatively executed\000 */
{ 23489 }, /* isb_spec\000instruction\000Barrier speculatively executed, ISB\000event=0x7c\000\00000\000Barrier speculatively executed, ISB\000 */
{ 4759 }, /* itlb_walk\000cache\000Access to instruction TLB that causes a translation table walk\000event=0x35\000\00000\000Access to instruction TLB that causes a translation table walk\000 */
{ 14161 }, /* ixu_issue_cancel\000core imp def\000Any uop issued was canceled for any reason\000event=0xd301\000\00000\000Any uop issued was canceled for any reason\000 */
{ 14618 }, /* ixu_ixa_issued\000core imp def\000Uops issued by the scheduler on IXA\000event=0xd304\000\00000\000Uops issued by the scheduler on IXA\000 */
{ 14735 }, /* ixu_ixa_par0_issued\000core imp def\000Uops issued by the scheduler on IXA Par 0\000event=0xd305\000\00000\000Uops issued by the scheduler on IXA Par 0\000 */
{ 14869 }, /* ixu_ixa_par1_issued\000core imp def\000Uops issued by the scheduler on IXA Par 1\000event=0xd306\000\00000\000Uops issued by the scheduler on IXA Par 1\000 */
{ 15003 }, /* ixu_ixb_issued\000core imp def\000Uops issued by the scheduler on IXB\000event=0xd307\000\00000\000Uops issued by the scheduler on IXB\000 */
{ 15120 }, /* ixu_ixb_par0_issued\000core imp def\000Uops issued by the scheduler on IXB Par 0\000event=0xd308\000\00000\000Uops issued by the scheduler on IXB Par 0\000 */
{ 15254 }, /* ixu_ixb_par1_issued\000core imp def\000Uops issued by the scheduler on IXB Par 1\000event=0xd309\000\00000\000Uops issued by the scheduler on IXB Par 1\000 */
{ 15388 }, /* ixu_ixc_issued\000core imp def\000Uops issued by the scheduler on IXC\000event=0xd30a\000\00000\000Uops issued by the scheduler on IXC\000 */
{ 15505 }, /* ixu_ixc_par0_issued\000core imp def\000Uops issued by the scheduler on IXC Par 0\000event=0xd30b\000\00000\000Uops issued by the scheduler on IXC Par 0\000 */
{ 15639 }, /* ixu_ixc_par1_issued\000core imp def\000Uops issued by the scheduler on IXC Par 1\000event=0xd30c\000\00000\000Uops issued by the scheduler on IXC Par 1\000 */
{ 15773 }, /* ixu_ixd_issued\000core imp def\000Uops issued by the scheduler on IXD\000event=0xd30d\000\00000\000Uops issued by the scheduler on IXD\000 */
{ 15890 }, /* ixu_ixd_par0_issued\000core imp def\000Uops issued by the scheduler on IXD Par 0\000event=0xd30e\000\00000\000Uops issued by the scheduler on IXD Par 0\000 */
{ 16024 }, /* ixu_ixd_par1_issued\000core imp def\000Uops issued by the scheduler on IXD Par 1\000event=0xd30f\000\00000\000Uops issued by the scheduler on IXD Par 1\000 */
{ 14294 }, /* ixu_load_cancel\000core imp def\000A load wakeup to the scheduler has been cancelled\000event=0xd302\000\00000\000A load wakeup to the scheduler has been cancelled\000 */
{ 14037 }, /* ixu_num_uops_issued\000core imp def\000Instructions issued by the scheduler\000event=0xd300\000\00000\000Instructions issued by the scheduler\000 */
{ 14440 }, /* ixu_slow_cancel\000core imp def\000The scheduler had to cancel one slow Uop due to resource conflict\000event=0xd303\000\00000\000The scheduler had to cancel one slow Uop due to resource conflict\000 */
{ 17910 }, /* l1_pfetch_dist_inc\000core imp def\000L1 prefetcher, distance was increased\000event=0xd60a\000\00000\000L1 prefetcher, distance was increased\000 */
{ 17793 }, /* l1_pfetch_dist_rst\000core imp def\000L1 prefetcher, distance was reset\000event=0xd609\000\00000\000L1 prefetcher, distance was reset\000 */
{ 18035 }, /* l1_pfetch_entry_trained\000core imp def\000L1 prefetcher, table entry is trained\000event=0xd60b\000\00000\000L1 prefetcher, table entry is trained\000 */
{ 17656 }, /* l1_pfetch_l2_req\000core imp def\000L1 prefetcher, load prefetch to L2 generated\000event=0xd608\000\00000\000L1 prefetcher, load prefetch to L2 generated\000 */
{ 17502 }, /* l1_pfetch_ld_fill\000core imp def\000L1 prefetcher, load prefetch fills into the L1 cache\000event=0xd607\000\00000\000L1 prefetcher, load prefetch fills into the L1 cache\000 */
{ 17359 }, /* l1_pfetch_ld_gen\000core imp def\000L1 prefetcher, load prefetch requests generated\000event=0xd606\000\00000\000L1 prefetcher, load prefetch requests generated\000 */
{ 3313 }, /* l1d_cache\000cache\000Level 1 data cache access\000event=0x4\000\00000\000Level 1 data cache access\000 */
{ 1902 }, /* l1d_cache_inval\000cache\000L1D cache invalidate\000event=0x48\000\00000\000Attributable Level 1 data cache invalidate\000 */
{ 4916 }, /* l1d_cache_lmiss_rd\000cache\000Level 1 data cache long-latency read miss\000event=0x39\000\00000\000Level 1 data cache long-latency read miss.  The counter counts each memory read access counted by L1D_CACHE that incurs additional latency because it returns data from outside the Level 1 data or unified cache of this processing element\000 */
{ 1587 }, /* l1d_cache_rd\000cache\000L1D cache access, read\000event=0x40\000\00000\000Attributable Level 1 data cache access, read\000 */
{ 3224 }, /* l1d_cache_refill\000cache\000Level 1 data cache refill\000event=0x3\000\00000\000Level 1 data cache refill\000 */
{ 1793 }, /* l1d_cache_refill_rd\000cache\000L1D cache refill, read\000event=0x42\000\00000\000Attributable Level 1 data cache refill, read\000 */
{ 1689 }, /* l1d_cache_wr\000cache\000L1D cache access, write\000event=0x41\000\00000\000Attributable Level 1 data cache access, write\000 */
{ 3908 }, /* l1d_tlb\000cache\000Attributable Level 1 data or unified TLB access\000event=0x25\000\00000\000Attributable Level 1 data or unified TLB access\000 */
{ 17090 }, /* l1d_tlb_miss\000core imp def\000L1D TLB miss\000event=0xd600\000\00000\000L1D TLB miss\000 */
{ 3395 }, /* l1d_tlb_refill\000cache\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Attributable Level 1 data TLB refill\000 */
{ 2003 }, /* l1d_tlb_refill_rd\000cache\000L1D tlb refill, read\000event=0x4c\000\00000\000Attributable Level 1 data TLB refill, read\000 */
{ 2106 }, /* l1d_tlb_refill_wr\000cache\000L1D tlb refill, write\000event=0x4d\000\00000\000Attributable Level 1 data TLB refill, write\000 */
{ 3504 }, /* l1i_cache\000cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Attributable Level 1 instruction cache access\000 */
{ 5235 }, /* l1i_cache_lmiss\000cache\000Level 1 instruction cache long-latency read miss\000event=0x4006\000\00000\000Level 1 instruction cache long-latency read miss.  If the L1I_CACHE_RD event is implemented, the counter counts each access counted by L1I_CACHE_RD that incurs additional latency because it returns instructions from outside of the Level 1 instruction cache of this PE.  If the L1I_CACHE_RD event is not implemented, the counter counts each access counted by L1I_CACHE that incurs additional latency because it returns instructions from outside the Level 1 instruction cache of this PE.  The event indicates to software that the access missed in the Level 1 instruction cache and might have a significant performance impact due to the additional latency, compared to the latency of an access that hits in the Level 1 instruction cache\000 */
{ 2998 }, /* l1i_cache_refill\000cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Level 1 instruction cache refill\000 */
{ 4033 }, /* l1i_tlb\000cache\000Attributable Level 1 instruction TLB access\000event=0x26\000\00000\000Attributable Level 1 instruction TLB access\000 */
{ 11442 }, /* l1i_tlb_miss\000core imp def\000L1I TLB miss\000event=0xd103\000\00000\000L1I TLB miss\000 */
{ 3101 }, /* l1i_tlb_refill\000cache\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Attributable Level 1 instruction TLB refill\000 */
{ 6604 }, /* l2_prefetch_refill\000core imp def\000Level 2 prefetch requests, refilled to L2 cache\000event=0x10a\000\00000\000Level 2 prefetch requests, refilled to L2 cache\000 */
{ 18676 }, /* l2_prefetch_req\000core imp def\000L2 prefetcher, load prefetch requests generated\000event=0xd703\000\00000\000L2 prefetcher, load prefetch requests generated\000 */
{ 6748 }, /* l2_prefetch_upgrade\000core imp def\000Level 2 prefetch requests, late\000event=0x10b\000\00000\000Level 2 prefetch requests, late\000 */
{ 10918 }, /* l2c_dat_crd_stall\000core imp def\000L2 TXDAT LCRD blocked\000event=0x169\000\00000\000L2 TXDAT LCRD blocked\000 */
{ 18576 }, /* l2c_data_refill\000core imp def\000L2 refill from D-side miss\000event=0xd702\000\00000\000L2 refill from D-side miss\000 */
{ 18476 }, /* l2c_inst_refill\000core imp def\000L2 refill from I-side miss\000event=0xd701\000\00000\000L2 refill from I-side miss\000 */
{ 18392 }, /* l2c_pipe_replay\000core imp def\000L2 pipeline replay\000event=0xd700\000\00000\000L2 pipeline replay\000 */
{ 11100 }, /* l2c_req_crd_stall\000core imp def\000L2 TXREQ LCRD blocked\000event=0x16b\000\00000\000L2 TXREQ LCRD blocked\000 */
{ 11009 }, /* l2c_rsp_crd_stall\000core imp def\000L2 TXRSP LCRD blocked\000event=0x16a\000\00000\000L2 TXRSP LCRD blocked\000 */
{ 10849 }, /* l2c_snoop\000core imp def\000Bus request sn\000event=0x156\000\00000\000Bus request sn\000 */
{ 3627 }, /* l2d_cache\000cache\000Level 2 data cache access\000event=0x16\000\00000\000Level 2 data cache access\000 */
{ 2897 }, /* l2d_cache_inval\000cache\000L2D cache invalidate\000event=0x58\000\00000\000Attributable Level 2 data cache invalidate\000 */
{ 6057 }, /* l2d_cache_lmiss_rd\000cache\000Level 2 data cache long-latency read miss\000event=0x4009\000\00000\000Level 2 data cache long-latency read miss.  The counter counts each memory read access counted by L2D_CACHE that incurs additional latency because it returns data from outside the Level 2 data or unified cache of this processing element.  The event indicates to software that the access missed in the Level 2 data or unified cache and might have a significant performance impact compared to the latency of an access that hits in the Level 2 data or unified cache\000 */
{ 2211 }, /* l2d_cache_rd\000cache\000L2D cache access, read\000event=0x50\000\00000\000Attributable Level 2 data cache access, read\000 */
{ 3710 }, /* l2d_cache_refill\000cache\000Level 2 data refill\000event=0x17\000\00000\000Level 2 data refill\000 */
{ 2417 }, /* l2d_cache_refill_rd\000cache\000L2D cache refill, read\000event=0x52\000\00000\000Attributable Level 2 data cache refill, read\000 */
{ 2526 }, /* l2d_cache_refill_wr\000cache\000L2D cache refill, write\000event=0x53\000\00000\000Attributable Level 2 data cache refill, write\000 */
{ 3788 }, /* l2d_cache_wb\000cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Attributable Level 2 data cache write-back\000 */
{ 2758 }, /* l2d_cache_wb_clean\000cache\000L2D cache Write-Back, cleaning and coherency\000event=0x57\000\00000\000Level 2 data cache Write-Back, cleaning and coherency\000 */
{ 2637 }, /* l2d_cache_wb_victim\000cache\000L2D cache Write-Back, victim\000event=0x56\000\00000\000Attributable Level 2 data cache Write-Back, victim\000 */
{ 2313 }, /* l2d_cache_wr\000cache\000L2D cache access, write\000event=0x51\000\00000\000Attributable Level 2 data cache access, write\000 */
{ 4384 }, /* l2d_tlb\000cache\000Attributable Level 2 data or unified TLB access\000event=0x2f\000\00000\000Attributable Level 2 data or unified TLB access\000 */
{ 4150 }, /* l2d_tlb_refill\000cache\000Attributable Level 2 data TLB refill\000event=0x2d\000\00000\000Attributable Level 2 data TLB refill\000 */
{ 4509 }, /* l2i_tlb\000cache\000Attributable Level 2 instruction TLB access\000event=0x30\000\00000\000Attributable Level 2 instruction TLB access\000 */
{ 4260 }, /* l2i_tlb_refill\000cache\000Attributable Level 2 instruction TLB refill\000event=0x2e\000\00000\000Attributable Level 2 instruction TLB refill\000 */
{ 26884 }, /* ld_align_lat\000memory\000Load with additional latency from alignment\000event=0x4021\000\00000\000Load with additional latency from alignment\000 */
{ 17230 }, /* ld_from_st_fwd\000core imp def\000Load satisified from store forwarded data\000event=0xd605\000\00000\000Load satisified from store forwarded data\000 */
{ 26288 }, /* ld_retired\000memory\000Instruction architecturally executed, condition code check pass, load\000event=0x6\000\00000\000Instruction architecturally executed, condition code check pass, load\000 */
{ 21982 }, /* ld_spec\000instruction\000Operation speculatively executed, load\000event=0x70\000\00000\000Operation speculatively executed, load\000 */
{ 25658 }, /* ldrex_spec\000intrinsic\000Exclusive operation speculatively executed, LDREX or LDX\000event=0x6c\000\00000\000Exclusive operation speculatively executed, LDREX or LDX\000 */
{ 27280 }, /* ldst_align_lat\000memory\000Access with additional latency from alignment\000event=0x4020\000\00000\000Access with additional latency from alignment\000 */
{ 22210 }, /* ldst_spec\000instruction\000Operation speculatively executed, load or store\000event=0x72\000\00000\000Operation speculatively executed, load or store\000 */
{ 18165 }, /* lsu_st_retire_stall\000core imp def\000Store retirement pipe stall\000event=0xd60c\000\00000\000Store retirement pipe stall\000 */
{ 27136 }, /* mem_access\000memory\000Data memory access\000event=0x13\000\00000\000Data memory access\000 */
{ 27411 }, /* mem_access_checked\000memory\000Checked data memory access\000event=0x4024\000\00000\000Checked data memory access\000 */
{ 27508 }, /* mem_access_checked_rd\000memory\000Checked data memory access, read\000event=0x4025\000\00000\000Checked data memory access, read\000 */
{ 27620 }, /* mem_access_checked_wr\000memory\000Checked data memory access, write\000event=0x4026\000\00000\000Checked data memory access, write\000 */
{ 26460 }, /* mem_access_rd\000memory\000Data memory access, read\000event=0x66\000\00000\000Data memory access, read\000 */
{ 26546 }, /* mem_access_wr\000memory\000Data memory access, write\000event=0x67\000\00000\000Data memory access, write\000 */
{ 27207 }, /* memory_error\000memory\000Local memory error\000event=0x1a\000\00000\000Local memory error\000 */
{ 18818 }, /* mmu_d_otb_alloc\000core imp def\000L2D OTB allocate\000event=0xd800\000\00000\000L2D OTB allocate\000 */
{ 18898 }, /* mmu_d_s1_walk_fault\000core imp def\000D-side Stage1 tablewalk fault\000event=0xd80b\000\00000\000D-side Stage1 tablewalk fault\000 */
{ 19008 }, /* mmu_d_s2_walk_fault\000core imp def\000D-side Stage2 tablewalk fault\000event=0xd80c\000\00000\000D-side Stage2 tablewalk fault\000 */
{ 19118 }, /* mmu_d_walk_steps\000core imp def\000D-side Tablewalk steps or descriptor fetches\000event=0xd80d\000\00000\000D-side Tablewalk steps or descriptor fetches\000 */
{ 19255 }, /* mmu_i_otb_alloc\000core imp def\000L2I OTB allocate\000event=0xd900\000\00000\000L2I OTB allocate\000 */
{ 19335 }, /* mmu_i_s1_walk_fault\000core imp def\000I-side Stage1 tablewalk fault\000event=0xd90b\000\00000\000I-side Stage1 tablewalk fault\000 */
{ 19445 }, /* mmu_i_s2_walk_fault\000core imp def\000I-side Stage2 tablewalk fault\000event=0xd90c\000\00000\000I-side Stage2 tablewalk fault\000 */
{ 19555 }, /* mmu_i_walk_steps\000core imp def\000I-side Tablewalk steps or descriptor fetches\000event=0xd90d\000\00000\000I-side Tablewalk steps or descriptor fetches\000 */
{ 10533 }, /* msc_etm_extout0\000core imp def\000ETM extout bit 0\000event=0x141\000\00000\000ETM extout bit 0\000 */
{ 10612 }, /* msc_etm_extout1\000core imp def\000ETM extout bit 1\000event=0x142\000\00000\000ETM extout bit 1\000 */
{ 10691 }, /* msc_etm_extout2\000core imp def\000ETM extout bit 2\000event=0x143\000\00000\000ETM extout bit 2\000 */
{ 10770 }, /* msc_etm_extout3\000core imp def\000ETM extout bit 3\000event=0x144\000\00000\000ETM extout bit 3\000 */
{ 25555 }, /* nop_spec\000instruction\000Speculatively executed, NOP\000event=0x100\000\00000\000Operation speculatively executed, NOP\000 */
{ 17159 }, /* ofb_full\000core imp def\000OFB full cycles\000event=0xd601\000\00000\000OFB full cycles\000 */
{ 25309 }, /* op_retired\000instruction\000Micro-operation architecturally executed\000event=0x3a\000\00000\000Micro-operation architecturally executed.  The counter counts each operation counted by OP_SPEC that would be executed in a simple sequential execution of the program\000 */
{ 21665 }, /* op_spec\000instruction\000Micro-operation speculatively executed\000event=0x3b\000\00000\000Micro-operation speculatively executed.  The counter counts the number of operations executed by the processing element, including those that are executed speculatively and would not be executed in a simple sequential execution of the program\000 */
{ 21665 }, /* op_spec\000instruction\000Micro-operation speculatively executed\000event=0x3b\000\00000\000Micro-operation speculatively executed.  The counter counts the number of operations executed by the processing element, including those that are executed speculatively and would not be executed in a simple sequential execution of the program\000 */
{ 24486 }, /* pc_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, software change of the PC\000event=0xc\000\00000\000Instruction architecturally executed, condition code check pass, software change of the PC\000 */
{ 22808 }, /* pc_write_spec\000instruction\000Operation speculatively executed, software change of the PC\000event=0x76\000\00000\000Operation speculatively executed, software change of the PC\000 */
{ 23813 }, /* rc_ld_spec\000instruction\000Release consistency operation speculatively executed, Load-Acquire\000event=0x90\000\00000\000Release consistency operation speculatively executed, Load-Acquire\000 */
{ 23985 }, /* rc_st_spec\000instruction\000Release consistency operation speculatively executed, Store-Release\000event=0x91\000\00000\000Release consistency operation speculatively executed, Store-Release\000 */
{ 29831 }, /* sample_collision\000spe\000Sample collided with previous sample\000event=0x4003\000\00000\000Sample collided with previous sample\000 */
{ 29651 }, /* sample_feed\000spe\000Sample Taken\000event=0x4001\000\00000\000Sample Taken\000 */
{ 29710 }, /* sample_filtrate\000spe\000Sample Taken and not removed by filtering\000event=0x4002\000\00000\000Sample Taken and not removed by filtering\000 */
{ 29583 }, /* sample_pop\000spe\000Sample Population\000event=0x4000\000\00000\000Sample Population\000 */
{ 27009 }, /* st_align_lat\000memory\000Store with additional latency from alignment\000event=0x4022\000\00000\000Store with additional latency from alignment\000 */
{ 21486 }, /* st_retired\000instruction\000Instruction architecturally executed, condition code check pass, store\000event=0x7\000\00000\000Instruction architecturally executed, condition code check pass, store\000 */
{ 22095 }, /* st_spec\000instruction\000Operation speculatively executed, store\000event=0x71\000\00000\000Operation speculatively executed, store\000 */
{ 28008 }, /* stall\000pipeline\000Impacted by errata, use metrics instead -  Spec update: Errata AC03_CPU_29\000event=0x3c\000\00000\000No operation sent for execution.  The counter counts every attributable cycle on which no attributable instruction or operation was sent for execution on this processing element\000 */
{ 27892 }, /* stall_backend\000pipeline\000No operation issued due to the backend\000event=0x24\000\00000\000No operation issued due to the backend\000 */
{ 13630 }, /* stall_backend_cache\000core imp def\000Dispatch stall due to L1 data cache miss\000event=0xd20d\000\00000\000Dispatch stall due to L1 data cache miss\000 */
{ 29344 }, /* stall_backend_mem\000pipeline\000Memory stall cycles\000event=0x4005\000\00000\000Memory stall cycles.  The counter counts each cycle counted by STALL_BACKEND where there is a cache miss in the last level of cache within the processing element clock domain\000 */
{ 13888 }, /* stall_backend_resource\000core imp def\000Dispatch stall due to lack of any core resource\000event=0xd20f\000\00000\000Dispatch stall due to lack of any core resource\000 */
{ 13762 }, /* stall_backend_tlb\000core imp def\000Dispatch stall due to L1 data TLB miss\000event=0xd20e\000\00000\000Dispatch stall due to L1 data TLB miss\000 */
{ 27734 }, /* stall_frontend\000pipeline\000Impacted by errata, use metrics instead -  Spec update: Errata AC03_CPU_29\000event=0x23\000\00000\000No operation issued because of the frontend\000 */
{ 13342 }, /* stall_frontend_cache\000core imp def\000Dispatch stall due to L1 instruction cache miss\000event=0xd20b\000\00000\000Dispatch stall due to L1 instruction cache miss\000 */
{ 13489 }, /* stall_frontend_tlb\000core imp def\000Dispatch stall due to L1 instruction TLB miss\000event=0xd20c\000\00000\000Dispatch stall due to L1 instruction TLB miss\000 */
{ 29039 }, /* stall_slot\000pipeline\000No operation sent for execution on a slot\000event=0x3f\000\00000\000No operation sent for execution on a slot.  The counter counts on each attributable cycle the number of instruction or operation slots that were not occupied by an instruction or operation attributable to the processing element\000 */
{ 28291 }, /* stall_slot_backend\000pipeline\000No operation sent for execution on a slot due to the backend\000event=0x3d\000\00000\000No operation sent for execution on a slot due to the backend.  Counts each slot counted by STALL_SLOT where no attributable instruction or operation was sent for execution because the backend is unable to accept it\000 */
{ 28610 }, /* stall_slot_frontend\000pipeline\000Impacted by errata, use metrics instead -  Spec update: Errata AC03_CPU_29\000event=0x3e\000\00000\000No operation sent for execution on a slot due to the frontend.  Counts each slot counted by STALL_SLOT where no attributable instruction or operation was sent for execution because there was no attributable instruction or operation available to issue from the processing element from the frontend for the slot\000 */
{ 25973 }, /* strex_fail_spec\000intrinsic\000Exclusive operation speculatively executed, STREX or STX fail\000event=0x6e\000\00000\000Exclusive operation speculatively executed, STREX or STX fail\000 */
{ 25808 }, /* strex_pass_spec\000intrinsic\000Exclusive operation speculatively executed, STREX or STX pass\000event=0x6d\000\00000\000Exclusive operation speculatively executed, STREX or STX pass\000 */
{ 26138 }, /* strex_spec\000intrinsic\000Exclusive operation speculatively executed, STREX or STX\000event=0x6f\000\00000\000Exclusive operation speculatively executed, STREX or STX\000 */
{ 21286 }, /* sw_incr\000instruction\000Instruction architecturally executed, Condition code check pass, software increment\000event=0\000\00000\000Instruction architecturally executed, Condition code check pass, software increment\000 */
{ 24814 }, /* ttbr_write_retired\000instruction\000Instruction architecturally executed, Condition code check pass, write to TTBR\000event=0x1c\000\00000\000Instruction architecturally executed, Condition code check pass, write to TTBR\000 */
{ 26634 }, /* unaligned_ld_spec\000memory\000Unaligned access, read\000event=0x68\000\00000\000Unaligned access, read\000 */
{ 26808 }, /* unaligned_ldst_spec\000memory\000Unaligned access\000event=0x6a\000\00000\000Unaligned access\000 */
{ 26720 }, /* unaligned_st_spec\000memory\000Unaligned access, write\000event=0x69\000\00000\000Unaligned access, write\000 */
{ 22650 }, /* vfp_spec\000instruction\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Operation speculatively executed, floating-point instruction\000 */
{ 18271 }, /* watchpoint_match\000core imp def\000LSU detected a Watchpoint data match\000event=0xd60d\000\00000\000LSU detected a Watchpoint data match\000 */

};

const struct pmu_table_entry pmu_events__ampere_ampereone[] = {
{
     .entries = pmu_events__ampere_ampereone_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__ampere_ampereone_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_metrics__ampere_ampereone_default_core[] = {
{ 195193 }, /* Miss_Ratio;l1d_cache_read_miss\000Cache\000L1D_CACHE_LMISS_RD / L1D_CACHE_RD\000\000L1D cache read miss rate\000\0001per cache read access\000\000\000\00000 */
{ 200025 }, /* backend_bound\000Default;TopdownL1\000100 * (stall_slot_backend / (#slots * cpu_cycles))\000\000This metric is the percentage of total slots that were stalled due to resource constraints in the backend of the processor\000\0001percent of slots\000\000\000TopdownL1\00000 */
{ 200873 }, /* backend_core\000TopdownL2\000backend_bound / 100 - backend_memory\000\000Fraction of slots the CPU was stalled due to backend non-memory subsystem issues\000\000100%\000\000\000\00000 */
{ 201026 }, /* backend_memory\000TopdownL2\000(STALL_BACKEND_TLB + STALL_BACKEND_CACHE) / CPU_CYCLES\000\000Fraction of slots the CPU was stalled due to backend memory subsystem issues (cache/tlb miss)\000\000100%\000\000\000\00000 */
{ 201212 }, /* branch_mispredict\000TopdownL2\000BR_MIS_PRED_RETIRED / GPC_FLUSH * slots_lost_misspeculation_fraction\000\000Fraction of slots lost due to branch misprediciton\000\0001percent of slots\000\000\000\00000 */
{ 194242 }, /* branch_miss_pred_rate\000branch\000BR_MIS_PRED / BR_PRED\000\000Branch predictor misprediction rate. May not count branches that are never resolved because they are in the misprediction shadow of an earlier branch\000\000100%\000\000\000\00000 */
{ 194455 }, /* bus_utilization\000Bus\000BUS_ACCESS / BUS_CYCLES * 100\000\000Core-to-uncore bus utilization\000\0001percent of bus cycles\000\000\000\00000 */
{ 196449 }, /* crypto_percentage\000Operation_Mix\000CRYPTO_SPEC / INST_SPEC * 100\000\000This metric measures crypto operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000 */
{ 202194 }, /* d_cache_l2_miss_rate\000TopdownL3\000STALL_BACKEND_MEM / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled due to data L2 cache miss\000\0001percent of cycles\000\000\000\00000 */
{ 202353 }, /* d_cache_miss_rate\000TopdownL3\000STALL_BACKEND_CACHE / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled due to data cache miss\000\0001percent of cycles\000\000\000\00000 */
{ 202508 }, /* d_tlb_miss_rate\000TopdownL3\000STALL_BACKEND_TLB / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled due to data TLB miss\000\0001percent of cycles\000\000\000\00000 */
{ 199502 }, /* dtlb_walk_ratio\000Miss_Ratio;DTLB_Effectiveness\000DTLB_WALK / L1D_TLB\000\000This metric measures the ratio of data TLB Walks to the total number of data TLB accesses. This gives an indication of the effectiveness of the data TLB accesses\000\0001per TLB access\000\000\000\00000 */
{ 201385 }, /* frontend_bandwidth\000TopdownL2\000frontend_bound - frontend_latency\000\000Fraction of slots the CPU did not dispatch at full bandwidth - able to dispatch partial slots only (1, 2, or 3 uops)\000\0001percent of slots\000\000\000\00000 */
{ 200265 }, /* frontend_bound\000Default;TopdownL1\000100 - (retired_fraction + slots_lost_misspeculation_fraction + backend_bound)\000\000This metric is the percentage of total slots that were stalled due to resource constraints in the frontend of the processor\000\0001percent of slots\000\000\000TopdownL1\00000 */
{ 201590 }, /* frontend_latency\000TopdownL2\000(STALL_FRONTEND - (STALL_SLOT_FRONTEND - frontend_bound / 100 * CPU_CYCLES * #slots) / #slots) / CPU_CYCLES * 100\000\000Fraction of slots the CPU was stalled due to frontend latency issues (cache/tlb miss); nothing to dispatch\000\0001percent of slots\000\000\000\00000 */
{ 202657 }, /* fsu_pipe_utilization\000TopdownL3\000FSU_ISSUED / (CPU_CYCLES * 2) * 100\000\000Fraction of FSU execute slots utilized\000\0001percent of slots\000\000\000\00000 */
{ 196633 }, /* gflops\000InstructionMix\000VFP_SPEC / (duration_time * 1e9)\000\000Giga-floating point operations per second\000\000\000\000\000\00000 */
{ 202788 }, /* i_cache_miss_rate\000TopdownL3\000STALL_FRONTEND_CACHE / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled due to instruction cache miss\000\0001percent of slots\000\000\000\00000 */
{ 202950 }, /* i_tlb_miss_rate\000TopdownL3\000STALL_FRONTEND_TLB / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled due to instruction TLB miss\000\0001percent of slots\000\000\000\00000 */
{ 196738 }, /* integer_dp_percentage\000Operation_Mix\000DP_SPEC / INST_SPEC * 100\000\000This metric measures scalar integer operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000 */
{ 196930 }, /* ipc\000General\000INST_RETIRED / CPU_CYCLES\000\000This metric measures the number of instructions retired per cycle\000\0001per cycle\000\000\000\00000 */
{ 199753 }, /* itlb_walk_ratio\000Miss_Ratio;ITLB_Effectiveness\000ITLB_WALK / L1I_TLB\000\000This metric measures the ratio of instruction TLB Walks to the total number of instruction TLB accesses. This gives an indication of the effectiveness of the instruction TLB accesses\000\0001per TLB access\000\000\000\00000 */
{ 203106 }, /* ixu_pipe_utilization\000TopdownL3\000IXU_NUM_UOPS_ISSUED / (CPU_CYCLES * #slots) * 100\000\000Fraction of IXU execute slots utilized\000\0001percent of slots\000\000\000\00000 */
{ 196015 }, /* l1d_cache_miss_mpki\000Cache\000L1D_CACHE_LMISS_RD * 1e3 / INST_RETIRED\000\000Misses per thousand instructions (data)\000\0001MPKI\000\000\000\00000 */
{ 194566 }, /* l1d_cache_miss_ratio\000Miss_Ratio;L1D_Cache_Effectiveness\000L1D_CACHE_REFILL / L1D_CACHE\000\000This metric measures the ratio of level 1 data cache accesses missed to the total number of level 1 data cache accesses. This gives an indication of the effectiveness of the level 1 data cache\000\0001per cache access\000\000\000\00000 */
{ 196134 }, /* l1i_cache_miss_mpki\000Cache\000L1I_CACHE_LMISS * 1e3 / INST_RETIRED\000\000Misses per thousand instructions (instruction)\000\0001MPKI\000\000\000\00000 */
{ 194869 }, /* l1i_cache_miss_ratio\000Miss_Ratio;L1I_Cache_Effectiveness\000L1I_CACHE_REFILL / L1I_CACHE\000\000This metric measures the ratio of level 1 instruction cache accesses missed to the total number of level 1 instruction cache accesses. This gives an indication of the effectiveness of the level 1 instruction cache\000\0001per cache access\000\000\000\00000 */
{ 195787 }, /* l1i_cache_read_miss_rate\000Cache\000L1I_CACHE_LMISS / L1I_CACHE\000\000L1I cache read miss rate\000\0001per cache access\000\000\000\00000 */
{ 195319 }, /* l2_cache_miss_ratio\000Miss_Ratio;L2_Cache_Effectiveness\000L2D_CACHE_REFILL / L2D_CACHE\000\000This metric measures the ratio of level 2 cache accesses missed to the total number of level 2 cache accesses. This gives an indication of the effectiveness of the level 2 cache, which is a unified cache that stores both data and instruction. Note that cache accesses in this cache are either data memory access or instruction fetch as this is a unified cache\000\0001per cache access\000\000\000\00000 */
{ 195896 }, /* l2d_cache_read_miss_rate\000Cache\000L2D_CACHE_LMISS_RD / L2D_CACHE_RD\000\000L2 cache read miss rate\000\0001per cache read access\000\000\000\00000 */
{ 197052 }, /* load_percentage\000Operation_Mix\000LD_SPEC / INST_SPEC * 100\000\000This metric measures load operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000 */
{ 197228 }, /* load_store_spec_rate\000Operation_Mix\000LDST_SPEC / INST_SPEC * 100\000\000The rate of load or store instructions speculatively executed to overall instructions speclatively executed\000\0001percent of operations\000\000\000\00000 */
{ 201863 }, /* other_miss_pred\000TopdownL2\000slots_lost_misspeculation_fraction - branch_mispredict\000\000Fraction of slots lost due to other/non-branch misprediction misspeculation\000\0001percent of slots\000\000\000\00000 */
{ 197652 }, /* pc_write_spec_rate\000Operation_Mix\000PC_WRITE_SPEC / INST_SPEC * 100\000\000The rate of software change of the PC speculatively executed to overall instructions speclatively executed\000\0001percent of operations\000\000\000\00000 */
{ 202045 }, /* pipe_utilization\000TopdownL2\000100 * ((IXU_NUM_UOPS_ISSUED + FSU_ISSUED) / (CPU_CYCLES * 6))\000\000Fraction of execute slots utilized\000\0001percent of slots\000\000\000\00000 */
{ 200721 }, /* retired_fraction\000Default;TopdownL1\000100 * (OP_RETIRED / (CPU_CYCLES * #slots))\000\000Fraction of slots retiring, useful work\000\0001percent of slots\000\000\000TopdownL1\00000 */
{ 197429 }, /* retired_mips\000InstructionMix\000INST_RETIRED / (duration_time * 1e6)\000\000Millions of instructions per second\000\000\000\000\000\00000 */
{ 198231 }, /* retired_rate\000General\000OP_RETIRED / OP_SPEC\000\000Of all the micro-operations issued, what percentage are retired(committed)\000\000100%\000\000\000\00000 */
{ 198032 }, /* scalar_fp_percentage\000Operation_Mix\000VFP_SPEC / INST_SPEC * 100\000\000This metric measures scalar floating point operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000 */
{ 196257 }, /* simd_percentage\000Operation_Mix\000ASE_SPEC / INST_SPEC * 100\000\000This metric measures advanced SIMD operations as a percentage of total operations speculatively executed\000\0001percent of operations\000\000\000\00000 */
{ 200534 }, /* slots_lost_misspeculation_fraction\000Default;TopdownL1\000100 * ((OP_SPEC - OP_RETIRED) / (CPU_CYCLES * #slots))\000\000Fraction of slots lost due to misspeculation\000\0001percent of slots\000\000\000TopdownL1\00000 */
{ 197538 }, /* spec_utilization_mips\000PEutilization\000INST_SPEC / (duration_time * 1e6)\000\000Millions of instructions per second\000\000\000\000\000\00000 */
{ 198623 }, /* stall_backend_cache_rate\000Stall\000STALL_BACKEND_CACHE / CPU_CYCLES * 100\000\000Proportion of cycles stalled and no operations issued to backend and cache miss\000\0001percent of cycles\000\000\000\00000 */
{ 198799 }, /* stall_backend_resource_rate\000Stall\000STALL_BACKEND_RESOURCE / CPU_CYCLES * 100\000\000Proportion of cycles stalled and no operations issued to backend and resource full\000\0001percent of cycles\000\000\000\00000 */
{ 198984 }, /* stall_backend_tlb_rate\000Stall\000STALL_BACKEND_TLB / CPU_CYCLES * 100\000\000Proportion of cycles stalled and no operations issued to backend and TLB miss\000\0001percent of cycles\000\000\000\00000 */
{ 199154 }, /* stall_frontend_cache_rate\000Stall\000STALL_FRONTEND_CACHE / CPU_CYCLES * 100\000\000Proportion of cycles stalled and no ops delivered from frontend and cache miss\000\0001percent of cycles\000\000\000\00000 */
{ 199331 }, /* stall_frontend_tlb_rate\000Stall\000STALL_FRONTEND_TLB / CPU_CYCLES * 100\000\000Proportion of cycles stalled and no ops delivered from frontend and TLB miss\000\0001percent of cycles\000\000\000\00000 */
{ 203402 }, /* stall_fsu_sched_rate\000TopdownL4\000IDR_STALL_FSU_SCHED / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled and FSU was full\000\0001percent of cycles\000\000\000\00000 */
{ 203554 }, /* stall_ixu_sched_rate\000TopdownL4\000IDR_STALL_IXU_SCHED / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled and IXU was full\000\0001percent of cycles\000\000\000\00000 */
{ 203706 }, /* stall_lob_id_rate\000TopdownL4\000IDR_STALL_LOB_ID / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled and LOB was full\000\0001percent of cycles\000\000\000\00000 */
{ 203251 }, /* stall_recovery_rate\000TopdownL3\000IDR_STALL_FLUSH / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled due to flush recovery\000\0001percent of slots\000\000\000\00000 */
{ 203852 }, /* stall_rob_id_rate\000TopdownL4\000IDR_STALL_ROB_ID / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled and ROB was full\000\0001percent of cycles\000\000\000\00000 */
{ 203998 }, /* stall_sob_id_rate\000TopdownL4\000IDR_STALL_SOB_ID / CPU_CYCLES * 100\000\000Fraction of cycles the CPU was stalled and SOB was full\000\0001percent of cycles\000\000\000\00000 */
{ 197854 }, /* store_percentage\000Operation_Mix\000ST_SPEC / INST_SPEC * 100\000\000This metric measures store operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000 */
{ 198360 }, /* wasted\000General\0001 - OP_RETIRED / (CPU_CYCLES * #slots)\000\000Of all the micro-operations issued, what proportion are lost\000\000100%\000\000\000\00000 */
{ 198487 }, /* wasted_rate\000General\0001 - OP_RETIRED / OP_SPEC\000\000Of all the micro-operations issued, what percentage are not retired(committed)\000\000100%\000\000\000\00000 */

};

const struct pmu_table_entry pmu_metrics__ampere_ampereone[] = {
{
     .entries = pmu_metrics__ampere_ampereone_default_core,
     .num_entries = ARRAY_SIZE(pmu_metrics__ampere_ampereone_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__ampere_emag_default_core[] = {
{ 22494 }, /* ase_spec\000instruction\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Operation speculatively executed, Advanced SIMD instruction\000 */
{ 13 }, /* br_immed_spec\000branch\000Branch speculatively executed, immediate branch\000event=0x78\000\00000\000Branch speculatively executed, immediate branch\000 */
{ 278 }, /* br_indirect_spec\000branch\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000 */
{ 29943 }, /* br_mis_pred\000branch\000Branch mispredicted\000event=0x10\000\00000\000Mispredicted or not predicted branch speculatively executed\000 */
{ 32593 }, /* br_mis_pred_retired\000instruction\000Instruction architecturally executed, mispredicted branch\000event=0x22\000\00000\000This event counts any branch counted by BR_RETIRED which is not correctly predicted and causes a pipeline flush\000 */
{ 30057 }, /* br_pred\000branch\000Predictable branch\000event=0x12\000\00000\000Predictable branch speculatively executed\000 */
{ 32397 }, /* br_retired\000instruction\000Instruction architecturally executed, branch\000event=0x21\000\00000\000This event counts all branches, taken or not. This excludes exception entries, debug entries and CCFAIL branches\000 */
{ 145 }, /* br_return_spec\000branch\000Branch speculatively executed, procedure return\000event=0x79\000\00000\000Branch speculatively executed, procedure return\000 */
{ 30800 }, /* btb_mis_pred\000cache\000BTB misprediction\000event=0x102\000\00000\000Branch target buffer misprediction\000 */
{ 1311 }, /* bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000Attributable Bus access\000 */
{ 1153 }, /* bus_access_normal\000bus\000Bus access, Normal\000event=0x64\000\00000\000Bus access, Normal\000 */
{ 1022 }, /* bus_access_not_shared\000bus\000Bus access, not Normal, Cacheable, Shareable\000event=0x63\000\00000\000Bus access, not Normal, Cacheable, Shareable\000 */
{ 1228 }, /* bus_access_periph\000bus\000Bus access, peripheral\000event=0x65\000\00000\000Bus access, peripheral\000 */
{ 771 }, /* bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Bus access read\000 */
{ 903 }, /* bus_access_shared\000bus\000Bus access, Normal, Cacheable, Shareable\000event=0x62\000\00000\000Bus access, Normal, Cacheable, Shareable\000 */
{ 836 }, /* bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Bus access write\000 */
{ 33272 }, /* bx_stall\000pipeline\000BX stalled\000event=0x10c\000\00000\000BX Op non-issue\000 */
{ 32248 }, /* cid_write_retired\000instruction\000Write to CONTEXTIDR\000event=0xb\000\00000\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000 */
{ 31901 }, /* cpu_cycles\000clock\000Cycle\000event=0x11\000\00000\000The number of core clock cycles\000 */
{ 23330 }, /* crypto_spec\000instruction\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Operation speculatively executed, Cryptographic instruction\000 */
{ 32971 }, /* decode_stall\000pipeline\000Decode starved\000event=0x108\000\00000\000Decode starved for instruction cycle\000 */
{ 33061 }, /* dispatch_stall\000pipeline\000Dispatch stalled\000event=0x109\000\00000\000Op dispatch stalled cycle\000 */
{ 23705 }, /* dmb_spec\000instruction\000Barrier speculatively executed, DMB\000event=0x7e\000\00000\000Barrier speculatively executed, DMB\000 */
{ 22343 }, /* dp_spec\000instruction\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Operation speculatively executed, integer data processing\000 */
{ 23597 }, /* dsb_spec\000instruction\000Barrier speculatively executed, DSB\000event=0x7d\000\00000\000Barrier speculatively executed, DSB\000 */
{ 30937 }, /* dtb_miss\000cache\000DTB miss\000event=0x104\000\00000\000DTB miss\000 */
{ 20002 }, /* exc_dabort\000exception\000Exception taken, Data Abort and SError\000event=0x84\000\00000\000Exception taken, Data Abort and SError\000 */
{ 20191 }, /* exc_fiq\000exception\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000 */
{ 20266 }, /* exc_hvc\000exception\000Exception taken, Hypervisor Call\000event=0x8a\000\00000\000Exception taken, Hypervisor Call\000 */
{ 20116 }, /* exc_irq\000exception\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000 */
{ 19896 }, /* exc_pabort\000exception\000Exception taken, Instruction Abort\000event=0x83\000\00000\000Exception taken, Instruction Abort\000 */
{ 21097 }, /* exc_return\000exception\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Instruction architecturally executed, condition check pass, exception return\000 */
{ 19797 }, /* exc_svc\000exception\000Exception taken, Supervisor Call\000event=0x82\000\00000\000Exception taken, Supervisor Call\000 */
{ 21031 }, /* exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Exception taken\000 */
{ 20512 }, /* exc_trap_dabort\000exception\000Exception taken, Data Abort or SError not taken locally\000event=0x8c\000\00000\000Exception taken, Data Abort or SError not taken locally\000 */
{ 20915 }, /* exc_trap_fiq\000exception\000Exception taken, FIQ not taken locally\000event=0x8f\000\00000\000Exception taken, FIQ not taken locally\000 */
{ 20799 }, /* exc_trap_irq\000exception\000Exception taken, IRQ not taken locally\000event=0x8e\000\00000\000Exception taken, IRQ not taken locally\000 */
{ 20665 }, /* exc_trap_other\000exception\000Exception taken, Other traps not taken locally\000event=0x8d\000\00000\000Exception taken, Other traps not taken locally\000 */
{ 20365 }, /* exc_trap_pabort\000exception\000Exception taken, Instruction Abort not taken locally\000event=0x8b\000\00000\000Exception taken, Instruction Abort not taken locally\000 */
{ 19692 }, /* exc_undef\000exception\000Exception taken, Other synchronous\000event=0x81\000\00000\000Exception taken, Other synchronous\000 */
{ 31971 }, /* fsu_clock_off_cycles\000clock\000FSU clocking gated off cycle\000event=0x101\000\00000\000FSU clocking gated off cycle\000 */
{ 33455 }, /* fx_stall\000pipeline\000FX stalled\000event=0x10f\000\00000\000FX Op non-issue\000 */
{ 24159 }, /* inst_retired\000instruction\000Instruction architecturally executed\000event=0x8\000\00000\000Instruction architecturally executed\000 */
{ 24711 }, /* inst_spec\000instruction\000Operation speculatively executed\000event=0x1b\000\00000\000Operation speculatively executed\000 */
{ 23489 }, /* isb_spec\000instruction\000Barrier speculatively executed, ISB\000event=0x7c\000\00000\000Barrier speculatively executed, ISB\000 */
{ 30888 }, /* itb_miss\000cache\000ITB miss\000event=0x103\000\00000\000ITB miss\000 */
{ 33144 }, /* ixa_stall\000pipeline\000IXA stalled\000event=0x10a\000\00000\000IXA Op non-issue\000 */
{ 33208 }, /* ixb_stall\000pipeline\000IXB stalled\000event=0x10b\000\00000\000IXB Op non-issue\000 */
{ 31269 }, /* l1_stage2_tlb_refill\000cache\000L1 stage 2 TLB refill\000event=0x111\000\00000\000Level 1 stage 2 TLB refill\000 */
{ 3313 }, /* l1d_cache\000cache\000Level 1 data cache access\000event=0x4\000\00000\000Level 1 data cache access\000 */
{ 1902 }, /* l1d_cache_inval\000cache\000L1D cache invalidate\000event=0x48\000\00000\000Attributable Level 1 data cache invalidate\000 */
{ 30986 }, /* l1d_cache_late_miss\000cache\000L1D cache late miss\000event=0x105\000\00000\000Level 1 data cache late miss\000 */
{ 31077 }, /* l1d_cache_prefetch\000cache\000L1D cache prefetch\000event=0x106\000\00000\000Level 1 data cache prefetch request\000 */
{ 1587 }, /* l1d_cache_rd\000cache\000L1D cache access, read\000event=0x40\000\00000\000Attributable Level 1 data cache access, read\000 */
{ 3224 }, /* l1d_cache_refill\000cache\000Level 1 data cache refill\000event=0x3\000\00000\000Level 1 data cache refill\000 */
{ 1793 }, /* l1d_cache_refill_rd\000cache\000L1D cache refill, read\000event=0x42\000\00000\000Attributable Level 1 data cache refill, read\000 */
{ 1689 }, /* l1d_cache_wr\000cache\000L1D cache access, write\000event=0x41\000\00000\000Attributable Level 1 data cache access, write\000 */
{ 30148 }, /* l1d_tlb\000cache\000L1D TLB access\000event=0x25\000\00000\000This event counts any load or store operation which accesses the data L1 TLB\000 */
{ 3395 }, /* l1d_tlb_refill\000cache\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Attributable Level 1 data TLB refill\000 */
{ 2003 }, /* l1d_tlb_refill_rd\000cache\000L1D tlb refill, read\000event=0x4c\000\00000\000Attributable Level 1 data TLB refill, read\000 */
{ 2106 }, /* l1d_tlb_refill_wr\000cache\000L1D tlb refill, write\000event=0x4d\000\00000\000Attributable Level 1 data TLB refill, write\000 */
{ 3504 }, /* l1i_cache\000cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Attributable Level 1 instruction cache access\000 */
{ 2998 }, /* l1i_cache_refill\000cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Level 1 instruction cache refill\000 */
{ 30269 }, /* l1i_tlb\000cache\000Attributable Level 1 instruction TLB access\000event=0x26\000\00000\000This event counts any instruction fetch which accesses the instruction L1 TLB\000 */
{ 3101 }, /* l1i_tlb_refill\000cache\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Attributable Level 1 instruction TLB refill\000 */
{ 3627 }, /* l2d_cache\000cache\000Level 2 data cache access\000event=0x16\000\00000\000Level 2 data cache access\000 */
{ 2897 }, /* l2d_cache_inval\000cache\000L2D cache invalidate\000event=0x58\000\00000\000Attributable Level 2 data cache invalidate\000 */
{ 31173 }, /* l2d_cache_prefetch\000cache\000L2D cache prefetch\000event=0x107\000\00000\000Level 2 data cache prefetch request\000 */
{ 2211 }, /* l2d_cache_rd\000cache\000L2D cache access, read\000event=0x50\000\00000\000Attributable Level 2 data cache access, read\000 */
{ 3710 }, /* l2d_cache_refill\000cache\000Level 2 data refill\000event=0x17\000\00000\000Level 2 data refill\000 */
{ 2417 }, /* l2d_cache_refill_rd\000cache\000L2D cache refill, read\000event=0x52\000\00000\000Attributable Level 2 data cache refill, read\000 */
{ 2526 }, /* l2d_cache_refill_wr\000cache\000L2D cache refill, write\000event=0x53\000\00000\000Attributable Level 2 data cache refill, write\000 */
{ 3788 }, /* l2d_cache_wb\000cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Attributable Level 2 data cache write-back\000 */
{ 2758 }, /* l2d_cache_wb_clean\000cache\000L2D cache Write-Back, cleaning and coherency\000event=0x57\000\00000\000Level 2 data cache Write-Back, cleaning and coherency\000 */
{ 2637 }, /* l2d_cache_wb_victim\000cache\000L2D cache Write-Back, victim\000event=0x56\000\00000\000Attributable Level 2 data cache Write-Back, victim\000 */
{ 2313 }, /* l2d_cache_wr\000cache\000L2D cache access, write\000event=0x51\000\00000\000Attributable Level 2 data cache access, write\000 */
{ 30420 }, /* l2d_tlb_access\000cache\000L2D TLB access\000event=0x34\000\00000\000Level 2 access to data TLB that caused a page table walk. This event counts on any data access which causes L2D_TLB_REFILL to count\000 */
{ 30603 }, /* l2i_tlb_access\000cache\000L2I TLB access\000event=0x35\000\00000\000Level 2 access to instruciton TLB that caused a page table walk. This event counts on any instruciton access which causes L2I_TLB_REFILL to count\000 */
{ 21982 }, /* ld_spec\000instruction\000Operation speculatively executed, load\000event=0x70\000\00000\000Operation speculatively executed, load\000 */
{ 25658 }, /* ldrex_spec\000intrinsic\000Exclusive operation speculatively executed, LDREX or LDX\000event=0x6c\000\00000\000Exclusive operation speculatively executed, LDREX or LDX\000 */
{ 22210 }, /* ldst_spec\000instruction\000Operation speculatively executed, load or store\000event=0x72\000\00000\000Operation speculatively executed, load or store\000 */
{ 33333 }, /* lx_stall\000pipeline\000LX stalled\000event=0x10d\000\00000\000LX Op non-issue\000 */
{ 27136 }, /* mem_access\000memory\000Data memory access\000event=0x13\000\00000\000Data memory access\000 */
{ 26460 }, /* mem_access_rd\000memory\000Data memory access, read\000event=0x66\000\00000\000Data memory access, read\000 */
{ 26546 }, /* mem_access_wr\000memory\000Data memory access, write\000event=0x67\000\00000\000Data memory access, write\000 */
{ 32810 }, /* memory_error\000memory\000Local memory error\000event=0x1a\000\00000\000This event counts any correctable or uncorrectable memory error (ECC or parity) in the protected core RAMs\000 */
{ 25555 }, /* nop_spec\000instruction\000Speculatively executed, NOP\000event=0x100\000\00000\000Operation speculatively executed, NOP\000 */
{ 31361 }, /* page_walk_l0_stage1_hit\000cache\000Page walk, L0 stage-1 hit\000event=0x112\000\00000\000Page walk cache level-0 stage-1 hit\000 */
{ 31469 }, /* page_walk_l1_stage1_hit\000cache\000Page walk, L1 stage-1 hit\000event=0x113\000\00000\000Page walk cache level-1 stage-1 hit\000 */
{ 31685 }, /* page_walk_l1_stage2_hit\000cache\000Page walk, L1 stage-2 hit\000event=0x115\000\00000\000Page walk cache level-1 stage-2 hit\000 */
{ 31577 }, /* page_walk_l2_stage1_hit\000cache\000Page walk, L2 stage-1 hit\000event=0x114\000\00000\000Page walk cache level-2 stage-1 hit\000 */
{ 31793 }, /* page_walk_l2_stage2_hit\000cache\000Page walk, L2 stage-2 hit\000event=0x116\000\00000\000Page walk cache level-2 stage-2 hit\000 */
{ 22808 }, /* pc_write_spec\000instruction\000Operation speculatively executed, software change of the PC\000event=0x76\000\00000\000Operation speculatively executed, software change of the PC\000 */
{ 23813 }, /* rc_ld_spec\000instruction\000Release consistency operation speculatively executed, Load-Acquire\000event=0x90\000\00000\000Release consistency operation speculatively executed, Load-Acquire\000 */
{ 23985 }, /* rc_st_spec\000instruction\000Release consistency operation speculatively executed, Store-Release\000event=0x91\000\00000\000Release consistency operation speculatively executed, Store-Release\000 */
{ 22095 }, /* st_spec\000instruction\000Operation speculatively executed, store\000event=0x71\000\00000\000Operation speculatively executed, store\000 */
{ 25973 }, /* strex_fail_spec\000intrinsic\000Exclusive operation speculatively executed, STREX or STX fail\000event=0x6e\000\00000\000Exclusive operation speculatively executed, STREX or STX fail\000 */
{ 25808 }, /* strex_pass_spec\000intrinsic\000Exclusive operation speculatively executed, STREX or STX pass\000event=0x6d\000\00000\000Exclusive operation speculatively executed, STREX or STX pass\000 */
{ 26138 }, /* strex_spec\000intrinsic\000Exclusive operation speculatively executed, STREX or STX\000event=0x6f\000\00000\000Exclusive operation speculatively executed, STREX or STX\000 */
{ 32140 }, /* sw_incr\000instruction\000Software increment\000event=0\000\00000\000Instruction architecturally executed, software increment\000 */
{ 33394 }, /* sx_stall\000pipeline\000SX stalled\000event=0x10e\000\00000\000SX Op non-issue\000 */
{ 24814 }, /* ttbr_write_retired\000instruction\000Instruction architecturally executed, Condition code check pass, write to TTBR\000event=0x1c\000\00000\000Instruction architecturally executed, Condition code check pass, write to TTBR\000 */
{ 26634 }, /* unaligned_ld_spec\000memory\000Unaligned access, read\000event=0x68\000\00000\000Unaligned access, read\000 */
{ 26808 }, /* unaligned_ldst_spec\000memory\000Unaligned access\000event=0x6a\000\00000\000Unaligned access\000 */
{ 26720 }, /* unaligned_st_spec\000memory\000Unaligned access, write\000event=0x69\000\00000\000Unaligned access, write\000 */
{ 22650 }, /* vfp_spec\000instruction\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Operation speculatively executed, floating-point instruction\000 */
{ 32072 }, /* wait_cycles\000clock\000Wait state cycle\000event=0x110\000\00000\000Wait state cycle\000 */

};

const struct pmu_table_entry pmu_events__ampere_emag[] = {
{
     .entries = pmu_events__ampere_emag_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__ampere_emag_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__arm_cortex_a34_default_core[] = {
{ 22969 }, /* br_immed_retired\000instruction\000Instruction architecturally executed, immediate branch\000event=0xd\000\00000\000Instruction architecturally executed, immediate branch\000 */
{ 278 }, /* br_indirect_spec\000branch\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000 */
{ 411 }, /* br_mis_pred\000branch\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000Mispredicted or not predicted branch speculatively executed\000 */
{ 565 }, /* br_pred\000branch\000Predictable branch speculatively executed\000event=0x12\000\00000\000Predictable branch speculatively executed\000 */
{ 23122 }, /* br_return_retired\000instruction\000Instruction architecturally executed, condition code check pass, procedure return\000event=0xe\000\00000\000Instruction architecturally executed, condition code check pass, procedure return\000 */
{ 1311 }, /* bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000Attributable Bus access\000 */
{ 771 }, /* bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Bus access read\000 */
{ 836 }, /* bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Bus access write\000 */
{ 721 }, /* bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000Bus cycle\000 */
{ 24272 }, /* cid_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000 */
{ 679 }, /* cpu_cycles\000bus\000Cycle\000event=0x11\000\00000\000Cycle\000 */
{ 20191 }, /* exc_fiq\000exception\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000 */
{ 20116 }, /* exc_irq\000exception\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000 */
{ 33889 }, /* exc_return\000instruction\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Instruction architecturally executed, condition check pass, exception return\000 */
{ 21031 }, /* exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Exception taken\000 */
{ 24159 }, /* inst_retired\000instruction\000Instruction architecturally executed\000event=0x8\000\00000\000Instruction architecturally executed\000 */
{ 3313 }, /* l1d_cache\000cache\000Level 1 data cache access\000event=0x4\000\00000\000Level 1 data cache access\000 */
{ 3224 }, /* l1d_cache_refill\000cache\000Level 1 data cache refill\000event=0x3\000\00000\000Level 1 data cache refill\000 */
{ 33516 }, /* l1d_cache_wb\000cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000Attributable Level 1 data cache write-back\000 */
{ 3395 }, /* l1d_tlb_refill\000cache\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Attributable Level 1 data TLB refill\000 */
{ 3504 }, /* l1i_cache\000cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Attributable Level 1 instruction cache access\000 */
{ 2998 }, /* l1i_cache_refill\000cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Level 1 instruction cache refill\000 */
{ 3101 }, /* l1i_tlb_refill\000cache\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Attributable Level 1 instruction TLB refill\000 */
{ 3627 }, /* l2d_cache\000cache\000Level 2 data cache access\000event=0x16\000\00000\000Level 2 data cache access\000 */
{ 3710 }, /* l2d_cache_refill\000cache\000Level 2 data refill\000event=0x17\000\00000\000Level 2 data refill\000 */
{ 3788 }, /* l2d_cache_wb\000cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Attributable Level 2 data cache write-back\000 */
{ 33712 }, /* ld_retired\000instruction\000Instruction architecturally executed, condition code check pass, load\000event=0x6\000\00000\000Instruction architecturally executed, condition code check pass, load\000 */
{ 27136 }, /* mem_access\000memory\000Data memory access\000event=0x13\000\00000\000Data memory access\000 */
{ 33636 }, /* memory_error\000exception\000Local memory error\000event=0x1a\000\00000\000Local memory error\000 */
{ 24486 }, /* pc_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, software change of the PC\000event=0xc\000\00000\000Instruction architecturally executed, condition code check pass, software change of the PC\000 */
{ 21486 }, /* st_retired\000instruction\000Instruction architecturally executed, condition code check pass, store\000event=0x7\000\00000\000Instruction architecturally executed, condition code check pass, store\000 */
{ 21286 }, /* sw_incr\000instruction\000Instruction architecturally executed, Condition code check pass, software increment\000event=0\000\00000\000Instruction architecturally executed, Condition code check pass, software increment\000 */
{ 34080 }, /* unaligned_ldst_retired\000memory\000Instruction architecturally executed, condition code check pass, unaligned\000event=0xf\000\00000\000Instruction architecturally executed, condition code check pass, unaligned\000 */

};

const struct pmu_table_entry pmu_events__arm_cortex_a34[] = {
{
     .entries = pmu_events__arm_cortex_a34_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__arm_cortex_a34_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__arm_cortex_a35_default_core[] = {
{ 22494 }, /* ase_spec\000instruction\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Operation speculatively executed, Advanced SIMD instruction\000 */
{ 22969 }, /* br_immed_retired\000instruction\000Instruction architecturally executed, immediate branch\000event=0xd\000\00000\000Instruction architecturally executed, immediate branch\000 */
{ 278 }, /* br_indirect_spec\000branch\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000 */
{ 411 }, /* br_mis_pred\000branch\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000Mispredicted or not predicted branch speculatively executed\000 */
{ 565 }, /* br_pred\000branch\000Predictable branch speculatively executed\000event=0x12\000\00000\000Predictable branch speculatively executed\000 */
{ 23122 }, /* br_return_retired\000instruction\000Instruction architecturally executed, condition code check pass, procedure return\000event=0xe\000\00000\000Instruction architecturally executed, condition code check pass, procedure return\000 */
{ 1311 }, /* bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000Attributable Bus access\000 */
{ 771 }, /* bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Bus access read\000 */
{ 836 }, /* bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Bus access write\000 */
{ 721 }, /* bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000Bus cycle\000 */
{ 24272 }, /* cid_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000 */
{ 679 }, /* cpu_cycles\000bus\000Cycle\000event=0x11\000\00000\000Cycle\000 */
{ 23330 }, /* crypto_spec\000instruction\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Operation speculatively executed, Cryptographic instruction\000 */
{ 22343 }, /* dp_spec\000instruction\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Operation speculatively executed, integer data processing\000 */
{ 20191 }, /* exc_fiq\000exception\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000 */
{ 20116 }, /* exc_irq\000exception\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000 */
{ 33889 }, /* exc_return\000instruction\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Instruction architecturally executed, condition check pass, exception return\000 */
{ 21031 }, /* exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Exception taken\000 */
{ 24159 }, /* inst_retired\000instruction\000Instruction architecturally executed\000event=0x8\000\00000\000Instruction architecturally executed\000 */
{ 24711 }, /* inst_spec\000instruction\000Operation speculatively executed\000event=0x1b\000\00000\000Operation speculatively executed\000 */
{ 3313 }, /* l1d_cache\000cache\000Level 1 data cache access\000event=0x4\000\00000\000Level 1 data cache access\000 */
{ 3224 }, /* l1d_cache_refill\000cache\000Level 1 data cache refill\000event=0x3\000\00000\000Level 1 data cache refill\000 */
{ 33516 }, /* l1d_cache_wb\000cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000Attributable Level 1 data cache write-back\000 */
{ 3395 }, /* l1d_tlb_refill\000cache\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Attributable Level 1 data TLB refill\000 */
{ 3504 }, /* l1i_cache\000cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Attributable Level 1 instruction cache access\000 */
{ 2998 }, /* l1i_cache_refill\000cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Level 1 instruction cache refill\000 */
{ 3101 }, /* l1i_tlb_refill\000cache\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Attributable Level 1 instruction TLB refill\000 */
{ 3627 }, /* l2d_cache\000cache\000Level 2 data cache access\000event=0x16\000\00000\000Level 2 data cache access\000 */
{ 3710 }, /* l2d_cache_refill\000cache\000Level 2 data refill\000event=0x17\000\00000\000Level 2 data refill\000 */
{ 3788 }, /* l2d_cache_wb\000cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Attributable Level 2 data cache write-back\000 */
{ 33712 }, /* ld_retired\000instruction\000Instruction architecturally executed, condition code check pass, load\000event=0x6\000\00000\000Instruction architecturally executed, condition code check pass, load\000 */
{ 27136 }, /* mem_access\000memory\000Data memory access\000event=0x13\000\00000\000Data memory access\000 */
{ 33636 }, /* memory_error\000exception\000Local memory error\000event=0x1a\000\00000\000Local memory error\000 */
{ 24486 }, /* pc_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, software change of the PC\000event=0xc\000\00000\000Instruction architecturally executed, condition code check pass, software change of the PC\000 */
{ 21486 }, /* st_retired\000instruction\000Instruction architecturally executed, condition code check pass, store\000event=0x7\000\00000\000Instruction architecturally executed, condition code check pass, store\000 */
{ 21286 }, /* sw_incr\000instruction\000Instruction architecturally executed, Condition code check pass, software increment\000event=0\000\00000\000Instruction architecturally executed, Condition code check pass, software increment\000 */
{ 34080 }, /* unaligned_ldst_retired\000memory\000Instruction architecturally executed, condition code check pass, unaligned\000event=0xf\000\00000\000Instruction architecturally executed, condition code check pass, unaligned\000 */
{ 22650 }, /* vfp_spec\000instruction\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Operation speculatively executed, floating-point instruction\000 */

};

const struct pmu_table_entry pmu_events__arm_cortex_a35[] = {
{
     .entries = pmu_events__arm_cortex_a35_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__arm_cortex_a35_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__arm_cortex_a510_default_core[] = {
{ 22494 }, /* ase_spec\000instruction\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Operation speculatively executed, Advanced SIMD instruction\000 */
{ 45947 }, /* ase_sve_int16_spec\000instruction\000Advanced SIMD and SVE 16-bit integer operations speculatively executed\000event=0x80e7\000\00000\000Advanced SIMD and SVE 16-bit integer operations speculatively executed\000 */
{ 46137 }, /* ase_sve_int32_spec\000instruction\000Advanced SIMD and SVE 32-bit integer operations speculatively executed\000event=0x80eb\000\00000\000Advanced SIMD and SVE 32-bit integer operations speculatively executed\000 */
{ 46327 }, /* ase_sve_int64_spec\000instruction\000Advanced SIMD and SVE 64-bit integer operations speculatively executed\000event=0x80ef\000\00000\000Advanced SIMD and SVE 64-bit integer operations speculatively executed\000 */
{ 45760 }, /* ase_sve_int8_spec\000instruction\000Advanced SIMD and SVE 8-bit integer operations speculatively executed\000event=0x80e3\000\00000\000Advanced SIMD and SVE 8-bit integer operations speculatively executed\000 */
{ 36040 }, /* br_cond_mis_pred\000branch\000Conditional branch mispredicted. This event counts when any branch that the conditional predictor can predict is retired and has mispredicted the condition. This event still counts when branch prediction is disabled due to the MMU being off. Conditional indirect branches that correctly predict the condition but mispredict the address do not count\000event=0xcc\000\00000\000Conditional branch mispredicted. This event counts when any branch that the conditional predictor can predict is retired and has mispredicted the condition. This event still counts when branch prediction is disabled due to the MMU being off. Conditional indirect branches that correctly predict the condition but mispredict the address do not count\000 */
{ 34274 }, /* br_cond_pred\000branch\000Predicted conditional branch executed. This event counts when any branch that the conditional predictor can predict is retired. This event still counts when branch prediction is disabled due to the Memory Management Unit (MMU) being off\000event=0xc9\000\00000\000Predicted conditional branch executed. This event counts when any branch that the conditional predictor can predict is retired. This event still counts when branch prediction is disabled due to the Memory Management Unit (MMU) being off\000 */
{ 22969 }, /* br_immed_retired\000instruction\000Instruction architecturally executed, immediate branch\000event=0xd\000\00000\000Instruction architecturally executed, immediate branch\000 */
{ 13 }, /* br_immed_spec\000branch\000Branch speculatively executed, immediate branch\000event=0x78\000\00000\000Branch speculatively executed, immediate branch\000 */
{ 35390 }, /* br_indirect_addr_mis_pred\000branch\000Indirect branch mispredicted due to address miscompare. This event counts when any indirect branch that the BTAC can predict is retired, was taken, correctly predicted the condition, and has mispredicted the address. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xcb\000\00000\000Indirect branch mispredicted due to address miscompare. This event counts when any indirect branch that the BTAC can predict is retired, was taken, correctly predicted the condition, and has mispredicted the address. This event still counts when branch prediction is disabled due to the MMU being off\000 */
{ 36777 }, /* br_indirect_addr_pred\000branch\000Indirect branch with predicted address executed. This event counts when any indirect branch that the BTAC can predict is retired, was taken, and correctly predicted the condition. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xcd\000\00000\000Indirect branch with predicted address executed. This event counts when any indirect branch that the BTAC can predict is retired, was taken, and correctly predicted the condition. This event still counts when branch prediction is disabled due to the MMU being off\000 */
{ 34783 }, /* br_indirect_mis_pred\000branch\000Indirect branch mispredicted. This event counts when any indirect branch that the Branch Target Address Cache (BTAC) can predict is retired and has mispredicted either the condition or the address. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xca\000\00000\000Indirect branch mispredicted. This event counts when any indirect branch that the Branch Target Address Cache (BTAC) can predict is retired and has mispredicted either the condition or the address. This event still counts when branch prediction is disabled due to the MMU being off\000 */
{ 278 }, /* br_indirect_spec\000branch\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000 */
{ 411 }, /* br_mis_pred\000branch\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000Mispredicted or not predicted branch speculatively executed\000 */
{ 25146 }, /* br_mis_pred_retired\000instruction\000Instruction architecturally executed, mispredicted branch\000event=0x22\000\00000\000Instruction architecturally executed, mispredicted branch\000 */
{ 565 }, /* br_pred\000branch\000Predictable branch speculatively executed\000event=0x12\000\00000\000Predictable branch speculatively executed\000 */
{ 25018 }, /* br_retired\000instruction\000Instruction architecturally executed, branch\000event=0x21\000\00000\000Instruction architecturally executed, branch\000 */
{ 37949 }, /* br_return_addr_mis_pred\000branch\000Procedure return mispredicted due to address miscompare. This event counts when any procedure return that the call-return stack can predict is retired, was taken, correctly predicted the condition, and has mispredicted the address. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xcf\000\00000\000Procedure return mispredicted due to address miscompare. This event counts when any procedure return that the call-return stack can predict is retired, was taken, correctly predicted the condition, and has mispredicted the address. This event still counts when branch prediction is disabled due to the MMU being off\000 */
{ 37349 }, /* br_return_addr_pred\000branch\000Procedure return with predicted address executed. This event counts when any procedure return that the call-return stack can predict is retired, was taken, and correctly predicted the condition. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xce\000\00000\000Procedure return with predicted address executed. This event counts when any procedure return that the call-return stack can predict is retired, was taken, and correctly predicted the condition. This event still counts when branch prediction is disabled due to the MMU being off\000 */
{ 23122 }, /* br_return_retired\000instruction\000Instruction architecturally executed, condition code check pass, procedure return\000event=0xe\000\00000\000Instruction architecturally executed, condition code check pass, procedure return\000 */
{ 145 }, /* br_return_spec\000branch\000Branch speculatively executed, procedure return\000event=0x79\000\00000\000Branch speculatively executed, procedure return\000 */
{ 1311 }, /* bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000Attributable Bus access\000 */
{ 771 }, /* bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Bus access read\000 */
{ 836 }, /* bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Bus access write\000 */
{ 721 }, /* bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000Bus cycle\000 */
{ 24272 }, /* cid_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000 */
{ 679 }, /* cpu_cycles\000bus\000Cycle\000event=0x11\000\00000\000Cycle\000 */
{ 23330 }, /* crypto_spec\000instruction\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Operation speculatively executed, Cryptographic instruction\000 */
{ 54196 }, /* cti_trigout4\000trace\000Cross-trigger Interface output trigger 4\000event=0x4018\000\00000\000Cross-trigger Interface output trigger 4\000 */
{ 54314 }, /* cti_trigout5\000trace\000Cross-trigger Interface output trigger 5 \000event=0x4019\000\00000\000Cross-trigger Interface output trigger 5 \000 */
{ 54434 }, /* cti_trigout6\000trace\000Cross-trigger Interface output trigger 6\000event=0x401a\000\00000\000Cross-trigger Interface output trigger 6\000 */
{ 54552 }, /* cti_trigout7\000trace\000Cross-trigger Interface output trigger 7\000event=0x401b\000\00000\000Cross-trigger Interface output trigger 7\000 */
{ 22343 }, /* dp_spec\000instruction\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Operation speculatively executed, integer data processing\000 */
{ 4626 }, /* dtlb_walk\000cache\000Access to data TLB causes a translation table walk\000event=0x34\000\00000\000Access to data TLB causes a translation table walk\000 */
{ 20191 }, /* exc_fiq\000exception\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000 */
{ 20116 }, /* exc_irq\000exception\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000 */
{ 33889 }, /* exc_return\000instruction\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Instruction architecturally executed, condition check pass, exception return\000 */
{ 21031 }, /* exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Exception taken\000 */
{ 45588 }, /* fp_dp_spec\000instruction\000Floating-point double-precision operations speculatively executed\000event=0x801c\000\00000\000Floating-point double-precision operations speculatively executed\000 */
{ 45248 }, /* fp_hp_spec\000instruction\000Floating-point half-precision operations speculatively executed\000event=0x8014\000\00000\000Floating-point half-precision operations speculatively executed\000 */
{ 45416 }, /* fp_sp_spec\000instruction\000Floating-point single-precision operations speculatively executed\000event=0x8018\000\00000\000Floating-point single-precision operations speculatively executed\000 */
{ 24159 }, /* inst_retired\000instruction\000Instruction architecturally executed\000event=0x8\000\00000\000Instruction architecturally executed\000 */
{ 24711 }, /* inst_spec\000instruction\000Operation speculatively executed\000event=0x1b\000\00000\000Operation speculatively executed\000 */
{ 4759 }, /* itlb_walk\000cache\000Access to instruction TLB that causes a translation table walk\000event=0x35\000\00000\000Access to instruction TLB that causes a translation table walk\000 */
{ 3313 }, /* l1d_cache\000cache\000Level 1 data cache access\000event=0x4\000\00000\000Level 1 data cache access\000 */
{ 4916 }, /* l1d_cache_lmiss_rd\000cache\000Level 1 data cache long-latency read miss\000event=0x39\000\00000\000Level 1 data cache long-latency read miss.  The counter counts each memory read access counted by L1D_CACHE that incurs additional latency because it returns data from outside the Level 1 data or unified cache of this processing element\000 */
{ 1587 }, /* l1d_cache_rd\000cache\000L1D cache access, read\000event=0x40\000\00000\000Attributable Level 1 data cache access, read\000 */
{ 3224 }, /* l1d_cache_refill\000cache\000Level 1 data cache refill\000event=0x3\000\00000\000Level 1 data cache refill\000 */
{ 39214 }, /* l1d_cache_refill_inner\000cache\000L1D cache refill, inner\000event=0x44\000\00000\000Attributable Level 1 data cache refill, inner\000 */
{ 39328 }, /* l1d_cache_refill_outer\000cache\000L1D cache refill, outer\000event=0x45\000\00000\000Attributable Level 1 data cache refill, outer\000 */
{ 40504 }, /* l1d_cache_refill_prefetch\000cache\000L1 data cache refill due to prefetch. This event counts any linefills from the prefetcher that cause an allocation into the L1 data cache\000event=0xc2\000\00000\000L1 data cache refill due to prefetch. This event counts any linefills from the prefetcher that cause an allocation into the L1 data cache\000 */
{ 1793 }, /* l1d_cache_refill_rd\000cache\000L1D cache refill, read\000event=0x42\000\00000\000Attributable Level 1 data cache refill, read\000 */
{ 39103 }, /* l1d_cache_refill_wr\000cache\000L1D cache refill, write\000event=0x43\000\00000\000Attributable Level 1 data cache refill, write\000 */
{ 33516 }, /* l1d_cache_wb\000cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000Attributable Level 1 data cache write-back\000 */
{ 1689 }, /* l1d_cache_wr\000cache\000L1D cache access, write\000event=0x41\000\00000\000Attributable Level 1 data cache access, write\000 */
{ 3908 }, /* l1d_tlb\000cache\000Attributable Level 1 data or unified TLB access\000event=0x25\000\00000\000Attributable Level 1 data or unified TLB access\000 */
{ 3395 }, /* l1d_tlb_refill\000cache\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Attributable Level 1 data TLB refill\000 */
{ 41413 }, /* l1d_ws_mode\000cache\000L1 data cache write streaming mode. This event counts for each cycle where the core is in write streaming mode and is not allocating writes into the L1 data cache\000event=0xc5\000\00000\000L1 data cache write streaming mode. This event counts for each cycle where the core is in write streaming mode and is not allocating writes into the L1 data cache\000 */
{ 41166 }, /* l1d_ws_mode_entry\000cache\000L1 data cache entering write streaming mode. This event counts for each entry into write streaming mode\000event=0xc4\000\00000\000L1 data cache entering write streaming mode. This event counts for each entry into write streaming mode\000 */
{ 3504 }, /* l1i_cache\000cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Attributable Level 1 instruction cache access\000 */
{ 5235 }, /* l1i_cache_lmiss\000cache\000Level 1 instruction cache long-latency read miss\000event=0x4006\000\00000\000Level 1 instruction cache long-latency read miss.  If the L1I_CACHE_RD event is implemented, the counter counts each access counted by L1I_CACHE_RD that incurs additional latency because it returns instructions from outside of the Level 1 instruction cache of this PE.  If the L1I_CACHE_RD event is not implemented, the counter counts each access counted by L1I_CACHE that incurs additional latency because it returns instructions from outside the Level 1 instruction cache of this PE.  The event indicates to software that the access missed in the Level 1 instruction cache and might have a significant performance impact due to the additional latency, compared to the latency of an access that hits in the Level 1 instruction cache\000 */
{ 2998 }, /* l1i_cache_refill\000cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Level 1 instruction cache refill\000 */
{ 4033 }, /* l1i_tlb\000cache\000Attributable Level 1 instruction TLB access\000event=0x26\000\00000\000Attributable Level 1 instruction TLB access\000 */
{ 3101 }, /* l1i_tlb_refill\000cache\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Attributable Level 1 instruction TLB refill\000 */
{ 3627 }, /* l2d_cache\000cache\000Level 2 data cache access\000event=0x16\000\00000\000Level 2 data cache access\000 */
{ 38627 }, /* l2d_cache_allocate\000cache\000Attributable Level 2 data cache allocation without refill\000event=0x20\000\00000\000Attributable Level 2 data cache allocation without refill\000 */
{ 6057 }, /* l2d_cache_lmiss_rd\000cache\000Level 2 data cache long-latency read miss\000event=0x4009\000\00000\000Level 2 data cache long-latency read miss.  The counter counts each memory read access counted by L2D_CACHE that incurs additional latency because it returns data from outside the Level 2 data or unified cache of this processing element.  The event indicates to software that the access missed in the Level 2 data or unified cache and might have a significant performance impact compared to the latency of an access that hits in the Level 2 data or unified cache\000 */
{ 2211 }, /* l2d_cache_rd\000cache\000L2D cache access, read\000event=0x50\000\00000\000Attributable Level 2 data cache access, read\000 */
{ 3710 }, /* l2d_cache_refill\000cache\000Level 2 data refill\000event=0x17\000\00000\000Level 2 data refill\000 */
{ 39741 }, /* l2d_cache_refill_prefetch\000cache\000L2 cache refill due to prefetch. If the complex is configured with a per-complex L2 cache, this event does not count. If the complex is configured without a per-complex L2 cache, this event counts the cluster cache event, as defined by L3D_CACHE_REFILL_PREFETCH. If neither a per-complex cache or a cluster cache is configured, this event is not implemented\000event=0xc1\000\00000\000L2 cache refill due to prefetch. If the complex is configured with a per-complex L2 cache, this event does not count. If the complex is configured without a per-complex L2 cache, this event counts the cluster cache event, as defined by L3D_CACHE_REFILL_PREFETCH. If neither a per-complex cache or a cluster cache is configured, this event is not implemented\000 */
{ 2417 }, /* l2d_cache_refill_rd\000cache\000L2D cache refill, read\000event=0x52\000\00000\000Attributable Level 2 data cache refill, read\000 */
{ 2526 }, /* l2d_cache_refill_wr\000cache\000L2D cache refill, write\000event=0x53\000\00000\000Attributable Level 2 data cache refill, write\000 */
{ 43949 }, /* l2d_cache_stash_dropped\000cache\000L2 cache stash dropped. This event counts on each stash request that is received from the interconnect or the Accelerator Coherency Port (ACP), that targets L2 cache and is dropped due to lack of buffer space to hold the request\000event=0xd6\000\00000\000L2 cache stash dropped. This event counts on each stash request that is received from the interconnect or the Accelerator Coherency Port (ACP), that targets L2 cache and is dropped due to lack of buffer space to hold the request\000 */
{ 3788 }, /* l2d_cache_wb\000cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Attributable Level 2 data cache write-back\000 */
{ 2313 }, /* l2d_cache_wr\000cache\000L2D cache access, write\000event=0x51\000\00000\000Attributable Level 2 data cache access, write\000 */
{ 42848 }, /* l2d_s2_tlb\000cache\000L2 TLB IPA cache access. This event counts on each access to the IPA cache. If a single translation table walk needs to make multiple accesses to the IPA cache, each access is counted. If stage 2 translation is disabled, this event does not count\000event=0xd4\000\00000\000L2 TLB IPA cache access. This event counts on each access to the IPA cache. If a single translation table walk needs to make multiple accesses to the IPA cache, each access is counted. If stage 2 translation is disabled, this event does not count\000 */
{ 43374 }, /* l2d_s2_tlb_refill\000cache\000L2 TLB IPA cache refill. This event counts on each refill of the IPA cache. If a single translation table walk needs to make multiple accesses to the IPA cache, each access that causes a refill is counted. If stage 2 translation is disabled, this event does not count\000event=0xd5\000\00000\000L2 TLB IPA cache refill. This event counts on each refill of the IPA cache. If a single translation table walk needs to make multiple accesses to the IPA cache, each access that causes a refill is counted. If stage 2 translation is disabled, this event does not count\000 */
{ 4384 }, /* l2d_tlb\000cache\000Attributable Level 2 data or unified TLB access\000event=0x2f\000\00000\000Attributable Level 2 data or unified TLB access\000 */
{ 4150 }, /* l2d_tlb_refill\000cache\000Attributable Level 2 data TLB refill\000event=0x2d\000\00000\000Attributable Level 2 data TLB refill\000 */
{ 42473 }, /* l2d_walk_tlb\000cache\000L2 TLB walk cache access. This event does not count if the MMU is disabled\000event=0xd0\000\00000\000L2 TLB walk cache access. This event does not count if the MMU is disabled\000 */
{ 42657 }, /* l2d_walk_tlb_refill\000cache\000L2 TLB walk cache refill. This event does not count if the MMU is disabled\000event=0xd1\000\00000\000L2 TLB walk cache refill. This event does not count if the MMU is disabled\000 */
{ 40827 }, /* l2d_ws_mode\000cache\000L2 cache write streaming mode. This event counts for each cycle where the core is in write streaming mode and is not allocating writes into the L2 cache\000event=0xc3\000\00000\000L2 cache write streaming mode. This event counts for each cycle where the core is in write streaming mode and is not allocating writes into the L2 cache\000 */
{ 38783 }, /* l3d_cache\000cache\000Attributable Level 3 data cache access\000event=0x2b\000\00000\000Attributable Level 3 data cache access\000 */
{ 44452 }, /* l3d_cache_lmiss_rd\000cache\000Level 3 data cache long-latency read miss\000event=0x400b\000\00000\000Level 3 data cache long-latency read miss.  The counter counts each memory read access counted by L3D_CACHE that incurs additional latency because it returns data from outside the Level 3 data or unified cache of this processing element.  The event indicates to software that the access missed in the Level 3 data or unified cache and might have a significant performance impact compared to the latency of an access that hits in the Level 3 data or unified cache\000 */
{ 39442 }, /* l3d_cache_rd\000cache\000Attributable Level 3 data or unified cache access, read\000event=0xa0\000\00000\000Attributable Level 3 data or unified cache access, read\000 */
{ 39588 }, /* l3d_cache_refill_rd\000cache\000Attributable Level 3 data or unified cache refill, read\000event=0xa2\000\00000\000Attributable Level 3 data or unified cache refill, read\000 */
{ 41772 }, /* l3d_ws_mode\000cache\000L3 cache write streaming mode. This event counts for each cycle where the core is in write streaming mode and is not allocating writes into the L3 cache\000event=0xc7\000\00000\000L3 cache write streaming mode. This event counts for each cycle where the core is in write streaming mode and is not allocating writes into the L3 cache\000 */
{ 26884 }, /* ld_align_lat\000memory\000Load with additional latency from alignment\000event=0x4021\000\00000\000Load with additional latency from alignment\000 */
{ 33712 }, /* ld_retired\000instruction\000Instruction architecturally executed, condition code check pass, load\000event=0x6\000\00000\000Instruction architecturally executed, condition code check pass, load\000 */
{ 21982 }, /* ld_spec\000instruction\000Operation speculatively executed, load\000event=0x70\000\00000\000Operation speculatively executed, load\000 */
{ 27280 }, /* ldst_align_lat\000memory\000Access with additional latency from alignment\000event=0x4020\000\00000\000Access with additional latency from alignment\000 */
{ 22210 }, /* ldst_spec\000instruction\000Operation speculatively executed, load or store\000event=0x72\000\00000\000Operation speculatively executed, load or store\000 */
{ 39009 }, /* ll_cache_miss_rd\000cache\000Last level cache miss, read\000event=0x37\000\00000\000Last level cache miss, read\000 */
{ 38892 }, /* ll_cache_rd\000cache\000Attributable Last level cache memory read\000event=0x36\000\00000\000Attributable Last level cache memory read\000 */
{ 42111 }, /* ll_ws_mode\000cache\000Last level cache write streaming mode. This event counts for each cycle where the core is in write streaming mode and is not allocating writes into the system cache\000event=0xc8\000\00000\000Last level cache write streaming mode. This event counts for each cycle where the core is in write streaming mode and is not allocating writes into the system cache\000 */
{ 27136 }, /* mem_access\000memory\000Data memory access\000event=0x13\000\00000\000Data memory access\000 */
{ 27411 }, /* mem_access_checked\000memory\000Checked data memory access\000event=0x4024\000\00000\000Checked data memory access\000 */
{ 27508 }, /* mem_access_checked_rd\000memory\000Checked data memory access, read\000event=0x4025\000\00000\000Checked data memory access, read\000 */
{ 27620 }, /* mem_access_checked_wr\000memory\000Checked data memory access, write\000event=0x4026\000\00000\000Checked data memory access, write\000 */
{ 26460 }, /* mem_access_rd\000memory\000Data memory access, read\000event=0x66\000\00000\000Data memory access, read\000 */
{ 26546 }, /* mem_access_wr\000memory\000Data memory access, write\000event=0x67\000\00000\000Data memory access, write\000 */
{ 33636 }, /* memory_error\000exception\000Local memory error\000event=0x1a\000\00000\000Local memory error\000 */
{ 25309 }, /* op_retired\000instruction\000Micro-operation architecturally executed\000event=0x3a\000\00000\000Micro-operation architecturally executed.  The counter counts each operation counted by OP_SPEC that would be executed in a simple sequential execution of the program\000 */
{ 21665 }, /* op_spec\000instruction\000Micro-operation speculatively executed\000event=0x3b\000\00000\000Micro-operation speculatively executed.  The counter counts the number of operations executed by the processing element, including those that are executed speculatively and would not be executed in a simple sequential execution of the program\000 */
{ 24486 }, /* pc_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, software change of the PC\000event=0xc\000\00000\000Instruction architecturally executed, condition code check pass, software change of the PC\000 */
{ 22808 }, /* pc_write_spec\000instruction\000Operation speculatively executed, software change of the PC\000event=0x76\000\00000\000Operation speculatively executed, software change of the PC\000 */
{ 53475 }, /* pmu_hovfs\000pmu\000PMU overflow, counters reserved for use by EL2\000event=0x400f\000\00000\000PMU overflow, counters reserved for use by EL2\000 */
{ 53347 }, /* pmu_ovfs\000pmu\000PMU overflow, counters accessible to EL1 and EL0\000event=0x400d\000\00000\000PMU overflow, counters accessible to EL1 and EL0\000 */
{ 46517 }, /* remote_access_rd\000memory\000Attributable memory read access to another socket in a multi-socket system\000event=0x38\000\00000\000Attributable memory read access to another socket in a multi-socket system\000 */
{ 27009 }, /* st_align_lat\000memory\000Store with additional latency from alignment\000event=0x4022\000\00000\000Store with additional latency from alignment\000 */
{ 21486 }, /* st_retired\000instruction\000Instruction architecturally executed, condition code check pass, store\000event=0x7\000\00000\000Instruction architecturally executed, condition code check pass, store\000 */
{ 22095 }, /* st_spec\000instruction\000Operation speculatively executed, store\000event=0x71\000\00000\000Operation speculatively executed, store\000 */
{ 46833 }, /* stall\000pipeline\000No operation sent for execution\000event=0x3c\000\00000\000No operation sent for execution.  The counter counts every attributable cycle on which no attributable instruction or operation was sent for execution on this processing element\000 */
{ 27892 }, /* stall_backend\000pipeline\000No operation issued due to the backend\000event=0x24\000\00000\000No operation issued due to the backend\000 */
{ 48494 }, /* stall_backend_ilock\000pipeline\000No operation issued due to the backend interlock. This event counts every cycle where the issue of an operation is stalled and there is an interlock. Stall cycles due to a stall in the Wr stage are excluded\000event=0xe4\000\00000\000No operation issued due to the backend interlock. This event counts every cycle where the issue of an operation is stalled and there is an interlock. Stall cycles due to a stall in the Wr stage are excluded\000 */
{ 48952 }, /* stall_backend_ilock_addr\000pipeline\000No operation issued due to the backend, address interlock. This event counts every cycle where the issue of an operation is stalled and there is an interlock on an address operand. This type of interlock is caused by a load/store instruction waiting for data to calculate the address. Stall cycles due to a stall in the Wr stage are excluded\000event=0xe5\000\00000\000No operation issued due to the backend, address interlock. This event counts every cycle where the issue of an operation is stalled and there is an interlock on an address operand. This type of interlock is caused by a load/store instruction waiting for data to calculate the address. Stall cycles due to a stall in the Wr stage are excluded\000 */
{ 49685 }, /* stall_backend_ilock_vpu\000pipeline\000No operation issued due to the backend, interlock, or the Vector Processing Unit (VPU). This event counts every cycle where there is a stall or an interlock that is caused by a VPU instruction. Stall cycles due to a stall in the Wr stage are excluded\000event=0xe6\000\00000\000No operation issued due to the backend, interlock, or the Vector Processing Unit (VPU). This event counts every cycle where there is a stall or an interlock that is caused by a VPU instruction. Stall cycles due to a stall in the Wr stage are excluded\000 */
{ 50235 }, /* stall_backend_ld\000pipeline\000No operation issued due to the backend, load. This event counts every cycle where there is a stall in the Wr stage due to a load\000event=0xe7\000\00000\000No operation issued due to the backend, load. This event counts every cycle where there is a stall in the Wr stage due to a load\000 */
{ 50837 }, /* stall_backend_ld_cache\000pipeline\000No operation issued due to the backend, load, cache miss. This event counts every cycle where there is a stall in the Wr stage due to a load that is waiting on data. The event counts for stalls that are caused by missing the cache or where the data is Non-cacheable\000event=0xe9\000\00000\000No operation issued due to the backend, load, cache miss. This event counts every cycle where there is a stall in the Wr stage due to a load that is waiting on data. The event counts for stalls that are caused by missing the cache or where the data is Non-cacheable\000 */
{ 51416 }, /* stall_backend_ld_tlb\000pipeline\000No operation issued due to the backend, load, TLB miss. This event counts every cycle where there is a stall in the Wr stage due to a load that misses in the L1 TLB\000event=0xea\000\00000\000No operation issued due to the backend, load, TLB miss. This event counts every cycle where there is a stall in the Wr stage due to a load that misses in the L1 TLB\000 */
{ 29344 }, /* stall_backend_mem\000pipeline\000Memory stall cycles\000event=0x4005\000\00000\000Memory stall cycles.  The counter counts each cycle counted by STALL_BACKEND where there is a cache miss in the last level of cache within the processing element clock domain\000 */
{ 50534 }, /* stall_backend_st\000pipeline\000No operation issued due to the backend, store. This event counts every cycle where there is a stall in the Wr stage due to a store\000event=0xe8\000\00000\000No operation issued due to the backend, store. This event counts every cycle where there is a stall in the Wr stage due to a store\000 */
{ 51791 }, /* stall_backend_st_stb\000pipeline\000No operation issued due to the backend, store, Store Buffer (STB) full. This event counts every cycle where there is a stall in the Wr stage because of a store operation that is waiting due to the STB being full\000event=0xeb\000\00000\000No operation issued due to the backend, store, Store Buffer (STB) full. This event counts every cycle where there is a stall in the Wr stage because of a store operation that is waiting due to the STB being full\000 */
{ 52260 }, /* stall_backend_st_tlb\000pipeline\000No operation issued due to the backend, store, TLB miss. This event counts every cycle where there is a stall in the Wr stage because of a store operation that has missed in the L1 TLB\000event=0xec\000\00000\000No operation issued due to the backend, store, TLB miss. This event counts every cycle where there is a stall in the Wr stage because of a store operation that has missed in the L1 TLB\000 */
{ 52675 }, /* stall_backend_vpu_hazard\000pipeline\000No operation issued due to the backend, VPU hazard. This event counts every cycle where the core stalls due to contention for the VPU with the other core\000event=0xed\000\00000\000No operation issued due to the backend, VPU hazard. This event counts every cycle where the core stalls due to contention for the VPU with the other core\000 */
{ 46706 }, /* stall_frontend\000pipeline\000No operation issued because of the frontend\000event=0x23\000\00000\000No operation issued because of the frontend\000 */
{ 47489 }, /* stall_frontend_cache\000pipeline\000No operation issued due to the frontend, cache miss. This event counts every cycle that the Data Processing Unit (DPU) instruction queue is empty and there is an instruction cache miss being processed\000event=0xe1\000\00000\000No operation issued due to the frontend, cache miss. This event counts every cycle that the Data Processing Unit (DPU) instruction queue is empty and there is an instruction cache miss being processed\000 */
{ 48333 }, /* stall_frontend_pderr\000pipeline\000No operation issued due to the frontend, pre-decode error\000event=0xe3\000\00000\000No operation issued due to the frontend, pre-decode error\000 */
{ 47936 }, /* stall_frontend_tlb\000pipeline\000No operation issued due to the frontend, TLB miss. This event counts every cycle that the DPU instruction queue is empty and there is an instruction L1 TLB miss being processed\000event=0xe2\000\00000\000No operation issued due to the frontend, TLB miss. This event counts every cycle that the DPU instruction queue is empty and there is an instruction L1 TLB miss being processed\000 */
{ 29039 }, /* stall_slot\000pipeline\000No operation sent for execution on a slot\000event=0x3f\000\00000\000No operation sent for execution on a slot.  The counter counts on each attributable cycle the number of instruction or operation slots that were not occupied by an instruction or operation attributable to the processing element\000 */
{ 28291 }, /* stall_slot_backend\000pipeline\000No operation sent for execution on a slot due to the backend\000event=0x3d\000\00000\000No operation sent for execution on a slot due to the backend.  Counts each slot counted by STALL_SLOT where no attributable instruction or operation was sent for execution because the backend is unable to accept it\000 */
{ 53032 }, /* stall_slot_backend_ilock\000pipeline\000Issue slot not issued due to interlock. For each cycle, this event counts each dispatch slot that does not issue due to an interlock\000event=0xee\000\00000\000Issue slot not issued due to interlock. For each cycle, this event counts each dispatch slot that does not issue due to an interlock\000 */
{ 47073 }, /* stall_slot_frontend\000pipeline\000No operation sent for execution on a slot due to the frontend\000event=0x3e\000\00000\000No operation sent for execution on a slot due to the frontend.  Counts each slot counted by STALL_SLOT where no attributable instruction or operation was sent for execution because there was no attributable instruction or operation available to issue from the processing element from the frontend for the slot\000 */
{ 44999 }, /* sve_inst_retired\000instruction\000Instruction architecturally executed, SVE\000event=0x8002\000\00000\000Instruction architecturally executed, SVE\000 */
{ 45129 }, /* sve_inst_spec\000instruction\000SVE operations speculatively executed\000event=0x8006\000\00000\000SVE operations speculatively executed\000 */
{ 53718 }, /* trb_trig\000trace\000Trace buffer Trigger Event\000event=0x400e\000\00000\000Trace buffer Trigger Event\000 */
{ 53600 }, /* trb_wrap\000trace\000Trace buffer current write pointer wrapped\000event=0x400c\000\00000\000Trace buffer current write pointer wrapped\000 */
{ 53804 }, /* trcextout0\000trace\000PE Trace Unit external output 0\000event=0x4010\000\00000\000PE Trace Unit external output 0\000 */
{ 53902 }, /* trcextout1\000trace\000PE Trace Unit external output 1\000event=0x4011\000\00000\000PE Trace Unit external output 1\000 */
{ 54000 }, /* trcextout2\000trace\000PE Trace Unit external output 2\000event=0x4012\000\00000\000PE Trace Unit external output 2\000 */
{ 54098 }, /* trcextout3\000trace\000PE Trace Unit external output 3\000event=0x4013\000\00000\000PE Trace Unit external output 3\000 */
{ 24814 }, /* ttbr_write_retired\000instruction\000Instruction architecturally executed, Condition code check pass, write to TTBR\000event=0x1c\000\00000\000Instruction architecturally executed, Condition code check pass, write to TTBR\000 */
{ 22650 }, /* vfp_spec\000instruction\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Operation speculatively executed, floating-point instruction\000 */

};

const struct pmu_table_entry pmu_events__arm_cortex_a510[] = {
{
     .entries = pmu_events__arm_cortex_a510_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__arm_cortex_a510_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__arm_cortex_a53_default_core[] = {
{ 56714 }, /* agu_dep_stall\000pipeline\000Cycles there is an interlock for a load/store instruction waiting for data to calculate the address in the AGU\000event=0xe5\000\00000\000\000 */
{ 54670 }, /* br_cond\000branch\000Conditional branch executed\000event=0xc9\000\00000\000\000 */
{ 54908 }, /* br_cond_mispred\000branch\000Conditional branch mispredicted\000event=0xcc\000\00000\000\000 */
{ 54729 }, /* br_indirect_mispred\000branch\000Indirect branch mispredicted\000event=0xca\000\00000\000\000 */
{ 54801 }, /* br_indirect_mispred_addr\000branch\000Indirect branch mispredicted because of address miscompare\000event=0xcb\000\00000\000\000 */
{ 278 }, /* br_indirect_spec\000branch\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000 */
{ 771 }, /* bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Bus access read\000 */
{ 836 }, /* bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Bus access write\000 */
{ 56441 }, /* decode_dep_stall\000pipeline\000Cycles the DPU IQ is empty and there is a pre-decode error being processed\000event=0xe3\000\00000\000\000 */
{ 55533 }, /* exc_fiq\000other\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000 */
{ 55462 }, /* exc_irq\000other\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000 */
{ 55327 }, /* ext_mem_req\000memory\000External memory request\000event=0xc0\000\00000\000\000 */
{ 55386 }, /* ext_mem_req_nc\000memory\000Non-cacheable external memory request\000event=0xc1\000\00000\000\000 */
{ 55248 }, /* ext_snoop\000cache\000SCU Snooped data from another CPU for this CPU\000event=0xc8\000\00000\000\000 */
{ 56194 }, /* ic_dep_stall\000pipeline\000Cycles the DPU IQ is empty and there is an instruction cache miss being processed\000event=0xe1\000\00000\000\000 */
{ 56314 }, /* iutlb_dep_stall\000pipeline\000Cycles the DPU IQ is empty and there is an instruction micro-TLB miss being processed\000event=0xe2\000\00000\000\000 */
{ 55742 }, /* l1d_cache_err\000other\000L1 Data Cache (data, tag or dirty) memory error, correctable or non-correctable\000event=0xd1\000\00000\000\000 */
{ 55658 }, /* l1i_cache_err\000other\000L1 Instruction Cache (data or tag) memory error\000event=0xd0\000\00000\000\000 */
{ 56979 }, /* ld_dep_stall\000pipeline\000Cycles there is a stall in the Wr stage because of a load miss\000event=0xe7\000\00000\000\000 */
{ 56558 }, /* other_interlock_stall\000pipeline\000Cycles there is an interlock other than  Advanced SIMD/Floating-point instructions or load/store instruction\000event=0xe4\000\00000\000\000 */
{ 56023 }, /* other_iq_dep_stall\000pipeline\000Cycles that the DPU IQ is empty and that is not because of a recent micro-TLB miss, instruction cache miss or pre-decode error\000event=0xe0\000\00000\000\000 */
{ 55604 }, /* pre_decode_err\000other\000Pre-decode error\000event=0xc6\000\00000\000\000 */
{ 54979 }, /* prefetch_linefill\000cache\000Linefill because of prefetch\000event=0xc2\000\00000\000\000 */
{ 55048 }, /* prefetch_linefill_drop\000cache\000Instruction Cache Throttle occurred\000event=0xc3\000\00000\000\000 */
{ 55196 }, /* read_alloc\000cache\000Read allocate mode\000event=0xc5\000\00000\000\000 */
{ 55129 }, /* read_alloc_enter\000cache\000Entering read allocate mode\000event=0xc4\000\00000\000\000 */
{ 56864 }, /* simd_dep_stall\000pipeline\000Cycles there is an interlock for an Advanced SIMD/Floating-point operation\000event=0xe6\000\00000\000\000 */
{ 57080 }, /* st_dep_stall\000pipeline\000Cycles there is a stall in the Wr stage because of a store\000event=0xe8\000\00000\000\000 */
{ 55905 }, /* stall_sb_full\000pipeline\000Data Write operation that stalls the pipeline because the store buffer is full\000event=0xc7\000\00000\000\000 */
{ 55858 }, /* tlb_err\000other\000TLB memory error\000event=0xd2\000\00000\000\000 */

};

const struct pmu_table_entry pmu_events__arm_cortex_a53[] = {
{
     .entries = pmu_events__arm_cortex_a53_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__arm_cortex_a53_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__arm_cortex_a55_default_core[] = {
{ 22494 }, /* ase_spec\000instruction\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Operation speculatively executed, Advanced SIMD instruction\000 */
{ 58903 }, /* br_cond_mis_pred\000branch\000Conditional branch mis-predicted.This event counts when any branch which can be predicted by the conditional predictor is retired, and has mis-predicted the condition. This event still counts when branch prediction is disabled due to the MMU being off. Conditional indirect branches which correctly predicted the condition but mis-predicted on the address do not count this event\000event=0xcc\000\00000\000Conditional branch mis-predicted.This event counts when any branch which can be predicted by the conditional predictor is retired, and has mis-predicted the condition. This event still counts when branch prediction is disabled due to the MMU being off. Conditional indirect branches which correctly predicted the condition but mis-predicted on the address do not count this event\000 */
{ 57177 }, /* br_cond_pred\000branch\000Predicted conditional branch executed.This event counts when any branch which can be predicted by the conditional predictor is retired. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xc9\000\00000\000Predicted conditional branch executed.This event counts when any branch which can be predicted by the conditional predictor is retired. This event still counts when branch prediction is disabled due to the MMU being off\000 */
{ 22969 }, /* br_immed_retired\000instruction\000Instruction architecturally executed, immediate branch\000event=0xd\000\00000\000Instruction architecturally executed, immediate branch\000 */
{ 13 }, /* br_immed_spec\000branch\000Branch speculatively executed, immediate branch\000event=0x78\000\00000\000Branch speculatively executed, immediate branch\000 */
{ 58227 }, /* br_indirect_addr_mis_pred\000branch\000Indirect branch mis-predicted due to address mis-compare.This event counts when any indirect branch which can be predicted by the BTAC is retired, was taken and correctly predicted the condition, and has mispredicted the address. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xcb\000\00000\000Indirect branch mis-predicted due to address mis-compare.This event counts when any indirect branch which can be predicted by the BTAC is retired, was taken and correctly predicted the condition, and has mispredicted the address. This event still counts when branch prediction is disabled due to the MMU being off\000 */
{ 59702 }, /* br_indirect_addr_pred\000branch\000Indirect branch with predicted address executed.This event counts when any indirect branch which can be predicted by the BTAC is retired, was taken and correctly predicted the condition. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xcd\000\00000\000Indirect branch with predicted address executed.This event counts when any indirect branch which can be predicted by the BTAC is retired, was taken and correctly predicted the condition. This event still counts when branch prediction is disabled due to the MMU being off\000 */
{ 57652 }, /* br_indirect_mis_pred\000branch\000Indirect branch mis-predicted.This event counts when any indirect branch which can be predicted by the BTAC is retired, and has mispredicted for either the condition or the address. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xca\000\00000\000Indirect branch mis-predicted.This event counts when any indirect branch which can be predicted by the BTAC is retired, and has mispredicted for either the condition or the address. This event still counts when branch prediction is disabled due to the MMU being off\000 */
{ 278 }, /* br_indirect_spec\000branch\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000 */
{ 411 }, /* br_mis_pred\000branch\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000Mispredicted or not predicted branch speculatively executed\000 */
{ 25146 }, /* br_mis_pred_retired\000instruction\000Instruction architecturally executed, mispredicted branch\000event=0x22\000\00000\000Instruction architecturally executed, mispredicted branch\000 */
{ 565 }, /* br_pred\000branch\000Predictable branch speculatively executed\000event=0x12\000\00000\000Predictable branch speculatively executed\000 */
{ 25018 }, /* br_retired\000instruction\000Instruction architecturally executed, branch\000event=0x21\000\00000\000Instruction architecturally executed, branch\000 */
{ 60874 }, /* br_return_addr_mis_pred\000branch\000Procedure return mis-predicted due to address mis-compare.This event counts when any procedure return which can be predicted by the CRS is retired, was taken and correctly predicted the condition, and has mispredicted the address. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xcf\000\00000\000Procedure return mis-predicted due to address mis-compare.This event counts when any procedure return which can be predicted by the CRS is retired, was taken and correctly predicted the condition, and has mispredicted the address. This event still counts when branch prediction is disabled due to the MMU being off\000 */
{ 60288 }, /* br_return_addr_pred\000branch\000Procedure return with predicted address executed.This event counts when any procedure return which can be predicted by the CRS is retired, was taken and correctly predicted the condition. This event still counts when branch prediction is disabled due to the MMU being off\000event=0xce\000\00000\000Procedure return with predicted address executed.This event counts when any procedure return which can be predicted by the CRS is retired, was taken and correctly predicted the condition. This event still counts when branch prediction is disabled due to the MMU being off\000 */
{ 23122 }, /* br_return_retired\000instruction\000Instruction architecturally executed, condition code check pass, procedure return\000event=0xe\000\00000\000Instruction architecturally executed, condition code check pass, procedure return\000 */
{ 145 }, /* br_return_spec\000branch\000Branch speculatively executed, procedure return\000event=0x79\000\00000\000Branch speculatively executed, procedure return\000 */
{ 1311 }, /* bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000Attributable Bus access\000 */
{ 771 }, /* bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Bus access read\000 */
{ 836 }, /* bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Bus access write\000 */
{ 721 }, /* bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000Bus cycle\000 */
{ 24272 }, /* cid_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000 */
{ 679 }, /* cpu_cycles\000bus\000Cycle\000event=0x11\000\00000\000Cycle\000 */
{ 23330 }, /* crypto_spec\000instruction\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Operation speculatively executed, Cryptographic instruction\000 */
{ 22343 }, /* dp_spec\000instruction\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Operation speculatively executed, integer data processing\000 */
{ 4626 }, /* dtlb_walk\000cache\000Access to data TLB causes a translation table walk\000event=0x34\000\00000\000Access to data TLB causes a translation table walk\000 */
{ 20191 }, /* exc_fiq\000exception\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000 */
{ 20116 }, /* exc_irq\000exception\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000 */
{ 33889 }, /* exc_return\000instruction\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Instruction architecturally executed, condition check pass, exception return\000 */
{ 21031 }, /* exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Exception taken\000 */
{ 24159 }, /* inst_retired\000instruction\000Instruction architecturally executed\000event=0x8\000\00000\000Instruction architecturally executed\000 */
{ 24711 }, /* inst_spec\000instruction\000Operation speculatively executed\000event=0x1b\000\00000\000Operation speculatively executed\000 */
{ 4759 }, /* itlb_walk\000cache\000Access to instruction TLB that causes a translation table walk\000event=0x35\000\00000\000Access to instruction TLB that causes a translation table walk\000 */
{ 3313 }, /* l1d_cache\000cache\000Level 1 data cache access\000event=0x4\000\00000\000Level 1 data cache access\000 */
{ 1587 }, /* l1d_cache_rd\000cache\000L1D cache access, read\000event=0x40\000\00000\000Attributable Level 1 data cache access, read\000 */
{ 3224 }, /* l1d_cache_refill\000cache\000Level 1 data cache refill\000event=0x3\000\00000\000Level 1 data cache refill\000 */
{ 39214 }, /* l1d_cache_refill_inner\000cache\000L1D cache refill, inner\000event=0x44\000\00000\000Attributable Level 1 data cache refill, inner\000 */
{ 39328 }, /* l1d_cache_refill_outer\000cache\000L1D cache refill, outer\000event=0x45\000\00000\000Attributable Level 1 data cache refill, outer\000 */
{ 63648 }, /* l1d_cache_refill_prefetch\000cache\000Level 1 data cache refill due to prefetch. This event counts any linefills from the prefetcher which cause an allocation into the L1 D-cache\000event=0xc2\000\00000\000Level 1 data cache refill due to prefetch. This event counts any linefills from the prefetcher which cause an allocation into the L1 D-cache\000 */
{ 1793 }, /* l1d_cache_refill_rd\000cache\000L1D cache refill, read\000event=0x42\000\00000\000Attributable Level 1 data cache refill, read\000 */
{ 39103 }, /* l1d_cache_refill_wr\000cache\000L1D cache refill, write\000event=0x43\000\00000\000Attributable Level 1 data cache refill, write\000 */
{ 33516 }, /* l1d_cache_wb\000cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000Attributable Level 1 data cache write-back\000 */
{ 1689 }, /* l1d_cache_wr\000cache\000L1D cache access, write\000event=0x41\000\00000\000Attributable Level 1 data cache access, write\000 */
{ 3908 }, /* l1d_tlb\000cache\000Attributable Level 1 data or unified TLB access\000event=0x25\000\00000\000Attributable Level 1 data or unified TLB access\000 */
{ 3395 }, /* l1d_tlb_refill\000cache\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Attributable Level 1 data TLB refill\000 */
{ 64575 }, /* l1d_ws_mode\000cache\000Level 1 data cache write streaming mode.This event counts for each cycle where the core is in write-streaming mode and not allocating writes into the L1 D-cache\000event=0xc5\000\00000\000Level 1 data cache write streaming mode.This event counts for each cycle where the core is in write-streaming mode and not allocating writes into the L1 D-cache\000 */
{ 64320 }, /* l1d_ws_mode_entry\000cache\000Level 1 data cache entering write streaming mode.This event counts for each entry into write-streaming mode\000event=0xc4\000\00000\000Level 1 data cache entering write streaming mode.This event counts for each entry into write-streaming mode\000 */
{ 3504 }, /* l1i_cache\000cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Attributable Level 1 instruction cache access\000 */
{ 2998 }, /* l1i_cache_refill\000cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Level 1 instruction cache refill\000 */
{ 4033 }, /* l1i_tlb\000cache\000Attributable Level 1 instruction TLB access\000event=0x26\000\00000\000Attributable Level 1 instruction TLB access\000 */
{ 3101 }, /* l1i_tlb_refill\000cache\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Attributable Level 1 instruction TLB refill\000 */
{ 3627 }, /* l2d_cache\000cache\000Level 2 data cache access\000event=0x16\000\00000\000Level 2 data cache access\000 */
{ 38627 }, /* l2d_cache_allocate\000cache\000Attributable Level 2 data cache allocation without refill\000event=0x20\000\00000\000Attributable Level 2 data cache allocation without refill\000 */
{ 2211 }, /* l2d_cache_rd\000cache\000L2D cache access, read\000event=0x50\000\00000\000Attributable Level 2 data cache access, read\000 */
{ 3710 }, /* l2d_cache_refill\000cache\000Level 2 data refill\000event=0x17\000\00000\000Level 2 data refill\000 */
{ 62861 }, /* l2d_cache_refill_prefetch\000cache\000Level 2 cache refill due to prefetch. +//0 If the core is configured with a per-core L2 cache: This event does not count. +//0 If the core is configured without a per-core L2 cache: This event counts the cluster cache event, as defined by L3D_CACHE_REFILL_PREFETCH. +//0 If there is neither a per-core cache nor a cluster cache configured, this event is not implemented\000event=0xc1\000\00000\000Level 2 cache refill due to prefetch. +//0 If the core is configured with a per-core L2 cache: This event does not count. +//0 If the core is configured without a per-core L2 cache: This event counts the cluster cache event, as defined by L3D_CACHE_REFILL_PREFETCH. +//0 If there is neither a per-core cache nor a cluster cache configured, this event is not implemented\000 */
{ 2417 }, /* l2d_cache_refill_rd\000cache\000L2D cache refill, read\000event=0x52\000\00000\000Attributable Level 2 data cache refill, read\000 */
{ 2526 }, /* l2d_cache_refill_wr\000cache\000L2D cache refill, write\000event=0x53\000\00000\000Attributable Level 2 data cache refill, write\000 */
{ 67622 }, /* l2d_cache_stash_dropped\000cache\000Level 2 cache stash dropped.This event counts on each stash request received from the interconnect or ACP, that is targeting L2 and gets dropped due to lack of buffer space to hold the request\000event=0xd6\000\00000\000Level 2 cache stash dropped.This event counts on each stash request received from the interconnect or ACP, that is targeting L2 and gets dropped due to lack of buffer space to hold the request\000 */
{ 3788 }, /* l2d_cache_wb\000cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Attributable Level 2 data cache write-back\000 */
{ 2313 }, /* l2d_cache_wr\000cache\000L2D cache access, write\000event=0x51\000\00000\000Attributable Level 2 data cache access, write\000 */
{ 65710 }, /* l2d_l2walk_tlb\000cache\000Level 2 TLB level-2 walk cache access.This event counts accesses to the level-2 walk cache where the last-level walk cache has missed. The event only counts when the translation regime of the pagewalk uses level 2 descriptors. This event does not count if the MMU is disabled\000event=0xd2\000\00000\000Level 2 TLB level-2 walk cache access.This event counts accesses to the level-2 walk cache where the last-level walk cache has missed. The event only counts when the translation regime of the pagewalk uses level 2 descriptors. This event does not count if the MMU is disabled\000 */
{ 66298 }, /* l2d_l2walk_tlb_refill\000cache\000Level 2 TLB level-2 walk cache refill.This event does not count if the MMU is disabled\000event=0xd3\000\00000\000Level 2 TLB level-2 walk cache refill.This event does not count if the MMU is disabled\000 */
{ 65271 }, /* l2d_llwalk_tlb\000cache\000Level 2 TLB last-level walk cache access.This event does not count if the MMU is disabled\000event=0xd0\000\00000\000Level 2 TLB last-level walk cache access.This event does not count if the MMU is disabled\000 */
{ 65487 }, /* l2d_llwalk_tlb_refill\000cache\000Level 2 TLB last-level walk cache refill.This event does not count if the MMU is disabled\000event=0xd1\000\00000\000Level 2 TLB last-level walk cache refill.This event does not count if the MMU is disabled\000 */
{ 66515 }, /* l2d_s2_tlb\000cache\000Level 2 TLB IPA cache access. This event counts on each access to the IPA cache. +//0 If a single pagewalk needs to make multiple accesses to the IPA cache, each access is counted. +//0 If stage 2 translation is disabled, this event does not count\000event=0xd4\000\00000\000Level 2 TLB IPA cache access. This event counts on each access to the IPA cache. +//0 If a single pagewalk needs to make multiple accesses to the IPA cache, each access is counted. +//0 If stage 2 translation is disabled, this event does not count\000 */
{ 67043 }, /* l2d_s2_tlb_refill\000cache\000Level 2 TLB IPA cache refill. This event counts on each refill of the IPA cache. +//0 If a single pagewalk needs to make multiple accesses to the IPA cache, each access which causes a refill is counted. +//0 If stage 2 translation is disabled, this event does not count\000event=0xd5\000\00000\000Level 2 TLB IPA cache refill. This event counts on each refill of the IPA cache. +//0 If a single pagewalk needs to make multiple accesses to the IPA cache, each access which causes a refill is counted. +//0 If stage 2 translation is disabled, this event does not count\000 */
{ 4384 }, /* l2d_tlb\000cache\000Attributable Level 2 data or unified TLB access\000event=0x2f\000\00000\000Attributable Level 2 data or unified TLB access\000 */
{ 4150 }, /* l2d_tlb_refill\000cache\000Attributable Level 2 data TLB refill\000event=0x2d\000\00000\000Attributable Level 2 data TLB refill\000 */
{ 63977 }, /* l2d_ws_mode\000cache\000Level 2 cache write streaming mode. This event counts for each cycle where the core is in write-streaming mode and not allocating writes into the L2 cache\000event=0xc3\000\00000\000Level 2 cache write streaming mode. This event counts for each cycle where the core is in write-streaming mode and not allocating writes into the L2 cache\000 */
{ 38783 }, /* l3d_cache\000cache\000Attributable Level 3 data cache access\000event=0x2b\000\00000\000Attributable Level 3 data cache access\000 */
{ 61550 }, /* l3d_cache_allocate\000cache\000Attributable Level 3 data cache allocation without refill\000event=0x29\000\00000\000Attributable Level 3 data cache allocation without refill\000 */
{ 39442 }, /* l3d_cache_rd\000cache\000Attributable Level 3 data or unified cache access, read\000event=0xa0\000\00000\000Attributable Level 3 data or unified cache access, read\000 */
{ 61706 }, /* l3d_cache_refill\000cache\000Attributable Level 3 data cache refill\000event=0x2a\000\00000\000Attributable Level 3 data cache refill\000 */
{ 61822 }, /* l3d_cache_refill_prefetch\000cache\000Level 3 cache refill due to prefetch. This event counts any linefills from the hardware prefetcher which cause an allocation into the L3 cache. Note It might not be possible to both distinguish hardware vs software prefetches and also which prefetches cause an allocation. If so, only hardware prefetches should be counted, regardless of whether they allocate. If either the core is configured without a per-core L2 or the cluster is configured without an L3 cache, this event is not implemented\000event=0xc0\000\00000\000Level 3 cache refill due to prefetch. This event counts any linefills from the hardware prefetcher which cause an allocation into the L3 cache. Note It might not be possible to both distinguish hardware vs software prefetches and also which prefetches cause an allocation. If so, only hardware prefetches should be counted, regardless of whether they allocate. If either the core is configured without a per-core L2 or the cluster is configured without an L3 cache, this event is not implemented\000 */
{ 39588 }, /* l3d_cache_refill_rd\000cache\000Attributable Level 3 data or unified cache refill, read\000event=0xa2\000\00000\000Attributable Level 3 data or unified cache refill, read\000 */
{ 64930 }, /* l3d_ws_mode\000cache\000Level 3 cache write streaming mode.This event counts for each cycle where the core is in write-streaming mode and not allocating writes into the L3 cache\000event=0xc7\000\00000\000Level 3 cache write streaming mode.This event counts for each cycle where the core is in write-streaming mode and not allocating writes into the L3 cache\000 */
{ 33712 }, /* ld_retired\000instruction\000Instruction architecturally executed, condition code check pass, load\000event=0x6\000\00000\000Instruction architecturally executed, condition code check pass, load\000 */
{ 21982 }, /* ld_spec\000instruction\000Operation speculatively executed, load\000event=0x70\000\00000\000Operation speculatively executed, load\000 */
{ 22210 }, /* ldst_spec\000instruction\000Operation speculatively executed, load or store\000event=0x72\000\00000\000Operation speculatively executed, load or store\000 */
{ 39009 }, /* ll_cache_miss_rd\000cache\000Last level cache miss, read\000event=0x37\000\00000\000Last level cache miss, read\000 */
{ 38892 }, /* ll_cache_rd\000cache\000Attributable Last level cache memory read\000event=0x36\000\00000\000Attributable Last level cache memory read\000 */
{ 27136 }, /* mem_access\000memory\000Data memory access\000event=0x13\000\00000\000Data memory access\000 */
{ 26460 }, /* mem_access_rd\000memory\000Data memory access, read\000event=0x66\000\00000\000Data memory access, read\000 */
{ 26546 }, /* mem_access_wr\000memory\000Data memory access, write\000event=0x67\000\00000\000Data memory access, write\000 */
{ 33636 }, /* memory_error\000exception\000Local memory error\000event=0x1a\000\00000\000Local memory error\000 */
{ 24486 }, /* pc_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, software change of the PC\000event=0xc\000\00000\000Instruction architecturally executed, condition code check pass, software change of the PC\000 */
{ 22808 }, /* pc_write_spec\000instruction\000Operation speculatively executed, software change of the PC\000event=0x76\000\00000\000Operation speculatively executed, software change of the PC\000 */
{ 68053 }, /* predecode_error\000exception\000Predecode error\000event=0xc6\000\00000\000Predecode error\000 */
{ 46517 }, /* remote_access_rd\000memory\000Attributable memory read access to another socket in a multi-socket system\000event=0x38\000\00000\000Attributable memory read access to another socket in a multi-socket system\000 */
{ 21486 }, /* st_retired\000instruction\000Instruction architecturally executed, condition code check pass, store\000event=0x7\000\00000\000Instruction architecturally executed, condition code check pass, store\000 */
{ 22095 }, /* st_spec\000instruction\000Operation speculatively executed, store\000event=0x71\000\00000\000Operation speculatively executed, store\000 */
{ 27892 }, /* stall_backend\000pipeline\000No operation issued due to the backend\000event=0x24\000\00000\000No operation issued due to the backend\000 */
{ 69197 }, /* stall_backend_ilock\000pipeline\000No operation issued due to the backend interlock.This event counts every cycle that issue is stalled and there is an interlock. Stall cycles due to a stall in Wr (typically awaiting load data) are excluded\000event=0xe4\000\00000\000No operation issued due to the backend interlock.This event counts every cycle that issue is stalled and there is an interlock. Stall cycles due to a stall in Wr (typically awaiting load data) are excluded\000 */
{ 69653 }, /* stall_backend_ilock_agu\000pipeline\000No operation issued due to the backend, interlock, AGU.This event counts every cycle that issue is stalled and there is an interlock that is due to a load/store instruction waiting for data to calculate the address in the AGU. Stall cycles due to a stall in Wr (typically awaiting load data) are excluded\000event=0xe5\000\00000\000No operation issued due to the backend, interlock, AGU.This event counts every cycle that issue is stalled and there is an interlock that is due to a load/store instruction waiting for data to calculate the address in the AGU. Stall cycles due to a stall in Wr (typically awaiting load data) are excluded\000 */
{ 70311 }, /* stall_backend_ilock_fpu\000pipeline\000No operation issued due to the backend, interlock, FPU.This event counts every cycle that issue is stalled and there is an interlock that is due to an FPU/NEON instruction. Stall cycles due to a stall in the Wr stage (typically awaiting load data) are excluded\000event=0xe6\000\00000\000No operation issued due to the backend, interlock, FPU.This event counts every cycle that issue is stalled and there is an interlock that is due to an FPU/NEON instruction. Stall cycles due to a stall in the Wr stage (typically awaiting load data) are excluded\000 */
{ 70881 }, /* stall_backend_ld\000pipeline\000No operation issued due to the backend, load.This event counts every cycle there is a stall in the Wr stage due to a load\000event=0xe7\000\00000\000No operation issued due to the backend, load.This event counts every cycle there is a stall in the Wr stage due to a load\000 */
{ 71455 }, /* stall_backend_ld_cache\000pipeline\000No operation issued due to the backend, load, cache miss.This event counts every cycle there is a stall in the Wr stage due to a load which is waiting on data (due to missing the cache or being non-cacheable)\000event=0xe9\000\00000\000No operation issued due to the backend, load, cache miss.This event counts every cycle there is a stall in the Wr stage due to a load which is waiting on data (due to missing the cache or being non-cacheable)\000 */
{ 71920 }, /* stall_backend_ld_tlb\000pipeline\000No operation issued due to the backend, load, TLB miss.This event counts every cycle there is a stall in the Wr stage due to a load which has missed in the L1 TLB\000event=0xea\000\00000\000No operation issued due to the backend, load, TLB miss.This event counts every cycle there is a stall in the Wr stage due to a load which has missed in the L1 TLB\000 */
{ 71166 }, /* stall_backend_st\000pipeline\000No operation issued due to the backend, store.This event counts every cycle there is a stall in the Wr stage due to a store\000event=0xe8\000\00000\000No operation issued due to the backend, store.This event counts every cycle there is a stall in the Wr stage due to a store\000 */
{ 72291 }, /* stall_backend_st_stb\000pipeline\000No operation issued due to the backend, store, STB full.This event counts every cycle there is a stall in the Wr stage due to a store which is waiting due to the STB being full\000event=0xeb\000\00000\000No operation issued due to the backend, store, STB full.This event counts every cycle there is a stall in the Wr stage due to a store which is waiting due to the STB being full\000 */
{ 72690 }, /* stall_backend_st_tlb\000pipeline\000No operation issued due to the backend, store, TLB miss.This event counts every cycle there is a stall in the Wr stage due to a store which has missed in the L1 TLB\000event=0xec\000\00000\000No operation issued due to the backend, store, TLB miss.This event counts every cycle there is a stall in the Wr stage due to a store which has missed in the L1 TLB\000 */
{ 46706 }, /* stall_frontend\000pipeline\000No operation issued because of the frontend\000event=0x23\000\00000\000No operation issued because of the frontend\000 */
{ 68126 }, /* stall_frontend_cache\000pipeline\000No operation issued due to the frontend, cache miss.This event counts every cycle the DPU IQ is empty and there is an instruction cache miss being processed\000event=0xe1\000\00000\000No operation issued due to the frontend, cache miss.This event counts every cycle the DPU IQ is empty and there is an instruction cache miss being processed\000 */
{ 68840 }, /* stall_frontend_pderr\000pipeline\000No operation issued due to the frontend, pre-decode error.This event counts every cycle the DPU IQ is empty and there is a pre-decode error being processed\000event=0xe3\000\00000\000No operation issued due to the frontend, pre-decode error.This event counts every cycle the DPU IQ is empty and there is a pre-decode error being processed\000 */
{ 68485 }, /* stall_frontend_tlb\000pipeline\000No operation issued due to the frontend, TLB miss.This event counts every cycle the DPU IQ is empty and there is an instruction L1 TLB miss being processed\000event=0xe2\000\00000\000No operation issued due to the frontend, TLB miss.This event counts every cycle the DPU IQ is empty and there is an instruction L1 TLB miss being processed\000 */
{ 21286 }, /* sw_incr\000instruction\000Instruction architecturally executed, Condition code check pass, software increment\000event=0\000\00000\000Instruction architecturally executed, Condition code check pass, software increment\000 */
{ 24814 }, /* ttbr_write_retired\000instruction\000Instruction architecturally executed, Condition code check pass, write to TTBR\000event=0x1c\000\00000\000Instruction architecturally executed, Condition code check pass, write to TTBR\000 */
{ 34080 }, /* unaligned_ldst_retired\000memory\000Instruction architecturally executed, condition code check pass, unaligned\000event=0xf\000\00000\000Instruction architecturally executed, condition code check pass, unaligned\000 */
{ 22650 }, /* vfp_spec\000instruction\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Operation speculatively executed, floating-point instruction\000 */

};

const struct pmu_table_entry pmu_events__arm_cortex_a55[] = {
{
     .entries = pmu_events__arm_cortex_a55_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__arm_cortex_a55_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__arm_cortex_a57_a72_default_core[] = {
{ 22494 }, /* ase_spec\000instruction\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Operation speculatively executed, Advanced SIMD instruction\000 */
{ 13 }, /* br_immed_spec\000branch\000Branch speculatively executed, immediate branch\000event=0x78\000\00000\000Branch speculatively executed, immediate branch\000 */
{ 278 }, /* br_indirect_spec\000branch\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000 */
{ 411 }, /* br_mis_pred\000branch\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000Mispredicted or not predicted branch speculatively executed\000 */
{ 565 }, /* br_pred\000branch\000Predictable branch speculatively executed\000event=0x12\000\00000\000Predictable branch speculatively executed\000 */
{ 145 }, /* br_return_spec\000branch\000Branch speculatively executed, procedure return\000event=0x79\000\00000\000Branch speculatively executed, procedure return\000 */
{ 1311 }, /* bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000Attributable Bus access\000 */
{ 1153 }, /* bus_access_normal\000bus\000Bus access, Normal\000event=0x64\000\00000\000Bus access, Normal\000 */
{ 1022 }, /* bus_access_not_shared\000bus\000Bus access, not Normal, Cacheable, Shareable\000event=0x63\000\00000\000Bus access, not Normal, Cacheable, Shareable\000 */
{ 1228 }, /* bus_access_periph\000bus\000Bus access, peripheral\000event=0x65\000\00000\000Bus access, peripheral\000 */
{ 771 }, /* bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Bus access read\000 */
{ 903 }, /* bus_access_shared\000bus\000Bus access, Normal, Cacheable, Shareable\000event=0x62\000\00000\000Bus access, Normal, Cacheable, Shareable\000 */
{ 836 }, /* bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Bus access write\000 */
{ 721 }, /* bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000Bus cycle\000 */
{ 24272 }, /* cid_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000 */
{ 679 }, /* cpu_cycles\000bus\000Cycle\000event=0x11\000\00000\000Cycle\000 */
{ 23330 }, /* crypto_spec\000instruction\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Operation speculatively executed, Cryptographic instruction\000 */
{ 23705 }, /* dmb_spec\000instruction\000Barrier speculatively executed, DMB\000event=0x7e\000\00000\000Barrier speculatively executed, DMB\000 */
{ 22343 }, /* dp_spec\000instruction\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Operation speculatively executed, integer data processing\000 */
{ 23597 }, /* dsb_spec\000instruction\000Barrier speculatively executed, DSB\000event=0x7d\000\00000\000Barrier speculatively executed, DSB\000 */
{ 20002 }, /* exc_dabort\000exception\000Exception taken, Data Abort and SError\000event=0x84\000\00000\000Exception taken, Data Abort and SError\000 */
{ 20191 }, /* exc_fiq\000exception\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000 */
{ 20266 }, /* exc_hvc\000exception\000Exception taken, Hypervisor Call\000event=0x8a\000\00000\000Exception taken, Hypervisor Call\000 */
{ 20116 }, /* exc_irq\000exception\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000 */
{ 19896 }, /* exc_pabort\000exception\000Exception taken, Instruction Abort\000event=0x83\000\00000\000Exception taken, Instruction Abort\000 */
{ 33889 }, /* exc_return\000instruction\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Instruction architecturally executed, condition check pass, exception return\000 */
{ 73325 }, /* exc_smc\000exception\000Exception taken, Secure Monitor Call\000event=0x88\000\00000\000Exception taken, Secure Monitor Call\000 */
{ 19797 }, /* exc_svc\000exception\000Exception taken, Supervisor Call\000event=0x82\000\00000\000Exception taken, Supervisor Call\000 */
{ 21031 }, /* exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Exception taken\000 */
{ 20512 }, /* exc_trap_dabort\000exception\000Exception taken, Data Abort or SError not taken locally\000event=0x8c\000\00000\000Exception taken, Data Abort or SError not taken locally\000 */
{ 20915 }, /* exc_trap_fiq\000exception\000Exception taken, FIQ not taken locally\000event=0x8f\000\00000\000Exception taken, FIQ not taken locally\000 */
{ 20799 }, /* exc_trap_irq\000exception\000Exception taken, IRQ not taken locally\000event=0x8e\000\00000\000Exception taken, IRQ not taken locally\000 */
{ 20665 }, /* exc_trap_other\000exception\000Exception taken, Other traps not taken locally\000event=0x8d\000\00000\000Exception taken, Other traps not taken locally\000 */
{ 20365 }, /* exc_trap_pabort\000exception\000Exception taken, Instruction Abort not taken locally\000event=0x8b\000\00000\000Exception taken, Instruction Abort not taken locally\000 */
{ 19692 }, /* exc_undef\000exception\000Exception taken, Other synchronous\000event=0x81\000\00000\000Exception taken, Other synchronous\000 */
{ 24159 }, /* inst_retired\000instruction\000Instruction architecturally executed\000event=0x8\000\00000\000Instruction architecturally executed\000 */
{ 24711 }, /* inst_spec\000instruction\000Operation speculatively executed\000event=0x1b\000\00000\000Operation speculatively executed\000 */
{ 23489 }, /* isb_spec\000instruction\000Barrier speculatively executed, ISB\000event=0x7c\000\00000\000Barrier speculatively executed, ISB\000 */
{ 3313 }, /* l1d_cache\000cache\000Level 1 data cache access\000event=0x4\000\00000\000Level 1 data cache access\000 */
{ 1902 }, /* l1d_cache_inval\000cache\000L1D cache invalidate\000event=0x48\000\00000\000Attributable Level 1 data cache invalidate\000 */
{ 1587 }, /* l1d_cache_rd\000cache\000L1D cache access, read\000event=0x40\000\00000\000Attributable Level 1 data cache access, read\000 */
{ 3224 }, /* l1d_cache_refill\000cache\000Level 1 data cache refill\000event=0x3\000\00000\000Level 1 data cache refill\000 */
{ 1793 }, /* l1d_cache_refill_rd\000cache\000L1D cache refill, read\000event=0x42\000\00000\000Attributable Level 1 data cache refill, read\000 */
{ 39103 }, /* l1d_cache_refill_wr\000cache\000L1D cache refill, write\000event=0x43\000\00000\000Attributable Level 1 data cache refill, write\000 */
{ 33516 }, /* l1d_cache_wb\000cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000Attributable Level 1 data cache write-back\000 */
{ 73186 }, /* l1d_cache_wb_clean\000cache\000L1D cache Write-Back, cleaning and coherency\000event=0x47\000\00000\000Level 1 data cache Write-Back, cleaning and coherency\000 */
{ 73065 }, /* l1d_cache_wb_victim\000cache\000L1D cache Write-Back, victim\000event=0x46\000\00000\000Attributable Level 1 data cache Write-Back, victim\000 */
{ 1689 }, /* l1d_cache_wr\000cache\000L1D cache access, write\000event=0x41\000\00000\000Attributable Level 1 data cache access, write\000 */
{ 3395 }, /* l1d_tlb_refill\000cache\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Attributable Level 1 data TLB refill\000 */
{ 2003 }, /* l1d_tlb_refill_rd\000cache\000L1D tlb refill, read\000event=0x4c\000\00000\000Attributable Level 1 data TLB refill, read\000 */
{ 2106 }, /* l1d_tlb_refill_wr\000cache\000L1D tlb refill, write\000event=0x4d\000\00000\000Attributable Level 1 data TLB refill, write\000 */
{ 3504 }, /* l1i_cache\000cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Attributable Level 1 instruction cache access\000 */
{ 2998 }, /* l1i_cache_refill\000cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Level 1 instruction cache refill\000 */
{ 3101 }, /* l1i_tlb_refill\000cache\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Attributable Level 1 instruction TLB refill\000 */
{ 3627 }, /* l2d_cache\000cache\000Level 2 data cache access\000event=0x16\000\00000\000Level 2 data cache access\000 */
{ 2897 }, /* l2d_cache_inval\000cache\000L2D cache invalidate\000event=0x58\000\00000\000Attributable Level 2 data cache invalidate\000 */
{ 2211 }, /* l2d_cache_rd\000cache\000L2D cache access, read\000event=0x50\000\00000\000Attributable Level 2 data cache access, read\000 */
{ 3710 }, /* l2d_cache_refill\000cache\000Level 2 data refill\000event=0x17\000\00000\000Level 2 data refill\000 */
{ 2417 }, /* l2d_cache_refill_rd\000cache\000L2D cache refill, read\000event=0x52\000\00000\000Attributable Level 2 data cache refill, read\000 */
{ 2526 }, /* l2d_cache_refill_wr\000cache\000L2D cache refill, write\000event=0x53\000\00000\000Attributable Level 2 data cache refill, write\000 */
{ 3788 }, /* l2d_cache_wb\000cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Attributable Level 2 data cache write-back\000 */
{ 2758 }, /* l2d_cache_wb_clean\000cache\000L2D cache Write-Back, cleaning and coherency\000event=0x57\000\00000\000Level 2 data cache Write-Back, cleaning and coherency\000 */
{ 2637 }, /* l2d_cache_wb_victim\000cache\000L2D cache Write-Back, victim\000event=0x56\000\00000\000Attributable Level 2 data cache Write-Back, victim\000 */
{ 2313 }, /* l2d_cache_wr\000cache\000L2D cache access, write\000event=0x51\000\00000\000Attributable Level 2 data cache access, write\000 */
{ 21982 }, /* ld_spec\000instruction\000Operation speculatively executed, load\000event=0x70\000\00000\000Operation speculatively executed, load\000 */
{ 73432 }, /* ldrex_spec\000instruction\000Exclusive operation speculatively executed, LDREX or LDX\000event=0x6c\000\00000\000Exclusive operation speculatively executed, LDREX or LDX\000 */
{ 22210 }, /* ldst_spec\000instruction\000Operation speculatively executed, load or store\000event=0x72\000\00000\000Operation speculatively executed, load or store\000 */
{ 27136 }, /* mem_access\000memory\000Data memory access\000event=0x13\000\00000\000Data memory access\000 */
{ 26460 }, /* mem_access_rd\000memory\000Data memory access, read\000event=0x66\000\00000\000Data memory access, read\000 */
{ 26546 }, /* mem_access_wr\000memory\000Data memory access, write\000event=0x67\000\00000\000Data memory access, write\000 */
{ 33636 }, /* memory_error\000exception\000Local memory error\000event=0x1a\000\00000\000Local memory error\000 */
{ 22808 }, /* pc_write_spec\000instruction\000Operation speculatively executed, software change of the PC\000event=0x76\000\00000\000Operation speculatively executed, software change of the PC\000 */
{ 23813 }, /* rc_ld_spec\000instruction\000Release consistency operation speculatively executed, Load-Acquire\000event=0x90\000\00000\000Release consistency operation speculatively executed, Load-Acquire\000 */
{ 23985 }, /* rc_st_spec\000instruction\000Release consistency operation speculatively executed, Store-Release\000event=0x91\000\00000\000Release consistency operation speculatively executed, Store-Release\000 */
{ 22095 }, /* st_spec\000instruction\000Operation speculatively executed, store\000event=0x71\000\00000\000Operation speculatively executed, store\000 */
{ 73751 }, /* strex_fail_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX fail\000event=0x6e\000\00000\000Exclusive operation speculatively executed, STREX or STX fail\000 */
{ 73584 }, /* strex_pass_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX pass\000event=0x6d\000\00000\000Exclusive operation speculatively executed, STREX or STX pass\000 */
{ 21286 }, /* sw_incr\000instruction\000Instruction architecturally executed, Condition code check pass, software increment\000event=0\000\00000\000Instruction architecturally executed, Condition code check pass, software increment\000 */
{ 24814 }, /* ttbr_write_retired\000instruction\000Instruction architecturally executed, Condition code check pass, write to TTBR\000event=0x1c\000\00000\000Instruction architecturally executed, Condition code check pass, write to TTBR\000 */
{ 26634 }, /* unaligned_ld_spec\000memory\000Unaligned access, read\000event=0x68\000\00000\000Unaligned access, read\000 */
{ 26808 }, /* unaligned_ldst_spec\000memory\000Unaligned access\000event=0x6a\000\00000\000Unaligned access\000 */
{ 26720 }, /* unaligned_st_spec\000memory\000Unaligned access, write\000event=0x69\000\00000\000Unaligned access, write\000 */
{ 22650 }, /* vfp_spec\000instruction\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Operation speculatively executed, floating-point instruction\000 */

};

const struct pmu_table_entry pmu_events__arm_cortex_a57_a72[] = {
{
     .entries = pmu_events__arm_cortex_a57_a72_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__arm_cortex_a57_a72_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__arm_cortex_a65_e1_default_core[] = {
{ 22494 }, /* ase_spec\000instruction\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Operation speculatively executed, Advanced SIMD instruction\000 */
{ 87886 }, /* biu_ext_mem_req\000memory\000External memory request\000event=0xc1\000\00000\000External memory request\000 */
{ 87972 }, /* biu_ext_mem_req_nc\000memory\000External memory request to non-cacheable memory\000event=0xc2\000\00000\000External memory request to non-cacheable memory\000 */
{ 22969 }, /* br_immed_retired\000instruction\000Instruction architecturally executed, immediate branch\000event=0xd\000\00000\000Instruction architecturally executed, immediate branch\000 */
{ 13 }, /* br_immed_spec\000branch\000Branch speculatively executed, immediate branch\000event=0x78\000\00000\000Branch speculatively executed, immediate branch\000 */
{ 278 }, /* br_indirect_spec\000branch\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000 */
{ 411 }, /* br_mis_pred\000branch\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000Mispredicted or not predicted branch speculatively executed\000 */
{ 25146 }, /* br_mis_pred_retired\000instruction\000Instruction architecturally executed, mispredicted branch\000event=0x22\000\00000\000Instruction architecturally executed, mispredicted branch\000 */
{ 565 }, /* br_pred\000branch\000Predictable branch speculatively executed\000event=0x12\000\00000\000Predictable branch speculatively executed\000 */
{ 25018 }, /* br_retired\000instruction\000Instruction architecturally executed, branch\000event=0x21\000\00000\000Instruction architecturally executed, branch\000 */
{ 23122 }, /* br_return_retired\000instruction\000Instruction architecturally executed, condition code check pass, procedure return\000event=0xe\000\00000\000Instruction architecturally executed, condition code check pass, procedure return\000 */
{ 145 }, /* br_return_spec\000branch\000Branch speculatively executed, procedure return\000event=0x79\000\00000\000Branch speculatively executed, procedure return\000 */
{ 1311 }, /* bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000Attributable Bus access\000 */
{ 771 }, /* bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Bus access read\000 */
{ 836 }, /* bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Bus access write\000 */
{ 721 }, /* bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000Bus cycle\000 */
{ 24272 }, /* cid_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000 */
{ 679 }, /* cpu_cycles\000bus\000Cycle\000event=0x11\000\00000\000Cycle\000 */
{ 23330 }, /* crypto_spec\000instruction\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Operation speculatively executed, Cryptographic instruction\000 */
{ 22343 }, /* dp_spec\000instruction\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Operation speculatively executed, integer data processing\000 */
{ 84013 }, /* dpu_br_cond_mis\000dpu\000Instruction retired, conditional branch, mispredicted\000event=0xea\000\00000\000Instruction retired, conditional branch, mispredicted\000 */
{ 87759 }, /* dpu_br_cond_retired\000instruction\000Instruction retired, conditional branch\000event=0xe8\000\00000\000Instruction retired, conditional branch\000 */
{ 83877 }, /* dpu_br_ind_mis\000dpu\000Instruction retired, indirect branch, mispredicted\000event=0xe9\000\00000\000Instruction retired, indirect branch, mispredicted\000 */
{ 84257 }, /* dpu_mem_err_dcu\000dpu\000Memory error (any type) from DCU\000event=0xec\000\00000\000Memory error (any type) from DCU\000 */
{ 84156 }, /* dpu_mem_err_ifu\000dpu\000Memory error (any type) from IFU\000event=0xeb\000\00000\000Memory error (any type) from IFU\000 */
{ 84358 }, /* dpu_mem_err_tlb\000dpu\000Memory error (any type) from TLB\000event=0xed\000\00000\000Memory error (any type) from TLB\000 */
{ 4626 }, /* dtlb_walk\000cache\000Access to data TLB causes a translation table walk\000event=0x34\000\00000\000Access to data TLB causes a translation table walk\000 */
{ 20191 }, /* exc_fiq\000exception\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000 */
{ 20116 }, /* exc_irq\000exception\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000 */
{ 33889 }, /* exc_return\000instruction\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Instruction architecturally executed, condition check pass, exception return\000 */
{ 21031 }, /* exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Exception taken\000 */
{ 86054 }, /* ifu_all_thrds_rdy\000ifu\000This thread and the other thread both ready for scheduling in if0\000event=0xdc\000\00000\000This thread and the other thread both ready for scheduling in if0\000 */
{ 85909 }, /* ifu_flushed_excl_tlb_miss\000ifu\000Thread flushed due to reasons other than TLB miss\000event=0xdb\000\00000\000Thread flushed due to reasons other than TLB miss\000 */
{ 85807 }, /* ifu_flushed_tlb_miss\000ifu\000Thread flushed due to TLB miss\000event=0xda\000\00000\000Thread flushed due to TLB miss\000 */
{ 87002 }, /* ifu_goto_idle\000ifu\000The thread moved from an active state to an inactive state (long-term sleep state, causing deallocation of some resources)\000event=0xe0\000\00000\000The thread moved from an active state to an inactive state (long-term sleep state, causing deallocation of some resources)\000 */
{ 87667 }, /* ifu_ic_lf_sp\000ifu\000I-Cache Speculative line fill\000event=0xe4\000\00000\000I-Cache Speculative line fill\000 */
{ 87281 }, /* ifu_ic_lookup_under_miss\000ifu\000I-Cache lookup under miss from other thread\000event=0xe1\000\00000\000I-Cache lookup under miss from other thread\000 */
{ 87413 }, /* ifu_ic_miss_under_miss\000ifu\000I-Cache miss under miss from other thread\000event=0xe2\000\00000\000I-Cache miss under miss from other thread\000 */
{ 84459 }, /* ifu_ic_miss_wait\000ifu\000I-Cache miss on an access from the prefetch block\000event=0xd0\000\00000\000I-Cache miss on an access from the prefetch block\000 */
{ 87539 }, /* ifu_instr_pushed\000ifu\000This thread pushed an instruction into the IQ\000event=0xe3\000\00000\000This thread pushed an instruction into the IQ\000 */
{ 84595 }, /* ifu_iutlb_miss_wait\000ifu\000Counts the cycles spent on a request for Level 2 TLB lookup after a Level 1l ITLB miss\000event=0xd1\000\00000\000Counts the cycles spent on a request for Level 2 TLB lookup after a Level 1l ITLB miss\000 */
{ 85020 }, /* ifu_micro_caddr_mispred\000ifu\000Micro-predictor address mispredict, with respect to if3/if4 predictor\000event=0xd3\000\00000\000Micro-predictor address mispredict, with respect to if3/if4 predictor\000 */
{ 84808 }, /* ifu_micro_cond_mispred\000ifu\000Micro-predictor conditional/direction mispredict, with respect to. if3/if4 predictor\000event=0xd2\000\00000\000Micro-predictor conditional/direction mispredict, with respect to. if3/if4 predictor\000 */
{ 85431 }, /* ifu_micro_correction\000ifu\000Micro-predictor correction\000event=0xd7\000\00000\000Micro-predictor correction\000 */
{ 85203 }, /* ifu_micro_hit\000ifu\000Micro-predictor hit with immediate redirect\000event=0xd4\000\00000\000Micro-predictor hit with immediate redirect\000 */
{ 85324 }, /* ifu_micro_neg_hit\000ifu\000Micro-predictor negative cache hit\000event=0xd6\000\00000\000Micro-predictor negative cache hit\000 */
{ 85525 }, /* ifu_micro_no_instr1\000ifu\000A 2nd instruction could have been pushed but was not because it was nonsequential\000event=0xd8\000\00000\000A 2nd instruction could have been pushed but was not because it was nonsequential\000 */
{ 85728 }, /* ifu_micro_no_pred\000ifu\000Micro-predictor miss\000event=0xd9\000\00000\000Micro-predictor miss\000 */
{ 86729 }, /* ifu_not_rdy_for_arb\000ifu\000This thread was not arbitrated because it was not ready for scheduling. For example, due to a cache miss or TLB miss\000event=0xdf\000\00000\000This thread was not arbitrated because it was not ready for scheduling. For example, due to a cache miss or TLB miss\000 */
{ 86422 }, /* ifu_win_arb_other_act\000ifu\000This thread was arbitrated when the other thread was also active, but not necessarily ready. For example, waiting for I-Cache or TLB\000event=0xde\000\00000\000This thread was arbitrated when the other thread was also active, but not necessarily ready. For example, waiting for I-Cache or TLB\000 */
{ 86223 }, /* ifu_win_arb_other_rdy\000ifu\000This thread was arbitrated when the other thread was also ready for scheduling\000event=0xdd\000\00000\000This thread was arbitrated when the other thread was also ready for scheduling\000 */
{ 24159 }, /* inst_retired\000instruction\000Instruction architecturally executed\000event=0x8\000\00000\000Instruction architecturally executed\000 */
{ 24711 }, /* inst_spec\000instruction\000Operation speculatively executed\000event=0x1b\000\00000\000Operation speculatively executed\000 */
{ 23489 }, /* isb_spec\000instruction\000Barrier speculatively executed, ISB\000event=0x7c\000\00000\000Barrier speculatively executed, ISB\000 */
{ 4759 }, /* itlb_walk\000cache\000Access to instruction TLB that causes a translation table walk\000event=0x35\000\00000\000Access to instruction TLB that causes a translation table walk\000 */
{ 3313 }, /* l1d_cache\000cache\000Level 1 data cache access\000event=0x4\000\00000\000Level 1 data cache access\000 */
{ 1587 }, /* l1d_cache_rd\000cache\000L1D cache access, read\000event=0x40\000\00000\000Attributable Level 1 data cache access, read\000 */
{ 3224 }, /* l1d_cache_refill\000cache\000Level 1 data cache refill\000event=0x3\000\00000\000Level 1 data cache refill\000 */
{ 39214 }, /* l1d_cache_refill_inner\000cache\000L1D cache refill, inner\000event=0x44\000\00000\000Attributable Level 1 data cache refill, inner\000 */
{ 39328 }, /* l1d_cache_refill_outer\000cache\000L1D cache refill, outer\000event=0x45\000\00000\000Attributable Level 1 data cache refill, outer\000 */
{ 1793 }, /* l1d_cache_refill_rd\000cache\000L1D cache refill, read\000event=0x42\000\00000\000Attributable Level 1 data cache refill, read\000 */
{ 39103 }, /* l1d_cache_refill_wr\000cache\000L1D cache refill, write\000event=0x43\000\00000\000Attributable Level 1 data cache refill, write\000 */
{ 33516 }, /* l1d_cache_wb\000cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000Attributable Level 1 data cache write-back\000 */
{ 1689 }, /* l1d_cache_wr\000cache\000L1D cache access, write\000event=0x41\000\00000\000Attributable Level 1 data cache access, write\000 */
{ 74001 }, /* l1d_pref_line_fill\000cache\000Level 1 data cache refill started due to prefetch. Counts any linefills from the prefetcher which cause an allocation into the L1 D-cache\000event=0xc3\000\00000\000Level 1 data cache refill started due to prefetch. Counts any linefills from the prefetcher which cause an allocation into the L1 D-cache\000 */
{ 3908 }, /* l1d_tlb\000cache\000Attributable Level 1 data or unified TLB access\000event=0x25\000\00000\000Attributable Level 1 data or unified TLB access\000 */
{ 3395 }, /* l1d_tlb_refill\000cache\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Attributable Level 1 data TLB refill\000 */
{ 76231 }, /* l1d_ws_mode\000cache\000L1D is in write stream mode\000event=0xc7\000\00000\000L1D is in write stream mode\000 */
{ 76130 }, /* l1d_ws_mode_enter\000cache\000L1D entering write stream mode\000event=0xc6\000\00000\000L1D entering write stream mode\000 */
{ 3504 }, /* l1i_cache\000cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Attributable Level 1 instruction cache access\000 */
{ 2998 }, /* l1i_cache_refill\000cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Level 1 instruction cache refill\000 */
{ 4033 }, /* l1i_tlb\000cache\000Attributable Level 1 instruction TLB access\000event=0x26\000\00000\000Attributable Level 1 instruction TLB access\000 */
{ 3101 }, /* l1i_tlb_refill\000cache\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Attributable Level 1 instruction TLB refill\000 */
{ 79398 }, /* l2_l1d_cache_wb_unatt\000cache\000Unattributable Level 1 data cache write-back. This event occurs when a requestor outside the PE makes a coherency request that results in writeback\000event=0xf0\000\00000\000Unattributable Level 1 data cache write-back. This event occurs when a requestor outside the PE makes a coherency request that results in writeback\000 */
{ 80097 }, /* l2_l2d_cache_rd_unatt\000cache\000Unattributable Level 2 data cache access, read. This event occurs when a requestor outside the PE makes a coherency request that results in level 2 data cache read access\000event=0xf2\000\00000\000Unattributable Level 2 data cache access, read. This event occurs when a requestor outside the PE makes a coherency request that results in level 2 data cache read access\000 */
{ 79737 }, /* l2_l2d_cache_unatt\000cache\000Unattributable Level 2 data cache access. This event occurs when a requestor outside the PE makes a coherency request that results in level 2 data cache access\000event=0xf1\000\00000\000Unattributable Level 2 data cache access. This event occurs when a requestor outside the PE makes a coherency request that results in level 2 data cache access\000 */
{ 81237 }, /* l2_l3d_cache_alloc_unatt\000cache\000Unattributable Level 3 data or unified cache allocation without refill. This event occurs when a requestor outside the PE makes a coherency request that results in level 3 cache allocate without refill\000event=0xf5\000\00000\000Unattributable Level 3 data or unified cache allocation without refill. This event occurs when a requestor outside the PE makes a coherency request that results in level 3 cache allocate without refill\000 */
{ 80852 }, /* l2_l3d_cache_rd_unatt\000cache\000Unattributable Level 3 data cache access, read. This event occurs when a requestor outside the PE makes a coherency request that results in level 3 data cache read access\000event=0xf4\000\00000\000Unattributable Level 3 data cache access, read. This event occurs when a requestor outside the PE makes a coherency request that results in level 3 data cache read access\000 */
{ 81687 }, /* l2_l3d_cache_refill_unatt\000cache\000Unattributable Level 3 data or unified cache refill. This event occurs when a requestor outside the PE makes a coherency request that results in level 3 cache refill\000event=0xf6\000\00000\000Unattributable Level 3 data or unified cache refill. This event occurs when a requestor outside the PE makes a coherency request that results in level 3 cache refill\000 */
{ 80482 }, /* l2_l3d_cache_unatt\000cache\000Unattributable Level 3 data cache access. This event occurs when a requestor outside the PE makes a coherency request that results in level 3 data cache read access\000event=0xf3\000\00000\000Unattributable Level 3 data cache access. This event occurs when a requestor outside the PE makes a coherency request that results in level 3 data cache read access\000 */
{ 3627 }, /* l2d_cache\000cache\000Level 2 data cache access\000event=0x16\000\00000\000Level 2 data cache access\000 */
{ 38627 }, /* l2d_cache_allocate\000cache\000Attributable Level 2 data cache allocation without refill\000event=0x20\000\00000\000Attributable Level 2 data cache allocation without refill\000 */
{ 2211 }, /* l2d_cache_rd\000cache\000L2D cache access, read\000event=0x50\000\00000\000Attributable Level 2 data cache access, read\000 */
{ 3710 }, /* l2d_cache_refill\000cache\000Level 2 data refill\000event=0x17\000\00000\000Level 2 data refill\000 */
{ 2417 }, /* l2d_cache_refill_rd\000cache\000L2D cache refill, read\000event=0x52\000\00000\000Attributable Level 2 data cache refill, read\000 */
{ 2526 }, /* l2d_cache_refill_wr\000cache\000L2D cache refill, write\000event=0x53\000\00000\000Attributable Level 2 data cache refill, write\000 */
{ 82066 }, /* l2d_cache_stash_dropped\000cache\000Level 2 cache stash dropped. This event counts on each stash request received from the interconnect or ACP, that is targeting L2 and gets dropped due to lack of buffer space to hold the request. L2 and L3 cache events (L2D_CACHE*, L3D_CACHE*) The behavior of these events depends on the configuration of the core. If the private L2 cache is present, the L2D_CACHE* events count the activity in the private L2 cache, and the L3D_CACHE* events count the activity in the DSU L3 cache (if present). If the private L2 cache is not present but the DSU L3 cache is present, the L2D_CACHE* events count activity in the DSU L3 cache and the L3D_CACHE* events do not count. The L2D_CACHE_WB, L2D_CACHE_WR and L2D_CACHE_REFILL_WR events do not count in this configuration. If neither the private L2 cache nor the DSU L3 cache are present, neither the L2D_CACHE* or L3D_CACHE* events will count\000event=0xf7\000\00000\000Level 2 cache stash dropped. This event counts on each stash request received from the interconnect or ACP, that is targeting L2 and gets dropped due to lack of buffer space to hold the request. L2 and L3 cache events (L2D_CACHE*, L3D_CACHE*) The behavior of these events depends on the configuration of the core. If the private L2 cache is present, the L2D_CACHE* events count the activity in the private L2 cache, and the L3D_CACHE* events count the activity in the DSU L3 cache (if present). If the private L2 cache is not present but the DSU L3 cache is present, the L2D_CACHE* events count activity in the DSU L3 cache and the L3D_CACHE* events do not count. The L2D_CACHE_WB, L2D_CACHE_WR and L2D_CACHE_REFILL_WR events do not count in this configuration. If neither the private L2 cache nor the DSU L3 cache are present, neither the L2D_CACHE* or L3D_CACHE* events will count\000 */
{ 3788 }, /* l2d_cache_wb\000cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Attributable Level 2 data cache write-back\000 */
{ 2313 }, /* l2d_cache_wr\000cache\000L2D cache access, write\000event=0x51\000\00000\000Attributable Level 2 data cache access, write\000 */
{ 74317 }, /* l2d_pref_line_fill\000cache\000Level 2 cache refill due to prefetch. +//0 If the core is configured with a per-core L2 cache: This event does not count. +//0 If the core is configured without a per-core L2 cache: This event counts the cluster cache event, as defined by L3_PREF_LINE_FILL. +//0 If there is neither a per-core cache nor a cluster cache configured, this event is not implemented\000event=0xc4\000\00000\000Level 2 cache refill due to prefetch. +//0 If the core is configured with a per-core L2 cache: This event does not count. +//0 If the core is configured without a per-core L2 cache: This event counts the cluster cache event, as defined by L3_PREF_LINE_FILL. +//0 If there is neither a per-core cache nor a cluster cache configured, this event is not implemented\000 */
{ 4384 }, /* l2d_tlb\000cache\000Attributable Level 2 data or unified TLB access\000event=0x2f\000\00000\000Attributable Level 2 data or unified TLB access\000 */
{ 4150 }, /* l2d_tlb_refill\000cache\000Attributable Level 2 data TLB refill\000event=0x2d\000\00000\000Attributable Level 2 data TLB refill\000 */
{ 76320 }, /* l2d_ws_mode\000cache\000Level 2 cache write streaming mode. This event counts for each cycle where the core is in write-streaming mode and not allocating writes into the L2 cache\000event=0xc8\000\00000\000Level 2 cache write streaming mode. This event counts for each cycle where the core is in write-streaming mode and not allocating writes into the L2 cache\000 */
{ 75081 }, /* l3_pref_line_fill\000cache\000Level 3 cache refill due to prefetch. This event counts any linefills from the hardware prefetcher which cause an allocation into the L3 cache. Note It might not be possible to distinguish between both hardware and software prefetches and also which prefetches cause an allocation. If so, only hardware prefetches should be counted, regardless of whether they allocate. If either the core is configured without a per-core L2 or the cluster is configured without an L3 cache, this event is not implemented\000event=0xc5\000\00000\000Level 3 cache refill due to prefetch. This event counts any linefills from the hardware prefetcher which cause an allocation into the L3 cache. Note It might not be possible to distinguish between both hardware and software prefetches and also which prefetches cause an allocation. If so, only hardware prefetches should be counted, regardless of whether they allocate. If either the core is configured without a per-core L2 or the cluster is configured without an L3 cache, this event is not implemented\000 */
{ 38783 }, /* l3d_cache\000cache\000Attributable Level 3 data cache access\000event=0x2b\000\00000\000Attributable Level 3 data cache access\000 */
{ 61550 }, /* l3d_cache_allocate\000cache\000Attributable Level 3 data cache allocation without refill\000event=0x29\000\00000\000Attributable Level 3 data cache allocation without refill\000 */
{ 39442 }, /* l3d_cache_rd\000cache\000Attributable Level 3 data or unified cache access, read\000event=0xa0\000\00000\000Attributable Level 3 data or unified cache access, read\000 */
{ 61706 }, /* l3d_cache_refill\000cache\000Attributable Level 3 data cache refill\000event=0x2a\000\00000\000Attributable Level 3 data cache refill\000 */
{ 39588 }, /* l3d_cache_refill_rd\000cache\000Attributable Level 3 data or unified cache refill, read\000event=0xa2\000\00000\000Attributable Level 3 data or unified cache refill, read\000 */
{ 76663 }, /* l3d_ws_mode\000cache\000Level 3 cache write streaming mode. This event counts for each cycle where the core is in write-streaming mode and not allocating writes into the L3 cache\000event=0xc9\000\00000\000Level 3 cache write streaming mode. This event counts for each cycle where the core is in write-streaming mode and not allocating writes into the L3 cache\000 */
{ 33712 }, /* ld_retired\000instruction\000Instruction architecturally executed, condition code check pass, load\000event=0x6\000\00000\000Instruction architecturally executed, condition code check pass, load\000 */
{ 21982 }, /* ld_spec\000instruction\000Operation speculatively executed, load\000event=0x70\000\00000\000Operation speculatively executed, load\000 */
{ 22210 }, /* ldst_spec\000instruction\000Operation speculatively executed, load or store\000event=0x72\000\00000\000Operation speculatively executed, load or store\000 */
{ 39009 }, /* ll_cache_miss_rd\000cache\000Last level cache miss, read\000event=0x37\000\00000\000Last level cache miss, read\000 */
{ 38892 }, /* ll_cache_rd\000cache\000Attributable Last level cache memory read\000event=0x36\000\00000\000Attributable Last level cache memory read\000 */
{ 27136 }, /* mem_access\000memory\000Data memory access\000event=0x13\000\00000\000Data memory access\000 */
{ 26460 }, /* mem_access_rd\000memory\000Data memory access, read\000event=0x66\000\00000\000Data memory access, read\000 */
{ 26546 }, /* mem_access_wr\000memory\000Data memory access, write\000event=0x67\000\00000\000Data memory access, write\000 */
{ 33636 }, /* memory_error\000exception\000Local memory error\000event=0x1a\000\00000\000Local memory error\000 */
{ 24486 }, /* pc_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, software change of the PC\000event=0xc\000\00000\000Instruction architecturally executed, condition code check pass, software change of the PC\000 */
{ 46517 }, /* remote_access_rd\000memory\000Attributable memory read access to another socket in a multi-socket system\000event=0x38\000\00000\000Attributable memory read access to another socket in a multi-socket system\000 */
{ 21486 }, /* st_retired\000instruction\000Instruction architecturally executed, condition code check pass, store\000event=0x7\000\00000\000Instruction architecturally executed, condition code check pass, store\000 */
{ 22095 }, /* st_spec\000instruction\000Operation speculatively executed, store\000event=0x71\000\00000\000Operation speculatively executed, store\000 */
{ 27892 }, /* stall_backend\000pipeline\000No operation issued due to the backend\000event=0x24\000\00000\000No operation issued due to the backend\000 */
{ 46706 }, /* stall_frontend\000pipeline\000No operation issued because of the frontend\000event=0x23\000\00000\000No operation issued because of the frontend\000 */
{ 73918 }, /* stb_stall\000cache\000Merge in the store buffer\000event=0xc0\000\00000\000Merge in the store buffer\000 */
{ 21286 }, /* sw_incr\000instruction\000Instruction architecturally executed, Condition code check pass, software increment\000event=0\000\00000\000Instruction architecturally executed, Condition code check pass, software increment\000 */
{ 77460 }, /* tlb_l2tlb_l2walk_access\000cache\000Level 2 TLB level-2 walk cache access. This event counts accesses to the level-2 walk cache where the last-level walk cache has missed. The event only counts when the translation regime of the pagewalk uses level 2 descriptors. This event does not count if the MMU is disabled\000event=0xcc\000\00000\000Level 2 TLB level-2 walk cache access. This event counts accesses to the level-2 walk cache where the last-level walk cache has missed. The event only counts when the translation regime of the pagewalk uses level 2 descriptors. This event does not count if the MMU is disabled\000 */
{ 78059 }, /* tlb_l2tlb_l2walk_refill\000cache\000Level 2 TLB level-2 walk cache refill. This event does not count if the MMU is disabled\000event=0xcd\000\00000\000Level 2 TLB level-2 walk cache refill. This event does not count if the MMU is disabled\000 */
{ 77006 }, /* tlb_l2tlb_llwalk_access\000cache\000Level 2 TLB last-level walk cache access. This event does not count if the MMU is disabled\000event=0xca\000\00000\000Level 2 TLB last-level walk cache access. This event does not count if the MMU is disabled\000 */
{ 77233 }, /* tlb_l2tlb_llwalk_refill\000cache\000Level 2 TLB last-level walk cache refill. This event does not count if the MMU is disabled\000event=0xcb\000\00000\000Level 2 TLB last-level walk cache refill. This event does not count if the MMU is disabled\000 */
{ 78280 }, /* tlb_l2tlb_s2_access\000cache\000Level 2 TLB IPA cache access. This event counts on each access to the IPA cache. +//0 If a single pagewalk needs to make multiple accesses to the IPA cache, each access is counted. +//0 If stage 2 translation is disabled, this event does not count\000event=0xce\000\00000\000Level 2 TLB IPA cache access. This event counts on each access to the IPA cache. +//0 If a single pagewalk needs to make multiple accesses to the IPA cache, each access is counted. +//0 If stage 2 translation is disabled, this event does not count\000 */
{ 78817 }, /* tlb_l2tlb_s2_refill\000cache\000Level 2 TLB IPA cache refill. This event counts on each refill of the IPA cache. +//0 If a single pagewalk needs to make multiple accesses to the IPA cache, each access which causes a refill is counted. +//0 If stage 2 translation is disabled, this event does not count\000event=0xcf\000\00000\000Level 2 TLB IPA cache refill. This event counts on each refill of the IPA cache. +//0 If a single pagewalk needs to make multiple accesses to the IPA cache, each access which causes a refill is counted. +//0 If stage 2 translation is disabled, this event does not count\000 */
{ 24814 }, /* ttbr_write_retired\000instruction\000Instruction architecturally executed, Condition code check pass, write to TTBR\000event=0x1c\000\00000\000Instruction architecturally executed, Condition code check pass, write to TTBR\000 */
{ 26634 }, /* unaligned_ld_spec\000memory\000Unaligned access, read\000event=0x68\000\00000\000Unaligned access, read\000 */
{ 26808 }, /* unaligned_ldst_spec\000memory\000Unaligned access\000event=0x6a\000\00000\000Unaligned access\000 */
{ 26720 }, /* unaligned_st_spec\000memory\000Unaligned access, write\000event=0x69\000\00000\000Unaligned access, write\000 */
{ 22650 }, /* vfp_spec\000instruction\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Operation speculatively executed, floating-point instruction\000 */

};

const struct pmu_table_entry pmu_events__arm_cortex_a65_e1[] = {
{
     .entries = pmu_events__arm_cortex_a65_e1_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__arm_cortex_a65_e1_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__arm_cortex_a710_default_core[] = {
{ 88931 }, /* ase_inst_spec\000instruction\000ASE operations speculatively executed\000event=0x8005\000\00000\000ASE operations speculatively executed\000 */
{ 22494 }, /* ase_spec\000instruction\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Operation speculatively executed, Advanced SIMD instruction\000 */
{ 45947 }, /* ase_sve_int16_spec\000instruction\000Advanced SIMD and SVE 16-bit integer operations speculatively executed\000event=0x80e7\000\00000\000Advanced SIMD and SVE 16-bit integer operations speculatively executed\000 */
{ 46137 }, /* ase_sve_int32_spec\000instruction\000Advanced SIMD and SVE 32-bit integer operations speculatively executed\000event=0x80eb\000\00000\000Advanced SIMD and SVE 32-bit integer operations speculatively executed\000 */
{ 46327 }, /* ase_sve_int64_spec\000instruction\000Advanced SIMD and SVE 64-bit integer operations speculatively executed\000event=0x80ef\000\00000\000Advanced SIMD and SVE 64-bit integer operations speculatively executed\000 */
{ 45760 }, /* ase_sve_int8_spec\000instruction\000Advanced SIMD and SVE 8-bit integer operations speculatively executed\000event=0x80e3\000\00000\000Advanced SIMD and SVE 8-bit integer operations speculatively executed\000 */
{ 13 }, /* br_immed_spec\000branch\000Branch speculatively executed, immediate branch\000event=0x78\000\00000\000Branch speculatively executed, immediate branch\000 */
{ 278 }, /* br_indirect_spec\000branch\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000 */
{ 411 }, /* br_mis_pred\000branch\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000Mispredicted or not predicted branch speculatively executed\000 */
{ 25146 }, /* br_mis_pred_retired\000instruction\000Instruction architecturally executed, mispredicted branch\000event=0x22\000\00000\000Instruction architecturally executed, mispredicted branch\000 */
{ 565 }, /* br_pred\000branch\000Predictable branch speculatively executed\000event=0x12\000\00000\000Predictable branch speculatively executed\000 */
{ 25018 }, /* br_retired\000instruction\000Instruction architecturally executed, branch\000event=0x21\000\00000\000Instruction architecturally executed, branch\000 */
{ 145 }, /* br_return_spec\000branch\000Branch speculatively executed, procedure return\000event=0x79\000\00000\000Branch speculatively executed, procedure return\000 */
{ 1311 }, /* bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000Attributable Bus access\000 */
{ 771 }, /* bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Bus access read\000 */
{ 836 }, /* bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Bus access write\000 */
{ 721 }, /* bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000Bus cycle\000 */
{ 24272 }, /* cid_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000 */
{ 1389 }, /* cnt_cycles\000bus\000Constant frequency cycles\000event=0x4004\000\00000\000Constant frequency cycles.  The counter increments at a constant frequency equal to the rate of increment of the system counter, CNTPCT_EL0\000 */
{ 679 }, /* cpu_cycles\000bus\000Cycle\000event=0x11\000\00000\000Cycle\000 */
{ 23330 }, /* crypto_spec\000instruction\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Operation speculatively executed, Cryptographic instruction\000 */
{ 54196 }, /* cti_trigout4\000trace\000Cross-trigger Interface output trigger 4\000event=0x4018\000\00000\000Cross-trigger Interface output trigger 4\000 */
{ 54314 }, /* cti_trigout5\000trace\000Cross-trigger Interface output trigger 5 \000event=0x4019\000\00000\000Cross-trigger Interface output trigger 5 \000 */
{ 54434 }, /* cti_trigout6\000trace\000Cross-trigger Interface output trigger 6\000event=0x401a\000\00000\000Cross-trigger Interface output trigger 6\000 */
{ 54552 }, /* cti_trigout7\000trace\000Cross-trigger Interface output trigger 7\000event=0x401b\000\00000\000Cross-trigger Interface output trigger 7\000 */
{ 23705 }, /* dmb_spec\000instruction\000Barrier speculatively executed, DMB\000event=0x7e\000\00000\000Barrier speculatively executed, DMB\000 */
{ 22343 }, /* dp_spec\000instruction\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Operation speculatively executed, integer data processing\000 */
{ 23597 }, /* dsb_spec\000instruction\000Barrier speculatively executed, DSB\000event=0x7d\000\00000\000Barrier speculatively executed, DSB\000 */
{ 4626 }, /* dtlb_walk\000cache\000Access to data TLB causes a translation table walk\000event=0x34\000\00000\000Access to data TLB causes a translation table walk\000 */
{ 20002 }, /* exc_dabort\000exception\000Exception taken, Data Abort and SError\000event=0x84\000\00000\000Exception taken, Data Abort and SError\000 */
{ 20191 }, /* exc_fiq\000exception\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000 */
{ 20266 }, /* exc_hvc\000exception\000Exception taken, Hypervisor Call\000event=0x8a\000\00000\000Exception taken, Hypervisor Call\000 */
{ 20116 }, /* exc_irq\000exception\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000 */
{ 19896 }, /* exc_pabort\000exception\000Exception taken, Instruction Abort\000event=0x83\000\00000\000Exception taken, Instruction Abort\000 */
{ 33889 }, /* exc_return\000instruction\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Instruction architecturally executed, condition check pass, exception return\000 */
{ 73325 }, /* exc_smc\000exception\000Exception taken, Secure Monitor Call\000event=0x88\000\00000\000Exception taken, Secure Monitor Call\000 */
{ 19797 }, /* exc_svc\000exception\000Exception taken, Supervisor Call\000event=0x82\000\00000\000Exception taken, Supervisor Call\000 */
{ 21031 }, /* exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Exception taken\000 */
{ 20512 }, /* exc_trap_dabort\000exception\000Exception taken, Data Abort or SError not taken locally\000event=0x8c\000\00000\000Exception taken, Data Abort or SError not taken locally\000 */
{ 20915 }, /* exc_trap_fiq\000exception\000Exception taken, FIQ not taken locally\000event=0x8f\000\00000\000Exception taken, FIQ not taken locally\000 */
{ 20799 }, /* exc_trap_irq\000exception\000Exception taken, IRQ not taken locally\000event=0x8e\000\00000\000Exception taken, IRQ not taken locally\000 */
{ 20665 }, /* exc_trap_other\000exception\000Exception taken, Other traps not taken locally\000event=0x8d\000\00000\000Exception taken, Other traps not taken locally\000 */
{ 20365 }, /* exc_trap_pabort\000exception\000Exception taken, Instruction Abort not taken locally\000event=0x8b\000\00000\000Exception taken, Instruction Abort not taken locally\000 */
{ 19692 }, /* exc_undef\000exception\000Exception taken, Other synchronous\000event=0x81\000\00000\000Exception taken, Other synchronous\000 */
{ 45588 }, /* fp_dp_spec\000instruction\000Floating-point double-precision operations speculatively executed\000event=0x801c\000\00000\000Floating-point double-precision operations speculatively executed\000 */
{ 90530 }, /* fp_fixed_ops_spec\000instruction\000Non-scalable floating-point element Operations speculatively executed\000event=0x80c1\000\00000\000Non-scalable floating-point element Operations speculatively executed\000 */
{ 45248 }, /* fp_hp_spec\000instruction\000Floating-point half-precision operations speculatively executed\000event=0x8014\000\00000\000Floating-point half-precision operations speculatively executed\000 */
{ 90351 }, /* fp_scale_ops_spec\000instruction\000Scalable floating-point element Operations speculatively executed\000event=0x80c0\000\00000\000Scalable floating-point element Operations speculatively executed\000 */
{ 45416 }, /* fp_sp_spec\000instruction\000Floating-point single-precision operations speculatively executed\000event=0x8018\000\00000\000Floating-point single-precision operations speculatively executed\000 */
{ 24159 }, /* inst_retired\000instruction\000Instruction architecturally executed\000event=0x8\000\00000\000Instruction architecturally executed\000 */
{ 24711 }, /* inst_spec\000instruction\000Operation speculatively executed\000event=0x1b\000\00000\000Operation speculatively executed\000 */
{ 23489 }, /* isb_spec\000instruction\000Barrier speculatively executed, ISB\000event=0x7c\000\00000\000Barrier speculatively executed, ISB\000 */
{ 4759 }, /* itlb_walk\000cache\000Access to instruction TLB that causes a translation table walk\000event=0x35\000\00000\000Access to instruction TLB that causes a translation table walk\000 */
{ 3313 }, /* l1d_cache\000cache\000Level 1 data cache access\000event=0x4\000\00000\000Level 1 data cache access\000 */
{ 1902 }, /* l1d_cache_inval\000cache\000L1D cache invalidate\000event=0x48\000\00000\000Attributable Level 1 data cache invalidate\000 */
{ 4916 }, /* l1d_cache_lmiss_rd\000cache\000Level 1 data cache long-latency read miss\000event=0x39\000\00000\000Level 1 data cache long-latency read miss.  The counter counts each memory read access counted by L1D_CACHE that incurs additional latency because it returns data from outside the Level 1 data or unified cache of this processing element\000 */
{ 1587 }, /* l1d_cache_rd\000cache\000L1D cache access, read\000event=0x40\000\00000\000Attributable Level 1 data cache access, read\000 */
{ 3224 }, /* l1d_cache_refill\000cache\000Level 1 data cache refill\000event=0x3\000\00000\000Level 1 data cache refill\000 */
{ 39214 }, /* l1d_cache_refill_inner\000cache\000L1D cache refill, inner\000event=0x44\000\00000\000Attributable Level 1 data cache refill, inner\000 */
{ 39328 }, /* l1d_cache_refill_outer\000cache\000L1D cache refill, outer\000event=0x45\000\00000\000Attributable Level 1 data cache refill, outer\000 */
{ 1793 }, /* l1d_cache_refill_rd\000cache\000L1D cache refill, read\000event=0x42\000\00000\000Attributable Level 1 data cache refill, read\000 */
{ 39103 }, /* l1d_cache_refill_wr\000cache\000L1D cache refill, write\000event=0x43\000\00000\000Attributable Level 1 data cache refill, write\000 */
{ 33516 }, /* l1d_cache_wb\000cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000Attributable Level 1 data cache write-back\000 */
{ 73186 }, /* l1d_cache_wb_clean\000cache\000L1D cache Write-Back, cleaning and coherency\000event=0x47\000\00000\000Level 1 data cache Write-Back, cleaning and coherency\000 */
{ 73065 }, /* l1d_cache_wb_victim\000cache\000L1D cache Write-Back, victim\000event=0x46\000\00000\000Attributable Level 1 data cache Write-Back, victim\000 */
{ 1689 }, /* l1d_cache_wr\000cache\000L1D cache access, write\000event=0x41\000\00000\000Attributable Level 1 data cache access, write\000 */
{ 3908 }, /* l1d_tlb\000cache\000Attributable Level 1 data or unified TLB access\000event=0x25\000\00000\000Attributable Level 1 data or unified TLB access\000 */
{ 88109 }, /* l1d_tlb_rd\000cache\000L1D tlb access, read\000event=0x4e\000\00000\000Attributable Level 1 data or unified TLB access, read\000 */
{ 3395 }, /* l1d_tlb_refill\000cache\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Attributable Level 1 data TLB refill\000 */
{ 2003 }, /* l1d_tlb_refill_rd\000cache\000L1D tlb refill, read\000event=0x4c\000\00000\000Attributable Level 1 data TLB refill, read\000 */
{ 2106 }, /* l1d_tlb_refill_wr\000cache\000L1D tlb refill, write\000event=0x4d\000\00000\000Attributable Level 1 data TLB refill, write\000 */
{ 88216 }, /* l1d_tlb_wr\000cache\000L1D tlb access, write\000event=0x4f\000\00000\000Attributable Level 1 data or unified TLB access, write\000 */
{ 3504 }, /* l1i_cache\000cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Attributable Level 1 instruction cache access\000 */
{ 5235 }, /* l1i_cache_lmiss\000cache\000Level 1 instruction cache long-latency read miss\000event=0x4006\000\00000\000Level 1 instruction cache long-latency read miss.  If the L1I_CACHE_RD event is implemented, the counter counts each access counted by L1I_CACHE_RD that incurs additional latency because it returns instructions from outside of the Level 1 instruction cache of this PE.  If the L1I_CACHE_RD event is not implemented, the counter counts each access counted by L1I_CACHE that incurs additional latency because it returns instructions from outside the Level 1 instruction cache of this PE.  The event indicates to software that the access missed in the Level 1 instruction cache and might have a significant performance impact due to the additional latency, compared to the latency of an access that hits in the Level 1 instruction cache\000 */
{ 2998 }, /* l1i_cache_refill\000cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Level 1 instruction cache refill\000 */
{ 4033 }, /* l1i_tlb\000cache\000Attributable Level 1 instruction TLB access\000event=0x26\000\00000\000Attributable Level 1 instruction TLB access\000 */
{ 3101 }, /* l1i_tlb_refill\000cache\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Attributable Level 1 instruction TLB refill\000 */
{ 3627 }, /* l2d_cache\000cache\000Level 2 data cache access\000event=0x16\000\00000\000Level 2 data cache access\000 */
{ 38627 }, /* l2d_cache_allocate\000cache\000Attributable Level 2 data cache allocation without refill\000event=0x20\000\00000\000Attributable Level 2 data cache allocation without refill\000 */
{ 2897 }, /* l2d_cache_inval\000cache\000L2D cache invalidate\000event=0x58\000\00000\000Attributable Level 2 data cache invalidate\000 */
{ 6057 }, /* l2d_cache_lmiss_rd\000cache\000Level 2 data cache long-latency read miss\000event=0x4009\000\00000\000Level 2 data cache long-latency read miss.  The counter counts each memory read access counted by L2D_CACHE that incurs additional latency because it returns data from outside the Level 2 data or unified cache of this processing element.  The event indicates to software that the access missed in the Level 2 data or unified cache and might have a significant performance impact compared to the latency of an access that hits in the Level 2 data or unified cache\000 */
{ 2211 }, /* l2d_cache_rd\000cache\000L2D cache access, read\000event=0x50\000\00000\000Attributable Level 2 data cache access, read\000 */
{ 3710 }, /* l2d_cache_refill\000cache\000Level 2 data refill\000event=0x17\000\00000\000Level 2 data refill\000 */
{ 2417 }, /* l2d_cache_refill_rd\000cache\000L2D cache refill, read\000event=0x52\000\00000\000Attributable Level 2 data cache refill, read\000 */
{ 2526 }, /* l2d_cache_refill_wr\000cache\000L2D cache refill, write\000event=0x53\000\00000\000Attributable Level 2 data cache refill, write\000 */
{ 3788 }, /* l2d_cache_wb\000cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Attributable Level 2 data cache write-back\000 */
{ 2758 }, /* l2d_cache_wb_clean\000cache\000L2D cache Write-Back, cleaning and coherency\000event=0x57\000\00000\000Level 2 data cache Write-Back, cleaning and coherency\000 */
{ 2637 }, /* l2d_cache_wb_victim\000cache\000L2D cache Write-Back, victim\000event=0x56\000\00000\000Attributable Level 2 data cache Write-Back, victim\000 */
{ 2313 }, /* l2d_cache_wr\000cache\000L2D cache access, write\000event=0x51\000\00000\000Attributable Level 2 data cache access, write\000 */
{ 4384 }, /* l2d_tlb\000cache\000Attributable Level 2 data or unified TLB access\000event=0x2f\000\00000\000Attributable Level 2 data or unified TLB access\000 */
{ 88559 }, /* l2d_tlb_rd\000cache\000L2D cache access, read\000event=0x5e\000\00000\000Attributable Level 2 data or unified TLB access, read\000 */
{ 4150 }, /* l2d_tlb_refill\000cache\000Attributable Level 2 data TLB refill\000event=0x2d\000\00000\000Attributable Level 2 data TLB refill\000 */
{ 88325 }, /* l2d_tlb_refill_rd\000cache\000L2D cache refill, read\000event=0x5c\000\00000\000Attributable Level 2 data or unified TLB refill, read\000 */
{ 88441 }, /* l2d_tlb_refill_wr\000cache\000L2D cache refill, write\000event=0x5d\000\00000\000Attributable Level 2 data or unified TLB refill, write\000 */
{ 88668 }, /* l2d_tlb_wr\000cache\000L2D cache access, write\000event=0x5f\000\00000\000Attributable Level 2 data or unified TLB access, write\000 */
{ 38783 }, /* l3d_cache\000cache\000Attributable Level 3 data cache access\000event=0x2b\000\00000\000Attributable Level 3 data cache access\000 */
{ 61550 }, /* l3d_cache_allocate\000cache\000Attributable Level 3 data cache allocation without refill\000event=0x29\000\00000\000Attributable Level 3 data cache allocation without refill\000 */
{ 44452 }, /* l3d_cache_lmiss_rd\000cache\000Level 3 data cache long-latency read miss\000event=0x400b\000\00000\000Level 3 data cache long-latency read miss.  The counter counts each memory read access counted by L3D_CACHE that incurs additional latency because it returns data from outside the Level 3 data or unified cache of this processing element.  The event indicates to software that the access missed in the Level 3 data or unified cache and might have a significant performance impact compared to the latency of an access that hits in the Level 3 data or unified cache\000 */
{ 39442 }, /* l3d_cache_rd\000cache\000Attributable Level 3 data or unified cache access, read\000event=0xa0\000\00000\000Attributable Level 3 data or unified cache access, read\000 */
{ 61706 }, /* l3d_cache_refill\000cache\000Attributable Level 3 data cache refill\000event=0x2a\000\00000\000Attributable Level 3 data cache refill\000 */
{ 26884 }, /* ld_align_lat\000memory\000Load with additional latency from alignment\000event=0x4021\000\00000\000Load with additional latency from alignment\000 */
{ 21982 }, /* ld_spec\000instruction\000Operation speculatively executed, load\000event=0x70\000\00000\000Operation speculatively executed, load\000 */
{ 73432 }, /* ldrex_spec\000instruction\000Exclusive operation speculatively executed, LDREX or LDX\000event=0x6c\000\00000\000Exclusive operation speculatively executed, LDREX or LDX\000 */
{ 27280 }, /* ldst_align_lat\000memory\000Access with additional latency from alignment\000event=0x4020\000\00000\000Access with additional latency from alignment\000 */
{ 39009 }, /* ll_cache_miss_rd\000cache\000Last level cache miss, read\000event=0x37\000\00000\000Last level cache miss, read\000 */
{ 38892 }, /* ll_cache_rd\000cache\000Attributable Last level cache memory read\000event=0x36\000\00000\000Attributable Last level cache memory read\000 */
{ 27136 }, /* mem_access\000memory\000Data memory access\000event=0x13\000\00000\000Data memory access\000 */
{ 27411 }, /* mem_access_checked\000memory\000Checked data memory access\000event=0x4024\000\00000\000Checked data memory access\000 */
{ 27508 }, /* mem_access_checked_rd\000memory\000Checked data memory access, read\000event=0x4025\000\00000\000Checked data memory access, read\000 */
{ 27620 }, /* mem_access_checked_wr\000memory\000Checked data memory access, write\000event=0x4026\000\00000\000Checked data memory access, write\000 */
{ 26460 }, /* mem_access_rd\000memory\000Data memory access, read\000event=0x66\000\00000\000Data memory access, read\000 */
{ 26546 }, /* mem_access_wr\000memory\000Data memory access, write\000event=0x67\000\00000\000Data memory access, write\000 */
{ 33636 }, /* memory_error\000exception\000Local memory error\000event=0x1a\000\00000\000Local memory error\000 */
{ 25309 }, /* op_retired\000instruction\000Micro-operation architecturally executed\000event=0x3a\000\00000\000Micro-operation architecturally executed.  The counter counts each operation counted by OP_SPEC that would be executed in a simple sequential execution of the program\000 */
{ 21665 }, /* op_spec\000instruction\000Micro-operation speculatively executed\000event=0x3b\000\00000\000Micro-operation speculatively executed.  The counter counts the number of operations executed by the processing element, including those that are executed speculatively and would not be executed in a simple sequential execution of the program\000 */
{ 22808 }, /* pc_write_spec\000instruction\000Operation speculatively executed, software change of the PC\000event=0x76\000\00000\000Operation speculatively executed, software change of the PC\000 */
{ 23813 }, /* rc_ld_spec\000instruction\000Release consistency operation speculatively executed, Load-Acquire\000event=0x90\000\00000\000Release consistency operation speculatively executed, Load-Acquire\000 */
{ 23985 }, /* rc_st_spec\000instruction\000Release consistency operation speculatively executed, Store-Release\000event=0x91\000\00000\000Release consistency operation speculatively executed, Store-Release\000 */
{ 90717 }, /* remote_access\000memory\000Access to another socket in a multi-socket system\000event=0x31\000\00000\000Access to another socket in a multi-socket system\000 */
{ 27009 }, /* st_align_lat\000memory\000Store with additional latency from alignment\000event=0x4022\000\00000\000Store with additional latency from alignment\000 */
{ 22095 }, /* st_spec\000instruction\000Operation speculatively executed, store\000event=0x71\000\00000\000Operation speculatively executed, store\000 */
{ 46833 }, /* stall\000pipeline\000No operation sent for execution\000event=0x3c\000\00000\000No operation sent for execution.  The counter counts every attributable cycle on which no attributable instruction or operation was sent for execution on this processing element\000 */
{ 27892 }, /* stall_backend\000pipeline\000No operation issued due to the backend\000event=0x24\000\00000\000No operation issued due to the backend\000 */
{ 29344 }, /* stall_backend_mem\000pipeline\000Memory stall cycles\000event=0x4005\000\00000\000Memory stall cycles.  The counter counts each cycle counted by STALL_BACKEND where there is a cache miss in the last level of cache within the processing element clock domain\000 */
{ 46706 }, /* stall_frontend\000pipeline\000No operation issued because of the frontend\000event=0x23\000\00000\000No operation issued because of the frontend\000 */
{ 29039 }, /* stall_slot\000pipeline\000No operation sent for execution on a slot\000event=0x3f\000\00000\000No operation sent for execution on a slot.  The counter counts on each attributable cycle the number of instruction or operation slots that were not occupied by an instruction or operation attributable to the processing element\000 */
{ 28291 }, /* stall_slot_backend\000pipeline\000No operation sent for execution on a slot due to the backend\000event=0x3d\000\00000\000No operation sent for execution on a slot due to the backend.  Counts each slot counted by STALL_SLOT where no attributable instruction or operation was sent for execution because the backend is unable to accept it\000 */
{ 47073 }, /* stall_slot_frontend\000pipeline\000No operation sent for execution on a slot due to the frontend\000event=0x3e\000\00000\000No operation sent for execution on a slot due to the frontend.  Counts each slot counted by STALL_SLOT where no attributable instruction or operation was sent for execution because there was no attributable instruction or operation available to issue from the processing element from the frontend for the slot\000 */
{ 73751 }, /* strex_fail_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX fail\000event=0x6e\000\00000\000Exclusive operation speculatively executed, STREX or STX fail\000 */
{ 73584 }, /* strex_pass_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX pass\000event=0x6d\000\00000\000Exclusive operation speculatively executed, STREX or STX pass\000 */
{ 88779 }, /* strex_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX\000event=0x6f\000\00000\000Exclusive operation speculatively executed, STREX or STX\000 */
{ 45129 }, /* sve_inst_spec\000instruction\000SVE operations speculatively executed\000event=0x8006\000\00000\000SVE operations speculatively executed\000 */
{ 90146 }, /* sve_ldff_fault_spec\000instruction\000SVE first-fault load operations speculatively executed which set FFR bit to 0\000event=0x80bd\000\00000\000SVE first-fault load operations speculatively executed which set FFR bit to 0\000 */
{ 89993 }, /* sve_ldff_spec\000instruction\000SVE First-fault load Operations speculatively executed\000event=0x80bc\000\00000\000SVE First-fault load Operations speculatively executed\000 */
{ 89191 }, /* sve_pred_empty_spec\000instruction\000SVE predicated operations with no active predicates speculatively executed\000event=0x8075\000\00000\000SVE predicated operations with no active predicates speculatively executed\000 */
{ 89390 }, /* sve_pred_full_spec\000instruction\000SVE predicated operations speculatively executed with all active predicates\000event=0x8076\000\00000\000SVE predicated operations speculatively executed with all active predicates\000 */
{ 89805 }, /* sve_pred_not_full_spec\000instruction\000SVE predicated operations with empty or partially active predicates\000event=0x8079\000\00000\000SVE predicated operations with empty or partially active predicates\000 */
{ 89590 }, /* sve_pred_partial_spec\000instruction\000SVE predicated operations speculatively executed with partially active predicates\000event=0x8077\000\00000\000SVE predicated operations speculatively executed with partially active predicates\000 */
{ 89050 }, /* sve_pred_spec\000instruction\000SVE predicated Operations speculatively executed\000event=0x8074\000\00000\000SVE predicated Operations speculatively executed\000 */
{ 21286 }, /* sw_incr\000instruction\000Instruction architecturally executed, Condition code check pass, software increment\000event=0\000\00000\000Instruction architecturally executed, Condition code check pass, software increment\000 */
{ 53600 }, /* trb_wrap\000trace\000Trace buffer current write pointer wrapped\000event=0x400c\000\00000\000Trace buffer current write pointer wrapped\000 */
{ 53804 }, /* trcextout0\000trace\000PE Trace Unit external output 0\000event=0x4010\000\00000\000PE Trace Unit external output 0\000 */
{ 53902 }, /* trcextout1\000trace\000PE Trace Unit external output 1\000event=0x4011\000\00000\000PE Trace Unit external output 1\000 */
{ 54000 }, /* trcextout2\000trace\000PE Trace Unit external output 2\000event=0x4012\000\00000\000PE Trace Unit external output 2\000 */
{ 54098 }, /* trcextout3\000trace\000PE Trace Unit external output 3\000event=0x4013\000\00000\000PE Trace Unit external output 3\000 */
{ 24814 }, /* ttbr_write_retired\000instruction\000Instruction architecturally executed, Condition code check pass, write to TTBR\000event=0x1c\000\00000\000Instruction architecturally executed, Condition code check pass, write to TTBR\000 */
{ 26634 }, /* unaligned_ld_spec\000memory\000Unaligned access, read\000event=0x68\000\00000\000Unaligned access, read\000 */
{ 26808 }, /* unaligned_ldst_spec\000memory\000Unaligned access\000event=0x6a\000\00000\000Unaligned access\000 */
{ 26720 }, /* unaligned_st_spec\000memory\000Unaligned access, write\000event=0x69\000\00000\000Unaligned access, write\000 */
{ 22650 }, /* vfp_spec\000instruction\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Operation speculatively executed, floating-point instruction\000 */

};

const struct pmu_table_entry pmu_events__arm_cortex_a710[] = {
{
     .entries = pmu_events__arm_cortex_a710_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__arm_cortex_a710_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__arm_cortex_a73_default_core[] = {
{ 22494 }, /* ase_spec\000instruction\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Operation speculatively executed, Advanced SIMD instruction\000 */
{ 22969 }, /* br_immed_retired\000instruction\000Instruction architecturally executed, immediate branch\000event=0xd\000\00000\000Instruction architecturally executed, immediate branch\000 */
{ 278 }, /* br_indirect_spec\000branch\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000 */
{ 411 }, /* br_mis_pred\000branch\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000Mispredicted or not predicted branch speculatively executed\000 */
{ 565 }, /* br_pred\000branch\000Predictable branch speculatively executed\000event=0x12\000\00000\000Predictable branch speculatively executed\000 */
{ 23122 }, /* br_return_retired\000instruction\000Instruction architecturally executed, condition code check pass, procedure return\000event=0xe\000\00000\000Instruction architecturally executed, condition code check pass, procedure return\000 */
{ 1311 }, /* bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000Attributable Bus access\000 */
{ 1153 }, /* bus_access_normal\000bus\000Bus access, Normal\000event=0x64\000\00000\000Bus access, Normal\000 */
{ 1022 }, /* bus_access_not_shared\000bus\000Bus access, not Normal, Cacheable, Shareable\000event=0x63\000\00000\000Bus access, not Normal, Cacheable, Shareable\000 */
{ 1228 }, /* bus_access_periph\000bus\000Bus access, peripheral\000event=0x65\000\00000\000Bus access, peripheral\000 */
{ 903 }, /* bus_access_shared\000bus\000Bus access, Normal, Cacheable, Shareable\000event=0x62\000\00000\000Bus access, Normal, Cacheable, Shareable\000 */
{ 721 }, /* bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000Bus cycle\000 */
{ 24272 }, /* cid_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000 */
{ 91358 }, /* cp15_utlb_refill\000cache\000Level 1 CP15 TLB refill\000event=0xe8\000\00000\000Level 1 CP15 TLB refill\000 */
{ 679 }, /* cpu_cycles\000bus\000Cycle\000event=0x11\000\00000\000Cycle\000 */
{ 23330 }, /* crypto_spec\000instruction\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Operation speculatively executed, Cryptographic instruction\000 */
{ 93559 }, /* d_lsu_slot_full\000pipeline\000Duration for which all slots in the Load-Store Unit are busy\000event=0xd3\000\00000\000Duration for which all slots in the Load-Store Unit are busy\000 */
{ 91646 }, /* dcache_self_hit_vipt\000cache\000Data cache hit in itself due to VIPT aliasing\000event=0xec\000\00000\000Data cache hit in itself due to VIPT aliasing\000 */
{ 94073 }, /* de_iq_full\000pipeline\000Duration for which all slots in the Data Engine issue queue are busy\000event=0xda\000\00000\000Duration for which all slots in the Data Engine issue queue are busy\000 */
{ 23705 }, /* dmb_spec\000instruction\000Barrier speculatively executed, DMB\000event=0x7e\000\00000\000Barrier speculatively executed, DMB\000 */
{ 93892 }, /* dp_iq_full\000pipeline\000Duration for which all slots in the data processing issue queue are busy\000event=0xd9\000\00000\000Duration for which all slots in the data processing issue queue are busy\000 */
{ 22343 }, /* dp_spec\000instruction\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Operation speculatively executed, integer data processing\000 */
{ 23597 }, /* dsb_spec\000instruction\000Barrier speculatively executed, DSB\000event=0x7d\000\00000\000Barrier speculatively executed, DSB\000 */
{ 91780 }, /* etm_ext_out0\000etm\000ETM trace unit output 0\000event=0xde\000\00000\000ETM trace unit output 0\000 */
{ 91860 }, /* etm_ext_out1\000etm\000ETM trace unit output 1\000event=0xdf\000\00000\000ETM trace unit output 1\000 */
{ 20266 }, /* exc_hvc\000exception\000Exception taken, Hypervisor Call\000event=0x8a\000\00000\000Exception taken, Hypervisor Call\000 */
{ 33889 }, /* exc_return\000instruction\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Instruction architecturally executed, condition check pass, exception return\000 */
{ 21031 }, /* exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Exception taken\000 */
{ 91940 }, /* exc_trap_hyp\000exception\000Number of Traps to hypervisor\000event=0xdc\000\00000\000Number of Traps to hypervisor\000 */
{ 91144 }, /* i_btac_ram_rd\000cache\000Number of ways read in the instruction BTAC RAM\000event=0xc4\000\00000\000Number of ways read in the instruction BTAC RAM\000 */
{ 90997 }, /* i_data_ram_rd\000cache\000Number of ways read in the instruction cache - Data RAM\000event=0xc3\000\00000\000Number of ways read in the instruction cache - Data RAM\000 */
{ 90853 }, /* i_tag_ram_rd\000cache\000Number of ways read in the instruction cache - Tag RAM\000event=0xc2\000\00000\000Number of ways read in the instruction cache - Tag RAM\000 */
{ 24159 }, /* inst_retired\000instruction\000Instruction architecturally executed\000event=0x8\000\00000\000Instruction architecturally executed\000 */
{ 24711 }, /* inst_spec\000instruction\000Operation speculatively executed\000event=0x1b\000\00000\000Operation speculatively executed\000 */
{ 23489 }, /* isb_spec\000instruction\000Barrier speculatively executed, ISB\000event=0x7c\000\00000\000Barrier speculatively executed, ISB\000 */
{ 3313 }, /* l1d_cache\000cache\000Level 1 data cache access\000event=0x4\000\00000\000Level 1 data cache access\000 */
{ 1587 }, /* l1d_cache_rd\000cache\000L1D cache access, read\000event=0x40\000\00000\000Attributable Level 1 data cache access, read\000 */
{ 3224 }, /* l1d_cache_refill\000cache\000Level 1 data cache refill\000event=0x3\000\00000\000Level 1 data cache refill\000 */
{ 33516 }, /* l1d_cache_wb\000cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000Attributable Level 1 data cache write-back\000 */
{ 1689 }, /* l1d_cache_wr\000cache\000L1D cache access, write\000event=0x41\000\00000\000Attributable Level 1 data cache access, write\000 */
{ 3395 }, /* l1d_tlb_refill\000cache\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Attributable Level 1 data TLB refill\000 */
{ 3504 }, /* l1i_cache\000cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Attributable Level 1 instruction cache access\000 */
{ 2998 }, /* l1i_cache_refill\000cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Level 1 instruction cache refill\000 */
{ 3101 }, /* l1i_tlb_refill\000cache\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Attributable Level 1 instruction TLB refill\000 */
{ 3627 }, /* l2d_cache\000cache\000Level 2 data cache access\000event=0x16\000\00000\000Level 2 data cache access\000 */
{ 2897 }, /* l2d_cache_inval\000cache\000L2D cache invalidate\000event=0x58\000\00000\000Attributable Level 2 data cache invalidate\000 */
{ 2211 }, /* l2d_cache_rd\000cache\000L2D cache access, read\000event=0x50\000\00000\000Attributable Level 2 data cache access, read\000 */
{ 3710 }, /* l2d_cache_refill\000cache\000Level 2 data refill\000event=0x17\000\00000\000Level 2 data refill\000 */
{ 3788 }, /* l2d_cache_wb\000cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Attributable Level 2 data cache write-back\000 */
{ 2758 }, /* l2d_cache_wb_clean\000cache\000L2D cache Write-Back, cleaning and coherency\000event=0x57\000\00000\000Level 2 data cache Write-Back, cleaning and coherency\000 */
{ 2637 }, /* l2d_cache_wb_victim\000cache\000L2D cache Write-Back, victim\000event=0x56\000\00000\000Attributable Level 2 data cache Write-Back, victim\000 */
{ 2313 }, /* l2d_cache_wr\000cache\000L2D cache access, write\000event=0x51\000\00000\000Attributable Level 2 data cache access, write\000 */
{ 21982 }, /* ld_spec\000instruction\000Operation speculatively executed, load\000event=0x70\000\00000\000Operation speculatively executed, load\000 */
{ 73432 }, /* ldrex_spec\000instruction\000Exclusive operation speculatively executed, LDREX or LDX\000event=0x6c\000\00000\000Exclusive operation speculatively executed, LDREX or LDX\000 */
{ 22210 }, /* ldst_spec\000instruction\000Operation speculatively executed, load or store\000event=0x72\000\00000\000Operation speculatively executed, load or store\000 */
{ 93288 }, /* lf_stall\000pipeline\000A linefill caused an instruction side stall\000event=0xc0\000\00000\000A linefill caused an instruction side stall\000 */
{ 93721 }, /* ls_iq_full\000pipeline\000Duration for which all slots in the load-store issue queue are busy\000event=0xd8\000\00000\000Duration for which all slots in the load-store issue queue are busy\000 */
{ 27136 }, /* mem_access\000memory\000Data memory access\000event=0x13\000\00000\000Data memory access\000 */
{ 26460 }, /* mem_access_rd\000memory\000Data memory access, read\000event=0x66\000\00000\000Data memory access, read\000 */
{ 26546 }, /* mem_access_wr\000memory\000Data memory access, write\000event=0x67\000\00000\000Data memory access, write\000 */
{ 92038 }, /* mmu_ptw\000mmu\000Duration of a translation table walk handled by the MMU\000event=0xe0\000\00000\000Duration of a translation table walk handled by the MMU\000 */
{ 93032 }, /* mmu_ptw_cp15\000mmu\000Duration of a translation table walk requested by a CP15 operation (maintenance by MVA and VA to PA operations)\000event=0xe6\000\00000\000Duration of a translation table walk requested by a CP15 operation (maintenance by MVA and VA to PA operations)\000 */
{ 92642 }, /* mmu_ptw_iside\000mmu\000Duration of a translation table walk requested by the Instruction Side\000event=0xe4\000\00000\000Duration of a translation table walk requested by the Instruction Side\000 */
{ 92495 }, /* mmu_ptw_lsu\000mmu\000Duration of a translation table walk requested by the LSU\000event=0xe3\000\00000\000Duration of a translation table walk requested by the LSU\000 */
{ 92817 }, /* mmu_ptw_pld\000mmu\000Duration of a translation table walk requested by a Preload instruction or Prefetch request\000event=0xe5\000\00000\000Duration of a translation table walk requested by a Preload instruction or Prefetch request\000 */
{ 92177 }, /* mmu_ptw_st1\000mmu\000Duration of a Stage 1 translation table walk handled by the MMU\000event=0xe1\000\00000\000Duration of a Stage 1 translation table walk handled by the MMU\000 */
{ 92336 }, /* mmu_ptw_st2\000mmu\000Duration of a Stage 2 translation table walk handled by the MMU\000event=0xe2\000\00000\000Duration of a Stage 2 translation table walk handled by the MMU\000 */
{ 24486 }, /* pc_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, software change of the PC\000event=0xc\000\00000\000Instruction architecturally executed, condition code check pass, software change of the PC\000 */
{ 91275 }, /* pld_utlb_refill\000cache\000Level 1 PLD TLB refill\000event=0xe7\000\00000\000Level 1 PLD TLB refill\000 */
{ 93409 }, /* ptw_stall\000pipeline\000A translation table walk caused an instruction side stall\000event=0xc1\000\00000\000A translation table walk caused an instruction side stall\000 */
{ 22095 }, /* st_spec\000instruction\000Operation speculatively executed, store\000event=0x71\000\00000\000Operation speculatively executed, store\000 */
{ 73751 }, /* strex_fail_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX fail\000event=0x6e\000\00000\000Exclusive operation speculatively executed, STREX or STX fail\000 */
{ 21286 }, /* sw_incr\000instruction\000Instruction architecturally executed, Condition code check pass, software increment\000event=0\000\00000\000Instruction architecturally executed, Condition code check pass, software increment\000 */
{ 91512 }, /* tlb_access\000cache\000Level 2 TLB access\000event=0xea\000\00000\000Level 2 TLB access\000 */
{ 91582 }, /* tlb_miss\000cache\000Level 2 TLB miss\000event=0xeb\000\00000\000Level 2 TLB miss\000 */
{ 24814 }, /* ttbr_write_retired\000instruction\000Instruction architecturally executed, Condition code check pass, write to TTBR\000event=0x1c\000\00000\000Instruction architecturally executed, Condition code check pass, write to TTBR\000 */
{ 26808 }, /* unaligned_ldst_spec\000memory\000Unaligned access\000event=0x6a\000\00000\000Unaligned access\000 */
{ 91444 }, /* utlb_flush\000cache\000Level 1 TLB flush\000event=0xe9\000\00000\000Level 1 TLB flush\000 */
{ 22650 }, /* vfp_spec\000instruction\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Operation speculatively executed, floating-point instruction\000 */

};

const struct pmu_table_entry pmu_events__arm_cortex_a73[] = {
{
     .entries = pmu_events__arm_cortex_a73_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__arm_cortex_a73_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__arm_cortex_a75_default_core[] = {
{ 22494 }, /* ase_spec\000instruction\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Operation speculatively executed, Advanced SIMD instruction\000 */
{ 22969 }, /* br_immed_retired\000instruction\000Instruction architecturally executed, immediate branch\000event=0xd\000\00000\000Instruction architecturally executed, immediate branch\000 */
{ 278 }, /* br_indirect_spec\000branch\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000 */
{ 411 }, /* br_mis_pred\000branch\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000Mispredicted or not predicted branch speculatively executed\000 */
{ 565 }, /* br_pred\000branch\000Predictable branch speculatively executed\000event=0x12\000\00000\000Predictable branch speculatively executed\000 */
{ 25018 }, /* br_retired\000instruction\000Instruction architecturally executed, branch\000event=0x21\000\00000\000Instruction architecturally executed, branch\000 */
{ 23122 }, /* br_return_retired\000instruction\000Instruction architecturally executed, condition code check pass, procedure return\000event=0xe\000\00000\000Instruction architecturally executed, condition code check pass, procedure return\000 */
{ 1311 }, /* bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000Attributable Bus access\000 */
{ 771 }, /* bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Bus access read\000 */
{ 836 }, /* bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Bus access write\000 */
{ 721 }, /* bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000Bus cycle\000 */
{ 24272 }, /* cid_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000 */
{ 679 }, /* cpu_cycles\000bus\000Cycle\000event=0x11\000\00000\000Cycle\000 */
{ 23330 }, /* crypto_spec\000instruction\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Operation speculatively executed, Cryptographic instruction\000 */
{ 97336 }, /* d_lsu_slot_full\000pipeline\000Duration for which all slots in the Load-Store Unit (LSU) are busy\000event=0xd3\000\00000\000Duration for which all slots in the Load-Store Unit (LSU) are busy\000 */
{ 98472 }, /* de_iq_full\000pipeline\000Duration for which all slots in the data engine issue queue are busy. This event is set every time that the data engine rename has at least one valid instruction, excluding No Operations (NOPs), that cannot move to the issue stage because accpt_instr is LOW\000event=0xda\000\00000\000Duration for which all slots in the data engine issue queue are busy. This event is set every time that the data engine rename has at least one valid instruction, excluding No Operations (NOPs), that cannot move to the issue stage because accpt_instr is LOW\000 */
{ 23705 }, /* dmb_spec\000instruction\000Barrier speculatively executed, DMB\000event=0x7e\000\00000\000Barrier speculatively executed, DMB\000 */
{ 97977 }, /* dp_iq_full\000pipeline\000Duration for which all slots in the data processing issue queue are busy. This event counts the cycles where all slots in the DP0 and DP1 IQs are full with micro-operations waiting for issuing, and the despatch stage is not empty\000event=0xd9\000\00000\000Duration for which all slots in the data processing issue queue are busy. This event counts the cycles where all slots in the DP0 and DP1 IQs are full with micro-operations waiting for issuing, and the despatch stage is not empty\000 */
{ 22343 }, /* dp_spec\000instruction\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Operation speculatively executed, integer data processing\000 */
{ 23597 }, /* dsb_spec\000instruction\000Barrier speculatively executed, DSB\000event=0x7d\000\00000\000Barrier speculatively executed, DSB\000 */
{ 4626 }, /* dtlb_walk\000cache\000Access to data TLB causes a translation table walk\000event=0x34\000\00000\000Access to data TLB causes a translation table walk\000 */
{ 91780 }, /* etm_ext_out0\000etm\000ETM trace unit output 0\000event=0xde\000\00000\000ETM trace unit output 0\000 */
{ 91860 }, /* etm_ext_out1\000etm\000ETM trace unit output 1\000event=0xdf\000\00000\000ETM trace unit output 1\000 */
{ 20266 }, /* exc_hvc\000exception\000Exception taken, Hypervisor Call\000event=0x8a\000\00000\000Exception taken, Hypervisor Call\000 */
{ 33889 }, /* exc_return\000instruction\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Instruction architecturally executed, condition check pass, exception return\000 */
{ 21031 }, /* exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Exception taken\000 */
{ 95565 }, /* exc_trap_hyp\000exception\000Number of traps to hypervisor. This event counts the number of exception traps taken to EL2, excluding HVC instructions. This event is set every time that an exception is executed because of a decoded trap to the hypervisor. CCFAIL exceptions and traps caused by HVC instructions are excluded. This event is not counted when it is accessible from Non-secure EL0 or EL1\000event=0xdc\000\00000\000Number of traps to hypervisor. This event counts the number of exception traps taken to EL2, excluding HVC instructions. This event is set every time that an exception is executed because of a decoded trap to the hypervisor. CCFAIL exceptions and traps caused by HVC instructions are excluded. This event is not counted when it is accessible from Non-secure EL0 or EL1\000 */
{ 19692 }, /* exc_undef\000exception\000Exception taken, Other synchronous\000event=0x81\000\00000\000Exception taken, Other synchronous\000 */
{ 91144 }, /* i_btac_ram_rd\000cache\000Number of ways read in the instruction BTAC RAM\000event=0xc4\000\00000\000Number of ways read in the instruction BTAC RAM\000 */
{ 90997 }, /* i_data_ram_rd\000cache\000Number of ways read in the instruction cache - Data RAM\000event=0xc3\000\00000\000Number of ways read in the instruction cache - Data RAM\000 */
{ 90853 }, /* i_tag_ram_rd\000cache\000Number of ways read in the instruction cache - Tag RAM\000event=0xc2\000\00000\000Number of ways read in the instruction cache - Tag RAM\000 */
{ 24159 }, /* inst_retired\000instruction\000Instruction architecturally executed\000event=0x8\000\00000\000Instruction architecturally executed\000 */
{ 24711 }, /* inst_spec\000instruction\000Operation speculatively executed\000event=0x1b\000\00000\000Operation speculatively executed\000 */
{ 23489 }, /* isb_spec\000instruction\000Barrier speculatively executed, ISB\000event=0x7c\000\00000\000Barrier speculatively executed, ISB\000 */
{ 4759 }, /* itlb_walk\000cache\000Access to instruction TLB that causes a translation table walk\000event=0x35\000\00000\000Access to instruction TLB that causes a translation table walk\000 */
{ 3313 }, /* l1d_cache\000cache\000Level 1 data cache access\000event=0x4\000\00000\000Level 1 data cache access\000 */
{ 94246 }, /* l1d_cache_allocate\000cache\000Level 1 data cache allocation without refill\000event=0x1f\000\00000\000Level 1 data cache allocation without refill\000 */
{ 1902 }, /* l1d_cache_inval\000cache\000L1D cache invalidate\000event=0x48\000\00000\000Attributable Level 1 data cache invalidate\000 */
{ 1587 }, /* l1d_cache_rd\000cache\000L1D cache access, read\000event=0x40\000\00000\000Attributable Level 1 data cache access, read\000 */
{ 3224 }, /* l1d_cache_refill\000cache\000Level 1 data cache refill\000event=0x3\000\00000\000Level 1 data cache refill\000 */
{ 33516 }, /* l1d_cache_wb\000cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000Attributable Level 1 data cache write-back\000 */
{ 73186 }, /* l1d_cache_wb_clean\000cache\000L1D cache Write-Back, cleaning and coherency\000event=0x47\000\00000\000Level 1 data cache Write-Back, cleaning and coherency\000 */
{ 73065 }, /* l1d_cache_wb_victim\000cache\000L1D cache Write-Back, victim\000event=0x46\000\00000\000Attributable Level 1 data cache Write-Back, victim\000 */
{ 1689 }, /* l1d_cache_wr\000cache\000L1D cache access, write\000event=0x41\000\00000\000Attributable Level 1 data cache access, write\000 */
{ 3908 }, /* l1d_tlb\000cache\000Attributable Level 1 data or unified TLB access\000event=0x25\000\00000\000Attributable Level 1 data or unified TLB access\000 */
{ 3395 }, /* l1d_tlb_refill\000cache\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Attributable Level 1 data TLB refill\000 */
{ 3504 }, /* l1i_cache\000cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Attributable Level 1 instruction cache access\000 */
{ 2998 }, /* l1i_cache_refill\000cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Level 1 instruction cache refill\000 */
{ 4033 }, /* l1i_tlb\000cache\000Attributable Level 1 instruction TLB access\000event=0x26\000\00000\000Attributable Level 1 instruction TLB access\000 */
{ 3101 }, /* l1i_tlb_refill\000cache\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Attributable Level 1 instruction TLB refill\000 */
{ 94719 }, /* l1pld_tlb\000cache\000Level 1 preload TLB access. This event only counts software and hardware prefetches at Level 1. This event counts all accesses to the preload data micro TLB, that is L1 prefetcher and preload instructions. This event does not take into account whether the MMU is enabled or not\000event=0xeb\000\00000\000Level 1 preload TLB access. This event only counts software and hardware prefetches at Level 1. This event counts all accesses to the preload data micro TLB, that is L1 prefetcher and preload instructions. This event does not take into account whether the MMU is enabled or not\000 */
{ 94376 }, /* l1pld_tlb_refill\000cache\000Level 1 PLD TLB refill\000event=0xe7\000\00000\000Level 1 PLD TLB refill\000 */
{ 3627 }, /* l2d_cache\000cache\000Level 2 data cache access\000event=0x16\000\00000\000Level 2 data cache access\000 */
{ 38627 }, /* l2d_cache_allocate\000cache\000Attributable Level 2 data cache allocation without refill\000event=0x20\000\00000\000Attributable Level 2 data cache allocation without refill\000 */
{ 2897 }, /* l2d_cache_inval\000cache\000L2D cache invalidate\000event=0x58\000\00000\000Attributable Level 2 data cache invalidate\000 */
{ 2211 }, /* l2d_cache_rd\000cache\000L2D cache access, read\000event=0x50\000\00000\000Attributable Level 2 data cache access, read\000 */
{ 3710 }, /* l2d_cache_refill\000cache\000Level 2 data refill\000event=0x17\000\00000\000Level 2 data refill\000 */
{ 3788 }, /* l2d_cache_wb\000cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Attributable Level 2 data cache write-back\000 */
{ 2758 }, /* l2d_cache_wb_clean\000cache\000L2D cache Write-Back, cleaning and coherency\000event=0x57\000\00000\000Level 2 data cache Write-Back, cleaning and coherency\000 */
{ 2637 }, /* l2d_cache_wb_victim\000cache\000L2D cache Write-Back, victim\000event=0x56\000\00000\000Attributable Level 2 data cache Write-Back, victim\000 */
{ 2313 }, /* l2d_cache_wr\000cache\000L2D cache access, write\000event=0x51\000\00000\000Attributable Level 2 data cache access, write\000 */
{ 4384 }, /* l2d_tlb\000cache\000Attributable Level 2 data or unified TLB access\000event=0x2f\000\00000\000Attributable Level 2 data or unified TLB access\000 */
{ 4150 }, /* l2d_tlb_refill\000cache\000Attributable Level 2 data TLB refill\000event=0x2d\000\00000\000Attributable Level 2 data TLB refill\000 */
{ 4509 }, /* l2i_tlb\000cache\000Attributable Level 2 instruction TLB access\000event=0x30\000\00000\000Attributable Level 2 instruction TLB access\000 */
{ 94460 }, /* l2pld_tlb\000cache\000Level 2 preload and MMU prefetcher TLB access. This event only counts software and hardware prefetches at Level 2\000event=0xe8\000\00000\000Level 2 preload and MMU prefetcher TLB access. This event only counts software and hardware prefetches at Level 2\000 */
{ 38783 }, /* l3d_cache\000cache\000Attributable Level 3 data cache access\000event=0x2b\000\00000\000Attributable Level 3 data cache access\000 */
{ 61550 }, /* l3d_cache_allocate\000cache\000Attributable Level 3 data cache allocation without refill\000event=0x29\000\00000\000Attributable Level 3 data cache allocation without refill\000 */
{ 39442 }, /* l3d_cache_rd\000cache\000Attributable Level 3 data or unified cache access, read\000event=0xa0\000\00000\000Attributable Level 3 data or unified cache access, read\000 */
{ 61706 }, /* l3d_cache_refill\000cache\000Attributable Level 3 data cache refill\000event=0x2a\000\00000\000Attributable Level 3 data cache refill\000 */
{ 39588 }, /* l3d_cache_refill_rd\000cache\000Attributable Level 3 data or unified cache refill, read\000event=0xa2\000\00000\000Attributable Level 3 data or unified cache refill, read\000 */
{ 21982 }, /* ld_spec\000instruction\000Operation speculatively executed, load\000event=0x70\000\00000\000Operation speculatively executed, load\000 */
{ 73432 }, /* ldrex_spec\000instruction\000Exclusive operation speculatively executed, LDREX or LDX\000event=0x6c\000\00000\000Exclusive operation speculatively executed, LDREX or LDX\000 */
{ 22210 }, /* ldst_spec\000instruction\000Operation speculatively executed, load or store\000event=0x72\000\00000\000Operation speculatively executed, load or store\000 */
{ 93288 }, /* lf_stall\000pipeline\000A linefill caused an instruction side stall\000event=0xc0\000\00000\000A linefill caused an instruction side stall\000 */
{ 39009 }, /* ll_cache_miss_rd\000cache\000Last level cache miss, read\000event=0x37\000\00000\000Last level cache miss, read\000 */
{ 38892 }, /* ll_cache_rd\000cache\000Attributable Last level cache memory read\000event=0x36\000\00000\000Attributable Last level cache memory read\000 */
{ 97510 }, /* ls_iq_full\000pipeline\000Duration for which all slots in the load-store issue queue are busy. This event counts the cycles where all slots in the LS IQs are full with micro-operations waiting for issuing, and the dispatch stage is not empty\000event=0xd8\000\00000\000Duration for which all slots in the load-store issue queue are busy. This event counts the cycles where all slots in the LS IQs are full with micro-operations waiting for issuing, and the dispatch stage is not empty\000 */
{ 27136 }, /* mem_access\000memory\000Data memory access\000event=0x13\000\00000\000Data memory access\000 */
{ 26460 }, /* mem_access_rd\000memory\000Data memory access, read\000event=0x66\000\00000\000Data memory access, read\000 */
{ 26546 }, /* mem_access_wr\000memory\000Data memory access, write\000event=0x67\000\00000\000Data memory access, write\000 */
{ 92038 }, /* mmu_ptw\000mmu\000Duration of a translation table walk handled by the MMU\000event=0xe0\000\00000\000Duration of a translation table walk handled by the MMU\000 */
{ 97138 }, /* mmu_ptw_cp15\000mmu\000Duration of a translation table walk requested by an address translation operation\000event=0xe6\000\00000\000Duration of a translation table walk requested by an address translation operation\000 */
{ 96963 }, /* mmu_ptw_iside\000mmu\000Duration of a translation table walk requested by the instruction side\000event=0xe4\000\00000\000Duration of a translation table walk requested by the instruction side\000 */
{ 92495 }, /* mmu_ptw_lsu\000mmu\000Duration of a translation table walk requested by the LSU\000event=0xe3\000\00000\000Duration of a translation table walk requested by the LSU\000 */
{ 92817 }, /* mmu_ptw_pld\000mmu\000Duration of a translation table walk requested by a Preload instruction or Prefetch request\000event=0xe5\000\00000\000Duration of a translation table walk requested by a Preload instruction or Prefetch request\000 */
{ 96341 }, /* mmu_ptw_st1\000mmu\000Duration of a Stage 1 translation table walk handled by the MMU. This event is not counted when it is accessible from Non-secure EL0 or EL1\000event=0xe1\000\00000\000Duration of a Stage 1 translation table walk handled by the MMU. This event is not counted when it is accessible from Non-secure EL0 or EL1\000 */
{ 96652 }, /* mmu_ptw_st2\000mmu\000Duration of a Stage 2 translation table walk handled by the MMU. This event is not counted when it is accessible from Non-secure EL0 or EL1\000event=0xe2\000\00000\000Duration of a Stage 2 translation table walk handled by the MMU. This event is not counted when it is accessible from Non-secure EL0 or EL1\000 */
{ 24486 }, /* pc_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, software change of the PC\000event=0xc\000\00000\000Instruction architecturally executed, condition code check pass, software change of the PC\000 */
{ 95306 }, /* pldtlb_walk\000cache\000Prefetch access to unified TLB that caused a page table walk. This event counts software and hardware prefetches\000event=0xec\000\00000\000Prefetch access to unified TLB that caused a page table walk. This event counts software and hardware prefetches\000 */
{ 93409 }, /* ptw_stall\000pipeline\000A translation table walk caused an instruction side stall\000event=0xc1\000\00000\000A translation table walk caused an instruction side stall\000 */
{ 46517 }, /* remote_access_rd\000memory\000Attributable memory read access to another socket in a multi-socket system\000event=0x38\000\00000\000Attributable memory read access to another socket in a multi-socket system\000 */
{ 22095 }, /* st_spec\000instruction\000Operation speculatively executed, store\000event=0x71\000\00000\000Operation speculatively executed, store\000 */
{ 27892 }, /* stall_backend\000pipeline\000No operation issued due to the backend\000event=0x24\000\00000\000No operation issued due to the backend\000 */
{ 46706 }, /* stall_frontend\000pipeline\000No operation issued because of the frontend\000event=0x23\000\00000\000No operation issued because of the frontend\000 */
{ 73751 }, /* strex_fail_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX fail\000event=0x6e\000\00000\000Exclusive operation speculatively executed, STREX or STX fail\000 */
{ 73584 }, /* strex_pass_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX pass\000event=0x6d\000\00000\000Exclusive operation speculatively executed, STREX or STX pass\000 */
{ 88779 }, /* strex_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX\000event=0x6f\000\00000\000Exclusive operation speculatively executed, STREX or STX\000 */
{ 21286 }, /* sw_incr\000instruction\000Instruction architecturally executed, Condition code check pass, software increment\000event=0\000\00000\000Instruction architecturally executed, Condition code check pass, software increment\000 */
{ 91512 }, /* tlb_access\000cache\000Level 2 TLB access\000event=0xea\000\00000\000Level 2 TLB access\000 */
{ 24814 }, /* ttbr_write_retired\000instruction\000Instruction architecturally executed, Condition code check pass, write to TTBR\000event=0x1c\000\00000\000Instruction architecturally executed, Condition code check pass, write to TTBR\000 */
{ 26808 }, /* unaligned_ldst_spec\000memory\000Unaligned access\000event=0x6a\000\00000\000Unaligned access\000 */
{ 91444 }, /* utlb_flush\000cache\000Level 1 TLB flush\000event=0xe9\000\00000\000Level 1 TLB flush\000 */
{ 22650 }, /* vfp_spec\000instruction\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Operation speculatively executed, floating-point instruction\000 */

};

const struct pmu_table_entry pmu_events__arm_cortex_a75[] = {
{
     .entries = pmu_events__arm_cortex_a75_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__arm_cortex_a75_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__arm_cortex_a76_default_core[] = {
{ 22494 }, /* ase_spec\000instruction\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Operation speculatively executed, Advanced SIMD instruction\000 */
{ 105418 }, /* br_immed_spec\000instruction\000Branch speculatively executed, immediate branch\000event=0x78\000\00000\000Branch speculatively executed, immediate branch\000 */
{ 105555 }, /* br_indirect_spec\000instruction\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000 */
{ 99023 }, /* br_mis_pred\000branch\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000This event counts any predictable branch instruction which is mispredicted either due to dynamic misprediction or because the MMU is off and the branches are statically predicted not taken\000 */
{ 32593 }, /* br_mis_pred_retired\000instruction\000Instruction architecturally executed, mispredicted branch\000event=0x22\000\00000\000This event counts any branch counted by BR_RETIRED which is not correctly predicted and causes a pipeline flush\000 */
{ 99306 }, /* br_pred\000branch\000Predictable branch speculatively executed\000event=0x12\000\00000\000This event counts all predictable branches\000 */
{ 32397 }, /* br_retired\000instruction\000Instruction architecturally executed, branch\000event=0x21\000\00000\000This event counts all branches, taken or not. This excludes exception entries, debug entries and CCFAIL branches\000 */
{ 105693 }, /* br_return_spec\000instruction\000Branch speculatively executed, procedure return\000event=0x79\000\00000\000Branch speculatively executed, procedure return\000 */
{ 99515 }, /* bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000This event counts for every beat of data transferred over the data channels between the core and the SCU. If both read and write data beats are transferred on a given cycle, this event is counted twice on that cycle. This event counts the sum of BUS_ACCESS_RD and BUS_ACCESS_WR\000 */
{ 771 }, /* bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Bus access read\000 */
{ 836 }, /* bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Bus access write\000 */
{ 99847 }, /* bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000This event duplicates CPU_CYCLES\000 */
{ 104946 }, /* cid_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000This event only counts writes to CONTEXTIDR in AArch32 state, and via the CONTEXTIDR_EL1 mnemonic in AArch64 state\000 */
{ 99421 }, /* cpu_cycles\000bus\000The number of core clock cycles\000event=0x11\000\00000\000The number of core clock cycles\000 */
{ 23330 }, /* crypto_spec\000instruction\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Operation speculatively executed, Cryptographic instruction\000 */
{ 23705 }, /* dmb_spec\000instruction\000Barrier speculatively executed, DMB\000event=0x7e\000\00000\000Barrier speculatively executed, DMB\000 */
{ 22343 }, /* dp_spec\000instruction\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Operation speculatively executed, integer data processing\000 */
{ 23597 }, /* dsb_spec\000instruction\000Barrier speculatively executed, DSB\000event=0x7d\000\00000\000Barrier speculatively executed, DSB\000 */
{ 104283 }, /* dtlb_walk\000cache\000Access to data TLB causes a translation table walk\000event=0x34\000\00000\000This event counts on any data access which causes L2D_TLB_REFILL to count\000 */
{ 20002 }, /* exc_dabort\000exception\000Exception taken, Data Abort and SError\000event=0x84\000\00000\000Exception taken, Data Abort and SError\000 */
{ 20191 }, /* exc_fiq\000exception\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000 */
{ 20266 }, /* exc_hvc\000exception\000Exception taken, Hypervisor Call\000event=0x8a\000\00000\000Exception taken, Hypervisor Call\000 */
{ 20116 }, /* exc_irq\000exception\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000 */
{ 19896 }, /* exc_pabort\000exception\000Exception taken, Instruction Abort\000event=0x83\000\00000\000Exception taken, Instruction Abort\000 */
{ 33889 }, /* exc_return\000instruction\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Instruction architecturally executed, condition check pass, exception return\000 */
{ 73325 }, /* exc_smc\000exception\000Exception taken, Secure Monitor Call\000event=0x88\000\00000\000Exception taken, Secure Monitor Call\000 */
{ 19797 }, /* exc_svc\000exception\000Exception taken, Supervisor Call\000event=0x82\000\00000\000Exception taken, Supervisor Call\000 */
{ 21031 }, /* exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Exception taken\000 */
{ 20512 }, /* exc_trap_dabort\000exception\000Exception taken, Data Abort or SError not taken locally\000event=0x8c\000\00000\000Exception taken, Data Abort or SError not taken locally\000 */
{ 20915 }, /* exc_trap_fiq\000exception\000Exception taken, FIQ not taken locally\000event=0x8f\000\00000\000Exception taken, FIQ not taken locally\000 */
{ 20799 }, /* exc_trap_irq\000exception\000Exception taken, IRQ not taken locally\000event=0x8e\000\00000\000Exception taken, IRQ not taken locally\000 */
{ 20665 }, /* exc_trap_other\000exception\000Exception taken, Other traps not taken locally\000event=0x8d\000\00000\000Exception taken, Other traps not taken locally\000 */
{ 20365 }, /* exc_trap_pabort\000exception\000Exception taken, Instruction Abort not taken locally\000event=0x8b\000\00000\000Exception taken, Instruction Abort not taken locally\000 */
{ 19692 }, /* exc_undef\000exception\000Exception taken, Other synchronous\000event=0x81\000\00000\000Exception taken, Other synchronous\000 */
{ 104778 }, /* inst_retired\000instruction\000Instruction architecturally executed\000event=0x8\000\00000\000This event counts all retired instructions, including those that fail their condition check\000 */
{ 24711 }, /* inst_spec\000instruction\000Operation speculatively executed\000event=0x1b\000\00000\000Operation speculatively executed\000 */
{ 23489 }, /* isb_spec\000instruction\000Barrier speculatively executed, ISB\000event=0x7c\000\00000\000Barrier speculatively executed, ISB\000 */
{ 104439 }, /* itlb_walk\000cache\000Access to instruction TLB that causes a translation table walk\000event=0x35\000\00000\000This event counts on any instruction access which causes L2D_TLB_REFILL to count\000 */
{ 100498 }, /* l1d_cache\000cache\000Level 1 data cache access\000event=0x4\000\00000\000This event counts any load or store operation or page table walk access which looks up in the L1 data cache. In particular, any access which could count the L1D_CACHE_REFILL event causes this event to count\000 */
{ 1902 }, /* l1d_cache_inval\000cache\000L1D cache invalidate\000event=0x48\000\00000\000Attributable Level 1 data cache invalidate\000 */
{ 1587 }, /* l1d_cache_rd\000cache\000L1D cache access, read\000event=0x40\000\00000\000Attributable Level 1 data cache access, read\000 */
{ 100264 }, /* l1d_cache_refill\000cache\000Level 1 data cache refill\000event=0x3\000\00000\000This event counts any load or store operation or page table walk access which causes data to be read from outside the L1, including accesses which do not allocate into L1\000 */
{ 39214 }, /* l1d_cache_refill_inner\000cache\000L1D cache refill, inner\000event=0x44\000\00000\000Attributable Level 1 data cache refill, inner\000 */
{ 39328 }, /* l1d_cache_refill_outer\000cache\000L1D cache refill, outer\000event=0x45\000\00000\000Attributable Level 1 data cache refill, outer\000 */
{ 1793 }, /* l1d_cache_refill_rd\000cache\000L1D cache refill, read\000event=0x42\000\00000\000Attributable Level 1 data cache refill, read\000 */
{ 39103 }, /* l1d_cache_refill_wr\000cache\000L1D cache refill, write\000event=0x43\000\00000\000Attributable Level 1 data cache refill, write\000 */
{ 101200 }, /* l1d_cache_wb\000cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000This event counts any write-back of data from the L1 data cache to L2 or L3. This counts both victim line evictions and snoops, including cache maintenance operations\000 */
{ 73186 }, /* l1d_cache_wb_clean\000cache\000L1D cache Write-Back, cleaning and coherency\000event=0x47\000\00000\000Level 1 data cache Write-Back, cleaning and coherency\000 */
{ 73065 }, /* l1d_cache_wb_victim\000cache\000L1D cache Write-Back, victim\000event=0x46\000\00000\000Attributable Level 1 data cache Write-Back, victim\000 */
{ 1689 }, /* l1d_cache_wr\000cache\000L1D cache access, write\000event=0x41\000\00000\000Attributable Level 1 data cache access, write\000 */
{ 102602 }, /* l1d_tlb\000cache\000Level 1 data TLB access\000event=0x25\000\00000\000This event counts any load or store operation which accesses the data L1 TLB. If both a load and a store are executed on a cycle, this event counts twice. This event counts regardless of whether the MMU is enabled\000 */
{ 88109 }, /* l1d_tlb_rd\000cache\000L1D tlb access, read\000event=0x4e\000\00000\000Attributable Level 1 data or unified TLB access, read\000 */
{ 100761 }, /* l1d_tlb_refill\000cache\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000This event counts any refill of the data L1 TLB from the L2 TLB. This includes refills that result in a translation fault\000 */
{ 2003 }, /* l1d_tlb_refill_rd\000cache\000L1D tlb refill, read\000event=0x4c\000\00000\000Attributable Level 1 data TLB refill, read\000 */
{ 2106 }, /* l1d_tlb_refill_wr\000cache\000L1D tlb refill, write\000event=0x4d\000\00000\000Attributable Level 1 data TLB refill, write\000 */
{ 88216 }, /* l1d_tlb_wr\000cache\000L1D tlb access, write\000event=0x4f\000\00000\000Attributable Level 1 data or unified TLB access, write\000 */
{ 100955 }, /* l1i_cache\000cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Level 1 instruction cache access or Level 0 Macro-op cache access. This event counts any instruction fetch which accesses the L1 instruction cache or L0 Macro-op cache\000 */
{ 99920 }, /* l1i_cache_refill\000cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000This event counts any instruction fetch which misses in the cache\000 */
{ 102869 }, /* l1i_tlb\000cache\000Level 1 instruction TLB access\000event=0x26\000\00000\000This event counts any instruction fetch which accesses the instruction L1 TLB.This event counts regardless of whether the MMU is enabled\000 */
{ 100056 }, /* l1i_tlb_refill\000cache\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000This event counts any refill of the instruction L1 TLB from the L2 TLB. This includes refills that result in a translation fault\000 */
{ 101444 }, /* l2d_cache\000cache\000Level 2 data cache access\000event=0x16\000\00000\000This event counts any transaction from L1 which looks up in the L2 cache, and any write-back from the L1 to the L2. Snoops from outside the core and cache maintenance operations are not counted\000 */
{ 102322 }, /* l2d_cache_allocate\000cache\000Attributable Level 2 data cache allocation without refill\000event=0x20\000\00000\000This event counts any full cache line write into the L2 cache which does not cause a linefill, including write-backs from L1 to L2 and full-line writes which do not allocate into L1\000 */
{ 2897 }, /* l2d_cache_inval\000cache\000L2D cache invalidate\000event=0x58\000\00000\000Attributable Level 2 data cache invalidate\000 */
{ 2211 }, /* l2d_cache_rd\000cache\000L2D cache access, read\000event=0x50\000\00000\000Attributable Level 2 data cache access, read\000 */
{ 101695 }, /* l2d_cache_refill\000cache\000Level 2 data refill\000event=0x17\000\00000\000L2 data cache refill. This event counts any cacheable transaction from L1 which causes data to be read from outside the core. L2 refills caused by stashes into L2 should not be counted\000 */
{ 2417 }, /* l2d_cache_refill_rd\000cache\000L2D cache refill, read\000event=0x52\000\00000\000Attributable Level 2 data cache refill, read\000 */
{ 2526 }, /* l2d_cache_refill_wr\000cache\000L2D cache refill, write\000event=0x53\000\00000\000Attributable Level 2 data cache refill, write\000 */
{ 101938 }, /* l2d_cache_wb\000cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000This event counts any write-back of data from the L2 cache to outside the core. This includes snoops to the L2 which return data, regardless of whether they cause an invalidation. Invalidations from the L2 which do not write data outside of the core and snoops which return data from the L1 are not counted\000 */
{ 2758 }, /* l2d_cache_wb_clean\000cache\000L2D cache Write-Back, cleaning and coherency\000event=0x57\000\00000\000Level 2 data cache Write-Back, cleaning and coherency\000 */
{ 2637 }, /* l2d_cache_wb_victim\000cache\000L2D cache Write-Back, victim\000event=0x56\000\00000\000Attributable Level 2 data cache Write-Back, victim\000 */
{ 2313 }, /* l2d_cache_wr\000cache\000L2D cache access, write\000event=0x51\000\00000\000Attributable Level 2 data cache access, write\000 */
{ 104067 }, /* l2d_tlb\000cache\000Attributable Level 2 data or unified TLB access\000event=0x2f\000\00000\000This event counts on any access to the L2 TLB (caused by a refill of any of the L1 TLBs). This event does not count if the MMU is disabled\000 */
{ 88559 }, /* l2d_tlb_rd\000cache\000L2D cache access, read\000event=0x5e\000\00000\000Attributable Level 2 data or unified TLB access, read\000 */
{ 103846 }, /* l2d_tlb_refill\000cache\000Attributable L2 data or unified TLB refill\000event=0x2d\000\00000\000This event counts on anyrefill of the L2 TLB, caused by either an instruction or data access.This event does not count if the MMU is disabled\000 */
{ 88325 }, /* l2d_tlb_refill_rd\000cache\000L2D cache refill, read\000event=0x5c\000\00000\000Attributable Level 2 data or unified TLB refill, read\000 */
{ 88441 }, /* l2d_tlb_refill_wr\000cache\000L2D cache refill, write\000event=0x5d\000\00000\000Attributable Level 2 data or unified TLB refill, write\000 */
{ 88668 }, /* l2d_tlb_wr\000cache\000L2D cache access, write\000event=0x5f\000\00000\000Attributable Level 2 data or unified TLB access, write\000 */
{ 103654 }, /* l3d_cache\000cache\000Attributable Level 3 unified cache access\000event=0x2b\000\00000\000This event counts for any cacheable read transaction returning datafrom the SCU, or for any cacheable write to the SCU\000 */
{ 103066 }, /* l3d_cache_allocate\000cache\000Allocation without refill\000event=0x29\000\00000\000This event counts any full cache line write into the L3 cache which does not cause a linefill, including write-backs from L2 to L3 and full-line writes which do not allocate into L2\000 */
{ 39442 }, /* l3d_cache_rd\000cache\000Attributable Level 3 data or unified cache access, read\000event=0xa0\000\00000\000Attributable Level 3 data or unified cache access, read\000 */
{ 103314 }, /* l3d_cache_refill\000cache\000Attributable Level 3 unified cache refill\000event=0x2a\000\00000\000This event counts for any cacheable read transaction returning datafrom the SCU for which the data source was outside the cluster. Transactions such as ReadUnique are counted here as 'read' transactions, even though they can be generated by store instructions\000 */
{ 21982 }, /* ld_spec\000instruction\000Operation speculatively executed, load\000event=0x70\000\00000\000Operation speculatively executed, load\000 */
{ 73432 }, /* ldrex_spec\000instruction\000Exclusive operation speculatively executed, LDREX or LDX\000event=0x6c\000\00000\000Exclusive operation speculatively executed, LDREX or LDX\000 */
{ 22210 }, /* ldst_spec\000instruction\000Operation speculatively executed, load or store\000event=0x72\000\00000\000Operation speculatively executed, load or store\000 */
{ 39009 }, /* ll_cache_miss_rd\000cache\000Last level cache miss, read\000event=0x37\000\00000\000Last level cache miss, read\000 */
{ 38892 }, /* ll_cache_rd\000cache\000Attributable Last level cache memory read\000event=0x36\000\00000\000Attributable Last level cache memory read\000 */
{ 105831 }, /* mem_access\000memory\000Data memory access\000event=0x13\000\00000\000This event counts memory accesses due to load or store instructions. This event counts the sum of MEM_ACCESS_RD and MEM_ACCESS_WR\000 */
{ 26460 }, /* mem_access_rd\000memory\000Data memory access, read\000event=0x66\000\00000\000Data memory access, read\000 */
{ 26546 }, /* mem_access_wr\000memory\000Data memory access, write\000event=0x67\000\00000\000Data memory access, write\000 */
{ 104614 }, /* memory_error\000exception\000Local memory error\000event=0x1a\000\00000\000This event counts any correctable or uncorrectable memory error (ECC or parity) in the protected core RAMs\000 */
{ 22808 }, /* pc_write_spec\000instruction\000Operation speculatively executed, software change of the PC\000event=0x76\000\00000\000Operation speculatively executed, software change of the PC\000 */
{ 23813 }, /* rc_ld_spec\000instruction\000Release consistency operation speculatively executed, Load-Acquire\000event=0x90\000\00000\000Release consistency operation speculatively executed, Load-Acquire\000 */
{ 23985 }, /* rc_st_spec\000instruction\000Release consistency operation speculatively executed, Store-Release\000event=0x91\000\00000\000Release consistency operation speculatively executed, Store-Release\000 */
{ 90717 }, /* remote_access\000memory\000Access to another socket in a multi-socket system\000event=0x31\000\00000\000Access to another socket in a multi-socket system\000 */
{ 22095 }, /* st_spec\000instruction\000Operation speculatively executed, store\000event=0x71\000\00000\000Operation speculatively executed, store\000 */
{ 106189 }, /* stall_backend\000pipeline\000No operation issued due to the backend\000event=0x24\000\00000\000The counter counts on any cycle fetched instructions are not dispatched due to resource constraints\000 */
{ 106013 }, /* stall_frontend\000pipeline\000No operation issued because of the frontend\000event=0x23\000\00000\000The counter counts on any cycle when there are no fetched instructions available to dispatch\000 */
{ 73751 }, /* strex_fail_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX fail\000event=0x6e\000\00000\000Exclusive operation speculatively executed, STREX or STX fail\000 */
{ 73584 }, /* strex_pass_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX pass\000event=0x6d\000\00000\000Exclusive operation speculatively executed, STREX or STX pass\000 */
{ 88779 }, /* strex_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX\000event=0x6f\000\00000\000Exclusive operation speculatively executed, STREX or STX\000 */
{ 21286 }, /* sw_incr\000instruction\000Instruction architecturally executed, Condition code check pass, software increment\000event=0\000\00000\000Instruction architecturally executed, Condition code check pass, software increment\000 */
{ 105190 }, /* ttbr_write_retired\000instruction\000Instruction architecturally executed, Condition code check pass, write to TTBR\000event=0x1c\000\00000\000This event only counts writes to TTBR0/TTBR1 in AArch32 state and TTBR0_EL1/TTBR1_EL1 in AArch64 state\000 */
{ 26634 }, /* unaligned_ld_spec\000memory\000Unaligned access, read\000event=0x68\000\00000\000Unaligned access, read\000 */
{ 26808 }, /* unaligned_ldst_spec\000memory\000Unaligned access\000event=0x6a\000\00000\000Unaligned access\000 */
{ 26720 }, /* unaligned_st_spec\000memory\000Unaligned access, write\000event=0x69\000\00000\000Unaligned access, write\000 */
{ 22650 }, /* vfp_spec\000instruction\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Operation speculatively executed, floating-point instruction\000 */

};

const struct pmu_table_entry pmu_events__arm_cortex_a76[] = {
{
     .entries = pmu_events__arm_cortex_a76_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__arm_cortex_a76_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__arm_cortex_a77_default_core[] = {
{ 22494 }, /* ase_spec\000instruction\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Operation speculatively executed, Advanced SIMD instruction\000 */
{ 13 }, /* br_immed_spec\000branch\000Branch speculatively executed, immediate branch\000event=0x78\000\00000\000Branch speculatively executed, immediate branch\000 */
{ 278 }, /* br_indirect_spec\000branch\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000 */
{ 411 }, /* br_mis_pred\000branch\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000Mispredicted or not predicted branch speculatively executed\000 */
{ 25146 }, /* br_mis_pred_retired\000instruction\000Instruction architecturally executed, mispredicted branch\000event=0x22\000\00000\000Instruction architecturally executed, mispredicted branch\000 */
{ 565 }, /* br_pred\000branch\000Predictable branch speculatively executed\000event=0x12\000\00000\000Predictable branch speculatively executed\000 */
{ 25018 }, /* br_retired\000instruction\000Instruction architecturally executed, branch\000event=0x21\000\00000\000Instruction architecturally executed, branch\000 */
{ 145 }, /* br_return_spec\000branch\000Branch speculatively executed, procedure return\000event=0x79\000\00000\000Branch speculatively executed, procedure return\000 */
{ 1311 }, /* bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000Attributable Bus access\000 */
{ 771 }, /* bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Bus access read\000 */
{ 836 }, /* bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Bus access write\000 */
{ 721 }, /* bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000Bus cycle\000 */
{ 24272 }, /* cid_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000 */
{ 679 }, /* cpu_cycles\000bus\000Cycle\000event=0x11\000\00000\000Cycle\000 */
{ 23330 }, /* crypto_spec\000instruction\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Operation speculatively executed, Cryptographic instruction\000 */
{ 23705 }, /* dmb_spec\000instruction\000Barrier speculatively executed, DMB\000event=0x7e\000\00000\000Barrier speculatively executed, DMB\000 */
{ 22343 }, /* dp_spec\000instruction\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Operation speculatively executed, integer data processing\000 */
{ 23597 }, /* dsb_spec\000instruction\000Barrier speculatively executed, DSB\000event=0x7d\000\00000\000Barrier speculatively executed, DSB\000 */
{ 4626 }, /* dtlb_walk\000cache\000Access to data TLB causes a translation table walk\000event=0x34\000\00000\000Access to data TLB causes a translation table walk\000 */
{ 20002 }, /* exc_dabort\000exception\000Exception taken, Data Abort and SError\000event=0x84\000\00000\000Exception taken, Data Abort and SError\000 */
{ 20191 }, /* exc_fiq\000exception\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000 */
{ 20266 }, /* exc_hvc\000exception\000Exception taken, Hypervisor Call\000event=0x8a\000\00000\000Exception taken, Hypervisor Call\000 */
{ 20116 }, /* exc_irq\000exception\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000 */
{ 19896 }, /* exc_pabort\000exception\000Exception taken, Instruction Abort\000event=0x83\000\00000\000Exception taken, Instruction Abort\000 */
{ 33889 }, /* exc_return\000instruction\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Instruction architecturally executed, condition check pass, exception return\000 */
{ 73325 }, /* exc_smc\000exception\000Exception taken, Secure Monitor Call\000event=0x88\000\00000\000Exception taken, Secure Monitor Call\000 */
{ 19797 }, /* exc_svc\000exception\000Exception taken, Supervisor Call\000event=0x82\000\00000\000Exception taken, Supervisor Call\000 */
{ 21031 }, /* exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Exception taken\000 */
{ 20512 }, /* exc_trap_dabort\000exception\000Exception taken, Data Abort or SError not taken locally\000event=0x8c\000\00000\000Exception taken, Data Abort or SError not taken locally\000 */
{ 20915 }, /* exc_trap_fiq\000exception\000Exception taken, FIQ not taken locally\000event=0x8f\000\00000\000Exception taken, FIQ not taken locally\000 */
{ 20799 }, /* exc_trap_irq\000exception\000Exception taken, IRQ not taken locally\000event=0x8e\000\00000\000Exception taken, IRQ not taken locally\000 */
{ 20665 }, /* exc_trap_other\000exception\000Exception taken, Other traps not taken locally\000event=0x8d\000\00000\000Exception taken, Other traps not taken locally\000 */
{ 20365 }, /* exc_trap_pabort\000exception\000Exception taken, Instruction Abort not taken locally\000event=0x8b\000\00000\000Exception taken, Instruction Abort not taken locally\000 */
{ 19692 }, /* exc_undef\000exception\000Exception taken, Other synchronous\000event=0x81\000\00000\000Exception taken, Other synchronous\000 */
{ 24159 }, /* inst_retired\000instruction\000Instruction architecturally executed\000event=0x8\000\00000\000Instruction architecturally executed\000 */
{ 24711 }, /* inst_spec\000instruction\000Operation speculatively executed\000event=0x1b\000\00000\000Operation speculatively executed\000 */
{ 23489 }, /* isb_spec\000instruction\000Barrier speculatively executed, ISB\000event=0x7c\000\00000\000Barrier speculatively executed, ISB\000 */
{ 4759 }, /* itlb_walk\000cache\000Access to instruction TLB that causes a translation table walk\000event=0x35\000\00000\000Access to instruction TLB that causes a translation table walk\000 */
{ 3313 }, /* l1d_cache\000cache\000Level 1 data cache access\000event=0x4\000\00000\000Level 1 data cache access\000 */
{ 1902 }, /* l1d_cache_inval\000cache\000L1D cache invalidate\000event=0x48\000\00000\000Attributable Level 1 data cache invalidate\000 */
{ 1587 }, /* l1d_cache_rd\000cache\000L1D cache access, read\000event=0x40\000\00000\000Attributable Level 1 data cache access, read\000 */
{ 3224 }, /* l1d_cache_refill\000cache\000Level 1 data cache refill\000event=0x3\000\00000\000Level 1 data cache refill\000 */
{ 39214 }, /* l1d_cache_refill_inner\000cache\000L1D cache refill, inner\000event=0x44\000\00000\000Attributable Level 1 data cache refill, inner\000 */
{ 39328 }, /* l1d_cache_refill_outer\000cache\000L1D cache refill, outer\000event=0x45\000\00000\000Attributable Level 1 data cache refill, outer\000 */
{ 1793 }, /* l1d_cache_refill_rd\000cache\000L1D cache refill, read\000event=0x42\000\00000\000Attributable Level 1 data cache refill, read\000 */
{ 39103 }, /* l1d_cache_refill_wr\000cache\000L1D cache refill, write\000event=0x43\000\00000\000Attributable Level 1 data cache refill, write\000 */
{ 33516 }, /* l1d_cache_wb\000cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000Attributable Level 1 data cache write-back\000 */
{ 73186 }, /* l1d_cache_wb_clean\000cache\000L1D cache Write-Back, cleaning and coherency\000event=0x47\000\00000\000Level 1 data cache Write-Back, cleaning and coherency\000 */
{ 73065 }, /* l1d_cache_wb_victim\000cache\000L1D cache Write-Back, victim\000event=0x46\000\00000\000Attributable Level 1 data cache Write-Back, victim\000 */
{ 1689 }, /* l1d_cache_wr\000cache\000L1D cache access, write\000event=0x41\000\00000\000Attributable Level 1 data cache access, write\000 */
{ 3908 }, /* l1d_tlb\000cache\000Attributable Level 1 data or unified TLB access\000event=0x25\000\00000\000Attributable Level 1 data or unified TLB access\000 */
{ 88109 }, /* l1d_tlb_rd\000cache\000L1D tlb access, read\000event=0x4e\000\00000\000Attributable Level 1 data or unified TLB access, read\000 */
{ 3395 }, /* l1d_tlb_refill\000cache\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Attributable Level 1 data TLB refill\000 */
{ 2003 }, /* l1d_tlb_refill_rd\000cache\000L1D tlb refill, read\000event=0x4c\000\00000\000Attributable Level 1 data TLB refill, read\000 */
{ 2106 }, /* l1d_tlb_refill_wr\000cache\000L1D tlb refill, write\000event=0x4d\000\00000\000Attributable Level 1 data TLB refill, write\000 */
{ 88216 }, /* l1d_tlb_wr\000cache\000L1D tlb access, write\000event=0x4f\000\00000\000Attributable Level 1 data or unified TLB access, write\000 */
{ 3504 }, /* l1i_cache\000cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Attributable Level 1 instruction cache access\000 */
{ 2998 }, /* l1i_cache_refill\000cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Level 1 instruction cache refill\000 */
{ 4033 }, /* l1i_tlb\000cache\000Attributable Level 1 instruction TLB access\000event=0x26\000\00000\000Attributable Level 1 instruction TLB access\000 */
{ 3101 }, /* l1i_tlb_refill\000cache\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Attributable Level 1 instruction TLB refill\000 */
{ 3627 }, /* l2d_cache\000cache\000Level 2 data cache access\000event=0x16\000\00000\000Level 2 data cache access\000 */
{ 38627 }, /* l2d_cache_allocate\000cache\000Attributable Level 2 data cache allocation without refill\000event=0x20\000\00000\000Attributable Level 2 data cache allocation without refill\000 */
{ 2897 }, /* l2d_cache_inval\000cache\000L2D cache invalidate\000event=0x58\000\00000\000Attributable Level 2 data cache invalidate\000 */
{ 2211 }, /* l2d_cache_rd\000cache\000L2D cache access, read\000event=0x50\000\00000\000Attributable Level 2 data cache access, read\000 */
{ 3710 }, /* l2d_cache_refill\000cache\000Level 2 data refill\000event=0x17\000\00000\000Level 2 data refill\000 */
{ 2417 }, /* l2d_cache_refill_rd\000cache\000L2D cache refill, read\000event=0x52\000\00000\000Attributable Level 2 data cache refill, read\000 */
{ 2526 }, /* l2d_cache_refill_wr\000cache\000L2D cache refill, write\000event=0x53\000\00000\000Attributable Level 2 data cache refill, write\000 */
{ 3788 }, /* l2d_cache_wb\000cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Attributable Level 2 data cache write-back\000 */
{ 2758 }, /* l2d_cache_wb_clean\000cache\000L2D cache Write-Back, cleaning and coherency\000event=0x57\000\00000\000Level 2 data cache Write-Back, cleaning and coherency\000 */
{ 2637 }, /* l2d_cache_wb_victim\000cache\000L2D cache Write-Back, victim\000event=0x56\000\00000\000Attributable Level 2 data cache Write-Back, victim\000 */
{ 2313 }, /* l2d_cache_wr\000cache\000L2D cache access, write\000event=0x51\000\00000\000Attributable Level 2 data cache access, write\000 */
{ 4384 }, /* l2d_tlb\000cache\000Attributable Level 2 data or unified TLB access\000event=0x2f\000\00000\000Attributable Level 2 data or unified TLB access\000 */
{ 88559 }, /* l2d_tlb_rd\000cache\000L2D cache access, read\000event=0x5e\000\00000\000Attributable Level 2 data or unified TLB access, read\000 */
{ 4150 }, /* l2d_tlb_refill\000cache\000Attributable Level 2 data TLB refill\000event=0x2d\000\00000\000Attributable Level 2 data TLB refill\000 */
{ 88325 }, /* l2d_tlb_refill_rd\000cache\000L2D cache refill, read\000event=0x5c\000\00000\000Attributable Level 2 data or unified TLB refill, read\000 */
{ 88441 }, /* l2d_tlb_refill_wr\000cache\000L2D cache refill, write\000event=0x5d\000\00000\000Attributable Level 2 data or unified TLB refill, write\000 */
{ 88668 }, /* l2d_tlb_wr\000cache\000L2D cache access, write\000event=0x5f\000\00000\000Attributable Level 2 data or unified TLB access, write\000 */
{ 38783 }, /* l3d_cache\000cache\000Attributable Level 3 data cache access\000event=0x2b\000\00000\000Attributable Level 3 data cache access\000 */
{ 61550 }, /* l3d_cache_allocate\000cache\000Attributable Level 3 data cache allocation without refill\000event=0x29\000\00000\000Attributable Level 3 data cache allocation without refill\000 */
{ 39442 }, /* l3d_cache_rd\000cache\000Attributable Level 3 data or unified cache access, read\000event=0xa0\000\00000\000Attributable Level 3 data or unified cache access, read\000 */
{ 61706 }, /* l3d_cache_refill\000cache\000Attributable Level 3 data cache refill\000event=0x2a\000\00000\000Attributable Level 3 data cache refill\000 */
{ 21982 }, /* ld_spec\000instruction\000Operation speculatively executed, load\000event=0x70\000\00000\000Operation speculatively executed, load\000 */
{ 73432 }, /* ldrex_spec\000instruction\000Exclusive operation speculatively executed, LDREX or LDX\000event=0x6c\000\00000\000Exclusive operation speculatively executed, LDREX or LDX\000 */
{ 22210 }, /* ldst_spec\000instruction\000Operation speculatively executed, load or store\000event=0x72\000\00000\000Operation speculatively executed, load or store\000 */
{ 39009 }, /* ll_cache_miss_rd\000cache\000Last level cache miss, read\000event=0x37\000\00000\000Last level cache miss, read\000 */
{ 38892 }, /* ll_cache_rd\000cache\000Attributable Last level cache memory read\000event=0x36\000\00000\000Attributable Last level cache memory read\000 */
{ 27136 }, /* mem_access\000memory\000Data memory access\000event=0x13\000\00000\000Data memory access\000 */
{ 26460 }, /* mem_access_rd\000memory\000Data memory access, read\000event=0x66\000\00000\000Data memory access, read\000 */
{ 26546 }, /* mem_access_wr\000memory\000Data memory access, write\000event=0x67\000\00000\000Data memory access, write\000 */
{ 33636 }, /* memory_error\000exception\000Local memory error\000event=0x1a\000\00000\000Local memory error\000 */
{ 22808 }, /* pc_write_spec\000instruction\000Operation speculatively executed, software change of the PC\000event=0x76\000\00000\000Operation speculatively executed, software change of the PC\000 */
{ 23813 }, /* rc_ld_spec\000instruction\000Release consistency operation speculatively executed, Load-Acquire\000event=0x90\000\00000\000Release consistency operation speculatively executed, Load-Acquire\000 */
{ 23985 }, /* rc_st_spec\000instruction\000Release consistency operation speculatively executed, Store-Release\000event=0x91\000\00000\000Release consistency operation speculatively executed, Store-Release\000 */
{ 90717 }, /* remote_access\000memory\000Access to another socket in a multi-socket system\000event=0x31\000\00000\000Access to another socket in a multi-socket system\000 */
{ 22095 }, /* st_spec\000instruction\000Operation speculatively executed, store\000event=0x71\000\00000\000Operation speculatively executed, store\000 */
{ 27892 }, /* stall_backend\000pipeline\000No operation issued due to the backend\000event=0x24\000\00000\000No operation issued due to the backend\000 */
{ 46706 }, /* stall_frontend\000pipeline\000No operation issued because of the frontend\000event=0x23\000\00000\000No operation issued because of the frontend\000 */
{ 73751 }, /* strex_fail_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX fail\000event=0x6e\000\00000\000Exclusive operation speculatively executed, STREX or STX fail\000 */
{ 73584 }, /* strex_pass_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX pass\000event=0x6d\000\00000\000Exclusive operation speculatively executed, STREX or STX pass\000 */
{ 88779 }, /* strex_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX\000event=0x6f\000\00000\000Exclusive operation speculatively executed, STREX or STX\000 */
{ 21286 }, /* sw_incr\000instruction\000Instruction architecturally executed, Condition code check pass, software increment\000event=0\000\00000\000Instruction architecturally executed, Condition code check pass, software increment\000 */
{ 24814 }, /* ttbr_write_retired\000instruction\000Instruction architecturally executed, Condition code check pass, write to TTBR\000event=0x1c\000\00000\000Instruction architecturally executed, Condition code check pass, write to TTBR\000 */
{ 26634 }, /* unaligned_ld_spec\000memory\000Unaligned access, read\000event=0x68\000\00000\000Unaligned access, read\000 */
{ 26808 }, /* unaligned_ldst_spec\000memory\000Unaligned access\000event=0x6a\000\00000\000Unaligned access\000 */
{ 26720 }, /* unaligned_st_spec\000memory\000Unaligned access, write\000event=0x69\000\00000\000Unaligned access, write\000 */
{ 22650 }, /* vfp_spec\000instruction\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Operation speculatively executed, floating-point instruction\000 */

};

const struct pmu_table_entry pmu_events__arm_cortex_a77[] = {
{
     .entries = pmu_events__arm_cortex_a77_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__arm_cortex_a77_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__arm_cortex_a78_default_core[] = {
{ 22494 }, /* ase_spec\000instruction\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Operation speculatively executed, Advanced SIMD instruction\000 */
{ 13 }, /* br_immed_spec\000branch\000Branch speculatively executed, immediate branch\000event=0x78\000\00000\000Branch speculatively executed, immediate branch\000 */
{ 278 }, /* br_indirect_spec\000branch\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000 */
{ 411 }, /* br_mis_pred\000branch\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000Mispredicted or not predicted branch speculatively executed\000 */
{ 25146 }, /* br_mis_pred_retired\000instruction\000Instruction architecturally executed, mispredicted branch\000event=0x22\000\00000\000Instruction architecturally executed, mispredicted branch\000 */
{ 565 }, /* br_pred\000branch\000Predictable branch speculatively executed\000event=0x12\000\00000\000Predictable branch speculatively executed\000 */
{ 25018 }, /* br_retired\000instruction\000Instruction architecturally executed, branch\000event=0x21\000\00000\000Instruction architecturally executed, branch\000 */
{ 145 }, /* br_return_spec\000branch\000Branch speculatively executed, procedure return\000event=0x79\000\00000\000Branch speculatively executed, procedure return\000 */
{ 1311 }, /* bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000Attributable Bus access\000 */
{ 771 }, /* bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Bus access read\000 */
{ 836 }, /* bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Bus access write\000 */
{ 721 }, /* bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000Bus cycle\000 */
{ 24272 }, /* cid_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000 */
{ 1389 }, /* cnt_cycles\000bus\000Constant frequency cycles\000event=0x4004\000\00000\000Constant frequency cycles.  The counter increments at a constant frequency equal to the rate of increment of the system counter, CNTPCT_EL0\000 */
{ 679 }, /* cpu_cycles\000bus\000Cycle\000event=0x11\000\00000\000Cycle\000 */
{ 23330 }, /* crypto_spec\000instruction\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Operation speculatively executed, Cryptographic instruction\000 */
{ 23705 }, /* dmb_spec\000instruction\000Barrier speculatively executed, DMB\000event=0x7e\000\00000\000Barrier speculatively executed, DMB\000 */
{ 22343 }, /* dp_spec\000instruction\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Operation speculatively executed, integer data processing\000 */
{ 23597 }, /* dsb_spec\000instruction\000Barrier speculatively executed, DSB\000event=0x7d\000\00000\000Barrier speculatively executed, DSB\000 */
{ 4626 }, /* dtlb_walk\000cache\000Access to data TLB causes a translation table walk\000event=0x34\000\00000\000Access to data TLB causes a translation table walk\000 */
{ 20002 }, /* exc_dabort\000exception\000Exception taken, Data Abort and SError\000event=0x84\000\00000\000Exception taken, Data Abort and SError\000 */
{ 20191 }, /* exc_fiq\000exception\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000 */
{ 20266 }, /* exc_hvc\000exception\000Exception taken, Hypervisor Call\000event=0x8a\000\00000\000Exception taken, Hypervisor Call\000 */
{ 20116 }, /* exc_irq\000exception\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000 */
{ 19896 }, /* exc_pabort\000exception\000Exception taken, Instruction Abort\000event=0x83\000\00000\000Exception taken, Instruction Abort\000 */
{ 33889 }, /* exc_return\000instruction\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Instruction architecturally executed, condition check pass, exception return\000 */
{ 73325 }, /* exc_smc\000exception\000Exception taken, Secure Monitor Call\000event=0x88\000\00000\000Exception taken, Secure Monitor Call\000 */
{ 19797 }, /* exc_svc\000exception\000Exception taken, Supervisor Call\000event=0x82\000\00000\000Exception taken, Supervisor Call\000 */
{ 21031 }, /* exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Exception taken\000 */
{ 20512 }, /* exc_trap_dabort\000exception\000Exception taken, Data Abort or SError not taken locally\000event=0x8c\000\00000\000Exception taken, Data Abort or SError not taken locally\000 */
{ 20915 }, /* exc_trap_fiq\000exception\000Exception taken, FIQ not taken locally\000event=0x8f\000\00000\000Exception taken, FIQ not taken locally\000 */
{ 20799 }, /* exc_trap_irq\000exception\000Exception taken, IRQ not taken locally\000event=0x8e\000\00000\000Exception taken, IRQ not taken locally\000 */
{ 20665 }, /* exc_trap_other\000exception\000Exception taken, Other traps not taken locally\000event=0x8d\000\00000\000Exception taken, Other traps not taken locally\000 */
{ 20365 }, /* exc_trap_pabort\000exception\000Exception taken, Instruction Abort not taken locally\000event=0x8b\000\00000\000Exception taken, Instruction Abort not taken locally\000 */
{ 19692 }, /* exc_undef\000exception\000Exception taken, Other synchronous\000event=0x81\000\00000\000Exception taken, Other synchronous\000 */
{ 24159 }, /* inst_retired\000instruction\000Instruction architecturally executed\000event=0x8\000\00000\000Instruction architecturally executed\000 */
{ 24711 }, /* inst_spec\000instruction\000Operation speculatively executed\000event=0x1b\000\00000\000Operation speculatively executed\000 */
{ 23489 }, /* isb_spec\000instruction\000Barrier speculatively executed, ISB\000event=0x7c\000\00000\000Barrier speculatively executed, ISB\000 */
{ 4759 }, /* itlb_walk\000cache\000Access to instruction TLB that causes a translation table walk\000event=0x35\000\00000\000Access to instruction TLB that causes a translation table walk\000 */
{ 3313 }, /* l1d_cache\000cache\000Level 1 data cache access\000event=0x4\000\00000\000Level 1 data cache access\000 */
{ 1902 }, /* l1d_cache_inval\000cache\000L1D cache invalidate\000event=0x48\000\00000\000Attributable Level 1 data cache invalidate\000 */
{ 4916 }, /* l1d_cache_lmiss_rd\000cache\000Level 1 data cache long-latency read miss\000event=0x39\000\00000\000Level 1 data cache long-latency read miss.  The counter counts each memory read access counted by L1D_CACHE that incurs additional latency because it returns data from outside the Level 1 data or unified cache of this processing element\000 */
{ 1587 }, /* l1d_cache_rd\000cache\000L1D cache access, read\000event=0x40\000\00000\000Attributable Level 1 data cache access, read\000 */
{ 3224 }, /* l1d_cache_refill\000cache\000Level 1 data cache refill\000event=0x3\000\00000\000Level 1 data cache refill\000 */
{ 39214 }, /* l1d_cache_refill_inner\000cache\000L1D cache refill, inner\000event=0x44\000\00000\000Attributable Level 1 data cache refill, inner\000 */
{ 39328 }, /* l1d_cache_refill_outer\000cache\000L1D cache refill, outer\000event=0x45\000\00000\000Attributable Level 1 data cache refill, outer\000 */
{ 1793 }, /* l1d_cache_refill_rd\000cache\000L1D cache refill, read\000event=0x42\000\00000\000Attributable Level 1 data cache refill, read\000 */
{ 39103 }, /* l1d_cache_refill_wr\000cache\000L1D cache refill, write\000event=0x43\000\00000\000Attributable Level 1 data cache refill, write\000 */
{ 33516 }, /* l1d_cache_wb\000cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000Attributable Level 1 data cache write-back\000 */
{ 73186 }, /* l1d_cache_wb_clean\000cache\000L1D cache Write-Back, cleaning and coherency\000event=0x47\000\00000\000Level 1 data cache Write-Back, cleaning and coherency\000 */
{ 73065 }, /* l1d_cache_wb_victim\000cache\000L1D cache Write-Back, victim\000event=0x46\000\00000\000Attributable Level 1 data cache Write-Back, victim\000 */
{ 1689 }, /* l1d_cache_wr\000cache\000L1D cache access, write\000event=0x41\000\00000\000Attributable Level 1 data cache access, write\000 */
{ 3908 }, /* l1d_tlb\000cache\000Attributable Level 1 data or unified TLB access\000event=0x25\000\00000\000Attributable Level 1 data or unified TLB access\000 */
{ 88109 }, /* l1d_tlb_rd\000cache\000L1D tlb access, read\000event=0x4e\000\00000\000Attributable Level 1 data or unified TLB access, read\000 */
{ 3395 }, /* l1d_tlb_refill\000cache\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Attributable Level 1 data TLB refill\000 */
{ 2003 }, /* l1d_tlb_refill_rd\000cache\000L1D tlb refill, read\000event=0x4c\000\00000\000Attributable Level 1 data TLB refill, read\000 */
{ 2106 }, /* l1d_tlb_refill_wr\000cache\000L1D tlb refill, write\000event=0x4d\000\00000\000Attributable Level 1 data TLB refill, write\000 */
{ 88216 }, /* l1d_tlb_wr\000cache\000L1D tlb access, write\000event=0x4f\000\00000\000Attributable Level 1 data or unified TLB access, write\000 */
{ 3504 }, /* l1i_cache\000cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Attributable Level 1 instruction cache access\000 */
{ 5235 }, /* l1i_cache_lmiss\000cache\000Level 1 instruction cache long-latency read miss\000event=0x4006\000\00000\000Level 1 instruction cache long-latency read miss.  If the L1I_CACHE_RD event is implemented, the counter counts each access counted by L1I_CACHE_RD that incurs additional latency because it returns instructions from outside of the Level 1 instruction cache of this PE.  If the L1I_CACHE_RD event is not implemented, the counter counts each access counted by L1I_CACHE that incurs additional latency because it returns instructions from outside the Level 1 instruction cache of this PE.  The event indicates to software that the access missed in the Level 1 instruction cache and might have a significant performance impact due to the additional latency, compared to the latency of an access that hits in the Level 1 instruction cache\000 */
{ 2998 }, /* l1i_cache_refill\000cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Level 1 instruction cache refill\000 */
{ 4033 }, /* l1i_tlb\000cache\000Attributable Level 1 instruction TLB access\000event=0x26\000\00000\000Attributable Level 1 instruction TLB access\000 */
{ 3101 }, /* l1i_tlb_refill\000cache\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Attributable Level 1 instruction TLB refill\000 */
{ 3627 }, /* l2d_cache\000cache\000Level 2 data cache access\000event=0x16\000\00000\000Level 2 data cache access\000 */
{ 38627 }, /* l2d_cache_allocate\000cache\000Attributable Level 2 data cache allocation without refill\000event=0x20\000\00000\000Attributable Level 2 data cache allocation without refill\000 */
{ 2897 }, /* l2d_cache_inval\000cache\000L2D cache invalidate\000event=0x58\000\00000\000Attributable Level 2 data cache invalidate\000 */
{ 6057 }, /* l2d_cache_lmiss_rd\000cache\000Level 2 data cache long-latency read miss\000event=0x4009\000\00000\000Level 2 data cache long-latency read miss.  The counter counts each memory read access counted by L2D_CACHE that incurs additional latency because it returns data from outside the Level 2 data or unified cache of this processing element.  The event indicates to software that the access missed in the Level 2 data or unified cache and might have a significant performance impact compared to the latency of an access that hits in the Level 2 data or unified cache\000 */
{ 2211 }, /* l2d_cache_rd\000cache\000L2D cache access, read\000event=0x50\000\00000\000Attributable Level 2 data cache access, read\000 */
{ 3710 }, /* l2d_cache_refill\000cache\000Level 2 data refill\000event=0x17\000\00000\000Level 2 data refill\000 */
{ 2417 }, /* l2d_cache_refill_rd\000cache\000L2D cache refill, read\000event=0x52\000\00000\000Attributable Level 2 data cache refill, read\000 */
{ 2526 }, /* l2d_cache_refill_wr\000cache\000L2D cache refill, write\000event=0x53\000\00000\000Attributable Level 2 data cache refill, write\000 */
{ 3788 }, /* l2d_cache_wb\000cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Attributable Level 2 data cache write-back\000 */
{ 2758 }, /* l2d_cache_wb_clean\000cache\000L2D cache Write-Back, cleaning and coherency\000event=0x57\000\00000\000Level 2 data cache Write-Back, cleaning and coherency\000 */
{ 2637 }, /* l2d_cache_wb_victim\000cache\000L2D cache Write-Back, victim\000event=0x56\000\00000\000Attributable Level 2 data cache Write-Back, victim\000 */
{ 2313 }, /* l2d_cache_wr\000cache\000L2D cache access, write\000event=0x51\000\00000\000Attributable Level 2 data cache access, write\000 */
{ 4384 }, /* l2d_tlb\000cache\000Attributable Level 2 data or unified TLB access\000event=0x2f\000\00000\000Attributable Level 2 data or unified TLB access\000 */
{ 88559 }, /* l2d_tlb_rd\000cache\000L2D cache access, read\000event=0x5e\000\00000\000Attributable Level 2 data or unified TLB access, read\000 */
{ 4150 }, /* l2d_tlb_refill\000cache\000Attributable Level 2 data TLB refill\000event=0x2d\000\00000\000Attributable Level 2 data TLB refill\000 */
{ 88325 }, /* l2d_tlb_refill_rd\000cache\000L2D cache refill, read\000event=0x5c\000\00000\000Attributable Level 2 data or unified TLB refill, read\000 */
{ 88441 }, /* l2d_tlb_refill_wr\000cache\000L2D cache refill, write\000event=0x5d\000\00000\000Attributable Level 2 data or unified TLB refill, write\000 */
{ 88668 }, /* l2d_tlb_wr\000cache\000L2D cache access, write\000event=0x5f\000\00000\000Attributable Level 2 data or unified TLB access, write\000 */
{ 38783 }, /* l3d_cache\000cache\000Attributable Level 3 data cache access\000event=0x2b\000\00000\000Attributable Level 3 data cache access\000 */
{ 61550 }, /* l3d_cache_allocate\000cache\000Attributable Level 3 data cache allocation without refill\000event=0x29\000\00000\000Attributable Level 3 data cache allocation without refill\000 */
{ 44452 }, /* l3d_cache_lmiss_rd\000cache\000Level 3 data cache long-latency read miss\000event=0x400b\000\00000\000Level 3 data cache long-latency read miss.  The counter counts each memory read access counted by L3D_CACHE that incurs additional latency because it returns data from outside the Level 3 data or unified cache of this processing element.  The event indicates to software that the access missed in the Level 3 data or unified cache and might have a significant performance impact compared to the latency of an access that hits in the Level 3 data or unified cache\000 */
{ 39442 }, /* l3d_cache_rd\000cache\000Attributable Level 3 data or unified cache access, read\000event=0xa0\000\00000\000Attributable Level 3 data or unified cache access, read\000 */
{ 61706 }, /* l3d_cache_refill\000cache\000Attributable Level 3 data cache refill\000event=0x2a\000\00000\000Attributable Level 3 data cache refill\000 */
{ 21982 }, /* ld_spec\000instruction\000Operation speculatively executed, load\000event=0x70\000\00000\000Operation speculatively executed, load\000 */
{ 73432 }, /* ldrex_spec\000instruction\000Exclusive operation speculatively executed, LDREX or LDX\000event=0x6c\000\00000\000Exclusive operation speculatively executed, LDREX or LDX\000 */
{ 39009 }, /* ll_cache_miss_rd\000cache\000Last level cache miss, read\000event=0x37\000\00000\000Last level cache miss, read\000 */
{ 38892 }, /* ll_cache_rd\000cache\000Attributable Last level cache memory read\000event=0x36\000\00000\000Attributable Last level cache memory read\000 */
{ 27136 }, /* mem_access\000memory\000Data memory access\000event=0x13\000\00000\000Data memory access\000 */
{ 26460 }, /* mem_access_rd\000memory\000Data memory access, read\000event=0x66\000\00000\000Data memory access, read\000 */
{ 26546 }, /* mem_access_wr\000memory\000Data memory access, write\000event=0x67\000\00000\000Data memory access, write\000 */
{ 33636 }, /* memory_error\000exception\000Local memory error\000event=0x1a\000\00000\000Local memory error\000 */
{ 25309 }, /* op_retired\000instruction\000Micro-operation architecturally executed\000event=0x3a\000\00000\000Micro-operation architecturally executed.  The counter counts each operation counted by OP_SPEC that would be executed in a simple sequential execution of the program\000 */
{ 21665 }, /* op_spec\000instruction\000Micro-operation speculatively executed\000event=0x3b\000\00000\000Micro-operation speculatively executed.  The counter counts the number of operations executed by the processing element, including those that are executed speculatively and would not be executed in a simple sequential execution of the program\000 */
{ 22808 }, /* pc_write_spec\000instruction\000Operation speculatively executed, software change of the PC\000event=0x76\000\00000\000Operation speculatively executed, software change of the PC\000 */
{ 23813 }, /* rc_ld_spec\000instruction\000Release consistency operation speculatively executed, Load-Acquire\000event=0x90\000\00000\000Release consistency operation speculatively executed, Load-Acquire\000 */
{ 23985 }, /* rc_st_spec\000instruction\000Release consistency operation speculatively executed, Store-Release\000event=0x91\000\00000\000Release consistency operation speculatively executed, Store-Release\000 */
{ 90717 }, /* remote_access\000memory\000Access to another socket in a multi-socket system\000event=0x31\000\00000\000Access to another socket in a multi-socket system\000 */
{ 22095 }, /* st_spec\000instruction\000Operation speculatively executed, store\000event=0x71\000\00000\000Operation speculatively executed, store\000 */
{ 46833 }, /* stall\000pipeline\000No operation sent for execution\000event=0x3c\000\00000\000No operation sent for execution.  The counter counts every attributable cycle on which no attributable instruction or operation was sent for execution on this processing element\000 */
{ 27892 }, /* stall_backend\000pipeline\000No operation issued due to the backend\000event=0x24\000\00000\000No operation issued due to the backend\000 */
{ 29344 }, /* stall_backend_mem\000pipeline\000Memory stall cycles\000event=0x4005\000\00000\000Memory stall cycles.  The counter counts each cycle counted by STALL_BACKEND where there is a cache miss in the last level of cache within the processing element clock domain\000 */
{ 46706 }, /* stall_frontend\000pipeline\000No operation issued because of the frontend\000event=0x23\000\00000\000No operation issued because of the frontend\000 */
{ 29039 }, /* stall_slot\000pipeline\000No operation sent for execution on a slot\000event=0x3f\000\00000\000No operation sent for execution on a slot.  The counter counts on each attributable cycle the number of instruction or operation slots that were not occupied by an instruction or operation attributable to the processing element\000 */
{ 28291 }, /* stall_slot_backend\000pipeline\000No operation sent for execution on a slot due to the backend\000event=0x3d\000\00000\000No operation sent for execution on a slot due to the backend.  Counts each slot counted by STALL_SLOT where no attributable instruction or operation was sent for execution because the backend is unable to accept it\000 */
{ 47073 }, /* stall_slot_frontend\000pipeline\000No operation sent for execution on a slot due to the frontend\000event=0x3e\000\00000\000No operation sent for execution on a slot due to the frontend.  Counts each slot counted by STALL_SLOT where no attributable instruction or operation was sent for execution because there was no attributable instruction or operation available to issue from the processing element from the frontend for the slot\000 */
{ 73751 }, /* strex_fail_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX fail\000event=0x6e\000\00000\000Exclusive operation speculatively executed, STREX or STX fail\000 */
{ 73584 }, /* strex_pass_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX pass\000event=0x6d\000\00000\000Exclusive operation speculatively executed, STREX or STX pass\000 */
{ 88779 }, /* strex_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX\000event=0x6f\000\00000\000Exclusive operation speculatively executed, STREX or STX\000 */
{ 21286 }, /* sw_incr\000instruction\000Instruction architecturally executed, Condition code check pass, software increment\000event=0\000\00000\000Instruction architecturally executed, Condition code check pass, software increment\000 */
{ 24814 }, /* ttbr_write_retired\000instruction\000Instruction architecturally executed, Condition code check pass, write to TTBR\000event=0x1c\000\00000\000Instruction architecturally executed, Condition code check pass, write to TTBR\000 */
{ 26634 }, /* unaligned_ld_spec\000memory\000Unaligned access, read\000event=0x68\000\00000\000Unaligned access, read\000 */
{ 26808 }, /* unaligned_ldst_spec\000memory\000Unaligned access\000event=0x6a\000\00000\000Unaligned access\000 */
{ 26720 }, /* unaligned_st_spec\000memory\000Unaligned access, write\000event=0x69\000\00000\000Unaligned access, write\000 */
{ 22650 }, /* vfp_spec\000instruction\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Operation speculatively executed, floating-point instruction\000 */

};

const struct pmu_table_entry pmu_events__arm_cortex_a78[] = {
{
     .entries = pmu_events__arm_cortex_a78_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__arm_cortex_a78_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__arm_cortex_x1_default_core[] = {
{ 22494 }, /* ase_spec\000instruction\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Operation speculatively executed, Advanced SIMD instruction\000 */
{ 13 }, /* br_immed_spec\000branch\000Branch speculatively executed, immediate branch\000event=0x78\000\00000\000Branch speculatively executed, immediate branch\000 */
{ 278 }, /* br_indirect_spec\000branch\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000 */
{ 411 }, /* br_mis_pred\000branch\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000Mispredicted or not predicted branch speculatively executed\000 */
{ 25146 }, /* br_mis_pred_retired\000instruction\000Instruction architecturally executed, mispredicted branch\000event=0x22\000\00000\000Instruction architecturally executed, mispredicted branch\000 */
{ 565 }, /* br_pred\000branch\000Predictable branch speculatively executed\000event=0x12\000\00000\000Predictable branch speculatively executed\000 */
{ 25018 }, /* br_retired\000instruction\000Instruction architecturally executed, branch\000event=0x21\000\00000\000Instruction architecturally executed, branch\000 */
{ 145 }, /* br_return_spec\000branch\000Branch speculatively executed, procedure return\000event=0x79\000\00000\000Branch speculatively executed, procedure return\000 */
{ 1311 }, /* bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000Attributable Bus access\000 */
{ 771 }, /* bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Bus access read\000 */
{ 836 }, /* bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Bus access write\000 */
{ 721 }, /* bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000Bus cycle\000 */
{ 24272 }, /* cid_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000 */
{ 1389 }, /* cnt_cycles\000bus\000Constant frequency cycles\000event=0x4004\000\00000\000Constant frequency cycles.  The counter increments at a constant frequency equal to the rate of increment of the system counter, CNTPCT_EL0\000 */
{ 679 }, /* cpu_cycles\000bus\000Cycle\000event=0x11\000\00000\000Cycle\000 */
{ 23330 }, /* crypto_spec\000instruction\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Operation speculatively executed, Cryptographic instruction\000 */
{ 23705 }, /* dmb_spec\000instruction\000Barrier speculatively executed, DMB\000event=0x7e\000\00000\000Barrier speculatively executed, DMB\000 */
{ 22343 }, /* dp_spec\000instruction\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Operation speculatively executed, integer data processing\000 */
{ 23597 }, /* dsb_spec\000instruction\000Barrier speculatively executed, DSB\000event=0x7d\000\00000\000Barrier speculatively executed, DSB\000 */
{ 4626 }, /* dtlb_walk\000cache\000Access to data TLB causes a translation table walk\000event=0x34\000\00000\000Access to data TLB causes a translation table walk\000 */
{ 20002 }, /* exc_dabort\000exception\000Exception taken, Data Abort and SError\000event=0x84\000\00000\000Exception taken, Data Abort and SError\000 */
{ 20191 }, /* exc_fiq\000exception\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000 */
{ 20266 }, /* exc_hvc\000exception\000Exception taken, Hypervisor Call\000event=0x8a\000\00000\000Exception taken, Hypervisor Call\000 */
{ 20116 }, /* exc_irq\000exception\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000 */
{ 19896 }, /* exc_pabort\000exception\000Exception taken, Instruction Abort\000event=0x83\000\00000\000Exception taken, Instruction Abort\000 */
{ 33889 }, /* exc_return\000instruction\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Instruction architecturally executed, condition check pass, exception return\000 */
{ 73325 }, /* exc_smc\000exception\000Exception taken, Secure Monitor Call\000event=0x88\000\00000\000Exception taken, Secure Monitor Call\000 */
{ 19797 }, /* exc_svc\000exception\000Exception taken, Supervisor Call\000event=0x82\000\00000\000Exception taken, Supervisor Call\000 */
{ 21031 }, /* exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Exception taken\000 */
{ 20512 }, /* exc_trap_dabort\000exception\000Exception taken, Data Abort or SError not taken locally\000event=0x8c\000\00000\000Exception taken, Data Abort or SError not taken locally\000 */
{ 20915 }, /* exc_trap_fiq\000exception\000Exception taken, FIQ not taken locally\000event=0x8f\000\00000\000Exception taken, FIQ not taken locally\000 */
{ 20799 }, /* exc_trap_irq\000exception\000Exception taken, IRQ not taken locally\000event=0x8e\000\00000\000Exception taken, IRQ not taken locally\000 */
{ 20665 }, /* exc_trap_other\000exception\000Exception taken, Other traps not taken locally\000event=0x8d\000\00000\000Exception taken, Other traps not taken locally\000 */
{ 20365 }, /* exc_trap_pabort\000exception\000Exception taken, Instruction Abort not taken locally\000event=0x8b\000\00000\000Exception taken, Instruction Abort not taken locally\000 */
{ 19692 }, /* exc_undef\000exception\000Exception taken, Other synchronous\000event=0x81\000\00000\000Exception taken, Other synchronous\000 */
{ 24159 }, /* inst_retired\000instruction\000Instruction architecturally executed\000event=0x8\000\00000\000Instruction architecturally executed\000 */
{ 24711 }, /* inst_spec\000instruction\000Operation speculatively executed\000event=0x1b\000\00000\000Operation speculatively executed\000 */
{ 23489 }, /* isb_spec\000instruction\000Barrier speculatively executed, ISB\000event=0x7c\000\00000\000Barrier speculatively executed, ISB\000 */
{ 4759 }, /* itlb_walk\000cache\000Access to instruction TLB that causes a translation table walk\000event=0x35\000\00000\000Access to instruction TLB that causes a translation table walk\000 */
{ 3313 }, /* l1d_cache\000cache\000Level 1 data cache access\000event=0x4\000\00000\000Level 1 data cache access\000 */
{ 1902 }, /* l1d_cache_inval\000cache\000L1D cache invalidate\000event=0x48\000\00000\000Attributable Level 1 data cache invalidate\000 */
{ 4916 }, /* l1d_cache_lmiss_rd\000cache\000Level 1 data cache long-latency read miss\000event=0x39\000\00000\000Level 1 data cache long-latency read miss.  The counter counts each memory read access counted by L1D_CACHE that incurs additional latency because it returns data from outside the Level 1 data or unified cache of this processing element\000 */
{ 1587 }, /* l1d_cache_rd\000cache\000L1D cache access, read\000event=0x40\000\00000\000Attributable Level 1 data cache access, read\000 */
{ 3224 }, /* l1d_cache_refill\000cache\000Level 1 data cache refill\000event=0x3\000\00000\000Level 1 data cache refill\000 */
{ 39214 }, /* l1d_cache_refill_inner\000cache\000L1D cache refill, inner\000event=0x44\000\00000\000Attributable Level 1 data cache refill, inner\000 */
{ 39328 }, /* l1d_cache_refill_outer\000cache\000L1D cache refill, outer\000event=0x45\000\00000\000Attributable Level 1 data cache refill, outer\000 */
{ 1793 }, /* l1d_cache_refill_rd\000cache\000L1D cache refill, read\000event=0x42\000\00000\000Attributable Level 1 data cache refill, read\000 */
{ 39103 }, /* l1d_cache_refill_wr\000cache\000L1D cache refill, write\000event=0x43\000\00000\000Attributable Level 1 data cache refill, write\000 */
{ 33516 }, /* l1d_cache_wb\000cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000Attributable Level 1 data cache write-back\000 */
{ 73186 }, /* l1d_cache_wb_clean\000cache\000L1D cache Write-Back, cleaning and coherency\000event=0x47\000\00000\000Level 1 data cache Write-Back, cleaning and coherency\000 */
{ 73065 }, /* l1d_cache_wb_victim\000cache\000L1D cache Write-Back, victim\000event=0x46\000\00000\000Attributable Level 1 data cache Write-Back, victim\000 */
{ 1689 }, /* l1d_cache_wr\000cache\000L1D cache access, write\000event=0x41\000\00000\000Attributable Level 1 data cache access, write\000 */
{ 3908 }, /* l1d_tlb\000cache\000Attributable Level 1 data or unified TLB access\000event=0x25\000\00000\000Attributable Level 1 data or unified TLB access\000 */
{ 88109 }, /* l1d_tlb_rd\000cache\000L1D tlb access, read\000event=0x4e\000\00000\000Attributable Level 1 data or unified TLB access, read\000 */
{ 3395 }, /* l1d_tlb_refill\000cache\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Attributable Level 1 data TLB refill\000 */
{ 2003 }, /* l1d_tlb_refill_rd\000cache\000L1D tlb refill, read\000event=0x4c\000\00000\000Attributable Level 1 data TLB refill, read\000 */
{ 2106 }, /* l1d_tlb_refill_wr\000cache\000L1D tlb refill, write\000event=0x4d\000\00000\000Attributable Level 1 data TLB refill, write\000 */
{ 88216 }, /* l1d_tlb_wr\000cache\000L1D tlb access, write\000event=0x4f\000\00000\000Attributable Level 1 data or unified TLB access, write\000 */
{ 3504 }, /* l1i_cache\000cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Attributable Level 1 instruction cache access\000 */
{ 5235 }, /* l1i_cache_lmiss\000cache\000Level 1 instruction cache long-latency read miss\000event=0x4006\000\00000\000Level 1 instruction cache long-latency read miss.  If the L1I_CACHE_RD event is implemented, the counter counts each access counted by L1I_CACHE_RD that incurs additional latency because it returns instructions from outside of the Level 1 instruction cache of this PE.  If the L1I_CACHE_RD event is not implemented, the counter counts each access counted by L1I_CACHE that incurs additional latency because it returns instructions from outside the Level 1 instruction cache of this PE.  The event indicates to software that the access missed in the Level 1 instruction cache and might have a significant performance impact due to the additional latency, compared to the latency of an access that hits in the Level 1 instruction cache\000 */
{ 2998 }, /* l1i_cache_refill\000cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Level 1 instruction cache refill\000 */
{ 4033 }, /* l1i_tlb\000cache\000Attributable Level 1 instruction TLB access\000event=0x26\000\00000\000Attributable Level 1 instruction TLB access\000 */
{ 3101 }, /* l1i_tlb_refill\000cache\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Attributable Level 1 instruction TLB refill\000 */
{ 3627 }, /* l2d_cache\000cache\000Level 2 data cache access\000event=0x16\000\00000\000Level 2 data cache access\000 */
{ 38627 }, /* l2d_cache_allocate\000cache\000Attributable Level 2 data cache allocation without refill\000event=0x20\000\00000\000Attributable Level 2 data cache allocation without refill\000 */
{ 2897 }, /* l2d_cache_inval\000cache\000L2D cache invalidate\000event=0x58\000\00000\000Attributable Level 2 data cache invalidate\000 */
{ 6057 }, /* l2d_cache_lmiss_rd\000cache\000Level 2 data cache long-latency read miss\000event=0x4009\000\00000\000Level 2 data cache long-latency read miss.  The counter counts each memory read access counted by L2D_CACHE that incurs additional latency because it returns data from outside the Level 2 data or unified cache of this processing element.  The event indicates to software that the access missed in the Level 2 data or unified cache and might have a significant performance impact compared to the latency of an access that hits in the Level 2 data or unified cache\000 */
{ 2211 }, /* l2d_cache_rd\000cache\000L2D cache access, read\000event=0x50\000\00000\000Attributable Level 2 data cache access, read\000 */
{ 3710 }, /* l2d_cache_refill\000cache\000Level 2 data refill\000event=0x17\000\00000\000Level 2 data refill\000 */
{ 2417 }, /* l2d_cache_refill_rd\000cache\000L2D cache refill, read\000event=0x52\000\00000\000Attributable Level 2 data cache refill, read\000 */
{ 2526 }, /* l2d_cache_refill_wr\000cache\000L2D cache refill, write\000event=0x53\000\00000\000Attributable Level 2 data cache refill, write\000 */
{ 3788 }, /* l2d_cache_wb\000cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Attributable Level 2 data cache write-back\000 */
{ 2758 }, /* l2d_cache_wb_clean\000cache\000L2D cache Write-Back, cleaning and coherency\000event=0x57\000\00000\000Level 2 data cache Write-Back, cleaning and coherency\000 */
{ 2637 }, /* l2d_cache_wb_victim\000cache\000L2D cache Write-Back, victim\000event=0x56\000\00000\000Attributable Level 2 data cache Write-Back, victim\000 */
{ 2313 }, /* l2d_cache_wr\000cache\000L2D cache access, write\000event=0x51\000\00000\000Attributable Level 2 data cache access, write\000 */
{ 4384 }, /* l2d_tlb\000cache\000Attributable Level 2 data or unified TLB access\000event=0x2f\000\00000\000Attributable Level 2 data or unified TLB access\000 */
{ 88559 }, /* l2d_tlb_rd\000cache\000L2D cache access, read\000event=0x5e\000\00000\000Attributable Level 2 data or unified TLB access, read\000 */
{ 4150 }, /* l2d_tlb_refill\000cache\000Attributable Level 2 data TLB refill\000event=0x2d\000\00000\000Attributable Level 2 data TLB refill\000 */
{ 88325 }, /* l2d_tlb_refill_rd\000cache\000L2D cache refill, read\000event=0x5c\000\00000\000Attributable Level 2 data or unified TLB refill, read\000 */
{ 88441 }, /* l2d_tlb_refill_wr\000cache\000L2D cache refill, write\000event=0x5d\000\00000\000Attributable Level 2 data or unified TLB refill, write\000 */
{ 88668 }, /* l2d_tlb_wr\000cache\000L2D cache access, write\000event=0x5f\000\00000\000Attributable Level 2 data or unified TLB access, write\000 */
{ 38783 }, /* l3d_cache\000cache\000Attributable Level 3 data cache access\000event=0x2b\000\00000\000Attributable Level 3 data cache access\000 */
{ 61550 }, /* l3d_cache_allocate\000cache\000Attributable Level 3 data cache allocation without refill\000event=0x29\000\00000\000Attributable Level 3 data cache allocation without refill\000 */
{ 44452 }, /* l3d_cache_lmiss_rd\000cache\000Level 3 data cache long-latency read miss\000event=0x400b\000\00000\000Level 3 data cache long-latency read miss.  The counter counts each memory read access counted by L3D_CACHE that incurs additional latency because it returns data from outside the Level 3 data or unified cache of this processing element.  The event indicates to software that the access missed in the Level 3 data or unified cache and might have a significant performance impact compared to the latency of an access that hits in the Level 3 data or unified cache\000 */
{ 39442 }, /* l3d_cache_rd\000cache\000Attributable Level 3 data or unified cache access, read\000event=0xa0\000\00000\000Attributable Level 3 data or unified cache access, read\000 */
{ 61706 }, /* l3d_cache_refill\000cache\000Attributable Level 3 data cache refill\000event=0x2a\000\00000\000Attributable Level 3 data cache refill\000 */
{ 21982 }, /* ld_spec\000instruction\000Operation speculatively executed, load\000event=0x70\000\00000\000Operation speculatively executed, load\000 */
{ 73432 }, /* ldrex_spec\000instruction\000Exclusive operation speculatively executed, LDREX or LDX\000event=0x6c\000\00000\000Exclusive operation speculatively executed, LDREX or LDX\000 */
{ 39009 }, /* ll_cache_miss_rd\000cache\000Last level cache miss, read\000event=0x37\000\00000\000Last level cache miss, read\000 */
{ 38892 }, /* ll_cache_rd\000cache\000Attributable Last level cache memory read\000event=0x36\000\00000\000Attributable Last level cache memory read\000 */
{ 27136 }, /* mem_access\000memory\000Data memory access\000event=0x13\000\00000\000Data memory access\000 */
{ 26460 }, /* mem_access_rd\000memory\000Data memory access, read\000event=0x66\000\00000\000Data memory access, read\000 */
{ 26546 }, /* mem_access_wr\000memory\000Data memory access, write\000event=0x67\000\00000\000Data memory access, write\000 */
{ 33636 }, /* memory_error\000exception\000Local memory error\000event=0x1a\000\00000\000Local memory error\000 */
{ 25309 }, /* op_retired\000instruction\000Micro-operation architecturally executed\000event=0x3a\000\00000\000Micro-operation architecturally executed.  The counter counts each operation counted by OP_SPEC that would be executed in a simple sequential execution of the program\000 */
{ 21665 }, /* op_spec\000instruction\000Micro-operation speculatively executed\000event=0x3b\000\00000\000Micro-operation speculatively executed.  The counter counts the number of operations executed by the processing element, including those that are executed speculatively and would not be executed in a simple sequential execution of the program\000 */
{ 22808 }, /* pc_write_spec\000instruction\000Operation speculatively executed, software change of the PC\000event=0x76\000\00000\000Operation speculatively executed, software change of the PC\000 */
{ 23813 }, /* rc_ld_spec\000instruction\000Release consistency operation speculatively executed, Load-Acquire\000event=0x90\000\00000\000Release consistency operation speculatively executed, Load-Acquire\000 */
{ 23985 }, /* rc_st_spec\000instruction\000Release consistency operation speculatively executed, Store-Release\000event=0x91\000\00000\000Release consistency operation speculatively executed, Store-Release\000 */
{ 90717 }, /* remote_access\000memory\000Access to another socket in a multi-socket system\000event=0x31\000\00000\000Access to another socket in a multi-socket system\000 */
{ 22095 }, /* st_spec\000instruction\000Operation speculatively executed, store\000event=0x71\000\00000\000Operation speculatively executed, store\000 */
{ 46833 }, /* stall\000pipeline\000No operation sent for execution\000event=0x3c\000\00000\000No operation sent for execution.  The counter counts every attributable cycle on which no attributable instruction or operation was sent for execution on this processing element\000 */
{ 27892 }, /* stall_backend\000pipeline\000No operation issued due to the backend\000event=0x24\000\00000\000No operation issued due to the backend\000 */
{ 29344 }, /* stall_backend_mem\000pipeline\000Memory stall cycles\000event=0x4005\000\00000\000Memory stall cycles.  The counter counts each cycle counted by STALL_BACKEND where there is a cache miss in the last level of cache within the processing element clock domain\000 */
{ 46706 }, /* stall_frontend\000pipeline\000No operation issued because of the frontend\000event=0x23\000\00000\000No operation issued because of the frontend\000 */
{ 29039 }, /* stall_slot\000pipeline\000No operation sent for execution on a slot\000event=0x3f\000\00000\000No operation sent for execution on a slot.  The counter counts on each attributable cycle the number of instruction or operation slots that were not occupied by an instruction or operation attributable to the processing element\000 */
{ 28291 }, /* stall_slot_backend\000pipeline\000No operation sent for execution on a slot due to the backend\000event=0x3d\000\00000\000No operation sent for execution on a slot due to the backend.  Counts each slot counted by STALL_SLOT where no attributable instruction or operation was sent for execution because the backend is unable to accept it\000 */
{ 47073 }, /* stall_slot_frontend\000pipeline\000No operation sent for execution on a slot due to the frontend\000event=0x3e\000\00000\000No operation sent for execution on a slot due to the frontend.  Counts each slot counted by STALL_SLOT where no attributable instruction or operation was sent for execution because there was no attributable instruction or operation available to issue from the processing element from the frontend for the slot\000 */
{ 73751 }, /* strex_fail_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX fail\000event=0x6e\000\00000\000Exclusive operation speculatively executed, STREX or STX fail\000 */
{ 73584 }, /* strex_pass_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX pass\000event=0x6d\000\00000\000Exclusive operation speculatively executed, STREX or STX pass\000 */
{ 88779 }, /* strex_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX\000event=0x6f\000\00000\000Exclusive operation speculatively executed, STREX or STX\000 */
{ 21286 }, /* sw_incr\000instruction\000Instruction architecturally executed, Condition code check pass, software increment\000event=0\000\00000\000Instruction architecturally executed, Condition code check pass, software increment\000 */
{ 24814 }, /* ttbr_write_retired\000instruction\000Instruction architecturally executed, Condition code check pass, write to TTBR\000event=0x1c\000\00000\000Instruction architecturally executed, Condition code check pass, write to TTBR\000 */
{ 26634 }, /* unaligned_ld_spec\000memory\000Unaligned access, read\000event=0x68\000\00000\000Unaligned access, read\000 */
{ 26808 }, /* unaligned_ldst_spec\000memory\000Unaligned access\000event=0x6a\000\00000\000Unaligned access\000 */
{ 26720 }, /* unaligned_st_spec\000memory\000Unaligned access, write\000event=0x69\000\00000\000Unaligned access, write\000 */
{ 22650 }, /* vfp_spec\000instruction\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Operation speculatively executed, floating-point instruction\000 */

};

const struct pmu_table_entry pmu_events__arm_cortex_x1[] = {
{
     .entries = pmu_events__arm_cortex_x1_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__arm_cortex_x1_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__arm_cortex_x2_default_core[] = {
{ 88931 }, /* ase_inst_spec\000instruction\000ASE operations speculatively executed\000event=0x8005\000\00000\000ASE operations speculatively executed\000 */
{ 22494 }, /* ase_spec\000instruction\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Operation speculatively executed, Advanced SIMD instruction\000 */
{ 45947 }, /* ase_sve_int16_spec\000instruction\000Advanced SIMD and SVE 16-bit integer operations speculatively executed\000event=0x80e7\000\00000\000Advanced SIMD and SVE 16-bit integer operations speculatively executed\000 */
{ 46137 }, /* ase_sve_int32_spec\000instruction\000Advanced SIMD and SVE 32-bit integer operations speculatively executed\000event=0x80eb\000\00000\000Advanced SIMD and SVE 32-bit integer operations speculatively executed\000 */
{ 46327 }, /* ase_sve_int64_spec\000instruction\000Advanced SIMD and SVE 64-bit integer operations speculatively executed\000event=0x80ef\000\00000\000Advanced SIMD and SVE 64-bit integer operations speculatively executed\000 */
{ 45760 }, /* ase_sve_int8_spec\000instruction\000Advanced SIMD and SVE 8-bit integer operations speculatively executed\000event=0x80e3\000\00000\000Advanced SIMD and SVE 8-bit integer operations speculatively executed\000 */
{ 13 }, /* br_immed_spec\000branch\000Branch speculatively executed, immediate branch\000event=0x78\000\00000\000Branch speculatively executed, immediate branch\000 */
{ 278 }, /* br_indirect_spec\000branch\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000 */
{ 411 }, /* br_mis_pred\000branch\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000Mispredicted or not predicted branch speculatively executed\000 */
{ 25146 }, /* br_mis_pred_retired\000instruction\000Instruction architecturally executed, mispredicted branch\000event=0x22\000\00000\000Instruction architecturally executed, mispredicted branch\000 */
{ 565 }, /* br_pred\000branch\000Predictable branch speculatively executed\000event=0x12\000\00000\000Predictable branch speculatively executed\000 */
{ 25018 }, /* br_retired\000instruction\000Instruction architecturally executed, branch\000event=0x21\000\00000\000Instruction architecturally executed, branch\000 */
{ 145 }, /* br_return_spec\000branch\000Branch speculatively executed, procedure return\000event=0x79\000\00000\000Branch speculatively executed, procedure return\000 */
{ 1311 }, /* bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000Attributable Bus access\000 */
{ 771 }, /* bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Bus access read\000 */
{ 836 }, /* bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Bus access write\000 */
{ 721 }, /* bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000Bus cycle\000 */
{ 24272 }, /* cid_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000 */
{ 1389 }, /* cnt_cycles\000bus\000Constant frequency cycles\000event=0x4004\000\00000\000Constant frequency cycles.  The counter increments at a constant frequency equal to the rate of increment of the system counter, CNTPCT_EL0\000 */
{ 679 }, /* cpu_cycles\000bus\000Cycle\000event=0x11\000\00000\000Cycle\000 */
{ 23330 }, /* crypto_spec\000instruction\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Operation speculatively executed, Cryptographic instruction\000 */
{ 54196 }, /* cti_trigout4\000trace\000Cross-trigger Interface output trigger 4\000event=0x4018\000\00000\000Cross-trigger Interface output trigger 4\000 */
{ 54314 }, /* cti_trigout5\000trace\000Cross-trigger Interface output trigger 5 \000event=0x4019\000\00000\000Cross-trigger Interface output trigger 5 \000 */
{ 54434 }, /* cti_trigout6\000trace\000Cross-trigger Interface output trigger 6\000event=0x401a\000\00000\000Cross-trigger Interface output trigger 6\000 */
{ 54552 }, /* cti_trigout7\000trace\000Cross-trigger Interface output trigger 7\000event=0x401b\000\00000\000Cross-trigger Interface output trigger 7\000 */
{ 23705 }, /* dmb_spec\000instruction\000Barrier speculatively executed, DMB\000event=0x7e\000\00000\000Barrier speculatively executed, DMB\000 */
{ 22343 }, /* dp_spec\000instruction\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Operation speculatively executed, integer data processing\000 */
{ 23597 }, /* dsb_spec\000instruction\000Barrier speculatively executed, DSB\000event=0x7d\000\00000\000Barrier speculatively executed, DSB\000 */
{ 4626 }, /* dtlb_walk\000cache\000Access to data TLB causes a translation table walk\000event=0x34\000\00000\000Access to data TLB causes a translation table walk\000 */
{ 20002 }, /* exc_dabort\000exception\000Exception taken, Data Abort and SError\000event=0x84\000\00000\000Exception taken, Data Abort and SError\000 */
{ 20191 }, /* exc_fiq\000exception\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000 */
{ 20266 }, /* exc_hvc\000exception\000Exception taken, Hypervisor Call\000event=0x8a\000\00000\000Exception taken, Hypervisor Call\000 */
{ 20116 }, /* exc_irq\000exception\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000 */
{ 19896 }, /* exc_pabort\000exception\000Exception taken, Instruction Abort\000event=0x83\000\00000\000Exception taken, Instruction Abort\000 */
{ 33889 }, /* exc_return\000instruction\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Instruction architecturally executed, condition check pass, exception return\000 */
{ 73325 }, /* exc_smc\000exception\000Exception taken, Secure Monitor Call\000event=0x88\000\00000\000Exception taken, Secure Monitor Call\000 */
{ 19797 }, /* exc_svc\000exception\000Exception taken, Supervisor Call\000event=0x82\000\00000\000Exception taken, Supervisor Call\000 */
{ 21031 }, /* exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Exception taken\000 */
{ 20512 }, /* exc_trap_dabort\000exception\000Exception taken, Data Abort or SError not taken locally\000event=0x8c\000\00000\000Exception taken, Data Abort or SError not taken locally\000 */
{ 20915 }, /* exc_trap_fiq\000exception\000Exception taken, FIQ not taken locally\000event=0x8f\000\00000\000Exception taken, FIQ not taken locally\000 */
{ 20799 }, /* exc_trap_irq\000exception\000Exception taken, IRQ not taken locally\000event=0x8e\000\00000\000Exception taken, IRQ not taken locally\000 */
{ 20665 }, /* exc_trap_other\000exception\000Exception taken, Other traps not taken locally\000event=0x8d\000\00000\000Exception taken, Other traps not taken locally\000 */
{ 20365 }, /* exc_trap_pabort\000exception\000Exception taken, Instruction Abort not taken locally\000event=0x8b\000\00000\000Exception taken, Instruction Abort not taken locally\000 */
{ 19692 }, /* exc_undef\000exception\000Exception taken, Other synchronous\000event=0x81\000\00000\000Exception taken, Other synchronous\000 */
{ 45588 }, /* fp_dp_spec\000instruction\000Floating-point double-precision operations speculatively executed\000event=0x801c\000\00000\000Floating-point double-precision operations speculatively executed\000 */
{ 90530 }, /* fp_fixed_ops_spec\000instruction\000Non-scalable floating-point element Operations speculatively executed\000event=0x80c1\000\00000\000Non-scalable floating-point element Operations speculatively executed\000 */
{ 45248 }, /* fp_hp_spec\000instruction\000Floating-point half-precision operations speculatively executed\000event=0x8014\000\00000\000Floating-point half-precision operations speculatively executed\000 */
{ 90351 }, /* fp_scale_ops_spec\000instruction\000Scalable floating-point element Operations speculatively executed\000event=0x80c0\000\00000\000Scalable floating-point element Operations speculatively executed\000 */
{ 45416 }, /* fp_sp_spec\000instruction\000Floating-point single-precision operations speculatively executed\000event=0x8018\000\00000\000Floating-point single-precision operations speculatively executed\000 */
{ 24159 }, /* inst_retired\000instruction\000Instruction architecturally executed\000event=0x8\000\00000\000Instruction architecturally executed\000 */
{ 24711 }, /* inst_spec\000instruction\000Operation speculatively executed\000event=0x1b\000\00000\000Operation speculatively executed\000 */
{ 23489 }, /* isb_spec\000instruction\000Barrier speculatively executed, ISB\000event=0x7c\000\00000\000Barrier speculatively executed, ISB\000 */
{ 4759 }, /* itlb_walk\000cache\000Access to instruction TLB that causes a translation table walk\000event=0x35\000\00000\000Access to instruction TLB that causes a translation table walk\000 */
{ 3313 }, /* l1d_cache\000cache\000Level 1 data cache access\000event=0x4\000\00000\000Level 1 data cache access\000 */
{ 1902 }, /* l1d_cache_inval\000cache\000L1D cache invalidate\000event=0x48\000\00000\000Attributable Level 1 data cache invalidate\000 */
{ 4916 }, /* l1d_cache_lmiss_rd\000cache\000Level 1 data cache long-latency read miss\000event=0x39\000\00000\000Level 1 data cache long-latency read miss.  The counter counts each memory read access counted by L1D_CACHE that incurs additional latency because it returns data from outside the Level 1 data or unified cache of this processing element\000 */
{ 1587 }, /* l1d_cache_rd\000cache\000L1D cache access, read\000event=0x40\000\00000\000Attributable Level 1 data cache access, read\000 */
{ 3224 }, /* l1d_cache_refill\000cache\000Level 1 data cache refill\000event=0x3\000\00000\000Level 1 data cache refill\000 */
{ 39214 }, /* l1d_cache_refill_inner\000cache\000L1D cache refill, inner\000event=0x44\000\00000\000Attributable Level 1 data cache refill, inner\000 */
{ 39328 }, /* l1d_cache_refill_outer\000cache\000L1D cache refill, outer\000event=0x45\000\00000\000Attributable Level 1 data cache refill, outer\000 */
{ 1793 }, /* l1d_cache_refill_rd\000cache\000L1D cache refill, read\000event=0x42\000\00000\000Attributable Level 1 data cache refill, read\000 */
{ 39103 }, /* l1d_cache_refill_wr\000cache\000L1D cache refill, write\000event=0x43\000\00000\000Attributable Level 1 data cache refill, write\000 */
{ 33516 }, /* l1d_cache_wb\000cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000Attributable Level 1 data cache write-back\000 */
{ 73186 }, /* l1d_cache_wb_clean\000cache\000L1D cache Write-Back, cleaning and coherency\000event=0x47\000\00000\000Level 1 data cache Write-Back, cleaning and coherency\000 */
{ 73065 }, /* l1d_cache_wb_victim\000cache\000L1D cache Write-Back, victim\000event=0x46\000\00000\000Attributable Level 1 data cache Write-Back, victim\000 */
{ 1689 }, /* l1d_cache_wr\000cache\000L1D cache access, write\000event=0x41\000\00000\000Attributable Level 1 data cache access, write\000 */
{ 3908 }, /* l1d_tlb\000cache\000Attributable Level 1 data or unified TLB access\000event=0x25\000\00000\000Attributable Level 1 data or unified TLB access\000 */
{ 88109 }, /* l1d_tlb_rd\000cache\000L1D tlb access, read\000event=0x4e\000\00000\000Attributable Level 1 data or unified TLB access, read\000 */
{ 3395 }, /* l1d_tlb_refill\000cache\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Attributable Level 1 data TLB refill\000 */
{ 2003 }, /* l1d_tlb_refill_rd\000cache\000L1D tlb refill, read\000event=0x4c\000\00000\000Attributable Level 1 data TLB refill, read\000 */
{ 2106 }, /* l1d_tlb_refill_wr\000cache\000L1D tlb refill, write\000event=0x4d\000\00000\000Attributable Level 1 data TLB refill, write\000 */
{ 88216 }, /* l1d_tlb_wr\000cache\000L1D tlb access, write\000event=0x4f\000\00000\000Attributable Level 1 data or unified TLB access, write\000 */
{ 3504 }, /* l1i_cache\000cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Attributable Level 1 instruction cache access\000 */
{ 5235 }, /* l1i_cache_lmiss\000cache\000Level 1 instruction cache long-latency read miss\000event=0x4006\000\00000\000Level 1 instruction cache long-latency read miss.  If the L1I_CACHE_RD event is implemented, the counter counts each access counted by L1I_CACHE_RD that incurs additional latency because it returns instructions from outside of the Level 1 instruction cache of this PE.  If the L1I_CACHE_RD event is not implemented, the counter counts each access counted by L1I_CACHE that incurs additional latency because it returns instructions from outside the Level 1 instruction cache of this PE.  The event indicates to software that the access missed in the Level 1 instruction cache and might have a significant performance impact due to the additional latency, compared to the latency of an access that hits in the Level 1 instruction cache\000 */
{ 2998 }, /* l1i_cache_refill\000cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Level 1 instruction cache refill\000 */
{ 4033 }, /* l1i_tlb\000cache\000Attributable Level 1 instruction TLB access\000event=0x26\000\00000\000Attributable Level 1 instruction TLB access\000 */
{ 3101 }, /* l1i_tlb_refill\000cache\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Attributable Level 1 instruction TLB refill\000 */
{ 3627 }, /* l2d_cache\000cache\000Level 2 data cache access\000event=0x16\000\00000\000Level 2 data cache access\000 */
{ 38627 }, /* l2d_cache_allocate\000cache\000Attributable Level 2 data cache allocation without refill\000event=0x20\000\00000\000Attributable Level 2 data cache allocation without refill\000 */
{ 2897 }, /* l2d_cache_inval\000cache\000L2D cache invalidate\000event=0x58\000\00000\000Attributable Level 2 data cache invalidate\000 */
{ 6057 }, /* l2d_cache_lmiss_rd\000cache\000Level 2 data cache long-latency read miss\000event=0x4009\000\00000\000Level 2 data cache long-latency read miss.  The counter counts each memory read access counted by L2D_CACHE that incurs additional latency because it returns data from outside the Level 2 data or unified cache of this processing element.  The event indicates to software that the access missed in the Level 2 data or unified cache and might have a significant performance impact compared to the latency of an access that hits in the Level 2 data or unified cache\000 */
{ 2211 }, /* l2d_cache_rd\000cache\000L2D cache access, read\000event=0x50\000\00000\000Attributable Level 2 data cache access, read\000 */
{ 3710 }, /* l2d_cache_refill\000cache\000Level 2 data refill\000event=0x17\000\00000\000Level 2 data refill\000 */
{ 2417 }, /* l2d_cache_refill_rd\000cache\000L2D cache refill, read\000event=0x52\000\00000\000Attributable Level 2 data cache refill, read\000 */
{ 2526 }, /* l2d_cache_refill_wr\000cache\000L2D cache refill, write\000event=0x53\000\00000\000Attributable Level 2 data cache refill, write\000 */
{ 3788 }, /* l2d_cache_wb\000cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Attributable Level 2 data cache write-back\000 */
{ 2758 }, /* l2d_cache_wb_clean\000cache\000L2D cache Write-Back, cleaning and coherency\000event=0x57\000\00000\000Level 2 data cache Write-Back, cleaning and coherency\000 */
{ 2637 }, /* l2d_cache_wb_victim\000cache\000L2D cache Write-Back, victim\000event=0x56\000\00000\000Attributable Level 2 data cache Write-Back, victim\000 */
{ 2313 }, /* l2d_cache_wr\000cache\000L2D cache access, write\000event=0x51\000\00000\000Attributable Level 2 data cache access, write\000 */
{ 4384 }, /* l2d_tlb\000cache\000Attributable Level 2 data or unified TLB access\000event=0x2f\000\00000\000Attributable Level 2 data or unified TLB access\000 */
{ 88559 }, /* l2d_tlb_rd\000cache\000L2D cache access, read\000event=0x5e\000\00000\000Attributable Level 2 data or unified TLB access, read\000 */
{ 4150 }, /* l2d_tlb_refill\000cache\000Attributable Level 2 data TLB refill\000event=0x2d\000\00000\000Attributable Level 2 data TLB refill\000 */
{ 88325 }, /* l2d_tlb_refill_rd\000cache\000L2D cache refill, read\000event=0x5c\000\00000\000Attributable Level 2 data or unified TLB refill, read\000 */
{ 88441 }, /* l2d_tlb_refill_wr\000cache\000L2D cache refill, write\000event=0x5d\000\00000\000Attributable Level 2 data or unified TLB refill, write\000 */
{ 88668 }, /* l2d_tlb_wr\000cache\000L2D cache access, write\000event=0x5f\000\00000\000Attributable Level 2 data or unified TLB access, write\000 */
{ 38783 }, /* l3d_cache\000cache\000Attributable Level 3 data cache access\000event=0x2b\000\00000\000Attributable Level 3 data cache access\000 */
{ 61550 }, /* l3d_cache_allocate\000cache\000Attributable Level 3 data cache allocation without refill\000event=0x29\000\00000\000Attributable Level 3 data cache allocation without refill\000 */
{ 44452 }, /* l3d_cache_lmiss_rd\000cache\000Level 3 data cache long-latency read miss\000event=0x400b\000\00000\000Level 3 data cache long-latency read miss.  The counter counts each memory read access counted by L3D_CACHE that incurs additional latency because it returns data from outside the Level 3 data or unified cache of this processing element.  The event indicates to software that the access missed in the Level 3 data or unified cache and might have a significant performance impact compared to the latency of an access that hits in the Level 3 data or unified cache\000 */
{ 39442 }, /* l3d_cache_rd\000cache\000Attributable Level 3 data or unified cache access, read\000event=0xa0\000\00000\000Attributable Level 3 data or unified cache access, read\000 */
{ 61706 }, /* l3d_cache_refill\000cache\000Attributable Level 3 data cache refill\000event=0x2a\000\00000\000Attributable Level 3 data cache refill\000 */
{ 26884 }, /* ld_align_lat\000memory\000Load with additional latency from alignment\000event=0x4021\000\00000\000Load with additional latency from alignment\000 */
{ 21982 }, /* ld_spec\000instruction\000Operation speculatively executed, load\000event=0x70\000\00000\000Operation speculatively executed, load\000 */
{ 73432 }, /* ldrex_spec\000instruction\000Exclusive operation speculatively executed, LDREX or LDX\000event=0x6c\000\00000\000Exclusive operation speculatively executed, LDREX or LDX\000 */
{ 27280 }, /* ldst_align_lat\000memory\000Access with additional latency from alignment\000event=0x4020\000\00000\000Access with additional latency from alignment\000 */
{ 39009 }, /* ll_cache_miss_rd\000cache\000Last level cache miss, read\000event=0x37\000\00000\000Last level cache miss, read\000 */
{ 38892 }, /* ll_cache_rd\000cache\000Attributable Last level cache memory read\000event=0x36\000\00000\000Attributable Last level cache memory read\000 */
{ 27136 }, /* mem_access\000memory\000Data memory access\000event=0x13\000\00000\000Data memory access\000 */
{ 27411 }, /* mem_access_checked\000memory\000Checked data memory access\000event=0x4024\000\00000\000Checked data memory access\000 */
{ 27508 }, /* mem_access_checked_rd\000memory\000Checked data memory access, read\000event=0x4025\000\00000\000Checked data memory access, read\000 */
{ 27620 }, /* mem_access_checked_wr\000memory\000Checked data memory access, write\000event=0x4026\000\00000\000Checked data memory access, write\000 */
{ 26460 }, /* mem_access_rd\000memory\000Data memory access, read\000event=0x66\000\00000\000Data memory access, read\000 */
{ 26546 }, /* mem_access_wr\000memory\000Data memory access, write\000event=0x67\000\00000\000Data memory access, write\000 */
{ 33636 }, /* memory_error\000exception\000Local memory error\000event=0x1a\000\00000\000Local memory error\000 */
{ 25309 }, /* op_retired\000instruction\000Micro-operation architecturally executed\000event=0x3a\000\00000\000Micro-operation architecturally executed.  The counter counts each operation counted by OP_SPEC that would be executed in a simple sequential execution of the program\000 */
{ 21665 }, /* op_spec\000instruction\000Micro-operation speculatively executed\000event=0x3b\000\00000\000Micro-operation speculatively executed.  The counter counts the number of operations executed by the processing element, including those that are executed speculatively and would not be executed in a simple sequential execution of the program\000 */
{ 22808 }, /* pc_write_spec\000instruction\000Operation speculatively executed, software change of the PC\000event=0x76\000\00000\000Operation speculatively executed, software change of the PC\000 */
{ 23813 }, /* rc_ld_spec\000instruction\000Release consistency operation speculatively executed, Load-Acquire\000event=0x90\000\00000\000Release consistency operation speculatively executed, Load-Acquire\000 */
{ 23985 }, /* rc_st_spec\000instruction\000Release consistency operation speculatively executed, Store-Release\000event=0x91\000\00000\000Release consistency operation speculatively executed, Store-Release\000 */
{ 90717 }, /* remote_access\000memory\000Access to another socket in a multi-socket system\000event=0x31\000\00000\000Access to another socket in a multi-socket system\000 */
{ 27009 }, /* st_align_lat\000memory\000Store with additional latency from alignment\000event=0x4022\000\00000\000Store with additional latency from alignment\000 */
{ 22095 }, /* st_spec\000instruction\000Operation speculatively executed, store\000event=0x71\000\00000\000Operation speculatively executed, store\000 */
{ 46833 }, /* stall\000pipeline\000No operation sent for execution\000event=0x3c\000\00000\000No operation sent for execution.  The counter counts every attributable cycle on which no attributable instruction or operation was sent for execution on this processing element\000 */
{ 27892 }, /* stall_backend\000pipeline\000No operation issued due to the backend\000event=0x24\000\00000\000No operation issued due to the backend\000 */
{ 29344 }, /* stall_backend_mem\000pipeline\000Memory stall cycles\000event=0x4005\000\00000\000Memory stall cycles.  The counter counts each cycle counted by STALL_BACKEND where there is a cache miss in the last level of cache within the processing element clock domain\000 */
{ 46706 }, /* stall_frontend\000pipeline\000No operation issued because of the frontend\000event=0x23\000\00000\000No operation issued because of the frontend\000 */
{ 29039 }, /* stall_slot\000pipeline\000No operation sent for execution on a slot\000event=0x3f\000\00000\000No operation sent for execution on a slot.  The counter counts on each attributable cycle the number of instruction or operation slots that were not occupied by an instruction or operation attributable to the processing element\000 */
{ 28291 }, /* stall_slot_backend\000pipeline\000No operation sent for execution on a slot due to the backend\000event=0x3d\000\00000\000No operation sent for execution on a slot due to the backend.  Counts each slot counted by STALL_SLOT where no attributable instruction or operation was sent for execution because the backend is unable to accept it\000 */
{ 47073 }, /* stall_slot_frontend\000pipeline\000No operation sent for execution on a slot due to the frontend\000event=0x3e\000\00000\000No operation sent for execution on a slot due to the frontend.  Counts each slot counted by STALL_SLOT where no attributable instruction or operation was sent for execution because there was no attributable instruction or operation available to issue from the processing element from the frontend for the slot\000 */
{ 73751 }, /* strex_fail_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX fail\000event=0x6e\000\00000\000Exclusive operation speculatively executed, STREX or STX fail\000 */
{ 73584 }, /* strex_pass_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX pass\000event=0x6d\000\00000\000Exclusive operation speculatively executed, STREX or STX pass\000 */
{ 88779 }, /* strex_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX\000event=0x6f\000\00000\000Exclusive operation speculatively executed, STREX or STX\000 */
{ 45129 }, /* sve_inst_spec\000instruction\000SVE operations speculatively executed\000event=0x8006\000\00000\000SVE operations speculatively executed\000 */
{ 90146 }, /* sve_ldff_fault_spec\000instruction\000SVE first-fault load operations speculatively executed which set FFR bit to 0\000event=0x80bd\000\00000\000SVE first-fault load operations speculatively executed which set FFR bit to 0\000 */
{ 89993 }, /* sve_ldff_spec\000instruction\000SVE First-fault load Operations speculatively executed\000event=0x80bc\000\00000\000SVE First-fault load Operations speculatively executed\000 */
{ 89191 }, /* sve_pred_empty_spec\000instruction\000SVE predicated operations with no active predicates speculatively executed\000event=0x8075\000\00000\000SVE predicated operations with no active predicates speculatively executed\000 */
{ 89390 }, /* sve_pred_full_spec\000instruction\000SVE predicated operations speculatively executed with all active predicates\000event=0x8076\000\00000\000SVE predicated operations speculatively executed with all active predicates\000 */
{ 89805 }, /* sve_pred_not_full_spec\000instruction\000SVE predicated operations with empty or partially active predicates\000event=0x8079\000\00000\000SVE predicated operations with empty or partially active predicates\000 */
{ 89590 }, /* sve_pred_partial_spec\000instruction\000SVE predicated operations speculatively executed with partially active predicates\000event=0x8077\000\00000\000SVE predicated operations speculatively executed with partially active predicates\000 */
{ 89050 }, /* sve_pred_spec\000instruction\000SVE predicated Operations speculatively executed\000event=0x8074\000\00000\000SVE predicated Operations speculatively executed\000 */
{ 21286 }, /* sw_incr\000instruction\000Instruction architecturally executed, Condition code check pass, software increment\000event=0\000\00000\000Instruction architecturally executed, Condition code check pass, software increment\000 */
{ 53600 }, /* trb_wrap\000trace\000Trace buffer current write pointer wrapped\000event=0x400c\000\00000\000Trace buffer current write pointer wrapped\000 */
{ 53804 }, /* trcextout0\000trace\000PE Trace Unit external output 0\000event=0x4010\000\00000\000PE Trace Unit external output 0\000 */
{ 53902 }, /* trcextout1\000trace\000PE Trace Unit external output 1\000event=0x4011\000\00000\000PE Trace Unit external output 1\000 */
{ 54000 }, /* trcextout2\000trace\000PE Trace Unit external output 2\000event=0x4012\000\00000\000PE Trace Unit external output 2\000 */
{ 54098 }, /* trcextout3\000trace\000PE Trace Unit external output 3\000event=0x4013\000\00000\000PE Trace Unit external output 3\000 */
{ 24814 }, /* ttbr_write_retired\000instruction\000Instruction architecturally executed, Condition code check pass, write to TTBR\000event=0x1c\000\00000\000Instruction architecturally executed, Condition code check pass, write to TTBR\000 */
{ 26634 }, /* unaligned_ld_spec\000memory\000Unaligned access, read\000event=0x68\000\00000\000Unaligned access, read\000 */
{ 26808 }, /* unaligned_ldst_spec\000memory\000Unaligned access\000event=0x6a\000\00000\000Unaligned access\000 */
{ 26720 }, /* unaligned_st_spec\000memory\000Unaligned access, write\000event=0x69\000\00000\000Unaligned access, write\000 */
{ 22650 }, /* vfp_spec\000instruction\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Operation speculatively executed, floating-point instruction\000 */

};

const struct pmu_table_entry pmu_events__arm_cortex_x2[] = {
{
     .entries = pmu_events__arm_cortex_x2_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__arm_cortex_x2_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__arm_neoverse_n1_default_core[] = {
{ 128148 }, /* ase_spec\000spec_operation\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Counts speculatively executed Advanced SIMD operations excluding load, store and move micro-operations that move data to or from SIMD (vector) registers\000 */
{ 129087 }, /* br_immed_spec\000spec_operation\000Branch speculatively executed, immediate branch\000event=0x78\000\00000\000Counts immediate branch operations which are speculatively executed\000 */
{ 129414 }, /* br_indirect_spec\000spec_operation\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Counts indirect branch operations including procedure returns, which are speculatively executed. This includes operations that force a software change of the PC, other than exception-generating operations.  Eg: BR Xn, RET\000 */
{ 125447 }, /* br_mis_pred\000spec_operation\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000Counts branches which are speculatively executed and mispredicted\000 */
{ 124626 }, /* br_mis_pred_retired\000retired\000Instruction architecturally executed, mispredicted branch\000event=0x22\000\00000\000Counts branches counted by BR_RETIRED which were mispredicted and caused a pipeline flush\000 */
{ 125615 }, /* br_pred\000spec_operation\000Predictable branch speculatively executed\000event=0x12\000\00000\000Counts branches speculatively executed and were predicted right\000 */
{ 124407 }, /* br_retired\000retired\000Instruction architecturally executed, branch\000event=0x21\000\00000\000Counts architecturally executed branches, whether the branch is taken or not. Instructions that explicitly write to the PC are also counted\000 */
{ 129247 }, /* br_return_spec\000spec_operation\000Branch speculatively executed, procedure return\000event=0x79\000\00000\000Counts procedure return operations (RET) which are speculatively executed\000 */
{ 106366 }, /* bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000Counts memory transactions issued by the CPU to the external bus, including snoop requests and snoop responses. Each beat of data is counted individually\000 */
{ 106886 }, /* bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Counts memory read transactions seen on the external bus. Each beat of data is counted individually\000 */
{ 107035 }, /* bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Counts memory write transactions seen on the external bus. Each beat of data is counted individually\000 */
{ 106574 }, /* bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000Counts bus cycles in the CPU. Bus cycles represent a clock cycle in which a transaction could be sent or received on the interface from the CPU to the external bus. Since that interface is driven at the same clock speed as the CPU, this event is a duplicate of CPU_CYCLES\000 */
{ 123682 }, /* cid_write_retired\000retired\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000Counts architecturally executed writes to the CONTEXTIDR register, which usually contain the kernel PID and can be output with hardware trace\000 */
{ 110542 }, /* cpu_cycles\000general\000Cycle\000event=0x11\000\00000\000Counts CPU clock cycles (not timer cycles). The clock measured by this event is defined as the physical clock driving the CPU logic\000 */
{ 128892 }, /* crypto_spec\000spec_operation\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Counts speculatively executed cryptographic operations except for PMULL and VMULL operations\000 */
{ 130002 }, /* dmb_spec\000spec_operation\000Barrier speculatively executed, DMB\000event=0x7e\000\00000\000Counts DMB operations that are speculatively issued to the Load/Store unit in the CPU. This event does not count implied barriers from load acquire/store release operations\000 */
{ 127960 }, /* dp_spec\000spec_operation\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Counts speculatively executed logical or arithmetic instructions such as MOV/MVN operations\000 */
{ 129845 }, /* dsb_spec\000spec_operation\000Barrier speculatively executed, DSB\000event=0x7d\000\00000\000Counts DSB operations that are speculatively issued to Load/Store unit in the CPU\000 */
{ 133458 }, /* dtlb_walk\000tlb\000Access to data TLB causes a translation table walk\000event=0x34\000\00000\000Counts data memory translation table walks caused by a miss in the L2 TLB driven by a memory access. Note that partial translations that also cause a table walk are counted. This event does not count table walks caused by TLB maintenance operations\000 */
{ 108353 }, /* exc_dabort\000exception\000Exception taken, Data Abort and SError\000event=0x84\000\00000\000Counts exceptions that are taken locally and are caused by data aborts or SErrors. Conditions that could cause those exceptions are attempting to read or write memory where the MMU generates a fault, attempting to read or write memory with a misaligned address, interrupts from the nSEI inputs and internally generated SErrors\000 */
{ 108881 }, /* exc_fiq\000exception\000Exception taken, FIQ\000event=0x87\000\00000\000Counts FIQ exceptions including the virtual FIQs that are taken locally\000 */
{ 109111 }, /* exc_hvc\000exception\000Exception taken, Hypervisor Call\000event=0x8a\000\00000\000Counts HVC exceptions taken to EL2\000 */
{ 108755 }, /* exc_irq\000exception\000Exception taken, IRQ\000event=0x86\000\00000\000Counts IRQ exceptions including the virtual IRQs that are taken locally\000 */
{ 108196 }, /* exc_pabort\000exception\000Exception taken, Instruction Abort\000event=0x83\000\00000\000Counts synchronous exceptions that are taken locally and caused by Instruction Aborts\000 */
{ 107409 }, /* exc_return\000exception\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Counts any architecturally executed exception return instructions. Eg: AArch64: ERET\000 */
{ 109007 }, /* exc_smc\000exception\000Exception taken, Secure Monitor Call\000event=0x88\000\00000\000Counts SMC exceptions take to EL3\000 */
{ 108094 }, /* exc_svc\000exception\000Exception taken, Supervisor Call\000event=0x82\000\00000\000Counts SVC exceptions taken locally\000 */
{ 107186 }, /* exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Counts any taken architecturally visible exceptions such as IRQ, FIQ, SError, and other synchronous exceptions. Exceptions are counted whether or not they are taken locally\000 */
{ 109467 }, /* exc_trap_dabort\000exception\000Exception taken, Data Abort or SError not taken locally\000event=0x8c\000\00000\000Counts exceptions which are traps not taken locally and are caused by Data Aborts or SError interrupts. Conditions that could cause those exceptions are:\n\n1. Attempting to read or write memory where the MMU generates a fault,\n2. Attempting to read or write memory with a misaligned address,\n3. Interrupts from the SEI input.\n4. internally generated SErrors\000 */
{ 110312 }, /* exc_trap_fiq\000exception\000Exception taken, FIQ not taken locally\000event=0x8f\000\00000\000Counts FIQs which are not taken locally but taken from EL0, EL1,\n or EL2 to EL3 (which would be the normal behavior for FIQs when not executing\n in EL3)\000 */
{ 110159 }, /* exc_trap_irq\000exception\000Exception taken, IRQ not taken locally\000event=0x8e\000\00000\000Counts IRQ exceptions including the virtual IRQs that are not taken locally\000 */
{ 109921 }, /* exc_trap_other\000exception\000Exception taken, Other traps not taken locally\000event=0x8d\000\00000\000Counts the number of synchronous trap exceptions which are not taken locally and are not SVC, SMC, HVC, data aborts, Instruction Aborts, or interrupts\000 */
{ 109212 }, /* exc_trap_pabort\000exception\000Exception taken, Instruction Abort not taken locally\000event=0x8b\000\00000\000Counts exceptions which are traps not taken locally and are caused by Instruction Aborts. For example, attempting to execute an instruction with a misaligned PC\000 */
{ 107606 }, /* exc_undef\000exception\000Exception taken, Other synchronous\000event=0x81\000\00000\000Counts the number of synchronous exceptions which are taken locally that are due to attempting to execute an instruction that is UNDEFINED. Attempting to execute instruction bit patterns that have not been allocated. Attempting to execute instructions when they are disabled. Attempting to execute instructions at an inappropriate Exception level. Attempting to execute an instruction when the value of PSTATE.IL is 1\000 */
{ 123550 }, /* inst_retired\000retired\000Instruction architecturally executed\000event=0x8\000\00000\000Counts instructions that have been architecturally executed\000 */
{ 125759 }, /* inst_spec\000spec_operation\000Operation speculatively executed\000event=0x1b\000\00000\000Counts operations that have been speculatively executed\000 */
{ 129730 }, /* isb_spec\000spec_operation\000Barrier speculatively executed, ISB\000event=0x7c\000\00000\000Counts ISB operations that are executed\000 */
{ 133787 }, /* itlb_walk\000tlb\000Access to instruction TLB that causes a translation table walk\000event=0x35\000\00000\000Counts instruction memory translation table walks caused by a miss in the L2 TLB driven by a memory access. Partial translations that also cause a table walk are counted. This event does not count table walks caused by TLB maintenance operations\000 */
{ 111071 }, /* l1d_cache\000l1d_cache\000Level 1 data cache access\000event=0x4\000\00000\000Counts level 1 data cache accesses from any load/store operations. Atomic operations that resolve in the CPUs caches (near atomic operations) counts as both a write access and read access. Each access to a cache line is counted including the multiple accesses caused by single instructions such as LDM or STM. Each access to other level 1 data or unified memory structures, for example refill buffers, write buffers, and write-back buffers, are also counted\000 */
{ 114308 }, /* l1d_cache_inval\000l1d_cache\000L1D cache invalidate\000event=0x48\000\00000\000Counts each explicit invalidation of a cache line in the level 1 data cache caused by:\n\n- Cache Maintenance Operations (CMO) that operate by a virtual address.\n- Broadcast cache coherency operations from another CPU in the system.\n\nThis event does not count for the following conditions:\n\n1. A cache refill invalidates a cache line.\n2. A CMO which is executed on that CPU and invalidates a cache line specified by set/way.\n\nNote that CMOs that operate by set/way cannot be broadcast from one CPU to another\000 */
{ 112288 }, /* l1d_cache_rd\000l1d_cache\000L1D cache access, read\000event=0x40\000\00000\000Counts level 1 data cache accesses from any load operation. Atomic load operations that resolve in the CPUs caches counts as both a write access and read access\000 */
{ 110714 }, /* l1d_cache_refill\000l1d_cache\000Level 1 data cache refill\000event=0x3\000\00000\000Counts level 1 data cache refills caused by speculatively executed load or store operations that missed in the level 1 data cache. This event only counts one event per cache line. This event does not count cache line allocations from preload instructions or from hardware cache prefetching\000 */
{ 113382 }, /* l1d_cache_refill_inner\000l1d_cache\000L1D cache refill, inner\000event=0x44\000\00000\000Counts level 1 data cache refills where the cache line data came from caches inside the immediate cluster of the core\000 */
{ 113572 }, /* l1d_cache_refill_outer\000l1d_cache\000L1D cache refill, outer\000event=0x45\000\00000\000Counts level 1 data cache refills for which the cache line data came from outside the immediate cluster of the core, like an SLC in the system interconnect or DRAM\000 */
{ 112845 }, /* l1d_cache_refill_rd\000l1d_cache\000L1D cache refill, read\000event=0x42\000\00000\000Counts level 1 data cache refills caused by speculatively executed load instructions where the memory read operation misses in the level 1 data cache. This event only counts one event per cache line\000 */
{ 113112 }, /* l1d_cache_refill_wr\000l1d_cache\000L1D cache refill, write\000event=0x43\000\00000\000Counts level 1 data cache refills caused by speculatively executed store instructions where the memory write operation misses in the level 1 data cache. This event only counts one event per cache line\000 */
{ 111589 }, /* l1d_cache_wb\000l1d_cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000Counts write-backs of dirty data from the L1 data cache to the L2 cache. This occurs when either a dirty cache line is evicted from L1 data cache and allocated in the L2 cache or dirty data is written to the L2 and possibly to the next level of cache. This event counts both victim cache line evictions and cache write-backs from snoops or cache maintenance operations. The following cache operations are not counted:\n\n1. Invalidations which do not result in data being transferred out of the L1 (such as evictions of clean data),\n2. Full line writes which write to L2 without writing L1, such as write streaming mode\000 */
{ 114058 }, /* l1d_cache_wb_clean\000l1d_cache\000L1D cache Write-Back, cleaning and coherency\000event=0x47\000\00000\000Counts write-backs from the level 1 data cache that are a result of a coherency operation made by another CPU. Event count includes cache maintenance operations\000 */
{ 113808 }, /* l1d_cache_wb_victim\000l1d_cache\000L1D cache Write-Back, victim\000event=0x46\000\00000\000Counts dirty cache line evictions from the level 1 data cache caused by a new cache line allocation. This event does not count evictions caused by cache maintenance operations\000 */
{ 112510 }, /* l1d_cache_wr\000l1d_cache\000L1D cache access, write\000event=0x41\000\00000\000Counts level 1 data cache accesses generated by store operations. This event also counts accesses caused by a DC ZVA (data cache zero, specified by virtual address) instruction. Near atomic operations that resolve in the CPUs caches count as a write access and read access\000 */
{ 132529 }, /* l1d_tlb\000tlb\000Attributable Level 1 data or unified TLB access\000event=0x25\000\00000\000Counts level 1 data TLB accesses caused by any memory load or store operation. Note that load or store instructions can be broken up into multiple memory operations. This event does not count TLB maintenance operations\000 */
{ 135483 }, /* l1d_tlb_rd\000tlb\000L1D tlb access, read\000event=0x4e\000\00000\000Counts level 1 data TLB accesses caused by memory read operations. This event counts whether the access hits or misses in the TLB. This event does not count TLB maintenance operations\000 */
{ 131843 }, /* l1d_tlb_refill\000tlb\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Counts level 1 data TLB accesses that resulted in TLB refills. If there are multiple misses in the TLB that are resolved by the refill, then this event only counts once. This event counts for refills caused by preload instructions or hardware prefetch accesses. This event counts regardless of whether the miss hits in L2 or results in a translation table walk. This event will not count if the translation table walk results in a fault (such as a translation or access fault), since there is no new translation created for the TLB. This event will not count on an access from an AT(address translation) instruction\000 */
{ 134125 }, /* l1d_tlb_refill_rd\000tlb\000L1D tlb refill, read\000event=0x4c\000\00000\000Counts level 1 data TLB refills caused by memory read operations. If there are multiple misses in the TLB that are resolved by the refill, then this event only counts once. This event counts for refills caused by preload instructions or hardware prefetch accesses. This event counts regardless of whether the miss hits in L2 or results in a translation table walk. This event will not count if the translation table walk results in a fault (such as a translation or access fault), since there is no new translation created for the TLB. This event will not count on an access from an Address Translation (AT) instruction\000 */
{ 134803 }, /* l1d_tlb_refill_wr\000tlb\000L1D tlb refill, write\000event=0x4d\000\00000\000Counts level 1 data TLB refills caused by data side memory write operations. If there are multiple misses in the TLB that are resolved by the refill, then this event only counts once. This event counts for refills caused by preload instructions or hardware prefetch accesses. This event counts regardless of whether the miss hits in L2 or results in a translation table walk. This event will not count if the table walk results in a fault (such as a translation or access fault), since there is no new translation created for the TLB. This event will not count with an access from an Address Translation (AT) instruction\000 */
{ 135718 }, /* l1d_tlb_wr\000tlb\000L1D tlb access, write\000event=0x4f\000\00000\000Counts any L1 data side TLB accesses caused by memory write operations. This event counts whether the access hits or misses in the TLB. This event does not count TLB maintenance operations\000 */
{ 115166 }, /* l1i_cache\000l1i_cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Counts instruction fetches which access the level 1 instruction cache. Instruction cache accesses caused by cache maintenance operations are not counted\000 */
{ 114877 }, /* l1i_cache_refill\000l1i_cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Counts cache line refills in the level 1 instruction cache caused by a missed instruction fetch. Instruction fetches may include accessing multiple instructions, but the single cache line allocation is counted once\000 */
{ 132823 }, /* l1i_tlb\000tlb\000Attributable Level 1 instruction TLB access\000event=0x26\000\00000\000Counts level 1 instruction TLB accesses, whether the access hits or misses in the TLB. This event counts both demand accesses and prefetch or preload generated accesses\000 */
{ 131422 }, /* l1i_tlb_refill\000tlb\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Counts level 1 instruction TLB refills from any Instruction fetch. If there are multiple misses in the TLB that are resolved by the refill, then this event only counts once. This event will not count if the translation table walk results in a fault (such as a translation or access fault), since there is no new translation created for the TLB\000 */
{ 115400 }, /* l2d_cache\000l2_cache\000Level 2 data cache access\000event=0x16\000\00000\000Counts level 2 cache accesses. level 2 cache is a unified cache for data and instruction accesses. Accesses are for misses in the first level caches or translation resolutions due to accesses. This event also counts write back of dirty data from level 1 data cache to the L2 cache\000 */
{ 116475 }, /* l2d_cache_allocate\000l2_cache\000Attributable Level 2 data cache allocation without refill\000event=0x20\000\00000\000TBD\000 */
{ 118224 }, /* l2d_cache_inval\000l2_cache\000L2D cache invalidate\000event=0x58\000\00000\000Counts each explicit invalidation of a cache line in the level 2 cache by cache maintenance operations that operate by a virtual address, or by external coherency operations. This event does not count if either:\n\n1. A cache refill invalidates a cache line or,\n2. A Cache Maintenance Operation (CMO), which invalidates a cache line specified by set/way, is executed on that CPU.\n\nCMOs that operate by set/way cannot be broadcast from one CPU to another\000 */
{ 116580 }, /* l2d_cache_rd\000l2_cache\000L2D cache access, read\000event=0x50\000\00000\000Counts level 2 cache accesses due to memory read operations. level 2 cache is a unified cache for data and instruction accesses, accesses are for misses in the level 1 caches or translation resolutions due to accesses\000 */
{ 115741 }, /* l2d_cache_refill\000l2_cache\000Level 2 data refill\000event=0x17\000\00000\000Counts cache line refills into the level 2 cache. level 2 cache is a unified cache for data and instruction accesses. Accesses are for misses in the level 1 caches or translation resolutions due to accesses\000 */
{ 117138 }, /* l2d_cache_refill_rd\000l2_cache\000L2D cache refill, read\000event=0x52\000\00000\000Counts refills for memory accesses due to memory read operation counted by L2D_CACHE_RD. level 2 cache is a unified cache for data and instruction accesses, accesses are for misses in the level 1 caches or translation resolutions due to accesses\000 */
{ 117451 }, /* l2d_cache_refill_wr\000l2_cache\000L2D cache refill, write\000event=0x53\000\00000\000Counts refills for memory accesses due to memory write operation counted by L2D_CACHE_WR. level 2 cache is a unified cache for data and instruction accesses, accesses are for misses in the level 1 caches or translation resolutions due to accesses\000 */
{ 116009 }, /* l2d_cache_wb\000l2_cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Counts write-backs of data from the L2 cache to outside the CPU. This includes snoops to the L2 (from other CPUs) which return data even if the snoops cause an invalidation. L2 cache line invalidations which do not write data outside the CPU and snoops which return data from an L1 cache are not counted. Data would not be written outside the cache when invalidating a clean cache line\000 */
{ 117931 }, /* l2d_cache_wb_clean\000l2_cache\000L2D cache Write-Back, cleaning and coherency\000event=0x57\000\00000\000Counts write-backs from the level 2 cache that are a result of either:\n\n1. Cache maintenance operations,\n\n2. Snoop responses or,\n\n3. Direct cache transfers to another CPU due to a forwarding snoop request\000 */
{ 117766 }, /* l2d_cache_wb_victim\000l2_cache\000L2D cache Write-Back, victim\000event=0x56\000\00000\000Counts evictions from the level 2 cache because of a line being allocated into the L2 cache\000 */
{ 116858 }, /* l2d_cache_wr\000l2_cache\000L2D cache access, write\000event=0x51\000\00000\000Counts level 2 cache accesses due to memory write operations. level 2 cache is a unified cache for data and instruction accesses, accesses are for misses in the level 1 caches or translation resolutions due to accesses\000 */
{ 133305 }, /* l2d_tlb\000tlb\000Attributable Level 2 data or unified TLB access\000event=0x2f\000\00000\000Counts level 2 TLB accesses except those caused by TLB maintenance operations\000 */
{ 136406 }, /* l2d_tlb_rd\000tlb\000L2D cache access, read\000event=0x5e\000\00000\000Counts level 2 TLB accesses caused by memory read operations from both data and instruction fetch except for those caused by TLB maintenance operations\000 */
{ 133063 }, /* l2d_tlb_refill\000tlb\000Attributable Level 2 data TLB refill\000event=0x2d\000\00000\000Counts level 2 TLB refills caused by memory operations from both data and instruction fetch, except for those caused by TLB maintenance operations and hardware prefetches\000 */
{ 135959 }, /* l2d_tlb_refill_rd\000tlb\000L2D cache refill, read\000event=0x5c\000\00000\000Counts level 2 TLB refills caused by memory read operations from both data and instruction fetch except for those caused by TLB maintenance operations or hardware prefetches\000 */
{ 136193 }, /* l2d_tlb_refill_wr\000tlb\000L2D cache refill, write\000event=0x5d\000\00000\000Counts level 2 TLB refills caused by memory write operations from both data and instruction fetch except for those caused by TLB maintenance operations\000 */
{ 136611 }, /* l2d_tlb_wr\000tlb\000L2D cache access, write\000event=0x5f\000\00000\000Counts level 2 TLB accesses caused by memory write operations from both data and instruction fetch except for those caused by TLB maintenance operations\000 */
{ 119140 }, /* l3d_cache\000l3_cache\000Attributable Level 3 data cache access\000event=0x2b\000\00000\000Counts level 3 cache accesses. level 3 cache is a unified cache for data and instruction accesses. Accesses are for misses in the lower level caches or translation resolutions due to accesses\000 */
{ 118737 }, /* l3d_cache_allocate\000l3_cache\000Attributable Level 3 data cache allocation without refill\000event=0x29\000\00000\000Counts level 3 cache line allocates that do not fetch data from outside the level 3 data or unified cache. For example, allocates due to streaming stores\000 */
{ 119405 }, /* l3d_cache_rd\000l3_cache\000Attributable Level 3 data or unified cache access, read\000event=0xa0\000\00000\000TBD\000 */
{ 118992 }, /* l3d_cache_refill\000l3_cache\000Attributable Level 3 data cache refill\000event=0x2a\000\00000\000Counts level 3 accesses that receive data from outside the L3 cache\000 */
{ 127579 }, /* ld_spec\000spec_operation\000Operation speculatively executed, load\000event=0x70\000\00000\000Counts speculatively executed load operations including Single Instruction Multiple Data (SIMD) load operations\000 */
{ 126756 }, /* ldrex_spec\000spec_operation\000Exclusive operation speculatively executed, LDREX or LDX\000event=0x6c\000\00000\000Counts Load-Exclusive operations that have been speculatively executed. Eg: LDREX, LDX\000 */
{ 119926 }, /* ll_cache_miss_rd\000ll_cache\000Last level cache miss, read\000event=0x37\000\00000\000Counts read transactions that were returned from outside the core cluster but missed in the system level cache. This event counts when the system register CPUECTLR.EXTLLC bit is set. This event counts read transactions returned from outside the core if those transactions are missed in the System level Cache. The data source of the transaction is indicated by a field in the CHI transaction returning to the CPU. This event does not count reads caused by cache maintenance operations\000 */
{ 119502 }, /* ll_cache_rd\000ll_cache\000Attributable Last level cache memory read\000event=0x36\000\00000\000Counts read transactions that were returned from outside the core cluster. This event counts when the system register CPUECTLR.EXTLLC bit is set. This event counts read transactions returned from outside the core if those transactions are either hit in the system level cache or missed in the SLC and are returned from any other external sources\000 */
{ 120480 }, /* mem_access\000memory\000Data memory access\000event=0x13\000\00000\000Counts memory accesses issued by the CPU load store unit, where those accesses are issued due to load or store operations. This event counts memory accesses no matter whether the data is received from any level of cache hierarchy or external memory. If memory accesses are broken up into smaller transactions than what were specified in the load or store instructions, then the event counts those smaller memory transactions\000 */
{ 121838 }, /* mem_access_rd\000memory\000Data memory access, read\000event=0x66\000\00000\000Counts memory accesses issued by the CPU due to load operations. The event counts any memory load access, no matter whether the data is received from any level of cache hierarchy or external memory. The event also counts atomic load operations. If memory accesses are broken up by the load/store unit into smaller transactions that are issued by the bus interface, then the event counts those smaller transactions\000 */
{ 122313 }, /* mem_access_wr\000memory\000Data memory access, write\000event=0x67\000\00000\000Counts memory accesses issued by the CPU due to store operations. The event counts any memory store access, no matter whether the data is located in any level of cache or external memory. The event also counts atomic load and store operations. If memory accesses are broken up by the load/store unit into smaller transactions that are issued by the bus interface, then the event counts those smaller transactions\000 */
{ 120957 }, /* memory_error\000memory\000Local memory error\000event=0x1a\000\00000\000Counts any detected correctable or uncorrectable physical memory errors (ECC or parity) in protected CPUs RAMs. On the core, this event counts errors in the caches (including data and tag rams). Any detected memory error (from either a speculative and abandoned access, or an architecturally executed access) is counted. Note that errors are only detected when the actual protected memory is accessed by an operation\000 */
{ 128654 }, /* pc_write_spec\000spec_operation\000Operation speculatively executed, software change of the PC\000event=0x76\000\00000\000Counts speculatively executed operations which cause software changes of the PC. Those operations include all taken branch operations\000 */
{ 130250 }, /* rc_ld_spec\000spec_operation\000Release consistency operation speculatively executed, Load-Acquire\000event=0x90\000\00000\000Counts any load acquire operations that are speculatively executed. Eg: LDAR, LDARH, LDARB\000 */
{ 130449 }, /* rc_st_spec\000spec_operation\000Release consistency operation speculatively executed, Store-Release\000event=0x91\000\00000\000Counts any store release operations that are speculatively executed. Eg: STLR, STLRH, STLRB'\000 */
{ 121428 }, /* remote_access\000memory\000Access to another socket in a multi-socket system\000event=0x31\000\00000\000Counts accesses to another chip, which is implemented as a different CMN mesh in the system. If the CHI bus response back to the core indicates that the data source is from another chip (mesh), then the counter is updated. If no data is returned, even if the system snoops another chip/mesh, then the counter is not updated\000 */
{ 125266 }, /* sample_collision\000spe\000Sample collided with previous sample\000event=0x4003\000\00000\000Counts statistical profiling samples that have collided with a previous sample and so therefore not taken\000 */
{ 125007 }, /* sample_feed\000spe\000Sample Taken\000event=0x4001\000\00000\000Counts statistical profiling samples taken for sampling\000 */
{ 125109 }, /* sample_filtrate\000spe\000Sample Taken and not removed by filtering\000event=0x4002\000\00000\000Counts statistical profiling samples taken which are not removed by filtering\000 */
{ 124817 }, /* sample_pop\000spe\000Sample Population\000event=0x4000\000\00000\000Counts statistical profiling sample population, the count of all operations that could be sampled but may or may not be chosen for sampling\000 */
{ 127768 }, /* st_spec\000spec_operation\000Operation speculatively executed, store\000event=0x71\000\00000\000Counts speculatively executed store operations including Single Instruction Multiple Data (SIMD) store operations\000 */
{ 130990 }, /* stall_backend\000stall\000No operation issued due to the backend\000event=0x24\000\00000\000Counts cycles whenever the rename unit is unable to send any micro-operations to the backend of the pipeline because of backend resource constraints. Backend resource constraints can include issue stage fullness, execution stage fullness, or other internal pipeline resource fullness. All the backend slots were empty during the cycle when this event counts\000 */
{ 130651 }, /* stall_frontend\000stall\000No operation issued because of the frontend\000event=0x23\000\00000\000Counts cycles when frontend could not send any micro-operations to the rename stage because of frontend resource stalls caused by fetch memory latency or branch prediction flow stalls. All the frontend slots were empty during the cycle when this event counts\000 */
{ 127173 }, /* strex_fail_spec\000spec_operation\000Exclusive operation speculatively executed, STREX or STX fail\000event=0x6e\000\00000\000Counts store-exclusive operations that have been speculatively executed and have not successfully completed the store operation\000 */
{ 126941 }, /* strex_pass_spec\000spec_operation\000Exclusive operation speculatively executed, STREX or STX pass\000event=0x6d\000\00000\000Counts store-exclusive operations that have been speculatively executed and have successfully completed the store operation\000 */
{ 127409 }, /* strex_spec\000spec_operation\000Exclusive operation speculatively executed, STREX or STX\000event=0x6f\000\00000\000Counts store-exclusive operations that have been speculatively executed\000 */
{ 122788 }, /* sw_incr\000retired\000Instruction architecturally executed, Condition code check pass, software increment\000event=0\000\00000\000Counts software writes to the PMSWINC_EL0 (software PMU increment) register. The PMSWINC_EL0 register is a manually updated counter for use by application software.\n\nThis event could be used to measure any user program event, such as accesses to a particular data structure (by writing to the PMSWINC_EL0 register each time the data structure is accessed).\n\nTo use the PMSWINC_EL0 register and event, developers must insert instructions that write to the PMSWINC_EL0 register into the source code.\n\nSince the SW_INCR event records writes to the PMSWINC_EL0 register, there is no need to do a read/increment/write sequence to the PMSWINC_EL0 register\000 */
{ 123949 }, /* ttbr_write_retired\000retired\000Instruction architecturally executed, Condition code check pass, write to TTBR\000event=0x1c\000\00000\000Counts architectural writes to TTBR0/1_EL1. If virtualization host extensions are enabled (by setting the HCR_EL2.E2H bit to 1), then accesses to TTBR0/1_EL1 that are redirected to TTBR0/1_EL2, or accesses to TTBR0/1_EL12, are counted. TTBRn registers are typically updated when the kernel is swapping user-space threads or applications\000 */
{ 125888 }, /* unaligned_ld_spec\000spec_operation\000Unaligned access, read\000event=0x68\000\00000\000Counts unaligned memory read operations issued by the CPU. This event counts unaligned accesses (as defined by the actual instruction), even if they are subsequently issued as multiple aligned accesses. The event does not count preload operations (PLD, PLI)\000 */
{ 126492 }, /* unaligned_ldst_spec\000spec_operation\000Unaligned access\000event=0x6a\000\00000\000Counts unaligned memory operations issued by the CPU. This event counts unaligned accesses (as defined by the actual instruction), even if they are subsequently issued as multiple aligned accesses\000 */
{ 126217 }, /* unaligned_st_spec\000spec_operation\000Unaligned access, write\000event=0x69\000\00000\000Counts unaligned memory write operations issued by the CPU. This event counts unaligned accesses (as defined by the actual instruction), even if they are subsequently issued as multiple aligned accesses\000 */
{ 128400 }, /* vfp_spec\000spec_operation\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Counts speculatively executed floating point operations. This event does not count operations that move data to or from floating point (vector) registers\000 */

};

const struct pmu_table_entry pmu_events__arm_neoverse_n1[] = {
{
     .entries = pmu_events__arm_neoverse_n1_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__arm_neoverse_n1_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_metrics__arm_neoverse_n1_default_core[] = {
{ 204144 }, /* backend_stalled_cycles\000Cycle_Accounting\000STALL_BACKEND / CPU_CYCLES * 100\000\000This metric is the percentage of cycles that were stalled due to resource constraints in the backend unit of the processor\000\0001percent of cycles\000\000\000\00000 */
{ 204366 }, /* branch_misprediction_ratio\000Miss_Ratio;Branch_Effectiveness\000BR_MIS_PRED_RETIRED / BR_RETIRED\000\000This metric measures the ratio of branches mispredicted to the total number of branches architecturally executed. This gives an indication of the effectiveness of the branch prediction unit\000\0001per branch\000\000\000\00000 */
{ 204667 }, /* branch_mpki\000MPKI;Branch_Effectiveness\000BR_MIS_PRED_RETIRED / INST_RETIRED * 1e3\000\000This metric measures the number of branch mispredictions per thousand instructions executed\000\0001MPKI\000\000\000\00000 */
{ 204851 }, /* branch_percentage\000Operation_Mix\000(BR_IMMED_SPEC + BR_INDIRECT_SPEC) / INST_SPEC * 100\000\000This metric measures branch operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000 */
{ 196449 }, /* crypto_percentage\000Operation_Mix\000CRYPTO_SPEC / INST_SPEC * 100\000\000This metric measures crypto operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000 */
{ 205058 }, /* dtlb_mpki\000MPKI;DTLB_Effectiveness\000DTLB_WALK / INST_RETIRED * 1e3\000\000This metric measures the number of data TLB Walks per thousand instructions executed\000\0001MPKI\000\000\000\00000 */
{ 205221 }, /* dtlb_walk_ratio\000Miss_Ratio;DTLB_Effectiveness\000DTLB_WALK / L1D_TLB\000\000This metric measures the ratio of instruction TLB Walks to the total number of data TLB accesses. This gives an indication of the effectiveness of the data TLB accesses\000\0001per TLB access\000\000\000\00000 */
{ 205479 }, /* frontend_stalled_cycles\000Cycle_Accounting\000STALL_FRONTEND / CPU_CYCLES * 100\000\000This metric is the percentage of cycles that were stalled due to resource constraints in the frontend unit of the processor\000\0001percent of cycles\000\000\000\00000 */
{ 196738 }, /* integer_dp_percentage\000Operation_Mix\000DP_SPEC / INST_SPEC * 100\000\000This metric measures scalar integer operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000 */
{ 196930 }, /* ipc\000General\000INST_RETIRED / CPU_CYCLES\000\000This metric measures the number of instructions retired per cycle\000\0001per cycle\000\000\000\00000 */
{ 205704 }, /* itlb_mpki\000MPKI;ITLB_Effectiveness\000ITLB_WALK / INST_RETIRED * 1e3\000\000This metric measures the number of instruction TLB Walks per thousand instructions executed\000\0001MPKI\000\000\000\00000 */
{ 199753 }, /* itlb_walk_ratio\000Miss_Ratio;ITLB_Effectiveness\000ITLB_WALK / L1I_TLB\000\000This metric measures the ratio of instruction TLB Walks to the total number of instruction TLB accesses. This gives an indication of the effectiveness of the instruction TLB accesses\000\0001per TLB access\000\000\000\00000 */
{ 194566 }, /* l1d_cache_miss_ratio\000Miss_Ratio;L1D_Cache_Effectiveness\000L1D_CACHE_REFILL / L1D_CACHE\000\000This metric measures the ratio of level 1 data cache accesses missed to the total number of level 1 data cache accesses. This gives an indication of the effectiveness of the level 1 data cache\000\0001per cache access\000\000\000\00000 */
{ 205874 }, /* l1d_cache_mpki\000MPKI;L1D_Cache_Effectiveness\000L1D_CACHE_REFILL / INST_RETIRED * 1e3\000\000This metric measures the number of level 1 data cache accesses missed per thousand instructions executed\000\0001MPKI\000\000\000\00000 */
{ 206074 }, /* l1d_tlb_miss_ratio\000Miss_Ratio;DTLB_Effectiveness\000L1D_TLB_REFILL / L1D_TLB\000\000This metric measures the ratio of level 1 data TLB accesses missed to the total number of level 1 data TLB accesses. This gives an indication of the effectiveness of the level 1 data TLB\000\0001per TLB access\000\000\000\00000 */
{ 206358 }, /* l1d_tlb_mpki\000MPKI;DTLB_Effectiveness\000L1D_TLB_REFILL / INST_RETIRED * 1e3\000\000This metric measures the number of level 1 instruction TLB accesses missed per thousand instructions executed\000\0001MPKI\000\000\000\00000 */
{ 194869 }, /* l1i_cache_miss_ratio\000Miss_Ratio;L1I_Cache_Effectiveness\000L1I_CACHE_REFILL / L1I_CACHE\000\000This metric measures the ratio of level 1 instruction cache accesses missed to the total number of level 1 instruction cache accesses. This gives an indication of the effectiveness of the level 1 instruction cache\000\0001per cache access\000\000\000\00000 */
{ 206554 }, /* l1i_cache_mpki\000MPKI;L1I_Cache_Effectiveness\000L1I_CACHE_REFILL / INST_RETIRED * 1e3\000\000This metric measures the number of level 1 instruction cache accesses missed per thousand instructions executed\000\0001MPKI\000\000\000\00000 */
{ 206761 }, /* l1i_tlb_miss_ratio\000Miss_Ratio;ITLB_Effectiveness\000L1I_TLB_REFILL / L1I_TLB\000\000This metric measures the ratio of level 1 instruction TLB accesses missed to the total number of level 1 instruction TLB accesses. This gives an indication of the effectiveness of the level 1 instruction TLB\000\0001per TLB access\000\000\000\00000 */
{ 207066 }, /* l1i_tlb_mpki\000MPKI;ITLB_Effectiveness\000L1I_TLB_REFILL / INST_RETIRED * 1e3\000\000This metric measures the number of level 1 instruction TLB accesses missed per thousand instructions executed\000\0001MPKI\000\000\000\00000 */
{ 195319 }, /* l2_cache_miss_ratio\000Miss_Ratio;L2_Cache_Effectiveness\000L2D_CACHE_REFILL / L2D_CACHE\000\000This metric measures the ratio of level 2 cache accesses missed to the total number of level 2 cache accesses. This gives an indication of the effectiveness of the level 2 cache, which is a unified cache that stores both data and instruction. Note that cache accesses in this cache are either data memory access or instruction fetch as this is a unified cache\000\0001per cache access\000\000\000\00000 */
{ 207262 }, /* l2_cache_mpki\000MPKI;L2_Cache_Effectiveness\000L2D_CACHE_REFILL / INST_RETIRED * 1e3\000\000This metric measures the number of level 2 unified cache accesses missed per thousand instructions executed. Note that cache accesses in this cache are either data memory access or instruction fetch as this is a unified cache\000\0001MPKI\000\000\000\00000 */
{ 207581 }, /* l2_tlb_miss_ratio\000Miss_Ratio;ITLB_Effectiveness;DTLB_Effectiveness\000L2D_TLB_REFILL / L2D_TLB\000\000This metric measures the ratio of level 2 unified TLB accesses missed to the total number of level 2 unified TLB accesses. This gives an indication of the effectiveness of the level 2 TLB\000\0001per TLB access\000\000\000\00000 */
{ 207884 }, /* l2_tlb_mpki\000MPKI;ITLB_Effectiveness;DTLB_Effectiveness\000L2D_TLB_REFILL / INST_RETIRED * 1e3\000\000This metric measures the number of level 2 unified TLB accesses missed per thousand instructions executed\000\0001MPKI\000\000\000\00000 */
{ 208094 }, /* ll_cache_read_hit_ratio\000LL_Cache_Effectiveness\000(LL_CACHE_RD - LL_CACHE_MISS_RD) / LL_CACHE_RD\000\000This metric measures the ratio of last level cache read accesses hit in the cache to the total number of last level cache accesses. This gives an indication of the effectiveness of the last level cache for read traffic. Note that cache accesses in this cache are either data memory access or instruction fetch as this is a system level cache\000\0001per cache access\000\000\000\00000 */
{ 208555 }, /* ll_cache_read_miss_ratio\000Miss_Ratio;LL_Cache_Effectiveness\000LL_CACHE_MISS_RD / LL_CACHE_RD\000\000This metric measures the ratio of last level cache read accesses missed to the total number of last level cache accesses. This gives an indication of the effectiveness of the last level cache for read traffic. Note that cache accesses in this cache are either data memory access or instruction fetch as this is a system level cache\000\0001per cache access\000\000\000\00000 */
{ 209002 }, /* ll_cache_read_mpki\000MPKI;LL_Cache_Effectiveness\000LL_CACHE_MISS_RD / INST_RETIRED * 1e3\000\000This metric measures the number of last level cache read accesses missed per thousand instructions executed\000\0001MPKI\000\000\000\00000 */
{ 197052 }, /* load_percentage\000Operation_Mix\000LD_SPEC / INST_SPEC * 100\000\000This metric measures load operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000 */
{ 198032 }, /* scalar_fp_percentage\000Operation_Mix\000VFP_SPEC / INST_SPEC * 100\000\000This metric measures scalar floating point operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000 */
{ 196257 }, /* simd_percentage\000Operation_Mix\000ASE_SPEC / INST_SPEC * 100\000\000This metric measures advanced SIMD operations as a percentage of total operations speculatively executed\000\0001percent of operations\000\000\000\00000 */
{ 197854 }, /* store_percentage\000Operation_Mix\000ST_SPEC / INST_SPEC * 100\000\000This metric measures store operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000 */

};

const struct pmu_table_entry pmu_metrics__arm_neoverse_n1[] = {
{
     .entries = pmu_metrics__arm_neoverse_n1_default_core,
     .num_entries = ARRAY_SIZE(pmu_metrics__arm_neoverse_n1_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__arm_neoverse_n2_v2_default_core[] = {
{ 141250 }, /* ase_inst_spec\000spec_operation\000ASE operations speculatively executed\000event=0x8005\000\00000\000Counts speculatively executed Advanced SIMD operations\000 */
{ 128148 }, /* ase_spec\000spec_operation\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Counts speculatively executed Advanced SIMD operations excluding load, store and move micro-operations that move data to or from SIMD (vector) registers\000 */
{ 144287 }, /* ase_sve_int16_spec\000sve\000Advanced SIMD and SVE 16-bit integer operations speculatively executed\000event=0x80e7\000\00000\000Counts speculatively executed Advanced SIMD or SVE integer operations with the largest data type a 16-bit integer\000 */
{ 144512 }, /* ase_sve_int32_spec\000sve\000Advanced SIMD and SVE 32-bit integer operations speculatively executed\000event=0x80eb\000\00000\000Counts speculatively executed Advanced SIMD or SVE integer operations with the largest data type a 32-bit integer\000 */
{ 144737 }, /* ase_sve_int64_spec\000sve\000Advanced SIMD and SVE 64-bit integer operations speculatively executed\000event=0x80ef\000\00000\000Counts speculatively executed Advanced SIMD or SVE integer operations with the largest data type a 64-bit integer\000 */
{ 144064 }, /* ase_sve_int8_spec\000sve\000Advanced SIMD and SVE 8-bit integer operations speculatively executed\000event=0x80e3\000\00000\000Counts speculatively executed Advanced SIMD or SVE integer operations with the largest data type an 8-bit integer\000 */
{ 129087 }, /* br_immed_spec\000spec_operation\000Branch speculatively executed, immediate branch\000event=0x78\000\00000\000Counts immediate branch operations which are speculatively executed\000 */
{ 129414 }, /* br_indirect_spec\000spec_operation\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Counts indirect branch operations including procedure returns, which are speculatively executed. This includes operations that force a software change of the PC, other than exception-generating operations.  Eg: BR Xn, RET\000 */
{ 125447 }, /* br_mis_pred\000spec_operation\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000Counts branches which are speculatively executed and mispredicted\000 */
{ 124626 }, /* br_mis_pred_retired\000retired\000Instruction architecturally executed, mispredicted branch\000event=0x22\000\00000\000Counts branches counted by BR_RETIRED which were mispredicted and caused a pipeline flush\000 */
{ 125615 }, /* br_pred\000spec_operation\000Predictable branch speculatively executed\000event=0x12\000\00000\000Counts branches speculatively executed and were predicted right\000 */
{ 124407 }, /* br_retired\000retired\000Instruction architecturally executed, branch\000event=0x21\000\00000\000Counts architecturally executed branches, whether the branch is taken or not. Instructions that explicitly write to the PC are also counted\000 */
{ 129247 }, /* br_return_spec\000spec_operation\000Branch speculatively executed, procedure return\000event=0x79\000\00000\000Counts procedure return operations (RET) which are speculatively executed\000 */
{ 106366 }, /* bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000Counts memory transactions issued by the CPU to the external bus, including snoop requests and snoop responses. Each beat of data is counted individually\000 */
{ 106886 }, /* bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Counts memory read transactions seen on the external bus. Each beat of data is counted individually\000 */
{ 107035 }, /* bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Counts memory write transactions seen on the external bus. Each beat of data is counted individually\000 */
{ 106574 }, /* bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000Counts bus cycles in the CPU. Bus cycles represent a clock cycle in which a transaction could be sent or received on the interface from the CPU to the external bus. Since that interface is driven at the same clock speed as the CPU, this event is a duplicate of CPU_CYCLES\000 */
{ 123682 }, /* cid_write_retired\000retired\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000Counts architecturally executed writes to the CONTEXTIDR register, which usually contain the kernel PID and can be output with hardware trace\000 */
{ 137754 }, /* cnt_cycles\000general\000Constant frequency cycles\000event=0x4004\000\00000\000Counts constant frequency cycles\000 */
{ 110542 }, /* cpu_cycles\000general\000Cycle\000event=0x11\000\00000\000Counts CPU clock cycles (not timer cycles). The clock measured by this event is defined as the physical clock driving the CPU logic\000 */
{ 128892 }, /* crypto_spec\000spec_operation\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Counts speculatively executed cryptographic operations except for PMULL and VMULL operations\000 */
{ 145708 }, /* cti_trigout4\000trace\000Cross-trigger Interface output trigger 4\000event=0x4018\000\00000\000This event is generated each time an event is signaled on CTI output trigger 4\000 */
{ 145864 }, /* cti_trigout5\000trace\000Cross-trigger Interface output trigger 5 \000event=0x4019\000\00000\000This event is generated each time an event is signaled on CTI output trigger 5\000 */
{ 146021 }, /* cti_trigout6\000trace\000Cross-trigger Interface output trigger 6\000event=0x401a\000\00000\000This event is generated each time an event is signaled on CTI output trigger 6\000 */
{ 146177 }, /* cti_trigout7\000trace\000Cross-trigger Interface output trigger 7\000event=0x401b\000\00000\000This event is generated each time an event is signaled on CTI output trigger 7\000 */
{ 130002 }, /* dmb_spec\000spec_operation\000Barrier speculatively executed, DMB\000event=0x7e\000\00000\000Counts DMB operations that are speculatively issued to the Load/Store unit in the CPU. This event does not count implied barriers from load acquire/store release operations\000 */
{ 127960 }, /* dp_spec\000spec_operation\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Counts speculatively executed logical or arithmetic instructions such as MOV/MVN operations\000 */
{ 129845 }, /* dsb_spec\000spec_operation\000Barrier speculatively executed, DSB\000event=0x7d\000\00000\000Counts DSB operations that are speculatively issued to Load/Store unit in the CPU\000 */
{ 133458 }, /* dtlb_walk\000tlb\000Access to data TLB causes a translation table walk\000event=0x34\000\00000\000Counts data memory translation table walks caused by a miss in the L2 TLB driven by a memory access. Note that partial translations that also cause a table walk are counted. This event does not count table walks caused by TLB maintenance operations\000 */
{ 108353 }, /* exc_dabort\000exception\000Exception taken, Data Abort and SError\000event=0x84\000\00000\000Counts exceptions that are taken locally and are caused by data aborts or SErrors. Conditions that could cause those exceptions are attempting to read or write memory where the MMU generates a fault, attempting to read or write memory with a misaligned address, interrupts from the nSEI inputs and internally generated SErrors\000 */
{ 108881 }, /* exc_fiq\000exception\000Exception taken, FIQ\000event=0x87\000\00000\000Counts FIQ exceptions including the virtual FIQs that are taken locally\000 */
{ 109111 }, /* exc_hvc\000exception\000Exception taken, Hypervisor Call\000event=0x8a\000\00000\000Counts HVC exceptions taken to EL2\000 */
{ 108755 }, /* exc_irq\000exception\000Exception taken, IRQ\000event=0x86\000\00000\000Counts IRQ exceptions including the virtual IRQs that are taken locally\000 */
{ 108196 }, /* exc_pabort\000exception\000Exception taken, Instruction Abort\000event=0x83\000\00000\000Counts synchronous exceptions that are taken locally and caused by Instruction Aborts\000 */
{ 107409 }, /* exc_return\000exception\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Counts any architecturally executed exception return instructions. Eg: AArch64: ERET\000 */
{ 109007 }, /* exc_smc\000exception\000Exception taken, Secure Monitor Call\000event=0x88\000\00000\000Counts SMC exceptions take to EL3\000 */
{ 108094 }, /* exc_svc\000exception\000Exception taken, Supervisor Call\000event=0x82\000\00000\000Counts SVC exceptions taken locally\000 */
{ 107186 }, /* exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Counts any taken architecturally visible exceptions such as IRQ, FIQ, SError, and other synchronous exceptions. Exceptions are counted whether or not they are taken locally\000 */
{ 109467 }, /* exc_trap_dabort\000exception\000Exception taken, Data Abort or SError not taken locally\000event=0x8c\000\00000\000Counts exceptions which are traps not taken locally and are caused by Data Aborts or SError interrupts. Conditions that could cause those exceptions are:\n\n1. Attempting to read or write memory where the MMU generates a fault,\n2. Attempting to read or write memory with a misaligned address,\n3. Interrupts from the SEI input.\n4. internally generated SErrors\000 */
{ 110312 }, /* exc_trap_fiq\000exception\000Exception taken, FIQ not taken locally\000event=0x8f\000\00000\000Counts FIQs which are not taken locally but taken from EL0, EL1,\n or EL2 to EL3 (which would be the normal behavior for FIQs when not executing\n in EL3)\000 */
{ 110159 }, /* exc_trap_irq\000exception\000Exception taken, IRQ not taken locally\000event=0x8e\000\00000\000Counts IRQ exceptions including the virtual IRQs that are not taken locally\000 */
{ 109921 }, /* exc_trap_other\000exception\000Exception taken, Other traps not taken locally\000event=0x8d\000\00000\000Counts the number of synchronous trap exceptions which are not taken locally and are not SVC, SMC, HVC, data aborts, Instruction Aborts, or interrupts\000 */
{ 109212 }, /* exc_trap_pabort\000exception\000Exception taken, Instruction Abort not taken locally\000event=0x8b\000\00000\000Counts exceptions which are traps not taken locally and are caused by Instruction Aborts. For example, attempting to execute an instruction with a misaligned PC\000 */
{ 107606 }, /* exc_undef\000exception\000Exception taken, Other synchronous\000event=0x81\000\00000\000Counts the number of synchronous exceptions which are taken locally that are due to attempting to execute an instruction that is UNDEFINED. Attempting to execute instruction bit patterns that have not been allocated. Attempting to execute instructions when they are disabled. Attempting to execute instructions at an inappropriate Exception level. Attempting to execute an instruction when the value of PSTATE.IL is 1\000 */
{ 137174 }, /* fp_dp_spec\000fp_operation\000Floating-point double-precision operations speculatively executed\000event=0x801c\000\00000\000Counts speculatively executed double precision floating point operations\000 */
{ 137550 }, /* fp_fixed_ops_spec\000fp_operation\000Non-scalable floating-point element Operations speculatively executed\000event=0x80c1\000\00000\000Counts speculatively executed non-scalable single precision floating point operations\000 */
{ 136818 }, /* fp_hp_spec\000fp_operation\000Floating-point half-precision operations speculatively executed\000event=0x8014\000\00000\000Counts speculatively executed half precision floating point operations\000 */
{ 137354 }, /* fp_scale_ops_spec\000fp_operation\000Scalable floating-point element Operations speculatively executed\000event=0x80c0\000\00000\000Counts speculatively executed scalable single precision floating point operations\000 */
{ 136994 }, /* fp_sp_spec\000fp_operation\000Floating-point single-precision operations speculatively executed\000event=0x8018\000\00000\000Counts speculatively executed single precision floating point operations\000 */
{ 123550 }, /* inst_retired\000retired\000Instruction architecturally executed\000event=0x8\000\00000\000Counts instructions that have been architecturally executed\000 */
{ 125759 }, /* inst_spec\000spec_operation\000Operation speculatively executed\000event=0x1b\000\00000\000Counts operations that have been speculatively executed\000 */
{ 129730 }, /* isb_spec\000spec_operation\000Barrier speculatively executed, ISB\000event=0x7c\000\00000\000Counts ISB operations that are executed\000 */
{ 133787 }, /* itlb_walk\000tlb\000Access to instruction TLB that causes a translation table walk\000event=0x35\000\00000\000Counts instruction memory translation table walks caused by a miss in the L2 TLB driven by a memory access. Partial translations that also cause a table walk are counted. This event does not count table walks caused by TLB maintenance operations\000 */
{ 137849 }, /* l1d_cache\000l1d_cache\000Level 1 data cache access\000event=0x4\000\00000\000Counts level 1 data cache accesses from any load/store operations. Atomic operations that resolve in the CPUs caches (near atomic operations) count as both a write access and read access. Each access to a cache line is counted including the multiple accesses caused by single instructions such as LDM or STM. Each access to other level 1 data or unified memory structures, for example refill buffers, write buffers, and write-back buffers, are also counted\000 */
{ 114308 }, /* l1d_cache_inval\000l1d_cache\000L1D cache invalidate\000event=0x48\000\00000\000Counts each explicit invalidation of a cache line in the level 1 data cache caused by:\n\n- Cache Maintenance Operations (CMO) that operate by a virtual address.\n- Broadcast cache coherency operations from another CPU in the system.\n\nThis event does not count for the following conditions:\n\n1. A cache refill invalidates a cache line.\n2. A CMO which is executed on that CPU and invalidates a cache line specified by set/way.\n\nNote that CMOs that operate by set/way cannot be broadcast from one CPU to another\000 */
{ 138366 }, /* l1d_cache_lmiss_rd\000l1d_cache\000Level 1 data cache long-latency read miss\000event=0x39\000\00000\000Counts cache line refills into the level 1 data cache from any memory read operations, that incurred additional latency\000 */
{ 138572 }, /* l1d_cache_rd\000l1d_cache\000L1D cache access, read\000event=0x40\000\00000\000Counts level 1 data cache accesses from any load operation. Atomic load operations that resolve in the CPUs caches count as both a write access and read access\000 */
{ 110714 }, /* l1d_cache_refill\000l1d_cache\000Level 1 data cache refill\000event=0x3\000\00000\000Counts level 1 data cache refills caused by speculatively executed load or store operations that missed in the level 1 data cache. This event only counts one event per cache line. This event does not count cache line allocations from preload instructions or from hardware cache prefetching\000 */
{ 113382 }, /* l1d_cache_refill_inner\000l1d_cache\000L1D cache refill, inner\000event=0x44\000\00000\000Counts level 1 data cache refills where the cache line data came from caches inside the immediate cluster of the core\000 */
{ 113572 }, /* l1d_cache_refill_outer\000l1d_cache\000L1D cache refill, outer\000event=0x45\000\00000\000Counts level 1 data cache refills for which the cache line data came from outside the immediate cluster of the core, like an SLC in the system interconnect or DRAM\000 */
{ 112845 }, /* l1d_cache_refill_rd\000l1d_cache\000L1D cache refill, read\000event=0x42\000\00000\000Counts level 1 data cache refills caused by speculatively executed load instructions where the memory read operation misses in the level 1 data cache. This event only counts one event per cache line\000 */
{ 113112 }, /* l1d_cache_refill_wr\000l1d_cache\000L1D cache refill, write\000event=0x43\000\00000\000Counts level 1 data cache refills caused by speculatively executed store instructions where the memory write operation misses in the level 1 data cache. This event only counts one event per cache line\000 */
{ 111589 }, /* l1d_cache_wb\000l1d_cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000Counts write-backs of dirty data from the L1 data cache to the L2 cache. This occurs when either a dirty cache line is evicted from L1 data cache and allocated in the L2 cache or dirty data is written to the L2 and possibly to the next level of cache. This event counts both victim cache line evictions and cache write-backs from snoops or cache maintenance operations. The following cache operations are not counted:\n\n1. Invalidations which do not result in data being transferred out of the L1 (such as evictions of clean data),\n2. Full line writes which write to L2 without writing L1, such as write streaming mode\000 */
{ 114058 }, /* l1d_cache_wb_clean\000l1d_cache\000L1D cache Write-Back, cleaning and coherency\000event=0x47\000\00000\000Counts write-backs from the level 1 data cache that are a result of a coherency operation made by another CPU. Event count includes cache maintenance operations\000 */
{ 113808 }, /* l1d_cache_wb_victim\000l1d_cache\000L1D cache Write-Back, victim\000event=0x46\000\00000\000Counts dirty cache line evictions from the level 1 data cache caused by a new cache line allocation. This event does not count evictions caused by cache maintenance operations\000 */
{ 112510 }, /* l1d_cache_wr\000l1d_cache\000L1D cache access, write\000event=0x41\000\00000\000Counts level 1 data cache accesses generated by store operations. This event also counts accesses caused by a DC ZVA (data cache zero, specified by virtual address) instruction. Near atomic operations that resolve in the CPUs caches count as a write access and read access\000 */
{ 132529 }, /* l1d_tlb\000tlb\000Attributable Level 1 data or unified TLB access\000event=0x25\000\00000\000Counts level 1 data TLB accesses caused by any memory load or store operation. Note that load or store instructions can be broken up into multiple memory operations. This event does not count TLB maintenance operations\000 */
{ 135483 }, /* l1d_tlb_rd\000tlb\000L1D tlb access, read\000event=0x4e\000\00000\000Counts level 1 data TLB accesses caused by memory read operations. This event counts whether the access hits or misses in the TLB. This event does not count TLB maintenance operations\000 */
{ 131843 }, /* l1d_tlb_refill\000tlb\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Counts level 1 data TLB accesses that resulted in TLB refills. If there are multiple misses in the TLB that are resolved by the refill, then this event only counts once. This event counts for refills caused by preload instructions or hardware prefetch accesses. This event counts regardless of whether the miss hits in L2 or results in a translation table walk. This event will not count if the translation table walk results in a fault (such as a translation or access fault), since there is no new translation created for the TLB. This event will not count on an access from an AT(address translation) instruction\000 */
{ 134125 }, /* l1d_tlb_refill_rd\000tlb\000L1D tlb refill, read\000event=0x4c\000\00000\000Counts level 1 data TLB refills caused by memory read operations. If there are multiple misses in the TLB that are resolved by the refill, then this event only counts once. This event counts for refills caused by preload instructions or hardware prefetch accesses. This event counts regardless of whether the miss hits in L2 or results in a translation table walk. This event will not count if the translation table walk results in a fault (such as a translation or access fault), since there is no new translation created for the TLB. This event will not count on an access from an Address Translation (AT) instruction\000 */
{ 134803 }, /* l1d_tlb_refill_wr\000tlb\000L1D tlb refill, write\000event=0x4d\000\00000\000Counts level 1 data TLB refills caused by data side memory write operations. If there are multiple misses in the TLB that are resolved by the refill, then this event only counts once. This event counts for refills caused by preload instructions or hardware prefetch accesses. This event counts regardless of whether the miss hits in L2 or results in a translation table walk. This event will not count if the table walk results in a fault (such as a translation or access fault), since there is no new translation created for the TLB. This event will not count with an access from an Address Translation (AT) instruction\000 */
{ 135718 }, /* l1d_tlb_wr\000tlb\000L1D tlb access, write\000event=0x4f\000\00000\000Counts any L1 data side TLB accesses caused by memory write operations. This event counts whether the access hits or misses in the TLB. This event does not count TLB maintenance operations\000 */
{ 115166 }, /* l1i_cache\000l1i_cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Counts instruction fetches which access the level 1 instruction cache. Instruction cache accesses caused by cache maintenance operations are not counted\000 */
{ 138793 }, /* l1i_cache_lmiss\000l1i_cache\000Level 1 instruction cache long-latency read miss\000event=0x4006\000\00000\000Counts cache line refills into the level 1 instruction cache, that incurred additional latency\000 */
{ 114877 }, /* l1i_cache_refill\000l1i_cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Counts cache line refills in the level 1 instruction cache caused by a missed instruction fetch. Instruction fetches may include accessing multiple instructions, but the single cache line allocation is counted once\000 */
{ 132823 }, /* l1i_tlb\000tlb\000Attributable Level 1 instruction TLB access\000event=0x26\000\00000\000Counts level 1 instruction TLB accesses, whether the access hits or misses in the TLB. This event counts both demand accesses and prefetch or preload generated accesses\000 */
{ 131422 }, /* l1i_tlb_refill\000tlb\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Counts level 1 instruction TLB refills from any Instruction fetch. If there are multiple misses in the TLB that are resolved by the refill, then this event only counts once. This event will not count if the translation table walk results in a fault (such as a translation or access fault), since there is no new translation created for the TLB\000 */
{ 115400 }, /* l2d_cache\000l2_cache\000Level 2 data cache access\000event=0x16\000\00000\000Counts level 2 cache accesses. level 2 cache is a unified cache for data and instruction accesses. Accesses are for misses in the first level caches or translation resolutions due to accesses. This event also counts write back of dirty data from level 1 data cache to the L2 cache\000 */
{ 116475 }, /* l2d_cache_allocate\000l2_cache\000Attributable Level 2 data cache allocation without refill\000event=0x20\000\00000\000TBD\000 */
{ 118224 }, /* l2d_cache_inval\000l2_cache\000L2D cache invalidate\000event=0x58\000\00000\000Counts each explicit invalidation of a cache line in the level 2 cache by cache maintenance operations that operate by a virtual address, or by external coherency operations. This event does not count if either:\n\n1. A cache refill invalidates a cache line or,\n2. A Cache Maintenance Operation (CMO), which invalidates a cache line specified by set/way, is executed on that CPU.\n\nCMOs that operate by set/way cannot be broadcast from one CPU to another\000 */
{ 138980 }, /* l2d_cache_lmiss_rd\000l2_cache\000Level 2 data cache long-latency read miss\000event=0x4009\000\00000\000Counts cache line refills into the level 2 unified cache from any memory read operations that incurred additional latency\000 */
{ 116580 }, /* l2d_cache_rd\000l2_cache\000L2D cache access, read\000event=0x50\000\00000\000Counts level 2 cache accesses due to memory read operations. level 2 cache is a unified cache for data and instruction accesses, accesses are for misses in the level 1 caches or translation resolutions due to accesses\000 */
{ 115741 }, /* l2d_cache_refill\000l2_cache\000Level 2 data refill\000event=0x17\000\00000\000Counts cache line refills into the level 2 cache. level 2 cache is a unified cache for data and instruction accesses. Accesses are for misses in the level 1 caches or translation resolutions due to accesses\000 */
{ 117138 }, /* l2d_cache_refill_rd\000l2_cache\000L2D cache refill, read\000event=0x52\000\00000\000Counts refills for memory accesses due to memory read operation counted by L2D_CACHE_RD. level 2 cache is a unified cache for data and instruction accesses, accesses are for misses in the level 1 caches or translation resolutions due to accesses\000 */
{ 117451 }, /* l2d_cache_refill_wr\000l2_cache\000L2D cache refill, write\000event=0x53\000\00000\000Counts refills for memory accesses due to memory write operation counted by L2D_CACHE_WR. level 2 cache is a unified cache for data and instruction accesses, accesses are for misses in the level 1 caches or translation resolutions due to accesses\000 */
{ 116009 }, /* l2d_cache_wb\000l2_cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Counts write-backs of data from the L2 cache to outside the CPU. This includes snoops to the L2 (from other CPUs) which return data even if the snoops cause an invalidation. L2 cache line invalidations which do not write data outside the CPU and snoops which return data from an L1 cache are not counted. Data would not be written outside the cache when invalidating a clean cache line\000 */
{ 117931 }, /* l2d_cache_wb_clean\000l2_cache\000L2D cache Write-Back, cleaning and coherency\000event=0x57\000\00000\000Counts write-backs from the level 2 cache that are a result of either:\n\n1. Cache maintenance operations,\n\n2. Snoop responses or,\n\n3. Direct cache transfers to another CPU due to a forwarding snoop request\000 */
{ 117766 }, /* l2d_cache_wb_victim\000l2_cache\000L2D cache Write-Back, victim\000event=0x56\000\00000\000Counts evictions from the level 2 cache because of a line being allocated into the L2 cache\000 */
{ 116858 }, /* l2d_cache_wr\000l2_cache\000L2D cache access, write\000event=0x51\000\00000\000Counts level 2 cache accesses due to memory write operations. level 2 cache is a unified cache for data and instruction accesses, accesses are for misses in the level 1 caches or translation resolutions due to accesses\000 */
{ 133305 }, /* l2d_tlb\000tlb\000Attributable Level 2 data or unified TLB access\000event=0x2f\000\00000\000Counts level 2 TLB accesses except those caused by TLB maintenance operations\000 */
{ 136406 }, /* l2d_tlb_rd\000tlb\000L2D cache access, read\000event=0x5e\000\00000\000Counts level 2 TLB accesses caused by memory read operations from both data and instruction fetch except for those caused by TLB maintenance operations\000 */
{ 133063 }, /* l2d_tlb_refill\000tlb\000Attributable Level 2 data TLB refill\000event=0x2d\000\00000\000Counts level 2 TLB refills caused by memory operations from both data and instruction fetch, except for those caused by TLB maintenance operations and hardware prefetches\000 */
{ 135959 }, /* l2d_tlb_refill_rd\000tlb\000L2D cache refill, read\000event=0x5c\000\00000\000Counts level 2 TLB refills caused by memory read operations from both data and instruction fetch except for those caused by TLB maintenance operations or hardware prefetches\000 */
{ 136193 }, /* l2d_tlb_refill_wr\000tlb\000L2D cache refill, write\000event=0x5d\000\00000\000Counts level 2 TLB refills caused by memory write operations from both data and instruction fetch except for those caused by TLB maintenance operations\000 */
{ 136611 }, /* l2d_tlb_wr\000tlb\000L2D cache access, write\000event=0x5f\000\00000\000Counts level 2 TLB accesses caused by memory write operations from both data and instruction fetch except for those caused by TLB maintenance operations\000 */
{ 119140 }, /* l3d_cache\000l3_cache\000Attributable Level 3 data cache access\000event=0x2b\000\00000\000Counts level 3 cache accesses. level 3 cache is a unified cache for data and instruction accesses. Accesses are for misses in the lower level caches or translation resolutions due to accesses\000 */
{ 118737 }, /* l3d_cache_allocate\000l3_cache\000Attributable Level 3 data cache allocation without refill\000event=0x29\000\00000\000Counts level 3 cache line allocates that do not fetch data from outside the level 3 data or unified cache. For example, allocates due to streaming stores\000 */
{ 139189 }, /* l3d_cache_lmiss_rd\000l3_cache\000Level 3 data cache long-latency read miss\000event=0x400b\000\00000\000Counts any cache line refill into the level 3 cache from memory read operations that incurred additional latency\000 */
{ 119405 }, /* l3d_cache_rd\000l3_cache\000Attributable Level 3 data or unified cache access, read\000event=0xa0\000\00000\000TBD\000 */
{ 118992 }, /* l3d_cache_refill\000l3_cache\000Attributable Level 3 data cache refill\000event=0x2a\000\00000\000Counts level 3 accesses that receive data from outside the L3 cache\000 */
{ 139696 }, /* ld_align_lat\000memory\000Load with additional latency from alignment\000event=0x4021\000\00000\000Counts the number of memory read accesses in a cycle that incurred additional latency, due to the alignment of the address and size of data being accessed, which results in load crossing a single cache line\000 */
{ 127579 }, /* ld_spec\000spec_operation\000Operation speculatively executed, load\000event=0x70\000\00000\000Counts speculatively executed load operations including Single Instruction Multiple Data (SIMD) load operations\000 */
{ 126756 }, /* ldrex_spec\000spec_operation\000Exclusive operation speculatively executed, LDREX or LDX\000event=0x6c\000\00000\000Counts Load-Exclusive operations that have been speculatively executed. Eg: LDREX, LDX\000 */
{ 139389 }, /* ldst_align_lat\000memory\000Access with additional latency from alignment\000event=0x4020\000\00000\000Counts the number of memory read and write accesses in a cycle that incurred additional latency, due to the alignment of the address and the size of data being accessed, which results in store crossing a single cache line\000 */
{ 119926 }, /* ll_cache_miss_rd\000ll_cache\000Last level cache miss, read\000event=0x37\000\00000\000Counts read transactions that were returned from outside the core cluster but missed in the system level cache. This event counts when the system register CPUECTLR.EXTLLC bit is set. This event counts read transactions returned from outside the core if those transactions are missed in the System level Cache. The data source of the transaction is indicated by a field in the CHI transaction returning to the CPU. This event does not count reads caused by cache maintenance operations\000 */
{ 119502 }, /* ll_cache_rd\000ll_cache\000Attributable Last level cache memory read\000event=0x36\000\00000\000Counts read transactions that were returned from outside the core cluster. This event counts when the system register CPUECTLR.EXTLLC bit is set. This event counts read transactions returned from outside the core if those transactions are either hit in the system level cache or missed in the SLC and are returned from any other external sources\000 */
{ 120480 }, /* mem_access\000memory\000Data memory access\000event=0x13\000\00000\000Counts memory accesses issued by the CPU load store unit, where those accesses are issued due to load or store operations. This event counts memory accesses no matter whether the data is received from any level of cache hierarchy or external memory. If memory accesses are broken up into smaller transactions than what were specified in the load or store instructions, then the event counts those smaller memory transactions\000 */
{ 140248 }, /* mem_access_checked\000memory\000Checked data memory access\000event=0x4024\000\00000\000Counts the number of memory read and write accesses in a cycle that are tag checked by the Memory Tagging Extension (MTE)\000 */
{ 140440 }, /* mem_access_checked_rd\000memory\000Checked data memory access, read\000event=0x4025\000\00000\000Counts the number of memory read accesses in a cycle that are tag checked by the Memory Tagging Extension (MTE)\000 */
{ 140631 }, /* mem_access_checked_wr\000memory\000Checked data memory access, write\000event=0x4026\000\00000\000Counts the number of memory write accesses in a cycle that is tag checked by the Memory Tagging Extension (MTE)\000 */
{ 121838 }, /* mem_access_rd\000memory\000Data memory access, read\000event=0x66\000\00000\000Counts memory accesses issued by the CPU due to load operations. The event counts any memory load access, no matter whether the data is received from any level of cache hierarchy or external memory. The event also counts atomic load operations. If memory accesses are broken up by the load/store unit into smaller transactions that are issued by the bus interface, then the event counts those smaller transactions\000 */
{ 122313 }, /* mem_access_wr\000memory\000Data memory access, write\000event=0x67\000\00000\000Counts memory accesses issued by the CPU due to store operations. The event counts any memory store access, no matter whether the data is located in any level of cache or external memory. The event also counts atomic load and store operations. If memory accesses are broken up by the load/store unit into smaller transactions that are issued by the bus interface, then the event counts those smaller transactions\000 */
{ 120957 }, /* memory_error\000memory\000Local memory error\000event=0x1a\000\00000\000Counts any detected correctable or uncorrectable physical memory errors (ECC or parity) in protected CPUs RAMs. On the core, this event counts errors in the caches (including data and tag rams). Any detected memory error (from either a speculative and abandoned access, or an architecturally executed access) is counted. Note that errors are only detected when the actual protected memory is accessed by an operation\000 */
{ 140823 }, /* op_retired\000retired\000Micro-operation architecturally executed\000event=0x3a\000\00000\000Counts micro-operations that are architecturally executed. This is a count of number of micro-operations retired from the commit queue in a single cycle\000 */
{ 141051 }, /* op_spec\000spec_operation\000Micro-operation speculatively executed\000event=0x3b\000\00000\000Counts micro-operations speculatively executed. This is the count of the number of micro-operations dispatched in a cycle\000 */
{ 128654 }, /* pc_write_spec\000spec_operation\000Operation speculatively executed, software change of the PC\000event=0x76\000\00000\000Counts speculatively executed operations which cause software changes of the PC. Those operations include all taken branch operations\000 */
{ 130250 }, /* rc_ld_spec\000spec_operation\000Release consistency operation speculatively executed, Load-Acquire\000event=0x90\000\00000\000Counts any load acquire operations that are speculatively executed. Eg: LDAR, LDARH, LDARB\000 */
{ 130449 }, /* rc_st_spec\000spec_operation\000Release consistency operation speculatively executed, Store-Release\000event=0x91\000\00000\000Counts any store release operations that are speculatively executed. Eg: STLR, STLRH, STLRB'\000 */
{ 121428 }, /* remote_access\000memory\000Access to another socket in a multi-socket system\000event=0x31\000\00000\000Counts accesses to another chip, which is implemented as a different CMN mesh in the system. If the CHI bus response back to the core indicates that the data source is from another chip (mesh), then the counter is updated. If no data is returned, even if the system snoops another chip/mesh, then the counter is not updated\000 */
{ 125266 }, /* sample_collision\000spe\000Sample collided with previous sample\000event=0x4003\000\00000\000Counts statistical profiling samples that have collided with a previous sample and so therefore not taken\000 */
{ 125007 }, /* sample_feed\000spe\000Sample Taken\000event=0x4001\000\00000\000Counts statistical profiling samples taken for sampling\000 */
{ 125109 }, /* sample_filtrate\000spe\000Sample Taken and not removed by filtering\000event=0x4002\000\00000\000Counts statistical profiling samples taken which are not removed by filtering\000 */
{ 124817 }, /* sample_pop\000spe\000Sample Population\000event=0x4000\000\00000\000Counts statistical profiling sample population, the count of all operations that could be sampled but may or may not be chosen for sampling\000 */
{ 139984 }, /* st_align_lat\000memory\000Store with additional latency from alignment\000event=0x4022\000\00000\000Counts the number of memory write access in a cycle that incurred additional latency, due to the alignment of the address and size of data being accessed incurred additional latency\000 */
{ 127768 }, /* st_spec\000spec_operation\000Operation speculatively executed, store\000event=0x71\000\00000\000Counts speculatively executed store operations including Single Instruction Multiple Data (SIMD) store operations\000 */
{ 141389 }, /* stall\000stall\000No operation sent for execution\000event=0x3c\000\00000\000Counts cycles when no operations are sent to the rename unit from the frontend or from the rename unit to the backend for any reason (either frontend or backend stall)\000 */
{ 130990 }, /* stall_backend\000stall\000No operation issued due to the backend\000event=0x24\000\00000\000Counts cycles whenever the rename unit is unable to send any micro-operations to the backend of the pipeline because of backend resource constraints. Backend resource constraints can include issue stage fullness, execution stage fullness, or other internal pipeline resource fullness. All the backend slots were empty during the cycle when this event counts\000 */
{ 142331 }, /* stall_backend_mem\000stall\000Memory stall cycles\000event=0x4005\000\00000\000Counts cycles when the backend is stalled because there is a pending demand load request in progress in the last level core cache\000 */
{ 130651 }, /* stall_frontend\000stall\000No operation issued because of the frontend\000event=0x23\000\00000\000Counts cycles when frontend could not send any micro-operations to the rename stage because of frontend resource stalls caused by fetch memory latency or branch prediction flow stalls. All the frontend slots were empty during the cycle when this event counts\000 */
{ 142076 }, /* stall_slot\000stall\000No operation sent for execution on a slot\000event=0x3f\000\00000\000Counts slots per cycle in which no operations are sent to the rename unit from the frontend or from the rename unit to the backend for any reason (either frontend or backend stall)\000 */
{ 141616 }, /* stall_slot_backend\000stall\000No operation sent for execution on a slot due to the backend\000event=0x3d\000\00000\000Counts slots per cycle in which no operations are sent from the rename unit to the backend due to backend resource constraints\000 */
{ 141844 }, /* stall_slot_frontend\000stall\000No operation sent for execution on a slot due to the frontend\000event=0x3e\000\00000\000Counts slots per cycle in which no operations are sent to the rename unit from the frontend due to frontend resource constraints\000 */
{ 127173 }, /* strex_fail_spec\000spec_operation\000Exclusive operation speculatively executed, STREX or STX fail\000event=0x6e\000\00000\000Counts store-exclusive operations that have been speculatively executed and have not successfully completed the store operation\000 */
{ 126941 }, /* strex_pass_spec\000spec_operation\000Exclusive operation speculatively executed, STREX or STX pass\000event=0x6d\000\00000\000Counts store-exclusive operations that have been speculatively executed and have successfully completed the store operation\000 */
{ 127409 }, /* strex_spec\000spec_operation\000Exclusive operation speculatively executed, STREX or STX\000event=0x6f\000\00000\000Counts store-exclusive operations that have been speculatively executed\000 */
{ 142522 }, /* sve_inst_spec\000sve\000SVE operations speculatively executed\000event=0x8006\000\00000\000Counts speculatively executed operations that are SVE operations\000 */
{ 143831 }, /* sve_ldff_fault_spec\000sve\000SVE first-fault load operations speculatively executed which set FFR bit to 0\000event=0x80bd\000\00000\000Counts speculatively executed SVE first fault or non-fault load operations that clear at least one bit in the FFR\000 */
{ 143666 }, /* sve_ldff_spec\000sve\000SVE First-fault load Operations speculatively executed\000event=0x80bc\000\00000\000Counts speculatively executed SVE first fault or non-fault load operations\000 */
{ 142800 }, /* sve_pred_empty_spec\000sve\000SVE predicated operations with no active predicates speculatively executed\000event=0x8075\000\00000\000Counts speculatively executed predicated SVE operations with no active predicate elements\000 */
{ 143006 }, /* sve_pred_full_spec\000sve\000SVE predicated operations speculatively executed with all active predicates\000event=0x8076\000\00000\000Counts speculatively executed predicated SVE operations with all predicate elements active\000 */
{ 143450 }, /* sve_pred_not_full_spec\000sve\000SVE predicated operations with empty or partially active predicates\000event=0x8079\000\00000\000Counts speculatively executed predicated SVE operations with at least one non active predicate elements\000 */
{ 143213 }, /* sve_pred_partial_spec\000sve\000SVE predicated operations speculatively executed with partially active predicates\000event=0x8077\000\00000\000Counts speculatively executed predicated SVE operations with at least one but not all active predicate elements\000 */
{ 142660 }, /* sve_pred_spec\000sve\000SVE predicated Operations speculatively executed\000event=0x8074\000\00000\000Counts speculatively executed predicated SVE operations\000 */
{ 122788 }, /* sw_incr\000retired\000Instruction architecturally executed, Condition code check pass, software increment\000event=0\000\00000\000Counts software writes to the PMSWINC_EL0 (software PMU increment) register. The PMSWINC_EL0 register is a manually updated counter for use by application software.\n\nThis event could be used to measure any user program event, such as accesses to a particular data structure (by writing to the PMSWINC_EL0 register each time the data structure is accessed).\n\nTo use the PMSWINC_EL0 register and event, developers must insert instructions that write to the PMSWINC_EL0 register into the source code.\n\nSince the SW_INCR event records writes to the PMSWINC_EL0 register, there is no need to do a read/increment/write sequence to the PMSWINC_EL0 register\000 */
{ 144962 }, /* trb_wrap\000trace\000Trace buffer current write pointer wrapped\000event=0x400c\000\00000\000This event is generated each time the current write pointer is wrapped to the base pointer\000 */
{ 145128 }, /* trcextout0\000trace\000PE Trace Unit external output 0\000event=0x4010\000\00000\000This event is generated each time an event is signaled by ETE external event 0\000 */
{ 145273 }, /* trcextout1\000trace\000PE Trace Unit external output 1\000event=0x4011\000\00000\000This event is generated each time an event is signaled by ETE external event 1\000 */
{ 145418 }, /* trcextout2\000trace\000PE Trace Unit external output 2\000event=0x4012\000\00000\000This event is generated each time an event is signaled by ETE external event 2\000 */
{ 145563 }, /* trcextout3\000trace\000PE Trace Unit external output 3\000event=0x4013\000\00000\000This event is generated each time an event is signaled by ETE external event 3\000 */
{ 123949 }, /* ttbr_write_retired\000retired\000Instruction architecturally executed, Condition code check pass, write to TTBR\000event=0x1c\000\00000\000Counts architectural writes to TTBR0/1_EL1. If virtualization host extensions are enabled (by setting the HCR_EL2.E2H bit to 1), then accesses to TTBR0/1_EL1 that are redirected to TTBR0/1_EL2, or accesses to TTBR0/1_EL12, are counted. TTBRn registers are typically updated when the kernel is swapping user-space threads or applications\000 */
{ 125888 }, /* unaligned_ld_spec\000spec_operation\000Unaligned access, read\000event=0x68\000\00000\000Counts unaligned memory read operations issued by the CPU. This event counts unaligned accesses (as defined by the actual instruction), even if they are subsequently issued as multiple aligned accesses. The event does not count preload operations (PLD, PLI)\000 */
{ 126492 }, /* unaligned_ldst_spec\000spec_operation\000Unaligned access\000event=0x6a\000\00000\000Counts unaligned memory operations issued by the CPU. This event counts unaligned accesses (as defined by the actual instruction), even if they are subsequently issued as multiple aligned accesses\000 */
{ 126217 }, /* unaligned_st_spec\000spec_operation\000Unaligned access, write\000event=0x69\000\00000\000Counts unaligned memory write operations issued by the CPU. This event counts unaligned accesses (as defined by the actual instruction), even if they are subsequently issued as multiple aligned accesses\000 */
{ 128400 }, /* vfp_spec\000spec_operation\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Counts speculatively executed floating point operations. This event does not count operations that move data to or from floating point (vector) registers\000 */

};

const struct pmu_table_entry pmu_events__arm_neoverse_n2_v2[] = {
{
     .entries = pmu_events__arm_neoverse_n2_v2_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__arm_neoverse_n2_v2_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_metrics__arm_neoverse_n2_v2_default_core[] = {
{ 209208 }, /* backend_bound\000Default;TopdownL1\000100 * (STALL_SLOT_BACKEND / (CPU_CYCLES * #slots) - BR_MIS_PRED * 3 / CPU_CYCLES)\000\000This metric is the percentage of total slots that were stalled due to resource constraints in the backend of the processor\000\0001percent of slots\000\000\000TopdownL1\00000 */
{ 204144 }, /* backend_stalled_cycles\000Cycle_Accounting\000STALL_BACKEND / CPU_CYCLES * 100\000\000This metric is the percentage of cycles that were stalled due to resource constraints in the backend unit of the processor\000\0001percent of cycles\000\000\000\00000 */
{ 209479 }, /* bad_speculation\000Default;TopdownL1\000100 * ((1 - OP_RETIRED / OP_SPEC) * (1 - (STALL_SLOT if strcmp_cpuid_str(0x410fd493) | strcmp_cpuid_str(0x410fd490) ^ 1 else STALL_SLOT - CPU_CYCLES) / (CPU_CYCLES * #slots)) + BR_MIS_PRED * 4 / CPU_CYCLES)\000\000This metric is the percentage of total slots that executed operations and didn't retire due to a pipeline flush.\nThis indicates cycles that were utilized but inefficiently\000\0001percent of slots\000\000\000TopdownL1\00000 */
{ 211292 }, /* branch_immed_spec_rate\000Operation_Mix\000BR_IMMED_SPEC / INST_SPEC\000\000The rate of branch immediate instructions speculatively executed to overall instructions speculatively executed\000\000100%\000\000\000\00000 */
{ 211668 }, /* branch_indirect_spec_rate\000Operation_Mix\000BR_INDIRECT_SPEC / INST_SPEC\000\000The rate of indirect branch instructions speculatively executed to overall instructions speculatively executed\000\000100%\000\000\000\00000 */
{ 204366 }, /* branch_misprediction_ratio\000Miss_Ratio;Branch_Effectiveness\000BR_MIS_PRED_RETIRED / BR_RETIRED\000\000This metric measures the ratio of branches mispredicted to the total number of branches architecturally executed. This gives an indication of the effectiveness of the branch prediction unit\000\0001per branch\000\000\000\00000 */
{ 204667 }, /* branch_mpki\000MPKI;Branch_Effectiveness\000BR_MIS_PRED_RETIRED / INST_RETIRED * 1e3\000\000This metric measures the number of branch mispredictions per thousand instructions executed\000\0001MPKI\000\000\000\00000 */
{ 204851 }, /* branch_percentage\000Operation_Mix\000(BR_IMMED_SPEC + BR_INDIRECT_SPEC) / INST_SPEC * 100\000\000This metric measures branch operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000 */
{ 210955 }, /* branch_pki\000MPKI;Branch_Effectiveness\000BR_RETIRED / INST_RETIRED * 1e3\000\000The rate of branches retired per kilo instructions\000\0001PKI\000\000\000\00000 */
{ 211479 }, /* branch_return_spec_rate\000Operation_Mix\000BR_RETURN_SPEC / INST_SPEC\000\000The rate of procedure return instructions speculatively executed to overall instructions speculatively executed\000\000100%\000\000\000\00000 */
{ 196449 }, /* crypto_percentage\000Operation_Mix\000CRYPTO_SPEC / INST_SPEC * 100\000\000This metric measures crypto operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000 */
{ 205058 }, /* dtlb_mpki\000MPKI;DTLB_Effectiveness\000DTLB_WALK / INST_RETIRED * 1e3\000\000This metric measures the number of data TLB Walks per thousand instructions executed\000\0001MPKI\000\000\000\00000 */
{ 199502 }, /* dtlb_walk_ratio\000Miss_Ratio;DTLB_Effectiveness\000DTLB_WALK / L1D_TLB\000\000This metric measures the ratio of data TLB Walks to the total number of data TLB accesses. This gives an indication of the effectiveness of the data TLB accesses\000\0001per TLB access\000\000\000\00000 */
{ 209926 }, /* frontend_bound\000Default;TopdownL1\000100 * ((STALL_SLOT_FRONTEND if strcmp_cpuid_str(0x410fd493) | strcmp_cpuid_str(0x410fd490) ^ 1 else STALL_SLOT_FRONTEND - CPU_CYCLES) / (CPU_CYCLES * #slots) - BR_MIS_PRED / CPU_CYCLES)\000\000This metric is the percentage of total slots that were stalled due to resource constraints in the frontend of the processor\000\0001percent of slots\000\000\000TopdownL1\00000 */
{ 205479 }, /* frontend_stalled_cycles\000Cycle_Accounting\000STALL_FRONTEND / CPU_CYCLES * 100\000\000This metric is the percentage of cycles that were stalled due to resource constraints in the frontend unit of the processor\000\0001percent of cycles\000\000\000\00000 */
{ 196738 }, /* integer_dp_percentage\000Operation_Mix\000DP_SPEC / INST_SPEC * 100\000\000This metric measures scalar integer operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000 */
{ 196930 }, /* ipc\000General\000INST_RETIRED / CPU_CYCLES\000\000This metric measures the number of instructions retired per cycle\000\0001per cycle\000\000\000\00000 */
{ 211087 }, /* ipc_rate\000General\000ipc / #slots\000\000IPC percentage of peak. The peak of IPC is the number of slots\000\000100%\000\000\000\00000 */
{ 205704 }, /* itlb_mpki\000MPKI;ITLB_Effectiveness\000ITLB_WALK / INST_RETIRED * 1e3\000\000This metric measures the number of instruction TLB Walks per thousand instructions executed\000\0001MPKI\000\000\000\00000 */
{ 199753 }, /* itlb_walk_ratio\000Miss_Ratio;ITLB_Effectiveness\000ITLB_WALK / L1I_TLB\000\000This metric measures the ratio of instruction TLB Walks to the total number of instruction TLB accesses. This gives an indication of the effectiveness of the instruction TLB accesses\000\0001per TLB access\000\000\000\00000 */
{ 194566 }, /* l1d_cache_miss_ratio\000Miss_Ratio;L1D_Cache_Effectiveness\000L1D_CACHE_REFILL / L1D_CACHE\000\000This metric measures the ratio of level 1 data cache accesses missed to the total number of level 1 data cache accesses. This gives an indication of the effectiveness of the level 1 data cache\000\0001per cache access\000\000\000\00000 */
{ 205874 }, /* l1d_cache_mpki\000MPKI;L1D_Cache_Effectiveness\000L1D_CACHE_REFILL / INST_RETIRED * 1e3\000\000This metric measures the number of level 1 data cache accesses missed per thousand instructions executed\000\0001MPKI\000\000\000\00000 */
{ 206074 }, /* l1d_tlb_miss_ratio\000Miss_Ratio;DTLB_Effectiveness\000L1D_TLB_REFILL / L1D_TLB\000\000This metric measures the ratio of level 1 data TLB accesses missed to the total number of level 1 data TLB accesses. This gives an indication of the effectiveness of the level 1 data TLB\000\0001per TLB access\000\000\000\00000 */
{ 206358 }, /* l1d_tlb_mpki\000MPKI;DTLB_Effectiveness\000L1D_TLB_REFILL / INST_RETIRED * 1e3\000\000This metric measures the number of level 1 instruction TLB accesses missed per thousand instructions executed\000\0001MPKI\000\000\000\00000 */
{ 194869 }, /* l1i_cache_miss_ratio\000Miss_Ratio;L1I_Cache_Effectiveness\000L1I_CACHE_REFILL / L1I_CACHE\000\000This metric measures the ratio of level 1 instruction cache accesses missed to the total number of level 1 instruction cache accesses. This gives an indication of the effectiveness of the level 1 instruction cache\000\0001per cache access\000\000\000\00000 */
{ 206554 }, /* l1i_cache_mpki\000MPKI;L1I_Cache_Effectiveness\000L1I_CACHE_REFILL / INST_RETIRED * 1e3\000\000This metric measures the number of level 1 instruction cache accesses missed per thousand instructions executed\000\0001MPKI\000\000\000\00000 */
{ 206761 }, /* l1i_tlb_miss_ratio\000Miss_Ratio;ITLB_Effectiveness\000L1I_TLB_REFILL / L1I_TLB\000\000This metric measures the ratio of level 1 instruction TLB accesses missed to the total number of level 1 instruction TLB accesses. This gives an indication of the effectiveness of the level 1 instruction TLB\000\0001per TLB access\000\000\000\00000 */
{ 207066 }, /* l1i_tlb_mpki\000MPKI;ITLB_Effectiveness\000L1I_TLB_REFILL / INST_RETIRED * 1e3\000\000This metric measures the number of level 1 instruction TLB accesses missed per thousand instructions executed\000\0001MPKI\000\000\000\00000 */
{ 195319 }, /* l2_cache_miss_ratio\000Miss_Ratio;L2_Cache_Effectiveness\000L2D_CACHE_REFILL / L2D_CACHE\000\000This metric measures the ratio of level 2 cache accesses missed to the total number of level 2 cache accesses. This gives an indication of the effectiveness of the level 2 cache, which is a unified cache that stores both data and instruction. Note that cache accesses in this cache are either data memory access or instruction fetch as this is a unified cache\000\0001per cache access\000\000\000\00000 */
{ 207262 }, /* l2_cache_mpki\000MPKI;L2_Cache_Effectiveness\000L2D_CACHE_REFILL / INST_RETIRED * 1e3\000\000This metric measures the number of level 2 unified cache accesses missed per thousand instructions executed. Note that cache accesses in this cache are either data memory access or instruction fetch as this is a unified cache\000\0001MPKI\000\000\000\00000 */
{ 207581 }, /* l2_tlb_miss_ratio\000Miss_Ratio;ITLB_Effectiveness;DTLB_Effectiveness\000L2D_TLB_REFILL / L2D_TLB\000\000This metric measures the ratio of level 2 unified TLB accesses missed to the total number of level 2 unified TLB accesses. This gives an indication of the effectiveness of the level 2 TLB\000\0001per TLB access\000\000\000\00000 */
{ 207884 }, /* l2_tlb_mpki\000MPKI;ITLB_Effectiveness;DTLB_Effectiveness\000L2D_TLB_REFILL / INST_RETIRED * 1e3\000\000This metric measures the number of level 2 unified TLB accesses missed per thousand instructions executed\000\0001MPKI\000\000\000\00000 */
{ 210804 }, /* l3d_cache_miss_rate\000Miss_Ratio;L3_Cache_Effectiveness\000L3D_CACHE_REFILL / L3D_CACHE\000\000The rate of L3 D-Cache misses to the overall L3 D-Cache\000\000100%\000\000\000\00000 */
{ 210658 }, /* l3d_cache_mpki\000MPKI;L3_Cache_Effectiveness\000L3D_CACHE_REFILL / INST_RETIRED * 1e3\000\000The rate of L3 D-Cache misses per kilo instructions\000\0001MPKI\000\000\000\00000 */
{ 208094 }, /* ll_cache_read_hit_ratio\000LL_Cache_Effectiveness\000(LL_CACHE_RD - LL_CACHE_MISS_RD) / LL_CACHE_RD\000\000This metric measures the ratio of last level cache read accesses hit in the cache to the total number of last level cache accesses. This gives an indication of the effectiveness of the last level cache for read traffic. Note that cache accesses in this cache are either data memory access or instruction fetch as this is a system level cache\000\0001per cache access\000\000\000\00000 */
{ 208555 }, /* ll_cache_read_miss_ratio\000Miss_Ratio;LL_Cache_Effectiveness\000LL_CACHE_MISS_RD / LL_CACHE_RD\000\000This metric measures the ratio of last level cache read accesses missed to the total number of last level cache accesses. This gives an indication of the effectiveness of the last level cache for read traffic. Note that cache accesses in this cache are either data memory access or instruction fetch as this is a system level cache\000\0001per cache access\000\000\000\00000 */
{ 209002 }, /* ll_cache_read_mpki\000MPKI;LL_Cache_Effectiveness\000LL_CACHE_MISS_RD / INST_RETIRED * 1e3\000\000This metric measures the number of last level cache read accesses missed per thousand instructions executed\000\0001MPKI\000\000\000\00000 */
{ 197052 }, /* load_percentage\000Operation_Mix\000LD_SPEC / INST_SPEC * 100\000\000This metric measures load operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000 */
{ 198231 }, /* retired_rate\000General\000OP_RETIRED / OP_SPEC\000\000Of all the micro-operations issued, what percentage are retired(committed)\000\000100%\000\000\000\00000 */
{ 210303 }, /* retiring\000Default;TopdownL1\000100 * (OP_RETIRED / OP_SPEC * (1 - (STALL_SLOT if strcmp_cpuid_str(0x410fd493) | strcmp_cpuid_str(0x410fd490) ^ 1 else STALL_SLOT - CPU_CYCLES) / (CPU_CYCLES * #slots)))\000\000This metric is the percentage of total slots that retired operations, which indicates cycles that were utilized efficiently\000\0001percent of slots\000\000\000TopdownL1\00000 */
{ 198032 }, /* scalar_fp_percentage\000Operation_Mix\000VFP_SPEC / INST_SPEC * 100\000\000This metric measures scalar floating point operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000 */
{ 196257 }, /* simd_percentage\000Operation_Mix\000ASE_SPEC / INST_SPEC * 100\000\000This metric measures advanced SIMD operations as a percentage of total operations speculatively executed\000\0001percent of operations\000\000\000\00000 */
{ 211192 }, /* spec_ipc\000General\000INST_SPEC / CPU_CYCLES\000\000Speculatively executed Instructions Per Cycle (IPC)\000\000\000\000\000\00000 */
{ 197854 }, /* store_percentage\000Operation_Mix\000ST_SPEC / INST_SPEC * 100\000\000This metric measures store operations as a percentage of operations speculatively executed\000\0001percent of operations\000\000\000\00000 */
{ 198487 }, /* wasted_rate\000General\0001 - OP_RETIRED / OP_SPEC\000\000Of all the micro-operations issued, what percentage are not retired(committed)\000\000100%\000\000\000\00000 */

};

const struct pmu_table_entry pmu_metrics__arm_neoverse_n2_v2[] = {
{
     .entries = pmu_metrics__arm_neoverse_n2_v2_default_core,
     .num_entries = ARRAY_SIZE(pmu_metrics__arm_neoverse_n2_v2_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__arm_neoverse_v1_default_core[] = {
{ 88931 }, /* ase_inst_spec\000instruction\000ASE operations speculatively executed\000event=0x8005\000\00000\000ASE operations speculatively executed\000 */
{ 22494 }, /* ase_spec\000instruction\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Operation speculatively executed, Advanced SIMD instruction\000 */
{ 105418 }, /* br_immed_spec\000instruction\000Branch speculatively executed, immediate branch\000event=0x78\000\00000\000Branch speculatively executed, immediate branch\000 */
{ 105555 }, /* br_indirect_spec\000instruction\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000 */
{ 411 }, /* br_mis_pred\000branch\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000Mispredicted or not predicted branch speculatively executed\000 */
{ 25146 }, /* br_mis_pred_retired\000instruction\000Instruction architecturally executed, mispredicted branch\000event=0x22\000\00000\000Instruction architecturally executed, mispredicted branch\000 */
{ 565 }, /* br_pred\000branch\000Predictable branch speculatively executed\000event=0x12\000\00000\000Predictable branch speculatively executed\000 */
{ 25018 }, /* br_retired\000instruction\000Instruction architecturally executed, branch\000event=0x21\000\00000\000Instruction architecturally executed, branch\000 */
{ 105693 }, /* br_return_spec\000instruction\000Branch speculatively executed, procedure return\000event=0x79\000\00000\000Branch speculatively executed, procedure return\000 */
{ 1311 }, /* bus_access\000bus\000Attributable Bus access\000event=0x19\000\00000\000Attributable Bus access\000 */
{ 771 }, /* bus_access_rd\000bus\000Bus access read\000event=0x60\000\00000\000Bus access read\000 */
{ 836 }, /* bus_access_wr\000bus\000Bus access write\000event=0x61\000\00000\000Bus access write\000 */
{ 721 }, /* bus_cycles\000bus\000Bus cycle\000event=0x1d\000\00000\000Bus cycle\000 */
{ 24272 }, /* cid_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000 */
{ 1389 }, /* cnt_cycles\000bus\000Constant frequency cycles\000event=0x4004\000\00000\000Constant frequency cycles.  The counter increments at a constant frequency equal to the rate of increment of the system counter, CNTPCT_EL0\000 */
{ 679 }, /* cpu_cycles\000bus\000Cycle\000event=0x11\000\00000\000Cycle\000 */
{ 23330 }, /* crypto_spec\000instruction\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Operation speculatively executed, Cryptographic instruction\000 */
{ 23705 }, /* dmb_spec\000instruction\000Barrier speculatively executed, DMB\000event=0x7e\000\00000\000Barrier speculatively executed, DMB\000 */
{ 22343 }, /* dp_spec\000instruction\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Operation speculatively executed, integer data processing\000 */
{ 23597 }, /* dsb_spec\000instruction\000Barrier speculatively executed, DSB\000event=0x7d\000\00000\000Barrier speculatively executed, DSB\000 */
{ 4626 }, /* dtlb_walk\000cache\000Access to data TLB causes a translation table walk\000event=0x34\000\00000\000Access to data TLB causes a translation table walk\000 */
{ 20002 }, /* exc_dabort\000exception\000Exception taken, Data Abort and SError\000event=0x84\000\00000\000Exception taken, Data Abort and SError\000 */
{ 20191 }, /* exc_fiq\000exception\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000 */
{ 20266 }, /* exc_hvc\000exception\000Exception taken, Hypervisor Call\000event=0x8a\000\00000\000Exception taken, Hypervisor Call\000 */
{ 20116 }, /* exc_irq\000exception\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000 */
{ 19896 }, /* exc_pabort\000exception\000Exception taken, Instruction Abort\000event=0x83\000\00000\000Exception taken, Instruction Abort\000 */
{ 33889 }, /* exc_return\000instruction\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Instruction architecturally executed, condition check pass, exception return\000 */
{ 73325 }, /* exc_smc\000exception\000Exception taken, Secure Monitor Call\000event=0x88\000\00000\000Exception taken, Secure Monitor Call\000 */
{ 19797 }, /* exc_svc\000exception\000Exception taken, Supervisor Call\000event=0x82\000\00000\000Exception taken, Supervisor Call\000 */
{ 21031 }, /* exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Exception taken\000 */
{ 20512 }, /* exc_trap_dabort\000exception\000Exception taken, Data Abort or SError not taken locally\000event=0x8c\000\00000\000Exception taken, Data Abort or SError not taken locally\000 */
{ 20915 }, /* exc_trap_fiq\000exception\000Exception taken, FIQ not taken locally\000event=0x8f\000\00000\000Exception taken, FIQ not taken locally\000 */
{ 20799 }, /* exc_trap_irq\000exception\000Exception taken, IRQ not taken locally\000event=0x8e\000\00000\000Exception taken, IRQ not taken locally\000 */
{ 20665 }, /* exc_trap_other\000exception\000Exception taken, Other traps not taken locally\000event=0x8d\000\00000\000Exception taken, Other traps not taken locally\000 */
{ 20365 }, /* exc_trap_pabort\000exception\000Exception taken, Instruction Abort not taken locally\000event=0x8b\000\00000\000Exception taken, Instruction Abort not taken locally\000 */
{ 19692 }, /* exc_undef\000exception\000Exception taken, Other synchronous\000event=0x81\000\00000\000Exception taken, Other synchronous\000 */
{ 90530 }, /* fp_fixed_ops_spec\000instruction\000Non-scalable floating-point element Operations speculatively executed\000event=0x80c1\000\00000\000Non-scalable floating-point element Operations speculatively executed\000 */
{ 90351 }, /* fp_scale_ops_spec\000instruction\000Scalable floating-point element Operations speculatively executed\000event=0x80c0\000\00000\000Scalable floating-point element Operations speculatively executed\000 */
{ 24159 }, /* inst_retired\000instruction\000Instruction architecturally executed\000event=0x8\000\00000\000Instruction architecturally executed\000 */
{ 24711 }, /* inst_spec\000instruction\000Operation speculatively executed\000event=0x1b\000\00000\000Operation speculatively executed\000 */
{ 23489 }, /* isb_spec\000instruction\000Barrier speculatively executed, ISB\000event=0x7c\000\00000\000Barrier speculatively executed, ISB\000 */
{ 4759 }, /* itlb_walk\000cache\000Access to instruction TLB that causes a translation table walk\000event=0x35\000\00000\000Access to instruction TLB that causes a translation table walk\000 */
{ 3313 }, /* l1d_cache\000cache\000Level 1 data cache access\000event=0x4\000\00000\000Level 1 data cache access\000 */
{ 1902 }, /* l1d_cache_inval\000cache\000L1D cache invalidate\000event=0x48\000\00000\000Attributable Level 1 data cache invalidate\000 */
{ 4916 }, /* l1d_cache_lmiss_rd\000cache\000Level 1 data cache long-latency read miss\000event=0x39\000\00000\000Level 1 data cache long-latency read miss.  The counter counts each memory read access counted by L1D_CACHE that incurs additional latency because it returns data from outside the Level 1 data or unified cache of this processing element\000 */
{ 1587 }, /* l1d_cache_rd\000cache\000L1D cache access, read\000event=0x40\000\00000\000Attributable Level 1 data cache access, read\000 */
{ 3224 }, /* l1d_cache_refill\000cache\000Level 1 data cache refill\000event=0x3\000\00000\000Level 1 data cache refill\000 */
{ 39214 }, /* l1d_cache_refill_inner\000cache\000L1D cache refill, inner\000event=0x44\000\00000\000Attributable Level 1 data cache refill, inner\000 */
{ 39328 }, /* l1d_cache_refill_outer\000cache\000L1D cache refill, outer\000event=0x45\000\00000\000Attributable Level 1 data cache refill, outer\000 */
{ 1793 }, /* l1d_cache_refill_rd\000cache\000L1D cache refill, read\000event=0x42\000\00000\000Attributable Level 1 data cache refill, read\000 */
{ 39103 }, /* l1d_cache_refill_wr\000cache\000L1D cache refill, write\000event=0x43\000\00000\000Attributable Level 1 data cache refill, write\000 */
{ 33516 }, /* l1d_cache_wb\000cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000Attributable Level 1 data cache write-back\000 */
{ 73186 }, /* l1d_cache_wb_clean\000cache\000L1D cache Write-Back, cleaning and coherency\000event=0x47\000\00000\000Level 1 data cache Write-Back, cleaning and coherency\000 */
{ 73065 }, /* l1d_cache_wb_victim\000cache\000L1D cache Write-Back, victim\000event=0x46\000\00000\000Attributable Level 1 data cache Write-Back, victim\000 */
{ 1689 }, /* l1d_cache_wr\000cache\000L1D cache access, write\000event=0x41\000\00000\000Attributable Level 1 data cache access, write\000 */
{ 3908 }, /* l1d_tlb\000cache\000Attributable Level 1 data or unified TLB access\000event=0x25\000\00000\000Attributable Level 1 data or unified TLB access\000 */
{ 88109 }, /* l1d_tlb_rd\000cache\000L1D tlb access, read\000event=0x4e\000\00000\000Attributable Level 1 data or unified TLB access, read\000 */
{ 3395 }, /* l1d_tlb_refill\000cache\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Attributable Level 1 data TLB refill\000 */
{ 2003 }, /* l1d_tlb_refill_rd\000cache\000L1D tlb refill, read\000event=0x4c\000\00000\000Attributable Level 1 data TLB refill, read\000 */
{ 2106 }, /* l1d_tlb_refill_wr\000cache\000L1D tlb refill, write\000event=0x4d\000\00000\000Attributable Level 1 data TLB refill, write\000 */
{ 88216 }, /* l1d_tlb_wr\000cache\000L1D tlb access, write\000event=0x4f\000\00000\000Attributable Level 1 data or unified TLB access, write\000 */
{ 3504 }, /* l1i_cache\000cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Attributable Level 1 instruction cache access\000 */
{ 5235 }, /* l1i_cache_lmiss\000cache\000Level 1 instruction cache long-latency read miss\000event=0x4006\000\00000\000Level 1 instruction cache long-latency read miss.  If the L1I_CACHE_RD event is implemented, the counter counts each access counted by L1I_CACHE_RD that incurs additional latency because it returns instructions from outside of the Level 1 instruction cache of this PE.  If the L1I_CACHE_RD event is not implemented, the counter counts each access counted by L1I_CACHE that incurs additional latency because it returns instructions from outside the Level 1 instruction cache of this PE.  The event indicates to software that the access missed in the Level 1 instruction cache and might have a significant performance impact due to the additional latency, compared to the latency of an access that hits in the Level 1 instruction cache\000 */
{ 2998 }, /* l1i_cache_refill\000cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Level 1 instruction cache refill\000 */
{ 4033 }, /* l1i_tlb\000cache\000Attributable Level 1 instruction TLB access\000event=0x26\000\00000\000Attributable Level 1 instruction TLB access\000 */
{ 3101 }, /* l1i_tlb_refill\000cache\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Attributable Level 1 instruction TLB refill\000 */
{ 3627 }, /* l2d_cache\000cache\000Level 2 data cache access\000event=0x16\000\00000\000Level 2 data cache access\000 */
{ 38627 }, /* l2d_cache_allocate\000cache\000Attributable Level 2 data cache allocation without refill\000event=0x20\000\00000\000Attributable Level 2 data cache allocation without refill\000 */
{ 2897 }, /* l2d_cache_inval\000cache\000L2D cache invalidate\000event=0x58\000\00000\000Attributable Level 2 data cache invalidate\000 */
{ 6057 }, /* l2d_cache_lmiss_rd\000cache\000Level 2 data cache long-latency read miss\000event=0x4009\000\00000\000Level 2 data cache long-latency read miss.  The counter counts each memory read access counted by L2D_CACHE that incurs additional latency because it returns data from outside the Level 2 data or unified cache of this processing element.  The event indicates to software that the access missed in the Level 2 data or unified cache and might have a significant performance impact compared to the latency of an access that hits in the Level 2 data or unified cache\000 */
{ 2211 }, /* l2d_cache_rd\000cache\000L2D cache access, read\000event=0x50\000\00000\000Attributable Level 2 data cache access, read\000 */
{ 3710 }, /* l2d_cache_refill\000cache\000Level 2 data refill\000event=0x17\000\00000\000Level 2 data refill\000 */
{ 2417 }, /* l2d_cache_refill_rd\000cache\000L2D cache refill, read\000event=0x52\000\00000\000Attributable Level 2 data cache refill, read\000 */
{ 2526 }, /* l2d_cache_refill_wr\000cache\000L2D cache refill, write\000event=0x53\000\00000\000Attributable Level 2 data cache refill, write\000 */
{ 3788 }, /* l2d_cache_wb\000cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Attributable Level 2 data cache write-back\000 */
{ 2758 }, /* l2d_cache_wb_clean\000cache\000L2D cache Write-Back, cleaning and coherency\000event=0x57\000\00000\000Level 2 data cache Write-Back, cleaning and coherency\000 */
{ 2637 }, /* l2d_cache_wb_victim\000cache\000L2D cache Write-Back, victim\000event=0x56\000\00000\000Attributable Level 2 data cache Write-Back, victim\000 */
{ 2313 }, /* l2d_cache_wr\000cache\000L2D cache access, write\000event=0x51\000\00000\000Attributable Level 2 data cache access, write\000 */
{ 4384 }, /* l2d_tlb\000cache\000Attributable Level 2 data or unified TLB access\000event=0x2f\000\00000\000Attributable Level 2 data or unified TLB access\000 */
{ 88559 }, /* l2d_tlb_rd\000cache\000L2D cache access, read\000event=0x5e\000\00000\000Attributable Level 2 data or unified TLB access, read\000 */
{ 4150 }, /* l2d_tlb_refill\000cache\000Attributable Level 2 data TLB refill\000event=0x2d\000\00000\000Attributable Level 2 data TLB refill\000 */
{ 88325 }, /* l2d_tlb_refill_rd\000cache\000L2D cache refill, read\000event=0x5c\000\00000\000Attributable Level 2 data or unified TLB refill, read\000 */
{ 88441 }, /* l2d_tlb_refill_wr\000cache\000L2D cache refill, write\000event=0x5d\000\00000\000Attributable Level 2 data or unified TLB refill, write\000 */
{ 88668 }, /* l2d_tlb_wr\000cache\000L2D cache access, write\000event=0x5f\000\00000\000Attributable Level 2 data or unified TLB access, write\000 */
{ 38783 }, /* l3d_cache\000cache\000Attributable Level 3 data cache access\000event=0x2b\000\00000\000Attributable Level 3 data cache access\000 */
{ 61550 }, /* l3d_cache_allocate\000cache\000Attributable Level 3 data cache allocation without refill\000event=0x29\000\00000\000Attributable Level 3 data cache allocation without refill\000 */
{ 44452 }, /* l3d_cache_lmiss_rd\000cache\000Level 3 data cache long-latency read miss\000event=0x400b\000\00000\000Level 3 data cache long-latency read miss.  The counter counts each memory read access counted by L3D_CACHE that incurs additional latency because it returns data from outside the Level 3 data or unified cache of this processing element.  The event indicates to software that the access missed in the Level 3 data or unified cache and might have a significant performance impact compared to the latency of an access that hits in the Level 3 data or unified cache\000 */
{ 39442 }, /* l3d_cache_rd\000cache\000Attributable Level 3 data or unified cache access, read\000event=0xa0\000\00000\000Attributable Level 3 data or unified cache access, read\000 */
{ 61706 }, /* l3d_cache_refill\000cache\000Attributable Level 3 data cache refill\000event=0x2a\000\00000\000Attributable Level 3 data cache refill\000 */
{ 21982 }, /* ld_spec\000instruction\000Operation speculatively executed, load\000event=0x70\000\00000\000Operation speculatively executed, load\000 */
{ 73432 }, /* ldrex_spec\000instruction\000Exclusive operation speculatively executed, LDREX or LDX\000event=0x6c\000\00000\000Exclusive operation speculatively executed, LDREX or LDX\000 */
{ 39009 }, /* ll_cache_miss_rd\000cache\000Last level cache miss, read\000event=0x37\000\00000\000Last level cache miss, read\000 */
{ 38892 }, /* ll_cache_rd\000cache\000Attributable Last level cache memory read\000event=0x36\000\00000\000Attributable Last level cache memory read\000 */
{ 27136 }, /* mem_access\000memory\000Data memory access\000event=0x13\000\00000\000Data memory access\000 */
{ 26460 }, /* mem_access_rd\000memory\000Data memory access, read\000event=0x66\000\00000\000Data memory access, read\000 */
{ 26546 }, /* mem_access_wr\000memory\000Data memory access, write\000event=0x67\000\00000\000Data memory access, write\000 */
{ 33636 }, /* memory_error\000exception\000Local memory error\000event=0x1a\000\00000\000Local memory error\000 */
{ 25309 }, /* op_retired\000instruction\000Micro-operation architecturally executed\000event=0x3a\000\00000\000Micro-operation architecturally executed.  The counter counts each operation counted by OP_SPEC that would be executed in a simple sequential execution of the program\000 */
{ 21665 }, /* op_spec\000instruction\000Micro-operation speculatively executed\000event=0x3b\000\00000\000Micro-operation speculatively executed.  The counter counts the number of operations executed by the processing element, including those that are executed speculatively and would not be executed in a simple sequential execution of the program\000 */
{ 22808 }, /* pc_write_spec\000instruction\000Operation speculatively executed, software change of the PC\000event=0x76\000\00000\000Operation speculatively executed, software change of the PC\000 */
{ 23813 }, /* rc_ld_spec\000instruction\000Release consistency operation speculatively executed, Load-Acquire\000event=0x90\000\00000\000Release consistency operation speculatively executed, Load-Acquire\000 */
{ 23985 }, /* rc_st_spec\000instruction\000Release consistency operation speculatively executed, Store-Release\000event=0x91\000\00000\000Release consistency operation speculatively executed, Store-Release\000 */
{ 90717 }, /* remote_access\000memory\000Access to another socket in a multi-socket system\000event=0x31\000\00000\000Access to another socket in a multi-socket system\000 */
{ 22095 }, /* st_spec\000instruction\000Operation speculatively executed, store\000event=0x71\000\00000\000Operation speculatively executed, store\000 */
{ 46833 }, /* stall\000pipeline\000No operation sent for execution\000event=0x3c\000\00000\000No operation sent for execution.  The counter counts every attributable cycle on which no attributable instruction or operation was sent for execution on this processing element\000 */
{ 27892 }, /* stall_backend\000pipeline\000No operation issued due to the backend\000event=0x24\000\00000\000No operation issued due to the backend\000 */
{ 29344 }, /* stall_backend_mem\000pipeline\000Memory stall cycles\000event=0x4005\000\00000\000Memory stall cycles.  The counter counts each cycle counted by STALL_BACKEND where there is a cache miss in the last level of cache within the processing element clock domain\000 */
{ 46706 }, /* stall_frontend\000pipeline\000No operation issued because of the frontend\000event=0x23\000\00000\000No operation issued because of the frontend\000 */
{ 29039 }, /* stall_slot\000pipeline\000No operation sent for execution on a slot\000event=0x3f\000\00000\000No operation sent for execution on a slot.  The counter counts on each attributable cycle the number of instruction or operation slots that were not occupied by an instruction or operation attributable to the processing element\000 */
{ 28291 }, /* stall_slot_backend\000pipeline\000No operation sent for execution on a slot due to the backend\000event=0x3d\000\00000\000No operation sent for execution on a slot due to the backend.  Counts each slot counted by STALL_SLOT where no attributable instruction or operation was sent for execution because the backend is unable to accept it\000 */
{ 47073 }, /* stall_slot_frontend\000pipeline\000No operation sent for execution on a slot due to the frontend\000event=0x3e\000\00000\000No operation sent for execution on a slot due to the frontend.  Counts each slot counted by STALL_SLOT where no attributable instruction or operation was sent for execution because there was no attributable instruction or operation available to issue from the processing element from the frontend for the slot\000 */
{ 73751 }, /* strex_fail_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX fail\000event=0x6e\000\00000\000Exclusive operation speculatively executed, STREX or STX fail\000 */
{ 73584 }, /* strex_pass_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX pass\000event=0x6d\000\00000\000Exclusive operation speculatively executed, STREX or STX pass\000 */
{ 88779 }, /* strex_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX\000event=0x6f\000\00000\000Exclusive operation speculatively executed, STREX or STX\000 */
{ 45129 }, /* sve_inst_spec\000instruction\000SVE operations speculatively executed\000event=0x8006\000\00000\000SVE operations speculatively executed\000 */
{ 90146 }, /* sve_ldff_fault_spec\000instruction\000SVE first-fault load operations speculatively executed which set FFR bit to 0\000event=0x80bd\000\00000\000SVE first-fault load operations speculatively executed which set FFR bit to 0\000 */
{ 89993 }, /* sve_ldff_spec\000instruction\000SVE First-fault load Operations speculatively executed\000event=0x80bc\000\00000\000SVE First-fault load Operations speculatively executed\000 */
{ 89191 }, /* sve_pred_empty_spec\000instruction\000SVE predicated operations with no active predicates speculatively executed\000event=0x8075\000\00000\000SVE predicated operations with no active predicates speculatively executed\000 */
{ 89390 }, /* sve_pred_full_spec\000instruction\000SVE predicated operations speculatively executed with all active predicates\000event=0x8076\000\00000\000SVE predicated operations speculatively executed with all active predicates\000 */
{ 89590 }, /* sve_pred_partial_spec\000instruction\000SVE predicated operations speculatively executed with partially active predicates\000event=0x8077\000\00000\000SVE predicated operations speculatively executed with partially active predicates\000 */
{ 89050 }, /* sve_pred_spec\000instruction\000SVE predicated Operations speculatively executed\000event=0x8074\000\00000\000SVE predicated Operations speculatively executed\000 */
{ 21286 }, /* sw_incr\000instruction\000Instruction architecturally executed, Condition code check pass, software increment\000event=0\000\00000\000Instruction architecturally executed, Condition code check pass, software increment\000 */
{ 24814 }, /* ttbr_write_retired\000instruction\000Instruction architecturally executed, Condition code check pass, write to TTBR\000event=0x1c\000\00000\000Instruction architecturally executed, Condition code check pass, write to TTBR\000 */
{ 26634 }, /* unaligned_ld_spec\000memory\000Unaligned access, read\000event=0x68\000\00000\000Unaligned access, read\000 */
{ 26808 }, /* unaligned_ldst_spec\000memory\000Unaligned access\000event=0x6a\000\00000\000Unaligned access\000 */
{ 26720 }, /* unaligned_st_spec\000memory\000Unaligned access, write\000event=0x69\000\00000\000Unaligned access, write\000 */
{ 22650 }, /* vfp_spec\000instruction\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Operation speculatively executed, floating-point instruction\000 */

};

const struct pmu_table_entry pmu_events__arm_neoverse_v1[] = {
{
     .entries = pmu_events__arm_neoverse_v1_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__arm_neoverse_v1_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__cavium_thunderx2_default_core[] = {
{ 148345 }, /* bus_access_rd\000core imp def\000Bus access read\000event=0x60\000\00000\000Bus access read\000 */
{ 148419 }, /* bus_access_wr\000core imp def\000Bus access write\000event=0x61\000\00000\000Bus access write\000 */
{ 149268 }, /* exc_dabort\000core imp def\000Exception taken, Data Abort and SError\000event=0x84\000\00000\000Exception taken, Data Abort and SError\000 */
{ 149463 }, /* exc_fiq\000core imp def\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000 */
{ 149651 }, /* exc_hvc\000core imp def\000Exception taken, Hypervisor Call\000event=0x8a\000\00000\000Exception taken, Hypervisor Call\000 */
{ 149385 }, /* exc_irq\000core imp def\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000 */
{ 149159 }, /* exc_pabort\000core imp def\000Exception taken, Instruction Abort\000event=0x83\000\00000\000Exception taken, Instruction Abort\000 */
{ 149541 }, /* exc_smc\000core imp def\000Exception taken, Secure Monitor Call\000event=0x88\000\00000\000Exception taken, Secure Monitor Call\000 */
{ 149057 }, /* exc_svc\000core imp def\000Exception taken, Supervisor Call\000event=0x82\000\00000\000Exception taken, Supervisor Call\000 */
{ 149903 }, /* exc_trap_dabort\000core imp def\000Exception taken, Data Abort or SError not taken locally\000event=0x8c\000\00000\000Exception taken, Data Abort or SError not taken locally\000 */
{ 150315 }, /* exc_trap_fiq\000core imp def\000Exception taken, FIQ not taken locally\000event=0x8f\000\00000\000Exception taken, FIQ not taken locally\000 */
{ 150196 }, /* exc_trap_irq\000core imp def\000Exception taken, IRQ not taken locally\000event=0x8e\000\00000\000Exception taken, IRQ not taken locally\000 */
{ 150059 }, /* exc_trap_other\000core imp def\000Exception taken, Other traps not taken locally\000event=0x8d\000\00000\000Exception taken, Other traps not taken locally\000 */
{ 149753 }, /* exc_trap_pabort\000core imp def\000Exception taken, Instruction Abort not taken locally\000event=0x8b\000\00000\000Exception taken, Instruction Abort not taken locally\000 */
{ 148949 }, /* exc_undef\000core imp def\000Exception taken, Other synchronous\000event=0x81\000\00000\000Exception taken, Other synchronous\000 */
{ 147303 }, /* l1d_cache_inval\000core imp def\000L1D cache invalidate\000event=0x48\000\00000\000Attributable Level 1 data cache invalidate\000 */
{ 146333 }, /* l1d_cache_rd\000core imp def\000L1D cache access, read\000event=0x40\000\00000\000Attributable Level 1 data cache access, read\000 */
{ 146787 }, /* l1d_cache_refill_inner\000core imp def\000L1D cache refill, inner\000event=0x44\000\00000\000Attributable Level 1 data cache refill, inner\000 */
{ 146908 }, /* l1d_cache_refill_outer\000core imp def\000L1D cache refill, outer\000event=0x45\000\00000\000Attributable Level 1 data cache refill, outer\000 */
{ 146553 }, /* l1d_cache_refill_rd\000core imp def\000L1D cache refill, read\000event=0x42\000\00000\000Attributable Level 1 data cache refill, read\000 */
{ 146669 }, /* l1d_cache_refill_wr\000core imp def\000L1D cache refill, write\000event=0x43\000\00000\000Attributable Level 1 data cache refill, write\000 */
{ 147157 }, /* l1d_cache_wb_clean\000core imp def\000L1D cache Write-Back, cleaning and coherency\000event=0x47\000\00000\000Level 1 data cache Write-Back, cleaning and coherency\000 */
{ 147029 }, /* l1d_cache_wb_victim\000core imp def\000L1D cache Write-Back, victim\000event=0x46\000\00000\000Attributable Level 1 data cache Write-Back, victim\000 */
{ 146442 }, /* l1d_cache_wr\000core imp def\000L1D cache access, write\000event=0x41\000\00000\000Attributable Level 1 data cache access, write\000 */
{ 147633 }, /* l1d_tlb_rd\000core imp def\000L1D tlb access, read\000event=0x4e\000\00000\000Attributable Level 1 data or unified TLB access, read\000 */
{ 147411 }, /* l1d_tlb_refill_rd\000core imp def\000L1D tlb refill, read\000event=0x4c\000\00000\000Attributable Level 1 data TLB refill, read\000 */
{ 147521 }, /* l1d_tlb_refill_wr\000core imp def\000L1D tlb refill, write\000event=0x4d\000\00000\000Attributable Level 1 data TLB refill, write\000 */
{ 147747 }, /* l1d_tlb_wr\000core imp def\000L1D tlb access, write\000event=0x4f\000\00000\000Attributable Level 1 data or unified TLB access, write\000 */
{ 148111 }, /* l2d_tlb_rd\000core imp def\000L2D cache access, read\000event=0x5e\000\00000\000Attributable Level 2 data or unified TLB access, read\000 */
{ 147863 }, /* l2d_tlb_refill_rd\000core imp def\000L2D cache refill, read\000event=0x5c\000\00000\000Attributable Level 2 data or unified TLB refill, read\000 */
{ 147986 }, /* l2d_tlb_refill_wr\000core imp def\000L2D cache refill, write\000event=0x5d\000\00000\000Attributable Level 2 data or unified TLB refill, write\000 */
{ 148227 }, /* l2d_tlb_wr\000core imp def\000L2D cache access, write\000event=0x5f\000\00000\000Attributable Level 2 data or unified TLB access, write\000 */
{ 148495 }, /* mem_access_rd\000core imp def\000Data memory access, read\000event=0x66\000\00000\000Data memory access, read\000 */
{ 148587 }, /* mem_access_wr\000core imp def\000Data memory access, write\000event=0x67\000\00000\000Data memory access, write\000 */
{ 148681 }, /* unaligned_ld_spec\000core imp def\000Unaligned access, read\000event=0x68\000\00000\000Unaligned access, read\000 */
{ 148867 }, /* unaligned_ldst_spec\000core imp def\000Unaligned access\000event=0x6a\000\00000\000Unaligned access\000 */
{ 148773 }, /* unaligned_st_spec\000core imp def\000Unaligned access, write\000event=0x69\000\00000\000Unaligned access, write\000 */

};

const struct pmu_table_entry pmu_events__cavium_thunderx2[] = {
{
     .entries = pmu_events__cavium_thunderx2_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__cavium_thunderx2_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__freescale_imx8mm_sys_imx8_ddr[] = {
{ 150443 }, /* imx8mm_ddr.cycles\000ddrc\000ddr cycles event\000event=0\000i.MX8MM\00000\000\000 */
{ 150651 }, /* imx8mm_ddr.read\000ddrc\000ddr read event\000event=0x35\000i.MX8MM\00000\000\000 */
{ 150503 }, /* imx8mm_ddr.read_cycles\000ddrc\000ddr read-cycles event\000event=0x2a\000i.MX8MM\00000\000\000 */
{ 150710 }, /* imx8mm_ddr.write\000ddrc\000ddr write event\000event=0x38\000i.MX8MM\00000\000\000 */
{ 150576 }, /* imx8mm_ddr.write_cycles\000ddrc\000ddr write-cycles event\000event=0x2b\000i.MX8MM\00000\000\000 */

};

const struct pmu_table_entry pmu_events__freescale_imx8mm_sys[] = {
{
     .entries = pmu_events__freescale_imx8mm_sys_imx8_ddr,
     .num_entries = ARRAY_SIZE(pmu_events__freescale_imx8mm_sys_imx8_ddr),
     .pmu_name = { 150434 /* imx8_ddr\000 */ },
},
};

static const struct compact_pmu_event pmu_metrics__freescale_imx8mm_sys_imx8_ddr[] = {
{ 211860 }, /* imx8mm_ddr_read.all\000\000imx8mm_ddr.read_cycles * 4 * 4\000\000bytes all masters read from ddr based on read-cycles event\000\0009.765625e-4KB\000i.MX8MM\000\000\00000 */
{ 211999 }, /* imx8mm_ddr_write.all\000\000imx8mm_ddr.write_cycles * 4 * 4\000\000bytes all masters write to ddr based on write-cycles event\000\0009.765625e-4KB\000i.MX8MM\000\000\00000 */

};

const struct pmu_table_entry pmu_metrics__freescale_imx8mm_sys[] = {
{
     .entries = pmu_metrics__freescale_imx8mm_sys_imx8_ddr,
     .num_entries = ARRAY_SIZE(pmu_metrics__freescale_imx8mm_sys_imx8_ddr),
     .pmu_name = { 150434 /* imx8_ddr\000 */ },
},
};

static const struct compact_pmu_event pmu_events__freescale_imx8mn_sys_imx8_ddr[] = {
{ 150771 }, /* imx8mn_ddr.cycles\000ddrc\000ddr cycles event\000event=0\000i.MX8MN\00000\000\000 */
{ 150979 }, /* imx8mn_ddr.read\000ddrc\000ddr read event\000event=0x35\000i.MX8MN\00000\000\000 */
{ 150831 }, /* imx8mn_ddr.read_cycles\000ddrc\000ddr read-cycles event\000event=0x2a\000i.MX8MN\00000\000\000 */
{ 151038 }, /* imx8mn_ddr.write\000ddrc\000ddr write event\000event=0x38\000i.MX8MN\00000\000\000 */
{ 150904 }, /* imx8mn_ddr.write_cycles\000ddrc\000ddr write-cycles event\000event=0x2b\000i.MX8MN\00000\000\000 */

};

const struct pmu_table_entry pmu_events__freescale_imx8mn_sys[] = {
{
     .entries = pmu_events__freescale_imx8mn_sys_imx8_ddr,
     .num_entries = ARRAY_SIZE(pmu_events__freescale_imx8mn_sys_imx8_ddr),
     .pmu_name = { 150434 /* imx8_ddr\000 */ },
},
};

static const struct compact_pmu_event pmu_metrics__freescale_imx8mn_sys_imx8_ddr[] = {
{ 212140 }, /* imx8mn_ddr_read.all\000\000imx8mn_ddr.read_cycles * 4 * 2\000\000bytes all masters read from ddr based on read-cycles event\000\0009.765625e-4KB\000i.MX8MN\000\000\00000 */
{ 212279 }, /* imx8mn_ddr_write.all\000\000imx8mn_ddr.write_cycles * 4 * 2\000\000bytes all masters write to ddr based on write-cycles event\000\0009.765625e-4KB\000i.MX8MN\000\000\00000 */

};

const struct pmu_table_entry pmu_metrics__freescale_imx8mn_sys[] = {
{
     .entries = pmu_metrics__freescale_imx8mn_sys_imx8_ddr,
     .num_entries = ARRAY_SIZE(pmu_metrics__freescale_imx8mn_sys_imx8_ddr),
     .pmu_name = { 150434 /* imx8_ddr\000 */ },
},
};

static const struct compact_pmu_event pmu_events__freescale_imx8mp_sys_imx8_ddr[] = {
{ 151099 }, /* imx8mp_ddr.cycles\000ddrc\000ddr cycles event\000event=0\000i.MX8MP\00000\000\000 */
{ 151307 }, /* imx8mp_ddr.read\000ddrc\000ddr read event\000event=0x35\000i.MX8MP\00000\000\000 */
{ 151159 }, /* imx8mp_ddr.read_cycles\000ddrc\000ddr read-cycles event\000event=0x2a\000i.MX8MP\00000\000\000 */
{ 151366 }, /* imx8mp_ddr.write\000ddrc\000ddr write event\000event=0x38\000i.MX8MP\00000\000\000 */
{ 151232 }, /* imx8mp_ddr.write_cycles\000ddrc\000ddr write-cycles event\000event=0x2b\000i.MX8MP\00000\000\000 */

};

const struct pmu_table_entry pmu_events__freescale_imx8mp_sys[] = {
{
     .entries = pmu_events__freescale_imx8mp_sys_imx8_ddr,
     .num_entries = ARRAY_SIZE(pmu_events__freescale_imx8mp_sys_imx8_ddr),
     .pmu_name = { 150434 /* imx8_ddr\000 */ },
},
};

static const struct compact_pmu_event pmu_metrics__freescale_imx8mp_sys_imx8_ddr[] = {
{ 213543 }, /* imx8mp_ddr_read.2d\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0071@\000\000bytes of gpu 2d read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 213274 }, /* imx8mp_ddr_read.3d\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0070@\000\000bytes of gpu 3d read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 212701 }, /* imx8mp_ddr_read.a53\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0000@\000\000bytes of a53 core read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 212420 }, /* imx8mp_ddr_read.all\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0xffff\\,axi_id\\=0x0000@\000\000bytes of all masters read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 218928 }, /* imx8mp_ddr_read.audio_dsp\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0041@\000\000bytes of audio dsp read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 219530 }, /* imx8mp_ddr_read.audio_sdma2_burst\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0063@\000\000bytes of audio sdma2_burst read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 219217 }, /* imx8mp_ddr_read.audio_sdma2_per\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0062@\000\000bytes of audio sdma2_per read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 220164 }, /* imx8mp_ddr_read.audio_sdma3_burst\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0065@\000\000bytes of audio sdma3_burst read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 219851 }, /* imx8mp_ddr_read.audio_sdma3_per\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0064@\000\000bytes of audio sdma3_per read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 220485 }, /* imx8mp_ddr_read.audio_sdma_pif\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0066@\000\000bytes of audio sdma_pif read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 215823 }, /* imx8mp_ddr_read.dewarp\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x006f@\000\000bytes of display dewarp read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 218627 }, /* imx8mp_ddr_read.hdmi_hdcp\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0076@\000\000bytes of hdmi_tx tx_hdcp read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 218021 }, /* imx8mp_ddr_read.hdmi_hrv_mwr\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0074@\000\000bytes of hdmi_tx hrv_mwr read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 218328 }, /* imx8mp_ddr_read.hdmi_lcdif\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0075@\000\000bytes of hdmi_tx lcdif read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 214398 }, /* imx8mp_ddr_read.isi1\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x006a@\000\000bytes of display isi1 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 214683 }, /* imx8mp_ddr_read.isi2\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x006b@\000\000bytes of display isi2 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 214968 }, /* imx8mp_ddr_read.isi3\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x006c@\000\000bytes of display isi3 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 215253 }, /* imx8mp_ddr_read.isp1\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x006d@\000\000bytes of display isp1 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 215538 }, /* imx8mp_ddr_read.isp2\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x006e@\000\000bytes of display isp2 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 213812 }, /* imx8mp_ddr_read.lcdif1\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0068@\000\000bytes of display lcdif1 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 214105 }, /* imx8mp_ddr_read.lcdif2\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0069@\000\000bytes of display lcdif2 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 216923 }, /* imx8mp_ddr_read.npu\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0073@\000\000bytes of npu read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 217746 }, /* imx8mp_ddr_read.pci\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x007a@\000\000bytes of hsio pci read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 212976 }, /* imx8mp_ddr_read.supermix\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x000f\\,axi_id\\=0x0020@\000\000bytes of supermix(m7) core read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 217188 }, /* imx8mp_ddr_read.usb1\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0078@\000\000bytes of hsio usb1 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 217467 }, /* imx8mp_ddr_read.usb2\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x0079@\000\000bytes of hsio usb2 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 216116 }, /* imx8mp_ddr_read.vpu1\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x007c@\000\000bytes of vpu1 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 216385 }, /* imx8mp_ddr_read.vpu2\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x007d@\000\000bytes of vpu2 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 216654 }, /* imx8mp_ddr_read.vpu3\000\000imx8_ddr0@axid\\-read\\,axi_mask\\=0x0000\\,axi_id\\=0x007e@\000\000bytes of vpu3 read from ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 213677 }, /* imx8mp_ddr_write.2d\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0071@\000\000bytes of gpu 2d write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 213408 }, /* imx8mp_ddr_write.3d\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0070@\000\000bytes of gpu 3d write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 212838 }, /* imx8mp_ddr_write.a53\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0000@\000\000bytes of a53 core write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 212560 }, /* imx8mp_ddr_write.all\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0xffff\\,axi_id\\=0x0000@\000\000bytes of all masters write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 219072 }, /* imx8mp_ddr_write.audio_dsp\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0041@\000\000bytes of audio dsp write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 219690 }, /* imx8mp_ddr_write.audio_sdma2_burst\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0063@\000\000bytes of audio sdma2_burst write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 219373 }, /* imx8mp_ddr_write.audio_sdma2_per\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0062@\000\000bytes of audio sdma2_per write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 220324 }, /* imx8mp_ddr_write.audio_sdma3_burst\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0065@\000\000bytes of audio sdma3_burst write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 220007 }, /* imx8mp_ddr_write.audio_sdma3_per\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0064@\000\000bytes of audio sdma3_per write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 220639 }, /* imx8mp_ddr_write.audio_sdma_pif\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0066@\000\000bytes of audio sdma_pif write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 215969 }, /* imx8mp_ddr_write.dewarp\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x006f@\000\000bytes of display dewarp write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 218777 }, /* imx8mp_ddr_write.hdmi_hdcp\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0076@\000\000bytes of hdmi_tx tx_hdcp write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 218174 }, /* imx8mp_ddr_write.hdmi_hrv_mwr\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0074@\000\000bytes of hdmi_tx hrv_mwr write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 218477 }, /* imx8mp_ddr_write.hdmi_lcdif\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0075@\000\000bytes of hdmi_tx lcdif write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 214540 }, /* imx8mp_ddr_write.isi1\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x006a@\000\000bytes of display isi1 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 214825 }, /* imx8mp_ddr_write.isi2\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x006b@\000\000bytes of display isi2 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 215110 }, /* imx8mp_ddr_write.isi3\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x006c@\000\000bytes of display isi3 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 215395 }, /* imx8mp_ddr_write.isp1\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x006d@\000\000bytes of display isp1 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 215680 }, /* imx8mp_ddr_write.isp2\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x006e@\000\000bytes of display isp2 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 213958 }, /* imx8mp_ddr_write.lcdif1\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0068@\000\000bytes of display lcdif1 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 214251 }, /* imx8mp_ddr_write.lcdif2\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0069@\000\000bytes of display lcdif2 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 217055 }, /* imx8mp_ddr_write.npu\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0073@\000\000bytes of npu write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 217883 }, /* imx8mp_ddr_write.pci\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x007a@\000\000bytes of hsio pci write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 213127 }, /* imx8mp_ddr_write.supermix\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x000f\\,axi_id\\=0x0020@\000\000bytes of supermix(m7) write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 217327 }, /* imx8mp_ddr_write.usb1\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0078@\000\000bytes of hsio usb1 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 217606 }, /* imx8mp_ddr_write.usb2\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x0079@\000\000bytes of hsio usb2 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 216250 }, /* imx8mp_ddr_write.vpu1\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x007c@\000\000bytes of vpu1 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 216519 }, /* imx8mp_ddr_write.vpu2\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x007d@\000\000bytes of vpu2 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */
{ 216788 }, /* imx8mp_ddr_write.vpu3\000\000imx8_ddr0@axid\\-write\\,axi_mask\\=0x0000\\,axi_id\\=0x007e@\000\000bytes of vpu3 write to ddr\000\0009.765625e-4KB\000i.MX8MP\000\000\00000 */

};

const struct pmu_table_entry pmu_metrics__freescale_imx8mp_sys[] = {
{
     .entries = pmu_metrics__freescale_imx8mp_sys_imx8_ddr,
     .num_entries = ARRAY_SIZE(pmu_metrics__freescale_imx8mp_sys_imx8_ddr),
     .pmu_name = { 150434 /* imx8_ddr\000 */ },
},
};

static const struct compact_pmu_event pmu_events__freescale_imx8mq_sys_imx8_ddr[] = {
{ 151427 }, /* imx8mq_ddr.cycles\000ddrc\000ddr cycles event\000event=0\000i.MX8MQ\00000\000\000 */
{ 151635 }, /* imx8mq_ddr.read\000ddrc\000ddr read event\000event=0x35\000i.MX8MQ\00000\000\000 */
{ 151487 }, /* imx8mq_ddr.read_cycles\000ddrc\000ddr read-cycles event\000event=0x2a\000i.MX8MQ\00000\000\000 */
{ 151694 }, /* imx8mq_ddr.write\000ddrc\000ddr write event\000event=0x38\000i.MX8MQ\00000\000\000 */
{ 151560 }, /* imx8mq_ddr.write_cycles\000ddrc\000ddr write-cycles event\000event=0x2b\000i.MX8MQ\00000\000\000 */

};

const struct pmu_table_entry pmu_events__freescale_imx8mq_sys[] = {
{
     .entries = pmu_events__freescale_imx8mq_sys_imx8_ddr,
     .num_entries = ARRAY_SIZE(pmu_events__freescale_imx8mq_sys_imx8_ddr),
     .pmu_name = { 150434 /* imx8_ddr\000 */ },
},
};

static const struct compact_pmu_event pmu_metrics__freescale_imx8mq_sys_imx8_ddr[] = {
{ 220794 }, /* imx8mq_ddr_read.all\000\000imx8mq_ddr.read_cycles * 4 * 4\000\000bytes all masters read from ddr based on read-cycles event\000\0009.765625e-4KB\000i.MX8MQ\000\000\00000 */
{ 220933 }, /* imx8mq_ddr_write.all\000\000imx8mq_ddr.write_cycles * 4 * 4\000\000bytes all masters write to ddr based on write-cycles event\000\0009.765625e-4KB\000i.MX8MQ\000\000\00000 */

};

const struct pmu_table_entry pmu_metrics__freescale_imx8mq_sys[] = {
{
     .entries = pmu_metrics__freescale_imx8mq_sys_imx8_ddr,
     .num_entries = ARRAY_SIZE(pmu_metrics__freescale_imx8mq_sys_imx8_ddr),
     .pmu_name = { 150434 /* imx8_ddr\000 */ },
},
};

static const struct compact_pmu_event pmu_events__freescale_yitian710_sys_ali_drw[] = {
{ 156228 }, /* chi_rxdat\000ali_drw\000A packet at CHI RXDAT interface (write data)\000config=0x3b\000ali_drw_pmu\00000\000\000 */
{ 156319 }, /* chi_rxrsp\000ali_drw\000A packet at CHI RXRSP interface\000config=0x3c\000ali_drw_pmu\00000\000\000 */
{ 156138 }, /* chi_txdat\000ali_drw\000A packet at CHI TXDAT interface (read data)\000config=0x3a\000ali_drw_pmu\00000\000\000 */
{ 156050 }, /* chi_txreq\000ali_drw\000A packet at CHI TXREQ interface (request)\000config=0x39\000ali_drw_pmu\00000\000\000 */
{ 156469 }, /* ddr_cycles\000ali_drw\000The ddr cycles\000config=0x80\000ali_drw_pmu\00000\000\000 */
{ 152339 }, /* dfi_rd_data_cycles\000ali_drw\000A read data cycle at DFI interface (to DRAM)\000config=0x8\000ali_drw_pmu\00000\000\000 */
{ 152239 }, /* dfi_wr_data_cycles\000ali_drw\000A write data cycle at DFI interface (to DRAM)\000config=0x7\000ali_drw_pmu\00000\000\000 */
{ 152135 }, /* hif_hi_pri_rd\000ali_drw\000A high priority Read at HIF interface. The unit is 64B\000config=0x4\000ali_drw_pmu\00000\000\000 */
{ 151949 }, /* hif_rd\000ali_drw\000A Read Op at HIF interface. The unit is 64B\000config=0x2\000ali_drw_pmu\00000\000\000 */
{ 151763 }, /* hif_rd_or_wr\000ali_drw\000A Write or Read Op at HIF interface. The unit is 64B\000config=0\000ali_drw_pmu\00000\000\000 */
{ 152035 }, /* hif_rmw\000ali_drw\000A Read-Modify-Write Op at HIF interface. The unit is 64B\000config=0x3\000ali_drw_pmu\00000\000\000 */
{ 151862 }, /* hif_wr\000ali_drw\000A Write Op at HIF interface. The unit is 64B\000config=0x1\000ali_drw_pmu\00000\000\000 */
{ 152438 }, /* hpr_xact_when_critical\000ali_drw\000A high priority read becomes critical\000config=0x9\000ali_drw_pmu\00000\000\000 */
{ 152534 }, /* lpr_xact_when_critical\000ali_drw\000A low priority read becomes critical\000config=0xa\000ali_drw_pmu\00000\000\000 */
{ 152711 }, /* op_is_activate\000ali_drw\000An Activate(ACT) command to DRAM\000config=0xc\000ali_drw_pmu\00000\000\000 */
{ 155083 }, /* op_is_crit_ref\000ali_drw\000A critical auto-refresh(REF) command to DRAM\000config=0x2b\000ali_drw_pmu\00000\000\000 */
{ 155601 }, /* op_is_dqsosc_mpc\000ali_drw\000A DQS Oscillator MPC command to DRAM\000config=0x34\000ali_drw_pmu\00000\000\000 */
{ 155691 }, /* op_is_dqsosc_mrr\000ali_drw\000A DQS Oscillator MRR command to DRAM\000config=0x35\000ali_drw_pmu\00000\000\000 */
{ 154236 }, /* op_is_enter_powerdown_rk0\000ali_drw\000Rank0 enters power-down(PDE)\000config=0x1e\000ali_drw_pmu\00000\000\000 */
{ 154327 }, /* op_is_enter_powerdown_rk1\000ali_drw\000Rank1 enters power-down(PDE)\000config=0x1f\000ali_drw_pmu\00000\000\000 */
{ 154418 }, /* op_is_enter_powerdown_rk2\000ali_drw\000Rank2 enters power-down(PDE)\000config=0x20\000ali_drw_pmu\00000\000\000 */
{ 154509 }, /* op_is_enter_powerdown_rk3\000ali_drw\000Rank3 enters power-down(PDE)\000config=0x21\000ali_drw_pmu\00000\000\000 */
{ 153872 }, /* op_is_enter_selfref_rk0\000ali_drw\000Rank0 enters self-refresh(SRE)\000config=0x1a\000ali_drw_pmu\00000\000\000 */
{ 153963 }, /* op_is_enter_selfref_rk1\000ali_drw\000Rank1 enters self-refresh(SRE)\000config=0x1b\000ali_drw_pmu\00000\000\000 */
{ 154054 }, /* op_is_enter_selfref_rk2\000ali_drw\000Rank2 enters self-refresh(SRE)\000config=0x1c\000ali_drw_pmu\00000\000\000 */
{ 154145 }, /* op_is_enter_selfref_rk3\000ali_drw\000Rank3 enters self-refresh(SRE)\000config=0x1d\000ali_drw_pmu\00000\000\000 */
{ 155179 }, /* op_is_load_mode\000ali_drw\000An MRR or MRW command to DRAM\000config=0x2d\000ali_drw_pmu\00000\000\000 */
{ 153119 }, /* op_is_mwr\000ali_drw\000A Masked Write command to DRAM\000config=0x11\000ali_drw_pmu\00000\000\000 */
{ 153196 }, /* op_is_precharge\000ali_drw\000A Precharge(PRE) command to DRAM\000config=0x12\000ali_drw_pmu\00000\000\000 */
{ 152975 }, /* op_is_rd\000ali_drw\000A Read CAS command to DRAM\000config=0xf\000ali_drw_pmu\00000\000\000 */
{ 152880 }, /* op_is_rd_activate\000ali_drw\000An Activate(ACT) command for read to DRAM\000config=0xe\000ali_drw_pmu\00000\000\000 */
{ 152794 }, /* op_is_rd_or_wr\000ali_drw\000A Read or Write CAS command to DRAM\000config=0xd\000ali_drw_pmu\00000\000\000 */
{ 154996 }, /* op_is_refresh\000ali_drw\000An auto-refresh(REF) command to DRAM\000config=0x2a\000ali_drw_pmu\00000\000\000 */
{ 155781 }, /* op_is_tcr_mrr\000ali_drw\000A Temperature Compensated Refresh(TCR) MRR command to DRAM\000config=0x36\000ali_drw_pmu\00000\000\000 */
{ 153046 }, /* op_is_wr\000ali_drw\000A Write CAS command to DRAM\000config=0x10\000ali_drw_pmu\00000\000\000 */
{ 155261 }, /* op_is_zqcl\000ali_drw\000A ZQCal command to DRAM\000config=0x2e\000ali_drw_pmu\00000\000\000 */
{ 155970 }, /* op_is_zqlatch\000ali_drw\000A ZQCal Latch command to DRAM\000config=0x38\000ali_drw_pmu\00000\000\000 */
{ 155890 }, /* op_is_zqstart\000ali_drw\000A ZQCal Start command to DRAM\000config=0x37\000ali_drw_pmu\00000\000\000 */
{ 153379 }, /* precharge_for_other\000ali_drw\000A Precharge(PRE) required by other conditions\000config=0x14\000ali_drw_pmu\00000\000\000 */
{ 153281 }, /* precharge_for_rdwr\000ali_drw\000A Precharge(PRE) required by read or write\000config=0x13\000ali_drw_pmu\00000\000\000 */
{ 153725 }, /* raw_hazard\000ali_drw\000A Read-After-Write hazard\000config=0x18\000ali_drw_pmu\00000\000\000 */
{ 153481 }, /* rdwr_transitions\000ali_drw\000A read-write turnaround\000config=0x15\000ali_drw_pmu\00000\000\000 */
{ 154600 }, /* selfref_mode_rk0\000ali_drw\000A cycle that Rank0 stays in self-refresh mode\000config=0x26\000ali_drw_pmu\00000\000\000 */
{ 154699 }, /* selfref_mode_rk1\000ali_drw\000A cycle that Rank1 stays in self-refresh mode\000config=0x27\000ali_drw_pmu\00000\000\000 */
{ 154798 }, /* selfref_mode_rk2\000ali_drw\000A cycle that Rank2 stays in self-refresh mode\000config=0x28\000ali_drw_pmu\00000\000\000 */
{ 154897 }, /* selfref_mode_rk3\000ali_drw\000A cycle that Rank3 stays in self-refresh mode\000config=0x29\000ali_drw_pmu\00000\000\000 */
{ 156397 }, /* tsz_vio\000ali_drw\000A violation detected in TZC\000config=0x3d\000ali_drw_pmu\00000\000\000 */
{ 155332 }, /* visible_window_limit_reached_rd\000ali_drw\000At least one entry in read queue reaches the visible window limit\000config=0x30\000ali_drw_pmu\00000\000\000 */
{ 155466 }, /* visible_window_limit_reached_wr\000ali_drw\000At least one entry in write queue reaches the visible window limit\000config=0x31\000ali_drw_pmu\00000\000\000 */
{ 153652 }, /* war_hazard\000ali_drw\000A Write-After-Read hazard\000config=0x17\000ali_drw_pmu\00000\000\000 */
{ 153798 }, /* waw_hazard\000ali_drw\000A Write-After-Write hazard\000config=0x19\000ali_drw_pmu\00000\000\000 */
{ 152629 }, /* wr_xact_when_critical\000ali_drw\000A write becomes critical\000config=0xb\000ali_drw_pmu\00000\000\000 */
{ 153558 }, /* write_combine\000ali_drw\000A write combine(merge) in write data buffer\000config=0x16\000ali_drw_pmu\00000\000\000 */

};

const struct pmu_table_entry pmu_events__freescale_yitian710_sys[] = {
{
     .entries = pmu_events__freescale_yitian710_sys_ali_drw,
     .num_entries = ARRAY_SIZE(pmu_events__freescale_yitian710_sys_ali_drw),
     .pmu_name = { 151755 /* ali_drw\000 */ },
},
};

static const struct compact_pmu_event pmu_metrics__freescale_yitian710_sys_ali_drw[] = {
{ 221074 }, /* ddr_read_bandwidth.all\000ali_drw\000hif_rd * 64 / 1e6 / duration_time\000\000The ddr read bandwidth(MB/s)\000\0001MB/s\000ali_drw_pmu\000\000\00000 */
{ 221192 }, /* ddr_write_bandwidth.all\000ali_drw\000(hif_wr + hif_rmw) * 64 / 1e6 / duration_time\000\000The ddr write bandwidth(MB/s)\000\0001MB/s\000ali_drw_pmu\000\000\00000 */

};

const struct pmu_table_entry pmu_metrics__freescale_yitian710_sys[] = {
{
     .entries = pmu_metrics__freescale_yitian710_sys_ali_drw,
     .num_entries = ARRAY_SIZE(pmu_metrics__freescale_yitian710_sys_ali_drw),
     .pmu_name = { 151755 /* ali_drw\000 */ },
},
};

static const struct compact_pmu_event pmu_events__fujitsu_a64fx_default_core[] = {
{ 168593 }, /* _0inst_commit\000other\000This event counts every cycle that no instruction was committed, but counts at the time when commits MOVPRFX only\000event=0x190\000\00000\000This event counts every cycle that no instruction was committed, but counts at the time when commits MOVPRFX only\000 */
{ 168857 }, /* _1inst_commit\000other\000This event counts every cycle that one instruction is committed\000event=0x191\000\00000\000This event counts every cycle that one instruction is committed\000 */
{ 169021 }, /* _2inst_commit\000other\000This event counts every cycle that two instructions are committed\000event=0x192\000\00000\000This event counts every cycle that two instructions are committed\000 */
{ 169189 }, /* _3inst_commit\000other\000This event counts every cycle that three instructions are committed\000event=0x193\000\00000\000This event counts every cycle that three instructions are committed\000 */
{ 169361 }, /* _4inst_commit\000other\000This event counts every cycle that four instructions are committed\000event=0x194\000\00000\000This event counts every cycle that four instructions are committed\000 */
{ 22494 }, /* ase_spec\000instruction\000Operation speculatively executed, Advanced SIMD instruction\000event=0x74\000\00000\000Operation speculatively executed, Advanced SIMD instruction\000 */
{ 179437 }, /* ase_sve_int_spec\000sve\000Advanced SIMD and SVE integer Operations speculatively executed\000event=0x8043\000\00000\000Advanced SIMD and SVE integer Operations speculatively executed\000 */
{ 181623 }, /* ase_sve_ld_multi_spec\000sve\000Advanced SIMD and SVE contiguous load multiple vector Operations speculatively executed\000event=0x80a5\000\00000\000Advanced SIMD and SVE contiguous load multiple vector Operations speculatively executed\000 */
{ 180014 }, /* ase_sve_ld_spec\000sve\000Advanced SIMD and SVE load Operations speculatively executed\000event=0x8085\000\00000\000Advanced SIMD and SVE load Operations speculatively executed\000 */
{ 181842 }, /* ase_sve_st_multi_spec\000sve\000Advanced SIMD and SVE contiguous store multiple vector Operations speculatively executed\000event=0x80a6\000\00000\000Advanced SIMD and SVE contiguous store multiple vector Operations speculatively executed\000 */
{ 180173 }, /* ase_sve_st_spec\000sve\000Advanced SIMD and SVE store Operations speculatively executed\000event=0x8086\000\00000\000Advanced SIMD and SVE store Operations speculatively executed\000 */
{ 180450 }, /* base_ld_reg_spec\000sve\000General-purpose register load Operations speculatively executed\000event=0x8089\000\00000\000General-purpose register load Operations speculatively executed\000 */
{ 180616 }, /* base_st_reg_spec\000sve\000General-purpose register store Operations speculatively executed\000event=0x808a\000\00000\000General-purpose register store Operations speculatively executed\000 */
{ 162807 }, /* bc_ld_spec\000instruction\000This event counts architecturally executed SIMD broadcast floating-point load operations\000event=0x11a\000\00000\000This event counts architecturally executed SIMD broadcast floating-point load operations\000 */
{ 167581 }, /* br_comp_wait\000other\000This event counts every cycle that no instruction was committed and the oldest and uncommitted instruction is a branch instruction\000event=0x18b\000\00000\000This event counts every cycle that no instruction was committed and the oldest and uncommitted instruction is a branch instruction\000 */
{ 105418 }, /* br_immed_spec\000instruction\000Branch speculatively executed, immediate branch\000event=0x78\000\00000\000Branch speculatively executed, immediate branch\000 */
{ 105555 }, /* br_indirect_spec\000instruction\000Branch speculatively executed, indirect branch\000event=0x7a\000\00000\000Branch speculatively executed, indirect branch\000 */
{ 411 }, /* br_mis_pred\000branch\000Mispredicted or not predicted branch speculatively executed\000event=0x10\000\00000\000Mispredicted or not predicted branch speculatively executed\000 */
{ 565 }, /* br_pred\000branch\000Predictable branch speculatively executed\000event=0x12\000\00000\000Predictable branch speculatively executed\000 */
{ 105693 }, /* br_return_spec\000instruction\000Branch speculatively executed, procedure return\000event=0x79\000\00000\000Branch speculatively executed, procedure return\000 */
{ 156900 }, /* bus_read_total_mem\000bus\000This event counts read transactions from measured CMG local memory to measured CMG\000event=0x316\000\00000\000This event counts read transactions from measured CMG local memory to measured CMG\000 */
{ 156717 }, /* bus_read_total_pci\000bus\000This event counts read transactions from PCI controller to measured CMG\000event=0x315\000\00000\000This event counts read transactions from PCI controller to measured CMG\000 */
{ 156531 }, /* bus_read_total_tofu\000bus\000This event counts read transactions from tofu controller to measured CMG\000event=0x314\000\00000\000This event counts read transactions from tofu controller to measured CMG\000 */
{ 157105 }, /* bus_write_total_cmg0\000bus\000This event counts write transactions from measured CMG to CMG0, if measured CMG is not CMG0\000event=0x318\000\00000\000This event counts write transactions from measured CMG to CMG0, if measured CMG is not CMG0\000 */
{ 157330 }, /* bus_write_total_cmg1\000bus\000This event counts write transactions from measured CMG to CMG1, if measured CMG is not CMG1\000event=0x319\000\00000\000This event counts write transactions from measured CMG to CMG1, if measured CMG is not CMG1\000 */
{ 157555 }, /* bus_write_total_cmg2\000bus\000This event counts write transactions from measured CMG to CMG2, if measured CMG is not CMG2\000event=0x31a\000\00000\000This event counts write transactions from measured CMG to CMG2, if measured CMG is not CMG2\000 */
{ 157780 }, /* bus_write_total_cmg3\000bus\000This event counts write transactions from measured CMG to CMG3, if measured CMG is not CMG3\000event=0x31b\000\00000\000This event counts write transactions from measured CMG to CMG3, if measured CMG is not CMG3\000 */
{ 158380 }, /* bus_write_total_mem\000bus\000This event counts write transactions from measured CMG to measured CMG local memory\000event=0x31e\000\00000\000This event counts write transactions from measured CMG to measured CMG local memory\000 */
{ 158194 }, /* bus_write_total_pci\000bus\000This event counts write transactions from measured CMG to PCI controller\000event=0x31d\000\00000\000This event counts write transactions from measured CMG to PCI controller\000 */
{ 158005 }, /* bus_write_total_tofu\000bus\000This event counts write transactions from measured CMG to tofu controller\000event=0x31c\000\00000\000This event counts write transactions from measured CMG to tofu controller\000 */
{ 24272 }, /* cid_write_retired\000instruction\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000event=0xb\000\00000\000Instruction architecturally executed, condition code check pass, write to CONTEXTIDR\000 */
{ 161272 }, /* cpu_cycles\000cycle\000Cycle\000event=0x11\000\00000\000Cycle\000 */
{ 23330 }, /* crypto_spec\000instruction\000Operation speculatively executed, Cryptographic instruction\000event=0x77\000\00000\000Operation speculatively executed, Cryptographic instruction\000 */
{ 161316 }, /* dczva_spec\000instruction\000This event counts architecturally executed zero blocking operations due to the 'DC ZVA' instruction\000event=0x9f\000\00000\000This event counts architecturally executed zero blocking operations due to the 'DC ZVA' instruction\000 */
{ 23705 }, /* dmb_spec\000instruction\000Barrier speculatively executed, DMB\000event=0x7e\000\00000\000Barrier speculatively executed, DMB\000 */
{ 22343 }, /* dp_spec\000instruction\000Operation speculatively executed, integer data processing\000event=0x73\000\00000\000Operation speculatively executed, integer data processing\000 */
{ 23597 }, /* dsb_spec\000instruction\000Barrier speculatively executed, DSB\000event=0x7d\000\00000\000Barrier speculatively executed, DSB\000 */
{ 169917 }, /* ea_core\000other\000This event counts energy consumption per cycle of core\000event=0x1e0\000\00000\000This event counts energy consumption per cycle of core\000 */
{ 161126 }, /* ea_l2\000cache\000This event counts energy consumption per cycle of L2 cache\000event=0x3e0\000\00000\000This event counts energy consumption per cycle of L2 cache\000 */
{ 163739 }, /* ea_memory\000memory\000This event counts energy consumption per cycle of CMG local memory\000event=0x3e8\000\00000\000This event counts energy consumption per cycle of CMG local memory\000 */
{ 171738 }, /* eaga_val\000pipeline\000This event counts valid cycles of EAGA pipeline\000event=0x1a0\000\00000\000This event counts valid cycles of EAGA pipeline\000 */
{ 171868 }, /* eagb_val\000pipeline\000This event counts valid cycles of EAGB pipeline\000event=0x1a1\000\00000\000This event counts valid cycles of EAGB pipeline\000 */
{ 163024 }, /* effective_inst_spec\000instruction\000This event counts architecturally executed instructions, excluding the MOVPRFX instruction\000event=0x121\000\00000\000This event counts architecturally executed instructions, excluding the MOVPRFX instruction\000 */
{ 166911 }, /* eu_comp_wait\000other\000This event counts every cycle that no instruction was committed and the oldest and uncommitted instruction is an integer or floating-point/SIMD instruction\000event=0x189\000\00000\000This event counts every cycle that no instruction was committed and the oldest and uncommitted instruction is an integer or floating-point/SIMD instruction\000 */
{ 171998 }, /* exa_val\000pipeline\000This event counts valid cycles of EXA pipeline\000event=0x1a2\000\00000\000This event counts valid cycles of EXA pipeline\000 */
{ 172125 }, /* exb_val\000pipeline\000This event counts valid cycles of EXB pipeline\000event=0x1a3\000\00000\000This event counts valid cycles of EXB pipeline\000 */
{ 20002 }, /* exc_dabort\000exception\000Exception taken, Data Abort and SError\000event=0x84\000\00000\000Exception taken, Data Abort and SError\000 */
{ 20191 }, /* exc_fiq\000exception\000Exception taken, FIQ\000event=0x87\000\00000\000Exception taken, FIQ\000 */
{ 20266 }, /* exc_hvc\000exception\000Exception taken, Hypervisor Call\000event=0x8a\000\00000\000Exception taken, Hypervisor Call\000 */
{ 20116 }, /* exc_irq\000exception\000Exception taken, IRQ\000event=0x86\000\00000\000Exception taken, IRQ\000 */
{ 19896 }, /* exc_pabort\000exception\000Exception taken, Instruction Abort\000event=0x83\000\00000\000Exception taken, Instruction Abort\000 */
{ 33889 }, /* exc_return\000instruction\000Instruction architecturally executed, condition check pass, exception return\000event=0xa\000\00000\000Instruction architecturally executed, condition check pass, exception return\000 */
{ 73325 }, /* exc_smc\000exception\000Exception taken, Secure Monitor Call\000event=0x88\000\00000\000Exception taken, Secure Monitor Call\000 */
{ 19797 }, /* exc_svc\000exception\000Exception taken, Supervisor Call\000event=0x82\000\00000\000Exception taken, Supervisor Call\000 */
{ 21031 }, /* exc_taken\000exception\000Exception taken\000event=0x9\000\00000\000Exception taken\000 */
{ 19692 }, /* exc_undef\000exception\000Exception taken, Other synchronous\000event=0x81\000\00000\000Exception taken, Other synchronous\000 */
{ 167258 }, /* fl_comp_wait\000other\000This event counts every cycle that no instruction was committed and the oldest and uncommitted instruction is a floating-point/SIMD instruction\000event=0x18a\000\00000\000This event counts every cycle that no instruction was committed and the oldest and uncommitted instruction is a floating-point/SIMD instruction\000 */
{ 172252 }, /* fla_val\000pipeline\000This event counts valid cycles of FLA pipeline\000event=0x1a4\000\00000\000This event counts valid cycles of FLA pipeline\000 */
{ 172633 }, /* fla_val_prd_cnt\000pipeline\000This event counts the number of 1's in the predicate bits of request in FLA pipeline, where it is corrected so that it becomes 16 when all bits are 1\000event=0x1b4\000\00000\000This event counts the number of 1's in the predicate bits of request in FLA pipeline, where it is corrected so that it becomes 16 when all bits are 1\000 */
{ 172379 }, /* flb_val\000pipeline\000This event counts valid cycles of FLB pipeline\000event=0x1a5\000\00000\000This event counts valid cycles of FLB pipeline\000 */
{ 172974 }, /* flb_val_prd_cnt\000pipeline\000This event counts the number of 1's in the predicate bits of request in FLB pipeline, where it is corrected so that it becomes 16 when all bits are 1\000event=0x1b5\000\00000\000This event counts the number of 1's in the predicate bits of request in FLB pipeline, where it is corrected so that it becomes 16 when all bits are 1\000 */
{ 179290 }, /* fp_cvt_spec\000sve\000floating-point convert Operations speculatively executed\000event=0x8038\000\00000\000floating-point convert Operations speculatively executed\000 */
{ 184040 }, /* fp_dp_fixed_ops_spec\000sve\000Non-scalable double-precision floating-point element Operations speculatively executed\000event=0x80c7\000\00000\000Non-scalable double-precision floating-point element Operations speculatively executed\000 */
{ 183832 }, /* fp_dp_scale_ops_spec\000sve\000Scalable double-precision floating-point element Operations speculatively executed\000event=0x80c6\000\00000\000Scalable double-precision floating-point element Operations speculatively executed\000 */
{ 182813 }, /* fp_fixed_ops_spec\000sve\000Non-scalable floating-point element Operations speculatively executed\000event=0x80c1\000\00000\000Non-scalable floating-point element Operations speculatively executed\000 */
{ 178978 }, /* fp_fma_spec\000sve\000Floating-point FMA Operations speculatively executed\000event=0x8028\000\00000\000Floating-point FMA Operations speculatively executed\000 */
{ 183196 }, /* fp_hp_fixed_ops_spec\000sve\000Non-scalable half-precision floating-point element Operations speculatively executed\000event=0x80c3\000\00000\000Non-scalable half-precision floating-point element Operations speculatively executed\000 */
{ 182992 }, /* fp_hp_scale_ops_spec\000sve\000Scalable half-precision floating-point element Operations speculatively executed\000event=0x80c2\000\00000\000Scalable half-precision floating-point element Operations speculatively executed\000 */
{ 162347 }, /* fp_ld_spec\000instruction\000This event counts architecturally executed NOSIMD load operations that using SIMD&FP registers\000event=0x112\000\00000\000This event counts architecturally executed NOSIMD load operations that using SIMD&FP registers\000 */
{ 161554 }, /* fp_mv_spec\000instruction\000This event counts architecturally executed floating-point move operations\000event=0x105\000\00000\000This event counts architecturally executed floating-point move operations\000 */
{ 179117 }, /* fp_recpe_spec\000sve\000Floating-point reciprocal estimate Operations speculatively executed\000event=0x8034\000\00000\000Floating-point reciprocal estimate Operations speculatively executed\000 */
{ 182642 }, /* fp_scale_ops_spec\000sve\000Scalable floating-point element Operations speculatively executed\000event=0x80c0\000\00000\000Scalable floating-point element Operations speculatively executed\000 */
{ 183616 }, /* fp_sp_fixed_ops_spec\000sve\000Non-scalable single-precision floating-point element Operations speculatively executed\000event=0x80c5\000\00000\000Non-scalable single-precision floating-point element Operations speculatively executed\000 */
{ 183408 }, /* fp_sp_scale_ops_spec\000sve\000Scalable single-precision floating-point element Operations speculatively executed\000event=0x80c4\000\00000\000Scalable single-precision floating-point element Operations speculatively executed\000 */
{ 178851 }, /* fp_spec\000sve\000Floating-point Operations speculatively executed\000event=0x8010\000\00000\000Floating-point Operations speculatively executed\000 */
{ 162576 }, /* fp_st_spec\000instruction\000This event counts architecturally executed NOSIMD store operations that using SIMD&FP registers\000event=0x113\000\00000\000This event counts architecturally executed NOSIMD store operations that using SIMD&FP registers\000 */
{ 161946 }, /* iel_spec\000instruction\000This event counts architecturally executed inter-element manipulation operations\000event=0x109\000\00000\000This event counts architecturally executed inter-element manipulation operations\000 */
{ 24159 }, /* inst_retired\000instruction\000Instruction architecturally executed\000event=0x8\000\00000\000Instruction architecturally executed\000 */
{ 24711 }, /* inst_spec\000instruction\000Operation speculatively executed\000event=0x1b\000\00000\000Operation speculatively executed\000 */
{ 162145 }, /* ireg_spec\000instruction\000This event counts architecturally executed inter-register manipulation operations\000event=0x10a\000\00000\000This event counts architecturally executed inter-register manipulation operations\000 */
{ 23489 }, /* isb_spec\000instruction\000Barrier speculatively executed, ISB\000event=0x7c\000\00000\000Barrier speculatively executed, ISB\000 */
{ 159303 }, /* l1_miss_wait\000cache\000This event counts outstanding L1D cache miss requests per cycle\000event=0x208\000\00000\000This event counts outstanding L1D cache miss requests per cycle\000 */
{ 174555 }, /* l1_pipe0_comp\000pipeline\000This event counts completed requests in L1D cache pipeline#0\000event=0x260\000\00000\000This event counts completed requests in L1D cache pipeline#0\000 */
{ 176981 }, /* l1_pipe0_comp_prd_cnt\000pipeline\000This event counts the number of 1's in the predicate bits of request in L1D cache pipeline#0, where it is corrected so that it becomes 16 when all bits are 1\000event=0x2b8\000\00000\000This event counts the number of 1's in the predicate bits of request in L1D cache pipeline#0, where it is corrected so that it becomes 16 when all bits are 1\000 */
{ 173315 }, /* l1_pipe0_val\000pipeline\000This event counts valid cycles of L1D cache pipeline#0\000event=0x240\000\00000\000This event counts valid cycles of L1D cache pipeline#0\000 */
{ 175398 }, /* l1_pipe0_val_iu_not_sec0\000pipeline\000This event counts requests in L1D cache pipeline#0 that its sector cache ID is not 0\000event=0x2a0\000\00000\000This event counts requests in L1D cache pipeline#0 that its sector cache ID is not 0\000 */
{ 173847 }, /* l1_pipe0_val_iu_tag_adrs_pfe\000pipeline\000This event counts requests in L1D cache pipeline#0 that its pfe bit of tagged address is 1\000event=0x251\000\00000\000This event counts requests in L1D cache pipeline#0 that its pfe bit of tagged address is 1\000 */
{ 173611 }, /* l1_pipe0_val_iu_tag_adrs_sce\000pipeline\000This event counts requests in L1D cache pipeline#0 that its sce bit of tagged address is 1\000event=0x250\000\00000\000This event counts requests in L1D cache pipeline#0 that its sce bit of tagged address is 1\000 */
{ 174716 }, /* l1_pipe1_comp\000pipeline\000This event counts completed requests in L1D cache pipeline#1\000event=0x261\000\00000\000This event counts completed requests in L1D cache pipeline#1\000 */
{ 177344 }, /* l1_pipe1_comp_prd_cnt\000pipeline\000This event counts the number of 1's in the predicate bits of request in L1D cache pipeline#1, where it is corrected so that it becomes 16 when all bits are 1\000event=0x2b9\000\00000\000This event counts the number of 1's in the predicate bits of request in L1D cache pipeline#1, where it is corrected so that it becomes 16 when all bits are 1\000 */
{ 173463 }, /* l1_pipe1_val\000pipeline\000This event counts valid cycles of L1D cache pipeline#1\000event=0x241\000\00000\000This event counts valid cycles of L1D cache pipeline#1\000 */
{ 175618 }, /* l1_pipe1_val_iu_not_sec0\000pipeline\000This event counts requests in L1D cache pipeline#1 that its sector cache ID is not 0\000event=0x2a1\000\00000\000This event counts requests in L1D cache pipeline#1 that its sector cache ID is not 0\000 */
{ 174319 }, /* l1_pipe1_val_iu_tag_adrs_pfe\000pipeline\000This event counts requests in L1D cache pipeline#1 that its pfe bit of tagged address is 1\000event=0x253\000\00000\000This event counts requests in L1D cache pipeline#1 that its pfe bit of tagged address is 1\000 */
{ 174083 }, /* l1_pipe1_val_iu_tag_adrs_sce\000pipeline\000This event counts requests in L1D cache pipeline#1 that its sce bit of tagged address is 1\000event=0x252\000\00000\000This event counts requests in L1D cache pipeline#1 that its sce bit of tagged address is 1\000 */
{ 175178 }, /* l1_pipe_abort_stld_intlk\000pipeline\000This event counts aborted requests in L1D pipelines that due to store-load interlock\000event=0x274\000\00000\000This event counts aborted requests in L1D pipelines that due to store-load interlock\000 */
{ 176482 }, /* l1_pipe_comp_gather_0flow\000pipeline\000This event counts the number of times where 2 elements of the gather instructions became 0 flow because both predicate values are 0\000event=0x2b2\000\00000\000This event counts the number of times where 2 elements of the gather instructions became 0 flow because both predicate values are 0\000 */
{ 176165 }, /* l1_pipe_comp_gather_1flow\000pipeline\000This event counts the number of times where 2 elements of the gather instructions became 1 flow because 2 elements could be combined\000event=0x2b1\000\00000\000This event counts the number of times where 2 elements of the gather instructions became 1 flow because 2 elements could be combined\000 */
{ 175838 }, /* l1_pipe_comp_gather_2flow\000pipeline\000This event counts the number of times where 2 elements of the gather instructions became 2 flows because 2 elements could not be combined\000event=0x2b0\000\00000\000This event counts the number of times where 2 elements of the gather instructions became 2 flows because 2 elements could not be combined\000 */
{ 176797 }, /* l1_pipe_comp_scatter_1flow\000pipeline\000This event counts the number of flows of the scatter instructions\000event=0x2b3\000\00000\000This event counts the number of flows of the scatter instructions\000 */
{ 3313 }, /* l1d_cache\000cache\000Level 1 data cache access\000event=0x4\000\00000\000Level 1 data cache access\000 */
{ 3224 }, /* l1d_cache_refill\000cache\000Level 1 data cache refill\000event=0x3\000\00000\000Level 1 data cache refill\000 */
{ 158972 }, /* l1d_cache_refill_dm\000cache\000This event counts L1D_CACHE_REFILL caused by demand access\000event=0x200\000\00000\000This event counts L1D_CACHE_REFILL caused by demand access\000 */
{ 159132 }, /* l1d_cache_refill_hwprf\000cache\000This event counts L1D_CACHE_REFILL caused by hardware prefetch\000event=0x202\000\00000\000This event counts L1D_CACHE_REFILL caused by hardware prefetch\000 */
{ 158588 }, /* l1d_cache_refill_prf\000cache\000This event counts L1D_CACHE_REFILL caused by software or hardware prefetch\000event=0x49\000\00000\000This event counts L1D_CACHE_REFILL caused by software or hardware prefetch\000 */
{ 33516 }, /* l1d_cache_wb\000cache\000Attributable Level 1 data cache write-back\000event=0x15\000\00000\000Attributable Level 1 data cache write-back\000 */
{ 3395 }, /* l1d_tlb_refill\000cache\000Attributable Level 1 data TLB refill\000event=0x5\000\00000\000Attributable Level 1 data TLB refill\000 */
{ 170280 }, /* l1hwpf_inj_alloc_pf\000other\000This event counts allocation type prefetch injection requests to L1D cache generated by hardware prefetcher\000event=0x231\000\00000\000This event counts allocation type prefetch injection requests to L1D cache generated by hardware prefetcher\000 */
{ 170538 }, /* l1hwpf_inj_noalloc_pf\000other\000This event counts non-allocation type prefetch injection requests to L1D cache generated by hardware prefetcher\000event=0x232\000\00000\000This event counts non-allocation type prefetch injection requests to L1D cache generated by hardware prefetcher\000 */
{ 170057 }, /* l1hwpf_stream_pf\000other\000This event counts streaming prefetch requests to L1D cache generated by hardware prefetcher\000event=0x230\000\00000\000This event counts streaming prefetch requests to L1D cache generated by hardware prefetcher\000 */
{ 3504 }, /* l1i_cache\000cache\000Attributable Level 1 instruction cache access\000event=0x14\000\00000\000Attributable Level 1 instruction cache access\000 */
{ 2998 }, /* l1i_cache_refill\000cache\000Level 1 instruction cache refill\000event=0x1\000\00000\000Level 1 instruction cache refill\000 */
{ 159466 }, /* l1i_miss_wait\000cache\000This event counts outstanding L1I cache miss requests per cycle\000event=0x209\000\00000\000This event counts outstanding L1I cache miss requests per cycle\000 */
{ 174877 }, /* l1i_pipe_comp\000pipeline\000This event counts completed requests in L1I cache pipeline\000event=0x268\000\00000\000This event counts completed requests in L1I cache pipeline\000 */
{ 175034 }, /* l1i_pipe_val\000pipeline\000This event counts valid cycles of L1I cache pipeline\000event=0x269\000\00000\000This event counts valid cycles of L1I cache pipeline\000 */
{ 3101 }, /* l1i_tlb_refill\000cache\000Attributable Level 1 instruction TLB refill\000event=0x2\000\00000\000Attributable Level 1 instruction TLB refill\000 */
{ 160122 }, /* l2_miss_count\000cache\000This event counts the number of times of L2 cache miss\000event=0x309\000\00000\000This event counts the number of times of L2 cache miss\000 */
{ 159961 }, /* l2_miss_wait\000cache\000This event counts outstanding L2 cache miss requests per cycle\000event=0x308\000\00000\000This event counts outstanding L2 cache miss requests per cycle\000 */
{ 177848 }, /* l2_pipe_comp_all\000pipeline\000This event counts completed requests in L2 cache pipeline\000event=0x350\000\00000\000This event counts completed requests in L2 cache pipeline\000 */
{ 178006 }, /* l2_pipe_comp_pf_l2mib_mch\000pipeline\000This event counts operations where software or hardware prefetch hits an L2 cache refill buffer allocated by demand access\000event=0x370\000\00000\000This event counts operations where software or hardware prefetch hits an L2 cache refill buffer allocated by demand access\000 */
{ 177707 }, /* l2_pipe_val\000pipeline\000This event counts valid cycles of L2 cache pipeline\000event=0x330\000\00000\000This event counts valid cycles of L2 cache pipeline\000 */
{ 3627 }, /* l2d_cache\000cache\000Level 2 data cache access\000event=0x16\000\00000\000Level 2 data cache access\000 */
{ 160548 }, /* l2d_cache_mibmch_prf\000cache\000This event counts operations where software or hardware prefetch hits an L2 cache refill buffer allocated by demand access\000event=0x326\000\00000\000This event counts operations where software or hardware prefetch hits an L2 cache refill buffer allocated by demand access\000 */
{ 3710 }, /* l2d_cache_refill\000cache\000Level 2 data refill\000event=0x17\000\00000\000Level 2 data refill\000 */
{ 159630 }, /* l2d_cache_refill_dm\000cache\000This event counts L2D_CACHE_REFILL caused by demand access\000event=0x300\000\00000\000This event counts L2D_CACHE_REFILL caused by demand access\000 */
{ 159790 }, /* l2d_cache_refill_hwprf\000cache\000This event counts L2D_CACHE_REFILL caused by hardware prefetch\000event=0x302\000\00000\000This event counts L2D_CACHE_REFILL caused by hardware prefetch\000 */
{ 158780 }, /* l2d_cache_refill_prf\000cache\000This event counts L2D_CACHE_REFILL caused by software or hardware prefetch\000event=0x59\000\00000\000This event counts L2D_CACHE_REFILL caused by software or hardware prefetch\000 */
{ 160837 }, /* l2d_cache_swap_local\000cache\000This event counts operations where demand access hits an L2 cache refill buffer allocated by software or hardware prefetch\000event=0x396\000\00000\000This event counts operations where demand access hits an L2 cache refill buffer allocated by software or hardware prefetch\000 */
{ 3788 }, /* l2d_cache_wb\000cache\000Attributable Level 2 data cache write-back\000event=0x18\000\00000\000Attributable Level 2 data cache write-back\000 */
{ 160268 }, /* l2d_swap_dm\000cache\000This event counts operations where demand access hits an L2 cache refill buffer allocated by software or hardware prefetch\000event=0x325\000\00000\000This event counts operations where demand access hits an L2 cache refill buffer allocated by software or hardware prefetch\000 */
{ 4384 }, /* l2d_tlb\000cache\000Attributable Level 2 data or unified TLB access\000event=0x2f\000\00000\000Attributable Level 2 data or unified TLB access\000 */
{ 4150 }, /* l2d_tlb_refill\000cache\000Attributable Level 2 data TLB refill\000event=0x2d\000\00000\000Attributable Level 2 data TLB refill\000 */
{ 171025 }, /* l2hwpf_inj_alloc_pf\000other\000This event counts allocation type prefetch injection requests to L2 cache generated by hardware prefetcher\000event=0x234\000\00000\000This event counts allocation type prefetch injection requests to L2 cache generated by hardware prefetcher\000 */
{ 171281 }, /* l2hwpf_inj_noalloc_pf\000other\000This event counts non-allocation type prefetch injection requests to L2 cache generated by hardware prefetcher\000event=0x235\000\00000\000This event counts non-allocation type prefetch injection requests to L2 cache generated by hardware prefetcher\000 */
{ 171547 }, /* l2hwpf_other\000other\000This event counts prefetch requests to L2 cache generated by the other causes\000event=0x236\000\00000\000This event counts prefetch requests to L2 cache generated by the other causes\000 */
{ 170806 }, /* l2hwpf_stream_pf\000other\000This event counts streaming prefetch requests to L2 cache generated by hardware prefecher\000event=0x233\000\00000\000This event counts streaming prefetch requests to L2 cache generated by hardware prefecher\000 */
{ 4509 }, /* l2i_tlb\000cache\000Attributable Level 2 instruction TLB access\000event=0x30\000\00000\000Attributable Level 2 instruction TLB access\000 */
{ 4260 }, /* l2i_tlb_refill\000cache\000Attributable Level 2 instruction TLB refill\000event=0x2e\000\00000\000Attributable Level 2 instruction TLB refill\000 */
{ 165452 }, /* ld_comp_wait\000other\000This event counts every cycle that no instruction was committed because the oldest and uncommitted load/store/prefetch operation waits for L1D cache, L2 cache and memory access\000event=0x184\000\00000\000This event counts every cycle that no instruction was committed because the oldest and uncommitted load/store/prefetch operation waits for L1D cache, L2 cache and memory access\000 */
{ 165841 }, /* ld_comp_wait_ex\000other\000This event counts every cycle that no instruction was committed because the oldest and uncommitted integer load operation waits for L1D cache, L2 cache and memory access\000event=0x185\000\00000\000This event counts every cycle that no instruction was committed because the oldest and uncommitted integer load operation waits for L1D cache, L2 cache and memory access\000 */
{ 164757 }, /* ld_comp_wait_l1_miss\000other\000This event counts every cycle that no instruction was committed because the oldest and uncommitted load/store/prefetch operation waits for L2 cache access\000event=0x182\000\00000\000This event counts every cycle that no instruction was committed because the oldest and uncommitted load/store/prefetch operation waits for L2 cache access\000 */
{ 165110 }, /* ld_comp_wait_l1_miss_ex\000other\000This event counts every cycle that no instruction was committed because the oldest and uncommitted integer load operation waits for L2 cache access\000event=0x183\000\00000\000This event counts every cycle that no instruction was committed because the oldest and uncommitted integer load operation waits for L2 cache access\000 */
{ 164074 }, /* ld_comp_wait_l2_miss\000other\000This event counts every cycle that no operation was committed because the oldest and uncommitted load/store/prefetch operation waits for memory access\000event=0x180\000\00000\000This event counts every cycle that no operation was committed because the oldest and uncommitted load/store/prefetch operation waits for memory access\000 */
{ 164419 }, /* ld_comp_wait_l2_miss_ex\000other\000This event counts every cycle that no instruction was committed because the oldest and uncommitted integer load operation waits for memory access\000event=0x181\000\00000\000This event counts every cycle that no instruction was committed because the oldest and uncommitted integer load operation waits for memory access\000 */
{ 166219 }, /* ld_comp_wait_pfp_busy\000other\000This event counts every cycle that no instruction was committed due to the lack of an available prefetch port\000event=0x186\000\00000\000This event counts every cycle that no instruction was committed due to the lack of an available prefetch port\000 */
{ 166483 }, /* ld_comp_wait_pfp_busy_ex\000other\000This event counts the LD_COMP_WAIT_PFP_BUSY caused by an integer load operation\000event=0x187\000\00000\000This event counts the LD_COMP_WAIT_PFP_BUSY caused by an integer load operation\000 */
{ 166690 }, /* ld_comp_wait_pfp_busy_swpf\000other\000This event counts the LD_COMP_WAIT_PFP_BUSY caused by a software prefetch instruction\000event=0x188\000\00000\000This event counts the LD_COMP_WAIT_PFP_BUSY caused by a software prefetch instruction\000 */
{ 21982 }, /* ld_spec\000instruction\000Operation speculatively executed, load\000event=0x70\000\00000\000Operation speculatively executed, load\000 */
{ 73432 }, /* ldrex_spec\000instruction\000Exclusive operation speculatively executed, LDREX or LDX\000event=0x6c\000\00000\000Exclusive operation speculatively executed, LDREX or LDX\000 */
{ 22210 }, /* ldst_spec\000instruction\000Operation speculatively executed, load or store\000event=0x72\000\00000\000Operation speculatively executed, load or store\000 */
{ 22808 }, /* pc_write_spec\000instruction\000Operation speculatively executed, software change of the PC\000event=0x76\000\00000\000Operation speculatively executed, software change of the PC\000 */
{ 163495 }, /* post_index_spec\000instruction\000This event counts architecturally executed operations that uses 'post-index' as its addressing mode\000event=0x124\000\00000\000This event counts architecturally executed operations that uses 'post-index' as its addressing mode\000 */
{ 161741 }, /* prd_spec\000instruction\000This event counts architecturally executed operations that using predicate register\000event=0x108\000\00000\000This event counts architecturally executed operations that using predicate register\000 */
{ 163254 }, /* pre_index_spec\000instruction\000This event counts architecturally executed operations that uses 'pre-index' as its addressing mode\000event=0x123\000\00000\000This event counts architecturally executed operations that uses 'pre-index' as its addressing mode\000 */
{ 180334 }, /* prf_spec\000sve\000Prefetch Operations speculatively executed\000event=0x8087\000\00000\000Prefetch Operations speculatively executed\000 */
{ 172506 }, /* prx_val\000pipeline\000This event counts valid cycles of PRX pipeline\000event=0x1a6\000\00000\000This event counts valid cycles of PRX pipeline\000 */
{ 167878 }, /* rob_empty\000other\000This event counts every cycle that no instruction was committed because the CSE is empty\000event=0x18c\000\00000\000This event counts every cycle that no instruction was committed because the CSE is empty\000 */
{ 168088 }, /* rob_empty_stq_busy\000other\000This event counts every cycle that no instruction was committed because the CSE is empty and the store port (SP) is full\000event=0x18d\000\00000\000This event counts every cycle that no instruction was committed because the CSE is empty and the store port (SP) is full\000 */
{ 178303 }, /* simd_inst_retired\000sve\000SIMD Instruction architecturally executed\000event=0x8000\000\00000\000SIMD Instruction architecturally executed\000 */
{ 169719 }, /* single_movprfx_commit\000other\000This event counts every cycle that only the MOVPRFX instruction is committed\000event=0x199\000\00000\000This event counts every cycle that only the MOVPRFX instruction is committed\000 */
{ 22095 }, /* st_spec\000instruction\000Operation speculatively executed, store\000event=0x71\000\00000\000Operation speculatively executed, store\000 */
{ 27892 }, /* stall_backend\000pipeline\000No operation issued due to the backend\000event=0x24\000\00000\000No operation issued due to the backend\000 */
{ 46706 }, /* stall_frontend\000pipeline\000No operation issued because of the frontend\000event=0x23\000\00000\000No operation issued because of the frontend\000 */
{ 88779 }, /* strex_spec\000instruction\000Exclusive operation speculatively executed, STREX or STX\000event=0x6f\000\00000\000Exclusive operation speculatively executed, STREX or STX\000 */
{ 178426 }, /* sve_inst_retired\000sve\000Instruction architecturally executed, SVE\000event=0x8002\000\00000\000Instruction architecturally executed, SVE\000 */
{ 182063 }, /* sve_ld_gather_spec\000sve\000SVE gather-load Operations speculatively executed\000event=0x80ad\000\00000\000SVE gather-load Operations speculatively executed\000 */
{ 182497 }, /* sve_ldff_spec\000sve\000SVE First-fault load Operations speculatively executed\000event=0x80bc\000\00000\000SVE First-fault load Operations speculatively executed\000 */
{ 181122 }, /* sve_ldr_preg_spec\000sve\000SVE load predicate register Operations speculatively executed\000event=0x8095\000\00000\000SVE load predicate register Operations speculatively executed\000 */
{ 180784 }, /* sve_ldr_reg_spec\000sve\000SVE unpredicated load register Operations speculatively executed\000event=0x8091\000\00000\000SVE unpredicated load register Operations speculatively executed\000 */
{ 178706 }, /* sve_math_spec\000sve\000SVE Math accelerator Operations speculatively executed\000event=0x800e\000\00000\000SVE Math accelerator Operations speculatively executed\000 */
{ 179736 }, /* sve_movprfx_spec\000sve\000SVE MOVPRFX Operations speculatively executed\000event=0x807c\000\00000\000SVE MOVPRFX Operations speculatively executed\000 */
{ 179866 }, /* sve_movprfx_u_spec\000sve\000SVE MOVPRFX unfused Operations speculatively executed\000event=0x807f\000\00000\000SVE MOVPRFX unfused Operations speculatively executed\000 */
{ 179603 }, /* sve_pred_spec\000sve\000SVE predicated Operations speculatively executed\000event=0x8074\000\00000\000SVE predicated Operations speculatively executed\000 */
{ 181450 }, /* sve_prf_contig_spec\000sve\000SVE contiguous prefetch element Operations speculatively executed\000event=0x809f\000\00000\000SVE contiguous prefetch element Operations speculatively executed\000 */
{ 182348 }, /* sve_prf_gather_spec\000sve\000SVE gather-prefetch Operations speculatively executed\000event=0x80af\000\00000\000SVE gather-prefetch Operations speculatively executed\000 */
{ 182203 }, /* sve_st_scatter_spec\000sve\000SVE scatter-store Operations speculatively executed\000event=0x80ae\000\00000\000SVE scatter-store Operations speculatively executed\000 */
{ 181285 }, /* sve_str_preg_spec\000sve\000SVE store predicate register Operations speculatively executed\000event=0x8096\000\00000\000SVE store predicate register Operations speculatively executed\000 */
{ 180952 }, /* sve_str_reg_spec\000sve\000SVE unpredicated store register Operations speculatively executed\000event=0x8092\000\00000\000SVE unpredicated store register Operations speculatively executed\000 */
{ 21286 }, /* sw_incr\000instruction\000Instruction architecturally executed, Condition code check pass, software increment\000event=0\000\00000\000Instruction architecturally executed, Condition code check pass, software increment\000 */
{ 169531 }, /* uop_only_commit\000other\000This event counts every cycle that only any micro-operations are committed\000event=0x198\000\00000\000This event counts every cycle that only any micro-operations are committed\000 */
{ 178548 }, /* uop_spec\000sve\000Microarchitectural operation, Operations speculatively executed\000event=0x8008\000\00000\000Microarchitectural operation, Operations speculatively executed\000 */
{ 163906 }, /* uop_split\000other\000This event counts the occurrence count of the micro-operation split\000event=0x139\000\00000\000This event counts the occurrence count of the micro-operation split\000 */
{ 22650 }, /* vfp_spec\000instruction\000Operation speculatively executed, floating-point instruction\000event=0x75\000\00000\000Operation speculatively executed, floating-point instruction\000 */
{ 168371 }, /* wfe_wfi_cycle\000other\000This event counts every cycle that the instruction unit is halted by the WFE/WFI instruction\000event=0x18e\000\00000\000This event counts every cycle that the instruction unit is halted by the WFE/WFI instruction\000 */

};

const struct pmu_table_entry pmu_events__fujitsu_a64fx[] = {
{
     .entries = pmu_events__fujitsu_a64fx_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__fujitsu_a64fx_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__hisilicon_hip08_default_core[] = {
{ 185869 }, /* exe_stall_cycle\000core imp def\000Cycles of that the number of issue ups are less than 4\000event=0x7001\000\00000\000Cycles of that the number of issuing micro operations are less than 4\000 */
{ 185571 }, /* fetch_bubble\000core imp def\000Instructions can receive, but not send\000event=0x2014\000\00000\000Instructions can receive, but not send\000 */
{ 185782 }, /* hit_on_prf\000core imp def\000Hit on prefetched data\000event=0x6014\000\00000\000Hit on prefetched data\000 */
{ 185467 }, /* if_is_stall\000core imp def\000Instruction fetch stall cycles\000event=0x1044\000\00000\000Instruction fetch stall cycles\000 */
{ 185371 }, /* iq_is_empty\000core imp def\000Instruction queue is empty\000event=0x1043\000\00000\000Instruction queue is empty\000 */
{ 147303 }, /* l1d_cache_inval\000core imp def\000L1D cache invalidate\000event=0x48\000\00000\000Attributable Level 1 data cache invalidate\000 */
{ 146333 }, /* l1d_cache_rd\000core imp def\000L1D cache access, read\000event=0x40\000\00000\000Attributable Level 1 data cache access, read\000 */
{ 146553 }, /* l1d_cache_refill_rd\000core imp def\000L1D cache refill, read\000event=0x42\000\00000\000Attributable Level 1 data cache refill, read\000 */
{ 146669 }, /* l1d_cache_refill_wr\000core imp def\000L1D cache refill, write\000event=0x43\000\00000\000Attributable Level 1 data cache refill, write\000 */
{ 147157 }, /* l1d_cache_wb_clean\000core imp def\000L1D cache Write-Back, cleaning and coherency\000event=0x47\000\00000\000Level 1 data cache Write-Back, cleaning and coherency\000 */
{ 147029 }, /* l1d_cache_wb_victim\000core imp def\000L1D cache Write-Back, victim\000event=0x46\000\00000\000Attributable Level 1 data cache Write-Back, victim\000 */
{ 146442 }, /* l1d_cache_wr\000core imp def\000L1D cache access, write\000event=0x41\000\00000\000Attributable Level 1 data cache access, write\000 */
{ 147633 }, /* l1d_tlb_rd\000core imp def\000L1D tlb access, read\000event=0x4e\000\00000\000Attributable Level 1 data or unified TLB access, read\000 */
{ 147411 }, /* l1d_tlb_refill_rd\000core imp def\000L1D tlb refill, read\000event=0x4c\000\00000\000Attributable Level 1 data TLB refill, read\000 */
{ 147521 }, /* l1d_tlb_refill_wr\000core imp def\000L1D tlb refill, write\000event=0x4d\000\00000\000Attributable Level 1 data TLB refill, write\000 */
{ 147747 }, /* l1d_tlb_wr\000core imp def\000L1D tlb access, write\000event=0x4f\000\00000\000Attributable Level 1 data or unified TLB access, write\000 */
{ 185092 }, /* l1i_cache_prf\000core imp def\000L1I cache prefetch access count\000event=0x102e\000\00000\000Level 1 instruction cache prefetch access count\000 */
{ 185216 }, /* l1i_cache_prf_refill\000core imp def\000L1I cache miss due to prefetch access count\000event=0x102f\000\00000\000Level 1 instruction cache miss due to prefetch access count\000 */
{ 184984 }, /* l2d_cache_inval\000core imp def\000L2D cache invalidate\000event=0x58\000\00000\000Attributable Level 2 data cache invalidate\000 */
{ 184256 }, /* l2d_cache_rd\000core imp def\000L2D cache access, read\000event=0x50\000\00000\000Attributable Level 2 data cache access, read\000 */
{ 184476 }, /* l2d_cache_refill_rd\000core imp def\000L2D cache refill, read\000event=0x52\000\00000\000Attributable Level 2 data cache refill, read\000 */
{ 184592 }, /* l2d_cache_refill_wr\000core imp def\000L2D cache refill, write\000event=0x53\000\00000\000Attributable Level 2 data cache refill, write\000 */
{ 184838 }, /* l2d_cache_wb_clean\000core imp def\000L2D cache Write-Back, cleaning and coherency\000event=0x57\000\00000\000Level 2 data cache Write-Back, cleaning and coherency\000 */
{ 184710 }, /* l2d_cache_wb_victim\000core imp def\000L2D cache Write-Back, victim\000event=0x56\000\00000\000Attributable Level 2 data cache Write-Back, victim\000 */
{ 184365 }, /* l2d_cache_wr\000core imp def\000L2D cache access, write\000event=0x51\000\00000\000Attributable Level 2 data cache access, write\000 */
{ 186040 }, /* mem_stall_anyload\000core imp def\000No any micro operation is issued and meanwhile any load operation is not resolved\000event=0x7004\000\00000\000No any micro operation is issued and meanwhile any load operation is not resolved\000 */
{ 186252 }, /* mem_stall_l1miss\000core imp def\000No any micro operation is issued and meanwhile there is any load operation missing L1 cache and pending data refill\000event=0x7006\000\00000\000No any micro operation is issued and meanwhile there is any load operation missing L1 cache and pending data refill\000 */
{ 186531 }, /* mem_stall_l2miss\000core imp def\000No any micro operation is issued and meanwhile there is any load operation missing both L1 and L2 cache and pending data refill from L3 cache\000event=0x7007\000\00000\000No any micro operation is issued and meanwhile there is any load operation missing both L1 and L2 cache and pending data refill from L3 cache\000 */
{ 185692 }, /* prf_req\000core imp def\000Prefetch request from LSU\000event=0x6013\000\00000\000Prefetch request from LSU\000 */
};
static const struct compact_pmu_event pmu_events__hisilicon_hip08_hisi_sccl_ddrc[] = {
{ 187290 }, /* act_cmd\000uncore ddrc\000DDRC active commands\000config=0x5\000\00000\000DDRC active commands\000 */
{ 187132 }, /* flux_rcmd\000uncore ddrc\000DDRC read commands\000config=0x3\000\00000\000DDRC read commands\000 */
{ 186966 }, /* flux_rd\000uncore ddrc\000DDRC total read operations\000config=0x1\000\00000\000DDRC total read operations\000 */
{ 187055 }, /* flux_wcmd\000uncore ddrc\000DDRC write commands\000config=0x2\000\00000\000DDRC write commands\000 */
{ 186877 }, /* flux_wr\000uncore ddrc\000DDRC total write operations\000config=0\000\00000\000DDRC total write operations\000 */
{ 187207 }, /* pre_cmd\000uncore ddrc\000DDRC precharge commands\000config=0x4\000\00000\000DDRC precharge commands\000 */
{ 187367 }, /* rnk_chg\000uncore ddrc\000DDRC rank commands\000config=0x6\000\00000\000DDRC rank commands\000 */
{ 187440 }, /* rw_chg\000uncore ddrc\000DDRC read and write changes\000config=0x7\000\00000\000DDRC read and write changes\000 */
};
static const struct compact_pmu_event pmu_events__hisilicon_hip08_hisi_sccl_hha[] = {
{ 189419 }, /* bi_num\000uncore hha\000\000config=0x23\000\00000\000\000 */
{ 189828 }, /* edir-hit\000uncore hha\000\000config=0x43\000\00000\000\000 */
{ 189913 }, /* edir-home-migrate\000uncore hha\000\000config=0x4d\000\00000\000\000 */
{ 189749 }, /* edir-lookup\000uncore hha\000\000config=0x41\000\00000\000\000 */
{ 189455 }, /* mediated_num\000uncore hha\000\000config=0x32\000\00000\000\000 */
{ 188695 }, /* rd_ddr_128b\000uncore hha\000The number of read operations sent by HHA to DDRC which size is 128 bytes\000config=0x1e\000\00000\000The number of read operations sent by HHA to DDRC which size is 128 bytes\000 */
{ 188326 }, /* rd_ddr_64b\000uncore hha\000The number of read operations sent by HHA to DDRC which size is 64 bytes\000config=0x1c\000\00000\000The number of read operations sent by HHA to DDRC which size is 64bytes\000 */
{ 188048 }, /* rx_ccix\000uncore hha\000Count of the number of operations that HHA has received from CCIX\000config=0x3\000\00000\000Count of the number of operations that HHA has received from CCIX\000 */
{ 187544 }, /* rx_ops_num\000uncore hha\000The number of all operations received by the HHA\000config=0\000\00000\000The number of all operations received by the HHA\000 */
{ 187677 }, /* rx_outer\000uncore hha\000The number of all operations received by the HHA from another socket\000config=0x1\000\00000\000The number of all operations received by the HHA from another socket\000 */
{ 187850 }, /* rx_sccl\000uncore hha\000The number of all operations received by the HHA from another SCCL in this socket\000config=0x2\000\00000\000The number of all operations received by the HHA from another SCCL in this socket\000 */
{ 189663 }, /* rx_snprsp_outer\000uncore hha\000\000config=0x3c\000\00000\000\000 */
{ 189620 }, /* rx_snprspdata\000uncore hha\000\000config=0x38\000\00000\000\000 */
{ 188214 }, /* rx_wbi\000uncore hha\000\000config=0x4\000\00000\000\000 */
{ 188249 }, /* rx_wbip\000uncore hha\000\000config=0x5\000\00000\000\000 */
{ 188285 }, /* rx_wtistash\000uncore hha\000\000config=0x11\000\00000\000\000 */
{ 189790 }, /* sdir-hit\000uncore hha\000\000config=0x42\000\00000\000\000 */
{ 189866 }, /* sdir-home-migrate\000uncore hha\000\000config=0x4c\000\00000\000\000 */
{ 189708 }, /* sdir-lookup\000uncore hha\000\000config=0x40\000\00000\000\000 */
{ 189071 }, /* spill_num\000uncore hha\000Count of the number of spill operations that the HHA has sent\000config=0x20\000\00000\000Count of the number of spill operations that the HHA has sent\000 */
{ 189232 }, /* spill_success\000uncore hha\000Count of the number of successful spill operations that the HHA has sent\000config=0x21\000\00000\000Count of the number of successful spill operations that the HHA has sent\000 */
{ 189579 }, /* tx_snp_ccix\000uncore hha\000\000config=0x35\000\00000\000\000 */
{ 189497 }, /* tx_snp_num\000uncore hha\000\000config=0x33\000\00000\000\000 */
{ 189537 }, /* tx_snp_outer\000uncore hha\000\000config=0x34\000\00000\000\000 */
{ 188882 }, /* wr_ddr_128b\000uncore hha\000The number of write operations sent by HHA to DDRC which size is 128 bytes\000config=0x1f\000\00000\000The number of write operations sent by HHA to DDRC which size is 128 bytes\000 */
{ 188509 }, /* wr_ddr_64b\000uncore hha\000The number of write operations sent by HHA to DDRC which size is 64 bytes\000config=0x1d\000\00000\000The number of write operations sent by HHA to DDRC which size is 64 bytes\000 */
};
static const struct compact_pmu_event pmu_events__hisilicon_hip08_hisi_sccl_l3c[] = {
{ 191099 }, /* back_invalid\000uncore l3c\000Count of the number of L3C back invalid operations\000config=0x29\000\00000\000Count of the number of L3C back invalid operations\000 */
{ 191590 }, /* prefetch_drop\000uncore l3c\000Count of the number of prefetch drops from this L3C\000config=0x42\000\00000\000Count of the number of prefetch drops from this L3C\000 */
{ 189974 }, /* rd_cpipe\000uncore l3c\000Total read accesses\000config=0\000\00000\000Total read accesses\000 */
{ 190124 }, /* rd_hit_cpipe\000uncore l3c\000Total read hits\000config=0x2\000\00000\000Total read hits\000 */
{ 190757 }, /* rd_hit_spipe\000uncore l3c\000Count of the number of read lines that hits in spipe of this L3C\000config=0x22\000\00000\000Count of the number of read lines that hits in spipe of this L3C\000 */
{ 190351 }, /* rd_spipe\000uncore l3c\000Count of the number of read lines that come from this cluster of CPU core in spipe\000config=0x20\000\00000\000Count of the number of read lines that come from this cluster of CPU core in spipe\000 */
{ 191241 }, /* retry_cpu\000uncore l3c\000Count of the number of retry that L3C suppresses the CPU operations\000config=0x40\000\00000\000Count of the number of retry that L3C suppresses the CPU operations\000 */
{ 191414 }, /* retry_ring\000uncore l3c\000Count of the number of retry that L3C suppresses the ring operations\000config=0x41\000\00000\000Count of the number of retry that L3C suppresses the ring operations\000 */
{ 190268 }, /* victim_num\000uncore l3c\000l3c precharge commands\000config=0x4\000\00000\000l3c precharge commands\000 */
{ 190047 }, /* wr_cpipe\000uncore l3c\000Total write accesses\000config=0x1\000\00000\000Total write accesses\000 */
{ 190195 }, /* wr_hit_cpipe\000uncore l3c\000Total write hits\000config=0x3\000\00000\000Total write hits\000 */
{ 190927 }, /* wr_hit_spipe\000uncore l3c\000Count of the number of write lines that hits in spipe of this L3C\000config=0x23\000\00000\000Count of the number of write lines that hits in spipe of this L3C\000 */
{ 190553 }, /* wr_spipe\000uncore l3c\000Count of the number of write lines that come from this cluster of CPU core in spipe\000config=0x21\000\00000\000Count of the number of write lines that come from this cluster of CPU core in spipe\000 */

};

const struct pmu_table_entry pmu_events__hisilicon_hip08[] = {
{
     .entries = pmu_events__hisilicon_hip08_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__hisilicon_hip08_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
{
     .entries = pmu_events__hisilicon_hip08_hisi_sccl_ddrc,
     .num_entries = ARRAY_SIZE(pmu_events__hisilicon_hip08_hisi_sccl_ddrc),
     .pmu_name = { 186862 /* hisi_sccl,ddrc\000 */ },
},
{
     .entries = pmu_events__hisilicon_hip08_hisi_sccl_hha,
     .num_entries = ARRAY_SIZE(pmu_events__hisilicon_hip08_hisi_sccl_hha),
     .pmu_name = { 187530 /* hisi_sccl,hha\000 */ },
},
{
     .entries = pmu_events__hisilicon_hip08_hisi_sccl_l3c,
     .num_entries = ARRAY_SIZE(pmu_events__hisilicon_hip08_hisi_sccl_l3c),
     .pmu_name = { 189960 /* hisi_sccl,l3c\000 */ },
},
};

static const struct compact_pmu_event pmu_metrics__hisilicon_hip08_default_core[] = {
{ 221764 }, /* backend_bound\000Default;TopDownL1\0001 - (frontend_bound + bad_speculation + retiring)\000\000Backend Bound L1 topdown metric\000Backend Bound L1 topdown metric\000\000\000\000TopDownL1\00000 */
{ 221471 }, /* bad_speculation\000Default;TopDownL1\000(INST_SPEC - INST_RETIRED) / (4 * CPU_CYCLES)\000\000Bad Speculation L1 topdown metric\000Bad Speculation L1 topdown metric\000\000\000\000TopDownL1\00000 */
{ 223251 }, /* bp_misp_flush\000TopDownL3\000BR_MIS_PRED * 5 / CPU_CYCLES\000\000BP misp flush L3 topdown metric\000BP misp flush L3 topdown metric\000\000\000\000\00000 */
{ 222237 }, /* branch_mispredicts\000TopDownL2\000bad_speculation * BR_MIS_PRED / (BR_MIS_PRED + armv8_pmuv3_0@event\\=0x2013@)\000\000Branch mispredicts L2 topdown metric\000Branch mispredicts L2 topdown metric\000\000\000\000\00000 */
{ 222559 }, /* core_bound\000TopDownL2\000(EXE_STALL_CYCLE - (MEM_STALL_ANYLOAD + armv8_pmuv3_0@event\\=0x7005@)) / CPU_CYCLES\000\000Core bound L2 topdown metric\000Core bound L2 topdown metric\000\000\000\000\00000 */
{ 225315 }, /* divider\000TopDownL3\000armv8_pmuv3_0@event\\=0x7002@ / CPU_CYCLES\000\000Divider L3 topdown metric\000Divider L3 topdown metric\000\000\000\000\00000 */
{ 225559 }, /* exe_ports_util\000TopDownL3\000core_bound - divider - fsu_stall\000\000EXE ports util L3 topdown metric\000EXE ports util L3 topdown metric\000\000\000\000\00000 */
{ 222081 }, /* fetch_bandwidth_bound\000TopDownL2\000frontend_bound - fetch_latency_bound\000\000Fetch bandwidth bound L2 topdown metric\000Fetch bandwidth bound L2 topdown metric\000\000\000\000\00000 */
{ 221926 }, /* fetch_latency_bound\000TopDownL2\000armv8_pmuv3_0@event\\=0x201d@ / CPU_CYCLES\000\000Fetch latency bound L2 topdown metric\000Fetch latency bound L2 topdown metric\000\000\000\000\00000 */
{ 221324 }, /* frontend_bound\000Default;TopDownL1\000FETCH_BUBBLE / (4 * CPU_CYCLES)\000\000Frontend bound L1 topdown metric\000Frontend bound L1 topdown metric\000\000\000\000TopDownL1\00000 */
{ 225434 }, /* fsu_stall\000TopDownL3\000armv8_pmuv3_0@event\\=0x7003@ / CPU_CYCLES\000\000FSU stall L3 topdown metric\000FSU stall L3 topdown metric\000\000\000\000\00000 */
{ 223062 }, /* idle_by_icache_miss\000TopDownL3\000((L2I_CACHE - L2I_CACHE_REFILL) * 15 + L2I_CACHE_REFILL * 100) / CPU_CYCLES\000\000Idle by icache miss L3 topdown metric\000Idle by icache miss L3 topdown metric\000\000\000\000\00000 */
{ 222885 }, /* idle_by_itlb_miss\000TopDownL3\000((L2I_TLB - L2I_TLB_REFILL) * 15 + L2I_TLB_REFILL * 100) / CPU_CYCLES\000\000Idle by itlb miss L3 topdown metric\000Idle by itlb miss L3 topdown metric\000\000\000\000\00000 */
{ 223658 }, /* indirect_branch\000TopDownL3\000armv8_pmuv3_0@event\\=0x1010@ / BR_MIS_PRED\000\000Indirect branch L3 topdown metric\000Indirect branch L3 topdown metric\000\000\000\000\00000 */
{ 225690 }, /* l1_bound\000TopDownL3\000(MEM_STALL_ANYLOAD - MEM_STALL_L1MISS) / CPU_CYCLES\000\000L1 bound L3 topdown metric\000L1 bound L3 topdown metric\000\000\000\000\00000 */
{ 225822 }, /* l2_bound\000TopDownL3\000(MEM_STALL_L1MISS - MEM_STALL_L2MISS) / CPU_CYCLES\000\000L2 bound L3 topdown metric\000L2 bound L3 topdown metric\000\000\000\000\00000 */
{ 222424 }, /* machine_clears\000TopDownL2\000bad_speculation - branch_mispredicts\000\000Machine clears L2 topdown metric\000Machine clears L2 topdown metric\000\000\000\000\00000 */
{ 225953 }, /* mem_bound\000TopDownL3\000MEM_STALL_L2MISS / CPU_CYCLES\000\000Mem bound L3 topdown metric\000Mem bound L3 topdown metric\000\000\000\000\00000 */
{ 222729 }, /* memory_bound\000TopDownL2\000(MEM_STALL_ANYLOAD + armv8_pmuv3_0@event\\=0x7005@) / CPU_CYCLES\000\000Memory bound L2 topdown metric\000Memory bound L2 topdown metric\000\000\000\000\00000 */
{ 224340 }, /* nuke_flush\000TopDownL3\000armv8_pmuv3_0@event\\=0x2012@ / armv8_pmuv3_0@event\\=0x2013@\000\000Nuke flush L3 topdown metric\000Nuke flush L3 topdown metric\000\000\000\000\00000 */
{ 223375 }, /* ooo_flush\000TopDownL3\000armv8_pmuv3_0@event\\=0x2013@ * 5 / CPU_CYCLES\000\000OOO flush L3 topdown metric\000OOO flush L3 topdown metric\000\000\000\000\00000 */
{ 224096 }, /* other_branch\000TopDownL3\000(BR_MIS_PRED - armv8_pmuv3_0@event\\=0x1010@ - armv8_pmuv3_0@event\\=0x1013@ - armv8_pmuv3_0@event\\=0x1016@ - armv8_pmuv3_0@event\\=0x100d@) / BR_MIS_PRED\000\000Other branch L3 topdown metric\000Other branch L3 topdown metric\000\000\000\000\00000 */
{ 224486 }, /* other_flush\000TopDownL3\0001 - nuke_flush\000\000Other flush L3 topdown metric\000Other flush L3 topdown metric\000\000\000\000\00000 */
{ 225172 }, /* pc_buffer_stall\000TopDownL3\000armv8_pmuv3_0@event\\=0x2005@ / CPU_CYCLES\000\000PC buffer stall L3 topdown metric\000PC buffer stall L3 topdown metric\000\000\000\000\00000 */
{ 223967 }, /* pop_branch\000TopDownL3\000armv8_pmuv3_0@event\\=0x100d@ / BR_MIS_PRED\000\000Pop branch L3 topdown metric\000Pop branch L3 topdown metric\000\000\000\000\00000 */
{ 224843 }, /* ptag_stall\000TopDownL3\000(armv8_pmuv3_0@event\\=0x2006@ + armv8_pmuv3_0@event\\=0x2007@ + armv8_pmuv3_0@event\\=0x2008@) / CPU_CYCLES\000\000Ptag stall L3 topdown metric\000Ptag stall L3 topdown metric\000\000\000\000\00000 */
{ 223802 }, /* push_branch\000TopDownL3\000(armv8_pmuv3_0@event\\=0x1013@ + armv8_pmuv3_0@event\\=0x1016@) / BR_MIS_PRED\000\000Push branch L3 topdown metric\000Push branch L3 topdown metric\000\000\000\000\00000 */
{ 221635 }, /* retiring\000Default;TopDownL1\000INST_RETIRED / (CPU_CYCLES * 4)\000\000Retiring L1 topdown metric\000Retiring L1 topdown metric\000\000\000\000TopDownL1\00000 */
{ 224718 }, /* rob_stall\000TopDownL3\000armv8_pmuv3_0@event\\=0x2004@ / CPU_CYCLES\000\000Rob stall L3 topdown metric\000Rob stall L3 topdown metric\000\000\000\000\00000 */
{ 225035 }, /* saveopq_stall\000TopDownL3\000armv8_pmuv3_0@event\\=0x201e@ / CPU_CYCLES\000\000SaveOpQ stall L3 topdown metric\000SaveOpQ stall L3 topdown metric\000\000\000\000\00000 */
{ 223504 }, /* sp_flush\000TopDownL3\000armv8_pmuv3_0@event\\=0x1001@ * 5 / CPU_CYCLES\000\000Static predictor flush L3 topdown metric\000Static predictor flush L3 topdown metric\000\000\000\000\00000 */
{ 226066 }, /* store_bound\000TopDownL3\000armv8_pmuv3_0@event\\=0x7005@ / CPU_CYCLES\000\000Store bound L3 topdown metric\000Store bound L3 topdown metric\000\000\000\000\00000 */
{ 224590 }, /* sync_stall\000TopDownL3\000armv8_pmuv3_0@event\\=0x2010@ / CPU_CYCLES\000\000Sync stall L3 topdown metric\000Sync stall L3 topdown metric\000\000\000\000\00000 */

};

const struct pmu_table_entry pmu_metrics__hisilicon_hip08[] = {
{
     .entries = pmu_metrics__hisilicon_hip08_default_core,
     .num_entries = ARRAY_SIZE(pmu_metrics__hisilicon_hip08_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__hisilicon_hip09_sys_hisi_sicl_cpa[] = {
{ 191749 }, /* cpa_cycles\000uncore cpa\000count of CPA cycles\000config=0\0000x00000030\00000\000\000 */
{ 192359 }, /* cpa_p0_rd_dat\000uncore cpa\000Number of read ops transmitted by the P0 port\000config=0xe2\0000x00000030\00000\000\000 */
{ 192582 }, /* cpa_p0_rd_dat_32b\000uncore cpa\000Number of read ops transmitted by the P0 port which size is 32 bytes\000config=0x84\0000x00000030\00000\000\000 */
{ 192457 }, /* cpa_p0_rd_dat_64b\000uncore cpa\000Number of read ops transmitted by the P0 port which size is 64 bytes\000config=0x83\0000x00000030\00000\000\000 */
{ 192260 }, /* cpa_p0_wr_dat\000uncore cpa\000Number of write ops transmitted by the P0 port\000config=0xe1\0000x00000030\00000\000\000 */
{ 191914 }, /* cpa_p1_rd_dat\000uncore cpa\000Number of read ops transmitted by the P1 port\000config=0x62\0000x00000030\00000\000\000 */
{ 192136 }, /* cpa_p1_rd_dat_32b\000uncore cpa\000Number of read ops transmitted by the P1 port which size is 32 bytes\000config=0x4\0000x00000030\00000\000\000 */
{ 192012 }, /* cpa_p1_rd_dat_64b\000uncore cpa\000Number of read ops transmitted by the P1 port which size is 64 bytes\000config=0x3\0000x00000030\00000\000\000 */
{ 191815 }, /* cpa_p1_wr_dat\000uncore cpa\000Number of write ops transmitted by the P1 port\000config=0x61\0000x00000030\00000\000\000 */

};

const struct pmu_table_entry pmu_events__hisilicon_hip09_sys[] = {
{
     .entries = pmu_events__hisilicon_hip09_sys_hisi_sicl_cpa,
     .num_entries = ARRAY_SIZE(pmu_events__hisilicon_hip09_sys_hisi_sicl_cpa),
     .pmu_name = { 191735 /* hisi_sicl,cpa\000 */ },
},
};

static const struct compact_pmu_event pmu_metrics__hisilicon_hip09_sys_hisi_sicl_cpa[] = {
{ 226349 }, /* cpa_p0_avg_bw\000CPA\000(cpa_p0_wr_dat * 64 + cpa_p0_rd_dat_64b * 64 + cpa_p0_rd_dat_32b * 32) / cpa_cycles\000\000Average bandwidth of CPA Port 0\000\000\0000x00000030\000\000\00000 */
{ 226197 }, /* cpa_p1_avg_bw\000CPA\000(cpa_p1_wr_dat * 64 + cpa_p1_rd_dat_64b * 64 + cpa_p1_rd_dat_32b * 32) / cpa_cycles\000\000Average bandwidth of CPA Port 1\000\000\0000x00000030\000\000\00000 */

};

const struct pmu_table_entry pmu_metrics__hisilicon_hip09_sys[] = {
{
     .entries = pmu_metrics__hisilicon_hip09_sys_hisi_sicl_cpa,
     .num_entries = ARRAY_SIZE(pmu_metrics__hisilicon_hip09_sys_hisi_sicl_cpa),
     .pmu_name = { 191735 /* hisi_sicl,cpa\000 */ },
},
};

static const struct compact_pmu_event pmu_events__test_soc_cpu_default_core[] = {
{ 192707 }, /* bp_l1_btb_correct\000branch\000L1 BTB Correction\000event=0x8a\000\00000\000\000 */
{ 192766 }, /* bp_l2_btb_correct\000branch\000L2 BTB Correction\000event=0x8b\000\00000\000\000 */
{ 193021 }, /* dispatch_blocked.any\000other\000Memory cluster signals to block micro-op dispatch for any reason\000event=0x9,period=200000,umask=0x20\000\00000\000\000 */
{ 193153 }, /* eist_trans\000other\000Number of Enhanced Intel SpeedStep(R) Technology (EIST) transitions\000event=0x3a,period=200000,umask=0x0\000\00000\000\000 */
{ 192825 }, /* l3_cache_rd\000cache\000L3 cache access, read\000event=0x40\000\00000\000Attributable Level 3 cache access, read\000 */
{ 192920 }, /* segment_reg_loads.any\000other\000Number of segment register loads\000event=0x6,period=200000,umask=0x80\000\00000\000\000 */
};
static const struct compact_pmu_event pmu_events__test_soc_cpu_hisi_sccl_ddrc[] = {
{ 193278 }, /* uncore_hisi_ddrc.flux_wcmd\000uncore\000DDRC write commands\000event=0x2\000\00000\000DDRC write commands\000 */
};
static const struct compact_pmu_event pmu_events__test_soc_cpu_hisi_sccl_l3c[] = {
{ 193767 }, /* uncore_hisi_l3c.rd_hit_cpipe\000uncore\000Total read hits\000event=0x7\000\00000\000Total read hits\000 */
};
static const struct compact_pmu_event pmu_events__test_soc_cpu_uncore_cbox[] = {
{ 193609 }, /* event-hyphen\000uncore\000UNC_CBO_HYPHEN\000event=0xe0,umask=0x00\000\00000\000UNC_CBO_HYPHEN\000 */
{ 193685 }, /* event-two-hyph\000uncore\000UNC_CBO_TWO_HYPH\000event=0xc0,umask=0x00\000\00000\000UNC_CBO_TWO_HYPH\000 */
{ 193378 }, /* unc_cbo_xsnp_response.miss_eviction\000uncore\000A cross-core snoop resulted from L3 Eviction which misses in some processor core\000event=0x22,umask=0x81\000\00000\000A cross-core snoop resulted from L3 Eviction which misses in some processor core\000 */
};
static const struct compact_pmu_event pmu_events__test_soc_cpu_uncore_imc[] = {
{ 193979 }, /* uncore_imc.cache_hits\000uncore\000Total cache hits\000event=0x34\000\00000\000Total cache hits\000 */
};
static const struct compact_pmu_event pmu_events__test_soc_cpu_uncore_imc_free_running[] = {
{ 193873 }, /* uncore_imc_free_running.cache_miss\000uncore\000Total cache misses\000event=0x12\000\00000\000Total cache misses\000 */

};

const struct pmu_table_entry pmu_events__test_soc_cpu[] = {
{
     .entries = pmu_events__test_soc_cpu_default_core,
     .num_entries = ARRAY_SIZE(pmu_events__test_soc_cpu_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
{
     .entries = pmu_events__test_soc_cpu_hisi_sccl_ddrc,
     .num_entries = ARRAY_SIZE(pmu_events__test_soc_cpu_hisi_sccl_ddrc),
     .pmu_name = { 186862 /* hisi_sccl,ddrc\000 */ },
},
{
     .entries = pmu_events__test_soc_cpu_hisi_sccl_l3c,
     .num_entries = ARRAY_SIZE(pmu_events__test_soc_cpu_hisi_sccl_l3c),
     .pmu_name = { 189960 /* hisi_sccl,l3c\000 */ },
},
{
     .entries = pmu_events__test_soc_cpu_uncore_cbox,
     .num_entries = ARRAY_SIZE(pmu_events__test_soc_cpu_uncore_cbox),
     .pmu_name = { 193366 /* uncore_cbox\000 */ },
},
{
     .entries = pmu_events__test_soc_cpu_uncore_imc,
     .num_entries = ARRAY_SIZE(pmu_events__test_soc_cpu_uncore_imc),
     .pmu_name = { 193968 /* uncore_imc\000 */ },
},
{
     .entries = pmu_events__test_soc_cpu_uncore_imc_free_running,
     .num_entries = ARRAY_SIZE(pmu_events__test_soc_cpu_uncore_imc_free_running),
     .pmu_name = { 193849 /* uncore_imc_free_running\000 */ },
},
};

static const struct compact_pmu_event pmu_metrics__test_soc_cpu_default_core[] = {
{ 226501 }, /* CPI\000\0001 / IPC\000\000\000\000\000\000\000\00000 */
{ 227182 }, /* DCache_L2_All\000\000DCache_L2_All_Hits + DCache_L2_All_Miss\000\000\000\000\000\000\000\00000 */
{ 226954 }, /* DCache_L2_All_Hits\000\000l2_rqsts.demand_data_rd_hit + l2_rqsts.pf_hit + l2_rqsts.rfo_hit\000\000\000\000\000\000\000\00000 */
{ 227048 }, /* DCache_L2_All_Miss\000\000max(l2_rqsts.all_demand_data_rd - l2_rqsts.demand_data_rd_hit, 0) + l2_rqsts.pf_miss + l2_rqsts.rfo_miss\000\000\000\000\000\000\000\00000 */
{ 227246 }, /* DCache_L2_Hits\000\000d_ratio(DCache_L2_All_Hits, DCache_L2_All)\000\000\000\000\000\000\000\00000 */
{ 227314 }, /* DCache_L2_Misses\000\000d_ratio(DCache_L2_All_Miss, DCache_L2_All)\000\000\000\000\000\000\000\00000 */
{ 226586 }, /* Frontend_Bound_SMT\000\000idq_uops_not_delivered.core / (4 * (cpu_clk_unhalted.thread / 2 * (1 + cpu_clk_unhalted.one_thread_active / cpu_clk_unhalted.ref_xclk)))\000\000\000\000\000\000\000\00000 */
{ 226523 }, /* IPC\000group1\000inst_retired.any / cpu_clk_unhalted.thread\000\000\000\000\000\000\000\00000 */
{ 227448 }, /* L1D_Cache_Fill_BW\000\00064 * l1d.replacement / 1e9 / duration_time\000\000\000\000\000\000\000\00000 */
{ 227384 }, /* M1\000\000ipc + M2\000\000\000\000\000\000\000\00000 */
{ 227406 }, /* M2\000\000ipc + M1\000\000\000\000\000\000\000\00000 */
{ 227428 }, /* M3\000\0001 / M3\000\000\000\000\000\000\000\00000 */
{ 226883 }, /* cache_miss_cycles\000group1\000dcache_miss_cpi + icache_miss_cycles\000\000\000\000\000\000\000\00000 */
{ 226752 }, /* dcache_miss_cpi\000\000l1d\\-loads\\-misses / inst_retired.any\000\000\000\000\000\000\000\00000 */
{ 226816 }, /* icache_miss_cycles\000\000l1i\\-loads\\-misses / inst_retired.any\000\000\000\000\000\000\000\00000 */

};

const struct pmu_table_entry pmu_metrics__test_soc_cpu[] = {
{
     .entries = pmu_metrics__test_soc_cpu_default_core,
     .num_entries = ARRAY_SIZE(pmu_metrics__test_soc_cpu_default_core),
     .pmu_name = { 0 /* default_core\000 */ },
},
};

static const struct compact_pmu_event pmu_events__test_soc_sys_uncore_sys_ccn_pmu[] = {
{ 194168 }, /* sys_ccn_pmu.read_cycles\000uncore\000ccn read-cycles event\000config=0x2c\0000x01\00000\000\000 */
};
static const struct compact_pmu_event pmu_events__test_soc_sys_uncore_sys_ddr_pmu[] = {
{ 194076 }, /* sys_ddr_pmu.write_cycles\000uncore\000ddr write-cycles event\000event=0x2b\000v8\00000\000\000 */

};

const struct pmu_table_entry pmu_events__test_soc_sys[] = {
{
     .entries = pmu_events__test_soc_sys_uncore_sys_ccn_pmu,
     .num_entries = ARRAY_SIZE(pmu_events__test_soc_sys_uncore_sys_ccn_pmu),
     .pmu_name = { 194149 /* uncore_sys_ccn_pmu\000 */ },
},
{
     .entries = pmu_events__test_soc_sys_uncore_sys_ddr_pmu,
     .num_entries = ARRAY_SIZE(pmu_events__test_soc_sys_uncore_sys_ddr_pmu),
     .pmu_name = { 194057 /* uncore_sys_ddr_pmu\000 */ },
},
};


/* Struct used to make the PMU event table implementation opaque to callers. */
struct pmu_events_table {
        const struct pmu_table_entry *pmus;
        uint32_t num_pmus;
};

/* Struct used to make the PMU metric table implementation opaque to callers. */
struct pmu_metrics_table {
        const struct pmu_table_entry *pmus;
        uint32_t num_pmus;
};

/*
 * Map a CPU to its table of PMU events. The CPU is identified by the
 * cpuid field, which is an arch-specific identifier for the CPU.
 * The identifier specified in tools/perf/pmu-events/arch/xxx/mapfile
 * must match the get_cpuid_str() in tools/perf/arch/xxx/util/header.c)
 *
 * The  cpuid can contain any character other than the comma.
 */
struct pmu_events_map {
        const char *arch;
        const char *cpuid;
        struct pmu_events_table event_table;
        struct pmu_metrics_table metric_table;
};

/*
 * Global table mapping each known CPU for the architecture to its
 * table of PMU events.
 */
const struct pmu_events_map pmu_events_map[] = {
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd020",
	.event_table = {
		.pmus = pmu_events__arm_cortex_a34,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_cortex_a34)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd030",
	.event_table = {
		.pmus = pmu_events__arm_cortex_a53,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_cortex_a53)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000420f1000",
	.event_table = {
		.pmus = pmu_events__arm_cortex_a53,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_cortex_a53)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd040",
	.event_table = {
		.pmus = pmu_events__arm_cortex_a35,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_cortex_a35)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd050",
	.event_table = {
		.pmus = pmu_events__arm_cortex_a55,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_cortex_a55)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd060",
	.event_table = {
		.pmus = pmu_events__arm_cortex_a65_e1,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_cortex_a65_e1)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd4a0",
	.event_table = {
		.pmus = pmu_events__arm_cortex_a65_e1,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_cortex_a65_e1)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd070",
	.event_table = {
		.pmus = pmu_events__arm_cortex_a57_a72,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_cortex_a57_a72)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd080",
	.event_table = {
		.pmus = pmu_events__arm_cortex_a57_a72,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_cortex_a57_a72)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd090",
	.event_table = {
		.pmus = pmu_events__arm_cortex_a73,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_cortex_a73)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd0a0",
	.event_table = {
		.pmus = pmu_events__arm_cortex_a75,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_cortex_a75)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd0b0",
	.event_table = {
		.pmus = pmu_events__arm_cortex_a76,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_cortex_a76)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd0c0",
	.event_table = {
		.pmus = pmu_events__arm_neoverse_n1,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_neoverse_n1)
	},
	.metric_table = {
		.pmus = pmu_metrics__arm_neoverse_n1,
		.num_pmus = ARRAY_SIZE(pmu_metrics__arm_neoverse_n1)
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd0d0",
	.event_table = {
		.pmus = pmu_events__arm_cortex_a77,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_cortex_a77)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd400",
	.event_table = {
		.pmus = pmu_events__arm_neoverse_v1,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_neoverse_v1)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd410",
	.event_table = {
		.pmus = pmu_events__arm_cortex_a78,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_cortex_a78)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd4b0",
	.event_table = {
		.pmus = pmu_events__arm_cortex_a78,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_cortex_a78)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd440",
	.event_table = {
		.pmus = pmu_events__arm_cortex_x1,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_cortex_x1)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd4c0",
	.event_table = {
		.pmus = pmu_events__arm_cortex_x1,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_cortex_x1)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd460",
	.event_table = {
		.pmus = pmu_events__arm_cortex_a510,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_cortex_a510)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd470",
	.event_table = {
		.pmus = pmu_events__arm_cortex_a710,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_cortex_a710)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd480",
	.event_table = {
		.pmus = pmu_events__arm_cortex_x2,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_cortex_x2)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd490",
	.event_table = {
		.pmus = pmu_events__arm_neoverse_n2_v2,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_neoverse_n2_v2)
	},
	.metric_table = {
		.pmus = pmu_metrics__arm_neoverse_n2_v2,
		.num_pmus = ARRAY_SIZE(pmu_metrics__arm_neoverse_n2_v2)
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000410fd4f0",
	.event_table = {
		.pmus = pmu_events__arm_neoverse_n2_v2,
		.num_pmus = ARRAY_SIZE(pmu_events__arm_neoverse_n2_v2)
	},
	.metric_table = {
		.pmus = pmu_metrics__arm_neoverse_n2_v2,
		.num_pmus = ARRAY_SIZE(pmu_metrics__arm_neoverse_n2_v2)
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000420f5160",
	.event_table = {
		.pmus = pmu_events__cavium_thunderx2,
		.num_pmus = ARRAY_SIZE(pmu_events__cavium_thunderx2)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000430f0af0",
	.event_table = {
		.pmus = pmu_events__cavium_thunderx2,
		.num_pmus = ARRAY_SIZE(pmu_events__cavium_thunderx2)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000460f0010",
	.event_table = {
		.pmus = pmu_events__fujitsu_a64fx,
		.num_pmus = ARRAY_SIZE(pmu_events__fujitsu_a64fx)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000480fd010",
	.event_table = {
		.pmus = pmu_events__hisilicon_hip08,
		.num_pmus = ARRAY_SIZE(pmu_events__hisilicon_hip08)
	},
	.metric_table = {
		.pmus = pmu_metrics__hisilicon_hip08,
		.num_pmus = ARRAY_SIZE(pmu_metrics__hisilicon_hip08)
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000500f0000",
	.event_table = {
		.pmus = pmu_events__ampere_emag,
		.num_pmus = ARRAY_SIZE(pmu_events__ampere_emag)
	},
	.metric_table = {
		.pmus = NULL,
		.num_pmus = 0
	}
},
{
	.arch = "arm64",
	.cpuid = "0x00000000c00fac30",
	.event_table = {
		.pmus = pmu_events__ampere_ampereone,
		.num_pmus = ARRAY_SIZE(pmu_events__ampere_ampereone)
	},
	.metric_table = {
		.pmus = pmu_metrics__ampere_ampereone,
		.num_pmus = ARRAY_SIZE(pmu_metrics__ampere_ampereone)
	}
},
{
	.arch = "testarch",
	.cpuid = "testcpu",
	.event_table = {
		.pmus = pmu_events__test_soc_cpu,
		.num_pmus = ARRAY_SIZE(pmu_events__test_soc_cpu),
	},
	.metric_table = {
		.pmus = pmu_metrics__test_soc_cpu,
		.num_pmus = ARRAY_SIZE(pmu_metrics__test_soc_cpu),
	}
},
{
	.arch = 0,
	.cpuid = 0,
	.event_table = { 0, 0 },
	.metric_table = { 0, 0 },
}
};

struct pmu_sys_events {
	const char *name;
	struct pmu_events_table event_table;
	struct pmu_metrics_table metric_table;
};

static const struct pmu_sys_events pmu_sys_event_tables[] = {
	{
		.event_table = {
			.pmus = pmu_events__freescale_imx8mm_sys,
			.num_pmus = ARRAY_SIZE(pmu_events__freescale_imx8mm_sys)
		},
		.metric_table = {
			.pmus = pmu_metrics__freescale_imx8mm_sys,
			.num_pmus = ARRAY_SIZE(pmu_metrics__freescale_imx8mm_sys)
		},
		.name = "pmu_events__freescale_imx8mm_sys",
	},
	{
		.event_table = {
			.pmus = pmu_events__freescale_imx8mn_sys,
			.num_pmus = ARRAY_SIZE(pmu_events__freescale_imx8mn_sys)
		},
		.metric_table = {
			.pmus = pmu_metrics__freescale_imx8mn_sys,
			.num_pmus = ARRAY_SIZE(pmu_metrics__freescale_imx8mn_sys)
		},
		.name = "pmu_events__freescale_imx8mn_sys",
	},
	{
		.event_table = {
			.pmus = pmu_events__freescale_imx8mp_sys,
			.num_pmus = ARRAY_SIZE(pmu_events__freescale_imx8mp_sys)
		},
		.metric_table = {
			.pmus = pmu_metrics__freescale_imx8mp_sys,
			.num_pmus = ARRAY_SIZE(pmu_metrics__freescale_imx8mp_sys)
		},
		.name = "pmu_events__freescale_imx8mp_sys",
	},
	{
		.event_table = {
			.pmus = pmu_events__freescale_imx8mq_sys,
			.num_pmus = ARRAY_SIZE(pmu_events__freescale_imx8mq_sys)
		},
		.metric_table = {
			.pmus = pmu_metrics__freescale_imx8mq_sys,
			.num_pmus = ARRAY_SIZE(pmu_metrics__freescale_imx8mq_sys)
		},
		.name = "pmu_events__freescale_imx8mq_sys",
	},
	{
		.event_table = {
			.pmus = pmu_events__freescale_yitian710_sys,
			.num_pmus = ARRAY_SIZE(pmu_events__freescale_yitian710_sys)
		},
		.metric_table = {
			.pmus = pmu_metrics__freescale_yitian710_sys,
			.num_pmus = ARRAY_SIZE(pmu_metrics__freescale_yitian710_sys)
		},
		.name = "pmu_events__freescale_yitian710_sys",
	},
	{
		.event_table = {
			.pmus = pmu_events__hisilicon_hip09_sys,
			.num_pmus = ARRAY_SIZE(pmu_events__hisilicon_hip09_sys)
		},
		.metric_table = {
			.pmus = pmu_metrics__hisilicon_hip09_sys,
			.num_pmus = ARRAY_SIZE(pmu_metrics__hisilicon_hip09_sys)
		},
		.name = "pmu_events__hisilicon_hip09_sys",
	},
	{
		.event_table = {
			.pmus = pmu_events__test_soc_sys,
			.num_pmus = ARRAY_SIZE(pmu_events__test_soc_sys)
		},
		.name = "pmu_events__test_soc_sys",
	},
	{
		.event_table = { 0, 0 },
		.metric_table = { 0, 0 },
	},
};

static void decompress_event(int offset, struct pmu_event *pe)
{
	const char *p = &big_c_string[offset];

	pe->name = (*p == '\0' ? NULL : p);
	while (*p++);
	pe->topic = (*p == '\0' ? NULL : p);
	while (*p++);
	pe->desc = (*p == '\0' ? NULL : p);
	while (*p++);
	pe->event = (*p == '\0' ? NULL : p);
	while (*p++);
	pe->compat = (*p == '\0' ? NULL : p);
	while (*p++);
	pe->deprecated = *p - '0';
	p++;
	pe->perpkg = *p - '0';
	p++;
	pe->unit = (*p == '\0' ? NULL : p);
	while (*p++);
	pe->long_desc = (*p == '\0' ? NULL : p);
}

static void decompress_metric(int offset, struct pmu_metric *pm)
{
	const char *p = &big_c_string[offset];

	pm->metric_name = (*p == '\0' ? NULL : p);
	while (*p++);
	pm->metric_group = (*p == '\0' ? NULL : p);
	while (*p++);
	pm->metric_expr = (*p == '\0' ? NULL : p);
	while (*p++);
	pm->metric_threshold = (*p == '\0' ? NULL : p);
	while (*p++);
	pm->desc = (*p == '\0' ? NULL : p);
	while (*p++);
	pm->long_desc = (*p == '\0' ? NULL : p);
	while (*p++);
	pm->unit = (*p == '\0' ? NULL : p);
	while (*p++);
	pm->compat = (*p == '\0' ? NULL : p);
	while (*p++);
	pm->metricgroup_no_group = (*p == '\0' ? NULL : p);
	while (*p++);
	pm->default_metricgroup_name = (*p == '\0' ? NULL : p);
	while (*p++);
	pm->aggr_mode = *p - '0';
	p++;
	pm->event_grouping = *p - '0';
}

static int pmu_events_table__for_each_event_pmu(const struct pmu_events_table *table,
                                                const struct pmu_table_entry *pmu,
                                                pmu_event_iter_fn fn,
                                                void *data)
{
        int ret;
        struct pmu_event pe = {
                .pmu = &big_c_string[pmu->pmu_name.offset],
        };

        for (uint32_t i = 0; i < pmu->num_entries; i++) {
                decompress_event(pmu->entries[i].offset, &pe);
                if (!pe.name)
                        continue;
                ret = fn(&pe, table, data);
                if (ret)
                        return ret;
        }
        return 0;
 }

static int pmu_events_table__find_event_pmu(const struct pmu_events_table *table,
                                            const struct pmu_table_entry *pmu,
                                            const char *name,
                                            pmu_event_iter_fn fn,
                                            void *data)
{
        struct pmu_event pe = {
                .pmu = &big_c_string[pmu->pmu_name.offset],
        };
        int low = 0, high = pmu->num_entries - 1;

        while (low <= high) {
                int cmp, mid = (low + high) / 2;

                decompress_event(pmu->entries[mid].offset, &pe);

                if (!pe.name && !name)
                        goto do_call;

                if (!pe.name && name) {
                        low = mid + 1;
                        continue;
                }
                if (pe.name && !name) {
                        high = mid - 1;
                        continue;
                }

                cmp = strcasecmp(pe.name, name);
                if (cmp < 0) {
                        low = mid + 1;
                        continue;
                }
                if (cmp > 0) {
                        high = mid - 1;
                        continue;
                }
  do_call:
                return fn ? fn(&pe, table, data) : 0;
        }
        return -1000;
}

int pmu_events_table__for_each_event(const struct pmu_events_table *table,
                                    struct perf_pmu *pmu,
                                    pmu_event_iter_fn fn,
                                    void *data)
{
        for (size_t i = 0; i < table->num_pmus; i++) {
                const struct pmu_table_entry *table_pmu = &table->pmus[i];
                const char *pmu_name = &big_c_string[table_pmu->pmu_name.offset];
                int ret;

                if (pmu && !pmu__name_match(pmu, pmu_name))
                        continue;

                ret = pmu_events_table__for_each_event_pmu(table, table_pmu, fn, data);
                if (pmu || ret)
                        return ret;
        }
        return 0;
}

int pmu_events_table__find_event(const struct pmu_events_table *table,
                                 struct perf_pmu *pmu,
                                 const char *name,
                                 pmu_event_iter_fn fn,
                                 void *data)
{
        for (size_t i = 0; i < table->num_pmus; i++) {
                const struct pmu_table_entry *table_pmu = &table->pmus[i];
                const char *pmu_name = &big_c_string[table_pmu->pmu_name.offset];
                int ret;

                if (!pmu__name_match(pmu, pmu_name))
                        continue;

                ret = pmu_events_table__find_event_pmu(table, table_pmu, name, fn, data);
                if (ret != -1000)
                        return ret;
        }
        return -1000;
}

size_t pmu_events_table__num_events(const struct pmu_events_table *table,
                                    struct perf_pmu *pmu)
{
        size_t count = 0;

        for (size_t i = 0; i < table->num_pmus; i++) {
                const struct pmu_table_entry *table_pmu = &table->pmus[i];
                const char *pmu_name = &big_c_string[table_pmu->pmu_name.offset];

                if (pmu__name_match(pmu, pmu_name))
                        count += table_pmu->num_entries;
        }
        return count;
}

static int pmu_metrics_table__for_each_metric_pmu(const struct pmu_metrics_table *table,
                                                const struct pmu_table_entry *pmu,
                                                pmu_metric_iter_fn fn,
                                                void *data)
{
        int ret;
        struct pmu_metric pm = {
                .pmu = &big_c_string[pmu->pmu_name.offset],
        };

        for (uint32_t i = 0; i < pmu->num_entries; i++) {
                decompress_metric(pmu->entries[i].offset, &pm);
                if (!pm.metric_expr)
                        continue;
                ret = fn(&pm, table, data);
                if (ret)
                        return ret;
        }
        return 0;
}

int pmu_metrics_table__for_each_metric(const struct pmu_metrics_table *table,
                                     pmu_metric_iter_fn fn,
                                     void *data)
{
        for (size_t i = 0; i < table->num_pmus; i++) {
                int ret = pmu_metrics_table__for_each_metric_pmu(table, &table->pmus[i],
                                                                 fn, data);

                if (ret)
                        return ret;
        }
        return 0;
}

const struct pmu_events_table *perf_pmu__find_events_table(struct perf_pmu *pmu)
{
        const struct pmu_events_table *table = NULL;
        char *cpuid = perf_pmu__getcpuid(pmu);
        size_t i;

        /* on some platforms which uses cpus map, cpuid can be NULL for
         * PMUs other than CORE PMUs.
         */
        if (!cpuid)
                return NULL;

        i = 0;
        for (;;) {
                const struct pmu_events_map *map = &pmu_events_map[i++];
                if (!map->arch)
                        break;

                if (!strcmp_cpuid_str(map->cpuid, cpuid)) {
                        table = &map->event_table;
                        break;
                }
        }
        free(cpuid);
        if (!pmu || !table)
                return table;

        for (i = 0; i < table->num_pmus; i++) {
                const struct pmu_table_entry *table_pmu = &table->pmus[i];
                const char *pmu_name = &big_c_string[table_pmu->pmu_name.offset];

                if (pmu__name_match(pmu, pmu_name))
                        return table;
        }
        return NULL;
}

const struct pmu_metrics_table *perf_pmu__find_metrics_table(struct perf_pmu *pmu)
{
        const struct pmu_metrics_table *table = NULL;
        char *cpuid = perf_pmu__getcpuid(pmu);
        int i;

        /* on some platforms which uses cpus map, cpuid can be NULL for
         * PMUs other than CORE PMUs.
         */
        if (!cpuid)
                return NULL;

        i = 0;
        for (;;) {
                const struct pmu_events_map *map = &pmu_events_map[i++];
                if (!map->arch)
                        break;

                if (!strcmp_cpuid_str(map->cpuid, cpuid)) {
                        table = &map->metric_table;
                        break;
                }
        }
        free(cpuid);
        return table;
}

const struct pmu_events_table *find_core_events_table(const char *arch, const char *cpuid)
{
        for (const struct pmu_events_map *tables = &pmu_events_map[0];
             tables->arch;
             tables++) {
                if (!strcmp(tables->arch, arch) && !strcmp_cpuid_str(tables->cpuid, cpuid))
                        return &tables->event_table;
        }
        return NULL;
}

const struct pmu_metrics_table *find_core_metrics_table(const char *arch, const char *cpuid)
{
        for (const struct pmu_events_map *tables = &pmu_events_map[0];
             tables->arch;
             tables++) {
                if (!strcmp(tables->arch, arch) && !strcmp_cpuid_str(tables->cpuid, cpuid))
                        return &tables->metric_table;
        }
        return NULL;
}

int pmu_for_each_core_event(pmu_event_iter_fn fn, void *data)
{
        for (const struct pmu_events_map *tables = &pmu_events_map[0];
             tables->arch;
             tables++) {
                int ret = pmu_events_table__for_each_event(&tables->event_table,
                                                           /*pmu=*/ NULL, fn, data);

                if (ret)
                        return ret;
        }
        return 0;
}

int pmu_for_each_core_metric(pmu_metric_iter_fn fn, void *data)
{
        for (const struct pmu_events_map *tables = &pmu_events_map[0];
             tables->arch;
             tables++) {
                int ret = pmu_metrics_table__for_each_metric(&tables->metric_table, fn, data);

                if (ret)
                        return ret;
        }
        return 0;
}

const struct pmu_events_table *find_sys_events_table(const char *name)
{
        for (const struct pmu_sys_events *tables = &pmu_sys_event_tables[0];
             tables->name;
             tables++) {
                if (!strcmp(tables->name, name))
                        return &tables->event_table;
        }
        return NULL;
}

int pmu_for_each_sys_event(pmu_event_iter_fn fn, void *data)
{
        for (const struct pmu_sys_events *tables = &pmu_sys_event_tables[0];
             tables->name;
             tables++) {
                int ret = pmu_events_table__for_each_event(&tables->event_table,
                                                           /*pmu=*/ NULL, fn, data);

                if (ret)
                        return ret;
        }
        return 0;
}

int pmu_for_each_sys_metric(pmu_metric_iter_fn fn, void *data)
{
        for (const struct pmu_sys_events *tables = &pmu_sys_event_tables[0];
             tables->name;
             tables++) {
                int ret = pmu_metrics_table__for_each_metric(&tables->metric_table, fn, data);

                if (ret)
                        return ret;
        }
        return 0;
}

static const int metricgroups[][2] = {

};

const char *describe_metricgroup(const char *group)
{
        int low = 0, high = (int)ARRAY_SIZE(metricgroups) - 1;

        while (low <= high) {
                int mid = (low + high) / 2;
                const char *mgroup = &big_c_string[metricgroups[mid][0]];
                int cmp = strcmp(mgroup, group);

                if (cmp == 0) {
                        return &big_c_string[metricgroups[mid][1]];
                } else if (cmp < 0) {
                        low = mid + 1;
                } else {
                        high = mid - 1;
                }
        }
        return NULL;
}
