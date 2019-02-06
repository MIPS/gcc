/* { dg-do run } */
/* { dg-options "-ffat-lto-objects" } */

#include "atomic-fetch-add.x"
#include "atomic-fetch-sub.x"
#include "atomic-fetch-and.x"
#include "atomic-fetch-or.x"
#include "atomic-fetch-xor.x"
#include "atomic-fetch-nand.x"

int main (void)
{
  test_fetch_add (1);
  test_fetch_sub (1);
  test_fetch_and (~2);
  test_fetch_or (2);
  test_fetch_xor (2);
  test_fetch_nand (2);
  return 0;
}
