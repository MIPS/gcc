// Class instance creator.

// Copyright (C) 2004 Free Software Foundation, Inc.
//
// This file is part of GCC.
//
// gcjx is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// gcjx is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with gcjx; see the file COPYING.LIB.  If
// not, write to the Free Software Foundation, Inc.,
// 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

#ifndef GCJX_READER_CLASSINST_HH
#define GCJX_READER_CLASSINST_HH

#include <time.h>

/// This class is the base class for class-reading factory proxies.  A
/// concrete instance of this class can be applied to do the work of
/// reading a class or java file.  Each instance of a
/// class_instance_creator is associated with some underlying
/// resource, for instance a .class file inside a .jar file, a .java
/// file in the file system, or a class in a shared object.
class class_instance_creator
{
public:

  class_instance_creator ()
  {
  }

  virtual ~class_instance_creator ()
  {
  }

  /// Return the modification time of the associated resource.
  virtual time_t get_mtime () = 0;

  /// Read the resource and instantiate the contents.
  /// @param emit boolean which is true if code should be emitted for
  /// this compilation unit.
  virtual void apply (bool) = 0;
};

#endif // GCJX_READER_CLASSINST_HH
