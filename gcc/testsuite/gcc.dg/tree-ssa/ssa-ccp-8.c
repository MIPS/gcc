/* { dg-do compile } */
/* { dg-options "-O1 -fdump-tree-ccp" } */

/* structures -- more tests to come */


struct foo {
  int a;
  int b;
  int c[10];
  short d;
};

extern void link_error (void);

void
test8 (struct foo *f)
{
  f->a = 0;
  f->b = 0;
  if (f->a != 0)
    link_error ();
}

void
test88 (struct foo f)
{
  f.a = 0;
  f.b = 0;
  if (f.a != 0)
    link_error ();
}

void
test888 (struct foo *f)
{
  f->c[0] = 0;
  f->c[1] = 0;
  if (f->c[0] != 0)
    link_error ();
}


void
test8888 (struct foo f)
{
  f.a = 0;
  f.d = 0;
  if (f.a != 0)
    link_error ();
}

void
test88888 (struct foo *f)
{
  f->a = 0;
  f->d = 0;
  if (f->a != 0)
    link_error ();
}

void
test888888 (struct foo *f)
{
  short k;
  f->a = 0;
  k = 6;
  if (f->a != 0)
    link_error ();
}


void
test8888888 (struct foo f)
{
  short k;
  f.a = 0;
  k = 6;
  if (f.a != 0)
    link_error ();
}

void
test88888888 (int *arr)
{
  short k;
  arr[0] = 0;
  k = 6;
  if (arr[0] != 0)
    link_error ();
}


/* There should be no link_error calls, if there is any, the
   optimization has failed */
/* { dg-final { scan-tree-dump-times "link_error" 0 "ccp"} } */

