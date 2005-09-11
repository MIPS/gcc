// Formatting.

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

#ifndef GCJX_FORMAT_FORMAT_HH
#define GCJX_FORMAT_FORMAT_HH

#include "location.hh"
#include "owner.hh"

typedef enum
{
  format_warning = WARN,
  format_error = ERROR
} format_type;

/// Converts the given value into a string containing its hex
/// representation including the leading "0x".  Use this to
/// construct a suitable argument to operator% if you want to
/// include a hex number in formatted output.
template <typename T>
std::string to_hex_string (const T &value)
{
  std::ostringstream oss;
  oss << std::hex << std::showbase << value;
  return oss.str ();
}

/// A format object is used to format error and warning strings for
/// printing.  It works a bit like printf(), but is specialized for
/// the compiler.  Like Boost's formatter, it uses overloaded
/// operator% to specify arguments.
class format_repr
{
  /// Reference count.
  int refc;

  /// Location of this error.
  location where;

  /// The formatting string passed in by the user.
  const char *plan;

  /// The number of substitutions in the plan.
  size_t subst_count;

  /// Arguments passed in by the user.
  std::vector<std::string> args;

  /// Type of this message.
  format_type type;

  // Add a new string argument to the format.
  void add (const std::string &);

  // Implements operator <<.
  std::string get_message () const;
  void dump (std::ostream &) const;
  void report_bad_plan (char ch) const;

  // Returns a locale-dependent open-quote string ("'" in ASCII).
  const char *open_quote () const;
  // Returns a locale-dependent close-quote string ("'" in ASCII).
  const char *close_quote () const;

  friend const owner<format_repr> &operator% (const owner<format_repr> &,
					      const std::string &);
  friend const owner<format_repr> &operator% (const owner<format_repr> &,
					      const char *);
  friend const owner<format_repr> &operator% (const owner<format_repr> &,
					      char);
  friend std::ostream &operator<< (std::ostream &, const owner<format_repr> &);

  // For reference count access.
  template<typename T> friend class owner;

public:

  /// Create a new formatter given the location of the error and the
  /// formatting plan.
  format_repr (format_type, location, const char *);

  virtual ~format_repr ()
  {
    delete[] plan;
  }

  void set_location (const location &w)
  {
    where = w;
  }
};

typedef owner<format_repr> format;

const format &operator% (const format &fmt, const std::string &str);

const format &operator% (const format &fmt, const char *str);

const format &operator% (const format &fmt, char val);

const format &operator% (const format &fmt, int val);

const format &operator% (const format &fmt, size_t val);

template<typename T>
const format &operator% (const format &fmt, const owner<T> &ref);

const format &operator% (const format &fmt, const std::list<std::string> &l);

std::ostream &operator<< (std::ostream &, const format &);

#endif // GCJX_FORMAT_FORMAT_HH
