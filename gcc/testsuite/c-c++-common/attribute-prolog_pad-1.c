/* { dg-do compile } */

void f1(void) __attribute__((prolog_pad(1)));
void f2(void) __attribute__((prolog_pad(2)));

void
f1 (void)
{
  f2 ();
}

void f2 (void)
{
  f1 ();
}
