// Test that the nothrow optimization works properly.
// { dg-do compile }
// { dg-options "-O -fdump-tree-optimized" }

extern "C" int printf (const char *, ...);

int i, j, k;

int main()
{
  try
    {
      ++i;
      printf ("foo\n");
      ++j;
    }
  catch (...)
    {
      return 42;
    }
}

// The catch block should be optimized away.
// { dg-final { scan-tree-dump-times "42" 0 "optimized" } }
