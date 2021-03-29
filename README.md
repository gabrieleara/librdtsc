# librdtsc

A simple multi-platform library for reading TSC values on x86-64 and ARM architectures.

## Disclaimer

Most of the code in this library is adapted from the implementation of a similar mechanism in DPDK project.
Most source and header files maintain original copyright notices on top, since I just adjusted function names and a few things here and there.
In the adaptation process, I might have broken some stuff here and there, but the most essential functionality of the library remains.

## Tested On

For now, the library has been tested on Linux on a x86-64 platform. Testing on ARM platforms (both v8 and ARM64) will soon be carried out.
