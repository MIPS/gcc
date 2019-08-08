#include "msa_1.h"

#define TEST(TYPE, NUM)                                       \
TYPE                                                          \
test_ ## TYPE ##_callee (TYPE vect, const TYPE* pvect)        \
{                                                             \
  if (__builtin_memcmp (&vect, pvect, sizeof vect))           \
    __builtin_abort();                                        \
  return *pvect;                                              \
}

ITERATE_FOR_ALL_TYPES(TEST)

#undef TEST
