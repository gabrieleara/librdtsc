#ifndef LIBRDTSC_CYCLES_H
#define LIBRDTSC_CYCLES_H

#include <librdtsc/config.h>
#include <librdtsc/generic/cycles.h>

// Only one of these toggles must be on at any given time

#ifdef LIBRDTSC_ARCH_ARM
# include "arm/cycles_32.h"
#endif

#ifdef LIBRDTSC_ARCH_ARM64
#include "arm/cycles_64.h"
#endif

#ifdef LIBRDTSC_ARCH_x86
#  include "x86/cycles.h"
#endif

#endif // LIBRDTSC_CYCLES_H
