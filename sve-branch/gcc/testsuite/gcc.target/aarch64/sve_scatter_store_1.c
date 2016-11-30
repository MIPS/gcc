/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve --save-temps" } */

#define SCATTER_STORE1(OBJTYPE,STRIDETYPE,STRIDE)\
void scatter_store1##OBJTYPE##STRIDETYPE##STRIDE (OBJTYPE * restrict dst,\
						  OBJTYPE * restrict src,\
						  STRIDETYPE count)\
{\
  for (STRIDETYPE i=0; i<count; i++)\
    dst[i * STRIDE] = src[i];\
}

#define SCATTER_STORE2(OBJTYPE,STRIDETYPE)\
void scatter_store2##OBJTYPE##STRIDETYPE (OBJTYPE * restrict dst,\
					  OBJTYPE * restrict src,\
					  STRIDETYPE stride,\
					  STRIDETYPE count)\
{\
  for (STRIDETYPE i=0; i<count; i++)\
    dst[i * stride] = src[i];\
}

#define SCATTER_STORE3(OBJTYPE,STRIDETYPE)\
void scatter_store3s5##OBJTYPE##STRIDETYPE\
  (OBJTYPE * restrict dst, OBJTYPE * restrict s1, OBJTYPE * restrict s2,\
   OBJTYPE * restrict s3, OBJTYPE * restrict s4, OBJTYPE * restrict s5,\
   STRIDETYPE count)\
{\
  const STRIDETYPE STRIDE = 5;\
  for (STRIDETYPE i=0; i<count; i++)\
    {\
      dst[0 + (i * STRIDE)] = s1[i];\
      dst[4 + (i * STRIDE)] = s5[i];\
      dst[1 + (i * STRIDE)] = s2[i];\
      dst[2 + (i * STRIDE)] = s3[i];\
      dst[3 + (i * STRIDE)] = s4[i];\
    }\
}

#define SCATTER_STORE4(OBJTYPE,STRIDETYPE,STRIDE)\
void scatter_store4##OBJTYPE##STRIDETYPE##STRIDE (OBJTYPE * restrict dst,\
						  OBJTYPE * restrict src,\
						  STRIDETYPE count)\
{\
  for (STRIDETYPE i=0; i<count; i++)\
    {\
      *dst = *src;\
      dst += STRIDE;\
      src += 1;\
    }\
}

#define SCATTER_STORE5(OBJTYPE,STRIDETYPE)\
void scatter_store5##OBJTYPE##STRIDETYPE (OBJTYPE * restrict dst,\
					  OBJTYPE * restrict src,\
					  STRIDETYPE stride,\
					  STRIDETYPE count)\
{\
  for (STRIDETYPE i=0; i<count; i++)\
    {\
      *dst = *src;\
      dst += stride;\
      src += 1;\
    }\
}

SCATTER_STORE1 (double, long, 5)
SCATTER_STORE1 (double, long, 8)
SCATTER_STORE1 (double, long, 21)
SCATTER_STORE1 (double, long, 1009)

SCATTER_STORE1 (float, int, 5)
SCATTER_STORE1 (float, int, 8)
SCATTER_STORE1 (float, int, 21)
SCATTER_STORE1 (float, int, 1009)

SCATTER_STORE2 (double, long)
SCATTER_STORE2 (float, int)

SCATTER_STORE3 (double, long)
SCATTER_STORE3 (float, int)

SCATTER_STORE4 (double, long, 5)
/* NOTE: We can't vectorize SCATTER_STORE4 (float, int, 5) because we can't
   prove that the offsets used for the gather load won't overflow.  */

SCATTER_STORE5 (double, long)
SCATTER_STORE5 (float, int)

/* Widened forms.  */
SCATTER_STORE1 (double, int, 5)
SCATTER_STORE1 (double, int, 8)
SCATTER_STORE1 (double, short, 5)
SCATTER_STORE1 (double, short, 8)

SCATTER_STORE1 (float, short, 5)
SCATTER_STORE1 (float, short, 8)

SCATTER_STORE2 (double, int)
SCATTER_STORE2 (float, short)

SCATTER_STORE4 (double, int, 5)
SCATTER_STORE4 (float, short, 5)

SCATTER_STORE5 (double, int)

/* { dg-final { scan-assembler-times "st1d\\tz\[0-9\]+.d, p\[0-9\]+, \\\[x\[0-9\]+, z\[0-9\]+.d\\\]" 19 } } */
/* { dg-final { scan-assembler-times "st1w\\tz\[0-9\]+.s, p\[0-9\]+, \\\[x\[0-9\]+, z\[0-9\]+.s, sxtw 2\\\]" 12 } } */
/* { dg-final { scan-assembler-times "st1w\\tz\[0-9\]+.s, p\[0-9\]+, \\\[x\[0-9\]+, z\[0-9\]+.s, sxtw\\\]" 3 } } */
