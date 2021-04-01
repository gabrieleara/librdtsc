# librdtsc

A simple multi-platform library for reading TSC values on x86-64 and ARM
architectures.

## Disclaimer

Most of the code in this library is adapted from the implementation of a similar
mechanism in DPDK project. Most source and header files maintain original
copyright notices on top, since I just adjusted function names and a few things
here and there. In the adaptation process, I might have broken some stuff here
and there, but the most essential functionality of the library remains.

## Tested On

For now, the library has been tested on Linux on a x86-64 platform and an ARM64 one. Testing on
other ARM platforms will soon be carried out.

## Configuring the library

This project uses CMake as build system. It provides a set of CMake option
toggles that must be set `ON` or `OFF` to configure the build system before
building for any platform.

The toggles are used to identify the target OS and architecture, as well as the
features of each architecture that should be enabled.

### Selecting Target OS

For now, only Linux is supported, hence the following option should always be `ON` (default):
- `LIBRDTSC_OS_LINUX`

### Selecting Target Architecture
The target architecture can be selected by enabling **only one** among the
following toggles (the others **must be set to `OFF`**):
- `LIBRDTSC_ARCH_x86` (default `ON`) for x86-64 platforms
- `LIBRDTSC_ARCH_ARM` for ARM 32-bit platforms
- `LIBRDTSC_ARCH_ARM64` for ARM64 platforms

### Selecting Target Architecture Features
The library supports multiple mechanisms to access the TSC value on the
supported platforms. These can be enabled using the following toggles (both are
`OFF` by default):
- `LIBRDTSC_USE_HPET` to enable HPET usage on x86-64 platforms (**currently not
  working**)
- `LIBRDTSC_USE_PMC` to enable PMU usage on ARM platforms (**requires some
  options enabled from privileged kernel context**)

I personally suggest users to leave either of these features disabled.

## Building the library

To build the library, once the set of toggles to enable/disabled has been
decided, use the following snippet from the project top level directory:
```bash
mkdir build
cd build
cmake <options> ..
```
where options should be the complete list of CMake options to enable/disable.

Example for Linux x86-64 (nothing is selected because this is the default
configuration):
```bash
mkdir build
cd build
cmake ..
```

Example for Linux ARM64:
```bash
mkdir build
cd build
cmake -DLIBRDTSC_ARCH_x86=OFF -DLIBRDTSC_ARCH_ARM64=ON ..
```

I understand that this is not the most user-friendly way of providing
parameters, but it was very quick for me to implement. Future revisions of the
library may change this behavior.

## Installing the library

Once built, simply run `make install` (with the appropriate privileges) to install the library in the system.

## Writing an app using librdtsc

A sample application is provided in the [test](test) directory. For simplicity,
the code is also included here.

This sample application should output a difference between `NEW` and `OLD` TSC
values that is approximately equal to the `TSC HZ` value. While a more accurate
implementation should probably use `clock_nanosleep` and similar precision, to
get familiar with the library this should be enough.

```C
#include <librdtsc/rdtsc.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    // Initialization must be performed only if rdtsc_get_tsc_hz() is used.
    // The basic rdtsc() call does not require any initialization.
    int res = rdtsc_init();
    if (res != 0) return res;

    uint64_t tsc_hz = rdtsc_get_tsc_hz();
    uint64_t tsc_old = rdtsc();
    sleep(1);
    uint64_t tsc_new = rdtsc();

    printf("TSC OLD:  %ld\n", tsc_old);
    printf("TSC NEW:  %ld\n", tsc_new);
    printf("TSC DIFF: %ld\n", tsc_new - tsc_old);
    printf("TSC HZ:   %ld\n", tsc_hz);
    printf("Elapsed time ( s): %ld\n", rdtsc_elapsed_s(tsc_old, tsc_new));
    printf("Elapsed time (ms): %ld\n", rdtsc_elapsed_ms(tsc_old, tsc_new));
    printf("Elapsed time (us): %ld\n", rdtsc_elapsed_us(tsc_old, tsc_new));
    printf("Elapsed time (ns): %ld\n", rdtsc_elapsed_ns(tsc_old, tsc_new));

    return 0;
}
```
