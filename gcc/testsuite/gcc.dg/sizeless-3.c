/* { dg-options "-fstrict-aliasing -Wstrict-aliasing=1" } */

typedef __sizeless_1 ta;
typedef __sizeless_2 tb;
typedef __sizeless_3 tc;

void fb (tb *);
void fc (tc *);

void
g (ta *a, tb *b, tc *c)
{
  fb ((tb *) a);
  fb ((tb *) c); /* { dg-warning "dereferencing type-punned pointer might break strict-aliasing rules" } */
  fc ((tc *) a);
  fc ((tc *) b); /* { dg-warning "dereferencing type-punned pointer might break strict-aliasing rules" } */
}
