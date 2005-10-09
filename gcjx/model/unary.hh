// Represent a unary operator.

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

#ifndef GCJX_MODEL_UNARY_HH
#define GCJX_MODEL_UNARY_HH

/// This is the base class for all unary expressions.  It is never
/// directly instantiated, only subclassed.
class model_unary : public model_expression
{
protected:

  // Expression.
  ref_expression expr;

  // Convenience method.
  void ensure_variable (const ref_expression &);

  model_unary (const location &w)
    : model_expression (w)
  {
  }

  model_unary (const location &w, const ref_expression &e)
    : model_expression (w),
      expr (e)
  {
  }

  virtual std::string get_name () const = 0;

public:

  void set_expression (const ref_expression &e)
  {
    expr = e;
  }

  // This is for arithmetic operations only; logical operations must
  // override.
  void resolve (resolution_scope *scope);
};



/// This template is used for a number of different "simple" prefix
/// operators, such as unary "+".
template<unary_function OP, predicate P, char NAME>
class model_prefix_simple : public model_unary
{
  bool compute_constant_p ()
  {
    return expr->constant_p ();
  }

  std::string get_name () const;

public:

  model_prefix_simple (const location &w)
    : model_unary (w)
  {
  }

  void resolve (resolution_scope *);

  jvalue value ()
  {
    model_primitive_base *bt = assert_cast<model_primitive_base *> (type ());
    jvalue val = bt->convert (expr->type (), expr->value ());
    return (bt->*OP) (val);
  }

  void visit (visitor *);
};

typedef model_prefix_simple<&model_primitive_base::plus,
			    &model_type::numeric_p,
			    '+'>
  model_prefix_plus;
typedef model_prefix_simple<&model_primitive_base::minus,
			    &model_type::numeric_p,
			    '-'>
  model_prefix_minus;
typedef model_prefix_simple<&model_primitive_base::bnot,
			    &model_type::integral_p,
			    '~'>
  model_bitwise_not;



/// This represents the prefix "!" operator.
class model_logical_not
  : public model_prefix_simple<&model_primitive_base::lnot,
			       &model_type::boolean_p,
			       '!'>
{
public:

  model_logical_not (const location &w)
    : model_prefix_simple<&model_primitive_base::lnot,
			  &model_type::boolean_p,
			  '!'> (w)
  {
  }

  void resolve (resolution_scope *scope);

  void visit (visitor *);
};



/// This template handles the side-effecting prefix operators, "++"
/// and "--".
template<char NAME>
class model_prefix_side_effect : public model_unary
{
  std::string get_name () const;

  bool compute_constant_p ()
  {
    return false;
  }

public:

  model_prefix_side_effect (const location &w)
    : model_unary (w)
  {
  }

  void resolve (resolution_scope *);

  void visit (visitor *);
};

typedef model_prefix_side_effect<'+'> model_prefix_plusplus;
typedef model_prefix_side_effect<'-'> model_prefix_minusminus;



/// This template handles the side-effecting postfix operators, "++"
/// and "--".
template<char NAME>
class model_postfix : public model_unary
{
  std::string get_name () const;

  bool compute_constant_p ()
  {
    return false;
  }

public:

  model_postfix (const location &w, const ref_expression &e)
    : model_unary (w, e)
  {
  }

  void resolve (resolution_scope *);

  void visit (visitor *);
};

typedef model_postfix<'+'> model_postfix_plusplus;
typedef model_postfix<'-'> model_postfix_minusminus;

#endif // GCJX_MODEL_UNARY_HH
