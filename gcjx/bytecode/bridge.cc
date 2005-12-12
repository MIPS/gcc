// Bridge method generation

// Copyright (C) 2005 Free Software Foundation, Inc.
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
#include "bytecode/outpool.hh"
#include "bytecode/locals.hh"
#include "bytecode/block.hh"
#include "bytecode/attribute.hh"
#include "bytecode/bridge.hh"
#include "bytecode/byteutil.hh"

void
bridge_method::emit (bytecode_stream &out)
{
  bytecode_attribute::emit (out);

  out.put2 (max_stack);
  out.put2 (max_locals);
  out.put4 (bytecode_length);
  emit_code (&out);

  // Exception table.
  out.put2 (0);
  // Attributes.
  out.put2 (0);
}

int
bridge_method::emit_code (bytecode_stream *out)
{
  model_method *parent = method->get_override ()->get_parent ();
  assert (parent != NULL);

  std::list<ref_variable_decl> m_parms = method->get_parameters ();
  std::list<ref_variable_decl> p_parms = parent->get_parameters ();

  std::list<ref_variable_decl>::const_iterator m_it = m_parms.begin ();
  std::list<ref_variable_decl>::const_iterator p_it = p_parms.begin ();

  int result = 0;
  int index = 0;
  max_stack = 0;
  max_locals = 0;

  // Start with the 'load' opcode for 'this'.
  if (out)
    out->put (op_aload_0);
  else
    ++result;
  ++index;
  ++max_stack;
  ++max_locals;

  while (m_it != m_parms.end () && p_it != p_parms.end ())
    {
      model_type *p_type = (*p_it)->type ()->erasure ();

      max_stack += wide_p (p_type) ? 2 : 1;
      max_locals += wide_p (p_type) ? 2 : 1;

      // Emit the load opcode for the argument type.
      int base;
      if (p_type == primitive_long_type)
	base = op_lload;
      else if (p_type == primitive_float_type)
	base = op_fload;
      else if (p_type == primitive_double_type)
	base = op_dload;
      else if (p_type->reference_p ())
	base = op_aload;
      else
	base = op_iload;
      if (index <= 3)
	base = op_iload_0 + 4 * (base - op_iload) + index;

      if (out)
	{
	  out->put (base);
	  if (index > 3)
	    out->put (index);
	}
      else
	{
	  ++result;
	  if (index > 3)
	    ++result;
	}

      // Emit the 'checkcast' if needed.
      model_type *m_type = (*m_it)->type ()->erasure ();
      if (m_type != p_type)
	{
	  assert (p_type->reference_p ());
	  assert (m_type->reference_p ());
	  int entry = pool->add (m_type);
	  if (out)
	    {
	      out->put (op_checkcast);
	      out->put2 (entry);
	    }
	  else
	    result += 3;
	}

      ++m_it;
      ++p_it;
      ++index;
    }

  // Handle the invoke.
  int entry = pool->add (method->get_declaring_class (), method);
  if (out)
    {
      out->put (op_invokevirtual);
      out->put2 (entry);
    }
  else
    result += 3;

  // Handle the return.  FIXME: do we need a checkcast for the return?
  // I can't see why we would.
  if (out)
    {
      model_type *return_type = method->get_return_type ()->erasure ();
      int op;
      if (return_type == primitive_void_type)
	op = op_return;
      else if (return_type == primitive_long_type)
	op = op_lreturn;
      else if (return_type == primitive_float_type)
	op = op_freturn;
      else if (return_type == primitive_double_type)
	op = op_dreturn;
      else if (return_type->reference_p ())
	op = op_areturn;
      else
	op = op_ireturn;
      out->put (op);
    }
  else
    ++result;

  return result;
}

int
bridge_method::size ()
{
  // Include max stack, locals, etc.
  return 8 + bytecode_length + 2 + 2;
}

bool
bridge_method::required_p (model_method *method)
{
  if (method->static_p ())
    return false;
  model_method *parent = method->get_override ();
  if (parent == NULL)
    return false;
  parent = parent->get_parent ();

  std::list<ref_variable_decl> m_parms = method->get_parameters ();
  std::list<ref_variable_decl> p_parms = parent->get_parameters ();

  std::list<ref_variable_decl>::const_iterator m_it = m_parms.begin ();
  std::list<ref_variable_decl>::const_iterator p_it = p_parms.begin ();

  while (m_it != m_parms.end () && p_it != p_parms.end ())
    {
      if ((*m_it)->type ()->erasure () != (*p_it)->type ()->erasure ())
	return true;

      ++m_it;
      ++p_it;
    }

  return false;
}
