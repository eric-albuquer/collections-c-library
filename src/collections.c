#include "collections.h"
#include <stdlib.h>
#include <string.h>

#define START_CAPACITY 64

#define START_TRIE_CAPACITY 1024
#define ALPHABET_SIZE 256

#define HASHMAP_FACTOR 0.75f
#define HASHTABLE_EMPTY 0
#define HASHTABLE_REMOVED 1
#define HASHTABLE_USED 2

#define BISTET_WORD 64
#define BITSET_MASK 63
#define BITSET_SHIFT 6

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

uint64_t hash_combine(uint64_t seed, uint64_t hash) {
    return seed ^ (hash + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2));
}

uint64_t char_hash(const void* value) {
    if (value == NULL) return 0;
    return (uint64_t)(uint8_t)(*(const char*)value);
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

    while ((c = (uint8_t)*str++)) {
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

static inline void string_resize(String* s) {
    s->_capacity <<= 1;
    s->_memory = realloc(s->_memory, s->_capacity);
}

String string_create(const char* str) {
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

String string_empty() {
    String string = {
        ._capacity = START_CAPACITY,
        ._hash_computed = 0,
        ._size = 1,
        ._memory = malloc(START_CAPACITY),
    };
    string._memory[0] = 0;
    return string;
}

const char* string_get_str(const String* string) {
    return string->_memory;
}

int string_length(const String* string) {
    return string->_size - 1;
}

void string_append_char(String* string, const char chr) {
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

bool string_is_empty(const String* string) {
    return string->_size <= 1;
}

char string_get_char(const String* string, int idx) {
    return string->_memory[idx];
}

String string_trim(const String* string) {
    int left = 0;
    int right = string->_size - 2;

    while(string->_memory[left] == ' ')
        left++;

    while(string->_memory[right] == ' ')
        right--;

    int size = right - left + 1;
    String result = {
        ._capacity = size,
        ._hash_computed = 0,
        ._size = size,
        ._memory = malloc(size)
    };

    memcpy(result._memory, string->_memory + left, size);
    result._memory[size] = 0;
    return result;
}

String string_substring(const String* string, int start, int length) {
    String result = {
        ._capacity = length + 1,
        ._hash_computed = 0,
        ._size = length + 1,
        ._memory = malloc(length + 1)
    };

    memcpy(result._memory, string->_memory + start, length);
    result._memory[length] = 0;
    return result;
}

void string_to_lower(String* string) {
    static const int lower_offset = 'a' - 'A';
    for (int i = 0; i < string->_size - 1; i++){
        if (string->_memory[i] >= 'A' && string->_memory[i] <= 'Z')
            string->_memory[i] += lower_offset;
    }
}

void string_to_upper(String* string) {
    static const int upper_offset = 'A' - 'a';
    for (int i = 0; i < string->_size - 1; i++){
        if (string->_memory[i] >= 'a' && string->_memory[i] <= 'z')
            string->_memory[i] += upper_offset;
    }
}

bool string_starts_with(const String* string, const char* prefix) {
    char c;
    int idx = 0;
    while ((c = *(prefix++))){
        if (string->_memory[idx] != c)
            return 0;
        idx++;
    }
    return 1;
}

bool string_ends_with(const String* string, const char* suffix) {
    int length = strlen(suffix);
    int idx = string->_size - 2;
    for (int i = length - 1; i >= 0; i--){
        if (string->_memory[idx] != suffix[i])
            return 0;
        idx--;
    }
    return 1;
}

String string_copy(const String* string) {
    String copy = {
        ._capacity = string->_capacity,
        ._hash_computed = string->_hash_computed,
        ._hash = string->_hash,
        ._size = string->_size,
        ._memory = malloc(string->_capacity)
    };
    memcpy(copy._memory, string->_memory, string->_size);
        
    return copy;
}

int string_compare(const String* a, const String* b) {
    return strcmp(a->_memory, b->_memory);
}

void string_extend(String* a, const String* b) {
    a->_hash_computed = false;
    const int idx = a->_size - 1;
    a->_size += b->_size - 1;
    while(a->_size > a->_capacity)
        string_resize(a);
    memcpy(a->_memory + idx, b->_memory, b->_size);
}

static inline void build_shift_table(const char* pattern, int m, int* bad_char) {
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        bad_char[i] = m;
    }

    for (int i = 0; i < m - 1; i++) {
        uint8_t c = (uint8_t)pattern[i];
        bad_char[c] = m - 1 - i;
    }
}

int string_index_of(const String* a, const String* b) {
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

        uint8_t mismatched_char = (uint8_t)a->_memory[i + m - 1];
        i += bad_char[mismatched_char];
    }

    return -1;
}

bool string_contains(const String* a, const String* b) {
    return string_index_of(a, b) != -1;
}

bool string_equals(const String* a, const String* b) {
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

String string_concatenate(const String* a, const String* b) {
    String res = string_copy(a);
    string_extend(&res, b);
    return res;
}

void string_foreach(const String* string, void (*function)(int, char*)) {
    for (int i = 0; i < string->_size - 1; i++){
        function(i, string->_memory + i);  
    }
}

// ==========================================================
// ARRAY LIST
// ==========================================================

static inline void arraylist_resize(ArrayList* list){
    list->_capacity <<= 1;
    list->_memory = realloc(list->_memory, list->_capacity * list->_element_size);
}

ArrayList arraylist_create(int element_size, int initial_size) {
    int initial_capacity = int_max(initial_size, START_CAPACITY);
    ArrayList list = {
        ._capacity = initial_capacity,
        ._element_size = element_size,
        ._size = initial_size,
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

int arraylist_length(const ArrayList* list) {
    return list->_size;
}

bool arraylist_get(const ArrayList* list, int idx, void* out) {
    if ((unsigned int)idx >= list->_size)
        return 0;
    memcpy(out, list->_memory + idx * list->_element_size, list->_element_size);
    return 1;
}

bool arraylist_set(ArrayList* list, int idx, const void* element) {
    if ((unsigned int)idx >= list->_size)
        return 0;
    memcpy(list->_memory + idx * list->_element_size, element, list->_element_size);
    return 1;
}

bool arraylist_insert_index(ArrayList* list, int idx, const void* element) {
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

    if (out)
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

void arraylist_append(ArrayList* list, const void* element) {
    if (list->_size == list->_capacity)
        arraylist_resize(list);

    memcpy(list->_memory + list->_size * list->_element_size, element, list->_element_size);
    list->_size++;
}

bool arraylist_pop(ArrayList* list, void* out) {
    if (list->_size == 0) 
        return 0;

    list->_size--;
    if (out)
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

static inline void insertion_sort(ArrayList* list, int left, int right, cmp_fn cmp) {
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

static inline void merge(ArrayList* list, uint8_t* aux, int l, int m, int r, cmp_fn cmp) {
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

void arraylist_sort(ArrayList* list, cmp_fn cmp) {
    int n = list->_size;
    if (n < 2)
        return;

    int min_run = calc_min_run(n);

    for (int i = 0; i < n; i += min_run){
        int end = int_min(i + min_run - 1, n - 1);
        insertion_sort(list, i, end, cmp);
    }

    if (min_run == n)
        return;

    uint8_t* aux = malloc(list->_element_size * list->_size);
    
    int size = min_run;
    while (size < n) {
        const int double_size = size * 2;
        uint8_t* temp = list->_memory;
        list->_memory = aux;
        aux = temp;
        for (int i = 0; i < n; i += double_size){
            int mid = int_min(i + size, n);
            int end = int_min(i + double_size, n);

            if (i < end)
                merge(list, aux, i, mid, end, cmp);
        }
        size = double_size;
    }

    free(aux);
    //qsort(list->_memory, list->_size, list->_element_size, cmp);
}

int arraylist_bsearch(const ArrayList* list, const void* element, cmp_fn cmp) {
    void* idx = bsearch(element, list->_memory, list->_size, list->_element_size, cmp);
    if (idx == NULL) return -1;
    return (((uint8_t*)idx) - list->_memory) / list->_element_size;
}

ArrayList arraylist_copy(const ArrayList* list) {
    ArrayList copy = {
        ._capacity = list->_capacity,
        ._element_size = list->_element_size,
        ._size = list->_size,
        ._swap_buffer = malloc(list->_element_size),
        ._memory = malloc(list->_element_size * list->_capacity)
    };

    memcpy(copy._memory, list->_memory, list->_size * list->_element_size);
    return copy;
}

void arraylist_foreach(ArrayList* list, void(*function)(int, void*)) {
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

    uint8_t* old_memory = deque->_memory;
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

int arraydeque_length(const ArrayDeque* deque) {
    return deque->_size;
}

void arraydeque_append(ArrayDeque* deque, const void* element) {
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

void arraydeque_appendleft(ArrayDeque* deque, const void* element) {
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

    if (out)
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

    if (out)
        memcpy(
            out,
            deque->_memory + deque->_head * deque->_element_size,
            deque->_element_size
        );

    deque->_head = (deque->_head + 1) & deque->_mask;
    deque->_size--;
    return 1;
}

ArrayDeque arraydeque_copy(const ArrayDeque* deque) {
    ArrayDeque copy = {
        ._capacity = deque->_capacity,
        ._element_size = deque->_element_size,
        ._size = deque->_size,
        ._head = deque->_head,
        ._tail = deque->_tail,
        ._mask = deque->_mask,
        ._memory = malloc(deque->_capacity * deque->_element_size)
    };

    memcpy(copy._memory, deque->_memory, deque->_capacity * deque->_element_size);
    return copy;
}

void arraydeque_foreach(ArrayDeque* deque, void(*function)(int, void*)) {
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

Heap heap_create(int element_size, cmp_fn cmp) {
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

int heap_length(const Heap* heap) {
    return heap->_size;
}

void heap_push(Heap* heap, const void* element) {
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

    if (out)
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

Heap heap_copy(const Heap* heap) {
    Heap copy = {
        ._capacity = heap->_capacity,
        ._element_size = heap->_element_size,
        ._cmp = heap->_cmp,
        ._size = heap->_size,
        ._memory = malloc(heap->_capacity * heap->_element_size)
    };

    memcpy(copy._memory, heap->_memory, heap->_size * heap->_element_size);

    return copy;
}

void heap_foreach(const Heap* heap, void(*function)(int, const void*)) {
    for (int i = 0; i < heap->_size; i++){
        function(i, heap->_memory + i * heap->_element_size);
    }
}

// ==========================================================
// HASH MAP
// ==========================================================

static inline uint64_t double_hash_step(uint64_t hash) {
    return (hash * 0x9e3779b97f4a7c15ULL) | 1;
}

static inline void hashmap_resize(HashMap* map) {
    int new_capacity = map->_capacity << 1;
    int new_mask = new_capacity - 1;

    uint64_t* new_hash_cache = malloc(sizeof(uint64_t) * new_capacity);
    uint8_t* new_states = calloc(new_capacity, sizeof(uint8_t));
    uint8_t* new_keys = malloc(map->_key_size * new_capacity);
    uint8_t* new_values = malloc(map->_value_size * new_capacity);

    for (int i = 0; i < map->_capacity; i++){
        if (map->_states[i] != HASHTABLE_USED) 
            continue;
        
        uint64_t hash = map->_hash_cache[i];
        uint64_t step = double_hash_step(hash);

        uint64_t idx = hash & new_mask;

        for (int j = 0; j < new_capacity; j++){
            if (new_states[idx] == HASHTABLE_EMPTY) {
                new_states[idx] = HASHTABLE_USED;
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
    equals_fn equals,
    hash_fn hash
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
        ._states = calloc(START_CAPACITY, sizeof(uint8_t)),
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

int hashmap_length(const HashMap* map) {
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
        if (map->_states[idx] != HASHTABLE_USED && location == -1) {
            location = idx;
        } else if (map->_states[idx] == HASHTABLE_USED && map->_hash_cache[idx] == hash && map->_equals(map->_keys + map->_key_size * idx, key)) {
            void* ptr = map->_values + idx * map->_value_size;
            if (old_value)
                memcpy(old_value, ptr, map->_value_size);

            memcpy(ptr, value, map->_value_size);
            return;
        }
        if (map->_states[idx] == HASHTABLE_EMPTY)
            break;
        idx = (idx + step) & map->_mask;
    }
    
    memcpy(map->_keys + location * map->_key_size, key, map->_key_size);
    memcpy(map->_values + location * map->_value_size, value, map->_value_size);
    map->_hash_cache[location] = hash;
    map->_states[location] = HASHTABLE_USED;
    map->_size++;
}

bool hashmap_get(const HashMap* map, const void* key, void* out) {
    uint64_t hash = map->_hash(key);
    uint64_t step = double_hash_step(hash);

    uint64_t idx = hash & map->_mask;

    for (int i = 0; i < map->_capacity; i++){
        if (map->_states[idx] == HASHTABLE_EMPTY)
            return 0;
        if (map->_states[idx] == HASHTABLE_USED && map->_hash_cache[idx] == hash && map->_equals(map->_keys + idx * map->_key_size, key)) {
            if (out)
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
        if (map->_states[idx] == HASHTABLE_EMPTY)
            return 0;
        if (map->_states[idx] == HASHTABLE_USED && map->_hash_cache[idx] == hash && map->_equals(map->_keys + idx * map->_key_size, key)) {
            memcpy(
                out,
                map->_values + idx * map->_value_size,
                map->_value_size
            );
            map->_states[idx] = HASHTABLE_REMOVED;
            map->_size--;
            return 1;
        }
        idx = (idx + step) & map->_mask;
    }
    return 0;
}

HashMap hashmap_copy(const HashMap* map) {
    HashMap copy = hashmap_create(map->_key_size, map->_value_size, map->_equals, map->_hash);

    for (int i = 0; i < map->_capacity; i++){
        if (map->_states[i] != HASHTABLE_USED) 
            continue;
        
        hashmap_set(&copy, map->_keys + i * map->_key_size, map->_values + i * map->_value_size, NULL);
    }
    
    return copy;
}

void hashmap_keys(const HashMap* map, void (*function)(const void*)) {
    for (int i = 0; i < map->_capacity; i++){
        if (map->_states[i] != HASHTABLE_USED)
            continue;
        function(map->_keys + i * map->_key_size);
    }
}

void hashmap_values(HashMap* map, void (*function)(void*)) {
    for (int i = 0; i < map->_capacity; i++){
        if (map->_states[i] != HASHTABLE_USED)
            continue;
        function(map->_values + i * map->_value_size);
    }
}

void hashmap_items(HashMap* map, void (*function)(const void*, void*)) {
    for (int i = 0; i < map->_capacity; i++){
        if (map->_states[i] != HASHTABLE_USED)
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
    uint8_t* new_states = calloc(new_capacity, sizeof(uint8_t));
    uint8_t* new_elements = malloc(set->_element_size * new_capacity);

    for (int i = 0; i < set->_capacity; i++){
        if (set->_states[i] != HASHTABLE_USED) 
            continue;
        
        uint64_t hash = set->_hash_cache[i];
        uint64_t step = double_hash_step(hash);

        uint64_t idx = hash & new_mask;

        for (int j = 0; j < new_capacity; j++){
            if (new_states[idx] == HASHTABLE_EMPTY) {
                new_states[idx] = HASHTABLE_USED;
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
    equals_fn equals,
    hash_fn hash
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
        ._states = calloc(START_CAPACITY, sizeof(uint8_t)),
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

int hashset_length(const HashSet* set) {
    return set->_size;
}

bool hashset_add(HashSet* set, const void* element) {
    if (set->_size == set->_max_elements)
        hashset_resize(set);

    uint64_t hash = set->_hash(element);
    uint64_t step = double_hash_step(hash);

    uint64_t idx = hash & set->_mask;
    int location = -1;

    for (int i = 0; i < set->_capacity; i++){
        if (set->_states[idx] != HASHTABLE_USED && location == -1) {
            location = idx;
        } else if (set->_states[idx] == HASHTABLE_USED && set->_hash_cache[idx] == hash && set->_equals(set->_elements + set->_element_size * idx, element)) {
            return 0;
        }
        if (set->_states[idx] == HASHTABLE_EMPTY)
            break;
        idx = (idx + step) & set->_mask;
    }
    
    memcpy(set->_elements + location * set->_element_size, element, set->_element_size);
    set->_hash_cache[location] = hash;
    set->_states[location] = HASHTABLE_USED;
    set->_size++;
    return 1;
}

bool hashset_has(const HashSet* set, const void* element) {
    uint64_t hash = set->_hash(element);
    uint64_t step = double_hash_step(hash);

    uint64_t idx = hash & set->_mask;

    for (int i = 0; i < set->_capacity; i++){
        if (set->_states[idx] == HASHTABLE_EMPTY)
            return 0;
        if (set->_states[idx] == HASHTABLE_USED && set->_hash_cache[idx] == hash && set->_equals(set->_elements + idx * set->_element_size, element)) {
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
        if (set->_states[idx] == HASHTABLE_EMPTY)
            return 0;
        if (set->_states[idx] == HASHTABLE_USED && set->_hash_cache[idx] == hash && set->_equals(set->_elements + idx * set->_element_size, element)) {
            set->_states[idx] = HASHTABLE_REMOVED;
            set->_size--;
            return 1;
        }
        idx = (idx + step) & set->_mask;
    }
    return 0;
}

HashSet hashset_copy(const HashSet* set) {
    HashSet copy = hashset_create(set->_element_size, set->_equals, set->_hash);

    for (int i = 0; i < set->_capacity; i++){
        if (set->_states[i] != HASHTABLE_USED)
            continue;

        hashset_add(&copy, set->_elements + i * set->_element_size);
    }
    
    return copy;
}

HashSet hashset_union(const HashSet* a, const HashSet* b) {
    HashSet result = hashset_copy(a);
    
    for (int i = 0; i < b->_capacity; i++){
        if (b->_states[i] != HASHTABLE_USED)
            continue;

        hashset_add(&result, b->_elements + i * b->_element_size);
    }

    return result;
}

HashSet hashset_intersection(const HashSet* a, const HashSet* b) {
    const HashSet* bigger;
    const HashSet* smaller;

    if (a->_capacity >= b->_capacity) {
        bigger = a;
        smaller = b;
    } else {
        bigger = b;
        smaller = a;
    }

    HashSet result = hashset_create(a->_element_size, a->_equals, a->_hash);

    for (int i = 0; i < smaller->_capacity; i++){
        if (smaller->_states[i] != HASHTABLE_USED)
            continue;

        const void* element = smaller->_elements + i * smaller->_element_size;
        if (hashset_has(bigger, element))
            hashset_add(&result, element);
    }

    return result;
}

HashSet hashset_difference(const HashSet* a, const HashSet* b) {
    HashSet result = hashset_copy(a);

    for (int i = 0; i < b->_capacity; i++){
        if (b->_states[i] != HASHTABLE_USED)
            continue;

        hashset_remove(&result, b->_elements + i * b->_element_size);
    }
    
    return result;
}

void hashset_foreach(const HashSet* set, void (*function)(const void*)) {
    for (int i = 0; i < set->_capacity; i++){
        if (set->_states[i] != HASHTABLE_USED)
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

static uint64_t hash_element_bytes(const uint8_t* data, size_t size) {
    uint64_t hash = 14695981039346656037ULL;
    for (size_t i = 0; i < size; i++) {
        hash ^= data[i];
        hash *= 1099511628211ULL;           
    }
    return hash;
}

FrozenHashSet frozenhashset_empty(int element_size, equals_fn equals, hash_fn hash) {
    FrozenHashSet f_set = {
        ._checksum_computed = 1,
        ._checksum = 0,
        ._set = hashset_create(element_size, equals, hash)
    };
    return f_set;
}

FrozenHashSet frozenhashset_create(const HashSet* set) {
    HashSet copy_set = hashset_copy(set);
    FrozenHashSet f_set = {
        ._set = copy_set,
        ._checksum_computed = 0,
        ._checksum = 0
    };
    
    return f_set;
}

void frozenhashset_free(FrozenHashSet* f_set) {
    hashset_free(&f_set->_set);
}

FrozenHashSet frozenhashset_copy(const FrozenHashSet* f_set) {
    FrozenHashSet copy = *f_set;
    copy._set = hashset_copy(&f_set->_set);
    return copy;
}

int frozenhashset_length(const FrozenHashSet* f_set) {
    return f_set->_set._size;
}

bool frozenhashset_has(const FrozenHashSet* f_set, const void* element) {
    return hashset_has(&f_set->_set, element);
}

bool frozenhashset_equals(const FrozenHashSet* a, const FrozenHashSet* b) {
    if (a->_checksum_computed && b->_checksum_computed && a->_checksum != b->_checksum)
        return 0;
    if (a->_set._size != b->_set._size)
        return 0;
    if (a->_set._element_size != b->_set._element_size)
        return 0;
    
    for (int i = 0; i < a->_set._capacity; i++){
        if (a->_set._states[i] != HASHTABLE_USED)
            continue;
        
        const void* element = a->_set._elements + i * a->_set._element_size;
        if (!hashset_has(&b->_set, element))
            return 0;
    }
    return 1;
}

uint64_t frozenhashset_hash(FrozenHashSet* f_set) {
    if (f_set->_checksum_computed)
        return f_set->_checksum;

    for (int i = 0; i < f_set->_set._capacity; i++){
        if (f_set->_set._states[i] != HASHTABLE_USED)
            continue;
        
        uint64_t element_hash = hash_element_bytes(f_set->_set._elements + i * f_set->_set._element_size, f_set->_set._element_size);
        f_set->_checksum += mix_bits(element_hash);
    }

    f_set->_checksum_computed = 1;
    return f_set->_checksum;
}

FrozenHashSet frozenhashset_union(const FrozenHashSet* a, const FrozenHashSet* b) {
    HashSet u = hashset_union(&a->_set, &b->_set);
    return frozenhashset_create(&u);
}

FrozenHashSet frozenhashset_intersection(const FrozenHashSet* a, const FrozenHashSet* b) {
    HashSet i = hashset_intersection(&a->_set, &b->_set);
    return frozenhashset_create(&i);
}

FrozenHashSet frozenhashset_difference(const FrozenHashSet* a, const FrozenHashSet* b) {
    HashSet d = hashset_difference(&a->_set, &b->_set);
    return frozenhashset_create(&d);
}

FrozenHashSet frozenhashset_union_element(const FrozenHashSet* f_set, const void* element) {
    if (hashset_has(&f_set->_set, element))
        return *f_set;
    FrozenHashSet result = frozenhashset_copy(f_set);
    hashset_add(&result._set, element);
    result._checksum_computed = 0;
    return result;
}

void frozenhashset_foreach(const FrozenHashSet* f_set, void (*function)(const void*)) {
    hashset_foreach(&f_set->_set, function);
}

// ==========================================================
// TRIE
// ==========================================================

static inline int trie_get_or_create_children(Trie* trie, int parent) {
    if (trie->_nodes[parent]._children != -1)
        return trie->_nodes[parent]._children;

    if (trie->_used + ALPHABET_SIZE > trie->_capacity) {
        trie->_capacity <<= 1;
        trie->_nodes = realloc(trie->_nodes, trie->_capacity * sizeof(TrieNode));
    }

    int children = trie->_used;

    TrieNode default_node = {._parent = parent, ._children = -1, ._is_word = 0};
    for (int i = 0; i < ALPHABET_SIZE; i++){
        trie->_nodes[children + i] = default_node;
    }

    trie->_nodes[parent]._children = children;
    trie->_used += ALPHABET_SIZE;
    return children;
}

Trie trie_create() {
    Trie trie = {
        ._capacity = START_TRIE_CAPACITY,
        ._used = ALPHABET_SIZE,
        ._size = 0,
        ._nodes = malloc(sizeof(TrieNode) * START_TRIE_CAPACITY)
    };
    for (int i = 0; i < ALPHABET_SIZE; i++){
        trie._nodes[i] = (TrieNode) {
            ._parent = -1,
            ._is_word = 0,
            ._children = -1
        };
    }
    
    return trie;
}

void trie_free(Trie* trie) {
    free(trie->_nodes);
    trie->_nodes = NULL;
}

Trie trie_copy(const Trie* trie) {
    Trie copy = *trie;
    copy._nodes = malloc(sizeof(TrieNode) * trie->_capacity);
    memcpy(copy._nodes, trie->_nodes, trie->_used * sizeof(TrieNode));
    return copy;
}

void trie_add_str(Trie* trie, const char* str) {
    uint8_t c;
    int node_idx = (uint8_t)(*(str++));
    if (node_idx == 0)
        return;
    while((c = *(str++))) {
        int children = trie_get_or_create_children(trie, node_idx);
        node_idx = children + c;
    }
    if (trie->_nodes[node_idx]._is_word) 
        return;
    trie->_nodes[node_idx]._is_word = 1;
    trie->_size++;
}

void trie_add_string(Trie* trie, const String* string) {
    trie_add_str(trie, string->_memory);
}

int trie_length(const Trie* trie) {
    return trie->_size;
}

bool trie_has_str(const Trie* trie, const char* str) {
    uint8_t c;
    int node_idx = (uint8_t)*(str++);
    if (node_idx == 0)
        return 0;
    while((c = *(str++))) {
        if (trie->_nodes[node_idx]._children == -1)
            return 0;
        node_idx = trie->_nodes[node_idx]._children + c;
    }
    return trie->_nodes[node_idx]._is_word;
}

bool trie_has_string(const Trie* trie, const String* string) {
    return trie_has_str(trie, string->_memory);
}

bool trie_has_prefix_str(const Trie* trie, const char* str) {
    uint8_t c;
    int node_idx = (uint8_t)*(str++);
    if (node_idx == 0)
        return 0;
    while((c = *(str++))) {
        if (trie->_nodes[node_idx]._children == -1)
            return 0;
        node_idx = trie->_nodes[node_idx]._children + c;
    }
    return trie->_nodes[node_idx]._is_word || trie->_nodes[node_idx]._parent != -1;
}

bool trie_has_prefix_string(const Trie* trie, const String* string) {
    return trie_has_prefix_str(trie, string->_memory);
}

// ==========================================================
// OBJECT POOL
// ==========================================================

ObjectPool objpool_create(int max_capacity, int obj_size) {
    obj_size = (obj_size + 7) & ~7;
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

int objpool_available(const ObjectPool* pool) {
    return pool->_capacity - pool->_used;
}

void* objpool_malloc(ObjectPool* pool) {
    if (pool->_used == pool->_capacity) 
        return NULL;
    
    void* obj = pool->_memory + pool->_head * pool->_obj_size;
    pool->_head = *(int*)obj;
    pool->_used++;
    return obj;
}

void objpool_free_obj(ObjectPool* pool, void* obj) {
    *(int*)obj = pool->_head;
    pool->_head = ((uint8_t*)obj - pool->_memory) / pool->_obj_size;
    pool->_used--;
}

// ==========================================================
// ARENA POOL
// ==========================================================

ArenaPool arenapool_create(int total_bytes) {
    ArenaPool arena = {
        ._capacity = total_bytes,
        ._used = 0,
        ._memory = malloc(total_bytes)
    };

    return arena;
}

void arenapool_free(ArenaPool* arena) {
    free(arena->_memory);
    arena->_memory = NULL;
}

int arenapool_available(ArenaPool* arena) {
    return arena->_capacity - arena->_used;
}

void* arenapool_malloc(ArenaPool* arena, int byte_count) {
    byte_count = (byte_count + 7) & ~7;
    if (arena->_used + byte_count > arena->_capacity)
        return NULL;

    void* mem = arena->_memory + arena->_used;
    arena->_used += byte_count;
    return mem;
}

// ==========================================================
// BYTE ARRAY
// ==========================================================

static inline void bytearray_resize(ByteArray* bytearray) {
    bytearray->_capacity <<= 1;
    bytearray->_data = realloc(bytearray->_data, bytearray->_capacity);
}

ByteArray bytearray_create(int initial_capacity) {
    initial_capacity = int_max(initial_capacity, 0);
    ByteArray bytearray = {
        ._capacity = initial_capacity,
        ._size = 0,
        ._cursor = 0,
        ._data = malloc(initial_capacity)
    };

    return bytearray;
}

void bytearray_free(ByteArray* bytearray) {
    free(bytearray->_data);
    bytearray->_data = NULL;
}

ByteArray bytearray_copy(const ByteArray* bytearray){
    ByteArray copy = *bytearray;
    copy._data = malloc(bytearray->_capacity);
    memcpy(copy._data, bytearray->_data, bytearray->_size);
    return copy;
}

void bytearray_reserve(ByteArray* bytearray, int new_capacity) {
    if (new_capacity <= bytearray->_capacity)
        return;
    bytearray->_data = realloc(bytearray->_data, new_capacity);
    bytearray->_capacity = new_capacity;
}

void bytearray_clear(ByteArray* bytearray) {
    bytearray->_size = 0;
    bytearray->_cursor = 0;
}

void bytearray_write_bytes(ByteArray* bytearray, const void* bytes, int size) {
    while(bytearray->_cursor + size > bytearray->_capacity)
        bytearray_resize(bytearray);
    memcpy(bytearray->_data + bytearray->_cursor, bytes, size);
    bytearray->_cursor += size;
    bytearray->_size = int_max(bytearray->_size, bytearray->_cursor);
}

int bytearray_read_bytes(ByteArray* bytearray, void* out, int size) {
    int bytes_to_read = int_min((unsigned int)size, bytearray->_size - bytearray->_cursor);
    if (out)
        memcpy(out, bytearray->_data + bytearray->_cursor, bytes_to_read);
    bytearray->_cursor += bytes_to_read;
    return bytes_to_read;
}

bool bytearray_seek(ByteArray* bytearray, int offset) {
    if ((unsigned int)offset >= bytearray->_capacity)
        return 0;
    bytearray->_cursor = offset;
    return 1;
}

int bytearray_available(const ByteArray* bytearray) {
    return bytearray->_size - bytearray->_cursor;
}

// ==========================================================
// BIT SET
// ==========================================================

BitSet bitset_create(int size) {
    size = (size + BITSET_MASK) & ~BITSET_MASK;
    BitSet bitset = {
        ._size = size,
        ._data = calloc(size, 1)
    };

    return bitset;
}

void bitset_free(BitSet* bitset) {
    free(bitset->_data);
    bitset->_data = NULL;
}

BitSet bitset_copy(const BitSet* bitset) {
    BitSet copy = {
        ._size = bitset->_size,
        ._data = malloc(bitset->_size)
    };

    memcpy(copy._data, bitset->_data, bitset->_size);
    return copy;
}

void bitset_clear(BitSet* bitset) {
    memset(bitset->_data, 0, bitset->_size);
}

void bitset_fill(BitSet* bitset, bool value) {
    memset(bitset->_data, 0xff * value, bitset->_size);
}

void bitset_set(BitSet* bitset, int bit) {
    const int idx = bit >> BITSET_SHIFT;
    bitset->_data[idx] |= (1ULL << (bit & BITSET_MASK));
}

bool bitset_get(const BitSet* bitset, int bit) {
    const int idx = bit >> BITSET_SHIFT;
    return (bitset->_data[idx] & (1ULL << (bit & BITSET_MASK))) != 0;
}

void bitset_flip(BitSet* bitset, int bit) {
    const int idx = bit >> BITSET_SHIFT;
    bitset->_data[idx] ^= (1ULL << (bit & BITSET_MASK));
}

void bitset_reset(BitSet* bitset, int bit) {
    const int idx = bit >> BITSET_SHIFT;
    bitset->_data[idx] &= ~(1ULL << (bit & BITSET_MASK));
}

BitSet bitset_and(const BitSet* a, const BitSet* b) {
    BitSet result = bitset_copy(a);
    for (int i = 0; i < a->_size >> BITSET_SHIFT; i++)
        result._data[i] &= b->_data[i];
    return result;
}

BitSet bitset_or(const BitSet* a, const BitSet* b) {
    BitSet result = bitset_copy(a);
    for (int i = 0; i < a->_size >> BITSET_SHIFT; i++)
        result._data[i] |= b->_data[i];
    return result;
}

BitSet bitset_xor(const BitSet* a, const BitSet* b) {
    BitSet result = bitset_copy(a);
    for (int i = 0; i < a->_size >> BITSET_SHIFT; i++)
        result._data[i] ^= b->_data[i];
    return result;
}

BitSet bitset_not(const BitSet* a) {
    BitSet result = {
        ._size = a->_size,
        ._data = malloc(a->_size)
    };

    for (int i = 0; i < a->_size >> BITSET_SHIFT; i++)
        result._data[i] = ~a->_data[i];

    return result;
}

bool bitset_any(const BitSet* bitset) {
    for (int i = 0; i < bitset->_size >> BITSET_SHIFT; i++)
        if (bitset->_data[i]) return 1;
    
    return 0;
}

bool bitset_all(const BitSet* bitset) {
    for (int i = 0; i < bitset->_size >> BITSET_SHIFT; i++)
        if (bitset->_data[i] != 0xffffffffffffffffULL) return 0;

    return 1;
}

bool bitset_none(const BitSet* bitset) {
    for (int i = 0; i < bitset->_size >> BITSET_SHIFT; i++)
        if (bitset->_data[i] != 0) return 0;

    return 1;
}

#if defined(_MSC_VER)
    #include <intrin.h>
#endif

int bitset_first(const BitSet* bitset) {
    for (int i = 0; i < bitset->_size >> BITSET_SHIFT; i++){
        if (bitset->_data[i] == 0)
            continue;
        int bit = i << BITSET_SHIFT;
        uint64_t val = bitset->_data[i];

        #if defined(_MSC_VER)
            unsigned long index;
            _BitScanForward64(&index, val);
            bit += (int)index;
        #elif defined(__GNUC__) || defined(__clang__)
            bit += __builtin_ctzll(val);
        #else
            int pos = 0;
            if ((val & 0xFFFFFFFF) == 0) { val >>= 32; pos += 32; }
            if ((val & 0xFFFF) == 0)     { val >>= 16; pos += 16; }
            if ((val & 0xFF) == 0)       { val >>= 8;  pos += 8;  }
            if ((val & 0xF) == 0)        { val >>= 4;  pos += 4;  }
            if ((val & 0x3) == 0)        { val >>= 2;  pos += 2;  }
            if ((val & 0x1) == 0)        { pos += 1; }
            bit += pos;
        #endif
        return bit;
    }

    return -1;
}

int bitset_last(const BitSet* bitset) {
    for (int i = (bitset->_size >> BITSET_SHIFT) - 1; i >= 0; i--){
        if (bitset->_data[i] == 0)
            continue;
        int bit = i << BITSET_SHIFT;
        uint64_t val = bitset->_data[i];
        #if defined(_MSC_VER)
            unsigned long index;
            _BitScanReverse64(&index, val);
            bit += (int)index;
        #elif defined(__GNUC__) || defined(__clang__)
            bit += 63 - __builtin_clzll(val);
        #else
            int pos = 0;
            if (val >= 0x100000000ULL) { val >>= 32; pos += 32; }
            if (val >= 0x00010000ULL)  { val >>= 16; pos += 16; }
            if (val >= 0x00000100ULL)  { val >>= 8;  pos += 8;  }
            if (val >= 0x00000010ULL)  { val >>= 4;  pos += 4;  }
            if (val >= 0x00000004ULL)  { val >>= 2;  pos += 2;  }
            if (val >= 0x00000002ULL)  { pos += 1; }
            bit += pos;
        #endif
        return bit;
    }
    return -1;
}

// ==========================================================
// INDEXED PRIORITY QUEUE
// ==========================================================

IndexedPriorityQueue indexedpq_create(int element_size, int priority_count) {
    IndexedPriorityQueue ipq = {
        ._priority_count = priority_count,
        ._element_size = element_size,
        ._size = 0,
        ._min_bucket_reference = bitset_create(priority_count),
        ._buckets = malloc(sizeof(ArrayDeque) * priority_count)
    };

    for (int i = 0; i < priority_count; i++){
        ipq._buckets[i] = arraydeque_create(element_size);
    }

    return ipq;
}

void indexedpq_free(IndexedPriorityQueue* ipq) {
    for (int i = 0; i < ipq->_priority_count; i++){
        arraydeque_free(&ipq->_buckets[i]);
    }

    free(ipq->_buckets);
    ipq->_buckets = NULL;

    bitset_free(&ipq->_min_bucket_reference);
}

IndexedPriorityQueue indexedpq_copy(const IndexedPriorityQueue* ipq) {
    IndexedPriorityQueue copy = *ipq;
    copy._min_bucket_reference = bitset_copy(&ipq->_min_bucket_reference);
    for (int i = 0; i < ipq->_priority_count; i++){
        copy._buckets[i] = arraydeque_copy(&ipq->_buckets[i]);
    }
    return copy;
}

bool indexedpq_enqueue(IndexedPriorityQueue* ipq, int priority, const void* element) {
    if ((uint32_t)priority >= ipq->_priority_count)
        return 0;
    arraydeque_append(&ipq->_buckets[priority], element);
    bitset_set(&ipq->_min_bucket_reference, priority);
    ipq->_size++;
    return 1;
}

bool indexedpq_dequeue(IndexedPriorityQueue* ipq, void* out, int* priority_out) {
    if (ipq->_size == 0)
        return 0;
    uint32_t min_priority = bitset_first(&ipq->_min_bucket_reference);
    arraydeque_popleft(&ipq->_buckets[min_priority], out);
    if (priority_out)
        *priority_out = min_priority;

    if (arraydeque_length(&ipq->_buckets[min_priority]) == 0)
        bitset_reset(&ipq->_min_bucket_reference, min_priority);

    ipq->_size--;
    return 1;
}

int indexedpq_length(const IndexedPriorityQueue* ipq) {
    return ipq->_size;
}