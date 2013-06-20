/* { dg-do compile { target { powerpc*-*-* } } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler-times "[^n]and " 2 } } */
/* { dg-final { scan-assembler-times "[^nx]or " 2 } } */
/* { dg-final { scan-assembler-times "xor "     2 } } */
/* { dg-final { scan-assembler-times "nor "     4 } } */
/* { dg-final { scan-assembler-times "nand "    2 } } */
/* { dg-final { scan-assembler-times "eqv "     2 } } */
/* { dg-final { scan-assembler-times "andc "    2 } } */
/* { dg-final { scan-assembler-times "orc "     2 } } */
/* { dg-final { scan-assembler-not   "ld "        } } */
/* { dg-final { scan-assembler-not   "lw[az] "    } } */
/* { dg-final { scan-assembler-not   "st[dw] "    } } */

#ifndef TYPE
#ifdef _ARCH_PPC64
#define TYPE __int128_t
#else
#define TYPE long long
#endif
#endif

/* Test splitting of types in GPR registers.  */
TYPE bool1 (TYPE p, TYPE q) { return p & q; }		/* and */
TYPE bool2 (TYPE p, TYPE q) { return p | q; }		/* ior */
TYPE bool3 (TYPE p, TYPE q) { return p ^ q; }		/* xor */
TYPE bool4 (TYPE p) { return ~ p; }			/* not */
TYPE bool5 (TYPE p, TYPE q) { return ~(p & q); }	/* nand */
TYPE bool6 (TYPE p, TYPE q) { return ~(p | q); }	/* nor */
TYPE bool7 (TYPE p, TYPE q) { return ~(p ^ q); }	/* eqv */
TYPE bool8 (TYPE p, TYPE q) { return (~p) & q; }	/* andc */
TYPE bool9 (TYPE p, TYPE q) { return (~p) | q; }	/* orc */
