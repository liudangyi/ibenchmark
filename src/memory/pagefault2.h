#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

int pg_fault_fd;

BEGIN_TEST_PREP(mem_pagefault2)
    if (!pg_fault_fd) {
      // a big file
      pg_fault_fd = open("/var/db/dyld/dyld_shared_cache_x86_64h", O_RDONLY);
      purge();
    }
    size_t size = 256 << 20; // 256 MB
    volatile uint8_t *data = mmap(0, size, PROT_READ, MAP_PRIVATE, pg_fault_fd, 0);
    if (errno) {
        perror("mmap");
        exit(-1);
    }
BEGIN_TEST_ONCE
    for (int i = 0; i < (size >> 12); i++) {
        data[i << 12];
    }
END_TEST_ONCE
    global_count = (size >> 12);
END_TEST_PREP
