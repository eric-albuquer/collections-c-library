#include "collections.h"
#include <stdlib.h>
#include <string.h>

#define START_CAPACITY 64

// ==========================================================
// ARRAY LIST
// ==========================================================

ArrayList arraylist_create(int element_size, int initial_size) {
    initial_size = initial_size < START_CAPACITY ? START_CAPACITY : initial_size;
    ArrayList list = {
        ._capacity = initial_size,
        ._element_size = element_size,
        ._size = 0,
        ._memory = malloc(initial_size * element_size)
    };

    return list;
}

void arraylist_free(ArrayList list) {
    free(list._memory);
}

int arraylist_get(ArrayList* list, int idx, void* out) {
    if ((unsigned int)idx >= list->_size)
        return 0;
    memcpy(out, list->_memory + idx * list->_element_size, list->_element_size);
    return 1;
}

int arraylist_set(ArrayList* list, int idx, void* element) {
    if ((unsigned int)idx >= list->_size)
        return 0;
    memcpy(list->_memory + idx * list->_element_size, element, list->_element_size);
    return 1;
}

void arraylist_append(ArrayList* list, void* element) {
    if (list->_size == list->_capacity) {
        list->_capacity <<= 1;
        list->_memory = realloc(list->_memory, list->_capacity * list->_element_size);
    }

    memcpy(list->_memory + list->_size * list->_element_size, element, list->_element_size);
    list->_size++;
}
int arraylist_pop(ArrayList* list, void* out) {
    if (list->_size == 0) 
        return 0;

    list->_size--;
    memcpy(out, list->_memory + list->_size * list->_element_size, list->_element_size);
    return 1;
}

void arraylist_sort(ArrayList* list, int(*cmp)(const void*, const void*)) {
    qsort(list->_memory, list->_size, list->_element_size, cmp);
}

void arraylist_map(ArrayList* list, void(*function)(int, const void*)) {
    for (int i = 0; i < list->_size; i++){
        function(i, list->_memory + i * list->_element_size);   
    }
}

// ==========================================================
// ARRAY DEQUE
// ==========================================================

static inline void arraydeque_resize(ArrayDeque* deque) {
    deque->_capacity <<= 1;
    deque->_mask = deque->_capacity - 1;

    unsigned char* old_memory = deque->_memory;
    deque->_memory = malloc(deque->_element_size * deque->_capacity);

    memcpy(
        deque->_memory,
        old_memory + deque->_head * deque->_element_size,
        deque->_element_size * (deque->_size - deque->_head)
    );

    memcpy(
        deque->_memory + (deque->_size - deque->_head) * deque->_element_size,
        old_memory,
        deque->_head * deque->_element_size
    );

    free(old_memory);
    deque->_head = 0;
    deque->_tail = deque->_size;
}

ArrayDeque arraydeque_create(int element_size) {
    ArrayDeque deque = {
        ._capacity = START_CAPACITY,
        ._mask = START_CAPACITY - 1,
        ._element_size = element_size,

        ._size = 0,
        ._head = 0,
        ._tail = 0,
        
        ._memory = malloc(element_size * START_CAPACITY)
    };

    return deque;
}

void arraydeque_free(ArrayDeque deque) {
    free(deque._memory);
}

void arraydeque_append(ArrayDeque* deque, void* element) {
    if (deque->_size == deque->_capacity)
        arraydeque_resize(deque);

    memcpy(
        deque->_memory + deque->_tail * deque->_element_size,
        element,
        deque->_element_size
    );

    deque->_tail = (deque->_tail + 1) & deque->_mask;
    deque->_size++;
}

void arraydeque_appendleft(ArrayDeque* deque, void* element) {
    if (deque->_size == deque->_capacity)
        arraydeque_resize(deque);

    deque->_head = (deque->_head - 1 + deque->_capacity) & deque->_mask;

    memcpy(
        deque->_memory + deque->_head * deque->_element_size,
        element,
        deque->_element_size
    );
    
    deque->_size++;
}

int arraydeque_pop(ArrayDeque* deque, void* out) {
    if (deque->_size == 0) 
        return 0;

    deque->_tail = (deque->_tail - 1 + deque->_capacity) & deque->_mask;

    memcpy(
        out,
        deque->_memory + deque->_tail * deque->_element_size,
        deque->_element_size
    );

    deque->_size--;
    return 1;
}

int arraydeque_popleft(ArrayDeque* deque, void* out) {
    if (deque->_size == 0) 
        return 0;

    memcpy(
        out,
        deque->_memory + deque->_head * deque->_element_size,
        deque->_element_size
    );

    deque->_head = (deque->_head + 1) & deque->_mask;
    deque->_size--;
    return 1;
}

// ==========================================================
// HEAP
// ==========================================================

static inline void heap_resize(Heap* heap) {
    heap->_capacity <<= 1;
    heap->_memory = realloc(heap->_memory, heap->_capacity * heap->_element_size);
}

Heap heap_create(int element_size, int (*cmp)(const void*, const void*)) {
    Heap heap = {
        ._capacity = START_CAPACITY,
        ._element_size = element_size,
        ._cmp = cmp,
        ._size = 0,
        ._memory = malloc(element_size * START_CAPACITY)
    };

    return heap;
}

void heap_free(Heap heap) {
    free(heap._memory);
}

void heap_push(Heap* heap, void* element) {
    if (heap->_size == heap->_capacity)
        heap_resize(heap);

    int i = heap->_size;
    while (i > 0) {
        const int p = (i - 1) >> 1;
        const void* p_ptr = heap->_memory + p * heap->_element_size;
        if (heap->_cmp(p_ptr, element) <= 0)
            break;
        memcpy(
            heap->_memory + i * heap->_element_size,
            p_ptr,
            heap->_element_size
        );
        i = p;
    }

    memcpy(
        heap->_memory + i * heap->_element_size,
        element,
        heap->_element_size
    );
    heap->_size++;
}

int heap_pop(Heap* heap, void* out) {
    if (heap->_size == 0)
        return 0; 

    memcpy(
        out,
        heap->_memory,
        heap->_element_size
    );
    heap->_size--;

    if (heap->_size == 0)
        return 1;

    const void* value = heap->_memory + heap->_size * heap->_element_size;
    int i = 0;

    while (1) {
        const int l = (i << 1) + 1;
        const int r = (i << 1) + 2;

        int m = i;
        const void* min_ptr = value;

        if (l < heap->_size) {
            const void* l_ptr = heap->_memory + l * heap->_element_size;
            if (heap->_cmp(l_ptr, min_ptr) < 0) {
                min_ptr = l_ptr;
                m = l;
            }
        }

        if (r < heap->_size) {
            const void* r_ptr = heap->_memory + r * heap->_element_size;
            if (heap->_cmp(r_ptr, min_ptr) < 0) {
                min_ptr = r_ptr;
                m = r;
            }
        }

        if (m == i)
            break;

        memcpy(
            heap->_memory + i * heap->_element_size,
            min_ptr,
            heap->_element_size
        );

        i = m;
    }

    memcpy(
        heap->_memory + i * heap->_element_size,
        value,
        heap->_element_size
    );
    return 1;
}

// ==========================================================
// HASH MAP
// ==========================================================

#define HASHMAP_FACTOR 0.75f
#define EMPTY 0
#define REMOVED 1
#define USED 2

static inline unsigned int double_hash(unsigned int hash) {
    return (hash * 2654435761U) | 1;
}

static inline void hashmap_resize(HashMap* map) {
    int new_capacity = map->_capacity << 1;
    int new_mask = new_capacity - 1;

    unsigned int* new_hash_cache = malloc(sizeof(unsigned int) * new_capacity);
    unsigned char* new_states = calloc(new_capacity, sizeof(unsigned char));
    unsigned char* new_keys = malloc(map->_key_size * new_capacity);
    unsigned char* new_values = malloc(map->_value_size * new_capacity);

    for (int i = 0; i < map->_capacity; i++){
        if (map->_states[i] != USED) 
            continue;
        
        unsigned int hash = map->_hash_cache[i];
        unsigned int step = double_hash(hash);

        unsigned int idx = hash & new_mask;

        for (int j = 0; j < new_capacity; j++){
            if (new_states[idx] == EMPTY) {
                new_states[idx] = USED;
                new_hash_cache[idx] = hash;
                memcpy(
                    new_keys + idx * map->_key_size,
                    map->_keys + i * map->_key_size,
                    map->_key_size
                );
                memcpy(
                    new_values + idx * map->_value_size,
                    map->_values + i * map->_value_size,
                    map->_value_size
                );
                break;
            }
            idx = (idx + step) & new_mask;
        }
    }

    map->_capacity = new_capacity;
    map->_mask = new_mask;
    map->_max_elements = new_capacity * HASHMAP_FACTOR;
    free(map->_states);
    free(map->_hash_cache);
    free(map->_keys);
    free(map->_values);
    map->_states = new_states;
    map->_hash_cache = new_hash_cache;
    map->_keys = new_keys;
    map->_values = new_values;
}

HashMap hashmap_create(
    int key_size,
    int value_size,
    int (*equals)(const void*, const void*),
    unsigned int (*hash)(const void*)
) {
    HashMap map = {
        ._capacity = START_CAPACITY,
        ._mask = START_CAPACITY - 1,
        ._max_elements = START_CAPACITY * HASHMAP_FACTOR,
        ._key_size = key_size,
        ._value_size = value_size,
        ._equals = equals,
        ._hash = hash,
        ._size = 0,
        ._hash_cache = malloc(sizeof(unsigned int) * START_CAPACITY),
        ._states = calloc(START_CAPACITY, sizeof(unsigned char)),
        ._keys = malloc(START_CAPACITY * key_size),
        ._values = malloc(START_CAPACITY * value_size)
    };

    return map;
}

void hashmap_free(HashMap map) {
    free(map._states);
    free(map._keys);
    free(map._values);
    free(map._hash_cache);
}

int hashmap_set(HashMap* map, const void* key, const void* value, void* old_value) {
    if (map->_size == map->_max_elements)
        hashmap_resize(map);

    unsigned int hash = map->_hash(key);
    unsigned int step = double_hash(hash);

    unsigned int idx = hash & map->_mask;
    int location = -1;

    for (int i = 0; i < map->_capacity; i++){
        if (map->_states[idx] != USED && location == -1) {
            location = idx;
        } else if (map->_states[idx] == USED && map->_hash_cache[idx] == hash && map->_equals(map->_keys + map->_key_size * idx, key)) {
            void* ptr = map->_values + idx * map->_value_size;
            if (old_value)
                memcpy(old_value, ptr, map->_value_size);

            memcpy(ptr, value, map->_value_size);
            return 1;
        }
        if (map->_states[idx] == EMPTY)
            break;
        idx = (idx + step) & map->_mask;
    }
    
    memcpy(map->_keys + location * map->_key_size, key, map->_key_size);
    memcpy(map->_values + location * map->_value_size, value, map->_value_size);
    map->_hash_cache[location] = hash;
    map->_states[location] = USED;
    map->_size++;
    return 0;
}

int hashmap_get(HashMap* map, const void* key, void* out) {
    unsigned int hash = map->_hash(key);
    unsigned int step = double_hash(hash);

    unsigned int idx = hash & map->_mask;

    for (int i = 0; i < map->_capacity; i++){
        if (map->_states[idx] == EMPTY)
            return 0;
        if (map->_states[idx] == USED && map->_hash_cache[idx] == hash && map->_equals(map->_keys + idx * map->_key_size, key)) {
            memcpy(
                out,
                map->_values + idx * map->_value_size,
                map->_value_size
            );
            return 1;
        }
        idx = (idx + step) & map->_mask;
    }
    return 0;
}

int hashmap_remove(HashMap* map, const void* key, void* out) {
    unsigned int hash = map->_hash(key);
    unsigned int step = double_hash(hash);

    unsigned int idx = hash & map->_mask;

    for (int i = 0; i < map->_capacity; i++){
        if (map->_states[idx] == EMPTY)
            return 0;
        if (map->_states[idx] == USED && map->_hash_cache[idx] == hash && map->_equals(map->_keys + idx * map->_key_size, key)) {
            memcpy(
                out,
                map->_values + idx * map->_value_size,
                map->_value_size
            );
            map->_states[idx] = REMOVED;
            map->_size--;
            return 1;
        }
        idx = (idx + step) & map->_mask;
    }
    return 0;
}

static inline void hashset_resize(HashSet* set) {
    int new_capacity = set->_capacity << 1;
    int new_mask = new_capacity - 1;

    unsigned int* new_hash_cache = malloc(sizeof(unsigned int) * new_capacity);
    unsigned char* new_states = calloc(new_capacity, sizeof(unsigned char));
    unsigned char* new_elements = malloc(set->_element_size * new_capacity);

    for (int i = 0; i < set->_capacity; i++){
        if (set->_states[i] != USED) 
            continue;
        
        unsigned int hash = set->_hash_cache[i];
        unsigned int step = double_hash(hash);

        unsigned int idx = hash & new_mask;

        for (int j = 0; j < new_capacity; j++){
            if (new_states[idx] == EMPTY) {
                new_states[idx] = USED;
                new_hash_cache[idx] = hash;
                memcpy(
                    new_elements + idx * set->_element_size,
                    set->_elements + i * set->_element_size,
                    set->_element_size
                );
                break;
            }
            idx = (idx + step) & new_mask;
        }
    }

    set->_capacity = new_capacity;
    set->_mask = new_mask;
    set->_max_elements = new_capacity * HASHMAP_FACTOR;
    free(set->_states);
    free(set->_hash_cache);
    free(set->_elements);
    set->_states = new_states;
    set->_hash_cache = new_hash_cache;
    set->_elements = new_elements;
}

HashSet hashset_create(
    int element_size,
    int (*equals)(const void*, const void*),
    unsigned int (*hash)(const void*)
) {
    HashSet set = {
        ._capacity = START_CAPACITY,
        ._mask = START_CAPACITY - 1,
        ._max_elements = START_CAPACITY * HASHMAP_FACTOR,
        ._element_size = element_size,
        ._equals = equals,
        ._hash = hash,
        ._size = 0,
        ._hash_cache = malloc(sizeof(unsigned int) * START_CAPACITY),
        ._states = calloc(START_CAPACITY, sizeof(unsigned char)),
        ._elements = malloc(element_size * START_CAPACITY)
    };

    return set;
}

void hashset_free(HashSet set) {
    free(set._states);
    free(set._hash_cache);
    free(set._elements);
}

void hashset_add(HashSet* set, const void* element) {
    if (set->_size == set->_max_elements)
        hashset_resize(set);

    unsigned int hash = set->_hash(element);
    unsigned int step = double_hash(hash);

    unsigned int idx = hash & set->_mask;
    int location = -1;

    for (int i = 0; i < set->_capacity; i++){
        if (set->_states[idx] != USED && location == -1) {
            location = idx;
        } else if (set->_states[idx] == USED && set->_hash_cache[idx] == hash && set->_equals(set->_elements + set->_element_size * idx, element)) {
            return;
        }
        if (set->_states[idx] == EMPTY)
            break;
        idx = (idx + step) & set->_mask;
    }
    
    memcpy(set->_elements + location * set->_element_size, element, set->_element_size);
    set->_hash_cache[location] = hash;
    set->_states[location] = USED;
    set->_size++;
}

int hashset_has(HashSet* set, const void* element) {
    unsigned int hash = set->_hash(element);
    unsigned int step = double_hash(hash);

    unsigned int idx = hash & set->_mask;

    for (int i = 0; i < set->_capacity; i++){
        if (set->_states[idx] == EMPTY)
            return 0;
        if (set->_states[idx] == USED && set->_hash_cache[idx] == hash && set->_equals(set->_elements + idx * set->_element_size, element)) {
            return 1;
        }
        idx = (idx + step) & set->_mask;
    }
    return 0;
}

int hashset_remove(HashSet* set, const void* element) {
    unsigned int hash = set->_hash(element);
    unsigned int step = double_hash(hash);

    unsigned int idx = hash & set->_mask;

    for (int i = 0; i < set->_capacity; i++){
        if (set->_states[idx] == EMPTY)
            return 0;
        if (set->_states[idx] == USED && set->_hash_cache[idx] == hash && set->_equals(set->_elements + idx * set->_element_size, element)) {
            set->_states[idx] = REMOVED;
            set->_size--;
            return 1;
        }
        idx = (idx + step) & set->_mask;
    }
    return 0;
}