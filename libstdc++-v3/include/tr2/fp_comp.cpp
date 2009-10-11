#include <limits>

namespace std
{
  template<typename _Fp>
    bool
    fp_less(const _Fp& __a, const _Fp __b,
            const _Fp& __eps = std::numeric_limits<_Fp>::epsilon())
    { return __a < __b - __eps; }

  template<typename _Fp>
    bool
    fp_greater(const _Fp& __a, const _Fp __b,
               const _Fp& __eps = std::numeric_limits<_Fp>::epsilon())
    { return fp_less(__b, __a, __eps); }

  template<typename _Fp>
    bool
    fp_equal(const _Fp& __a, const _Fp __b,
             const _Fp& __eps = std::numeric_limits<_Fp>::epsilon())
    { return !fp_less(__a, __b, __eps) && !fp_less(__b, __a, __eps); }
}

#include <iostream>

int
main()
{
  float a = 5.0F;
  float b = 5.000001F;
  std::cout.precision(8);
  std::cout << " a = " << a << std::endl;
  std::cout << " b = " << b << std::endl;
  std::cout << " fp_less(a, b) = " << std::fp_less(a, b) << std::endl;
  std::cout << " fp_greater(a, b) = " << std::fp_greater(a, b) << std::endl;
  std::cout << " fp_equal(a, b) = " << std::fp_equal(a, b) << std::endl;

  std::cout << " fp_less(a, b, 0.001) = " << std::fp_less(a, b, 0.001) << std::endl;
  std::cout << " fp_greater(a, b, 0.001) = " << std::fp_greater(a, b, 0.001) << std::endl;
  std::cout << " fp_equal(a, b, 0.001) = " << std::fp_equal(a, b, 0.001) << std::endl;
}

