// Class filesystem::directory_entry etc. -*- C++ -*-

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
#include <utility>
#include <stack>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

namespace fs = std::experimental::filesystem;

struct fs::_Dir
{
  _Dir() : dirp(nullptr) { }
  explicit _Dir(DIR* dirp) : dirp(dirp) { }
  ~_Dir() { if (dirp) ::closedir(dirp); }
  _Dir(_Dir&& d) : dirp(std::exchange(d.dirp, nullptr)) { }
  _Dir& operator=(_Dir&&) = delete;
  DIR* dirp;
};

namespace
{
  template<typename Bitmask>
    bool is_set(Bitmask obj, Bitmask bits)
    {
      return (obj & bits) != Bitmask::none;
    }

  struct ErrorCode
  {
    ErrorCode(std::error_code* p) : ec(p) { }

    ErrorCode(ErrorCode&& e) : ec(std::exchange(e.ec, nullptr)) { }

    ~ErrorCode() { if (ec) ec->clear(); }

    void assign(int err)
    {
      ec->assign(err, std::generic_category());
      ec = nullptr;
    }

    explicit operator bool() { return ec != nullptr; }

    std::error_code* ec;
  };

  fs::_Dir
  opendir(const fs::path& p, fs::directory_options options, ErrorCode ec)
  {
    if (DIR* dirp = ::opendir(p.native().c_str()))
      return fs::_Dir{dirp};

    const int err = errno;
    if (err == EACCES
        && is_set(options, fs::directory_options::skip_permission_denied))
      return {};

    if (!ec)
      _GLIBCXX_THROW_OR_ABORT(fs::filesystem_error(
            "directory iterator cannot open directory", p,
            std::error_code(err, std::generic_category())));

    ec.assign(err);
    return {};
  }

  inline std::shared_ptr<fs::_Dir>
  make_shared_dir(fs::_Dir dir)
  {
    if (dir.dirp)
      return std::make_shared<fs::_Dir>(std::move(dir));
    return {};
  }

  fs::directory_entry
  read_next(fs::_Dir& dir, ErrorCode ec)
  {
    ::dirent entry;
    ::dirent* result;
    if (int err = readdir_r(dir.dirp, &entry, &result))
      {
	if (!ec)
	  _GLIBCXX_THROW_OR_ABORT(fs::filesystem_error(
		"directory iterator cannot advance",
		std::error_code(err, std::generic_category())));
	ec.assign(err);
      }
    else if (result != nullptr)
      {
	if (strcmp(entry.d_name, ".") && strcmp(entry.d_name, ".."))
	  return fs::directory_entry{fs::path(entry.d_name)};
	return read_next(dir, std::move(ec));
      }
    return {};
  }
}

fs::directory_iterator::
directory_iterator(const path& p, directory_options options, error_code* ec)
: directory_iterator(make_shared_dir(opendir(p, options, ec)), ec)
{ }

fs::directory_iterator::
directory_iterator(std::shared_ptr<_Dir> dir, error_code* ec)
: _M_dir(std::move(dir))
{
  if (_M_dir)
    {
      _M_cur = read_next(*_M_dir, ec);
      if (_M_cur == directory_entry())
	_M_dir.reset();
    }
}

fs::directory_iterator&
fs::directory_iterator::operator++()
{
  error_code ec;
  increment(ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(fs::filesystem_error(
            "cannot increment directory iterator", ec));
  return *this;
}

fs::directory_iterator&
fs::directory_iterator::increment(error_code& ec) noexcept
{
  if (!_M_dir)
    _GLIBCXX_THROW_OR_ABORT(fs::filesystem_error(
	  "cannot advance non-dereferenceable directory iterator",
	  std::make_error_code(errc::invalid_argument)));

  _M_cur = read_next(*_M_dir, &ec);
  if (_M_cur == directory_entry())
    _M_dir.reset();
  return *this;
}

using Dir_iter_pair = std::pair<fs::_Dir, fs::directory_iterator>;

struct fs::recursive_directory_iterator::_Dir_stack : std::stack<Dir_iter_pair>
{ };

fs::recursive_directory_iterator::
recursive_directory_iterator(const path& p, directory_options options,
                                 error_code* ec)
: _M_opt(options), _M_pending(true)
{
  if (DIR* dirp = ::opendir(p.native().c_str()))
    {
      _M_dirs = std::make_shared<_Dir_stack>();
      _M_push( _Dir{ dirp }, ec );
      if (ec && ec->value())
	_M_reset();
    }
  else
    {
      const int err = errno;
      if (err == EACCES
	  && is_set(options, fs::directory_options::skip_permission_denied))
	return;

      if (!ec)
	_GLIBCXX_THROW_OR_ABORT(fs::filesystem_error(
	      "recursive directory iterator cannot open directory", p,
	      std::error_code(err, std::generic_category())));

      ec->assign(err, std::generic_category());
    }
}

int
fs::recursive_directory_iterator::depth() const
{
  return int(_M_dirs->size()) - 1;
}

const fs::directory_entry&
fs::recursive_directory_iterator::operator*() const
{
  return *_M_dirs->top().second;
}

fs::recursive_directory_iterator&
fs::recursive_directory_iterator::operator++()
{
  error_code ec;
  increment(ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(fs::filesystem_error(
	  "cannot increment recursive directory iterator", ec));
  return *this;
}

fs::recursive_directory_iterator&
fs::recursive_directory_iterator::increment(error_code& ec) noexcept
{
  if (!_M_dirs)
    _GLIBCXX_THROW_OR_ABORT(fs::filesystem_error(
	  "cannot advance non-dereferenceable recursive directory iterator",
	  std::make_error_code(errc::invalid_argument)));

  directory_iterator& cur = _M_dirs->top().second;
  // TODO replace these two calls with one direct call to ::stat()
  bool is_dir = is_directory(cur->status());
  bool is_symlink = fs::is_symlink(cur->status());

  // TODO check symlink cond
  // (how does it work, a dir entry can't be both dir and symlink?!)

  if (is_dir && _M_pending)
    {
      _M_push(_Dir{ opendir(cur->path(), _M_opt, &ec) }, &ec);
      // TODO do we want to do: if (ec.value()) _M_reset();
      return *this;
    }

  while (++cur == directory_iterator())
    {
      _M_dirs->pop();
      if (_M_dirs->empty())
	{
	  _M_reset();
	  return *this;
	}
      cur = _M_dirs->top().second;
    }
  return *this;
}

void
fs::recursive_directory_iterator::pop()
{
  if (!_M_dirs)
    _GLIBCXX_THROW_OR_ABORT(fs::filesystem_error(
	  "cannot pop non-dereferenceable recursive directory iterator",
	  std::make_error_code(errc::invalid_argument)));

  do {
    _M_dirs->pop();
    if (_M_dirs->empty())
      {
	_M_reset();
	return;
      }
  } while (++_M_dirs->top().second == directory_iterator());
}

void
fs::recursive_directory_iterator::_M_push(_Dir&& dir, error_code* ec)
{
  _M_dirs->emplace(std::move(dir), directory_iterator{});
  auto& top = _M_dirs->top();
  // This creates a reference cycle in _M_dirs,
  // must use _M_reset() to break it, not _M_dirs.reset()
  top.second = directory_iterator{ shared_ptr<_Dir>{_M_dirs, &top.first}, ec };
}

void
fs::recursive_directory_iterator::_M_reset()
{
    // need to empty the stack first, to break reference cycles
    _M_dirs->empty();
    _M_dirs.reset();
}

/*
bool
fs::operator==(const recursive_directory_iterator& lhs,
               const recursive_directory_iterator& rhs)
{
    return lhs._M_dirs == rhs._M_dirs;
}
*/

#if 0
template void std::deque<Dir_iter_pair>::_M_pop_back_aux();
template void std::deque<Dir_iter_pair>::_M_destroy_data_aux(iterator, iterator);
template void std::deque<Dir_iter_pair>::_M_reserve_map_at_back(size_t);
template void std::deque<Dir_iter_pair>::_M_reallocate_map(size_t, bool);
template void std::deque<Dir_iter_pair>::emplace_back<fs::_Dir, fs::directory_iterator>(fs::_Dir&&, fs::directory_iterator&&);
template void std::deque<Dir_iter_pair>::_M_push_back_aux<fs::_Dir, fs::directory_iterator>(fs::_Dir&&, fs::directory_iterator&&);
template std::_Deque_base<Dir_iter_pair, std::allocator<Dir_iter_pair>>::~_Deque_base();
template void std::_Deque_base<Dir_iter_pair, std::allocator<Dir_iter_pair>>::_M_initialize_map(size_t);

inline template class std::_Sp_counted_base<__gnu_cxx::__default_lock_policy>;
inline template class std::_Sp_counted_ptr_inplace<fs::recursive_directory_iterator::_Dir_stack, std::allocator<fs::recursive_directory_iterator::_Dir_stack>, __gnu_cxx::__default_lock_policy>;
// template void* std::_Sp_counted_ptr_inplace<fs::recursive_directory_iterator::_Dir_stack, std::allocator<fs::recursive_directory_iterator::_Dir_stack>, __gnu_cxx::__default_lock_policy>::_M_get_deleter(std::type_info const&) noexcept;
inline template class std::_Sp_counted_ptr_inplace<fs::_Dir, std::allocator<fs::_Dir>, __gnu_cxx::__default_lock_policy>;
template void* std::_Sp_counted_ptr_inplace<fs::_Dir, std::allocator<fs::_Dir>, __gnu_cxx::__default_lock_policy>::_M_get_deleter(std::type_info const&) noexcept;
#endif
