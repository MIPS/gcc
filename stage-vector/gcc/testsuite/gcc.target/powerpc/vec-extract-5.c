/* { dg-do run { target { powerpc*-*-linux* && lp64 } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-skip-if "" { powerpc*-*-*spe* } { "*" } { "" } } */
/* { dg-require-effective-target p8vector_hw } */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <altivec.h>

#ifndef TYPE
#define TYPE short
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
  check_auto_element (a, vec_extract (a, 0), 0);
  check_auto_element (a, vec_extract (a, 1), 1);
  check_auto_element (a, vec_extract (a, 2), 2);
  check_auto_element (a, vec_extract (a, 3), 3);
  check_auto_element (a, vec_extract (a, 4), 4);
  check_auto_element (a, vec_extract (a, 5), 5);
  check_auto_element (a, vec_extract (a, 6), 6);
  check_auto_element (a, vec_extract (a, 7), 7);
}

void
do_pointer (vector TYPE *p)
{
  check_pointer_element (p, vec_extract (*p, 0), 0);
  check_pointer_element (p, vec_extract (*p, 1), 1);
  check_pointer_element (p, vec_extract (*p, 2), 2);
  check_pointer_element (p, vec_extract (*p, 3), 3);
  check_pointer_element (p, vec_extract (*p, 4), 4);
  check_pointer_element (p, vec_extract (*p, 5), 5);
  check_pointer_element (p, vec_extract (*p, 6), 6);
  check_pointer_element (p, vec_extract (*p, 7), 7);
}

void
do_static (void)
{
  check_static_element (vec_extract (s, 0), 0);
  check_static_element (vec_extract (s, 1), 1);
  check_static_element (vec_extract (s, 2), 2);
  check_static_element (vec_extract (s, 3), 3);
  check_static_element (vec_extract (s, 4), 4);
  check_static_element (vec_extract (s, 5), 5);
  check_static_element (vec_extract (s, 6), 6);
  check_static_element (vec_extract (s, 7), 7);
}

void
do_global (void)
{
  check_global_element (vec_extract (g, 0), 0);
  check_global_element (vec_extract (g, 1), 1);
  check_global_element (vec_extract (g, 2), 2);
  check_global_element (vec_extract (g, 3), 3);
  check_global_element (vec_extract (g, 4), 4);
  check_global_element (vec_extract (g, 5), 5);
  check_global_element (vec_extract (g, 6), 6);
  check_global_element (vec_extract (g, 7), 7);
}

int
main (void)
{
  vector TYPE x = { (TYPE)10, (TYPE)-20, (TYPE)30, (TYPE)-40,
		    (TYPE)50, (TYPE)-60, (TYPE)70, (TYPE)-80 };
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
