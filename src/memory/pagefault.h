#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

BEGIN_TEST_PREP(mem_pagefault)
    size_t size = 256 << 20; // 256 MB
    uint8_t *data = mmap(0, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, 0, 0);
    if (errno) {
      fprintf(stderr, "mmap: %s\n", strerror(errno));
      exit(-1);
    }
BEGIN_TEST_LOOP((size >> 12) / 2) // page size, divided by 2 since we will time times here
    data[i << 12] = 0x80;
END_TEST_LOOP
END_TEST_PREP
