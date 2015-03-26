/* { dg-options "(-mmicromips) -mno-interlink-compressed" } */
/* { dg-skip-if "code size optimization" { *-*-* } { "*" } { "-Os" } } */

/* Check that microMIPS functions are aligned on 16-bit (2^1 byte)
   boundaries with -Os.  */

int MICROMIPS
f (int x)
{
  return x + 42;
}

/* { dg-final { scan-assembler "\t\\.align\t1" } } */

