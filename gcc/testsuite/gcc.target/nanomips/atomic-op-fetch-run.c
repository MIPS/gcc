/* { dg-do run } */
/* { dg-options "-ffat-lto-objects" } */

#include "atomic-add-fetch.x"
#include "atomic-sub-fetch.x"
#include "atomic-and-fetch.x"
#include "atomic-or-fetch.x"
#include "atomic-xor-fetch.x"
#include "atomic-nand-fetch.x"

int main (void)
{
  test_add_fetch (1);
  test_sub_fetch (1);
  test_and_fetch (~2);
  test_or_fetch (2);
  test_xor_fetch (2);
  test_nand_fetch (2);
  return 0;
}
