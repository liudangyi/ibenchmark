void init_tmpfile_with_cache(uint64_t size) {
    init_tmpfile(size);
    check_error(fcntl(fs_tmpfile, F_NOCACHE, 0));
}

void fs_cache_seq(uint64_t size) {
    if (!fs_tmpfile) {
        init_tmpfile_with_cache(size);
    }
    size /= BLOCK_SIZE;
    BEGIN_TEST_ONCE
        for (uint64_t i = 0; i < size; i++) {
            assert(read(fs_tmpfile, buffer, BLOCK_SIZE) == BLOCK_SIZE);
        }
    END_TEST_ONCE
    global_count = size;
}

void test_fs_cache_seq_64m() { fs_cache_seq(64ull << 20); }
void test_fs_cache_seq_128m() { fs_cache_seq(128ull << 20); }
void test_fs_cache_seq_256m() { fs_cache_seq(256ull << 20); }
void test_fs_cache_seq_512m() { fs_cache_seq(512ull << 20); }
void test_fs_cache_seq_1g() { fs_cache_seq(1ull << 30); }
void test_fs_cache_seq_2g() { fs_cache_seq(2ull << 30); }
void test_fs_cache_seq_4g() { fs_cache_seq(4ull << 30); }
void test_fs_cache_seq_8g() { fs_cache_seq(8ull << 30); }
void test_fs_cache_seq_16g() { fs_cache_seq(16ull << 30); }


void fs_cache_rand(uint64_t size) {
    if (!fs_tmpfile) {
        init_tmpfile_with_cache(size);
    }
    size /= BLOCK_SIZE;
    BEGIN_TEST_ONCE
        for (uint64_t i = 0; i < size; i++) {
            int offset = ((size >> 1) * (i & 1) + (i >> 1)) * BLOCK_SIZE;
            assert(lseek(fs_tmpfile, offset, SEEK_SET) == offset);
            assert(read(fs_tmpfile, buffer, BLOCK_SIZE) == BLOCK_SIZE);
        }
    END_TEST_ONCE
    global_count = size;
}

void test_fs_cache_rand_64m() { fs_cache_rand(64ull << 20); }
void test_fs_cache_rand_128m() { fs_cache_rand(128ull << 20); }
void test_fs_cache_rand_256m() { fs_cache_rand(256ull << 20); }
void test_fs_cache_rand_512m() { fs_cache_rand(512ull << 20); }
void test_fs_cache_rand_1g() { fs_cache_rand(1ull << 30); }
void test_fs_cache_rand_2g() { fs_cache_rand(2ull << 30); }
void test_fs_cache_rand_4g() { fs_cache_rand(4ull << 30); }
void test_fs_cache_rand_8g() { fs_cache_rand(8ull << 30); }
void test_fs_cache_rand_16g() { fs_cache_rand(16ull << 30); }
