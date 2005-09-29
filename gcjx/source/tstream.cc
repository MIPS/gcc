// Implementation of the token stream.

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
#include "source/tstream.hh"

void
token_stream::lex_file ()
{
  token r;
  do
    {
      r = lexer::get_token ();
      buffer.push_back (r);
    }
  while (r != TOKEN_EOF);
}

token
token_stream::get_unfiltered_token ()
{
  if (read_position < buffer.size ())
    ++read_position;
  return buffer[read_position - 1];
}

token
token_stream::get_token ()
{
  token r;
  while (true)
    {
      r = get_unfiltered_token ();
      // In theory the lexer will never return a repeat or invalid
      // token.
      assert (r != TOKEN_REPEAT && r != TOKEN_INVALID);

      if (r != TOKEN_JAVADOC)
	break;
      if (javadoc_is_ok)
	{
	  // Look ahead -- if the next token is javadoc, we discard
	  // this one.
	  token n = peek_token ();
	  if (n != TOKEN_JAVADOC)
	    break;
	}
    }

  javadoc_is_ok = false;
  return r;
}

template<typename T>
class saver
{
  T &location;
  T save;

public:

  saver (T &loc, T new_val)
    : location (loc),
      save (loc)
  {
    location = new_val;
  }

  saver (T &loc)
    : location (loc),
      save (loc)
  {
  }

  ~saver ()
  {
    location = save;
  }
};

token
token_stream::peek_token ()
{
  saver<bool> save_jd (javadoc_is_ok);
  saver<int> save_read (read_position);
  token r = get_token ();
  return r;
}

token
token_stream::peek_token1 ()
{
  saver<bool> save_jd (javadoc_is_ok);
  saver<int> save_read (read_position);
  get_token ();
  token r = get_token ();
  return r;
}
