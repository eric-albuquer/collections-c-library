#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "collections.h"

static int g_foreach_sum = 0;
static int g_foreach_count = 0;

void accumulate_foreach_func(const void* element) {
    g_foreach_sum += *(const int*)element;
    g_foreach_count++;
}


void test_hashset_basic(void) {
    HashSet set = hashset_create(sizeof(int), int_equals, int_hash);
    assert(hashset_length(&set) == 0);

    int v1 = 10, v2 = 20, v3 = 30;

    hashset_add(&set, &v1);
    hashset_add(&set, &v2);
    hashset_add(&set, &v3);
    assert(hashset_length(&set) == 3);

    hashset_add(&set, &v1);
    assert(hashset_length(&set) == 3);

    assert(hashset_has(&set, &v1) == true);
    assert(hashset_has(&set, &v2) == true);

    int v_missing = 99;
    assert(hashset_has(&set, &v_missing) == false);

    assert(hashset_remove(&set, &v2) == true);
    assert(hashset_has(&set, &v2) == false);
    assert(hashset_length(&set) == 2);

    assert(hashset_remove(&set, &v_missing) == false);

    hashset_free(&set);
}

void test_hashset_copy(void) {
    HashSet set1 = hashset_create(sizeof(int), int_equals, int_hash);
    int a = 1, b = 2, c = 3;

    hashset_add(&set1, &a);
    hashset_add(&set1, &b);
    hashset_add(&set1, &c);

    HashSet set2 = hashset_copy(&set1);
    assert(hashset_length(&set2) == 3);
    assert(hashset_has(&set2, &a) == true);
    assert(hashset_has(&set2, &b) == true);
    assert(hashset_has(&set2, &c) == true);

    int d = 4;
    hashset_add(&set1, &d);
    assert(hashset_length(&set1) == 4);
    assert(hashset_length(&set2) == 3);
    assert(hashset_has(&set2, &d) == false);

    hashset_free(&set1);
    hashset_free(&set2);
}

void test_hashset_set_operations(void) {
    HashSet a = hashset_create(sizeof(int), int_equals, int_hash);
    HashSet b = hashset_create(sizeof(int), int_equals, int_hash);

    int v1 = 1, v2 = 2, v3 = 3;
    hashset_add(&a, &v1);
    hashset_add(&a, &v2);
    hashset_add(&a, &v3);

    int v4 = 4, v5 = 5;
    hashset_add(&b, &v3);
    hashset_add(&b, &v4);
    hashset_add(&b, &v5);

    HashSet u = hashset_union(&a, &b);
    assert(hashset_length(&u) == 5);
    assert(hashset_has(&u, &v1) == true);
    assert(hashset_has(&u, &v2) == true);
    assert(hashset_has(&u, &v3) == true);
    assert(hashset_has(&u, &v4) == true);
    assert(hashset_has(&u, &v5) == true);

    HashSet intersection = hashset_intersection(&a, &b);
    assert(hashset_length(&intersection) == 1);
    assert(hashset_has(&intersection, &v3) == true);
    assert(hashset_has(&intersection, &v1) == false);

    HashSet diff = hashset_difference(&a, &b);
    assert(hashset_length(&diff) == 2);
    assert(hashset_has(&diff, &v1) == true);
    assert(hashset_has(&diff, &v2) == true);
    assert(hashset_has(&diff, &v3) == false);

    hashset_free(&a);
    hashset_free(&b);
    hashset_free(&u);
    hashset_free(&intersection);
    hashset_free(&diff);

}

void test_hashset_foreach(void) {
    HashSet set = hashset_create(sizeof(int), int_equals, int_hash);
    int vals[] = {10, 20, 30, 40};
    int n = sizeof(vals) / sizeof(vals[0]);

    for (int i = 0; i < n; i++) {
        hashset_add(&set, &vals[i]);
    }

    g_foreach_sum = 0;
    g_foreach_count = 0;

    hashset_foreach(&set, accumulate_foreach_func);

    assert(g_foreach_count == n);
    assert(g_foreach_sum == (10 + 20 + 30 + 40));

    hashset_free(&set);
}

void test_hashset_stress_and_performance(void) {

    HashSet set = hashset_create(sizeof(int), int_equals, int_hash);
    int total = 10000;

    clock_t start = clock();

    for (int i = 0; i < total; i++) {
        hashset_add(&set, &i);
    }
    assert(hashset_length(&set) == total);

    for (int i = 0; i < total; i++) {
        assert(hashset_has(&set, &i) == true);
    }

    for (int i = 0; i < total / 2; i++) {
        assert(hashset_remove(&set, &i) == true);
        assert(hashset_has(&set, &i) == false);
    }
    assert(hashset_length(&set) == total / 2);
    for (int i = total / 2; i < total; i++) {
        assert(hashset_has(&set, &i) == true);
    }
    for (int i = 0; i < total / 2; i++) {
        hashset_add(&set, &i);
    }
    assert(hashset_length(&set) == total);

    hashset_free(&set);

    clock_t end = clock();
    double total_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("HashSet benchmark: %d elements inserted, queried, and removed in %.6f seconds.\n",
           total, total_time);

}

int main(void) {

    test_hashset_basic();
    test_hashset_copy();
    test_hashset_set_operations();
    test_hashset_foreach();
    test_hashset_stress_and_performance();

    puts("All tests passed.");
    return 0;
}