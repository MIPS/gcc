/* We should able to just produce one extraction.  */
/* { dg-options "-mgp64 isa_rev>=2 " } */
/* { dg-final { scan-assembler-times "dext\\t" 1 } } */

struct x
{
  int a:12;
  unsigned int b:1;
  long long c:31;
};

void g(struct x *a, struct x b, struct x f, int t)
{
  a->b |= b.b | f.b;
}

