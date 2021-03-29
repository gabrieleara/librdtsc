#ifndef LIBRDTSC_COMMON_TIMER_H
#define LIBRDTSC_COMMON_TIMER_H

#include <librdtsc/common.h>

extern uint64_t rdtsc_get_tsc_freq_arch();
extern uint64_t rdtsc_get_tsc_freq();
// extern uint64_t rdtsc_estimate_tsc_freq();

extern int rdtsc_init();
extern void rdtsc_set_tsc_freq();
extern uint64_t rdtsc_get_tsc_freq();

#endif
