// Simple state machines for parsing.

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

#ifndef GCJX_SOURCE_MACHINE_HH
#define GCJX_SOURCE_MACHINE_HH

#include <stdarg.h>

template<typename T>
class state_machine
{
  // The parser we're using.
  parse *parser;

public:

  // Type of the method pointers.
  typedef T (parse::*parse_function) ();

  state_machine (parse *p)
    : parser (p)
  {
  }

  T run (parse_function first, ...)
  {
    marker mark (parser->our_token_stream);

    T result;

    va_list ap;
    va_start (ap, first);

    parse_function f = first;
    while (f != NULL)
      {
	try
	  {
	    result = (parser->*f) ();
	    break;
	  }
	catch (syntax_error &)
	  {
	    mark.backtrack ();
	  }
	f = va_arg (ap, parse_function);
      }

    va_end (ap);

    return result;
  }
};


template<token_value V, typename T>
model_binary_base *token_matcher (const token &arg)
{
  if (arg == V)
    return new T ((location) arg);
  return NULL;
}

template<token_value V1, typename T1, token_value V2, typename T2>
model_binary_base *token_matcher (const token &arg)
{
  if (arg == V1)
    return new T1 ((location) arg);
  if (arg == V2)
    return new T2 ((location) arg);
  return NULL;
}

template<token_value V1, typename T1, token_value V2, typename T2,
	 token_value V3, typename T3>
model_binary_base *token_matcher (const token &arg)
{
  if (arg == V1)
    return new T1 ((location) arg);
  if (arg == V2)
    return new T2 ((location) arg);
  if (arg == V3)
    return new T3 ((location) arg);
  return NULL;
}

template<expression_parser FUN, model_binary_base *MATCH_P (const token &)>
class recursion_machine
{
  // The parser.
  parse *parser;

public:

  recursion_machine (parse *p)
    : parser (p)
  {
  }

  ref_expression run ()
  {
    ref_expression result = (parser->*FUN) ();

    while (true)
      {
	token t = parser->peek ();
	// FIXME: some base type other than binary
	ref_binary op = MATCH_P (t);
	if (! op)
	  break;

	// Ignore the next token, we already matched it.
	parser->get ();
	ref_expression rhs = (parser->*FUN) ();
	op->set_lhs (result);
	op->set_rhs (rhs);
	result = op;
      }

    return result;
  }
};

#endif // GCJX_SOURCE_MACHINE_HH
