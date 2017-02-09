/* GCC used to report an ICE for this test because we generated a LO_SUM
   for an illegitimate constant.  */
/* { dg-options "-m64 -msym32 -EB" } */
extern unsigned long a[];
int b (int);

int
c (void)
{
  return b (a[0]);
}
