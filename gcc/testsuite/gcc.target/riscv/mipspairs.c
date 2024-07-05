/* { dg-do compile } */
/* { dg-options "-march=rv64gc_xmipslsp -mtune=mips-p8700 -mno-double-align" { target { rv64 } } } */
/* { dg-skip-if "" { *-*-* } { "-O0" "-O1" "-Og" } } */

#define MYTEST(name, mytype) \
void test1 ## name (mytype *a, mytype *total) \
{ \
  mytype b = *a++; \
  mytype c = *a; \
  *total = b + c; \
} \
void test2 ## name (mytype *s, mytype a, mytype b) \
{ \
  *s++ = a; \
  *s = b; \
}

MYTEST(1, long long)
MYTEST(2, unsigned long long)
MYTEST(3, long)
MYTEST(4, unsigned long)
MYTEST(5, int)
MYTEST(6, unsigned int)

/* { dg-final { scan-assembler-times "ldp" 4 } } */
/* { dg-final { scan-assembler-times "sdp" 4 } } */
/* { dg-final { scan-assembler-times "lwp" 2 } } */
/* { dg-final { scan-assembler-times "swp" 2 } } */
