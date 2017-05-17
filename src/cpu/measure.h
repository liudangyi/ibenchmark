// Measurement overhead

BEGIN_TEST(nanosleep, 1)
    struct timespec ts = { 1, 0 };
    nanosleep(&ts, 0);
END_TEST

BEGIN_TEST(loop, 1e8)
    asm("");
END_TEST

BEGIN_TEST(rdtsc, 1e6)
    __rdtsc();
END_TEST
