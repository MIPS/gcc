/* { dg-options "-march=32r6 -mcmodel=large -mpcrel -mno-gpopt -mno-pid -ffat-lto-objects" } */
/* { dg-additional-options "-mjump-table-opt" } */
/* { dg-skip-if "" { *-*-* } { "-O0" } { "" } } */
#include "jump-table.c"
/* { dg-final { scan-assembler "\tbgeiuc\t" } } */
/* { dg-final { scan-assembler "\tlapc.b\t" } } */
/* { dg-final { scan-assembler "\tlbx\t" } } */
/* { dg-final { scan-assembler "\tbrsc\t" } } */
