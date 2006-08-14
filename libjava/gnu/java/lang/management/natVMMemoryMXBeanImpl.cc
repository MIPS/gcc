#include <config.h>

#include <gnu/java/lang/management/VMMemoryMXBeanImpl.h>
#include <gcj/cni.h>
#include <java/lang/UnsupportedOperationException.h>

::java::lang::management::MemoryUsage *
gnu::java::lang::management::VMMemoryMXBeanImpl::getNonHeapMemoryUsage ()
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMMemoryMXBeanImpl::getNonHeapMemoryUsage () not implemented"));
}


jint
gnu::java::lang::management::VMMemoryMXBeanImpl::getObjectPendingFinalizationCount ()
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMMemoryMXBeanImpl::getObjectPendingFinalizationCount () not implemented"));
}


jboolean
gnu::java::lang::management::VMMemoryMXBeanImpl::isVerbose ()
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMMemoryMXBeanImpl::isVerbose () not implemented"));
}


void
gnu::java::lang::management::VMMemoryMXBeanImpl::setVerbose (jboolean)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMMemoryMXBeanImpl::setVerbose (jboolean) not implemented"));
}
