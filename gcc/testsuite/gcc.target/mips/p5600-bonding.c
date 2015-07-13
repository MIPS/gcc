/* { dg-do compile } */
/* { dg-options "-dp -mtune=p5600  -mno-micromips -mno-mips16" } */
/* { dg-skip-if "Bonding needs peephole optimization." { *-*-* } { "-O0" "-O1" "-Os" } { "" } } */

void
memory_operation (void * __restrict src, void * __restrict dest, int num)
{
  int *vsrc = (int *) src;
  int *vdest = (int *) dest;
  int i;

  for (i = 0; i < num - 1; i += 2)
  {
    vdest[i] = vdest[i] + vsrc[i];
    vdest[i + 1] = vdest[i + 1] + vsrc[i + 1];
  }
}
/* { dg-final { scan-assembler "join2_" } }  */

