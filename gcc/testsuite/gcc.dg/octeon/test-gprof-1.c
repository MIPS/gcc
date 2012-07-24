/* Program to test gprof with different options, 
   -b: flat profile, 
   -lA: Annotate source listing, 
   --function-ordering: suggested funtion ordering. */

/* { dg-do run { target native } } */
/* { dg-options "-g -pg" } */
/* { dg-final { run-gprof "--function-ordering" "" } } */
/* { dg-final { run-gprof "-b" "40000000.*g" } } */
/* { dg-final { run-gprof "-lA" "10.*Executable lines in this file" } } */
/* { dg-final { remote_file host delete gmon.out } } */

void g() { }

void f()
{
  int j;
  for (j = 0; j < 4000000; j++)
    g();
}

main()
{
  int i;
  for (i=0; i < 10; i++)
    f();
  return 0;
}
