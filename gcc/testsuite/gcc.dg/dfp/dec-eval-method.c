/* { dg-do compile } */
/* { dg-options "-std=gnu99" } */
                                                                                
/* N1107 Section 4, C99 5.2.4.2.2a[2], verify that __DEC_EVAL_METHOD__ is
   defined; it will be used to define DEC_EVAL_METHOD in <decfloat.h>.  */

int i;
#ifndef __DEC_EVAL_METHOD__
#error __DEC_EVAL_METHOD__ is not defined
#endif
