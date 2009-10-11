#ifndef _MULTIPREC_INTEGER_H
#define _MULTIPREC_INTEGER_H

#include <iostream>
#include <iomanip>

#include <tr2/integer>
#include <tr2/rational>

namespace std
{
namespace multiprec
{

  integer::integer(const rational& __q)
  : integer_base()
  {
    mpz_set_q(this->_M_get(), __q._M_get());
  }

  template<typename CharT, typename Traits>
    std::basic_ostream<CharT,Traits>&
    integer::operator<<(std::basic_ostream<CharT,Traits>& __out) const
    {
      char __str[1000];
      int __base = 10;
      if (__out.flags() & std::ios::oct)
        __base = 8;
      else if (__out.flags() & std::ios::hex)
        __base = 16;
      mpz_get_str(__str, __base, this->_M_get());
      __out << __str;
      return __out;
    }

  template<typename CharT, typename Traits>
    std::basic_istream<CharT,Traits>&
    integer::operator>>(std::basic_istream<CharT,Traits>& __in)
    {
      std::string __str;
      __in >> __str;

      int __base = 10;
      mpz_set_str(this->_M_get(), __str.c_str(), __base);
      return __in;
    }

} // namespace multiprec
} // namespace std

#endif // _MULTIPREC_INTEGER_H
