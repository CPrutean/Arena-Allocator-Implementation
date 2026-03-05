#include "arena.h"

Arena *initArena(size_t size) {
  Arena *arena = (Arena *)ARENA_MALLOC(sizeof(Arena));
  arena->mem = (char *)ARENA_MALLOC(sizeof(char) * size + 1);
  arena->size = size;
  arena->offset = 0;
  return arena;
}

Arena *initArenaWithMem(size_t size, void *memArea) {
  Arena *arena = (Arena *)ARENA_MALLOC(sizeof(Arena));
  arena->mem = (char *)memArea;
  arena->size = size;
  arena->offset = 0;
  return arena;
}

void *arenaAlloc(Arena *arena, size_t s) {
  if (arena == NULL || s > arena->size - arena->offset) {
    return NULL;
  }
  // Remember to pad memory size to align with 8 bits
  s += (8 - s % 8);
  void *pointer = arena->mem + arena->offset;
  arena->offset += s;
  return pointer;
}

void freeArena(Arena *arena) {
  ARENA_FREE(arena->mem);
  arena->offset = 0;
  arena->size = 0;
  arena->mem = 0;
}

void arenaReset(Arena *arena) { arena->offset = 0; }
