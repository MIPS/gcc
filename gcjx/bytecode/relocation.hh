// Bytecode relocation.

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

#ifndef GCJX_BYTECODE_RELOCATION_HH
#define GCJX_BYTECODE_RELOCATION_HH

#include "bytecode/insns.hh"

class bytecode_block;
class bytecode_stream;

// The values in this enum matter and are used by the code.  In
// particular the simple operation entries have the same value as
// the corresponding opcodes, and each long- or wide- operation
// entry is the negation of the corresponding simple entry.
enum relocation_kind
{
  // Align a switch body.
  switch_align,
  // Entry in a switch table, a 4-byte relocation.
  switch_table,
  // Relocate a short opcode.
  reloc_goto = op_goto,
  jsr = op_jsr,
  // Relocate a wide opcode.
  goto_w = op_goto_w,
  jsr_w = op_jsr_w,
  // Relocate a near branch.
  ifeq = op_ifeq,
  ifne = op_ifne,
  iflt = op_iflt,
  ifle = op_ifle,
  ifge = op_ifge,
  ifgt = op_ifgt,
  if_icmpeq = op_if_icmpeq,
  if_icmpne = op_if_icmpne,
  if_icmplt = op_if_icmplt,
  if_icmpge = op_if_icmpge,
  if_icmpgt = op_if_icmpgt,
  if_icmple = op_if_icmple,
  if_acmpeq = op_if_acmpeq,
  if_acmpne = op_if_acmpne,
  ifnull = op_ifnull,
  ifnonnull = op_ifnonnull,
  // Relocate a far branch.
  long_ifeq = - op_ifeq,
  long_ifne = - op_ifne,
  long_iflt = - op_iflt,
  long_ifle = - op_ifle,
  long_ifge = - op_ifge,
  long_ifgt = - op_ifgt,
  long_if_icmpeq = - op_if_icmpeq,
  long_if_icmpne = - op_if_icmpne,
  long_if_icmplt = - op_if_icmplt,
  long_if_icmpge = - op_if_icmpge,
  long_if_icmpgt = - op_if_icmpgt,
  long_if_icmple = - op_if_icmple,
  long_if_acmpeq = - op_if_acmpeq,
  long_if_acmpne = - op_if_acmpne,
  long_ifnull = - op_ifnull,
  long_ifnonnull = - op_ifnonnull,
};

/// An object of class relocation is used to represent a single
/// relocatable branch target of some kind.  Each relocation knows its
/// own semantics and will handle situations such as branches out of
/// range.  Most relocations have a target and point to the beginning
/// of a block; the only exceptions to this rule are switch_align
/// relocations, which are used to add the correct amount of padding
/// to a switch statement, and exception handler relocations, which
/// can conceptually point to the end of a block.
class relocation
{
  // Reference count.
  int refc;

  // Type of the relocation.
  relocation_kind type;

  // Offset from the start of the current block.
  int offset;

  // Length of this use.  This is the length of the bytes involved in
  // the relocation itself -- it doesn't include any "fixed part",
  // like the opcode.
  int use_length;

  // The destination block.  All relocations point to the start of a
  // block.
  bytecode_block *destination;

  // For reference count access.
  template<typename T> friend class owner;

public:

  relocation (relocation_kind, int, bytecode_block *);

  relocation (relocation *, bytecode_block *);

  // Perform the relocation.
  bool perform (int, int, bytecode_stream *);

  int get_use_length () const
  {
    return use_length;
  }

  int get_offset () const
  {
    return offset;
  }

  relocation_kind get_kind () const
  {
    return type;
  }

  bool conditional_p () const;

  void update ();

  void invert_condition ();

  bytecode_block *get_target () const
  {
    return destination;
  }

  void set_target (bytecode_block *b)
  {
    destination = b;
  }
};

typedef owner<relocation> ref_relocation;

#endif // GCJX_BYTECODE_RELOCATION_HH
