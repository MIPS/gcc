/* PR c/25246 */
/* { dg-do run } */

extern void abort (void);

#pragma pack(1)
struct S
{
  char h;
  struct T { int h1; char h2[3]; } t;
  int i;
#pragma pack()
  int j;
};
struct S s;

int
main (void)
{
  char *p = (char *) &s;
  if (sizeof (s) != 16)
    abort ();
  if ((char *) &s.t.h1 != p + 1
      || &s.t.h2[0] != p + 5
      || (char *) &s.i != p + 8
      || (char *) &s.j != p + 12)
    abort ();
  return 0;
}
