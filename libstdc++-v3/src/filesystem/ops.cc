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
#include <functional>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#ifdef _GLIBCXX_HAVE_UNISTD_H
# include <unistd.h>
# if defined(_GLIBCXX_HAVE_SYS_STAT_H) && defined(_GLIBCXX_HAVE_SYS_TYPES_H)
#  include <sys/types.h>
#  include <sys/stat.h>
# endif
#endif
#ifdef _GLIBCXX_HAVE_FCNTL_H
# include <fcntl.h>
#endif
#ifdef _GLIBCXX_HAVE_SYS_STATVFS_H
# include <sys/statvfs.h>
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
      abs = base.is_absolute() ? base : absolute(base);
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

namespace
{
  struct free_as_in_malloc
  {
    void operator()(void* p) const { ::free(p); }
  };

  using char_ptr = std::unique_ptr<char[], free_as_in_malloc>;
}

fs::path
fs::canonical(const path& p, const path& base, error_code& ec)
{
  path can;
#ifdef _GLIBCXX_USE_REALPATH
  if (char_ptr rp = char_ptr{::realpath(absolute(p, base).c_str(), nullptr)})
    {
      can.assign(rp.get());
      ec.clear();
    }
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
  path cur = current_path(ec);
  if (ec.value())
    return {};
  return canonical(p, cur, ec);
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
fs::copy(const path& from, const path& to, copy_options options)
{
  error_code ec;
  copy(from, to, options, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot copy", from, to, ec));
}

void
fs::copy(const path& from, const path& to, copy_options options,
	 error_code& ec) noexcept
{
  // TODO
}

bool
fs::copy_file(const path& from, const path& to, copy_options option)
{
  error_code ec;
  bool result = copy_file(from, to, option, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot copy file", from, to,
	  ec));
  return result;
}

bool
fs::copy_file(const path& from, const path& to, copy_options option,
	      error_code& ec) noexcept
{
  // TODO
}


void
fs::copy_symlink(const path& existing_symlink, const path& new_symlink)
{
  error_code ec;
  copy_symlink(existing_symlink, new_symlink, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot copy symlink",
	  existing_symlink, new_symlink, ec));
}

void
fs::copy_symlink(const path& existing_symlink, const path& new_symlink,
		 error_code& ec) noexcept
{
  // TODO
}


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
fs::create_directories(const path& p, error_code& ec) noexcept
{
  // TODO
}

namespace
{
  bool
  create_dir(const fs::path& p, fs::perms perm, std::error_code& ec)
  {
#ifdef _GLIBCXX_HAVE_SYS_STAT_H
    ::mode_t mode = static_cast<std::underlying_type_t<fs::perms>>(perm);
    if (::mkdir(p.c_str(), mode))
      {
	ec.assign(errno, std::generic_category());
	return false;
      }
    else
      {
	ec.clear();
	return true;
      }
#else
    ec = std::make_error_code(std::errc::not_supported);
    return false;
#endif
  }
} // namespace

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
fs::create_directory(const path& p, error_code& ec) noexcept
{
  return create_dir(p, perms::all, ec);
}


bool
fs::create_directory(const path& p, const path& attributes)
{
  error_code ec;
  bool result = create_directory(p, attributes, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot create directory", p,
	  ec));
  return result;
}

bool
fs::create_directory(const path& p, const path& attributes,
		     error_code& ec) noexcept
{
#ifdef _GLIBCXX_HAVE_SYS_STAT_H
  struct ::stat st;
  if (::stat(attributes.c_str(), &st))
    {
      ec.assign(errno, std::generic_category());
      return false;
    }
  return create_dir(p, static_cast<perms>(st.st_mode), ec);
#else
  ec = std::make_error_code(std::errc::not_supported);
  return false;
#endif
}


void
fs::create_directory_symlink(const path& to, const path& new_symlink)
{
  error_code ec;
  create_directory_symlink(to, new_symlink, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot create directory symlink",
	  to, new_symlink, ec));
}

void
fs::create_directory_symlink(const path& to, const path& new_symlink,
			     error_code& ec) noexcept
{
#ifdef _GLIBCXX_FILESYSTEM_IS_WINDOWS
  ec = std::make_error_code(std::errc::not_supported);
#else
  create_symlink(to, new_symlink, ec);
#endif
}


void
fs::create_hard_link(const path& to, const path& new_hard_link)
{
  error_code ec;
  create_hard_link(to, new_hard_link, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot create hard link",
	  to, new_hard_link, ec));
}

void
fs::create_hard_link(const path& to, const path& new_hard_link,
		     error_code& ec) noexcept
{
#ifdef _GLIBCXX_HAVE_UNISTD_H
  if (::link(to.c_str(), new_hard_link.c_str()))
    ec.assign(errno, std::generic_category());
  else
    ec.clear();
#else
  ec = std::make_error_code(std::errc::not_supported);
#endif
}

void
fs::create_symlink(const path& to, const path& new_symlink)
{
  error_code ec;
  create_symlink(to, new_symlink, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot create symlink",
	  to, new_symlink, ec));
}

void
fs::create_symlink(const path& to, const path& new_symlink,
		   error_code& ec) noexcept
{
#ifdef _GLIBCXX_HAVE_UNISTD_H
  if (::symlink(to.c_str(), new_symlink.c_str()))
    ec.assign(errno, std::generic_category());
  else
    ec.clear();
#else
  ec = std::make_error_code(std::errc::not_supported);
#endif
}


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
  path p;
#ifdef _GLIBCXX_HAVE_UNISTD_H
#ifdef __GLIBC__
  if (char_ptr cwd = char_ptr{::getcwd(nullptr, 0)})
    {
      p.assign(cwd.get());
      ec.clear();
    }
  else
    ec.assign(errno, std::generic_category());
#else
  long path_max = pathconf(".", _PC_PATH_MAX);
  size_t size;
  if (path_max == -1)
      size = 1024;
  else if (path_max > 10240)
      size = 10240;
  else
      size = path_max;
  for (char_ptr buf; p.empty(); size *= 2)
    {
      if (char* ptr = realloc(buf.release(), size))
	{
	  buf.reset(ptr);
	  if (char* ptr = getcwd(buf.get(), size))
	    {
	      p.assign(buf.get());
	      ec.clear();
	    }
	  if (errno != ERANGE)
	    {
	      ec.assign(errno, std::generic_category());
	      return {};
	    }
	}
      else
	{
	  ec = std::make_error_code(std::errc::not_enough_memory);
	  return {};
	}
    }
#endif  // __GLIBC__
#else   // _GLIBCXX_HAVE_UNISTD_H
  ec = std::make_error_code(std::errc::not_supported);
#endif
  return p;
}

void
fs::current_path(const path& p)
{
  error_code ec;
  current_path(p, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot set current path", ec));
}

void
fs::current_path(const path& p, error_code& ec) noexcept
{
#ifdef _GLIBCXX_HAVE_UNISTD_H
  if (int err = ::chdir(p.c_str()))
    ec.assign(err, std::generic_category());
  else
    ec.clear();
#else
  ec = std::make_error_code(std::errc::not_supported);
#endif
}

bool
fs::equivalent(const path& p1, const path& p2)
{
  error_code ec;
  auto result = equivalent(p1, p2, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot check file equivalence",
	  p1, p2, ec));
  return result;
}

#ifdef _GLIBCXX_HAVE_SYS_STAT_H
namespace
{
  fs::file_status
  make_file_status(struct ::stat& st)
  {
    using fs::file_status;
    using fs::file_type;
    using fs::perms;
    file_type ft;
    perms perm = static_cast<perms>(st.st_mode) & perms::mask;
#ifdef _GLIBCXX_HAVE_S_ISREG
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
#endif
      ft = file_type::unknown;
    return file_status{ft, perm};
  }

  inline bool
  is_not_found_errno(int err)
  {
    return err == ENOENT || err == ENOTDIR;
  }
}

bool
fs::equivalent(const path& p1, const path& p2, error_code& ec) noexcept
{
  struct ::stat st1, st2;
  int err1 = ::stat(p1.c_str(), &st1);
  int err2 = ::stat(p2.c_str(), &st2);
  if (!err1 && !err2)
    {
      file_status s1 = make_file_status(st1);
      file_status s2 = make_file_status(st2);
      if (is_other(s1) && is_other(s2))
	{
	  ec = std::make_error_code(std::errc::not_supported);
	  return false;
	}
      ec.clear();
      return st1.st_dev == st2.st_dev && st1.st_ino == st2.st_ino;
    }
  if (is_not_found_errno(err1) && is_not_found_errno(err2))
    {
      ec = std::make_error_code(std::errc::no_such_file_or_directory);
      return false;
    }
  int err = err1 ? err1 : err2;
  ec.assign(err, std::generic_category());
  return false;
}
#endif

std::uintmax_t
fs::file_size(const path& p)
{
  error_code ec;
  auto sz = file_size(p, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot get file size", p, ec));
  return sz;
}

namespace
{
  template<typename Accessor, typename T>
    T
    do_stat(const fs::path& p, std::error_code& ec, Accessor f, T deflt)
    {
#ifdef _GLIBCXX_HAVE_SYS_STAT_H
      struct ::stat st;
      if (int err = ::stat(p.c_str(), &st))
	{
	  ec.assign(err, std::generic_category());
	  return deflt;
	}
      ec.clear();
      return f(st);
#else
      ec = std::make_error_code(std::errc::not_supported);
      return deflt;
#endif
    }
}

std::uintmax_t
fs::file_size(const path& p, error_code& ec) noexcept
{
  return do_stat(p, ec, std::mem_fn(&stat::st_size),
		 static_cast<uintmax_t>(-1));
}

std::uintmax_t
fs::hard_link_count(const path& p)
{
  error_code ec;
  auto count = hard_link_count(p, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot get link count", p, ec));
  return count;
}

std::uintmax_t
fs::hard_link_count(const path& p, error_code& ec) noexcept
{
  return do_stat(p, ec, std::mem_fn(&stat::st_nlink),
		 static_cast<uintmax_t>(-1));
}

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
  auto s = status(p, ec);
  if (ec.value())
    return false;
  return fs::is_directory(s)
    ? fs::directory_iterator(p, ec) == fs::directory_iterator()
    : fs::file_size(p, ec) == 0;
}

fs::file_time_type
fs::last_write_time(const path& p)
{
  error_code ec;
  auto t = last_write_time(p, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot get file time", p, ec));
  return t;
}

fs::file_time_type
fs::last_write_time(const path& p, error_code& ec) noexcept
{
  using namespace std::chrono;
  return do_stat(p, ec, [](const auto& st) {
      return file_time_type{ seconds(st.st_mtime) };
      }, file_time_type::min());
}

void
fs::last_write_time(const path& p, file_time_type new_time)
{
  error_code ec;
  last_write_time(p, new_time, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot set file time", p, ec));
}

void
fs::last_write_time(const path& p, file_time_type new_time,
		    error_code& ec) noexcept
{
  auto d = new_time.time_since_epoch();
  auto s = chrono::duration_cast<chrono::seconds>(d);
  auto ns = chrono::duration_cast<chrono::nanoseconds>(d - s);
#ifdef _GLIBCXX_USE_UTIMENSAT
  struct ::timespec ts[2] = {
    { 0, UTIME_OMIT },
    { static_cast<std::time_t>(s.count()), static_cast<long>(ns.count()) }
  };
  if (utimensat(AT_FDCWD, p.c_str(), ts, 0))
    ec.assign(errno, std::generic_category());
  else
    ec.clear();
#else
  ec = std::make_error_code(std::errc::not_supported);
#endif
}

void
fs::permissions(const path& p, perms prms)
{
  error_code ec;
  permissions(p, prms, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot set permissions", p, ec));
}

void fs::permissions(const path& p, perms prms, error_code& ec) noexcept
{
  if (int err = ::fchmodat(AT_FDCWD, p.c_str(), static_cast<mode_t>(prms), 0))
    ec.assign(err, std::generic_category());
  else
    ec.clear();
}

fs::path
fs::read_symlink(const path& p)
{
  error_code ec;
  path tgt = read_symlink(p, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("read_symlink", p, ec));
  return tgt;
}

fs::path fs::read_symlink(const path& p, error_code& ec)
{
#ifdef _GLIBCXX_HAVE_SYS_STAT_H
  struct ::stat st;
  if (int err = ::lstat(p.c_str(), &st))
    {
      ec.assign(err, std::generic_category());
      return {};
    }
  std::string buf(st.st_size, '\0');
  ssize_t len = ::readlink(p.c_str(), &buf.front(), buf.size());
  if (len == -1)
    {
      ec.assign(errno, std::generic_category());
      return {};
    }
  return path{buf.data(), buf.data()+len};
#else
  ec = std::make_error_code(std::errc::not_supported);
  return {};
#endif
}


bool
fs::remove(const path& p)
{
  error_code ec;
  bool result = fs::remove(p, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot remove", p, ec));
  return result;
}

bool
fs::remove(const path& p, error_code& ec) noexcept
{
  if (exists(symlink_status(p, ec)))
    {
      if (::remove(p.c_str()) == 0)
	{
	  ec.clear();
	  return true;
	}
      else
	ec.assign(errno, std::generic_category());
    }
  return false;
}


std::uintmax_t
fs::remove_all(const path& p)
{
  error_code ec;
  bool result = remove_all(p, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot remove all", p, ec));
  return result;
}

std::uintmax_t
fs::remove_all(const path& p, error_code& ec) noexcept
{
  auto fs = symlink_status(p, ec);
  uintmax_t count = 0;
  if (ec.value() == 0 && fs.type() == file_type::directory)
    for (directory_iterator d(p, ec), end; ec.value() == 0 && d != end; ++d)
      count += fs::remove(d->path(), ec);
  if (ec.value())
    return -1;
  return fs::remove(p, ec) ? ++count : -1;  // fs:remove() calls ec.clear()
}

void
fs::rename(const path& from, const path& to)
{
  error_code ec;
  rename(from, to, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot rename", from, to, ec));
}

void
fs::rename(const path& from, const path& to, error_code& ec) noexcept
{
  if (::rename(from.c_str(), to.c_str()))
    ec.assign(errno, std::generic_category());
  else
    ec.clear();
}

void
fs::resize_file(const path& p, uintmax_t size)
{
  error_code ec;
  resize_file(p, size, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot resize file", p, ec));
}

void
fs::resize_file(const path& p, uintmax_t size, error_code& ec) noexcept
{
#ifdef _GLIBCXX_HAVE_UNISTD_H
  if (size > static_cast<uintmax_t>(std::numeric_limits<off_t>::max()))
    ec.assign(EINVAL, std::generic_category());
  else if (::truncate(p.c_str(), size))
    ec.assign(errno, std::generic_category());
  else
    ec.clear();
#else
  ec = std::make_error_code(std::errc::not_supported);
#endif
}


fs::space_info
fs::space(const path& p)
{
  error_code ec;
  space_info s = space(p, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("cannot get free space", p, ec));
  return s;
}

fs::space_info
fs::space(const path& p, error_code& ec) noexcept
{
  space_info info = {
    static_cast<uintmax_t>(-1),
    static_cast<uintmax_t>(-1),
    static_cast<uintmax_t>(-1)
  };
#ifdef _GLIBCXX_HAVE_SYS_STATVFS_H
  struct ::statvfs f;
  if (int err = ::statvfs(p.c_str(), &f))
      ec.assign(err, std::generic_category());
  else
    {
      info = space_info{
	f.f_blocks * f.f_frsize,
	f.f_bfree * f.f_frsize,
	f.f_bavail * f.f_frsize
      };
      ec.clear();
    }
#endif
  return info;
}

#ifdef _GLIBCXX_HAVE_SYS_STAT_H
fs::file_status
fs::status(const fs::path& p, std::error_code& ec) noexcept
{
  file_status status;
  struct ::stat st;
  if (int err = ::stat(p.c_str(), &st))
    {
      ec.assign(err, std::generic_category());
      if (is_not_found_errno(err))
	status = file_status{file_type::not_found};
    }
  else
    {
      status = make_file_status(st);
      ec.clear();
    }
  return status;
}

fs::file_status
fs::symlink_status(const fs::path& p, std::error_code& ec) noexcept
{
  file_status status;
  struct ::stat st;
  if (int err = ::lstat(p.c_str(), &st))
    {
      ec.assign(err, std::generic_category());
      if (is_not_found_errno(err))
	status = file_status{file_type::not_found};
    }
  else
    {
      status = make_file_status(st);
      ec.clear();
    }
  return status;
}
#endif

fs::file_status
fs::status(const fs::path& p)
{
  std::error_code ec;
  auto s = status(p, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("status", p, ec));
  return s;
}

fs::file_status
fs::symlink_status(const fs::path& p)
{
  std::error_code ec;
  auto s = symlink_status(p, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("symlink_status", ec));
  return s;
}

fs::path
fs::system_complete(const path& p)
{
  error_code ec;
  path comp = system_complete(p, ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("system_complete", p, ec));
  return comp;
}

fs::path
fs::system_complete(const path& p, error_code& ec)
{
  path base = current_path(ec);
#ifdef _GLIBCXX_FILESYSTEM_IS_WINDOWS
  if (p.is_absolute() || !p.has_root_name()
      || p.root_name() == base.root_name())
    return absolute(p, base);
  // else TODO
  ec = std::make_error_code(std::errc::not_supported);
  return {};
#else
  if (ec.value())
    return {};
  return absolute(p, base);
#endif
}

fs::path fs::temp_directory_path()
{
  error_code ec;
  path tmp = temp_directory_path(ec);
  if (ec.value())
    _GLIBCXX_THROW_OR_ABORT(filesystem_error("temp_directory_path", ec));
  return tmp;
}

fs::path fs::temp_directory_path(error_code& ec)
{
#ifdef _GLIBCXX_FILESYSTEM_IS_WINDOWS
  return {}; // TODO
#else
  const char* tmpdir = ::getenv("TMPDIR");
  if (!tmpdir)
    tmpdir = "/tmp";
  ec.clear();
  return tmpdir;
#endif
}

