#include "arena.h"

Arena *init_arena(size_t size) {
  Arena *arena = (Arena *)ARENA_MALLOC(sizeof(Arena));
  arena->mem = (char *)ARENA_MALLOC(sizeof(char) * size + 1);
  arena->size = size;
  arena->offset = 0;
  return arena;
}

Arena *init_arena_mem(size_t size, void *memArea) {
  Arena *arena = (Arena *)ARENA_MALLOC(sizeof(Arena));
  arena->mem = (char *)memArea;
  arena->size = size;
  arena->offset = 0;
  return arena;
}

void *arena_malloc(Arena *arena, size_t s) {
  if (arena == NULL || s > arena->size - arena->offset) {
    return NULL;
  }
  // Remember to pad memory size to align with 8 bits
  s += (8 - s % 8);
  void *pointer = arena->mem + arena->offset;
  arena->offset += s;
  return pointer;
}

void free_arena(Arena *arena) {
  ARENA_FREE(arena->mem);
  arena->offset = 0;
  arena->size = 0;
  arena->mem = 0;
}

void arena_reset(Arena *arena) { arena->offset = 0; }
