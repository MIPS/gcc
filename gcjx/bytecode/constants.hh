// Bytecode constants.

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

#ifndef GCJX_BYTECODE_CONSTANTS_HH
#define GCJX_BYTECODE_CONSTANTS_HH

// Magic number for class files.
#define MAGIC 0xcafebabe

// Major and minor version numbers for various releases.
#define MAJOR_1_1 45
#define MINOR_1_1  3
#define MAJOR_1_2 46
#define MINOR_1_2  0
#define MAJOR_1_3 47
#define MINOR_1_3  0
#define MAJOR_1_4 48
#define MINOR_1_4  0
#define MAJOR_1_5 49
#define MINOR_1_5  0

typedef enum
{
  CONSTANT_Utf8 = 1,
  // FIXME: remove this, it isn't used... ?
  //  CONSTANT_Unicode = 2,
  CONSTANT_Integer = 3,
  CONSTANT_Float = 4,
  CONSTANT_Long = 5,
  CONSTANT_Double = 6,
  CONSTANT_Class = 7,
  CONSTANT_String = 8,
  CONSTANT_Fieldref = 9,
  CONSTANT_Methodref = 10,
  CONSTANT_InterfaceMethodref = 11,
  CONSTANT_NameAndType = 12,

  // These two are used by the verifier.
  CONSTANT_ResolvedClass = 16 | 7,
  CONSTANT_ResolvedString = 16 | 8
} classfile_type_constant;

#endif // GCJX_BYTECODE_CONSTANTS_HH
