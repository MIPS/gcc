/* { dg-do run { target { ! { hppa*-*-hpux* } } } } */

/* complex reductions.  */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <complex.h>

#define ng 32

int
main(void)
{
  const int n = 1000;
  int i;
  double complex vresult, result, array[n];
  bool lvresult, lresult;

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

  if (cabsf (result - vresult) > .0001)
    abort ();

  result = 5;
  vresult = 5;

  lresult = false;
  lvresult = false;

  /* '&&' reductions.  */
#pragma acc parallel num_gangs (ng) copy (lresult)
#pragma acc loop reduction (&&:lresult) gang
  for (i = 0; i < n; i++)
    lresult = lresult && (creal(result) > creal(array[i]));

  /* Verify the reduction.  */
  for (i = 0; i < n; i++)
    lvresult = lvresult && (creal(result) > creal(array[i]));

  if (lresult != lvresult)
    abort ();

  result = 5;
  vresult = 5;

  lresult = false;
  lvresult = false;

  /* '||' reductions.  */
#pragma acc parallel num_gangs (ng) copy (lresult)
#pragma acc loop reduction (||:lresult) gang
  for (i = 0; i < n; i++)
    lresult = lresult || (creal(result) > creal(array[i]));

  /* Verify the reduction.  */
  for (i = 0; i < n; i++)
    lvresult = lvresult || (creal(result) > creal(array[i]));

  if (lresult != lvresult)
    abort ();

  return 0;
}
