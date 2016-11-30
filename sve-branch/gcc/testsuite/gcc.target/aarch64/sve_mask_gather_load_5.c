/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve --save-temps" } */

#define MASK_GATHER_LOAD1(OBJTYPE,MASKTYPE,STRIDETYPE,STRIDE)\
void mgather_load1##OBJTYPE##STRIDETYPE##STRIDE (OBJTYPE * restrict dst,\
						 OBJTYPE * restrict src,\
						 MASKTYPE * restrict masks,\
						 STRIDETYPE count)\
{\
  for (STRIDETYPE i=0; i<count; i++)\
    if (masks[i * STRIDE])\
      dst[i] = src[i * STRIDE];\
}

#define MASK_GATHER_LOAD2(OBJTYPE,MASKTYPE,STRIDETYPE)\
void mgather_load2##OBJTYPE##STRIDETYPE (OBJTYPE * restrict dst,\
					 OBJTYPE * restrict src,\
					 MASKTYPE * restrict masks,\
					 STRIDETYPE stride,\
					 STRIDETYPE count)\
{\
  for (STRIDETYPE i=0; i<count; i++)\
    if (masks[i * stride])\
      dst[i] = src[i * stride];\
}

#define MASK_GATHER_LOAD3(OBJTYPE,MASKTYPE,STRIDETYPE)\
void mgather_load3s5##OBJTYPE##STRIDETYPE\
  (OBJTYPE * restrict d1, OBJTYPE * restrict d2, OBJTYPE * restrict d3,\
   OBJTYPE * restrict d4, OBJTYPE * restrict d5, OBJTYPE * restrict src,\
   MASKTYPE * restrict masks, STRIDETYPE count)\
{\
  const STRIDETYPE STRIDE = 5;\
  for (STRIDETYPE i=0; i<count; i++)\
    if (masks[i * STRIDE])\
      {\
	d1[i] = src[0 + (i * STRIDE)];\
	d2[i] = src[1 + (i * STRIDE)];\
	d3[i] = src[2 + (i * STRIDE)];\
	d4[i] = src[3 + (i * STRIDE)];\
	d5[i] = src[4 + (i * STRIDE)];\
      }\
}

#define MASK_GATHER_LOAD4(OBJTYPE,MASKTYPE,STRIDETYPE,STRIDE)\
void mgather_load4##OBJTYPE##STRIDETYPE##STRIDE (OBJTYPE * restrict dst,\
						 OBJTYPE * restrict src,\
						 MASKTYPE * restrict masks,\
						 STRIDETYPE count)\
{\
  for (STRIDETYPE i=0; i<count; i++)\
    {\
      if (masks[i * STRIDE])\
	*dst = *src;\
      dst += 1;\
      src += STRIDE;\
    }\
}

#define MASK_GATHER_LOAD5(OBJTYPE,MASKTYPE,STRIDETYPE)\
void mgather_load5##OBJTYPE##STRIDETYPE (OBJTYPE * restrict dst,\
					 OBJTYPE * restrict src,\
					 MASKTYPE * restrict masks,\
					 STRIDETYPE stride,\
					 STRIDETYPE count)\
{\
  for (STRIDETYPE i=0; i<count; i++)\
    {\
      if (masks[i * stride])\
	*dst = *src;\
      dst += 1;\
      src += stride;\
    }\
}

MASK_GATHER_LOAD1 (double, long, long, 5)
MASK_GATHER_LOAD1 (double, long, long, 8)
MASK_GATHER_LOAD1 (double, long, long, 21)
MASK_GATHER_LOAD1 (double, long, long, 1009)

MASK_GATHER_LOAD1 (float, int, int, 5)
MASK_GATHER_LOAD1 (float, int, int, 8)
MASK_GATHER_LOAD1 (float, int, int, 21)
MASK_GATHER_LOAD1 (float, int, int, 1009)

MASK_GATHER_LOAD2 (double, long, long)
MASK_GATHER_LOAD2 (float, int, int)

MASK_GATHER_LOAD3 (double, long, long)
MASK_GATHER_LOAD3 (float, int, int)

MASK_GATHER_LOAD4 (double, long, long, 5)

/* NOTE: We can't vectorize MASK_GATHER_LOAD4 (float, int, int, 5) because we
   can't prove that the offsets used for the gather load won't overflow.  */

MASK_GATHER_LOAD5 (double, long, long)
MASK_GATHER_LOAD5 (float, int, int)

/* Widened forms.  */
MASK_GATHER_LOAD1 (double, long, int, 5)
MASK_GATHER_LOAD1 (double, long, int, 8)
MASK_GATHER_LOAD1 (double, long, short, 5)
MASK_GATHER_LOAD1 (double, long, short, 8)

MASK_GATHER_LOAD1 (float, int, short, 5)
MASK_GATHER_LOAD1 (float, int, short, 8)

MASK_GATHER_LOAD2 (double, long, int)
MASK_GATHER_LOAD2 (float, int, short)

MASK_GATHER_LOAD4 (double, long, int, 5)
MASK_GATHER_LOAD4 (float, int, short, 5)

MASK_GATHER_LOAD5 (double, long, int)

/* Loads including masks.  */
/* { dg-final { scan-assembler-times "ld1d\\tz\[0-9\]+.d, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.d\\\]" 34 } } */
/* { dg-final { scan-assembler-times "ld1w\\tz\[0-9\]+.s, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.s, sxtw 2\\\]" 20 } } */
/* { dg-final { scan-assembler-times "ld1w\\tz\[0-9\]+.s, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.s, sxtw\\\]" 6 } } */
