/* { dg-do compile } */
/* { dg-require-effective-target arm_cmse_ok } */
/* { dg-options "-Os -mcmse -fdump-rtl-expand" }  */

#include <arm_cmse.h>

extern int a;
extern int bar (void);

int foo (char * p)
{
  cmse_address_info_t cait;

  cait = cmse_TT (&a);
  if (cait.flags.mpu_region)
    a++;

  cait = cmse_TT_fptr (&bar);
  if (cait.flags.mpu_region)
    a+= bar ();

  cait = cmse_TTA (&a);
  if (cait.flags.mpu_region)
    a++;

  cait = cmse_TTA_fptr (&bar);
  if (cait.flags.mpu_region)
    a+= bar ();

  cait = cmse_TTT (&a);
  if (cait.flags.mpu_region)
    a++;

  cait = cmse_TTT_fptr (&bar);
  if (cait.flags.mpu_region)
    a+= bar ();

  cait = cmse_TTAT (&a);
  if (cait.flags.mpu_region)
    a++;

  cait = cmse_TTAT_fptr (&bar);
  if (cait.flags.mpu_region)
    a+= bar ();

  p = (char *) cmse_check_address_range ((void *) p, sizeof (char), 0);
  p = (char *) cmse_check_address_range ((void *) p, sizeof (char),
					 CMSE_MPU_UNPRIV);
  p = (char *) cmse_check_address_range ((void *) p, sizeof (char),
					 CMSE_MPU_READWRITE);
  p = (char *) cmse_check_address_range ((void *) p, sizeof (char),
					 CMSE_MPU_UNPRIV | CMSE_MPU_READ);
  p = (char *) cmse_check_address_range ((void *) p, sizeof (char),
					 CMSE_AU_NONSECURE
					 | CMSE_MPU_NONSECURE);
  p = (char *) cmse_check_address_range ((void *) p, sizeof (char),
					 CMSE_NONSECURE | CMSE_MPU_UNPRIV);

  p = (char *) cmse_check_pointed_object (p, CMSE_NONSECURE | CMSE_MPU_UNPRIV);

  return a;
}
/* { dg-final { scan-assembler-times "\ttt " 2 } } */
/* { dg-final { scan-assembler-times "ttt " 2 } } */
/* { dg-final { scan-assembler-times "tta " 2 } } */
/* { dg-final { scan-assembler-times "ttat " 2 } } */
/* { dg-final { scan-assembler-times "bl.cmse_check_address_range" 7 } } */
/* { dg-final { scan-assembler-not "cmse_check_pointed_object" } } */

typedef int (*int_ret_funcptr_t) (void);
typedef int __attribute__ ((cmse_nonsecure_call)) (*int_ret_nsfuncptr_t) (void);

int __attribute__ ((cmse_nonsecure_entry))
baz (void)
{
  return cmse_nonsecure_caller ();
}

int __attribute__ ((cmse_nonsecure_entry))
qux (int_ret_funcptr_t int_ret_funcptr)
{
  int_ret_nsfuncptr_t int_ret_nsfunc_ptr;

  if (cmse_is_nsfptr (int_ret_funcptr))
    {
      int_ret_nsfunc_ptr = cmse_nsfptr_create (int_ret_funcptr);
      return int_ret_nsfunc_ptr ();
    }
  return 0;
}
/* { dg-final { scan-assembler "baz:" } } */
/* { dg-final { scan-assembler "__acle_se_baz:" } } */
/* { dg-final { scan-assembler-not "\tcmse_nonsecure_caller" } } */
/* { dg-final { scan-rtl-dump "and.*reg.*const_int 1" expand } } */
/* { dg-final { scan-assembler "bic" } } */
/* { dg-final { scan-assembler "push\t\{r4, r5, r6" } } */
/* { dg-final { scan-assembler "msr\tAPSR_nzcvq" } } */
