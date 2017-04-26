#include <pthread.h>

BEGIN_TEST(switch_process, 200)
    if (fork() == 0) {
        exit(0);
    }
    int k;
    wait(&k);
END_TEST

BEGIN_TEST(switch_thread, 200)
    pthread_t k;
    void *args;
    pthread_create(&k, NULL, &thread_start, args);
    void *y;
    pthread_join(k, &y);
END_TEST
