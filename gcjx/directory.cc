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

#include "typedefs.hh"
#include "directory.hh"

#include <sys/stat.h>
#include <errno.h>

void
directory_cache::ensure (const std::string &dirname)
{
  if (cache.find (dirname) != cache.end ())
    return;
  struct stat sb;
  // FIXME: check to see if we didn't get a directory?
  if (stat (dirname.c_str (), &sb) == -1
      && mkdir (dirname.c_str (), 0755) == -1
      // Note that some other process or thread might have made the
      // directory in the window between the stat and the mkdir.
      && errno != EEXIST)
    abort ();			// fixme throw
  cache.insert (dirname);
}

std::string
directory_cache::add (model_class *klass, const std::string &suffix)
{
  concurrence::sync::lock_sentinel sync (monitor);
  std::list<std::string> qualname = split (klass->get_fully_qualified_name (),
					   '.');
  std::string partial = base;
  std::list<std::string>::const_iterator i = qualname.begin ();
  while (true)
    {
      partial += FILE_SEPARATOR + *i;
      ++i;
      if (i == qualname.end ())
	break;
      ensure (partial);
    }
  return partial + suffix;
}

void
directory_cache::set_directory (const std::string &n)
{
  assert (base.empty ());
  assert (cache.empty ());
  base = n;
}
