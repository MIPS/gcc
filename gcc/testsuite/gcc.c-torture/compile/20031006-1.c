/* Caused ICE on AMD64 at -O2.  */

typedef struct S
{
  struct S *next;
} *T;

extern unsigned char foo (T a, T b);

unsigned char
bar (T x, T y, int *z)
{
  int i = 1;

  while (i <= *z)
    {
      x = x->next;
      i++;
    }
  while (x)
    {
      if (foo (x, y) == 2)
	{
	  *z = i;
	  return 1;
	}
      i++;
      x = x->next;
    }
  return 0;
}
