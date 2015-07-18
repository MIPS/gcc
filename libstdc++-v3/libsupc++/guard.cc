// Copyright (C) 2002 Free Software Foundation, Inc.
//  
// This file is part of GCC.
//
// GCC is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.

// GCC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with GCC; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA. 

// As a special exception, you may use this file as part of a free software
// library without restriction.  Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License.  This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.

// Written by Mark Mitchell, CodeSourcery LLC, <mark@codesourcery.com>
// Thread support written by Jason Merrill, Red Hat Inc. <jason@redhat.com>

#include <cxxabi.h>
#include <exception>
#include <bits/c++config.h>
#include <bits/gthr.h>
#include <bits/atomicity.h>
#if defined(__GTHREAD_HAS_COND) && !defined(__GTHREAD_COND_INIT)
#undef __GTHREAD_HAS_COND
#endif

// The IA64/generic ABI uses the first byte of the guard variable.
// The ARM EABI uses the least significant bit.

// Thread-safe static local initialization support.
#ifdef __GTHREADS
namespace
{
  // static_mutex is a single mutex controlling all static initializations.
  // This is a static class--the need for a static initialization function
  // to pass to __gthread_once precludes creating multiple instances, though
  // I suppose you could achieve the same effect with a template.
  struct static_mutex
  {
    static __gthread_recursive_mutex_t mutex;

#ifdef __GTHREAD_RECURSIVE_MUTEX_INIT_FUNCTION
    static void init();
#endif

  public:
    static void lock();
    static void unlock();
  };

  __gthread_recursive_mutex_t static_mutex::mutex
#ifdef __GTHREAD_RECURSIVE_MUTEX_INIT
  = __GTHREAD_RECURSIVE_MUTEX_INIT
#endif
  ;

#ifdef __GTHREAD_RECURSIVE_MUTEX_INIT_FUNCTION
  void static_mutex::init()
  {
    __GTHREAD_RECURSIVE_MUTEX_INIT_FUNCTION (&mutex);
  }
#endif

  void static_mutex::lock()
  {
#ifdef __GTHREAD_RECURSIVE_MUTEX_INIT_FUNCTION
    static __gthread_once_t once = __GTHREAD_ONCE_INIT;
    __gthread_once (&once, init);
#endif
    __gthread_recursive_mutex_lock (&mutex);
  }

  void static_mutex::unlock ()
  {
    __gthread_recursive_mutex_unlock (&mutex);
  }

  // Simple wrapper for exception safety.
  struct mutex_wrapper
  {
    bool unlock;
    mutex_wrapper() : unlock(true)
    { static_mutex::lock(); }

    ~mutex_wrapper()
    {
      if (unlock)
	static_mutex::unlock();
    }
  };
}

#ifdef __GTHREAD_HAS_COND
namespace
{
  // A single conditional variable controlling all static initializations.
  static __gthread_cond_t static_cond = __GTHREAD_COND_INIT;
}
#endif

#ifndef _GLIBCXX_GUARD_TEST_AND_ACQUIRE
inline bool
__test_and_acquire (__cxxabiv1::__guard *g)
{
  bool b = _GLIBCXX_GUARD_TEST (g);
  _GLIBCXX_READ_MEM_BARRIER;
  return b;
}
#define _GLIBCXX_GUARD_TEST_AND_ACQUIRE(G) __test_and_acquire (G)
#endif

#ifndef _GLIBCXX_GUARD_SET_AND_RELEASE
inline void
__set_and_release (__cxxabiv1::__guard *g)
{
  _GLIBCXX_WRITE_MEM_BARRIER;
  _GLIBCXX_GUARD_SET (g);
}
#define _GLIBCXX_GUARD_SET_AND_RELEASE(G) __set_and_release (G)
#endif

#else /* !__GTHREADS */

#undef _GLIBCXX_GUARD_TEST_AND_ACQUIRE
#undef _GLIBCXX_GUARD_SET_AND_RELEASE
#define _GLIBCXX_GUARD_SET_AND_RELEASE(G) _GLIBCXX_GUARD_SET (G)

#endif /* __GTHREADS */

namespace __gnu_cxx
{
  // 6.7[stmt.dcl]/4: If control re-enters the declaration (recursively)
  // while the object is being initialized, the behavior is undefined.

  // Since we already have a library function to handle locking, we might
  // as well check for this situation and throw an exception.
  // We use the second byte of the guard variable to remember that we're
  // in the middle of an initialization.
  class recursive_init: public std::exception
  {
  public:
    recursive_init() throw() { }
    virtual ~recursive_init() throw ();
  };

  recursive_init::~recursive_init() throw() { }
}

//
// Here are C++ run-time routines for guarded initiailization of static
// variables. There are 4 scenarios under which these routines are called:
//
//   1. Threads not supported (__GTHREADS not defined)
//   2. Threads are supported but not enabled at run-time.
//   3. Threads enabled at run-time but __gthreads_* are not fully POSIX.
//   4. Threads enabled at run-time and __gthreads_* support all POSIX threads
//      primitives we need here.
//
// The old code supported scenarios 1-3 but was broken since it used a global
// mutex for all threads and had the mutex locked during the whole duration of
// initlization of a guarded static variable. The following created a dead-lock
// with the old code.
//
//	Thread 1 acquires the global mutex.
//	Thread 1 starts initializing static variable.
//	Thread 1 creates thread 2 during initialization.
//	Thread 2 attempts to acuqire mutex to initialize another variable.
//	Thread 2 blocks since thread 1 is locking the mutex.
//	Thread 1 waits for result from thread 2 and also blocks. A deadlock.
//
// The new code here can handle this situation and thus is more robust. Howere,
// we need to use the POSIX thread conditional variable, which is not supported
// in all platforms, notably older versions of Microsoft Windows. The gthr*.h
// headers define a symbol __GTHREAD_HAS_COND for platforms that support POSIX
// like conditional variables. For platforms that do not support conditional
// variables, we need to fall back to the old code.
namespace __cxxabiv1 
{
  static inline int
  init_in_progress_flag(__guard* g)
  { return ((char *)g)[1]; }

  static inline void
  set_init_in_progress_flag(__guard* g, int v)
  { ((char *)g)[1] = v; }

  static int
  acquire_1 (__guard *g)
  {
    if (_GLIBCXX_GUARD_TEST (g))
      return 0;

    if (init_in_progress_flag (g))
      {
#ifdef __EXCEPTIONS
	throw __gnu_cxx::recursive_init();
#else
	// Use __builtin_trap so we don't require abort().
	__builtin_trap ();
#endif
      }
    set_init_in_progress_flag(g, 1);
    return 1;
  }

  extern "C"
  int __cxa_guard_acquire (__guard *g) 
  {
#ifdef __GTHREADS
    // If the target can reorder loads, we need to insert a read memory
    // barrier so that accesses to the guarded variable happen after the
    // guard test.
    if (_GLIBCXX_GUARD_TEST_AND_ACQUIRE (g))
      return 0;

    if (__gthread_active_p ())
      {
	mutex_wrapper mw;

	while (1)	// When this loop is executing, mutex is locked.
  	  {
#ifdef __GTHREAD_HAS_COND
	    // The static is allready initialized.
	    if (_GLIBCXX_GUARD_TEST(g))
	      return 0;	// The mutex will be unlocked via wrapper

	    if (init_in_progress_flag(g))
	      {
		// The guarded static is currently being initialized by
		// another thread, so we release mutex and wait for the
		// conditional variable. We will lock the mutex again after
		// this.
		__gthread_cond_wait_recursive(&static_cond,
					      &static_mutex::mutex);
	      }
	    else
	      {
		set_init_in_progress_flag(g, 1);
		return 1; // The mutex will be unlocked via wrapper.
	      }
#else
	    // This provides compatibility with older systems not supporting
	    // POSIX like conditional variables.
	    if (acquire_1 (g))
	      {
		mw.unlock = false;
		return 1; // The mutex still locked.
	      }
	    return 0; // The mutex will be unlocked via wrapper.
#endif
	  }
      }
#endif

    return acquire_1 (g);
  }

  extern "C"
  void __cxa_guard_abort (__guard *g)
  {
#ifdef __GTHREAD_HAS_COND
    if (__gthread_active_p())
      {	
	mutex_wrapper mw;

	set_init_in_progress_flag(g, 0);

	// If we abort, we still need to wake up all other threads waiting for
	// the conditional variable.
	__gthread_cond_broadcast(&static_cond);
	return;
      }	
#endif

    set_init_in_progress_flag(g, 0);
#if defined(__GTHREADS) && !defined(__GTHREAD_HAS_COND)
    // This provides compatibility with older systems not supporting POSIX like
    // conditional variables.
    if (__gthread_active_p ())
      static_mutex::unlock ();
#endif
  }

  extern "C"
  void __cxa_guard_release (__guard *g)
  {
#ifdef __GTHREAD_HAS_COND
    if (__gthread_active_p())
      {
	mutex_wrapper mw;

	set_init_in_progress_flag(g, 0);
	_GLIBCXX_GUARD_SET_AND_RELEASE(g);

	__gthread_cond_broadcast(&static_cond);
	return;
      }      
#endif       

    set_init_in_progress_flag(g, 0);
    _GLIBCXX_GUARD_SET_AND_RELEASE (g);

#if defined(__GTHREADS) && !defined(__GTHREAD_HAS_COND)
    // This provides compatibility with older systems not supporting POSIX like
    // conditional variables.
    if (__gthread_active_p())
      static_mutex::unlock();
#endif
  }
}
