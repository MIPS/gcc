/* complex reductions.  */

#define vl 32
#define n 1000

int
main(void)
{
  int i;
  __complex__ double result, array[n];
  int lresult;

  /* '+' reductions.  */
#pragma acc parallel vector_length (vl)
#pragma acc loop vector reduction (+:result)
  for (i = 0; i < n; i++)
    result += array[i];

  /* '*' reductions.  */
#pragma acc parallel vector_length (vl)
#pragma acc loop vector reduction (*:result)
  for (i = 0; i < n; i++)
    result *= array[i];

  /* 'max' reductions.  */
#if 0
  // error: 'result' has invalid type for 'reduction(max)'
#pragma acc parallel vector_length (vl)
#pragma acc loop vector reduction (max:result)
  for (i = 0; i < n; i++)
    result = result > array[i] ? result : array[i];
#endif

  /* 'min' reductions.  */
#if 0
  // error: 'result' has invalid type for 'reduction(min)'
#pragma acc parallel vector_length (vl)
#pragma acc loop vector reduction (min:result)
  for (i = 0; i < n; i++)
    result = result < array[i] ? result : array[i];
#endif

  /* '&&' reductions.  */
#pragma acc parallel vector_length (vl)
#pragma acc loop vector reduction (&&:lresult)
  for (i = 0; i < n; i++)
    lresult = lresult && (__real__(result) > __real__(array[i]));

  /* '||' reductions.  */
#pragma acc parallel vector_length (vl)
#pragma acc loop vector reduction (||:lresult)
  for (i = 0; i < n; i++)
    lresult = lresult || (__real__(result) > __real__(array[i]));

  return 0;
}
