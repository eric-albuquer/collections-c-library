#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "collections.h"

typedef struct Point {
    int x;
    int y;
} Point;


void test_objpool_basic(void) {
    size_t capacity = 5;
    ObjectPool pool = objpool_create(capacity, sizeof(Point));
    assert(pool._capacity == capacity);
    assert(pool._obj_size >= sizeof(Point));
    assert(pool._used == 0);
    assert(objpool_available(&pool) == capacity);

    Point* p1 = (Point*)objpool_malloc(&pool);
    Point* p2 = (Point*)objpool_malloc(&pool);
    Point* p3 = (Point*)objpool_malloc(&pool);

    assert(p1 != NULL);
    assert(p2 != NULL);
    assert(p3 != NULL);
    assert(pool._used == 3);
    assert(objpool_available(&pool) == capacity - 3);

    p1->x = 10; p1->y = 20;
    p2->x = 30; p2->y = 40;
    p3->x = 50; p3->y = 60;

    assert(p1->x == 10 && p1->y == 20);
    assert(p2->x == 30 && p2->y == 40);
    assert(p3->x == 50 && p3->y == 60);

    objpool_free_obj(&pool, p2);
    assert(pool._used == 2);
    assert(objpool_available(&pool) == capacity - 2);

    objpool_free(&pool);
}

void test_objpool_reuse(void) {
    ObjectPool pool = objpool_create(3, sizeof(int));

    int* a = (int*)objpool_malloc(&pool);
    int* b = (int*)objpool_malloc(&pool);
    int* c = (int*)objpool_malloc(&pool);

    *a = 100;
    *b = 200;
    *c = 300;

    objpool_free_obj(&pool, b);
    assert(pool._used == 2);

    int* d = (int*)objpool_malloc(&pool);
    assert(d == b);
    *d = 999;

    assert(*a == 100);
    assert(*c == 300);
    assert(*d == 999);
    assert(pool._used == 3);
    assert(objpool_available(&pool) == 0);

    objpool_free(&pool);
}

void test_objpool_capacity_limits(void) {
    ObjectPool pool = objpool_create(2, sizeof(int));

    int* p1 = (int*)objpool_malloc(&pool);
    int* p2 = (int*)objpool_malloc(&pool);
    assert(p1 != NULL);
    assert(p2 != NULL);
    assert(pool._used == 2);

    int* p_overflow = (int*)objpool_malloc(&pool);
    assert(p_overflow == NULL);

    objpool_free_obj(&pool, p1);
    assert(pool._used == 1);

    int* p3 = (int*)objpool_malloc(&pool);
    assert(p3 != NULL);
    assert(p3 == p1);
    assert(pool._used == 2);

    objpool_free(&pool);
}

void test_objpool_stress_and_performance(void) {

    size_t capacity = 50000;
    ObjectPool pool = objpool_create(capacity, sizeof(int));

    clock_t start = clock();

    int** ptrs = (int**)malloc(capacity * sizeof(int*));
    assert(ptrs != NULL);
    for (size_t i = 0; i < capacity; i++) {
        ptrs[i] = (int*)objpool_malloc(&pool);
        assert(ptrs[i] != NULL);
        *ptrs[i] = (int)i;
    }
    assert(pool._used == capacity);

    for (size_t i = 0; i < capacity; i++) {
        objpool_free_obj(&pool, ptrs[i]);
    }
    assert(pool._used == 0);

    for (size_t i = 0; i < capacity; i++) {
        ptrs[i] = (int*)objpool_malloc(&pool);
        assert(ptrs[i] != NULL);
    }
    assert(pool._used == capacity);

    free(ptrs);
    objpool_free(&pool);

    clock_t end = clock();
    double total_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("ObjectPool benchmark: %zu allocation/free operations in %.6f seconds.\n",
           capacity * 2, total_time);

}

int main(void) {

    test_objpool_basic();
    test_objpool_reuse();
    test_objpool_capacity_limits();
    test_objpool_stress_and_performance();

    puts("All ObjPool tests passed.");
    return 0;
}