// { dg-do compile }

void foo()
{
  bad1:
  #pragma omp parallel
    goto bad1;			// { dg-error "invalid exit" }

  goto bad2;			// { dg-error "invalid entry" }
  #pragma omp parallel
    {
      bad2: ;
    }
}
