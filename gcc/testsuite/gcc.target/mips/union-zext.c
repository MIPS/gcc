/* { dg-do compile } */
/* { dg-options "(HAS_INS) -mgp32" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler-not "\tandi\t"} } */

typedef struct bits
{
  unsigned B0:8, B1:8, B2:8, B3:8;
} bits_t;

typedef union
{
  unsigned v;
  bits_t b;
} bitfields_t;

void *
strcpy (void *__restrict__ dst, const void *__restrict__ _a)
{
  unsigned x = *(unsigned *) _a;
  bitfields_t bx;
  bx.v = x;

  unsigned char v2 = (unsigned char) bx.b.B2;
  ((unsigned char *) (dst))[2] = (v2);
  if (v2 == 0)
    return 0;
  return dst;
}