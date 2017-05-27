#include <string.h>

typedef struct linked_list {
    struct linked_list* next;
} linked_list;

/*
 * L1 cache is 32 KB with 8-way associativity
 * ....------------|---|---6---|---6---|
 *                tag    index   offset
 *
 * L2 cache is 256 KB with 4-way associativity
 * ....--------|--|-----10-----|---6---|
 *            tag      index     offset
 *
 * L3 cache is (up-to) 8 MB with 16-way associativity
 * ....---|----|-------13------|---6---|
 *       tag         index       offset
 */
void general_latency_test(uint64_t size, int times, int times2) {
    linked_list *list = malloc(size);
    linked_list *p = list;

    // flush the memory
    for (int i = 0; i < (size >> 3); i++) {
      list[i].next = &list[i+1];
    }

    for (int i = 0; i < (size >> 12); i++) { // this is the size of L1 cache
        list[i << 9].next = &list[(i + 1) << 9];
    }
    if (size >> 12) {
      list[((size >> 12) - 1) << 9].next = list;
    } else {
      list->next = list;
    }
BEGIN_TEST_LOOP(times2)
    p = p->next;
END_TEST_LOOP
    p->next = p;
    free(list);
}

void test_mem_latency_1k(int times) { general_latency_test(1 << 10, times, 4e7); }
void test_mem_latency_2k(int times) { general_latency_test(2 << 10, times, 4e7); }
void test_mem_latency_4k(int times) { general_latency_test(4 << 10, times, 4e7); }
void test_mem_latency_8k(int times) { general_latency_test(8 << 10, times, 4e7); }
void test_mem_latency_16k(int times) { general_latency_test(16 << 10, times, 2e7); }
void test_mem_latency_32k(int times) { general_latency_test(32 << 10, times, 2e7); }
void test_mem_latency_64k(int times) { general_latency_test(64 << 10, times, 1e7); }
void test_mem_latency_128k(int times) { general_latency_test(128 << 10, times, 1e7); }
void test_mem_latency_256k(int times) { general_latency_test(256 << 10, times, 1e7); }
void test_mem_latency_512k(int times) { general_latency_test(512 << 10, times, 4e6); }

void test_mem_latency_1m(int times) { general_latency_test(1 << 20, times, 4e6); }
void test_mem_latency_2m(int times) { general_latency_test(2 << 20, times, 4e6); }
void test_mem_latency_4m(int times) { general_latency_test(4 << 20, times, 4e6); }
void test_mem_latency_8m(int times) { general_latency_test(8 << 20, times, 2e6); }
void test_mem_latency_16m(int times) { general_latency_test(16 << 20, times, 2e6); }
void test_mem_latency_32m(int times) { general_latency_test(32 << 20, times, 1e6); }
void test_mem_latency_64m(int times) { general_latency_test(64 << 20, times, 1e6); }
void test_mem_latency_128m(int times) { general_latency_test(128 << 20, times, 1e6); }
