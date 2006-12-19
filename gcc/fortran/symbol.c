/* Maintain binary trees of symbols.
   Copyright (C) 2000, 2001, 2002, 2003, 2004, 2005, 2006 Free Software
   Foundation, Inc.
   Contributed by Andy Vaught

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 2, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */


#include "config.h"
#include "system.h"
#include "flags.h"
#include "gfortran.h"
#include "parse.h"


/* Strings for all symbol attributes.  We use these for dumping the
   parse tree, in error messages, and also when reading and writing
   modules.  */

const mstring flavors[] =
{
  minit ("UNKNOWN-FL", FL_UNKNOWN), minit ("PROGRAM", FL_PROGRAM),
  minit ("BLOCK-DATA", FL_BLOCK_DATA), minit ("MODULE", FL_MODULE),
  minit ("VARIABLE", FL_VARIABLE), minit ("PARAMETER", FL_PARAMETER),
  minit ("LABEL", FL_LABEL), minit ("PROCEDURE", FL_PROCEDURE),
  minit ("DERIVED", FL_DERIVED), minit ("NAMELIST", FL_NAMELIST),
  minit (NULL, -1)
};

const mstring procedures[] =
{
    minit ("UNKNOWN-PROC", PROC_UNKNOWN),
    minit ("MODULE-PROC", PROC_MODULE),
    minit ("INTERNAL-PROC", PROC_INTERNAL),
    minit ("DUMMY-PROC", PROC_DUMMY),
    minit ("INTRINSIC-PROC", PROC_INTRINSIC),
    minit ("EXTERNAL-PROC", PROC_EXTERNAL),
    minit ("STATEMENT-PROC", PROC_ST_FUNCTION),
    minit (NULL, -1)
};

const mstring intents[] =
{
    minit ("UNKNOWN-INTENT", INTENT_UNKNOWN),
    minit ("IN", INTENT_IN),
    minit ("OUT", INTENT_OUT),
    minit ("INOUT", INTENT_INOUT),
    minit (NULL, -1)
};

const mstring access_types[] =
{
    minit ("UNKNOWN-ACCESS", ACCESS_UNKNOWN),
    minit ("PUBLIC", ACCESS_PUBLIC),
    minit ("PRIVATE", ACCESS_PRIVATE),
    minit (NULL, -1)
};

const mstring ifsrc_types[] =
{
    minit ("UNKNOWN", IFSRC_UNKNOWN),
    minit ("DECL", IFSRC_DECL),
    minit ("BODY", IFSRC_IFBODY),
    minit ("USAGE", IFSRC_USAGE)
};


/* This is to make sure the backend generates setup code in the correct
   order.  */

static int next_dummy_order = 1;


gfc_namespace *gfc_current_ns;

gfc_gsymbol *gfc_gsym_root = NULL;

static gfc_symbol *changed_syms = NULL;


/*********** IMPLICIT NONE and IMPLICIT statement handlers ***********/

/* The following static variable indicates whether a particular element has
   been explicitly set or not.  */

static int new_flag[GFC_LETTERS];


/* Handle a correctly parsed IMPLICIT NONE.  */

void
gfc_set_implicit_none (void)
{
  int i;

  if (gfc_current_ns->seen_implicit_none)
    {
      gfc_error ("Duplicate IMPLICIT NONE statement at %C");
      return;
    }

  gfc_current_ns->seen_implicit_none = 1;

  for (i = 0; i < GFC_LETTERS; i++)
    {
      gfc_clear_ts (&gfc_current_ns->default_type[i]);
      gfc_current_ns->set_flag[i] = 1;
    }
}


/* Reset the implicit range flags.  */

void
gfc_clear_new_implicit (void)
{
  int i;

  for (i = 0; i < GFC_LETTERS; i++)
    new_flag[i] = 0;
}


/* Prepare for a new implicit range.  Sets flags in new_flag[].  */

try
gfc_add_new_implicit_range (int c1, int c2)
{
  int i;

  c1 -= 'a';
  c2 -= 'a';

  for (i = c1; i <= c2; i++)
    {
      if (new_flag[i])
	{
	  gfc_error ("Letter '%c' already set in IMPLICIT statement at %C",
		     i + 'A');
	  return FAILURE;
	}

      new_flag[i] = 1;
    }

  return SUCCESS;
}


/* Add a matched implicit range for gfc_set_implicit().  Check if merging
   the new implicit types back into the existing types will work.  */

try
gfc_merge_new_implicit (gfc_typespec * ts)
{
  int i;

  if (gfc_current_ns->seen_implicit_none)
    {
      gfc_error ("Cannot specify IMPLICIT at %C after IMPLICIT NONE");
      return FAILURE;
    }

  for (i = 0; i < GFC_LETTERS; i++)
    {
      if (new_flag[i])
	{

	  if (gfc_current_ns->set_flag[i])
	    {
	      gfc_error ("Letter %c already has an IMPLICIT type at %C",
			 i + 'A');
	      return FAILURE;
	    }
	  gfc_current_ns->default_type[i] = *ts;
	  gfc_current_ns->set_flag[i] = 1;
	}
    }
  return SUCCESS;
}


/* Given a symbol, return a pointer to the typespec for its default type.  */

gfc_typespec *
gfc_get_default_type (gfc_symbol * sym, gfc_namespace * ns)
{
  char letter;

  letter = sym->name[0];
  if (letter < 'a' || letter > 'z')
    gfc_internal_error ("gfc_get_default_type(): Bad symbol");

  if (ns == NULL)
    ns = gfc_current_ns;

  return &ns->default_type[letter - 'a'];
}


/* Given a pointer to a symbol, set its type according to the first
   letter of its name.  Fails if the letter in question has no default
   type.  */

try
gfc_set_default_type (gfc_symbol * sym, int error_flag, gfc_namespace * ns)
{
  gfc_typespec *ts;

  if (sym->ts.type != BT_UNKNOWN)
    gfc_internal_error ("gfc_set_default_type(): symbol already has a type");

  ts = gfc_get_default_type (sym, ns);

  if (ts->type == BT_UNKNOWN)
    {
      if (error_flag && !sym->attr.untyped)
	{
	  gfc_error ("Symbol '%s' at %L has no IMPLICIT type",
		     sym->name, &sym->declared_at);
	  sym->attr.untyped = 1; /* Ensure we only give an error once.  */
	}

      return FAILURE;
    }

  sym->ts = *ts;
  sym->attr.implicit_type = 1;

  if(sym->attr.is_bind_c == 1)
  {
     /* bind(c) variables can not be implicitly declared */
     gfc_error ("Variable '%s' at %L can not be implicitly declared "
                "because it is bind(c)", sym->name, &sym->declared_at);
  }

  return SUCCESS;
}


/******************** Symbol attribute stuff *********************/

/* This is a generic conflict-checker.  We do this to avoid having a
   single conflict in two places.  */

#define conf(a, b) if (attr->a && attr->b) { a1 = a; a2 = b; goto conflict; }
#define conf2(a) if (attr->a) { a2 = a; goto conflict; }
#define conf_std(a, b, std) if (attr->a && attr->b)\
                              {\
                                a1 = a;\
                                a2 = b;\
                                standard = std;\
                                goto conflict_std;\
                              }

static try
check_conflict (symbol_attribute * attr, const char * name, locus * where)
{
  static const char *dummy = "DUMMY", *save = "SAVE", *pointer = "POINTER",
    *target = "TARGET", *external = "EXTERNAL", *intent = "INTENT",
    *intent_in = "INTENT(IN)", *intrinsic = "INTRINSIC",
    *allocatable = "ALLOCATABLE", *elemental = "ELEMENTAL",
    *private = "PRIVATE", *recursive = "RECURSIVE",
    *in_common = "COMMON", *result = "RESULT", *in_namelist = "NAMELIST",
    *public = "PUBLIC", *optional = "OPTIONAL", *entry = "ENTRY",
    *function = "FUNCTION", *subroutine = "SUBROUTINE",
    *dimension = "DIMENSION", *in_equivalence = "EQUIVALENCE",
    *use_assoc = "USE ASSOCIATED", *cray_pointer = "CRAY POINTER",
    *cray_pointee = "CRAY POINTEE", *data = "DATA", *is_bind_c = "BIND(C)",
    *value = "VALUE", *volatile_ = "VOLATILE",
    *threadprivate = "THREADPRIVATE";

  const char *a1, *a2;
  int standard;

  if (where == NULL)
    where = &gfc_current_locus;

  if (attr->pointer && attr->intent != INTENT_UNKNOWN)
    {
      a1 = pointer;
      a2 = intent;
      goto conflict;
    }

  if(attr->value)
  {
     if(attr->intent != INTENT_UNKNOWN && attr->intent != INTENT_IN)
     {
        gfc_error("Invalid INTENT for symbol with VALUE attribute "
                  "at %L", where);
        return FAILURE;
     }/* end if(invalid intent for by value symbol) */
  }/* end if(by value with known intent) */
  
  /* Check for attributes not allowed in a BLOCK DATA.  */
  if (gfc_current_state () == COMP_BLOCK_DATA)
    {
      a1 = NULL;

      if (attr->in_namelist)
	a1 = in_namelist;
      if (attr->allocatable)
	a1 = allocatable;
      if (attr->external)
	a1 = external;
      if (attr->optional)
	a1 = optional;
      if (attr->access == ACCESS_PRIVATE)
	a1 = private;
      if (attr->access == ACCESS_PUBLIC)
	a1 = public;
      if (attr->intent != INTENT_UNKNOWN)
	a1 = intent;

      if (a1 != NULL)
	{
	  gfc_error
	    ("%s attribute not allowed in BLOCK DATA program unit at %L", a1,
	     where);
	  return FAILURE;
	}
    }

  conf (dummy, entry);
  conf (dummy, intrinsic);
  conf (dummy, save);
  conf (dummy, threadprivate);
  conf (pointer, target);
  if(attr->in_proc_decl != 1)
  conf (pointer, external);
  conf (pointer, intrinsic);
  conf (pointer, elemental);
  conf (allocatable, elemental);

  conf (target, external);
  conf (target, intrinsic);
  conf (external, dimension);   /* See Fortran 95's R504.  */

  conf (external, intrinsic);
    
  if (attr->if_source || attr->contained)
    {
      conf (external, subroutine);
      conf (external, function);
    }

  conf (allocatable, pointer);
  conf_std (allocatable, dummy, GFC_STD_F2003);
  conf_std (allocatable, function, GFC_STD_F2003);
  conf_std (allocatable, result, GFC_STD_F2003);
  conf (elemental, recursive);

  conf (in_common, dummy);
  conf (in_common, allocatable);
  conf (in_common, result);
  conf (in_common, save);
  conf (result, save);

  conf (dummy, result);

  conf (in_equivalence, use_assoc);
  conf (in_equivalence, dummy);
  conf (in_equivalence, target);
  conf (in_equivalence, pointer);
  conf (in_equivalence, function);
  conf (in_equivalence, result);
  conf (in_equivalence, entry);
  conf (in_equivalence, allocatable);
  conf (in_equivalence, threadprivate);

  conf (in_namelist, pointer);
  conf (in_namelist, allocatable);

  conf (entry, result);

  conf (function, subroutine);

  /* value for intent(inout) or intent(out) is error; caught above */
  conf (is_bind_c, dummy);
  conf (is_bind_c, cray_pointer);
  conf (is_bind_c, cray_pointee);
  conf (is_bind_c, allocatable);
  conf (value, allocatable);
  conf (value, external);
  conf (value, pointer);
  conf (value, dimension);
  /* need to also get volatile attr, according to
   * section 5.1 of f03 draft.  parameter conflict caught below..
   * also, value cannot be specified for a dummy procedure.
   */

  /* Cray pointer/pointee conflicts.  */
  conf (cray_pointer, cray_pointee);
  conf (cray_pointer, dimension);
  conf (cray_pointer, pointer);
  conf (cray_pointer, target);
  conf (cray_pointer, allocatable);
  conf (cray_pointer, external);
  conf (cray_pointer, intrinsic);
  conf (cray_pointer, in_namelist);
  conf (cray_pointer, function);
  conf (cray_pointer, subroutine);
  conf (cray_pointer, entry);

  conf (cray_pointee, allocatable);
  conf (cray_pointee, intent);
  conf (cray_pointee, optional);
  conf (cray_pointee, dummy);
  conf (cray_pointee, target);
  conf (cray_pointee, intrinsic);
  conf (cray_pointee, pointer);
  conf (cray_pointee, entry);
  conf (cray_pointee, in_common);
  conf (cray_pointee, in_equivalence);
  conf (cray_pointee, threadprivate);

  conf (data, dummy);
  conf (data, function);
  conf (data, result);
  conf (data, allocatable);
  conf (data, use_assoc);

  conf (volatile_, intrinsic)
  conf (volatile_, external)

  if (attr->volatile_ && attr->intent == INTENT_IN)
    {
      a1 = volatile_;
      a2 = intent_in;
      goto conflict;
    }

  a1 = gfc_code2string (flavors, attr->flavor);

  if (attr->in_namelist
      && attr->flavor != FL_VARIABLE
      && attr->flavor != FL_UNKNOWN)
    {

      a2 = in_namelist;
      goto conflict;
    }

  switch (attr->flavor)
    {
    case FL_PROGRAM:
    case FL_BLOCK_DATA:
    case FL_MODULE:
    case FL_LABEL:
      conf2 (dimension);
      conf2 (dummy);
      conf2 (save);
      conf2 (volatile_);
      conf2 (pointer);
      conf2 (target);
      conf2 (external);
      conf2 (intrinsic);
      conf2 (allocatable);
      conf2 (result);
      conf2 (in_namelist);
      conf2 (optional);
      conf2 (function);
      conf2 (subroutine);
      conf2 (threadprivate);
      break;

    case FL_VARIABLE:
    case FL_NAMELIST:
      break;

    case FL_PROCEDURE:
      conf2 (intent);
      conf2(save);

      if (attr->subroutine)
	{
	  conf2(pointer);
	  conf2(target);
	  conf2(allocatable);
	  conf2(result);
	  conf2(in_namelist);
	  conf2(dimension);
	  conf2(function);
	  conf2(threadprivate);
	}

      switch (attr->proc)
	{
	case PROC_ST_FUNCTION:
	  conf2 (in_common);
	  conf2 (dummy);
	  break;

	case PROC_MODULE:
	  conf2 (dummy);
	  break;

	case PROC_DUMMY:
	  conf2 (result);
	  conf2 (in_common);
	  conf2 (save);
	  conf2 (threadprivate);
	  break;

	default:
	  break;
	}

      break;

    case FL_DERIVED:
      conf2 (dummy);
      conf2 (save);
      conf2 (pointer);
      conf2 (target);
      conf2 (external);
      conf2 (intrinsic);
      conf2 (allocatable);
      conf2 (optional);
      conf2 (entry);
      conf2 (function);
      conf2 (subroutine);
      conf2 (threadprivate);

      if (attr->intent != INTENT_UNKNOWN)
	{
	  a2 = intent;
	  goto conflict;
	}
      break;

    case FL_PARAMETER:
      conf2 (external);
      conf2 (intrinsic);
      conf2 (optional);
      conf2 (allocatable);
      conf2 (function);
      conf2 (subroutine);
      conf2 (entry);
      conf2 (pointer);
      conf2 (target);
      conf2 (dummy);
      conf2 (in_common);
      conf2 (save);
      conf2 (volatile_);
      conf2 (threadprivate);
      /* hmm, double check this..  --Rickett, 01.24.06 */
      conf2 (value);
      break;

    default:
      break;
    }

  return SUCCESS;

conflict:
  if (name == NULL)
    gfc_error ("%s attribute conflicts with %s attribute at %L",
	       a1, a2, where);
  else
    gfc_error ("%s attribute conflicts with %s attribute in '%s' at %L",
	       a1, a2, name, where);

  return FAILURE;

conflict_std:
  if (name == NULL)
    {
      return gfc_notify_std (standard, "In the selected standard, %s attribute "
                             "conflicts with %s attribute at %L", a1, a2,
                             where);
    }
  else
    {
      return gfc_notify_std (standard, "In the selected standard, %s attribute "
                             "conflicts with %s attribute in '%s' at %L",
                             a1, a2, name, where);
    }
}

#undef conf
#undef conf2
#undef conf_std


/* Mark a symbol as referenced.  */

void
gfc_set_sym_referenced (gfc_symbol * sym)
{
  if (sym->attr.referenced)
    return;

  sym->attr.referenced = 1;

  /* Remember which order dummy variables are accessed in.  */
  if (sym->attr.dummy)
    sym->dummy_order = next_dummy_order++;
}


/* Common subroutine called by attribute changing subroutines in order
   to prevent them from changing a symbol that has been
   use-associated.  Returns zero if it is OK to change the symbol,
   nonzero if not.  */

static int
check_used (symbol_attribute * attr, const char * name, locus * where)
{

  if (attr->use_assoc == 0)
    return 0;

  if (where == NULL)
    where = &gfc_current_locus;

  if (name == NULL)
    gfc_error ("Cannot change attributes of USE-associated symbol at %L",
	       where);
  else
    gfc_error ("Cannot change attributes of USE-associated symbol %s at %L",
	       name, where);

  return 1;
}


/* Generate an error because of a duplicate attribute.  */

static void
duplicate_attr (const char *attr, locus * where)
{

  if (where == NULL)
    where = &gfc_current_locus;

  gfc_error ("Duplicate %s attribute specified at %L", attr, where);
}

/* Called from decl.c (attr_decl1) to check attributes, when declared separately.  */

try
gfc_add_attribute (symbol_attribute * attr, locus * where)
{
  if (check_used (attr, NULL, where))
    return FAILURE;

  return check_conflict (attr, NULL, where);
}

try
gfc_add_allocatable (symbol_attribute * attr, locus * where)
{

  if (check_used (attr, NULL, where))
    return FAILURE;

  if (attr->allocatable)
    {
      duplicate_attr ("ALLOCATABLE", where);
      return FAILURE;
    }

  attr->allocatable = 1;
  return check_conflict (attr, NULL, where);
}


try
gfc_add_dimension (symbol_attribute * attr, const char *name, locus * where)
{

  if (check_used (attr, name, where))
    return FAILURE;

  if (attr->dimension)
    {
      duplicate_attr ("DIMENSION", where);
      return FAILURE;
    }

  attr->dimension = 1;
  return check_conflict (attr, name, where);
}


try
gfc_add_external (symbol_attribute * attr, locus * where)
{

  if (check_used (attr, NULL, where))
    return FAILURE;

  if (attr->external)
    {
      duplicate_attr ("EXTERNAL", where);
      return FAILURE;
    }

  attr->external = 1;

  return check_conflict (attr, NULL, where);
}


try
gfc_add_intrinsic (symbol_attribute * attr, locus * where)
{

  if (check_used (attr, NULL, where))
    return FAILURE;

  if (attr->intrinsic)
    {
      duplicate_attr ("INTRINSIC", where);
      return FAILURE;
    }

  attr->intrinsic = 1;

  return check_conflict (attr, NULL, where);
}


try
gfc_add_optional (symbol_attribute * attr, locus * where)
{

  if (check_used (attr, NULL, where))
    return FAILURE;

  if (attr->optional)
    {
      duplicate_attr ("OPTIONAL", where);
      return FAILURE;
    }

  attr->optional = 1;
  return check_conflict (attr, NULL, where);
}


try
gfc_add_pointer (symbol_attribute * attr, locus * where)
{

  if (check_used (attr, NULL, where))
    return FAILURE;

  attr->pointer = 1;
  return check_conflict (attr, NULL, where);
}


try
gfc_add_cray_pointer (symbol_attribute * attr, locus * where)
{

  if (check_used (attr, NULL, where))
    return FAILURE;

  attr->cray_pointer = 1;
  return check_conflict (attr, NULL, where);
}


try
gfc_add_cray_pointee (symbol_attribute * attr, locus * where)
{

  if (check_used (attr, NULL, where))
    return FAILURE;

  if (attr->cray_pointee)
    {
      gfc_error ("Cray Pointee at %L appears in multiple pointer()"
		 " statements", where);
      return FAILURE;
    }

  attr->cray_pointee = 1;
  return check_conflict (attr, NULL, where);
}


try
gfc_add_result (symbol_attribute * attr, const char *name, locus * where)
{

  if (check_used (attr, name, where))
    return FAILURE;

  attr->result = 1;
  return check_conflict (attr, name, where);
}


try
gfc_add_save (symbol_attribute * attr, const char *name, locus * where)
{

  if (check_used (attr, name, where))
    return FAILURE;

  if (gfc_pure (NULL))
    {
      gfc_error
	("SAVE attribute at %L cannot be specified in a PURE procedure",
	 where);
      return FAILURE;
    }

  if (attr->save)
    {
	if (gfc_notify_std (GFC_STD_LEGACY, 
			    "Duplicate SAVE attribute specified at %L",
			    where) 
	    == FAILURE)
	  return FAILURE;
    }

  attr->save = 1;
  return check_conflict (attr, name, where);
}

try
gfc_add_volatile (symbol_attribute * attr, const char *name, locus * where)
{

  if (check_used (attr, name, where))
    return FAILURE;

  if (attr->volatile_)
    {
	if (gfc_notify_std (GFC_STD_LEGACY, 
			    "Duplicate VOLATILE attribute specified at %L",
			    where) 
	    == FAILURE)
	  return FAILURE;
    }

  attr->volatile_ = 1;
  return check_conflict (attr, name, where);
}


try
gfc_add_threadprivate (symbol_attribute * attr, const char *name, locus * where)
{
  if (check_used (attr, name, where))
    return FAILURE;

  if (attr->threadprivate)
    {
      duplicate_attr ("THREADPRIVATE", where);
      return FAILURE;
    }

  attr->threadprivate = 1;
  return check_conflict (attr, name, where);
}


try
gfc_add_target (symbol_attribute * attr, locus * where)
{

  if (check_used (attr, NULL, where))
    return FAILURE;

  if (attr->target)
    {
      duplicate_attr ("TARGET", where);
      return FAILURE;
    }

  attr->target = 1;
  return check_conflict (attr, NULL, where);
}


try
gfc_add_dummy (symbol_attribute * attr, const char *name, locus * where)
{

  if (check_used (attr, name, where))
    return FAILURE;

  /* Duplicate dummy arguments are allowed due to ENTRY statements.  */
  attr->dummy = 1;
  return check_conflict (attr, name, where);
}


try
gfc_add_in_common (symbol_attribute * attr, const char *name, locus * where)
{

  if (check_used (attr, name, where))
    return FAILURE;

  /* Duplicate attribute already checked for.  */
  attr->in_common = 1;
  if (check_conflict (attr, name, where) == FAILURE)
    return FAILURE;

  if (attr->flavor == FL_VARIABLE)
    return SUCCESS;

  return gfc_add_flavor (attr, FL_VARIABLE, name, where);
}

try
gfc_add_in_equivalence (symbol_attribute * attr, const char *name, locus * where)
{

  /* Duplicate attribute already checked for.  */
  attr->in_equivalence = 1;
  if (check_conflict (attr, name, where) == FAILURE)
    return FAILURE;

  if (attr->flavor == FL_VARIABLE)
    return SUCCESS;

  return gfc_add_flavor (attr, FL_VARIABLE, name, where);
}


try
gfc_add_data (symbol_attribute *attr, const char *name, locus *where)
{

  if (check_used (attr, name, where))
    return FAILURE;

  attr->data = 1;
  return check_conflict (attr, name, where);
}


try
gfc_add_in_namelist (symbol_attribute * attr, const char *name,
		     locus * where)
{

  attr->in_namelist = 1;
  return check_conflict (attr, name, where);
}


try
gfc_add_sequence (symbol_attribute * attr, const char *name, locus * where)
{

  if (check_used (attr, name, where))
    return FAILURE;

  attr->sequence = 1;
  return check_conflict (attr, name, where);
}


try
gfc_add_elemental (symbol_attribute * attr, locus * where)
{

  if (check_used (attr, NULL, where))
    return FAILURE;

  attr->elemental = 1;
  return check_conflict (attr, NULL, where);
}


try
gfc_add_pure (symbol_attribute * attr, locus * where)
{

  if (check_used (attr, NULL, where))
    return FAILURE;

  attr->pure = 1;
  return check_conflict (attr, NULL, where);
}


try
gfc_add_recursive (symbol_attribute * attr, locus * where)
{

  if (check_used (attr, NULL, where))
    return FAILURE;

  attr->recursive = 1;
  return check_conflict (attr, NULL, where);
}


try
gfc_add_entry (symbol_attribute * attr, const char *name, locus * where)
{

  if (check_used (attr, name, where))
    return FAILURE;

  if (attr->entry)
    {
      duplicate_attr ("ENTRY", where);
      return FAILURE;
    }

  attr->entry = 1;
  return check_conflict (attr, name, where);
}


try
gfc_add_function (symbol_attribute * attr, const char *name, locus * where)
{

  if (attr->flavor != FL_PROCEDURE
      && gfc_add_flavor (attr, FL_PROCEDURE, name, where) == FAILURE)
    return FAILURE;

  attr->function = 1;
  return check_conflict (attr, name, where);
}


try
gfc_add_subroutine (symbol_attribute * attr, const char *name, locus * where)
{

  if (attr->flavor != FL_PROCEDURE
      && gfc_add_flavor (attr, FL_PROCEDURE, name, where) == FAILURE)
    return FAILURE;

  attr->subroutine = 1;
  return check_conflict (attr, name, where);
}


try
gfc_add_generic (symbol_attribute * attr, const char *name, locus * where)
{

  if (attr->flavor != FL_PROCEDURE
      && gfc_add_flavor (attr, FL_PROCEDURE, name, where) == FAILURE)
    return FAILURE;

  attr->generic = 1;
  return check_conflict (attr, name, where);
}


/* Flavors are special because some flavors are not what Fortran
   considers attributes and can be reaffirmed multiple times.  */

try
gfc_add_flavor (symbol_attribute * attr, sym_flavor f, const char *name,
		locus * where)
{

  if ((f == FL_PROGRAM || f == FL_BLOCK_DATA || f == FL_MODULE
       || f == FL_PARAMETER || f == FL_LABEL || f == FL_DERIVED
       || f == FL_NAMELIST) && check_used (attr, name, where))
    return FAILURE;

  if (attr->flavor == f && f == FL_VARIABLE)
    return SUCCESS;

  if (attr->flavor != FL_UNKNOWN)
    {
      if (where == NULL)
	where = &gfc_current_locus;

      gfc_error ("%s attribute conflicts with %s attribute at %L",
		 gfc_code2string (flavors, attr->flavor),
		 gfc_code2string (flavors, f), where);

      return FAILURE;
    }

  attr->flavor = f;

  return check_conflict (attr, name, where);
}


try
gfc_add_procedure (symbol_attribute * attr, procedure_type t,
		   const char *name, locus * where)
{

  if (check_used (attr, name, where))
    return FAILURE;

  if (attr->flavor != FL_PROCEDURE
      && gfc_add_flavor (attr, FL_PROCEDURE, name, where) == FAILURE)
    return FAILURE;

  if (where == NULL)
    where = &gfc_current_locus;

  if (attr->proc != PROC_UNKNOWN)
    {
      gfc_error ("%s procedure at %L is already declared as %s procedure",
		 gfc_code2string (procedures, t), where,
		 gfc_code2string (procedures, attr->proc));

      return FAILURE;
    }

  attr->proc = t;

  /* Statement functions are always scalar and functions.  */
  if (t == PROC_ST_FUNCTION
      && ((!attr->function && gfc_add_function (attr, name, where) == FAILURE)
	  || attr->dimension))
    return FAILURE;

  return check_conflict (attr, name, where);
}


try
gfc_add_intent (symbol_attribute * attr, sym_intent intent, locus * where)
{

  if (check_used (attr, NULL, where))
    return FAILURE;

  if (attr->intent == INTENT_UNKNOWN)
    {
      attr->intent = intent;
      return check_conflict (attr, NULL, where);
    }

  if (where == NULL)
    where = &gfc_current_locus;

  gfc_error ("INTENT (%s) conflicts with INTENT(%s) at %L",
	     gfc_intent_string (attr->intent),
	     gfc_intent_string (intent), where);

  return FAILURE;
}


/* No checks for use-association in public and private statements.  */

try
gfc_add_access (symbol_attribute * attr, gfc_access access,
		const char *name, locus * where)
{

  if (attr->access == ACCESS_UNKNOWN)
    {
      attr->access = access;
      return check_conflict (attr, name, where);
    }

  if (where == NULL)
    where = &gfc_current_locus;
  gfc_error ("ACCESS specification at %L was already specified", where);

  return FAILURE;
}

/**
 * Set the <code>is_bind_c</code> field for the given <code>#symbol_attribute</code>.
 *
 * @param attr <code>#symbol_attribute</code> object to set <code>is_bind_c</code>
 * field in.
 * @param where Location where this <code>BIND(C)</code> attribute was encountered.
 * @return SUCCESS if no errors encountered; FAILURE otherwise.
 */
try gfc_add_is_bind_c(symbol_attribute *attr, locus *where)
{
   attr->is_bind_c = 1;

   if(where == NULL)
      where = &gfc_current_locus;
   
   if(gfc_notify_std(GFC_STD_F2003,
                     "New in Fortran 2003: BIND(C) at %L", where)
      == FAILURE)
      return FAILURE;

   return check_conflict(attr, NULL, where);
}/* end gfc_add_is_bind_c() */


/**
 * Set the <code>value</code> field for the given <code>#symbol_attribute</code>.
 *
 * @param attr <code>#symbol_attribute</code> object to set <code>value</code>
 * field in.
 * @param where Location where this <code>value</code> attribute was encountered.
 * @return SUCCESS if no errors encountered; FAILURE otherwise.
 */
try gfc_add_value(symbol_attribute *attr, locus *where)
{
   if(where == NULL)
      where = &gfc_current_locus;
   
   if(attr->value)
   {
      duplicate_attr("VALUE", where);
      return FAILURE;
   }

   attr->value = 1;

   if(gfc_notify_std(GFC_STD_F2003,
                     "New in Fortran 2003: 'value' attribute at %L",
                     where) == FAILURE)
      return FAILURE;
   
   return check_conflict(attr, NULL, where);
}/* end gfc_add_value() */


try
gfc_add_explicit_interface (gfc_symbol * sym, ifsrc source,
			    gfc_formal_arglist * formal, locus * where)
{

  if (check_used (&sym->attr, sym->name, where))
    return FAILURE;

  if (where == NULL)
    where = &gfc_current_locus;

  if (sym->attr.if_source != IFSRC_UNKNOWN
      && sym->attr.if_source != IFSRC_DECL)
    {
      gfc_error ("Symbol '%s' at %L already has an explicit interface",
		 sym->name, where);
      return FAILURE;
    }

  sym->formal = formal;
  sym->attr.if_source = source;

  return SUCCESS;
}


/* Add a type to a symbol.  */

try
gfc_add_type (gfc_symbol * sym, gfc_typespec * ts, locus * where)
{
  sym_flavor flavor;

  if (where == NULL)
    where = &gfc_current_locus;

  if (sym->ts.type != BT_UNKNOWN)
    {
      const char *msg = "Symbol '%s' at %L already has basic type of %s";
      if (!(sym->ts.type == ts->type
	     && (sym->attr.flavor == FL_PROCEDURE || sym->attr.result))
	   || gfc_notification_std (GFC_STD_GNU) == ERROR
	   || pedantic)
	{
	  gfc_error (msg, sym->name, where, gfc_basic_typename (sym->ts.type));
	  return FAILURE;
	}
      else if (gfc_notify_std (GFC_STD_GNU, msg, sym->name, where,
			       gfc_basic_typename (sym->ts.type)) == FAILURE)
	  return FAILURE;
    }

  flavor = sym->attr.flavor;

  if (flavor == FL_PROGRAM || flavor == FL_BLOCK_DATA || flavor == FL_MODULE
      || flavor == FL_LABEL || (flavor == FL_PROCEDURE
				&& sym->attr.subroutine)
      || flavor == FL_DERIVED || flavor == FL_NAMELIST)
    {
      gfc_error ("Symbol '%s' at %L cannot have a type", sym->name, where);
      return FAILURE;
    }

  sym->ts = *ts;
  return SUCCESS;
}


/* Clears all attributes.  */

void
gfc_clear_attr (symbol_attribute * attr)
{
  memset (attr, 0, sizeof(symbol_attribute));
}


/* Check for missing attributes in the new symbol.  Currently does
   nothing, but it's not clear that it is unnecessary yet.  */

try
gfc_missing_attr (symbol_attribute * attr ATTRIBUTE_UNUSED,
		  locus * where ATTRIBUTE_UNUSED)
{

  return SUCCESS;
}


/* Copy an attribute to a symbol attribute, bit by bit.  Some
   attributes have a lot of side-effects but cannot be present given
   where we are called from, so we ignore some bits.  */

try
gfc_copy_attr (symbol_attribute * dest, symbol_attribute * src, locus * where)
{

  if (src->allocatable && gfc_add_allocatable (dest, where) == FAILURE)
    goto fail;

  if (src->dimension && gfc_add_dimension (dest, NULL, where) == FAILURE)
    goto fail;
  if (src->optional && gfc_add_optional (dest, where) == FAILURE)
    goto fail;
  if (src->pointer && gfc_add_pointer (dest, where) == FAILURE)
    goto fail;
  if (src->save && gfc_add_save (dest, NULL, where) == FAILURE)
    goto fail;
  if (src->volatile_ && gfc_add_volatile (dest, NULL, where) == FAILURE)
    goto fail;
  if (src->threadprivate && gfc_add_threadprivate (dest, NULL, where) == FAILURE)
    goto fail;
  if (src->target && gfc_add_target (dest, where) == FAILURE)
    goto fail;
  if (src->dummy && gfc_add_dummy (dest, NULL, where) == FAILURE)
    goto fail;
  if (src->result && gfc_add_result (dest, NULL, where) == FAILURE)
    goto fail;
  if (src->entry)
    dest->entry = 1;

  if (src->in_namelist && gfc_add_in_namelist (dest, NULL, where) == FAILURE)
    goto fail;

  if (src->in_common && gfc_add_in_common (dest, NULL, where) == FAILURE)
    goto fail;

  if (src->generic && gfc_add_generic (dest, NULL, where) == FAILURE)
    goto fail;
  if (src->function && gfc_add_function (dest, NULL, where) == FAILURE)
    goto fail;
  if (src->subroutine && gfc_add_subroutine (dest, NULL, where) == FAILURE)
    goto fail;

  if (src->sequence && gfc_add_sequence (dest, NULL, where) == FAILURE)
    goto fail;
  if (src->elemental && gfc_add_elemental (dest, where) == FAILURE)
    goto fail;
  if (src->pure && gfc_add_pure (dest, where) == FAILURE)
    goto fail;
  if (src->recursive && gfc_add_recursive (dest, where) == FAILURE)
    goto fail;

  if (src->flavor != FL_UNKNOWN
      && gfc_add_flavor (dest, src->flavor, NULL, where) == FAILURE)
    goto fail;

  if (src->intent != INTENT_UNKNOWN
      && gfc_add_intent (dest, src->intent, where) == FAILURE)
    goto fail;

  if (src->access != ACCESS_UNKNOWN
      && gfc_add_access (dest, src->access, NULL, where) == FAILURE)
    goto fail;

  if (gfc_missing_attr (dest, where) == FAILURE)
    goto fail;

  if (src->cray_pointer && gfc_add_cray_pointer (dest, where) == FAILURE)
    goto fail;
  if (src->cray_pointee && gfc_add_cray_pointee (dest, where) == FAILURE)
    goto fail;    
  
  /* The subroutines that set these bits also cause flavors to be set,
     and that has already happened in the original, so don't let it
     happen again.  */
  if (src->external)
    dest->external = 1;
  if (src->intrinsic)
    dest->intrinsic = 1;

  if(src->is_bind_c && gfc_add_is_bind_c(dest, where) != SUCCESS)
     return FAILURE;
  if(src->value && gfc_add_value(dest, where) != SUCCESS)
     return FAILURE;
  if(src->is_c_interop)
     dest->is_c_interop = 1;
  if(src->is_iso_c)
     dest->is_iso_c = 1;
  if(src->in_proc_decl)
     dest->in_proc_decl = 1;
  
  return SUCCESS;

fail:
  return FAILURE;
}


/************** Component name management ************/

/* Component names of a derived type form their own little namespaces
   that are separate from all other spaces.  The space is composed of
   a singly linked list of gfc_component structures whose head is
   located in the parent symbol.  */


/* Add a component name to a symbol.  The call fails if the name is
   already present.  On success, the component pointer is modified to
   point to the additional component structure.  */

try
gfc_add_component (gfc_symbol * sym, const char *name, gfc_component ** component)
{
  gfc_component *p, *tail;

  tail = NULL;

  for (p = sym->components; p; p = p->next)
    {
      if (strcmp (p->name, name) == 0)
	{
	  gfc_error ("Component '%s' at %C already declared at %L",
		     name, &p->loc);
	  return FAILURE;
	}

      tail = p;
    }

  /* Allocate a new component.  */
  p = gfc_get_component ();

  if (tail == NULL)
    sym->components = p;
  else
    tail->next = p;

  p->name = gfc_get_string (name);
  p->loc = gfc_current_locus;

  *component = p;
  return SUCCESS;
}


/* Recursive function to switch derived types of all symbol in a
   namespace.  */

static void
switch_types (gfc_symtree * st, gfc_symbol * from, gfc_symbol * to)
{
  gfc_symbol *sym;

  if (st == NULL)
    return;

  sym = st->n.sym;
  if (sym->ts.type == BT_DERIVED && sym->ts.derived == from)
    sym->ts.derived = to;

  switch_types (st->left, from, to);
  switch_types (st->right, from, to);
}


/* This subroutine is called when a derived type is used in order to
   make the final determination about which version to use.  The
   standard requires that a type be defined before it is 'used', but
   such types can appear in IMPLICIT statements before the actual
   definition.  'Using' in this context means declaring a variable to
   be that type or using the type constructor.

   If a type is used and the components haven't been defined, then we
   have to have a derived type in a parent unit.  We find the node in
   the other namespace and point the symtree node in this namespace to
   that node.  Further reference to this name point to the correct
   node.  If we can't find the node in a parent namespace, then we have
   an error.

   This subroutine takes a pointer to a symbol node and returns a
   pointer to the translated node or NULL for an error.  Usually there
   is no translation and we return the node we were passed.  */

gfc_symbol *
gfc_use_derived (gfc_symbol * sym)
{
  gfc_symbol *s;
  gfc_typespec *t;
  gfc_symtree *st;
  int i;

  if (sym->components != NULL)
    return sym;               /* Already defined.  */

  if (sym->ns->parent == NULL)
    goto bad;

  if (gfc_find_symbol (sym->name, sym->ns->parent, 1, &s))
    {
      gfc_error ("Symbol '%s' at %C is ambiguous", sym->name);
      return NULL;
    }

  if (s == NULL || s->attr.flavor != FL_DERIVED)
    goto bad;

  /* Get rid of symbol sym, translating all references to s.  */
  for (i = 0; i < GFC_LETTERS; i++)
    {
      t = &sym->ns->default_type[i];
      if (t->derived == sym)
	t->derived = s;
    }

  st = gfc_find_symtree (sym->ns->sym_root, sym->name);
  st->n.sym = s;

  s->refs++;

  /* Unlink from list of modified symbols.  */
  gfc_commit_symbol (sym);

  switch_types (sym->ns->sym_root, sym, s);

  /* TODO: Also have to replace sym -> s in other lists like
     namelists, common lists and interface lists.  */
  gfc_free_symbol (sym);

  return s;

bad:
  gfc_error ("Derived type '%s' at %C is being used before it is defined",
	     sym->name);
  return NULL;
}


/* Given a derived type node and a component name, try to locate the
   component structure.  Returns the NULL pointer if the component is
   not found or the components are private.  */

gfc_component *
gfc_find_component (gfc_symbol * sym, const char *name)
{
  gfc_component *p;

  if (name == NULL)
    return NULL;

  sym = gfc_use_derived (sym);

  if (sym == NULL)
    return NULL;

  for (p = sym->components; p; p = p->next)
    if (strcmp (p->name, name) == 0)
      break;

  if (p == NULL)
    gfc_error ("'%s' at %C is not a member of the '%s' structure",
	       name, sym->name);
  else
    {
      if (sym->attr.use_assoc && sym->component_access == ACCESS_PRIVATE)
	{
	  gfc_error ("Component '%s' at %C is a PRIVATE component of '%s'",
		     name, sym->name);
	  p = NULL;
	}
    }

  return p;
}


/* Given a symbol, free all of the component structures and everything
   they point to.  */

static void
free_components (gfc_component * p)
{
  gfc_component *q;

  for (; p; p = q)
    {
      q = p->next;

      gfc_free_array_spec (p->as);
      gfc_free_expr (p->initializer);

      gfc_free (p);
    }
}


/* Set component attributes from a standard symbol attribute
   structure.  */

void
gfc_set_component_attr (gfc_component * c, symbol_attribute * attr)
{

  c->dimension = attr->dimension;
  c->pointer = attr->pointer;
  c->allocatable = attr->allocatable;
}


/* Get a standard symbol attribute structure given the component
   structure.  */

void
gfc_get_component_attr (symbol_attribute * attr, gfc_component * c)
{

  gfc_clear_attr (attr);
  attr->dimension = c->dimension;
  attr->pointer = c->pointer;
  attr->allocatable = c->allocatable;
}


/******************** Statement label management ********************/

/* Comparison function for statement labels, used for managing the
   binary tree.  */

static int
compare_st_labels (void * a1, void * b1)
{
  int a = ((gfc_st_label *)a1)->value;
  int b = ((gfc_st_label *)b1)->value;

  return (b - a);
}


/* Free a single gfc_st_label structure, making sure the tree is not
   messed up.  This function is called only when some parse error
   occurs.  */

void
gfc_free_st_label (gfc_st_label * label)
{
  if (label == NULL)
    return;

  gfc_delete_bbt (&gfc_current_ns->st_labels, label, compare_st_labels);

  if (label->format != NULL)
    gfc_free_expr (label->format);

  gfc_free (label);
}

/* Free a whole tree of gfc_st_label structures.  */

static void
free_st_labels (gfc_st_label * label)
{
  if (label == NULL)
    return;

  free_st_labels (label->left);
  free_st_labels (label->right);
  
  if (label->format != NULL)
    gfc_free_expr (label->format);
  gfc_free (label);
}


/* Given a label number, search for and return a pointer to the label
   structure, creating it if it does not exist.  */

gfc_st_label *
gfc_get_st_label (int labelno)
{
  gfc_st_label *lp;

  /* First see if the label is already in this namespace.  */
  lp = gfc_current_ns->st_labels;
  while (lp)
    {
      if (lp->value == labelno)
	return lp;

      if (lp->value < labelno)
	lp = lp->left;
      else
	lp = lp->right;
    }

  lp = gfc_getmem (sizeof (gfc_st_label));

  lp->value = labelno;
  lp->defined = ST_LABEL_UNKNOWN;
  lp->referenced = ST_LABEL_UNKNOWN;

  gfc_insert_bbt (&gfc_current_ns->st_labels, lp, compare_st_labels);

  return lp;
}


/* Called when a statement with a statement label is about to be
   accepted.  We add the label to the list of the current namespace,
   making sure it hasn't been defined previously and referenced
   correctly.  */

void
gfc_define_st_label (gfc_st_label * lp, gfc_sl_type type, locus * label_locus)
{
  int labelno;

  labelno = lp->value;

  if (lp->defined != ST_LABEL_UNKNOWN)
    gfc_error ("Duplicate statement label %d at %L and %L", labelno,
	       &lp->where, label_locus);
  else
    {
      lp->where = *label_locus;

      switch (type)
	{
	case ST_LABEL_FORMAT:
	  if (lp->referenced == ST_LABEL_TARGET)
	    gfc_error ("Label %d at %C already referenced as branch target",
		       labelno);
	  else
	    lp->defined = ST_LABEL_FORMAT;

	  break;

	case ST_LABEL_TARGET:
	  if (lp->referenced == ST_LABEL_FORMAT)
	    gfc_error ("Label %d at %C already referenced as a format label",
		       labelno);
	  else
	    lp->defined = ST_LABEL_TARGET;

	  break;

	default:
	  lp->defined = ST_LABEL_BAD_TARGET;
	  lp->referenced = ST_LABEL_BAD_TARGET;
	}
    }
}


/* Reference a label.  Given a label and its type, see if that
   reference is consistent with what is known about that label,
   updating the unknown state.  Returns FAILURE if something goes
   wrong.  */

try
gfc_reference_st_label (gfc_st_label * lp, gfc_sl_type type)
{
  gfc_sl_type label_type;
  int labelno;
  try rc;

  if (lp == NULL)
    return SUCCESS;

  labelno = lp->value;

  if (lp->defined != ST_LABEL_UNKNOWN)
    label_type = lp->defined;
  else
    {
      label_type = lp->referenced;
      lp->where = gfc_current_locus;
    }

  if (label_type == ST_LABEL_FORMAT && type == ST_LABEL_TARGET)
    {
      gfc_error ("Label %d at %C previously used as a FORMAT label", labelno);
      rc = FAILURE;
      goto done;
    }

  if ((label_type == ST_LABEL_TARGET || label_type == ST_LABEL_BAD_TARGET)
      && type == ST_LABEL_FORMAT)
    {
      gfc_error ("Label %d at %C previously used as branch target", labelno);
      rc = FAILURE;
      goto done;
    }

  lp->referenced = type;
  rc = SUCCESS;

done:
  return rc;
}


/************** Symbol table management subroutines ****************/

/* Basic details: Fortran 95 requires a potentially unlimited number
   of distinct namespaces when compiling a program unit.  This case
   occurs during a compilation of internal subprograms because all of
   the internal subprograms must be read before we can start
   generating code for the host.

   Given the tricky nature of the Fortran grammar, we must be able to
   undo changes made to a symbol table if the current interpretation
   of a statement is found to be incorrect.  Whenever a symbol is
   looked up, we make a copy of it and link to it.  All of these
   symbols are kept in a singly linked list so that we can commit or
   undo the changes at a later time.

   A symtree may point to a symbol node outside of its namespace.  In
   this case, that symbol has been used as a host associated variable
   at some previous time.  */

/* Allocate a new namespace structure.  Copies the implicit types from
   PARENT if PARENT_TYPES is set.  */

gfc_namespace *
gfc_get_namespace (gfc_namespace * parent, int parent_types)
{
  gfc_namespace *ns;
  gfc_typespec *ts;
  gfc_intrinsic_op in;
  int i;

  ns = gfc_getmem (sizeof (gfc_namespace));
  ns->sym_root = NULL;
  ns->uop_root = NULL;
  ns->default_access = ACCESS_UNKNOWN;
  ns->parent = parent;

  for (in = GFC_INTRINSIC_BEGIN; in != GFC_INTRINSIC_END; in++)
    ns->operator_access[in] = ACCESS_UNKNOWN;

  /* Initialize default implicit types.  */
  for (i = 'a'; i <= 'z'; i++)
    {
      ns->set_flag[i - 'a'] = 0;
      ts = &ns->default_type[i - 'a'];

      if (parent_types && ns->parent != NULL)
	{
	  /* Copy parent settings */
	  *ts = ns->parent->default_type[i - 'a'];
	  continue;
	}

      if (gfc_option.flag_implicit_none != 0)
	{
	  gfc_clear_ts (ts);
	  continue;
	}

      if ('i' <= i && i <= 'n')
	{
	  ts->type = BT_INTEGER;
	  ts->kind = gfc_default_integer_kind;
	}
      else
	{
	  ts->type = BT_REAL;
	  ts->kind = gfc_default_real_kind;
	}
    }

  ns->refs = 1;

  return ns;
}


/* Comparison function for symtree nodes.  */

static int
compare_symtree (void * _st1, void * _st2)
{
  gfc_symtree *st1, *st2;

  st1 = (gfc_symtree *) _st1;
  st2 = (gfc_symtree *) _st2;

  return strcmp (st1->name, st2->name);
}


/* Allocate a new symtree node and associate it with the new symbol.  */

gfc_symtree *
gfc_new_symtree (gfc_symtree ** root, const char *name)
{
  gfc_symtree *st;

  st = gfc_getmem (sizeof (gfc_symtree));
  st->name = gfc_get_string (name);

  gfc_insert_bbt (root, st, compare_symtree);
  return st;
}


/* Delete a symbol from the tree.  Does not free the symbol itself!  */

static void
delete_symtree (gfc_symtree ** root, const char *name)
{
  gfc_symtree st, *st0;

  st0 = gfc_find_symtree (*root, name);

  st.name = gfc_get_string (name);
  gfc_delete_bbt (root, &st, compare_symtree);

  gfc_free (st0);
}


/* Given a root symtree node and a name, try to find the symbol within
   the namespace.  Returns NULL if the symbol is not found.  */

gfc_symtree *
gfc_find_symtree (gfc_symtree * st, const char *name)
{
  int c;

  while (st != NULL)
    {
      c = strcmp (name, st->name);
      if (c == 0)
	return st;

      st = (c < 0) ? st->left : st->right;
    }

  return NULL;
}


/* Given a name find a user operator node, creating it if it doesn't
   exist.  These are much simpler than symbols because they can't be
   ambiguous with one another.  */

gfc_user_op *
gfc_get_uop (const char *name)
{
  gfc_user_op *uop;
  gfc_symtree *st;

  st = gfc_find_symtree (gfc_current_ns->uop_root, name);
  if (st != NULL)
    return st->n.uop;

  st = gfc_new_symtree (&gfc_current_ns->uop_root, name);

  uop = st->n.uop = gfc_getmem (sizeof (gfc_user_op));
  uop->name = gfc_get_string (name);
  uop->access = ACCESS_UNKNOWN;
  uop->ns = gfc_current_ns;

  return uop;
}


/* Given a name find the user operator node.  Returns NULL if it does
   not exist.  */

gfc_user_op *
gfc_find_uop (const char *name, gfc_namespace * ns)
{
  gfc_symtree *st;

  if (ns == NULL)
    ns = gfc_current_ns;

  st = gfc_find_symtree (ns->uop_root, name);
  return (st == NULL) ? NULL : st->n.uop;
}


/* Remove a gfc_symbol structure and everything it points to.  */

void
gfc_free_symbol (gfc_symbol * sym)
{

  if (sym == NULL)
    return;

  gfc_free_array_spec (sym->as);

  free_components (sym->components);

  gfc_free_expr (sym->value);

  gfc_free_namelist (sym->namelist);

  gfc_free_namespace (sym->formal_ns);

  gfc_free_interface (sym->generic);

  gfc_free_formal_arglist (sym->formal);

  gfc_free (sym);
}


/* Allocate and initialize a new symbol node.  */

gfc_symbol *
gfc_new_symbol (const char *name, gfc_namespace * ns)
{
  gfc_symbol *p;

  p = gfc_getmem (sizeof (gfc_symbol));

  gfc_clear_ts (&p->ts);
  gfc_clear_attr (&p->attr);
  p->ns = ns;

  p->declared_at = gfc_current_locus;

  if (strlen (name) > GFC_MAX_SYMBOL_LEN)
    gfc_internal_error ("new_symbol(): Symbol name too long");

  p->name = gfc_get_string (name);

  /* make sure flag for symbol being C bound is clear initially */
  p->attr.is_bind_c = 0;
  p->attr.is_iso_c = 0;
  /* make sure the binding label field has a nil char to start */
  p->binding_label[0] = '\0';

  /* clear the ptrs we may need */
  p->common_block = NULL;
  
  return p;
}


/* Generate an error if a symbol is ambiguous.  */

static void
ambiguous_symbol (const char *name, gfc_symtree * st)
{

  if (st->n.sym->module)
    gfc_error ("Name '%s' at %C is an ambiguous reference to '%s' "
	       "from module '%s'", name, st->n.sym->name, st->n.sym->module);
  else
    gfc_error ("Name '%s' at %C is an ambiguous reference to '%s' "
	       "from current program unit", name, st->n.sym->name);
}


/* Search for a symtree starting in the current namespace, resorting to
   any parent namespaces if requested by a nonzero parent_flag.
   Returns nonzero if the name is ambiguous.  */

int
gfc_find_sym_tree (const char *name, gfc_namespace * ns, int parent_flag,
		   gfc_symtree ** result)
{
  gfc_symtree *st;

  if (ns == NULL)
    ns = gfc_current_ns;

  do
    {
      st = gfc_find_symtree (ns->sym_root, name);
      if (st != NULL)
	{
	  *result = st;
	  if (st->ambiguous)
	    {
	      ambiguous_symbol (name, st);
	      return 1;
	    }

	  return 0;
	}

      if (!parent_flag)
	break;

      ns = ns->parent;
    }
  while (ns != NULL);

  *result = NULL;
  return 0;
}


/* Same, but returns the symbol instead.  */

int
gfc_find_symbol (const char *name, gfc_namespace * ns, int parent_flag,
		 gfc_symbol ** result)
{
  gfc_symtree *st;
  int i;

  i = gfc_find_sym_tree (name, ns, parent_flag, &st);

  if (st == NULL)
    *result = NULL;
  else
    *result = st->n.sym;

  return i;
}


/* Save symbol with the information necessary to back it out.  */

static void
save_symbol_data (gfc_symbol * sym)
{

  if (sym->new || sym->old_symbol != NULL)
    return;

  sym->old_symbol = gfc_getmem (sizeof (gfc_symbol));
  *(sym->old_symbol) = *sym;

  sym->tlink = changed_syms;
  changed_syms = sym;
}


/* Given a name, find a symbol, or create it if it does not exist yet
   in the current namespace.  If the symbol is found we make sure that
   it's OK.

   The integer return code indicates
     0   All OK
     1   The symbol name was ambiguous
     2   The name meant to be established was already host associated.

   So if the return value is nonzero, then an error was issued.  */

int
gfc_get_sym_tree (const char *name, gfc_namespace * ns, gfc_symtree ** result)
{
  gfc_symtree *st;
  gfc_symbol *p;

  /* This doesn't usually happen during resolution.  */
  if (ns == NULL)
    ns = gfc_current_ns;

  /* Try to find the symbol in ns.  */
  st = gfc_find_symtree (ns->sym_root, name);

  if (st == NULL)
    {
      /* If not there, create a new symbol.  */
      p = gfc_new_symbol (name, ns);

      /* Add to the list of tentative symbols.  */
      p->old_symbol = NULL;
      p->tlink = changed_syms;
      p->mark = 1;
      p->new = 1;
      changed_syms = p;

      st = gfc_new_symtree (&ns->sym_root, name);
      st->n.sym = p;
      p->refs++;

    }
  else
    {
      /* Make sure the existing symbol is OK.  */
      if (st->ambiguous)
	{
	  ambiguous_symbol (name, st);
	  return 1;
	}

      p = st->n.sym;

      if (p->ns != ns && (!p->attr.function || ns->proc_name != p))
	{
	  /* Symbol is from another namespace.  */
	  gfc_error ("Symbol '%s' at %C has already been host associated",
		     name);
	  return 2;
	}

      p->mark = 1;

      /* Copy in case this symbol is changed.  */
      save_symbol_data (p);
    }

  *result = st;
  return 0;
}


int
gfc_get_symbol (const char *name, gfc_namespace * ns, gfc_symbol ** result)
{
  gfc_symtree *st;
  int i;


  i = gfc_get_sym_tree (name, ns, &st);
  if (i != 0)
    return i;

  if (st)
    *result = st->n.sym;
  else
    *result = NULL;
  return i;
}


/* Subroutine that searches for a symbol, creating it if it doesn't
   exist, but tries to host-associate the symbol if possible.  */

int
gfc_get_ha_sym_tree (const char *name, gfc_symtree ** result)
{
  gfc_symtree *st;
  int i;

  i = gfc_find_sym_tree (name, gfc_current_ns, 0, &st);
  if (st != NULL)
    {
      save_symbol_data (st->n.sym);

      *result = st;
      return i;
    }

  if (gfc_current_ns->parent != NULL)
    {
      i = gfc_find_sym_tree (name, gfc_current_ns->parent, 1, &st);
      if (i)
	return i;

      if (st != NULL)
	{
	  *result = st;
	  return 0;
	}
    }

  return gfc_get_sym_tree (name, gfc_current_ns, result);
}


int
gfc_get_ha_symbol (const char *name, gfc_symbol ** result)
{
  int i;
  gfc_symtree *st;

  i = gfc_get_ha_sym_tree (name, &st);

  if (st)
    *result = st->n.sym;
  else
    *result = NULL;

  return i;
}

/* Return true if both symbols could refer to the same data object.  Does
   not take account of aliasing due to equivalence statements.  */

int
gfc_symbols_could_alias (gfc_symbol * lsym, gfc_symbol * rsym)
{
  /* Aliasing isn't possible if the symbols have different base types.  */
  if (gfc_compare_types (&lsym->ts, &rsym->ts) == 0)
    return 0;

  /* Pointers can point to other pointers, target objects and allocatable
     objects.  Two allocatable objects cannot share the same storage.  */
  if (lsym->attr.pointer
      && (rsym->attr.pointer || rsym->attr.allocatable || rsym->attr.target))
    return 1;
  if (lsym->attr.target && rsym->attr.pointer)
    return 1;
  if (lsym->attr.allocatable && rsym->attr.pointer)
    return 1;

  return 0;
}


/* Undoes all the changes made to symbols in the current statement.
   This subroutine is made simpler due to the fact that attributes are
   never removed once added.  */

void
gfc_undo_symbols (void)
{
  gfc_symbol *p, *q, *old;

  for (p = changed_syms; p; p = q)
    {
      q = p->tlink;

      if (p->new)
	{
	  /* Symbol was new.  */
	  delete_symtree (&p->ns->sym_root, p->name);

	  p->refs--;
	  if (p->refs < 0)
	    gfc_internal_error ("gfc_undo_symbols(): Negative refs");
	  if (p->refs == 0)
	    gfc_free_symbol (p);
	  continue;
	}

      /* Restore previous state of symbol.  Just copy simple stuff.  */
      p->mark = 0;
      old = p->old_symbol;

      p->ts.type = old->ts.type;
      p->ts.kind = old->ts.kind;

      p->attr = old->attr;

      if (p->value != old->value)
	{
	  gfc_free_expr (old->value);
	  p->value = NULL;
	}

      if (p->as != old->as)
	{
	  if (p->as)
	    gfc_free_array_spec (p->as);
	  p->as = old->as;
	}

      p->generic = old->generic;
      p->component_access = old->component_access;

      if (p->namelist != NULL && old->namelist == NULL)
	{
	  gfc_free_namelist (p->namelist);
	  p->namelist = NULL;
	}
      else
	{

	  if (p->namelist_tail != old->namelist_tail)
	    {
	      gfc_free_namelist (old->namelist_tail);
	      old->namelist_tail->next = NULL;
	    }
	}

      p->namelist_tail = old->namelist_tail;

      if (p->formal != old->formal)
	{
	  gfc_free_formal_arglist (p->formal);
	  p->formal = old->formal;
	}

      gfc_free (p->old_symbol);
      p->old_symbol = NULL;
      p->tlink = NULL;
    }

  changed_syms = NULL;
}


/* Free sym->old_symbol. sym->old_symbol is mostly a shallow copy of sym; the
   components of old_symbol that might need deallocation are the "allocatables"
   that are restored in gfc_undo_symbols(), with two exceptions: namelist and
   namelist_tail.  In case these differ between old_symbol and sym, it's just
   because sym->namelist has gotten a few more items.  */

static void
free_old_symbol (gfc_symbol * sym)
{
  if (sym->old_symbol == NULL)
    return;

  if (sym->old_symbol->as != sym->as) 
    gfc_free_array_spec (sym->old_symbol->as);

  if (sym->old_symbol->value != sym->value) 
    gfc_free_expr (sym->old_symbol->value);

  if (sym->old_symbol->formal != sym->formal)
    gfc_free_formal_arglist (sym->old_symbol->formal);

  gfc_free (sym->old_symbol);
  sym->old_symbol = NULL;
}


/* Makes the changes made in the current statement permanent-- gets
   rid of undo information.  */

void
gfc_commit_symbols (void)
{
  gfc_symbol *p, *q;

  for (p = changed_syms; p; p = q)
    {
      q = p->tlink;
      p->tlink = NULL;
      p->mark = 0;
      p->new = 0;

      free_old_symbol (p);
    }
  changed_syms = NULL;
}


/* Makes the changes made in one symbol permanent -- gets rid of undo
   information.  */

void
gfc_commit_symbol (gfc_symbol * sym)
{
  gfc_symbol *p;

  if (changed_syms == sym)
    changed_syms = sym->tlink;
  else
    {
      for (p = changed_syms; p; p = p->tlink)
        if (p->tlink == sym)
          {
            p->tlink = sym->tlink;
            break;
          }
    }

  sym->tlink = NULL;
  sym->mark = 0;
  sym->new = 0;

  free_old_symbol (sym);
}


/* Recursive function that deletes an entire tree and all the common
   head structures it points to.  */

static void
free_common_tree (gfc_symtree * common_tree)
{
  if (common_tree == NULL)
    return;

  free_common_tree (common_tree->left);
  free_common_tree (common_tree->right);

  gfc_free (common_tree);
}  


/* Recursive function that deletes an entire tree and all the user
   operator nodes that it contains.  */

static void
free_uop_tree (gfc_symtree * uop_tree)
{

  if (uop_tree == NULL)
    return;

  free_uop_tree (uop_tree->left);
  free_uop_tree (uop_tree->right);

  gfc_free_interface (uop_tree->n.uop->operator);

  gfc_free (uop_tree->n.uop);
  gfc_free (uop_tree);
}


/* Recursive function that deletes an entire tree and all the symbols
   that it contains.  */

static void
free_sym_tree (gfc_symtree * sym_tree)
{
  gfc_namespace *ns;
  gfc_symbol *sym;

  if (sym_tree == NULL)
    return;

  free_sym_tree (sym_tree->left);
  free_sym_tree (sym_tree->right);

  sym = sym_tree->n.sym;

  sym->refs--;
  if (sym->refs < 0)
    gfc_internal_error ("free_sym_tree(): Negative refs");

  if (sym->formal_ns != NULL && sym->refs == 1)
    {
      /* As formal_ns contains a reference to sym, delete formal_ns just
         before the deletion of sym.  */
      ns = sym->formal_ns;
      sym->formal_ns = NULL;
      gfc_free_namespace (ns);
    }
  else if (sym->refs == 0)
    {
      /* Go ahead and delete the symbol.  */
      gfc_free_symbol (sym);
    }

  gfc_free (sym_tree);
}


/* Free a derived type list.  */

static void
gfc_free_dt_list (gfc_dt_list * dt)
{
  gfc_dt_list *n;

  for (; dt; dt = n)
    {
      n = dt->next;
      gfc_free (dt);
    }
}


/* Free the gfc_equiv_info's.  */

static void
gfc_free_equiv_infos (gfc_equiv_info * s)
{
  if (s == NULL)
    return;
  gfc_free_equiv_infos (s->next);
  gfc_free (s);
}


/* Free the gfc_equiv_lists.  */

static void
gfc_free_equiv_lists (gfc_equiv_list * l)
{
  if (l == NULL)
    return;
  gfc_free_equiv_lists (l->next);
  gfc_free_equiv_infos (l->equiv);
  gfc_free (l);
}


/* Free a namespace structure and everything below it.  Interface
   lists associated with intrinsic operators are not freed.  These are
   taken care of when a specific name is freed.  */

void
gfc_free_namespace (gfc_namespace * ns)
{
  gfc_charlen *cl, *cl2;
  gfc_namespace *p, *q;
  gfc_intrinsic_op i;

  if (ns == NULL)
    return;

  ns->refs--;
  if (ns->refs > 0)
    return;
  gcc_assert (ns->refs == 0);

  gfc_free_statements (ns->code);

  free_sym_tree (ns->sym_root);
  free_uop_tree (ns->uop_root);
  free_common_tree (ns->common_root);

  for (cl = ns->cl_list; cl; cl = cl2)
    {
      cl2 = cl->next;
      gfc_free_expr (cl->length);
      gfc_free (cl);
    }

  free_st_labels (ns->st_labels);

  gfc_free_equiv (ns->equiv);
  gfc_free_equiv_lists (ns->equiv_lists);

  gfc_free_dt_list (ns->derived_types);

  for (i = GFC_INTRINSIC_BEGIN; i != GFC_INTRINSIC_END; i++)
    gfc_free_interface (ns->operator[i]);

  gfc_free_data (ns->data);
  p = ns->contained;
  gfc_free (ns);

  /* Recursively free any contained namespaces.  */
  while (p != NULL)
    {
      q = p;
      p = p->sibling;

      gfc_free_namespace (q);
    }
}


void
gfc_symbol_init_2 (void)
{

  gfc_current_ns = gfc_get_namespace (NULL, 0);
}


void
gfc_symbol_done_2 (void)
{

  gfc_free_namespace (gfc_current_ns);
  gfc_current_ns = NULL;
}


/* Clear mark bits from symbol nodes associated with a symtree node.  */

static void
clear_sym_mark (gfc_symtree * st)
{

  st->n.sym->mark = 0;
}


/* Recursively traverse the symtree nodes.  */

void
gfc_traverse_symtree (gfc_symtree * st, void (*func) (gfc_symtree *))
{
  if (st != NULL)
    {
      (*func) (st);

      gfc_traverse_symtree (st->left, func);
      gfc_traverse_symtree (st->right, func);
    }
}


/* Recursive namespace traversal function.  */

static void
traverse_ns (gfc_symtree * st, void (*func) (gfc_symbol *))
{

  if (st == NULL)
    return;

  if (st->n.sym->mark == 0)
    (*func) (st->n.sym);
  st->n.sym->mark = 1;

  traverse_ns (st->left, func);
  traverse_ns (st->right, func);
}


/* Call a given function for all symbols in the namespace.  We take
   care that each gfc_symbol node is called exactly once.  */

void
gfc_traverse_ns (gfc_namespace * ns, void (*func) (gfc_symbol *))
{

  gfc_traverse_symtree (ns->sym_root, clear_sym_mark);

  traverse_ns (ns->sym_root, func);
}


/* Return TRUE if the symbol is an automatic variable.  */
static bool
gfc_is_var_automatic (gfc_symbol * sym)
{
  /* Pointer and allocatable variables are never automatic.  */
  if (sym->attr.pointer || sym->attr.allocatable)
    return false;
  /* Check for arrays with non-constant size.  */
  if (sym->attr.dimension && sym->as
      && !gfc_is_compile_time_shape (sym->as))
    return true;
  /* Check for non-constant length character variables.  */
  if (sym->ts.type == BT_CHARACTER
      && sym->ts.cl
      && !gfc_is_constant_expr (sym->ts.cl->length))
    return true;
  return false;
}

/* Given a symbol, mark it as SAVEd if it is allowed.  */

static void
save_symbol (gfc_symbol * sym)
{

  if (sym->attr.use_assoc)
    return;

  if (sym->attr.in_common
      || sym->attr.dummy
      || sym->attr.flavor != FL_VARIABLE)
    return;
  /* Automatic objects are not saved.  */
  if (gfc_is_var_automatic (sym))
    return;
  gfc_add_save (&sym->attr, sym->name, &sym->declared_at);
}


/* Mark those symbols which can be SAVEd as such.  */

void
gfc_save_all (gfc_namespace * ns)
{

  gfc_traverse_ns (ns, save_symbol);
}


#ifdef GFC_DEBUG
/* Make sure that no changes to symbols are pending.  */

void
gfc_symbol_state(void) {

  if (changed_syms != NULL)
    gfc_internal_error("Symbol changes still pending!");
}
#endif


/************** Global symbol handling ************/


/* Search a tree for the global symbol.  */

gfc_gsymbol *
gfc_find_gsymbol (gfc_gsymbol *symbol, const char *name)
{
  gfc_gsymbol *s;

  if (symbol == NULL)
    return NULL;
  if (strcmp (symbol->name, name) == 0)
    return symbol;

  s = gfc_find_gsymbol (symbol->left, name);
  if (s != NULL)
    return s;

  s = gfc_find_gsymbol (symbol->right, name);
  if (s != NULL)
    return s;

  return NULL;
}


/* Compare two global symbols. Used for managing the BB tree.  */

static int
gsym_compare (void * _s1, void * _s2)
{
  gfc_gsymbol *s1, *s2;

  s1 = (gfc_gsymbol *)_s1;
  s2 = (gfc_gsymbol *)_s2;
  return strcmp(s1->name, s2->name);
}


/* Get a global symbol, creating it if it doesn't exist.  */

gfc_gsymbol *
gfc_get_gsymbol (const char *name)
{
  gfc_gsymbol *s;

  s = gfc_find_gsymbol (gfc_gsym_root, name);
  if (s != NULL)
    return s;

  s = gfc_getmem (sizeof (gfc_gsymbol));
  s->type = GSYM_UNKNOWN;
  s->name = gfc_get_string (name);

  gfc_insert_bbt (&gfc_gsym_root, s, gsym_compare);

  return s;
}


/**
 * Verifies that the given derived type symbol, <code>derived_sym</code>,
 * is interoperable with C.  This is necessary for any derived type that is
 * <code>BIND(C)</code> and for derived types that are parameters to
 * functions that are <code>BIND(C)</code>.  All fields of the derived
 * type are required to be interoperable, and are tested for such.  If
 * an error occurs, the errors are reported here, allowing for multiple
 * errors to be handled for a single derived type.
 *
 * @param derived_sym <code>#gfc_symbol</code> representing the derived type
 * to test for C interoperability.
 * @return None
 */
void verify_bind_c_derived_type(gfc_symbol *derived_sym)
{
   gfc_component *curr_comp = NULL;
   try is_c_interop = FAILURE;
   
   if(derived_sym == NULL)
      gfc_internal_error("verify_bind_c_derived_type(): Given symbol is "
                         "unexpectedly NULL");

   curr_comp = derived_sym->components;
   /* is this really an error??  --Rickett, 10.24.05 */
   if(curr_comp == NULL)
      gfc_error("No fields in the derived type %s at %C",
                derived_sym->name);

   /* initialize the derived type as being C interoperable.
    * if we find an error in the components, this will be set false
    */
   derived_sym->ts.is_c_interop = 1;
   
   /* loop through the list of components to verify that the kind of
    * each is a C interoperable type.
    */
   do
   {
      /* bind(c) derived types can't have derived types in them unless
       * they're c_ptr or c_funptr. J3/04-007, Section 15.2.3, C1502.
       */
      if(curr_comp->ts.type == BT_DERIVED &&
         curr_comp->ts.derived->ts.is_iso_c != 1)
      {
         gfc_error_now("BIND(C) derived type '%s' at %L can not have "
                       "the TYPE component '%s' at %L",
                       derived_sym->name, &(derived_sym->declared_at),
                       curr_comp->name, &(curr_comp->loc));
      }/* end if(type component inside bind(c) derived type) */
      else
      {
         /* grab the typespec for the given component and test the kind */ 
         is_c_interop = verify_c_interop(&(curr_comp->ts));

         if(is_c_interop != SUCCESS)
         {
            /* report warning and continue since not fatal. the
             * draft does specify a constraint that requires all fields
             * to interoperate, but if the user says real(4), etc., it
             * may interoperate with *something* in C, but the compiler
             * most likely won't know exactly what.  further, it may not
             * interoperate with the same data type(s) in C if the user
             * recompiles with different flags (e.g., -m32 and -m64 on
             * x86_64 and using integer(4) to claim interop with a
             * C_LONG).
             */
            if(derived_sym->attr.is_bind_c == 1)
               /* if the derived type is bind(c), all fields must be interop */
               gfc_warning_now("Component '%s' in derived type '%s' at %L "
                               "may not be C interoperable, even though "
                               "derived type '%s' is BIND(C)",
                               curr_comp->name, derived_sym->name,
                               &(curr_comp->loc), derived_sym->name);
            else
               /* if derived type is param to bind(c) routine, or to one
                * of the iso_c_binding procs, it must be interoperable, so
                * all fields must interop too.
                */
               gfc_warning_now("Component '%s' in derived type '%s' at %L "
                               "may not be C interoperable",
                               curr_comp->name, derived_sym->name,
                               &(curr_comp->loc));
            
            /* mark the derived type as not being C interoperable */
            derived_sym->ts.is_c_interop = 0;
         }/* end if(component isn't C interoperable) */

         /* the components can not be pointers (fortran sense) or
          * allocatable.  J3/04-007, Section 15.2.3, C1505
          */
         if(curr_comp->pointer != 0)
            gfc_error_now("Component '%s' at %L can not have the "
                          "POINTER attribute because it is a member "
                          "of the BIND(C) derived type '%s' at %L",
                          curr_comp->name, &(curr_comp->loc),
                          derived_sym->name, &(derived_sym->declared_at));
      }/* end else(was not a derived type, so checked the kind) */

      curr_comp = curr_comp->next;
   }while(curr_comp != NULL); /*  && isCInteroperable == SUCCESS); */
   
   /* make sure we don't have conflicts with the attributes */
   if(derived_sym->attr.access == ACCESS_PRIVATE)
      gfc_error_now("Derived type %s at %C can not be declared with both "
                    "PRIVATE and BIND(C) attributes", derived_sym->name);

   /* not sure what other attrs to check for conflicts with bind(c).
    * need to check the standard.  --Rickett, 10.24.05
    */

   if(derived_sym->attr.sequence != 0)
      gfc_error_now("Derived type '%s' at %L can not have the SEQUENCE "
                    "attribute because it is BIND(C)", derived_sym->name,
                    &(derived_sym->declared_at));

   /* make sure we clear out any buffered errors since we're not
    * returning a status to the caller about whether an error
    * was encountered or not
    */
/*    gfc_warning_check(); */
/*    gfc_error_check(); */
   
   return;
}/* end verify_bind_c_derived_type() */


/**
 * Generate a symbol for the given named constant that represents a
 * C interoperable kind.
 *
 * @param kind_obj <code>#CInteropKind_t</code> representing the C
 * interoperable kind.
 * @param module_name Name of the module that owns the symbol that
 * will be created: <code>iso_c_binding</code> (plus mangling).
 * @return None
 */
static void gen_c_interop_kind_sym(CInteropKind_t *kind_obj,
                                   const char *module_name)
{
   gfc_symbol *tmp_sym = NULL;
   gfc_symtree *tmp_symtree = NULL;

   /* generate symbols in the current namespace (scope) for the
    * iso_c_binding symbols, or just the symbols from the module
    * the user specifies (i.e., with the "only" clause)
    *
    * use the function gfc_get_sym_tree to force it to create
    * the symbol in the current namespace without trying to
    * traverse the parent scope(s) first!
    */
   tmp_symtree =
      gfc_find_symtree(gfc_current_ns->sym_root, kind_obj->name);
   
   if(tmp_symtree != NULL)
      /* already exists in this scope so don't re-add it */
      return;
   
   /* create the sym tree in the current ns */
   gfc_get_sym_tree(kind_obj->name, gfc_current_ns, &tmp_symtree);

   if(tmp_symtree != NULL)
      tmp_sym = tmp_symtree->n.sym;
   else
      gfc_internal_error("gen_c_interop_kind_sym(): Unable to "
                         "create symbol");

   /* initialize an integer constant expression node */
   tmp_sym->value = gfc_int_expr(kind_obj->value);
   
   /* make the variable a parameter so we can use it as kind param */
   tmp_sym->attr.flavor = FL_PARAMETER;
   /* have to setup the info in the symbol's typespec to match the
    * expr, otherwise the compiler will error due to the differences.
    */
   tmp_sym->ts.type = BT_INTEGER;
   /* set the kind of the integer to be the default kind, which is
    * a global variable, somewhere, defined in
    * trans-types.c: gfc_init_kinds()
    * --Rickett, 10.26.05
    */
   tmp_sym->ts.kind = gfc_default_integer_kind; 
   /* mark this type as a C interoperable one */
   tmp_sym->ts.is_c_interop = 1;
   tmp_sym->ts.is_iso_c = 1;
   tmp_sym->value->ts.is_c_interop = 1;   
   tmp_sym->value->ts.is_iso_c = 1;
   tmp_sym->attr.is_c_interop = 1;

   /* tell what f90 type this c interop kind is valid for */
   tmp_sym->ts.f90_type = kind_obj->f90_type;

   /* say what module this symbol belongs to */
   tmp_sym->module = gfc_get_string(module_name);

   /* say it's from the iso_c_binding module */
   tmp_sym->attr.is_iso_c = 1;

   /* make it use associated */
   tmp_sym->attr.use_assoc = 1;

   return;
}/* end gen_c_interop_kind_sym() */


/**
 * Generate symbol(s) for the given list of kind objects.  
 *
 * @param kinds_list List of names for the kinds that need symbols
 * generated for them.
 * @param module_name Name of the module that owns the symbols representing
 * the kind objects.
 * @return None
 */
static void gen_given_kinds(const char *kinds_list[],
                            const char *module_name)
{
   int i = 0;     /* loop variable */
   int index = 0;

   /* loop through given list of kinds and generate syms for them */
   while(kinds_list[i] != NULL)
   {
      index = get_c_kind(kinds_list[i], c_interop_kinds_table);
      gen_c_interop_kind_sym(&(c_interop_kinds_table[index]),
                             module_name);

      i++;
   }/* end while(int kinds exist to generate syms for) */
   
   return;
}/* end gen_given_kinds() */


/**
 * Generate symbols for the C interoperable integer kinds.  Currently,
 * these include:
 * <code>c_int, c_short, c_long, c_long_long, c_size_t</code>.
 *
 * @param module_name Name of the module that the created symbols are
 * from (<code>iso_c_binding</code> plus mangling).
 * @return None
 */
static void gen_c_interop_int_kinds(const char *module_name)
{
   /* array of the valid C kinds constant names */
   const char *int_kinds[] = {"c_int",
                              "c_short",
                              "c_long",
                              "c_long_long",
                              "c_size_t",
                              "c_int8_t",
                              "c_int16_t",
                              "c_int32_t",
                              "c_int64_t",
                              "c_int_least8_t",
                              "c_int_least16_t",
                              "c_int_least32_t",
                              "c_int_least64_t",
                              "c_int_fast8_t",
                              "c_int_fast16_t",
                              "c_int_fast32_t",
                              "c_int_fast64_t",
                              "c_intmax_t",
                              "c_intptr_t",
			      "c_signed_char",
                              NULL};

   gen_given_kinds(int_kinds, module_name);
   
   return;
}/* end gen_c_interop_int_kinds() */


/**
 * Generate a symbol for the C interoperable logical kind.  Currently,
 * it is:
 * <code>c_bool</code>.
 *
 * @param module_name Name of the module that the created symbols are
 * from (<code>iso_c_binding</code> plus mangling).
 * @return None
 */
static void gen_c_interop_logical_kinds(const char *module_name)
{
   /* array of the valid C logicalkinds constant names */
   const char *logical_kinds[] = {"c_bool",
                                  NULL};

   gen_given_kinds(logical_kinds, module_name);
   
   return;
}/* end gen_c_interop_logical_kinds() */


/**
 * Generate symbols for the C interoperable real kinds.  Currently, these
 * include:
 * <code>c_float, c_double, c_long_double</code>.
 *
 * @param module_name Name of the module that the created symbols are
 * from (<code>iso_c_binding</code> plus mangling).
 * @return None
 */
static void gen_c_interop_real_kinds(const char *module_name)
{
   /* array of the valid kinds for C reals */
   const char *real_kinds[] = {"c_float",
                               "c_double",
                               "c_long_double",
			       "c_float_complex",
			       "c_double_complex",
			       "c_long_double_complex",
                               NULL};

   gen_given_kinds(real_kinds, module_name);
   
   return;
}/* end gen_c_interop_real_kinds() */


/**
 * Generate symbols for kinds representing special characters, such
 * as <code>C_NULL_CHAR</code>.
 *
 * @param module_name Name of the module that the created symbols are
 * from (<code>iso_c_binding</code> plus mangling).
 * @return None
 */
static void gen_special_c_chars(const char *char_names[],
                                const char *module_name)
{
   int i = 0;   /* loop variable */
   
   /* go through the list of special chars.  they all are of type
    * character(C_CHAR), with a length of 1
    */
   while(char_names[i] != NULL && module_name)
   {
         
      i++;
   }/* end while(more names) */
   
   return;
}/* end gen_special_c_chars() */


/**
 * Generate symbols for the C interoperable character kinds.  Currently,
 * these include:
 * <code>c_char, c_null_char</code>.
 *
 * @param module_name Name of the module that the created symbols are
 * from (<code>iso_c_binding</code> plus mangling).
 * @return None
 */
static void gen_c_interop_char_kinds(const char *module_name)
{
   const char *char_kinds[] = {"c_char", NULL};
   const char *special_c_chars[] = {"c_null_char", NULL};

   gen_given_kinds(char_kinds, module_name);

   /* generate the special characters from C */
   gen_special_c_chars(special_c_chars, module_name);
   
   return;
}/* end gen_c_interop_char_kinds() */


/**
 * Generate symbols for the given list of C interoperable pointer kinds.  
 *
 * @param ptr_kinds Names of the pointer kinds to generate symbols for.
 * @param module_name Name of the module that the created symbols are
 * from (<code>iso_c_binding</code> plus mangling).
 * @return None
 */
static void gen_c_interop_ptr_syms(const char *ptr_kinds[],
                                   const char *module_name)
{
   gfc_symbol *tmp_sym = NULL;       /* tmp sym for creating ptr sym */
   gfc_component *tmp_comp = NULL;   /* component for c_address field */
   int i = 0;                        /* loop variable */
   gfc_symtree *tmp_symtree = NULL;  /* tmp symtree for ptr symtree */
   /* component's name, which will be a combination of the derived
    * type name and field name, so it's twice the size
    */
   char comp_name[(GFC_MAX_SYMBOL_LEN * 2) + 1];
   int index = -1;

   while(ptr_kinds[i] != NULL)
   {
      /* look for symtree in current ns */
      tmp_symtree =
         gfc_find_symtree(gfc_current_ns->sym_root, ptr_kinds[i]);
      if(tmp_symtree == NULL)
      {
         /* use the function gfc_get_sym_tree to force it to create
          * the symbol in the current namespace.
          */
         gfc_get_sym_tree(ptr_kinds[i], gfc_current_ns, &tmp_symtree);
         
         if(tmp_symtree != NULL)
            tmp_sym = tmp_symtree->n.sym;
         else
         {
            tmp_sym = NULL;
            gfc_internal_error("generateCInteropPtrKinds(): Unable to "
                               "create symbol for %s", ptr_kinds[i]);
         }

         /* set up the symbol's important fields */
         tmp_sym->attr.flavor = FL_DERIVED;
         tmp_sym->ts.is_c_interop = 1;
         tmp_sym->attr.is_c_interop = 1;
         tmp_sym->ts.is_iso_c = 1;
         tmp_sym->ts.type = BT_DERIVED; 
         /* c_ptr and c_funptr are derived types.
          * the ts.derived field will be accessed for the components.
          */
         tmp_sym->ts.derived = tmp_sym; 
         /* module name is some mangled version of iso_c_binding */
         tmp_sym->module = gfc_get_string(module_name);

         /* say it's from the iso_c_binding module. */
         tmp_sym->attr.is_iso_c = 1;
         
         /* set up the component of the derived type, which will be
          * an integer with kind equal to c_ptr_size
          *
          * mangle the name of the field for the c_address to prevent
          * the curious user from trying to access it from Fortran.
          */
         sprintf(comp_name, "__%s_%s", tmp_sym->name, "c_address");
         gfc_add_component(tmp_sym, comp_name, &tmp_comp);
         if(tmp_comp == NULL)
            gfc_internal_error("generateCInteropPtrKinds(): Unable to "
                               "create component for c_address");
         tmp_comp->ts.type = BT_INTEGER;
         /* the kinds for c_ptr and c_funptr are the same. */
         index = get_c_kind("c_ptr", c_interop_kinds_table);
         tmp_comp->ts.kind = c_interop_kinds_table[index].value;
         
         tmp_comp->pointer = 0;
         tmp_comp->dimension = 0;
         /* initialize the value of the c_address field to NULL */
         tmp_comp->initializer = gfc_int_expr(0);

         /* make it use associated (iso_c_binding module) */
         tmp_sym->attr.use_assoc = 1;
      }/* end if(ptr kind symtree doesn't exist in scope yet) */

      i++;
   }/* end while(more ptr kinds to create) */
      
   return;
}/* end gen_c_interop_ptr_syms() */


/**
 * Generate symbols for the named constants <code>c_null_ptr</code> and
 * <code>c_null_funptr</code>.  
 *
 * @param kinds List of pointer names provided either by the user with
 * an 'only' clause, or the default of both <code>c_ptr</code> and
 * <code>c_funptr</code>.  The name(s) given determine with NULL types
 * to generate.  If <code>c_ptr</code> is given, then <code>c_null_ptr</code>
 * is created; same idea for <code>c_funptr</code>.
 */
static void gen_special_c_interop_ptrs(const char *kinds[],
                                       const char *module_name)
{
   int i;
   gfc_symtree *tmp_symtree;
   gfc_symbol *tmp_sym;

   /* there should be only two possible ptr kinds; c_ptr and c_funptr */
   i = 0;
   while(kinds[i] != NULL)
   {
      /* here, we know that we can only be given valid iso_c_binding
       * constant names, because error checking already done.
       * so if we don't have one, it must be the other.
       */
      if(strcmp(kinds[i], "c_ptr") == 0)
         /* look for symtree for c_null_ptr */
         tmp_symtree =
            gfc_find_symtree(gfc_current_ns->sym_root, "c_null_ptr");
      else
         /* look for symtree for c_null_funptr */
         tmp_symtree =
            gfc_find_symtree(gfc_current_ns->sym_root, "c_null_funptr");
      if(tmp_symtree == NULL)
      {
         /* use the function gfc_get_sym_tree to force it to create
          * the symbol in the current namespace.
          */
         if(strcmp(kinds[i], "c_ptr") == 0)
            gfc_get_sym_tree("c_null_ptr", gfc_current_ns,
                             &tmp_symtree);
         else
            gfc_get_sym_tree("c_null_funptr", gfc_current_ns,
                             &tmp_symtree);
         
         if(tmp_symtree != NULL)
            tmp_sym = tmp_symtree->n.sym;
         else
         {
            tmp_sym = NULL;
            gfc_internal_error("gen_special_c_interop_ptrs(): Unable to "
                               "create symbol for %s", kinds[i]);
         }

         /* set up the symbol's important fields. */
         /* FIXME: this is causing the compiler to ICE with a seg'v..
          * --Rickett, 09.15.06
          */
/*          tmp_sym->attr.flavor = FL_PARAMETER; */
         /* save attr required so we can initialize the ptr to NULL */
         tmp_sym->attr.save = 1;
         tmp_sym->ts.is_c_interop = 1;
         tmp_sym->attr.is_c_interop = 1;
         tmp_sym->ts.is_iso_c = 1;
         tmp_sym->ts.type = BT_DERIVED;
         /* the c_ptr and c_funptr derived types will provide the
          * definition for c_null_ptr and c_null_funptr, respectively.
          */
         gfc_find_symbol(kinds[i], gfc_current_ns, 1,
                         &(tmp_sym->ts.derived));
         if(tmp_sym->ts.derived == NULL)
            gfc_error("'%s' at %C requires '%s' to be defined",
                      tmp_sym->name, kinds[i]);
            
         /* module name is some mangled version of iso_c_binding */
         tmp_sym->module = gfc_get_string(module_name);

         /* say it's from the iso_c_binding module. */
         tmp_sym->attr.is_iso_c = 1;

         tmp_sym->attr.use_assoc = 1;
         /* make them bind(c) because they are defined in the
          * iso_c_binding.h header file in the libgfortran intrinsics
          * dir. the defn. of c_null_ptr and c_null_funptr will be
          * pulled in at link time.
          */
         tmp_sym->attr.is_bind_c = 1;
         /* set the binding_label */
         sprintf(tmp_sym->binding_label, "%s_%s", module_name,
                 tmp_sym->name);

         /* the c_address field held by the derived types c_ptr
          * and c_funptr is initialized to 0 by default when they
          * are created.  see gen_c_interop_ptr_syms().
          */
      }/* end if(symbol hasn't already been created) */

      /* while loop counter */
      i++;
   }/* end while(more ptr names in list) */
   
   return;
}/* end gen_special_c_interop_ptrs() */

/**
 * Generate symbols for all C interoperable pointer kinds, or the
 * given ones, if a list is provided.  Currently, these include:
 * <code>c_ptr, c_funptr</code>.
 *
 * @param module_name Name of the module that the created symbols are
 * from (<code>iso_c_binding</code> plus mangling).
 * @param given_kinds Names of the pointer kinds to generate symbols for.
 * @return None
 */
static void gen_c_interop_ptr_kinds(const char *module_name,
                                    const char *given_kinds[])
{
   const char *ptr_kinds[] = {"c_ptr",
                              "c_funptr",
                              NULL};

   /* the ptrs are derived types, and different symbols must be
    * made for them from the named constants for other kinds.
    * generate the symbols for the ptr derived types
    */
   if(given_kinds != NULL)
   {
      gen_c_interop_ptr_syms(given_kinds, module_name);
      gen_special_c_interop_ptrs(given_kinds, module_name);
   }/* end if(user provided specific list, i.e., 'only:' clause) */
   else
   {
      gen_c_interop_ptr_syms(ptr_kinds, module_name);
      gen_special_c_interop_ptrs(ptr_kinds, module_name);
   }/* end else(gen both ptrs) */
   
   return;
}/* end gen_c_interop_ptr_kinds() */


/**
 * Add a formal argument, <code>#gfc_formal_arglist</code>, to the
 * end of the given list of arguments.  Set the reference to the
 * provided symbol, <code>param_sym</code>, in the argument.
 *
 * @param head Head of the list of arguments.
 * @param tail Tail of the list of arguments.
 * @param formal_arg Argument to add to the tail of the list.
 * @param param_sym Symbol that this argument represents.  Reference to
 * this symbol is stored in <code>formal_arg</code>.
 * @return None
 */
static void add_formal_arg(gfc_formal_arglist **head,
                           gfc_formal_arglist **tail,
                           gfc_formal_arglist *formal_arg,
                           gfc_symbol *param_sym)
{
   /* put in list, either as first arg or at the tail (curr arg) */
   if(*head == NULL)
      *head = *tail = formal_arg;
   else
   {
      (*tail)->next = formal_arg;
      (*tail) = formal_arg;
   }
   
   (*tail)->sym = param_sym;
   (*tail)->next = NULL;
   
   return;
}/* end add_formal_arg() */


/**
 * Generates a symbol representing the <code>CPTR</code> argument to
 * an <code>iso_c_binding</code> procedure.  Also, create a
 * <code>#gfc_formal_arglist</code> for the <code>CPTR</code> and add
 * it to the provided argument list.
 *
 * @param head Head of the list of arguments to add the <code>CPTR</code>
 * argument to.
 * @param tail Tail of the list of arguments to add the <code>CPTR</code>
 * argument to.
 * @param module_name Name of the module that <code>CPTR</code> belongs to.
 * @param ns Namespace to add the argument to.
 * @return None
 */
static void gen_cptr_param(gfc_formal_arglist **head,
                           gfc_formal_arglist **tail,
                           const char *module_name,
                           gfc_namespace *ns, const char *c_ptr_name)
{
   gfc_symbol *param_sym = NULL;
   gfc_symbol *c_ptr_sym = NULL;
   gfc_symtree *param_symtree = NULL;
   gfc_symtree *c_ptr_symtree = NULL;
   gfc_formal_arglist *formal_arg = NULL;
   const char *c_ptr_in;
   const char *c_ptr_type = "c_ptr";

   if(c_ptr_name == NULL)
      c_ptr_in = "gfc_cptr__";
   else
      c_ptr_in = c_ptr_name;
   gfc_get_sym_tree(c_ptr_in, ns, &param_symtree);
   if(param_symtree != NULL)
      param_sym = param_symtree->n.sym;
   else
      gfc_internal_error("gen_cptr_param(): Unable to "
                         "create symbol for %s", c_ptr_in);
   /* set up the appropriate fields for the new c_ptr param sym */
   param_sym->refs++;
   param_sym->attr.flavor = FL_DERIVED;
   param_sym->ts.type = BT_DERIVED;
   param_sym->attr.intent = INTENT_IN;
   param_sym->attr.dummy = 1;
   /* this will pass the ptr to the iso_c routines as a (void *) */
   param_sym->attr.value = 1;
   param_sym->attr.use_assoc = 1;
   /* get the symbol definition for the c_ptr derived type */
   c_ptr_symtree = gfc_find_symtree(gfc_current_ns->sym_root,
                                    c_ptr_type);
   /* if we're in an 'only' clause and the user lists one of the
    * procedures, we may not have the c_ptr (and c_funptr) defn's
    * available to us yet, so make sure they're generated if needed
    * --Rickett, 06.20.06
    */
   if(c_ptr_symtree == NULL)
      gen_c_interop_ptr_kinds(module_name, NULL);
   gfc_get_ha_symbol(c_ptr_type, &(c_ptr_sym));
   param_sym->ts.derived = c_ptr_sym;
   param_sym->module = gfc_get_string(module_name);

   /* make new formal arg */
   formal_arg = gfc_get_formal_arglist();
   /* add arg to list of formal args (the CPTR arg) */
   add_formal_arg(head, tail, formal_arg, param_sym);
   
   return;
}/* end gen_cptr_param() */


/**
 * Generates a symbol representing the <code>FPTR</code> argument to
 * an <code>iso_c_binding</code> procedure.  Also, create a
 * <code>#gfc_formal_arglist</code> for the <code>FPTR</code> and add
 * it to the provided argument list.
 *
 * @param head Head of the list of arguments to add the <code>FPTR</code>
 * argument to.
 * @param tail Tail of the list of arguments to add the <code>FPTR</code>
 * argument to.
 * @param module_name Name of the module that <code>FPTR</code> belongs to.
 * @param ns Namespace to add the argument to.
 * @return None
 */
static void gen_fptr_param(gfc_formal_arglist **head,
                           gfc_formal_arglist **tail,
                           const char *module_name,
                           gfc_namespace *ns)
{
   gfc_symbol *param_sym = NULL;
   gfc_symtree *param_symtree = NULL;
   gfc_formal_arglist *formal_arg = NULL;
   const char *f_ptr_out = "gfc_fptr__";

   gfc_get_sym_tree(f_ptr_out, ns, &param_symtree);
   if(param_symtree != NULL)
      param_sym = param_symtree->n.sym;
   else
      gfc_internal_error("generateFPtrParam(): Unable to "
                         "create symbol for %s", f_ptr_out);
   /* set up the necessary fields for the fptr output param sym */
   param_sym->refs++;
   param_sym->attr.pointer = 1;
   param_sym->attr.dummy = 1;
   param_sym->attr.use_assoc = 1;
   /* my own type, to allow any pointer type as actual param */
   param_sym->ts.type = BT_VOID;
   param_sym->module = gfc_get_string(module_name);
   
   /* make the arg */
   formal_arg = gfc_get_formal_arglist();
   /* add arg to list of formal args */
   add_formal_arg(head, tail, formal_arg, param_sym);
   
   return;
}/* end gen_fptr_param() */


/**
 * Generates a symbol representing the optional <code>SHAPE</code>
 * argument for the <code>iso_c_binding</code> <code>c_f_pointer()</code>
 * procedure.  Also, create a <code>#gfc_formal_arglist</code> for
 * the <code>SHAPE</code> and add it to the provided argument list.
 *
 * @param head Head of the list of arguments to add the <code>SHAPE</code>
 * argument to.
 * @param tail Tail of the list of arguments to add the <code>SHAPE</code>
 * argument to.
 * @param module_name Name of the module that <code>SHAPE</code> belongs to.
 * @param ns Namespace to add the argument to.
 * @return None
 */
static void gen_shape_param(gfc_formal_arglist **head,
                            gfc_formal_arglist **tail,
                            const char *module_name,
                            gfc_namespace *ns)
{
   gfc_symbol *param_sym = NULL;
   gfc_symtree *param_symtree = NULL;
   gfc_formal_arglist *formal_arg = NULL;
   const char *shape_param = "gfc_shape_array__";
   int i;

   gfc_get_sym_tree(shape_param, ns, &param_symtree);
   if(param_symtree != NULL)
      param_sym = param_symtree->n.sym;
   else
      gfc_internal_error("generateShapeParam(): Unable to "
                         "create symbol for %s", shape_param);
   
   /* set up the necessary fields for the shape input param sym */
   param_sym->refs++;
   param_sym->attr.dummy = 1;
   param_sym->attr.use_assoc = 1;
   /* integer array, rank 1, describing the shape of the object */
   param_sym->ts.type = BT_INTEGER;
   param_sym->ts.kind = gfc_default_integer_kind;   
   param_sym->as = gfc_get_array_spec();
   /* clear out the dimension info for the array */
   for(i = 0; i < GFC_MAX_DIMENSIONS; i++)
   {
      param_sym->as->lower[i] = NULL;
      param_sym->as->upper[i] = NULL;
   }/* end for(each possible dimension) */
   param_sym->as->rank = 1;
   param_sym->as->lower[0] = gfc_int_expr(1);
   /* the extent is unknown until we get it.  the length give us
    * the rank the incoming pointer
    */
   param_sym->as->type = AS_ASSUMED_SHAPE;
   /* the arg is also optional; it is required iff the second arg
    * (fptr) is to an array, otherwise, it's ignored
    */
   param_sym->attr.optional = 1;
   param_sym->attr.intent = INTENT_IN;
   param_sym->attr.dimension = 1;
   param_sym->module = gfc_get_string(module_name);
   
   /* make the arg */
   formal_arg = gfc_get_formal_arglist();
   /* add arg to list of formal args */
   add_formal_arg(head, tail, formal_arg, param_sym);
   
   return;
}/* end gen_shape_param() */

/**
 * Add a procedure interface to the given symbol (i.e., store a
 * reference to the list of formal arguments).
 *
 * @param sym Symbol to add the procedure interface to.
 * @param source
 * @param formal Argument list to add to the symbol.
 * @return None
 */
static void add_proc_interface(gfc_symbol * sym, ifsrc source,
                               gfc_formal_arglist * formal)
{
   sym->formal = formal;
   sym->attr.if_source = source;
   
   return;
}/* end add_proc_interface() */


/**
 * Builds the parameter list for the <code>iso_c_binding</code> procedure 
 * c_f_pointer or c_f_procpointer.  
 *
 * @param new_proc_sym The symbol the parameter list is being created for.
 * @param old_sym The symbol that the new procedure symbol is based on.
 * @note The <code>old_sym</code> typically refers to a generic version of either the
 * <code>c_f_pointer</code> or <code>c_f_procpointer</code> functions.  The
 * <code>new_proc_sym</code> represents a "resolved" version of the symbol.
 * The functions are resolved to match the types of their parameters; for
 * example, <code>c_f_pointer(cptr, fptr)</code> would resolve to something
 * similar to <code>c_f_pointer_i4</code> if the type of data object
 * <code>fptr</code> pointed to was a default integer.  The actual name
 * of the resolved procedure symbol is further mangled with the module
 * name, etc., but the idea holds true.  
 * @note This function can ONLY be called if the procedure represented 
 * by new_proc_sym is derived from c_f_pointer or c_f_procpointer!
 * @return None
 */
static void build_formal_args(gfc_symbol *new_proc_sym,
                              gfc_symbol *old_sym, int add_optional_arg)
{
   gfc_formal_arglist *head = NULL, *tail = NULL;
   gfc_namespace *parent_ns = NULL;

   parent_ns = gfc_current_ns;
   /* create a new namespace, which will be the formal ns (namespace
    * of the formal args).
    */
   gfc_current_ns = gfc_get_namespace(parent_ns, 0);
   gfc_current_ns->proc_name = new_proc_sym;

   /* generate the params */
   if((strcmp(old_sym->name, "c_f_pointer") == 0) ||
      (strcmp(old_sym->name, "c_f_procpointer") == 0))
   {
      gen_cptr_param(&head, &tail, (const char *)new_proc_sym->module,
                     gfc_current_ns, NULL);
      gen_fptr_param(&head, &tail, (const char *)new_proc_sym->module,
                     gfc_current_ns);

      /* if we're dealing with c_f_pointer, it has an optional third arg */
      if(old_sym != NULL && strcmp(old_sym->name, "c_f_pointer") == 0)
      {
         gen_shape_param(&head, &tail,
                         (const char *)new_proc_sym->module,
                         gfc_current_ns);
      }/* end if(building formal args for c_f_pointer) */
   }/* end if(c_f_pointer or c_f_procpointer) */
   else if(strcmp(old_sym->name, "c_associated") == 0)
   {
      /* c_associated has one required arg and one optional.  both
       * are c_ptrs.
       */
      gen_cptr_param(&head, &tail, (const char *)new_proc_sym->module,
                     gfc_current_ns, "c_ptr_1");
      if(add_optional_arg)
      {
         gen_cptr_param(&head, &tail, (const char *)new_proc_sym->module,
                        gfc_current_ns, "c_ptr_2");
         /* the last param is optional so mark it as such */
         tail->sym->attr.optional = 1;
      }/* end if(need the second c_ptr, which is optional) */
   }/* end else if(c_associated) */

   /* add the interface (store formal args to new_proc_sym) */
   add_proc_interface(new_proc_sym, IFSRC_DECL, head);

   /* set up the formal_ns pointer to the one created for the
    * new procedure so it'll get cleaned up during gfc_free_symbol()
    */
   new_proc_sym->formal_ns = gfc_current_ns;

   gfc_current_ns = parent_ns;
   
   return;
}/* end build_formal_args() */


/**
 * Generate a symbol for a given <code>iso_c_binding</code> subroutine 
 * in the current namespace.  The interface will be created to the
 * given subroutine (argument list will be set up).
 *
 * @param module_name Name of the module that the subroutine is a part of
 * (<code>iso_c_binding</code> plus mangling).
 * @param proc_sym_name Name of the subroutine to generate a symbol for.
 * @param ns Namespace to place the subroutine symbol into.
 * @return None
 * @note This function is for the subroutines
 * <code>c_f_pointer</code> and <code>c_f_procpointer</code> only, because
 * it also handles generating the various parameters.  It could be used
 * to generate other <code>iso_c_binding procedures</code>, such as
 * <code>c_associated</code> but it currently is not.
 */
static void gen_iso_c_proc(const char *module_name,
                           const char *proc_sym_name,
                           gfc_namespace *ns)
{
   gfc_symbol *proc_sym = NULL;
   gfc_symtree *proc_symtree = NULL;
   gfc_namespace *save_curr_ns = NULL;

   /* look for the iso_c procedure symbol in the current ns */
   proc_symtree = gfc_find_symtree(gfc_current_ns->sym_root,
                                   proc_sym_name);
   if(proc_symtree != NULL)
      /* already has been entered in this scope */
      return;
   
   /* create the symbol in the current namespace w/o looking in
    * parent namespace for it.
    */
   gfc_get_sym_tree(proc_sym_name, gfc_current_ns, &proc_symtree);
   
   if(proc_symtree != NULL)
      proc_sym = proc_symtree->n.sym;
   else
      gfc_internal_error("generateIsoCProc(): Unable to "
                         "create symbol for %s", proc_sym_name);

   /* set up the fields for the procedure */
   proc_sym->attr.proc = PROC_MODULE;
   /* i don't think the functions/subroutines themselves are intrinsics.
    * they are available if the user uses the intrinsic module only,
    * but not all the time by default.  --Rickett, 12.13.05
    */
   sprintf(proc_sym->binding_label, "%s_%s", module_name, proc_sym_name);
   proc_sym->module = gfc_get_string(module_name);
   /* say it's from the iso_c_binding module for resolving */
   proc_sym->attr.is_iso_c = 1;
   if(strcmp(proc_sym->name, "c_f_pointer") == 0 ||
      strcmp(proc_sym->name, "c_f_procpointer") == 0)
   {
      proc_sym->attr.subroutine = 1;
   }/* end if(proc sym is one of the iso c subroutines) */
   else
   {
      /* TODO!!! this needs to be finished more for the expr of the
       * function or something!!!  --Rickett, 08.10.06
       */
      /* this may not need to be here, because trying to do c_loc
       * as an external...  --Rickett, 08.24.06
       */
      if(strcmp(proc_sym->name, "c_associated") == 0)
      {
         proc_sym->attr.function = 1;
         /* return type of c_associated is a logical */
         proc_sym->ts.type = BT_LOGICAL;
         proc_sym->ts.kind = gfc_default_logical_kind;
         proc_sym->result = proc_sym;
      }/* end if(c_associated) */
      else if(strcmp(proc_sym->name, "c_loc") == 0)
      {
         /* here, we're taking the simple approach.  we're defining
          * c_loc as an external identifier so the compiler will put
          * what we expect on the stack for the address we want the
          * C address of.
          */
         /* returns a C_PTR derived type */
         proc_sym->ts.type = BT_DERIVED;
         /* look for a defn of c_ptr, including any parent ns if needed */
         gfc_find_symbol("c_ptr", gfc_current_ns, 1,
                         &(proc_sym->ts.derived));
         if(proc_sym->ts.derived == NULL)
            gfc_error_now("Type 'C_PTR' is not defined but is needed "
                          "for the return value of 'C_LOC' at %C");
         /* the function result is itself (no result clause) */
         proc_sym->result = proc_sym;
         proc_sym->attr.external = 1;
         proc_sym->attr.use_assoc = 0;
         proc_sym->attr.if_source = IFSRC_UNKNOWN;
         proc_sym->attr.proc = PROC_UNKNOWN;
      }/* end else if(c_loc) */
   }/* end else(proc_sym is for a function) */
      
   proc_sym->attr.flavor = FL_PROCEDURE;
   
   proc_sym->attr.contained = 0;

   /* we need to make the curr ns the ns that'll hold the proc sym */
   save_curr_ns = gfc_current_ns;
   gfc_current_ns = ns;

   /* try using this builder routine, with the new and old symbols
    * both being the generic iso_c proc sym being created.
    * this will create the formal args (and the new namespace for them).
    */
   /* don't build an arg list for c_loc because we're going to treat c_loc
    * as an external procedure.
    */
   if(strcmp(proc_sym->name, "c_loc") != 0)
      /* the 1 says to add any optional args, if applicable */
      build_formal_args(proc_sym, proc_sym, 1);

   /* restore the current namespace to what it was coming in */
   gfc_current_ns = save_curr_ns;

   /* set this after setting up the symbol, to prevent error messages */
   proc_sym->attr.use_assoc = 1;
   /* this symbol will not be referenced directly.  it will be
    * resolved to the implementation for the given f90 kind.
    */
   proc_sym->attr.referenced = 0;

   return;
}/* end gen_iso_c_proc() */


/**
 * Generate symbols for all procedures provided by the intrinsic module
 * <code>iso_c_binding</code>, or the given one (<code>given_proc</code>),
 * if provided.  Currently, the following are created:
 * <code>c_f_pointer, c_f_procpointer</code>.
 *
 * @param module_name Name of the module the procedures belong to
 * (<code>iso_c_binding</code> plus mangling).
 * @param given_proc Name of the specific procedure to create a symbol for.
 * @return None
 */
static void gen_c_interop_int_funcs(const char *module_name,
                                    const char *given_proc)
{
   const char *proc_names[] = {"c_f_pointer",
                               "c_f_procpointer",
                               "c_associated",
                               "c_loc",
                               NULL};
   int i;

   /* if we have a specific proc to generate */
   if(given_proc != NULL)
   {
      gen_iso_c_proc(module_name, given_proc, gfc_current_ns);
   }/* end if(generate specific proc) */
   else
   {
      i = 0;
      while(proc_names[i] != NULL)
      {
         /* generate all supported iso_c procs */
         gen_iso_c_proc(module_name, proc_names[i], gfc_current_ns);
         i++;
      }/* end while(have a list of proc's to generate) */
   }/* end else(generate all procs) */
   
   return;
}/* end gen_c_interop_int_funcs() */


/**
 * Generate the given set of C interoperable kind objects, or all
 * interoperable kinds.  This function will only be given kind objects
 * for valid <code>iso_c_binding</code> defined types because this is
 * verified when the 'use' statement is parsed.  If the user gives an
 * 'only' clause, the specific kinds are looked up; if they don't exist,
 * an error is reported.  If the user does not give an 'only' clause, all
 * iso_c_binding symbols are generated.  If a list of specific kinds is
 * given, it must have a <code>NULL</code> in the first empty spot to
 * mark the end of the list.  
 *
 * @param mod_name Module that the kind objects (named constants)
 * belong to.
 * @param kinds List of <code>CInteropKind_t</code> objects to use
 * in generating symbols for specific kinds only.
 * @return None
 */


void
generate_isocbinding_symbol (const char * mod_name, iso_c_binding_symbol s,
			     char * local_name)
{
  char * name = (local_name && local_name[0]) ? local_name
		: c_interop_kinds_table[s].name;
  gfc_symtree * tmp_symtree = NULL;
  gfc_symbol * tmp_sym = NULL;
  gfc_component * tmp_comp = NULL;
  char comp_name[(GFC_MAX_SYMBOL_LEN * 2) + 1];
  int index;

#if 0
  gfc_namespace *save_curr_ns;
#endif

  tmp_symtree = gfc_find_symtree (gfc_current_ns->sym_root, name);

  /* already exists in this scope so don't re-add it
     TODO: we should probably check that it's really the same symbol */
  if (tmp_symtree != NULL)
    return;

  /* create the sym tree in the current ns */
  gfc_get_sym_tree (name, gfc_current_ns, &tmp_symtree);
  if (tmp_symtree)
    tmp_sym = tmp_symtree->n.sym;
  else
    gfc_internal_error ("generate_isocbinding_symbol(): Unable to "
			"create symbol");

  /* say what module this symbol belongs to */
  tmp_sym->module = gfc_get_string (mod_name);

  switch (s)
    {

#define NAMED_INTCST(a,b,c) case a :
#define NAMED_REALCST(a,b,c) case a :
#define NAMED_CMPXCST(a,b,c) case a :
#define NAMED_LOGCST(a,b,c) case a :
#define NAMED_CHARKNDCST(a,b,c) case a :
#include "iso-c-binding.def"

	tmp_sym->value = gfc_int_expr (c_interop_kinds_table[s].value);

	/* initialize an integer constant expression node */
	tmp_sym->attr.flavor = FL_PARAMETER;
	tmp_sym->ts.type = BT_INTEGER;
	tmp_sym->ts.kind = gfc_default_integer_kind;

	/* mark this type as a C interoperable one */
	tmp_sym->ts.is_c_interop = 1;
	tmp_sym->ts.is_iso_c = 1;
	tmp_sym->value->ts.is_c_interop = 1;
	tmp_sym->value->ts.is_iso_c = 1;
	tmp_sym->attr.is_c_interop = 1;

	/* tell what f90 type this c interop kind is valid for */
	tmp_sym->ts.f90_type = c_interop_kinds_table[s].f90_type;

	/* say it's from the iso_c_binding module */
	tmp_sym->attr.is_iso_c = 1;

	/* make it use associated */
	tmp_sym->attr.use_assoc = 1;
	break;


#define NAMED_CHARCST(a,b,c) case a :
#include "iso-c-binding.def"

	/* Initialize an integer constant expression node for the
	   length of the character.  */
	tmp_sym->value = gfc_get_expr (); 
	tmp_sym->value->expr_type = EXPR_CONSTANT;
	tmp_sym->value->ts.type = BT_CHARACTER;
	tmp_sym->value->ts.kind = gfc_default_character_kind;
	tmp_sym->value->where = gfc_current_locus;
	tmp_sym->value->ts.is_c_interop = 1;
	tmp_sym->value->ts.is_iso_c = 1;
	tmp_sym->value->value.character.length = 1;
	tmp_sym->value->value.character.string = gfc_getmem (2);
	tmp_sym->value->value.character.string[0]
	  = (char) c_interop_kinds_table[s].value;
	tmp_sym->value->value.character.string[1] = '\0';

	/* may not need this in both attr and ts, but do need in
	   attr for writing module file.  --Rickett, 02.07.07  */
	tmp_sym->attr.is_c_interop = 1;

	tmp_sym->attr.flavor = FL_PARAMETER;
	tmp_sym->ts.type = BT_CHARACTER;

	/* Need to set it to the C_CHAR kind.  */
	tmp_sym->ts.kind = gfc_default_character_kind;

	/* mark this type as a C interoperable one */
	tmp_sym->ts.is_c_interop = 1;
	tmp_sym->ts.is_iso_c = 1;

	/* tell what f90 type this c interop kind is valid for */
	tmp_sym->ts.f90_type = BT_CHARACTER;

	/* say it's from the iso_c_binding module */
	tmp_sym->attr.is_iso_c = 1;

	/* make it use associated */
	tmp_sym->attr.use_assoc = 1;
	break;

      case ISOCBINDING_PTR:
      case ISOCBINDING_FUNPTR:

	/* initialize an integer constant expression node */
	tmp_sym->attr.flavor = FL_DERIVED;
	tmp_sym->ts.is_c_interop = 1;
	tmp_sym->attr.is_c_interop = 1;
	tmp_sym->attr.is_iso_c = 1;
	tmp_sym->ts.is_iso_c = 1;
	tmp_sym->ts.type = BT_DERIVED;

	tmp_sym->ts.derived = tmp_sym;

        /* Set up the component of the derived type, which will be
         * an integer with kind equal to c_ptr_size.
         *
         * Mangle the name of the field for the c_address to prevent
         * the curious user from trying to access it from Fortran.  */
        sprintf (comp_name, "__%s_%s", tmp_sym->name, "c_address");
        gfc_add_component (tmp_sym, comp_name, &tmp_comp);
        if (tmp_comp == NULL)
          gfc_internal_error ("generate_isocbinding_symbol(): Unable to "
			      "create component for c_address");

        tmp_comp->ts.type = BT_INTEGER;

        /* the kinds for c_ptr and c_funptr are the same. */
        index = get_c_kind("c_ptr", c_interop_kinds_table);
        tmp_comp->ts.kind = c_interop_kinds_table[index].value;

        tmp_comp->pointer = 0;
        tmp_comp->dimension = 0;

        /* initialize the value of the c_address field to NULL */
        tmp_comp->initializer = gfc_int_expr (0);

        /* make it use associated (iso_c_binding module) */
        tmp_sym->attr.use_assoc = 1;
	break;

      case ISOCBINDING_F_POINTER:
      case ISOCBINDING_ASSOCIATED:
      case ISOCBINDING_LOC:
      case ISOCBINDING_F_PROCPOINTER:

	tmp_sym->attr.proc = PROC_MODULE;
	sprintf (tmp_sym->binding_label, "%s_%s", mod_name, name);
	tmp_sym->attr.is_iso_c = 1;
	if (s == ISOCBINDING_F_POINTER || s == ISOCBINDING_F_PROCPOINTER)
	  tmp_sym->attr.subroutine = 1;
	else
	  {
            /* TODO!!! this needs to be finished more for the expr of the
               function or something!!!  --Rickett, 08.10.06
               this may not need to be here, because trying to do c_loc
               as an external...  --Rickett, 08.24.06 */
	    if (s == ISOCBINDING_ASSOCIATED)
	      {
		tmp_sym->attr.function = 1;
		tmp_sym->ts.type = BT_LOGICAL;
		tmp_sym->ts.kind = gfc_default_logical_kind;
		tmp_sym->result = tmp_sym;
	      }
	    else
	      {
               /* here, we're taking the simple approach.  we're defining
                  c_loc as an external identifier so the compiler will put
                  what we expect on the stack for the address we want the
                  C address of.  */
		tmp_sym->ts.type = BT_DERIVED;
		gfc_find_symbol ("c_ptr", gfc_current_ns, 1,
				 &(tmp_sym->ts.derived));
		if (tmp_sym->ts.derived == NULL)
		  gfc_error_now ("Type C_PTR is not defined but is needed "
				 "as return type of function C_LOC at %C");

		/* the function result is itself (no result clause) */
		tmp_sym->result = tmp_sym;
		tmp_sym->attr.external = 1;
		tmp_sym->attr.use_assoc = 0;
		tmp_sym->attr.if_source = IFSRC_UNKNOWN;
		tmp_sym->attr.proc = PROC_UNKNOWN;
	      }
	  }

	tmp_sym->attr.flavor = FL_PROCEDURE;
	tmp_sym->attr.contained = 0;

#if 0
	/* we need to make the curr ns the ns that'll hold the proc sym */
	save_curr_ns = gfc_current_ns;
	gfc_current_ns = ns;
#endif
	
       /* try using this builder routine, with the new and old symbols
          both being the generic iso_c proc sym being created.
          this will create the formal args (and the new namespace for them). */
       /* don't build an arg list for c_loc because we're going to treat c_loc
          as an external procedure. */
	if (s != ISOCBINDING_LOC)
          /* the 1 says to add any optional args, if applicable */
	  build_formal_args(tmp_sym, tmp_sym, 1);

#if 0
        /* restore the current namespace to what it was coming in */
	gfc_current_ns = save_curr_ns;
#endif

        /* set this after setting up the symbol, to prevent error messages */
	tmp_sym->attr.use_assoc = 1;

        /* this symbol will not be referenced directly.  it will be
           resolved to the implementation for the given f90 kind.  */
	tmp_sym->attr.referenced = 0;

	break;

      default:
	gcc_unreachable ();
    }
}

void gen_c_interop_kinds(const char *mod_name,
                         CInteropKind_t *kinds[])
{
   int i = 0;
   const char *given_ptr_kinds[3] = { NULL };
   int curr_ptr_kind = 0;

   if(kinds != NULL)
   {
      /* generate specific symbols, given by the 'only' clause */
      while(kinds[i] != NULL)
      {
         switch(kinds[i]->f90_type)
         {
         case BT_INTEGER:
            /* fall through */
         case BT_REAL:
            /* fall through */            
         case BT_COMPLEX:
            /* fall through */            
         case BT_LOGICAL:
            /* fall through */            
         case BT_CHARACTER:
            gen_c_interop_kind_sym(kinds[i], mod_name);
            break;
         case BT_DERIVED:
            given_ptr_kinds[curr_ptr_kind] = kinds[i]->name;
            curr_ptr_kind++;
            given_ptr_kinds[curr_ptr_kind] = NULL;
            break;
         case BT_PROCEDURE:
            gen_c_interop_int_funcs(mod_name, kinds[i]->name);
            break;
         default:
            gfc_internal_error("gen_c_interop_kinds(): Invalid kind");
         }/* end switch(f90 type) */
         
         i++;
      }/* end while(specific set of kinds to do) */

      if(curr_ptr_kind != 0)
         /* take care of the ptrs kinds given to us */
         gen_c_interop_ptr_kinds(mod_name, given_ptr_kinds);
   }/* end if(given specific set of syms (i.e., only clause)) */
   else
   {
      /* generate all iso_c_binding symbols */
      
      /* generate the syms for integer kinds */
      gen_c_interop_int_kinds(mod_name);
      
      /* generate the syms for real kinds */
      gen_c_interop_real_kinds(mod_name);
      
      /* generate the syms for char kinds */
      gen_c_interop_char_kinds(mod_name);
      
      /* generate the syms for ptr kinds */
      gen_c_interop_ptr_kinds(mod_name, NULL);

      /* generate the logical sym kind */
      gen_c_interop_logical_kinds(mod_name);

      /* generate the syms for the interop intrinsic functions */
      gen_c_interop_int_funcs(mod_name, NULL);
   }/* end else(generate all symbols in module) */

   return;
}/* end gen_c_interop_kinds() */


/**
 * Creates a new symbol based off of an old iso_c symbol, with a new
 * binding label.  This function can be used to create a new, resolved,
 * version of a procedure symbol for <code>c_f_pointer</code> or
 * <code>c_f_procpointer</code> that is based on the generic symbols.  A
 * new parameter list is created for the new symbol using
 * <code>#build_formal_args()</code>.
 *
 * @param old_sym Old symbol to use in setting fields of the new one.
 * @param new_name Name of the new symbol.
 * @param new_binding_label Binding label for the new symbol.  This
 * @param add_optional_arg Flag specifying whether the symbol being
 * created should have it's optional arg included in the arg list (if
 * applicable).  Example is for <code>c_associated</code> and whether
 * the arg for the second pointer should be added or not.
 * should represent the resolved version of the generic symbol.
 * @return The new <code>gfc_symbol</code>.
 */
gfc_symbol *get_iso_c_sym(gfc_symbol *old_sym, char *new_name,
                          char *new_binding_label, int add_optional_arg)
{
   gfc_symtree *new_symtree = NULL;

   /* see if we have a symbol by that name already available, looking
    * through any parent namespaces
    *
    * return value is being ignored.  if the return is nonzero, the
    * sym was ambiguous.  can this happen here??  --Rickett, 02.27.06
    */
   gfc_find_sym_tree(new_name, gfc_current_ns, 1, &new_symtree);
   if(new_symtree != NULL)
      /* return the existing symbol */
      return new_symtree->n.sym;

   /* create the symtree/symbol, with attempted host association */
   gfc_get_ha_sym_tree(new_name, &new_symtree);
   if(new_symtree == NULL)
      gfc_internal_error("get_iso_c_sym(): Unable to create "
                         "symtree for %s", new_name);

   /* now fill in the fields of the resolved symbol with the old sym */
   strcpy(new_symtree->n.sym->binding_label, new_binding_label);
   new_symtree->n.sym->attr = old_sym->attr;
   new_symtree->n.sym->ts = old_sym->ts;
   new_symtree->n.sym->module = gfc_get_string(old_sym->module);
   /* build the formal arg list */
   build_formal_args(new_symtree->n.sym, old_sym, add_optional_arg);

   gfc_commit_symbol(new_symtree->n.sym);

   return new_symtree->n.sym;
}/* end get_iso_c_sym() */


/**
 * Copy the formal args from an existing symbol (<code>src</code>) into
 * a new symbol (<code>dest</code>).  New formal args are created, and
 * the description of each arg is set according to the existing ones.
 * This function is used when creating procedure declaration variables
 * from a procedure declaration statement
 * (see <code>#match_proc_decl()</code>) to create the formal args based
 * on the args of a given named interface.  
 *
 * @param dest Symbol to create the new formal args for.
 * @param src Symbol to use in creating the new args.
 * @return None
 */
void copy_formal_args(gfc_symbol *dest, gfc_symbol *src)
{
   gfc_formal_arglist *head = NULL;
   gfc_formal_arglist *tail = NULL;
   gfc_formal_arglist *formal_arg = NULL;
   gfc_formal_arglist *curr_arg = NULL;
   gfc_formal_arglist *formal_prev = NULL;
   gfc_namespace *parent_ns = NULL;

   /* save current namespace so we can change it for formal args */
   parent_ns = gfc_current_ns;
   /* create a new namespace, which will be the formal ns (namespace
    * of the formal args).
    */
   gfc_current_ns = gfc_get_namespace(parent_ns, 0);
   gfc_current_ns->proc_name = dest;
   
   curr_arg = src->formal;
   formal_prev = NULL;
   while(curr_arg != NULL)
   {
      /* allocate a new struct for the formal arg */
      formal_arg = gfc_get_formal_arglist();

      /* create symbol for the arg */
      gfc_get_symbol(curr_arg->sym->name, gfc_current_ns,
                     &(formal_arg->sym));
      /* may need to copy more info for the symbol.. */
      formal_arg->sym->attr = curr_arg->sym->attr;
      formal_arg->sym->ts = curr_arg->sym->ts;
      /* if this isn't the first arg, set up the next ptr.  for the
       * last arg built, the formal_arg->next will never get set to
       * anything other than NULL.
       */
      formal_arg->next = NULL;
      if(formal_prev != NULL)
         formal_prev->next = formal_arg;
      formal_prev = formal_arg;
      
      /* add arg to list of formal args */
      add_formal_arg(&head, &tail, formal_arg, formal_arg->sym);
      
      /* will reuse for any additional arg(s) */
      formal_arg = NULL;
      /* go to the next arg, if any */
      curr_arg = curr_arg->next;
   }/* end while(more args exist) */

   /* add the interface to the symbol */
   add_proc_interface(dest, IFSRC_DECL, head);
   /* store the formal namespace information */
   if(dest->formal != NULL)
      /* the current ns should be that for the dest proc */
      dest->formal_ns = gfc_current_ns;

   /* restore the current namespace to what it was on entry */
   gfc_current_ns = parent_ns;
   
   return;
}/* end copy_formal_args() */
