// Class filesystem::path -*- C++ -*-

// Copyright (C) 2014 Free Software Foundation, Inc.
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

#include <experimental/filesystem>

using std::experimental::filesystem::path;

constexpr path::value_type path::preferred_separator;

path&
path::remove_filename()
{
  // TODO find filename cmpt, erase from list,
  // use string::erase to remove tail of _M_pathname
  return *this;
}

path&
path::replace_filename(const path& __replacement)
{
  // TODO find filename cmpt, replace in list
  // use string::assign to replace tail of _M_pathname
  (void)__replacement;
  return *this;
}

path&
path::replace_extension(const path& __replacement)
{
  // TODO find filename cmpt, use _M_split_filename,
  // use string::assign to replace tail of _M_pathname
  (void)__replacement;
  return *this;
}

namespace
{
  template<typename Iter1, typename Iter2>
    int do_compare(Iter1 begin1, Iter1 end1, Iter2 begin2, Iter2 end2)
    {
      int cmpt = 1;
      while (begin1 != end1 && begin2 != end2)
	{
	  if (begin1->native() < begin2->native())
	    return -cmpt;
	  if (begin1->native() > begin2->native())
	    return +cmpt;
	  ++begin1;
	  ++begin2;
	  ++cmpt;
	}
      if (begin1 == end1)
	{
	  if (begin2 == end2)
	    return 0;
	  return -cmpt;
	}
      return +cmpt;
    }
}

int
path::compare(const path& p) const noexcept
{
  if (_M_type == _Type::_Multi && p._M_type == _Type::_Multi)
    return do_compare(_M_cmpts.begin(), _M_cmpts.end(),
		   p._M_cmpts.begin(), p._M_cmpts.end());
  else if (_M_type == _Type::_Multi)
    {
      _Cmpt c[1] = { { p._M_pathname, p._M_type, 0 } };
      return do_compare(_M_cmpts.begin(), _M_cmpts.end(), c, c+1);
    }
  else if (p._M_type == _Type::_Multi)
    {
      _Cmpt c[1] = { { _M_pathname, _M_type, 0 } };
      return do_compare(c, c+1, p._M_cmpts.begin(), p._M_cmpts.end());
    }
  else
    return _M_pathname.compare(p._M_pathname);
}

path
path::root_name() const
{
  path __ret;
  if (_M_type == _Type::_Root_name)
    __ret = *this;
  else if (_M_cmpts.size()
      && _M_cmpts.begin()->_M_type == _Type::_Root_name)
    __ret = *_M_cmpts.begin();
  return __ret;
}

path
path::root_directory() const
{
  path __ret;
  if (_M_type == _Type::_Root_dir)
    __ret = *this;
  else if (!_M_cmpts.empty())
    {
      auto __it = _M_cmpts.begin();
      if (__it->_M_type == _Type::_Root_name)
        ++__it;
      if (__it != _M_cmpts.end() && __it->_M_type == _Type::_Root_dir)
        __ret = *__it;
    }
  return __ret;
}


path
path::root_path() const
{
  path __ret;
  if (_M_type == _Type::_Root_name || _M_type == _Type::_Root_dir)
    __ret = *this;
  else if (!_M_cmpts.empty())
    {
      auto __it = _M_cmpts.begin();
      if (__it->_M_type == _Type::_Root_name)
        {
          __ret = *__it++;
          if (__it != _M_cmpts.end() && __it->_M_type == _Type::_Root_dir)
            {
              __ret._M_pathname += preferred_separator;
              __ret._M_split_cmpts();
            }
        }
      else if (__it->_M_type == _Type::_Root_dir)
        __ret = *__it;
    }
  return __ret;
}

path
path::relative_path() const
{
  path __ret;
  if (!_M_cmpts.empty())
    {
      auto __it = _M_cmpts.begin();
      if (__it->_M_type == _Type::_Root_name)
        ++__it;
      if (__it != _M_cmpts.end() && __it->_M_type == _Type::_Root_dir)
        ++__it;
      if (__it != _M_cmpts.end())
        __ret.assign(_M_pathname.substr(__it->_M_pos));
    }
  return __ret;
}

path
path::parent_path() const
{
  path __ret;
  if (_M_cmpts.size() < 2)
    return __ret;
  for (auto __it = _M_cmpts.begin(), __end = --_M_cmpts.end();
      __it != __end; ++__it)
    {
      __ret /= *__it;
    }
  return __ret;
}

path
path::_M_split_filename(path __filename, _Split __which) const
{
  auto __pos = string_type::npos;
  if (__filename._M_pathname.size() < 3
      && __filename._M_pathname.find_first_not_of('.') == string_type::npos)
    __pos = __filename._M_pathname.size();
  else
    __pos = __filename._M_pathname.find_last_of('.');
  if (__which == _Split::_Stem)
    __filename._M_pathname.erase(__pos);
  else
    __filename._M_pathname.erase(0, __pos);
  return __filename;
}

bool
path::has_root_name() const
{
  if (_M_type == _Type::_Root_name)
    return true;
  if (!_M_cmpts.empty() && _M_cmpts.begin()->_M_type == _Type::_Root_name)
    return true;
  return false;
}

bool
path::has_root_directory() const
{
  if (_M_type == _Type::_Root_dir)
    return true;
  if (!_M_cmpts.empty())
    {
      auto __it = _M_cmpts.begin();
      if (__it->_M_type == _Type::_Root_name)
        ++__it;
      if (__it != _M_cmpts.end() && __it->_M_type == _Type::_Root_dir)
        return true;
    }
  return false;
}

bool
path::has_root_path() const
{
  if (_M_type == _Type::_Root_name || _M_type == _Type::_Root_dir)
    return true;
  if (!_M_cmpts.empty())
    {
      auto __type = _M_cmpts.front()._M_type;
      if (__type == _Type::_Root_name || __type == _Type::_Root_dir)
        return true;
    }
  return false;
}

bool
path::has_relative_path() const
{
  if (_M_type == _Type::_Filename)
    return true;
  if (!_M_cmpts.empty())
    {
      auto __it = _M_cmpts.begin();
      if (__it->_M_type == _Type::_Root_name)
        ++__it;
      if (__it != _M_cmpts.end() && __it->_M_type == _Type::_Root_dir)
        ++__it;
      if (__it != _M_cmpts.end())
        return true;
    }
  return false;
}


bool
path::has_parent_path() const
{
  return false; // TODO
}

bool
path::has_filename() const
{
  return false; // TODO
}

bool
path::has_stem() const
{
  return false; // TODO
}

bool
path::has_extension() const
{
  return false; // TODO
}

void
path::_M_split_cmpts()
{
  _M_type = _Type::_Multi;
  _M_cmpts.clear();

  if (_M_pathname.empty())
    return;

#ifdef _GLIBCXX_FILESYSTEM_IS_WINDOWS
  // identify disk designators like "C:/"
  auto __is_disk_designator = [](string_type const& __str) {
      return __str.size() == 2 && __str.back() == L':';
  };
  // identify UNCs like "//servername/sharename"
  auto __is_unc = [](string_type const& __str) {
      return __str.size() > 1
        && _S_is_dir_sep(__str[0]) && _S_is_dir_sep(__str[1]);
  };
#else
  auto __is_disk_designator = [](string_type const&) { return false; };
  auto __is_unc = [](string_type const&) { return false; };
#endif

  string_type __elem;
  size_t __pos = -1;

  for (value_type& __ch : _M_pathname)
    {
      if (_S_is_dir_sep(__ch))
        {
          if (!__elem.empty())
            {
              if (_M_cmpts.empty())
                {
                  if (__is_unc(_M_pathname))
                    _M_cmpts.emplace_back(_M_pathname.substr(0, 2) + __elem,
                                          _Type::_Root_name, 0);
                  else if (_S_is_dir_sep(_M_pathname[0]))
                    {
                      _M_cmpts.emplace_back(_M_pathname.substr(0, 1),
                                            _Type::_Root_dir, 0);
                      _M_cmpts.emplace_back(std::move(__elem),
                                            _Type::_Filename, __pos);
                    }
                  else if (__is_disk_designator(__elem))
                      _M_cmpts.emplace_back(std::move(__elem),
                                            _Type::_Root_name, 0);
                  else
                    _M_cmpts.emplace_back(std::move(__elem),
                                          _Type::_Filename, __pos);
                }
              else
                _M_cmpts.emplace_back(std::move(__elem), _Type::_Filename,
                                      __pos);
              __elem.clear();
            }
          else if (_M_cmpts.size() == 0
	      || (_M_cmpts.size() == 1
		&& _M_cmpts.front()._M_type == _Type::_Root_name))
            {
              _M_cmpts.emplace_back(string_type{1, __ch},
                                    _Type::_Root_dir,
                                    &__ch - _M_pathname.data());
            }
          // else adjacent dir separators, ignore
        }
      else
        {
          if (__elem.empty())
            __pos = &__ch - _M_pathname.data();
          __elem += __ch;
        }
    }

  if (_S_is_dir_sep(_M_pathname.back()))
    {
      if (_M_cmpts.empty())
        _M_type = _Type::_Root_dir;
      else if (_M_cmpts.back()._M_type == _Type::_Filename)
        _M_cmpts.emplace_back(string_type{1, '.'}, _Type::_Filename,
                              string_type::npos);
    }
  else if (!__elem.empty())
    _M_cmpts.emplace_back(std::move(__elem), _Type::_Filename, __pos);

  if (_M_cmpts.size() == 1)
    {
      _M_type = _M_cmpts.front()._M_type;
      _M_cmpts.clear();
    }
}

path::string_type
path::_S_convert_loc(const char* __first, const char* __last,
               const std::locale& __loc)
{
  auto& __cvt = use_facet<std::codecvt<wchar_t, char, mbstate_t>>(__loc);
  mbstate_t __st = mbstate_t();
  size_t __len = __cvt.length(__st, __first, __last,
                              (__last - __first) * __cvt.max_length());
  std::wstring __out(__len, L'\0');
  auto __outnext = &__out.front();
  __st = mbstate_t();
  auto __res = __cvt.in(__st, __first, __last, __first,
                        __outnext, &__out.back() + 1, __outnext);
  if (__res == codecvt_base::ok && __first == __last)
    return _Cvt<wchar_t>::_S_convert(&__out.front(), __outnext);
  _GLIBCXX_THROW_OR_ABORT(filesystem_error(
	"Cannot convert character sequence",
	std::make_error_code(errc::illegal_byte_sequence)));
}

#if 0
// template class std::list<path::_Cmpt>;

template void std::list<path::_Cmpt>::_M_assign_dispatch<std::experimental::filesystem::v1::path::_Cmpt const*>(path::_Cmpt const*, path::_Cmpt const*, std::__false_type);
#endif
