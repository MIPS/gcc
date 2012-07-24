/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* seb and sll is not needed as the below testcase can
   be represented as -x aka 0-x. */
/* { dg-final { scan-assembler-not "seb"  } } */
/* { dg-final { scan-assembler-not "sll"  } } */

div1 (char x)
{
  return x / -1;
}
