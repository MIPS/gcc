/* Test assembler directives for signed jump-table optimization relaxation.  */
/* { dg-do compile } */
/* { dg-options "-march=32r6 -m32 -fno-pic -mpid -ffat-lto-objects" } */
/* { dg-additional-options "-mjump-table-opt" } */
#include "jump-table-opt-signed.h"

/* { dg-final {scan-assembler "\t\.reloc\t1f,R_NANOMIPS_JUMPTABLE_LOAD,.L\[0-9\]+\n" } } */
/* { dg-final {scan-assembler "\t.jumptable \[124\],\[56\],0\n" } } */
