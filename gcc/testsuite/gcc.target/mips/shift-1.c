/* { dg-do compile } */
/* { dg-options "-mgp64" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler-not "\tsra\t" } } */
/* { dg-final { scan-assembler-times "\tdsra\t" 3 } } */
int f(long long a)
{
  int d = a>>32;
  return d>>2;
}

int g(long long a)
{
  return a>>34;
}

int h(long long c)
{
  long long d = c>>32;
  return d>>2;
}

