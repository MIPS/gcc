// natThreadLocal.cc - Native part of ThreadLocal class.

// Fast thread local storage for systems that support the __thread
// variable attribute.
   
/* Copyright (C) 2006  Free Software Foundation

   This file is part of libgcj.

This software is copyrighted work licensed under the terms of the
Libgcj License.  Please consult the file "LIBGCJ_LICENSE" for
details.  */

#include <config.h>

#include <stdlib.h>

#include <gcj/cni.h>
#include <jvm.h>
#include <java-threads.h>

#include <gnu/gcj/RawDataManaged.h>
#include <java/lang/ThreadLocal.h>
#include <java/lang/IllegalArgumentException.h>
#include <java/lang/IllegalThreadStateException.h>
#include <java/lang/InterruptedException.h>
#include <java/util/Map.h>

#include <jni.h>

/* We would like to have fast thread local variables that behave in
   the same way as C and C++ thread local variables.  This would mean
   having an field attribute "thread" (like static, final, etc.).
   However, this is not compatible with java semantics, which we wish
   to support transparently.  The problems we must overcome are:

   * In Java, ThreadLocal variables are not statically allocated: they
     are objects, created at runtime.

   * Class ThreadLocal is not final and neither are its methods, so it
     is possible to create a subclass of ThreadLocal that overrides
     any method.

   * __thread variables in DSOs are not visible to the garbage
     collector, so we must ensure that we keep a copy of every thread
     local variable somewhere on the heap.

   * Once a ThreadLocal instance has been created and assigned to a
     static field, that field may be reassigned to a different
     ThreadLocal instance or null.  

   So, we can't simply replace get() and set() with accesses of a
   __thread variable.

   This is how our soloution works:
     
   Whenever we create a static field of Class ThreadLocal, we also
   create a variable with __thread attribute, but it is only used as a
   cache of a ThreadLocal entry.  Whenever we set() a static
   ThreadLocal, we must update a map of ThreadLocals and the __thread
   variable we're using to cache the entry.  However, when we get() a
   static ThreadLocal, we only need to read the cache.

   
   In detail:

   When gcj allocates a static field that is an instance of
   ThreadLocal, it also allocates a thread-local cache variable.  This
   is is a static structure of type tls_t:

      typedef struct
      {
	jlong id;
	void *val;
      } tls_t;

   When gcj generates a store that sets a ThreadLocal static field, it
   also generates a call to setTLS(ptr).  ptr is a pointer to a
   function that returns the address of the cache variable in thread
   local memory.  ThreadLocal.setTLS(ptr) saves that function pointer
   and also generates a unique ID that will be used to identify this
   instance of ThreadLocal.  So, the ThreadLocal instance is now
   "tied" to that cache variable.

   The first time that ThreadLocal.set() is called in a particular
   thread, we check the ID in the cache variable to see if it is the
   same as the ID of this ThreadLocal instance.  We see that it is
   not, because all __thread variables are initialized to zero, so we
   write the ID of this ThreadLocal and the value into the cache
   variable.  We also update the map of ThreadLocals on the heap; we
   must do this for the benefit of the garbage collector.

   When get() is called, we look in the cache variable, check the ID,
   see that it is the same as the ID of this ThreadLocal instance, and
   return the value immediately.

   Whenever we need to invalidate the cache, we can do so by setting
   ID to 0.

   This is safe even in degenerate cases such as assigning the same
   ThreadLocal instance to multiple static fields or repeatedly
   re-assigning a static field in multiple threads.  If the cached
   value doesn't belong to this ThreadLocal, we simply ignore it.

   This scheme is biased towards efficiency when get() is called much
   more frequently than set().  It is slightly slower than the
   all-Java solution using the underlying map in the set() case.
   However, get() is very much more frequently invoked than set().

*/


typedef struct
{
  jlong id;
  void *val;
} tls_t;

static inline tls_t*
get_tls_addr (void *p)
{
  typedef tls_t* func (void);
  func *fptr = (func*)p;

  return fptr ();
}

void 
java::lang::ThreadLocal::set (::java::lang::Object *value)
{
  if (TLSPointer != NULL)
    {
      tls_t* tls_var_p = get_tls_addr (TLSPointer);
      tls_var_p->val = value;
      tls_var_p->id = id;
    }

  slowSet (value);
}

::java::lang::Object *
java::lang::ThreadLocal::get (void)
{
  if (TLSPointer == NULL)
    return slowGet ();

  tls_t* tls_var_p = get_tls_addr (TLSPointer);

  if (tls_var_p->id == this->id)
    return (::java::lang::Object *)tls_var_p->val;

  ::java::lang::Object *value = slowGet ();
  tls_var_p->val = value;
  tls_var_p->id = this->id;

  return value;
}

void 
java::lang::ThreadLocal::remove (void)
{
  if (TLSPointer != NULL)
    {
      tls_t* tls_var_p = get_tls_addr (TLSPointer);
      tls_var_p->val = NULL;
      tls_var_p->id = 0;
    }

  slowRemove ();
}

