// Bytecode utility functions.

// Copyright (C) 2004, 2005 Free Software Foundation, Inc.
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

#ifndef GCJX_BYTECODE_BYTEUTIL_HH
#define GCJX_BYTECODE_BYTEUTIL_HH

inline java_opcode
zero_if_opcode (java_opcode base)
{
  return java_opcode (op_ifeq + base - op_if_icmpeq);
}

inline java_opcode
invert_if_opcode (java_opcode base)
{
  if (base == op_ifnull)
    return op_ifnonnull;
  else if (base == op_ifnonnull)
    return op_ifnull;
  return java_opcode ((base & 1) ? base + 1 : base - 1);
}

/// Return true if the value is a 'return' bytecode of any kind.
inline bool
return_p (int op)
{
  return (op == op_ireturn || op == op_lreturn || op == op_freturn
	  || op == op_dreturn || op == op_areturn || op == op_return);
}

inline bool
wide_p (const model_type *t)
{
  return t == primitive_double_type || t == primitive_long_type;
}

#endif // GCJX_BYTECODE_BYTEUTIL_HH
