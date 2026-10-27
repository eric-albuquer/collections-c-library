#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "collections.h"

static size_t foreach_count;
static bool foreach_matches_text;

static void dummy_callback(size_t index, char* chr) {
    static const char expected[] = "Apple";
    if (index != foreach_count || *chr != expected[foreach_count]) {
        foreach_matches_text = false;
    }
    foreach_count++;
}

void test_string_correctness() {

    String s = string_create("  Hello, C Collections!  ");
    assert(string_length(&s) == 25);
    assert(strcmp(string_get_str(&s), "  Hello, C Collections!  ") == 0);
    assert(string_is_empty(&s) == false);

    String empty = string_create("");
    assert(string_length(&empty) == 0);
    assert(string_is_empty(&empty) == true);

    String copy = string_copy(&s);
    assert(string_equals(&s, &copy) == true);

    String mod = string_create("A");
    string_append_char(&mod, 'B');
    assert(strcmp(string_get_str(&mod), "AB") == 0);

    String ext = string_create("CD");
    string_extend(&mod, &ext);
    assert(strcmp(string_get_str(&mod), "ABCD") == 0);

    string_clear(&mod);
    assert(string_length(&mod) == 0);
    assert(string_is_empty(&mod) == true);

    String trimmed = string_trim(&s);
    assert(strcmp(string_get_str(&trimmed), "Hello, C Collections!") == 0);

    String sub = string_substring(&trimmed, 7, 13);
    assert(strcmp(string_get_str(&sub), "C Collections") == 0);

    string_to_lower(&sub);
    assert(strcmp(string_get_str(&sub), "c collections") == 0);

    string_to_upper(&sub);
    assert(strcmp(string_get_str(&sub), "C COLLECTIONS") == 0);

    String s1 = string_create("Data");
    String s2 = string_create("Structures");
    String concat = string_concatenate(&s1, &s2);
    assert(strcmp(string_get_str(&concat), "DataStructures") == 0);

    String text = string_create("Programming in C Language");
    String q1 = string_create("Language");
    String q2 = string_create("Python");

    assert(string_contains(&text, &q1) == true);
    assert(string_contains(&text, &q2) == false);
    assert(string_index_of(&text, &q1) == 17);
    assert(string_index_of(&text, &q2) == -1);
    assert(string_starts_with(&text, "Pro") == true);
    assert(string_ends_with(&text, "Language") == true);

    String a = string_create("Apple");
    String b = string_create("Banana");
    String a_clone = string_create("Apple");

    assert(string_compare(&a, &b) < 0);
    assert(string_compare(&b, &a) > 0);
    assert(string_compare(&a, &a_clone) == 0);
    assert(string_equals(&a, &a_clone) == true);

    uint64_t hash_a = string_hash(&a);
    uint64_t hash_clone = string_hash(&a_clone);
    assert(hash_a == hash_clone);

    foreach_count = 0;
    foreach_matches_text = true;
    string_foreach(&a, dummy_callback);
    assert(foreach_count == string_length(&a));
    assert(foreach_matches_text);

    char local_text[STRING_LOCAL_CAPACITY];
    memset(local_text, 'x', sizeof(local_text) - 1);
    local_text[sizeof(local_text) - 1] = '\0';

    char boundary_text[65];
    memset(boundary_text, 'x', sizeof(boundary_text) - 1);
    boundary_text[sizeof(boundary_text) - 1] = '\0';

    String local_boundary = string_create(local_text);
    assert(string_length(&local_boundary) == STRING_LOCAL_CAPACITY - 1);
    assert(local_boundary._is_local);
    string_append_char(&local_boundary, 'y');
    assert(string_length(&local_boundary) == STRING_LOCAL_CAPACITY);
    assert(!local_boundary._is_local);
    assert(string_get_char(&local_boundary, STRING_LOCAL_CAPACITY - 1) == 'y');

    String heap_boundary = string_create(boundary_text);
    assert(string_length(&heap_boundary) == sizeof(boundary_text) - 1);
    assert(!heap_boundary._is_local);
    assert(strcmp(string_get_str(&heap_boundary), boundary_text) == 0);

    String heap_copy = string_copy(&heap_boundary);
    assert(string_equals(&heap_copy, &heap_boundary));
    String heap_trimmed = string_trim(&heap_boundary);
    assert(string_length(&heap_trimmed) == string_length(&heap_boundary));
    assert(strcmp(string_get_str(&heap_trimmed), boundary_text) == 0);
    String clamped_substring = string_substring(&heap_boundary, 60, 20);
    assert(strcmp(string_get_str(&clamped_substring), "xxxx") == 0);
    String out_of_range_substring = string_substring(&heap_boundary, 100, 4);
    assert(string_is_empty(&out_of_range_substring));

    String self_extended = string_create(local_text);
    string_extend(&self_extended, &self_extended);
    assert(string_length(&self_extended) == 2 * (STRING_LOCAL_CAPACITY - 1));
    assert(memcmp(string_get_str(&self_extended),
                  string_get_str(&self_extended) + STRING_LOCAL_CAPACITY - 1,
                  STRING_LOCAL_CAPACITY - 1) == 0);

    String hash_mutated = string_create("HASH");
    uint64_t original_hash = string_hash(&hash_mutated);
    string_to_lower(&hash_mutated);
    assert(strcmp(string_get_str(&hash_mutated), "hash") == 0);
    assert(string_hash(&hash_mutated) != original_hash);
    string_clear(&hash_mutated);
    assert(string_equals(&hash_mutated, &empty));

    string_free(&s);
    string_free(&empty);
    string_free(&copy);
    string_free(&mod);
    string_free(&ext);
    string_free(&trimmed);
    string_free(&sub);
    string_free(&s1);
    string_free(&s2);
    string_free(&concat);
    string_free(&text);
    string_free(&q1);
    string_free(&q2);
    string_free(&a);
    string_free(&b);
    string_free(&a_clone);
    string_free(&local_boundary);
    string_free(&heap_boundary);
    string_free(&heap_copy);
    string_free(&heap_trimmed);
    string_free(&clamped_substring);
    string_free(&out_of_range_substring);
    string_free(&self_extended);
    string_free(&hash_mutated);

}

void test_string_performance() {

    clock_t start, end;
    double cpu_time_used;
    const size_t iterations = 50000;

    start = clock();
    String s = string_create("");
    for (size_t i = 0; i < iterations; i++) {
        string_append_char(&s, 'x');
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    start = clock();
    for (size_t i = 0; i < iterations; i++) {
        String copy = string_copy(&s);
        volatile size_t len = string_length(&copy);
        (void)len;
        string_free(&copy);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    String target = string_create("   High Performance C Code Optimization Routine   ");
    start = clock();
    for (size_t i = 0; i < iterations; i++) {
        String trimmed = string_trim(&target);
        String sub = string_substring(&trimmed, 5, 11);
        string_to_upper(&sub);
        string_to_lower(&sub);
        string_free(&trimmed);
        string_free(&sub);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    string_free(&target);

    String haystack = string_create("The quick brown fox jumps over the lazy dog and repeats the pattern continuously.");
    String needle = string_create("lazy dog");
    start = clock();
    for (size_t i = 0; i < iterations; i++) {
        volatile bool found = string_contains(&haystack, &needle);
        volatile int idx = string_index_of(&haystack, &needle);
        (void)found;
        (void)idx;
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    string_free(&haystack);
    string_free(&needle);

    start = clock();
    for (size_t i = 0; i < iterations; i++) {
        volatile uint64_t h = string_hash(&s);
        (void)h;
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    string_free(&s);
    printf("String benchmark: %zu iterations per workload; final workload took %.6f seconds.\n",
           iterations, cpu_time_used);

}

int main() {

    test_string_correctness();
    test_string_performance();

    puts("All String tests passed.");
    return 0;
}