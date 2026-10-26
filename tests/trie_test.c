#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "collections.h"

void test_trie_basic() {
    Trie trie = trie_create();
    assert(trie_length(&trie) == 0);

    trie_add_str(&trie, "home");
    trie_add_str(&trie, "homemento");
    trie_add_str(&trie, "car");

    assert(trie_length(&trie) == 3);

    assert(trie_has_str(&trie, "home") == true);
    assert(trie_has_str(&trie, "homemento") == true);
    assert(trie_has_str(&trie, "car") == true);

    assert(trie_has_str(&trie, "hom") == false);
    assert(trie_has_str(&trie, "dog") == false);
    assert(trie_has_str(&trie, "homesteads") == false);

    assert(trie_has_prefix_str(&trie, "hom") == true);
    assert(trie_has_prefix_str(&trie, "ho") == true);
    assert(trie_has_prefix_str(&trie, "car") == true);
    assert(trie_has_prefix_str(&trie, "dog") == false);

    trie_free(&trie);
}

void test_trie_with_string_struct() {
    Trie trie = trie_create();

    String s1 = string_create("programming");
    String s2 = string_create("progress");
    String s3 = string_create("pro");

    trie_add_string(&trie, &s1);
    trie_add_string(&trie, &s2);
    trie_add_string(&trie, &s3);

    assert(trie_length(&trie) == 3);

    assert(trie_has_string(&trie, &s1) == true);
    assert(trie_has_string(&trie, &s2) == true);
    assert(trie_has_string(&trie, &s3) == true);

    String p1 = string_create("progr");
    String p2 = string_create("dog");

    assert(trie_has_prefix_string(&trie, &p1) == true);
    assert(trie_has_prefix_string(&trie, &p2) == false);

    string_free(&s1);
    string_free(&s2);
    string_free(&s3);
    string_free(&p1);
    string_free(&p2);
    trie_free(&trie);
}

void test_trie_edge_cases() {
    Trie trie = trie_create();

    trie_add_str(&trie, "");
    assert(trie_has_str(&trie, "") == false);

    trie_add_str(&trie, "test");
    trie_add_str(&trie, "test");
    assert(trie_length(&trie) == 1);
    assert(trie_has_str(&trie, "test") == true);

    trie_free(&trie);
}

void test_trie_copy() {
    Trie original = trie_create();

    trie_add_str(&original, "apple");
    trie_add_str(&original, "app");
    trie_add_str(&original, "application");
    trie_add_str(&original, "banana");

    // 1. Create a deep copy
    Trie copy = trie_copy(&original);

    // 2. Verify length and elements match in the copy
    assert(trie_length(&copy) == trie_length(&original));
    assert(trie_has_str(&copy, "apple") == true);
    assert(trie_has_str(&copy, "app") == true);
    assert(trie_has_str(&copy, "application") == true);
    assert(trie_has_str(&copy, "banana") == true);
    assert(trie_has_prefix_str(&copy, "applic") == true);

    // 3. Mutate original to ensure independence (deep copy)
    trie_add_str(&original, "orange");
    assert(trie_has_str(&original, "orange") == true);
    assert(trie_has_str(&copy, "orange") == false);
    assert(trie_length(&copy) != trie_length(&original));

    trie_free(&original);
    trie_free(&copy);
}

void test_trie_performance() {
    const int num_words = 100000;
    const int word_len = 10;

    Trie trie = trie_create();
    char** words = (char**)malloc(num_words * sizeof(char*));

    srand(42); // Fixed seed for reproducible benchmarks

    // Generate random lower-case strings
    for (int i = 0; i < num_words; i++) {
        words[i] = (char*)malloc((word_len + 1) * sizeof(char));
        for (int j = 0; j < word_len; j++) {
            words[i][j] = 'a' + (rand() % 26);
        }
        words[i][word_len] = '\0';
    }

    clock_t start_time = clock();

    // 1. Batch Insertion Test
    for (int i = 0; i < num_words; i++) {
        trie_add_str(&trie, words[i]);
    }

    // 2. Batch Lookup Test
    for (int i = 0; i < num_words; i++) {
        assert(trie_has_str(&trie, words[i]) == true);
    }

    // 3. Copy Test under workload
    Trie copy = trie_copy(&trie);
    assert(trie_length(&copy) == trie_length(&trie));

    clock_t end_time = clock();
    double total_time_ms = ((double)(end_time - start_time) / CLOCKS_PER_SEC) * 1000.0;

    printf("[PERFORMANCE] %d insertions + %d lookups + 1 copy completed in %.2f ms\n",
           num_words, num_words, total_time_ms);

    // Cleanup
    for (int i = 0; i < num_words; i++) {
        free(words[i]);
    }
    free(words);

    trie_free(&trie);
    trie_free(&copy);
}

int main() {
    test_trie_basic();
    test_trie_with_string_struct();
    test_trie_edge_cases();
    test_trie_copy();
    test_trie_performance();

    puts("All Trie tests passed.");
    return 0;
}