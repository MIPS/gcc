// { dg-do compile }

void foo(int i)
{
  switch (i) // { dg-error "invalid entry to OpenACC structured block" }
  {
  #pragma acc parallel
    { case 0:; }
  }
}
