/* { dg-do compile { target powerpc64-*-linux* } } */
/* { dg-options "-Wall" } */
/* Testcase to check for ABI compliance of parameter passing
   for the PowerPC64 ABI.  */

typedef int __attribute__((mode(V4SI))) v4si;

v4si 
f(v4si v)
{
    return v;
} /* { dg-error "altivec instructions are disabled" } */


int 
main()
{
    v4si v;
    v = f (v); /* { dg-error "altivec instructions are disabled" } */
}
