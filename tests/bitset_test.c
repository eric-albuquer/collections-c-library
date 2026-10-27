#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include "collections.h"

void test_bitset_basic() {
    BitSet bs = bitset_create(128);

    assert(bitset_none(&bs) == true);
    assert(bitset_any(&bs) == false);
    assert(bitset_all(&bs) == false);

    bitset_set(&bs, 0);
    bitset_set(&bs, 63);
    bitset_set(&bs, 64);
    bitset_set(&bs, 127);

    assert(bitset_get(&bs, 0) == true);
    assert(bitset_get(&bs, 32) == false);
    assert(bitset_get(&bs, 63) == true);
    assert(bitset_get(&bs, 64) == true);
    assert(bitset_get(&bs, 127) == true);

    bitset_flip(&bs, 32);
    assert(bitset_get(&bs, 32) == true);

    bitset_flip(&bs, 0);
    assert(bitset_get(&bs, 0) == false);

    assert(bitset_any(&bs) == true);
    assert(bitset_none(&bs) == false);

    bitset_clear(&bs);
    assert(bitset_none(&bs) == true);
    assert(bitset_get(&bs, 64) == false);

    bitset_free(&bs);
}

void test_bitset_fill() {
    BitSet bs = bitset_create(100);

    bitset_fill(&bs, true);
    assert(bitset_all(&bs) == true);
    assert(bitset_get(&bs, 0) == true);
    assert(bitset_get(&bs, 99) == true);

    bitset_fill(&bs, false);
    assert(bitset_none(&bs) == true);

    bitset_free(&bs);
}

void test_bitset_logic_operations() {
    BitSet a = bitset_create(64);
    BitSet b = bitset_create(64);

    bitset_set(&a, 0);
    bitset_set(&a, 1);

    bitset_set(&b, 1);
    bitset_set(&b, 2);

    BitSet res_and = bitset_and(&a, &b);
    assert(bitset_get(&res_and, 0) == false);
    assert(bitset_get(&res_and, 1) == true);
    assert(bitset_get(&res_and, 2) == false);
    bitset_free(&res_and);

    BitSet res_or = bitset_or(&a, &b);
    assert(bitset_get(&res_or, 0) == true);
    assert(bitset_get(&res_or, 1) == true);
    assert(bitset_get(&res_or, 2) == true);
    assert(bitset_get(&res_or, 3) == false);
    bitset_free(&res_or);

    BitSet res_xor = bitset_xor(&a, &b);
    assert(bitset_get(&res_xor, 0) == true);
    assert(bitset_get(&res_xor, 1) == false);
    assert(bitset_get(&res_xor, 2) == true);
    bitset_free(&res_xor);

    BitSet res_not = bitset_not(&a);
    assert(bitset_get(&res_not, 0) == false);
    assert(bitset_get(&res_not, 1) == false);
    assert(bitset_get(&res_not, 2) == true);
    bitset_free(&res_not);

    BitSet a_copy = bitset_copy(&a);
    assert(bitset_get(&a_copy, 0) == true);
    assert(bitset_get(&a_copy, 1) == true);
    assert(bitset_get(&a_copy, 2) == false);
    bitset_free(&a_copy);

    bitset_free(&a);
    bitset_free(&b);
}

void test_bitset_new_methods() {
    BitSet bs = bitset_create(150);

    assert(bitset_first(&bs) == -1);
    assert(bitset_last(&bs) == -1);

    bitset_set(&bs, 42);
    assert(bitset_get(&bs, 42) == true);
    bitset_reset(&bs, 42);
    assert(bitset_get(&bs, 42) == false);
    assert(bitset_first(&bs) == -1);

    bitset_set(&bs, 10);
    bitset_set(&bs, 65);  
    bitset_set(&bs, 140);

    assert(bitset_first(&bs) == 10);
    assert(bitset_last(&bs) == 140);

    bitset_reset(&bs, 10);
    assert(bitset_first(&bs) == 65);

    bitset_reset(&bs, 140);
    assert(bitset_last(&bs) == 65);

    assert(bitset_first(&bs) == 65);
    assert(bitset_last(&bs) == 65);

    bitset_clear(&bs);
    bitset_set(&bs, 0);
    bitset_set(&bs, 63);

    assert(bitset_first(&bs) == 0);
    assert(bitset_last(&bs) == 63);

    bitset_free(&bs);
}

void test_bitset_performance() {
    const size_t size = 1000000;
    const size_t iterations = 5000000;

    BitSet a = bitset_create(size);
    BitSet b = bitset_create(size);

    srand(42);

    clock_t start_time = clock();

    // 1. Batch Bit setting
    for (size_t i = 0; i < iterations; i++) {
        size_t bit = (size_t)rand() % size;
        bitset_set(&a, bit);
    }

    // 2. Fast Bit Lookup & First/Last search
    assert(bitset_any(&a) == true);
    int first_bit = bitset_first(&a);
    int last_bit = bitset_last(&a);
    assert(first_bit >= 0 && last_bit >= first_bit);

    // 3. Bitwise logic operations on large bitsets
    bitset_fill(&b, true);
    BitSet res_and = bitset_and(&a, &b);
    BitSet res_xor = bitset_xor(&a, &b);

    // 4. Batch Bit resetting
    for (size_t i = 0; i < iterations; i++) {
        size_t bit = (size_t)rand() % size;
        bitset_reset(&a, bit);
    }

    clock_t end_time = clock();
    double total_time_ms = ((double)(end_time - start_time) / CLOCKS_PER_SEC) * 1000.0;

    printf("[PERFORMANCE] %zu bit operations + multi-word logic ops completed in %.2f ms\n",
           iterations * 2, total_time_ms);

    bitset_free(&a);
    bitset_free(&b);
    bitset_free(&res_and);
    bitset_free(&res_xor);
}

int main() {
    test_bitset_basic();
    test_bitset_fill();
    test_bitset_logic_operations();
    test_bitset_new_methods();
    test_bitset_performance();

    puts("All BitSet tests passed.");
    return 0;
}