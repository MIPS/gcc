// Access control.

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

static model_class *
unwrap_raw_class (model_class *k)
{
  model_raw_class *raw = dynamic_cast<model_raw_class *> (k);
  if (raw)
    k = raw->get_parent ();
  else
    {
      model_class_instance *classi = dynamic_cast<model_class_instance *> (k);
      if (classi)
	k = classi->get_parent ();
    }
  return k;
}

bool
accessible_p (model_type *t, IContext *request)
{
  // An array type is accessible if and only if its element type is.
  while (t->array_p ())
    t = t->element_type ();

  if (t->primitive_p ())
    return true;
  model_class *klass = unwrap_raw_class (assert_cast<model_class *> (t));

  if (! klass->member_p ())
    {
      // Top-level type is accessible if it is public or if the
      // request comes from the same package.  A top-level type
      // cannot be private or protected.
      modifier_t flags = klass->get_modifiers ();
      assert (! (flags & (ACC_PROTECTED | ACC_PRIVATE)));
      if ((flags & ACC_PUBLIC) != 0)
	return true;
      return klass->get_package () == request->get_package ();
    }

  // We have a member type.  First ensure that the enclosing type is
  // accessible.
  if (! accessible_p (klass->get_declaring_class (), request))
    return false;

  modifier_t flags = klass->get_modifiers ();
  if ((flags & ACC_PUBLIC) != 0)
    return true;

  if ((flags & ACC_PROTECTED) != 0)
    {
      // Protected access is allowed within the body of a subclass of
      // the declarer.  Note the initial cast will return NULL when
      // REQUEST is a package.
      model_class *declarer = klass->get_declaring_class ();
      for (model_class *prot
	     = unwrap_raw_class (dynamic_cast<model_class *> (request));
	   prot;
	   prot = prot->get_lexically_enclosing_class ())
	{
	  if (declarer->assignable_from_p (prot))
	    return true;
	}
    }

  if ((flags & ACC_PRIVATE) != 0)
    {
      // Access is permitted only within the body of the top-level
      // class enclosing the declaration.
      klass = klass->get_top_level_class ();
      model_class *req_class
	= unwrap_raw_class (dynamic_cast<model_class *> (request));
      while (req_class != NULL && req_class != klass)
	req_class = req_class->get_lexically_enclosing_class ();
      return req_class == klass;
    }

  // Default access, but also one case of protected access.
  return klass->get_package () == request->get_package ();
}

// FIXME: perhaps this should be on IContext.
static model_class *
get_top_level_class (IContext *ctx)
{
  model_class *r;
  // Cast will return NULL if CTX is not itself a class.
  for (r = dynamic_cast<model_class *> (ctx);
       r && r->get_lexically_enclosing_class () != NULL;
       r = r->get_lexically_enclosing_class ())
    ;
  return r;
}

bool
accessible_p (IMember *target, IContext *request, model_class *qualifier,
	      model_class *declarer)
{
  // If there is no declarer, use the member's declaring class.
  if (declarer == NULL)
    declarer = target->get_declaring_class ();

  // Simple check for access to a top-level class.
  if (declarer == NULL)
    {
      model_class *tclass = assert_cast<model_class *> (target);
      return accessible_p (tclass, request);
    }

  if (dynamic_cast<model_class *> (request) != NULL)
    request = unwrap_raw_class (assert_cast<model_class *> (request));
  declarer = unwrap_raw_class (declarer);

  // Make sure we can access the declaring class.
  if (! accessible_p (declarer, request))
    return false;

  // An IMember must always also be IModifiable.  We don't enforce
  // this via inheritance, a design buglet.
  IModifiable *mod = assert_cast<IModifiable *> (target);
  modifier_t flags = mod->get_modifiers ();

  // Public access is always ok.
  if ((flags & ACC_PUBLIC) != 0)
    return true;

  // Access is always ok from the body of the top-level class.
  if (get_top_level_class (declarer) == get_top_level_class (request))
    return true;

  if ((flags & ACC_PROTECTED) != 0)
    {
      // Protected access is allowed within the body of a subclass of
      // the declarer.  Note that this cast will fail when REQUEST is
      // a package; that is ok because there are still other checks we
      // can make.  We set DECLARER here since we have already checked
      // our "implicitly declaring" class for access, and protected
      // access is determined in terms of the actual declaring class.
      declarer = target->get_declaring_class ();
      for (model_class *prot = dynamic_cast<model_class *> (request);
	   prot;
	   prot = prot->get_lexically_enclosing_class ())
	{
	  if (declarer->assignable_from_p (prot))
	    {
	      // Qualified references to instance fields or instance
	      // methods must also satisfy the constraint that the
	      // qualifier be a subclass of the class from which the
	      // reference was made.
	      if (qualifier
		  && (flags & ACC_STATIC) == 0
		  && dynamic_cast<model_class *> (target) == NULL
		  && ! prot->assignable_from_p (qualifier))
		{
		  // Continue here, since there may be other reasons
		  // that this member is accessible.  FIXME?
		  continue;
		}
	      return true;
	    }
	}
    }

  if ((flags & ACC_PRIVATE) != 0)
    {
      // The only cases were handled above.
      return false;
    }

  // Default access or protected access.
  return declarer->get_package () == request->get_package ();
}

bool
trampoline_required_p (const IMember *target, model_class *request,
		       model_class **result)
{
  assert (result);
  request = unwrap_raw_class (request);

  // Access to the same class is fine.
  model_class *tclass = target->get_declaring_class ();
  if (request == tclass)
    return false;

  const IModifiable *mod = assert_cast<const IModifiable *> (target);
  modifier_t mods = mod->get_modifiers ();
  // Private access to another class requires a trampoline.
  if ((mods & ACC_PRIVATE) != 0)
    {
      *result = target->get_declaring_class ();
      return true;
    }

  // Protected access needs a special case if the member's declaring
  // class comes from another package and is not visible via
  // inheritance.
  if ((mods & ACC_PROTECTED) != 0)
    {
      if (tclass->get_package () == request->get_package ()
	  || tclass->assignable_from_p (request))
	return false;
      for (request = request->get_lexically_enclosing_class ();
	   request && ! tclass->assignable_from_p (request);
	   request = request->get_lexically_enclosing_class ())
	;
      assert (request);
      *result = request;
      return true;
    }

  // Other cases do not require a trampoline.
  return false;
}
