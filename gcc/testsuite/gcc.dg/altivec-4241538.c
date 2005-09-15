/* APPLE LOCAL file 4241538 */
/* { dg-do compile { target powerpc*-*-darwin* } } */
/* { dg-options "-faltivec" } */
/* Contributed by Devang Patel  <dpatel@apple.com>  */

int main( int argc, char **argv )
{
    float array[ 4 ];
    vector float some_vector = vec_ld( array );
    return 0;
}


