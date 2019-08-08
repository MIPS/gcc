#include "msa_1.h"

#define INIT_2  {1, 2}
#define INIT_4  {1, 2, 3, 4}
#define INIT_8  {1, 2, 3, 4, 5, 6, 7, 8}
#define INIT_16 {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 ,14 ,15 ,16}

#define TEST(TYPE, NUM)                                         \
__attribute__ ((noinline)) static void                          \
test_ ## TYPE ##_caller (void)                                  \
{                                                               \
  extern TYPE test_ ## TYPE ##_callee (TYPE, const TYPE*);      \
  TYPE vect = (TYPE) INIT_ ## NUM;                              \
  TYPE res = test_ ## TYPE ## _callee (vect, &vect);            \
  if (__builtin_memcmp (&vect, &res, sizeof vect))              \
    __builtin_abort();                                          \
}

ITERATE_FOR_ALL_TYPES(TEST)

#undef TEST
#undef INIT_2
#undef INIT_4
#undef INIT_8
#undef INIT_16

#define RUN_TEST(TYPE, NUM) test_ ## TYPE ##_caller ();

void
test (void)
{
  ITERATE_FOR_ALL_TYPES(RUN_TEST)
}

#undef RUN_TEST
