#ifndef LIBRDTSC_COMMON_TIMER_H
#define LIBRDTSC_COMMON_TIMER_H

#include <librdtsc/common.h>

extern uint64_t rdtsc_get_tsc_freq_arch();
extern uint64_t rdtsc_get_tsc_freq();
// extern uint64_t rdtsc_estimate_tsc_freq();

extern int rdtsc_init();
extern void rdtsc_set_tsc_freq();
extern uint64_t rdtsc_get_tsc_freq();
extern uint64_t rdtsc_get_tsc_hz();

#define MS_PER_S 1000
#define US_PER_S 1000000
#define NS_PER_S 1000000000

static inline uint64_t rdtsc_elapsed_unit(uint64_t before, uint64_t after,
                                          uint64_t multiplier) {
#define static_cast(t, x) ((t)(x))
    register uint64_t hz = rdtsc_get_tsc_hz();
    if (after < before)
        return 0;

    return (static_cast(double, after - before) *
            static_cast(double, multiplier)) /
           static_cast(double, hz);
#undef static_cast
}

static inline uint64_t rdtsc_elapsed_ns(uint64_t before, uint64_t after) {
    return rdtsc_elapsed_unit(before, after, NS_PER_S);
}

static inline uint64_t rdtsc_elapsed_us(uint64_t before, uint64_t after) {
    return rdtsc_elapsed_unit(before, after, US_PER_S);
}

static inline uint64_t rdtsc_elapsed_ms(uint64_t before, uint64_t after) {
    return rdtsc_elapsed_unit(before, after, MS_PER_S);
}

static inline uint64_t rdtsc_elapsed_s(uint64_t before, uint64_t after) {
    return rdtsc_elapsed_unit(before, after, 1);
}

#endif
