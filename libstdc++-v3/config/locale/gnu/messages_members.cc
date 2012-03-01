// std::messages implementation details, GNU version -*- C++ -*-

// Copyright (C) 2001, 2002, 2005, 2009, 2010, 2012
// Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

//
// ISO C++ 14882: 22.2.7.1.2  messages virtual functions
//

// Written by Benjamin Kosnik <bkoz@redhat.com>

#include <locale>
#include <bits/c++locale_internal.h>

#include <algorithm>
#include <utility>
#include <ext/concurrence.h>

namespace
{
  using namespace std;

  typedef messages_base::catalog catalog;
  typedef pair<catalog, const char*> _MapEntry;

  struct Comp
  {
    bool operator()(catalog __cat, const _MapEntry& __entry) const
    { return __cat < __entry.first; }

    bool operator()(const _MapEntry& __entry, catalog __cat) const
    { return __entry.first < __cat; }
  };

  class Catalogs
  {
  public:
    Catalogs() : _M_counter(0), _M_nb_entry(0) { }

    ~Catalogs()
    {
      if (_M_nb_entry)
	{
	  for (size_t i = 0; i != _M_nb_entry; ++i)
	    delete[] _M_map[i].second;
	  delete[] _M_map;
	}
    }

    catalog _M_add(const string& __s)
    {
      __gnu_cxx::__scoped_lock lock(_M_mutex);

      _MapEntry* __new_map = new _MapEntry[_M_nb_entry + 1];
      __try
	{
	  copy(_M_map, _M_map + _M_nb_entry, __new_map);
	  char* __s_copy = new char[__s.size() + 1];
	  __s.copy(__s_copy, __s.size());
	  __s_copy[__s.size()] = 0;
	  __new_map[_M_nb_entry] = make_pair(_M_counter, __s_copy);
	}
      __catch(...)
	{
	  delete[] __new_map;
	  __throw_exception_again;
	}


      // The counter is not likely to roll unless catalogs keep on being
      // open/close which is consider as an application mistake for the moment.
      catalog __cat = _M_counter++;
      delete[] _M_map;
      _M_map = __new_map;
      ++_M_nb_entry;

      return __cat;
    }

    void _M_erase(catalog __c)
    {
      __gnu_cxx::__scoped_lock lock(_M_mutex);

      _MapEntry* __entry =
	lower_bound(_M_map, _M_map + _M_nb_entry, __c, Comp());
      if (__entry == _M_map + _M_nb_entry || __entry->first != __c)
	return;
      
      _MapEntry* __new_map =
	_M_nb_entry > 1 ? new _MapEntry[_M_nb_entry - 1] : 0;
      copy(__entry + 1, _M_map + _M_nb_entry,
	   copy(_M_map, __entry, __new_map));

      delete[] __entry->second;
      delete[] _M_map;
      _M_map = __new_map;
      --_M_nb_entry;
    }

    pair<bool, const char*> _M_get(catalog __c) const
    {
      __gnu_cxx::__scoped_lock lock(_M_mutex);

      typedef pair<bool, const char*> _Ret;

      const _MapEntry* __entry =
	lower_bound(_M_map, _M_map + _M_nb_entry, __c, Comp());
      if (__entry != _M_map + _M_nb_entry && __entry->first == __c)
	return _Ret(true, __entry->second);
      return _Ret(false, 0);
    }

  private:
    mutable __gnu_cxx::__mutex _M_mutex;
    catalog _M_counter;
    _MapEntry* _M_map;
    size_t _M_nb_entry;
  };

  Catalogs&
  get_catalogs()
  {
    static Catalogs __catalogs;
    return __catalogs;
  }

  const char*
  get_glibc_msg(__c_locale __attribute__((unused)) __locale_messages,
		messages_base::catalog __c,
		const char* __dfault)
  {
    pair<bool, const char*> __ret = get_catalogs()._M_get(__c);

    if (!__ret.first)
      return __dfault;
#if __GLIBC__ > 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ > 2)
    std::__c_locale __old = __uselocale(__locale_messages);
    const char* __msg =
      const_cast<const char*>(dgettext(__ret.second, __dfault));
    __uselocale(__old);
    return __msg;
#else
    char* __old = setlocale(LC_ALL, 0);
    const size_t __len = strlen(__old) + 1;
    char* __sav = new char[__len];
    memcpy(__sav, __old, __len);
    setlocale(LC_ALL, _M_name_messages);
    const char* __msg = dgettext(__ret.second, __dfault);
    setlocale(LC_ALL, __sav);
    delete [] __sav;
    return __msg;
#endif
  }
}

namespace std _GLIBCXX_VISIBILITY(default)
{
  _GLIBCXX_BEGIN_NAMESPACE_VERSION

  // Specializations.
  template<>
    typename messages<char>::catalog
    messages<char>::do_open(const basic_string<char>& __s,
			    const locale&) const
    { return get_catalogs()._M_add(__s); }

  template<>
    void
    messages<char>::do_close(catalog __c) const
    { get_catalogs()._M_erase(__c); }

  template<>
    string
    messages<char>::do_get(catalog __c, int, int,
			   const string& __dfault) const
    {
      if (__c < 0)
	return __dfault;
      return string(get_glibc_msg(_M_c_locale_messages,
				  __c, __dfault.c_str()));
    }

#ifdef _GLIBCXX_USE_WCHAR_T
  template<>
    typename messages<wchar_t>::catalog
    messages<wchar_t>::do_open(const basic_string<char>& __s,
			       const locale&) const
    { return get_catalogs()._M_add(__s); }

  template<>
    void
    messages<wchar_t>::do_close(catalog __c) const
    { get_catalogs()._M_erase(__c); }

  template<>
    wstring
    messages<wchar_t>::do_get(catalog __c, int, int,
			      const wstring& __dfault) const
    {
      if (__c < 0)
	return __dfault;
      return _M_convert_from_char
	(const_cast<char*>(get_glibc_msg(_M_c_locale_messages, __c,
					 _M_convert_to_char(__dfault))));
    }
#endif

_GLIBCXX_END_NAMESPACE_VERSION
} // namespace
