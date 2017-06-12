void purge() {
  if (system("sudo purge")) {
      puts(
          "`sudo purge` fails. Please add the following line to your visudo\n"
          "\t%admin\t\tALL = (root) NOPASSWD: /usr/sbin/purge"
      );
      exit(1);
  }
}

void init_local_tmpfile(uint64_t size) {
    init_tmpfile("/tmp", size);
    check_error(fcntl(fs_tmpfile, F_NOCACHE, 1));
    purge();
}

void fs_local_seq(uint64_t size) {
    if (!fs_tmpfile) {
        init_local_tmpfile(size);
    }
    size /= BLOCK_SIZE;
    BEGIN_TEST_ONCE
        for (uint64_t i = 0; i < size; i++) {
            assert(read(fs_tmpfile, buffer, BLOCK_SIZE) == BLOCK_SIZE);
        }
    END_TEST_ONCE
    global_count = size;
}

void test_fs_local_seq_16m() { fs_local_seq(16ull << 20); }
void test_fs_local_seq_32m() { fs_local_seq(32ull << 20); }
void test_fs_local_seq_64m() { fs_local_seq(64ull << 20); }
void test_fs_local_seq_128m() { fs_local_seq(128ull << 20); }
void test_fs_local_seq_256m() { fs_local_seq(256ull << 20); }
void test_fs_local_seq_512m() { fs_local_seq(512ull << 20); }
void test_fs_local_seq_1g() { fs_local_seq(1ull << 30); }
void test_fs_local_seq_2g() { fs_local_seq(2ull << 30); }
void test_fs_local_seq_4g() { fs_local_seq(4ull << 30); }
void test_fs_local_seq_8g() { fs_local_seq(8ull << 30); }
void test_fs_local_seq_16g() { fs_local_seq(16ull << 30); }


void fs_local_rand(uint64_t size) {
    if (!fs_tmpfile) {
        init_local_tmpfile(size);
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

void test_fs_local_rand_16m() { fs_local_rand(16ull << 20); }
void test_fs_local_rand_32m() { fs_local_rand(32ull << 20); }
void test_fs_local_rand_64m() { fs_local_rand(64ull << 20); }
void test_fs_local_rand_128m() { fs_local_rand(128ull << 20); }
void test_fs_local_rand_256m() { fs_local_rand(256ull << 20); }
void test_fs_local_rand_512m() { fs_local_rand(512ull << 20); }
void test_fs_local_rand_1g() { fs_local_rand(1ull << 30); }
void test_fs_local_rand_2g() { fs_local_rand(2ull << 30); }
void test_fs_local_rand_4g() { fs_local_rand(4ull << 30); }
void test_fs_local_rand_8g() { fs_local_rand(8ull << 30); }
void test_fs_local_rand_16g() { fs_local_rand(16ull << 30); }
