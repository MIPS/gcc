// Code generator.

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

#ifndef GCJX_CODEGEN_HH
#define GCJX_CODEGEN_HH

#include "directory.hh"

/// This is the API for code generation.  A code generator is a
/// subclass of this class, and implements its abstract methods.  Code
/// generators are instantiated by the driver and passed to the
/// 'compiler' object; the compiler is responsible for deleting them.
class code_generator
{
protected:

  /// The code generator can use a shared directory cache to keep
  /// track of output directories, if desired.
  directory_cache &directories;

  code_generator (directory_cache &dirs)
    : directories (dirs)
  {
  }

public:

  /// Generate code for a single class.
  virtual void generate (model_class *) = 0;

  virtual ~code_generator ()
  {
  }
};

#endif // GCJX_CODEGEN_HH
