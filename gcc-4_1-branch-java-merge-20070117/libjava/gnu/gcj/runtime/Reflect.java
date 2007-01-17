/* Copyright (C) 2006  Free Software Foundation

   This file is part of libgcj.

This software is copyrighted work licensed under the terms of the
Libgcj License.  Please consult the file "LIBGCJ_LICENSE" for
details.  */


package gnu.gcj.runtime;

import java.lang.reflect.Constructor;

public class Reflect
{
  /**
   * Allocate an object whose class is the indicated class, using the
   * given constructor.  Unlike {@link Constructor.newInstance}, the
   * constructor need not be declared in the indicated class -- it
   * can be a constructor from any superclass.  Only a no-argument
   * constructor is allowed.
   */
  public static native Object allocateObject(Class klass,
					     Constructor constructor);
}
