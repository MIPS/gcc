/* { dg-options "-march=32r6 -mcmodel=auto -mgpopt -fno-pic -mpid -ffat-lto-objects" } */
/* { dg-skip-if ".half requires -O" { *-*-* } { "-O0" "-Os" } { "" } } */
int x0;

int x1;
int x2;
int x3;
int x4;
int x5;
int x6;
int x7;
int x8;
int x9;
int x10;
int x11;

int
foo (int i)
{
  switch (i)
    {
    case 1: return x1;
    case 2: return x2;
    case 3: return x3;
    case 4: return x4;
    case 5: return x5;
    case 6: return x6;
    case 7: return x7;
    case 8: return x8;
    case 9: return x9;
    case 10: return x10;
    case 11: return x11;
    default: return x0;
    }
}

/* { dg-final { scan-assembler "\tlw\t" } } */
/* { dg-final { scan-assembler-times "\t\\.word\t\[\\\$.\]L" 12 } } */
/* { dg-final { scan-assembler-times "lw\t\\\$a0,%gprel\\(x\[0-9\]\{1,2\}\\)\\(\\\$gp\\)" 12 } } */
