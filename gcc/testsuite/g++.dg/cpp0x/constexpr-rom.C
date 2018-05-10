// PR c++/49673: check that test_data goes into .rodata
// { dg-do compile { target c++11 } }
// { dg-additional-options -G0 { target { { alpha*-*-* frv*-*-* ia64-*-* lm32*-*-* m32r*-*-* microblaze*-*-* mips*-*-* nanomips*-*-* nios2-*-* powerpc*-*-* rs6000*-*-* } && { ! { *-*-darwin* *-*-aix* alpha*-*-*vms* } } } } }
// On nanoMIPS, PID mode will put static constants in .sdata instead of
// .rodata, which will make this test fail. We can work around this by
// disabling PID mode with the -mno-pid option.
// { dg-additional-options "-mno-pid" { target nanomips*-*-* } }
// { dg-final { scan-assembler "\\.rdata" { target mips*-*-* } } }
// { dg-final { scan-assembler "rodata" { target { { *-*-linux-gnu *-*-gnu* *-*-elf } && { ! mips*-*-* } } } } }

struct Data
{
  int i;
  constexpr Data(int i = 0) : i(i+1) {}
};

extern const Data test_data = { 1 };
