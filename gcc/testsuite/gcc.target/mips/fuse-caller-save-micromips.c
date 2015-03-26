/* { dg-do compile } */
/* { dg-options "-fuse-caller-save (-mmicromips)" } */
/* At -O0, -O1 and -Og, the register allocator behaves more conservatively,
   and the fuse-caller-save optimization doesn't trigger.  */
/* { dg-skip-if "" { *-*-* }  { "-O0" "-O1" "-Og" } } */
/* Testing -fuse-caller-save optimization option.  */

#define ATTRIBUTE MICROMIPS
#include "fuse-caller-save.h"

/* Check that the first caller-save register is unused.  */
/* { dg-final { scan-assembler-not "\\\$16" } } */
