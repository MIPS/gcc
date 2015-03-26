/* { dg-do compile } */
/* { dg-options "-fuse-caller-save" } */
/* At -O0, -O1 and -Og, the register allocator behaves more conservatively,
   and the fuse-caller-save optimization doesn't trigger.  */
/* { dg-skip-if "" { *-*-* }  { "-O0" "-O1" "-Og" } } */
/* Testing -fuse-caller-save optimization option.  */

#define ATTRIBUTE NOCOMPRESSION
#include "fuse-caller-save.h"

/* Check that there are only 2 stack-saves: r31 in main and foo.  */

/* Check that the first caller-save register is unused.  */
/* { dg-final { scan-assembler-not "\\\$16" } } */
