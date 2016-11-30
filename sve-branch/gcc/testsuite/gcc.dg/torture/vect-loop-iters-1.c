/* { dg-do run } */

#define SIZE1 69
#define SIZE2 5

extern void abort (void);

int a[3][SIZE1][SIZE2];

void __attribute__ ((weak))
f (int count)
{
  for (int i = 0; i < count; ++i)
    for (int j = 0; j < SIZE2; ++j)
      a[1][i][j] += j * 2 + 1;
}

int
main (void)
{
  f (SIZE1);
  for (int i = 0; i < SIZE1; ++i)
    for (int j = 0; j < SIZE2; ++j)
      if (a[0][i][j] != 0
	  || a[1][i][j] != j * 2 + 1
	  || a[2][i][j] != 0)
	abort ();
  return 0;
}
