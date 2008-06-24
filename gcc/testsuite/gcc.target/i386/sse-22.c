/* Same as sse-14, except converted to use #pragma GCC option.  */
/* { dg-do compile } */
/* { dg-options "-O0 -Werror-implicit-function-declaration -fno-unit-at-a-time" } */

#include <mm_malloc.h>

/* Test that the intrinsics compile without optimization.  All of them are
   defined as inline functions in {,x,e,p,t,s,w,a,b}mmintrin.h  and mm3dnow.h
   that reference the proper builtin functions.  Defining away "extern" and
   "__inline" results in all of them being compiled as proper functions.  */

#define extern
#define __inline

#define _CONCAT(x,y) x ## y

#define test_1(func, type, op1_type, imm)				\
  type _CONCAT(_,func) (op1_type A, int const I)			\
  { return func (A, imm); }

#define test_1x(func, type, op1_type, imm1, imm2)			\
  type _CONCAT(_,func) (op1_type A, int const I, int const L)		\
  { return func (A, imm1, imm2); }

#define test_2(func, type, op1_type, op2_type, imm)			\
  type _CONCAT(_,func) (op1_type A, op2_type B, int const I)		\
  { return func (A, B, imm); }

#define test_2x(func, type, op1_type, op2_type, imm1, imm2)		\
  type _CONCAT(_,func) (op1_type A, op2_type B, int const I, int const L) \
  { return func (A, B, imm1, imm2); }

#define test_4(func, type, op1_type, op2_type, op3_type, op4_type, imm)	\
  type _CONCAT(_,func) (op1_type A, op2_type B,				\
			op3_type C, op4_type D, int const I)		\
  { return func (A, B, C, D, imm); }


/* Following intrinsics require immediate arguments.  They
   are defined as macros for non-optimized compilations. */

/* mmintrin.h (MMX).  */
#pragma GCC option ("mmx")
#include <mmintrin.h>

/* mm3dnow.h (3DNOW).  */
#pragma GCC option ("3dnow")
#include <mm3dnow.h>

/* xmmintrin.h (SSE).  */
#pragma GCC option ("sse")
#include <xmmintrin.h>
test_2 (_mm_shuffle_ps, __m128, __m128, __m128, 1)
test_1 (_mm_extract_pi16, int, __m64, 1)
test_1 (_m_pextrw, int, __m64, 1)
test_2 (_mm_insert_pi16, __m64, __m64, int, 1)
test_2 (_m_pinsrw, __m64, __m64, int, 1)
test_1 (_mm_shuffle_pi16, __m64, __m64, 1)
test_1 (_m_pshufw, __m64, __m64, 1)
test_1 (_mm_prefetch, void, void *, _MM_HINT_NTA)

/* emmintrin.h (SSE2).  */
#pragma GCC option ("sse2")
#include <emmintrin.h>
test_2 (_mm_shuffle_pd, __m128d, __m128d, __m128d, 1)
test_1 (_mm_srli_si128, __m128i, __m128i, 1)
test_1 (_mm_slli_si128, __m128i, __m128i, 1)
test_1 (_mm_extract_epi16, int, __m128i, 1)
test_2 (_mm_insert_epi16, __m128i, __m128i, int, 1)
test_1 (_mm_shufflehi_epi16, __m128i, __m128i, 1)
test_1 (_mm_shufflelo_epi16, __m128i, __m128i, 1)
test_1 (_mm_shuffle_epi32, __m128i, __m128i, 1)

/* pmmintrin.h (SSE3).  */
#pragma GCC option ("sse3")
#include <pmmintrin.h>

/* tmmintrin.h (SSSE3).  */
#pragma GCC option ("ssse3")
#include <tmmintrin.h>
test_2 (_mm_alignr_epi8, __m128i, __m128i, __m128i, 1)
test_2 (_mm_alignr_pi8, __m64, __m64, __m64, 1)

/* ammintrin.h (SSE4A).  */
#pragma GCC option ("sse4a")
#include <ammintrin.h>
test_1x (_mm_extracti_si64, __m128i, __m128i, 1, 1)
test_2x (_mm_inserti_si64, __m128i, __m128i, __m128i, 1, 1)

/* smmintrin.h (SSE4.1).  */
/* nmmintrin.h (SSE4.2).  */
/* Note, nmmintrin.h includes smmintrin.h, and smmintrin.h checks for the
   #ifdef.  So just set the option to SSE4.2.  */
#pragma GCC option ("sse4.2")
#include <nmmintrin.h>
test_2 (_mm_blend_epi16, __m128i, __m128i, __m128i, 1)
test_2 (_mm_blend_ps, __m128, __m128, __m128, 1)
test_2 (_mm_blend_pd, __m128d, __m128d, __m128d, 1)
test_2 (_mm_dp_ps, __m128, __m128, __m128, 1)
test_2 (_mm_dp_pd, __m128d, __m128d, __m128d, 1)
test_2 (_mm_insert_ps, __m128, __m128, __m128, 1)
test_1 (_mm_extract_ps, int, __m128, 1)
test_2 (_mm_insert_epi8, __m128i, __m128i, int, 1)
test_2 (_mm_insert_epi32, __m128i, __m128i, int, 1)
#ifdef __x86_64__
test_2 (_mm_insert_epi64, __m128i, __m128i, long long, 1)
#endif
test_1 (_mm_extract_epi8, int, __m128i, 1)
test_1 (_mm_extract_epi32, int, __m128i, 1)
#ifdef __x86_64__
test_1 (_mm_extract_epi64, long long, __m128i, 1)
#endif
test_2 (_mm_mpsadbw_epu8, __m128i, __m128i, __m128i, 1)
test_2 (_mm_cmpistrm, __m128i, __m128i, __m128i, 1)
test_2 (_mm_cmpistri, int, __m128i, __m128i, 1)
test_4 (_mm_cmpestrm, __m128i, __m128i, int, __m128i, int, 1)
test_4 (_mm_cmpestri, int, __m128i, int, __m128i, int, 1)
test_2 (_mm_cmpistra, int, __m128i, __m128i, 1)
test_2 (_mm_cmpistrc, int, __m128i, __m128i, 1)
test_2 (_mm_cmpistro, int, __m128i, __m128i, 1)
test_2 (_mm_cmpistrs, int, __m128i, __m128i, 1)
test_2 (_mm_cmpistrz, int, __m128i, __m128i, 1)
test_4 (_mm_cmpestra, int, __m128i, int, __m128i, int, 1)
test_4 (_mm_cmpestrc, int, __m128i, int, __m128i, int, 1)
test_4 (_mm_cmpestro, int, __m128i, int, __m128i, int, 1)
test_4 (_mm_cmpestrs, int, __m128i, int, __m128i, int, 1)
test_4 (_mm_cmpestrz, int, __m128i, int, __m128i, int, 1)

/* bmmintrin.h (SSE5).  */
#pragma GCC option ("sse5")
#include <bmmintrin.h>
test_1 (_mm_roti_epi8, __m128i, __m128i, 1)
test_1 (_mm_roti_epi16, __m128i, __m128i, 1)
test_1 (_mm_roti_epi32, __m128i, __m128i, 1)
test_1 (_mm_roti_epi64, __m128i, __m128i, 1)

/* wmmintrin.h (AES/PCLMUL).  */
#pragma GCC option ("aes,pclmul")
#include <wmmintrin.h>
test_1 (_mm_aeskeygenassist_si128, __m128i, __m128i, 1)
test_2 (_mm_clmulepi64_si128, __m128i, __m128i, __m128i, 1)

/* mmintrin-common.h */
test_1 (_mm_round_pd, __m128d, __m128d, 1)
test_1 (_mm_round_ps, __m128, __m128, 1)
test_2 (_mm_round_sd, __m128d, __m128d, __m128d, 1)
test_2 (_mm_round_ss, __m128, __m128, __m128, 1)
