// Formatting values.

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

std::string classpath_double_to_string (jdouble, jboolean);
bool classpath_string_to_double (const char *, jdouble *);

std::string
primitive_format (const jboolean &val)
{
  return std::string (val ? "true" : "false");
}

std::string
primitive_format (const jbyte &val)
{
  char buffer[100];
  sprintf (buffer, "%d", (int) val);
  return std::string (buffer);
}

std::string
primitive_format (const jchar &val)
{
  char buffer[4];
  int offset = ucs2_to_utf8 (buffer, val);
  buffer[offset] = '\0';
  return std::string (buffer);
}

std::string
primitive_format (const jshort &val)
{
  char buffer[100];
  sprintf (buffer, "%d", (int) val);
  return std::string (buffer);
}

std::string
primitive_format (const jint &val)
{
  char buffer[100];
  sprintf (buffer, "%d", (int) val);
  return std::string (buffer);
}

std::string
primitive_format (const jlong &val)
{
  char buffer[100];
  // FIXME: portability
  sprintf (buffer, "%lld", (long long) val);
  return std::string (buffer);
}

std::string
primitive_format (const jfloat &val)
{
  return classpath_double_to_string (val, true);
}

std::string
primitive_format (const jdouble &val)
{
  return classpath_double_to_string (val, false);
}

bool
string_to_double (const char *buf, jdouble &result)
{
  return classpath_string_to_double (buf, &result);
}

bool
string_to_float (const char *buf, jfloat &result)
{
  jdouble rd;
  bool v = classpath_string_to_double (buf, &rd);
  result = rd;

  // Rounding checks for double->float conversion.
  if (std::isinf (result) && ! std::isinf (rd))
    return false;
  if (result == 0 && rd != 0)
    return false;

  return v;
}
