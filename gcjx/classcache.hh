// Look up and cache classes.

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

#ifndef GCJX_CLASSCACHE_HH
#define GCJX_CLASSCACHE_HH

class class_cache
{
  // Name of the class to look up.
  const char *name;

  // The cached class.
  model_class *cache;

  /// The mutex on which to synchronize.
  concurrence::sync &mutex;

  // This is here to make it impossible for user code to try to use
  // this class as anything other than a cache.
  class_cache &operator= (const class_cache &)
  {
    abort ();
  }

  void find_class ();

public:

  class_cache (concurrence::sync &m, const char *n)
    : name (n),
      cache (NULL),
      mutex (m)
  {
  }

  // Users use a function-like notation to access the cache value.
  model_class *operator() ()
  {
    if (cache == NULL)
      find_class ();
    return cache;
  }
};

#endif // GCJX_CLASSCACHE_HH
