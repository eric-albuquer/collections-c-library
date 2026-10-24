#include "collections.h"
#include <stdlib.h>
#include <string.h>

#define START_CAPACITY 64

// ==========================================================
// EQUAL GENERIC FUNCTIONS
// ==========================================================

bool int_equals(const void* a, const void* b) {
    if (a == b) return 1;
    if (a == NULL || b == NULL) return 0;
    return *(const int*)a == *(const int*)b;
}

bool long_long_equals(const void* a, const void* b) {
    if (a == b) return 1;
    if (a == NULL || b == NULL) return 0;
    return *(const long long*)a == *(const long long*)b;
}

bool float_equals(const void* a, const void* b) {
    if (a == b) return 1;
    if (a == NULL || b == NULL) return 0;
    return *(const float*)a == *(const float*)b;
}
bool double_equals(const void* a, const void* b) {
    if (a == b) return 1;
    if (a == NULL || b == NULL) return 0;
    return *(const double*)a == *(const double*)b;
}

bool char_equals(const void* a, const void* b) {
    if (a == b) return 1;
    if (a == NULL || b == NULL) return 0;
    return *(const char*)a == *(const char*)b;
}

bool str_equals(const void* a, const void* b) {
    if (a == b) return 1;
    if (a == NULL || b == NULL) return 0;
    return strcmp((const char*)a, (const char*)b) == 0;
}

// ==========================================================
// HASH GENERIC FUNCTIONS
// ==========================================================

uint64_t char_hash(const void* value) {
    if (value == NULL) return 0;
    return (uint64_t)(unsigned char)(*(const char*)value);
}

uint64_t long_long_hash(const void* value) {
    if (value == NULL) return 0;
    long long val = *(const long long*)value;
    uint64_t x = (uint64_t)val;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

uint64_t int_hash(const void* value) {
    if (value == NULL) return 0;
    int val = *(const int*)value;
    uint64_t x = (uint32_t)val;
    x = (x ^ (x >> 15)) * 0x2efa61cb23f12ee7ULL;
    x = (x ^ (x >> 13)) * 0x123456789abcdef0ULL;
    return x ^ (x >> 17);
}

uint64_t float_hash(const void* value) {
    if (value == NULL) return 0;
    float val = *(const float*)value;
    union {
        float f;
        uint32_t i;
    } u;
    u.f = val;
    uint64_t x = u.i;
    x = (x ^ (x >> 15)) * 0x2efa61cb23f12ee7ULL;
    x = (x ^ (x >> 13)) * 0x123456789abcdef0ULL;
    return x ^ (x >> 17);
}

uint64_t double_hash(const void* value) {
    if (value == NULL) return 0;
    double val = *(const double*)value;
    union {
        double d;
        uint64_t i;
    } u;
    u.d = val;
    uint64_t x = u.i;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    return x ^ (x >> 31);
}

uint64_t str_hash(const void* value) {
    if (value == NULL) return 0;
    const char* str = (const char*)value;
    
    uint64_t hash = 14695981039346656037ULL;
    int c;

    while ((c = (unsigned char)*str++)) {
        hash ^= c;
        hash *= 1099511628211ULL;
    }

    return hash;
}

// ==========================================================
// COMPARE GENERIC FUNCTIONS
// ==========================================================

int int_cmp(const void* a, const void* b) {
    if (a == b) return 0;
    if (a == NULL) return -1;
    if (b == NULL) return 1;

    int val_a = *(const int*)a;
    int val_b = *(const int*)b;

    if (val_a < val_b) return -1;
    if (val_a > val_b) return 1;
    return 0;
}

int long_long_cmp(const void* a, const void* b) {
    if (a == b) return 0;
    if (a == NULL) return -1;
    if (b == NULL) return 1;

    long long val_a = *(const long long*)a;
    long long val_b = *(const long long*)b;

    if (val_a < val_b) return -1;
    if (val_a > val_b) return 1;
    return 0;
}

int float_cmp(const void* a, const void* b) {
    if (a == b) return 0;
    if (a == NULL) return -1;
    if (b == NULL) return 1;

    float val_a = *(const float*)a;
    float val_b = *(const float*)b;

    if (val_a < val_b) return -1;
    if (val_a > val_b) return 1;
    return 0;
}

int double_cmp(const void* a, const void* b) {
    if (a == b) return 0;
    if (a == NULL) return -1;
    if (b == NULL) return 1;

    double val_a = *(const double*)a;
    double val_b = *(const double*)b;

    if (val_a < val_b) return -1;
    if (val_a > val_b) return 1;
    return 0;
}

int char_cmp(const void* a, const void* b) {
    if (a == b) return 0;
    if (a == NULL) return -1;
    if (b == NULL) return 1;

    char val_a = *(const char*)a;
    char val_b = *(const char*)b;

    return (int)val_a - (int)val_b;
}

int str_cmp(const void* a, const void* b) {
    if (a == b) return 0;
    if (a == NULL) return -1;
    if (b == NULL) return 1;

    return strcmp((const char*)a, (const char*)b);
}

// ==========================================================
// MIN MAX FUNCTIONS
// ==========================================================

int int_min(int a, int b) {
    return (a < b) ? a : b;
}

int int_max(int a, int b) {
    return (a > b) ? a : b;
}

long long long_long_min(long long a, long long b) {
    return (a < b) ? a : b;
}

long long long_long_max(long long a, long long b) {
    return (a > b) ? a : b;
}

float float_min(float a, float b) {
    return (a < b) ? a : b;
}

float float_max(float a, float b) {
    return (a > b) ? a : b;
}

double double_min(double a, double b) {
    return (a < b) ? a : b;
}

double double_max(double a, double b) {
    return (a > b) ? a : b;
}

char char_min(char a, char b) {
    return (a < b) ? a : b;
}

char char_max(char a, char b) {
    return (a > b) ? a : b;
}

// ==========================================================
// STRING
// ==========================================================

#define ALPHABET_SIZE 256

static inline void string_resize(String* s) {
    s->_capacity <<= 1;
    s->_memory = realloc(s->_memory, s->_capacity);
}

String string_create(char* str) {
    String string = {
        ._capacity = START_CAPACITY,
        ._hash_computed = 0,
        ._size = 0,
        ._memory = malloc(START_CAPACITY),
    };

    int idx = 0;
    char c;
    do {
        if (idx == string._capacity)
            string_resize(&string);

        c = str[idx];
        string._memory[idx] = c;
        idx++;
    } while (c != 0);
    string._size = idx;

    return string;
}

void string_free(String* string) {
    free(string->_memory);
    string->_memory = NULL;
}

char* string_get_str(String* string) {
    return string->_memory;
}

int string_length(String* string) {
    return string->_size - 1;
}

void string_append_char(String* string, char chr) {
    string->_hash_computed = false;
    if (string->_size == string->_capacity)
        string_resize(string);
    
    string->_memory[string->_size - 1] = chr;
    string->_memory[string->_size] = 0;
    string->_size++;
}

void string_clear(String* string) {
    string->_hash_computed = false;
    string->_memory[0] = 0;
    string->_size = 1;
}

String string_copy(String* string) {
    String copy = string_create(string->_memory);
    if (string->_hash_computed) {
        copy._hash = string->_hash;
        copy._hash_computed = 1;
    }
        
    return copy;
}

int string_compare(String* a, String* b) {
    return strcmp(a->_memory, b->_memory);
}

void string_append(String* a, String* b) {
    a->_hash_computed = false;
    int idx = --a->_size;
    for (int i = 0; i < b->_size; i++){
        if (idx == a->_capacity)
            string_resize(a);
        
        a->_memory[idx] = b->_memory[i];
        idx++;
    }
    a->_size += b->_size;
}

static inline void build_shift_table(const char* pattern, int m, int* bad_char) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        bad_char[i] = m;
    }

    for (int i = 0; i < m - 1; i++) {
        unsigned char c = (unsigned char)pattern[i];
        bad_char[c] = m - 1 - i;
    }
}

int string_contains(String* a, String* b) {
    int n = a->_size - 1;
    int m = b->_size - 1;

    if (m == 0) return 0; 
    if (n < m) return -1;   

    int bad_char[ALPHABET_SIZE];
    build_shift_table(b->_memory, m, bad_char);

    int i = 0;

    while (i <= n - m) {
        int j = m - 1; 

        while (j >= 0 && b->_memory[j] == a->_memory[i + j]) {
            j--;
        }

        if (j < 0) {
            return i;
        }

        unsigned char mismatched_char = (unsigned char)a->_memory[i + m - 1];
        i += bad_char[mismatched_char];
    }

    return -1;
}

bool string_equals(String* a, String* b) {
    if (a->_hash_computed && b->_hash_computed && a->_hash != b->_hash)
        return 0;
    return str_equals(a->_memory, b->_memory);
}

uint64_t string_hash(String* string) {
    if (string->_hash_computed)
        return string->_hash;
    string->_hash = str_hash(string->_memory);
    string->_hash_computed = 1;
    return string->_hash;
}

// ==========================================================
// ARRAY LIST
// ==========================================================

static inline void arraylist_resize(ArrayList* list){
    list->_capacity <<= 1;
    list->_memory = realloc(list->_memory, list->_capacity * list->_element_size);
}

ArrayList arraylist_create(int element_size, int initial_capacity) {
    initial_capacity = initial_capacity < START_CAPACITY ? START_CAPACITY : initial_capacity;
    ArrayList list = {
        ._capacity = initial_capacity,
        ._element_size = element_size,
        ._size = 0,
        ._swap_buffer = malloc(element_size),
        ._memory = malloc(initial_capacity * element_size)
    };

    return list;
}

void arraylist_free(ArrayList* list) {
    free(list->_swap_buffer);
    free(list->_memory);
    list->_swap_buffer = NULL;
    list->_memory = NULL;
}

int arraylist_length(ArrayList* list) {
    return list->_size;
}

bool arraylist_get(ArrayList* list, int idx, void* out) {
    if ((unsigned int)idx >= list->_size)
        return 0;
    memcpy(out, list->_memory + idx * list->_element_size, list->_element_size);
    return 1;
}

bool arraylist_set(ArrayList* list, int idx, void* element) {
    if ((unsigned int)idx >= list->_size)
        return 0;
    memcpy(list->_memory + idx * list->_element_size, element, list->_element_size);
    return 1;
}

bool arraylist_insert_index(ArrayList* list, int idx, void* element) {
    if ((unsigned int)idx > list->_size)
        return 0;

    if (list->_size == list->_capacity)
        arraylist_resize(list);

    for (int i = list->_size; i > idx; i--){
        memcpy(list->_memory + i * list->_element_size, list->_memory + (i - 1) * list->_element_size, list->_element_size);
    }

    memcpy(list->_memory + idx * list->_element_size, element, list->_element_size);
    list->_size++;
    return 1;
}

bool arraylist_remove_index(ArrayList* list, int idx, void* out) {
    if((unsigned int)idx >= list->_size)
        return 0;

    memcpy(out, list->_memory + idx * list->_element_size, list->_element_size);
    list->_size--;

    for (int i = idx; i < list->_size; i++){
        memcpy(list->_memory + i * list->_element_size, list->_memory + (i + 1) * list->_element_size, list->_element_size);
    }
    
    return 1;
}

void arraylist_clear(ArrayList* list) {
    list->_size = 0;
}

void arraylist_append(ArrayList* list, void* element) {
    if (list->_size == list->_capacity)
        arraylist_resize(list);

    memcpy(list->_memory + list->_size * list->_element_size, element, list->_element_size);
    list->_size++;
}

bool arraylist_pop(ArrayList* list, void* out) {
    if (list->_size == 0) 
        return 0;

    list->_size--;
    memcpy(out, list->_memory + list->_size * list->_element_size, list->_element_size);
    return 1;
}

bool arraylist_swap(ArrayList* list, int i, int j) {
    if ((unsigned int)i >= list->_size || (unsigned int)j >= list->_size || i == j)
        return 0;
    memcpy(list->_swap_buffer, list->_memory + i * list->_element_size, list->_element_size);
    memcpy(list->_memory + i * list->_element_size, list->_memory + j * list->_element_size, list->_element_size);
    memcpy(list->_memory + j * list->_element_size, list->_swap_buffer, list->_element_size);
    return 1;
}

void arraylist_shuffle(ArrayList* list) {
    for (int i = list->_size - 1; i > 0; i--){
        int idx = rand() % (i + 1);
        arraylist_swap(list, i, idx);
    }
}

static inline int calc_min_run(int n) {
    int r = 0;
    while(n >= 64) {
        r |= n & 1;
        n >>= 1;
    }
    return n + r;
}

static inline void insertion_sort(ArrayList* list, int left, int right, int(*cmp)(const void*, const void*)) {
    for (int i = left + 1; i < right + 1; i++){
        memcpy(list->_swap_buffer, list->_memory + i * list->_element_size, list->_element_size);
        int j = i - 1;
        while (j >= left && cmp(list->_memory + j * list->_element_size, list->_swap_buffer) > 0) {
            memcpy(list->_memory + (j + 1) * list->_element_size, list->_memory + j * list->_element_size, list->_element_size);
            j--;
        }
        memcpy(list->_memory + (j + 1) * list->_element_size, list->_swap_buffer, list->_element_size);
    }  
}

static inline void merge(ArrayList* list, unsigned char* aux, int l, int m, int r, int(*cmp)(const void*, const void*)) {
    int i = l;
    int j = m;
    int k = l;

    while (i < m && j < r) {
        if (cmp(aux + i * list->_element_size, aux + j * list->_element_size) <= 0) {
            memcpy(list->_memory + k * list->_element_size, aux + i * list->_element_size, list->_element_size);
            i++;
        } else {
            memcpy(list->_memory + k * list->_element_size, aux + j * list->_element_size, list->_element_size);
            j++;
        }
        k++;
    }

    while (i < m) {
        memcpy(list->_memory + k * list->_element_size, aux + i * list->_element_size, list->_element_size);
        i++;
        k++;
    }

    while (j < r) {
        memcpy(list->_memory + k * list->_element_size, aux + j * list->_element_size, list->_element_size);
        j++;
        k++;
    }
}

void arraylist_sort(ArrayList* list, int(*cmp)(const void*, const void*)) {
    int n = list->_size;
    int min_run = calc_min_run(n);

    for (int i = 0; i < n; i += min_run){
        int end = int_min(i + min_run - 1, n - 1);
        insertion_sort(list, i, end, cmp);
    }

    unsigned char* aux = malloc(list->_element_size * list->_size);
    
    int size = min_run;
    while (size < n) {
        const int double_size = size * 2;
        unsigned char* temp = list->_memory;
        list->_memory = aux;
        aux = temp;
        for (int i = 0; i < n; i += double_size){
            int mid = int_min(i + size - 1, n - 1);
            int end = int_min(i + double_size - 1, n - 1);

            if (mid < end)
                merge(list, aux, i, mid, end, cmp);
        }
        size = double_size;
    }

    free(aux);
    //qsort(list->_memory, list->_size, list->_element_size, cmp);
}

void arraylist_foreach(ArrayList* list, void(*function)(int, const void*)) {
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

void arraydeque_free(ArrayDeque* deque) {
    free(deque->_memory);
    deque->_memory = NULL;
}

int arraydeque_length(ArrayDeque* deque) {
    return deque->_size;
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

bool arraydeque_pop(ArrayDeque* deque, void* out) {
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

bool arraydeque_popleft(ArrayDeque* deque, void* out) {
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

void arraydeque_foreach(ArrayDeque* deque, void(*function)(int, const void*)) {
    int curr = deque->_head;
    for (int i = 0; i < deque->_size; i++){
        function(i, deque->_memory + curr * deque->_element_size);
        curr = (curr + 1) & deque->_mask;   
    }
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

void heap_free(Heap* heap) {
    free(heap->_memory);
    heap->_memory = NULL;
}

int heap_length(Heap* heap) {
    return heap->_size;
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

bool heap_pop(Heap* heap, void* out) {
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

void heap_foreach(Heap* heap, void(*function)(int, const void*)) {
    for (int i = 0; i < heap->_size; i++){
        function(i, heap->_memory + i * heap->_element_size);
    }
}

// ==========================================================
// HASH MAP
// ==========================================================

#define HASHMAP_FACTOR 0.75f
#define EMPTY 0
#define REMOVED 1
#define USED 2

static inline uint64_t double_hash_step(uint64_t hash) {
    return (hash * 0x9e3779b97f4a7c15ULL) | 1;
}

static inline void hashmap_resize(HashMap* map) {
    int new_capacity = map->_capacity << 1;
    int new_mask = new_capacity - 1;

    uint64_t* new_hash_cache = malloc(sizeof(uint64_t) * new_capacity);
    unsigned char* new_states = calloc(new_capacity, sizeof(unsigned char));
    unsigned char* new_keys = malloc(map->_key_size * new_capacity);
    unsigned char* new_values = malloc(map->_value_size * new_capacity);

    for (int i = 0; i < map->_capacity; i++){
        if (map->_states[i] != USED) 
            continue;
        
        uint64_t hash = map->_hash_cache[i];
        uint64_t step = double_hash_step(hash);

        uint64_t idx = hash & new_mask;

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
    bool (*equals)(const void*, const void*),
    uint64_t (*hash)(const void*)
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
        ._hash_cache = malloc(sizeof(uint64_t) * START_CAPACITY),
        ._states = calloc(START_CAPACITY, sizeof(unsigned char)),
        ._keys = malloc(START_CAPACITY * key_size),
        ._values = malloc(START_CAPACITY * value_size)
    };

    return map;
}

void hashmap_free(HashMap* map) {
    free(map->_states);
    free(map->_keys);
    free(map->_values);
    free(map->_hash_cache);
    map->_states = NULL;
    map->_keys = NULL;
    map->_values = NULL;
    map->_hash_cache = NULL;
}

int hashmap_length(HashMap* map) {
    return map->_size;
}

void hashmap_set(HashMap* map, const void* key, const void* value, void* old_value) {
    if (map->_size == map->_max_elements)
        hashmap_resize(map);

    uint64_t hash = map->_hash(key);
    uint64_t step = double_hash_step(hash);

    uint64_t idx = hash & map->_mask;
    int location = -1;

    for (int i = 0; i < map->_capacity; i++){
        if (map->_states[idx] != USED && location == -1) {
            location = idx;
        } else if (map->_states[idx] == USED && map->_hash_cache[idx] == hash && map->_equals(map->_keys + map->_key_size * idx, key)) {
            void* ptr = map->_values + idx * map->_value_size;
            if (old_value)
                memcpy(old_value, ptr, map->_value_size);

            memcpy(ptr, value, map->_value_size);
            return;
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
}

bool hashmap_get(HashMap* map, const void* key, void* out) {
    uint64_t hash = map->_hash(key);
    uint64_t step = double_hash_step(hash);

    uint64_t idx = hash & map->_mask;

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

bool hashmap_remove(HashMap* map, const void* key, void* out) {
    uint64_t hash = map->_hash(key);
    uint64_t step = double_hash_step(hash);

    uint64_t idx = hash & map->_mask;

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

void hashmap_keys(HashMap* map, void (*function)(const void*)) {
    for (int i = 0; i < map->_capacity; i++){
        if (map->_states[i] != USED)
            continue;
        function(map->_keys + i * map->_key_size);
    }
}

void hashmap_values(HashMap* map, void (*function)(const void*)) {
    for (int i = 0; i < map->_capacity; i++){
        if (map->_states[i] != USED)
            continue;
        function(map->_values + i * map->_value_size);
    }
}

void hashmap_items(HashMap* map, void (*function)(const void*, const void*)) {
    for (int i = 0; i < map->_capacity; i++){
        if (map->_states[i] != USED)
            continue;
        function(map->_keys + i * map->_key_size, map->_values + i * map->_value_size);
    }
}

// ==========================================================
// HASH SET
// ==========================================================

static inline void hashset_resize(HashSet* set) {
    int new_capacity = set->_capacity << 1;
    int new_mask = new_capacity - 1;

    uint64_t* new_hash_cache = malloc(sizeof(uint64_t) * new_capacity);
    unsigned char* new_states = calloc(new_capacity, sizeof(unsigned char));
    unsigned char* new_elements = malloc(set->_element_size * new_capacity);

    for (int i = 0; i < set->_capacity; i++){
        if (set->_states[i] != USED) 
            continue;
        
        uint64_t hash = set->_hash_cache[i];
        uint64_t step = double_hash_step(hash);

        uint64_t idx = hash & new_mask;

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
    bool (*equals)(const void*, const void*),
    uint64_t (*hash)(const void*)
) {
    HashSet set = {
        ._capacity = START_CAPACITY,
        ._mask = START_CAPACITY - 1,
        ._max_elements = START_CAPACITY * HASHMAP_FACTOR,
        ._element_size = element_size,
        ._equals = equals,
        ._hash = hash,
        ._size = 0,
        ._hash_cache = malloc(sizeof(uint64_t) * START_CAPACITY),
        ._states = calloc(START_CAPACITY, sizeof(unsigned char)),
        ._elements = malloc(element_size * START_CAPACITY)
    };

    return set;
}

void hashset_free(HashSet* set) {
    free(set->_states);
    free(set->_hash_cache);
    free(set->_elements);
    set->_states = NULL;
    set->_hash_cache = NULL;
    set->_elements = NULL;
}

int hashset_length(HashSet* set) {
    return set->_size;
}

void hashset_add(HashSet* set, const void* element) {
    if (set->_size == set->_max_elements)
        hashset_resize(set);

    uint64_t hash = set->_hash(element);
    uint64_t step = double_hash_step(hash);

    uint64_t idx = hash & set->_mask;
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

bool hashset_has(HashSet* set, const void* element) {
    uint64_t hash = set->_hash(element);
    uint64_t step = double_hash_step(hash);

    uint64_t idx = hash & set->_mask;

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

bool hashset_remove(HashSet* set, const void* element) {
    uint64_t hash = set->_hash(element);
    uint64_t step = double_hash_step(hash);

    uint64_t idx = hash & set->_mask;

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

HashSet hashset_copy(HashSet* set) {
    HashSet copy = hashset_create(set->_element_size, set->_equals, set->_hash);

    for (int i = 0; i < set->_capacity; i++){
        if (set->_states[i] != USED)
            continue;

        hashset_add(&copy, set->_elements + i * set->_element_size);
    }
    
    return copy;
}

HashSet hashset_union(HashSet* a, HashSet* b) {
    HashSet result = hashset_copy(a);
    
    for (int i = 0; i < b->_capacity; i++){
        if (b->_states[i] != USED)
            continue;

        hashset_add(&result, b->_elements + i * b->_element_size);
    }

    return result;
}

HashSet hashset_intersection(HashSet* a, HashSet* b) {
    HashSet* bigger;
    HashSet* smaller;

    if (a->_size >= b->_size) {
        bigger = a;
        smaller = b;
    } else {
        bigger = b;
        smaller = a;
    }

    HashSet result = hashset_create(a->_element_size, a->_equals, a->_hash);

    for (int i = 0; i < smaller->_capacity; i++){
        if (smaller->_states[i] != USED)
            continue;

        const void* element = smaller->_elements + i * smaller->_element_size;
        if (hashset_has(bigger, element))
            hashset_add(&result, element);
    }

    return result;
}

HashSet hashset_difference(HashSet* a, HashSet* b) {
    HashSet result = hashset_copy(a);

    for (int i = 0; i < b->_capacity; i++){
        if (b->_states[i] != USED)
            continue;

        hashset_remove(&result, b->_elements + i * b->_element_size);
    }
    
    return result;
}

void hashset_foreach(HashSet* set, void (*function)(const void*)) {
    for (int i = 0; i < set->_capacity; i++){
        if (set->_states[i] != USED)
            continue;
        function(set->_elements + i * set->_element_size);
    }
}

// ==========================================================
// FROZEN HASH SET
// ==========================================================

static inline uint64_t mix_bits(uint64_t val) {
    val ^= val >> 30;
    val *= 0xbf58476d1ce4e5b9ULL;
    val ^= val >> 27;
    val *= 0x94d049bb133111ebULL;
    val ^= val >> 31;
    return val;
}

static uint64_t hash_element_bytes(const unsigned char* data, size_t size) {
    uint64_t hash = 14695981039346656037ULL;
    for (size_t i = 0; i < size; i++) {
        hash ^= data[i];
        hash *= 1099511628211ULL;           
    }
    return hash;
}

FrozenHashSet frozenhashset_create(HashSet set) {
    HashSet copy_set = hashset_copy(&set);
    FrozenHashSet f_set = {
        ._set = copy_set,
        ._checksum = 0
    };

    for (int i = 0; i < copy_set._capacity; i++){
        if (copy_set._states[i] != USED)
            continue;
        
        uint64_t element_hash = hash_element_bytes(copy_set._elements + i * copy_set._element_size, copy_set._element_size);
        f_set._checksum += mix_bits(element_hash);
    }
    
    return f_set;
}

void frozenhashset_free(FrozenHashSet* f_set) {
    hashset_free(&f_set->_set);
}

int frozenhashset_length(FrozenHashSet* f_set) {
    return f_set->_set._size;
}

bool frozenhashset_has(FrozenHashSet* f_set, const void* element) {
    return hashset_has(&f_set->_set, element);
}

bool frozenhashset_equals(FrozenHashSet* a, FrozenHashSet* b) {
    if (a->_checksum != b->_checksum)
        return 0;
    if (a->_set._size != b->_set._size)
        return 0;
    if (a->_set._element_size != b->_set._element_size)
        return 0;
    
    for (int i = 0; i < a->_set._capacity; i++){
        if (a->_set._states[i] != USED)
            continue;
        
        const void* element = a->_set._elements + i * a->_set._element_size;
        if (!hashset_has(&b->_set, element))
            return 0;
    }
    return 1;
}

uint64_t frozenhashset_hash(FrozenHashSet* f_set) {
    return f_set->_checksum;
}

FrozenHashSet frozenhashset_union(FrozenHashSet* a, FrozenHashSet* b) {
    HashSet u = hashset_union(&a->_set, &b->_set);
    return frozenhashset_create(u);
}

FrozenHashSet frozenhashset_intersection(FrozenHashSet* a, FrozenHashSet* b) {
    HashSet i = hashset_intersection(&a->_set, &b->_set);
    return frozenhashset_create(i);
}

FrozenHashSet frozenhashset_difference(FrozenHashSet* a, FrozenHashSet* b) {
    HashSet d = hashset_difference(&a->_set, &b->_set);
    return frozenhashset_create(d);
}

void frozenhashset_foreach(FrozenHashSet* f_set, void (*function)(const void*)) {
    hashset_foreach(&f_set->_set, function);
}

// ==========================================================
// OBJECT POOL
// ==========================================================

ObjectPool objpool_create(int max_capacity, int obj_size) {
    obj_size = int_max(obj_size, sizeof(int));
    ObjectPool pool = {
        ._capacity = max_capacity,
        ._obj_size = obj_size,
        ._used = 0,
        ._head = 0,
        ._memory = malloc(obj_size * max_capacity)
    };

    for (int i = 0; i < max_capacity - 1; i++){
        *(int*)(pool._memory + i * obj_size) = i + 1;
    }

    return pool;
}

void objpool_free(ObjectPool* pool) {
    free(pool->_memory);
    pool->_memory = NULL;
}

void* objpool_malloc(ObjectPool* pool) {
    if (pool->_used == pool->_capacity) 
        return NULL;
    
    void* obj = pool->_memory + pool->_head * pool->_obj_size;
    pool->_head = *(int*)obj;
    pool->_used++;
    return obj;
}

void objpool_obj_free(ObjectPool* pool, void* obj) {
    *(int*)obj = pool->_head;
    pool->_head = ((unsigned char*)obj - pool->_memory) / pool->_obj_size;
    pool->_used--;
}