// Represent an annotation type.

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

#include "typedefs.hh"

model_annotation_type::model_annotation_type (const location &w)
  : model_class (w)
{
  set_interface ();
}

void
model_annotation_type::add_annotation_member (const ref_annotation_member &mem)
{
  if (anno_members.find (mem->get_name ()) != anno_members.end ())
    {
      std::cerr << mem->error ("duplicate annotation member %1")
	% mem->get_name ();
      std::cerr << anno_members[mem->get_name ()]->error ("original member"
							  " is here");
    }
  else
    anno_members[mem->get_name ()] = mem;

  // Each annotation member is also an interface method.
  ref_method meth = new model_method (mem->get_location (), this);
  meth->set_name (mem->get_name ());
  meth->set_return_type (mem->get_forwarding_type ());
  meth->set_modifiers (mem->get_modifiers ());
  add (meth);
}

void
model_annotation_type::resolve_hook (resolution_scope *scope)
{
  model_class *annot
    = global->get_compiler ()->java_lang_annotation_Annotation ();

  assert (interfaces.empty ());

  location w = get_location ();
  ref_forwarding_type anno = new model_forwarding_resolved (w, annot);
  std::list<ref_forwarding_type> ifaces;
  set_implements (ifaces);

  for (member_type::const_iterator i = anno_members.begin ();
       i != anno_members.end ();
       ++i)
    (*i).second->resolve (scope);
}

void
model_annotation_type::check_completeness (const std::list<ref_annotation_value> &args,
					   model_element *request)
{
  // We need our annotations resolved before we can proceed.
  resolve_classes ();

  std::set<std::string> seen;

  for (std::list<ref_annotation_value>::const_iterator i = args.begin ();
       i != args.end ();
       ++i)
    {
      std::string name = (*i)->get_name ();
      if (seen.find (name) != seen.end ())
	// FIXME: original location.
	throw (*i)->error ("annotation named %1 already seen")
	  % name;
      seen.insert (name);

      member_type::const_iterator rec = anno_members.find (name);
      if (rec == anno_members.end ())
	throw (*i)->error ("no annotation member named %1 in %2")
	  % name % this;

      if (! annotation_commensurate_p ((*rec).second->get_type (),
				       (*i)->get_value ()))
	throw (*i)->error ("value not commensurate with annotation "
			   "member's type");
    }

  for (member_type::const_iterator i = anno_members.begin ();
       i != anno_members.end ();
       ++i)
    {
      // If we didn't see it, and there isn't a default, error.
      if (! (*i).second->has_value_p ()
	  && seen.find ((*i).first) == seen.end ())
	throw request->error ("annotation member %1 not given a value")
	  % (*i).first;
    }
}

bool
model_annotation_type::element_compatible_p (annotation_kind kind)
{
  if (! valid_elements.set_p ())
    {
      model_annotation *targ
	= find_annotation (global->get_compiler ()->java_lang_annotation_Target ());
      int value = 0;
      if (targ == NULL)
	{
	  // Default allows everything.
	  value = (ANNOTATE_LOCAL | ANNOTATE_PARAMETER | ANNOTATE_FIELD
		   | ANNOTATE_METHOD | ANNOTATE_TYPE | ANNOTATE_ANNOTATION
		   | ANNOTATE_CONSTRUCTOR | ANNOTATE_PACKAGE);
	}
      else
	{
	  // Make sure the (meta-)annotation expressions are all
	  // resolved at this point.
	  resolve_members ();

	  model_annotation_value *val = targ->find_annotation ("value");
	  assert (val);
	  model_expression *expr = val->get_value ();

	  std::list<ref_expression> exprs;
	  // FIXME: inefficient, use a helper function instead.
	  if (dynamic_cast<model_memberref_forward *> (expr))
	    exprs.push_back (expr);
	  else
	    exprs = assert_cast<model_annotation_initializer *> (expr)->get_initializers ();

	  // FIXME: some of the asserts here are not really correct.
	  for (std::list<ref_expression>::const_iterator i = exprs.begin ();
	       i != exprs.end ();
	       ++i)
	    {
	      model_expression *e = (*i).get ();
	      model_memberref_base *b
		= assert_cast<model_memberref_base *> (e);
	      e = b->get_real ();
	      model_field_ref *fr = assert_cast<model_field_ref *> (e);

	      std::string name = fr->get_field ()->get_name ();
	      int bit;
	      if (name == "LOCAL_VARIABLE")
		bit = ANNOTATE_LOCAL;
	      else if (name == "PARAMETER")
		bit = ANNOTATE_PARAMETER;
	      else if (name == "FIELD")
		bit = ANNOTATE_FIELD;
	      else if (name == "METHOD")
		bit = ANNOTATE_METHOD;
	      else if (name == "TYPE")
		bit = ANNOTATE_TYPE;
	      else if (name == "ANNOTATION_TYPE")
		bit = ANNOTATE_ANNOTATION;
	      else if (name == "CONSTRUCTOR")
		bit = ANNOTATE_CONSTRUCTOR;
	      else
		{
		  assert (name == "PACKAGE");
		  bit = ANNOTATE_PACKAGE;
		}

	      if ((value & bit) != 0)
		std::cerr << error ("annotation target % appears "
				    "more than once")
		  % name;
	      value |= bit;
	    }

	  if ((value & ANNOTATE_TYPE) != 0)
	    value |= ANNOTATE_ANNOTATION;
	}
      valid_elements = value;
    }

  return (int (valid_elements) & kind) != 0;
}

annotation_retention
model_annotation_type::get_retention_policy ()
{
  if (! policy.set_p ())
    {
      // FIXME: some assertions here should be errors instead.
      model_annotation *p
	= find_annotation (global->get_compiler ()->java_lang_annotation_Retention ());

      if (p == NULL)
	{
	  policy = RETAIN_CLASS;
	  return annotation_retention (policy);
	}

      model_annotation_value *val = p->find_annotation ("value");
      assert (val);
      model_expression *expr = val->get_value ();
      model_memberref_forward *mem
	= assert_cast<model_memberref_forward *> (expr);
      model_field_ref *fr = assert_cast<model_field_ref *> (mem->get_real ());
      std::string name = fr->get_field ()->get_name ();
      if (name == "SOURCE")
	policy = RETAIN_SOURCE;
      else if (name == "CLASS")
	policy = RETAIN_CLASS;
      else
	{
	  assert (name == "RUNTIME");
	  policy = RETAIN_RUNTIME;
	}
    }

  return annotation_retention (policy);
}

void
model_annotation_type::visit (visitor *v)
{
  v->visit_annotation_type (this, descriptor, name);
}
