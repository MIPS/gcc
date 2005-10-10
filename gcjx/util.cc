// Utility functions.

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

#include <fstream>



// Sorted list of C++ keywords.
// FIXME: doesn't include some things that G++ might consider as
// keywords.
static const char *const keywords[] =
{
  "and",
  "and_eq",
  "asm",
  "auto",
  "bitand",
  "bitor",
  "bool",
  "break",
  "case",
  "catch",
  "char",
  "class",
  "compl",
  "const",
  "const_cast",
  "continue",
  "default",
  "delete",
  "do",
  "double",
  "dynamic_cast",
  "else",
  "enum",
  "explicit",
  "export",
  "extern",
  "false",
  "float",
  "for",
  "friend",
  "goto",
  "if",
  "inline",
  "int",
  "long",
  "mutable",
  "namespace",
  "new",
  "not",
  "not_eq",
  "operator",
  "or",
  "or_eq",
  "private",
  "protected",
  "public",
  "register",
  "reinterpret_cast",
  "return",
  "short",
  "signed",
  "sizeof",
  "static",
  "static_cast",
  "struct",
  "switch",
  "template",
  "this",      
  "throw",
  "true",
  "try",
  "typedef",
  "typeid",
  "typename",
  "typeof",
  "union",
  "unsigned",
  "using",
  "virtual",
  "void",
  "volatile",
  "wchar_t",
  "while",
  "xor",
  "xor_eq"
};

#define NUM_KEYWORDS  (sizeof (keywords) / sizeof (keywords[0]))



std::list<std::string>
drop_last_element (const std::list<std::string> &in)
{
  std::list<std::string> out;
  std::list<std::string>::const_iterator last = in.end ();
  if (last != in.begin ())
    {
      --last;
      out = std::list<std::string> (in.begin (), last);
    }
  return out;
}

void
replace_all (std::string &s, char from, char to)
{
  for (std::string::iterator it = s.begin (); it != s.end (); ++it)
    {
      if (*it == from) *it = to;
    }
}

std::string
join (const std::list<std::string> &id, char c)
{
  std::string result;
  bool first = true;
  for (std::list<std::string>::const_iterator i = id.begin ();
       i != id.end ();
       ++i)
    {
      if (! first)
	result += c;
      result += *i;
      first = false;
    }
  return result;
}

std::list<std::string>
split (const std::string &name, char c)
{
  std::list<std::string> split;

  std::string::size_type old = 0;
  for (std::string::size_type dot = name.find (c);
       dot != name.npos;
       dot = name.find (c, old))
    {
      split.push_back (name.substr (old, dot - old));
      old = dot + 1;
    }
  split.push_back (name.substr (old));

  return split;
}

int
ucs2_to_utf8 (char *buffer, unicode_w_t c)
{
  int offset = 0;
  // Java-style UTF-8, so \0 is treated as a 2-byte sequence.
  if (c >= 0x0001 && c <= 0x007f)
    buffer[offset++] = (char) c;
  else if (c == 0 || (c >= 0x0080 && c <= 0x07ff))
    {
      buffer[offset++] = (char) (0xc0 | ((0x7c0 & c) >> 6));
      buffer[offset++] = (char) (0x80 | ( 0x03f & c));
    }
  else
    {
      buffer[offset++] = (char) (0xe0 | ((0xf000 & c) >> 12));
      buffer[offset++] = (char) (0x80 | ((0x0fc0 & c) >>  6));
      buffer[offset++] = (char) (0x80 | ( 0x003f & c));
    }
  return offset;
}

void
maybe_swap (jint &x, jint &y)
{
#ifndef WORDS_BIGENDIAN
  jint z = x;
  x = y;
  y = z;
#endif
}

static bool
read_file (std::ifstream &in, std::string &result)
{
  char c;
  bool quote = false;
  char buf[50];
  int where = 0;

  // Skip whitespace here.
  in >> c;
  if (! in.good ())
    return true;
  if (c == '"')
    {
      quote = true;
      c = in.get ();
      if (! in.good ())
	return false;
    }

  do
    {
      if (quote && c == '"')
	break;
      else if (! quote && (c == ' ' || c == '\t' || c == '\r' || c == '\n'))
	break;
      buf[where++] = c;
      if (where == sizeof (buf) - 1)
	{
	  buf[where] = '\0';
	  result += buf;
	  where = 0;
	}
      c = in.get ();
    }
  while (in.good ());

  if (where > 0)
    {
      buf[where] = '\0';
      result += buf;
    }

  // Return false if we read a quoted empty string.
  return ! quote || ! result.empty ();
}

void
read_at_file (const char *filename, std::list<std::string> &results)
{
  std::ifstream in (filename);
  if (! in.is_open ())
    throw io_error (LOCATION_UNKNOWN, "couldn't open file %1") % filename;

  location where (filename);
  while (in.good ())
    {
      std::string fname;
      if (! read_file (in, fname))
        throw io_error (where, "empty quoted file name in %<@%> file");
      if (! fname.empty ())
        results.push_back (fname);
    }
}

bool
jar_file_p (const std::string &name)
{
  int name_len = name.length ();

  // FIXME:
  //   1. Fix this for case-insensitive file systems.
  //   2. Ensure that name actually represents an existing file...
  //   3. ...and not a folder, like "libjava.jar" in the GCJ testsuite.
  return (name_len > 4
          && (name.compare (name_len - 4, 4, ".jar") == 0
              || name.compare (name_len - 4, 4, ".zip") == 0));
}

bool
class_file_p (const std::string &name)
{
  int name_len = name.length ();

  // FIXME:
  //   1. Fix this for case-insensitive file systems.
  //   2. Ensure that name actually represents an existing file
  return name_len > 6 && name.compare (name_len - 6, 6, ".class") == 0;
}

bool
java_file_p (const std::string &name)
{
  int name_len = name.length ();

  // FIXME:
  //   1. Fix this for case-insensitive file systems.
  //   2. Ensure that name actually represents an existing file6
  return name_len > 5 && name.compare (name_len - 5, 5, ".java") == 0;
}

bool
cxx_keyword_p (const std::string &name)
{
  // Strip '$'s off the end.
  int i;
  for (i = name.length () - 1; i >= 0 && name[i] == '$'; --i)
    ;
  std::string newname = name.substr (0, i + 1);

  int low = 0;
  int high = NUM_KEYWORDS;
  int last = -1;

  while (true)
    {
      int current = (low + high) / 2;
      if (current == last)
	break;
      int cmp = newname.compare (keywords[current]);
      if (cmp == 0)
	return true;
      else if (cmp > 0)
	low = current;
      else
	high = current;
      last = current;
    }

  return false;
}
