//
// Created by Dangyi Liu on 4/25/17.
//

#ifndef IBEN_TEST_H

#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <x86intrin.h>

uint64_t global_count;

#define BEGIN_TEST(name, count) \
    BEGIN_TEST_PREP(name) \
    BEGIN_TEST_LOOP(count)

#define END_TEST }}

#define BEGIN_TEST_PREP(name) \
    __attribute__((noinline)) \
    void test_##name(int times) {

#define BEGIN_TEST_LOOP(count) \
        global_count = count; \
        const upper = times * global_count; \
        for (int i = 0; i < upper; i++) {

#define END_TEST_LOOP }
#define END_TEST_PREP }

#define IBEN_TEST_H

#endif //IBEN_TEST_H
