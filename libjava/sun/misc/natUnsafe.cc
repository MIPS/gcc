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
  return;
}

void
sun::misc::Unsafe::park (jboolean isAbsolute, jlong time)
{
//   ::java::lang::Thread::yield ();
}
