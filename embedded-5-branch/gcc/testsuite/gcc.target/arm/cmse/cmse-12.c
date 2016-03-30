/* { dg-do compile } */
/* { dg-require-effective-target arm_cmse_ok } */
/* { dg-options "-mcmse" }  */
#include <arm_cmse.h>

char *
foo (char * p)
{
  if (!cmse_is_nsfptr (p))
    return cmse_nsfptr_create (p);
}

/* Checks for saving and clearing prior to function call.  */
/* { dg-final { scan-assembler-not "cmse_is_nsfptr" } } */
/* { dg-final { scan-assembler-not "cmse_nsfptr_create" } } */
