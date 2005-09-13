// Thread definitions.

// Copyright (C) 2004, 2005 Free Software Foundation, Inc.
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

// #include <pthread.h>

/// Note that this is just stubs.  The names come from Benjamin
/// Kosnik's libthread++, which isn't checked in anywhere yet.
/// Meanwhile this implements exactly the subset used by gcjx.
namespace concurrence
{
  class exclusive_condition;

  class exclusive_mutex
  {
//     pthread_mutex_t lock;

    friend class exclusive_condition;

  public:

    exclusive_mutex ()
    {
      // FIXME: we must use a recursive mutex here due to uses in
      // compiler, but last time I looked (quite a long time ago),
      // recursive mutexes did not work properly with condition
      // variables.
//       pthread_mutex_init (&lock, NULL);
    }

    ~exclusive_mutex ()
    {
//       pthread_mutex_destroy (&lock);
    }

    // Acquire and release a mutex using RAII.
    class lock_sentinel
    {
      exclusive_mutex &m;

    public:

      lock_sentinel (exclusive_mutex &mu)
	: m (mu)
      {
// 	pthread_mutex_lock (&m.lock);
      }

      ~lock_sentinel ()
      {
// 	pthread_mutex_unlock (&m.lock);
      }
    };
  };

  // Condition variable.
  class exclusive_condition
  {
    exclusive_mutex &m;

//     pthread_cond_t cond;

  public:

    exclusive_condition (exclusive_mutex &mu)
      : m (mu)
    {
//       pthread_cond_init (&cond, NULL);
    }

    ~exclusive_condition ()
    {
//       pthread_cond_destroy (&cond);
    }

    void wait ()
    {
//       pthread_cond_wait (&cond, &m.lock);
    }

    void signal ()
    {
//       pthread_cond_signal (&cond);
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

  template<typename T>
  class thread
  {
    T *obj;

    void (T::*fn) ();

    pthread_t thr;

    static void *do_start (void *val)
    {
      thread *self = reinterpret_cast<thread *> (val);
      ((self->obj)->*(self->fn)) ();
      return NULL;
    }

  public:

    thread (T *o, void (T::*f) ())
      : obj (o),
	fn (f)
    {
    }

    void start ()
    {
      abort ();
//       pthread_create (&thr, NULL, do_start, this);
    }
  };

  template<typename T>
  thread<T> make_thread (T *obj, void (T::*fn) ())
  {
    return thread<T> (obj, fn);
  }

  /// Return true if threads are available.
  inline bool available ()
  {
    return false;
  }
};

#endif // GCJX_THREAD_THREAD_HH
