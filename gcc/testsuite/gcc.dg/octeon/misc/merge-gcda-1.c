/* Test arcs.  */

int ratios[4] = { 10, 40, 50, 70  };

unsigned int n = 100;

int a = 3;
int b = 4;

int
main ()
{
  int i, coreid;

  asm ("rdhwr %0, $0" : "=r"(coreid));

  for (i = 0; i < n; i++)
    if (i < ratios[coreid])
      f (i, a);
    else
      f (i, b);
}
