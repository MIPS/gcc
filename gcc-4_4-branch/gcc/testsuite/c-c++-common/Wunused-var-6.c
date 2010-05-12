/* PR c/43981 */
/* { dg-do compile } */
/* { dg-options "-Wunused -Wunused-but-set-variable" } */

void g (char *);

char
f (int a)
{
  int len = a * 3;
  char t[len];

  g (t);
  return t[0];
}
