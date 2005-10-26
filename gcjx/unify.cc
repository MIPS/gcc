// Type unification.

// Copyright (C) 2005 Free Software Foundation, Inc.
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

#include "typedefs.hh"


// U << V : U convertible to V by method invoc. conv.

/// This class implements the type inference algorithm as explained in
/// the JLS 3.  Names in this class are general chosen to follow the
/// JLS.  Reading the text is strongly advised, this code is not
/// intended to be easy to follow without having it alongside.
class unifier
{
  typedef std::list< std::pair<model_class *, model_class *> > constraint_list;

  typedef std::map< model_class *, std::set<model_class_instance *> >
    inv_map_type;

  // Inferred constraints; indexed by constraint_type.
  constraint_list constraints[3];

  std::map<model_class *, model_class *> mapping;

  // The formal type parameters for the method.
  std::set<model_type_variable *> formal_type_params;

  // This is used to avoid memory leaks when creating temporary
  // wildcards and other objects during unification.
  std::list<ref_element> gcpro;

  // Location we should use when creating things.
  location where;

  typedef enum
    {
      LESS_THAN = 0,
      EQUAL = 1,
      GREATER_THAN = 2
    } constraint_type;

  static constraint_type invert (constraint_type t)
  {
    if (t == LESS_THAN)
      return GREATER_THAN;
    if (t == GREATER_THAN)
      return LESS_THAN;
    return EQUAL;
  }

  void imply (constraint_type type, model_class *formal, model_class *actual)
  {
    constraints[type].push_back (std::make_pair (formal, actual));
  }

  bool formal_type_variable_p (model_class *klass)
  {
    model_type_variable *tv = dynamic_cast<model_type_variable *> (klass);
    return (tv != NULL
	    && formal_type_params.find (tv) != formal_type_params.end ());
  }

  // Compute the supertype set and the erased supertype set.
  void compute_supertype_sets (model_class *klass,
			       std::set<model_class *> &st,
			       std::set<model_class *> &est)
  {
    while (klass != NULL)
      {
	st.insert (klass);
	est.insert (assert_cast<model_class *> (klass->erasure ()));
	std::list<ref_forwarding_type> ifaces (klass->get_interfaces ());
	for (std::list<ref_forwarding_type>::const_iterator i
	       = ifaces.begin ();
	     i != ifaces.end ();
	     ++i)
	  compute_supertype_sets (assert_cast<model_class *> ((*i)->type ()),
				  st, est);
	klass = klass->get_superclass ();
      }
  }

  // Compute the erased candidate set and the complete supertype set.
  void compute_ec (const std::set<model_class *> &types,
		   std::set<model_class *> &ec,
		   std::set<model_class *> &st)
  {
    bool first = true;
    for (std::set<model_class *>::const_iterator i = types.begin ();
	 i != types.end ();
	 ++i)
      {
	std::set<model_class *> inter, newset;
	compute_supertype_sets (*i, st, first ? ec : inter);
	if (! first)
	  {
	    std::set_intersection (ec.begin (), ec.end (),
				   inter.begin (), inter.end (),
				   std::inserter (newset, newset.begin ()));
	    ec = newset;
	  }
	first = false;
      }
  }

  // Computed the minimal erased candidate set.
  void compute_mec (const std::set<model_class *> &ec,
		    std::set<model_class *> &mec)
  {
    for (std::set<model_class *>::const_iterator i = ec.begin ();
	 i != ec.end ();
	 ++i)
      {
	bool found = false;
	for (std::set<model_class *>::const_iterator j = ec.begin ();
	     j != ec.end ();
	     ++j)
	  {
	    // Don't compare to self.
	    if (*i == *j)
	      continue;
	    if ((*i)->assignable_from_p (*j))
	      {
		found = true;
		break;
	      }
	  }
	if (! found)
	  mec.insert (*i);
      }
  }

  // Compute the invocation sets, given a set of input types.
  void compute_inv (const std::set<model_class *> &input_types,
		    inv_map_type &invocation_map)
  {
    std::set<model_class *> ec, st, mec;
    compute_ec (input_types, ec, st);
    compute_mec (ec, mec);

    for (std::set<model_class *>::const_iterator i = mec.begin ();
	 i != mec.end ();
	 ++i)
      {
	std::set<model_class_instance *> one_inv;

	// Note that if the type is not generic, then it will simply
	// map to an empty set.  This is ok -- see the definition of
	// the Candidate() function in the JLS.  This situation is
	// recognized in compute_lub.
	if (! (*i)->get_type_parameters ().empty ())
	  {
	    for (std::set<model_class *>::const_iterator j = st.begin ();
		 j != st.end ();
		 ++j)
	      {
		model_class_instance *ci
		  = dynamic_cast<model_class_instance *> (*j);
		if (ci != NULL && ci->get_parent () == *i)
		  one_inv.insert (ci);
	      }
	  }

	invocation_map[*i] = one_inv;
      }
  }

  model_class *compute_glb (model_class *left, model_class *right)
  {
    std::list<model_class *> classes;
    classes.push_back (left);
    classes.push_back (right);
    return new model_intersection_type (where, classes);
  }

  // Compute the least containing type argument for a pair of classes.
  model_class *compute_lcta (model_class *left, model_class *right)
  {
    model_class *result;
    if (left->wildcard_p () && right->wildcard_p ())
      {
	model_wildcard *leftw = assert_cast<model_wildcard *> (left);
	model_wildcard *rightw = assert_cast<model_wildcard *> (right);
	model_class *lbound = leftw->get_bound ();
	model_class *rbound = rightw->get_bound ();

	if (leftw->super_p ())
	  {
	    assert (rightw->super_p ()); // FIXME
	    result = new model_wildcard (where, compute_glb (lbound, rbound),
					 true);
	  }
	else if (rightw->super_p ())
	  {
	    if (lbound == rbound)
	      result = lbound;
	    else
	      result = new model_wildcard (where);
	  }
	else
	  result = new model_wildcard (where, compute_lub (lbound, rbound));
      }
    else if (right->wildcard_p ())
      {
	model_wildcard *rw = assert_cast<model_wildcard *> (right);
	model_class *rbound = rw->get_bound ();
	model_class *new_bound;
	if (rw->super_p ())
	  new_bound = compute_glb (left, rbound);
	else
	  new_bound = compute_lub (left, rbound);
	result = new model_wildcard (where, new_bound, rw->super_p ());
	gcpro.push_back (result);
      }
    else if (left == right)
      result = left;
    else
      {
	model_class *lub = compute_lub (left, right);
	result = new model_wildcard (where, lub);
	gcpro.push_back (result);
      }
    return result;
  }

  // Compute the least containing invocation given an invocation set.
  model_class *compute_lci (const std::set<model_class_instance *> &inv)
  {
    assert (! inv.empty ());
    model_class *outer = NULL;
    std::list<model_class *> current;
    for (std::set<model_class_instance *>::const_iterator i = inv.begin ();
	 i != inv.end ();
	 ++i)
      {
	model_class_instance *ci = *i;
	if (outer == NULL)
	  {
	    // First time through.
	    outer = ci->get_parent ();
	    ci->get_type_map (current);
	    continue;
	  }

	assert (outer == ci->get_parent ());

	std::list<model_class *> ci_params;
	ci->get_type_map (ci_params);

	std::list<model_class *> nextparams;
	std::list<model_class *>::const_iterator it1 = current.begin ();
	std::list<model_class *>::const_iterator it2 = ci_params.begin ();
	while (it1 != current.end ())
	  {
	    nextparams.push_back (compute_lcta (*it1, *it2));
	    ++it1;
	    ++it2;
	  }
	assert (it2 == ci_params.end ());

	current = nextparams;
      }

    assert (outer != NULL);
    return outer->create_instance (outer /* FIXME */, current);
  }

  // This name comes from the JLS.
  model_class *compute_lub (const std::set<model_class *> &constraints)
  {
    inv_map_type inv_map;
    compute_inv (constraints, inv_map);

    model_class *result = NULL;

    for (inv_map_type::const_iterator i = inv_map.begin ();
	 i != inv_map.end ();
	 ++i)
      {
	model_class *arg = (*i).first;
	const std::set<model_class_instance *> &inv = (*i).second;
	if (! arg->get_type_parameters ().empty ())
	  arg = compute_lci (inv);
	// FIXME: this is wrong, we need to compute a bound.
	result = arg;
      }

    return result;
  }

  model_class *conforming_array_type (model_class *actual)
  {
    if (actual->array_p ())
      {
	// Maybe.
      }
    // FIXME: is the erasure correct here?
    // We actually want the upper bound.
    else if (actual->erasure ()->array_p ())
      actual = assert_cast<model_class *> (actual->erasure ());
    else
      return NULL;
    actual = assert_cast<model_class *> (actual->element_type ());
    return actual->reference_p () ? actual : NULL;
  }

  void unify (constraint_type constraint, model_type *actual_in,
	      model_class *formal)
  {
    if (actual_in == null_type)
      {
	// Nothing to do.
	return;
      }

    if (actual_in->primitive_p ())
      {
	if (constraint == LESS_THAN)
	  actual_in = boxing_conversion (actual_in);
	else
	  {
	    // Nothing to do.
	    return;
	  }
      }

    model_class *actual;
    actual = assert_cast<model_class *> (actual_in);

    // Note that we could see a type variable here that is not one of
    // the formal variables of the method in question, for instance if
    // this method is in a generic class.
    if (formal_type_variable_p (formal))
      {
	imply (invert (constraint), formal, actual);
	return;
      }

    model_class *elt;
    if (formal->array_p () && (elt = conforming_array_type (actual)))
      {
	unify (constraint, elt,
	       assert_cast<model_class *> (formal->element_type ()));
	return;
      }

    if (! formal->parameterized_p ())
      {
	// No constraint implied.
	return;
      }
    model_class_instance *formalci
      = assert_cast<model_class_instance *> (formal);
    model_class_instance *actualci
      = assert_cast<model_class_instance *> (actual);

    // FIXME: check that ACTUAL "inherits from FORMAL's erasure".
    // For '<' case only.

    std::list<model_class *> formal_map, actual_map;
    formalci->get_type_map (formal_map);
    actualci->get_type_map (actual_map);

    std::list<model_class *>::const_iterator i_f = formal_map.begin ();
    std::list<model_class *>::const_iterator i_a = actual_map.begin ();
    while (i_f != formal_map.end ())
      {
	model_class *inner_f = *i_f;
	model_wildcard *inner_f_w = dynamic_cast<model_wildcard *> (inner_f);
	model_class *inner_a = *i_a;
	model_wildcard *inner_a_w = dynamic_cast<model_wildcard *> (inner_a);

	if (! inner_f->wildcard_p ())
	  {
	    if (constraint == GREATER_THAN)
	      {
		// FIXME.
	      }
	    else
	      unify (EQUAL, inner_a, inner_f);
	  }
	else if (inner_f_w->super_p ())
	  {
	    if (inner_a->wildcard_p ())
	      {
		if (inner_a_w->super_p ())
		  unify (GREATER_THAN, inner_a, inner_f);
	      }
	    else
	      unify (GREATER_THAN, inner_a, inner_f);
	  }
	else if (inner_f_w->has_bound_p ())
	  {
	    // 'extends' wildcard.
	    if (inner_a->wildcard_p ())
	      {
		if (! inner_a_w->super_p () && inner_a_w->has_bound_p ())
		  unify (LESS_THAN, inner_a, inner_f);
	      }
	    else
	      unify (LESS_THAN, inner_a, inner_f);
	  }

	++i_a;
	++i_f;
      }
    assert (i_a == actual_map.end ());
  }

  model_class *maybe_map (const std::map<model_class *, model_class *> &themap,
			 model_class *type)
  {
    std::map<model_class *, model_class *>::const_iterator i
      = themap.find (type);
    if (i == themap.end ())
      return type;
    return (*i).second;
  }

  void update_map (std::map<model_class *, model_class *> &themap,
		   model_class *from, model_class *to)
  {
    // FIXME: what if we have an existing OLD->FROM mapping?
    // FIXME: and what if we create a loop?
    themap[from] = to;
  }

  void consider_equality ()
  {
    constraint_list &eq = constraints[EQUAL];
    for (constraint_list::const_iterator i = eq.begin ();
	 i != eq.end ();
	 ++i)
      {
	model_class *left = maybe_map (mapping, (*i).first);
	model_class *right = maybe_map (mapping, (*i).second);

	// Ignore identities.
	if (left == right)
	  continue;

	if (formal_type_variable_p (right))
	  std::swap (left, right);
	// Due to mapping we might see two non-type variables here.
	// That is an error as it means there are inconsistent
	// constraints.
	if (! formal_type_variable_p (left))
	  abort ();		// FIXME

	// If both happen to be type variables, either mapping will
	// do.
	update_map (mapping, left, right);
      }
  }

  bool mapping_complete_p ()
  {
    for (std::set<model_type_variable *>::const_iterator i
	   = formal_type_params.begin ();
	 i != formal_type_params.end ();
	 ++i)
      {
	if (mapping.find (*i) == mapping.end ())
	  return false;
      }
    return true;
  }

  void update_constraint_set (constraint_type type,
			      model_type_variable *var,
			      std::set<model_class *> &result)
  {
    for (constraint_list::const_iterator i = constraints[type].begin ();
	 i != constraints[type].end ();
	 ++i)
      {
	model_class *first = (*i).first;
	model_class *second = (*i).second;
	result.insert (first == var ? second : first);
      }
  }

  void resolve_constraints (model_type_map &result)
  {
    consider_equality ();
    for (std::set<model_type_variable *>::const_iterator i
	   = formal_type_params.begin ();
	 i != formal_type_params.end ();
	 ++i)
      {
	std::map<model_class *, model_class *>::const_iterator mp
	  = mapping.find (*i);
	if (mp != mapping.end ())
	  {
	    // FIXME: should we check the other constraints too?
	    result.add (*i, (*mp).second);
	  }
	else
	  {
	    std::set<model_class *> constraints;
	    update_constraint_set (LESS_THAN, *i, constraints);
	    update_constraint_set (GREATER_THAN, *i, constraints);
	    result.add (*i, compute_lub (constraints));
	  }
      }
  }

  void get_formal_argument_types (model_method *method,
				  std::list<model_type *> &formal)
  {
    std::list<ref_variable_decl> formal_v = method->get_parameters ();
    for (std::list<ref_variable_decl>::const_iterator i = formal_v.begin ();
	 i != formal_v.end ();
	 ++i)
      formal.push_back ((*i)->type ());
  }

  void get_formal_type_parameters (model_method *method)
  {
    const model_parameters &params = method->get_type_parameters ();
    for (std::list<ref_type_variable>::const_iterator i = params.begin ();
	 i != params.end ();
	 ++i)
      formal_type_params.insert ((*i).get ());
    assert (! formal_type_params.empty ());
  }

public:

  unifier (const location &w)
    : where (w)
  {
  }

  void unify (const std::list<model_type *> &actual, model_method *method,
	      model_type_map &result)
  {
    std::list<model_type *> formal;
    get_formal_argument_types (method, formal);
    get_formal_type_parameters (method);

    std::list<model_type *>::const_iterator ai = actual.begin ();
    std::list<model_type *>::const_iterator fi = formal.begin ();

    // FIXED_FORMAL is used when processing a varargs call.
    model_type *fixed_formal = NULL;

    while (ai != actual.end ()
	   && (fixed_formal || fi != formal.end ()))
      {
	model_type *ft;
	model_type *at = *ai++;

	if (fixed_formal)
	  ft = fixed_formal;
	else
	  {
	    ft = *fi++;
	    if (method->varargs_p () && fi == formal.end ())
	      {
		// The type of the last formal argument must be an
		// array type.  Every subsequent actual argument must
		// now be unified against the element type of the
		// array.
		fixed_formal = ft->element_type ();
		ft = fixed_formal;
	      }
	  }

	// It doesn't make sense to unify against a formal argument
	// with primitive type (?).
	if (! ft->primitive_p ())
	  unify (LESS_THAN, at, assert_cast<model_class *> (ft));
      }

    resolve_constraints (result);
  }

  model_class *compute_lub (model_class *one, model_class *two)
  {
    std::set<model_class *> constraints;
    constraints.insert (one);
    constraints.insert (two);
    return compute_lub (constraints);
  }
};

void
unify (const std::list<model_type *> &actual,
       model_method *method,
       model_class *assignment_type,
       model_type_map &result)
{
  // FIXME: correct location.
  unifier u (method->get_location ());
  u.unify (actual, method, result);
}

model_class *
compute_lub (model_element *request, model_class *one, model_class *two)
{
  unifier u (request->get_location ());
  return u.compute_lub (one, two);
}
