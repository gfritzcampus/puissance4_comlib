#ifndef _H_TEST
#define _H_TEST

#include <stdio.h>

#define VA_ARGS(...) , ##__VA_ARGS__
#define assert(cond, msg, ...) {\
  ++test_nb_asserts;\
  printf("  assert %03ld : "\
    msg\
    "\n", test_nb_asserts VA_ARGS(__VA_ARGS__));\
  if (!(cond)) {\
    printf("   ---> ASSERT FAIL at %d : \"%s\" is false \n"\
      , __LINE__, #cond );\
  }\
}

static size_t test_nb_asserts = 0;
static size_t test_nb_tests = 0;

static void setCurrentTest(const char * const test) {
  ++test_nb_tests;
  test_nb_asserts = 0;
  printf(" #%02ld %s\n", test_nb_tests, test);
}

#endif
