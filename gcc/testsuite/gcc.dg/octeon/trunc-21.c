/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "\tsll\t.*,0" } } */

int foo(int x)
{
    long long y;

    y = x;

    return y;
}
