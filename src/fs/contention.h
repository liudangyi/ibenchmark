#define CONTENTION_FILE_SIZE (1 << 20)

void *fs_contention_thread(void *arg) {
    int fd = init_tmpfile("/tmp", CONTENTION_FILE_SIZE);
    check_error(fcntl(fd, F_NOCACHE, 1));
    while (check_error(read(fd, buffer, BLOCK_SIZE))) {}
    return NULL;
}

void fs_contention(int threads) {
    BEGIN_TEST_ONCE
        pthread_t tid[threads];
        for (int i = 0; i < threads; i++) {
            pthread_create(&tid[i], NULL, fs_contention_thread, NULL);
        }
        for (int i = 0; i < threads; i++) {
            pthread_join(tid[i], NULL);
        }
    END_TEST_ONCE
    global_count = CONTENTION_FILE_SIZE / BLOCK_SIZE;
}

void test_fs_contention_1() { fs_contention(1); }
void test_fs_contention_2() { fs_contention(2); }
void test_fs_contention_3() { fs_contention(3); }
void test_fs_contention_4() { fs_contention(4); }
void test_fs_contention_5() { fs_contention(5); }
void test_fs_contention_6() { fs_contention(6); }
void test_fs_contention_7() { fs_contention(7); }
void test_fs_contention_8() { fs_contention(8); }
void test_fs_contention_9() { fs_contention(9); }
void test_fs_contention_10() { fs_contention(10); }
void test_fs_contention_11() { fs_contention(11); }
void test_fs_contention_12() { fs_contention(12); }
void test_fs_contention_13() { fs_contention(13); }
void test_fs_contention_14() { fs_contention(14); }
void test_fs_contention_15() { fs_contention(15); }
void test_fs_contention_16() { fs_contention(16); }
void test_fs_contention_17() { fs_contention(17); }
void test_fs_contention_18() { fs_contention(18); }
void test_fs_contention_19() { fs_contention(19); }
void test_fs_contention_20() { fs_contention(20); }
