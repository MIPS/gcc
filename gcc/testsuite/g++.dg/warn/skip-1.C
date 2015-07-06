// { dg-do compile }
// { dg-options "-Wall" }

// For delayed folding we will warn about code that will not be executed too.
extern int f2(int);
void
f1(int i)
{
  f2(1 == 1 ? 0 : f2(i >> -10));
  f2(1 == 1 ? 0 : f2(i >> 128));
  f2(1 == 1 ? 0 : f2(i << -10));
  f2(1 == 1 ? 0 : f2(1 << 128));
  f2(1 != 1 ? f2(i >> -10) : 0);
  f2(1 != 1 ? f2(i >> 128) : 0);
  f2(1 != 1 ? f2(i << -10) : 0);
  f2(1 != 1 ? f2(1 << 128) : 0);
}
