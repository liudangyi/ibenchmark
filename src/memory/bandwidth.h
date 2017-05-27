BEGIN_TEST_PREP(mem_bandwidth_w)
    const size_t size = 64 << 20;
    volatile uint64_t *array = malloc(size);
    const size_t length = size / sizeof(uint64_t);
BEGIN_TEST_LOOP(length * 32)
    array[i % length] = i;
END_TEST_LOOP
    global_count *= sizeof(uint64_t);
END_TEST_PREP


BEGIN_TEST_PREP(mem_bandwidth_r)
    const size_t size = 64 << 20;
    volatile uint64_t *array = malloc(size);
    const size_t length = size / sizeof(uint64_t);
    int xor = 0;
BEGIN_TEST_LOOP(length * 32)
    xor ^= array[i % length];
END_TEST_LOOP
    global_count *= sizeof(uint64_t);
END_TEST_PREP


BEGIN_TEST_PREP(mem_bandwidth_rw)
    const size_t size = 64 << 20;
    volatile uint64_t *array1 = malloc(size);
    volatile uint64_t *array2 = malloc(size);
    const size_t length = size / sizeof(uint64_t);
BEGIN_TEST_LOOP(length * 32)
    array1[i % length] = array2[i % length];
END_TEST_LOOP
    global_count *= sizeof(uint64_t);
END_TEST_PREP


BEGIN_TEST_PREP(mem_bandwidth_memset)
    const size_t size = 64 << 20;
    volatile uint64_t *array = malloc(size);
BEGIN_TEST_LOOP(32)
    memset(array, 0x88, size);
END_TEST_LOOP
    global_count *= size;
    array[0] = 0;
END_TEST_PREP


BEGIN_TEST_PREP(mem_bandwidth_memcpy)
    const size_t size = 64 << 20;
    uint64_t *array1 = malloc(size);
    uint64_t *array2 = malloc(size);
BEGIN_TEST_LOOP(32)
    bcopy(array1, array2, size);
END_TEST_LOOP
    global_count *= size;
END_TEST_PREP
