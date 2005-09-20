// Create a class from a byte source.

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
#include "bytecode/classreader.hh"
#include "reader/classbytes.hh"

class_byte_creator::~class_byte_creator ()
{
  delete the_reader;
}

void
class_byte_creator::apply (bool emit)
{
  if (global->get_compiler ()->verbose ())
    std::cout << "  + " << file << std::endl;

  try
    {
      class_reader parser (the_reader->read_all ());
      ref_unit unit = parser.parse (file);
      global->get_compiler ()->add_unit (unit, emit);
    }
  catch (class_file_error &cfe)
    {
      // Note that the exception prints a newline for us, and includes
      // the location.
      std::cerr << global->get_compiler ()->get_name () << ": " << cfe;
    }
}
