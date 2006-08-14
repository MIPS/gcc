#include <config.h>

#include <java/net/VMURLConnection.h>
#include <gcj/cni.h>
#include <java/lang/UnsupportedOperationException.h>

void
java::net::VMURLConnection::init ()
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("java::net::VMURLConnection::init () not implemented"));
}


::java::lang::String *
java::net::VMURLConnection::guessContentTypeFromBuffer (jbyteArray, jint)
{
  throw new ::java::lang::UnsupportedOperationException (JvNewStringLatin1 ("java::net::VMURLConnection::guessContentTypeFromBuffer (jbyteArray, jint) not implemented"));
}
