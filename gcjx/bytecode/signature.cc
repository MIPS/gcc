// Signature parsing.

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

#include "typedefs.hh"
#include "bytecode/signature.hh"

void
signature_parser::error_if (bool cond, const char *fmt)
{
  if (cond)
    throw class_file_error (where, fmt);
}

void
signature_parser::require (char c)
{
  if (it == done)
    throw class_file_error (where, "expected %1 but saw end of signature") % c;
  if (*it != c)
    throw class_file_error (where, "expected %1 but saw %2") % c % *it;
  ++it;
}

std::string
signature_parser::find_substring (char c)
{
  std::string::const_iterator start = it;
  while (it != done && *it != c)
    ++it;
  error_if (it == done, "premature end of signature");
  return std::string (start, it);
}

// This parses a reference to a type variable.
ref_forwarding_type
signature_parser::parse_one_type_variable ()
{
  require ('T');
  std::string name = find_substring (';');
  require (';');

  std::list<std::string> full;
  full.push_back (name);
  // Resolution handles this properly.
  return new model_forwarding_simple (where, full);
}

// This parses a single formal type parameter and bound.
ref_type_variable
signature_parser::parse_one_parameter ()
{
  std::string id = find_substring (':');

  std::list<ref_forwarding_type> bounds;
  bool first = true;
  while (it != done && *it == ':')
    {
      require (':');
      if (*it == '>' || *it == ':')
	{
	  // Empty bound is ok if it is the first.
	  error_if (! first, "empty bound not in first position");
	}
      else
	bounds.push_back (parse_one_type ());
      first = false;
    }
  error_if (it == done, "premature end of signature");

  ref_type_variable r = new model_type_variable (where, bounds);
  r->set_name (id);
  return r;
}

// This parses formal type parameters.
void
signature_parser::parse_type_parameters (std::list<ref_type_variable> &result)
{
  require ('<');

  while (it != done && *it != '>')
    result.push_back (parse_one_parameter ());

  require ('>');
}

ref_forwarding_type
signature_parser::parse_actual_type_arguments (const ref_forwarding_type &base)
{
  require ('<');

  ref_forwarding_parameterized result
    = new model_forwarding_parameterized (where, base);

  while (it != done && *it != '>')
    {
      char wrap = *it;
      ref_forwarding_type type;
      if (wrap == '*')
	{
	  // Unbounded wildcard.
	  require ('*');
	}
      else if (wrap == '-' || wrap == '+')
	{
	  // Wildcard with bound.
	  require (wrap);
	}
      else
	wrap = '\0';

      if (wrap != '*')
	{
	  // FIXME: can we have array types as wildcard bounds?
	  if (*it == 'T')
	    type = parse_one_type_variable ();
	  else
	    type = parse_ref_type ();
	}

      if (wrap != '\0')
	{
	  ref_wildcard wild = new model_wildcard (where);
	  if (type)
	    wild->set_bound (type);
	  if (wrap == '-')
	    wild->set_super ();
	  type = new model_forwarding_owned (where, wild);
	}
      result->add (type);
    }

  require ('>');

  return result;
}

// This parses a single reference type.
ref_forwarding_type
signature_parser::parse_ref_type ()
{
  require ('L');

  ref_forwarding_type result;

  // First parse the package and class name.
  {
    std::string::const_iterator start = it;
    while (it != done && *it != ';' && *it != '.' && *it != '.' && *it != '<')
      ++it;
    error_if (it == done, "premature end of signature");
    result = new model_forwarding_full (where, std::string (start, it));
  }

  // If the type was generic, read the arguments.
  if (*it == '<')
    result = parse_actual_type_arguments (result);

  // Now we might have a series of inner classes, each possibly
  // generic.
  while (it != done && *it == '.')
    {
      require ('.');

      std::list<std::string> ids;
      std::string::const_iterator start = it;
      while (it != done && *it != '.')
	++it;
      
      result = new model_forwarding_inner (where,
					   split (std::string (start, it),
						  '.'),
					   result);

      if (it != done && *it == '<')
	result = parse_actual_type_arguments (result);
    }

  require (';');
  return result;
}

// This parses a reference to a single type.  It could be a type
// variable, a reference type, a primitive type, wrapped in any number
// of arrays.
ref_forwarding_type
signature_parser::parse_one_type ()
{
  int array_count = 0;
  while (it != done && *it == '[')
    {
      ++it;
      ++array_count;
    }
  error_if (it == done, "premature end of signature");

  ref_forwarding_type result;
  if (*it == 'T')
    result = parse_one_type_variable ();
  else if (*it == 'L')
    result = parse_ref_type ();
  else
    {
      std::string::const_iterator start = it;
      ++it;
      model_type *prim
	= classify_primitive_type_descriptor (std::string (start, it));
      error_if (! prim, "invalid primitive type descriptor");
      assert (prim->primitive_p () || prim == primitive_void_type);
      result = new model_forwarding_resolved (where, prim);
    }
  assert (result);

  while (array_count-- > 0)
    result = result->array ();

  return result;
}

void
signature_parser::parse_method_signature (std::list<ref_forwarding_type> &arg_types,
					  std::list<ref_type_variable> &type_vars,
					  ref_forwarding_type &return_type,
					  std::list<ref_forwarding_type> &throws)
{
  if (*it == '<')
    parse_type_parameters (type_vars);

  require ('(');
  while (it != done && *it != ')')
    arg_types.push_back (parse_one_type ());
  require (')');

  return_type = parse_one_type ();

  while (it != done && *it == '^')
    {
      require ('^');
      throws.push_back (parse_one_type ());
    }

  error_if (it != done, "method signature too long");
}

void
signature_parser::parse_class_signature (std::list<ref_type_variable> &type_vars,
					 ref_forwarding_type &super,
					 std::list<ref_forwarding_type> &interfaces)
{
  if (*it == '<')
    parse_type_parameters (type_vars);

  super = parse_one_type ();
  while (it != done)
    interfaces.push_back (parse_one_type ());

  error_if (it != done, "class signature too long");
}

ref_forwarding_type
signature_parser::parse_type ()
{
  ref_forwarding_type result = parse_one_type ();
  error_if (it != done, "type signature too long");
  return result;
}
