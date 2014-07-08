/* complex reductions.  */

#define vl 32

int
main(void)
{
  const int n = 1000;
  int i;
  __complex__ double result, array[n];
  int lresult;

  /* '+' reductions.  */
#pragma acc parallel vector_length (vl)
#pragma acc loop reduction (+:result)
  for (i = 0; i < n; i++)
    result += array[i];
#pragma acc end parallel

  /* Needs support for complex multiplication.  */

//   /* '*' reductions.  */
// #pragma acc parallel vector_length (vl)
// #pragma acc loop reduction (*:result)
//   for (i = 0; i < n; i++)
//     result *= array[i];
// #pragma acc end parallel
//
//   /* 'max' reductions.  */
// #pragma acc parallel vector_length (vl)
// #pragma acc loop reduction (+:result)
//   for (i = 0; i < n; i++)
//       result = result > array[i] ? result : array[i];
// #pragma acc end parallel
// 
//   /* 'min' reductions.  */
// #pragma acc parallel vector_length (vl)
// #pragma acc loop reduction (+:result)
//   for (i = 0; i < n; i++)
//       result = result < array[i] ? result : array[i];
// #pragma acc end parallel

  /* '&&' reductions.  */
#pragma acc parallel vector_length (vl)
#pragma acc loop reduction (&&:lresult)
  for (i = 0; i < n; i++)
    lresult = lresult && (__real__(result) > __real__(array[i]));
#pragma acc end parallel

  /* '||' reductions.  */
#pragma acc parallel vector_length (vl)
#pragma acc loop reduction (||:lresult)
  for (i = 0; i < n; i++)
    lresult = lresult || (__real__(result) > __real__(array[i]));
#pragma acc end parallel

  return 0;
}
