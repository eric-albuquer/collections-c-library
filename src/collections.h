#ifndef COLLECTIONS_H
#define COLLECTIONS_H

#include <stdint.h>
#include <stdbool.h>

// ==========================================================
// EQUAL GENERIC FUNCTIONS
// ==========================================================

bool int_equals(const void* a, const void* b);
bool long_long_equals(const void* a, const void* b);
bool float_equals(const void* a, const void* b);
bool double_equals(const void* a, const void* b);
bool char_equals(const void* a, const void* b);
bool str_equals(const void* a, const void* b);

// ==========================================================
// HASH GENERIC FUNCTIONS
// ==========================================================

uint64_t int_hash(const void* value);
uint64_t long_long_hash(const void* value);
uint64_t float_hash(const void* value);
uint64_t double_hash(const void* value);
uint64_t char_hash(const void* value);
uint64_t str_hash(const void* value);

// ==========================================================
// STRING
// ==========================================================

typedef struct String {
    int _capacity;
    int _size;
    uint64_t _hash;
    bool _hash_computed;

    char* _memory;
} String;

String string_create(char* str);
void string_free(String* string);

char* string_get_str(String* string);
int string_length(String* string);

void string_append_char(String* string, char chr);
void string_clear(String* string);

String string_copy(String* string);

int string_compare(String* a, String* b);

void string_append(String* a, String* b);

bool string_equals(String* a, String* b);
uint64_t string_hash(String* string);

// ==========================================================
// ARRAY LIST
// ==========================================================

typedef struct ArrayList {
    int _capacity;
    int _element_size;
    int _size;

    unsigned char* _swap_buffer;
    unsigned char* _memory;
} ArrayList;

ArrayList arraylist_create(int element_size, int initial_size);
void arraylist_free(ArrayList* list);

int arraylist_length(ArrayList* list);

bool arraylist_get(ArrayList* list, int idx, void* out);
bool arraylist_set(ArrayList* list, int idx, void* element);

bool arraylist_insert_index(ArrayList* list, int idx, void* element);
bool arraylist_remove_index(ArrayList* list, int idx, void* out);

void arraylist_clear(ArrayList* list);

void arraylist_append(ArrayList* list, void* element);
bool arraylist_pop(ArrayList* list, void* out);

bool arraylist_swap(ArrayList* list, int i, int j);

void arraylist_shuffle(ArrayList* list);
void arraylist_sort(ArrayList* list, int(*cmp)(const void*, const void*));
void arraylist_map(ArrayList* list, void(*function)(int, const void*));

// ==========================================================
// ARRAY DEQUE
// ==========================================================

typedef struct ArrayDeque {
    int _capacity;
    int _mask;

    int _element_size;
    int _size;

    int _head;
    int _tail;

    unsigned char* _memory;
} ArrayDeque;

ArrayDeque arraydeque_create(int element_size);
void arraydeque_free(ArrayDeque* deque);

int arraydeque_length(ArrayDeque* deque);

void arraydeque_append(ArrayDeque* deque, void* element);
void arraydeque_appendleft(ArrayDeque* deque, void* element);
bool arraydeque_pop(ArrayDeque* deque, void* out);
bool arraydeque_popleft(ArrayDeque* deque, void* out);

// ==========================================================
// HEAP
// ==========================================================

typedef struct Heap {
    int _capacity;
    int _element_size;

    int (*_cmp)(const void*, const void*);

    int _size;
    unsigned char* _memory;
} Heap;

Heap heap_create(int element_size, int (*cmp)(const void*, const void*));
void heap_free(Heap* heap);

int heap_length(Heap* heap);

void heap_push(Heap* heap, void* element);
bool heap_pop(Heap* heap, void* out);

// ==========================================================
// HASH MAP
// ==========================================================

typedef struct HashMap {
    int _capacity;
    int _mask;
    int _max_elements;

    bool (*_equals)(const void*, const void*);
    uint64_t (*_hash)(const void*); 

    int _key_size;
    int _value_size;
    int _size;

    unsigned char* _states;
    uint64_t* _hash_cache;
    unsigned char* _keys;
    unsigned char* _values;
} HashMap;

HashMap hashmap_create(
    int key_size,
    int value_size,
    bool (*equals)(const void*, const void*),
    uint64_t (*hash)(const void*)
);
void hashmap_free(HashMap* map);

int hashmap_length(HashMap* map);

void hashmap_set(HashMap* map, const void* key, const void* value, void* old_value);
bool hashmap_get(HashMap* map, const void* key, void* out);
bool hashmap_remove(HashMap* map, const void* key, void* out);

// ==========================================================
// HASH SET
// ==========================================================

typedef struct HashSet {
    int _capacity;
    int _mask;
    int _max_elements;

    bool (*_equals)(const void*, const void*);
    uint64_t (*_hash)(const void*); 

    int _element_size;
    int _size;

    unsigned char* _states;
    uint64_t* _hash_cache;
    unsigned char* _elements;
} HashSet;

HashSet hashset_create(
    int element_size,
    bool (*equals)(const void*, const void*),
    uint64_t (*hash)(const void*)
);
void hashset_free(HashSet* set);

int hashset_length(HashSet* set);

void hashset_add(HashSet* set, const void* element);
bool hashset_has(HashSet* set, const void* element);
bool hashset_remove(HashSet* set, const void* element);

#endif