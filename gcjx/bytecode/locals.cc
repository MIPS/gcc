// Local variable handling.

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
#include "bytecode/outpool.hh"
#include "bytecode/locals.hh"
#include "bytecode/insns.hh"
#include "bytecode/block.hh"
#include "bytecode/attribute.hh"
#include "bytecode/generate.hh"
#include "bytecode/byteutil.hh"

void
locals::push_scope (const model_stmt *newscope)
{
  scope.push_back (newscope);
}

int
locals::request (bool is_wide)
{
  int delta = is_wide ? 1 : 0;

  // This loop is written a little strangely so that it will always
  // terminate with N == MAX if we didn't find a large enough empty
  // space.
  int n;
  for (n = 0; n < max; ++n)
    {
      if (! used[n] && (n + delta < max && ! used[n + delta]))
	break;
    }

  if (n >= max)
    {
      max += delta + 1;
      used.resize (max);
    }
  used[n] = true;
  used[n + delta] = true;

  return n;
}

int
locals::request (model_variable_decl *decl)
{
  assert (decl == NULL || vars.find (decl) == vars.end ());
  int n = request (decl ? wide_p (decl->type ()) : false);

  if (decl)
    {
      debug_info info;
      info.variable = decl;
      info.start = gen->get_current ();
      info.end = NULL;
      info.scope = scope.back ();
      info.index = n;
      vars[decl] = info;
    }
  return n;
}

int
locals::request ()
{
  return request (true);
}

int
locals::get_index (model_variable_decl *decl)
{
  // It would be way easier to have a field on the variable.
  std::map<model_variable_decl *, debug_info>::const_iterator i
    = vars.find (decl);
  assert (i != vars.end ());
  return (*i).second.index;
}

void
locals::remove (model_variable_decl *decl)
{
  std::map<model_variable_decl *, debug_info>::iterator i
    = vars.find (decl);
  assert (i != vars.end ());

  debug_info info = (*i).second;

  used[info.index] = false;
  if (wide_p (decl ? decl->type () : NULL))
    used[info.index + 1] = false;

  vars.erase (i);

  info.end = gen->get_current ();
  var_descriptions.push_back (info);
}

void
locals::remove (int n)
{
  // This should never be called for wide variables, but at the moment
  // we have no way of asserting that.
  used[n] = false;
}

void
locals::remove (const model_stmt *stmt)
{
  assert (stmt != NULL);
  assert (stmt == scope.back ());
  scope.pop_back ();

  std::list<model_variable_decl *> dels;
  for (std::map<model_variable_decl *, debug_info>::iterator i
	 = vars.begin ();
       i != vars.end ();
       ++i)
    {
      debug_info info = (*i).second;
      if (info.scope == stmt)
	{
	  used[info.index] = false;
	  if (wide_p (info.variable->type ()))
	    used[info.index + 1] = false;
	  info.end = gen->get_current ();
	  var_descriptions.push_back (info);
	  dels.push_back (info.variable);
	}
    }

  for (std::list<model_variable_decl *>::const_iterator i = dels.begin ();
       i != dels.end ();
       ++i)
    vars.erase (*i);
}

bool
locals::update ()
{
  valid = 0;
  for (std::list<debug_info>::iterator i = var_descriptions.begin ();
       i != var_descriptions.end ();
       ++i)
    {
      debug_info &info (*i);

      info.start = info.start->update ();
      // Note that we can wind up with NULL here if a variable is
      // completely optimized away.
      if (info.start != NULL && info.start->live_p ())
	{
	  info.end = info.end->update ();
	  // NULL is ok here since that just means it is live through
	  // the end of the bytecode.
	  assert (info.end == NULL || info.end->live_p ());
	  // Don't emit debug info for a zero-length range.
	  if (info.start != info.end)
	    ++valid;
	}
    }
  return valid > 0;
}

bool
locals::any_parameterized_p ()
{
  for (std::list<debug_info>::iterator i = var_descriptions.begin ();
       i != var_descriptions.end ();
       ++i)
    {
      debug_info &info (*i);
      if (! info.start->live_p () || info.start == info.end)
	continue;

      model_type *t = info.variable->type ();
      if (t->erasure () == t)
	continue;

      return true;
    }
  return false;
}

int
locals::emit (outgoing_constant_pool *pool, bytecode_stream *out, bool types)
{
  int count = 0;
  for (std::list<debug_info>::iterator i = var_descriptions.begin ();
       i != var_descriptions.end ();
       ++i)
    {
      debug_info &info (*i);
      if (info.start == NULL || ! info.start->live_p ()
	  || info.start == info.end)
	continue;

      model_type *t = info.variable->type ();
      if (types && t->erasure () == t)
	continue;

      ++count;
      if (! out)
	{
	  // Make sure these are in the pool.
	  pool->add_utf (info.variable->get_name ());
	  pool->add_utf (types ? t->get_signature () : t->get_descriptor ());
	}
    }

  assert (count == valid);
  assert (count > 0);

  if (! out)
    return 2 + 10 * count;

  out->put2 (count);
  for (std::list<debug_info>::const_iterator i = var_descriptions.begin ();
       i != var_descriptions.end ();
       ++i)
    {
      const debug_info &info (*i);
      if (info.start == NULL || ! info.start->live_p ()
	  || info.start == info.end)
	continue;

      model_type *t = info.variable->type ();
      if (types && t->erasure () == t)
	continue;

      out->put2 (info.start->get_pc ());
      int last = info.end ? info.end->get_pc () : gen->get_byte_count ();
      out->put2 (last - info.start->get_pc ());
      out->put2 (pool->add_utf (info.variable->get_name ()));
      out->put2 (pool->add_utf (types ? t->get_signature ()
				: t->get_descriptor ()));
      out->put2 (info.index);
    }

  // The return value here doesn't matter, but we might as well be
  // correct.
  return 2 + 10 * count;
}
