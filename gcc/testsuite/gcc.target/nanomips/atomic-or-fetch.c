/* { dg-do compile } */
/* { dg-options "-ffat-lto-objects" } */

#include "atomic-or-fetch.x"

/* One acquire fence for each RWM: consume, acquire, acq-rel */
/* { dg-final { scan-assembler-times "\tsync\t17" 3 } } */
/* One release fence for each RWM: acquire,  acq-rel */
/* { dg-final { scan-assembler-times "\tsync\t18" 2 } } */
/* RWM with SEQ_CST semantics gets 2 full barriers.  */
/* { dg-final { scan-assembler-times "\tsync\t16" 2 } } */
