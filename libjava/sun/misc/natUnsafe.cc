
#include <config.h>

#include <sun/misc/Unsafe.h>
#include <gcj/cni.h>
#include <java/lang/UnsupportedOperationException.h>

jlong
sun::misc::Unsafe::objectFieldOffset (::java::lang::reflect::Field *)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("sun::misc::Unsafe::objectFieldOffset (::java::lang::reflect::Field *) not implemented"));
}


jboolean
sun::misc::Unsafe::compareAndSwapInt (::java::lang::Object *, jlong, jint, jint)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("sun::misc::Unsafe::compareAndSwapInt (::java::lang::Object *, jlong, jint, jint) not implemented"));
}


jboolean
sun::misc::Unsafe::compareAndSwapLong (::java::lang::Object *, jlong, jlong, jlong)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("sun::misc::Unsafe::compareAndSwapLong (::java::lang::Object *, jlong, jlong, jlong) not implemented"));
}


jboolean
sun::misc::Unsafe::compareAndSwapObject (::java::lang::Object *, jlong, ::java::lang::Object *, ::java::lang::Object *)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("sun::misc::Unsafe::compareAndSwapObject (::java::lang::Object *, jlong, ::java::lang::Object *, ::java::lang::Object *) not implemented"));
}


void
sun::misc::Unsafe::putOrderedInt (::java::lang::Object *, jlong, jint)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("sun::misc::Unsafe::putOrderedInt (::java::lang::Object *, jlong, jint) not implemented"));
}


void
sun::misc::Unsafe::putOrderedLong (::java::lang::Object *, jlong, jlong)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("sun::misc::Unsafe::putOrderedLong (::java::lang::Object *, jlong, jlong) not implemented"));
}


void
sun::misc::Unsafe::putOrderedObject (::java::lang::Object *, jlong, ::java::lang::Object *)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("sun::misc::Unsafe::putOrderedObject (::java::lang::Object *, jlong, ::java::lang::Object *) not implemented"));
}


void
sun::misc::Unsafe::putIntVolatile (::java::lang::Object *, jlong, jint)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("sun::misc::Unsafe::putIntVolatile (::java::lang::Object *, jlong, jint) not implemented"));
}


jint
sun::misc::Unsafe::getIntVolatile (::java::lang::Object *, jlong)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("sun::misc::Unsafe::getIntVolatile (::java::lang::Object *, jlong) not implemented"));
}


void
sun::misc::Unsafe::putLongVolatile (::java::lang::Object *, jlong, jlong)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("sun::misc::Unsafe::putLongVolatile (::java::lang::Object *, jlong, jlong) not implemented"));
}


void
sun::misc::Unsafe::putLong (::java::lang::Object *, jlong, jlong)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("sun::misc::Unsafe::putLong (::java::lang::Object *, jlong, jlong) not implemented"));
}


jlong
sun::misc::Unsafe::getLongVolatile (::java::lang::Object *, jlong)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("sun::misc::Unsafe::getLongVolatile (::java::lang::Object *, jlong) not implemented"));
}


jlong
sun::misc::Unsafe::getLong (::java::lang::Object *, jlong)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("sun::misc::Unsafe::getLong (::java::lang::Object *, jlong) not implemented"));
}


void
sun::misc::Unsafe::putObjectVolatile (::java::lang::Object *, jlong, ::java::lang::Object *)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("sun::misc::Unsafe::putObjectVolatile (::java::lang::Object *, jlong, ::java::lang::Object *) not implemented"));
}


void
sun::misc::Unsafe::putObject (::java::lang::Object *, jlong, ::java::lang::Object *)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("sun::misc::Unsafe::putObject (::java::lang::Object *, jlong, ::java::lang::Object *) not implemented"));
}


::java::lang::Object *
sun::misc::Unsafe::getObjectVolatile (::java::lang::Object *, jlong)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("sun::misc::Unsafe::getObjectVolatile (::java::lang::Object *, jlong) not implemented"));
}


jint
sun::misc::Unsafe::arrayBaseOffset (::java::lang::Class *)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("sun::misc::Unsafe::arrayBaseOffset (::java::lang::Class *) not implemented"));
}


jint
sun::misc::Unsafe::arrayIndexScale (::java::lang::Class *)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("sun::misc::Unsafe::arrayIndexScale (::java::lang::Class *) not implemented"));
}


void
sun::misc::Unsafe::unpark (::java::lang::Thread *)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("sun::misc::Unsafe::unpark (::java::lang::Thread *) not implemented"));
}


void
sun::misc::Unsafe::park (jboolean, jlong)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("sun::misc::Unsafe::park (jboolean, jlong) not implemented"));
}
