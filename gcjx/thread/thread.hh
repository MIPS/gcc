// Thread definitions.

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

#ifndef GCJX_THREAD_THREAD_HH
#define GCJX_THREAD_THREAD_HH

/// Note that this is just stubs.  The names come from Benjamin
/// Kosnik's libthread++, which isn't checked in anywhere yet.
/// Meanwhile this implements exactly the subset used by gcjx.
namespace concurrence
{
  // Mutex base class.
  class sync
  {
  public:

    // Acquire and release a mutex using RAII.
    class lock_sentinel
    {
    public:

      lock_sentinel (const sync &)
      {
      }
    };
  };

  class exclusive_mutex : public sync
  {
  };

  // Condition variable.
  class exclusive_condition
  {
  public:

    exclusive_condition (const exclusive_mutex &)
    {
    }

    void wait ()
    {
    }

    void signal ()
    {
    }
  };

  // Benjamin hasn't accepted this one yet, but we need it.  The one I
  // sent him uses operators instead of methods.  This is only used in
  // one place in gcjx and is easy to change around.
  template<typename T>
  class thread_local
  {
    T value;

  public:

    void set (const T& v)
    {
      value = v;
    }

    T get ()
    {
      return value;
    }
  };

  class thread
  {
  public:

    void start ()
    {
    }
  };

  template<typename T>
  thread make_thread (const T &obj, void (T::*fn) ())
  {
    abort ();
  }
};

#endif // GCJX_THREAD_THREAD_HH
