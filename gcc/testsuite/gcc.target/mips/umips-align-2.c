/* { dg-options "(-mmicromips) -minterlink-compressed" } */
/* { dg-skip-if "code size optimization" { *-*-* } { "*" } { "-Os" } } */

/* Check that -minterlink-compressed suppresses 16-bit alignment of
   microMIPS functions with -Os.  */

int MICROMIPS
f (int x)
{
  return x + 42;
}

/* { dg-final { scan-assembler-not "\t\\.align\t1" } } */
