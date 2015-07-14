/* { dg-options "-mcompact-branches=never" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" "-O1" "-Os" } { "" } } */
/* { dg-final { scan-assembler "beq\t\\\$\[0-9\]+,\\\$\[0-9\]+,.L\[0-9\]\n\tlw" } } */
/* { dg-final { scan-assembler-times "\\(foo\\)" 2 } } */

/* Test that when compact branches are explicitly disabled, that a non-compact
   branch is produced. 'foo' should be referenced twice in the program text as the
   eager delay slot filler will duplicate the load of foo. */

#include <stdio.h>

struct list
{
  struct list *next;
  int element;
};

struct list *gr;

int foo;

void
f (struct list **ptr)
{
  if (gr != NULL)
    *ptr = gr->next;
  t (1, foo, &gr->element);
}
