#ifndef _MULTIPREC_REAL_H
#define _MULTIPREC_REAL_H

#include <iostream>
#include <iomanip>
#include <cstring> // For strlen.
#include <cmath> // For max.

#include <tr2/integer>
#include <tr2/rational>
#include <tr2/real>

namespace std
{
namespace multiprec
{

  real::real(const integer& __i)
  : real_base()
  {
    mpfr_set_z(this->_M_get(), __i._M_get(), this->_M_get_round());
  }


#ifdef _MP_NUMERIC_CAST
  real::operator integer() const
  {
    integer __ret;
    return mpfr_get_z(__ret._M_get(), this->_M_get(), this->_M_get_round());
    return __ret;
  }
#endif // _MP_NUMERIC_CAST

  real::real(const rational& __q)
  : real_base()
  {
    mpfr_set_q(this->_M_get(), __q._M_get(), this->_M_get_round());
  }

  template<typename CharT, typename Traits>
    std::basic_ostream<CharT,Traits>&
    real::operator<<(std::basic_ostream<CharT,Traits>& __out) const
    {
      const std::size_t __mant_len = __out.precision();
      const std::size_t __str_len = std::max(__mant_len + 2UL, 7UL);
      char __str[__str_len];
      mp_exp_t __exp;
      int __base = 10;
      if (__out.flags() & std::ios::oct)
        __base = 8;
      else if (__out.flags() & std::ios::hex)
        __base = 16;
      mpfr_get_str(__str, &__exp, __base, __mant_len, this->_M_get(),
                   this->_M_get_round());

      //  Cancel terminating zeros.
      for (size_t __i = std::strlen(__str) - 1;
           __str[__i] == '0' && __i > 0; --__i)
        __str[__i] = '\0';

      //  Show the appropriate sign.
      std::size_t __pos = 0;
      if (__str[0] == '-')
      {
        __out << "-";
        __pos = 1;
      }
      else if (__out.flags() & std::ios::showpos)
        __out << "+";

      //  Show the appropriate base.
      if (__out.flags() & std::ios::showbase)
      {
        if (__base == 8)
          __out << "0";
        else if (__base == 16)
        {
          if (__out.flags() & std::ios::uppercase)
            __out << "0X";
          else
            __out << "0x";
        }
      }

      if (__base == 16 && (__out.flags() & std::ios::uppercase))
        for (size_t __i = std::strlen(__str) - 1;
             __str[__i] == '0' && __i > 0; --__i)
          if (__str[__i] >= 'a' && __str[__i] >= 'f')
            __str[__i] += 'A' - 'a';

      __out << "0.";
      __out << __str + __pos;

      //  Show the exponent. 
      if (__base <= 10)
        if (__out.flags() & std::ios::uppercase)
          __out << "E";
        else
          __out << "e";
      else if (__base == 16)
        if (__out.flags() & std::ios::uppercase)
          __out << "P";
        else
          __out << "p";
      else
        __out << "@";
      if (exp >= 0)
        __out << "+";
      __out << __exp;

      return __out;
    }

  template<typename CharT, typename Traits>
    std::basic_istream<CharT,Traits>&
    real::operator>>(std::basic_istream<CharT,Traits>& __in)
    {
      std::string __str;
      __in >> __str;
      mpfr_set_str(this->_M_get(), __str.c_str(), __str.length() + 1,
                   this->_M_get_round());
      return __in;
    }

} // namespace multiprec
} // namespace std

#endif // _MULTIPREC_REAL_H
