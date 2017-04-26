#include <pthread.h>

BEGIN_TEST(create_process, 200)
    if (fork() == 0) {
        exit(0);
    }
END_TEST

void *thread_start(void *args) {
    return NULL;
}

BEGIN_TEST(create_thread, 200)
    pthread_t k;
    void *args;
    pthread_create(&k, NULL, &thread_start, args);
END_TEST
