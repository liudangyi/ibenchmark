#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

BEGIN_TEST_PREP(mem_pagefault)
    // a big file
    int fd = open("/var/db/dyld/dyld_shared_cache_x86_64h", O_RDONLY);
    size_t size = 256 << 20; // 256 MB
    volatile uint8_t *data = mmap(0, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (errno) {
      perror("mmap");
      exit(-1);
    }
BEGIN_TEST_LOOP((size >> 12) / 2) // page size, divided by 2 since we will time times here
    data[i << 12];
END_TEST_LOOP
    close(fd);
END_TEST_PREP
