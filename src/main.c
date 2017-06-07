//
// Created by Dangyi Liu on 4/25/17.
//

#include <stdio.h>
#include <x86intrin.h>
#include <unistd.h>
#include <math.h>
#include "test.h"

#define TSC_FREQ 2.592000000

void (*_test_func)();

uint64_t time_single(int times) {
    uint32_t cycles_low0, cycles_high0, cycles_low1, cycles_high1;

    global_times = times;

    asm volatile ("cpuid\n\t"
                  "rdtsc\n\t"
                  "mov %%edx, %0\n\t"
                  "mov %%eax, %1\n\t"
                  : "=r" (cycles_high0), "=r" (cycles_low0)
                  :: "%rax", "%rbx", "%rcx", "%rdx");

    _test_func();

    asm volatile ("rdtscp\n\t"
                  "mov %%edx, %0\n\t"
                  "mov %%eax, %1\n\t"
                  "cpuid\n\t"
                  : "=r" (cycles_high1), "=r" (cycles_low1)
                  :: "%rax", "%rbx", "%rcx", "%rdx");

    return (((uint64_t) cycles_high1 << 32) + cycles_low1) -
           (((uint64_t) cycles_high0 << 32) + cycles_low0);
}

uint64_t time_delta() {
    return time_single(2) - time_single(1);
}

int main(int argc, char const *argv[]) {
    int TEST_NUMBER = atoi(argv[1]);
    const char *TEST_NAME = argv[2];
    double cpis[TEST_NUMBER];

    _test_func = (void *) main + strtol(argv[3], 0, 0);
    printf("========== %s @ %p ==========\n", TEST_NAME, _test_func);
    fflush(stdout);
    for (int i = 0; i < TEST_NUMBER; i++) {
        uint64_t duration;
        double cpi;

        duration = time_delta();
        cpi = ((double) duration) / global_count;
        printf("count = %llu, cycles = %f, time = %fns\n", global_count, cpi, cpi / TSC_FREQ);
        cpis[i] = cpi;
    }

    double mean = 0;
    for (int i = 0; i < TEST_NUMBER; i++) {
        mean += cpis[i];
    }
    mean /= TEST_NUMBER;

    double sd = 0;
    for (int i = 0; i < TEST_NUMBER; i++) {
        sd += (cpis[i] - mean) * (cpis[i] - mean);
    }
    sd /= TEST_NUMBER - 1;
    sd = sqrt(sd);

    printf("mean = %.2f cycles (%.2f ns), sd = %.2f cycles\n", mean, mean / TSC_FREQ, sd);
}
