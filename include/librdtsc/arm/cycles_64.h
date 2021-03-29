/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2015 Cavium, Inc
 * Copyright(c) 2020 Arm Limited
 */

#ifndef LIBRDTSC_ARM_CYCLES_64_H
#define LIBRDTSC_ARM_CYCLES_64_H

#include <librdtsc/config.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <librdtsc/generic/cycles.h>

/** Read generic counter frequency */
static __always_inline uint64_t __arm64_cntfrq(void) {
    uint64_t freq;

    asm volatile("mrs %0, cntfrq_el0" : "=r"(freq));
    return freq;
}

/** Read generic counter */
static __always_inline uint64_t __arm64_cntvct(void) {
    uint64_t tsc;

    asm volatile("mrs %0, cntvct_el0" : "=r"(tsc));
    return tsc;
}

static __always_inline uint64_t __arm64_cntvct_precise(void) {
    asm volatile("isb" : : : "memory");
    return __arm64_cntvct();
}

/**
 * Read the time base register.
 *
 * @return
 *   The time base for this lcore.
 */
#ifndef LIBRDTSC_USE_PMU
/**
 * This call is portable to any ARMv8 architecture, however, typically
 * cntvct_el0 runs at <= 100MHz and it may be imprecise for some tasks.
 */
static __always_inline uint64_t rdtsc(void) {
    return __arm64_cntvct();
}
#else // LIBRDTSC_USE_PMU
/**
 * This is an alternative method to enable rdtsc() with high resolution
 * PMU cycles counter.The cycle counter runs at cpu frequency and this scheme
 * uses ARMv8 PMU subsystem to get the cycle counter at userspace, However,
 * access to PMU cycle counter from user space is not enabled by default in
 * arm64 linux kernel.
 * It is possible to enable cycle counter at user space access by configuring
 * the PMU from the privileged mode (kernel space).
 *
 * asm volatile("msr pmintenset_el1, %0" : : "r" ((u64)(0 << 31)));
 * asm volatile("msr pmcntenset_el0, %0" :: "r" BIT(31));
 * asm volatile("msr pmuserenr_el0, %0" : : "r"(BIT(0) | BIT(2)));
 * asm volatile("mrs %0, pmcr_el0" : "=r" (val));
 * val |= (BIT(0) | BIT(2));
 * isb();
 * asm volatile("msr pmcr_el0, %0" : : "r" (val));
 *
 */

/** Read PMU cycle counter */
static __always_inline uint64_t __arm64_pmccntr(void) {
    uint64_t tsc;

    asm volatile("mrs %0, pmccntr_el0" : "=r"(tsc));
    return tsc;
}

static __always_inline uint64_t rdtsc(void) {
    return __arm64_pmccntr();
}
#endif

static __always_inline uint64_t rdtsc_precise(void) {
    asm volatile("isb" : : : "memory");
    return rdtsc();
}

static __always_inline uint64_t rdtsc_get_tsc_cycles(void) {
    return rdtsc();
}

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // LIBRDTSC_ARM_CYCLES_64_H
