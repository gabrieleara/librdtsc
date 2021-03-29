#ifndef LIBRDTSC_CYCLES_C
#define LIBRDTSC_CYCLES_C

/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2015 Cavium, Inc
 */

#include <librdtsc/cycles.h>

uint64_t rdtsc_get_tsc_freq_arch(void) {
#if defined ARCH_ARM64 && !defined LIBRDTSC_USE_PMU
    return __arm64_cntfrq();
#elif defined ARCH_ARM64 && defined LIBRDTSC_USE_PMU
#define CYC_PER_1MHZ 1E6
    /* Use the generic counter ticks to calculate the PMU
     * cycle frequency.
     */
    uint64_t ticks;
    uint64_t start_ticks, cur_ticks;
    uint64_t start_pmu_cycles, end_pmu_cycles;

    /* Number of ticks for 1/10 second */
    ticks = __arm64_cntfrq() / 10;

    start_ticks = __arm64_cntvct_precise();
    start_pmu_cycles = rdtsc_precise();
    do {
        cur_ticks = __arm64_cntvct();
    } while ((cur_ticks - start_ticks) < ticks);
    end_pmu_cycles = rdtsc_precise();

    /* Adjust the cycles to next 1Mhz */
    return ALIGN_MUL_CEIL(end_pmu_cycles - start_pmu_cycles, CYC_PER_1MHZ) *
           10;
#else
    return 0;
#endif
}

#endif // LIBRDTSC_CYCLES_C
