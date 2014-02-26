//* { dg-do run } */
/* { dg-options "-O2 -fno-inline" } */
#define T_VAL   8
#define F_VAL  16

#define TEST_FUNC(i, OP1, OP2)   \
int test_##i (unsigned a, unsigned b)      \
{                                \
  if (a OP1 10 && b OP2 20)      \
    return T_VAL;             \
  else                           \
    return F_VAL;            \
}

TEST_FUNC(1, >=, ==)
TEST_FUNC(2, >=, !=)
TEST_FUNC(3, >=, <=)
TEST_FUNC(4, >=, <)
TEST_FUNC(5, >=, >=)
TEST_FUNC(6, >=, >)


int main ()
{
  /* TEST_FUNC(1, >=, ==)  */
  if (test_1 (5, 20) != F_VAL)
    __builtin_abort ();
  if (test_1 (5, 30) != F_VAL)
    __builtin_abort ();
  if (test_1 (10, 20) != T_VAL)
    __builtin_abort ();
  if (test_1 (10, 30) != F_VAL)
    __builtin_abort ();
  if (test_1 (20, 20) != T_VAL)
    __builtin_abort ();
  if (test_1 (20, 15) != F_VAL)
    __builtin_abort ();

  /* TEST_FUNC(2, >=, !=)  */
  if (test_2 (5, 30) != F_VAL)
    __builtin_abort ();
  if (test_2 (5, 20) != F_VAL)
    __builtin_abort ();
  if (test_2 (10, 30) != T_VAL)
    __builtin_abort ();
  if (test_2 (10, 20) != F_VAL)
    __builtin_abort ();
  if (test_2 (20, 20) != F_VAL)
    __builtin_abort ();
  if (test_2 (20, 30) != T_VAL)
    __builtin_abort ();

  /* TEST_FUNC(3, >=, <=)  */
  if (test_3 (5, 15) != F_VAL)
    __builtin_abort ();
  if (test_3 (5, 20) != F_VAL)
    __builtin_abort ();
  if (test_3 (5, 30) != F_VAL)
    __builtin_abort ();
  if (test_3 (10, 15) != T_VAL)
    __builtin_abort ();
  if (test_3 (10, 20) != T_VAL)
    __builtin_abort ();
  if (test_3 (10, 30) != F_VAL)
    __builtin_abort ();
  if (test_3 (20, 15) != T_VAL)
    __builtin_abort ();
  if (test_3 (20, 20) != T_VAL)
    __builtin_abort ();
  if (test_3 (20, 30) != F_VAL)
    __builtin_abort ();

  /* TEST_FUNC(4, >=, <)  */
  if (test_4 (5, 15) != F_VAL)
    __builtin_abort ();
  if (test_4 (5, 30) != F_VAL)
    __builtin_abort ();
  if (test_4 (5, 20) != F_VAL)
    __builtin_abort ();
  if (test_4 (10, 15) != T_VAL)
    __builtin_abort ();
  if (test_4 (10, 30) != F_VAL)
    __builtin_abort ();
  if (test_4 (10, 20) != F_VAL)
    __builtin_abort ();
  if (test_4 (20, 20) != F_VAL)
    __builtin_abort ();
  if (test_4 (20, 15) != T_VAL)
    __builtin_abort ();
  if (test_4 (20, 30) != F_VAL)
    __builtin_abort ();

  /* TEST_FUNC(5, >=, >=)  */
  if (test_5 (5, 20) != F_VAL)
    __builtin_abort ();
  if (test_5 (5, 25) != F_VAL)
    __builtin_abort ();
  if (test_5 (5, 15) != F_VAL)
    __builtin_abort ();
  if (test_5 (10, 20) != T_VAL)
    __builtin_abort ();
  if (test_5 (10, 25) != T_VAL)
    __builtin_abort ();
  if (test_5 (10, 15) != F_VAL)
    __builtin_abort ();
  if (test_5 (20, 20) != T_VAL)
    __builtin_abort ();
  if (test_5 (20, 25) != T_VAL)
    __builtin_abort ();
  if (test_5 (20, 15) != F_VAL)
    __builtin_abort ();

  /* TEST_FUNC(6, >=, >)  */
  if (test_6 (5, 25) != F_VAL)
    __builtin_abort ();
  if (test_6 (5, 30) != F_VAL)
    __builtin_abort ();
  if (test_6 (5, 15) != F_VAL)
    __builtin_abort ();
  if (test_6 (10, 25) != T_VAL)
    __builtin_abort ();
  if (test_6 (10, 20) != F_VAL)
    __builtin_abort ();
  if (test_6 (10, 15) != F_VAL)
    __builtin_abort ();
  if (test_6 (20, 25) != T_VAL)
    __builtin_abort ();
  if (test_6 (20, 20) != F_VAL)
    __builtin_abort ();
  if (test_6 (20, 15) != F_VAL)
    __builtin_abort ();

  return 0;
}
