// Relocations.

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

#include "typedefs.hh"
#include "bytecode/block.hh"
#include "bytecode/byteutil.hh"
#include "bytecode/outstream.hh"

relocation::relocation (relocation_kind what, int off,
			bytecode_block *dest)
  : refc (0),
    type (what),
    offset (off),
    use_length (0),
    destination (dest)
{
  // We should never construct far branches, they should only be
  // created by widening a near branch.
  assert (what >= 0 && what != jsr_w && what != goto_w);
  if (what == switch_table)
    use_length = 4;
  else if (what != switch_align)
    use_length = 2;
}

relocation::relocation (relocation *other, bytecode_block *new_dest)
  : refc (0),
    type (other->type),
    offset (other->offset),
    use_length (other->use_length),
    destination (new_dest)
{
}

bool
relocation::perform (int pc, int block_start, bytecode_stream *out)
{
  int delta = 0;
  int save = use_length;

  switch (type)
    {
    case switch_align:
      assert (! destination);
      use_length = ((pc + 3) & ~ 3) - pc;
      if (out)
	{
	  for (int i = 0; i < use_length; ++i)
	    out->put (0);
	}
      break;

    case switch_table:
      // A switch table relocation is from the switch opcode, which is
      // just before the start of this block.
      delta = destination->get_pc () - (block_start - 1);
      if (out)
	out->put4 (delta);
      break;

    case reloc_goto:
    case jsr:
      delta = destination->get_pc () - (pc - 1);
      if (delta >= -32768 && delta < 32768)
	{
	  if (out)
	    out->put2 (delta);
	}
      else
	{
	  assert (! out);
	  // We need to expand this branch.  We just make it
	  // permanently wide by modifying this relocation.
	  if (type == reloc_goto)
	    type = goto_w;
	  else
	    type = jsr_w;
	  use_length += 2;
	}
      break;

    case goto_w:
    case jsr_w:
      delta = destination->get_pc () - (pc - 1);
      if (out)
	{
	  out->remove ();
	  out->put (type);
	  out->put4 (delta);
	}
      break;

    case ifeq:
    case ifne:
    case iflt:
    case ifle:
    case ifge:
    case ifgt:
    case if_icmpeq:
    case if_icmpne:
    case if_icmplt:
    case if_icmpge:
    case if_icmpgt:
    case if_icmple:
    case if_acmpeq:
    case if_acmpne:
    case ifnull:
    case ifnonnull:
      delta = destination->get_pc () - (pc - 1);
      if (delta >= -32768 && delta < 32768)
	{
	  if (out)
	    out->put2 (delta);
	}
      else
	{
	  assert (! out);
	  type = relocation_kind (- invert_if_opcode (java_opcode (type)));
	  use_length += 5;
	}
      break;

    case long_ifeq:
    case long_ifne:
    case long_iflt:
    case long_ifle:
    case long_ifge:
    case long_ifgt:
    case long_if_icmpeq:
    case long_if_icmpne:
    case long_if_icmplt:
    case long_if_icmpge:
    case long_if_icmpgt:
    case long_if_icmple:
    case long_if_acmpeq:
    case long_if_acmpne:
    case long_ifnull:
    case long_ifnonnull:
      // "3" is the offset to the goto_w opcode.
      delta = destination->get_pc () - (pc + 3 - 1);
      if (out)
	{
	  // We have if<condition> TARGET.  Change to:
	  // if<! condition> SKIP; goto_w TARGET; SKIP:.
	  out->remove ();
	  out->put (- type);
	  // Length of the if<cond> instruction plus the length of
	  // the goto_w instruction.
	  out->put2 (3 + 5);
	  out->put (goto_w);
	  out->put4 (delta);
	}
      break;
    }

  return use_length != save;
}

// If we're pointing at an empty block, update to the next non-empty
// block.
void
relocation::update ()
{
  if (type == switch_align)
    return;
  assert (destination->live_p ());
  while (destination->can_complete_normally () && destination->empty_p ())
    destination = destination->next ();
  assert (destination->live_p ());
}

bool
relocation::conditional_p () const
{
  return (type == ifeq
	  || type == ifne
	  || type == iflt
	  || type == ifle
	  || type == ifge
	  || type == ifgt
	  || type == if_icmpeq
	  || type == if_icmpne
	  || type == if_icmplt
	  || type == if_icmple
	  || type == if_icmpge
	  || type == if_icmpgt
	  || type == if_icmple
	  || type == if_acmpeq
	  || type == if_acmpne
	  || type == ifnull
	  || type == ifnonnull);
}

void
relocation::invert_condition ()
{
  assert (conditional_p ());
  type = relocation_kind (invert_if_opcode (java_opcode (type)));
}
