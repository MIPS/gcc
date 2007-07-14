/* { dg-do compile } */

/* Test GS_ASSIGN for variable sized assignments (WITH_SIZE_EXPR).  */

void init (void *);

void f(int a) 
{
  struct {int b[a];} c, d;

  init(&c);
  d = c;
}

/* { dg-final { scan-tree-dump-times "gimpleir: GS_TRY tuple" 1 "gimple" } } */
/* { dg-final { scan-tree-dump-times "__builtin_stack_save" 1 "gimple" } } */
/* { dg-final { cleanup-tree-dump "gimple" } } */
