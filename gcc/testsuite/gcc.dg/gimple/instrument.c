/* { dg-do compile } */
/* { dg-options "-finstrument-functions -fgimple-only -fdump-tree-gimple-details" } */

/* Test gimplification of function instrumentation.  */

int foo()
{
  return 9;
}

/* { dg-final { scan-tree-dump-times "gimpleir: __builtin_profile_func_enter" 1 "gimple"} } */
/* { dg-final { scan-tree-dump-times "gimpleir: __builtin_profile_func_exit" 1 "gimple"} } */
/* { dg-final { cleanup-tree-dump "gimple" } } */
