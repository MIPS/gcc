/* { dg-do compile } */
/* { dg-options "-fcilkplus" } */


struct Blah
{
  int x;
  int q;
};

struct Blah A[16], a;

void
check ()
{
    a = __sec_reduce_add (A[:]); /* { dg-error "array notations in builtin functions must" } */ 
}

