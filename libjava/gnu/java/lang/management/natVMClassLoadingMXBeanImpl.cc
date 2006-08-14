#include <config.h>

#include <gnu/java/lang/management/VMClassLoadingMXBeanImpl.h>
#include <gcj/cni.h>
#include <java/lang/UnsupportedOperationException.h>

jint
gnu::java::lang::management::VMClassLoadingMXBeanImpl::getLoadedClassCount ()
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMClassLoadingMXBeanImpl::getLoadedClassCount () not implemented"));
}


jlong
gnu::java::lang::management::VMClassLoadingMXBeanImpl::getUnloadedClassCount ()
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMClassLoadingMXBeanImpl::getUnloadedClassCount () not implemented"));
}


jboolean
gnu::java::lang::management::VMClassLoadingMXBeanImpl::isVerbose ()
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMClassLoadingMXBeanImpl::isVerbose () not implemented"));
}


void
gnu::java::lang::management::VMClassLoadingMXBeanImpl::setVerbose (jboolean)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("gnu::java::lang::management::VMClassLoadingMXBeanImpl::setVerbose (jboolean) not implemented"));
}
