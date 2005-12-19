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

/// For a given integer comparison opcode, this will return the
/// corresponding opcode that compares against zero.  For instance,
/// for op_if_icmpge, this will return op_ifge.
inline java_opcode
zero_if_opcode (java_opcode base)
{
  return java_opcode (op_ifeq + base - op_if_icmpeq);
}

/// Invert the sense of a comparison opcode.  For instance, given
/// op_if_cmpge, this will return op_if_cmplt.
inline java_opcode
invert_if_opcode (java_opcode base)
{
  if (base == op_ifnull)
    return op_ifnonnull;
  else if (base == op_ifnonnull)
    return op_ifnull;
  return java_opcode ((base & 1) ? base + 1 : base - 1);
}

/// Flip a comparison opcode.  This differs from 'invert' in that it
/// acts as if the left and right sides of the comparison are also
/// swapped.  So, for op_if_cmpge, it will return op_if_cmple.
inline java_opcode
flip_if_opcode (java_opcode base)
{
  switch (base)
    {
    case op_ifeq:
    case op_ifne:
    case op_if_icmpeq:
    case op_if_icmpne:
    case op_if_acmpeq:
    case op_if_acmpne:
    case op_ifnull:
    case op_ifnonnull:
      // Nothing.
      break;
    case op_iflt:
      base = op_ifgt;
      break;
    case op_ifge:
      base = op_ifle;
      break;
    case op_ifgt:
      base = op_iflt;
      break;
    case op_ifle:
      base = op_ifge;
      break;
    case op_if_icmplt:
      base = op_if_icmpgt;
      break;
    case op_if_icmpge:
      base = op_if_icmple;
      break;
    case op_if_icmpgt:
      base = op_if_icmplt;
      break;
    case op_if_icmple:
      base = op_if_icmpge;
      break;

    default:
      assert (0);
    }

  return base;
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
