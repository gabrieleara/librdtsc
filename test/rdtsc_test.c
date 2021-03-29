#include <librdtsc/rdtsc.h>
#include <stdio.h>

int main() {

    int res = rdtsc_init();
    if (res != 0) {
        printf("ERROR DURING INITIALIZATION!!!\n");
        return res;
    }
    uint64_t tsc_hz = rdtsc_get_tsc_hz();

    uint64_t tsc_old = rdtsc();
    sleep(1);
    uint64_t tsc_new = rdtsc();

    printf("TSC OLD: %ld\n", tsc_old);
    printf("TSC NEW: %ld\n", tsc_new);
    printf("TSC DIFF: %ld\n", tsc_new - tsc_old);
    printf("TSC HZ: %ld\n", tsc_hz);

    return 0;
}
