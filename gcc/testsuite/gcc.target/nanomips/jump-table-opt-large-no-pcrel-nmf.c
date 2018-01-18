/* { dg-options "-march=32r6 -mcmodel=large -mno-pcrel" } */
/* { dg-additional-options "-mjump-table-opt" } */
/* { dg-skip-if "" { *-*-* } { "-O0" } { "" } } */
#include "jump-table.c"
/* { dg-final { scan-assembler "\tli\t.*,\\\$L" } } */
/* { dg-final { scan-assembler "\tlbx\t" } } */
/* { dg-final { scan-assembler "\tbrsc\t" } } */
