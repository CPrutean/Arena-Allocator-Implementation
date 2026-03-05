#include "CUnit/Basic.h"
#include "CUnit/CUnit.h"
#include "CUnit/TestDB.h"
#include "arena.h"
#include <stdlib.h>

#define TESTING_SIZE 64
#define FALSE_TEST_SIZE 128
#define TRUE_TEST_SIZE 16

void testNullValues(void);
void testAlloc(void);
void testArenaInits(void);
void testArenaFree(void);
void testArenaReset(void);
static Arena *arena = NULL;
static Arena *arena2 = NULL;
static char *preMem = NULL;
int init_suite1();
int clean_suite1();

int main() {
  CU_pSuite pSuite = NULL;
  /* initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  /* add a suite to the registry */
  pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* add the tests to the suite */
  /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
  if ((NULL == CU_add_test(pSuite, "test of null safety", testNullValues) ||
       (NULL == CU_add_test(pSuite, "test of arena_malloc()", testAlloc)) ||
       (NULL == CU_add_test(pSuite, "test of initializing the arena",
                            testArenaInits))) ||
      (NULL == CU_add_test(pSuite, "test arenaFree()", testArenaFree)) ||
      (NULL == CU_add_test(pSuite, "Test arena_reset", testArenaReset))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* Run all tests using the CUnit Basic interface */
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
  return 0;
}

int init_suite1() {
  preMem = (char *)malloc(sizeof(char) * TESTING_SIZE);
  arena = init_arena(TESTING_SIZE);
  arena2 = init_arena_mem(TESTING_SIZE, preMem);

  if (preMem == NULL || arena == NULL || arena2 == NULL) {
    return -1;
  } else {
    return 0;
  }
}

int clean_suite1() {
  free_arena(arena);
  free_arena(arena2);
  return 0;
}

void testArenaFree() {
  free_arena(arena);
  free_arena(arena2);
  CU_ASSERT(arena->mem == NULL);
  CU_ASSERT(arena->offset == 0);
  CU_ASSERT(arena->size == 0);
  init_suite1();
  arena_reset(arena);
  arena_reset(arena2);
}

void testNullValues() {
  CU_ASSERT(arena_malloc(arena, FALSE_TEST_SIZE) == NULL);
  CU_ASSERT(arena_malloc(arena, FALSE_TEST_SIZE) == NULL);
  arena_reset(arena);
  arena_reset(arena2);
}

void testArenaInits() {
  CU_ASSERT(arena2->mem == preMem);
  CU_ASSERT(arena->size == TESTING_SIZE && arena2->size == TESTING_SIZE);
  CU_ASSERT(arena->offset == 0 && arena2->offset == 0);
  arena_reset(arena);
  arena_reset(arena2);
}

void testAlloc() {
  void *ptr1;
  ptr1 = arena_malloc(arena, TRUE_TEST_SIZE);
  CU_ASSERT(ptr1 != NULL);

  void *ptr2;
  ptr2 = arena_malloc(arena2, TRUE_TEST_SIZE);
  CU_ASSERT(ptr1 != NULL);

  for (int i = 0; i < 3; i++) {
    ptr1 = arena_malloc(arena, TRUE_TEST_SIZE);
    CU_ASSERT(ptr1 != NULL);

    ptr2 = arena_malloc(arena2, TRUE_TEST_SIZE);
    CU_ASSERT(ptr2 != NULL);
  }

  arena_reset(arena);
  arena_reset(arena2);

  arena_malloc(arena, 5);
  arena_malloc(arena2, 5);
  CU_ASSERT(arena->offset == 8);
  CU_ASSERT(arena2->offset == 8);

  arena_reset(arena);
  arena_reset(arena2);
}

void testArenaReset() {
  arena_reset(arena);
  arena_reset(arena2);
  CU_ASSERT(arena->offset == 0);
  CU_ASSERT(arena2->offset == 0);
}
