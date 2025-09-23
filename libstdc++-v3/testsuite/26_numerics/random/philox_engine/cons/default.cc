// { dg-do run { target c++26 } }
// { dg-require-cstdint "" }

// 29.5.4 Random Number Engine Class Templates
// 29.5.4.5 Class Template philox_engine

#include <random>
#include <testsuite_hooks.h>

void
test01()
{
  std::philox_engine<std::uint_fast32_t,
		     32, 4, 10, 0xCD9E8D57,
		     0x9E3779B9, 0xD2511F53,
		     0xBB67AE85> philox4x32nullkey(0);

  VERIFY( philox4x32nullkey.min() == 0 );
  VERIFY( philox4x32nullkey.max() == (1ul << 31 | (1ul << 31) - 1) );
  VERIFY( philox4x32nullkey() == 0x6627e8d5ul );
}

int main()
{
  test01();
  return 0;
}
