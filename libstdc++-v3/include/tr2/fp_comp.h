#include <limits>

/*
 *  This could go in cmath.
 */

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

