#include <librdtsc/config.h>

// One and only one of these toggles must be enabled at any time!

#ifdef LIBRDTSC_ARCH_x86
#include "x86/cycles.c"
#endif

#ifdef LIBRDTSC_ARCH_ARM
#include "arm/cycles.c"
#endif

#ifdef LIBRDTSC_ARCH_ARM64
#include "arm/cycles.c"
#endif
