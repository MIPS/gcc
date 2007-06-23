/* { dg-do compile } */

/* Test GS_ASSIGN for variable sized assignments (WITH_SIZE_EXPR).  */

void init (void *);

void f(int a) 
{
  struct {int b[a];} c, d;

  init(&c);
  d = c;
}

