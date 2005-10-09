// Variable declaration statements.

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

#ifndef GCJX_MODEL_VARSTMT_HH
#define GCJX_MODEL_VARSTMT_HH

/// This represents a variable declaration statement as it would
/// appear in a method body.  It holds a list of individual variable
/// declarations.
class model_variable_stmt : public model_stmt
{
protected:

  // The variable declarations.
  std::list<ref_variable_decl> vars;

public:

  model_variable_stmt (const location &w)
    : model_stmt (w)
  {
  }

  void set_variables (const std::list<ref_variable_decl> &l)
  {
    vars = l;
  }

  void set_final ()
  {
    for (std::list<ref_variable_decl>::const_iterator i = vars.begin ();
	 i != vars.end ();
	 ++i)
      (*i)->set_final ();
  }

  void set_annotations (const std::list<ref_annotation> &annos)
  {
    for (std::list<ref_variable_decl>::const_iterator i = vars.begin ();
	 i != vars.end ();
	 ++i)
      (*i)->set_annotations (annos);
  }

  void resolve (resolution_scope *);

  void compute_normal_completion (normal_completion_state &)
  {
    normal_completion = true;
  }

  void visit (visitor *v);
};

#endif // GCJX_MODEL_VARSTMT_HH
