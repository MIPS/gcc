/* { dg-do compile } */
/* { dg-options "-fopenacc -fmax-errors=100" } */

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
  double da1;
  double c1;
  double ca1[7];
  int l1;
  int la1[3];
  char a1[5];
  struct t t1;
  struct t ta1[2];
  float* p1 = 0;
  int** aa1;

#pragma acc parallel reduction (+:i3, ia2, r1, ra1, d1, da1, c1, ca1)
  {}
#pragma acc parallel reduction (*:i3, ia2, r1, ra1, d1, da1, c1, ca1)
  {}
#pragma acc parallel reduction (-:i3, ia2, r1, ra1, d1, da1, c1, ca1)
  {}
#pragma acc parallel reduction (&&:l1, la1)
  {}
#pragma acc parallel reduction (||:l1, la1)
  {}
#pragma acc parallel reduction (min:i3, ia2, r1, ra1, d1, da1)
  {}
#pragma acc parallel reduction (max:i3, ia2, r1, ra1, d1, da1)
  {}
#pragma acc parallel reduction (&:i3, ia2)
  {}
#pragma acc parallel reduction (|:i3, ia2)
  {}
#pragma acc parallel reduction (^:i3, ia2)
  {}
#pragma acc parallel reduction (*:p1)		/* { dg-error "POINTER object" } */
  {}
#pragma acc parallel reduction (-:aa1)
  {}
#pragma acc parallel reduction (*:ia1)	/* { dg-error "Assumed size" } */
  {}
#pragma acc parallel reduction (+:l1)		/* { dg-error "must be of numeric type, got LOGICAL" } */
  {}
#pragma acc parallel reduction (*:la1)	/* { dg-error "must be of numeric type, got LOGICAL" } */
  {}
#pragma acc parallel reduction (-:a1)		/* { dg-error "must be of numeric type, got CHARACTER" } */
  {}
#pragma acc parallel reduction (+:t1)		/* { dg-error "must be of numeric type, got TYPE" } */
  {}
#pragma acc parallel reduction (*:ta1)	/* { dg-error "must be of numeric type, got TYPE" } */
  {}
#pragma acc parallel reduction (&&:i3)	/* { dg-error "must be LOGICAL" } */
  {}
#pragma acc parallel reduction (||:ia2)	/* { dg-error "must be LOGICAL" } */
  {}
#pragma acc parallel reduction (&&:d1)	/* { dg-error "must be LOGICAL" } */
  {}
#pragma acc parallel reduction (||:da1)	/* { dg-error "must be LOGICAL" } */
  {}
#pragma acc parallel reduction (&&:a1)	/* { dg-error "must be LOGICAL" } */
  {}
#pragma acc parallel reduction (||:t1)	/* { dg-error "must be LOGICAL" } */
  {}
#pragma acc parallel reduction (min:c1)	/* { dg-error "must be INTEGER or REAL" } */
  {}
#pragma acc parallel reduction (max:ca1)	/* { dg-error "must be INTEGER or REAL" } */
  {}
#pragma acc parallel reduction (max:l1)	/* { dg-error "must be INTEGER or REAL" } */
  {}
#pragma acc parallel reduction (min:la1)	/* { dg-error "must be INTEGER or REAL" } */
  {}
#pragma acc parallel reduction (max:a1)	/* { dg-error "must be INTEGER or REAL" } */
  {}
#pragma acc parallel reduction (min:t1)	/* { dg-error "must be INTEGER or REAL" } */
  {}
#pragma acc parallel reduction (max:ta1)	/* { dg-error "must be INTEGER or REAL" } */
  {}
#pragma acc parallel reduction (&:r1)	/* { dg-error "must be INTEGER" } */
  {}
#pragma acc parallel reduction (|:ra1)	/* { dg-error "must be INTEGER" } */
  {}
#pragma acc parallel reduction (^:d1)	/* { dg-error "must be INTEGER" } */
  {}
#pragma acc parallel reduction (|:da1)	/* { dg-error "must be INTEGER" } */
  {}
#pragma acc parallel reduction (&:c1)	/* { dg-error "must be INTEGER" } */
  {}
#pragma acc parallel reduction (|:ca1)	/* { dg-error "must be INTEGER" } */
  {}
#pragma acc parallel reduction (^:l1)	/* { dg-error "must be INTEGER" } */
  {}
#pragma acc parallel reduction (&:la1)	/* { dg-error "must be INTEGER" } */
  {}
#pragma acc parallel reduction (|:a1)	/* { dg-error "must be INTEGER" } */
  {}
#pragma acc parallel reduction (^:t1)	/* { dg-error "must be INTEGER" } */
  {}
#pragma acc parallel reduction (&:ta1)	/* { dg-error "must be INTEGER" } */
  {}

}
