# Collections C Library

A generic collection and memory-utility library written in C99/C11. Elements are stored as raw bytes, so each collection is configured with the size of its element type. Hash-based collections also receive caller-provided equality and hash callbacks; ordered collections receive a comparator where required.

The library has no third-party dependencies and uses only the C standard library.

## Features

- **String**: owned, growable strings with copying, concatenation, trimming, substring, case conversion, searching, comparison, hashing, and character iteration.
- **ArrayList**: resizable indexed storage with append/pop, positional get/set/insert/remove, swapping, shuffling, copying, sorting, binary search, and iteration.
- **ArrayDeque**: resizable circular double-ended queue with append/pop operations on both ends, copying, and iteration.
- **Heap**: generic binary priority queue with caller-defined ordering, copying, and iteration.
- **HashMap**: open-addressed key/value storage with updates, lookup, removal, copying, and key/value/item callbacks.
- **HashSet**: open-addressed unique-value storage with copying, iteration, and union/intersection/difference operations.
- **FrozenHashSet**: copied set storage with cached hash, equality, membership, set operations, and iteration.
- **Trie**: string storage with duplicate-aware length, complete-word lookup, and prefix lookup; accepts C strings and the library's `String`.
- **ObjectPool**: fixed-capacity allocation and slot reuse for same-sized objects.
- **ArenaPool**: bounded byte allocations released together.
- **ByteArray**: growable byte storage with reserve, cursor-based reads/writes, seeking, and available-byte reporting.
- **BitSet**: bit access, setting, flipping, fill/clear, copying, logical operations, and aggregate state queries.
- **Generic helpers**: built-in equality, hashing, comparison, and integer/number min/max helpers.

## Project layout

```text
.
├── src/
│   ├── collections.c
│   └── collections.h
└── tests/
    ├── arenapool_test.c
    ├── arraydeque_test.c
    ├── arraylist_test.c
    ├── bitset_test.c
    ├── bytearray_test.c
    ├── frozenhashset_test.c
    ├── hashmap_test.c
    ├── hashset_test.c
    ├── heap_test.c
    ├── objpool_test.c
    ├── string_test.c
    └── trie_test.c
```

## Build and run

Each test file is a separate executable with its own `main` function. Compile an individual suite with GCC or Clang:

```sh
gcc -std=c11 -Isrc tests/arraylist_test.c src/collections.c -o run_tests
./run_tests
```

## Create .a file WINDOWS
```sh
gcc -c -O3 src/collections.c -Iinclude -o lib/collections.o && ar rcs lib/libcollections.a lib/collections.o
```

### Run every suite on macOS or Linux

```sh
executable="$(mktemp)"
trap 'rm -f "$executable"' EXIT
for source in tests/*.c; do
    gcc -std=c11 -Isrc "$source" src/collections.c -o "$executable" || exit 1
    "$executable" || exit 1
done
```

```sh
for file in ./tests/*.c; do gcc -Isrc "$file" ./src/collections.c -o test_bin && ./test_bin || break; done;
```

### Run every suite in Windows PowerShell

```powershell
$failed = $false
Get-ChildItem .\tests\*.c | ForEach-Object {
    $executable = Join-Path $env:TEMP ($_.BaseName + ".exe")
    gcc -std=c11 -Isrc $_.FullName .\src\collections.c -o $executable
    if ($LASTEXITCODE -ne 0) {
        $failed = $true
    } else {
        & $executable
        if ($LASTEXITCODE -ne 0) {
            $failed = $true
        }
    }
    Remove-Item $executable -ErrorAction SilentlyContinue
}
if ($failed) { exit 1 }
```

The test programs use `assert`; compile them without defining `NDEBUG` so their checks remain active.

## Test coverage

All test sources and console messages are in English. Each suite checks normal operations, boundary conditions, and cleanup for its corresponding API:

| Test suite | Main coverage |
|---|---|
| `string_test.c` | Creation, empty strings, copy/equality/hash, append/extend/clear, trim/substring/concatenate, case conversion, searching, prefix/suffix, comparison, iteration, and repeated workloads. |
| `arraylist_test.c` | Capacity and length, append/pop, indexed access and mutation, insertion/removal, clear/swap, sort/search/shuffle, callback indices, independent copies, and a one-million-element benchmark. |
| `arraydeque_test.c` | FIFO/LIFO operations at both ends, underflow, resize, generic struct values, iteration order, length, and independent copies. |
| `heap_test.c` | Min-heap and max-heap ordering, empty pop, resizing, struct elements, length, iteration, and independent copies. |
| `hashmap_test.c` | Integer, C-string, `String`, and struct keys; insert/update/get/remove; resizing and tombstones; copying; key/value/item callbacks; and mutable value iteration. |
| `hashset_test.c` | Insertion, duplicate suppression, membership/removal, independent copy, union/intersection/difference, iteration, and bulk operations. |
| `frozenhashset_test.c` | Creation, length, membership, cached hashes, equality independent of insertion order, set operations, iteration, and bulk validation. |
| `trie_test.c` | C-string and `String` insertion/search, complete words versus prefixes, duplicate insertion, missing entries, and empty-string behavior. |
| `objpool_test.c` | Allocation, object values, remaining capacity, exhaustion, free-list reuse, and repeated full-pool allocation/release. |
| `arenapool_test.c` | Creation, available space, sequential allocations, out-of-memory behavior, alignment, data integrity, and repeated allocations. |
| `bytearray_test.c` | Lifecycle, reserve, growth, byte contents, cursor movement, overwrite, partial reads, read bounds, available bytes, and repeated single-byte writes. |
| `bitset_test.c` | Bits at word boundaries, set/get/flip, clear/fill, any/all/none, copy, and AND/OR/XOR/NOT. |

### Million-element performance test

`arraylist_test.c` appends and reads **1,000,000 integers**, sorts the reverse-ordered collection, verifies every sorted value, and reports the total and sorting CPU times. The test intentionally reports measurements instead of enforcing a machine-dependent time limit.

The expanded tests also cover fixes for empty-list sorting and merge-sort runs that do not have a matching pair, preservation of the logical size in copied deques, and the shared `BitSet` definition that previously prevented the library from compiling with its public header.

## Basic usage

```c
#include "collections.h"

int main(void) {
    ArrayList values = arraylist_create(sizeof(int), 0);
    int first = 10;
    int second = 20;

    arraylist_append(&values, &first);
    arraylist_append(&values, &second);

    int result = 0;
    if (arraylist_get(&values, 1, &result)) {
        /* result is 20 */
    }

    arraylist_free(&values);
    return 0;
}
```

Pass output buffers to operations that retrieve elements. Collections copy the configured number of bytes; when storing pointers, the collection copies the pointer value and does not take ownership of the pointed-to object. Call the matching `*_free` function for every initialized collection.

Hash maps and sets need equality and hash callbacks compatible with the configured key/element type. Heaps and sorting operations need a comparator whose sign indicates ordering. Callback iteration order is unspecified for hash-based collections and heaps.
