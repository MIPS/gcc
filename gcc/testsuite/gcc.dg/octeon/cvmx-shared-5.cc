/* Test link once cvmx_shared attribute variables.  */

/* { dg-do compile { target *-octeon-* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler ".cvmx_shared.linkonce." } } */
/* { dg-final { scan-assembler ".cvmx_shared_bss.linkonce." } } */
/* { dg-final { scan-assembler-not ".gnu.linkonce.s." } } */

#define CVMX_SHARED __attribute__ ((cvmx_shared)) 
 
class A 
{ 
public: 
  static bool test() 
    { 
      CVMX_SHARED static int testVal = 0; 
      CVMX_SHARED static int test2Val = 5;
                                                                              
      if (testVal == 0 || test2Val)
        {
          testVal++;  test2Val++;
          return true;
        }
      return false;
    }
};
                                                                              
void func()
{
  CVMX_SHARED static int hostGate = 0;
  CVMX_SHARED static int host2Gate = 6;
                                                                              
  if (hostGate)
    {
        A::test();
    }
  else
    host2Gate++;
  hostGate++;
}
