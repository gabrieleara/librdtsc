/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2010-2014 Intel Corporation
 */

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include <librdtsc/common.h>
// #include <rdtsc_compat.h>
#include <librdtsc/cycles.h>
#include <librdtsc/common_timer.h>
// #include "eal_memcfg.h"
// #include "eal_private.h"

/* The frequency of the RDTSC timer resolution */
static uint64_t rdtsc_resolution_hz;

/* Pointer to user delay function */
void (*rdtsc_delay_us)(unsigned int) = NULL;

void rdtsc_delay_us_block(unsigned int us) {
    const uint64_t start = rdtsc_get_timer_cycles();
    const uint64_t ticks = (uint64_t)us * rdtsc_get_timer_hz() / 1E6;
    while ((rdtsc_get_timer_cycles() - start) < ticks)
        // rdtsc_pause()
        ;
}

uint64_t rdtsc_get_tsc_hz(void) {
    return rdtsc_resolution_hz;
}

static uint64_t rdtsc_estimate_tsc_freq(void) {
#define CYC_PER_10MHZ 1E7
    // rdtsc_LOG(WARNING, EAL,
            // "WARNING: TSC frequency estimated roughly"
            // " - clock timings may be less accurate.\n");
    /* assume that the sleep(1) will sleep for 1 second */
    uint64_t start = rdtsc();
    sleep(1);
    /* Round up to 10Mhz. 1E7 ~ 10Mhz */
    return RDTSC_ALIGN_MUL_NEAR(rdtsc() - start, CYC_PER_10MHZ);
}

// NOTICE: this function works only on Linux
uint64_t get_tsc_freq(void) {
// #ifdef CLOCK_MONOTONIC_RAW
#define NS_PER_SEC 1E9
#define CYC_PER_10MHZ 1E7

    struct timespec sleeptime = {.tv_nsec = NS_PER_SEC / 10}; /* 1/10 second */

    struct timespec t_start, t_end;
    uint64_t tsc_hz;

    if (clock_gettime(CLOCK_MONOTONIC_RAW, &t_start) == 0) {
        uint64_t ns, end, start = rdtsc();
        nanosleep(&sleeptime, NULL);
        clock_gettime(CLOCK_MONOTONIC_RAW, &t_end);
        end = rdtsc();
        ns = ((t_end.tv_sec - t_start.tv_sec) * NS_PER_SEC);
        ns += (t_end.tv_nsec - t_start.tv_nsec);

        double secs = (double)ns / NS_PER_SEC;
        tsc_hz = (uint64_t)((end - start) / secs);
        /* Round up to 10Mhz. 1E7 ~ 10Mhz */
        return RDTSC_ALIGN_MUL_NEAR(tsc_hz, CYC_PER_10MHZ);
    }
// #endif
    return 0;
}

void rdtsc_set_tsc_freq(void) {
    // struct rdtsc_mem_config *mcfg = rdtsc_eal_get_configuration()->mem_config;
    uint64_t freq;

    // if (rdtsc_eal_process_type() == rdtsc_PROC_SECONDARY) {
    //     /*
    //      * Just use the primary process calculated TSC rate in any
    //      * secondary process.  It avoids any unnecessary overhead on
    //      * systems where arch-specific frequency detection is not
    //      * available.
    //      */
    //     rdtsc_resolution_hz = mcfg->tsc_hz;
    //     return;
    // }

    freq = rdtsc_get_tsc_freq_arch();
    if (!freq)
        freq = rdtsc_get_tsc_freq();
    if (!freq)
        freq = rdtsc_estimate_tsc_freq();

    // rdtsc_LOG(DEBUG, EAL, "TSC frequency is ~%" PRIu64 " KHz\n", freq / 1000);
    rdtsc_resolution_hz = freq;
    // mcfg->tsc_hz = freq;
}

void rdtsc_delay_us_callback_register(void (*userfunc)(unsigned int)) {
    rdtsc_delay_us = userfunc;
}

// RDTSC_INIT(rdtsc_timer_init) {
//     /* set rdtsc_delay_us_block as a delay function */
//     rdtsc_delay_us_callback_register(rdtsc_delay_us_block);
// }
