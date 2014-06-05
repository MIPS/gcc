void
fun1 (void)
{
#pragma acc parallel deviceptr(u) /* { dg-error "'u' undeclared" } */
  ;
#pragma acc kernels deviceptr(u[0:4]) /* { dg-error "expected '\\\)' before '\\\[' token" } */
  ;

#pragma acc data deviceptr(fun1) /* { dg-error "'fun1' is not a variable" } */
  ;
#pragma acc parallel deviceptr(fun1[2:5])
  /* { dg-error "'fun1' is not a variable" "not a variable" { target *-*-* } 11 } */
  /* { dg-error "expected '\\\)' before '\\\[' token" "array" { target *-*-* } 11 } */
  ;

  int i;
#pragma acc kernels deviceptr(i) /* { dg-error "'i' is not a pointer variable" } */
  ;
#pragma acc data deviceptr(i[0:4])
  /* { dg-error "'i' is not a pointer variable" "not a pointer variable" { target *-*-* } 19 } */
  /* { dg-error "expected '\\\)' before '\\\[' token" "array" { target *-*-* } 19 } */
  ;

  float fa[10];
#pragma acc parallel deviceptr(fa) /* { dg-error "'fa' is not a pointer variable" } */
  ;
#pragma acc kernels deviceptr(fa[1:5])
  /* { dg-error "'fa' is not a pointer variable" "not a pointer variable" { target *-*-* } 27 } */
  /* { dg-error "expected '\\\)' before '\\\[' token" "array" { target *-*-* } 27 } */
  ;

  float *fp;
#pragma acc data deviceptr(fp)
  ;
#pragma acc parallel deviceptr(fp[0:4]) /* { dg-error "expected '\\\)' before '\\\[' token" } */
  ;
}

void
fun2 (void)
{
  int i;
  float *fp;
#pragma acc kernels deviceptr(fp,u,fun2,i,fp)
  /* { dg-error "'u' undeclared" "u undeclared" { target *-*-* } 44 } */
  /* { dg-error "'fun2' is not a variable" "fun2 not a variable" { target *-*-* } 44 } */
  /* { dg-error "'i' is not a pointer variable" "i not a pointer variable" { target *-*-* } 44 } */
  /* { dg-error "'fp' appears more than once in map clauses" "fp more than once" { target *-*-* } 44 } */
  ;
}

void
fun3 (void)
{
  float *fp;
#pragma acc data deviceptr(fp,fp) /* { dg-error "'fp' appears more than once in map clauses" } */
  ;
#pragma acc parallel deviceptr(fp) deviceptr(fp) /* { dg-error "'fp' appears more than once in map clauses" } */
  ;
#pragma acc kernels copy(fp) deviceptr(fp) /* { dg-error "'fp' appears more than once in map clauses" } */
  ;
}

extern struct s s1;
extern struct s s2[1]; /* { dg-error "array type has incomplete element type" "" { target c } } */

void
fun4 (void)
{
  struct s *s1_p = &s1;
  struct s *s2_p = &s2;

#pragma acc parallel deviceptr(s1) /* { dg-error "'s1' is not a pointer variable" } */
  ;

#pragma acc parallel deviceptr(s2)
  ;

#pragma acc parallel deviceptr(s1_p)
  s1_p = 0;

#pragma acc parallel deviceptr(s2_p)
  s2_p = 0;
}
