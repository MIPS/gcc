/* { dg-do assemble } */
/* { dg-options "-O2 -ftree-vectorize -march=armv8-a+sve --save-temps" } */

#define GATHER_LOAD1(OBJTYPE,STRIDETYPE,STRIDE)\
void gather_load1##OBJTYPE##STRIDETYPE##STRIDE (OBJTYPE * restrict dst,\
						OBJTYPE * restrict src,\
						STRIDETYPE count)\
{\
  for (STRIDETYPE i=0; i<count; i++)\
    dst[i] = src[i * STRIDE];\
}

#define GATHER_LOAD2(OBJTYPE,STRIDETYPE)\
void gather_load2##OBJTYPE##STRIDETYPE (OBJTYPE * restrict dst,\
					OBJTYPE * restrict src,\
					STRIDETYPE stride,\
					STRIDETYPE count)\
{\
  for (STRIDETYPE i=0; i<count; i++)\
    dst[i] = src[i * stride];\
}

#define GATHER_LOAD3(OBJTYPE,STRIDETYPE)\
void gather_load3s5##OBJTYPE##STRIDETYPE\
  (OBJTYPE * restrict d1, OBJTYPE * restrict d2, OBJTYPE * restrict d3,\
   OBJTYPE * restrict d4, OBJTYPE * restrict d5, OBJTYPE * restrict src,\
   STRIDETYPE count)\
{\
  const STRIDETYPE STRIDE = 5;\
  for (STRIDETYPE i=0; i<count; i++)\
    {\
      d1[i] = src[0 + (i * STRIDE)];\
      d2[i] = src[1 + (i * STRIDE)];\
      d3[i] = src[2 + (i * STRIDE)];\
      d4[i] = src[3 + (i * STRIDE)];\
      d5[i] = src[4 + (i * STRIDE)];\
    }\
}

#define GATHER_LOAD4(OBJTYPE,STRIDETYPE,STRIDE)\
void gather_load4##OBJTYPE##STRIDETYPE##STRIDE (OBJTYPE * restrict dst,\
						OBJTYPE * restrict src,\
						STRIDETYPE count)\
{\
  for (STRIDETYPE i=0; i<count; i++)\
    {\
      *dst = *src;\
      dst += 1;\
      src += STRIDE;\
    }\
}

#define GATHER_LOAD5(OBJTYPE,STRIDETYPE)\
void gather_load5##OBJTYPE##STRIDETYPE (OBJTYPE * restrict dst,\
					OBJTYPE * restrict src,\
					STRIDETYPE stride,\
					STRIDETYPE count)\
{\
  for (STRIDETYPE i=0; i<count; i++)\
    {\
      *dst = *src;\
      dst += 1;\
      src += stride;\
    }\
}

GATHER_LOAD1 (double, long, 5)
GATHER_LOAD1 (double, long, 8)
GATHER_LOAD1 (double, long, 21)
GATHER_LOAD1 (double, long, 1009)

GATHER_LOAD1 (float, int, 5)
GATHER_LOAD1 (float, int, 8)
GATHER_LOAD1 (float, int, 21)
GATHER_LOAD1 (float, int, 1009)

GATHER_LOAD2 (double, long)
GATHER_LOAD2 (float, int)

GATHER_LOAD3 (double, long)
GATHER_LOAD3 (float, int)

GATHER_LOAD4 (double, long, 5)

/* NOTE: We can't vectorize GATHER_LOAD4 (float, int, 5) because we can't prove
   that the offsets used for the gather load won't overflow.  */

GATHER_LOAD5 (double, long)
GATHER_LOAD5 (float, int)

/* Widened forms.  */
GATHER_LOAD1 (double, int, 5)
GATHER_LOAD1 (double, int, 8)
GATHER_LOAD1 (double, short, 5)
GATHER_LOAD1 (double, short, 8)

GATHER_LOAD1 (float, short, 5)
GATHER_LOAD1 (float, short, 8)

GATHER_LOAD2 (double, int)
GATHER_LOAD2 (float, short)

GATHER_LOAD4 (double, int, 5)
GATHER_LOAD4 (float, short, 5)

GATHER_LOAD5 (double, int)

/* TODO: We generate abysmal code for this even though we don't use gathers.  */
/*GATHER_LOAD5 (float, short)*/

/* { dg-final { scan-assembler-times "ld1d\\tz\[0-9\]+.d, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.d\\\]" 19 } } */
/* { dg-final { scan-assembler-times "ld1w\\tz\[0-9\]+.s, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.s, sxtw 2\\\]" 12 } } */
/* { dg-final { scan-assembler-times "ld1w\\tz\[0-9\]+.s, p\[0-9\]+/z, \\\[x\[0-9\]+, z\[0-9\]+.s, sxtw\\\]" 3 } } */
