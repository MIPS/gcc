// { dg-do run }

extern "C" void abort (void);
bool v, x1, x2, x3, x4, x5, x6;

void
foo (void)
{
  #pragma omp atomic capture
  v = ++x1;
  if (!v)
    abort ();
  #pragma omp atomic capture
  v = x2++;
  if (v)
    abort ();
  #pragma omp atomic read
  v = x3;
  if (!v)
    abort ();
  #pragma omp atomic read
  v = x4;
  if (!v)
    abort ();
  #pragma omp atomic capture
  { v = x5; x5 |= 1; }
  if (v)
    abort ();
  #pragma omp atomic capture
  { x6 |= 1; v = x6; }
  if (!v)
    abort ();
}

int
main ()
{
  #pragma omp atomic write
  x3 = true;
  #pragma omp atomic write
  x4 = true;
  foo ();
  return 0;
}
