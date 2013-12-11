/* { dg-do compile } */
/* { dg-options "-fopenacc" } */

int 
main() 
{
  int i;

  #pragma acc parallel
  {
  /* TODO: implement in lowering pass */
  goto aa; /* { dg-error "invalid branch to/from" } */
  }
aa:  
  i = 0;

  #pragma acc parallel
  {
  /* TODO: implement in lowering pass */

bb:  
  i = 0;
  }
  goto bb; /* { dg-error "invalid entry" } */

}
