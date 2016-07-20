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
  return a;
}

static vector TYPE *
deoptimize_ptr (vector TYPE *p)
{
  return p;
}


static void do_auto     (vector TYPE)		__attribute__((__noinline__));
static TYPE get_auto_n  (vector TYPE, ssize_t)	__attribute__((__noinline__));
static TYPE get_auto_0  (vector TYPE)		__attribute__((__noinline__));
static TYPE get_auto_1  (vector TYPE)		__attribute__((__noinline__));
#if ELEMENTS >= 4
static TYPE get_auto_2  (vector TYPE)		__attribute__((__noinline__));
static TYPE get_auto_3  (vector TYPE)		__attribute__((__noinline__));
#if ELEMENTS >= 8
static TYPE get_auto_4  (vector TYPE)		__attribute__((__noinline__));
static TYPE get_auto_5  (vector TYPE)		__attribute__((__noinline__));
static TYPE get_auto_6  (vector TYPE)		__attribute__((__noinline__));
static TYPE get_auto_7  (vector TYPE)		__attribute__((__noinline__));
#if ELEMENTS >= 16
static TYPE get_auto_8  (vector TYPE)		__attribute__((__noinline__));
static TYPE get_auto_9  (vector TYPE)		__attribute__((__noinline__));
static TYPE get_auto_10 (vector TYPE)		__attribute__((__noinline__));
static TYPE get_auto_11 (vector TYPE)		__attribute__((__noinline__));
static TYPE get_auto_12 (vector TYPE)		__attribute__((__noinline__));
static TYPE get_auto_13 (vector TYPE)		__attribute__((__noinline__));
static TYPE get_auto_14 (vector TYPE)		__attribute__((__noinline__));
static TYPE get_auto_15 (vector TYPE)		__attribute__((__noinline__));
#endif
#endif
#endif

static TYPE
get_auto_n (vector TYPE a, ssize_t n)
{
  return vec_extract (a, n);
}

static TYPE
get_auto_0 (vector TYPE a)
{
  return vec_extract (a, 0);
}

static TYPE
get_auto_1 (vector TYPE a)
{
  return vec_extract (a, 1);
}

#if ELEMENTS >= 4
static TYPE
get_auto_2 (vector TYPE a)
{
  return vec_extract (a, 2);
}

static TYPE
get_auto_3 (vector TYPE a)
{
  return vec_extract (a, 3);
}

#if ELEMENTS >= 8
static TYPE
get_auto_4 (vector TYPE a)
{
  return vec_extract (a, 4);
}

static TYPE
get_auto_5 (vector TYPE a)
{
  return vec_extract (a, 5);
}

static TYPE
get_auto_6 (vector TYPE a)
{
  return vec_extract (a, 6);
}

static TYPE
get_auto_7 (vector TYPE a)
{
  return vec_extract (a, 7);
}

#if ELEMENTS >= 16
static TYPE
get_auto_8 (vector TYPE a)
{
  return vec_extract (a, 8);
}

static TYPE
get_auto_9 (vector TYPE a)
{
  return vec_extract (a, 9);
}

static TYPE
get_auto_10 (vector TYPE a)
{
  return vec_extract (a, 10);
}

static TYPE
get_auto_11 (vector TYPE a)
{
  return vec_extract (a, 11);
}

static TYPE
get_auto_12 (vector TYPE a)
{
  return vec_extract (a, 12);
}

static TYPE
get_auto_13 (vector TYPE a)
{
  return vec_extract (a, 13);
}

static TYPE
get_auto_14 (vector TYPE a)
{
  return vec_extract (a, 14);
}

static TYPE
get_auto_15 (vector TYPE a)
{
  return vec_extract (a, 15);
}

#endif
#endif
#endif

static void
do_auto (vector TYPE a)
{
  check (get_auto_n (a, 0),  get_auto_0  (a));
  check (get_auto_n (a, 1),  get_auto_1  (a));
#if ELEMENTS >= 4
  check (get_auto_n (a, 2),  get_auto_2  (a));
  check (get_auto_n (a, 3),  get_auto_3  (a));
#if ELEMENTS >= 8
  check (get_auto_n (a, 4),  get_auto_4  (a));
  check (get_auto_n (a, 5),  get_auto_5  (a));
  check (get_auto_n (a, 6),  get_auto_6  (a));
  check (get_auto_n (a, 7),  get_auto_7  (a));
#if ELEMENTS >= 16
  check (get_auto_n (a, 8),  get_auto_8  (a));
  check (get_auto_n (a, 9),  get_auto_9  (a));
  check (get_auto_n (a, 10), get_auto_10 (a));
  check (get_auto_n (a, 11), get_auto_11 (a));
  check (get_auto_n (a, 12), get_auto_12 (a));
  check (get_auto_n (a, 13), get_auto_13 (a));
  check (get_auto_n (a, 14), get_auto_14 (a));
  check (get_auto_n (a, 15), get_auto_15 (a));
#endif
#endif
#endif
}


static void do_pointer     (vector TYPE *)		__attribute__((__noinline__));
static TYPE get_pointer_n  (vector TYPE *, ssize_t)	__attribute__((__noinline__));
static TYPE get_pointer_0  (vector TYPE *)		__attribute__((__noinline__));
static TYPE get_pointer_1  (vector TYPE *)		__attribute__((__noinline__));
#if ELEMENTS >= 4
static TYPE get_pointer_2  (vector TYPE *)		__attribute__((__noinline__));
static TYPE get_pointer_3  (vector TYPE *)		__attribute__((__noinline__));
#if ELEMENTS >= 8
static TYPE get_pointer_4  (vector TYPE *)		__attribute__((__noinline__));
static TYPE get_pointer_5  (vector TYPE *)		__attribute__((__noinline__));
static TYPE get_pointer_6  (vector TYPE *)		__attribute__((__noinline__));
static TYPE get_pointer_7  (vector TYPE *)		__attribute__((__noinline__));
#if ELEMENTS >= 16
static TYPE get_pointer_8  (vector TYPE *)		__attribute__((__noinline__));
static TYPE get_pointer_9  (vector TYPE *)		__attribute__((__noinline__));
static TYPE get_pointer_10 (vector TYPE *)		__attribute__((__noinline__));
static TYPE get_pointer_11 (vector TYPE *)		__attribute__((__noinline__));
static TYPE get_pointer_12 (vector TYPE *)		__attribute__((__noinline__));
static TYPE get_pointer_13 (vector TYPE *)		__attribute__((__noinline__));
static TYPE get_pointer_14 (vector TYPE *)		__attribute__((__noinline__));
static TYPE get_pointer_15 (vector TYPE *)		__attribute__((__noinline__));
#endif
#endif
#endif

static TYPE
get_pointer_n (vector TYPE *p, ssize_t n)
{
  return vec_extract (*p, n);
}

static TYPE
get_pointer_0 (vector TYPE *p)
{
  return vec_extract (*p, 0);
}

static TYPE
get_pointer_1 (vector TYPE *p)
{
  return vec_extract (*p, 1);
}

#if ELEMENTS >= 4
static TYPE
get_pointer_2 (vector TYPE *p)
{
  return vec_extract (*p, 2);
}

static TYPE
get_pointer_3 (vector TYPE *p)
{
  return vec_extract (*p, 3);
}

#if ELEMENTS >= 8
static TYPE
get_pointer_4 (vector TYPE *p)
{
  return vec_extract (*p, 4);
}

static TYPE
get_pointer_5 (vector TYPE *p)
{
  return vec_extract (*p, 5);
}

static TYPE
get_pointer_6 (vector TYPE *p)
{
  return vec_extract (*p, 6);
}

static TYPE
get_pointer_7 (vector TYPE *p)
{
  return vec_extract (*p, 7);
}

#if ELEMENTS >= 16
static TYPE
get_pointer_8 (vector TYPE *p)
{
  return vec_extract (*p, 8);
}

static TYPE
get_pointer_9 (vector TYPE *p)
{
  return vec_extract (*p, 9);
}

static TYPE
get_pointer_10 (vector TYPE *p)
{
  return vec_extract (*p, 10);
}

static TYPE
get_pointer_11 (vector TYPE *p)
{
  return vec_extract (*p, 11);
}

static TYPE
get_pointer_12 (vector TYPE *p)
{
  return vec_extract (*p, 12);
}

static TYPE
get_pointer_13 (vector TYPE *p)
{
  return vec_extract (*p, 13);
}

static TYPE
get_pointer_14 (vector TYPE *p)
{
  return vec_extract (*p, 14);
}

static TYPE
get_pointer_15 (vector TYPE *p)
{
  return vec_extract (*p, 15);
}

#endif
#endif
#endif

static void
do_pointer (vector TYPE *p)
{
  check (get_pointer_n (p, 0),  get_pointer_0  (p));
  check (get_pointer_n (p, 1),  get_pointer_1  (p));
#if ELEMENTS >= 4
  check (get_pointer_n (p, 2),  get_pointer_2  (p));
  check (get_pointer_n (p, 3),  get_pointer_3  (p));
#if ELEMENTS >= 8
  check (get_pointer_n (p, 4),  get_pointer_4  (p));
  check (get_pointer_n (p, 5),  get_pointer_5  (p));
  check (get_pointer_n (p, 6),  get_pointer_6  (p));
  check (get_pointer_n (p, 7),  get_pointer_7  (p));
#if ELEMENTS >= 16
  check (get_pointer_n (p, 8),  get_pointer_8  (p));
  check (get_pointer_n (p, 9),  get_pointer_9  (p));
  check (get_pointer_n (p, 10), get_pointer_10 (p));
  check (get_pointer_n (p, 11), get_pointer_11 (p));
  check (get_pointer_n (p, 12), get_pointer_12 (p));
  check (get_pointer_n (p, 13), get_pointer_13 (p));
  check (get_pointer_n (p, 14), get_pointer_14 (p));
  check (get_pointer_n (p, 15), get_pointer_15 (p));
#endif
#endif
#endif
}



static vector TYPE s;

static void do_static     (void)	__attribute__((__noinline__));
static TYPE get_static_n  (ssize_t)	__attribute__((__noinline__));
static TYPE get_static_0  (void)	__attribute__((__noinline__));
static TYPE get_static_1  (void)	__attribute__((__noinline__));
#if ELEMENTS >= 4
static TYPE get_static_2  (void)	__attribute__((__noinline__));
static TYPE get_static_3  (void)	__attribute__((__noinline__));
#if ELEMENTS >= 8
static TYPE get_static_4  (void)	__attribute__((__noinline__));
static TYPE get_static_5  (void)	__attribute__((__noinline__));
static TYPE get_static_6  (void)	__attribute__((__noinline__));
static TYPE get_static_7  (void)	__attribute__((__noinline__));
#if ELEMENTS >= 16
static TYPE get_static_8  (void)	__attribute__((__noinline__));
static TYPE get_static_9  (void)	__attribute__((__noinline__));
static TYPE get_static_10 (void)	__attribute__((__noinline__));
static TYPE get_static_11 (void)	__attribute__((__noinline__));
static TYPE get_static_12 (void)	__attribute__((__noinline__));
static TYPE get_static_13 (void)	__attribute__((__noinline__));
static TYPE get_static_14 (void)	__attribute__((__noinline__));
static TYPE get_static_15 (void)	__attribute__((__noinline__));
#endif
#endif
#endif

static TYPE
get_static_n (ssize_t n)
{
  return vec_extract (s, n);
}

static TYPE
get_static_0 (void)
{
  return vec_extract (s, 0);
}

static TYPE
get_static_1 (void)
{
  return vec_extract (s, 1);
}

#if ELEMENTS >= 4
static TYPE
get_static_2 (void)
{
  return vec_extract (s, 2);
}

static TYPE
get_static_3 (void)
{
  return vec_extract (s, 3);
}

#if ELEMENTS >= 8
static TYPE
get_static_4 (void)
{
  return vec_extract (s, 4);
}

static TYPE
get_static_5 (void)
{
  return vec_extract (s, 5);
}

static TYPE
get_static_6 (void)
{
  return vec_extract (s, 6);
}

static TYPE
get_static_7 (void)
{
  return vec_extract (s, 7);
}

#if ELEMENTS >= 16
static TYPE
get_static_8 (void)
{
  return vec_extract (s, 8);
}

static TYPE
get_static_9 (void)
{
  return vec_extract (s, 9);
}

static TYPE
get_static_10 (void)
{
  return vec_extract (s, 10);
}

static TYPE
get_static_11 (void)
{
  return vec_extract (s, 11);
}

static TYPE
get_static_12 (void)
{
  return vec_extract (s, 12);
}

static TYPE
get_static_13 (void)
{
  return vec_extract (s, 13);
}

static TYPE
get_static_14 (void)
{
  return vec_extract (s, 14);
}

static TYPE
get_static_15 (void)
{
  return vec_extract (s, 15);
}

#endif
#endif
#endif

static void
do_static (void)
{
  check (get_static_n (0),  get_static_0  ());
  check (get_static_n (1),  get_static_1  ());
#if ELEMENTS >= 4
  check (get_static_n (2),  get_static_2  ());
  check (get_static_n (3),  get_static_3  ());
#if ELEMENTS >= 8
  check (get_static_n (4),  get_static_4  ());
  check (get_static_n (5),  get_static_5  ());
  check (get_static_n (6),  get_static_6  ());
  check (get_static_n (7),  get_static_7  ());
#if ELEMENTS >= 16
  check (get_static_n (8),  get_static_8  ());
  check (get_static_n (9),  get_static_9  ());
  check (get_static_n (10), get_static_10 ());
  check (get_static_n (11), get_static_11 ());
  check (get_static_n (12), get_static_12 ());
  check (get_static_n (13), get_static_13 ());
  check (get_static_n (14), get_static_14 ());
  check (get_static_n (15), get_static_15 ());
#endif
#endif
#endif
}



vector TYPE g;

static void do_global     (void)	__attribute__((__noinline__));
static TYPE get_global_n  (ssize_t)	__attribute__((__noinline__));
static TYPE get_global_0  (void)	__attribute__((__noinline__));
static TYPE get_global_1  (void)	__attribute__((__noinline__));
#if ELEMENTS >= 4
static TYPE get_global_2  (void)	__attribute__((__noinline__));
static TYPE get_global_3  (void)	__attribute__((__noinline__));
#if ELEMENTS >= 8
static TYPE get_global_4  (void)	__attribute__((__noinline__));
static TYPE get_global_5  (void)	__attribute__((__noinline__));
static TYPE get_global_6  (void)	__attribute__((__noinline__));
static TYPE get_global_7  (void)	__attribute__((__noinline__));
#if ELEMENTS >= 16
static TYPE get_global_8  (void)	__attribute__((__noinline__));
static TYPE get_global_9  (void)	__attribute__((__noinline__));
static TYPE get_global_10 (void)	__attribute__((__noinline__));
static TYPE get_global_11 (void)	__attribute__((__noinline__));
static TYPE get_global_12 (void)	__attribute__((__noinline__));
static TYPE get_global_13 (void)	__attribute__((__noinline__));
static TYPE get_global_14 (void)	__attribute__((__noinline__));
static TYPE get_global_15 (void)	__attribute__((__noinline__));
#endif
#endif
#endif

static TYPE
get_global_n (ssize_t n)
{
  return vec_extract (g, n);
}

static TYPE
get_global_0 (void)
{
  return vec_extract (g, 0);
}

static TYPE
get_global_1 (void)
{
  return vec_extract (g, 1);
}

#if ELEMENTS >= 4
static TYPE
get_global_2 (void)
{
  return vec_extract (g, 2);
}

static TYPE
get_global_3 (void)
{
  return vec_extract (g, 3);
}

#if ELEMENTS >= 8
static TYPE
get_global_4 (void)
{
  return vec_extract (g, 4);
}

static TYPE
get_global_5 (void)
{
  return vec_extract (g, 5);
}

static TYPE
get_global_6 (void)
{
  return vec_extract (g, 6);
}

static TYPE
get_global_7 (void)
{
  return vec_extract (g, 7);
}

#if ELEMENTS >= 16
static TYPE
get_global_8 (void)
{
  return vec_extract (g, 8);
}

static TYPE
get_global_9 (void)
{
  return vec_extract (g, 9);
}

static TYPE
get_global_10 (void)
{
  return vec_extract (g, 10);
}

static TYPE
get_global_11 (void)
{
  return vec_extract (g, 11);
}

static TYPE
get_global_12 (void)
{
  return vec_extract (g, 12);
}

static TYPE
get_global_13 (void)
{
  return vec_extract (g, 13);
}

static TYPE
get_global_14 (void)
{
  return vec_extract (g, 14);
}

static TYPE
get_global_15 (void)
{
  return vec_extract (g, 15);
}

#endif
#endif
#endif

static void
do_global (void)
{
  check (get_global_n (0),  get_global_0  ());
  check (get_global_n (1),  get_global_1  ());
#if ELEMENTS >= 4
  check (get_global_n (2),  get_global_2  ());
  check (get_global_n (3),  get_global_3  ());
#if ELEMENTS >= 8
  check (get_global_n (4),  get_global_4  ());
  check (get_global_n (5),  get_global_5  ());
  check (get_global_n (6),  get_global_6  ());
  check (get_global_n (7),  get_global_7  ());
#if ELEMENTS >= 16
  check (get_global_n (8),  get_global_8  ());
  check (get_global_n (9),  get_global_9  ());
  check (get_global_n (10), get_global_10 ());
  check (get_global_n (11), get_global_11 ());
  check (get_global_n (12), get_global_12 ());
  check (get_global_n (13), get_global_13 ());
  check (get_global_n (14), get_global_14 ());
  check (get_global_n (15), get_global_15 ());
#endif
#endif
#endif
}


int
main (void)
{
  vector TYPE x = INITIAL;
  vector TYPE *p, a, y;

  a = deoptimize (x);
  s = deoptimize (x);
  g = deoptimize (x);
  y = deoptimize (x);
  p = deoptimize_ptr (&y);

  do_auto (a);
  do_pointer (p);
  do_static ();
  do_global ();
  return 0;
}
