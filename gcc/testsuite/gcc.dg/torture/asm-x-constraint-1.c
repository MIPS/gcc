void
foo (int **x, int y, int z)
{
  int *ptr = *x + y * z / 11;
  __asm__ __volatile__ ("foo %0" : : "X" (*ptr));
}
