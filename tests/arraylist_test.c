#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "collections.h"


int int_asc_compare(const void* a, const void* b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

static int g_map_sum = 0;
static size_t g_map_calls = 0;
static bool g_map_indices_valid = true;

void accumulate_map_func(size_t idx, void* element) {
    if (idx != g_map_calls) {
        g_map_indices_valid = false;
    }
    g_map_sum += *(const int*)element;
    g_map_calls++;
}


void test_arraylist_creation_and_basic(void) {
    ArrayList list = arraylist_create(sizeof(int), 0);
    assert(list._size == 0);
    assert(list._element_size == sizeof(int));
    assert(arraylist_length(&list) == 0);

    int v1 = 10, v2 = 20, v3 = 30;

    arraylist_append(&list, &v1);
    arraylist_append(&list, &v2);
    arraylist_append(&list, &v3);
    assert(arraylist_length(&list) == 3);

    int out = 0;
    assert(arraylist_pop(&list, &out) == true);
    assert(out == 30);
    assert(arraylist_length(&list) == 2);

    assert(arraylist_pop(&list, &out) == true);
    assert(out == 20);

    assert(arraylist_pop(&list, &out) == true);
    assert(out == 10);
    assert(arraylist_length(&list) == 0);

    assert(arraylist_pop(&list, &out) == false);

    arraylist_free(&list);

    ArrayList custom_list = arraylist_create(sizeof(int), 50);
    assert(custom_list._capacity >= 50);
    assert(arraylist_length(&custom_list) == 50);
    arraylist_free(&custom_list);

}

void test_arraylist_get_set(void) {
    ArrayList list = arraylist_create(sizeof(int), 0);

    int values[] = {100, 200, 300};
    for (size_t i = 0; i < 3; i++) {
        arraylist_append(&list, &values[i]);
    }

    int out = 0;
    assert(arraylist_get(&list, 0, &out) == true);
    assert(out == 100);

    assert(arraylist_get(&list, 2, &out) == true);
    assert(out == 300);

    assert(arraylist_get(&list, SIZE_MAX, &out) == false);
    assert(arraylist_get(&list, 3, &out) == false);

    int new_val = 999;
    assert(arraylist_set(&list, 1, &new_val) == true);
    assert(arraylist_get(&list, 1, &out) == true);
    assert(out == 999);

    assert(arraylist_set(&list, SIZE_MAX, &new_val) == false);
    assert(arraylist_set(&list, 10, &new_val) == false);

    arraylist_free(&list);
}

void test_arraylist_insert_remove_index(void) {
    ArrayList list = arraylist_create(sizeof(int), 0);

    int a = 1, b = 3, c = 2;
    arraylist_append(&list, &a);
    arraylist_append(&list, &b);

    assert(arraylist_insert_index(&list, 1, &c) == true);
    assert(arraylist_length(&list) == 3);

    int out = 0;
    arraylist_get(&list, 1, &out);
    assert(out == 2);

    int invalid = 99;
    assert(arraylist_insert_index(&list, SIZE_MAX, &invalid) == false);
    assert(arraylist_insert_index(&list, 10, &invalid) == false);

    assert(arraylist_remove_index(&list, 1, &out) == true);
    assert(out == 2);
    assert(arraylist_length(&list) == 2);

    arraylist_get(&list, 1, &out);
    assert(out == 3);

    assert(arraylist_remove_index(&list, SIZE_MAX, &out) == false);
    assert(arraylist_remove_index(&list, 5, &out) == false);

    arraylist_free(&list);
}

void test_arraylist_swap_and_clear(void) {
    ArrayList list = arraylist_create(sizeof(int), 0);
    int x = 10, y = 20;

    arraylist_append(&list, &x);
    arraylist_append(&list, &y);

    assert(arraylist_swap(&list, 0, 1) == true);
    int out = 0;
    arraylist_get(&list, 0, &out);
    assert(out == 20);
    arraylist_get(&list, 1, &out);
    assert(out == 10);

    assert(arraylist_swap(&list, 0, 5) == false);

    arraylist_clear(&list);
    assert(arraylist_length(&list) == 0);

    assert(arraylist_get(&list, 0, &out) == false);

    arraylist_free(&list);
}

void test_arraylist_sort_map_shuffle(void) {
    ArrayList list = arraylist_create(sizeof(int), 0);

    int unsorted[] = {42, 12, 88, 3, 25};
    size_t n = sizeof(unsorted) / sizeof(unsorted[0]);

    for (size_t i = 0; i < n; i++) {
        arraylist_append(&list, &unsorted[i]);
    }

    arraylist_sort(&list, int_asc_compare);
    int expected_sorted[] = {3, 12, 25, 42, 88};
    for (size_t i = 0; i < n; i++) {
        int out = 0;
        arraylist_get(&list, i, &out);
        assert(out == expected_sorted[i]);
    }

    g_map_sum = 0;
    g_map_calls = 0;
    g_map_indices_valid = true;
    arraylist_foreach(&list, accumulate_map_func);
    assert(g_map_calls == n);
    assert(g_map_sum == (3 + 12 + 25 + 42 + 88));
    assert(g_map_indices_valid);

    size_t size_before = arraylist_length(&list);
    arraylist_shuffle(&list);
    assert(arraylist_length(&list) == size_before);
    arraylist_sort(&list, int_asc_compare);
    for (size_t i = 0; i < n; i++) {
        int out = 0;
        assert(arraylist_get(&list, i, &out));
        assert(out == expected_sorted[i]);
    }

    arraylist_free(&list);
}

void test_arraylist_copy(void) {
    ArrayList original = arraylist_create(sizeof(int), 0);
    int values[] = {4, 8, 15, 16, 23, 42};
    for (size_t i = 0; i < 6; i++) {
        arraylist_append(&original, &values[i]);
    }

    ArrayList copy = arraylist_copy(&original);
    assert(arraylist_length(&copy) == 6);

    int out = 0;
    for (size_t i = 0; i < 6; i++) {
        assert(arraylist_get(&copy, i, &out));
        assert(out == values[i]);
    }

    int replacement = 99;
    assert(arraylist_set(&copy, 0, &replacement));
    assert(arraylist_get(&original, 0, &out));
    assert(out == values[0]);
    assert(arraylist_get(&copy, 0, &out));
    assert(out == replacement);

    arraylist_free(&original);
    arraylist_free(&copy);
}

void test_arraylist_bsearch(void) {
    ArrayList list = arraylist_create(sizeof(int), 0);

    int v1 = 10, v2 = 25, v3 = 42, v4 = 50, v5 = 99;
    arraylist_append(&list, &v1);
    arraylist_append(&list, &v2);
    arraylist_append(&list, &v3);
    arraylist_append(&list, &v4);
    arraylist_append(&list, &v5);

    assert(arraylist_length(&list) == 5);

    int middle_key = 42;
    int middle_index = arraylist_bsearch(&list, &middle_key, int_asc_compare);
    assert(middle_index == 2);

    int start_key = 10;
    int start_index = arraylist_bsearch(&list, &start_key, int_asc_compare);
    assert(start_index == 0);

    int end_key = 99;
    int end_index = arraylist_bsearch(&list, &end_key, int_asc_compare);
    assert(end_index == 4);

    int smaller_key = 5;
    int smaller_index = arraylist_bsearch(&list, &smaller_key, int_asc_compare);
    assert(smaller_index == -1);

    int between_key = 30;
    int between_index = arraylist_bsearch(&list, &between_key, int_asc_compare);
    assert(between_index == -1);

    int larger_key = 150;
    int larger_index = arraylist_bsearch(&list, &larger_key, int_asc_compare);
    assert(larger_index == -1);

    ArrayList empty_list = arraylist_create(sizeof(int), 0);
    int search_key = 42;
    int empty_index = arraylist_bsearch(&empty_list, &search_key, int_asc_compare);
    assert(empty_index == -1);
    arraylist_sort(&empty_list, int_asc_compare);
    assert(arraylist_length(&empty_list) == 0);

    arraylist_free(&list);
    arraylist_free(&empty_list);

}

void test_arraylist_performance_and_stress(void) {

    clock_t start = clock();

    ArrayList list = arraylist_create(sizeof(int), 0);
    const size_t total = 1000000;

    for (size_t i = 0; i < total; i++) {
        int value = (int)(total - i - 1);
        arraylist_append(&list, &value);
    }

    assert(arraylist_length(&list) == total);

    for (size_t i = 0; i < total; i++) {
        int out = 0;
        assert(arraylist_get(&list, i, &out) == true);
        assert(out == (int)(total - i - 1));
    }

    clock_t sort_start = clock();
    arraylist_sort(&list, int_asc_compare);
    clock_t sort_end = clock();

    int first = -1;
    int last = -1;
    assert(arraylist_get(&list, 0, &first));
    assert(arraylist_get(&list, total - 1, &last));
    assert(first == 0);
    assert(last == (int)(total - 1));
    for (size_t i = 0; i < total; i++) {
        int out = -1;
        assert(arraylist_get(&list, i, &out));
        assert(out == (int)i);
    }

    arraylist_free(&list);

    clock_t end = clock();
    double total_time = (double)(end - start) / CLOCKS_PER_SEC;
    double sort_time = (double)(sort_end - sort_start) / CLOCKS_PER_SEC;
    printf("ArrayList benchmark: %zu elements in %.6f seconds (sort: %.6f seconds).\n",
           total, total_time, sort_time);

}

int main(void) {

    test_arraylist_creation_and_basic();
    test_arraylist_get_set();
    test_arraylist_insert_remove_index();
    test_arraylist_bsearch();
    test_arraylist_swap_and_clear();
    test_arraylist_sort_map_shuffle();
    test_arraylist_copy();
    test_arraylist_performance_and_stress();

    puts("All ArrayList tests passed.");
    return 0;
}