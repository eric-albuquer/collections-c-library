#include <stdio.h>
#include <assert.h>
#include "collections.h"

static int foreach_sum;
static int foreach_count;
static bool foreach_indices_valid;

static void accumulate_deque_element(int index, void* element) {
    if (index != foreach_count) {
        foreach_indices_valid = false;
    }
    foreach_sum += *(int*)element;
    foreach_count++;
}

void test_arraydeque_basic(void) {
    ArrayDeque deque = arraydeque_create(sizeof(int));
    assert(deque._size == 0);

    int val1 = 10, val2 = 20, val3 = 30;
    arraydeque_append(&deque, &val1);
    arraydeque_append(&deque, &val2);
    arraydeque_append(&deque, &val3);

    assert(deque._size == 3);

    int out = 0;
    assert(arraydeque_popleft(&deque, &out) == 1);
    assert(out == 10);

    assert(arraydeque_popleft(&deque, &out) == 1);
    assert(out == 20);

    assert(arraydeque_popleft(&deque, &out) == 1);
    assert(out == 30);

    assert(deque._size == 0);

    assert(arraydeque_popleft(&deque, &out) == 0);
    assert(arraydeque_pop(&deque, &out) == 0);

    arraydeque_free(&deque);
}

void test_arraydeque_stack_and_deque_ops(void) {
    ArrayDeque deque = arraydeque_create(sizeof(int));

    int v1 = 100, v2 = 200, v3 = 300;

    arraydeque_appendleft(&deque, &v1);
    arraydeque_appendleft(&deque, &v2);
    arraydeque_appendleft(&deque, &v3);

    assert(deque._size == 3);

    int out;
    assert(arraydeque_pop(&deque, &out) == 1);
    assert(out == 100);

    assert(arraydeque_popleft(&deque, &out) == 1);
    assert(out == 300);

    assert(deque._size == 1);

    arraydeque_free(&deque);
}

void test_arraydeque_stress_and_resize(void) {
    ArrayDeque deque = arraydeque_create(sizeof(int));
    int total_elements = 1000;

    for (int i = 0; i < total_elements; i++) {
        arraydeque_append(&deque, &i);
    }

    assert(deque._size == total_elements);

    for (int i = 0; i < total_elements; i++) {
        int out = -1;
        int status = arraydeque_popleft(&deque, &out);
        assert(status == 1);
        assert(out == i);
    }

    assert(deque._size == 0);

    arraydeque_free(&deque);
}

void test_arraydeque_generic_types(void) {
    typedef struct {
        int x;
        int y;
    } Point;

    ArrayDeque deque = arraydeque_create(sizeof(Point));

    Point p1 = {10, 20};
    Point p2 = {30, 40};

    arraydeque_append(&deque, &p1);
    arraydeque_append(&deque, &p2);

    Point out;
    assert(arraydeque_popleft(&deque, &out) == 1);
    assert(out.x == 10 && out.y == 20);

    assert(arraydeque_popleft(&deque, &out) == 1);
    assert(out.x == 30 && out.y == 40);

    arraydeque_free(&deque);
}

void test_arraydeque_copy_and_foreach(void) {
    ArrayDeque deque = arraydeque_create(sizeof(int));
    int out = -1;

    for (int value = 0; value < 64; value++) {
        arraydeque_append(&deque, &value);
    }
    for (int expected = 0; expected < 32; expected++) {
        assert(arraydeque_popleft(&deque, &out));
        assert(out == expected);
    }
    for (int value = 64; value < 96; value++) {
        arraydeque_append(&deque, &value);
    }

    ArrayDeque copy = arraydeque_copy(&deque);
    assert(arraydeque_length(&copy) == 64);

    foreach_sum = 0;
    foreach_count = 0;
    foreach_indices_valid = true;
    arraydeque_foreach(&copy, accumulate_deque_element);
    assert(foreach_count == 64);
    assert(foreach_sum == 4064);
    assert(foreach_indices_valid);

    assert(arraydeque_popleft(&copy, &out));
    assert(out == 32);
    assert(arraydeque_length(&deque) == 64);
    assert(arraydeque_length(&copy) == 63);

    arraydeque_free(&deque);
    arraydeque_free(&copy);
}

void test_arraydeque_wraparound_resize(void) {
    ArrayDeque deque = arraydeque_create(sizeof(int));
    int out = -1;

    for (int value = 0; value < 64; value++) {
        arraydeque_append(&deque, &value);
    }
    for (int expected = 0; expected < 32; expected++) {
        assert(arraydeque_popleft(&deque, &out));
        assert(out == expected);
    }
    for (int value = 64; value < 128; value++) {
        arraydeque_append(&deque, &value);
    }

    assert(arraydeque_length(&deque) == 96);
    for (int expected = 32; expected < 128; expected++) {
        assert(arraydeque_popleft(&deque, &out));
        assert(out == expected);
    }
    assert(arraydeque_length(&deque) == 0);
    assert(!arraydeque_pop(&deque, &out));

    arraydeque_free(&deque);
}

int main(void) {

    test_arraydeque_basic();
    test_arraydeque_stack_and_deque_ops();
    test_arraydeque_stress_and_resize();
    test_arraydeque_generic_types();
    test_arraydeque_copy_and_foreach();
    test_arraydeque_wraparound_resize();

    puts("All ArrayDeque tests passed.");
    return 0;
}