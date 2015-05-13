// { dg-do compile }
// { dg-options "-fopenacc" }

int
test1 (int &ref)
{
#pragma acc kernels copy (ref) // { dg-error "reference types are not supported in OpenACC" }
  {
    ref = 10;
  }
}

int
test2 (int &ref)
{
  int b;
#pragma acc kernels copyout (b)
  {
    b = ref + 10; // { dg-error "referenced in target region does not have a mappable type" }
  }

#pragma acc parallel copyout (b)
  {
    b = ref + 10; // { dg-error "referenced in target region does not have a mappable type" }
  }

  ref = b;
}

int
main()
{
  int a = 0;
  int &ref_a = a;

  #pragma acc parallel copy (a, ref_a) // { dg-error "reference types are not supported in OpenACC" }
  {
    ref_a = 5;
  }

  return a;
}
