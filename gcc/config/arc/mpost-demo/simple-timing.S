/* Definitions from auxreg.h */
#define LP_START        0x02
#define LP_END          0x03
#define IDENTITY        0x04
#define STATUS32        0x0a
#define COUNT0          0x21 /* Timer 0 count */
#define CONTROL0        0x22 /* Timer 0 control */
#define LIMIT0          0x23 /* Timer 0 limit */
#define INT_VECTOR_BASE 0x25
#define D_CACHE_BUILD   0x72
#define DC_FLDL         0x4c

	.section .init
        lr      r12,[STATUS32]
        bclr_s  r12,r12,1 ; disallow level 1 interrupts
        flag    r12
	mov_s	r3,0
        sr      r3,[CONTROL0]
        sr      r3,[COUNT0]
        sr	0xffffffff,[LIMIT0] ; use maximum available counting range
        mov_s   r12,3   ; enable timer interrupts; count only when not halted.
        sr      r12,[CONTROL0]
	.text
	.global exit
exit:	lr	r1,[COUNT0]
0:	flag 1
	b 0b

