// Code generator for bytecode.

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

#ifndef GCJX_BYTECODE_BYTEGEN_HH
#define GCJX_BYTECODE_BYTEGEN_HH

#include "codegen.hh"
#include "bytecode/classwriter.hh"

/// This is the primary interface to bytecode generation.
class bytecode_code_generator : public code_generator
{
public:

  /// Construct a new bytecode code generator.  Argument is the
  /// compiler being used to generate code.
  bytecode_code_generator (compiler *, directory_cache &);

  /// This is called to generate code for a class and write it.
  void generate (model_class *);
};

#endif // GCJX_BYTECODE_BYTEGEN_HH
