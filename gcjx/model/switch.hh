// Represent a switch statement.

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

#ifndef GCJX_MODEL_SWITCH_HH
#define GCJX_MODEL_SWITCH_HH

#include <algorithm>

/// This represents a single "switch block" -- a group of 'case'
/// statements which share the same code.  It holds the switch labels
/// and a list of corresponding statements.
class model_switch_block : public model_element
{
  // The switch labels for this block.
  std::list<ref_expression> labels;

  // The statements for this block.
  std::list<ref_stmt> statements;

  // True if we can complete normally.
  watch<bool> normal_completion;

public:

  model_switch_block (const location &w)
    : model_element (w)
  {
  }

  void add (const ref_expression &expr)
  {
    labels.push_back (expr);
  }

  std::list<ref_expression> get_labels () const
  {
    return labels;
  }

  void set_statements (const std::list<ref_stmt> &ss)
  {
    statements = ss;
  }

  std::list<ref_stmt> get_statements () const
  {
    return statements;
  }

  void resolve (resolution_scope *, model_type *,
		std::map<jint, model_expression *> &, bool);

  void compute_normal_completion (normal_completion_state &state)
  {
    normal_completion = ::compute_normal_completion (state, statements);
  }

  bool can_complete_normally () const
  {
    return normal_completion;
  }

  void visit (visitor *);
};

typedef owner<model_switch_block> ref_switch_block;

/// This represents a 'switch' statement.  It holds the switch
/// expression, a list of switch blocks making up the body of the
/// statement, and a pointer to the 'default' block, if any.
class model_switch : public model_stmt, public Ilabel
{
  // The switch expression;
  ref_expression expression;

  // Blocks making up the switch.
  std::list<ref_switch_block> blocks;

  // True if a reachable break statement was found.
  bool break_found;

  // Points to the default block, NULL if no default.
  model_switch_block *default_block;

public:

  model_switch (const location &w)
    : model_stmt (w),
      break_found (false),
      default_block (NULL)
  {
  }

  void set_expression (const ref_expression &expr)
  {
    expression = expr;
  }

  void resolve (resolution_scope *);

  void add (const ref_switch_block &b)
  {
    blocks.push_back (b);
  }

  void set_default (model_switch_block *b)
  {
    if (default_block != NULL)
      {
	std::cerr << b->error ("duplicate default case");
	std::cerr << default_block->error ("original declaration is here");
      }
    default_block = b;
  }

  model_switch_block *get_default () const
  {
    return default_block;
  }

  void break_reached ()
  {
    break_found = true;
  }

  model_loop *get_continue_target ()
  {
    return NULL;
  }

  model_stmt *get_break_target ()
  {
    return this;
  }

  bool match_label_p (token_value kind, const std::string &n)
  {
    // An anonymous break will leave a switch statement.
    return kind == TOKEN_BREAK && n.empty ();
  }

  void compute_normal_completion (normal_completion_state &);

  void visit (visitor *v);
};

#endif // GCJX_MODEL_SWITCH_HH
