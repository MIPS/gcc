/* { dg-do compile } */
/* { dg-options "-O -fdump-tree-forwprop1-details" }  */

unsigned char
rotate_1 (unsigned char x)
{
  unsigned char t1 = x << 5;
  unsigned char t2 = x >> 3;
  unsigned char rotate_1_val = t1 + t2;
  return rotate_1_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to rotate_1_val_\\d\+ = x_\\d\+\\(D\\) r<< 5" "forwprop1" } } */

unsigned char
rotate_2 (unsigned char x)
{
  unsigned char t1 = x << 4;
  unsigned char t2 = x >> 3;
  unsigned char rotate_2_val = t1 + t2;
  return rotate_2_val;
}
/* { dg-final { scan-tree-dump-not "gimple_simplified to rotate_2_val_\\d\+ = x_\\d\+\\(D\\) r<< 5" "forwprop1" } } */

unsigned char
rotate_3 (unsigned char x)
{
  unsigned char t1 = x << 5;
  unsigned char t2 = x >> 3;
  unsigned char rotate_3_val = t1 | t2;
  return rotate_3_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to rotate_3_val_\\d\+ = x_\\d\+\\(D\\) r<< 5" "forwprop1" } } */

unsigned char
rotate_4 (unsigned char x)
{
  unsigned char t1 = x << 5;
  unsigned char t2 = x >> 3;
  unsigned char rotate_4_val = t1 ^ t2;
  return rotate_4_val;
}
/* { dg-final { scan-tree-dump "gimple_simplified to rotate_4_val_\\d\+ = x_\\d\+\\(D\\) r<< 5" "forwprop1" } } */
