#pragma once
#include <stddef.h>

// headers allow for people to define their own malloc if needed

#ifndef ARENA_MALLOC
#include <stdlib.h>
#define ARENA_MALLOC malloc
#endif

#ifndef ARENA_FREE
#include <stdlib.h>
#define ARENA_FREE free
#endif

#ifndef ARENA_MEMCPY
#include <stdlib.h>
#define ARENA_MEMCPY memcpy
#endif

// Define a arena memory allocator with a pointer to memory
// Has the size and the current arena offset
typedef struct {
  char *mem;
  size_t offset;
  size_t size;
} Arena;

// Generate a new arena with a new heap buffer depending
Arena *init_arena(size_t size);
// Generate a new arena with a already preallocated memory buffer
Arena *init_arena_mem(size_t size, void *memArea);
// Allocate memory with a new size
void *arena_malloc(Arena *arena, size_t s);

// Free the arena pointer, should be called before terminating a
// arena pointer
void free_arena(Arena *arena);

// Set arena to allow overwriting of the buffer
void arena_reset(Arena *arena);
