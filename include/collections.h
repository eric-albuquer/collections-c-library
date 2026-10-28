#ifndef COLLECTIONS_H
#define COLLECTIONS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define STRING_LOCAL_CAPACITY 32

// ==========================================================
// EQUAL GENERIC FUNCTIONS
// ==========================================================

typedef bool (*equals_fn)(const void*, const void*);

bool int_equals(const void* a, const void* b);
bool long_long_equals(const void* a, const void* b);
bool float_equals(const void* a, const void* b);
bool double_equals(const void* a, const void* b);
bool char_equals(const void* a, const void* b);
bool str_equals(const void* a, const void* b);

// ==========================================================
// HASH GENERIC FUNCTIONS
// ==========================================================

uint64_t hash_combine(uint64_t seed, uint64_t hash);

typedef uint64_t (*hash_fn)(const void*);

uint64_t int_hash(const void* value);
uint64_t long_long_hash(const void* value);
uint64_t float_hash(const void* value);
uint64_t double_hash(const void* value);
uint64_t char_hash(const void* value);
uint64_t str_hash(const void* value);

// ==========================================================
// COMPARE GENERIC FUNCTIONS
// ==========================================================

typedef int (*cmp_fn)(const void*, const void*);

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
    size_t _capacity;
    size_t _size;
    uint64_t _hash;
    bool _hash_computed;
    bool _is_local;

    union {
        char _local[STRING_LOCAL_CAPACITY];
        char* _memory;
    } _data;
} String;

void string_reserve(String* string, size_t required_capacity);

String string_create(const char* str);
void string_free(String* string);
String string_copy(const String* string);

String string_empty(void);

const char* string_get_str(const String* string);
size_t string_length(const String* string);
bool string_is_empty(const String* string);

char string_get_char(const String* string, size_t idx);

void string_append_char(String* string, const char chr);
void string_extend(String* a, const String* b);
void string_clear(String* string);

String string_trim(const String* string);
String string_substring(const String* string, size_t start, size_t length);
String string_concatenate(const String* a, const String* b);
void string_to_lower(String* string);
void string_to_upper(String* string);

int string_index_of(const String* a, const String* b);
bool string_contains(const String* a, const String* b);
bool string_starts_with(const String* string, const char* prefix);
bool string_ends_with(const String* string, const char* suffix);

int string_compare(const String* a, const String* b);
bool string_equals(const String* a, const String* b);
uint64_t string_hash(String* string);

void string_foreach(String* string, void (*function)(size_t, char*));

// ==========================================================
// ARRAY LIST
// ==========================================================

typedef struct ArrayList {
    size_t _capacity;
    size_t _element_size;
    size_t _size;

    uint8_t* _swap_buffer;
    uint8_t* _memory;
} ArrayList;

void arraylist_reserve(ArrayList* list, size_t capacity);

ArrayList arraylist_create(size_t element_size, size_t initial_size);
void arraylist_free(ArrayList* list);
ArrayList arraylist_copy(const ArrayList* list);

void arraylist_clear(ArrayList* list);

size_t arraylist_length(const ArrayList* list);

bool arraylist_get(const ArrayList* list, size_t idx, void* out);
bool arraylist_set(ArrayList* list, size_t idx, const void* element);

bool arraylist_insert_index(ArrayList* list, size_t idx, const void* element);
bool arraylist_remove_index(ArrayList* list, size_t idx, void* out);

void arraylist_append(ArrayList* list, const void* element);
bool arraylist_pop(ArrayList* list, void* out);

bool arraylist_swap(ArrayList* list, size_t i, size_t j);

void arraylist_shuffle(ArrayList* list);

void arraylist_sort(ArrayList* list, cmp_fn cmp);
int arraylist_bsearch(const ArrayList* list, const void* element, cmp_fn cmp);
void arraylist_foreach(ArrayList* list, void (*function)(size_t, void*));

// ==========================================================
// ARRAY DEQUE
// ==========================================================

typedef struct ArrayDeque {
    size_t _capacity;
    size_t _mask;

    size_t _element_size;
    size_t _size;

    size_t _head;
    size_t _tail;

    uint8_t* _memory;
} ArrayDeque;

void arraydeque_reserve(ArrayDeque* deque, size_t capacity);

ArrayDeque arraydeque_create(size_t element_size);
void arraydeque_free(ArrayDeque* deque);

ArrayDeque arraydeque_copy(const ArrayDeque* deque);
void arraydeque_clear(ArrayDeque* deque);

size_t arraydeque_length(const ArrayDeque* deque);

bool arraydeque_get(const ArrayDeque* deque, size_t idx, void* out);

bool arraydeque_peek(const ArrayDeque* deque, void* out);
bool arraydeque_peekleft(const ArrayDeque* deque, void* out);

void arraydeque_append(ArrayDeque* deque, const void* element);
void arraydeque_appendleft(ArrayDeque* deque, const void* element);
bool arraydeque_pop(ArrayDeque* deque, void* out);
bool arraydeque_popleft(ArrayDeque* deque, void* out);

void arraydeque_foreach(ArrayDeque* deque, void (*function)(size_t, void*));

// ==========================================================
// HEAP
// ==========================================================

typedef struct Heap {
    size_t _capacity;
    size_t _element_size;

    cmp_fn _cmp;

    size_t _size;
    uint8_t* _memory;
} Heap;

void heap_reserve(Heap* heap, size_t capacity);

Heap heap_create(size_t element_size, cmp_fn cmp);
void heap_free(Heap* heap);

Heap heap_copy(const Heap* heap);
void heap_clear(Heap* heap);

size_t heap_length(const Heap* heap);

void heap_push(Heap* heap, const void* element);
bool heap_pop(Heap* heap, void* out);

bool heap_peek(Heap* heap, void* out);

void heap_foreach(const Heap* heap, void (*function)(size_t, const void*));

// ==========================================================
// HASH MAP
// ==========================================================

typedef struct HashMap {
    size_t _capacity;
    size_t _mask;
    size_t _max_elements;

    equals_fn _equals;
    hash_fn _hash;

    size_t _key_size;
    size_t _value_size;
    size_t _size;

    uint8_t* _states;
    uint64_t* _hash_cache;
    uint8_t* _keys;
    uint8_t* _values;
} HashMap;

void hashmap_reserve(HashMap* map, size_t capacity);

HashMap hashmap_create(size_t key_size, size_t value_size, equals_fn equals, hash_fn hash);
void hashmap_free(HashMap* map);

HashMap hashmap_copy(const HashMap* map);
void hashmap_clear(HashMap* map);

size_t hashmap_length(const HashMap* map);

void hashmap_set(HashMap* map, const void* key, const void* value, void* old_value);
bool hashmap_get(const HashMap* map, const void* key, void* out);
bool hashmap_remove(HashMap* map, const void* key, void* out);

void hashmap_keys(const HashMap* map, void (*function)(const void*));
void hashmap_values(HashMap* map, void (*function)(void*));
void hashmap_items(HashMap* map, void (*function)(const void*, void*));

// ==========================================================
// HASH SET
// ==========================================================

typedef struct HashSet {
    size_t _capacity;
    size_t _mask;
    size_t _max_elements;

    equals_fn _equals;
    hash_fn _hash;

    size_t _element_size;
    size_t _size;

    uint8_t* _states;
    uint64_t* _hash_cache;
    uint8_t* _elements;
} HashSet;

void hashset_reserve(HashSet* set, size_t capacity);

HashSet hashset_create(size_t element_size, equals_fn equals, hash_fn hash);
void hashset_free(HashSet* set);

HashSet hashset_copy(const HashSet* set);
void hashset_clear(HashSet* set);

size_t hashset_length(const HashSet* set);

bool hashset_add(HashSet* set, const void* element);
bool hashset_has(const HashSet* set, const void* element);
bool hashset_remove(HashSet* set, const void* element);

HashSet hashset_union(const HashSet* a, const HashSet* b);
HashSet hashset_intersection(const HashSet* a, const HashSet* b);
HashSet hashset_difference(const HashSet* a, const HashSet* b);

void hashset_foreach(const HashSet* set, void (*function)(const void*));

// ==========================================================
// FROZEN HASH SET
// ==========================================================

typedef struct FrozenHashSet {
    HashSet _set;
    bool _checksum_computed;
    uint64_t _checksum;
} FrozenHashSet;

FrozenHashSet frozenhashset_empty(size_t element_size, equals_fn equals, hash_fn hash);

FrozenHashSet frozenhashset_create(const HashSet* set);
void frozenhashset_free(FrozenHashSet* f_set);

FrozenHashSet frozenhashset_copy(const FrozenHashSet* f_set);

size_t frozenhashset_length(const FrozenHashSet* f_set);

bool frozenhashset_has(const FrozenHashSet* f_set, const void* element);
bool frozenhashset_equals(const FrozenHashSet* a, const FrozenHashSet* b);
uint64_t frozenhashset_hash(FrozenHashSet* f_set);

FrozenHashSet frozenhashset_union(const FrozenHashSet* a, const FrozenHashSet* b);
FrozenHashSet frozenhashset_intersection(const FrozenHashSet* a, const FrozenHashSet* b);
FrozenHashSet frozenhashset_difference(const FrozenHashSet* a, const FrozenHashSet* b);

FrozenHashSet frozenhashset_union_element(const FrozenHashSet* f_set, const void* element);

void frozenhashset_foreach(const FrozenHashSet* f_set, void (*function)(const void*));

// ==========================================================
// TRIE
// ==========================================================

typedef struct TrieNode {
    unsigned _is_word : 1;
    size_t _parent;
    size_t _children;
} TrieNode;

typedef struct Trie {
    size_t _capacity;
    size_t _used;
    size_t _size;
    TrieNode* _nodes;  // root = 0
} Trie;

Trie trie_create();
void trie_free(Trie* trie);

Trie trie_copy(const Trie* trie);
void trie_clear(Trie* trie);

void trie_add_str(Trie* trie, const char* str);
void trie_add_string(Trie* trie, const String* string);

size_t trie_length(const Trie* trie);

bool trie_has_str(const Trie* trie, const char* str);
bool trie_has_string(const Trie* trie, const String* string);

bool trie_has_prefix_str(const Trie* trie, const char* str);
bool trie_has_prefix_string(const Trie* trie, const String* string);

// ==========================================================
// OBJECT POOL
// ==========================================================

typedef struct ObjectPool {
    size_t _capacity;
    size_t _obj_size;
    size_t _used;

    size_t _head;
    uint8_t* _memory;
} ObjectPool;

ObjectPool objpool_create(size_t max_capacity, size_t obj_size);
void objpool_free(ObjectPool* pool);

size_t objpool_available(const ObjectPool* pool);

void* objpool_malloc(ObjectPool* pool);
void objpool_free_obj(ObjectPool* pool, void* obj);

// ==========================================================
// ARENA POOL
// ==========================================================

typedef struct ArenaPool {
    size_t _capacity;
    size_t _used;

    uint8_t* _memory;
} ArenaPool;

ArenaPool arenapool_create(size_t total_bytes);
void arenapool_free(ArenaPool* arena);

size_t arenapool_available(const ArenaPool* arena);

void* arenapool_malloc(ArenaPool* arena, size_t byte_count);

// ==========================================================
// BYTE ARRAY
// ==========================================================

typedef struct ByteArray {
    size_t _capacity;
    size_t _size;
    size_t _cursor;
    uint8_t* _data;
} ByteArray;

ByteArray bytearray_create(size_t initial_capacity);
void bytearray_free(ByteArray* bytearray);

ByteArray bytearray_copy(const ByteArray* bytearray);
void bytearray_clear(ByteArray* bytearray);

void bytearray_reserve(ByteArray* bytearray, size_t new_capacity);
void bytearray_write_bytes(ByteArray* bytearray, const void* bytes, size_t size);
size_t bytearray_read_bytes(ByteArray* bytearray, void* out, size_t size);

bool bytearray_seek(ByteArray* bytearray, size_t offset);
size_t bytearray_available(const ByteArray* bytearray);

// ==========================================================
// BIT SET
// ==========================================================

typedef struct BitSet {
    size_t _size;
    uint64_t _hash;
    bool _hash_computed;
    uint64_t* _data;
} BitSet;

BitSet bitset_create(size_t size);
void bitset_free(BitSet* bitset);

BitSet bitset_copy(const BitSet* bitset);

void bitset_clear(BitSet* bitset);
void bitset_fill(BitSet* bitset, bool value);

uint64_t bistet_hash(BitSet* bitset);

size_t bitset_count(const BitSet* bitset);

void bitset_set(BitSet* bitset, size_t bit);
bool bitset_get(const BitSet* bitset, size_t bit);
void bitset_flip(BitSet* bitset, size_t bit);
void bitset_reset(BitSet* bitset, size_t bit);

BitSet bitset_and(const BitSet* a, const BitSet* b);
BitSet bitset_or(const BitSet* a, const BitSet* b);
BitSet bitset_xor(const BitSet* a, const BitSet* b);
BitSet bitset_not(const BitSet* a);

bool bitset_any(const BitSet* bitset);
bool bitset_all(const BitSet* bitset);
bool bitset_none(const BitSet* bitset);

int bitset_first(const BitSet* bitset);
int bitset_last(const BitSet* bitset);

// ==========================================================
// INDEXED PRIORITY QUEUE
// ==========================================================

typedef struct IndexedPriorityQueue {
    size_t _element_size;
    size_t _size;
    size_t _priority_count;
    BitSet _min_bucket_reference;
    ArrayDeque* _buckets;
} IndexedPriorityQueue;

IndexedPriorityQueue indexedpq_create(size_t element_size, size_t priority_count);
void indexedpq_free(IndexedPriorityQueue* ipq);

IndexedPriorityQueue indexedpq_copy(const IndexedPriorityQueue* ipq);
void indexedpq_clear(IndexedPriorityQueue* ipq);

bool indexedpq_enqueue(IndexedPriorityQueue* ipq, size_t priority, const void* element);
bool indexedpq_dequeue(IndexedPriorityQueue* ipq, void* out, size_t* priority_out);

bool indexedpq_peek(IndexedPriorityQueue* ipq, void* out, size_t* priority_out);

size_t indexedpq_length(const IndexedPriorityQueue* ipq);

#endif