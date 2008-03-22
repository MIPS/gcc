/* { dg-do run } */
/* { dg-require-effective-target ilp32 } */
/* { dg-options "-O2" } */

extern void abort (void);

int test_nested (int i)
{
  int __attribute__ ((__noinline__, __regparm__(3))) foo (int j, int k, int l)
  {
    return i + j + k + l;
  }

  return foo(i, i+1, i+2) * i;
}

int __attribute__ ((__noinline__, __regparm__(3), __force_align_arg_pointer__))
test_realigned (int j, int k, int l)
{
  return j + k + l;
}

int main ()
{
  if (test_nested(10) != 430)
    abort ();

  if (test_realigned(10, 11, 12) != 33)
    abort ();

  return 0;
}
