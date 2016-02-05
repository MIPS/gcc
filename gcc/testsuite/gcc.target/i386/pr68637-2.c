/* { dg-do compile { target ia32 } } */

static void 
__attribute__ ((regparm (2)))
bar0 (int i, int j)
{
  if (i != 1 || j != 2)
    __builtin_abort ();
}

typedef void (*func_t) (int, int) __attribute__ ((regparm (2)));

func_t bar[] =
{
  bar0,
};

extern void foo (void);

int
main ()
{
  foo ();
  return 0;
}
