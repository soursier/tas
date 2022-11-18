#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include "tas.h"

#define FREE_BLOCK -1
#define INIT_VAL 0
void test_tas_malloc_example(){

    init_heap();
    char* heap = get_heap();
    char* p1 = tas_malloc(10);
    /*print_heap();*/

    CU_ASSERT(p1-1 == heap);
    CU_ASSERT(*(p1+1) != FREE_BLOCK);
    printf("getlibre() = %d",get_libre());
    CU_ASSERT(get_libre() == 10+1);
    CU_ASSERT(*(heap + get_libre()) == 116);
    CU_ASSERT(*(heap + get_libre() + 1) == FREE_BLOCK);

    char* p2 = tas_malloc(9);
    /*print_heap();*/

    CU_ASSERT(p2 == heap+12);
    CU_ASSERT(*(p2-1) == 9);
    CU_ASSERT(get_libre() == 21);

    char* p3 = tas_malloc(5);
    /*print_heap();*/

    CU_ASSERT(p3 == heap + 22);
    CU_ASSERT(*(p3-1) == 5);
    CU_ASSERT(get_libre() == 27);

    char* p4 = tas_malloc(101);
    /*print_heap();*/

    CU_ASSERT(p4 == NULL);
}
void test_tas_free_several(){

    init_heap();
    char* heap = get_heap();

    char* p1 = tas_malloc(10);
    char* p2 = tas_malloc(10);
    char* p3 = tas_malloc(10);
    char* p4 = tas_malloc(10);

    strcpy(p1,"tp1");
    strcpy(p2,"tp2");
    strcpy(p3,"tp3");
    strcpy(p4,"tp4");
    
    tas_free(p2); // simple free

    CU_ASSERT(*(p2-1) == 10);
    CU_ASSERT(*(p2) == FREE_BLOCK);
    CU_ASSERT(get_libre() == p2 - 1 - heap);

    print_heap_debug();
    tas_free(p3); // testing merge left
    print_heap_debug();

    CU_ASSERT(*(p2-1) == 21);
    CU_ASSERT(*(p2) == FREE_BLOCK);
    CU_ASSERT(get_libre() == p2 - 1 - heap);

    tas_free(p1); // testing merge right
    CU_ASSERT(*(p1-1) == 32);
    CU_ASSERT(*(p1) == FREE_BLOCK);
    CU_ASSERT(get_libre() == 0);

}


  int init_suite(void) { return 0; }
  int clean_suite(void) { return 0; }

int main()
{

  CU_pSuite pSuite = NULL;

  /* initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  /* add a suite to the registry */
  pSuite = CU_add_suite("Suite_1", init_suite, clean_suite);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* add the tests to the suite */
  if (
    NULL == CU_add_test(pSuite, "test_malloc", test_tas_malloc_example) ||
    NULL == CU_add_test(pSuite, "test_free", test_tas_free_several)
  )
  {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* Run all tests using the CUnit Basic interface */
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}