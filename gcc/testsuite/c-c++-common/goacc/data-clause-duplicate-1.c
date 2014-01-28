void
fun (void)
{
  float *fp;
#pragma acc parallel copy(fp[0:2],fp[0:2]) /* { dg-error "'fp' appears more than once in map clauses" } */
  ;
#pragma acc parallel present_or_copyin(fp[3]) present_or_copyout(fp[7:4]) /* { dg-error "'fp' appears more than once in map clauses" } */
  ;
#pragma acc parallel create(fp[:10]) deviceptr(fp)
  /* { dg-error "'fp' appears more than once in map clauses" "" { target *-*-* } 9 } */
  /* { dg-message "sorry, unimplemented: data clause not yet implemented" "" { target *-*-* } 9 } */
  ;
}
