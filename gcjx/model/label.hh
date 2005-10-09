// Represent a label.

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

#ifndef GCJX_MODEL_LABEL_HH
#define GCJX_MODEL_LABEL_HH

/// Represents a label statement.  This holds the name of the label
/// and a contained statement.
class model_label : public model_stmt, public Ilabel
{
  // The name of the label.
  std::string name;

  // The contained statement.
  ref_stmt statement;

  // True if a reachable break statement was found.
  bool break_found;

  // True if this label was ever used.
  bool label_used;

public:

  model_label (const location &w)
    : model_stmt (w),
      break_found (false),
      label_used (false)
  {
  }

  void resolve (resolution_scope *);

  void set_name (const std::string &n)
  {
    name = n;
  }

  std::string get_name () const
  {
    return name;
  }

  void set_statement (const ref_stmt &s)
  {
    statement = s;
  }

  void break_reached ()
  {
    break_found = true;
  }

  bool match_label_p (token_value, const std::string &n)
  {
    // If the name matches, we say yes.  If this label does not wrap a
    // loop, and the branch kind is 'continue', we'll catch that
    // during semantic analysis.
    return name == n;
  }

  model_loop *get_continue_target ();

  model_stmt *get_break_target ()
  {
    // A label might directly wrap a loop.
    label_used = true;
    return statement->get_break_target ();
  }

  void compute_normal_completion (normal_completion_state &);

  void visit (visitor *v);
};

#endif // GCJX_MODEL_LABEL_HH
