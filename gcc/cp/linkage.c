/* Code for handling linkage of vtables, inlines et al in GNU C++.
   Copyright (C) 2001 Free Software Foundation, Inc.
   Hacked by Jason Merrill (jason@redhat.com)

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "tree.h"
#include "cp-tree.h"
#include "ggc.h"
#include "flags.h"
#include "toplev.h"

#include "splay-tree.h"
#include "varray.h"

/* Functions and data structures for #pragma interface.

   `#pragma implementation' means that the main file being compiled
   is considered to implement (provide) the classes that appear in
   its main body.  I.e., if this is file "foo.cc", and class `bar'
   is defined in "foo.cc", then we say that "foo.cc implements bar".

   All main input files "implement" themselves automagically.

   `#pragma interface' means that unless this file (of the form "foo.h"
   is presently being included by file "foo.cc", the FIXME FIXME
   CLASSTYPE_INTERFACE_ONLY bit gets set.  The effect is that none
   of the vtables nor any of the inline functions defined in foo.h
   will ever be output.

   There are cases when we want to link files such as "defs.h" and
   "main.cc".  In this case, we give "defs.h" a `#pragma interface',
   and "main.cc" has `#pragma implementation "defs.h"'.  */

struct cp_fileinfo
{
  varray_type items;
  int implemented;
};
static splay_tree cp_fileinfo_tree;

/* The list of entities with vague linkage in the current file, if we've
   seen #pragma interface.  */

varray_type *interface_itemsptr;

static varray_type impl_files;

static int interface_strcmp PARAMS ((const char *, const char *));

/* Helper function to load global variables with interface
   information.  */

#if 0
void
extract_interface_info ()
{
  struct c_fileinfo *finfo = 0;
  struct cp_fileinfo *ifile = 0;

  if (flag_alt_external_templates)
    {
      tree til = tinst_for_decl ();

      if (til)
	finfo = get_fileinfo (TINST_FILE (til));
    }
  if (!finfo)
    finfo = get_fileinfo (input_filename);

  ifile = (struct cp_fileinfo *) (finfo->lang_data);

  interface_unknown = 1;
  if (ifile)
    {
      interface_itemsptr = &(ifile->items);
      interface_only = 1;
    }
  else
    {
      interface_itemsptr = NULL;
      interface_only = 0;
    }

  /* This happens to be a convenient place to put this.  */
  if (flag_gnu_xref) GNU_xref_file (input_filename);
}
#endif

/* Return zero if S and T work as an INTERFACE/IMPLEMENTATION pair.
   Basically, this is a strcmp ignoring the file extension, so that foo.C and
   foo.h will match.

   Otherwise, return as strcmp.  */

static int
interface_strcmp (s1, t1)
     const char *s1;
     const char *t1;
{
  while (*s1 == *t1 && *s1 != 0)
    s1++, t1++;

  /* A match.  */
  if (*s1 == *t1)
    return 0;

  /* Don't get faked out by xxx.yyy.cc vs xxx.zzz.cc.  */
  if (strchr (s1, '.') || strchr (t1, '.'))
    return *s1 - *t1;

  if (*s1 == '\0' || s1[-1] != '.' || t1[-1] != '.')
    return *s1 - *t1;

  /* A match.  */
  return 0;
}

static struct cp_fileinfo *
maybe_get_cp_fileinfo (name)
     const char *name;
{
  splay_tree_node n;

  n = splay_tree_lookup (cp_fileinfo_tree, (splay_tree_key) name);
  if (n)
    return (struct cp_fileinfo *) n->value;
  return NULL;
}

static struct cp_fileinfo *
insert_cp_fileinfo (name)
     const char *name;
{
  struct cp_fileinfo *fi
    = (struct cp_fileinfo *) xcalloc (1, sizeof (struct cp_fileinfo));
  VARRAY_TREE_INIT (fi->items, 5, "interface items");
  splay_tree_insert (cp_fileinfo_tree, (splay_tree_key) name,
		     (splay_tree_value) fi);
  return fi;
}

void
do_pragma_interface (fname)
     tree fname;
{
  struct c_fileinfo *finfo;
  struct cp_fileinfo *cp_finfo;
  const char *main_filename;

  if (fname == 0)
    main_filename = file_name_nondirectory (input_filename);
  else
    main_filename = TREE_STRING_POINTER (fname);

  finfo = get_fileinfo (input_filename);

  cp_finfo = maybe_get_cp_fileinfo (ggc_strdup (main_filename));
  if (cp_finfo)
    warning ("duplicate #pragma interface for %s", main_filename);
  else
    cp_finfo = insert_cp_fileinfo (main_filename);

  finfo->lang_data = cp_finfo;

  extract_interface_info ();
}

/* Note that we have seen a #pragma implementation for the key MAIN_FILENAME.
   We used to only allow this at toplevel, but that restriction was buggy
   in older compilers and it seems reasonable to allow it in the headers
   themselves, too.  */

void
do_pragma_implementation (fname)
     tree fname;
{
  const char *main_filename;

  if (fname == 0)
    main_filename = file_name_nondirectory (main_input_filename);
  else
    main_filename = TREE_STRING_POINTER (fname);

  VARRAY_PUSH_CHAR_PTR (impl_files, ggc_strdup (main_filename));
}

void
maybe_add_interface_item (item)
     tree item;
{
  if (interface_itemsptr)
    VARRAY_PUSH_TREE (*interface_itemsptr, item);
}

static int
foreach_process_interface (node, data)
     splay_tree_node node;
     void *data ATTRIBUTE_UNUSED;
{
  struct cp_fileinfo *cp_finfo = (struct cp_fileinfo *) node->value;
  varray_type items = cp_finfo->items;
  int impl = cp_finfo->implemented;
  unsigned i;

  for (i = 0; i < VARRAY_ACTIVE_SIZE (items); ++i)
    {
      tree item = VARRAY_TREE (items, i);
      if (TYPE_P (item))
	{
	  SET_CLASSTYPE_INTERFACE_KNOWN (item);
	  CLASSTYPE_INTERFACE_ONLY (item) = !impl;
#if 0
	  CLASSTYPE_VTABLE_NEEDS_WRITING (item) = impl;
#endif
	  TYPE_DECL_SUPPRESS_DEBUG (TYPE_NAME (item)) = !impl;
	  if (impl && !uses_template_parms (item))
	    {
	      CLASSTYPE_DEBUG_REQUESTED (item) = 1;
	      rest_of_type_compilation (item, 1);
	    }
	  if (flag_external_templates && !flag_alt_external_templates
	      && uses_template_parms (item))
	    {
	      tree s
		= DECL_TEMPLATE_INSTANTIATIONS (CLASSTYPE_TI_TEMPLATE (item));
	      for (; s; s = TREE_CHAIN (s))
		{
		  tree t = TREE_VALUE (s);
		  SET_CLASSTYPE_INTERFACE_KNOWN (t);
		  CLASSTYPE_INTERFACE_ONLY (t) = !impl;
#if 0
		  CLASSTYPE_VTABLE_NEEDS_WRITING (t) = impl;
#endif
		  TYPE_DECL_SUPPRESS_DEBUG (TYPE_NAME (t)) = !impl;
		  if (impl && !uses_template_parms (t))
		    {
		      CLASSTYPE_DEBUG_REQUESTED (t) = 1;
		      rest_of_type_compilation (t, 1);
		    }
		}		  
	    }
	}
      else
	{
	  DECL_INTERFACE_KNOWN (item) = 1;
	  if (DECL_LANG_SPECIFIC (item))
	    DECL_NOT_REALLY_EXTERN (item) = impl;
	  else
	    DECL_EXTERNAL (item) = !impl;
	  if (flag_external_templates && !flag_alt_external_templates
	      && uses_template_parms (item))
	    {
	      tree s = DECL_TEMPLATE_SPECIALIZATIONS (DECL_TI_TEMPLATE (item));
	      for (; s; s = TREE_CHAIN (s))
		{
		  tree d = TREE_VALUE (s);
		  DECL_INTERFACE_KNOWN (d) = 1;
		  DECL_NOT_REALLY_EXTERN (d) = impl;
		}
	    }
	}
    }
  return 0;
}

void
process_interface_items ()
{
  unsigned int i;
  for (i = 0; i < VARRAY_ACTIVE_SIZE (impl_files); ++i)
    {
      char *file = VARRAY_CHAR_PTR (impl_files, i);
      struct cp_fileinfo *cp_finfo = maybe_get_cp_fileinfo (file);
      if (! cp_finfo)
	warning ("\
#pragma implementation for %s does not match any #pragma interface",
		    file);
      else
	{
	  if (cp_finfo->implemented)
	    warning ("duplicate #pragma implementation for %s", file);
	  cp_finfo->implemented = 1;
	}
    }
  splay_tree_foreach (cp_fileinfo_tree, foreach_process_interface, NULL);
}

/* Type layout information for cp_fileinfo_tree for the gtype machinery.  */

static const struct field_definition_s cpf_tree_node_field_defs[] = {
  { 0, 0, offsetof (struct splay_tree_node_s, key), string_type_def },
  { 0, 0, offsetof (struct splay_tree_node_s, value), cp_fileinfo_type_def },
  { 0, 0, offsetof (struct splay_tree_node_s, left), cpf_tree_node_type_def },
  { 0, 0, offsetof (struct splay_tree_node_s, right), cpf_tree_node_type_def },
  NO_MORE_FIELDS
};
static const struct field_definition_s cp_fileinfo_field_defs[] = {
  { 0, 0, offsetof (struct cp_fileinfo, items), varray_tree_type_def },
  NO_MORE_FIELDS
};

void
init_linkage ()
{
  cpf_tree_node_type_def->size = sizeof (struct splay_tree_node_s);
  cpf_tree_node_type_def->field_definitions = cpf_tree_node_field_defs;
  cpf_tree_node_type_def->ggc_p = -1;
  cp_fileinfo_type_def->size = sizeof (struct cp_fileinfo);
  cp_fileinfo_type_def->field_definitions = cp_fileinfo_field_defs;
  cp_fileinfo_type_def->ggc_p = -1;

  cp_fileinfo_tree = splay_tree_new ((splay_tree_compare_fn)interface_strcmp,
				     0,
				     (splay_tree_delete_value_fn)free);

  ggc_add_typed_root (&cp_fileinfo_tree->root, cpf_tree_node_type_def, 1, "cp_fileinfo_tree->root");
  add_typed_addresses (&data_to_save, (void **)&cp_fileinfo_tree->root,
		       cpf_tree_node_type_def, 1, "cp_fileinfo_tree->root");

  /* Not saved in PCH.  */
  VARRAY_CHAR_PTR_INIT (impl_files, 1, "impl_files");
  ggc_add_string_varray_root (&impl_files, 1, "impl_files");
}
