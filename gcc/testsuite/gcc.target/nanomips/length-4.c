/* { dg-options "-dp -mno-gpopt -mcmodel=medium -mno-pcrel -fno-pic" } */
/* { dg-final { scan-assembler "\[^\n\]*movsi_nanomips/7\[^\n\]*length = 6" } } */
/* { dg-xfail-if "incompatible with PID mode" { *-*-* } { "-mpid" } { "" } } */

int g;

int* f1()
{
  return &g;
}

