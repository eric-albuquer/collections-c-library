#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include "collections.h"

static int foreach_count;
static bool foreach_matches_text;

static void dummy_callback(int index, char* chr) {
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

}

void test_string_performance() {

    clock_t start, end;
    double cpu_time_used;
    const int iterations = 50000;

    start = clock();
    String s = string_create("");
    for (int i = 0; i < iterations; i++) {
        string_append_char(&s, 'x');
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    start = clock();
    for (int i = 0; i < iterations; i++) {
        String copy = string_copy(&s);
        volatile int len = string_length(&copy);
        (void)len;
        string_free(&copy);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    String target = string_create("   High Performance C Code Optimization Routine   ");
    start = clock();
    for (int i = 0; i < iterations; i++) {
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
    for (int i = 0; i < iterations; i++) {
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
    for (int i = 0; i < iterations; i++) {
        volatile uint64_t h = string_hash(&s);
        (void)h;
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    string_free(&s);
    printf("String benchmark: %d iterations per workload; final workload took %.6f seconds.\n",
           iterations, cpu_time_used);

}

int main() {

    test_string_correctness();
    test_string_performance();

    puts("All tests passed.");
    return 0;
}