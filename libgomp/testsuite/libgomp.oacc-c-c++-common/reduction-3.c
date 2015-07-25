/* { dg-do run } */

/* double reductions.  */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define ng 32

int
main(void)
{
  const int n = 1000;
  int i;
  double vresult, result, array[n];
  int lvresult, lresult;

  for (i = 0; i < n; i++)
    array[i] = i;

  result = 0;
  vresult = 0;

  /* '+' reductions.  */
#pragma acc parallel num_gangs (ng) copy (result)
#pragma acc loop reduction (+:result) gang
  for (i = 0; i < n; i++)
    result += array[i];

  /* Verify the reduction.  */
  for (i = 0; i < n; i++)
    vresult += array[i];

  if (result != vresult)
    abort ();

  result = 0;
  vresult = 0;

  /* '*' reductions.  */
#pragma acc parallel num_gangs (ng) copy (result)
#pragma acc loop reduction (*:result) gang
  for (i = 0; i < n; i++)
    result *= array[i];

  /* Verify the reduction.  */
  for (i = 0; i < n; i++)
    vresult *= array[i];

  if (fabs(result - vresult) > .0001)
    abort ();

  result = 0;
  vresult = 0;

  /* 'max' reductions.  */
#pragma acc parallel num_gangs (ng) copy (result)
#pragma acc loop reduction (max:result) gang
  for (i = 0; i < n; i++)
      result = result > array[i] ? result : array[i];

  /* Verify the reduction.  */
  for (i = 0; i < n; i++)
      vresult = vresult > array[i] ? vresult : array[i];

  if (result != vresult)
    abort ();

  result = 0;
  vresult = 0;

  /* 'min' reductions.  */
#pragma acc parallel num_gangs (ng) copy (result)
#pragma acc loop reduction (min:result) gang
  for (i = 0; i < n; i++)
      result = result < array[i] ? result : array[i];

  /* Verify the reduction.  */
  for (i = 0; i < n; i++)
      vresult = vresult < array[i] ? vresult : array[i];

  if (result != vresult)
    abort ();

  result = 5;
  vresult = 5;

  lresult = 0;
  lvresult = 0;

  /* '&&' reductions.  */
#pragma acc parallel num_gangs (ng) copy (lresult)
#pragma acc loop reduction (&&:lresult) gang
  for (i = 0; i < n; i++)
    lresult = lresult && (result > array[i]);

  /* Verify the reduction.  */
  for (i = 0; i < n; i++)
    lvresult = lresult && (result > array[i]);

  if (lresult != lvresult)
    abort ();

  result = 5;
  vresult = 5;

  lresult = 0;
  lvresult = 0;

  /* '||' reductions.  */
#pragma acc parallel num_gangs (ng) copy (lresult)
#pragma acc loop reduction (||:lresult) gang
  for (i = 0; i < n; i++)
    lresult = lresult || (result > array[i]);

  /* Verify the reduction.  */
  for (i = 0; i < n; i++)
    lvresult = lresult || (result > array[i]);

  if (lresult != lvresult)
    abort ();

  return 0;
}
