/* Test jump-table-opt link-time relaxation.  */
/* { dg-do run } */
/* { dg-options "-march=32r6 -m32 -ffat-lto-objects" } */
/* { dg-additional-options "-mjump-table-opt -fno-pic -mpid" } */

#define F(n)								\
__attribute__((noclone, noinline)) int					\
f##n (int fa1, int fa2, int fa3, int fa4, int fa5, int fa6, int fa7, int fa8) \
{									\
  a0[0] = fa2 + fa3 * fa4;			\
  return n;					\
}

#define TESTS					\
F (0) \
F (1) \
F (2) \
F (3) \
F (4) \
F (5)

/* Declare a large array to blow up the common section.  */
int a0[524288];
/* All of these end-up outside direct GP-relative range.  */
volatile int a1, a2, a3, a4, a5, a6, a7, a8;

TESTS
#undef F
#define F(n) case n: ret = f##n (a1+a2, a2, a3+a4, a4, a5+a6, a6, a7*a8, a8-a1); break;

int
main (void) 
{
  int i;
  for (i = 0; i <= 5; i++)
    {
      int ret;
      switch (i) {
	TESTS
	default: ret = 0; break;
	  }
      if (ret != i)
	return 1;
    }
  return 0;
}
