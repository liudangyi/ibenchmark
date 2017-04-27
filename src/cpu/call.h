// Procedure call overhead

__attribute__((noinline))
void func0(void) { asm (""); }
__attribute__((noinline))
void func(int x, ...) { asm (""); }

BEGIN_TEST(call0, 1e7)
    func0();
END_TEST

BEGIN_TEST(call1, 1e7)
    func(0);
END_TEST

BEGIN_TEST(call2, 1e7)
    func(0, 1);
END_TEST

BEGIN_TEST(call3, 1e7)
    func(0, 1, 2);
END_TEST

BEGIN_TEST(call4, 1e7)
    func(0, 1, 2, 3);
END_TEST

BEGIN_TEST(call5, 1e7)
    func(0, 1, 2, 3, 4);
END_TEST

BEGIN_TEST(call6, 1e7)
    func(0, 1, 2, 3, 4, 5);
END_TEST

BEGIN_TEST(call7, 1e7)
    func(0, 1, 2, 3, 4, 5, 6);
END_TEST
