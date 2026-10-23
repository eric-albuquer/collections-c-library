# Collections C Library

A lightweight, generic, and high-performance collection library written in ANSI/C11 C. It provides dynamic arrays, double-ended queues, priority heaps, hash maps, and hash sets with automatic memory reallocation and byte-level generic storage using `void*`.

---

## Features

- **Generic Types**: Stores data of any size using byte-level copying (`sizeof(T)`).
- **Dynamic Memory**: Automated capacity management and resizing.
- **Pure C**: Compatible with C99/C11 standards with zero external dependencies (standard C runtime only).
- **Comprehensive Data Structures**:
  - `ArrayList`: Dynamic array with sorting and mapping utilities.
  - `ArrayDeque`: Double-ended ring buffer (FIFO/LIFO queue).
  - `Heap`: Generic Priority Queue driven by custom comparison callbacks.
  - `HashMap`: Open-addressing hash table with cached hashes and tombstone states.
  - `HashSet`: Fast unique-value container backed by open addressing.

---

## Project Structure

```text
.
├── src/
│   ├── collections.c
│   └── collections.h
└── tests/
    ├── arraylist_test.c
    ├── arraydeque_test.c
    ├── heap_test.c
    ├── hashmap_test.c
    └── hashset_test.c
```

---

## Building & Testing

### Compiling Tests with `gcc` / `clang`

To compile and run an individual test file:

```bash
# Example: Compiling the HashMap test
gcc -Isrc tests/hashmap_test.c src/collections.c -o run_tests && ./run_tests
```

To run all test suites in a single loop:

MACOS / LINUX
```bash
for file in ./tests/*.c; do gcc -Isrc "$file" ./src/collections.c -o test_bin && ./test_bin || break; done
```

WINDOWS
```bash
FOR %f IN (.\tests\*.c) DO gcc -Isrc "%f" .\src\collections.c -o test_bin.exe && test_bin.exe
```

---

## API & Usage Examples

### 1. ArrayList

A dynamic array that expands automatically.

```c
#include <stdio.h>
#include "collections.h"

int compare_ints(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

void print_element(int index, const void* element) {
    printf("[%d]: %d\n", index, *(int*)element);
}

int main(void) {
    // Create ArrayList for integers (0 uses default capacity)
    ArrayList list = arraylist_create(sizeof(int), 0);

    int values[] = {40, 10, 30, 20};
    for (int i = 0; i < 4; i++) {
        arraylist_append(&list, &values[i]);
    }

    // Sort in-place
    arraylist_sort(&list, compare_ints);

    // Map over elements
    arraylist_map(&list, print_element);

    // Pop element (removes 40)
    int popped;
    if (arraylist_pop(&list, &popped)) {
        printf("Popped: %d\n", popped);
    }

    arraylist_free(list);
    return 0;
}
```

---

### 2. ArrayDeque

A double-ended queue implemented over a circular ring buffer.

```c
#include <stdio.h>
#include "collections.h"

int main(void) {
    ArrayDeque deque = arraydeque_create(sizeof(int));

    int a = 10, b = 20, c = 5;
    arraydeque_append(&deque, &a);      // [10]
    arraydeque_append(&deque, &b);      // [10, 20]
    arraydeque_appendleft(&deque, &c);  // [5, 10, 20]

    int front_out, back_out;
    arraydeque_popleft(&deque, &front_out); // Pops 5
    arraydeque_pop(&deque, &back_out);     // Pops 20

    printf("Front: %d, Back: %d\n", front_out, back_out);

    arraydeque_free(deque);
    return 0;
}
```

---

### 3. Heap (Priority Queue)

A binary heap configured via a user-defined comparator function.

```c
#include <stdio.h>
#include "collections.h"

// Comparator for Min-Heap (smallest element on top)
int min_heap_cmp(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int main(void) {
    Heap min_heap = heap_create(sizeof(int), min_heap_cmp);

    int items[] = {50, 10, 40, 5};
    for (int i = 0; i < 4; i++) {
        heap_push(&min_heap, &items[i]);
    }

    int top;
    while (heap_pop(&min_heap, &top)) {
        printf("%d ", top); // Output: 5 10 40 50
    }
    printf("\n");

    heap_free(min_heap);
    return 0;
}
```

---

### 4. HashMap

An open-addressed key-value map. `hashmap_set` returns `1` when replacing an existing key value, or `0` when inserting a new key.

```c
#include <stdio.h>
#include <string.h>
#include "collections.h"

unsigned int string_hash(const void* key) {
    const char* str = *(const char**)key;
    unsigned int hash = 5381;
    int c;
    while ((c = *str++)) hash = ((hash << 5) + hash) + c;
    return hash;
}

int string_equals(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b) == 0;
}

int main(void) {
    HashMap map = hashmap_create(
        sizeof(char*), 
        sizeof(int), 
        string_equals, 
        string_hash
    );

    const char* key = "score";
    int val = 100;

    // Insert new entry
    hashmap_set(&map, &key, &val, NULL);

    // Update entry (returns 1 on replacement)
    int new_val = 150;
    int old_val;
    if (hashmap_set(&map, &key, &new_val, &old_val) == 1) {
        printf("Updated key '%s'. Old value was %d\n", key, old_val);
    }

    int out;
    if (hashmap_get(&map, &key, &out)) {
        printf("Current '%s': %d\n", key, out);
    }

    hashmap_free(map);
    return 0;
}
```

---

### 5. HashSet

A hash-based set guaranteeing unique entries.

```c
#include <stdio.h>
#include "collections.h"

unsigned int int_hash(const void* key) {
    unsigned int x = *(const unsigned int*)key;
    return ((x >> 16) ^ x) * 0x45d9f3b;
}

int int_equals(const void* a, const void* b) {
    return *(const int*)a == *(const int*)b;
}

int main(void) {
    HashSet set = hashset_create(sizeof(int), int_equals, int_hash);

    int val = 42;
    hashset_add(&set, &val);
    hashset_add(&set, &val); // Duplicate, ignored automatically

    if (hashset_has(&set, &val)) {
        printf("Set contains %d\n", val);
    }

    hashset_remove(&set, &val);
    printf("Contains after removal: %d\n", hashset_has(&set, &val));

    hashset_free(set);
    return 0;
}
```

---

## Convention & Design Notes

1. **Pass-by-value output buffers**: Methods such as `get`, `pop`, and `remove` take an `out` pointer to copy the retrieved memory into:
   ```c
   int out_value;
   hashmap_get(&map, &key, &out_value);
   ```
2. **Memory Safety**: Always call the corresponding `<collection>_free()` destructor to release heap-allocated buffers.