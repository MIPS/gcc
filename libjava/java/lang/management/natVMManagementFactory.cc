#include <config.h>

#include <java/lang/management/VMManagementFactory.h>
#include <gcj/cni.h>
#include <java/lang/UnsupportedOperationException.h>

JArray< ::java::lang::String *> *
java::lang::management::VMManagementFactory::getMemoryPoolNames ()
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("java::lang::management::VMManagementFactory::getMemoryPoolNames () not implemented"));
}


JArray< ::java::lang::String *> *
java::lang::management::VMManagementFactory::getMemoryManagerNames ()
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("java::lang::management::VMManagementFactory::getMemoryManagerNames () not implemented"));
}


JArray< ::java::lang::String *> *
java::lang::management::VMManagementFactory::getGarbageCollectorNames ()
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("java::lang::management::VMManagementFactory::getGarbageCollectorNames () not implemented"));
}
