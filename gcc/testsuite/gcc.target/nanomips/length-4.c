/* { dg-options "-dp -mno-gpopt -mcmodel=medium  -mno-pcrel" } */
/* { dg-final { scan-assembler "\[^\n\]*movsi_nanomips/7\[^\n\]*length = 6" } } */

int g;

int* f1()
{
  return &g;
}

