// Implementation of the token stream.

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
#include "source/tstream.hh"

int
token_stream::set_mark ()
{
  if (marks.empty () && read_position > 0)
    {
      // There is no mark at present, but we're still buffering some
      // tokens.  Let's move the tokens to the start of the buffer to
      // make some space.  FIXME: or we could implement a circular
      // buffer, which means more complex logic here and there.
      // FIXME: or we could just optimistically wait until the buffer
      // is about to overflow.
      for (int i = read_position; i < buffer_end; ++i)
	buffer[i - read_position] = buffer[i];
      buffer_end -= read_position;
      read_position = 0;
    }
  else if (buffer == NULL)
    {
      buffer_size = 64;
      buffer = new token[buffer_size];
    }

  int result = read_position;
  assert (! marks.empty () || read_position == 0);
  marks.push (result);
  mark_buffering = true;
  return result;
}

void
token_stream::unset_mark (int position)
{
  assert (marks.top () == position);
  marks.pop ();
  mark_buffering = ! marks.empty ();
}

void
token_stream::reset_to_mark (int position)
{
  // Since we require marks to be nested, and since the mark
  // destructor removes it, we know we can only rewind to the most
  // recent mark.
  assert (position == marks.top ());
  read_position = position;
}

token
token_stream::get_unfiltered_token ()
{
  // If we're in the buffer, read from it.
  if (read_position < buffer_end)
    return buffer[read_position++];

  // Ask the lexer, and buffer the result if needed.
  token r = lexer::get_token ();
  if (mark_buffering || peek_buffering)
    {
      if (buffer == NULL)
	{
	  buffer_size = 64;
	  buffer = new token[buffer_size];
	}
      else if (buffer_end == buffer_size)
	{
	  buffer_size *= 2;
	  token *nb = new token[buffer_size];
	  for (int i = 0; i < buffer_end; ++i)
	    nb[i] = buffer[i];
	  delete [] buffer;
	  buffer = nb;
	}

      buffer[buffer_end] = r;
      ++buffer_end;
      ++read_position;
    }

  return r;
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
  saver<bool> save_peek (peek_buffering, true);
  saver<int> save_read (read_position);
  token r = get_token ();
  return r;
}

token
token_stream::peek_token1 ()
{
  saver<bool> save_jd (javadoc_is_ok);
  saver<bool> save_peek (peek_buffering, true);
  saver<int> save_read (read_position);
  get_token ();
  token r = get_token ();
  return r;
}
