// Exceptions thrown by lexer, parser, etc.

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

#ifndef GCJX_EXCEPTION_HH
#define GCJX_EXCEPTION_HH

#include "format/format.hh"
#include "source/lex.hh"

class exception_base
{
protected:

  // The formatter for this exception.
  format formatter;

public:

  exception_base (location w, const char *fmt)
    : formatter (new format_repr (format_error, w, fmt))
  {
  }

  virtual ~exception_base ()
  {
  }

  void set_location (const location &w)
  {
    formatter->set_location (w);
  }

  /// Set the lexer to use to retrieve lines from source files.
  void set_lexer (lexer *src_lexer)
  {
    formatter->set_lexer (src_lexer);
  }

  friend std::ostream &operator<< (std::ostream &, const exception_base &);
};

inline std::ostream &
operator<< (std::ostream &os, const exception_base &exc)
{
  return os << exc.formatter;
}

template<int selector>
class exception_tmpl : public exception_base
{
public:

  exception_tmpl (location w, const char *fmt)
    : exception_base (w, fmt)
  {
  }

  template<int s, typename T>
  friend const exception_tmpl<s> &operator% (const exception_tmpl<s> &,
					     const T &);
};

template<int selector, typename T>
inline const exception_tmpl<selector> &
operator% (const exception_tmpl<selector> &exc, const T &val)
{
  exc.formatter % val;
  return exc;
}

// Invalid character sequence, etc.
typedef exception_tmpl<0> conversion_error;

// Error during lexical analysis.
typedef exception_tmpl<1> lex_error;

// Syntax errors.
typedef exception_tmpl<2> syntax_error;

// Errors which occur during semantic analysis.
typedef exception_tmpl<3> semantic_error;

// Error thrown when a type name is found to be ambiguous.
typedef exception_tmpl<4> ambiguous_type_error;

// Error thrown when a type is not found.
typedef exception_tmpl<5> type_not_found_error;

// Generic type errors.
typedef exception_tmpl<6> other_type_error;

// Duplicate declarations.
// FIXME should have list of other locations
typedef exception_tmpl<7> duplicate_declaration_error;

// Error in .class file.
typedef exception_tmpl<8> class_file_error;

// General-purpose I/O error, used for JAR and @-file problems.
typedef exception_tmpl<9> io_error;

// Error in command-line arguments.
typedef exception_tmpl<10> command_line_error;

#endif // GCJX_EXCEPTION_HH
