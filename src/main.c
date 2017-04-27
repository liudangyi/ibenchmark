//
// Created by Dangyi Liu on 4/25/17.
//

#include <stdio.h>
#include <x86intrin.h>
#include <unistd.h>
#include "test.h"

#define TSC_FREQ 2.592000000

#ifndef TEST_NAME
#define TEST_NAME test_loop
#endif

uint64_t timeit() {
    uint64_t s1, e1, s2, e2;

    s1 = __rdtsc();
    TEST_NAME(1);
    e1 = __rdtsc();

    s2 = __rdtsc();
    TEST_NAME(2);
    e2 = __rdtsc();

    return (e2 - s2) - (e1 - s1);
}

int main(int argc, char const *argv[]) {
    uint64_t duration = timeit();
    double cpi = ((double) duration) / global_count;
    printf("count = %d, cycles = %f, time = %fns\n", global_count, cpi, cpi / TSC_FREQ);
    printf("%.2f cycles, %.2f ns\n", cpi, cpi / TSC_FREQ);
    return 0;
}
