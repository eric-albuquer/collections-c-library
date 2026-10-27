#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "collections.h"

typedef struct User {
    int id;
    int age;
} User;

static int enumerated_sum;
static size_t enumerated_count;
static bool enumerated_items_valid;

static void accumulate_key(const void* key) {
    enumerated_sum += *(const int*)key;
    enumerated_count++;
}

static void accumulate_value(void* value) {
    enumerated_sum += *(int*)value;
    enumerated_count++;
}

static void validate_map_item(const void* key, void* value) {
    if (*(const int*)value != *(const int*)key * 10) {
        enumerated_items_valid = false;
    }
    enumerated_count++;
}

static void increment_map_value(void* value) {
    (*(int*)value)++;
}

uint64_t user_hash(const void* value) {
    if (value == NULL) return 0;
    const User* u = (const User*)value;
    uint64_t h = (uint64_t)u->id * 0xbf58476d1ce4e5b9ULL;
    h ^= (uint64_t)u->age * 0x94d049bb133111ebULL;
    return h ^ (h >> 31);
}

bool user_equals(const void* a, const void* b) {
    if (a == b) return true;
    if (a == NULL || b == NULL) return false;
    const User* ua = (const User*)a;
    const User* ub = (const User*)b;
    return ua->id == ub->id && ua->age == ub->age;
}


void test_hashmap_basic_int(void) {
    HashMap map = hashmap_create(sizeof(int), sizeof(int), int_equals, int_hash);
    assert(hashmap_length(&map) == 0);

    int k1 = 1, v1 = 100;
    int k2 = 2, v2 = 200;
    int k3 = 3, v3 = 300;

    hashmap_set(&map, &k1, &v1, NULL);
    hashmap_set(&map, &k2, &v2, NULL);
    hashmap_set(&map, &k3, &v3, NULL);
    assert(hashmap_length(&map) == 3);

    int out = 0;
    assert(hashmap_get(&map, &k1, &out) == true);
    assert(out == 100);

    assert(hashmap_get(&map, &k3, &out) == true);
    assert(out == 300);

    int k_missing = 99;
    assert(hashmap_get(&map, &k_missing, &out) == false);

    int v1_updated = 150;
    int old_val = 0;
    hashmap_set(&map, &k1, &v1_updated, &old_val);
    assert(old_val == 100);
    assert(hashmap_length(&map) == 3);

    assert(hashmap_get(&map, &k1, &out) == true);
    assert(out == 150);

    assert(hashmap_remove(&map, &k2, &out) == true);
    assert(out == 200);
    assert(hashmap_length(&map) == 2);

    assert(hashmap_remove(&map, &k2, &out) == false);
    assert(hashmap_get(&map, &k2, &out) == false);

    hashmap_free(&map);
}

void test_hashmap_string_keys(void) {
    HashMap map = hashmap_create(sizeof(char*), sizeof(int), str_equals, str_hash);

    char* k1 = "apple";
    char* k2 = "banana";
    int v1 = 10;
    int v2 = 20;

    hashmap_set(&map, &k1, &v1, NULL);
    hashmap_set(&map, &k2, &v2, NULL);
    assert(hashmap_length(&map) == 2);

    int out = 0;
    char* search_key = "apple";
    assert(hashmap_get(&map, &search_key, &out) == true);
    assert(out == 10);

    char* missing_key = "orange";
    assert(hashmap_get(&map, &missing_key, &out) == false);

    hashmap_free(&map);
}

void test_hashmap_custom_string_type(void) {
    HashMap map = hashmap_create(sizeof(String), sizeof(int),
        (bool(*)(const void*, const void*))string_equals,
        (uint64_t(*)(const void*))string_hash);

    String s1 = string_create("user_one");
    String s2 = string_create("user_two");
    int v1 = 500;
    int v2 = 800;

    hashmap_set(&map, &s1, &v1, NULL);
    hashmap_set(&map, &s2, &v2, NULL);
    assert(hashmap_length(&map) == 2);

    int out = 0;
    String search = string_create("user_one");
    assert(hashmap_get(&map, &search, &out) == true);
    assert(out == 500);

    string_free(&search);
    string_free(&s1);
    string_free(&s2);
    hashmap_free(&map);

}

void test_hashmap_struct_keys(void) {
    HashMap map = hashmap_create(sizeof(User), sizeof(int), user_equals, user_hash);

    User u1 = { .id = 1, .age = 25 };
    User u2 = { .id = 2, .age = 30 };
    int score1 = 95;
    int score2 = 88;

    hashmap_set(&map, &u1, &score1, NULL);
    hashmap_set(&map, &u2, &score2, NULL);
    assert(hashmap_length(&map) == 2);

    int out = 0;
    User query = { .id = 1, .age = 25 };
    assert(hashmap_get(&map, &query, &out) == true);
    assert(out == 95);

    hashmap_free(&map);
}

void test_hashmap_stress_and_rehash(void) {

    HashMap map = hashmap_create(sizeof(int), sizeof(int), int_equals, int_hash);
    size_t total = 10000;

    clock_t start = clock();

    for (size_t i = 0; i < total; i++) {
        int key = (int)i;
        int val = key * 2;
        hashmap_set(&map, &key, &val, NULL);
    }

    assert(hashmap_length(&map) == total);

    for (size_t i = 0; i < total; i++) {
        int key = (int)i;
        int out = 0;
        assert(hashmap_get(&map, &key, &out) == true);
        assert(out == key * 2);
    }

    for (size_t i = 0; i < total / 2; i++) {
        int key = (int)i;
        int out = 0;
        assert(hashmap_remove(&map, &key, &out) == true);
        assert(out == key * 2);
        assert(hashmap_get(&map, &key, &out) == false);
    }

    assert(hashmap_length(&map) == total / 2);
    for (size_t i = 0; i < total / 2; i++) {
        int key = (int)i;
        int value = -key;
        hashmap_set(&map, &key, &value, NULL);
    }
    assert(hashmap_length(&map) == total);
    for (size_t i = total / 2; i < total; i++) {
        int key = (int)i;
        int out = 0;
        assert(hashmap_get(&map, &key, &out) == true);
        assert(out == key * 2);
    }
    for (size_t i = 0; i < total / 2; i++) {
        int key = (int)i;
        int out = 0;
        assert(hashmap_get(&map, &key, &out) == true);
        assert(out == -key);
    }

    hashmap_free(&map);

    clock_t end = clock();
    double total_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("HashMap benchmark: %zu keys inserted and queried in %.6f seconds.\n",
           total, total_time);

}

void test_hashmap_copy_and_iteration(void) {
    HashMap original = hashmap_create(sizeof(int), sizeof(int), int_equals, int_hash);
    for (int key = 1; key <= 3; key++) {
        int value = key * 10;
        hashmap_set(&original, &key, &value, NULL);
    }

    HashMap copy = hashmap_copy(&original);
    assert(hashmap_length(&copy) == 3);

    enumerated_sum = 0;
    enumerated_count = 0;
    hashmap_keys(&copy, accumulate_key);
    assert(enumerated_count == 3);
    assert(enumerated_sum == 6);

    enumerated_sum = 0;
    enumerated_count = 0;
    hashmap_values(&copy, accumulate_value);
    assert(enumerated_count == 3);
    assert(enumerated_sum == 60);

    enumerated_count = 0;
    enumerated_items_valid = true;
    hashmap_items(&copy, validate_map_item);
    assert(enumerated_count == 3);
    assert(enumerated_items_valid);

    hashmap_values(&copy, increment_map_value);
    for (int key = 1; key <= 3; key++) {
        int original_value = 0;
        int copied_value = 0;
        assert(hashmap_get(&original, &key, &original_value));
        assert(hashmap_get(&copy, &key, &copied_value));
        assert(original_value == key * 10);
        assert(copied_value == key * 10 + 1);
    }

    hashmap_free(&original);
    hashmap_free(&copy);
}

int main(void) {

    test_hashmap_basic_int();
    test_hashmap_string_keys();
    test_hashmap_custom_string_type();
    test_hashmap_struct_keys();
    test_hashmap_stress_and_rehash();
    test_hashmap_copy_and_iteration();

    puts("All HashMap tests passed.");
    return 0;
}