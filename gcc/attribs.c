/* Functions dealing with attribute handling, used by most front ends.
   Copyright (C) 1992, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001,
   2002, 2003, 2004 Free Software Foundation, Inc.

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
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "flags.h"
#include "toplev.h"
#include "output.h"
#include "rtl.h"
#include "ggc.h"
#include "tm_p.h"
#include "cpplib.h"
#include "target.h"
#include "langhooks.h"

static void init_attributes (void);

/* Table of the tables of attributes (common, language, format, machine)
   searched.  */
static const struct attribute_spec *attribute_tables[4];

static bool attributes_initialized = false;

/* Default empty table of attributes.  */
static const struct attribute_spec empty_attribute_table[] =
{
  { NULL, 0, 0, false, false, false, NULL }
};

/* Initialize attribute tables, and make some sanity checks
   if --enable-checking.  */

static void
init_attributes (void)
{
  size_t i;

  attribute_tables[0] = lang_hooks.common_attribute_table;
  attribute_tables[1] = lang_hooks.attribute_table;
  attribute_tables[2] = lang_hooks.format_attribute_table;
  attribute_tables[3] = targetm.attribute_table;

  /* Translate NULL pointers to pointers to the empty table.  */
  for (i = 0; i < ARRAY_SIZE (attribute_tables); i++)
    if (attribute_tables[i] == NULL)
      attribute_tables[i] = empty_attribute_table;

#ifdef ENABLE_CHECKING
  /* Make some sanity checks on the attribute tables.  */
  for (i = 0; i < ARRAY_SIZE (attribute_tables); i++)
    {
      int j;

      for (j = 0; attribute_tables[i][j].name != NULL; j++)
	{
	  /* The name must not begin and end with __.  */
	  const char *name = attribute_tables[i][j].name;
	  int len = strlen (name);
	  
	  gcc_assert (!(name[0] == '_' && name[1] == '_'
			&& name[len - 1] == '_' && name[len - 2] == '_'));
	  
	  /* The minimum and maximum lengths must be consistent.  */
	  gcc_assert (attribute_tables[i][j].min_length >= 0);
	  
	  gcc_assert (attribute_tables[i][j].max_length == -1
		      || (attribute_tables[i][j].max_length
			  >= attribute_tables[i][j].min_length));
	  
	  /* An attribute cannot require both a DECL and a TYPE.  */
	  gcc_assert (!attribute_tables[i][j].decl_required
		      || !attribute_tables[i][j].type_required);
	  
	  /* If an attribute requires a function type, in particular
	     it requires a type.  */
	  gcc_assert (!attribute_tables[i][j].function_type_required
		      || attribute_tables[i][j].type_required);
	}
    }

  /* Check that each name occurs just once in each table.  */
  for (i = 0; i < ARRAY_SIZE (attribute_tables); i++)
    {
      int j, k;
      for (j = 0; attribute_tables[i][j].name != NULL; j++)
	for (k = j + 1; attribute_tables[i][k].name != NULL; k++)
	  gcc_assert (strcmp (attribute_tables[i][j].name,
			      attribute_tables[i][k].name));
    }
  /* Check that no name occurs in more than one table.  */
  for (i = 0; i < ARRAY_SIZE (attribute_tables); i++)
    {
      size_t j, k, l;

      for (j = i + 1; j < ARRAY_SIZE (attribute_tables); j++)
	for (k = 0; attribute_tables[i][k].name != NULL; k++)
	  for (l = 0; attribute_tables[j][l].name != NULL; l++)
	    gcc_assert (strcmp (attribute_tables[i][k].name,
				attribute_tables[j][l].name));
    }
#endif

  attributes_initialized = true;
}

/* When adding an attribute, there are a bunch of possibilities for what
   might need to be done with it.  */
enum attribute_disposition {
  /* It might be ignored, typically after printing an error message.  */
  IGNORE,
  /* It might need to be applied to a later stage, for example if we're
     given a type and the attribute applies to declarations.  */
  DEFER,
  /* It might be added to this type or decl.  */
  ADD,
  /* It might need to be added to the type of this decl, by creating
     a new type that includes this attribute and making it the type of
     this decl.  */
  ADD_TYPE,
  /* It might need to be a function type, not a pointer-to-function type.
     This will involve creating a new function type and a new type to
     point to it.  */
  ADD_FN_TYPE,
  NUM_DISPOSITIONS
};

/* Fill in DISP with the disposition of each attribute listed in
   ATTRIBUTES when adding them to NODE.  FLAGS is a bitwise OR of
   flags in enum attribute_flags.  Some attributes might be processed
   here, modifying 'node'.  */

static void
determine_dispositions (tree *node, attribute_count num_attributes,
			const struct one_attribute * attributes, int flags,
			char * disp)
{
  attribute_count ac;
  
  for (ac = 0; ac < num_attributes; ac++)
    {
      tree name = attributes[ac].name;
      tree args = attributes[ac].value;
      tree *anode = node;
      const struct attribute_spec *spec = NULL;
      size_t i;

      for (i = 0; i < ARRAY_SIZE (attribute_tables); i++)
	{
	  int j;

	  for (j = 0; attribute_tables[i][j].name != NULL; j++)
	    {
	      if (is_attribute_p (attribute_tables[i][j].name, name))
		{
		  spec = &attribute_tables[i][j];
		  break;
		}
	    }
	  if (spec != NULL)
	    break;
	}

      disp[ac] = ADD;

      if (spec == NULL)
	{
	  warning ("%qs attribute directive ignored",
		   IDENTIFIER_POINTER (name));
	  disp[ac] = IGNORE;
	}
      else if (list_length (args) < spec->min_length
	       || (spec->max_length >= 0
		   && list_length (args) > spec->max_length))
	{
	  error ("wrong number of arguments specified for %qs attribute",
		 IDENTIFIER_POINTER (name));
	  disp[ac] = IGNORE;
	}
      else if (spec->decl_required && !DECL_P (*anode))
	{
	  if (flags & ((int) ATTR_FLAG_DECL_NEXT
		       | (int) ATTR_FLAG_FUNCTION_NEXT
		       | (int) ATTR_FLAG_ARRAY_NEXT))
	    disp[ac] = DEFER;
	  else
	    {
	      warning ("%qs attribute does not apply to types",
		       IDENTIFIER_POINTER (name));
	      disp[ac] = IGNORE;
	    }
	}

      /* If we require a type, but were passed a decl, set up to make a
	 new type and update the one in the decl.  If a function type
         is required, find one.  */
      else if (spec->type_required)
	{
	  if (DECL_P (*anode))
	    {
	      anode = &TREE_TYPE (*anode);
	      disp[ac] = ADD_TYPE;
	    }

	  if (spec->function_type_required 
	      && TREE_CODE (*anode) != FUNCTION_TYPE
	      && TREE_CODE (*anode) != METHOD_TYPE)
	    {
	      if (TREE_CODE (*anode) == POINTER_TYPE
		  && (TREE_CODE (TREE_TYPE (*anode)) == FUNCTION_TYPE
		      || TREE_CODE (TREE_TYPE (*anode)) == METHOD_TYPE))
		{
		  anode = &TREE_TYPE (*anode);
		  disp[ac]++;
		}
	      else if (flags & (int) ATTR_FLAG_FUNCTION_NEXT)
		disp[ac] = DEFER;
	      else if (TREE_CODE (*anode) != FUNCTION_TYPE
		       && TREE_CODE (*anode) != METHOD_TYPE)
		{
		  warning ("%qs attribute only applies to function types",
			   IDENTIFIER_POINTER (name));
		  disp[ac] = IGNORE;
		}
	    }
	}
      
      if (disp[ac] != IGNORE && disp[ac] != DEFER && spec->handler)
	{
	  bool ignore = false;
	  bool defer = false;
	  (*spec->handler) (anode, name, args, flags, &ignore, &defer);
	  if (ignore)
	    disp[ac] = IGNORE;
	  else if (defer)
	    disp[ac] = DEFER;
	}
    }
}

/* Process the attributes listed in ATTRIBUTES and install them in *NODE,
   which is either a DECL (including a TYPE_DECL) or a TYPE.  If a DECL,
   it should be modified in place; if a TYPE, a copy should be created
   unless ATTR_FLAG_TYPE_IN_PLACE is set in FLAGS.  FLAGS gives further
   information, in the form of a bitwise OR of flags in enum attribute_flags
   from tree.h.  Depending on these flags, some attributes may be
   returned to be applied at a later stage (for example, to apply
   a decl attribute to the declaration rather than to its type).  */

attribute_list
decl_attributes (tree *node, attribute_list attributes, int flags)
{
  char * disp;
  attribute_count num_machine_attribs;
  const struct one_attribute * machine_attribs = NULL;
  attribute_count num_attributes;
  attribute_count i;
  attribute_count total_attributes;
  struct one_attribute * all_attribs;
  attribute_count num_per_disp[NUM_DISPOSITIONS];
  tree old_type;

  if (!attributes_initialized)
    init_attributes ();

  num_machine_attribs = targetm.add_attributes (*node, attributes,
						&machine_attribs);

  num_attributes = ATTRIBUTE_COUNT (attributes);
  total_attributes = num_machine_attribs + num_attributes;

  /* Check for overflow.  */
  if (total_attributes < num_attributes)
    {
      error ("too many attributes");
      return NULL;
    }

  /* Early exit.  */
  if (total_attributes == 0)
    return NULL;

  /* Put all the attributes into one list for convenience, so that
     it's easier to add them all to a type or decl at once.  */
  disp = alloca (sizeof (*disp) * total_attributes);
  all_attribs = alloca (sizeof (*all_attribs) * total_attributes);
  if (num_machine_attribs != 0)
    memcpy (all_attribs, machine_attribs,
	    sizeof (*all_attribs) * num_machine_attribs);
  if (attributes)
    memcpy (all_attribs + num_machine_attribs, attributes->attribs,
	    sizeof (*all_attribs) * num_attributes);

  /* Save away the old type, so that we know whether or not we need to
     re-layout a DECL.  */
  old_type = TREE_TYPE (*node);
  
  determine_dispositions (node, total_attributes, all_attribs, flags, disp);
  
  /* Count the dispositions.  */
  memset (num_per_disp, 0, sizeof (num_per_disp));
  for (i = 0; i < total_attributes; i++)
    num_per_disp[(int) disp[i]]++;
  
  /* Process the possible destinations from the simplest type outwards.  */
  if (num_per_disp[ADD_FN_TYPE] != 0)
    {
      tree old_type, new_type;
      attribute_list new_attribs;
      
      old_type = TREE_TYPE (TREE_TYPE (*node));
      new_attribs = merge_attributes_1 (TYPE_ATTRIBUTES (old_type),
					total_attributes, all_attribs,
					disp, ADD_FN_TYPE);
      new_type = build_type_attribute_variant (old_type, new_attribs);
      gcc_assert (DECL_P (*node));
      TREE_TYPE (*node) = build_pointer_type (new_type);
    }
  if (num_per_disp[ADD_TYPE] != 0)
    {
      tree old_type, new_type;
      attribute_list new_attribs;
      
      old_type = TREE_TYPE (*node);
      new_attribs = merge_attributes_1 (TYPE_ATTRIBUTES (old_type),
					total_attributes, all_attribs,
					disp, ADD_TYPE);
      new_type = build_type_attribute_variant (old_type, new_attribs);

      if (DECL_P (*node))
	TREE_TYPE (*node) = new_type;
      else
	{
	  gcc_assert (TREE_CODE (*node) == POINTER_TYPE);
	  *node = build_pointer_type (new_type);
	}
    }
  /* If the above changed the type, layout the decl again.  */
  if (DECL_P (*node) && TREE_TYPE (*node) != old_type
      && TREE_CODE (*node) != LABEL_DECL
      && TREE_CODE (*node) != FUNCTION_DECL)
    {
      /* Force a recalculation of mode and size.  */
      DECL_MODE (*node) = VOIDmode;
      DECL_SIZE (*node) = 0;
      
      layout_decl (*node, 0);
    }
  if (num_per_disp[ADD] != 0)
    {
      attribute_list old_attribs, new_attribs;
      
      if (DECL_P (*node))
	old_attribs = DECL_ATTRIBUTES (*node);
      else
	old_attribs = TYPE_ATTRIBUTES (*node);
      
      new_attribs = merge_attributes_1 (old_attribs,
					total_attributes, all_attribs,
					disp, ADD);
      if (DECL_P (*node))
	DECL_ATTRIBUTES (*node) = new_attribs;
      else if (flags & ATTR_FLAG_TYPE_IN_PLACE)
	TYPE_ATTRIBUTES (*node) = new_attribs;
      else
	*node = build_type_attribute_variant (*node, new_attribs);
    }
  if (num_per_disp[DEFER] != 0)
    return merge_attributes_1 (NULL, total_attributes, all_attribs,
			       disp, DEFER);
  else
    return NULL;
}
