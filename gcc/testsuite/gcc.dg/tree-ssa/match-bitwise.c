/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-forwprop-details" }  */

/* x & x -> x */
int bitwise_1(int x)
{
  int t1 = x;
  int bitwise_1_val = t1 & x;
  return bitwise_1_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to bitwise_1_val_\\d\+ = x_\\d\+\\(D\\)" "forwprop1" } } */

/* x & ~x -> 0 */
int bitwise_2(int x)
{
  int t1 = ~x;
  int bitwise_2_val = t1 & x;
  return bitwise_2_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to bitwise_2_val_\\d\+ = 0" "forwprop1" } } */

/* x ^ x -> 0 */
int bitwise_3(int x)
{
  int t1 = x;
  int bitwise_3_val = t1 ^ x;
  return bitwise_3_val; 
}
/* { dg-final { scan-tree-dump "gimple_simplified to bitwise_3_val_\\d\+ = 0" "forwprop1" } } */

/* ~~x -> 0 */
int bitwise_4(int x)
{
  int t1 = ~x;
  int bitwise_4_val = ~t1;
  return bitwise_4_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to bitwise_4_val_\\d\+ = x_\\d\+\\(D\\)" "forwprop1" } } */

/* (x | y) & x -> x */
int bitwise_5(int x, int y)
{
  int t1 = x | y;
  int bitwise_5_val = t1 & x;
  return bitwise_5_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to bitwise_5_val_\\d\+ = x_\\d\+\\(D\\)" "forwprop1" } } */

/* (x & y) | x -> x */
int bitwise_6(int x, int y)
{
  int t1 = x & y;
  int bitwise_6_val = t1 | x;
  return bitwise_6_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to bitwise_6_val_\\d\+ = x_\\d\+\\(D\\)" "forwprop1" } } */

/* (~x & y) | x -> x | y */
int bitwise_7(int x, int y)
{
  int t1 = ~x;
  int t2 = t1 & y;
  int bitwise_7_val = t2 | x;
  return bitwise_7_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to bitwise_7_val_\\d\+ = x_\\d\+\\(D\\) | y_\\d\+\\(D\\)" "forwprop1" } } */

/* (~x | y) & x -> x & y */
int bitwise_8(int x, int y)
{
  int t1 = ~x;
  int t2 = t1 | y;
  int bitwise_8_val = t2 & x;
  return bitwise_8_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to bitwise_8_val_\\d\+ = x_\\d\+\\(D\\) & y_\\d\+\\(D\\)" "forwprop1" } } */

/*  ((x & y) & ~x) & ~y -> 0 */
int bitwise_9(int x, int y)
{
  int t1 = x & y;
  int t2 = ~x;
  int bitwise_9_val = t1 & t2; 
  return bitwise_9_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to bitwise_9_val_\\d\+ = 0" "forwprop1" } } */
