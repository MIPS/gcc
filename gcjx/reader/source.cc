// Read .java files from a byte stream.

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
#include "source/parse.hh"
#include "reader/source.hh"

source_file_creator::~source_file_creator ()
{
  delete the_reader;
}

void
source_file_creator::apply (bool emit)
{
  if (global->get_compiler ()->verbose ())
    std::cout << "[parsing " << file
	      << (emit ? " (will generate code)" : "")
	      << "]"
	      << std::endl;

  // FIXME we could just make these things on the stack with some API
  // changes here and there.
  // FIXME: perhaps creating a new reader will throw an exception.
  ucs2_reader *r
    = create_ucs2_reader (the_reader->read_all (),
			  global->get_compiler ()->get_encoding ());
  token_stream *ts = new token_stream (r, file.c_str ());
  parse p (ts);
  try
    {
      ref_unit unit = p.compilation_unit ();
      unit->set_file_name (file);
      unit->set_lexer (ts);
      global->get_compiler ()->add_unit (unit, emit);
    }
  catch (exception_base &exc)
    {
      exc.set_lexer (ts);
      std::cerr << exc;
    }
  // fixme propagate the exceptions on up?
  // should at least indicate if this compilation failed so we
  // don't try reading the file again
}
