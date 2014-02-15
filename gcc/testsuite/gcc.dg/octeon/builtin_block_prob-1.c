/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2 -fpropagate-block-prob" } */
/* { dg-final { scan-assembler "\tbne\t\[^\n\]*,\\\$0," } } */

/* __builtin_block_prob takes an integer, a percentage value multiplied by
   100.  */

f (void *p)
{
  if (p == 0)
    {
      /* 99.7% */
      __builtin_block_prob (9970);
      g ();
    }
}
