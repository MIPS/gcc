void Foo ()
{
  int ary[10];
  
#pragma acc parallel default(none) /* { dg-error "enclosing" } */
  {
    ary[0] = 5; /* { dg-error "not specified" }  */
  }
}

void Baz ()
{
  int ary[10];
#pragma acc data copy (ary)
  {
#pragma acc parallel default(none)
    {
      ary[0] = 5;
    }
  }
}
