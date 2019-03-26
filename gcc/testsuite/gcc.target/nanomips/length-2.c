/* { dg-options "-dp -mno-gpopt -mcmodel=medium -march=nms1" } */
/* { dg-final { scan-assembler "\[^\n\]*movsi_nanomips/7\[^\n\]*length = 4" } } */
/* { dg-xfail-if "incompatible with PID mode" { *-*-* } { "-mpid" } { "" } } */

int g;

int* f1()
{
  return &g;
}

