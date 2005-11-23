/* { dg-do run } */
/* { dg-options "-O2" } */
/* { dg-options "-O2 -march=pentium4" { target i?86-*-* } } */

struct S
{
  unsigned int s1[2];
  unsigned int s2[8];
};

extern void abort (void);
extern void exit (int);

void
check (void *x, const unsigned char *y, unsigned int z)
{
  int i;
  if (y[0] != 0x80)
    abort ();
  for (i = 1; i < 64; ++i)
    if (y[i] != 0)
      abort ();
  exit (0);
}

void
test (struct S *x, unsigned char *y)
{
  unsigned char a[8];
  unsigned int b, c, d;
  int i, j;
  const unsigned char e[64] = { 0x80, };

  d = x->s1[0];
  a[7] = d;
  d >>= 8;
  a[6] = d;
  d >>= 8;
  a[5] = d;
  d >>= 8;
  a[4] = d;
  d = x->s1[1];
  a[3] = d;
  d >>= 8;
  a[2] = d;
  d >>= 8;
  a[1] = d;
  d >>= 8;
  a[0] = d;

  b = (x->s1[0] >> 3) & 0x3f;
  c = b < 56 ? 56 - b : 120 - b;
  check (x, e, c);
  check (x, a, 8);

  for (i = j = 0; i < 8; i++, j += 4)
    {
      d = x->s2[i];
      y[j + 3] = d;
      d >>= 8;
      y[j + 2] = d;
      d >>= 8;
      y[j + 1] = d;
      d >>= 8;
      y[j] = d;
    }
  __builtin_memset (x, 0, sizeof (*x));
}

void
do_test (void)
{
  struct S s;
  __builtin_memset (&s, 0, sizeof (s));
  test (&s, (void *) 0);
}

#ifdef __i386__
void
bail_if_no_sse2 (void)
{
  int fl1, fl2;

  /* See if we can use cpuid.  */
  __asm__ ("pushfl; pushfl; popl %0; movl %0,%1; xorl %2,%0;"
           "pushl %0; popfl; pushfl; popl %0; popfl"
           : "=&r" (fl1), "=&r" (fl2)
           : "i" (0x00200000));
  if (((fl1 ^ fl2) & 0x00200000) == 0)
    exit (0);

  /* See if cpuid gives capabilities.  */
  __asm__ ("cpuid" : "=a" (fl1) : "0" (0) : "ebx", "ecx", "edx", "cc");
  if (fl1 == 0)
    exit (0);

  /* See if capabilities include SSE2 (26th bit; 25 for SSE).  */
  __asm__ ("cpuid" : "=a" (fl1), "=d" (fl2) : "0" (1) : "ebx", "ecx", "cc");
  if ((fl2 & (1 << 26)) == 0)
    exit (0);
}
#else
#define bail_if_no_sse2() do { } while (0)
#endif

int
main (void)
{
  bail_if_no_sse2 ();
  do_test ();
  return 1;
}
