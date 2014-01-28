void
fun1 (void)
{
#pragma acc parallel deviceptr(u) /* { dg-error "'u' undeclared" } */
  ;
#pragma acc parallel deviceptr(u[0:4]) /* { dg-error "expected '\\\)' before '\\\[' token" } */
  ;

#pragma acc parallel deviceptr(fun1) /* { dg-error "'fun1' is not a variable" } */
  ;
#pragma acc parallel deviceptr(fun1[2:5])
  /* { dg-error "'fun1' is not a variable" "not a variable" { target *-*-* } 11 } */
  /* { dg-error "expected '\\\)' before '\\\[' token" "array" { target *-*-* } 11 } */
  ;

  int i;
#pragma acc parallel deviceptr(i) /* { dg-error "'i' is not a pointer variable" } */
  ;
#pragma acc parallel deviceptr(i[0:4])
  /* { dg-error "'i' is not a pointer variable" "not a pointer variable" { target *-*-* } 19 } */
  /* { dg-error "expected '\\\)' before '\\\[' token" "array" { target *-*-* } 19 } */
  ;

  float fa[10];
#pragma acc parallel deviceptr(fa) /* { dg-error "'fa' is not a pointer variable" } */
  ;
#pragma acc parallel deviceptr(fa[1:5])
  /* { dg-error "'fa' is not a pointer variable" "not a pointer variable" { target *-*-* } 27 } */
  /* { dg-error "expected '\\\)' before '\\\[' token" "array" { target *-*-* } 27 } */
  ;

  float *fp;
#pragma acc parallel deviceptr(fp)
  ;
#pragma acc parallel deviceptr(fp[0:4]) /* { dg-error "expected '\\\)' before '\\\[' token" } */
  ;
}

void
fun2 (void)
{
  int i;
  float *fp;
#pragma acc parallel deviceptr(fp,u,fun2,i,fp)
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
#pragma acc parallel deviceptr(fp,fp) /* { dg-error "'fp' appears more than once in map clauses" } */
  ;
#pragma acc parallel deviceptr(fp) deviceptr(fp) /* { dg-error "'fp' appears more than once in map clauses" } */
  ;
#pragma acc parallel copy(fp) deviceptr(fp) /* { dg-error "'fp' appears more than once in map clauses" } */
  ;
}

/* { dg-prune-output "sorry, unimplemented: data clause not yet implemented" } */
