#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "collections.h"


int min_int_cmp(const void* a, const void* b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    return arg1 - arg2;
}

int max_int_cmp(const void* a, const void* b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    return arg2 - arg1;
}

typedef struct {
    int priority;
    char name[20];
} Task;

static int heap_foreach_sum;
static int heap_foreach_count;

static void accumulate_heap_element(int index, const void* element) {
    (void)index;
    heap_foreach_sum += *(const int*)element;
    heap_foreach_count++;
}

int task_compare(const void* a, const void* b) {
    const Task* t1 = (const Task*)a;
    const Task* t2 = (const Task*)b;
    return t2->priority - t1->priority;
}


void test_heap_min_int(void) {
    Heap heap = heap_create(sizeof(int), min_int_cmp);
    assert(heap._size == 0);

    int values[] = {45, 12, 89, 3, 27, 1};
    int n = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < n; i++) {
        heap_push(&heap, &values[i]);
    }

    assert(heap._size == n);

    int expected_order[] = {1, 3, 12, 27, 45, 89};
    int out;

    for (int i = 0; i < n; i++) {
        assert(heap_pop(&heap, &out) == 1);
        assert(out == expected_order[i]);
    }

    assert(heap._size == 0);

    assert(heap_pop(&heap, &out) == 0);

    heap_free(&heap);
}

void test_heap_max_int(void) {
    Heap heap = heap_create(sizeof(int), max_int_cmp);

    int values[] = {10, 50, 20, 40, 30};
    int n = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < n; i++) {
        heap_push(&heap, &values[i]);
    }

    int expected_order[] = {50, 40, 30, 20, 10};
    int out;

    for (int i = 0; i < n; i++) {
        assert(heap_pop(&heap, &out) == 1);
        assert(out == expected_order[i]);
    }

    heap_free(&heap);
}

void test_heap_stress(void) {
    Heap heap = heap_create(sizeof(int), min_int_cmp);
    int total = 1000;

    for (int i = total; i > 0; i--) {
        heap_push(&heap, &i);
    }

    assert(heap._size == total);

    for (int i = 1; i <= total; i++) {
        int out;
        assert(heap_pop(&heap, &out) == 1);
        assert(out == i);
    }

    assert(heap._size == 0);

    heap_free(&heap);
}

void test_heap_structs(void) {
    Heap heap = heap_create(sizeof(Task), task_compare);

    Task t1 = {2, "Presentation"};
    Task t2 = {5, "Critical Bug"};
    Task t3 = {1, "Email"};
    Task t4 = {5, "Another Bug"};

    heap_push(&heap, &t1);
    heap_push(&heap, &t2);
    heap_push(&heap, &t3);
    heap_push(&heap, &t4);

    Task out;

    assert(heap_pop(&heap, &out) == 1);
    assert(out.priority == 5);

    assert(heap_pop(&heap, &out) == 1);
    assert(out.priority == 5);

    assert(heap_pop(&heap, &out) == 1);
    assert(out.priority == 2);
    assert(strcmp(out.name, "Presentation") == 0);

    assert(heap_pop(&heap, &out) == 1);
    assert(out.priority == 1);

    heap_free(&heap);
}

void test_heap_length_copy_and_foreach(void) {
    Heap original = heap_create(sizeof(int), min_int_cmp);
    int values[] = {9, 2, 7, 4};
    for (int i = 0; i < 4; i++) {
        heap_push(&original, &values[i]);
    }
    assert(heap_length(&original) == 4);

    heap_foreach_sum = 0;
    heap_foreach_count = 0;
    heap_foreach(&original, accumulate_heap_element);
    assert(heap_foreach_count == 4);
    assert(heap_foreach_sum == 22);

    Heap copy = heap_copy(&original);
    assert(heap_length(&copy) == 4);

    int out = 0;
    assert(heap_pop(&copy, &out));
    assert(out == 2);
    assert(heap_length(&copy) == 3);
    assert(heap_length(&original) == 4);

    heap_free(&original);
    heap_free(&copy);
}

int main(void) {

    test_heap_min_int();
    test_heap_max_int();
    test_heap_stress();
    test_heap_structs();
    test_heap_length_copy_and_foreach();

    puts("All tests passed.");
    return 0;
}