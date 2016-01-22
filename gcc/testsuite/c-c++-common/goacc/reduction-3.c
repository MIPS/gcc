/* double reductions.  */

#define vl 32
#define n 1000

int
main(void)
{
  int i;
  double result, array[n];
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
#pragma acc parallel vector_length (vl)
#pragma acc loop vector reduction (max:result)
  for (i = 0; i < n; i++)
    result = result > array[i] ? result : array[i];

  /* 'min' reductions.  */
#pragma acc parallel vector_length (vl)
#pragma acc loop vector reduction (min:result)
  for (i = 0; i < n; i++)
    result = result < array[i] ? result : array[i];

  /* '&&' reductions.  */
#pragma acc parallel vector_length (vl)
#pragma acc loop vector reduction (&&:lresult)
  for (i = 0; i < n; i++)
    lresult = lresult && (result > array[i]);

  /* '||' reductions.  */
#pragma acc parallel vector_length (vl)
#pragma acc loop vector reduction (||:lresult)
  for (i = 0; i < n; i++)
    lresult = lresult || (result > array[i]);

  return 0;
}
