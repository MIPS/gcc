/* { dg-do compile } */
/* { dg-options "-O2" } */

int bar (int, const char *, int, ...);
int baz (int, const char *, long int);

int
f1 (int x, ...)
{
  return bar (5, "", 6, __builtin_va_arg_pack ());	/* { dg-error "invalid use of" } */
}
