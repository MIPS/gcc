/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-forwprop-details" }  */

/* x + (-y) -> x - y */
int f1(int x, int y)
{
  int t1 = -y;
  return x + t1; 
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= x_\\d\+\\(D\\) - y_\\d\+\\(D\\)" "forwprop1" } } */

/* x - (-y) -> y + x */
int f2(int x, int y)
{
  int t1 = -y;
  return x - t1; 
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= y_\\d\+\\(D\\) \\+ x_\\d\+\\(D\\)" "forwprop1" } } */

/* (x + y) - x -> y */
int f3(int x, int y)
{
  int t1 = x + y;
  return t1 - x;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= y_\\d\+\\(D\\)" "forwprop1" } } */

/* (x - y) - x -> -y */
int f4(int x, int y)
{
  int t1 = x - y;
  return t1 - x;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= -y_\\d\+\\(D\\)" "forwprop1" } } */ 

/* (x + y) - y -> x */
int f5(int x, int y)
{
  int t1 = x + y;
  return t1 - y;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= x_\\d\+\\(D\\)" "forwprop1" } } */

/* (x - y) + y -> x */
int f6(int x, int y)
{
  int t1 = x - y;
  return t1 + y;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= x_\\d\+\\(D\\)" "forwprop1" } } */

/* (x + cst1) + cst2 -> x + (cst1 + cst2) */
int f7(int x)
{
  int t1 = x + 3;
  return t1 + 4;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= x_\\d\+\\(D\\) \\+ 7" "forwprop1" } } */ 

/* (cst1 - x) + cst2 -> (cst1 + cst2) - x */
int f8(int x)
{
  int t1 = 3 - x;
  return t1 + 4;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= 7 - x_\\d\+\\(D\\)" "forwprop1" } } */

/* (x >> 31) & 1 -> x >> 31 */
int f9(int x)
{
  int t1 = x >> 31;
  return t1 & 1;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= t1_\\d\+" "forwprop1" } } */

/* -(~x) -> x + 1 */
int f10(int x)
{
  int t1, t2;
  t1 = ~x;
  t2 = -t1;
  return t2;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= x_\\d\+\\(D\\) \\+ 1" "forwprop1" } } */

/* x + ~x -> -1 */
int f11(int x)
{
  int t1 = ~x;
  return t1 + x;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= -1" "forwprop1" } } */

/* ~x + 1 -> -x */
int f12(int x)
{
  int t1 = ~x;
  return t1 + 1;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= -x_\\d\+\\(D\\)" "forwprop1" } } */

/* __real complex (__real x) = x */
double f13(double x)
{
  _Complex double t1 = x;
  return __real t1;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= x_\\d\+\\(D\\)" "forwprop1" } } */

/* __imag complex (__imag x) = x */
double f14(double x)
{
  _Complex double t1 = x;
  return __imag t1;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= x_\\d\+\\(D\\)" "forwprop1" } } */

/* x & x -> x */
int f15(int x)
{
  int t1 = x;
  return t1 & x;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= x_\\d\+\\(D\\)" "forwprop1" } } */

/* x & ~x -> 0 */
int f16(int x)
{
  int t1 = ~x;
  return t1 & x;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= 0" "forwprop1" } } */

/* x ^ x -> 0 */
int f17(int x)
{
  int t1 = x;
  return t1 ^ x;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= 0" "forwprop1" } } */

/* ~~x -> 0 */
int f18(int x)
{
  int t1 = ~x;
  return ~t1;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= x_\\d\+\\(D\\)" "forwprop1" } } */

/* (x | y) & x -> x */
int f19(int x, int y)
{
  int t1 = x | y;
  return t1 & x;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= x_\\d\+\\(D\\)" "forwprop1" } } */

/* (x & y) | x -> x */
int f20(int x, int y)
{
  int t1 = x & y;
  return t1 | x;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= x_\\d\+\\(D\\)" "forwprop1" } } */

/* (~x & y) | x -> x & y */
int f21(int x, int y)
{
  int t1 = ~x;
  int t2 = t1 & y;
  return t2 | x;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= x_\\d\+\\(D\\) & y_\\d\+\\(D\\)" "forwprop1" } } */

/* (~x | y) & x -> x & y */
int f22(int x, int y)
{
  int t1 = ~x;
  int t2 = t1 | y;
  return t2 & x;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= x_\\d\+\\(D\\) & y_\\d\+\\(D\\)" "forwprop1" } } */

/*  ((x & y) & ~x) & ~y -> 0 */
int f23(int x, int y)
{
  int t1 = x & y;
  int t2 = ~x;
  int t3 = t1 & t2;
  int t4 = ~y;
  return t3 & t4;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= 0" "forwprop1" } } */

/* x & 0 -> 0 */
int f24(int x)
{
  int t1 = 0;
  return x & t1;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= 0" "forwprop1" } } */

/* x & -1 -> x */
int f25(int x)
{
  int t1 = -1;
  return x & t1;
}
/* { dg-final { scan-tree-dump "gimple_match_and_simplified to \[^\n\r\]*= x_\\d\+\\(D\\)" "forwprop1" } } */

/* { dg-final { cleanup-tree-dump "forwprop2" } } */
