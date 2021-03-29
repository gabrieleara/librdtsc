/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright(c) 2010-2014 Intel Corporation.
 * Copyright(c) 2013 6WIND S.A.
 */

#ifndef LIBRDTSC_GENERIC_CYCLES_H
#define LIBRDTSC_GENERIC_CYCLES_H

#include <librdtsc/config.h>
#include <librdtsc/common.h>

/**
 * @file
 *
 * Simple Time Reference Functions (Cycles and HPET).
 */

// #include <atomic.h>
// #include <compat.h>
// #include <debug.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#define MS_PER_S 1000
#define US_PER_S 1000000
#define NS_PER_S 1000000000

enum timer_source {
    RDTSC_TIMER_TSC = 0,
#ifdef LIBRDTSC_USE_HPET
    RDTSC_TIMER_HPET = 1,
#endif
};
extern enum timer_source timer_source;

/**
 * Get the measured frequency of the RDTSC counter
 *
 * @return
 *   The TSC frequency for this lcore
 */
uint64_t rdtsc_get_tsc_hz(void);

/**
 * Return the number of TSC cycles since boot
 *
 * @return
 *   the number of cycles
 */
static inline uint64_t rdtsc_get_tsc_cycles(void);

#ifdef LIBRDTSC_USE_HPET
/**
 * Return the number of HPET cycles since boot
 *
 * This counter is global for all execution units. The number of
 * cycles in one second can be retrieved using rdtsc_get_hpet_hz().
 *
 * @return
 *   the number of cycles
 */
uint64_t rdtsc_get_hpet_cycles(void);

/**
 * Get the number of HPET cycles in one second.
 *
 * @return
 *   The number of cycles in one second.
 */
uint64_t rdtsc_get_hpet_hz(void);

/**
 * Initialise the HPET for use. This must be called before the rdtsc_get_hpet_hz
 * and rdtsc_get_hpet_cycles APIs are called. If this function does not succeed,
 * then the HPET functions are unavailable and should not be called.
 *
 * @param make_default
 *	If set, the hpet timer becomes the default timer whose values are
 *	returned by the rdtsc_get_timer_hz/cycles API calls
 *
 * @return
 *	0 on success,
 *	-1 on error, and the make_default parameter is ignored.
 */
int hpet_init(int make_default);

#endif

/**
 * Get the number of cycles since boot from the default timer.
 *
 * @return
 *   The number of cycles
 */
static inline uint64_t rdtsc_get_timer_cycles(void) {
#ifdef LIBRDTSC_USE_HPET
    switch (timer_source) {
    case RDTSC_TIMER_TSC:
#endif
        return rdtsc_get_tsc_cycles();
#ifdef LIBRDTSC_USE_HPET
    case RDTSC_TIMER_HPET:
        return rdtsc_get_hpet_cycles();
    default:
        assert(false && "Invalid timer source specified\n");
        return 0;
    }
#endif
}

/**
 * Get the number of cycles in one second for the default timer.
 *
 * @return
 *   The number of cycles in one second.
 */
static inline uint64_t rdtsc_get_timer_hz(void) {
#ifdef LIBRDTSC_USE_HPET
    switch (timer_source) {
    case RDTSC_TIMER_TSC:
#endif
        return rdtsc_get_tsc_hz();
#ifdef LIBRDTSC_USE_HPET
    case RDTSC_TIMER_HPET:
        return rdtsc_get_hpet_hz();
    default:
        assert(false && "Invalid timer source specified\n");
        return 0;
    }
#endif
}
/**
 * Wait at least us microseconds.
 * This function can be replaced with user-defined function.
 * @see delay_us_callback_register
 *
 * @param us
 *   The number of microseconds to wait.
 */
extern void (*rdtsc_delay_us)(unsigned int us);

/**
 * Wait at least ms milliseconds.
 *
 * @param ms
 *   The number of milliseconds to wait.
 */
static inline void rdtsc_delay_ms(unsigned ms) {
    rdtsc_delay_us(ms * 1000);
}

/**
 * Blocking delay function.
 *
 * @param us
 *   Number of microseconds to wait.
 */
void rdtsc_delay_us_block(unsigned int us);

/**
 * Delay function that uses system sleep.
 * Does not block the CPU core.
 *
 * @param us
 *   Number of microseconds to wait.
 */
__experimental void rdtsc_delay_us_sleep(unsigned int us);

/**
 * Replace delay_us with user defined function.
 *
 * @param userfunc
 *   User function which replaces delay_us. delay_us_block restores
 *   builtin block delay function.
 */
void rdtsc_delay_us_callback_register(void (*userfunc)(unsigned int));

#endif /* LIBRDTSC_GENERIC_CYCLES_H */
