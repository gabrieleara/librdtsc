/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2010-2014 Intel Corporation.
 * Copyright(c) 2013 6WIND S.A.
 */

#ifndef LIBRDTSC_CYCLES_X86_64_H
#define LIBRDTSC_CYCLES_X86_64_H

#include <librdtsc/config.h>
#include <librdtsc/common.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "cycles.h"

#ifdef LIBRDTSC_VMWARE_TSC_MAP_SUPPORT
/* Global switch to use VMWARE mapping of TSC instead of RDTSC */
extern int rdtsc_cycles_vmware_tsc_map;
#include <rdtsc_branch_prediction.h>
#endif
// #include <rdtsc_common.h>
// #include <rdtsc_config.h>

static inline uint64_t rdtsc(void) {
    union {
        uint64_t tsc_64;
        RDTSC_STD_C11
        struct {
            uint32_t lo_32;
            uint32_t hi_32;
        };
    } tsc;

#ifdef LIBRDTSC_VMWARE_TSC_MAP_SUPPORT
    if (unlikely(rdtsc_cycles_vmware_tsc_map)) {
        /* ecx = 0x10000 corresponds to the physical TSC for VMware */
        asm volatile("rdpmc" : "=a"(tsc.lo_32), "=d"(tsc.hi_32) : "c"(0x10000));
        return tsc.tsc_64;
    }
#endif

    asm volatile("rdtsc" : "=a"(tsc.lo_32), "=d"(tsc.hi_32));
    return tsc.tsc_64;
}

static void rdtsc_mb() {} // FIXME: implement a proper memory barrier!

static inline uint64_t rdtsc_precise(void) {
    rdtsc_mb();
    return rdtsc();
}

static inline uint64_t rdtsc_get_tsc_cycles(void) {
    return rdtsc();
}

#ifdef __cplusplus
}
#endif

#endif /* LIBRDTSC_CYCLES_X86_64_H */
