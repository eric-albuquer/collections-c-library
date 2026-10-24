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
// COMPARE GENERIC FUNCTIONS
// ==========================================================

int int_cmp(const void* a, const void* b);
int long_long_cmp(const void* a, const void* b);
int float_cmp(const void* a, const void* b);
int double_cmp(const void* a, const void* b);
int char_cmp(const void* a, const void* b);
int str_cmp(const void* a, const void* b);

// ==========================================================
// MIN MAX FUNCTIONS
// ==========================================================

int int_min(int a, int b);
int int_max(int a, int b);
long long long_long_min(long long a, long long b);
long long long_long_max(long long a, long long b);
float float_min(float a, float b);
float float_max(float a, float b);
double double_min(double a, double b);
double double_max(double a, double b);
char char_min(char a, char b);
char char_max(char a, char b);

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

String string_create(const char* str);
void string_free(String* string);

const char* string_get_str(const String* string);
int string_length(const String* string);

void string_append_char(String* string, const char chr);
void string_clear(String* string);

String string_copy(const String* string);

int string_compare(const String* a, const String* b);

void string_extend(String* a, const String* b);

String string_concatenate(const String* a, const String* b);

int string_index_of(const String* a, const String* b); 
bool string_contains(const String* a, const String* b); 

bool string_equals(const String* a, const String* b);
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

int arraylist_length(const ArrayList* list);

bool arraylist_get(const ArrayList* list, int idx, void* out);
bool arraylist_set(ArrayList* list, int idx, const void* element);

bool arraylist_insert_index(ArrayList* list, int idx, const void* element);
bool arraylist_remove_index(ArrayList* list, int idx, void* out);

void arraylist_clear(ArrayList* list);

void arraylist_append(ArrayList* list, const void* element);
bool arraylist_pop(ArrayList* list, void* out);

bool arraylist_swap(ArrayList* list, int i, int j);

void arraylist_shuffle(ArrayList* list);

ArrayList arraylist_copy(const ArrayList* list);
void arraylist_sort(ArrayList* list, int(*cmp)(const void*, const void*));
void arraylist_foreach(ArrayList* list, void(*function)(int, const void*));

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

int arraydeque_length(const ArrayDeque* deque);

void arraydeque_append(ArrayDeque* deque, const void* element);
void arraydeque_appendleft(ArrayDeque* deque, const void* element);
bool arraydeque_pop(ArrayDeque* deque, void* out);
bool arraydeque_popleft(ArrayDeque* deque, void* out);

ArrayDeque arraydeque_copy(const ArrayDeque* deque);

void arraydeque_foreach(ArrayDeque* deque, void(*function)(int, const void*));

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

int heap_length(const Heap* heap);

void heap_push(Heap* heap, const void* element);
bool heap_pop(Heap* heap, void* out);

Heap heap_copy(const Heap* heap);

void heap_foreach(Heap* heap, void(*function)(int, const void*));

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

int hashmap_length(const HashMap* map);

void hashmap_set(HashMap* map, const void* key, const void* value, void* old_value);
bool hashmap_get(const HashMap* map, const void* key, void* out);
bool hashmap_remove(HashMap* map, const void* key, void* out);

HashMap hashmap_copy(const HashMap* map);

void hashmap_keys(HashMap* map, void (*function)(const void*));
void hashmap_values(HashMap* map, void (*function)(const void*));
void hashmap_items(HashMap* map, void (*function)(const void*, const void*));

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

int hashset_length(const HashSet* set);

void hashset_add(HashSet* set, const void* element);
bool hashset_has(const HashSet* set, const void* element);
bool hashset_remove(HashSet* set, const void* element);

HashSet hashset_copy(const HashSet* set);

HashSet hashset_union(const HashSet* a, const HashSet* b);
HashSet hashset_intersection(const HashSet* a, const HashSet* b);
HashSet hashset_difference(const HashSet* a, const HashSet* b);

void hashset_foreach(HashSet* set, void (*function)(const void*));

// ==========================================================
// FROZEN HASH SET
// ==========================================================

typedef struct FrozenHashSet {
    HashSet _set;
    bool _checksum_computed;
    uint64_t _checksum;
} FrozenHashSet;

FrozenHashSet frozenhashset_create(const HashSet set);
void frozenhashset_free(FrozenHashSet* set);

int frozenhashset_length(const FrozenHashSet* set);

bool frozenhashset_has(const FrozenHashSet* set, const void* element);
bool frozenhashset_equals(const FrozenHashSet* a, const FrozenHashSet* b);
uint64_t frozenhashset_hash(FrozenHashSet* f_set);

FrozenHashSet frozenhashset_union(const FrozenHashSet* a, const FrozenHashSet* b);
FrozenHashSet frozenhashset_intersection(const FrozenHashSet* a, const FrozenHashSet* b);
FrozenHashSet frozenhashset_difference(const FrozenHashSet* a, const FrozenHashSet* b);

void frozenhashset_foreach(FrozenHashSet* set, void (*function)(const void*));

// ==========================================================
// OBJECT POOL
// ==========================================================

typedef struct ObjectPool {
    int _capacity;
    int _obj_size;
    int _used;

    int _head;
    unsigned char* _memory;
} ObjectPool;

ObjectPool objpool_create(int max_capacity, int obj_size);
void objpool_free(ObjectPool* pool);

void* objpool_malloc(ObjectPool* pool);
void objpool_free_obj(ObjectPool* pool, void* obj);

#endif