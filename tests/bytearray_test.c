#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include "collections.h"


void test_bytearray_creation_and_lifecycle(void) {
    ByteArray ba = bytearray_create(10);
    assert(ba._capacity == 10);
    assert(ba._size == 0);
    assert(ba._cursor == 0);
    assert(ba._data != NULL);
    assert(bytearray_available(&ba) == 0);

    ba._size = 5;
    ba._cursor = 3;
    bytearray_clear(&ba);
    assert(ba._size == 0);
    assert(ba._cursor == 0);
    assert(ba._capacity == 10);

    bytearray_free(&ba);

    ByteArray empty = bytearray_create(0);
    assert(empty._size == 0);
    assert(empty._cursor == 0);
    bytearray_free(&empty);

}

void test_bytearray_reserve(void) {
    ByteArray ba = bytearray_create(16);

    bytearray_reserve(&ba, 64);
    assert(ba._capacity == 64);
    assert(ba._size == 0);

    bytearray_reserve(&ba, 32);
    assert(ba._capacity == 64);

    bytearray_free(&ba);
}

void test_bytearray_write_and_expansion(void) {
    ByteArray ba = bytearray_create(4);

    uint8_t data1[4] = {0xAA, 0xBB, 0xCC, 0xDD};
    bytearray_write_bytes(&ba, data1, 4);
    assert(ba._size == 4);
    assert(ba._cursor == 4);
    assert(ba._capacity == 4);
    assert(bytearray_available(&ba) == 0);

    uint8_t data2[2] = {0xEE, 0xFF};
    bytearray_write_bytes(&ba, data2, 2);
    assert(ba._size == 6);
    assert(ba._cursor == 6);
    assert(ba._capacity > 4);
    assert(bytearray_available(&ba) == 0);

    assert(ba._data[0] == 0xAA);
    assert(ba._data[4] == 0xEE);
    assert(ba._data[5] == 0xFF);

    bytearray_free(&ba);
}

void test_bytearray_read_and_bounds(void) {
    ByteArray ba = bytearray_create(8);
    uint8_t input[5] = {10, 20, 30, 40, 50};
    bytearray_write_bytes(&ba, input, 5);

    bytearray_seek(&ba, 0);
    assert(ba._cursor == 0);
    assert(bytearray_available(&ba) == 5);

    uint8_t out1[2] = {0};
    int bytes_read_1 = bytearray_read_bytes(&ba, out1, 2);
    assert(bytes_read_1 == 2);
    assert(out1[0] == 10);
    assert(out1[1] == 20);
    assert(ba._cursor == 2);
    assert(bytearray_available(&ba) == 3);

    uint8_t out2[3] = {0};
    int bytes_read_2 = bytearray_read_bytes(&ba, out2, 3);
    assert(bytes_read_2 == 3);
    assert(out2[0] == 30);
    assert(out2[2] == 50);
    assert(ba._cursor == 5);
    assert(ba._cursor == ba._size);
    assert(bytearray_available(&ba) == 0);

    uint8_t out_err[1];
    int bytes_read_error = bytearray_read_bytes(&ba, out_err, 1);
    assert(bytes_read_error == 0);
    assert(ba._cursor == 5);

    bytearray_free(&ba);
}

void test_bytearray_seek_and_overwrite(void) {
    ByteArray ba = bytearray_create(10);
    uint8_t data[4] = {0x01, 0x02, 0x03, 0x04};
    bytearray_write_bytes(&ba, data, 4);

    bool s1 = bytearray_seek(&ba, 0);
    assert(s1 == true);
    assert(ba._cursor == 0);

    uint8_t new_data[2] = {0x99, 0x88};
    bytearray_write_bytes(&ba, new_data, 2);

    assert(ba._size == 4);
    assert(ba._cursor == 2);
    assert(bytearray_available(&ba) == 2);
    assert(ba._data[0] == 0x99);
    assert(ba._data[1] == 0x88);
    assert(ba._data[2] == 0x03);

    bool s_err = bytearray_seek(&ba, 10);
    assert(s_err == false);
    assert(ba._cursor == 2);

    bytearray_free(&ba);
}

void test_bytearray_performance_and_stress(void) {

    clock_t start = clock();

    ByteArray ba = bytearray_create(1);
    int iterations = 50000;
    uint8_t sample_byte = 0xAA;

    for (int i = 0; i < iterations; i++) {
        bytearray_write_bytes(&ba, &sample_byte, 1);
    }

    assert(ba._size == iterations);
    assert(ba._cursor == iterations);
    assert(ba._capacity >= (size_t)iterations);

    bytearray_seek(&ba, 0);
    uint8_t* output_buffer = (uint8_t*)malloc(iterations);
    assert(output_buffer != NULL);

    int bytes_read = bytearray_read_bytes(&ba, output_buffer, iterations);
    assert(bytes_read == iterations);
    assert(output_buffer[iterations - 1] == 0xAA);

    free(output_buffer);
    bytearray_free(&ba);

    clock_t end = clock();
    double total_time = (double)(end - start) / CLOCKS_PER_SEC;
    printf("ByteArray benchmark: %d byte writes and reads in %.6f seconds.\n",
           iterations, total_time);

}

int main(void) {

    test_bytearray_creation_and_lifecycle();
    test_bytearray_reserve();
    test_bytearray_write_and_expansion();
    test_bytearray_read_and_bounds();
    test_bytearray_seek_and_overwrite();
    test_bytearray_performance_and_stress();

    puts("All tests passed.");
    return 0;
}
