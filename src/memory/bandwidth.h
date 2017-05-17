BEGIN_TEST_PREP(mem_bandwidth_w)
    const size_t size = 64 << 20;
    volatile uint64_t *array = malloc(size);
    const size_t length = size / sizeof(uint64_t);
BEGIN_TEST_LOOP(1e8)
    array[i % length] = i;
END_TEST_LOOP
    global_count *= sizeof(uint64_t);
END_TEST_PREP


BEGIN_TEST_PREP(mem_bandwidth_r)
    const size_t size = 64 << 20;
    volatile uint64_t *array = malloc(size);
    const size_t length = size / sizeof(uint64_t);
    int xor = 0;
BEGIN_TEST_LOOP(1e8)
    xor ^= array[i % length];
END_TEST_LOOP
    global_count *= sizeof(uint64_t);
END_TEST_PREP


BEGIN_TEST_PREP(mem_bandwidth_rw)
    const size_t size = 64 << 20;
    volatile uint64_t *array1 = malloc(size);
    volatile uint64_t *array2 = malloc(size);
    const size_t length = size / sizeof(uint64_t);
BEGIN_TEST_LOOP(1e8)
    array1[i % length] = array2[i % length];
END_TEST_LOOP
    global_count *= sizeof(uint64_t);
END_TEST_PREP


BEGIN_TEST_PREP(mem_bandwidth_memset)
    const size_t size = 64 << 20;
    volatile uint64_t *array = malloc(size);
BEGIN_TEST_LOOP(8)
    memset(array, 0x80, size);
END_TEST_LOOP
    global_count *= size;
    // in prevent the memset is optimized away
    array[0] = 0;
END_TEST_PREP


BEGIN_TEST_PREP(mem_bandwidth_memcpy)
    const size_t size = 64 << 20;
    volatile uint64_t *array1 = malloc(size);
    volatile uint64_t *array2 = malloc(size);
BEGIN_TEST_LOOP(8)
    memcpy(array1, array2, size);
END_TEST_LOOP
    array1[0] = array2[0] = 0;
    global_count *= size;
END_TEST_PREP
