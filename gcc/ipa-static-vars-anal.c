/* Callgraph based analysis of static variables.
   Copyright (C) 2004, 2005 Free Software Foundation, Inc.
   Contributed by Kenneth Zadeck <zadeck@naturalbridge.com>

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

/* This file gathers information about how statics that are local to
   the compilation unit are used.  It is to be run after the call
   graph is built and after inlining decisions have been made.

   First each function and static variable initialization is analyzed
   to determine which local static variables are either read, written,
   or have their address taken.  Any local static that has its address
   taken is removed from consideration.  Once the local read and
   writes are determined, a transitive closure of this information is
   performed over the call graph to determine the worst case set of
   side effects of each call.  In later parts of the compiler, these
   local and global sets are examined to make the call clobbering less
   traumatic, promote some statics to registers, and improve aliasing
   information.
   
   As a direct result of the information gathered here, the
   TREE_READONLY bits can be set and TREE_ADDRESSABLE can be cleared
   for all compilation unit level static variables that satisfy those
   criteria.

   Lastly, all of this information is used to mark functions as being
   either const (TREE_READONLY) or pure (DECL_IS_PURE).

   This must be run after inlining decisions have been made since
   otherwise, the local sets will not contain information that is
   consistent with post inlined state.  The global sets are not prone
   to this problem since they are by definition transitive.  */

/* The code in this module is called by the ipa pass manager. It
   should be one of the later passes since it's information is used by
   the rest of the compilation. */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "tree-pass.h"
#include "langhooks.h"
#include "pointer-set.h"
#include "ggc.h"
#include "ipa-static.h"
#include "c-common.h"
#include "tree-gimple.h"
#include "cgraph.h"
#include "output.h"
#include "flags.h"
#include "timevar.h"
#include "diagnostic.h"
#include "langhooks.h"

/* FIXME -- PROFILE-RESTRUCTURE: change comment from DECL_UID to var-ann. */    
/* This splay tree contains all of the static variables that are
   being considered by the compilation level alias analysis.  For
   module_at_a_time compilation, this is the set of static but not
   public variables.  Any variables that either have their address
   taken or participate in otherwise unsavory operations are deleted
   from this list.  */
static GTY((param1_is(int), param2_is(tree)))
     splay_tree static_vars_to_consider_by_uid;

/* This bitmap is used to knock out the module static variables whose
   addresses have been taken and passed around.  This is indexed by
   uid.  */
static bitmap module_statics_escape;

/* This bitmap is used to knock out the module static variables that
   are not readonly.  This is indexed by uid.  */
static bitmap module_statics_written;

/* FIXME -- PROFILE-RESTRUCTURE: change comment from DECL_UID to var-ann. */    
/* A bit is set for every module static we are considering and is
   indexed by DECL_UID.  This is ored into the local info when asm
   code is found that clobbers all memory. */
static bitmap all_module_statics;

/* This bitmap contains the set of local vars that are the lhs of
   calls to mallocs.  These variables, when seen on the rhs as part of
   a cast, the cast are not marked as doing bad things to the type
   even though they are generally of the form 
   "foo = (type_of_foo)void_temp". */
static bitmap results_of_malloc;

/* Scratch bitmap for avoiding work. */
static bitmap been_there_done_that;

/* There are two levels of escape that types can undergo.

   EXPOSED_PARAMETER - some instance of the variable is
   passed by value into an externally visible function or some
   instance of the variable is passed out of an externally visible
   function as a return value.  In this case any of the fields of the
   variable that are pointer types end up having their types marked as
   FULL_ESCAPE.

   FULL_ESCAPE - when bad things happen to good types. One of the
   following things happens to the type: (a) either an instance of the
   variable has it's address passed to an externally visible function,
   (b) the address is taken and some bad cast happens to the address
   or (c) explicit arithmetic is done to the address.
*/

enum escape_t
{
  EXPOSED_PARAMETER,
  FULL_ESCAPE
};

/* The following two bit vectors global_types_* correspond to
   previous cases above.  During the analysis phase, a bit is set in
   one of these vectors if an operation of the offending class is
   discovered to happen on the associated type.  */
 
static bitmap global_types_exposed_parameter;
static bitmap global_types_full_escape;

/* All of the types seen in this compilation unit. */
static bitmap global_types_seen;
static splay_tree uid_to_type;

/* Map the several instances of a type into a single instance.  These
   can arise in several ways, nono of which can be justified except by
   laziness and stupidity.  */
static splay_tree uid_to_unique_type;
static splay_tree all_unique_types;

/* A splay tree of bitmaps.  An element X in the splay tree has a bit
   set in its bitmap at TYPE_UID (TYPE_MAIN_VARIANT (Y) if there was
   an operation in the program of the form "&X.Y".  */
static splay_tree uid_to_addressof_map;

/* Tree to hold the subtype maps used to mark subtypes of escaped
   types.  */
static splay_tree uid_to_subtype_map;

/* Records tree nodes seen in cgraph_create_edges.  Simply using
   walk_tree_without_duplicates doesn't guarantee each node is visited
   once because it gets a new htab upon each recursive call from
   scan_for_static_refs.  */
static struct pointer_set_t *visited_nodes;

static bitmap_obstack ipa_obstack;

enum initialization_status_t
{
  UNINITIALIZED,
  RUNNING,
  FINISHED
};

static enum initialization_status_t initialization_status = UNINITIALIZED;
  

tree memory_identifier_string;

/* Debugging function for postorder and inorder code. NOTE is a string
   that is printed before the nodes are printed.  ORDER is an array of
   cgraph_nodes that has COUNT useful nodes in it.  */

static void 
print_order (FILE* out, 
	     const char * note, 
	     struct cgraph_node** order, 
	     int count) 
{
  int i;
  fprintf (out, "\n\n ordered call graph: %s\n", note);
  
  for (i = count - 1; i >= 0; i--)
    dump_cgraph_node(dump_file, order[i]);
  fprintf (out, "\n");
  fflush(out);
}

/* All of the "unique_type" code is a hack to get around the sleezy
   implementation used to compile more than file.  If the same type is
   declared in several files, multiple types will appear that are the
   same.  The code in this unit chooses one "unique" instance of that
   type as the representative and has all of the others point to
   it. If ALLOW_MISSING is true, just return UID if it is not found in
   the table, otherwise abort. */

/* Find the unique representative for a type with UID.  */  
static int
unique_type_id_for (int uid, bool allow_missing ATTRIBUTE_UNUSED)
{
  splay_tree_node result = 
    splay_tree_lookup(uid_to_unique_type, (splay_tree_key) uid);

  if (result)
    return TYPE_UID((tree) result->value);
  else 
    {
      /* ICE when compiling libstdc++.  */
      if (!allow_missing)
	abort();
      return uid;
    }
}

/* Return true if the type with UID is the unique representative.  */
static bool 
unique_type_id_p (int uid)
{
  return uid == unique_type_id_for (uid, false);
}

/* FIXME -- PROFILE-RESTRUCTURE: Remove this function, it becomes a nop. */    
/* Convert IN_DECL bitmap which is indexed by DECL_UID to IN_ANN, a
   bitmap indexed by var_ann (VAR_DECL)->uid.  */

static void 
convert_UIDs_in_bitmap (bitmap in_ann, bitmap in_decl) 
{
  unsigned int index;
  bitmap_iterator bi;

  EXECUTE_IF_SET_IN_BITMAP(in_decl, 0, index, bi)
    {
      splay_tree_node n = 
	      splay_tree_lookup (static_vars_to_consider_by_uid, index);
      if (n != NULL) 
	{
	  tree t = (tree)n->value;
	  var_ann_t va = var_ann (t);
	  if (va) 
	    bitmap_set_bit (in_ann, va->uid);
	}
    }
}


/* FIXME -- PROFILE-RESTRUCTURE: Remove this function, it becomes a
   nop. */    
/* The bitmaps used to represent the static global variables are
   indexed by DECL_UID however, this is not used inside of functions
   to index the ssa variables.  The denser var_ann (VAR_DECL)->uid is
   used there.  This function is called from
   tree_dfa:find_referenced_vars after the denser representation is
   built.  This function invalidates any cached indexes.  */ 

void
ipa_static_reset_maps (void) 
{
  struct cgraph_node *node;
  
  for (node = cgraph_nodes; node; node = node->next)
    {
      ipa_static_vars_info_t info = node->static_vars_info;

      if (info) 
	{
	  ipa_local_static_vars_info_t l = info->local;
	  ipa_global_static_vars_info_t g = info->global;

	  if (l->var_anns_valid) 
	    {
	      bitmap_clear (l->statics_read_by_ann_uid);
	      bitmap_clear (l->statics_written_by_ann_uid);
	      l->var_anns_valid = false;
	    }

	  /* There may be orphans in the cgraph so must check that
	     there is global info.  */
	  if (g && g->var_anns_valid) 
	    {
	      bitmap_clear (g->statics_read_by_ann_uid);
	      bitmap_clear (g->statics_written_by_ann_uid);
	      bitmap_clear (g->statics_not_read_by_ann_uid);
	      bitmap_clear (g->statics_not_written_by_ann_uid);
	      g->var_anns_valid = false;
	    }
	}
    }
}

/* Get the cgraph_node for the local function and make sure the
   var_ann bitmaps are properly converted.  */
 
static ipa_local_static_vars_info_t
get_local_static_vars_info (tree fn) 
{
  ipa_local_static_vars_info_t l;

  /* Was not compiled -O2 or higher.  */ 
  ipa_static_vars_info_t info = get_var_ann (fn)->static_vars_info;

  if (!info)
    return NULL;

  l = info->local;
  if (!l->var_anns_valid) 
    {
      convert_UIDs_in_bitmap (l->statics_read_by_ann_uid, 
			      l->statics_read_by_decl_uid);
      convert_UIDs_in_bitmap (l->statics_written_by_ann_uid, 
			      l->statics_written_by_decl_uid);
      l->var_anns_valid = true;
    }
  return l;
}

/* Get the global static_vars_info structure for the function FN and
   make sure the ann_uid's bitmaps are properly converted.  */
 
static ipa_global_static_vars_info_t
get_global_static_vars_info (tree fn) 
{
  ipa_global_static_vars_info_t g;

  /* Was not compiled -O2 or higher.  */ 
  ipa_static_vars_info_t info = get_var_ann (fn)->static_vars_info;

  if (!info)
    return NULL;

  g = info->global;
  if (!g) 
    return NULL;

  if (!g->var_anns_valid) 
    {
      convert_UIDs_in_bitmap (g->statics_read_by_ann_uid, 
			      g->statics_read_by_decl_uid);
      convert_UIDs_in_bitmap (g->statics_written_by_ann_uid, 
			      g->statics_written_by_decl_uid);
      convert_UIDs_in_bitmap (g->statics_not_read_by_ann_uid, 
			      g->statics_not_read_by_decl_uid);
      convert_UIDs_in_bitmap (g->statics_not_written_by_ann_uid, 
			      g->statics_not_written_by_decl_uid);
      g->var_anns_valid = true;
    }
  return g;
}

/* Return a bitmap indexed by var_ann (VAR_DECL)->uid for the static
   variables that may be read locally by the execution of the function
   fn.  Returns NULL if no data is available, such as it was not
   compiled with -O2 or higher.  */

bitmap 
ipa_get_statics_read_local (tree fn)
{
  ipa_local_static_vars_info_t l = get_local_static_vars_info (fn);
  if (l) 
    return l->statics_read_by_ann_uid;
  else
    return NULL;
}

/* Return a bitmap indexed by var_ann (VAR_DECL)->uid for the static
   variables that may be written locally by the execution of the function
   fn.  Returns NULL if no data is available, such as it was not
   compiled with -O2 or higher or the function was not available.  */

bitmap 
ipa_get_statics_written_local (tree fn)
{
  ipa_local_static_vars_info_t l = get_local_static_vars_info (fn);
  if (l) 
    return l->statics_written_by_ann_uid;
  else
    return NULL;
}

/* Return a bitmap indexed by var_ann (VAR_DECL)->uid for the static
   variables that are read during the execution of the function
   FN.  Returns NULL if no data is available, such as it was not
   compiled with -O2 or higher or the function was not available.  */

bitmap 
ipa_get_statics_read_global (tree fn) 
{
  ipa_global_static_vars_info_t g = get_global_static_vars_info (fn);
  if (g) 
    return g->statics_read_by_ann_uid;
  else
    return NULL;
}

/* Return a bitmap indexed by var_ann (VAR_DECL)->uid for the static
   variables that are written during the execution of the function
   FN.  Note that variables written may or may not be read during the
   function call.  Returns NULL if no data is available, such as it
   was not compiled with -O2 or higher or the function was not available.  */

bitmap 
ipa_get_statics_written_global (tree fn) 
{
  ipa_global_static_vars_info_t g = get_global_static_vars_info (fn);
  if (g) 
    return g->statics_written_by_ann_uid;
  else
    return NULL;
}

/* Return a bitmap indexed by var_ann (VAR_DECL)->uid for the static
   variables that are not read during the execution of the function
   FN.  Returns NULL if no data is available, such as it was not
   compiled with -O2 or higher or the function was not available.  */

bitmap 
ipa_get_statics_not_read_global (tree fn) 
{
  ipa_global_static_vars_info_t g = get_global_static_vars_info (fn);
  if (g) 
    return g->statics_not_read_by_ann_uid;
  else
    return NULL;
}

/* Return a bitmap indexed by var_ann (VAR_DECL)->uid for the static
   variables that are not written during the execution of the function
   FN.  Note that variables written may or may not be read during the
   function call.  Returns NULL if no data is available, such as it
   was not compiled with -O2 or higher or the function was not available.  */

bitmap 
ipa_get_statics_not_written_global (tree fn) 
{
  ipa_global_static_vars_info_t g = get_global_static_vars_info (fn);
  if (g) 
    return g->statics_not_written_by_ann_uid;
  else
    return NULL;
}

/* Return 0 if TYPE is a record or union type.  Return a positive
   number if TYPE is a pointer to a record or union.  The number is
   the number of pointer types stripped to get to the record or union
   type.  Return -1 if TYPE is none of the above.  */
 
int
ipa_static_star_count_of_interesting_type (tree type) 
{
  int count = 0;
  /* Strip the *'s off.  */
  type = TYPE_MAIN_VARIANT (type);
  while (POINTER_TYPE_P (type))
    {
      type = TYPE_MAIN_VARIANT (TREE_TYPE (type));
      count++;
    }

  /* We are interested in records, and unions only.  */
  if (TREE_CODE (type) == RECORD_TYPE 
      || TREE_CODE (type) == QUAL_UNION_TYPE 
      || TREE_CODE (type) == UNION_TYPE)
    return count;
  else 
    return -1;
} 


/* Return 0 if TYPE is a record or union type.  Return a positive
   number if TYPE is a pointer to a record or union.  The number is
   the number of pointer types stripped to get to the record or union
   type.  Return -1 if TYPE is none of the above.  */
 
int
ipa_static_star_count_of_interesting_or_array_type (tree type) 
{
  int count = 0;
  /* Strip the *'s off.  */
  type = TYPE_MAIN_VARIANT (type);
  while (POINTER_TYPE_P (type) || TREE_CODE (type) == ARRAY_TYPE)
    {
      type = TYPE_MAIN_VARIANT (TREE_TYPE (type));
      count++;
    }

  /* We are interested in records, and unions only.  */
  if (TREE_CODE (type) == RECORD_TYPE 
      || TREE_CODE (type) == QUAL_UNION_TYPE 
      || TREE_CODE (type) == UNION_TYPE)
    return count;
  else 
    return -1;
} 
 
 
/* Return true if the record, or union TYPE passed in escapes this
   compilation unit.  */

bool
ipa_static_type_contained_p (tree type)
{
  int uid;

  if (initialization_status == UNINITIALIZED)
    return false;

  type = TYPE_MAIN_VARIANT (type);
  while (POINTER_TYPE_P (type))
    type = TYPE_MAIN_VARIANT (TREE_TYPE (type));

  uid = unique_type_id_for (TYPE_UID (type), true);
  return !bitmap_bit_p (global_types_full_escape, uid);
}

/* Return true a modification to a field of type FIELD_TYPE cannot
   clobber a record of RECORD_TYPE.  */

bool 
ipa_static_field_does_not_clobber_p (tree record_type, tree field_type)
{ 
  splay_tree_node result;
  int uid;
  
  if (initialization_status == UNINITIALIZED)
    return false;

  /* Strip off all of the pointer tos on the record type.  Strip the
     same number of pointer tos from the field type.  If the field
     type has fewer, it could not have been aliased. */
  record_type = TYPE_MAIN_VARIANT (record_type);
  field_type = TYPE_MAIN_VARIANT (field_type);
  while (POINTER_TYPE_P (record_type))
    {
      record_type = TYPE_MAIN_VARIANT (TREE_TYPE (record_type));
      if (POINTER_TYPE_P (field_type)) 
	field_type = TYPE_MAIN_VARIANT (TREE_TYPE (field_type));
      else 
	/* However, if field_type is a union, this quick test is not
	   correct since one of the variants of the union may be a
	   pointer to type and we cannot see across that here.  So we
	   just strip the remaining pointer tos off the record type
	   and fall thru to the more precise code.  */
	if (TREE_CODE (field_type) == QUAL_UNION_TYPE 
	    || TREE_CODE (field_type) == UNION_TYPE)
	  {
	    while (POINTER_TYPE_P (record_type))
	      record_type = TYPE_MAIN_VARIANT (TREE_TYPE (record_type));
	    break;
	  } 
	else 
	  return true;
    }
  
  /* The record type must be contained.  The field type may
     escape.  */
  if (!ipa_static_type_contained_p (record_type))
    return false;

  uid = unique_type_id_for (TYPE_UID (record_type), false);
  result = splay_tree_lookup (uid_to_addressof_map, (splay_tree_key) uid);
  
  if (result) 
    {
      bitmap field_type_map = (bitmap) result->value;
      uid = unique_type_id_for (TYPE_UID (field_type), true);
      /* If the bit is there, the address was taken. If not, it
	 wasn't.  */
      return !bitmap_bit_p (field_type_map, uid);
    }
  else
    /* No bitmap means no addresses were taken.  */
    return true;
}


struct searchc_env {
  struct cgraph_node **stack;
  int stack_size;
  struct cgraph_node **result;
  int order_pos;
  splay_tree nodes_marked_new;
  bool reduce;
  int count;
};

struct dfs_info {
  int dfn_number;
  int low_link;
  bool new;
  bool on_stack;
};

/* This is an implementation of Tarjan's strongly connected region
   finder as reprinted in Aho Hopcraft and Ullman's The Design and
   Analysis of Computer Programs (1975) pages 192-193.  This version
   has been customized for cgraph_nodes.  The env parameter is because
   it is recursive and there are no nested functions here.  This
   function should only be called from itself or
   cgraph_reduced_inorder.  ENV is a stack env and would be
   unnecessary if C had nested functions.  V is the node to start
   searching from.  */

static void
searchc (struct searchc_env* env, struct cgraph_node *v) 
{
  struct cgraph_edge *edge;
  struct dfs_info *v_info = v->aux;
  
  /* mark node as old */
  v_info->new = false;
  splay_tree_remove (env->nodes_marked_new, v->uid);
  
  v_info->dfn_number = env->count;
  v_info->low_link = env->count;
  env->count++;
  env->stack[(env->stack_size)++] = v;
  v_info->on_stack = true;
  
  for (edge = v->callees; edge; edge = edge->next_callee)
    {
      struct dfs_info * w_info;
      struct cgraph_node *w = edge->callee;
      /* Bypass the clones and only look at the master node.  Skip
	 external and other bogus nodes.  */
      w = cgraph_master_clone (w);
      if (w && w->aux) 
	{
	  w_info = w->aux;
	  if (w_info->new) 
	    {
	      searchc (env, w);
	      v_info->low_link =
		(v_info->low_link < w_info->low_link) ?
		v_info->low_link : w_info->low_link;
	    } 
	  else 
	    if ((w_info->dfn_number < v_info->dfn_number) 
		&& (w_info->on_stack)) 
	      v_info->low_link =
		(w_info->dfn_number < v_info->low_link) ?
		w_info->dfn_number : v_info->low_link;
	}
    }


  if (v_info->low_link == v_info->dfn_number) 
    {
      struct cgraph_node *last = NULL;
      struct cgraph_node *x;
      struct dfs_info *x_info;
      do {
	x = env->stack[--(env->stack_size)];
	x_info = x->aux;
	x_info->on_stack = false;
	
	if (env->reduce) 
	  {
	    x->next_cycle = last;
	    last = x;
	  } 
	else 
	  env->result[env->order_pos++] = x;
      } 
      while (v != x);
      if (env->reduce) 
	env->result[env->order_pos++] = v;
    }
}

/* Topsort the call graph by caller relation.  Put the result in ORDER.

   The REDUCE flag is true if you want the cycles reduced to single
   nodes.  Only consider nodes that have the output bit set. */

static int
reduced_inorder (struct cgraph_node **order, bool reduce)
{
  struct cgraph_node *node;
  struct searchc_env env;
  splay_tree_node result;
  env.stack = xcalloc (cgraph_n_nodes, sizeof (struct cgraph_node *));
  env.stack_size = 0;
  env.result = order;
  env.order_pos = 0;
  env.nodes_marked_new = splay_tree_new (splay_tree_compare_ints, 0, 0);
  env.count = 1;
  env.reduce = reduce;
  
  for (node = cgraph_nodes; node; node = node->next) 
    if (cgraph_is_master_clone (node)
	&& (node->local.finalized))
      {
	struct dfs_info *info = xcalloc (1, sizeof (struct dfs_info));
	info->new = true;
	info->on_stack = false;
	node->aux = info;
	node->next_cycle = NULL;
	
	splay_tree_insert (env.nodes_marked_new,
			   (splay_tree_key)node->uid, 
			   (splay_tree_value)node);
      } 
    else 
      node->aux = NULL;
  result = splay_tree_min (env.nodes_marked_new);
  while (result)
    {
      node = (struct cgraph_node *)result->value;
      searchc (&env, node);
      result = splay_tree_min (env.nodes_marked_new);
    }
  splay_tree_delete (env.nodes_marked_new);
  free (env.stack);

  for (node = cgraph_nodes; node; node = node->next)
    if (node->aux)
      {
	free (node->aux);
	node->aux = NULL;
      }
  return env.order_pos;
}



/* Mark a TYPE as being seen.  */ 

static bool
mark_any_type_seen (tree type)
{
  int uid;
  
  type = TYPE_MAIN_VARIANT (type);
  uid = TYPE_UID (type);
  if (bitmap_bit_p (global_types_seen, uid))
    return false;
  else
    {
      splay_tree_insert (uid_to_type, 
			 (splay_tree_key) uid,
			 (splay_tree_value) type);	  
      bitmap_set_bit (global_types_seen, uid);
    }
  return true;
}

/* Mark the underlying record or union type of TYPE as being seen.
   Pointer tos and array ofs are stripped from the type and non record
   or unions are not considered.  */

static bool
mark_type_seen (tree type)
{
  type = TYPE_MAIN_VARIANT (type);
  while (POINTER_TYPE_P (type) || TREE_CODE (type) == ARRAY_TYPE)
    type = TYPE_MAIN_VARIANT (TREE_TYPE (type));

  /* We are interested in records, and unions only.  */
  if (TREE_CODE (type) == RECORD_TYPE 
	  || TREE_CODE (type) == QUAL_UNION_TYPE 
	  || TREE_CODE (type) == UNION_TYPE)
    return mark_any_type_seen (type);
  else 
    {
      /* Allow primitive and function types to get into the set of
	 types seen, but do not return true because there is not
	 reason to recurse in the calls that check the result.  */
      mark_any_type_seen (type);
      return false;
    }
}

/* Add TYPE to the suspect type set. Return true if the bit needed to
   be marked.  */

static bool
mark_type (tree type, enum escape_t escape_status)
{
  bitmap map = NULL;
  int uid;

  type = TYPE_MAIN_VARIANT (type);
  while (POINTER_TYPE_P (type) || TREE_CODE (type) == ARRAY_TYPE)
    type = TYPE_MAIN_VARIANT (TREE_TYPE (type));
  
  switch (escape_status) 
    {
    case EXPOSED_PARAMETER:
      map = global_types_exposed_parameter;
      break;
    case FULL_ESCAPE:
      map = global_types_full_escape;
      break;
    }

  uid = TYPE_UID (type);
  if (bitmap_bit_p (map, uid))
    return false;
  else
    {
      bitmap_set_bit (map, uid);
      mark_any_type_seen (type);

      if (escape_status == FULL_ESCAPE)
	{
	  /* Effeciency hack. When things are bad, do not mess around
	     with this type anymore.  */
	  bitmap_set_bit (global_types_exposed_parameter, uid);
	}      
    }
  return true;
}

/* Add interesting TYPE to the suspect type set. If the set is
   EXPOSED_PARAMETER and the TYPE is a pointer type, the set is
   changed to FULL_ESCAPE.  */

static void 
mark_interesting_type (tree type, enum escape_t escape_status)
{
  if (ipa_static_star_count_of_interesting_type (type) >= 0)
    {
      if ((escape_status == EXPOSED_PARAMETER)
	  && POINTER_TYPE_P (type))
	/* EXPOSED_PARAMETERs are only structs or unions are passed by
	   value.  Anything passed by reference to an external
	   function fully exposes the type.  */
	mark_type (type, FULL_ESCAPE);
      else
	mark_type (type, escape_status);
    }
}

/* Return true if PARENT is supertype of CHILD.  Both types must be
   known to be structures or unions. */
 
static bool
parent_type_p (tree parent, tree child)
{
  int i;
  tree binfo, base_binfo;
  if (TYPE_BINFO (parent)) 
    for (binfo = TYPE_BINFO (parent), i = 0;
	 BINFO_BASE_ITERATE (binfo, i, base_binfo); i++)
      {
	tree binfotype = BINFO_TYPE (base_binfo);
	if (binfotype == child) 
	  return true;
	else if (parent_type_p (binfotype, child))
	  return true;
      }
  if (TREE_CODE (parent) == UNION_TYPE
      || TREE_CODE (parent) == QUAL_UNION_TYPE) 
    {
      tree field;
      /* Search all of the variants in the union to see if one of them
	 is the child.  */
      for (field = TYPE_FIELDS (parent);
	   field;
	   field = TREE_CHAIN (field))
	{
	  tree field_type;
	  if (TREE_CODE (field) != FIELD_DECL)
	    continue;
	  
	  field_type = TREE_TYPE (field);
	  if (field_type == child) 
	    return true;
	}

      /* If we did not find it, recursively ask the variants if one of
	 their children is the child type.  */
      for (field = TYPE_FIELDS (parent);
	   field;
	   field = TREE_CHAIN (field))
	{
	  tree field_type;
	  if (TREE_CODE (field) != FIELD_DECL)
	    continue;
	  
	  field_type = TREE_TYPE (field);
	  if (TREE_CODE (field_type) == RECORD_TYPE 
	      || TREE_CODE (field_type) == QUAL_UNION_TYPE 
	      || TREE_CODE (field_type) == UNION_TYPE)
	    if (parent_type_p (field_type, child)) 
	      return true;
	}
    }
  
  if (TREE_CODE (parent) == RECORD_TYPE)
    {
      tree field;
      for (field = TYPE_FIELDS (parent);
	   field;
	   field = TREE_CHAIN (field))
	{
	  tree field_type;
	  if (TREE_CODE (field) != FIELD_DECL)
	    continue;
	  
	  field_type = TREE_TYPE (field);
	  if (field_type == child) 
	    return true;
	  /* You can only cast to the first field so if it does not
	     match, quit.  */
	  if (TREE_CODE (field_type) == RECORD_TYPE 
	      || TREE_CODE (field_type) == QUAL_UNION_TYPE 
	      || TREE_CODE (field_type) == UNION_TYPE)
	    {
	      if (parent_type_p (field_type, child)) 
		return true;
	      else 
		break;
	    }
	}
    }
  return false;
}

/* Return the number of pointer tos for TYPE and return TYPE with all
   of these stripped off.  */

static int 
count_stars (tree* type_ptr)
{
  tree type = *type_ptr;
  int i = 0;
  type = TYPE_MAIN_VARIANT (type);
  while (POINTER_TYPE_P (type))
    {
      type = TYPE_MAIN_VARIANT (TREE_TYPE (type));
      i++;
    }

  *type_ptr = type;
  return i;
}

enum cast_type {
  CT_UP,
  CT_DOWN,
  CT_SIDEWAYS,
  CT_USELESS
};

/* Check the cast FROM_TYPE to TO_TYPE.  This function requires that
   the two types have already passed the
   ipa_static_star_count_of_interesting_type test.  */

static enum cast_type
check_cast_type (tree to_type, tree from_type)
{
  int to_stars = count_stars (&to_type);
  int from_stars = count_stars (&from_type);
  if (to_stars != from_stars) 
    return CT_SIDEWAYS;

  if (to_type == from_type)
    return CT_USELESS;

  if (parent_type_p (to_type, from_type)) return CT_UP;
  if (parent_type_p (from_type, to_type)) return CT_DOWN;
  return CT_SIDEWAYS;
}     

/* Check a cast FROM this variable, TO_TYPE.  Mark the escaping types
   if appropriate.  */ 
static void
check_cast (tree to_type, tree from) 
{
  tree from_type = TYPE_MAIN_VARIANT (TREE_TYPE (from));
  bool to_interesting_type, from_interesting_type;

  to_type = TYPE_MAIN_VARIANT (to_type);
  if (from_type == to_type)
    {
      mark_type_seen (to_type);
      return;
    }

  to_interesting_type = 
    ipa_static_star_count_of_interesting_type (to_type) >= 0;
  from_interesting_type = 
    ipa_static_star_count_of_interesting_type (from_type) >= 0;

  if (to_interesting_type) 
    if (from_interesting_type)
      {
	/* Both types are interesting. This can be one of four types
	   of cast: useless, up, down, or sideways.  We do not care
	   about up or useless.  Sideways casts are always bad and
	   both sides get marked as escaping.  Downcasts are not
	   interesting here because if type is marked as escaping, all
	   of it's subtypes escape.  */
	switch (check_cast_type (to_type, from_type)) 
	  {
	  case CT_UP:
	  case CT_USELESS:
	  case CT_DOWN:
	    mark_type_seen (to_type);
	    mark_type_seen (from_type);
	    break;

	  case CT_SIDEWAYS:
	    mark_type (to_type, FULL_ESCAPE);
	    mark_type (from_type, FULL_ESCAPE);
	    break;
	  }
      }
    else
      {
	/* If this is a cast from the local that is a result from a
	   call to malloc, do not mark the cast as bad.  */
	if (DECL_P (from) && !bitmap_bit_p (results_of_malloc, DECL_UID (from)))
	  mark_type (to_type, FULL_ESCAPE);
	else
	  mark_type_seen (to_type);
      }
  else if (from_interesting_type)
    mark_type (from_type, FULL_ESCAPE);
}

/* Register the parameter and return types of function FN as
   escaping.  */
static void 
check_function_parameter_and_return_types (tree fn, bool escapes) 
{
  tree arg;
  
  if (TYPE_ARG_TYPES (TREE_TYPE (fn)))
    {
      for (arg = TYPE_ARG_TYPES (TREE_TYPE (fn));
	   arg && TREE_VALUE (arg) != void_type_node;
	   arg = TREE_CHAIN (arg))
	{
	  if (escapes)
	    mark_interesting_type (TREE_VALUE (arg), EXPOSED_PARAMETER);
	  else
	    mark_type_seen (TREE_VALUE (arg));
	}
    }
  else
    {
      /* FIXME - According to Geoff Keating, we should never have to
	 do this; the front ends should always process the arg list
	 from the TYPE_ARG_LIST. */

      for (arg = DECL_ARGUMENTS (fn); arg; arg = TREE_CHAIN (arg))
	{
	  if (escapes)
	    mark_interesting_type (TREE_TYPE (arg), EXPOSED_PARAMETER);
	  else
	    mark_type_seen (TREE_TYPE (arg));
	}
    }
  if (escapes)
    mark_interesting_type (TREE_TYPE (TREE_TYPE (fn)), EXPOSED_PARAMETER); 
  else 
    mark_type_seen (TREE_TYPE (TREE_TYPE (fn)));
}

/* Add VAR to all_module_statics and the two static_vars_to_consider*
   sets.  */

static inline void 
add_static_var (tree var) 
{
  int uid = DECL_UID (var);
  if (!bitmap_bit_p (all_module_statics, uid))
    {
      /* FIXME -- PROFILE-RESTRUCTURE: Change the call from
	 DECL_UID to get the uid from the var_ann field. */    
      splay_tree_insert (static_vars_to_consider_by_uid,
			 uid, (splay_tree_value)var);
      
      /* FIXME -- PROFILE-RESTRUCTURE: Change the call from
	 DECL_UID to get the uid from the var_ann field. */    
      bitmap_set_bit (all_module_statics, uid);
    }
}

/* FIXME this needs to be enhanced.  If we are compiling a single
   module this returns true if the variable is a module level static,
   but if we are doing whole program compilation, this could return
   true if TREE_PUBLIC is true. */
/* Return true if the variable T is the right kind of static variable to
   perform compilation unit scope escape analysis.  */

static inline bool 
has_proper_scope_for_analysis (ipa_local_static_vars_info_t local, 
			       tree t, bool checking_write)
{
  /* Do not want to do anything with volatile except mark any
     function that uses one to be not const or pure.  */
  if (TREE_THIS_VOLATILE (t)) 
    { 
      if (local && local->pure_const_not_set_in_source)
	local->pure_const_state = IPA_NEITHER;
      return false;
    }

  /* Do not care about a local automatic that is not static.  */
  if (!TREE_STATIC (t) && !DECL_EXTERNAL (t))
    return false;

  /* Since we have dispatched the locals and params, if we are writing, this
     cannot be a pure or constant function.  */
  if (checking_write && local && local->pure_const_not_set_in_source) 
    local->pure_const_state = IPA_NEITHER;

  if (DECL_EXTERNAL (t) || TREE_PUBLIC (t))
    {
      /* If the front end set the variable to be READONLY and
	 constant, we can allow this variable in pure or const
	 functions but the scope is too large for our analysis to set
	 these bits ourselves.  */
      
      if (TREE_READONLY (t)
	  && DECL_INITIAL (t)
	  && is_gimple_min_invariant (DECL_INITIAL (t)))
	; /* Read of a constant, do not change the function state.  */
      else 
	{
	  /* Just a regular read.  */
	  if (local 
	      && local->pure_const_not_set_in_source
	      && local->pure_const_state == IPA_CONST)
	    local->pure_const_state = IPA_PURE;
	 
	  /* The type escapes for all public and externs. */
	  mark_interesting_type (TREE_TYPE (t), FULL_ESCAPE);
	}
      return false;
    }

  /* This is a variable we care about.  Check if we have seen it
     before, and if not add it the set of variables we care about.  */
  if (!bitmap_bit_p (all_module_statics, DECL_UID (t)))
    add_static_var (t);

  return true;
}

/* If T is a VAR_DECL for a static that we are interrested in, add the
   uid to the bitmap.  */

static void
check_operand (ipa_local_static_vars_info_t local, 
	       tree t, bool checking_write)
{
  if (!t) return;

  /* This is an assignment from a function, register the types as
     escaping.  */
  if (TREE_CODE (t) == FUNCTION_DECL)
    check_function_parameter_and_return_types (t, true);

  /* FIXME -- PROFILE-RESTRUCTURE: Change the call from DECL_UID to
     get the uid from the var_ann field. */    
  else if (TREE_CODE (t) == VAR_DECL)
    {
      mark_type_seen (TREE_TYPE (t));
      if (has_proper_scope_for_analysis (local, t, checking_write)) 
	{
	  if (checking_write)
	    {
	      if (local)
		bitmap_set_bit (local->statics_written_by_decl_uid, DECL_UID (t));
	      /* Mark the write so we can tell which statics are
		 readonly.  */
	      bitmap_set_bit (module_statics_written, DECL_UID (t));
	    }
	  else if (local)
	    bitmap_set_bit (local->statics_read_by_decl_uid, DECL_UID (t));
	}
    }
}

/* Examine tree T for references to static variables. All internal
   references like array references or indirect references are added
   to the READ_BM. Direct references are added to either READ_BM or
   WRITE_BM depending on the value of CHECKING_WRITE.   */

static void
check_tree (ipa_local_static_vars_info_t local, tree t, bool checking_write)
{
  if ((TREE_CODE (t) == EXC_PTR_EXPR) || (TREE_CODE (t) == FILTER_EXPR))
    return;

  while (TREE_CODE (t) == REALPART_EXPR 
	 || TREE_CODE (t) == IMAGPART_EXPR
	 || handled_component_p (t))
    {
      if (TREE_CODE (t) == ARRAY_REF)
	check_operand (local, TREE_OPERAND (t, 1), false);
      t = TREE_OPERAND (t, 0);
    }

  /* The bottom of an indirect reference can only be read, not
     written.  So just recurse and whatever we find, check it against
     the read bitmaps.  */
  if (INDIRECT_REF_P (t) || TREE_CODE (t) == MEM_REF)
    {
      check_tree (local, TREE_OPERAND (t, 0), false);
      
      if (local && local->pure_const_not_set_in_source)
	{
	  /* Any indirect reference that occurs on the lhs
	     disqualifies the function from being pure or const. Any
	     indirect reference that occurs on the rhs disqualifies
	     the function from being const.  */
	  if (checking_write) 
	    local->pure_const_state = IPA_NEITHER;
	  else 
	    if (local->pure_const_state == IPA_CONST)
	      local->pure_const_state = IPA_PURE;
	}
    }

  if (SSA_VAR_P (t) || (TREE_CODE (t) == FUNCTION_DECL))
    check_operand (local, t, checking_write);
}

/* Given a memory reference T, will return the variable at the bottom
   of the access.  Unlike get_base_address, this will recurse thru
   INDIRECT_REFS.  */

static tree
get_base_var (tree t)
{
  if ((TREE_CODE (t) == EXC_PTR_EXPR) || (TREE_CODE (t) == FILTER_EXPR))
    return t;

  while (!SSA_VAR_P (t) 
	 && (!CONSTANT_CLASS_P (t))
	 && TREE_CODE (t) != LABEL_DECL
	 && TREE_CODE (t) != FUNCTION_DECL
	 && TREE_CODE (t) != CONST_DECL)
    {
      t = TREE_OPERAND (t, 0);
    }
  return t;
} 

/* Create an address_of edge FROM_TYPE.TO_TYPE.  */
static void
mark_interesting_addressof (tree to_type, tree from_type)
{
  from_type = TYPE_MAIN_VARIANT (from_type);
  to_type = TYPE_MAIN_VARIANT (to_type);
  if (ipa_static_star_count_of_interesting_type (from_type) == 0)
    {
      int uid = TYPE_UID (from_type);
      bitmap from_type_map;
      splay_tree_node result = 
	splay_tree_lookup (uid_to_addressof_map, (splay_tree_key) uid);
 
      if (result) 
	from_type_map = (bitmap) result->value;  
      else 
	{
	  from_type_map = BITMAP_ALLOC (&ipa_obstack);
	  splay_tree_insert (uid_to_addressof_map,
			     uid, 
			     (splay_tree_value)from_type_map);
	}
      bitmap_set_bit (from_type_map, TYPE_UID (to_type));
      mark_type_seen (from_type);
      mark_any_type_seen (to_type);
    }
  else 
    {
      fprintf(stderr, "trying to mark the address of pointer type ");
      print_generic_expr (stderr, from_type, 0);
      fprintf(stderr, "\n");
      abort ();
    }
}



/* Scan tree T to see if there are any addresses taken in within T.  */

static void 
look_for_address_of (ipa_local_static_vars_info_t local, tree t)
{
  if (TREE_CODE (t) == ADDR_EXPR)
    {
      tree x = get_base_var (t);
      tree cref = TREE_OPERAND (t, 0);

      /* If we have an expression of the form "&a.b.c.d", mark a.b,
	 b.c and c.d. as having it's address taken.  */ 
      tree fielddecl = NULL_TREE;
      while (cref!= x)
	{
	  if (TREE_CODE (cref) == COMPONENT_REF)
	    {
	      fielddecl =  TREE_OPERAND (cref, 1);
	      mark_interesting_addressof (TREE_TYPE (fielddecl), 
					  DECL_FIELD_CONTEXT (fielddecl));
	    }
	  else if (TREE_CODE (cref) == ARRAY_REF)
	    mark_type_seen (TREE_TYPE (cref));

	  cref = TREE_OPERAND (cref, 0);
	}

      if (TREE_CODE (x) == VAR_DECL) 
	{
	  if (has_proper_scope_for_analysis (local, x, false))
	    /* FIXME -- PROFILE-RESTRUCTURE: Change the call from
	       DECL_UID to get the uid from the var_ann field. */    
	    bitmap_set_bit (module_statics_escape, DECL_UID (x));
	  
	  /* Taking the address of something appears to be reasonable
	     in PURE code.  Not allowed in const.  */
	  if (local && local->pure_const_not_set_in_source
	      && local->pure_const_state == IPA_CONST)
	    local->pure_const_state = IPA_PURE;
	}
    }
}


/* Scan tree T to see if there are any casts within it.
   LHS Is the LHS of the expression involving the cast.  */

static void 
look_for_casts (tree lhs __attribute__((unused)), tree t)
{
  if (is_gimple_cast (t) || TREE_CODE (t) == VIEW_CONVERT_EXPR)
    {
      tree castfromvar = TREE_OPERAND (t, 0);
      check_cast (TREE_TYPE (t), castfromvar);
    }
  else if (TREE_CODE (t) == COMPONENT_REF
	   || TREE_CODE (t) == INDIRECT_REF
	   || TREE_CODE (t) == BIT_FIELD_REF)
    {
      tree base = get_base_address (t);
      while (t != base)
	{
	  t = TREE_OPERAND (t, 0);
	  if (TREE_CODE (t) == VIEW_CONVERT_EXPR)
	    {
	      /* This may be some part of a component ref.
		 IE it may be a.b.VIEW_CONVERT_EXPR<weird_type>(c).d, AFAIK.
		 castfromref will give you a.b.c, not a. */
	      tree castfromref = TREE_OPERAND (t, 0);
	      check_cast (TREE_TYPE (t), castfromref);
	    }
	  else if (TREE_CODE (t) == COMPONENT_REF)
	    mark_type_seen (TREE_TYPE (TREE_OPERAND (t, 1)));
	}
    } 
} 

/* Check to see if T is a read or address of operation on a static var
   we are interested in analyzing.  LOCAL is passed in to get access to
   its bit vectors.  */

static void
check_rhs_var (ipa_local_static_vars_info_t local, tree t)
{
  look_for_address_of (local, t);

  if (local == NULL) 
    return;

  /* Memcmp and strlen can both trap and they are declared pure.  */
  if (tree_could_trap_p (t)
      && local->pure_const_not_set_in_source
      && local->pure_const_state == IPA_CONST)
    local->pure_const_state = IPA_PURE;

  check_tree(local, t, false);
}

/* Check to see if T is an assignment to a static var we are
   interrested in analyzing.  LOCAL is passed in to get access to its bit
   vectors.
*/

static void
check_lhs_var (ipa_local_static_vars_info_t local, tree t)
{
  if (local == NULL) 
    return;

  /* Memcmp and strlen can both trap and they are declared pure.  */
  if (tree_could_trap_p (t)
      && local->pure_const_not_set_in_source
      && local->pure_const_state == IPA_CONST)
    local->pure_const_state = IPA_PURE;
    
  check_tree(local, t, true);
}

/* This is a scaled down version of get_asm_expr_operands from
   tree_ssa_operands.c.  The version there runs much later and assumes
   that aliasing information is already available. Here we are just
   trying to find if the set of inputs and outputs contain references
   or address of operations to local static variables.  FN is the
   function being analyzed and STMT is the actual asm statement.  */

static void
get_asm_expr_operands (ipa_local_static_vars_info_t local, tree stmt)
{
  int noutputs = list_length (ASM_OUTPUTS (stmt));
  const char **oconstraints
    = (const char **) alloca ((noutputs) * sizeof (const char *));
  int i;
  tree link;
  const char *constraint;
  bool allows_mem, allows_reg, is_inout;
  
  for (i=0, link = ASM_OUTPUTS (stmt); link; ++i, link = TREE_CHAIN (link))
    {
      oconstraints[i] = constraint
	= TREE_STRING_POINTER (TREE_VALUE (TREE_PURPOSE (link)));
      parse_output_constraint (&constraint, i, 0, 0,
			       &allows_mem, &allows_reg, &is_inout);
      
      check_lhs_var (local, TREE_VALUE (link));
    }

  for (link = ASM_INPUTS (stmt); link; link = TREE_CHAIN (link))
    {
      constraint
	= TREE_STRING_POINTER (TREE_VALUE (TREE_PURPOSE (link)));
      parse_input_constraint (&constraint, 0, 0, noutputs, 0,
			      oconstraints, &allows_mem, &allows_reg);
      
      check_rhs_var (local, TREE_VALUE (link));
    }
  
  for (link = ASM_CLOBBERS (stmt); link; link = TREE_CHAIN (link))
    if (simple_cst_equal(TREE_VALUE (link), memory_identifier_string) == 1) 
      {
	/* Abandon all hope, ye who enter here. */
	local->calls_read_all = true;
	local->calls_write_all = true;
	if (local->pure_const_not_set_in_source)
	  local->pure_const_state = IPA_NEITHER;
      }      

  if (ASM_VOLATILE_P (stmt))
    if (local->pure_const_not_set_in_source)
      local->pure_const_state = IPA_NEITHER;
}

/* Check the parameters of a function call from CALLER to CALL_EXPR to
   see if any of them are static vars.  Also check to see if this is
   either an indirect call, a call outside the compilation unit, or
   has special attributes that effect the clobbers.  The caller
   parameter is the tree node for the caller and the second operand is
   the tree node for the entire call expression.  */

static bool
check_call (ipa_local_static_vars_info_t local, 
	      tree call_expr) 
{
  int flags = call_expr_flags(call_expr);
  tree operand_list = TREE_OPERAND (call_expr, 1);
  tree operand;
  tree callee_t = get_callee_fndecl (call_expr);
  tree argument;
  struct cgraph_node* callee;
  enum availability avail = AVAIL_NOT_AVAILABLE;

  for (operand = operand_list;
       operand != NULL_TREE;
       operand = TREE_CHAIN (operand))
    {
      tree argument = TREE_VALUE (operand);
      check_rhs_var (local, argument);
    }
  
  /* The const and pure flags are set by a variety of places in the
     compiler (including here).  If someone has already set the flags
     for the callee, (such as for some of the builtins) we will use
     them, otherwise we will compute our own information.  */
  
  /* Const and pure functions have less clobber effects than other
     functions so we process these first.  Otherwise if it is a call
     outside the compilation unit or an indirect call we punt.  This
     leaves local calls which will be processed by following the call
     graph.  */  


  if (callee_t)
    {
      tree arg_type;
      tree last_arg_type = NULL;
      callee = cgraph_node(callee_t);
      avail = cgraph_function_body_availability (callee);

      /* If the function is POINTER_NO_ESCAPE or a wrapper it is
	 allowed to make an implicit cast to void* without causing the
	 type to escape.  */
      if (!(flags & ECF_POINTER_NO_ESCAPE)) 
	{
	  /* Check that there are no implicit casts in the passing of
	     parameters.  */
	  if (TYPE_ARG_TYPES (TREE_TYPE (callee_t)))
	    {
	      operand = operand_list;
	      for (arg_type = TYPE_ARG_TYPES (TREE_TYPE (callee_t));
		   arg_type && TREE_VALUE (arg_type) != void_type_node;
		   arg_type = TREE_CHAIN (arg_type))
		{
		  if (operand)
		    {
		      argument = TREE_VALUE (operand);
		      last_arg_type = TREE_VALUE(arg_type);
		      check_cast (last_arg_type, argument);
		      operand = TREE_CHAIN (operand);
		    }
		  else 
		    /* The code reaches here for some unfortunate
		       builtin functions that do not have a list of
		       argument types.  */
		    break; 
		}
	    } 
	  else  
	    { 
	      /* FIXME - According to Geoff Keating, we should never
		 have to do this; the front ends should always process
		 the arg list from the TYPE_ARG_LIST. */
	      operand = operand_list;
	      for (arg_type = DECL_ARGUMENTS (callee_t); 
		   arg_type;
		   arg_type = TREE_CHAIN (arg_type))
		{
		  if (operand)
		    {
		      argument = TREE_VALUE (operand);
		      last_arg_type = TREE_TYPE(arg_type);
		      check_cast (last_arg_type, argument);
		      operand = TREE_CHAIN (operand);
		    } 
		  else 
		    /* The code reaches here for some unfortunate
		       builtin functions that do not have a list of
		       argument types.  */
		    break; 
		}
	    }
	  
	  /* In the case where we have a var_args function, we need to
	     check the remaining parameters against the last argument.  */
	  arg_type = last_arg_type;
	  for (;
	       operand != NULL_TREE;
	       operand = TREE_CHAIN (operand))
	    {
	      argument = TREE_VALUE (operand);
	      if (arg_type)
		check_cast (arg_type, argument);
	      else 
		{
		  /* The code reaches here for some unfortunate
		     builtin functions that do not have a list of
		     argument types.  Most of these functions have
		     been marked as having their parameters not
		     escape, but for the rest, the type is doomed.  */
		  mark_interesting_type (TREE_TYPE (argument), FULL_ESCAPE);
		}
	    }
	}

      /* When bad things happen to bad functions, they cannot be const
	 or pure.  */
      if (local && local->pure_const_not_set_in_source)
	if (setjmp_call_p (callee_t))
	  local->pure_const_state = IPA_NEITHER;

      if (DECL_BUILT_IN_CLASS (callee_t) == BUILT_IN_NORMAL)
	switch (DECL_FUNCTION_CODE (callee_t))
	  {
	  case BUILT_IN_LONGJMP:
	  case BUILT_IN_NONLOCAL_GOTO:
	    local->pure_const_state = IPA_NEITHER;
	    break;
	  default:
	    break;
	  }
    }

  /* The callee is either unknown (indirect call) or there is just no
     scannable code for it (external call) .  We look to see if there
     are any bits available for the callee (such as by declaration or
     because it is builtin) and process solely on the basis of those
     bits. */

  if (avail == AVAIL_NOT_AVAILABLE || avail == AVAIL_OVERWRITABLE)
    {
      if (!(flags & ECF_POINTER_NO_ESCAPE))
	{
	  /* If this is a direct call to an external function, mark all of
	     the parameter and return types.  */
	  for (operand = operand_list;
	       operand != NULL_TREE;
	       operand = TREE_CHAIN (operand))
	    {
	      mark_interesting_type (TREE_TYPE (TREE_VALUE (operand)), 
				     EXPOSED_PARAMETER);
	    }
	  
	  if (callee_t) 
	    mark_interesting_type (TREE_TYPE (TREE_TYPE (callee_t)), 
				   EXPOSED_PARAMETER);
	}

      if (local) 
	{
	  if (flags & ECF_PURE) 
	    {
	      local->calls_read_all = true;
	      if (local->pure_const_not_set_in_source
		  && local->pure_const_state == IPA_CONST)
		local->pure_const_state = IPA_PURE;
	    }
	  else 
	    {
	      local->calls_read_all = true;
	      local->calls_write_all = true;
	      if (local->pure_const_not_set_in_source)
		local->pure_const_state = IPA_NEITHER;
	    }
	}
    }
  else
    {
      /* We have the code and we will scan it for the effects. */
      if (local && (flags & ECF_PURE)) 
	{
	  /* Since we have the code for the function, we do not need to
	     set calls_read_all, we can determine the precise reads
	     ourself.  */
	  if (local->pure_const_not_set_in_source
	      && local->pure_const_state == IPA_CONST)
	    local->pure_const_state = IPA_PURE;
	}
    }

  return (flags & ECF_MALLOC);
}

/* OP0 is the one we *know* is a pointer type.
   OP1 may be a pointer type.  */
static bool 
okay_pointer_operation (enum tree_code code,  tree op0, tree op1)
{
  tree op0type = TYPE_MAIN_VARIANT (TREE_TYPE (op0));
  tree op1type = TYPE_MAIN_VARIANT (TREE_TYPE (op1));
  if (POINTER_TYPE_P (op1type))
    return false;
  switch (code)
    {
    case MULT_EXPR:
    case PLUS_EXPR:
    case MINUS_EXPR:
      /* TODO: Handle multiples of op0 size as well */
      if (operand_equal_p (size_in_bytes (op0type), op1, 0))
	return true;
      /* fallthrough */

    default:
      return false;
    }
  return false;
}

/* FIXME -- PROFILE-RESTRUCTURE: Change to walk by explicitly walking
   the basic blocks rather than calling walktree.  */    

/* Walk tree and record all calls.  Called via walk_tree.  FIXME When
   this is moved into the tree-profiling-branch, and is dealing with
   low GIMPLE, this routine should be changed to use tree iterators
   rather than being a walk_tree callback.  The data is the function
   that is being scanned.  */
/* TP is the part of the tree currently under the
   microscope. WALK_SUBTREES is part of the walk_tree api but is
   unused here.  DATA is cgraph_node of the function being walked.  */

static tree
scan_for_static_refs (tree *tp, 
		      int *walk_subtrees, 
		      void *data)
{
  struct cgraph_node *fn = data;
  tree t = *tp;
  ipa_local_static_vars_info_t local = NULL;
  if (fn)
    local = fn->static_vars_info->local;

  switch (TREE_CODE (t))  
    {
    case VAR_DECL:
      if (DECL_INITIAL (t))
	walk_tree (&DECL_INITIAL (t), scan_for_static_refs, fn, visited_nodes);
      *walk_subtrees = 0;
      break;

    case MODIFY_EXPR:
      {
	/* First look on the lhs and see what variable is stored to */
	tree lhs = TREE_OPERAND (t, 0);
	tree rhs = TREE_OPERAND (t, 1);

	check_lhs_var (local, lhs);
 	check_cast (TREE_TYPE (lhs), rhs);

	/* For the purposes of figuring out what the cast affects */

	/* Next check the operands on the rhs to see if they are ok. */
	switch (TREE_CODE_CLASS (TREE_CODE (rhs))) 
	  {
	  case tcc_binary:	    
 	    {
 	      tree op0 = TREE_OPERAND (rhs, 0);
 	      tree op1 = TREE_OPERAND (rhs, 1);
 
 	      /* If this is pointer arithmetic of any bad sort, then
 		 we need to mark the types as bad.  For binary
 		 operations, no binary operator we currently support
 		 is always "safe" in regard to what it would do to
 		 pointers for purposes of determining which types
 		 escape, except operations of the size of the type.
 		 It is possible that min and max under the right set
 		 of circumstances and if the moon is in the correct
 		 place could be safe, but it is hard to see how this
 		 is worth the effort.  */
 
 	      if (POINTER_TYPE_P (TREE_TYPE (op0))
		  && !okay_pointer_operation (TREE_CODE (rhs), op0, op1))
 		mark_interesting_type (TREE_TYPE (op0), FULL_ESCAPE);
 	      if (POINTER_TYPE_P (TREE_TYPE (op1))
		  && !okay_pointer_operation (TREE_CODE (rhs), op1, op0))
 		mark_interesting_type (TREE_TYPE (op1), FULL_ESCAPE);
 	      
	      look_for_casts (lhs, op0);
	      look_for_casts (lhs, op1);
 	      check_rhs_var (local, op0);
 	      check_rhs_var (local, op1);
	    }
	    break;
	  case tcc_unary:
 	    {
 	      tree op0 = TREE_OPERAND (rhs, 0);
	      /* For unary operations, if the operation is NEGATE or
		 ABS on a pointer, this is also considered pointer
		 arithmetic and thus, bad for business.  */
 	      if ((TREE_CODE (op0) == NEGATE_EXPR
 		   || TREE_CODE (op0) == ABS_EXPR)
 		  && POINTER_TYPE_P (TREE_TYPE (op0)))
 		{
 		  mark_interesting_type (TREE_TYPE (op0), FULL_ESCAPE);
 		}
 	      check_rhs_var (local, op0);
	      look_for_casts (lhs, op0);
	      look_for_casts (lhs, rhs);
 	    }

	    break;
	  case tcc_reference:
	    look_for_casts (lhs, rhs);
	    check_rhs_var (local, rhs);
	    break;
	  case tcc_declaration:
	    check_rhs_var (local, rhs);
	    break;
	  case tcc_expression:
	    switch (TREE_CODE (rhs)) 
	      {
	      case ADDR_EXPR:
		look_for_casts (lhs, TREE_OPERAND (rhs, 0));
		check_rhs_var (local, rhs);
		break;
	      case CALL_EXPR: 
		/* If this is a call to malloc, squirrel away the
		   result so we do mark the resulting cast as being
		   bad.  */
		if (check_call (local, rhs))
		  bitmap_set_bit (results_of_malloc, DECL_UID (lhs));
		break;
	      default:
		break;
	      }
	    break;
	  default:
	    break;
	  }
	*walk_subtrees = 0;
      }
      break;

    case ADDR_EXPR:
      /* This case is here to find addresses on rhs of constructors in
	 decl_initial of static variables. */
      check_rhs_var (local, t);
      *walk_subtrees = 0;
      break;

    case LABEL_EXPR:
      if (DECL_NONLOCAL (TREE_OPERAND (t, 0)))
	{
	  /* Target of long jump. */
	  local->calls_read_all = true;
	  local->calls_write_all = true;
	  if (local->pure_const_not_set_in_source)
	    local->pure_const_state = IPA_NEITHER;
	}
      break;

    case CALL_EXPR: 
      check_call (local, t);
      *walk_subtrees = 0;
      break;
      
    case ASM_EXPR:
      get_asm_expr_operands (local, t);
      *walk_subtrees = 0;
      break;
      
    default:
      break;
    }
  return NULL;
}


/* Lookup the tree node for the static variable that has UID.  */
static tree
get_static_decl_by_uid (int index)
{
  splay_tree_node stn = 
    splay_tree_lookup (static_vars_to_consider_by_uid, index);
  if (stn)
    return (tree)stn->value;
  return NULL;
}

/* Lookup the tree node for the static variable that has UID and
   conver the name to a string for debugging.  */

static const char *
get_static_name_by_uid (int index)
{
  splay_tree_node stn = 
    splay_tree_lookup (static_vars_to_consider_by_uid, index);
  if (stn)
    return lang_hooks.decl_printable_name ((tree)(stn->value), 2);
  return NULL;
}

/* FIXME -- PROFILE-RESTRUCTURE: Change all *_decl_uid to *_ann_uid.  */

/* Or in all of the bits from every callee into X, the caller's, bit
   vector.  There are several cases to check to avoid the sparse
   bitmap oring.  */

static void
propagate_bits (struct cgraph_node *x)
{
  ipa_static_vars_info_t x_info = x->static_vars_info;
  ipa_global_static_vars_info_t x_global = x_info->global;

  struct cgraph_edge *e;
  for (e = x->callees; e; e = e->next_callee) 
    {
      struct cgraph_node *y = e->callee;

      /* Only look at the master nodes and skip external nodes.  */
      y = cgraph_master_clone (y);
      if (y)
	{
	  if (y->static_vars_info)
	    {
	      ipa_static_vars_info_t y_info = y->static_vars_info;
	      ipa_global_static_vars_info_t y_global = y_info->global;
	      
	      if (x_global->pure_const_state < y_global->pure_const_state)
		{
		  if (dump_file)
		    fprintf (dump_file, 
			     "$$$$raising level for call: %s(%d) -> %s(%d)\n",  
			     lang_hooks.decl_printable_name(x->decl, 2), 
			     x_global->pure_const_state,
			     lang_hooks.decl_printable_name(y->decl, 2),
			     y_global->pure_const_state); 
		  x_global->pure_const_state = y_global->pure_const_state;
		}
	      if (x_global->statics_read_by_decl_uid != all_module_statics)
		{
		  if (y_global->statics_read_by_decl_uid 
		      == all_module_statics)
		    {
		      BITMAP_FREE (x_global->statics_read_by_decl_uid);
		      x_global->statics_read_by_decl_uid 
			= all_module_statics;
		    }
		  /* Skip bitmaps that are pointer equal to node's bitmap
		     (no reason to spin within the cycle).  */
		  else if (x_global->statics_read_by_decl_uid 
			   != y_global->statics_read_by_decl_uid)
		    bitmap_ior_into (x_global->statics_read_by_decl_uid,
				     y_global->statics_read_by_decl_uid);
		}
	      
	      if (x_global->statics_written_by_decl_uid != all_module_statics)
		{
		  if (y_global->statics_written_by_decl_uid 
		      == all_module_statics)
		    {
		      BITMAP_FREE (x_global->statics_written_by_decl_uid);
		      x_global->statics_written_by_decl_uid 
			= all_module_statics;
		    }
		  /* Skip bitmaps that are pointer equal to node's bitmap
		     (no reason to spin within the cycle).  */
		  else if (x_global->statics_written_by_decl_uid 
			   != y_global->statics_written_by_decl_uid)
		    bitmap_ior_into (x_global->statics_written_by_decl_uid,
				     y_global->statics_written_by_decl_uid);
		}
	    }
	  else 
	    {
	      abort ();
	    }
	}
    }
}

/* FIXME -- PROFILE-RESTRUCTURE: Change all *_decl_uid to *_ann_uid.  */

/* Look at all of the callees of X to see which ones represent inlined
   calls.  For each of these callees, merge their local info into TARGET
   and check their children recursively.  */

static void 
merge_callee_local_info (struct cgraph_node *target, 
			 struct cgraph_node *x)
{
  struct cgraph_edge *e;
  ipa_local_static_vars_info_t x_l = target->static_vars_info->local;

  /* Make the world safe for tail recursion.  */
  if (x->aux) 
    return;

  x->aux = x;

  for (e = x->callees; e; e = e->next_callee) 
    {
      struct cgraph_node *y = e->callee;
      if (y->global.inlined_to) 
	{
	  ipa_static_vars_info_t y_info;
	  ipa_local_static_vars_info_t y_l;
	  struct cgraph_node* orig_y = y;
	 
	  y = cgraph_master_clone (y);
	  if (y)
	    {
	      y_info = y->static_vars_info;
	      y_l = y_info->local;
	      if (x_l != y_l)
		{
		  bitmap_ior_into (x_l->statics_read_by_decl_uid,
				   y_l->statics_read_by_decl_uid);
		  bitmap_ior_into (x_l->statics_written_by_decl_uid,
				   y_l->statics_written_by_decl_uid);
		}
	      x_l->calls_read_all |= y_l->calls_read_all;
	      x_l->calls_write_all |= y_l->calls_write_all;
	      merge_callee_local_info (target, y);
	    }
	  else 
	    {
	      fprintf(stderr, "suspect inlining of ");
	      dump_cgraph_node (stderr, orig_y);
	      fprintf(stderr, "\ninto ");
	      dump_cgraph_node (stderr, target);
	      dump_cgraph (stderr);
	      gcc_assert(false);
	    }
	}
    }

  x->aux = NULL;
}

/* The init routine for analyzing global static variable usage.  See
   comments at top for description.  */
int cant_touch = 0;
static void 
ipa_init (void) 
{
  initialization_status = RUNNING;

  memory_identifier_string = build_string(7, "memory");

  static_vars_to_consider_by_uid =
    splay_tree_new_ggc (splay_tree_compare_ints);

  bitmap_obstack_initialize (&ipa_obstack);
  module_statics_escape = BITMAP_ALLOC (&ipa_obstack);
  module_statics_written = BITMAP_ALLOC (&ipa_obstack);
  all_module_statics = BITMAP_ALLOC (&ipa_obstack);
  global_types_seen = BITMAP_ALLOC (&ipa_obstack);
  global_types_exposed_parameter = BITMAP_ALLOC (&ipa_obstack);
  global_types_full_escape = BITMAP_ALLOC (&ipa_obstack);
  results_of_malloc = BITMAP_ALLOC (&ipa_obstack);
  cant_touch = 1;
  uid_to_type = splay_tree_new (splay_tree_compare_ints, 0, 0);
  uid_to_subtype_map = splay_tree_new (splay_tree_compare_ints, 0, 0);
  uid_to_addressof_map = splay_tree_new (splay_tree_compare_ints, 0, 0);

  /* There are some shared nodes, in particular the initializers on
     static declarations.  We do not need to scan them more than once
     since all we would be interrested in are the addressof
     operations.  */
  visited_nodes = pointer_set_create ();
}

/* Check out the rhs of a static or global initialization VNODE to see
   if any of them contain addressof operations.  Note that some of
   these variables may  not even be referenced in the code in this
   compilation unit but their right hand sides may contain references
   to variables defined within this unit.  */

static void 
analyze_variable (struct cgraph_varpool_node *vnode)
{
  tree global = vnode->decl;
  switch (initialization_status) 
    {
    case UNINITIALIZED:
      ipa_init();
      break;

    case RUNNING:
      break;

    case FINISHED:
      fprintf(stderr,  
	      "AV analyze_variable called after execute for variable %s\n" , 
	      lang_hooks.decl_printable_name (global, 2)); 
      abort ();
      break;
    }

  /* If this variable has exposure beyond the compilation unit, add
     it's type to the global types.  */
  if (vnode->externally_visible)
    mark_interesting_type (TREE_TYPE (global), FULL_ESCAPE);
  else 
    mark_type_seen (TREE_TYPE (global));

  if (TREE_CODE (global) == VAR_DECL)
    {
      if (DECL_INITIAL (global)) 
	walk_tree (&DECL_INITIAL (global), scan_for_static_refs, 
		   NULL, visited_nodes);
    } 
  else abort();
}

/* This is the main routine for finding the reference patterns for
   global variables within a function FN.  */

static void
analyze_function (struct cgraph_node *fn)
{
  ipa_static_vars_info_t info 
    = xcalloc (1, sizeof (struct ipa_static_vars_info_d));
  ipa_local_static_vars_info_t l
    = xcalloc (1, sizeof (struct ipa_local_static_vars_info_d));
  var_ann_t var_ann = get_var_ann (fn->decl);
  tree decl = fn->decl;

  switch (initialization_status) 
    {
    case UNINITIALIZED:
      ipa_init();
      break;

    case RUNNING:
      break;

    case FINISHED:
      abort ();
    }

/*   fprintf (stderr, "AF called %s(%d)\n", cgraph_node_name (fn), fn->uid); */
/*  		  fprintf(stderr, "decl=%x\n", */
/*  			  fn->decl); */
/* 		  fprintf(stderr, "cfun=%x\n", */
/* 			  DECL_STRUCT_FUNCTION (fn->decl)); */
/* 		  fprintf(stderr, "cfg=%x\n", */
/* 			  DECL_STRUCT_FUNCTION (fn->decl) -> cfg); */

  /* If this function can be called from the outside, register the
     types as escaping.  */
  check_function_parameter_and_return_types (decl, fn->local.externally_visible);
    

  /* Add the info to the tree's annotation.  */
  fn->static_vars_info = info;
  var_ann->static_vars_info = info;

  info->local = l;
  l->statics_read_by_decl_uid = BITMAP_ALLOC (&ipa_obstack);
  l->statics_written_by_decl_uid = BITMAP_ALLOC (&ipa_obstack);
  l->statics_read_by_ann_uid = BITMAP_ALLOC (&ipa_obstack);
  l->statics_written_by_ann_uid = BITMAP_ALLOC (&ipa_obstack);

  l->pure_const_not_set_in_source = true;
  l->pure_const_state = IPA_CONST;

  /* If this is a volatile function, do not touch this unless it has
     been marked as const or pure by the front end.  */
  if (TREE_THIS_VOLATILE (decl))
    l->pure_const_state = IPA_NEITHER;

  if (TREE_READONLY (decl))
    {
      l->pure_const_state = IPA_CONST;
      l->pure_const_not_set_in_source = false;
    }
  if (DECL_IS_PURE (decl))
    {
      l->pure_const_state = IPA_PURE;
      l->pure_const_not_set_in_source = false;
    }

  l->pure_const_not_set_in_source = true;
  l->pure_const_state = IPA_CONST;

  /* If this is a volatile function, do not touch this unless it has
     been marked as const or pure by the front end.  */
  if (TREE_THIS_VOLATILE (decl))
    l->pure_const_state = IPA_NEITHER;

  if (TREE_READONLY (decl))
    {
      l->pure_const_state = IPA_CONST;
      l->pure_const_not_set_in_source = false;
    }
  if (DECL_IS_PURE (decl))
    {
      l->pure_const_state = IPA_PURE;
      l->pure_const_not_set_in_source = false;
    }

  if (dump_file)
    fprintf (dump_file, "\n local analysis of %s", cgraph_node_name (fn));
  
  {
    struct function *this_cfun = DECL_STRUCT_FUNCTION (decl);
    basic_block this_block;

    FOR_EACH_BB_FN (this_block, this_cfun)
      {
	block_stmt_iterator bsi;
	for (bsi = bsi_start (this_block); !bsi_end_p (bsi); bsi_next (&bsi))
	  walk_tree (bsi_stmt_ptr (bsi), scan_for_static_refs, 
		     fn, visited_nodes);
      }
  }

  /* FIXME - When Jan gets the local statics promoted to the global
     variable list, the next two loops go away.  */
  /* Walk over any private statics that may take addresses of functions.  */
  if (TREE_CODE (DECL_INITIAL (decl)) == BLOCK)
    {
      tree step;
      for (step = BLOCK_VARS (DECL_INITIAL (decl));
	   step;
	   step = TREE_CHAIN (step))
	{
	  if (DECL_INITIAL (step))
	    walk_tree (&DECL_INITIAL (step), scan_for_static_refs, 
		       fn, visited_nodes);
	  mark_type_seen (TREE_TYPE (step));
	}

    }
  
  /* Also look here for private statics.  */
  if (DECL_STRUCT_FUNCTION (decl))
    {
      tree step;
      for (step = DECL_STRUCT_FUNCTION (decl)->unexpanded_var_list;
	   step;
	   step = TREE_CHAIN (step))
	{
	  tree var = TREE_VALUE (step);
	  if (DECL_INITIAL (var) && TREE_STATIC (var))
	    walk_tree (&DECL_INITIAL (var), scan_for_static_refs, 
		       fn, visited_nodes);
	  mark_type_seen (TREE_TYPE (var));
	}
    }
}



/* Convert a type_UID into a type.  */
static tree
type_for_uid (int uid)
{
  splay_tree_node result = 
    splay_tree_lookup (uid_to_type, (splay_tree_key) uid);
  
  if (result)
    return (tree) result->value;  
  else return NULL;
}

/* Return the a bitmap with the subtypes of the type for UID.  If it
   does not exist, return either NULL or a new bitmap depending on the
   value of CREATE.  */ 

static bitmap
subtype_map_for_uid (int uid, bool create)
{
  splay_tree_node result = 
    splay_tree_lookup (uid_to_subtype_map, (splay_tree_key) uid);
  
  if (result) 
    return (bitmap) result->value;  
  else if (create)
    {
      bitmap subtype_map = BITMAP_ALLOC (&ipa_obstack);
      splay_tree_insert (uid_to_subtype_map,
			 uid, 
			 (splay_tree_value)subtype_map);
      return subtype_map;
    }
  else return NULL;
}

/* Mark all of the supertypes and field types of TYPE as being seen.
   Also accumulate the subtypes for each type so that
   close_types_full_escape can mark a subtype as escaping if the
   supertype escapes.  */

static void
close_type_seen (tree type)
{
  tree field;
  int i, uid;
  tree binfo, base_binfo;

  /* See thru all pointer tos and array ofs. */
  type = TYPE_MAIN_VARIANT (type);
  while (POINTER_TYPE_P (type) || TREE_CODE (type) == ARRAY_TYPE)
    type = TYPE_MAIN_VARIANT (TREE_TYPE (type));
  
  uid = TYPE_UID (type);

  if (bitmap_bit_p (been_there_done_that, uid))
    return;
  bitmap_set_bit (been_there_done_that, uid);

  /* If we are doing a language with a type heirarchy, mark all of
     the superclasses.  */
  if (TYPE_BINFO (type)) 
    for (binfo = TYPE_BINFO (type), i = 0;
	 BINFO_BASE_ITERATE (binfo, i, base_binfo); i++)
      {
	tree binfo_type = BINFO_TYPE (base_binfo);
	bitmap subtype_map = subtype_map_for_uid 
	  (TYPE_UID (TYPE_MAIN_VARIANT (binfo_type)), true);
	bitmap_set_bit (subtype_map, uid);
	if (mark_type_seen (binfo_type))
	  close_type_seen (binfo_type);
      }
      
  /* If the field is a struct or union type, mark all of the
     subfields.  */
  for (field = TYPE_FIELDS (type);
       field;
       field = TREE_CHAIN (field))
    {
      tree field_type;
      if (TREE_CODE (field) != FIELD_DECL)
	continue;

      field_type = TREE_TYPE (field);
      if (ipa_static_star_count_of_interesting_or_array_type (field_type) >= 0)
	if (mark_type_seen (field_type))
	  close_type_seen (field_type);
    }
}

struct type_brand_s 
{
  char* name;
  int seq;
};

/* Splay tree comparison function on type_brand_s structures.  */

static int 
compare_type_brand (splay_tree_key sk1, splay_tree_key sk2)
{
  struct type_brand_s * k1 = (struct type_brand_s *) sk1;
  struct type_brand_s * k2 = (struct type_brand_s *) sk2;

  int value = strcmp(k1->name, k2->name);
  if (value == 0)
    return k2->seq - k1->seq;
  else 
    return value;
}

/* Get the name of TYPE or return the string "<UNNAMED>".  */
static char*
get_name_of_type (tree type)
{
  tree name = TYPE_NAME (type);
  
  if (!name)
    /* Unnamed type, do what you like here.  */
    return (char*)"<UNNAMED>";
  
  /* It will be a TYPE_DECL in the case of a typedef, otherwise, an
     identifier_node */
  if (TREE_CODE (name) == TYPE_DECL)
    {
      /*  Each DECL has a DECL_NAME field which contains an
	  IDENTIFIER_NODE.  (Some decls, most often labels, may have
	  zero as the DECL_NAME).  */
      if (DECL_NAME (name))
	return (char*)IDENTIFIER_POINTER (DECL_NAME (name));
      else
	/* Unnamed type, do what you like here.  */
	return (char*)"<UNNAMED>";
    }
  else if (TREE_CODE (name) == IDENTIFIER_NODE)
    return (char*)IDENTIFIER_POINTER (name);
  else 
    return (char*)"<UNNAMED>";
}


/* Use a completely lame algorithm for removing duplicate types.  This
   code should not be here except for a bad implementation of whole
   program compilation.  */
/* Return either TYPE if this is first time TYPE has been seen an
   compatible TYPE that has already been processed.  */ 

static tree
discover_unique_type (tree type)
{
  struct type_brand_s * brand = xmalloc(sizeof(struct type_brand_s));
  int i = 0;
  splay_tree_node result;
  
  while (1)
  {
    brand->name = get_name_of_type (type);
    brand->seq = i;
    result = splay_tree_lookup (all_unique_types, (splay_tree_key) brand);
    if (result)
      {
	tree other_type = (tree) result->value;
	if (lang_hooks.types_compatible_p (type, other_type) == 1)
	  {
	    free (brand);
	    return other_type;
	  }
	/* Not compatible, look for next instance with same name.  */
      }
    else 
      {
	/* No more instances, create new one.  */
	brand->seq = i++;
	splay_tree_insert (all_unique_types, 
			   (splay_tree_key) brand,
			   (splay_tree_value) type);	  
	
	return type;
      }
    i++;
  } 
}

/* Take a TYPE that has been passed by value to an external function
   and mark all of the fields that have pointer types as escaping. For
   any of the non pointer types that are structures or unions,
   recurse.  TYPE is never a pointer type.  */ 

static void
close_type_exposed_parameter (tree type)
{
  tree field;
  int uid = TYPE_UID (TYPE_MAIN_VARIANT (type));

  if (bitmap_bit_p (been_there_done_that, uid))
    return;
  bitmap_set_bit (been_there_done_that, uid);

  /* If the field is a struct or union type, mark all of the
     subfields.  */
  for (field = TYPE_FIELDS (type);
       field;
       field = TREE_CHAIN (field))
    {
      tree field_type;

      if (TREE_CODE (field) != FIELD_DECL)
	continue;

      field_type = TREE_TYPE (field);
      mark_interesting_type (field_type, EXPOSED_PARAMETER);

      if (ipa_static_star_count_of_interesting_type (field_type) == 0) 
	close_type_exposed_parameter (field_type);
    }
}

/* The next function handles the case where a type fully escapes.
   This means that not only does the type itself escape, 

   a) the type of every field recursively escapes
   b) the type of every subtype escapes as well as the super as well
   as all of the pointer to types for each field.

   Note that pointer to types are not marked as escaping.  If the
   pointed to type escapes, the pointer to type also escapes.

   Take a TYPE that has had the address taken for an instance of it
   and mark all of the types for its fields as having their addresses
   taken. */ 

static void
close_type_full_escape (tree type)
{
  tree field;
  unsigned int i;
  int uid;
  tree binfo, base_binfo;
  bitmap_iterator bi;
  bitmap subtype_map;

  /* Strip off any pointer or array types.  */
  type = TYPE_MAIN_VARIANT (type);
  while (POINTER_TYPE_P (type) || TREE_CODE(type) == ARRAY_TYPE)
    type = TYPE_MAIN_VARIANT (TREE_TYPE (type));

  uid = TYPE_UID (type);

  if (bitmap_bit_p (been_there_done_that, uid))
    return;
  bitmap_set_bit (been_there_done_that, uid);

  subtype_map = subtype_map_for_uid (uid, false);

  /* If we are doing a language with a type heirarchy, mark all of
     the superclasses.  */
  if (TYPE_BINFO (type)) 
    for (binfo = TYPE_BINFO (type), i = 0;
	 BINFO_BASE_ITERATE (binfo, i, base_binfo); i++)
      {
	tree binfotype = BINFO_TYPE (base_binfo);
	if (mark_type (binfotype, FULL_ESCAPE))
	  close_type_full_escape (binfotype);
      }
      
  /* Mark as escaped any types that have been down casted to
     this type. */
  if (subtype_map)
    EXECUTE_IF_SET_IN_BITMAP (subtype_map, 0, i, bi)
      {
	tree subtype = type_for_uid (i); 
	if (mark_type (subtype, FULL_ESCAPE))
	  close_type_full_escape (subtype);
      }

  /* If the field is a struct or union type, mark all of the
     subfields.  */
  for (field = TYPE_FIELDS (type);
       field;
       field = TREE_CHAIN (field))
    {
      tree field_type;
      if (TREE_CODE (field) != FIELD_DECL)
	continue;

      field_type = TREE_TYPE (field);
      if (ipa_static_star_count_of_interesting_or_array_type (field_type) >= 0)
	if (mark_type (field_type, FULL_ESCAPE))
	  close_type_full_escape (field_type);
    }
}

/* It is not necessary to carry around the addressof map for any
   escaping TYPE.  */
 
static void 
delete_addressof_map (tree from_type) 
{
  int uid = TYPE_UID (TYPE_MAIN_VARIANT (from_type));
  splay_tree_node result = 
    splay_tree_lookup (uid_to_addressof_map, (splay_tree_key) uid);
  
  if (result) 
    {
      bitmap map = (bitmap) result->value;
      BITMAP_FREE (map);
      splay_tree_remove (uid_to_addressof_map, (splay_tree_key) uid);
    }
}

/* Transitively close the addressof bitmap for the type with UID.
   This means that if we had a.b and b.c, a would have both b an c in
   it's maps.  */ 

static bitmap
close_addressof (int uid) 
{
  bitmap_iterator bi;
  splay_tree_node result = 
    splay_tree_lookup (uid_to_addressof_map, (splay_tree_key) uid);
  bitmap map = NULL;
  bitmap new_map;
  unsigned int i;
  
  if (result) 
    map = (bitmap) result->value;
  else 
    return NULL;

  if (bitmap_bit_p (been_there_done_that, uid))
    return map;
  bitmap_set_bit (been_there_done_that, uid);

  /* The new_map will have all of the bits for the enclosed fields and
     will have the unique id version of the old map.  */
  new_map = BITMAP_ALLOC (&ipa_obstack);

  EXECUTE_IF_SET_IN_BITMAP (map, 0, i, bi)
    {
      int new_uid = unique_type_id_for (i, false);
      bitmap submap = close_addressof (new_uid);
      bitmap_set_bit (new_map, new_uid);
      if (submap) 
	bitmap_ior_into (new_map, submap);
    }      
  result->value = (splay_tree_value) new_map;

  BITMAP_FREE (map);
  return new_map;
}

/* Do all of the closures to discover which types escape the
   compilation unit.  */

static void
do_type_analysis (void)
{ 
  unsigned int i;
  bitmap_iterator bi;
  splay_tree_node result;

  been_there_done_that = BITMAP_ALLOC (&ipa_obstack);

  /* Examine the types that we have directly seen in scanning the code
     and add to that any contained types or superclasses.  */

  EXECUTE_IF_SET_IN_BITMAP (global_types_seen, 0, i, bi)
    {
      tree type = type_for_uid (i);
      /* Only look at records and unions with no pointer tos.  */
      if (ipa_static_star_count_of_interesting_or_array_type (type) == 0)
	close_type_seen (type);
    }
  bitmap_clear (been_there_done_that);

  /* Map the duplicate types to a single unique type.  This is a hack
     it is not a general algorithm.  */
  uid_to_unique_type = splay_tree_new (splay_tree_compare_ints, 0, 0);
  all_unique_types = splay_tree_new (compare_type_brand, 0, 0);
  
  EXECUTE_IF_SET_IN_BITMAP (global_types_seen, 0, i, bi)
    {
      tree unique_type = discover_unique_type (type_for_uid (i));
      splay_tree_insert (uid_to_unique_type, 
			 (splay_tree_key) i, 
			 (splay_tree_value) unique_type);
      if (dump_file)
	fprintf(dump_file, "dta i=%d,%d j=%d,%s\n", i, 
		TYPE_UID(type_for_uid(i)),
		TYPE_UID(unique_type), get_name_of_type(unique_type));
    }

  /* Get rid of the temporary data structures used to find the unique
     type.  */
  result = splay_tree_min (all_unique_types);
  while (result)
    {
      struct type_brand_s * b = (struct type_brand_s *) result->key;
      splay_tree_remove (all_unique_types, result->key);
      free (b);
      result = splay_tree_min (all_unique_types);
    }
  splay_tree_delete (all_unique_types);
  all_unique_types = NULL;

  /* Examine all of the types passed by value and mark any enclosed
     pointer types as escaping.  */

  EXECUTE_IF_SET_IN_BITMAP (global_types_exposed_parameter, 0, i, bi)
    {
      close_type_exposed_parameter (type_for_uid (i));
    }
  bitmap_clear (been_there_done_that);

  /* Close the types for escape.  If something escapes, then any
     enclosed types escape as well as any subtypes.  */

  EXECUTE_IF_SET_IN_BITMAP (global_types_full_escape, 0, i, bi)
    {
      tree type = type_for_uid (i);
      close_type_full_escape (type);
    }
  bitmap_clear (been_there_done_that);

  result = splay_tree_min (uid_to_addressof_map);
  while (result)
    {
      int uid = result->key;
      tree type = type_for_uid (uid);
      if (bitmap_bit_p (global_types_full_escape, uid))
	/* If the type escaped, we will never use the map, so get rid
	   of it.  */ 
	delete_addressof_map (type);
      else  
	{
	  if (unique_type_id_p (uid))
	    /* Close the addressof map, i.e. copy all of the
	       transitive substructures up to this level.  */
	    close_addressof (uid);
	  else
	    /* This type is not the unique designate, so get rid of
	       it.  */
	    delete_addressof_map (type);
	}
      result = splay_tree_successor (uid_to_addressof_map, uid);
    }
  
  /* If a type is set in global_types_full_escape, make sure that the
     unique type is also set in that map.  */
  bitmap_copy (been_there_done_that, global_types_full_escape);
  EXECUTE_IF_SET_IN_BITMAP (been_there_done_that, 0, i, bi)
    {
      unsigned int j = unique_type_id_for (i, false);
      if (i != j)
	{
	  bitmap_set_bit(global_types_full_escape, j);
	  bitmap_clear_bit(global_types_full_escape, i);
	}
    }
  bitmap_clear (been_there_done_that);

  if (dump_file)
    { 
      EXECUTE_IF_SET_IN_BITMAP (global_types_seen, 0, i, bi)
	{
	  /* The pointer types are in the global_types_full_escape bitmap
	     but not in the backwards map.  */
	  tree type = type_for_uid (i);
	  fprintf(dump_file, "type %d ", i);
	  print_generic_expr (dump_file, type, 0);
	  if (bitmap_bit_p (global_types_full_escape, i))
	    fprintf(dump_file, " escaped\n");
	  else if (unique_type_id_p (i))
	    fprintf(dump_file, " contained\n");
	  else
	    fprintf(dump_file, " replaced\n");
	}
    }

  /* Get rid of the subtype map.  */
  result = splay_tree_min (uid_to_subtype_map);
  while (result)
    {
      bitmap b = (bitmap)result->value;
      BITMAP_FREE(b);
      splay_tree_remove (uid_to_subtype_map, result->key);
      result = splay_tree_min (uid_to_subtype_map);
    }
  splay_tree_delete (uid_to_subtype_map);
  uid_to_subtype_map = NULL;

  BITMAP_FREE (global_types_exposed_parameter);
  BITMAP_FREE (been_there_done_that);
  BITMAP_FREE (results_of_malloc);

  cant_touch = 0;
}


/* Produce the global information by preforming a transitive closure
   on the local information that was produced by ipa_analyze_function
   and ipa_analyze_variable.  */

static void
static_execute (void)
{
  struct cgraph_node *node;
  struct cgraph_node *w;
  struct cgraph_node **order =
    xcalloc (cgraph_n_nodes, sizeof (struct cgraph_node *));
  int order_pos = order_pos = reduced_inorder (order, false);
  int i;

  switch (initialization_status) 
    {
    case UNINITIALIZED:
      ipa_init();
      break;

    case RUNNING:
      break;

    case FINISHED:
      abort ();
    }

  /* This will force an abort if analyze_variable or analyze_function
     is called after static_execute.  */
  initialization_status = FINISHED;

  pointer_set_destroy (visited_nodes);
  visited_nodes = NULL;
  if (dump_file) 
    dump_cgraph (dump_file);

  /* Prune out the variables that were found to behave badly
     (i.e. have there address taken).  */
  {
    unsigned int index;
    bitmap_iterator bi;
    bitmap module_statics_readonly = BITMAP_ALLOC (&ipa_obstack);
    bitmap module_statics_const = BITMAP_ALLOC (&ipa_obstack);
    bitmap bm_temp = BITMAP_ALLOC (&ipa_obstack);

    EXECUTE_IF_SET_IN_BITMAP (module_statics_escape, 0, index, bi)
      {
	splay_tree_remove (static_vars_to_consider_by_uid, index);
      }

    bitmap_and_compl_into (all_module_statics, module_statics_escape);

    bitmap_and_compl (module_statics_readonly, all_module_statics,
		      module_statics_written);

    /* If the address is not taken, we can unset the addressable bit
       on this variable.  */
    EXECUTE_IF_SET_IN_BITMAP (all_module_statics, 0, index, bi)
      {
	tree var = get_static_decl_by_uid (index);
 	TREE_ADDRESSABLE (var) = 0;
	if (dump_file) 
	  fprintf (dump_file, "&&&&NOT TREE ADDRESSABLE VAR %s\n",
		   get_static_name_by_uid (index));
      }

    /* If the variable is never written, we can set the TREE_READONLY
       flag.  Additionally if it has a DECL_INITIAL that is made up of
       constants we can treat the entire global as a constant.  */

    bitmap_and_compl (module_statics_readonly, all_module_statics,
		      module_statics_written);
    EXECUTE_IF_SET_IN_BITMAP (module_statics_readonly, 0, index, bi)
      {
	tree var = get_static_decl_by_uid (index);
	TREE_READONLY (var) = 1;
	if (dump_file)
	  fprintf (dump_file, "&&&&READONLY VAR %s\n", 
		   get_static_name_by_uid (index)); 
	if (DECL_INITIAL (var)
	    && is_gimple_min_invariant (DECL_INITIAL (var)))
	  {
 	    bitmap_set_bit (module_statics_const, index);
	    if (dump_file)
	      fprintf (dump_file, "&&&&READONLY CONST %s\n",
		       get_static_name_by_uid (index));
	  }
      }

    BITMAP_FREE(module_statics_escape);
    BITMAP_FREE(module_statics_written);

    if (0) {
      FILE *ok_statics_file = fopen("/home/zadeck/ok_statics", "r");
      char line[100];
      int value;
      fgets(line, sizeof(line), ok_statics_file);
      sscanf(line, "%d", &value);
      i = 0;
      while (!bitmap_empty_p (all_module_statics)) {
   	{
	  int j;
	  if (i >= value) break;
   	  j = bitmap_first_set_bit (all_module_statics);
   	  fprintf(stderr, "  not considering %s\n",
   		  get_static_name_by_uid (j));
   	  splay_tree_remove (static_vars_to_consider_by_uid, j);
   	  bitmap_clear_bit(all_module_statics, j);
	  i++;
   	}
      }
    }

    if (dump_file)
      EXECUTE_IF_SET_IN_BITMAP (all_module_statics, 0, index, bi)
	{
	  fprintf (dump_file, "\nPromotable global:%s",
		   get_static_name_by_uid (index));
	}

    for (i = 0; i < order_pos; i++ )
      {
	ipa_local_static_vars_info_t l;
	node = order[i];
	l = node->static_vars_info->local;

	/* First we check the bitmaps to see if there is anything in
	   them that would disqualify the CONST or PURE status. */
	/* WARNING, this case statement has drop throughs.  */
	if (l->pure_const_not_set_in_source)
	  switch (l->pure_const_state)
	    {
	    case IPA_CONST:
	      /* Any bits in the read bitmap that are not in the
		 module_statics_const bitmap disqualify a function from
		 being const.  There is no reason to check the
		 calls_read_all flag since if the bit has been set the
		 function cannot have been marked const.  */
	      bitmap_and_compl (bm_temp, l->statics_read_by_decl_uid,
				module_statics_const);
	      if (!bitmap_empty_p (bm_temp))
		l->pure_const_state = IPA_PURE;
	      else 
		{
		  tree old_decl = current_function_decl;
		  /* Const functions cannot have back edges (an
		     indication of possible infinite loop side
		     effect.  */

		  current_function_decl = node->decl;
		  push_cfun (DECL_STRUCT_FUNCTION (node->decl));
/* 		  fprintf(stderr, "decl=%x\n", */
/* 			  node->decl); */
/* 		  fprintf(stderr, "cfun=%x\n", */
/* 			  DECL_STRUCT_FUNCTION (node->decl)); */
/* 		  fprintf(stderr, "cfg=%x\n", */
/* 			  DECL_STRUCT_FUNCTION (node->decl) -> cfg); */

/* 		  fprintf (stderr, "n_basic_block=%d\n",  */
/* 			   n_basic_blocks); */

		  /* This is an utter HACK FIXEME PLEASE!!!!!!!  The
		     C++ front end, for reasons that are only apparent
		     to it's designers, decides that it was only
		     kidding when it generated some of the functions.
		     As a further joke it then decides, after
		     compilation is underway, to null out the
		     DECL_STRUCT_FUNCTION (node->decl) field of these
		     function so that no one can use them.  What a
		     funny joke!!!!  */
		  
		  if (DECL_STRUCT_FUNCTION (node->decl))
		    {
		      if (mark_dfs_back_edges ())
			l->pure_const_state = IPA_NEITHER;
		    }
		  else 
		    {
/* 		      fprintf (stderr, "jan it is still happening\n"); */
		      l->pure_const_state = IPA_NEITHER;
		    }

		  current_function_decl = old_decl;
		  pop_cfun ();
		}
	      
	    case IPA_PURE:
	      /* There is no reason to check the calls_write_all flag
		 since if the bit has been set the function cannot have
		 been marked const.  Any bits in the function's write
		 bitmap disqualify pure and const.  */
	      if (!bitmap_empty_p(l->statics_written_by_decl_uid))
		l->pure_const_state = IPA_NEITHER;
	      
	    default:
	      break;
	    }

	/* Any variables that are not in all_module_statics are
	   removed from the local maps.  This will include all of the
	   variables that were found to escape in the function
	   scanning.  */

	bitmap_and_into (l->statics_read_by_decl_uid, 
		         all_module_statics);
	bitmap_and_into (l->statics_written_by_decl_uid, 
		         all_module_statics);
      }

    BITMAP_FREE(module_statics_readonly);
    BITMAP_FREE(module_statics_const);
    BITMAP_FREE(bm_temp);
  }

  if (dump_file)
    {
      for (i = 0; i < order_pos; i++ )
	{
	  unsigned int index;
	  ipa_local_static_vars_info_t l;
	  bitmap_iterator bi;

	  node = order[i];
	  l = node->static_vars_info->local;
	  fprintf (dump_file, 
		   "\nFunction name:%s/%i:", 
		   cgraph_node_name (node), node->uid);
	  fprintf (dump_file, "\n  locals read: ");
	  EXECUTE_IF_SET_IN_BITMAP (l->statics_read_by_decl_uid,
				    0, index, bi)
	    {
	      fprintf (dump_file, "%s ",
		       get_static_name_by_uid (index));
	    }
	  fprintf (dump_file, "\n  locals written: ");
	  EXECUTE_IF_SET_IN_BITMAP (l->statics_written_by_decl_uid,
				    0, index, bi)
	    {
	      fprintf(dump_file, "%s ",
		      get_static_name_by_uid (index));
	    }
	}
    }

  /* Propagate the local information thru the call graph to produce
     the global information.  All the nodes within a cycle will have
     the same info so we collapse cycles first.  Then we can do the
     propagation in one pass from the leaves to the roots.  */
  order_pos = reduced_inorder (order, true);
  if (dump_file)
    print_order(dump_file, "reduced", order, order_pos);

  for (i = 0; i < order_pos; i++ )
    {
      ipa_static_vars_info_t node_info;
      ipa_global_static_vars_info_t node_g = 
	xcalloc (1, sizeof (struct ipa_global_static_vars_info_d));
      ipa_local_static_vars_info_t node_l;
      
      bool read_all;
      bool write_all;
      enum ipa_static_pure_const_state pure_const_state;

      node = order[i];
      node_info = node->static_vars_info;
      if (!node_info) 
	{
	  dump_cgraph_node (stderr, node);
	  dump_cgraph (stderr);
	  abort ();
	}

      node_info->global = node_g;
      node_l = node_info->local;

      read_all = node_l->calls_read_all;
      write_all = node_l->calls_write_all;
      pure_const_state = node_l->pure_const_state;

      /* If any node in a cycle is calls_read_all or calls_write_all
	 they all are. */
      w = node->next_cycle;
      while (w)
	{
	  ipa_local_static_vars_info_t w_l = w->static_vars_info->local;
	  read_all |= w_l->calls_read_all;
	  write_all |= w_l->calls_write_all;
	  if (node_l->pure_const_state < w_l->pure_const_state)
	    pure_const_state = w_l->pure_const_state;

	  w = w->next_cycle;
	}

      /* Initialized the bitmaps for the reduced nodes */
      if (read_all) 
	node_g->statics_read_by_decl_uid = all_module_statics;
      else 
	{
	  node_g->statics_read_by_decl_uid = BITMAP_ALLOC (&ipa_obstack);
	  bitmap_copy (node_g->statics_read_by_decl_uid, 
		       node_l->statics_read_by_decl_uid);
	}

      if (write_all) 
	node_g->statics_written_by_decl_uid = all_module_statics;
      else
	{
	  node_g->statics_written_by_decl_uid = BITMAP_ALLOC (&ipa_obstack);
	  bitmap_copy (node_g->statics_written_by_decl_uid, 
		       node_l->statics_written_by_decl_uid);
	}

      /* Copy the region's pure_const_state which is shared by all
	 nodes in the region.  */
      node_g->pure_const_state = pure_const_state;

      w = node->next_cycle;
      while (w)
	{
	  ipa_static_vars_info_t w_info = w->static_vars_info;
	  ipa_local_static_vars_info_t w_l = w_info->local;

	  /* All nodes within a cycle share the same global info bitmaps.  */
	  w_info->global = node_g;
	  
	  /* These global bitmaps are initialized from the local info
	     of all of the nodes in the region.  However there is no
	     need to do any work if the bitmaps were set to
	     all_module_statics.  */
	  if (!read_all)
	    bitmap_ior_into (node_g->statics_read_by_decl_uid,
			     w_l->statics_read_by_decl_uid);
	  if (!write_all)
	    bitmap_ior_into (node_g->statics_written_by_decl_uid,
			     w_l->statics_written_by_decl_uid);
	  w = w->next_cycle;
	}

      w = node;
      while (w)
	{
	  propagate_bits (w);
	  w = w->next_cycle;
	}
    }

  /* Need to fix up the local information sets.  The information that
     has been gathered so far is preinlining.  However, the
     compilation will progress post inlining so the local sets for the
     inlined calls need to be merged into the callers.  Note that the
     local sets are not shared between all of the nodes in a cycle so
     those nodes in the cycle must be processed explicitly.  */
  for (i = 0; i < order_pos; i++ )
    {
      node = order[i];
      merge_callee_local_info (node, node);
      
      w = node->next_cycle;
      while (w)
	{
	  merge_callee_local_info (w, w);
	  w = w->next_cycle;
	}
    }

  if (dump_file)
    {
      for (i = 0; i < order_pos; i++ )
	{
	  ipa_static_vars_info_t node_info;
	  ipa_global_static_vars_info_t node_g;
	  ipa_local_static_vars_info_t node_l;
	  unsigned int index;
	  bitmap_iterator bi;

	  node = order[i];
	  node_info = node->static_vars_info;
	  node_g = node_info->global;
	  node_l = node_info->local;
	  fprintf (dump_file, 
		   "\nFunction name:%s/%i:", 
		   cgraph_node_name (node), node->uid);
	  fprintf (dump_file, "\n  locals read: ");
	  EXECUTE_IF_SET_IN_BITMAP (node_l->statics_read_by_decl_uid,
				    0, index, bi)
	    {
	      fprintf (dump_file, "%s ",
		       get_static_name_by_uid (index));
	    }
	  fprintf (dump_file, "\n  locals written: ");
	  EXECUTE_IF_SET_IN_BITMAP (node_l->statics_written_by_decl_uid,
				    0, index, bi)
	    {
	      fprintf(dump_file, "%s ",
		      get_static_name_by_uid (index));
	    }

	  w = node->next_cycle;
	  while (w) 
	    {
	      ipa_static_vars_info_t w_info = w->static_vars_info;
	      ipa_local_static_vars_info_t w_l = w_info->local;
	      fprintf (dump_file, "\n  next cycle: %s/%i ",
		       cgraph_node_name (w), w->uid);
 	      fprintf (dump_file, "\n    locals read: ");
	      EXECUTE_IF_SET_IN_BITMAP (w_l->statics_read_by_decl_uid,
					0, index, bi)
		{
		  fprintf (dump_file, "%s ",
			   get_static_name_by_uid (index));
		}

	      fprintf (dump_file, "\n    locals written: ");
	      EXECUTE_IF_SET_IN_BITMAP (w_l->statics_written_by_decl_uid,
					0, index, bi)
		{
		  fprintf(dump_file, "%s ",
			  get_static_name_by_uid (index));
		}
	      

	      w = w->next_cycle;
	    }
	  fprintf (dump_file, "\n  globals read: ");
	  EXECUTE_IF_SET_IN_BITMAP (node_g->statics_read_by_decl_uid,
				    0, index, bi)
	    {
	      fprintf (dump_file, "%s ",
		       get_static_name_by_uid (index));
	    }
	  fprintf (dump_file, "\n  globals written: ");
	  EXECUTE_IF_SET_IN_BITMAP (node_g->statics_written_by_decl_uid,
				    0, index, bi)
	    {
	      fprintf (dump_file, "%s ",
		       get_static_name_by_uid (index));
	    }
	}
    }

  do_type_analysis ();

  /* Cleanup. */
  for (i = 0; i < order_pos; i++ )
    {
      ipa_static_vars_info_t node_info;
      ipa_global_static_vars_info_t node_g;
      node = order[i];
      node_info = node->static_vars_info;
      node_g = node_info->global;
      
      node_g->var_anns_valid = false;

      /* Create the complimentary sets.  These are more useful for
	 certain apis.  */
      node_g->statics_not_read_by_decl_uid = BITMAP_ALLOC (&ipa_obstack);
      node_g->statics_not_written_by_decl_uid = BITMAP_ALLOC (&ipa_obstack);

      if (node_g->statics_read_by_decl_uid != all_module_statics) 
	{
	  bitmap_and_compl (node_g->statics_not_read_by_decl_uid, 
			    all_module_statics,
			    node_g->statics_read_by_decl_uid);
	}

      if (node_g->statics_written_by_decl_uid != all_module_statics) 
	bitmap_and_compl (node_g->statics_not_written_by_decl_uid, 
			  all_module_statics,
			  node_g->statics_written_by_decl_uid);

      /* FIXME -- PROFILE-RESTRUCTURE: Delete next 4 assignments.  */
      node_g->statics_read_by_ann_uid = BITMAP_ALLOC (&ipa_obstack);
      node_g->statics_written_by_ann_uid = BITMAP_ALLOC (&ipa_obstack);
      node_g->statics_not_read_by_ann_uid = BITMAP_ALLOC (&ipa_obstack);
      node_g->statics_not_written_by_ann_uid = BITMAP_ALLOC (&ipa_obstack);

      w = node;
      while (w)
	{
	  struct cgraph_node * last = w;
	  ipa_static_vars_info_t w_info = w->static_vars_info;
	  ipa_local_static_vars_info_t w_l = w_info->local;
	  w_l->var_anns_valid = false;

	  if (w_l->pure_const_not_set_in_source) 
	    switch (node_g->pure_const_state)
	      {
	      case IPA_CONST:
   		TREE_READONLY (w->decl) = 1;
		if (dump_file)
		  fprintf (dump_file, "$$$$CONST: %s\n",  
			   lang_hooks.decl_printable_name(w->decl, 2)); 
		break;
		
	      case IPA_PURE:
   		DECL_IS_PURE (w->decl) = 1;
		if (dump_file)
		  fprintf (dump_file, "$$$$PURE: %s\n",  
			   lang_hooks.decl_printable_name(w->decl, 2)); 
		break;
		
	      default:
		break;
	      }
	  
	  w = w->next_cycle;
	  last->next_cycle = NULL;
	}
    }

  free (order);
}

/* If FN is avail == AVAIL_OVERWRITABLE, replace the effects bit vectors with 
   worst case bit vectors.  */

static void
clean_function (struct cgraph_node *fn)
{
  if (cgraph_function_body_availability (fn) == AVAIL_OVERWRITABLE)
    {
      ipa_local_static_vars_info_t l = fn->static_vars_info->local;
      ipa_global_static_vars_info_t g = fn->static_vars_info->global;
      var_ann_t var_ann = get_var_ann (fn->decl);

      if (l)
	{
	  if (l->statics_read_by_decl_uid
	      && l->statics_read_by_decl_uid != all_module_statics)
	    BITMAP_FREE (l->statics_read_by_decl_uid);
	  if (l->statics_written_by_decl_uid
	      &&l->statics_written_by_decl_uid != all_module_statics)
	    BITMAP_FREE (l->statics_written_by_decl_uid);
	  free (l);
	  fn->static_vars_info->local = NULL;
	}

      if (g)
	{
	  if (g->statics_read_by_decl_uid
	      && g->statics_read_by_decl_uid != all_module_statics)
	    BITMAP_FREE (g->statics_read_by_decl_uid);

	  if (g->statics_written_by_decl_uid
	      && g->statics_written_by_decl_uid != all_module_statics)
	    BITMAP_FREE (g->statics_written_by_decl_uid);

	  if (g->statics_not_read_by_decl_uid
	      && g->statics_not_read_by_decl_uid != all_module_statics)
	    BITMAP_FREE (g->statics_not_read_by_decl_uid);
	  
	  if (g->statics_not_written_by_decl_uid
	      && g->statics_not_written_by_decl_uid != all_module_statics)
	    BITMAP_FREE (g->statics_not_written_by_decl_uid);
	  free (g);
	  fn->static_vars_info->global = NULL;
	}
      
      free (fn->static_vars_info);
      fn->static_vars_info = NULL;
      var_ann->static_vars_info = NULL;
    }
}

static bool
gate_static_vars (void)
{
  return (flag_unit_at_a_time != 0
	  /* Don't bother doing anything if the program has errors.  */
	  && !(errorcount || sorrycount));
}

struct tree_opt_pass pass_ipa_static =
{
  "static-var",				/* name */
  gate_static_vars,			/* gate */
  analyze_function,                     /* IPA function */
  analyze_variable,		        /* IPA variable */
  static_execute,			/* execute */
  clean_function,                       /* IPA function */
  NULL,	                                /* IPA variable */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_IPA_STATIC_VAR,		        /* tv_id */
  0,	                                /* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  0,                                    /* todo_flags_finish */
  0					/* letter */
};

#include "gt-ipa-static-vars-anal.h"
