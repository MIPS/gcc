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

#ifndef GCJX_UTIL_HH
#define GCJX_UTIL_HH

#include <assert.h>

#include <cstdlib>
#include <cstring>

class model_type;

/// Copies the contents of the given C-style string into dynamically
/// allocated memory and returns a pointer to it.
inline char*
copy_str (const char* str)
{
  int len = strlen (str);
  char* n_str = new char[len+ 1];
  strcpy (n_str, str);
  n_str[len] = '\0';
  return n_str;
}

inline std::string
get_simple_name (const std::list<std::string> &id)
{
  return id.back ();
}

std::list<std::string> drop_last_element (const std::list<std::string> &);

void replace_all (std::string &s, char from, char to);
std::string join (const std::list<std::string> &id, char c);
std::list<std::string> split (const std::string &str, char c);

inline std::string operator+ (const std::string &str, int val)
{
  char buffer[20];
  sprintf (buffer, "%d", val);
  return str + buffer;
}

inline std::string operator+ (const std::string &str, char val)
{
  std::string result (str);
  result.push_back (val);
  return result;
}

inline bool
simple_name_p (const std::list<std::string> &n)
{
  std::list<std::string>::const_iterator i = n.begin ();
  if (i == n.end ())
    return false;
  ++i;
  return i == n.end ();
}

int ucs2_to_utf8 (char *buffer, unicode_w_t c);

template<typename T>
class stack_temporary
{
  // Stack on which to push.
  std::deque<T> &the_stack;

  // True if we pushed.
  bool pushed;

public:

  stack_temporary (std::deque<T> &t, const T &item)
    : the_stack (t),
      pushed (true)
  {
    the_stack.push_back (item);
  }

  stack_temporary (std::deque<T> &t)
    : the_stack (t),
      pushed (false)
  {
  }

  ~stack_temporary ()
  {
    if (pushed)
      the_stack.pop_back ();
  }

  // this is sort of bogus, perhaps we should
  // just add explicit code where it is needed;
  // currently only in the bytecode generator.
  void push (const T &item)
  {
    assert (! pushed);
    the_stack.push_back (item);
    pushed = true;
  }
};

void maybe_swap (jint &, jint &);

/// Read an "@" file as specified on the command line.
/// On success puts file names in RESULTS.
/// On failure throws an io_error.
void read_at_file (const char *filename, std::list<std::string> &results);

/// Return true if the string names a jar or zip file, false
/// otherwise.
bool jar_file_p (const std::string &);

/// Return true if the string names a class file.
bool class_file_p (const std::string &);

/// Return true if the string names a java source file.
bool java_file_p (const std::string &);

#endif // GCJX_UTIL_HH
