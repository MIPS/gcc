/* ./cc1 -O3 -quiet -fdump-tree-all -ftree-vectorize -fdump-tree-vect-details mxp-demo-20090810.c -ftree-vectorizer-verbose=9 -mA7 -msimd -da  */


#define LEN 1024
int a[LEN], b[LEN*2], c[LEN*2];

void init_fn (void);

void (*init_pnt) (void) = init_fn;

int
f ()
{
  int max = -0x7fff-1;
  int i, j0, k0, j, k;

  (*init_pnt) ();

  for (j0 = 0; j0 < LEN; j0 += 8)
    for (k0 = 0; k0 < LEN; k0 += 8)
      for (j = j0, k = k0, i = 0; i < LEN; i++, j++, k++)
	{
	  int xor1 = (a[i] ^ b[j]);
	  int xor2 = (a[i] ^ c[k]);
	  int sum = xor1 + xor2;

	  max = sum > max ? sum : max;
	}
 return max;
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

struct { int *a, sz; } init_tab[] = { A_D (a), A_D (b), A_D (c), {0,0} };

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
