// natReflection.cc

/* Copyright (C) 2006
   Free Software Foundation

   This file is part of libgcj.

This software is copyrighted work licensed under the terms of the
Libgcj License.  Please consult the file "LIBGCJ_LICENSE" for
details.  */

#include <sun/reflect/Reflection.h>
#include <java/lang/Thread.h>

#include <java-interp.h>
#include <java-stack.h>
#include <execution.h>

jclass
sun::reflect::Reflection::getCallerClass (jint n)
{
  // FIXME: grossly inefficient.  What we really need is
  // _Jv_StackTrace::GetCallerClass(int), so we don't end up tracing
  // the entire stack.

  JArray<jclass> *result = _Jv_StackTrace::GetClassContext (NULL);
  return elements (result)[n-1];
}
