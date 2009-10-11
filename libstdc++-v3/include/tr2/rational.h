#ifndef _MULTIPREC_RATIONAL_H
#define _MULTIPREC_RATIONAL_H

#include <iostream>
#include <iomanip>

#include <tr2/integer>
#include <tr2/rational>

namespace std
{
namespace multiprec
{

  rational::rational(const integer& __i)
  : rational_base()
  {
    mpq_set_z(this->_M_get(), __i._M_get());
  }


  rational::rational(const integer& __i, const integer& __j)
  : rational_base()
  {
    mpq_set_num(this->_M_get(), __i._M_get());
    mpq_set_den(this->_M_get(), __j._M_get());
  }

  integer
  rational::numerator()
  {
    integer __num;
    mpq_get_num(__num._M_get(), this->_M_get());
    return __num;
  }

  integer
  rational::denominator()
  {
    integer __den;
    mpq_get_den(__den._M_get(), this->_M_get());
    return __den;
  }

  template<typename CharT, typename Traits>
    std::basic_ostream<CharT,Traits>&
    rational::operator<<(std::basic_ostream<CharT,Traits>& __out) const
    {
      char __str[1000];
      int __base = 10;
      if (__out.flags() & std::ios::oct)
        __base = 8;
      else if (__out.flags() & std::ios::hex)
        __base = 16;
      mpq_get_str(__str, __base, this->_M_get());
      __out << __str;
      return __out;
    }

  template<typename CharT, typename Traits>
    std::basic_istream<CharT,Traits>&
    rational::operator>>(std::basic_istream<CharT,Traits>& __in)
    {
      std::string __str;
      __in >> __str;

      int __base = 10;
      mpq_set_str(this->_M_get(), __str.c_str(), __base);
      return __in;
    }

} // namespace multiprec
} // namespace std

#endif // _MULTIPREC_RATIONAL_H
