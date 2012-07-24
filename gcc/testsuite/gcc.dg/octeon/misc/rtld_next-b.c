#define _GNU_SOURCE
#include <dlfcn.h>

f ()
{
  void (*p)() = dlsym (RTLD_NEXT, "f");
  extern int a;

  a -= 2;
  puts ("b");
  if (p)
    p ();
}
