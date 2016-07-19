/* { dg-do run { target { powerpc*-*-linux* && lp64 } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-skip-if "" { powerpc*-*-*spe* } { "*" } { "" } } */
/* { dg-require-effective-target p8vector_hw } */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <altivec.h>

#ifndef TYPE
#define TYPE unsigned char
#endif

extern void		check_auto_element	(vector TYPE, TYPE, int)	__attribute__((__noinline__));
extern void		check_pointer_element	(vector TYPE *, TYPE, int)	__attribute__((__noinline__));
extern void		check_static_element	(TYPE, int)			__attribute__((__noinline__));
extern void		check_global_element	(TYPE, int)			__attribute__((__noinline__));
extern void		do_auto			(vector TYPE)			__attribute__((__noinline__));
extern void		do_pointer		(vector TYPE *)			__attribute__((__noinline__));
extern void		do_static		(void)				__attribute__((__noinline__));
extern void		do_global		(void)				__attribute__((__noinline__));
extern vector TYPE	deoptimize		(vector TYPE)			__attribute__((__noinline__));
extern vector TYPE *	deoptimize_ptr		(vector TYPE *)			__attribute__((__noinline__));

vector TYPE
deoptimize (vector TYPE a)
{
  return a;
}

vector TYPE *
deoptimize_ptr (vector TYPE *p)
{
  return p;
}

void
check_auto_element (vector TYPE a, TYPE e, int n)
{
  TYPE v = vec_extract (a, n);
  if (v != e)
    abort ();
}

void
check_pointer_element (vector TYPE *p, TYPE e, int n)
{
  TYPE v = vec_extract (*p, n);
  if (v != e)
    abort ();
}

static vector TYPE s;

void
check_static_element (TYPE e, int n)
{
  TYPE v = vec_extract (s, n);
  if (v != e)
    abort ();
}

vector TYPE g;

void
check_global_element (TYPE e, int n)
{
  TYPE v = vec_extract (g, n);
  if (v != e)
    abort ();
}

void
do_auto (vector TYPE a)
{
  check_auto_element (a, vec_extract (a,  0),  0);
  check_auto_element (a, vec_extract (a,  1),  1);
  check_auto_element (a, vec_extract (a,  2),  2);
  check_auto_element (a, vec_extract (a,  3),  3);
  check_auto_element (a, vec_extract (a,  4),  4);
  check_auto_element (a, vec_extract (a,  5),  5);
  check_auto_element (a, vec_extract (a,  6),  6);
  check_auto_element (a, vec_extract (a,  7),  7);
  check_auto_element (a, vec_extract (a,  8),  8);
  check_auto_element (a, vec_extract (a,  9),  9);
  check_auto_element (a, vec_extract (a, 10), 10);
  check_auto_element (a, vec_extract (a, 11), 11);
  check_auto_element (a, vec_extract (a, 12), 12);
  check_auto_element (a, vec_extract (a, 13), 13);
  check_auto_element (a, vec_extract (a, 14), 14);
  check_auto_element (a, vec_extract (a, 15), 15);
}

void
do_pointer (vector TYPE *p)
{
  check_pointer_element (p, vec_extract (*p,  0),  0);
  check_pointer_element (p, vec_extract (*p,  1),  1);
  check_pointer_element (p, vec_extract (*p,  2),  2);
  check_pointer_element (p, vec_extract (*p,  3),  3);
  check_pointer_element (p, vec_extract (*p,  4),  4);
  check_pointer_element (p, vec_extract (*p,  5),  5);
  check_pointer_element (p, vec_extract (*p,  6),  6);
  check_pointer_element (p, vec_extract (*p,  7),  7);
  check_pointer_element (p, vec_extract (*p,  8),  8);
  check_pointer_element (p, vec_extract (*p,  9),  9);
  check_pointer_element (p, vec_extract (*p, 10), 10);
  check_pointer_element (p, vec_extract (*p, 11), 11);
  check_pointer_element (p, vec_extract (*p, 12), 12);
  check_pointer_element (p, vec_extract (*p, 13), 13);
  check_pointer_element (p, vec_extract (*p, 14), 14);
  check_pointer_element (p, vec_extract (*p, 15), 15);
}

void
do_static (void)
{
  check_static_element (vec_extract (s,  0),  0);
  check_static_element (vec_extract (s,  1),  1);
  check_static_element (vec_extract (s,  2),  2);
  check_static_element (vec_extract (s,  3),  3);
  check_static_element (vec_extract (s,  4),  4);
  check_static_element (vec_extract (s,  5),  5);
  check_static_element (vec_extract (s,  6),  6);
  check_static_element (vec_extract (s,  7),  7);
  check_static_element (vec_extract (s,  8),  8);
  check_static_element (vec_extract (s,  9),  9);
  check_static_element (vec_extract (s, 10), 10);
  check_static_element (vec_extract (s, 11), 11);
  check_static_element (vec_extract (s, 12), 12);
  check_static_element (vec_extract (s, 13), 13);
  check_static_element (vec_extract (s, 14), 14);
  check_static_element (vec_extract (s, 15), 15);
}

void
do_global (void)
{
  check_global_element (vec_extract (g,  0),  0);
  check_global_element (vec_extract (g,  1),  1);
  check_global_element (vec_extract (g,  2),  2);
  check_global_element (vec_extract (g,  3),  3);
  check_global_element (vec_extract (g,  4),  4);
  check_global_element (vec_extract (g,  5),  5);
  check_global_element (vec_extract (g,  6),  6);
  check_global_element (vec_extract (g,  7),  7);
  check_global_element (vec_extract (g,  8),  8);
  check_global_element (vec_extract (g,  9),  9);
  check_global_element (vec_extract (g, 10), 10);
  check_global_element (vec_extract (g, 11), 11);
  check_global_element (vec_extract (g, 12), 12);
  check_global_element (vec_extract (g, 13), 13);
  check_global_element (vec_extract (g, 14), 14);
  check_global_element (vec_extract (g, 15), 15);
}

int
main (void)
{
  vector TYPE x = { (TYPE)10,  (TYPE)242, (TYPE)30,  (TYPE)244,
		    (TYPE)50,  (TYPE)246, (TYPE)70,  (TYPE)248,
		    (TYPE)90,  (TYPE)250, (TYPE)110, (TYPE)252,
		    (TYPE)130, (TYPE)254, (TYPE)150, (TYPE)255 };
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
