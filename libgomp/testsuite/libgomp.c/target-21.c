extern void abort (void);
union U { int x; long long y; };
struct T { int a; union U b; int c; };
struct S { int s; int u; struct T v; union U w; };

int
main ()
{
  struct S s;
  s.s = 0;
  s.u = 1;
  s.v.a = 2;
  s.v.b.y = 3LL;
  s.v.c = 19;
  s.w.x = 4;
  int err = 0;
  #pragma omp target map (to:s.v.b, s.u) map (from: s.w, err)
  {
    err = 0;
    if (s.u != 1 || s.v.b.y != 3LL)
      err = 1;
    s.w.x = 6;
  }
  if (err || s.w.x != 6)
    abort ();
  s.u++;
  s.v.a++;
  s.v.b.y++;
  s.w.x++;
  #pragma omp target data map (tofrom: s)
  #pragma omp target map (always to: s.w, err) map (alloc:s.u, s.v.b)
  {
    err = 0;
    if (s.u != 2 || s.v.b.y != 4LL || s.w.x != 7)
      err = 1;
    s.w.x = 8;
  }
  if (err || s.w.x != 8)
    abort ();
  s.u++;
  s.v.a++;
  s.v.b.y++;
  s.w.x++;
  #pragma omp target data map (from: s.w) map (to: s.v.b, s.u)
  #pragma omp target map (always to: s.w, err) map (alloc:s.u, s.v.b)
  {
    err = 0;
    if (s.u != 3 || s.v.b.y != 5LL || s.w.x != 9)
      err = 1;
    s.w.x = 11;
  }
  if (err || s.w.x != 11)
    abort ();
  return 0;
}
