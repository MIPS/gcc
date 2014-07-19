// Filesystem operations -*- C++ -*-

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
#include <stdlib.h>
#ifdef _GLIBCXX_HAVE_UNISTD_H
# include <unistd.h>
# if defined(_GLIBCXX_HAVE_SYS_STAT_H) && defined(_GLIBCXX_HAVE_SYS_TYPES_H)
#  include <sys/types.h>
#  include <sys/stat.h>
# endif
#endif

namespace fs = std::experimental::filesystem;

fs::path
fs::absolute(const path& p, const path& base)
{
  const bool has_root_dir = p.has_root_directory();
  const bool has_root_name = p.has_root_name();
  path abs;
  if (has_root_dir && has_root_name)
    abs = p;
  else
    {
      abs = absolute(base);
      if (has_root_dir)
	abs = abs.root_name() / p;
      else if (has_root_name)
	abs = p.root_name() / abs.root_directory() / abs.relative_path()
	  / p.relative_path();
      else
	abs = abs / p;
    }
  return abs;
}

fs::path
fs::canonical(const path& p, const path& base, error_code& ec)
{
  path can;
#ifdef _GLIBCXX_HAVE_REALPATH
  std::unique_ptr<char, void(*)(void*)> ptr{
    ::realpath(absolute(p, base).native().c_str(), nullptr), &::free
  };
  if (result)
    can.assign(ptr.get());
  else
    ec.assign(errno, std::generic_category());
#else
  ec = std::make_error_code(std::errc::not_supported);
#endif
  return can;
}

fs::path
fs::canonical(const path& p, error_code& ec)
{
  return canonical(p, current_path(), ec);
}

fs::path
fs::canonical(const path& p, const path& base)
{
  error_code ec;
  path can = canonical(p, base, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot canonicalize", p, ec));
  return can;
}

void
fs::copy(const path& from, const path& to, copy_options options);

void
fs::copy(const path& from, const path& to, copy_options options,
	 error_code& ec) noexcept;

bool
fs::copy_file(const path& from, const path& to, copy_options option);

bool
fs::copy_file(const path& from, const path& to, copy_options option,
	      error_code& ec) noexcept;

void
fs::copy_symlink(const path& existing_symlink, const path& new_symlink);

void
fs::copy_symlink(const path& existing_symlink, const path& new_symlink,
		 error_code& ec) noexcept;

bool
fs::create_directories(const path& p)
{
  error_code ec;
  bool result = create_directories(p, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot create directories", p,
	  ec));
  return result;
}

bool
fs::create_directories(const path& p, error_code& ec) noexcept;

bool
fs::create_directory(const path& p)
{
  error_code ec;
  bool result = create_directory(p, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot create directory", p,
	  ec));
  return result;
}

bool
fs::create_directory(const path& p, error_code& ec) noexcept;

bool
fs::create_directory(const path& p, const path& attributes);
bool
fs::create_directory(const path& p, const path& attributes,
		     error_code& ec) noexcept;

void
fs::create_directory_symlink(const path& to, const path& new_symlink);
void
fs::create_directory_symlink(const path& to, const path& new_symlink,
			     error_code& ec) noexcept;

void
fs::create_hard_link(const path& to, const path& new_hard_link);
void
fs::create_hard_link(const path& to, const path& new_hard_link,
		     error_code& ec) noexcept;

void fs::create_symlink(const path& to, const path& new_symlink);
void fs::create_symlink(const path& to, const path& new_symlink,
		    error_code& ec) noexcept;

fs::path
fs::current_path()
{
  error_code ec;
  path p = current_path(ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot get current path", ec));
  return p;
}

fs::path
fs::current_path(error_code& ec)
{
  // TODO return ::getcwd
}

void fs::current_path(const path& p)
{
  error_code ec;
  current_path(ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot get current path", ec));
}

void fs::current_path(const path& p, error_code& ec) noexcept;

bool fs::equivalent(const path& p1, const path& p2);
bool fs::equivalent(const path& p1, const path& p2, error_code& ec) noexcept;

uintmax_t fs::file_size(const path& p);
uintmax_t fs::file_size(const path& p, error_code& ec) noexcept;

uintmax_t fs::hard_link_count(const path& p);
uintmax_t fs::hard_link_count(const path& p, error_code& ec) noexcept;

bool
fs::is_empty(const path& p)
{
  return fs::is_directory(status(p))
    ? fs::directory_iterator(p) == fs::directory_iterator()
    : fs::file_size(p) == 0;
}

bool
fs::is_empty(const path& p, error_code& ec) noexcept
{
  // TODO file issue, spec for is_empty() should use "[, ec]"
  auto s = status(p, ec);
  if (ec.value())
    return false;
  return fs::is_directory(s)
    ? fs::directory_iterator(p, ec) == fs::directory_iterator()
    : fs::file_size(p, ec) == 0;
}

fs::file_time_type fs::last_write_time(const path& p);
fs::file_time_type  fs::last_write_time(const path& p, error_code& ec) noexcept;
void fs::last_write_time(const path& p, file_time_type new_time);
void fs::last_write_time(const path& p, file_time_type new_time,
		     error_code& ec) noexcept;

void fs::permissions(const path& p, perms prms);
void fs::permissions(const path& p, perms prms, error_code& ec) noexcept;

fs::path fs::read_symlink(const path& p);
fs::path fs::read_symlink(const path& p, error_code& ec);

bool fs::remove(const path& p);
bool fs::remove(const path& p, error_code& ec) noexcept;

uintmax_t fs::remove_all(const path& p);
uintmax_t fs::remove_all(const path& p, error_code& ec) noexcept;

void fs::rename(const path& from, const path& to);
void fs::rename(const path& from, const path& to, error_code& ec) noexcept;

void fs::resize_file(const path& p, uintmax_t size);
void fs::resize_file(const path& p, uintmax_t size, error_code& ec) noexcept;

fs::space_info fs::space(const path& p);
fs::space_info fs::space(const path& p, error_code& ec) noexcept;

#ifdef _GLIBCXX_HAVE_SYS_STAT_H
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
#endif

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
fs::symlink_status(const fs::path& p)
{
  std::error_code ec;
  auto s = symlink_status(p, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error(
	  "symlink_status", ec));
  return s;
}

fs::path fs::system_complete(const path& p);
fs::path fs::system_complete(const path& p, error_code& ec);

fs::path fs::temp_directory_path();
fs::path fs::temp_directory_path(error_code& ec);

