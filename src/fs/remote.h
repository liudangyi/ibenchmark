void init_remote_tmpfile(uint64_t size) {
    init_tmpfile("/Volumes/Macintosh HD-1/tmp", size);
    check_error(fcntl(fs_tmpfile, F_NOCACHE, 1));
}

void fs_remote_seq(uint64_t size) {
    if (!fs_tmpfile) {
        init_remote_tmpfile(size);
    }
    size /= BLOCK_SIZE;
    BEGIN_TEST_ONCE
        for (uint64_t i = 0; i < size; i++) {
            assert(read(fs_tmpfile, buffer, BLOCK_SIZE) == BLOCK_SIZE);
        }
    END_TEST_ONCE
    global_count = size;
}

void test_fs_remote_seq_16k() { fs_remote_seq(16ull << 10); }
void test_fs_remote_seq_32k() { fs_remote_seq(32ull << 10); }
void test_fs_remote_seq_64k() { fs_remote_seq(64ull << 10); }
void test_fs_remote_seq_128k() { fs_remote_seq(128ull << 10); }
void test_fs_remote_seq_256k() { fs_remote_seq(256ull << 10); }
void test_fs_remote_seq_512k() { fs_remote_seq(512ull << 10); }
void test_fs_remote_seq_1m() { fs_remote_seq(1ull << 20); }
void test_fs_remote_seq_2m() { fs_remote_seq(2ull << 20); }
void test_fs_remote_seq_4m() { fs_remote_seq(4ull << 20); }
void test_fs_remote_seq_8m() { fs_remote_seq(8ull << 20); }
void test_fs_remote_seq_16m() { fs_remote_seq(16ull << 20); }
void test_fs_remote_seq_32m() { fs_remote_seq(32ull << 20); }
void test_fs_remote_seq_64m() { fs_remote_seq(64ull << 20); }
void test_fs_remote_seq_128m() { fs_remote_seq(128ull << 20); }
void test_fs_remote_seq_256m() { fs_remote_seq(256ull << 20); }
void test_fs_remote_seq_512m() { fs_remote_seq(512ull << 20); }
void test_fs_remote_seq_1g() { fs_remote_seq(1ull << 30); }
void test_fs_remote_seq_2g() { fs_remote_seq(2ull << 30); }
void test_fs_remote_seq_4g() { fs_remote_seq(4ull << 30); }
void test_fs_remote_seq_8g() { fs_remote_seq(8ull << 30); }
void test_fs_remote_seq_16g() { fs_remote_seq(16ull << 30); }


void fs_remote_rand(uint64_t size) {
    if (!fs_tmpfile) {
        init_remote_tmpfile(size);
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

void test_fs_remote_rand_16k() { fs_remote_rand(16ull << 10); }
void test_fs_remote_rand_32k() { fs_remote_rand(32ull << 10); }
void test_fs_remote_rand_64k() { fs_remote_rand(64ull << 10); }
void test_fs_remote_rand_128k() { fs_remote_rand(128ull << 10); }
void test_fs_remote_rand_256k() { fs_remote_rand(256ull << 10); }
void test_fs_remote_rand_512k() { fs_remote_rand(512ull << 10); }
void test_fs_remote_rand_1m() { fs_remote_rand(1ull << 20); }
void test_fs_remote_rand_2m() { fs_remote_rand(2ull << 20); }
void test_fs_remote_rand_4m() { fs_remote_rand(4ull << 20); }
void test_fs_remote_rand_8m() { fs_remote_rand(8ull << 20); }
void test_fs_remote_rand_16m() { fs_remote_rand(16ull << 20); }
void test_fs_remote_rand_32m() { fs_remote_rand(32ull << 20); }
void test_fs_remote_rand_64m() { fs_remote_rand(64ull << 20); }
void test_fs_remote_rand_128m() { fs_remote_rand(128ull << 20); }
void test_fs_remote_rand_256m() { fs_remote_rand(256ull << 20); }
void test_fs_remote_rand_512m() { fs_remote_rand(512ull << 20); }
void test_fs_remote_rand_1g() { fs_remote_rand(1ull << 30); }
void test_fs_remote_rand_2g() { fs_remote_rand(2ull << 30); }
void test_fs_remote_rand_4g() { fs_remote_rand(4ull << 30); }
void test_fs_remote_rand_8g() { fs_remote_rand(8ull << 30); }
void test_fs_remote_rand_16g() { fs_remote_rand(16ull << 30); }
