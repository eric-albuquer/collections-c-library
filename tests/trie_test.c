#include <stdio.h>
#include <assert.h>
#include <string.h>
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

int main() {

    test_trie_basic();

    test_trie_with_string_struct();

    test_trie_edge_cases();

    puts("All tests passed.");
    return 0;
}