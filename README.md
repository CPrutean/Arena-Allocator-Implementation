# Arena Allocator

A simple arena (region-based) memory allocator written in C. Arena allocation lets you batch-allocate memory into a single contiguous buffer and free it all at once, avoiding the overhead of repeated `malloc`/`free` calls.

## How It Works

An arena holds a fixed-size memory buffer (`mem`), a current write position (`offset`), and the total buffer size (`size`). Allocations advance the offset forward; individual allocations cannot be freed — the entire arena is reset or freed at once. All allocations are 8-byte aligned.

## Files

| File               | Description                                                                                                                            |
| ------------------ | -------------------------------------------------------------------------------------------------------------------------------------- |
| `arena.h`          | Public API — struct definition and function declarations. Supports overriding `malloc`, `free`, and `memcpy` via preprocessor defines. |
| `arena.c`          | Implementation of all arena functions.                                                                                                 |
| `tests.c`          | CUnit test suite covering null-safety, allocation, initialization, freeing, and reset behavior.                                        |
| `compile_tests.sh` | Shell script to compile the test binary using GCC and CUnit (Homebrew path).                                                           |
| `test_add`         | Compiled test binary (output of `compile_tests.sh`).                                                                                   |
| `a.out`            | Default compiled output artifact.                                                                                                      |

## API

```c
// Create a new arena backed by a freshly heap-allocated buffer
Arena *initArena(size_t size);

// Create a new arena backed by an existing memory buffer
Arena *initArenaWithMem(size_t size, void *memArea);

// Allocate `s` bytes from the arena (returns NULL if out of space)
void *arenaAlloc(Arena *arena, size_t s);

// Free the arena's backing buffer and zero out its fields
void freeArena(Arena *arena);

// Reset the offset to 0, allowing the buffer to be reused
void arenaReset(Arena *arena);
```

## Customizing the Allocator

Before including `arena.h`, you can override the underlying allocator macros:

```c
#define ARENA_MALLOC  my_malloc
#define ARENA_FREE    my_free
#define ARENA_MEMCPY  my_memcpy
#include "arena.h"
```

If not defined, these default to the standard library equivalents.

## Building & Running Tests

Tests use [CUnit](http://cunit.sourceforge.net/). Install it via Homebrew:

```sh
brew install cunit
```

Then compile and run:

```sh
bash compile_tests.sh
./test_add
```

The test suite covers:

- **`testNullValues`** — allocating more than the arena size returns `NULL`
- **`testAlloc`** — successful allocations and 8-byte alignment padding
- **`testArenaInits`** — both init paths (`initArena` and `initArenaWithMem`) set fields correctly
- **`testArenaFree`** — `freeArena` zeroes out `mem`, `offset`, and `size`
- **`testArenaReset`** — `arenaReset` sets `offset` back to `0` without freeing memory
