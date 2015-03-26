/* { dg-options "(-mmicromips) -mno-interlink-compressed" } */
/* { dg-skip-if "code size optimization" { *-*-* } { "*" } { "-Os" } } */

/* Check that explicit alignment attribute suppresses 16-bit alignment of
   microMIPS functions with -Os.  */

int MICROMIPS
__attribute__ ((aligned (16)))
f (int x) 
{
  return x + 42;
}

/* { dg-final { scan-assembler-not "\t\\.align\t1" } } */
