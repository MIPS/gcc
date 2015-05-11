int
main ()
{
  int i;

#pragma acc parallel loop tile (10)
  for (i = 0; i < 100; i++)
    ;

#pragma acc parallel loop tile (*)
  for (i = 0; i < 100; i++)
    ;

#pragma acc parallel loop tile (10, *)
  for (i = 0; i < 100; i++)
    ;

#pragma acc parallel loop tile (10, *, i) /* { dg-error "positive constant integer expression" } */
  for (i = 0; i < 100; i++)
    ;

  return 0;
}
/* { dg-bogus "sorry, unimplemented: Clause not supported yet" "tile" { xfail *-*-* } 6 } */
/* { dg-bogus "sorry, unimplemented: Clause not supported yet" "tile" { xfail *-*-* } 10 } */
/* { dg-bogus "sorry, unimplemented: Clause not supported yet" "tile" { xfail *-*-* } 14 } */
