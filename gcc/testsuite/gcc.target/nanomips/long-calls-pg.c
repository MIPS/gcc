/* { dg-do compile } */
/* { dg-options "-m32 -pg -mlong-calls" } */
/* { dg-final { scan-assembler-not "\tjal\t_mcount" } } */
void
foo (void)
{
}
