/* { dg-do compile } */
/* { dg-options "-fopenacc -fmax-errors=100 -fsyntax-only" } */

struct t
{
  int i;
};

void foo (int *ia1)
{
  int i1, i2, i3;
  int ia2[10];
  float r1;
  float ra1[5];
  double d1;
  double ca1[7];
  char a1[5];
  struct t t1;
  struct t ta1[2];
  float* p1 = 0;
  int** aa1;
  char *p;
  struct S { int i; int j; } s;
  char a[32];
  double d;
  int i;
  const int c = 8;

#pragma acc parallel reduction (+:p) /* { dg-error "has invalid type for" } */
    {}
#pragma acc parallel reduction (*:s) /* { dg-error "has invalid type for" } */
    {}
#pragma acc parallel reduction (-:a) /* { dg-error "has invalid type for" } */
    {}
#pragma acc parallel reduction (-:ia2) /* { dg-error "has invalid type for" } */
    {}
#pragma acc parallel reduction (-:ra1) /* { dg-error "has invalid type for" } */
    {}
#pragma acc parallel reduction (-:ca1) /* { dg-error "has invalid type for" } */
    {}
  d = 0;
#pragma acc parallel reduction (*:d)
    {}
#pragma acc parallel reduction (|:d) /* { dg-error "has invalid type for" } */
    {}
#pragma acc parallel reduction (&&:d) /* { dg-error "has invalid type for" } */
    {}
#pragma acc parallel reduction (+:i3, r1, d1)
  {}
#pragma acc parallel reduction (*:i3, r1, d1)
  {}
#pragma acc parallel reduction (-:i3, r1, d1)
  {}
#pragma acc parallel reduction (min:i3, r1, d1)
  {}
#pragma acc parallel reduction (max:i3, r1, d1)
  {}
#pragma acc parallel reduction (&:i3)
  {}
#pragma acc parallel reduction (|:i3)
  {}
#pragma acc parallel reduction (^:i3)
  {}
#pragma acc parallel reduction (*:p1)		/* { dg-error "has invalid type for" } */
  {}
#pragma acc parallel reduction (-:aa1) /* { dg-error "has invalid type for" } */
  {}
#pragma acc parallel reduction (*:ia1)	/* { dg-error "has invalid type for" } */
  {}
#pragma acc parallel reduction (-:a1)		/* { dg-error "has invalid type for" } */
  {}
#pragma acc parallel reduction (+:t1)		/* { dg-error "has invalid type for" } */
  {}
#pragma acc parallel reduction (*:ta1)	/* { dg-error "has invalid type for" } */
  {}
#pragma acc parallel reduction (||:ia2)	/* { dg-error "has invalid type for" } */
  {}
#pragma acc parallel reduction (&&:d1)	/* { dg-error "has invalid type for" } */
  {}
#pragma acc parallel reduction (&&:a1)	/* { dg-error "has invalid type for" } */
  {}
#pragma acc parallel reduction (||:t1)	/* { dg-error "has invalid type for" } */
  {}
#pragma acc parallel reduction (max:a1)	/* { dg-error "has invalid type for" } */
  {}
#pragma acc parallel reduction (min:t1)	/* { dg-error "has invalid type for" } */
  {}
#pragma acc parallel reduction (max:ta1)	/* { dg-error "has invalid type for" } */
  {}
#pragma acc parallel reduction (&:r1)	/* { dg-error "has invalid type for" } */
  {}
#pragma acc parallel reduction (|:ra1)	/* { dg-error "has invalid type for" } */
  {}
#pragma acc parallel reduction (^:d1)	/* { dg-error "has invalid type for" } */
  {}
#pragma acc parallel reduction (|:a1)	/* { dg-error "has invalid type for" } */
  {}
#pragma acc parallel reduction (^:t1)	/* { dg-error "has invalid type for" } */
  {}
#pragma acc parallel reduction (&:ta1)	/* { dg-error "has invalid type for" } */
  {}

#pragma acc kernels loop reduction(*:d) /* { dg-error "kernels region" } */
  for (i = 0; i < 10; ++i)
  {}
#pragma acc kernels 
  {
#pragma acc loop reduction(*:d) /* { dg-error "kernels region" } */
  for (i = 0; i < 10; ++i)
  {}
  }

#pragma acc parallel loop reduction(*:d) 
  for (i = 0; i < 10; ++i)
  {}
#pragma acc parallel 
  {
#pragma acc loop reduction(*:d) 
  for (i = 0; i < 10; ++i)
  {}
  }
}
