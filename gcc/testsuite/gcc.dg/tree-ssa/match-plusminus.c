/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-forwprop-details" }  */

/* x + (-y) -> x - y */
int plusminus_1(int x, int y)
{
  int t1 = -y;
  int plusminus_1_val = x + t1;
  return plusminus_1_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to plusminus_1_val_\\d\+ = x_\\d\+\\(D\\) - y_\\d\+\\(D\\)" "forwprop1" } } */

/* x - (-y) -> y + x */
int plusminus_2(int x, int y)
{
  int t1 = -y;
  int plusminus_2_val = x - t1;
  return plusminus_2_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to plusminus_2_val_\\d\+ = y_\\d\+\\(D\\) \\+ x_\\d\+\\(D\\)" "forwprop1" } } */

/* (x + y) - x -> y */
int plusminus_3(int x, int y)
{
  int t1 = x + y;
  int plusminus_3_val = t1 - x;
  return plusminus_3_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to plusminus_3_val_\\d\+ = y_\\d\+\\(D\\)" "forwprop1" } } */

/* (x - y) - x -> -y */
int plusminus_4(int x, int y)
{
  int t1 = x - y;
  int plusminus_4_val = t1 - x;
  return plusminus_4_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to plusminus_4_val_\\d\+ = -y_\\d\+\\(D\\)" "forwprop1" } } */ 

/* (x + y) - y -> x */
int plusminus_5(int x, int y)
{
  int t1 = x + y;
  int plusminus_5_val = t1 - y;
  return plusminus_5_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to plusminus_5_val_\\d\+ = x_\\d\+\\(D\\)" "forwprop1" } } */

/* (x - y) + y -> x */
int plusminus_6(int x, int y)
{
  int t1 = x - y;
  int plusminus_6_val = t1 + y;
  return plusminus_6_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to plusminus_6_val_\\d\+ = x_\\d\+\\(D\\)" "forwprop1" } } */

/* (x + cst1) + cst2 -> x + (cst1 + cst2) */
int plusminus_7(int x)
{
  int t1 = x + 3;
  int plusminus_7_val = t1 + 4;
  return plusminus_7_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to plusminus_7_val_\\d\+ = x_\\d\+\\(D\\) \\+ 7" "forwprop1" } } */ 

/* (cst1 - x) + cst2 -> (cst1 + cst2) - x */
int plusminus_8(int x)
{
  int t1 = 3 - x;
  int plusminus_8_val = t1 + 4;
  return plusminus_8_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to plusminus_8_val_\\d\+ = 7 - x_\\d\+\\(D\\)" "forwprop1" } } */

/* ptr1 p+ (ptr2 - ptr1) -> ptr2 */
unsigned char *
plusminus_9(unsigned char *ptr1, unsigned char *ptr2)
{
  long t1 = ptr2 - ptr1;
  unsigned char *plusminus_9_val = ptr1 + t1;
  return plusminus_9_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to plusminus_9_val_\\d\+ = ptr2" "forwprop1" } } */
