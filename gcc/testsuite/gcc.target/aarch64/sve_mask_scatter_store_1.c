/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve --save-temps" } */

#define MASK_SCATTER_STORE1(OBJTYPE,MASKTYPE,STRIDETYPE,STRIDE)\
void mscatter_store1##OBJTYPE##STRIDETYPE##STRIDE (OBJTYPE * restrict dst,\
						   OBJTYPE * restrict src,\
						   MASKTYPE * restrict masks,\
						   STRIDETYPE count)\
{\
  for (STRIDETYPE i=0; i<count; i++)\
    if (masks[i * STRIDE])\
      dst[i * STRIDE] = src[i];\
}

#define MASK_SCATTER_STORE2(OBJTYPE,MASKTYPE,STRIDETYPE)\
void mscatter_store2##OBJTYPE##STRIDETYPE (OBJTYPE * restrict dst,\
					   OBJTYPE * restrict src,\
					   MASKTYPE * restrict masks,\
					   STRIDETYPE stride,\
					   STRIDETYPE count)\
{\
  for (STRIDETYPE i=0; i<count; i++)\
    if (masks[i * stride])\
      dst[i * stride] = src[i];\
}

#define MASK_SCATTER_STORE3(OBJTYPE,MASKTYPE,STRIDETYPE)\
void mscatter_store3s5##OBJTYPE##STRIDETYPE\
  (OBJTYPE * restrict dst, OBJTYPE * restrict s1, OBJTYPE * restrict s2,\
   OBJTYPE * restrict s3, OBJTYPE * restrict s4, OBJTYPE * restrict s5,\
   MASKTYPE * restrict masks, STRIDETYPE count)\
{\
  const STRIDETYPE STRIDE = 5;\
  for (STRIDETYPE i=0; i<count; i++)\
    if (masks[i * STRIDE])\
      {\
	dst[0 + (i * STRIDE)] = s1[i];\
	dst[1 + (i * STRIDE)] = s2[i];\
	dst[2 + (i * STRIDE)] = s3[i];\
	dst[3 + (i * STRIDE)] = s4[i];\
	dst[4 + (i * STRIDE)] = s5[i];\
      }\
}

#define MASK_SCATTER_STORE4(OBJTYPE,MASKTYPE,STRIDETYPE,STRIDE)\
void mscatter_store4##OBJTYPE##STRIDETYPE##STRIDE (OBJTYPE * restrict dst,\
						   OBJTYPE * restrict src,\
						   MASKTYPE * restrict masks,\
						   STRIDETYPE count)\
{\
  for (STRIDETYPE i=0; i<count; i++)\
    {\
      if (masks[i * STRIDE])\
	*dst = *src;\
      dst += STRIDE;\
      src += 1;\
    }\
}

#define MASK_SCATTER_STORE5(OBJTYPE,MASKTYPE,STRIDETYPE)\
void mscatter_store5##OBJTYPE##STRIDETYPE (OBJTYPE * restrict dst,\
					   OBJTYPE * restrict src,\
					   MASKTYPE * restrict masks,\
					   STRIDETYPE stride,\
					   STRIDETYPE count)\
{\
  for (STRIDETYPE i=0; i<count; i++)\
    {\
      if (masks[i * stride])\
	*dst = *src;\
      dst += stride;\
      src += 1;\
    }\
}

MASK_SCATTER_STORE1 (double, long, long, 5)
MASK_SCATTER_STORE1 (double, long, long, 8)
MASK_SCATTER_STORE1 (double, long, long, 21)
MASK_SCATTER_STORE1 (double, long, long, 1009)

MASK_SCATTER_STORE1 (float, int, int, 5)

MASK_SCATTER_STORE1 (float, int, int, 8)
MASK_SCATTER_STORE1 (float, int, int, 21)
MASK_SCATTER_STORE1 (float, int, int, 1009)

MASK_SCATTER_STORE2 (double, long, long)
MASK_SCATTER_STORE2 (float, int, int)

MASK_SCATTER_STORE3 (double, long, long)
MASK_SCATTER_STORE3 (float, int, int)

MASK_SCATTER_STORE4 (double, long, long, 5)
/* NOTE: We can't vectorize MASK_SCATTER_STORE4 (float, int, int, 3) because we
   can't prove that the offsets used for the gather load won't overflow.  */

MASK_SCATTER_STORE5 (double, long, long)
MASK_SCATTER_STORE5 (float, int, int)

/* Widened forms.  */
MASK_SCATTER_STORE1 (double, long, int, 5)
MASK_SCATTER_STORE1 (double, long, int, 8)
MASK_SCATTER_STORE1 (double, long, short, 5)
MASK_SCATTER_STORE1 (double, long, short, 8)

MASK_SCATTER_STORE1 (float, int, short, 5)
MASK_SCATTER_STORE1 (float, int, short, 8)

MASK_SCATTER_STORE2 (double, long, int)
MASK_SCATTER_STORE2 (float, int, short)

MASK_SCATTER_STORE4 (double, long, int, 5)
MASK_SCATTER_STORE4 (float, int, short, 5)

MASK_SCATTER_STORE5 (double, long, int)

/* Gather loads are for the masks.  */
/* { dg-final { scan-assembler-times "ld1d\\tz\[0-9\]+.d, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.d\\\]" 15 } } */
/* { dg-final { scan-assembler-times "ld1w\\tz\[0-9\]+.s, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.s, sxtw 2\\\]" 8 } } */
/* { dg-final { scan-assembler-times "ld1w\\tz\[0-9\]+.s, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.s, sxtw\\\]" 3 } } */

/* { dg-final { scan-assembler-times "st1d\\tz\[0-9\]+.d, p\[0-9\]+, \\\[x\[0-9\]+, z\[0-9\]+.d\\\]" 19 } } */
/* { dg-final { scan-assembler-times "st1w\\tz\[0-9\]+.s, p\[0-9\]+, \\\[x\[0-9\]+, z\[0-9\]+.s, sxtw 2\\\]" 12 } } */
/* { dg-final { scan-assembler-times "st1w\\tz\[0-9\]+.s, p\[0-9\]+, \\\[x\[0-9\]+, z\[0-9\]+.s, sxtw\\\]" 3 } } */
