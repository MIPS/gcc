/* Make sure there are only 2 loads. */
/* { dg-do compile { target spu-*-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-times "lqd	\\$\[0-9\]+,0\\(\\$\[0-9\]+\\)" 1 } } */
/* { dg-final { scan-assembler-times "lqd	\\$\[0-9\]+,16\\(\\$\[0-9\]+\\)" 1 } } */
/* { dg-final { scan-assembler-times "lq\[dx\]" 2 } } */
  
struct __attribute__ ((__aligned__(16))) S {
  int a, b, c, d;
  int e, f, g, h;
};
  
int
f(struct S *s)
{ 
  return s->a + s->b + s->c + s->d + s->e + s->f + s->g + s->h;
} 
