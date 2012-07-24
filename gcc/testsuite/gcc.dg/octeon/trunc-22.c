/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-not "\tsll\t.*,0" } } */

int x;

int foo(void)
{
    long long copy;

    copy = x;

    return copy;
}
