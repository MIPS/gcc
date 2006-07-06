/* SSA operands management for trees.
   Copyright (C) 2003, 2004, 2005, 2006 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "flags.h"
#include "function.h"
#include "diagnostic.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "tree-pass.h"
#include "ggc.h"
#include "timevar.h"
#include "toplev.h"
#include "langhooks.h"
#include "ipa-reference.h"

/* This file contains the code required to manage the operands cache of the 
   SSA optimizer.  For every stmt, we maintain an operand cache in the stmt 
   annotation.  This cache contains operands that will be of interest to 
   optimizers and other passes wishing to manipulate the IL. 

   The operand type are broken up into REAL and VIRTUAL operands.  The real 
   operands are represented as pointers into the stmt's operand tree.  Thus 
   any manipulation of the real operands will be reflected in the actual tree.
   Virtual operands are represented solely in the cache, although the base 
   variable for the SSA_NAME may, or may not occur in the stmt's tree.  
   Manipulation of the virtual operands will not be reflected in the stmt tree.

   The routines in this file are concerned with creating this operand cache 
   from a stmt tree.

   The operand tree is the parsed by the various get_* routines which look 
   through the stmt tree for the occurrence of operands which may be of 
   interest, and calls are made to the append_* routines whenever one is 
   found.  There are 4 of these routines, each representing one of the 
   4 types of operands. Defs, Uses, Virtual Uses, and Virtual May Defs.

   The append_* routines check for duplication, and simply keep a list of 
   unique objects for each operand type in the build_* extendable vectors.

   Once the stmt tree is completely parsed, the finalize_ssa_operands() 
   routine is called, which proceeds to perform the finalization routine 
   on each of the 4 operand vectors which have been built up.

   If the stmt had a previous operand cache, the finalization routines 
   attempt to match up the new operands with the old ones.  If it's a perfect 
   match, the old vector is simply reused.  If it isn't a perfect match, then 
   a new vector is created and the new operands are placed there.  For 
   virtual operands, if the previous cache had SSA_NAME version of a 
   variable, and that same variable occurs in the same operands cache, then 
   the new cache vector will also get the same SSA_NAME.

  i.e., if a stmt had a VUSE of 'a_5', and 'a' occurs in the new operand 
  vector for VUSE, then the new vector will also be modified such that 
  it contains 'a_5' rather than 'a'.  */


/* Structure storing statistics on how many call clobbers we have, and
   how many where avoided.  */

static struct 
{
  /* Number of call-clobbered ops we attempt to add to calls in
     add_call_clobbered_mem_symbols.  */
  unsigned int clobbered_vars;

  /* Number of write-clobbers (VDEFs) avoided by using
     not_written information.  */
  unsigned int static_write_clobbers_avoided;

  /* Number of reads (VUSEs) avoided by using not_read information.  */
  unsigned int static_read_clobbers_avoided;
  
  /* Number of write-clobbers avoided because the variable can't escape to
     this call.  */
  unsigned int unescapable_clobbers_avoided;

  /* Number of read-only uses we attempt to add to calls in
     add_call_read_mem_symbols.  */
  unsigned int readonly_clobbers;

  /* Number of read-only uses we avoid using not_read information.  */
  unsigned int static_readonly_clobbers_avoided;
} clobber_stats;
  

/* MEM is an artificial symbol representing memory references.  Any
   symbol for which for which is_gimple_reg returns false is
   considered "memory".  SSA renaming will build factored use-def and
   def-def chains over MEM, joining all versions that access
   conflicting regions of memory (be it due to aliasing, or
   overlapping references to the same aggregate object, or side-effect
   references to call-clobbered symbols at call-sites).  */
tree mem_var;

/* Flags to describe operand properties in helpers.  */

/* By default, operands are loaded.  */
#define opf_use		0

/* Operand is the target of an assignment expression or a 
   call-clobbered variable.  */
#define opf_def 	(1 << 0)

/* No virtual operands should be created in the expression.  This is used
   when traversing ADDR_EXPR nodes which have different semantics than
   other expressions.  Inside an ADDR_EXPR node, the only operands that we
   need to consider are indices into arrays.  For instance, &a.b[i] should
   generate a USE of 'i' but it should not generate a VUSE for 'a' nor a
   VUSE for 'b'.  */
#define opf_no_vops 	(1 << 2)

/* Operand is an implicit reference.  This is used to distinguish
   explicit assignments in the form of MODIFY_EXPRs from clobbering
   sites like function calls or ASM_EXPRs.  */
#define opf_implicit	(1 << 3)

/* Array for building all the def operands.  */
static VEC(tree,heap) *build_defs;

/* Array for building all the use operands.  */
static VEC(tree,heap) *build_uses;

/* Array for building all the VDEF operands.  */
static VEC(tree,heap) *build_vdefs;

/* Array for building all the VUSE operands.  */
static VEC(tree,heap) *build_vuses;

/* These arrays are the cached operand vectors for call clobbered calls.  */
static bool ops_active = false;

static GTY (()) struct ssa_operand_memory_d *operand_memory = NULL;
static unsigned operand_memory_index;

static void get_expr_operands (tree, tree *, int);

static def_optype_p free_defs = NULL;
static use_optype_p free_uses = NULL;
static vuse_optype_p free_vuses = NULL;
static vdef_optype_p free_vdefs = NULL;

/* Sets of symbols loaded and stored by a statement.  These sets are
   only used when the operand scanner is called via get_loads_and_stores.  */
static bitmap loaded_syms = NULL;
static bitmap stored_syms = NULL;

/* Statement change buffer.  Data structure used to record state
   information for statements.  This is used to determine what needs
   to be done in order to update the SSA web after a statement is
   modified by a pass.  If STMT is a statement that has just been
   created, or needs to be folded via fold_stmt, or anything that
   changes its physical structure then the pass should:

   1- Call push_stmt_changes (&stmt) to record the current state of
      STMT before any modifications are made.

   2- Make all appropriate modifications to the statement.

   3- Call pop_stmt_changes (&stmt) to find new symbols that
      need to be put in SSA form, SSA name mappings for names that
      have disappeared, recompute invariantness for address
      expressions, cleanup EH information, etc.  */

struct scb_d
{
  /* Pointer to the statement being modified.  */
  tree *stmt_p;

  /* If the statement references memory these are the sets of symbols
     loaded and stored by the statement.  */
  bitmap loads;
  bitmap stores;
};

typedef struct scb_d *scb_t;
DEF_VEC_P(scb_t);
DEF_VEC_ALLOC_P(scb_t,heap);

/* Stack of statement change buffers (SCB).  Every call to
   push_stmt_changes pushes a new buffer onto the stack.  Calls to
   pop_stmt_changes pop a buffer off of the stack and compute the set
   of changes for the popped statement.  */
static VEC(scb_t,heap) *scb_stack;


/* Return true if the parser should just gather symbols loaded/stored
   by the statement instead of scanning for operands.  */

static inline bool
gathering_loads_stores (void)
{
  return loaded_syms || stored_syms;
}


/* Return the DECL_UID of the base variable of T.  */

static inline unsigned
get_name_decl (tree t)
{
  if (TREE_CODE (t) != SSA_NAME)
    return DECL_UID (t);
  else
    return DECL_UID (SSA_NAME_VAR (t));
}


/* Comparison function for qsort used in operand_build_sort_virtual.  */

static int
operand_build_cmp (const void *p, const void *q)
{
  tree e1 = *((const tree *)p);
  tree e2 = *((const tree *)q);
  unsigned int u1,u2;

  u1 = get_name_decl (e1);
  u2 = get_name_decl (e2);

  /* We want to sort in ascending order.  They can never be equal.  */
#ifdef ENABLE_CHECKING
  gcc_assert (u1 != u2);
#endif
  return (u1 > u2 ? 1 : -1);
}


/* Sort the virtual operands in LIST from lowest DECL_UID to highest.  */

static inline void
operand_build_sort_virtual (VEC(tree,heap) *list)
{
  int num = VEC_length (tree, list);

  if (num < 2)
    return;

  if (num == 2)
    {
      if (get_name_decl (VEC_index (tree, list, 0)) 
	  > get_name_decl (VEC_index (tree, list, 1)))
	{  
	  /* Swap elements if in the wrong order.  */
	  tree tmp = VEC_index (tree, list, 0);
	  VEC_replace (tree, list, 0, VEC_index (tree, list, 1));
	  VEC_replace (tree, list, 1, tmp);
	}
      return;
    }

  /* There are 3 or more elements, call qsort.  */
  qsort (VEC_address (tree, list), 
	 VEC_length (tree, list), 
	 sizeof (tree),
	 operand_build_cmp);
}


/*  Return true if the SSA operands cache is active.  */

bool
ssa_operands_active (void)
{
  return ops_active;
}


/* Initialize the operand cache routines.  */

void
init_ssa_operands (void)
{
  build_defs = VEC_alloc (tree, heap, 5);
  build_uses = VEC_alloc (tree, heap, 10);
  build_vuses = VEC_alloc (tree, heap, 25);
  build_vdefs = VEC_alloc (tree, heap, 25);
  scb_stack = VEC_alloc (scb_t, heap, 20);

  gcc_assert (operand_memory == NULL);
  operand_memory_index = SSA_OPERAND_MEMORY_SIZE;
  ops_active = true;
}


/* Dispose of anything required by the operand routines.  */

void
fini_ssa_operands (void)
{
  struct ssa_operand_memory_d *ptr;
  VEC_free (tree, heap, build_defs);
  VEC_free (tree, heap, build_uses);
  VEC_free (tree, heap, build_vdefs);
  VEC_free (tree, heap, build_vuses);
  free_defs = NULL;
  free_uses = NULL;
  free_vuses = NULL;
  free_vdefs = NULL;

  /* The change buffer stack had better be empty.  */
  gcc_assert (VEC_length (scb_t, scb_stack) == 0);
  VEC_free (scb_t, heap, scb_stack);
  scb_stack = NULL;

  while ((ptr = operand_memory) != NULL)
    {
      operand_memory = operand_memory->next;
      ggc_free (ptr);
    }

  ops_active = false;
}


/* Reset the operand cache arrays.  */

static void
truncate_ssa_stmt_operands (void)
{
  VEC_truncate (tree, build_defs, 0);
  VEC_truncate (tree, build_uses, 0);
  VEC_truncate (tree, build_vdefs, 0);
  VEC_truncate (tree, build_vuses, 0);
}


/* Return memory for operands of SIZE chunks.  */
                                                                              
static inline void *
ssa_operand_alloc (unsigned size)
{
  char *ptr;

  gcc_assert (size <= SSA_OPERAND_MEMORY_SIZE);

  if (operand_memory_index + size >= SSA_OPERAND_MEMORY_SIZE)
    {
      struct ssa_operand_memory_d *ptr;
      ptr = GGC_NEW (struct ssa_operand_memory_d);
      ptr->next = operand_memory;
      operand_memory = ptr;
      operand_memory_index = 0;
    }
  ptr = &(operand_memory->mem[operand_memory_index]);
  operand_memory_index += size;
  return ptr;
}


static inline struct def_optype_d *
alloc_def (void)
{
  struct def_optype_d *ret;
  if (free_defs)
    {
      ret = free_defs;
      free_defs = free_defs->next;
    }
  else
    ret = (struct def_optype_d *)
		      ssa_operand_alloc (sizeof (struct def_optype_d));
  return ret;
}


static inline struct use_optype_d *
alloc_use (void)
{
  struct use_optype_d *ret;
  if (free_uses)
    {
      ret = free_uses;
      free_uses = free_uses->next;
    }
  else
    ret = (struct use_optype_d *)ssa_operand_alloc (sizeof (struct use_optype_d));
  return ret;
}




static inline struct vdef_optype_d *
alloc_vdef (int num)
{
  struct vdef_optype_d *ret;
  /* Eliminate free list for the moment.  */
#if 0
  if (free_vdefs)
    {
      ret = free_vdefs;
      free_vdefs = free_vdefs->next;
    }
  else
#endif
    ret = (struct vdef_optype_d *)ssa_operand_alloc (
	sizeof (struct vdef_optype_d) + (num - 1) * sizeof (vuse_element_t));
  VUSE_VECT_NUM_ELEM (ret->usev) = num;
  return ret;
}




static inline struct vuse_optype_d *
alloc_vuse (int num)
{
  struct vuse_optype_d *ret;
/* No free list for the moment.  */
#if 0    
  if (free_vuses)
    {
      ret = free_vuses;
      free_vuses = free_vuses->next;
    }
  else
#endif
    ret = (struct vuse_optype_d *)ssa_operand_alloc (
	sizeof (struct vuse_optype_d) + (num - 1) * sizeof (vuse_element_t));
  VUSE_VECT_NUM_ELEM (ret->usev) = num;
  return ret;
}


/* This routine makes sure that PTR is in an immediate use list, and makes
   sure the stmt pointer is set to the current stmt.  */

static inline void
set_virtual_use_link (use_operand_p ptr, tree stmt)
{
  /*  fold_stmt may have changed the stmt pointers.  */
  if (ptr->stmt != stmt)
    ptr->stmt = stmt;

  /* If this use isn't in a list, add it to the correct list.  */
  if (!ptr->prev)
    link_imm_use (ptr, *(ptr->use));
}

/* Appends ELT after TO, and moves the TO pointer to ELT.  */

#define APPEND_OP_AFTER(ELT, TO)	\
  do					\
    {					\
      (TO)->next = (ELT);		\
      (TO) = (ELT);			\
    } while (0)

/* Appends head of list FROM after TO, and move both pointers
   to their successors.  */

#define MOVE_HEAD_AFTER(FROM, TO)	\
  do					\
    {					\
      APPEND_OP_AFTER (FROM, TO);	\
      (FROM) = (FROM)->next;		\
    } while (0)

/* Moves OP to appropriate freelist.  OP is set to its successor.  */

#define MOVE_HEAD_TO_FREELIST(OP, TYPE)			\
  do							\
    {							\
      TYPE##_optype_p next = (OP)->next;		\
      (OP)->next = free_##TYPE##s;			\
      free_##TYPE##s = (OP);				\
      (OP) = next;					\
    } while (0)

/* Initializes immediate use at USE_PTR to value VAL, and links it to the list
   of immediate uses.  STMT is the current statement.  */

#define INITIALIZE_USE(USE_PTR, VAL, STMT)		\
  do							\
    {							\
      (USE_PTR)->use = (VAL);				\
      link_imm_use_stmt ((USE_PTR), *(VAL), (STMT));	\
    } while (0)

/* Adds OP to the list of defs after LAST, and moves
   LAST to the new element.  */

static inline def_optype_p 
add_def_op (tree *op, def_optype_p *last)
{
  def_optype_p new;

  new = alloc_def ();
  DEF_OP_PTR (new) = op;
  APPEND_OP_AFTER (new, *last);  
  return new;
}

/* Adds OP to the list of uses of statement STMT after LAST, and moves
   LAST to the new element.  */

static inline use_optype_p
add_use_op (tree stmt, tree *op, use_optype_p *last)
{
  use_optype_p new;

  new = alloc_use ();
  INITIALIZE_USE (USE_OP_PTR (new), op, stmt);
  APPEND_OP_AFTER (new, *last);  
  return new;
}

/* Adds OP to the list of vuses of statement STMT after LAST, and moves
   LAST to the new element.  */

static inline vuse_optype_p
add_vuse_op (tree stmt, tree op, int num, vuse_optype_p *last)
{
  vuse_optype_p new;
  int x;

  new = alloc_vuse (num);
  for (x = 0; x < num; x++)
    {
      VUSE_OP (new, x) = op;
      INITIALIZE_USE (VUSE_OP_PTR (new, x), &(VUSE_OP (new, x)), stmt);
    }
  APPEND_OP_AFTER (new, *last);  
  return new;
}

/* Adds OP to the list of vdefs of statement STMT after LAST, and moves
   LAST to the new element.  */

static inline vdef_optype_p
add_vdef_op (tree stmt, tree op, int num, vdef_optype_p *last)
{
  int x;
  vdef_optype_p new;

  new = alloc_vdef (num);
  VDEF_RESULT (new) = op;
  for (x = 0; x < num; x++)
    {
      VDEF_OP (new, x) = op;
      INITIALIZE_USE (VDEF_OP_PTR (new, x), &(VDEF_OP (new, x)), stmt);
    }
  APPEND_OP_AFTER (new, *last);  
  return new;
}


struct vdef_optype_d *
realloc_vdef (struct vdef_optype_d *ptr, int num_elem)
{
  int x, lim;
  tree val, stmt;
  struct vdef_optype_d *ret, *tmp;

  if (VUSE_VECT_NUM_ELEM (ptr->usev) == num_elem)
    return ptr; 
  
  val = VDEF_RESULT (ptr);
  if (TREE_CODE (val) == SSA_NAME)
    val = SSA_NAME_VAR (val);

  stmt = USE_STMT (VDEF_OP_PTR (ptr, 0));

  /* Delink all the existing uses.  */

  for (x = 0; x < VUSE_VECT_NUM_ELEM (ptr->usev); x++)
    {
      use_operand_p use_p = VDEF_OP_PTR (ptr, x);
      delink_imm_use (use_p);
    }

  /* If we want less space, simply use this one, and shrink the size.  */
  if (VUSE_VECT_NUM_ELEM (ptr->usev) > num_elem)
    {
      VUSE_VECT_NUM_ELEM (ptr->usev) = num_elem;
      return ptr;
    }

  /* its growing. Allocate a new one and replace the old one.  */
  tmp = ptr;;
  ret = add_vdef_op (stmt, val, num_elem, &ptr);
  ret->next = NULL;
  ptr = tmp;

  lim = VUSE_VECT_NUM_ELEM (ptr->usev);
  memset (ptr, 0, sizeof (struct vdef_optype_d) + sizeof (vuse_element_t) * (lim- 1));
  /* Now simply remove the old one.  */
  if (VDEF_OPS (stmt) == ptr)
    {
      VDEF_OPS (stmt) = ret;
      return ret;
    }
  else
    for (tmp = VDEF_OPS (stmt); 
	 tmp != NULL && tmp->next != ptr; 
	 tmp = tmp->next)
      {
	tmp->next = ret;
	return ret;
      }
  /* The pointer passed in isnt in the stmt's vdef lists.  */
  gcc_unreachable ();

}



struct vuse_optype_d *
realloc_vuse (struct vuse_optype_d *ptr, int num_elem)
{
  int x, lim;
  tree val, stmt;
  struct vuse_optype_d *ret, *tmp;

  if (VUSE_VECT_NUM_ELEM (ptr->usev) == num_elem)
    return ptr; 
  
  val = VUSE_OP (ptr, 0);
  if (TREE_CODE (val) == SSA_NAME)
    val = SSA_NAME_VAR (val);

  stmt = USE_STMT (VUSE_OP_PTR (ptr, 0));

  /* Delink all the existing uses.  */

  for (x = 0; x < VUSE_VECT_NUM_ELEM (ptr->usev); x++)
    {
      use_operand_p use_p = VUSE_OP_PTR (ptr, x);
      delink_imm_use (use_p);
    }

  /* If we want less space, simply use this one, and shrink the size.  */
  if (VUSE_VECT_NUM_ELEM (ptr->usev) > num_elem)
    {
      VUSE_VECT_NUM_ELEM (ptr->usev) = num_elem;
      return ptr;
    }

  /* its growing. Allocate a new one and reaplce the old one.  */
  tmp = ptr;
  ret = add_vuse_op (stmt, val, num_elem, &ptr);
  ret->next = NULL;
  ptr = tmp;

  lim = VUSE_VECT_NUM_ELEM (ptr->usev);
  memset (ptr, 0, 
	  sizeof (struct vuse_optype_d) + sizeof (vuse_element_t) * (lim - 1));
  /* No wsimply link it in, find the node which points to this one.  */
  if (VUSE_OPS (stmt) == ptr)
    {
      VUSE_OPS (stmt) = ret;
      return ret;
    }
  else
    for (tmp = VUSE_OPS (stmt); 
	 tmp != NULL && tmp->next != ptr; 
	 tmp = tmp->next)
      {
	tmp->next = ret;
	return ret;
      }
  /* The pointer passed in isnt in the stmt's vdef lists.  */
  gcc_unreachable ();

}



/* Takes elements from build_defs and turns them into def operands of STMT.
   TODO -- Given that def operands list is not necessarily sorted, merging
	   the operands this way does not make much sense.
	-- Make build_defs VEC of tree *.  */

static inline void
finalize_ssa_def_ops (tree stmt)
{
  unsigned new_i;
  struct def_optype_d new_list;
  def_optype_p old_ops, last;
  tree *old_base;

  new_list.next = NULL;
  last = &new_list;

  old_ops = DEF_OPS (stmt);

  new_i = 0;
  while (old_ops && new_i < VEC_length (tree, build_defs))
    {
      tree *new_base = (tree *) VEC_index (tree, build_defs, new_i);
      old_base = DEF_OP_PTR (old_ops);

      if (old_base == new_base)
        {
	  /* if variables are the same, reuse this node.  */
	  MOVE_HEAD_AFTER (old_ops, last);
	  new_i++;
	}
      else if (old_base < new_base)
	{
	  /* if old is less than new, old goes to the free list.  */
	  MOVE_HEAD_TO_FREELIST (old_ops, def);
	}
      else
	{
	  /* This is a new operand.  */
	  add_def_op (new_base, &last);
	  new_i++;
	}
    }

  /* If there is anything remaining in the build_defs list, simply emit it.  */
  for ( ; new_i < VEC_length (tree, build_defs); new_i++)
    add_def_op ((tree *) VEC_index (tree, build_defs, new_i), &last);

  last->next = NULL;

  /* If there is anything in the old list, free it.  */
  if (old_ops)
    {
      old_ops->next = free_defs;
      free_defs = old_ops;
    }

  /* Now set the stmt's operands.  */
  DEF_OPS (stmt) = new_list.next;

#ifdef ENABLE_CHECKING
  {
    def_optype_p ptr;
    unsigned x = 0;
    for (ptr = DEF_OPS (stmt); ptr; ptr = ptr->next)
      x++;

    gcc_assert (x == VEC_length (tree, build_defs));
  }
#endif
}

/* This routine will create stmt operands for STMT from the def build list.  */

static void
finalize_ssa_defs (tree stmt)
{
  unsigned int num = VEC_length (tree, build_defs);

  /* There should only be a single real definition per assignment.  */
  gcc_assert ((stmt && TREE_CODE (stmt) != MODIFY_EXPR) || num <= 1);

  /* If there is an old list, often the new list is identical, or close, so
     find the elements at the beginning that are the same as the vector.  */
  finalize_ssa_def_ops (stmt);
  VEC_truncate (tree, build_defs, 0);
}

/* Takes elements from build_uses and turns them into use operands of STMT.
   TODO -- Make build_uses VEC of tree *.  */

static inline void
finalize_ssa_use_ops (tree stmt)
{
  unsigned new_i;
  struct use_optype_d new_list;
  use_optype_p old_ops, ptr, last;

  new_list.next = NULL;
  last = &new_list;

  old_ops = USE_OPS (stmt);

  /* If there is anything in the old list, free it.  */
  if (old_ops)
    {
      for (ptr = old_ops; ptr; ptr = ptr->next)
	delink_imm_use (USE_OP_PTR (ptr));
      old_ops->next = free_uses;
      free_uses = old_ops;
    }

  /* Now create nodes for all the new nodes.  */
  for (new_i = 0; new_i < VEC_length (tree, build_uses); new_i++)
    add_use_op (stmt, (tree *) VEC_index (tree, build_uses, new_i), &last);

  last->next = NULL;

  /* Now set the stmt's operands.  */
  USE_OPS (stmt) = new_list.next;

#ifdef ENABLE_CHECKING
  {
    unsigned x = 0;
    for (ptr = USE_OPS (stmt); ptr; ptr = ptr->next)
      x++;

    gcc_assert (x == VEC_length (tree, build_uses));
  }
#endif
}

/* Return a new use operand vector for STMT, comparing to OLD_OPS_P.  */
                                                                              
static void
finalize_ssa_uses (tree stmt)
{
#ifdef ENABLE_CHECKING
  {
    unsigned x;
    unsigned num = VEC_length (tree, build_uses);

    /* If the pointer to the operand is the statement itself, something is
       wrong.  It means that we are pointing to a local variable (the 
       initial call to update_stmt_operands does not pass a pointer to a 
       statement).  */
    for (x = 0; x < num; x++)
      gcc_assert (*((tree *)VEC_index (tree, build_uses, x)) != stmt);
  }
#endif
  finalize_ssa_use_ops (stmt);
  VEC_truncate (tree, build_uses, 0);
}


/* Takes elements from build_vdefs and turns them into vdef operands of
   STMT.  */

static inline void
finalize_ssa_vdef_ops (tree stmt)
{
  vdef_optype_p old_ops;

  old_ops = VDEF_OPS (stmt);

  if (old_ops)
    {
      int x;

      if (VEC_length (tree, build_vdefs) == 0)
	{
	  /* The statement used to have a VDEF but it doesn't
	     anymore.  Unlink the operands in the RHS.  */
	  gcc_assert (old_ops->next == NULL);
	  for (x = 0; x < VUSE_VECT_NUM_ELEM (old_ops->usev); x++)
	    delink_imm_use (VDEF_OP_PTR (old_ops, x));

	  /* The SSA name on the LHS is not needed anymore, release it
	     so the SSA updater can consider it a stale name and
	     ignore it when rewriting VOPS.  */
	  release_ssa_name_after_update_ssa (VDEF_RESULT (old_ops));

	  old_ops->next = free_vdefs;
	  free_vdefs = old_ops;
	  VDEF_OPS (stmt) = NULL;
	}
      else
	{
	  /* Make sure all the immediate use links are still correct.  */
	  for (x = 0; x < VUSE_VECT_NUM_ELEM (old_ops->usev); x++)
	    set_virtual_use_link (VDEF_OP_PTR (old_ops, x), stmt);
	}
    }
  else if (old_ops == NULL && VEC_length (tree, build_vdefs) > 0)
    {
      /* This is the first time the statement gets a VDEF,
	 instantiate it.  */
      tree op;
      struct vdef_optype_d new_list;
      vdef_optype_p last;

      gcc_assert (VEC_length (tree, build_vdefs) == 1);

      new_list.next = NULL;
      last = &new_list;
      op = VEC_index (tree, build_vdefs, 0);
      add_vdef_op (stmt, op, 1, &last);
      last->next = NULL;

      /* Now set the stmt's operands.  */
      VDEF_OPS (stmt) = new_list.next;
    }
}


static void
finalize_ssa_vdefs (tree stmt)
{
  finalize_ssa_vdef_ops (stmt);
  VEC_truncate (tree, build_vdefs, 0);
}


/* Takes elements from build_vuses and turns them into vuse operands of
   STMT.  */

static inline void
finalize_ssa_vuse_ops (tree stmt)
{
  vuse_optype_p old_ops;

  old_ops = VUSE_OPS (stmt);

  if (old_ops)
    {
      int x;

      if (VEC_length (tree, build_vuses) == 0)
	{
	  /* The statement used to have a VDEF but it doesn't
	     anymore.  Unlink the operands in the RHS.  */
	  gcc_assert (old_ops->next == NULL);
	  for (x = 0; x < VUSE_VECT_NUM_ELEM (old_ops->usev); x++)
	    delink_imm_use (VUSE_OP_PTR (old_ops, x));

	  old_ops->next = free_vuses;
	  free_vuses = old_ops;
	  VUSE_OPS (stmt) = NULL;
	}
      else
	{
	  /* Make sure all the immediate use links are still correct.  */
	  for (x = 0; x < VUSE_VECT_NUM_ELEM (old_ops->usev); x++)
	    set_virtual_use_link (VUSE_OP_PTR (old_ops, x), stmt);
	}
    }
  else if (VEC_length (tree, build_vuses) > 0)
    {
      /* This is the first time the statement gets a VUSE,
	 instantiate it.  */
      tree op;
      struct vuse_optype_d new_list;
      vuse_optype_p last;

      gcc_assert (VEC_length (tree, build_vuses) == 1);

      new_list.next = NULL;
      last = &new_list;
      op = VEC_index (tree, build_vuses, 0);
      add_vuse_op (stmt, op, 1, &last);
      last->next = NULL;

      /* Now set STMT's operands.  */
      VUSE_OPS (stmt) = new_list.next;
    }
}
                                                                              

/* Return a new VUSE operand vector for STMT.  */
                                                                              
static void
finalize_ssa_vuses (tree stmt)
{
  finalize_ssa_vuse_ops (stmt);
  VEC_truncate (tree, build_vuses, 0);
}

/* Finalize all the build vectors, fill the new ones into INFO.  */
                                                                              
static inline void
finalize_ssa_stmt_operands (tree stmt)
{
  finalize_ssa_defs (stmt);
  finalize_ssa_uses (stmt);
  finalize_ssa_vdefs (stmt);
  finalize_ssa_vuses (stmt);
}


/* Start the process of building up operands vectors in INFO.  */

static inline void
start_ssa_stmt_operands (void)
{
  gcc_assert (VEC_length (tree, build_defs) == 0);
  gcc_assert (VEC_length (tree, build_uses) == 0);
  gcc_assert (VEC_length (tree, build_vuses) == 0);
  gcc_assert (VEC_length (tree, build_vdefs) == 0);
}


/* Add DEF_P to the list of pointers to operands.  */

static inline void
append_def (tree *def_p)
{
  VEC_safe_push (tree, heap, build_defs, (tree)def_p);
}


/* Add USE_P to the list of pointers to operands.  */

static inline void
append_use (tree *use_p)
{
  VEC_safe_push (tree, heap, build_uses, (tree)use_p);
}


/* Add a new VDEF operator for .MEM to the build array.  */

static inline void
append_vdef (void)
{
  /* We only ever need one VDEF operator.  FIXME.  build_vdefs
     should not be a vector anymore.  */
  if (VEC_length (tree, build_vdefs) == 1)
    return;

  VEC_safe_push (tree, heap, build_vdefs, mem_var);
}


/* Add a new VUSE operator for .MEM to the build array.  */

static inline void
append_vuse (void)
{
  /* We only need one VUSE operator.  FIXME, build_vuses should not be
     a vector anymore.  */
  if (VEC_length (tree, build_vuses) == 1)
    return;

  VEC_safe_push (tree, heap, build_vuses, mem_var);
}


/* Add a virtual operator for .MEM according to value of FLAGS.  */

static inline void
add_virtual_operator (int flags)
{
  /* If we are inside an ADDR_EXPR, then VAR is not being referenced
     but used to compute address arithmetic.  */
  if (flags & opf_no_vops)
    return;

  if (flags & opf_def)
    append_vdef ();
  else
    append_vuse ();
}


/* REF is a tree that contains the entire pointer dereference
   expression, if available, or NULL otherwise.  ALIAS is the variable
   we are asking if REF can access.  OFFSET and SIZE come from the
   memory access expression that generated this virtual operand.
   FOR_CLOBBER is true is this is adding a virtual operand for a call
   clobber.  */

static bool
access_can_touch_variable (tree ref, tree alias, HOST_WIDE_INT offset,
			   HOST_WIDE_INT size)
{  
  bool offsetgtz = offset > 0;
  unsigned HOST_WIDE_INT uoffset = (unsigned HOST_WIDE_INT) offset;
  tree base = ref ? get_base_address (ref) : NULL;

  /* If ALIAS is an SFT, it can't be touched if the offset     
     and size of the access is not overlapping with the SFT offset and
     size.  This is only true if we are accessing through a pointer
     to a type that is the same as SFT_PARENT_VAR.  Otherwise, we may
     be accessing through a pointer to some substruct of the
     structure, and if we try to prune there, we will have the wrong
     offset, and get the wrong answer.
     i.e., we can't prune without more work if we have something like

     struct gcc_target
     {
       struct asm_out
       {
         const char *byte_op;
	 struct asm_int_op
	 {    
	   const char *hi;
	 } aligned_op;
       } asm_out;
     } targetm;
     
     foo = &targetm.asm_out.aligned_op;
     return foo->hi;

     SFT.1, which represents hi, will have SFT_OFFSET=32 because in
     terms of SFT_PARENT_VAR, that is where it is.
     However, the access through the foo pointer will be at offset 0.  */
  if (size != -1
      && TREE_CODE (alias) == STRUCT_FIELD_TAG
      && base
      && TREE_TYPE (base) == TREE_TYPE (SFT_PARENT_VAR (alias))
      && !overlap_subvar (offset, size, alias, NULL))
    {
#ifdef ACCESS_DEBUGGING
      fprintf (stderr, "Access to ");
      print_generic_expr (stderr, ref, 0);
      fprintf (stderr, " may not touch ");
      print_generic_expr (stderr, alias, 0);
      fprintf (stderr, " in function %s\n", get_name (current_function_decl));
#endif
      return false;
    }

  /* Without strict aliasing, it is impossible for a component access
     through a pointer to touch a random variable, unless that
     variable *is* a structure or a pointer.

     That is, given p->c, and some random global variable b,
     there is no legal way that p->c could be an access to b.
     
     Without strict aliasing on, we consider it legal to do something
     like:

     struct foos { int l; };
     int foo;
     static struct foos *getfoo(void);
     int main (void)
     {
       struct foos *f = getfoo();
       f->l = 1;
       foo = 2;
       if (f->l == 1)
         abort();
       exit(0);
     }
     static struct foos *getfoo(void)     
     { return (struct foos *)&foo; }
     
     (taken from 20000623-1.c)

     The docs also say/imply that access through union pointers
     is legal (but *not* if you take the address of the union member,
     i.e. the inverse), such that you can do

     typedef union {
       int d;
     } U;

     int rv;
     void breakme()
     {
       U *rv0;
       U *pretmp = (U*)&rv;
       rv0 = pretmp;
       rv0->d = 42;    
     }
     To implement this, we just punt on accesses through union
     pointers entirely.
  */
  else if (ref 
	   && flag_strict_aliasing
	   && TREE_CODE (ref) != INDIRECT_REF
	   && !MTAG_P (alias)
	   && (TREE_CODE (base) != INDIRECT_REF
	       || TREE_CODE (TREE_TYPE (base)) != UNION_TYPE)
	   && !AGGREGATE_TYPE_P (TREE_TYPE (alias))
	   && TREE_CODE (TREE_TYPE (alias)) != COMPLEX_TYPE
	   && !POINTER_TYPE_P (TREE_TYPE (alias)))
    {
#ifdef ACCESS_DEBUGGING
      fprintf (stderr, "Access to ");
      print_generic_expr (stderr, ref, 0);
      fprintf (stderr, " may not touch ");
      print_generic_expr (stderr, alias, 0);
      fprintf (stderr, " in function %s\n", get_name (current_function_decl));
#endif
      return false;
    }

  /* If the offset of the access is greater than the size of one of
     the possible aliases, it can't be touching that alias, because it
     would be past the end of the structure.  */
  else if (ref
	   && flag_strict_aliasing
	   && TREE_CODE (ref) != INDIRECT_REF
	   && !MTAG_P (alias)
	   && !POINTER_TYPE_P (TREE_TYPE (alias))
	   && offsetgtz
	   && DECL_SIZE (alias)
	   && TREE_CODE (DECL_SIZE (alias)) == INTEGER_CST
	   && uoffset > TREE_INT_CST_LOW (DECL_SIZE (alias)))
    {
#ifdef ACCESS_DEBUGGING
      fprintf (stderr, "Access to ");
      print_generic_expr (stderr, ref, 0);
      fprintf (stderr, " may not touch ");
      print_generic_expr (stderr, alias, 0);
      fprintf (stderr, " in function %s\n", get_name (current_function_decl));
#endif
      return false;
    }	   

  return true;
}


/* Add VAR to STORED_SYMS or LOADED_SYMS depending on the value of
   FLAGS.  */

static inline void
add_mem_symbol (tree decl, int flags)
{
  unsigned uid;

  /* GIMPLE registers should not be added as memory symbols.  */
  gcc_assert (!is_gimple_reg (decl));

  /* If the variable cannot be modified and this is an implicit store
     operation, change it into a load.  This happens when read-only
     variables are marked call-clobbered and/or aliased to writable
     variables.  So we only check that this only happens on
     non-specific stores.

     Note that if this is an explicit store (i.e. associated with a
     MODIFY_EXPR), then we can't suppress it, lest we run into
     validation problems.  This can happen when programs cast away
     const, leaving us with a store to read-only memory.  If the
     statement is actually executed at runtime, then the program is
     ill formed.  If the statement is not executed then all is well.
     At the very least, we cannot ICE.  */
  if ((flags & opf_implicit) && unmodifiable_var_p (decl))
    flags &= ~opf_def;

  uid = DECL_UID (decl);
  if (flags & opf_def)
    bitmap_set_bit (stored_syms, uid);
  else
    bitmap_set_bit (loaded_syms, uid);
}


/* Add memory symbols associated with variable DECL to LOADED_SYMS or
   STORED_SYMS according to FLAGS.  If DECL has sub-variables, then
   its sub-variables are added.  Otherwise, DECL is added.  FLAGS is
   as in get_expr_operands.  */

static void
add_mem_symbols_in_decl (tree decl, int flags)
{
  subvar_t svars;

  /* GIMPLE registers should not be added as memory symbols.  */
  gcc_assert (!is_gimple_reg (decl));

  /* If DECL has sub-variables, add them instead of DECL.  */
  if (var_can_have_subvars (decl) && (svars = get_subvars_for_var (decl)))
    {
      subvar_t sv;
      for (sv = svars; sv; sv = sv->next)
	add_mem_symbol (sv->var, flags);
    }
  else
    add_mem_symbol (decl, flags);
}


/* Get memory symbols associated with memory tag TAG.  FLAGS, FULL_REF,
   OFFSET and SIZE are as in get_mem_symbols_in_indirect_ref.  */

static void
get_mem_symbols_in_tag (tree tag, int flags, tree full_ref,
			HOST_WIDE_INT offset, HOST_WIDE_INT size)
{
  VEC(tree,gc) *aliases;
  tree sym, alias;
  unsigned i;
  var_ann_t v_ann;
  bool added_symbols_p;
  
  sym = (TREE_CODE (tag) == SSA_NAME ? SSA_NAME_VAR (tag) : tag);
  v_ann = var_ann (sym);
  
  aliases = v_ann->may_aliases;

  added_symbols_p = false;
  if (aliases)
    {
      /* If the tag has aliases, add all the aliases that may be
	 affected by FULL_REF.  */
      for (i = 0; VEC_iterate (tree, aliases, i, alias); i++)
	if (access_can_touch_variable (full_ref, alias, offset, size))
	  {
	    add_mem_symbol (alias, flags);
	    added_symbols_p = true;
	  }
    }

  /* If no symbols were added, we need to add the tag itself to avoid
     missing dependencies in cases where no symbols are involved in
     aliasing (e.g., when two pointers point to the same
     heap-allocated structure).  */
  if (!added_symbols_p)
    add_mem_symbol (sym, flags);
}


/* Get memory symbols referenced by the *INDIRECT_REF node EXPR.

   FLAGS is as in get_expr_operands.

   FULL_REF contains the full pointer dereference expression, if
      available.  NULL otherwise.

   OFFSET and SIZE are the location of the access inside the
      dereferenced pointer, if known.  Otherwise, OFFSET should be 0
      and SIZE should be -1.  */

static void
get_mem_symbols_in_indirect_ref (tree expr, int flags, tree full_ref,
				 HOST_WIDE_INT offset, HOST_WIDE_INT size)
{
  struct ptr_info_def *pi = NULL;
  tree *pptr = &TREE_OPERAND (expr, 0);
  tree ptr = *pptr;

  /* If we don't have aliasing information, do nothing.  */
  if (!aliases_computed_p)
    return;

  /* No symbols referenced if PTR is not a variable.  */
  if (!SSA_VAR_P (ptr))
    return;

  if (TREE_CODE (ptr) == SSA_NAME
      && (pi = SSA_NAME_PTR_INFO (ptr)) != NULL
      && pi->name_mem_tag)
    {
      /* If PTR has flow-sensitive points-to information, use it.  */
      get_mem_symbols_in_tag (pi->name_mem_tag, flags, full_ref, offset, size);
    }
  else
    {
      var_ann_t ann;

      /* If PTR is not an SSA_NAME or it doesn't have a name
	 tag, use its symbol memory tag.  */
      if (TREE_CODE (ptr) == SSA_NAME)
	ptr = SSA_NAME_VAR (ptr);

      /* If alias information has not been computed for PTR, it may
	 not yet have a memory tag (e.g., when folding builtins new
	 pointers get instantiated which force an alias pass).  */
      ann = var_ann (ptr);
      if (ann->symbol_mem_tag)
	get_mem_symbols_in_tag (ann->symbol_mem_tag, flags, full_ref, offset,
	                        size);
    }
}


/* Add symbols referenced by an aggregate type.  */

static void
get_mem_symbols_in_aggregate (tree expr, int flags)
{
  tree ref;
  HOST_WIDE_INT offset, size, maxsize;

  ref = get_ref_base_and_extent (expr, &offset, &size, &maxsize);
  if (SSA_VAR_P (ref))
    {
      bool added_symbols_p = false;

      if (get_subvars_for_var (ref))
	{
	  subvar_t svars, sv;

	  svars = get_subvars_for_var (ref);
	  for (sv = svars; sv; sv = sv->next)
	    if (overlap_subvar (offset, maxsize, sv->var, NULL))
	      {
		added_symbols_p = true;
		add_mem_symbol (sv->var, flags);
	      }
	}

      /* If no symbols were added, we need to add REF itself to avoid
	 missing dependencies when EXPR does not reference any of the
	 sub-variables we computed for REF.  */
      if (!added_symbols_p)
	add_mem_symbol (ref, flags);
    }
  else if (TREE_CODE (ref) == INDIRECT_REF)
    get_mem_symbols_in_indirect_ref (ref, flags, expr, offset, maxsize);
}


/* Get memory symbols accessed by TARGET_MEM_REF EXPR in statement
   STMT.  FLAGS is as in get_expr_operands.  */

static void
get_mem_symbols_in_tmr (tree expr, int flags)
{
  tree tag;

  tag = TMR_TAG (expr);
  if (!tag)
    return;

  if (MTAG_P (tag))
    get_mem_symbols_in_tag (tag, flags, NULL_TREE, 0, -1);
  else if (DECL_P (tag))
    add_mem_symbol (tag, flags);
  else
    get_mem_symbols_in_aggregate (tag, flags);
}


/* Helper for get_call_expr_operands.  When gathering memory symbols,
   add all the call-clobbered symbols that may be modified by a call
   to function CALLEE to the set STORED_SYMS.  CALL_FLAGS are the
   flags for the CALL_EXPR site.
   
   If we are scanning for regular operands, add a VDEF operator if any
   call-clobbered symbol may be modified by CALLEE and add a VUSE
   operator if any call-clobbered symbol may be read by CALLEE.  */

static void
add_call_clobbered_mem_symbols (tree callee, int call_flags)
{
  unsigned u;
  bitmap_iterator bi;
  bitmap not_read_b, not_written_b;
  bool added_symbols_p, add_vdef_p, add_vuse_p;
  
  /* Get info for local and module level statics.  There is a bit
     set for each static if the call being processed does not read
     or write that variable.  */
  not_read_b = callee ? ipa_reference_get_not_read_global (callee) : NULL; 
  not_written_b = callee ? ipa_reference_get_not_written_global (callee) : NULL; 
  /* Check which call-clobbered variables may be modified by a call to
     CALLEE.  Add those affected to STORED_SYMS.  */
  added_symbols_p = false;
  add_vdef_p = false;
  add_vuse_p = false;
  EXECUTE_IF_SET_IN_BITMAP (call_clobbered_vars, 0, u, bi)
    {
      tree real_var, var;
      unsigned int escape_mask;
      bool not_read, not_written;

      /* If we have already decided to add both a VDEF and a VUSE,
	 there is no point in examining any more call-clobbered
	 symbols.  */
      if (add_vdef_p && add_vuse_p)
	break;

      var = referenced_var_lookup (u);
      escape_mask = var_ann (var)->escape_mask;

      /* Not read and not written are computed on regular vars, not
	 subvars, so look at the parent var if this is an SFT. */
      real_var = var;
      if (TREE_CODE (var) == STRUCT_FIELD_TAG)
	real_var = SFT_PARENT_VAR (var);

      gcc_assert (!unmodifiable_var_p (var));

      clobber_stats.clobbered_vars++;

      /* See if this variable is really clobbered by a call to CALLEE.  */

      /* Trivial case: Things escaping only to pure/const are not
	 clobbered by non-pure-const, and only read by pure/const. */
      if (escape_mask == ESCAPE_TO_PURE_CONST)
	{
	  if (call_flags & (ECF_CONST | ECF_PURE))
	    {
	      if (gathering_loads_stores ())
		{
		  added_symbols_p = true;
		  add_mem_symbols_in_decl (var, opf_use);
		}
	      else
		add_vuse_p = true;

	      clobber_stats.unescapable_clobbers_avoided++;
	      continue;
	    }
	  else
	    {
	      clobber_stats.unescapable_clobbers_avoided++;
	      continue;
	    }
	}

      /* Analysis on static globals may have determined that the
	 variable is never written to nor read from by any function.
	 Use that, if possible.  */
      not_read = not_read_b
	         ? bitmap_bit_p (not_read_b, DECL_UID (real_var))
	         : false;

      not_written = not_written_b
	            ? bitmap_bit_p (not_written_b, DECL_UID (real_var))
		    : false;

      if (not_written)
	{
	  clobber_stats.static_write_clobbers_avoided++;
	  if (!not_read)
	    {
	      if (gathering_loads_stores ())
		{
		  added_symbols_p = true;
		  add_mem_symbols_in_decl (var, opf_use);
		}
	      else
		add_vuse_p = true;
	    }
	  else
	    clobber_stats.static_read_clobbers_avoided++;
	}
      else
	{
	  if (gathering_loads_stores ())
	    {
	      added_symbols_p = true;
	      add_mem_symbols_in_decl (var, opf_def | opf_implicit);
	    }
	  else
	    add_vdef_p = true;
	}
    }

  if (add_vdef_p)
    add_virtual_operator (opf_def);

  if (add_vuse_p)
    add_virtual_operator (opf_use);
}


/* Helper for get_call_expr_operands.  When gathering memory symbols,
   add all the call-clobbered symbols that may be read by a call to
   function CALLEE to the set LOADED_SYMS.

   If we are scanning for regular operands, add a VUSE operator if any
   call-clobbered symbol may be read by CALLEE.  */

static void
add_call_read_mem_symbols (tree callee)
{
  unsigned u;
  bitmap_iterator bi;
  bitmap not_read_b;
  bool added_symbols_p;

  /* Get info for local and module level statics.  There is a bit
     set for each static if the call being processed does not read
     or write that variable.  */
  not_read_b = callee ? ipa_reference_get_not_read_global (callee) : NULL; 

  /* Check which call-clobbered variables may be read by a call to
     CALLEE.  Add those to LOADED_SYMS.  */
  added_symbols_p = false;
  EXECUTE_IF_SET_IN_BITMAP (call_clobbered_vars, 0, u, bi)
    {
      tree real_var, var;
      bool not_read;
      var_ann_t ann;

      clobber_stats.readonly_clobbers++;

      var = referenced_var (u);
      ann = var_ann (var);

      /* Not read and not written are computed on regular vars, not
	 subvars, so look at the parent var if this is an SFT. */
      real_var = var;
      if (TREE_CODE (var) == STRUCT_FIELD_TAG)
	real_var = SFT_PARENT_VAR (var);

      not_read = not_read_b
	         ? bitmap_bit_p (not_read_b, DECL_UID (real_var))
	         : false;

      if (not_read)
	{
	  clobber_stats.static_readonly_clobbers_avoided++;
	  continue;
	}

      if (gathering_loads_stores ())
	{
	  added_symbols_p = true;
	  add_mem_symbols_in_decl (var, opf_use);
	}
      else
	{
	  /* If we have already decided to add a VUSE, there is no
	     point in examining any more call-clobbered symbols.  */
	  add_virtual_operator (opf_use);
	  return;
	}
    }

  /* Similarly to the logic in add_call_clobbered_mem_symbols, add a
     load of .MEM if no symbols have been added.  */
  if (gathering_loads_stores () && !added_symbols_p)
    add_mem_symbol (mem_var, opf_use);
}


/* Add *VAR_P to the appropriate operand array for S_ANN.  FLAGS is as in
   get_expr_operands.  If *VAR_P is a GIMPLE register, it will be added to
   the statement's real operands, otherwise it is added to virtual
   operands.  */

static void
add_stmt_operand (tree *var_p, stmt_ann_t s_ann, int flags)
{
  tree var, sym;
  var_ann_t v_ann;

  gcc_assert (SSA_VAR_P (*var_p) && s_ann);

  var = *var_p;
  sym = (TREE_CODE (var) == SSA_NAME ? SSA_NAME_VAR (var) : var);
  v_ann = var_ann (sym);

  /* Mark statements with volatile operands.  */
  if (TREE_THIS_VOLATILE (sym))
    s_ann->has_volatile_ops = true;

  if (is_gimple_reg (sym))
    {
      /* The variable is a GIMPLE register.  Add it to real operands.  */
      if (flags & opf_def)
	append_def (var_p);
      else
	append_use (var_p);
    }
  else
    {
      /* .MEM should not be found in the code.  */
      gcc_assert (var != mem_var);

      /* When gathering loads and stores, look for the symbols
	 associated with VAR, otherwise add the virtual operator
	 indicated by FLAGS.  */
      if (gathering_loads_stores ())
	add_mem_symbols_in_decl (var, flags);
      else
	add_virtual_operator (flags);
    }
}


/* A subroutine of get_expr_operands to handle INDIRECT_REF,
   ALIGN_INDIRECT_REF and MISALIGNED_INDIRECT_REF.  

   STMT is the statement being processed, EXPR is the INDIRECT_REF
      that got us here.
   
   FLAGS is as in get_expr_operands.

   FULL_REF contains the full pointer dereference expression, if we
      have it, or NULL otherwise.

   OFFSET and SIZE are the location of the access inside the
      dereferenced pointer, if known.  */

static void
get_indirect_ref_operands (tree stmt, tree expr, int flags)
{
  tree *pptr = &TREE_OPERAND (expr, 0);
  tree ptr = *pptr;
  stmt_ann_t s_ann = stmt_ann (stmt);

  /* If the dereference is volatile, mark the statement accordingly.  */
  if (TREE_THIS_VOLATILE (expr))
    s_ann->has_volatile_ops = true;

  /* When gathering memory symbols, there is no need to keep analyzing
     the rest of the expression.  */
  if (gathering_loads_stores ())
    {
      get_mem_symbols_in_indirect_ref (expr, flags, NULL_TREE, 0, -1);
      return;
    }

  if (SSA_VAR_P (ptr))
    {
      /* Only add a reference to .MEM if we have alias information
	 available.  Otherwise, the SSA renamer will try to access the
	 aliased symbols associated with this pointer dereference and
	 find nothing.  */
      if (aliases_computed_p)
	{
	  tree tag;

	  if (TREE_CODE (ptr) == SSA_NAME
	      && SSA_NAME_PTR_INFO (ptr)
	      && SSA_NAME_PTR_INFO (ptr)->name_mem_tag)
	    tag = SSA_NAME_PTR_INFO (ptr)->name_mem_tag;
	  else
	    {
	      tree sym = DECL_P (ptr) ? ptr : SSA_NAME_VAR (ptr);
	      tag = var_ann (sym)->symbol_mem_tag;
	    }

	  if (tag)
	    add_virtual_operator (flags);
	}
    }
  else if (TREE_CODE (ptr) == INTEGER_CST)
    {
      /* If a constant is used as a pointer, we can't generate a real
	 operand for it but we mark the statement volatile to prevent
	 optimizations from messing things up.  */
      if (s_ann)
	s_ann->has_volatile_ops = true;
      return;
    }
  else
    {
      /* OK, this isn't even is_gimple_min_invariant.  Something's
	 broke.  */
      gcc_unreachable ();
    }

  /* Recurse to get to the base pointer and add it as a USEd operand.  */
  get_expr_operands (stmt, &TREE_OPERAND (expr, 0), opf_use);
}


/* A subroutine of get_expr_operands to handle TARGET_MEM_REF.  */

static void
get_tmr_operands (tree stmt, tree expr, int flags)
{
  tree tag;

  /* When gathering memory symbols, there is no need to keep analyzing
     the rest of the expression.  */
  if (gathering_loads_stores ())
    {
      get_mem_symbols_in_tmr (expr, flags);
      return;
    }

  /* First record the real operands.  */
  get_expr_operands (stmt, &TMR_BASE (expr), opf_use);
  get_expr_operands (stmt, &TMR_INDEX (expr), opf_use);

  if (TMR_SYMBOL (expr))
    {
      stmt_ann_t ann = stmt_ann (stmt);
      add_to_addressable_set (TMR_SYMBOL (expr), &ann->addresses_taken);
    }

  tag = TMR_TAG (expr);
  if (!tag)
    {
      /* Something weird, so ensure that we will be careful.  */
      stmt_ann (stmt)->has_volatile_ops = true;
      return;
    }

  add_virtual_operator (flags);
}


/* A subroutine of get_expr_operands to handle CALL_EXPR.  */

static void
get_call_expr_operands (tree stmt, tree expr)
{
  tree op;
  int call_flags = call_expr_flags (expr);

  if (!bitmap_empty_p (call_clobbered_vars) && !(call_flags & ECF_NOVOPS))
    {
      /* A 'pure' or a 'const' function never call-clobbers anything. 
	 A 'noreturn' function might, but since we don't return anyway 
	 there is no point in recording that.  */ 
      if (TREE_SIDE_EFFECTS (expr)
	  && !(call_flags & (ECF_PURE | ECF_CONST | ECF_NORETURN)))
	add_call_clobbered_mem_symbols (get_callee_fndecl (expr), call_flags);
      else if (!(call_flags & ECF_CONST))
	add_call_read_mem_symbols (get_callee_fndecl (expr));
    }

  /* Find uses in the called function.  */
  if (!gathering_loads_stores ())
    {
      /* There may not be memory symbols in these operands, so we only
	 traverse them when scanning for real operands.  */
      get_expr_operands (stmt, &TREE_OPERAND (expr, 0), opf_use);
      get_expr_operands (stmt, &TREE_OPERAND (expr, 2), opf_use);
    }

  for (op = TREE_OPERAND (expr, 1); op; op = TREE_CHAIN (op))
    get_expr_operands (stmt, &TREE_VALUE (op), opf_use);
}


/* Scan operands in the ASM_EXPR stmt referred to in INFO.  */

static void
get_asm_expr_operands (tree stmt)
{
  stmt_ann_t s_ann;
  int i, noutputs;
  const char **oconstraints;
  const char *constraint;
  bool allows_mem, allows_reg, is_inout, add_vdef_p, add_vuse_p;
  tree link;

  s_ann = stmt_ann (stmt);
  noutputs = list_length (ASM_OUTPUTS (stmt));
  oconstraints = (const char **) alloca ((noutputs) * sizeof (const char *));

  /* Gather all output operands.  */
  for (i = 0, link = ASM_OUTPUTS (stmt); link; i++, link = TREE_CHAIN (link))
    {
      constraint = TREE_STRING_POINTER (TREE_VALUE (TREE_PURPOSE (link)));
      oconstraints[i] = constraint;
      parse_output_constraint (&constraint, i, 0, 0, &allows_mem,
	                       &allows_reg, &is_inout);

      /* This should have been split in gimplify_asm_expr.  */
      gcc_assert (!allows_reg || !is_inout);

      /* Memory operands are addressable.  Note that STMT needs the
	 address of this operand.  */
      if (!allows_reg && allows_mem)
	{
	  tree t = get_base_address (TREE_VALUE (link));
	  if (t && DECL_P (t) && s_ann)
	    add_to_addressable_set (t, &s_ann->addresses_taken);
	}

      get_expr_operands (stmt, &TREE_VALUE (link), opf_def);
    }

  /* Gather all input operands.  */
  for (link = ASM_INPUTS (stmt); link; link = TREE_CHAIN (link))
    {
      constraint = TREE_STRING_POINTER (TREE_VALUE (TREE_PURPOSE (link)));
      parse_input_constraint (&constraint, 0, 0, noutputs, 0, oconstraints,
	                      &allows_mem, &allows_reg);

      /* Memory operands are addressable.  Note that STMT needs the
	 address of this operand.  */
      if (!allows_reg && allows_mem)
	{
	  tree t = get_base_address (TREE_VALUE (link));
	  if (t && DECL_P (t) && s_ann)
	    add_to_addressable_set (t, &s_ann->addresses_taken);
	}

      get_expr_operands (stmt, &TREE_VALUE (link), 0);
    }

  /* Clobber all memory and addressable symbols for asm ("" : : : "memory");  */
  add_vdef_p = false;
  add_vuse_p = false;
  for (link = ASM_CLOBBERS (stmt); link; link = TREE_CHAIN (link))
    if (strcmp (TREE_STRING_POINTER (TREE_VALUE (link)), "memory") == 0)
      {
	unsigned i;
	bitmap_iterator bi;

	EXECUTE_IF_SET_IN_BITMAP (call_clobbered_vars, 0, i, bi)
	  {
	    tree var;

	    /* Don't bother checking any more variables if we have
	       decided to add both a VDEF and a VUSE operator already.  */
	    if (add_vdef_p && add_vuse_p)
	      break;

	    var = referenced_var (i);

	    if (gathering_loads_stores ())
	      add_mem_symbols_in_decl (var, opf_def | opf_implicit);
	    else if (unmodifiable_var_p (var))
	      add_vuse_p = true;
	    else
	      add_vdef_p = true;
	  }

	EXECUTE_IF_SET_IN_BITMAP (addressable_vars, 0, i, bi)
	  {
	    tree var;

	    /* Don't bother checking any more variables if we have
	       decided to add both a VDEF and a VUSE operator already.  */
	    if (add_vdef_p && add_vuse_p)
	      break;

	    var = referenced_var (i);

	    /* Subvars are explicitly represented in this list, so we
	       don't need the original to be added to the clobber ops,
	       but the original *will* be in this list because we keep
	       the addressability of the original variable up-to-date
	       to avoid confusing the back-end.  */
	    if (var_can_have_subvars (var)
		&& get_subvars_for_var (var) != NULL)
	      continue;		

	    if (gathering_loads_stores ())
	      add_mem_symbols_in_decl (var, opf_def | opf_implicit);
	    else if (unmodifiable_var_p (var))
	      add_vuse_p = true;
	    else
	      add_vdef_p = true;
	  }
	break;
      }

  if (add_vdef_p)
    add_virtual_operator (opf_def);

  if (add_vuse_p)
    add_virtual_operator (opf_use);
}


/* Scan operands for the assignment expression EXPR in statement STMT.  */

static void
get_modify_expr_operands (tree stmt, tree expr)
{
  /* First get operands from the RHS.  */
  get_expr_operands (stmt, &TREE_OPERAND (expr, 1), opf_use);

  /* For the LHS, use a regular definition (opf_def) for GIMPLE
     registers.  If the LHS is a store to memory, we will need
     a preserving definition (VDEF).

     Preserving definitions are those that modify a part of an
     aggregate object for which no subvars have been computed (or the
     reference does not correspond exactly to one of them). Stores
     through a pointer are also represented with VDEF operators.

     We used to distinguish between preserving and killing definitions.
     We always emit preserving definitions now.  */
  get_expr_operands (stmt, &TREE_OPERAND (expr, 0), opf_def);
}


/* Scan operands in EXPR, a reference to an aggregate type (arrays,
   structures and complex).  STMT is the statement holding EXPR.
   FLAGS is as in get_expr_operands.  */

static void
get_aggregate_operands (tree stmt, tree expr, int flags)
{
  enum tree_code code = TREE_CODE (expr);
  stmt_ann_t s_ann = stmt_ann (stmt);

  /* If the reference is volatile, mark the statement accordingly.  */
  if (TREE_THIS_VOLATILE (expr))
    s_ann->has_volatile_ops = true;

  /* When gathering memory symbols, there is no need to keep analyzing
     the rest of the expression.  */
  if (gathering_loads_stores ())
    {
      get_mem_symbols_in_aggregate (expr, flags);
      return;
    }

  /* References to aggregates are references to the virtual variable
     representing the aggregate.  If the aggregate is a
     non-addressable stack variable, the virtual variable will be the
     base symbol for the reference.  Otherwise, addressable and global
     variables are represented with the artificial symbol MEM.  */
  get_expr_operands (stmt, &TREE_OPERAND (expr, 0), flags);

  if (code == COMPONENT_REF)
    {
      get_expr_operands (stmt, &TREE_OPERAND (expr, 2), opf_use);
    }
  else if (code == ARRAY_REF || code == ARRAY_RANGE_REF)
    {
      get_expr_operands (stmt, &TREE_OPERAND (expr, 1), opf_use);
      get_expr_operands (stmt, &TREE_OPERAND (expr, 2), opf_use);
      get_expr_operands (stmt, &TREE_OPERAND (expr, 3), opf_use);
    }
}


/* Recursively scan the expression pointed to by EXPR_P in statement
   STMT.  FLAGS is one of the OPF_* constants modifying how to
   interpret the operands found.  */

static void
get_expr_operands (tree stmt, tree *expr_p, int flags)
{
  enum tree_code code;
  enum tree_code_class class;
  tree expr = *expr_p;
  stmt_ann_t s_ann = stmt_ann (stmt);

  if (expr == NULL)
    return;

  code = TREE_CODE (expr);
  class = TREE_CODE_CLASS (code);

  switch (code)
    {
    case ADDR_EXPR:
      /* Taking the address of a variable does not represent a
	 reference to it, but the fact that the statement takes its
	 address will be of interest to some passes (e.g. alias
	 resolution).  */
      add_to_addressable_set (TREE_OPERAND (expr, 0), &s_ann->addresses_taken);

      /* If the address is invariant, there may be no interesting
	 variable references inside.  */
      if (is_gimple_min_invariant (expr))
	return;

      /* Otherwise, there may be variables referenced inside but there
	 should be no VUSEs created, since the referenced objects are
	 not really accessed.  The only operands that we should find
	 here are ARRAY_REF indices which will always be real operands
	 (GIMPLE does not allow non-registers as array indices).  */
      if (gathering_loads_stores ())
	return;

      flags |= opf_no_vops;
      get_expr_operands (stmt, &TREE_OPERAND (expr, 0), flags);
      return;

    case SSA_NAME:
    case VAR_DECL:
    case PARM_DECL:
    case RESULT_DECL:
      add_stmt_operand (expr_p, s_ann, flags);
      return;

    case MISALIGNED_INDIRECT_REF:
      get_expr_operands (stmt, &TREE_OPERAND (expr, 1), flags);
      /* fall through */

    case ALIGN_INDIRECT_REF:
    case INDIRECT_REF:
      get_indirect_ref_operands (stmt, expr, flags);
      return;

    case TARGET_MEM_REF:
      get_tmr_operands (stmt, expr, flags);
      return;

    case ARRAY_REF:
    case ARRAY_RANGE_REF:
    case COMPONENT_REF:
    case REALPART_EXPR:
    case IMAGPART_EXPR:
      get_aggregate_operands (stmt, expr, flags);
      return;

    case WITH_SIZE_EXPR:
      /* WITH_SIZE_EXPR is a pass-through reference to its first argument,
	 and an rvalue reference to its second argument.  */
      get_expr_operands (stmt, &TREE_OPERAND (expr, 1), opf_use);
      get_expr_operands (stmt, &TREE_OPERAND (expr, 0), flags);
      return;

    case CALL_EXPR:
      get_call_expr_operands (stmt, expr);
      return;

    case COND_EXPR:
    case VEC_COND_EXPR:
      get_expr_operands (stmt, &TREE_OPERAND (expr, 0), opf_use);
      get_expr_operands (stmt, &TREE_OPERAND (expr, 1), opf_use);
      get_expr_operands (stmt, &TREE_OPERAND (expr, 2), opf_use);
      return;

    case MODIFY_EXPR:
      get_modify_expr_operands (stmt, expr);
      return;

    case CONSTRUCTOR:
      {
	/* General aggregate CONSTRUCTORs have been decomposed, but they
	   are still in use as the COMPLEX_EXPR equivalent for vectors.  */
	constructor_elt *ce;
	unsigned HOST_WIDE_INT idx;

	for (idx = 0;
	     VEC_iterate (constructor_elt, CONSTRUCTOR_ELTS (expr), idx, ce);
	     idx++)
	  get_expr_operands (stmt, &ce->value, opf_use);

	return;
      }

    case BIT_FIELD_REF:
    case TRUTH_NOT_EXPR:
    case VIEW_CONVERT_EXPR:
    do_unary:
      get_expr_operands (stmt, &TREE_OPERAND (expr, 0), flags);
      return;

    case TRUTH_AND_EXPR:
    case TRUTH_OR_EXPR:
    case TRUTH_XOR_EXPR:
    case COMPOUND_EXPR:
    case OBJ_TYPE_REF:
    case ASSERT_EXPR:
    do_binary:
      {
	get_expr_operands (stmt, &TREE_OPERAND (expr, 0), flags);
	get_expr_operands (stmt, &TREE_OPERAND (expr, 1), flags);
	return;
      }

    case DOT_PROD_EXPR:
    case REALIGN_LOAD_EXPR:
      {
	get_expr_operands (stmt, &TREE_OPERAND (expr, 0), flags);
        get_expr_operands (stmt, &TREE_OPERAND (expr, 1), flags);
        get_expr_operands (stmt, &TREE_OPERAND (expr, 2), flags);
        return;
      }

    case BLOCK:
    case FUNCTION_DECL:
    case EXC_PTR_EXPR:
    case FILTER_EXPR:
    case LABEL_DECL:
    case CONST_DECL:
    case OMP_PARALLEL:
    case OMP_SECTIONS:
    case OMP_FOR:
    case OMP_SINGLE:
    case OMP_MASTER:
    case OMP_ORDERED:
    case OMP_CRITICAL:
    case OMP_RETURN:
    case OMP_CONTINUE:
      /* Expressions that make no memory references.  */
      return;

    default:
      if (class == tcc_unary)
	goto do_unary;
      if (class == tcc_binary || class == tcc_comparison)
	goto do_binary;
      if (class == tcc_constant || class == tcc_type)
	return;
    }

  /* If we get here, something has gone wrong.  */
#ifdef ENABLE_CHECKING
  fprintf (stderr, "unhandled expression in get_expr_operands():\n");
  debug_tree (expr);
  fputs ("\n", stderr);
#endif
  gcc_unreachable ();
}


/* Parse STMT looking for operands.  When finished, the various
   build_* operand vectors will have potential operands in them.  */

static void
parse_ssa_operands (tree stmt)
{
  enum tree_code code;

  code = TREE_CODE (stmt);
  switch (code)
    {
    case MODIFY_EXPR:
      get_modify_expr_operands (stmt, stmt);
      break;

    case COND_EXPR:
      get_expr_operands (stmt, &COND_EXPR_COND (stmt), opf_use);
      break;

    case SWITCH_EXPR:
      get_expr_operands (stmt, &SWITCH_COND (stmt), opf_use);
      break;

    case ASM_EXPR:
      get_asm_expr_operands (stmt);
      break;

    case RETURN_EXPR:
      get_expr_operands (stmt, &TREE_OPERAND (stmt, 0), opf_use);
      break;

    case GOTO_EXPR:
      get_expr_operands (stmt, &GOTO_DESTINATION (stmt), opf_use);
      break;

    case LABEL_EXPR:
      get_expr_operands (stmt, &LABEL_EXPR_LABEL (stmt), opf_use);
      break;

    case BIND_EXPR:
    case CASE_LABEL_EXPR:
    case TRY_CATCH_EXPR:
    case TRY_FINALLY_EXPR:
    case EH_FILTER_EXPR:
    case CATCH_EXPR:
    case RESX_EXPR:
      /* These nodes contain no variable references.  */
      break;

    default:
      /* Notice that if get_expr_operands tries to use &STMT as the
	 operand pointer (which may only happen for USE operands), we
	 will fail in add_stmt_operand.  This default will handle
	 statements like empty statements, or CALL_EXPRs that may
	 appear on the RHS of a statement or as statements themselves.  */
      get_expr_operands (stmt, &stmt, opf_use);
      break;
    }
}


/* Create an operands cache for STMT.  */

static void
build_ssa_operands (tree stmt)
{
  stmt_ann_t ann = get_stmt_ann (stmt);
  
  /* Initially assume that the statement has no volatile operands.  */
  if (ann)
    ann->has_volatile_ops = false;

  start_ssa_stmt_operands ();

  parse_ssa_operands (stmt);
  operand_build_sort_virtual (build_vuses);
  operand_build_sort_virtual (build_vdefs);

  finalize_ssa_stmt_operands (stmt);
}


/* Free any operands vectors in OPS.  */

void 
free_ssa_operands (stmt_operands_p ops)
{
  ops->def_ops = NULL;
  ops->use_ops = NULL;
  ops->vdef_ops = NULL;
  ops->vuse_ops = NULL;
}


/* Get the operands of statement STMT.  */

void
update_stmt_operands (tree stmt)
{
  stmt_ann_t ann = get_stmt_ann (stmt);

  /* If update_stmt_operands is called before SSA is initialized, do
     nothing.  */
  if (!ssa_operands_active ())
    return;

  /* The optimizers cannot handle statements that are nothing but a
     _DECL.  This indicates a bug in the gimplifier.  */
  gcc_assert (!SSA_VAR_P (stmt));

  gcc_assert (ann->modified);

  /* This cannot be called from get_loads_and_stores.  */
  gcc_assert (!gathering_loads_stores ());

  timevar_push (TV_TREE_OPS);

  build_ssa_operands (stmt);

  /* Clear the modified bit for STMT.  */
  ann->modified = 0;

  timevar_pop (TV_TREE_OPS);
}


/* Copies virtual operands from SRC to DST.  */

void
copy_virtual_operands (tree dest, tree src)
{
  tree t;
  unsigned int n;
  ssa_op_iter iter;

  n = NUM_SSA_OPERANDS (src, SSA_OP_VUSE);
  if (n > 0)
    {
      int x = 0;
      struct vuse_optype_d vuse;
      vuse_optype_p vuse_p = &vuse;
      vuse_p = add_vuse_op (dest, NULL, n, &vuse_p);
      vuse_p->next = NULL;
      FOR_EACH_SSA_TREE_OPERAND (t, src, iter, SSA_OP_VUSE)
	SET_USE (VUSE_OP_PTR (vuse_p, x++), t);
      VUSE_OPS (dest) = vuse.next;
    }
  else
    VUSE_OPS (dest) = NULL;

  n = NUM_SSA_OPERANDS (src, SSA_OP_VMAYUSE);
  if (n > 0)
    {
      int x = 0;
      struct vdef_optype_d vuse;
      vdef_optype_p vuse_p = &vuse;
      vuse_p = add_vdef_op (dest, NULL, n, &vuse_p);
      vuse_p->next = NULL;
      FOR_EACH_SSA_TREE_OPERAND (t, src, iter, SSA_OP_VMAYUSE)
	SET_USE (VDEF_OP_PTR (vuse_p, x++), t);
      SET_DEF (VDEF_RESULT_PTR (vuse_p), VDEF_RESULT (VDEF_OPS (src)));
      VDEF_OPS (dest) = vuse.next;
    }
  else
    VDEF_OPS (dest) = NULL;
}


/* Specifically for use in DOM's expression analysis.  Given a store, we
   create an artificial stmt which looks like a load from the store, this can
   be used to eliminate redundant loads.  OLD_OPS are the operands from the 
   store stmt, and NEW_STMT is the new load which represents a load of the
   values stored.  */

void
create_ssa_artficial_load_stmt (tree new_stmt, tree old_stmt)
{
  tree op;
  ssa_op_iter iter;
  use_operand_p use_p;
  vdef_optype_p vdefs;
  vuse_optype_p vuses;

  get_stmt_ann (new_stmt);

  /* Process NEW_STMT looking for operands.  */
  start_ssa_stmt_operands ();
  parse_ssa_operands (new_stmt);

  /* Remove any virtual operands that were found.  */
  VEC_truncate (tree, build_vdefs, 0);
  VEC_truncate (tree, build_vuses, 0);

  /* Create a VUSE for the LHS of the VDEF operator in the original
     statement (OLD_STMT).  */
  vdefs = VDEF_OPS (old_stmt);
  gcc_assert (vdefs && vdefs->next == NULL);

  /* Add a VUSE operator for NEW_STMT and set its operand to the LHS
     of the old VDEF.  */
  append_vuse ();
  finalize_ssa_stmt_operands (new_stmt);
  op = VDEF_RESULT (vdefs);
  vuses = VUSE_OPS (new_stmt);
  SET_USE (VUSE_OP_PTR (vuses, 0), op);

  /* All uses in this fake stmt must not be in the immediate use lists.  */
  FOR_EACH_SSA_USE_OPERAND (use_p, new_stmt, iter, SSA_OP_ALL_USES)
    delink_imm_use (use_p);
}


/* Swap operands EXP0 and EXP1 in statement STMT.  No attempt is done
   to test the validity of the swap operation.  */

void
swap_tree_operands (tree stmt, tree *exp0, tree *exp1)
{
  tree op0, op1;
  op0 = *exp0;
  op1 = *exp1;

  /* If the operand cache is active, attempt to preserve the relative
     positions of these two operands in their respective immediate use
     lists.  */
  if (ssa_operands_active () && op0 != op1)
    {
      use_optype_p use0, use1, ptr;
      use0 = use1 = NULL;

      /* Find the 2 operands in the cache, if they are there.  */
      for (ptr = USE_OPS (stmt); ptr; ptr = ptr->next)
	if (USE_OP_PTR (ptr)->use == exp0)
	  {
	    use0 = ptr;
	    break;
	  }

      for (ptr = USE_OPS (stmt); ptr; ptr = ptr->next)
	if (USE_OP_PTR (ptr)->use == exp1)
	  {
	    use1 = ptr;
	    break;
	  }

      /* If both uses don't have operand entries, there isn't much we can do
         at this point.  Presumably we don't need to worry about it.  */
      if (use0 && use1)
        {
	  tree *tmp = USE_OP_PTR (use1)->use;
	  USE_OP_PTR (use1)->use = USE_OP_PTR (use0)->use;
	  USE_OP_PTR (use0)->use = tmp;
	}
    }

  /* Now swap the data.  */
  *exp0 = op1;
  *exp1 = op0;
}


/* Add the base address of REF to the set *ADDRESSES_TAKEN.  If
   *ADDRESSES_TAKEN is NULL, a new set is created.  REF may be
   a single variable whose address has been taken or any other valid
   GIMPLE memory reference (structure reference, array, etc).  If the
   base address of REF is a decl that has sub-variables, also add all
   of its sub-variables.  */

void
add_to_addressable_set (tree ref, bitmap *addresses_taken)
{
  tree var;
  subvar_t svars;

  gcc_assert (addresses_taken);

  /* Note that it is *NOT OKAY* to use the target of a COMPONENT_REF
     as the only thing we take the address of.  If VAR is a structure,
     taking the address of a field means that the whole structure may
     be referenced using pointer arithmetic.  See PR 21407 and the
     ensuing mailing list discussion.  */
  var = get_base_address (ref);
  if (var && SSA_VAR_P (var))
    {
      if (*addresses_taken == NULL)
	*addresses_taken = BITMAP_GGC_ALLOC ();      
      
      if (var_can_have_subvars (var)
	  && (svars = get_subvars_for_var (var)))
	{
	  subvar_t sv;
	  for (sv = svars; sv; sv = sv->next)
	    {
	      bitmap_set_bit (*addresses_taken, DECL_UID (sv->var));
	      TREE_ADDRESSABLE (sv->var) = 1;
	    }
	}
      else
	{
	  bitmap_set_bit (*addresses_taken, DECL_UID (var));
	  TREE_ADDRESSABLE (var) = 1;
	}
    }
}


/* Scan the immediate_use list for VAR making sure its linked properly.
   Return TRUE if there is a problem and emit an error message to F.  */

bool
verify_imm_links (FILE *f, tree var)
{
  use_operand_p ptr, prev, list;
  int count;

  gcc_assert (TREE_CODE (var) == SSA_NAME);

  list = &(SSA_NAME_IMM_USE_NODE (var));
  gcc_assert (list->use == NULL);

  if (list->prev == NULL)
    {
      gcc_assert (list->next == NULL);
      return false;
    }

  prev = list;
  count = 0;
  for (ptr = list->next; ptr != list; )
    {
      if (prev != ptr->prev)
	goto error;
      
      if (ptr->use == NULL)
	goto error; /* 2 roots, or SAFE guard node.  */
      else if (*(ptr->use) != var)
	goto error;

      prev = ptr;
      ptr = ptr->next;

      /* Avoid infinite loops.  50,000,000 uses probably indicates a
	 problem.  */
      if (count++ > 50000000)
	goto error;
    }

  /* Verify list in the other direction.  */
  prev = list;
  for (ptr = list->prev; ptr != list; )
    {
      if (prev != ptr->next)
	goto error;
      prev = ptr;
      ptr = ptr->prev;
      if (count-- < 0)
	goto error;
    }

  if (count != 0)
    goto error;

  return false;

 error:
  if (ptr->stmt && stmt_modified_p (ptr->stmt))
    {
      fprintf (f, " STMT MODIFIED. - <%p> ", (void *)ptr->stmt);
      print_generic_stmt (f, ptr->stmt, TDF_SLIM);
    }
  fprintf (f, " IMM ERROR : (use_p : tree - %p:%p)", (void *)ptr, 
	   (void *)ptr->use);
  print_generic_expr (f, USE_FROM_PTR (ptr), TDF_SLIM);
  fprintf(f, "\n");
  return true;
}


/* Dump all the immediate uses to FILE.  */

void
dump_immediate_uses_for (FILE *file, tree var)
{
  imm_use_iterator iter;
  use_operand_p use_p;

  gcc_assert (var && TREE_CODE (var) == SSA_NAME);

  print_generic_expr (file, var, TDF_SLIM);
  fprintf (file, " : -->");
  if (has_zero_uses (var))
    fprintf (file, " no uses.\n");
  else
    if (has_single_use (var))
      fprintf (file, " single use.\n");
    else
      fprintf (file, "%d uses.\n", num_imm_uses (var));

  FOR_EACH_IMM_USE_FAST (use_p, iter, var)
    {
      if (use_p->stmt == NULL && use_p->use == NULL)
        fprintf (file, "***end of stmt iterator marker***\n");
      else
	if (!is_gimple_reg (USE_FROM_PTR (use_p)))
	  print_generic_stmt (file, USE_STMT (use_p), TDF_VOPS);
	else
	  print_generic_stmt (file, USE_STMT (use_p), TDF_SLIM);
    }
  fprintf(file, "\n");
}


/* Dump all the immediate uses to FILE.  */

void
dump_immediate_uses (FILE *file)
{
  tree var;
  unsigned int x;

  fprintf (file, "Immediate_uses: \n\n");
  for (x = 1; x < num_ssa_names; x++)
    {
      var = ssa_name(x);
      if (!var)
        continue;
      dump_immediate_uses_for (file, var);
    }
}


/* Dump def-use edges on stderr.  */

void
debug_immediate_uses (void)
{
  dump_immediate_uses (stderr);
}


/* Dump def-use edges on stderr.  */

void
debug_immediate_uses_for (tree var)
{
  dump_immediate_uses_for (stderr, var);
}

void debug_loads_and_stores (tree);
void dump_loads_and_stores (FILE *, tree, bitmap, bitmap);


/* Dump symbols loaded and stored by STMT to FILE.  If LOADS and
   STORES are given, the statement is not scanned.  */

void
dump_loads_and_stores (FILE *file, tree stmt, bitmap loads, bitmap stores)
{
  unsigned i;
  bitmap_iterator bi;
  bool free_memory_p = false;

  if (loads == NULL && stores == NULL)
    {
      loads = BITMAP_ALLOC (NULL);
      stores = BITMAP_ALLOC (NULL);
      get_loads_and_stores (stmt, loads, stores);
      free_memory_p = true;
    }

  fprintf (file, "\n\n");
  print_generic_stmt (file, stmt, TDF_VOPS);
  fprintf (file, "\tLOADS:  { ");
  EXECUTE_IF_SET_IN_BITMAP (loads, 0, i, bi)
    {
      print_generic_expr (file, referenced_var (i), 0);
      fprintf (file, " ");
    }
  fprintf (file, "}\n");

  fprintf (file, "\tSTORES: { ");
  EXECUTE_IF_SET_IN_BITMAP (stores, 0, i, bi)
    {
      print_generic_expr (file, referenced_var (i), 0);
      fprintf (file, " ");
    }
  fprintf (file, "}\n");

  if (free_memory_p)
    {
      BITMAP_FREE (loads);
      BITMAP_FREE (stores);
    }
}


/* Dump symbols loaded and stored by STMT to stderr.  */

void
debug_loads_and_stores (tree stmt)
{
  dump_loads_and_stores (stderr, stmt, NULL, NULL);
}


/* Collect all the symbols loaded and stored by the arguments of PHI
   node PHI.  Store the sets in LOADS and STORES respectively.  */

static void
get_loads_and_stores_for_phi (tree phi, bitmap loads, bitmap stores)
{
  int i;
  tree lhs = PHI_RESULT (phi);

  gcc_assert (!is_gimple_reg (lhs));

  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
    {
      tree sym, arg;

      arg = PHI_ARG_DEF (phi, i);

      /* Avoid infinite recursion.  */
      if (arg == lhs)
	continue;

      sym = SSA_NAME_VAR (arg);

      if (sym == mem_var)
	{
	  /* Recurse for an memory factored SSA name.  */
	  get_loads_and_stores (SSA_NAME_DEF_STMT (arg), loads, stores);
	}
      else
	{
	  /* Otherwise, this PHI node will both load and store the
	     underlying symbol for ARG.  */
	  bitmap_set_bit (loads, DECL_UID (sym));
	  bitmap_set_bit (stores, DECL_UID (sym));
	}
    }
}




/* Add to LOADED the set of symbols loaded by STMT and add to STORED
   the set of symbols stored by STMT.  Both sets must be allocated by
   the caller.  FIXME, should allow for either set to be NULL.  */

void
get_loads_and_stores (tree stmt, bitmap loads, bitmap stores)
{
  gcc_assert (loads && stores);
  memset (&clobber_stats, 0, sizeof (clobber_stats));

  if (TREE_CODE (stmt) != PHI_NODE)
    {
      gcc_assert (stmt_references_memory_p (stmt));

      /* Point the internal loaded/stored sets to the ones provided.  */
      loaded_syms = loads;
      stored_syms = stores;

      /* Parse the statement.  We don't really care for its operands, so
	 there's no need to initialize anything.  If any operand was added
	 to the cache, it is discarded.  */
      parse_ssa_operands (stmt);
      truncate_ssa_stmt_operands ();

      /* We don't need the symbol sets anymore.  */
      loaded_syms = NULL;
      stored_syms = NULL;
    }
  else
    {
      /* PHI nodes need special treatment.  A PHI node loads/stores
	 all the symbols loaded/stored by its arguments.  */
      get_loads_and_stores_for_phi (stmt, loads, stores);
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    dump_loads_and_stores (dump_file, stmt, loads, stores);

  if (dump_file && (dump_flags & TDF_STATS))
    {
      fprintf (dump_file, "Original clobbered vars:           %d\n",
	       clobber_stats.clobbered_vars);
      fprintf (dump_file, "Static write clobbers avoided:     %d\n",
	       clobber_stats.static_write_clobbers_avoided);
      fprintf (dump_file, "Static read clobbers avoided:      %d\n",
	       clobber_stats.static_read_clobbers_avoided);
      fprintf (dump_file, "Unescapable clobbers avoided:      %d\n",
	       clobber_stats.unescapable_clobbers_avoided);
      fprintf (dump_file, "Original read-only clobbers:       %d\n",
	       clobber_stats.readonly_clobbers);
      fprintf (dump_file, "Static read-only clobbers avoided: %d\n",
	       clobber_stats.static_readonly_clobbers_avoided);
    }
}


/* Create a new change buffer for the statement pointed by STMT_P and
   push the buffer into SCB_STACK.  Each change buffer
   records state information needed to determine what changed in the
   statement.  Mainly, this keeps track of symbols that may need to be
   put into SSA form, SSA name replacements and other information
   needed to keep the SSA form up to date.  */

void
push_stmt_changes (tree *stmt_p)
{
  tree stmt;
  scb_t buf;
  
  stmt = *stmt_p;

  /* It makes no sense to keep track of PHI nodes.  */
  if (TREE_CODE (stmt) == PHI_NODE)
    return;

  buf = xmalloc (sizeof *buf);
  memset (buf, 0, sizeof *buf);

  buf->stmt_p = stmt_p;

  if (stmt_references_memory_p (stmt))
    {
      buf->loads = BITMAP_ALLOC (NULL);
      buf->stores = BITMAP_ALLOC (NULL);
      get_loads_and_stores (stmt, buf->loads, buf->stores);
    }

  VEC_safe_push (scb_t, heap, scb_stack, buf);
}


/* Given two sets S1 and S2, mark the symbols in S1 U S2 for renaming
   if S1 != S2.  Sets S1 and S2 may be overwritten by this function.  */

static void
mark_difference_for_renaming (bitmap s1, bitmap s2)
{
  bitmap tmp = NULL;

  if (s1 == NULL && s2 == NULL)
    return;

  if (s1 && s2 == NULL)
    tmp = s1;
  else if (s1 == NULL && s2)
    tmp = s2;
  else if (!bitmap_equal_p (s1, s2))
    {
      tmp = s1;
      bitmap_ior_into (tmp, s2);
    }

  if (tmp)
    mark_set_for_renaming (tmp);
}


/* Pop the top SCB from SCB_STACK and act on the differences between
   what was recorded by push_stmt_changes and the current state of
   the statement.  */

void
pop_stmt_changes (tree *stmt_p)
{
  tree op, stmt;
  ssa_op_iter iter;
  bitmap loads, stores;
  scb_t buf;

  stmt = *stmt_p;

  /* It makes no sense to keep track of PHI nodes.  */
  if (TREE_CODE (stmt) == PHI_NODE)
    return;

  buf = VEC_pop (scb_t, scb_stack);
  gcc_assert (stmt_p == buf->stmt_p);

  /* Force an operand re-scan on the statement and mark any newly
     exposed variables.  */
  update_stmt (stmt);

  /* Determine whether any memory symbols need to be renamed.  If the
     sets of loads and stores are different after the statement is
     modified, then the affected symbols need to be renamed.  */
  loads = stores = NULL;
  if (stmt_references_memory_p (stmt))
    {
      loads = BITMAP_ALLOC (NULL);
      stores = BITMAP_ALLOC (NULL);
      get_loads_and_stores (stmt, loads, stores);
    }

  /* If LOADS is different from BUF->LOADS, the affected
     symbols need to be marked for renaming.  */
  mark_difference_for_renaming (loads, buf->loads);

  /* Similarly for STORES And BUF->STORES.  */
  mark_difference_for_renaming (stores, buf->stores);

  /* Mark all the GIMPLE register operands for renaming.  */
  FOR_EACH_SSA_TREE_OPERAND (op, stmt, iter, SSA_OP_DEF|SSA_OP_USE)
    if (DECL_P (op))
      mark_sym_for_renaming (op);

  /* FIXME, need to add more finalizers here.  Cleanup EH info,
     recompute invariants for address expressions, add
     SSA replacement mappings, etc.  For instance, given
     testsuite/gcc.c-torture/compile/pr16808.c, we fold a statement of
     the form:

	  # SMT.4_20 = VDEF <SMT.4_16>
	  D.1576_11 = 1.0e+0;

     So, the VDEF will disappear, but instead of marking SMT.4 for
     renaming it would be far more efficient to establish a
     replacement mapping that would replace every reference of
     SMT.4_20 with SMT.4_16.  */

  /* Free memory used by the buffer.  */
  BITMAP_FREE (buf->loads);
  BITMAP_FREE (buf->stores);
  buf->stmt_p = NULL;
  free (buf);

  BITMAP_FREE (loads);
  BITMAP_FREE (stores);
}

#include "gt-tree-ssa-operands.h"
