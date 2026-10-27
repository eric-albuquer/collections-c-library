#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "collections.h"

void test_indexedpq_create_and_free() {
    IndexedPriorityQueue ipq = indexedpq_create(sizeof(int), 5);

    assert(indexedpq_length(&ipq) == 0);

    int out_val = 0;
    size_t out_prio = SIZE_MAX;
    assert(indexedpq_dequeue(&ipq, &out_val, &out_prio) == false);

    indexedpq_free(&ipq);
}

void test_indexedpq_order_by_priority() {
    IndexedPriorityQueue ipq = indexedpq_create(sizeof(int), 10);

    int v1 = 100, v2 = 200, v3 = 300, v4 = 400;

    assert(indexedpq_enqueue(&ipq, 3, &v1) == true);
    assert(indexedpq_enqueue(&ipq, 0, &v2) == true);
    assert(indexedpq_enqueue(&ipq, 8, &v3) == true);
    assert(indexedpq_enqueue(&ipq, 1, &v4) == true);

    assert(indexedpq_length(&ipq) == 4);

    int out_val = 0;
    size_t out_prio = SIZE_MAX;

    assert(indexedpq_dequeue(&ipq, &out_val, &out_prio) == true);
    assert(out_prio == 0 && out_val == 200);

    assert(indexedpq_dequeue(&ipq, &out_val, &out_prio) == true);
    assert(out_prio == 1 && out_val == 400);

    assert(indexedpq_dequeue(&ipq, &out_val, &out_prio) == true);
    assert(out_prio == 3 && out_val == 100);

    assert(indexedpq_dequeue(&ipq, &out_val, &out_prio) == true);
    assert(out_prio == 8 && out_val == 300);

    assert(indexedpq_length(&ipq) == 0);
    assert(indexedpq_dequeue(&ipq, &out_val, &out_prio) == false);

    indexedpq_free(&ipq);
}

void test_indexedpq_fifo_same_priority() {
    IndexedPriorityQueue ipq = indexedpq_create(sizeof(int), 5);

    int first = 10, second = 20, third = 30;

    indexedpq_enqueue(&ipq, 2, &first);
    indexedpq_enqueue(&ipq, 2, &second);
    indexedpq_enqueue(&ipq, 2, &third);

    assert(indexedpq_length(&ipq) == 3);

    int out_val = 0;
    size_t out_prio = SIZE_MAX;

    assert(indexedpq_dequeue(&ipq, &out_val, &out_prio) == true);
    assert(out_prio == 2 && out_val == 10);

    assert(indexedpq_dequeue(&ipq, &out_val, &out_prio) == true);
    assert(out_prio == 2 && out_val == 20);

    assert(indexedpq_dequeue(&ipq, &out_val, &out_prio) == true);
    assert(out_prio == 2 && out_val == 30);

    indexedpq_free(&ipq);
}

void test_indexedpq_bounds_and_null_out() {
    IndexedPriorityQueue ipq = indexedpq_create(sizeof(int), 4);

    int val = 42;

    assert(indexedpq_enqueue(&ipq, SIZE_MAX, &val) == false);
    assert(indexedpq_enqueue(&ipq, 4, &val) == false);
    assert(indexedpq_length(&ipq) == 0);

    indexedpq_enqueue(&ipq, 1, &val);
    assert(indexedpq_dequeue(&ipq, NULL, NULL) == true);
    assert(indexedpq_length(&ipq) == 0);

    indexedpq_free(&ipq);
}

void test_indexedpq_performance() {
    const size_t num_operations = 1000000;
    const size_t priority_count = 64;

    IndexedPriorityQueue ipq = indexedpq_create(sizeof(int), priority_count);

    srand((unsigned int)time(NULL));

    clock_t start_time = clock();

    for (size_t i = 0; i < num_operations; i++) {
        size_t priority = (size_t)rand() % priority_count;
        int value = (int)i;
        assert(indexedpq_enqueue(&ipq, priority, &value) == true);
    }

    assert(indexedpq_length(&ipq) == num_operations);

    int out_val;
    size_t out_prio;
    size_t last_prio = 0;

    for (size_t i = 0; i < num_operations; i++) {
        assert(indexedpq_dequeue(&ipq, &out_val, &out_prio) == true);
        
        assert(out_prio >= last_prio);
        last_prio = out_prio;
    }

    assert(indexedpq_length(&ipq) == 0);

    clock_t end_time = clock();
    double total_time_ms = ((double)(end_time - start_time) / CLOCKS_PER_SEC) * 1000.0;

    printf("IndexedPriorityQueue %zu insertions + %zu dequeues completed in %.2f ms (%.2f ops/ms)\n",
           num_operations, num_operations, total_time_ms,
           (double)(num_operations * 2) / total_time_ms);

    indexedpq_free(&ipq);
}

int main() {
    test_indexedpq_create_and_free();
    test_indexedpq_order_by_priority();
    test_indexedpq_fifo_same_priority();
    test_indexedpq_bounds_and_null_out();
    test_indexedpq_performance();

    puts("All IndexedPriorityQueue tests passed.");
    return 0;
}