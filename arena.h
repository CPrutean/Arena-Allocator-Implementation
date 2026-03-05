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
Arena *initArena(size_t size);
// Generate a new arena with a already preallocated memory buffer
Arena *initArenaWithMem(size_t size, void *memArea);
// Allocate memory with a new size
void *arenaAlloc(Arena *arena, size_t s);

// Free the arena pointer, should be called before terminating a
// arena pointer
void freeArena(Arena *arena);

// Set arena to allow overwriting of the buffer
void arenaReset(Arena *arena);
