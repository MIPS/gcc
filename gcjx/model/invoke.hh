// Method invocation in various forms.

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

#ifndef GCJX_MODEL_INVOKE_HH
#define GCJX_MODEL_INVOKE_HH

class model_invocation_base : public model_expression
{
protected:

  // The method.
  model_method *method;

  // The name of the method.
  std::string name;

  // The arguments.
  std::list<ref_expression> arguments;

  // The 'this' expression.
  ref_expression expression;

  // We might need to create a new phony method, in which case this
  // object owns it.
  ref_method phony;

  // This is the class that should be considered the qualifying class
  // for this invocation.  This is needed to properly implement binary
  // compatibility.
  model_class *search;

  // True if this is an unqualified method invocation.
  bool unqualified;

  void try_method_conversion (const std::set<model_method *> &,
			      const std::list<model_type *> &,
			      std::set<model_method *> &);
  void search_applicable_methods (resolution_scope *,
				  model_class *,
				  const std::list<model_type *> &,
				  std::set<model_method *> &);

  // This returns the class or interface we must search to find the
  // method named NAME, as well as the qualifying class or interface.
  virtual void determine_search_class (resolution_scope *,
				       model_class **,
				       model_class **) = 0;

  // Helper that finds the actual method.
  void find_method (resolution_scope *);

  bool compute_constant_p ()
  {
    return false;
  }

  virtual bool super_call_p () const
  {
    return false;
  }

  // A helper function that handles most of the steps of resolution.
  void handle_resolve (resolution_scope *, bool = false);

  model_invocation_base (const location &w)
    : model_expression (w),
      method (NULL),
      search (NULL),
      unqualified (false)
  {
  }

public:

  void set_method (const std::string &mname)
  {
    name = mname;
  }

  void set_arguments (const std::list<ref_expression> &args)
  {
    arguments = args;
  }

  void set_expression (const ref_expression &e)
  {
    expression = e;
  }

  /// Return the qualifying expression, or NULL if there wasn't one.
  model_expression *get_expression () const
  {
    return expression.get ();
  }

  void set_unqualified ()
  {
    unqualified = true;
  }

  model_class *get_qualifying_class () const;

  void resolve (resolution_scope *);
};

class model_method_invocation : public model_invocation_base
{
  void determine_search_class (resolution_scope *, model_class **,
			       model_class **);

public:

  model_method_invocation (const location &w)
    : model_invocation_base (w)
  {
  }

  model_method_invocation (const location &w,
			   const std::list<ref_expression> &args)
    : model_invocation_base (w)
  {
    set_arguments (args);
  }

  void visit (visitor *);
};

class model_type_qualified_invocation : public model_invocation_base
{
  // The class to search.
  ref_forwarding_type klass;

  // If true, this is a 'super' call.
  bool super;

  void determine_search_class (resolution_scope *, model_class **,
			       model_class **);

protected:

  bool super_call_p () const
  {
    return super;
  }

public:

  model_type_qualified_invocation (const location &w)
    : model_invocation_base (w),
      super (false)
  {
  }

  void set_super_call ()
  {
    super = true;
  }

  void set_class (const ref_forwarding_type &k)
  {
    klass = k;
  }

  void visit (visitor *);

  void resolve (resolution_scope *);
};

/// It might seem strange that this is an expression, since it is only
/// used in a statement context, and since it always has 'void' type.
/// It is simpler to have it be an invocation, however, in order to
/// reuse the method lookup code.
class model_super_invocation : public model_invocation_base
{
  // If not NULL, the lexically enclosing class.
  model_class *enclosing;

  // Any constructor with a 'super' call must initialize the local
  // fields after the superclass is initialized.  It is simplest for
  // us to have this call be considered as part of the 'super' call.
  // This can be NULL if this class does not require such a call.
  ref_expression finit;

  // True if this is a synthetic 'super' call, false if it appears in
  // the source code.
  bool synthetic;

  void determine_search_class (resolution_scope *, model_class **,
			       model_class **);

public:

  model_super_invocation (const location &w)
    : model_invocation_base (w),
      enclosing (NULL),
      synthetic (false)
  {
    name = "<init>";
  }

  void set_enclosing_class (model_class *n)
  {
    enclosing = n;
  }

  void set_synthetic ()
  {
    synthetic = true;
  }

  void resolve (resolution_scope *);

  void visit (visitor *);
};

/// See comments on model_super_invocation to understand why this is
/// an expression and not a statement.
class model_this_invocation : public model_invocation_base
{
  // If not NULL, the lexically enclosing class.
  model_class *enclosing;

  void determine_search_class (resolution_scope *, model_class **,
			       model_class **);

public:

  model_this_invocation (const location &w)
    : model_invocation_base (w),
      enclosing (NULL)
  {
    name = "<init>";
  }

  void set_enclosing_class (model_class *n)
  {
    enclosing = n;
  }

  void resolve (resolution_scope *);

  void visit (visitor *);
};

/// This is used to represent a generic method invocation whose type
/// arguments are explicitly specified.  Since such invocations are
/// much less common than other invocations, we introduce a subclass
/// of every other method invocation class which is holds the actual
/// type parameters.  Note that this is not only done for expressions;
/// this template is also used with model_memberref_forward.
template<typename T>
class model_generic_invocation : public T
{
  /// The actual type parameters.
  std::list<ref_forwarding_type> actual_type_params;

public:

  model_generic_invocation (const location &,
			    const std::list<ref_forwarding_type> &);
};

/// These classes are used to represent explicitly-parameterized
/// invocations of generic methods.
typedef class model_generic_invocation<model_type_qualified_invocation>
  model_generic_type_qualified_invocation;
typedef class model_generic_invocation<model_super_invocation>
  model_generic_super_invocation;
typedef class model_generic_invocation<model_this_invocation>
  model_generic_this_invocation;

#endif // GCJX_MODEL_INVOKE_HH
