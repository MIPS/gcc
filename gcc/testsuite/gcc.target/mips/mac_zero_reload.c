/* { dg-do compile } */
/* { dg-options "-mgp32 -mdspr2" } */
/* { dg-skip-if "code quality test" { *-*-* } { "-O0" } { "" } } */
/* { dg-final { scan-assembler-not "\tmflo\t" } } */
/* { dg-final { scan-assembler-not "\tmfhi\t" } } */
/* { dg-final { scan-assembler-not "\tmtlo\t" } } */
/* { dg-final { scan-assembler-not "\tmthi\t" } } */

typedef short v2i16 __attribute__ ((vector_size(4)));

extern v2i16 ps32Ptrl[4096];

extern int sink[4096];

int main(void)
{
    v2i16 v2i16_h0;
    long long   s64Acc;

    for (int i = 0; i < 4; ++i)
    {
        v2i16_h0 = ps32Ptrl[i];

        s64Acc = 0;

        s64Acc = __builtin_mips_dpa_w_ph(s64Acc, v2i16_h0, v2i16_h0);

        sink[i] = __builtin_mips_extr_rs_w(s64Acc, 0);
    }

    return 0;
}