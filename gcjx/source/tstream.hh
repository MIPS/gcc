// Token stream.

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

#ifndef GCJX_SOURCE_TSTREAM_HH
#define GCJX_SOURCE_TSTREAM_HH

#include <stack>

#include "source/lex.hh"

// The token stream wraps the lexer with a buffer.  The user can set a
// mark, and no token before the mark will be lost.  The token stream
// also understands a couple of special cases for java, and will
// filter the actual tokens according to what the parser requires.
class token_stream : public lexer
{
  typedef unsigned int position_type;

  // Our buffer.
  std::deque<token> buffer;

  // Position of next unread element in buffer.
  position_type read_position;

  // This holds the location of all tokens that were preceded by a
  // javadoc containing 'deprecated'.
  std::set<position_type> deprecated;


  // Set a mark at the current point.  Only called by marker class.
  // Returns the position.
  position_type set_mark ()
  {
    return read_position;
  }

  // Reset the read pointer to a position.  Only called by marker
  // class.
  void reset_to_mark (position_type where)
  {
    read_position = where;
  }

  friend class marker;


  // Lex the file and fill our buffer.
  void lex_file ();

public:

  token_stream (ucs2_reader *source, const char *file)
    : lexer (source, file),
      read_position (0)
  {
    lex_file ();
  }

  ~token_stream ()
  {
  }

  // Fetch the next token.  Javadoc tokens will be ignored.
  token &get_token ()
  {
    return buffer[read_position++];
  }

  // If the next token is a javadoc token, return the javadoc item.
  // Otherwise return an empty javadoc wrapper.
  bool get_javadoc ();

  // Look at the next token without committing to it.
  token &peek_token ()
  {
    return buffer[read_position];
  }

  // Look ahead two tokens without committing.  This is ad hoc, but it
  // is all the parser requires.
  token &peek_token1 ()
  {
    return buffer[read_position + 1];
  }
};

// This marks a location in a token stream buffer and ensures that no
// token past that point is destroyed.
class marker
{
  // The location of this mark.
  token_stream::position_type location;

  // Associated token stream.
  token_stream *stream;

public:

  marker (token_stream *s)
    : stream (s)
  {
    location = stream->set_mark ();
  }

  ~marker ()
  {
  }

  void backtrack ()
  {
    stream->reset_to_mark (location);
  }
};

#endif // GCJX_SOURCE_TSTREAM_HH
