/* { dg-options "-m32 -march=32r6 -msoft-float -fno-pic -g0" } */
/* { dg-additional-options "-foptimize-sibling-calls -fschedule-insns2" } */
/* { dg-skip-if "no sibling calls at O0" { *-*-* } { "-O0" } { "" } } */

extern void f0 (unsigned char);
extern void f1 (unsigned char);
int check (unsigned char);

void function(unsigned char idx)
{
    void (*f[])(unsigned char) =
    {
        f0,
        f1
    };

    if (check(idx))
        f[idx](idx);

    return;
}

/* { dg-final { scan-assembler "\trestore\t32,\\\$ra,\\\$s.\n\tjrc\t\\\$\[at0-9\]+\n" } } */
