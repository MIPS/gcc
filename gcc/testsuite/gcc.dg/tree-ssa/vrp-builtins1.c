/* { dg-do compile } */
/* { dg-options "-O2 -fdump-tree-vrp1" } */

struct f1
{
char a[4];
};

int f(int *a, struct f1 b)
{
  int *c = __builtin_memcpy(a, b.a, 4);
  if (c == 0)
    return 0;
  return *a;
}


int f1(int *a, struct f1 b)
{
  int *c = __builtin_memcpy(a, b.a, 4);
  if (a == 0)
    return 0;
  return *a;
}

/* Both the if statements should be folded when the target does not keep around null pointer checks. */
/* { dg-final { scan-tree-dump-times "Folding predicate" 0 "vrp1" { target {   keeps_null_pointer_checks } } } } */
/* { dg-final { scan-tree-dump-times "Folding predicate" 2 "vrp1" { target { ! keeps_null_pointer_checks } } } } */
/* { dg-final { cleanup-tree-dump "vrp1" } } */
