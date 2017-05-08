typedef struct linked_list {
    struct linked_list* next;
} linked_list;

BEGIN_TEST_PREP(latency_l1)
    linked_list *p = malloc(sizeof(linked_list));
    p->next = p;
BEGIN_TEST_LOOP(1e7)
    p = p->next; // movq (%rax), %rax
END_TEST_LOOP
    free(p);
END_TEST_PREP

/*
 * L1 cache is 32 KB with 8-way associativity
 * ....---------|---|---6---|---6---|
 *             tag    index   offset
 *
 * L2 cache is 256 KB with 4-way associativity
 * ....-----|--|-----10-----|---6---|
 *         tag      index     offset
 *
 * L3 cache is (up-to) 8 MB with 16-way associativity
 *
 *
 * sizeof(linked_list) => 8 bytes
 */
BEGIN_TEST_PREP(latency_l2)
    linked_list *list = calloc(sizeof(linked_list), 1 << 13); // 64 KB
    linked_list *p = list;
    for (int i = 0; i < 16; i++) {
        list[i << 9].next = &list[(i + 1) << 9];
    }
    list[(16 - 1) << 9].next = list;
BEGIN_TEST_LOOP(1e6)
    p = p->next;
END_TEST_LOOP
    p->next = p;
    free(list);
END_TEST_PREP


BEGIN_TEST_PREP(latency_l3)
    linked_list *list = calloc(sizeof(linked_list), 1 << 16); // 512 KB
    linked_list *p = list;
    for (int i = 0; i < 128; i++) {
        list[i << 9].next = &list[(i + 1) << 9];
    }
    list[(128 - 1) << 9].next = list;
BEGIN_TEST_LOOP(1e6)
    p = p->next;
END_TEST_LOOP
    p->next = p;
    free(list);
END_TEST_PREP

/*
 * L2 cache is 256 KB with 4-way associativity
 * ....--------|--|-----10-----|---6---|
 *            tag      index     offset
 *
 * L3 cache is (up-to) 8 MB with 16-way associativity
 * ....---|----|-------13------|---6---|
 *       tag         index       offset
 */
BEGIN_TEST_PREP(latency_mem)
    linked_list *list = malloc(sizeof(linked_list) << 21); // 16 MB
    linked_list *p = list;
    for (int i = 0; i < 2048; i++) {
        list[i << 10].next = &list[(i + 1) << 10];
    }
    list[(2048 - 1) << 10].next = list;
BEGIN_TEST_LOOP(1e6)
    p = p->next;
END_TEST_LOOP
    p->next = p;
    free(list);
END_TEST_PREP
