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
#include <tuple>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#define _GLIBCXX_USE_DIRENT_D_TYPE 1

namespace fs = std::experimental::filesystem;

namespace
{
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
}

struct fs::_Dir
{
  _Dir() : dirp(nullptr) { }

  _Dir(DIR* dirp, const fs::path& path) : dirp(dirp), path(path) { }

  _Dir(_Dir&& d)
  : dirp(std::exchange(d.dirp, nullptr)), path(std::move(d.path)),
    entry(std::move(d.entry)), type(d.type)
  { }

  _Dir& operator=(_Dir&&) = delete;

  ~_Dir() { if (dirp) ::closedir(dirp); }

  bool advance(ErrorCode);
  DIR*			dirp;
  fs::path		path;
  directory_entry	entry;
  file_type		type = file_type::not_found;
};

namespace
{
  template<typename Bitmask>
    bool is_set(Bitmask obj, Bitmask bits)
    {
      return (obj & bits) != Bitmask::none;
    }

  fs::_Dir
  opendir(const fs::path& p, fs::directory_options options, ErrorCode ec)
  {
    if (DIR* dirp = ::opendir(p.c_str()))
      return {dirp, p};

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
  make_shared_dir(fs::_Dir&& dir)
  {
    if (dir.dirp)
      return std::make_shared<fs::_Dir>(std::move(dir));
    return {};
  }

  inline fs::file_type
  get_file_type(const dirent& d)
  {
    switch (d.d_type)
    {
#if _GLIBCXX_USE_DIRENT_D_TYPE
    case DT_BLK:
      return fs::file_type::block;
    case DT_CHR:
      return fs::file_type::character;
    case DT_DIR:
      return fs::file_type::directory;
    case DT_FIFO:
      return fs::file_type::fifo;
    case DT_LNK:
      return fs::file_type::symlink;
    case DT_REG:
      return fs::file_type::regular;
    case DT_SOCK:
      return fs::file_type::socket;
    case DT_UNKNOWN:
      // fall through, so we call status() as needed
#endif
    default:
      return fs::file_type::not_found;
    }
  }
}

bool
fs::_Dir::advance(ErrorCode ec)
{
  ::dirent ent;
  ::dirent* result;
  if (int err = readdir_r(dirp, &ent, &result))
    {
      if (!ec)
	_GLIBCXX_THROW_OR_ABORT(filesystem_error(
	      "directory iterator cannot advance",
	      std::error_code(err, std::generic_category())));
      ec.assign(err);
      return true;
    }
  else if (result != nullptr)
    {
      // skip past dot and dot-dot
      if (!strcmp(ent.d_name, ".") || !strcmp(ent.d_name, ".."))
	return advance(std::move(ec));
      entry = fs::directory_entry{path / ent.d_name};
      type = get_file_type(ent);
      return true;
    }
  else
    {
      // reached the end
      entry = {};
      type = fs::file_type::none;
      return false;
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
  if (_M_dir && !_M_dir->advance(ec))
    _M_dir.reset();
}

const fs::directory_entry&
fs::directory_iterator::operator*() const
{
  if (!_M_dir)
    _GLIBCXX_THROW_OR_ABORT(filesystem_error(
	  "non-dereferenceable directory iterator",
	  std::make_error_code(errc::invalid_argument)));
  return _M_dir->entry;
}

fs::directory_iterator&
fs::directory_iterator::operator++()
{
  if (!_M_dir)
    _GLIBCXX_THROW_OR_ABORT(filesystem_error(
	  "cannot advance non-dereferenceable directory iterator",
	  std::make_error_code(errc::invalid_argument)));
  if (!_M_dir->advance(nullptr))
    _M_dir.reset();
  return *this;
}

fs::directory_iterator&
fs::directory_iterator::increment(error_code& ec) noexcept
{
  if (!_M_dir)
    {
      ec = std::make_error_code(errc::invalid_argument);
      return *this;
    }
  if (!_M_dir->advance(&ec))
    _M_dir.reset();
  return *this;
}

using Dir_iter_pair = std::pair<fs::_Dir, fs::directory_iterator>;

struct fs::recursive_directory_iterator::_Dir_stack : std::stack<Dir_iter_pair>
{
  // need to empty the stack first, to break reference cycles
  ~_Dir_stack() { c.clear(); }
};

fs::recursive_directory_iterator::
recursive_directory_iterator(const path& p, directory_options options,
                                 error_code* ec)
: _M_options(options), _M_pending(true)
{
  if (DIR* dirp = ::opendir(p.c_str()))
    {
      _M_dirs = std::make_shared<_Dir_stack>();
      _M_push( _Dir{ dirp, p }, ec );
      if (ec && ec->value())
	_M_dirs.reset();
    }
  else
    {
      const int err = errno;
      if (err == EACCES
	  && is_set(options, fs::directory_options::skip_permission_denied))
	return;

      if (!ec)
	_GLIBCXX_THROW_OR_ABORT(filesystem_error(
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
    _GLIBCXX_THROW_OR_ABORT(filesystem_error(
	  "cannot increment recursive directory iterator", ec));
  return *this;
}

namespace
{
  fs::file_status
  map_status(struct ::stat& st)
  {
    using fs::file_status;
    using fs::file_type;
    using fs::perms;
    file_type ft;
    perms perm = static_cast<perms>(st.st_mode) & perms::mask;
    if (S_ISREG(st.st_mode))
      ft = file_type::regular;
    else if (S_ISDIR(st.st_mode))
      ft = file_type::directory;
    else if (S_ISCHR(st.st_mode))
      ft = file_type::character;
    else if (S_ISBLK(st.st_mode))
      ft = file_type::block;
    else if (S_ISFIFO(st.st_mode))
      ft = file_type::fifo;
    else if (S_ISLNK(st.st_mode))
      ft = file_type::symlink;
    else if (S_ISSOCK(st.st_mode))
      ft = file_type::socket;
    else
      ft = file_type::unknown;
    return file_status{ft, perm};
  }
}

fs::file_status
fs::status(const fs::path& p, std::error_code& ec) noexcept
{
    file_status status;
    struct ::stat st;
    int err = ::stat(p.native().c_str(), &st);
    if (err)
      ec.assign(err, std::generic_category());
    else
      status = map_status(st);
    return status;
}

fs::file_status
fs::status(const fs::path& p)
{
  std::error_code ec;
  auto s = status(p, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error(
	  "status", ec));
  return s;
}

fs::file_status
fs::symlink_status(const fs::path& p, std::error_code& ec) noexcept
{
    file_status status;
    struct ::stat st;
    int err = ::lstat(p.native().c_str(), &st);
    if (err)
      ec.assign(err, std::generic_category());
    else
      status = map_status(st);
    return status;
}

fs::file_status
fs::symlink_status(const fs::path& p)
{
  std::error_code ec;
  auto s = symlink_status(p, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error(
	  "symlink_status", ec));
  return s;
}

namespace
{
  bool
  recurse(const fs::_Dir& d, fs::directory_options options, std::error_code& ec)
  {
    bool follow_symlink
      = is_set(options, fs::directory_options::follow_directory_symlink);
#if _GLIBCXX_USE_DIRENT_D_TYPE
    if (d.type == fs::file_type::directory)
      return true;
    if (d.type == fs::file_type::symlink && follow_symlink)
      return d.entry.status().type() == fs::file_type::directory;
    if (d.type != fs::file_type::not_found)
      return false;
#endif
    const fs::path& path = d.entry.path();
    auto type = fs::symlink_status(path, ec).type();
    if (ec.value())
      return false;
    if (type == fs::file_type::symlink)
      {
	if (!follow_symlink)
	  return false;
	type = fs::status(path, ec).type();
      }
    return type == fs::file_type::directory;
  }
}

fs::recursive_directory_iterator&
fs::recursive_directory_iterator::increment(error_code& ec) noexcept
{
  if (!_M_dirs)
    {
      ec = std::make_error_code(errc::invalid_argument);
      return *this;
    }

  auto& top = _M_dirs->top();
  directory_iterator* cur = &top.second;

  if (std::exchange(_M_pending, true) && recurse(top.first, _M_options, ec))
    {
      _M_push(_Dir{ opendir((*cur)->path(), _M_options, &ec) }, &ec);
      if (ec.value())
	if (ec != std::make_error_code(errc::permission_denied)
	    || !is_set(_M_options, directory_options::skip_permission_denied))
	  return *this;
      if (_M_dirs->top().second == directory_iterator())
	pop();
      return *this;
    }

  while (++*cur == directory_iterator())
    {
      _M_dirs->pop();
      if (_M_dirs->empty())
	{
	  _M_dirs.reset();
	  return *this;
	}
      cur = &_M_dirs->top().second;
    }
  return *this;
}

void
fs::recursive_directory_iterator::pop()
{
  if (!_M_dirs)
    _GLIBCXX_THROW_OR_ABORT(filesystem_error(
	  "cannot pop non-dereferenceable recursive directory iterator",
	  std::make_error_code(errc::invalid_argument)));

  do {
    _M_dirs->pop();
    if (_M_dirs->empty())
      {
	_M_dirs.reset();
	return;
      }
  } while (++_M_dirs->top().second == directory_iterator());
}

void
fs::recursive_directory_iterator::_M_push(_Dir&& dir, error_code* ec)
{
  if (!dir.dirp)
    return;
  _M_dirs->emplace(std::move(dir), directory_iterator{});
  auto& top = _M_dirs->top();
  // N.B. This creates a reference cycle in _M_dirs
  top.second = directory_iterator{ shared_ptr<_Dir>{_M_dirs, &top.first}, ec };
}

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
