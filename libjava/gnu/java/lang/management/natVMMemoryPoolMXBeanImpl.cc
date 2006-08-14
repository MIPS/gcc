#include <config.h>

#include <gnu/java/lang/management/VMMemoryPoolMXBeanImpl.h>
#include <gcj/cni.h>
#include <java/lang/UnsupportedOperationException.h>

::java::lang::management::MemoryUsage *
gnu::java::lang::management::VMMemoryPoolMXBeanImpl::getCollectionUsage (::java::lang::String *)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMMemoryPoolMXBeanImpl::getCollectionUsage (::java::lang::String *) not implemented"));
}


jlong
gnu::java::lang::management::VMMemoryPoolMXBeanImpl::getCollectionUsageThreshold (::java::lang::String *)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMMemoryPoolMXBeanImpl::getCollectionUsageThreshold (::java::lang::String *) not implemented"));
}


jlong
gnu::java::lang::management::VMMemoryPoolMXBeanImpl::getCollectionUsageThresholdCount (::java::lang::String *)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMMemoryPoolMXBeanImpl::getCollectionUsageThresholdCount (::java::lang::String *) not implemented"));
}


JArray< ::java::lang::String *> *
gnu::java::lang::management::VMMemoryPoolMXBeanImpl::getMemoryManagerNames (::java::lang::String *)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMMemoryPoolMXBeanImpl::getMemoryManagerNames (::java::lang::String *) not implemented"));
}


::java::lang::management::MemoryUsage *
gnu::java::lang::management::VMMemoryPoolMXBeanImpl::getPeakUsage (::java::lang::String *)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMMemoryPoolMXBeanImpl::getPeakUsage (::java::lang::String *) not implemented"));
}


::java::lang::String *
gnu::java::lang::management::VMMemoryPoolMXBeanImpl::getType (::java::lang::String *)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMMemoryPoolMXBeanImpl::getType (::java::lang::String *) not implemented"));
}


::java::lang::management::MemoryUsage *
gnu::java::lang::management::VMMemoryPoolMXBeanImpl::getUsage (::java::lang::String *)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMMemoryPoolMXBeanImpl::getUsage (::java::lang::String *) not implemented"));
}


jlong
gnu::java::lang::management::VMMemoryPoolMXBeanImpl::getUsageThreshold (::java::lang::String *)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMMemoryPoolMXBeanImpl::getUsageThreshold (::java::lang::String *) not implemented"));
}


jlong
gnu::java::lang::management::VMMemoryPoolMXBeanImpl::getUsageThresholdCount (::java::lang::String *)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMMemoryPoolMXBeanImpl::getUsageThresholdCount (::java::lang::String *) not implemented"));
}


jboolean
gnu::java::lang::management::VMMemoryPoolMXBeanImpl::isValid (::java::lang::String *)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMMemoryPoolMXBeanImpl::isValid (::java::lang::String *) not implemented"));
}


void
gnu::java::lang::management::VMMemoryPoolMXBeanImpl::resetPeakUsage (::java::lang::String *)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMMemoryPoolMXBeanImpl::resetPeakUsage (::java::lang::String *) not implemented"));
}


void
gnu::java::lang::management::VMMemoryPoolMXBeanImpl::setCollectionUsageThreshold (::java::lang::String *, jlong)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMMemoryPoolMXBeanImpl::setCollectionUsageThreshold (::java::lang::String *, jlong) not implemented"));
}


void
gnu::java::lang::management::VMMemoryPoolMXBeanImpl::setUsageThreshold (::java::lang::String *, jlong)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMMemoryPoolMXBeanImpl::setUsageThreshold (::java::lang::String *, jlong) not implemented"));
}
