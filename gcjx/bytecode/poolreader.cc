// Parsing bytes representing a constant pool.

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
#include "bytecode/cpool.hh"
#include "bytecode/poolreader.hh"

constant_pool_reader::constant_pool_reader (const location &w,
					    const uint8 *b, int l)
  : where (w),
    bytes (b),
    length (l),
    ptr (b)
{
}

class_file_error
constant_pool_reader::error (const char *fmt) const
{
  return class_file_error (where, fmt);
}

uint8
constant_pool_reader::read_u1 ()
{
  if (ptr - bytes + 1 > length)
    throw error ("unexpected end of file");
  return *ptr++;
}

uint16
constant_pool_reader::read_u2 ()
{
  if (ptr - bytes + 2 > length)
    throw error ("unexpected end of file");
  uint8 hi = *ptr++;
  uint8 lo = *ptr++;

  return (hi << 8) | lo;
}

uint32
constant_pool_reader::read_u4 ()
{
  if (ptr - bytes + 4 > length)
    throw error ("unexpected end of file");
  uint8 one = *ptr++;
  uint8 two = *ptr++;
  uint8 three = *ptr++;
  uint8 four = *ptr++;

  return (one << 24) | (two << 16) | (three << 8) | four;
}

constant_pool *
constant_pool_reader::parse (const uint8 **out_pointer)
{
  uint16 count = read_u2 ();

  uint8 *tags = new uint8[count];
  jword *data = new jword[count];
  std::string *utf_bytes = new std::string[count];

  try
    {
      // FIXME: should validate the constant pool as well.
      for (int i = 1; i < count; ++i)
	{
	  tags[i] = read_u1 ();
	  switch (tags[i])
	    {
	    case CONSTANT_Class:
	    case CONSTANT_String:
	      data[i] = read_u2 ();
	      break;

	    case CONSTANT_Integer:
	    case CONSTANT_Float:
	    case CONSTANT_Fieldref:
	    case CONSTANT_Methodref:
	    case CONSTANT_InterfaceMethodref:
	    case CONSTANT_NameAndType:
	      data[i] = read_u4 ();
	      break;

	    case CONSTANT_Long:
	    case CONSTANT_Double:
	      data[i++] = read_u4 ();
	      if (i >= count)
	        throw error ("too much data in constant pool");
	      // FIXME: set tags[i] --- to what?
	      data[i] = read_u4 ();
	      break;

	    case CONSTANT_Utf8:
	      {
		uint16 len = read_u2 ();
		// FIXME: length checking
		utf_bytes[i] = std::string ((char *) ptr, len);
		ptr += len;
	      }
	      break;

	    default:
	      throw error ("unrecognized constant pool tag %1")
		% tags[i];
	    }
	}
    }
  catch (...)
    {
      delete[] tags;
      delete[] data;
      delete[] utf_bytes;

      throw;
    }

  *out_pointer = ptr;
  return new constant_pool (where, count, tags, data, utf_bytes);
}
