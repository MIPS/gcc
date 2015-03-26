/* { dg-do compile } */
/* { dg-options "-fno-use-caller-save (-mmicromips)" } */
/* At -O0 and -O1, the register allocator behaves more conservatively,
   and the fuse-caller-save optimization doesn't trigger.  */
/* { dg-skip-if "" { *-*-* }  { "-O0" "-O1" } } */
/* Testing -fuse-caller-save optimization option.  */

#define ATTRIBUTE MICROMIPS
#include "fuse-caller-save.h"

/* Check that the first caller-save register is used.  This is to make sure that
   test fuse-caller-save-mips16 tests something meaningful.  */
/* { dg-final { scan-assembler "\\\$16" } } */
