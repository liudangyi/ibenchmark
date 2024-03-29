//
// Created by Dangyi Liu on 4/25/17.
//

#ifndef IBEN_TEST_H

#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <x86intrin.h>

uint64_t global_count;
int global_times;

#define BEGIN_TEST(name, count) \
    BEGIN_TEST_PREP(name) \
    BEGIN_TEST_LOOP(count)

#define END_TEST }}

#define BEGIN_TEST_PREP(name) \
    __attribute__((noinline)) \
    void test_##name() {

#define BEGIN_TEST_LOOP(count) \
        global_count = count; \
        const uint64_t upper = global_times * global_count; \
        for (uint64_t i = 0; i < upper; i++) {

#define BEGIN_TEST_ONCE \
        global_count = 1; \
        if (global_times == 2) {

#define END_TEST_ONCE }
#define END_TEST_LOOP }
#define END_TEST_PREP }

#define IBEN_TEST_H

#endif //IBEN_TEST_H
