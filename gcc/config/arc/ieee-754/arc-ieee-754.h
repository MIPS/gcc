#ifdef __LITTLE_ENDIAN__
#define DBL0L r0
#define DBL0H r1
#define DBL1L r2
#define DBL1H r3
#else
#define DBL0L r1
#define DBL0H r0
#define DBL1L r3
#define DBL1H r2
#endif
#define add_l add
#define asr_l asr
#define j_l j
#define jne_l jne
#define jeq_l jeq
#define or_l or
#define mov_l mov
#define b_l b
#define beq_l beq
#define bne_l bne
#define brne_l brne
#define bset_l bset
#define sub_l sub
#define sub1_l sub1
#define lsr_l lsr
#define xor_l xor
#define bic_l bic
#define bmsk_l bmsk
#define bxor_l bxor
#define bcs_s blo_s
