extern void abort (void);

struct S
{
  char *s1;
  int s2;
};
struct T
{
  int t1;
  struct S *t2;
} *s1;

extern int bar (const struct T *, unsigned int, unsigned int,
		const struct T *, unsigned int, unsigned int);

extern inline void *
baz (void *x, const void *y, unsigned int z)
{
  unsigned char *s1 = x;
  const unsigned char *s2 = y;

  if (z > 4 || __builtin_constant_p (z))
    __builtin_memcpy (x, y, z);
  else
    switch (z)
      {
      case 4:
	s1[3] = s2[3];
      case 3:
	s1[2] = s2[2];
      case 2:
	s1[1] = s2[1];
      case 1:
	s1[0] = s2[0];
      case 0:
	break;
      }

  return x;
}

extern inline int
foo (struct T *b, unsigned int x, const void *y, unsigned int z)
{
  if (!b || !z)
    return 0;
  if (x == b->t1)
    {
      struct S *r = b->t2;
      baz (r->s1 + r->s2, y, z);
      return 1;
    }

  return 0;
}

int
test (struct T *a, struct T *b, struct T *c, struct T *d)
{
  if (!a)
    abort ();
  if (!b)
    abort ();

  if (bar (a, 1, a->t1, b, 1, b->t1) > 0)
    abort ();
  if (bar (a, 41, a->t1 - 40, b, 1, b->t1) > 0)
    abort ();

  if (!c)
    abort ();
  if (!d)
    abort ();

  if (bar (c, 1, c->t1, d, 1, d->t1) < 0)
    abort ();
  if (bar (c, 41, c->t1 - 40, d, 1, d->t1) < 0)
    abort ();

  foo (s1, 0, "abcd", 4);
  return 0;
}
