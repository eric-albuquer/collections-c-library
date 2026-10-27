#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "collections.h"


void test_arenapool_creation_and_lifecycle(void) {
    size_t initial_capacity = 1024;
    ArenaPool arena = arenapool_create(initial_capacity);

    assert(arena._capacity == initial_capacity);
    assert(arena._used == 0);
    assert(arena._memory != NULL);
    assert(arenapool_available(&arena) == initial_capacity);

    arenapool_free(&arena);

    ArenaPool empty = arenapool_create(0);
    assert(empty._capacity == 0);
    assert(empty._used == 0);
    assert(arenapool_available(&empty) == 0);
    arenapool_free(&empty);

}

void test_arenapool_sequential_allocations(void) {
    ArenaPool arena = arenapool_create(512);

    void* ptr1 = arenapool_malloc(&arena, 100);
    assert(ptr1 != NULL);
    assert(arena._used >= 100);

    size_t remaining_space_1 = arenapool_available(&arena);
    assert(remaining_space_1 <= 412);

    void* ptr2 = arenapool_malloc(&arena, 50);
    assert(ptr2 != NULL);
    assert(ptr2 != ptr1);

    assert((uint8_t*)ptr2 >= (uint8_t*)ptr1 + 100);

    arenapool_free(&arena);
}

void test_arenapool_out_of_memory(void) {
    ArenaPool arena = arenapool_create(64);

    void* ptr1 = arenapool_malloc(&arena, 32);
    assert(ptr1 != NULL);

    void* ptr_err = arenapool_malloc(&arena, 40);
    assert(ptr_err == NULL);

    assert(arena._used == 32);
    assert(arena._used <= 64);
    assert(arenapool_available(&arena) <= arena._capacity);

    arenapool_free(&arena);
}

void test_arenapool_memory_alignment(void) {
    ArenaPool arena = arenapool_create(128);

    void* ptr1 = arenapool_malloc(&arena, 3);
    assert(ptr1 != NULL);

    void* ptr2 = arenapool_malloc(&arena, 4);
    assert(ptr2 != NULL);

    uintptr_t address = (uintptr_t)ptr2;

    assert(address % 4 == 0);

    arenapool_free(&arena);
}

void test_arenapool_data_integrity(void) {
    ArenaPool arena = arenapool_create(256);

    char* text = (char*)arenapool_malloc(&arena, 12);
    int* numbers = (int*)arenapool_malloc(&arena, 4 * sizeof(int));

    assert(text != NULL);
    assert(numbers != NULL);

    strcpy(text, "Arena Test");
    numbers[0] = 42;
    numbers[1] = 84;
    numbers[2] = 126;
    numbers[3] = 168;

    assert(strcmp(text, "Arena Test") == 0);
    assert(numbers[0] == 42);
    assert(numbers[3] == 168);

    arenapool_free(&arena);
}

void test_arenapool_performance_and_stress(void) {

    clock_t start = clock();

    size_t iterations = 10000;
    size_t block_size = 8;
    ArenaPool arena = arenapool_create(iterations * block_size * 2);

    for (size_t i = 0; i < iterations; i++) {
        void* ptr = arenapool_malloc(&arena, block_size);
        assert(ptr != NULL);
    }

    assert(arena._used >= (iterations * block_size));

    arenapool_free(&arena);

    clock_t end = clock();
    double total_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("ArenaPool benchmark: %zu allocations in %.6f seconds.\n",
           iterations, total_time);

}

int main(void) {

    test_arenapool_creation_and_lifecycle();
    test_arenapool_sequential_allocations();
    test_arenapool_out_of_memory();
    test_arenapool_memory_alignment();
    test_arenapool_data_integrity();
    test_arenapool_performance_and_stress();

    puts("All ArenaPool tests passed.");
    return 0;
}
