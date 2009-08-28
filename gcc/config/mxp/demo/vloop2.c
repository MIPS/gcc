/* Benchmark inspired by mandelbrot set.  */
#define T int
#define SIZE 3000

typedef T align_a[SIZE] __attribute__ ((aligned (128)));

align_a R, I, rr, ri;

void init_fn (void);

void (*init_pnt) (void) = init_fn;

void
f ()
{
  int i;

  (*init_pnt) ();

  for (i = 0; i < SIZE; i++)
{
  T zr = 0, zi = 0;
  T cr = R[i], ci = I[i];

#define STEP zr >>= 16, zi >>= 16, \
  zr = zr * zr - zr * zi + cr, zi = 2 * zr * zi + ci
  STEP;
  STEP;
  STEP;
  STEP;
  STEP;
  STEP;
  STEP;
  STEP;
  STEP;
  STEP;
#if 0
#endif
  rr[i] = zr, ri[i] = zi;
}
}

long
simple_rand (void)
{
  static unsigned long seed = 47114711;
  unsigned long this = seed * 1103515245 + 12345;
  seed = this;
  return this >> 8;
}

/* Data needed to initialize array A.  */
#define A_D(A) {(A), sizeof (A)/sizeof((A)[0])}

struct { int *a, sz; } init_tab[]
  = { A_D (R), A_D (I), A_D (rr), A_D (ri), {0,0} };

void
init_fn (void)
{
  int i, j, *a;

  for (i = 0; a = init_tab[i].a; i++)
    {
      for (j = init_tab[i].sz - 1; --j >= 0; )
	a[j] = simple_rand ();
    }
}
