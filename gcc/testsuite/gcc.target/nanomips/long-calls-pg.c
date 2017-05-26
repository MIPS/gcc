/* { dg-do compile } */
/* { dg-options "-mabi=p32 -pg -mlong-calls" } */
/* { dg-final { scan-assembler-not "\tjal\t_mcount" } } */
void
foo (void)
{
}
