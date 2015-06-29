/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-ccp-details -fdump-tree-forwprop-details" }  */

/* x + 0 -> x */
int c1(int x)
{
  int t1 = 0;
  int c1_val = x + t1;
  return c1_val;
}
/* { dg-final { scan-tree-dump "Match-and-simplified x_\\d\+\\(D\\) \\+ t1_\\d\+ to x_\\d\+\\(D\\)" "ccp1" } } */ 

/* x ptrplus 0 -> x */
char *c2(char *p)
{
  int t1 = 0;
  char *c2_val = p + t1;
  return c2_val;
}
/* { dg-final { scan-tree-dump "Match-and-simplified p_\\d\+\\(D\\) \\+ _\\d\+ to p_\\d\+\\(D\\)" "ccp1" } } */

/* x - 0 -> x */ 
int c3(int x)
{
  int t1 = 0;
  int c3_val = x - t1;  
  return c3_val;
}
/* { dg-final { scan-tree-dump "Match-and-simplified x_\\d\+\\(D\\) - t1_\\d\+ to x_\\d\+\\(D\\)" "ccp1" } } */

/* x - x -> 0 */
int c4(int x)
{
  int t1 = x;
  int c4_val = x - t1;
  return c4_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to c4_val_\\d\+ = 0" "forwprop1" } } */

/* x * 0 -> 0 */
int c5(int x)
{
  int t1 = 0;
  int c5_val = x * t1;
  return c5_val;
}
/* { dg-final { scan-tree-dump "Match-and-simplified x_\\d\+\\(D\\) \\* t1_\\d\+ to 0" "ccp1" } } */

/* x * 1 -> x */
int c6(int x)
{
  int t1 = 1;
  int c6_val = x * t1;
  return c6_val;
}
/* { dg-final { scan-tree-dump "Match-and-simplified x_\\d\+\\(D\\) \\* t1_\\d\+ to x_\\d\+\\(D\\)" "ccp1" } } */

/* x / 1 -> x */
int c7(int x)
{
  int t1 = 1;
  int c7_val = x / t1;
  return c7_val;
}
/* { dg-final { scan-tree-dump "Match-and-simplified x_\\d\+\\(D\\) / t1_\\d\+ to x_\\d\+\\(D\\)" "ccp1" } } */

/* x % x -> 0 */
int c8(int x)
{
  int t1 = x;
  int c8_val = x % t1;
  return c8_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to c8_val_\\d\+ = 0" "forwprop1" } } */

/* x | 0 -> x */
int c9(int x)
{
  int t1 = 0;
  int c9_val = x | t1;
  return c9_val;
}
/* { dg-final { scan-tree-dump "Match-and-simplified x_\\d\+\\(D\\) | t1_\\d\+ to x_\\d\+\\(D\\)" "ccp1" } } */

/* x | -1 -> -1 */
int c10(int x)
{
  int t1 = -1;
  int c10_val = x | t1;
  return c10_val;
}
/* { dg-final { scan-tree-dump "Match-and-simplified x_\\d\+\\(D\\) | t1_\\d\+ to -1" "ccp1" } } */

/* x & -1 -> x */
int c11(int x)
{
  int t1 = -1;
  int c11_val = x & t1;
  return c11_val;
}
/* { dg-final { scan-tree-dump "Match-and-simplified x_\\d\+\\(D\\) & t1_\\d\+ to x_\\d\+\\(D\\)" "ccp1" } } */

/* x & 0 -> 0 */
int c12(int x)
{
  int t1 = 0;
  int c12_val = x & t1;
  return c12_val;
}
/* { dg-final { scan-tree-dump "Match-and-simplified x_\\d\+\\(D\\) & t1_\\d\+ to 0" "ccp1" } } */

/* x ^ 0 -> x */
int c13(int x)
{
  int t1 = 0;
  int c13_val = x ^ t1;
  return c13_val;
}
/* { dg-final { scan-tree-dump "Match-and-simplified x_\\d\+\\(D\\) \\^ t1_\\d\+ to x_\\d\+\\(D\\)" "ccp1" } } */

/* x % 1 -> 0 */
int c14(int x)
{
  int t1 = 1;
  int c14_val = x % t1;
  return c14_val;
}
/* { dg-final { scan-tree-dump "Match-and-simplified x_\\d\+\\(D\\) % t1_\\d\+ to 0" "ccp1" } } */
