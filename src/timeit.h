uint64_t timeit_start_at;

void timeit_start() {
    uint32_t cycles_low0, cycles_high0;

    asm volatile ("cpuid\n\t"
                  "rdtsc\n\t"
                  "mov %%edx, %0\n\t"
                  "mov %%eax, %1\n\t"
                  : "=r" (cycles_high0), "=r" (cycles_low0)
                  :: "%rax", "%rbx", "%rcx", "%rdx");

    timeit_start_at = ((uint64_t) cycles_high0 << 32) + cycles_low0;
}

void timeit_end() {
    uint32_t cycles_low1, cycles_high1;

    asm volatile ("rdtscp\n\t"
                  "mov %%edx, %0\n\t"
                  "mov %%eax, %1\n\t"
                  "cpuid\n\t"
                  : "=r" (cycles_high1), "=r" (cycles_low1)
                  :: "%rax", "%rbx", "%rcx", "%rdx");

    timeit_start_at = (((uint64_t) cycles_high1 << 32) + cycles_low1) - timeit_start_at;
}
