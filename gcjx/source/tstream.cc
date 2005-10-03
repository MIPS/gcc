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
      r = lex_token ();
      // In theory the lexer will never return a repeat or invalid
      // token.
      assert (r != TOKEN_REPEAT && r != TOKEN_INVALID);

      if (r == TOKEN_JAVADOC)
	deprecated.insert (buffer.size ());
      else
	buffer.push_back (r);
    }
  while (r != TOKEN_EOF);
}

bool
token_stream::get_javadoc ()
{
  return deprecated.find (read_position) != deprecated.end ();
}
