// { dg-do compile }
// { dg-options "" }

int
main (int argc, char **argv)
{
  typedef char A[argc];
  new A;
  new A[0];
  new A[5];
  new (A[0]);
  new (A[5]);
}
