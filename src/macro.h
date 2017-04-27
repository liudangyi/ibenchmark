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
    void test_##name(int times) { \
        global_count = count; \
        for (int i = 0; i < times * count; ++i) {

#define END_TEST }}

#define BEGIN_TEST_PREP(name) \
    void test_##name(int times) {

#define BEGIN_TEST_LOOP(count) \
        global_count = count; \
        for (int i = 0; i < times * count; ++i) {

#define END_TEST_LOOP }
#define END_TEST_PREP }

#define IBEN_TEST_H

#endif //IBEN_TEST_H
