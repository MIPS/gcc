
/* { dg-do compile } */
/* { dg-options "-Wuninitialized -fno-tree-dominator-opts -O2" } */

int g;
void bar();
void blah(int);

int foo (int n, int l, int m, int r)
{
  int v;

  if (n < 10 || m > 100 || r < 20 || l)
    v = r;

  if (m) g++;
  else   bar();

  if ( n < 10 ||  m > 100 || r < 20 )
/* xfailed because the predicated unitialized warning infastructure is just broken: Bug #4568; FSF PR49498 */
      blah(v); /* { dg-bogus "uninitialized" "bogus warning" { xfail *-*-* } } */

  if ( n < 10 ||  m > 100 || r < 10 )
      blah(v); /* { dg-bogus "uninitialized" "bogus warning" } */

  return 0;
}

int foo_2 (int n, int l, int m, int r)
{
  int v;

  if (n < 10 || m > 100 || r < 20 || l)
    v = r;

  if (m) g++;
  else   bar();

  if ( n < 10 ||  m > 100 || r < 20 )
      blah(v); /* { dg-bogus "uninitialized" "bogus warning" } */

  if ( n < 10 ||  m > 100 || r < 30 )
      blah(v); /* { dg-warning "uninitialized" "warning" } */

  return 0;
}
