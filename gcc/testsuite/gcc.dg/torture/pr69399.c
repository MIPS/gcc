/* { dg-do run { target int128 } } */

typedef __UINT64_TYPE__ u64;
typedef unsigned __int128 u128;

static unsigned __attribute__((noinline, noclone))
foo(u64 u)
{
  u128 v = u | 0xffffff81;
  v >>= 64;
  return v;
}

int
main()
{
  unsigned x = foo(27);
  if (x != 0)
    __builtin_abort();
  return 0;
}
