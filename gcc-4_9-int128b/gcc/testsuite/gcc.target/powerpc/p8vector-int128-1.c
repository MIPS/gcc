/* { dg-do compile { target { powerpc*-*-* && lp64 } } } */
/* { dg-skip-if "" { powerpc*-*-darwin* } { "*" } { "" } } */
/* { dg-require-effective-target powerpc_p8vector_ok } */
/* { dg-options "-mcpu=power8 -O3 -mvsx-timode" } */

#include <altivec.h>

#ifndef TYPE
#define TYPE __int128_t
#endif

void do_addcuq (TYPE *p, TYPE *q, TYPE *r)
{
  *p = __builtin_vec_vaddcuq (*q, *r);
}

void do_adduqm (TYPE *p, TYPE *q, TYPE *r)
{
  *p = __builtin_vec_add (*q, *r);
}

void do_addeuqm (TYPE *p, TYPE *q, TYPE *r, TYPE *s)
{
  *p = __builtin_vec_vaddeuqm (*q, *r, *s);
}

void do_addecuq (TYPE *p, TYPE *q, TYPE *r, TYPE *s)
{
  *p = __builtin_vec_vaddecuq (*q, *r, *s);
}

void do_subeuqm (TYPE *p, TYPE *q, TYPE *r, TYPE *s)
{
  *p = __builtin_vec_vsubeuqm (*q, *r, *s);
}

void do_subecuq (TYPE *p, TYPE *q, TYPE *r, TYPE *s)
{
  *p = __builtin_vec_vsubecuq (*q, *r, *s);
}

void do_subcuq (TYPE *p, TYPE *q, TYPE *r)
{
  *p = __builtin_vec_vsubcuq (*q, *r);
}

void do_subuqm (TYPE *p, TYPE *q, TYPE *r)
{
  *p = __builtin_vec_vsubuqm (*q, *r);
}

void do_bpermq (TYPE *p, TYPE *q, TYPE *r)
{
  *p = __builtin_vec_vbpermq (*q, *r);
}

/* { dg-final { scan-assembler	   "vaddcuq"   } } */
/* { dg-final { scan-assembler	   "vadduqm"   } } */
/* { dg-final { scan-assembler	   "vaddecuq"  } } */
/* { dg-final { scan-assembler	   "vaddeuqm"  } } */
/* { dg-final { scan-assembler	   "vbpermq"   } } */
/* { dg-final { scan-assembler	   "vsubecuq"  } } */
/* { dg-final { scan-assembler	   "vsubeuqm"  } } */
/* { dg-final { scan-assembler	   "vsubcuq"   } } */
/* { dg-final { scan-assembler	   "vsubuqm"   } } */
/* { dg-final { scan-assembler-not "mtvsrd"    } } */
/* { dg-final { scan-assembler-not "mfvsrd"    } } */
/* { dg-final { scan-assembler-not "ori 2,2,0" } } */
/* { dg-final { scan-assembler-not "xxpermdi"  } } */

