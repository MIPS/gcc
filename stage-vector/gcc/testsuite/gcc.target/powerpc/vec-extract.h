#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <altivec.h>

static void
check (TYPE expected, TYPE got)
{
  if (expected != got)
    abort ();
}

static vector TYPE  deoptimize     (vector TYPE)	__attribute__((__noinline__));
static vector TYPE *deoptimize_ptr (vector TYPE *)	__attribute__((__noinline__));

static vector TYPE
deoptimize (vector TYPE a)
{
  __asm__ (" # %x0" : "+wa" (a));
  return a;
}

static vector TYPE *
deoptimize_ptr (vector TYPE *p)
{
  __asm__ (" # %0" : "+r" (p));
  return p;
}


TYPE
get_auto_n (vector TYPE a, ssize_t n)
{
  return vec_extract (a, n);
}

TYPE
get_auto_0 (vector TYPE a)
{
  return vec_extract (a, 0);
}

TYPE
get_auto_1 (vector TYPE a)
{
  return vec_extract (a, 1);
}

#if ELEMENTS >= 4
TYPE
get_auto_2 (vector TYPE a)
{
  return vec_extract (a, 2);
}

TYPE
get_auto_3 (vector TYPE a)
{
  return vec_extract (a, 3);
}

#if ELEMENTS >= 8
TYPE
get_auto_4 (vector TYPE a)
{
  return vec_extract (a, 4);
}

TYPE
get_auto_5 (vector TYPE a)
{
  return vec_extract (a, 5);
}

TYPE
get_auto_6 (vector TYPE a)
{
  return vec_extract (a, 6);
}

TYPE
get_auto_7 (vector TYPE a)
{
  return vec_extract (a, 7);
}

#if ELEMENTS >= 16
TYPE
get_auto_8 (vector TYPE a)
{
  return vec_extract (a, 8);
}

TYPE
get_auto_9 (vector TYPE a)
{
  return vec_extract (a, 9);
}

TYPE
get_auto_10 (vector TYPE a)
{
  return vec_extract (a, 10);
}

TYPE
get_auto_11 (vector TYPE a)
{
  return vec_extract (a, 11);
}

TYPE
get_auto_12 (vector TYPE a)
{
  return vec_extract (a, 12);
}

TYPE
get_auto_13 (vector TYPE a)
{
  return vec_extract (a, 13);
}

TYPE
get_auto_14 (vector TYPE a)
{
  return vec_extract (a, 14);
}

TYPE
get_auto_15 (vector TYPE a)
{
  return vec_extract (a, 15);
}

#endif
#endif
#endif

typedef TYPE (*auto_func_type) (vector TYPE);

static auto_func_type get_auto_const[] = {
  get_auto_0,
  get_auto_1,
#if ELEMENTS >= 4
  get_auto_2,
  get_auto_3,
#if ELEMENTS >= 8
  get_auto_4,
  get_auto_5,
  get_auto_6,
  get_auto_7,
#if ELEMENTS >= 16
  get_auto_8,
  get_auto_9,
  get_auto_10,
  get_auto_11,
  get_auto_12,
  get_auto_13,
  get_auto_14,
  get_auto_15,
#endif
#endif
#endif
};

extern void do_auto (vector TYPE a) __attribute__((__noinline__));

void
do_auto (vector TYPE a)
{
  size_t i;

  for (i = 0; i < sizeof (get_auto_const) / sizeof (get_auto_const[0]); i++)
    check (get_auto_n (a, i),  (get_auto_const[i]) (a));
}


TYPE
get_pointer_n (vector TYPE *p, ssize_t n)
{
  return vec_extract (*p, n);
}

TYPE
get_pointer_0 (vector TYPE *p)
{
  return vec_extract (*p, 0);
}

TYPE
get_pointer_1 (vector TYPE *p)
{
  return vec_extract (*p, 1);
}

#if ELEMENTS >= 4
TYPE
get_pointer_2 (vector TYPE *p)
{
  return vec_extract (*p, 2);
}

TYPE
get_pointer_3 (vector TYPE *p)
{
  return vec_extract (*p, 3);
}

#if ELEMENTS >= 8
TYPE
get_pointer_4 (vector TYPE *p)
{
  return vec_extract (*p, 4);
}

static TYPE
get_pointer_5 (vector TYPE *p)
{
  return vec_extract (*p, 5);
}

TYPE
get_pointer_6 (vector TYPE *p)
{
  return vec_extract (*p, 6);
}

TYPE
get_pointer_7 (vector TYPE *p)
{
  return vec_extract (*p, 7);
}

#if ELEMENTS >= 16
TYPE
get_pointer_8 (vector TYPE *p)
{
  return vec_extract (*p, 8);
}

TYPE
get_pointer_9 (vector TYPE *p)
{
  return vec_extract (*p, 9);
}

TYPE
get_pointer_10 (vector TYPE *p)
{
  return vec_extract (*p, 10);
}

TYPE
get_pointer_11 (vector TYPE *p)
{
  return vec_extract (*p, 11);
}

TYPE
get_pointer_12 (vector TYPE *p)
{
  return vec_extract (*p, 12);
}

TYPE
get_pointer_13 (vector TYPE *p)
{
  return vec_extract (*p, 13);
}

TYPE
get_pointer_14 (vector TYPE *p)
{
  return vec_extract (*p, 14);
}

TYPE
get_pointer_15 (vector TYPE *p)
{
  return vec_extract (*p, 15);
}

#endif
#endif
#endif

typedef TYPE (*pointer_func_type) (vector TYPE *);

static pointer_func_type get_pointer_const[] = {
  get_pointer_0,
  get_pointer_1,
#if ELEMENTS >= 4
  get_pointer_2,
  get_pointer_3,
#if ELEMENTS >= 8
  get_pointer_4,
  get_pointer_5,
  get_pointer_6,
  get_pointer_7,
#if ELEMENTS >= 16
  get_pointer_8,
  get_pointer_9,
  get_pointer_10,
  get_pointer_11,
  get_pointer_12,
  get_pointer_13,
  get_pointer_14,
  get_pointer_15,
#endif
#endif
#endif
};

extern void do_pointer (vector TYPE *p) __attribute__((__noinline__));

void
do_pointer (vector TYPE *p)
{
  size_t i;

  for (i = 0; i < sizeof (get_pointer_const) / sizeof (get_pointer_const[0]); i++)
    check (get_pointer_n (p, i),  (get_pointer_const[i]) (p));
}


TYPE
get_indexed_n (vector TYPE *p, size_t x, ssize_t n)
{
  return vec_extract (p[x], n);
}

TYPE
get_indexed_0 (vector TYPE *p, size_t x)
{
  return vec_extract (p[x], 0);
}

TYPE
get_indexed_1 (vector TYPE *p, size_t x)
{
  return vec_extract (p[x], 1);
}

#if ELEMENTS >= 4
TYPE
get_indexed_2 (vector TYPE *p, size_t x)
{
  return vec_extract (p[x], 2);
}

TYPE
get_indexed_3 (vector TYPE *p, size_t x)
{
  return vec_extract (p[x], 3);
}

#if ELEMENTS >= 8
TYPE
get_indexed_4 (vector TYPE *p, size_t x)
{
  return vec_extract (p[x], 4);
}

static TYPE
get_indexed_5 (vector TYPE *p, size_t x)
{
  return vec_extract (p[x], 5);
}

TYPE
get_indexed_6 (vector TYPE *p, size_t x)
{
  return vec_extract (p[x], 6);
}

TYPE
get_indexed_7 (vector TYPE *p, size_t x)
{
  return vec_extract (p[x], 7);
}

#if ELEMENTS >= 16
TYPE
get_indexed_8 (vector TYPE *p, size_t x)
{
  return vec_extract (p[x], 8);
}

TYPE
get_indexed_9 (vector TYPE *p, size_t x)
{
  return vec_extract (p[x], 9);
}

TYPE
get_indexed_10 (vector TYPE *p, size_t x)
{
  return vec_extract (p[x], 10);
}

TYPE
get_indexed_11 (vector TYPE *p, size_t x)
{
  return vec_extract (p[x], 11);
}

TYPE
get_indexed_12 (vector TYPE *p, size_t x)
{
  return vec_extract (p[x], 12);
}

TYPE
get_indexed_13 (vector TYPE *p, size_t x)
{
  return vec_extract (p[x], 13);
}

TYPE
get_indexed_14 (vector TYPE *p, size_t x)
{
  return vec_extract (p[x], 14);
}

TYPE
get_indexed_15 (vector TYPE *p, size_t x)
{
  return vec_extract (p[x], 15);
}

#endif
#endif
#endif

typedef TYPE (*indexed_func_type) (vector TYPE *, size_t);

static indexed_func_type get_indexed_const[] = {
  get_indexed_0,
  get_indexed_1,
#if ELEMENTS >= 4
  get_indexed_2,
  get_indexed_3,
#if ELEMENTS >= 8
  get_indexed_4,
  get_indexed_5,
  get_indexed_6,
  get_indexed_7,
#if ELEMENTS >= 16
  get_indexed_8,
  get_indexed_9,
  get_indexed_10,
  get_indexed_11,
  get_indexed_12,
  get_indexed_13,
  get_indexed_14,
  get_indexed_15,
#endif
#endif
#endif
};

extern void do_indexed (vector TYPE *p, size_t x) __attribute__((__noinline__));

void
do_indexed (vector TYPE *p, size_t x)
{
  size_t i;

  for (i = 0; i < sizeof (get_indexed_const) / sizeof (get_indexed_const[0]); i++)
    check (get_indexed_n (p, x, i),  (get_indexed_const[i]) (p, x));
}


static vector TYPE s;

TYPE
get_static_n (ssize_t n)
{
  return vec_extract (s, n);
}

TYPE
get_static_0 (void)
{
  return vec_extract (s, 0);
}

TYPE
get_static_1 (void)
{
  return vec_extract (s, 1);
}

#if ELEMENTS >= 4
TYPE
get_static_2 (void)
{
  return vec_extract (s, 2);
}

TYPE
get_static_3 (void)
{
  return vec_extract (s, 3);
}

#if ELEMENTS >= 8
TYPE
get_static_4 (void)
{
  return vec_extract (s, 4);
}

TYPE
get_static_5 (void)
{
  return vec_extract (s, 5);
}

TYPE
get_static_6 (void)
{
  return vec_extract (s, 6);
}

TYPE
get_static_7 (void)
{
  return vec_extract (s, 7);
}

#if ELEMENTS >= 16
TYPE
get_static_8 (void)
{
  return vec_extract (s, 8);
}

TYPE
get_static_9 (void)
{
  return vec_extract (s, 9);
}

TYPE
get_static_10 (void)
{
  return vec_extract (s, 10);
}

TYPE
get_static_11 (void)
{
  return vec_extract (s, 11);
}

TYPE
get_static_12 (void)
{
  return vec_extract (s, 12);
}

TYPE
get_static_13 (void)
{
  return vec_extract (s, 13);
}

TYPE
get_static_14 (void)
{
  return vec_extract (s, 14);
}

TYPE
get_static_15 (void)
{
  return vec_extract (s, 15);
}

#endif
#endif
#endif

typedef TYPE (*static_func_type) (void);

static static_func_type get_static_const[] = {
  get_static_0,
  get_static_1,
#if ELEMENTS >= 4
  get_static_2,
  get_static_3,
#if ELEMENTS >= 8
  get_static_4,
  get_static_5,
  get_static_6,
  get_static_7,
#if ELEMENTS >= 16
  get_static_8,
  get_static_9,
  get_static_10,
  get_static_11,
  get_static_12,
  get_static_13,
  get_static_14,
  get_static_15,
#endif
#endif
#endif
};

extern void do_static (void) __attribute__((__noinline__));

void
do_static (void)
{
  size_t i;

  for (i = 0; i < sizeof (get_static_const) / sizeof (get_static_const[0]); i++)
    check (get_static_n (i),  (get_static_const[i]) ());
}


vector TYPE g;

TYPE
get_global_n (ssize_t n)
{
  return vec_extract (g, n);
}

TYPE
get_global_0 (void)
{
  return vec_extract (g, 0);
}

TYPE
get_global_1 (void)
{
  return vec_extract (g, 1);
}

#if ELEMENTS >= 4
TYPE
get_global_2 (void)
{
  return vec_extract (g, 2);
}

TYPE
get_global_3 (void)
{
  return vec_extract (g, 3);
}

#if ELEMENTS >= 8
TYPE
get_global_4 (void)
{
  return vec_extract (g, 4);
}

TYPE
get_global_5 (void)
{
  return vec_extract (g, 5);
}

TYPE
get_global_6 (void)
{
  return vec_extract (g, 6);
}

TYPE
get_global_7 (void)
{
  return vec_extract (g, 7);
}

#if ELEMENTS >= 16
TYPE
get_global_8 (void)
{
  return vec_extract (g, 8);
}

TYPE
get_global_9 (void)
{
  return vec_extract (g, 9);
}

TYPE
get_global_10 (void)
{
  return vec_extract (g, 10);
}

TYPE
get_global_11 (void)
{
  return vec_extract (g, 11);
}

TYPE
get_global_12 (void)
{
  return vec_extract (g, 12);
}

TYPE
get_global_13 (void)
{
  return vec_extract (g, 13);
}

TYPE
get_global_14 (void)
{
  return vec_extract (g, 14);
}

TYPE
get_global_15 (void)
{
  return vec_extract (g, 15);
}

#endif
#endif
#endif

typedef TYPE (*global_func_type) (void);

static global_func_type get_global_const[] = {
  get_global_0,
  get_global_1,
#if ELEMENTS >= 4
  get_global_2,
  get_global_3,
#if ELEMENTS >= 8
  get_global_4,
  get_global_5,
  get_global_6,
  get_global_7,
#if ELEMENTS >= 16
  get_global_8,
  get_global_9,
  get_global_10,
  get_global_11,
  get_global_12,
  get_global_13,
  get_global_14,
  get_global_15,
#endif
#endif
#endif
};

extern void do_global (void) __attribute__((__noinline__));

void
do_global (void)
{
  size_t i;

  for (i = 0; i < sizeof (get_global_const) / sizeof (get_global_const[0]); i++)
    check (get_global_n (i),  (get_global_const[i]) ());
}


int
main (void)
{
  size_t i;
  vector TYPE x = INITIAL;
  vector TYPE *p, *p2, a, y;
  vector TYPE z[2];

  a = deoptimize (x);
  s = deoptimize (x);
  g = deoptimize (x);
  y = deoptimize (x);
  z[0] = deoptimize (x);
  z[1] = deoptimize (x);
  p = deoptimize_ptr (&y);
  p2 = deoptimize_ptr (&z[0]);

  do_auto (a);
  do_pointer (p);
  for (i = 0; i < 2; i++)
    do_indexed (p2, i);
  do_static ();
  do_global ();
  return 0;
}
