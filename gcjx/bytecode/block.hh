// Bytecode blocks.

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

#ifndef GCJX_BYTECODE_BLOCK_HH
#define GCJX_BYTECODE_BLOCK_HH

#include "bytecode/relocation.hh"

class bytecode_generator;

/// This is a single block of bytecode.  A bytecode block can have a
/// number of relocations associated with it.  Branches may point out
/// of the block arbitrarily, but all incoming relocations point only
/// at the beginning of the block.  Blocks are garbage collected and
/// kept in a chain.  A block also knows whether or not control flow
/// can "fall off the end" of itself.
class bytecode_block
{
  // PC for the start of this block.
  int pc;

  // Line number for the start of this block.
  int line;

  // Relocations for this block.
  std::list<ref_relocation> relocations;

  // Bytecode for this block.
  std::deque<jbyte> bytecode;

  // True if this block can fall through to the next block.
  bool fall_through;

  // True if this block has been marked live.
  bool live;

  // Next block.
  bytecode_block *next_block;

  // Perform some optimizations.  Returns true if anything changed,
  // false otherwise.
  bool optimize ();

  void copy (bytecode_stream *, int, int);

public:

  bytecode_block ()
    : pc (0),
      line (-1),
      fall_through (true),
      live (false),
      next_block (NULL)
  {
  }

  ~bytecode_block ()
  {
  }

  /// @return true if control flow can fall off the end of this block,
  /// false otherwise.
  bool can_complete_normally () const
  {
    return fall_through;
  }

  /// Set the normal-completion flag for this block.
  void set_can_complete_normally (bool f)
  {
    fall_through = f;
  }

  /// Return the next block in the chain.
  bytecode_block *next () const
  {
    return next_block;
  }

  /// Set the next block.
  void set_next (bytecode_block *n)
  {
    next_block = n;
  }

  /// Return the size of the bytecode in this block.  Note that this
  /// doesn't return the true value.  Relocations are under-counted,
  /// and in particular a relocation counts only for its "fixed
  /// cost".  E.g., a "goto" relocation will count as one byte, but a
  /// switch align relocation will count as zero.
  int length () const
  {
    int len = bytecode.size ();
    assert (! (len == 0 && ! fall_through));
    return len;
  }

  /// Return true if the block can be considered empty.
  bool empty_p () const
  {
    return bytecode.size () == 0 && relocations.size () == 0;
  }

  int get_pc () const
  {
    return pc;
  }

  void emit (jbyte val)
  {
    bytecode.push_back (val);
  }

  /// Relocate this block.
  /// @param pc an in-out parameter holding the current PC
  /// @return true if this block changed in some way, false otherwise
  bool relocate (int &, bytecode_stream *);

  /// Insert some code and a relocation.  If the relocation is for an
  /// unconditional branch, mark this block as not completing normally.
  /// @return false if the relocation is for a block-terminating
  /// (unconditional) branch.
  bool emit_relocation (relocation_kind what, bytecode_block *dest);

  /// @return true if live.
  bool live_p () const
  {
    return live;
  }

  /// Called at the start of the mark phase to reset liveness.
  void reset ()
  {
    live = false;
  }

  /// Call to mark this block as live and then push its references
  /// onto the work list.
  void mark (std::set<bytecode_block *> &work);

  /// Returns the next live code-containing block, starting with
  /// 'this'.  This method may return 'this' if this block is live and
  /// not empty.
  bytecode_block *update ();

  /// Set the line number for this block.  This can only be set once.
  void set_line_number (int l)
  {
    assert (line == -1);
    assert (l != -1);
    line = l;
  }
    
  /// Get the line number of this block.  Returns -1 if there isn't a
  /// line number.
  int get_line_number () const
  {
    return line;
  }
};

#endif // GCJX_BYTECODE_BLOCK_HH
