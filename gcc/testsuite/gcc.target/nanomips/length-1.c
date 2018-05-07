/* { dg-options "-dp" } */
/* { dg-final { scan-assembler "\[^\n\]*movsi_nanomips/6\[^\n\]*length = 6" } } */
/* { dg-final { scan-assembler "\[^\n\]*movsi_nanomips/6\[^\n\]*length = 4" } } */

int f1 ()
{
  return 0x87654321;
}

int f2 ()
{
  return -0x000000f1;
}
