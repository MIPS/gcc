// Targets for 'break' and 'continue'.

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

#ifndef GCJX_MODEL_ILABEL_HH
#define GCJX_MODEL_ILABEL_HH

class model_loop;
class model_stmt;

/// This class represents an abstract label.  It is conceptually a
/// kind of statement that can be the target of a "break" or
/// "continue".
class Ilabel
{
public:

  virtual ~Ilabel ()
  {
  }

  /// See whether this label matches the arguments.
  /// @param kind  the token value of the requesting statement, either
  ///              TOKEN_BREAK or TOKEN_CONTINUE.
  /// @param name   the requested label name; in the case of a plain
  ///               "break;" or "continue;", it will be the empty string.
  /// @return true if this label matches, false otherwise
  virtual bool match_label_p (token_value kind, const std::string &name) = 0;

  /// Find the "continue" target corresponding to this object.
  /// @return a pointer to the statement, or NULL if this isn't a
  /// loop statement.
  virtual model_loop *get_continue_target () = 0;

  /// Find the "break" target corresponding to this object.
  /// @return a pointer to the statement, or NULL if this isn't a
  /// loop statement.
  virtual model_stmt *get_break_target () = 0;

  /// Indicate that a break statement that targets this label is
  /// reachable.
  virtual void break_reached () = 0;
};

#endif // GCJX_MODEL_ILABEL_HH
