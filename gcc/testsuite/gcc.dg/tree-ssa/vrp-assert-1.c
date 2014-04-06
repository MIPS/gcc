/* { dg-do compile } */
/* { dg-options "-O2 -Wall -fdump-tree-optimized" } */

int fff(int);
void link_error (void);
int f(int ciu_sum_13)
{
  int t[1024];
  int iftmp119_15;
  long D31121_17;
  int ______r_19;
  int D34526_53;
  int num_47, D34527_48;
  int D31115_14;

  iftmp119_15 = ciu_sum_13 != 0;
  D31115_14 = __builtin_constant_p (ciu_sum_13);
  if (D31115_14 == 0) {
    D31121_17 = (long int) iftmp119_15;
    ______r_19 = (int) D31121_17;
    fff (______r_19);
  }

  if (iftmp119_15 == 0)
    return 0;

  if (ciu_sum_13 == 0)
    link_error ();
  else {
    __asm__("" : "=r" (num_47) : "r" (ciu_sum_13));
    D34527_48 = 63 - num_47;
    D34526_53 = D34527_48 + 1;
  }
  /* D34526_53 should not be considered as unitialized as ciu_sum_13 is never
     0 in this if/else statement as we check for non zero with iftmp119_15.  */
  t[D34526_53-1] = 1; /* { dg-bogus "" "" } */
  return t[0];
}

/* The call to link_error should be removed by VRP. 
   This used to fail on ilp32 because we get: 
   D.1694_4 = ciu_sum_13_3(D) != 0;
   iftmp119_15_5 = (int) D.1694_4;
   which was not handled before.  */
/* { dg-final { scan-tree-dump-times "link_error" 0 "optimized"  } } */
/* { dg-final { cleanup-tree-dump "optimized" } } */


