// Directory cache.

// Copyright (C) 2004 Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// gcjx is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// gcjx is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with gcjx; see the file COPYING.LIB.  If
// not, write to the Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#ifndef GCJX_DIRECTORY_HH
#define GCJX_DIRECTORY_HH

/// This keeps track of which directories we know to exist, so we can
/// avoid excessive stat() and mkdir() calls.
class directory_cache
{
  /// The base directory.
  std::string base;

  /// The directory cache.  A name appears in this cache if we know
  /// that the directory exists, either due to a stat() or a mkdir().
  std::set<std::string> cache;

  /// Monitor we use to synchronize in multi-threaded environment.
  concurrence::exclusive_mutex monitor;

  /// Internal method to ensure that a directory exists.
  void ensure (const std::string &);

public:

  directory_cache ()
  {
  }

  /// Set the base directory.  This can only be done before 'add' is
  /// used, and can only be called once.
  void set_directory (const std::string &);

  /// Given a class, return the name of the output file for the class.
  /// The result is a full path name, and all the required directories
  /// will have been created.  The string is the suffix that will be
  /// appended to the file name.
  std::string add (model_class *, const std::string &);
};

#endif // GCJX_DIRECTORY_HH
