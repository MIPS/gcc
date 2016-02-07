// PR c++/60336
// { dg-do run { target i?86-*-* x86_64-*-* } }
// { dg-options "-x c" }
// { dg-additional-sources "empty12a.c" }
// { dg-prune-output "command line option" }

#include "empty12.h"
extern "C" void fun(struct dummy, struct foo);

int main()
{
  struct dummy d;
  struct foo f = { -1, -2, -3, -4, -5 };

  fun(d, f); // { dg-message "note: the ABI of passing empty record has changed in GCC 6" }
  return 0;
}
