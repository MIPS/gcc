/* { dg-do run } */
/* { dg-options "" } */


#define NUMBER 5

#if HAVE_IO
#include <cstdio>
#endif

template <class T> int main2 (int argc, char **argv);

int main (int argc, char **argv)
{
 int x, y, z, q;
  x = main2<int> (argc, argv);
  y = main2<long> (argc, argv);
  z = main2<char> (argc, argv);
  q = main2<short> (argc, argv);

  return (x+y+z+q);
}
int func (int *x)
{
  int q = *x;
   q++;
   *x = q;
#if HAVE_IO
   printf("%d\n", (q));
#endif
   return *x;
}
template <class T>
int main2 (int argc, char **argv)
{
  T array[NUMBER], array2[NUMBER];
  int ii, d = 2, y = 0;
#if 1
  for (ii = 0; ii < NUMBER; ii++)  {
   array[ii] = 5;
   array2[ii]= 2;
  }
#endif
  d = func (&d);    /* d = 2, after this, it is 3.  */
  y = 3;
  array2[:] = (T)d * array[:] + (T) func (&y); /* 3 * 5 + 4 */
#if HAVE_IO
  for (ii = 0; ii < NUMBER; ii++)
    printf("array2[%d] = %5d\n", ii, array2[ii]);
#endif  
  d = 2;
  d = func (&d);
  y = 3;
  y = func (&y);
  for (ii = 0; ii < NUMBER; ii++)
    {
#if HAVE_IO
      printf("correct: %d\n", (int) ((T)d * array[ii] + (T)y));
#endif 
    if (array2[ii] != ((T)d * array[ii] + (T)y))
      return 1;
    }
  
  return 0;
}
  


