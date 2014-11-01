/* { dg-do compile } */
/* { dg-options "-O2 -fmerge-bitfields -fdump-tree-esra" }  */

struct S
{
  unsigned f1:7;
  unsigned f2:9;
  unsigned f3:3;
  unsigned f4:5;
  unsigned f5:1;
  unsigned f6:2;
};

unsigned
foo (struct S *p1, struct S *p2, int *ptr)
{
  p2->f1 = p1->f1;
  p2->f2 = p1->f2;
  p2->f3 = p1->f3;
  *ptr = 7;
  p2->f4 = p1->f4;
  p2->f5 = p1->f5;
  p2->f6 = p1->f6;
  return 0;
}

/* Check if bit-field access is lowered.  */
/* { dg-final { scan-tree-dump "BIT_FIELD_REF" "esra" } } */
/* { dg-final { cleanup-tree-dump "esra" } } */

