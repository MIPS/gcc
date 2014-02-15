/* { dg-do compile { target *-octeon-* } } */

int i __attribute__ ((cvmx_shared));
extern int j __attribute__ ((cvmx_shared));
static int k __attribute__ ((cvmx_shared));

void f ()
{
  extern int l __attribute__ ((cvmx_shared));
}
