/* { dg-options "-march=32r6 -mcmodel=large -mno-pcrel -mno-gpopt -mno-pid -ffat-lto-objects" } */
/* { dg-additional-options "-mjump-table-opt" } */
/* { dg-skip-if "" { *-*-* } { "-O0" } { "" } } */
#include "jump-table.c"
/* { dg-final { scan-assembler "\tbgeiuc\t" } } */
/* { dg-final { scan-assembler "\tli\t.*,\\\$L" } } */
/* { dg-final { scan-assembler "\tlbx\t" } } */
/* { dg-final { scan-assembler "\tbrsc\t" } } */
