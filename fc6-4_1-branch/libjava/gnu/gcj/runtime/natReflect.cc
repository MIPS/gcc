// natReflect.cc

/* Copyright (C) 2006  Free Software Foundation

   This file is part of libgcj.

This software is copyrighted work licensed under the terms of the
Libgcj License.  Please consult the file "LIBGCJ_LICENSE" for
details.  */

#include <config.h>
#include <platform.h>

#include <gcj/cni.h>
#include <gcj/method.h>
#include <jvm.h>

#include <gnu/gcj/runtime/Reflect.h>
#include <java/lang/reflect/Constructor.h>

jobject
gnu::gcj::runtime::Reflect::allocateObject (jclass klass,
					    java::lang::reflect::Constructor *cons)
{
  jobject result = _Jv_AllocObject (klass);
  // Only allow no-argument constructors.
  jmethodID meth = _Jv_FromReflectedConstructor (cons);
  ((void (*) (jobject)) meth->ncode) (result);
  return result;
}
