/* This program tests the effects of initialized static variables on in cgraph. */

static int foo1 = 1;
static int * foop = &foo1;

static int foo2;
static int foo3;

static int bar1 () 
{
  foo1 = 10;
  foo2 = 2;
  return foo2;
}

static int (*barp) () = bar1;

static int bar2 () 
{
  foo1 = 12;
  foo3 = 3;
  return foo3;
}

int main()
{
  int x;
  foo1 = 0;
  foo2 = 0;
  x = (barp)();
  if (foo2 != x) abort();
  if (foo1 != 10) abort(); 
  x = bar2();
  if (foo3 != x) abort();
  if (foo1 != 12) abort(); 

  return 0;
}
