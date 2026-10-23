#ifndef COLLECTIONS_H
#define COLLECTIONS_H

// ==========================================================
// ARRAY LIST
// ==========================================================

typedef struct ArrayList {
    int _capacity;
    int _element_size;
    int _size;

    unsigned char* _memory;
} ArrayList;

ArrayList arraylist_create(int element_size, int initial_size);
void arraylist_free(ArrayList list);

int arraylist_get(ArrayList* list, int idx, void* out);
int arraylist_set(ArrayList* list, int idx, void* element);

void arraylist_append(ArrayList* list, void* element);
int arraylist_pop(ArrayList* list, void* out);

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
void arraydeque_free(ArrayDeque deque);

void arraydeque_append(ArrayDeque* deque, void* element);
void arraydeque_appendleft(ArrayDeque* deque, void* element);
int arraydeque_pop(ArrayDeque* deque, void* out);
int arraydeque_popleft(ArrayDeque* deque, void* out);

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
void heap_free(Heap heap);

void heap_push(Heap* heap, void* element);
int heap_pop(Heap* heap, void* out);

// ==========================================================
// HASH MAP
// ==========================================================

typedef struct HashMap {
    int _capacity;
    int _mask;
    int _max_elements;

    int (*_equals)(const void*, const void*);
    unsigned int (*_hash)(const void*); 

    int _key_size;
    int _value_size;
    int _size;

    unsigned char* _states;
    unsigned int* _hash_cache;
    unsigned char* _keys;
    unsigned char* _values;
} HashMap;

HashMap hashmap_create(
    int key_size,
    int value_size,
    int (*equals)(const void*, const void*),
    unsigned int (*hash)(const void*)
);
void hashmap_free(HashMap map);

int hashmap_set(HashMap* map, const void* key, const void* value, void* old_value);
int hashmap_get(HashMap* map, const void* key, void* out);
int hashmap_remove(HashMap* map, const void* key, void* out);

// ==========================================================
// HASH SET
// ==========================================================

typedef struct HashSet {
    int _capacity;
    int _mask;
    int _max_elements;

    int (*_equals)(const void*, const void*);
    unsigned int (*_hash)(const void*); 

    int _element_size;
    int _size;

    unsigned char* _states;
    unsigned int* _hash_cache;
    unsigned char* _elements;
} HashSet;

HashSet hashset_create(
    int element_size,
    int (*equals)(const void*, const void*),
    unsigned int (*hash)(const void*)
);
void hashset_free(HashSet set);

void hashset_add(HashSet* set, const void* element);
int hashset_has(HashSet* set, const void* element);
int hashset_remove(HashSet* set, const void* element);

#endif