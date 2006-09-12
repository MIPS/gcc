#include <gcj/cni.h>
#include <gcj/field.h>
#include <gcj/javaprims.h>
#include <jvm.h>
#include <sun/misc/Unsafe.h>
#include <java/lang/System.h>
#include <java/lang/InterruptedException.h>

#include <java/lang/Thread.h>
#include <java/lang/Long.h>

jlong
sun::misc::Unsafe::objectFieldOffset (::java::lang::reflect::Field *field)
{
  _Jv_Field *fld = _Jv_FromReflectedField (field);
  // FIXME: what if it is not an instance field?
  return fld->getOffset();
}

jboolean
sun::misc::Unsafe::compareAndSwapInt (jobject obj, jlong offset,
				      jint expect, jint update)
{
  jint *addr = (jint *)((char *)obj + offset);
  return __sync_bool_compare_and_swap (addr, expect, update);
}

jint
sun::misc::Unsafe::arrayBaseOffset (jclass arrayClass)
{
  // FIXME: assert that arrayClass is array.
  jclass eltClass = arrayClass->getComponentType();
  return (jint)(jlong) _Jv_GetArrayElementFromElementType (NULL, eltClass);
}

jint
sun::misc::Unsafe::arrayIndexScale (jclass arrayClass)
{
  // FIXME: assert that arrayClass is array.
  jclass eltClass = arrayClass->getComponentType();
  if (eltClass->isPrimitive())
    return eltClass->size();
  return sizeof (void *);
}

jboolean
sun::misc::Unsafe::compareAndSwapLong (jobject obj, jlong offset,
				       jlong expect, jlong update)
{
  jlong *addr = (jlong*)((char *) obj + offset);
  return __sync_bool_compare_and_swap (addr, expect, update);
}

jboolean
sun::misc::Unsafe::compareAndSwapObject (jobject obj, jlong offset,
					 jobject expect, jobject update)
{
  jobject *addr = (jobject*)((char *) obj + offset);
  return __sync_bool_compare_and_swap (addr, expect, update);
}

void
sun::misc::Unsafe::putOrderedInt (jobject obj, jlong offset, jint value)
{
  jint *addr = (jint *) ((char *) obj + offset);
  *addr = value;
}

void
sun::misc::Unsafe::putOrderedLong (jobject obj, jlong offset, jlong value)
{
  jlong *addr = (jlong *) ((char *) obj + offset);
  *addr = value;
}

void
sun::misc::Unsafe::putOrderedObject (jobject obj, jlong offset, jobject value)
{
  jobject *addr = (jobject *) ((char *) obj + offset);
  *addr = value;
}

void
sun::misc::Unsafe::putIntVolatile (jobject obj, jlong offset, jint value)
{
  jint *addr = (jint *) ((char *) obj + offset);
  *addr = value;
}

void
sun::misc::Unsafe::putLongVolatile (jobject obj, jlong offset, jlong value)
{
  jlong *addr = (jlong *) ((char *) obj + offset);
  *addr = value;
}

void
sun::misc::Unsafe::putObjectVolatile (jobject obj, jlong offset, jobject value)
{
  jobject *addr = (jobject *) ((char *) obj + offset);
  *addr = value;
}

#if 0  // FIXME
void
sun::misc::Unsafe::putInt (jobject obj, jlong offset, jint value)
{
  jint *addr = (jint *) ((char *) obj + offset);
  *addr = value;
}
#endif

void
sun::misc::Unsafe::putLong (jobject obj, jlong offset, jlong value)
{
  jlong *addr = (jlong *) ((char *) obj + offset);
  *addr = value;
}

void
sun::misc::Unsafe::putObject (jobject obj, jlong offset, jobject value)
{
  jobject *addr = (jobject *) ((char *) obj + offset);
  *addr = value;
}

jint
sun::misc::Unsafe::getIntVolatile (jobject obj, jlong offset)
{
  jint *addr = (jint *) ((char *) obj + offset);
  return *addr;
}

jobject
sun::misc::Unsafe::getObjectVolatile (jobject obj, jlong offset)
{
  jobject *addr = (jobject *) ((char *) obj + offset);
  return *addr;
}

jlong
sun::misc::Unsafe::getLong (jobject obj, jlong offset)
{
  jlong *addr = (jlong *) ((char *) obj + offset);
  return *addr;
}

jlong
sun::misc::Unsafe::getLongVolatile (jobject obj, jlong offset)
{
  jlong *addr = (jlong *) ((char *) obj + offset);
  return *addr;
}

void
sun::misc::Unsafe::unpark (::java::lang::Thread *thread)
{
  using namespace ::java::lang;
  volatile jbyte *ptr = &thread->parkPermit;

  /* If this thread is in state RUNNING, give it a permit and return
     immediately.  */
  if (__sync_bool_compare_and_swap 
      (ptr, Thread::THREAD_PARK_RUNNING, Thread::THREAD_PARK_PERMIT))
    return;
  
  /* If this thread is parked, put it into state RUNNING and send it a
     signal.  */
  if (__sync_bool_compare_and_swap 
      (ptr, Thread::THREAD_PARK_PARKED, Thread::THREAD_PARK_RUNNING))
    {
      natThread *nt = (natThread *) thread->data;
      pthread_mutex_lock (&nt->park_mutex);
      pthread_cond_signal (&nt->park_cond);
      pthread_mutex_unlock (&nt->park_mutex);
    }
}

void
sun::misc::Unsafe::park (jboolean isAbsolute, jlong time)
{
  using namespace ::java::lang;
  Thread *thread = Thread::currentThread();
  volatile jbyte *ptr = &thread->parkPermit;

  /* If we have a permit, return immediately.  */
  if (__sync_bool_compare_and_swap 
      (ptr, Thread::THREAD_PARK_PERMIT, Thread::THREAD_PARK_RUNNING))
    return;

  struct timespec ts;
  jlong millis = 0, nanos = 0;

  if (time)
    {
      if (isAbsolute)
	{
	  millis = time;
	  nanos = 0;
	}
      else
	{
	  millis = java::lang::System::currentTimeMillis();
	  nanos = time;
	}

      if (millis > 0 || nanos > 0)
	{
	  // Calculate the abstime corresponding to the timeout.
	  // Everything is in milliseconds.
	  //
	  // We use `unsigned long long' rather than jlong because our
	  // caller may pass up to Long.MAX_VALUE millis.  This would
	  // overflow the range of a timespec.

	  unsigned long long m = (unsigned long long)millis;
	  unsigned long long seconds = m / 1000; 

	  ts.tv_sec = seconds;
	  if (ts.tv_sec < 0 || (unsigned long long)ts.tv_sec != seconds)
	    {
	      // We treat a timeout that won't fit into a struct timespec
	      // as a wait forever.
	      millis = nanos = 0;
	    }
	  else
	    {
	      m %= 1000;
	      ts.tv_nsec = m * 1000000 + (unsigned long long)nanos;
	    }
	}
    }
      
  natThread *nt = (natThread *) thread->data;
  pthread_mutex_lock (&nt->park_mutex);
  if (__sync_bool_compare_and_swap 
      (ptr, Thread::THREAD_PARK_RUNNING, Thread::THREAD_PARK_PARKED))
    {
      if (millis == 0 && nanos == 0)
	pthread_cond_wait (&nt->park_cond, &nt->park_mutex);
      else
	pthread_cond_timedwait (&nt->park_cond, &nt->park_mutex, 
					&ts);
      /* If we were unparked by some other thread, this will already
	 be in state THREAD_PARK_RUNNING.  If we timed out, we have to
	 do it ourself.  */
      __sync_bool_compare_and_swap 
	(ptr, Thread::THREAD_PARK_PARKED, Thread::THREAD_PARK_RUNNING);
    }
  pthread_mutex_unlock (&nt->park_mutex);
}
