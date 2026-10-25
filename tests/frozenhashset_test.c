#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "collections.h"

static int g_frozen_foreach_sum = 0;
static int g_frozen_foreach_count = 0;

void accumulate_frozen_foreach_func(const void* element) {
    g_frozen_foreach_sum += *(const int*)element;
    g_frozen_foreach_count++;
}


void test_frozenhashset_basic(void) {
    HashSet set = hashset_create(sizeof(int), int_equals, int_hash);
    int v1 = 10, v2 = 20, v3 = 30;

    hashset_add(&set, &v1);
    hashset_add(&set, &v2);
    hashset_add(&set, &v3);

    FrozenHashSet fset = frozenhashset_create(&set);
    hashset_free(&set);

    assert(frozenhashset_length(&fset) == 3);

    assert(frozenhashset_has(&fset, &v1) == true);
    assert(frozenhashset_has(&fset, &v2) == true);
    assert(frozenhashset_has(&fset, &v3) == true);

    int v_missing = 99;
    assert(frozenhashset_has(&fset, &v_missing) == false);

    uint64_t h1 = frozenhashset_hash(&fset);
    assert(h1 != 0);
    assert(frozenhashset_hash(&fset) == h1);

    frozenhashset_free(&fset);
}

void test_frozenhashset_equals(void) {
    HashSet setA = hashset_create(sizeof(int), int_equals, int_hash);
    int x1 = 1, x2 = 2, x3 = 3;
    hashset_add(&setA, &x1);
    hashset_add(&setA, &x2);
    hashset_add(&setA, &x3);
    FrozenHashSet fsetA = frozenhashset_create(&setA);

    HashSet setB = hashset_create(sizeof(int), int_equals, int_hash);
    hashset_add(&setB, &x3);
    hashset_add(&setB, &x1);
    hashset_add(&setB, &x2);
    FrozenHashSet fsetB = frozenhashset_create(&setB);

    HashSet setC = hashset_create(sizeof(int), int_equals, int_hash);
    int x4 = 4;
    hashset_add(&setC, &x1);
    hashset_add(&setC, &x2);
    hashset_add(&setC, &x4);
    FrozenHashSet fset_different = frozenhashset_create(&setC);
    hashset_free(&setA);
    hashset_free(&setB);
    hashset_free(&setC);

    assert(frozenhashset_equals(&fsetA, &fsetB) == true);
    assert(frozenhashset_equals(&fsetA, &fset_different) == false);

    assert(frozenhashset_hash(&fsetA) == frozenhashset_hash(&fsetB));
    assert(frozenhashset_hash(&fsetA) != frozenhashset_hash(&fset_different));

    frozenhashset_free(&fsetA);
    frozenhashset_free(&fsetB);
    frozenhashset_free(&fset_different);

}

void test_frozenhashset_set_operations(void) {
    HashSet sa = hashset_create(sizeof(int), int_equals, int_hash);
    int v1 = 1, v2 = 2, v3 = 3;
    hashset_add(&sa, &v1);
    hashset_add(&sa, &v2);
    hashset_add(&sa, &v3);
    FrozenHashSet fa = frozenhashset_create(&sa);
    hashset_free(&sa);

    HashSet sb = hashset_create(sizeof(int), int_equals, int_hash);
    int v4 = 4, v5 = 5;
    hashset_add(&sb, &v3);
    hashset_add(&sb, &v4);
    hashset_add(&sb, &v5);
    FrozenHashSet fb = frozenhashset_create(&sb);
    hashset_free(&sb);

    FrozenHashSet fu = frozenhashset_union(&fa, &fb);
    assert(frozenhashset_length(&fu) == 5);
    assert(frozenhashset_has(&fu, &v1) == true);
    assert(frozenhashset_has(&fu, &v2) == true);
    assert(frozenhashset_has(&fu, &v3) == true);
    assert(frozenhashset_has(&fu, &v4) == true);
    assert(frozenhashset_has(&fu, &v5) == true);

    FrozenHashSet fintersect = frozenhashset_intersection(&fa, &fb);
    assert(frozenhashset_length(&fintersect) == 1);
    assert(frozenhashset_has(&fintersect, &v3) == true);
    assert(frozenhashset_has(&fintersect, &v1) == false);
    assert(frozenhashset_has(&fintersect, &v2) == false);
    assert(frozenhashset_has(&fintersect, &v4) == false);
    assert(frozenhashset_has(&fintersect, &v5) == false);

    FrozenHashSet fdiff = frozenhashset_difference(&fa, &fb);
    assert(frozenhashset_length(&fdiff) == 2);
    assert(frozenhashset_has(&fdiff, &v1) == true);
    assert(frozenhashset_has(&fdiff, &v2) == true);
    assert(frozenhashset_has(&fdiff, &v3) == false);

    frozenhashset_free(&fa);
    frozenhashset_free(&fb);
    frozenhashset_free(&fu);
    frozenhashset_free(&fintersect);
    frozenhashset_free(&fdiff);

}

void test_frozenhashset_foreach(void) {
    HashSet set = hashset_create(sizeof(int), int_equals, int_hash);
    int vals[] = {5, 15, 25};
    int n = sizeof(vals) / sizeof(vals[0]);

    for (int i = 0; i < n; i++) {
        hashset_add(&set, &vals[i]);
    }

    FrozenHashSet fset = frozenhashset_create(&set);
    hashset_free(&set);

    g_frozen_foreach_sum = 0;
    g_frozen_foreach_count = 0;

    frozenhashset_foreach(&fset, accumulate_frozen_foreach_func);

    assert(g_frozen_foreach_count == n);
    assert(g_frozen_foreach_sum == (5 + 15 + 25));

    frozenhashset_free(&fset);
}

void test_frozenhashset_stress_and_performance(void) {

    HashSet set = hashset_create(sizeof(int), int_equals, int_hash);
    int total = 5000;

    for (int i = 0; i < total; i++) {
        hashset_add(&set, &i);
    }

    clock_t start = clock();

    FrozenHashSet fset = frozenhashset_create(&set);
    hashset_free(&set);
    assert(frozenhashset_length(&fset) == total);

    uint64_t checksum_cached = frozenhashset_hash(&fset);
    assert(checksum_cached != 0);

    for (int i = 0; i < total; i++) {
        assert(frozenhashset_has(&fset, &i) == true);
    }

    HashSet copy = hashset_copy(&fset._set);
    FrozenHashSet fset_copy = frozenhashset_create(&copy);
    hashset_free(&copy);
    assert(frozenhashset_equals(&fset, &fset_copy) == true);

    frozenhashset_free(&fset);
    frozenhashset_free(&fset_copy);

    clock_t end = clock();
    double total_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("FrozenHashSet benchmark: %d elements validated in %.6f seconds.\n",
           total, total_time);

}

int main(void) {

    test_frozenhashset_basic();
    test_frozenhashset_equals();
    test_frozenhashset_set_operations();
    test_frozenhashset_foreach();
    test_frozenhashset_stress_and_performance();

    puts("All tests passed.");
    return 0;
}