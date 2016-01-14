/* specifying stack-limit-register = r2 is only
   meaningful on certain architectures */
/* { target { powerpc*-*-* } } */
/* { dg-do compile } */
/* { dg-options "-fstack-limit-register=r2" } */
void foo ()
{
  int N = 2;
  int slots[N];

}

