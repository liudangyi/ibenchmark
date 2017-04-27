#include <pthread.h>
#include <assert.h>

int pipes[4];
char buf[100];

BEGIN_TEST_PREP(switch_process)
    int is_child;
    pipe(pipes);
    pipe(pipes+2);
    if (fork() == 0) {
        close(pipes[1]); // write
        close(pipes[2]); // read
        is_child = 1;
    } else {
        close(pipes[0]); // read
        close(pipes[3]); // write
        is_child = 0;
    }
BEGIN_TEST_LOOP(1e5)
    if (is_child) {
        assert(read(pipes[0], buf, 100));
        assert(write(pipes[3], "test", 4));
    } else {
        assert(write(pipes[1], "test", 4));
        assert(read(pipes[2], buf, 100));
    }
END_TEST_LOOP
    if (is_child) {
        close(pipes[0]);
        close(pipes[3]);
        exit(0);
    } else {
        close(pipes[1]);
        close(pipes[2]);
    }
END_TEST_PREP


int switch_thread_times;

void *child_thread(void *args) {
    for (int i = 0; i < switch_thread_times * global_count; i++) {
        assert(read(pipes[0], buf, 100));
        assert(write(pipes[3], "test", 4));
    }
    return NULL;
}

BEGIN_TEST_PREP(switch_thread)
    pthread_t k;
    void *args;

    switch_thread_times = times;
    global_count = 1e5;
    pipe(pipes);
    pipe(pipes+2);
    pthread_create(&k, NULL, &child_thread, args);

    for (int i = 0; i < switch_thread_times * global_count; i++) {
        assert(write(pipes[1], "test", 4));
        assert(read(pipes[2], buf, 100));
    }

    close(pipes[0]);
    close(pipes[1]);
    close(pipes[2]);
    close(pipes[3]);
END_TEST_PREP
