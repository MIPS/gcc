/* { dg-options "-m32 -march=32r6 -msoft-float -fno-pic -mcmodel=auto -g0" } */
/* { dg-additional-options "-foptimize-sibling-calls -fschedule-insns2 -ffixed-s0 -ffixed-s1 -ffixed-s2 -ffixed-s3 -ffixed-s4 -ffixed-s5 -ffixed-s6 -ffixed-s7 -ffixed-fp" } */
/* { dg-skip-if "no sibling calls at O0" { *-*-* } { "-O0" } { "" } } */

extern void spill (void);
extern void sink (int);

struct block
{
  int data[2];
};

void function (struct block b, int i)
{
	spill();
	sink(b.data[i]);
}

/* { dg-final { scan-assembler "\trestore\t32,\\\$ra\n\tbc\tsink\n" } } */
