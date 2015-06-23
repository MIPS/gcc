/* { dg-require-effective-target offload_device } */

#include <assert.h>

int main ()
{
  int aa = 0, bb = 0, cc = 0, dd = 0;

  #pragma omp target data map(tofrom: aa) map(to: bb) map(from: cc, dd)
    {
      int ok;
      aa = bb = cc = 1;

      /* Set dd on target to 0 for the further check.  */
      #pragma omp target map(always to: dd)
	{ dd; }

      dd = 1;
      #pragma omp target map(tofrom: aa) map(always to: bb) \
	map(always from: cc) map(to: dd) map(from: ok)
	{
	  /* bb is always to, aa and dd are not.  */
	  ok = (aa == 0) && (bb == 1) && (dd == 0);
	  aa = bb = cc = dd = 2;
	}

      assert (ok);
      assert (aa == 1);
      assert (bb == 1);
      assert (cc == 2); /* cc is always from.  */
      assert (dd == 1);

      dd = 3;
      #pragma omp target map(from: cc) map(always to: dd) map(from: ok)
	{
	  ok = (dd == 3); /* dd is always to.  */
	  cc = dd = 4;
	}

      assert (ok);
      assert (cc == 2);
      assert (dd == 3);
    }

  assert (aa == 2);
  assert (bb == 1);
  assert (cc == 4);
  assert (dd == 4);

  return 0;
}
