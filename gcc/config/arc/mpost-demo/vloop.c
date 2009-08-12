#define SIZE 256

typedef unsigned short align_a[SIZE] __attribute__ ((aligned (128)));

align_a b = { 1, 2, 3, 4, 5, 6, 7, 8, 9, };
align_a c = { 13, 17, 19, 23, 29, 31, 37, };
align_a a;

void
f ()
{
  int i;

  for (i = 0; i < SIZE; i++)
{
    a[i] = b[i] + c[i];
}
}
