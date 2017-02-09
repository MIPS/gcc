/* { dg-options "-march=32r6" } */
/* { dg-skip-if "bswap recognition needs expensive optimizations" { *-*-* } { "-O0" "-O1" } { "" } } */

unsigned int
foo (unsigned int x)
{
  return (((x << 24) & 0xff000000)
	  | ((x << 8) & 0xff0000)
	  | ((x >> 8) & 0xff00)
	  | ((x >> 24) & 0xff));
}

/* { dg-final { scan-assembler "byterevw" } } */
