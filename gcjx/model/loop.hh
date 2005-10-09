// Represent a loop of some kind.

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

#ifndef GCJX_MODEL_LOOP_HH
#define GCJX_MODEL_LOOP_HH

/// This is the base class for the looping statements -- for, do, and
/// while.  It is never instantiated, only subclassed.
class model_loop : public model_stmt, public Ilabel
{
protected:

  // True if a reachable continue statement was found.
  bool continue_found;

  // True if a reachable break statement was found.
  bool break_found;

  model_loop (const location &w)
    : model_stmt (w),
      continue_found (false),
      break_found (false)
  {
  }

public:

  void continue_reached ()
  {
    continue_found = true;
  }

  void break_reached ()
  {
    break_found = true;
  }

  bool match_label_p (token_value /*kind*/, const std::string &n)
  {
    // An anonymous break or continue is ok.
    return n.empty ();
  }

  model_loop *get_continue_target ()
  {
    // This can point to the loop, not the precise sub-statement, if
    // there is one, of the continue target.
    return this;
  }

  model_stmt *get_break_target ()
  {
    // This can point to the loop, not the precise sub-statement, if
    // there is one, of the break target.
    return this;
  }
};

#endif // GCJX_MODEL_LOOP_HH
