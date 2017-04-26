//
// Created by Dangyi Liu on 4/25/17.
//

#ifndef IBEN_TEST_H

#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <x86intrin.h>

int global_count;

#define BEGIN_TEST(name, count) \
    void test_##name(int time) { \
        global_count = count; \
        for (int i = 0; i < time * count; ++i) {

#define END_TEST }}

#define IBEN_TEST_H

#endif //IBEN_TEST_H
