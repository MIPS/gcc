/* { dg-do compile } */

/* Although somewhat suspect, this isn't actively wrong, and doesn't need
   to be diagnosed.  Any attempt to call the function before including
   arm_sve.h will lead to a link failure.  (Same for taking its address,
   etc.)  */
extern __SVUInt8_t svadd_u8_x (__SVBool_t, __SVUInt8_t, __SVUInt8_t);

#pragma GCC aarch64 "arm_sve.h"
