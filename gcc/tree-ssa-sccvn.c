/* SCC value numbering for trees
   Copyright (C) 2006
   Free Software Foundation, Inc.
   Contributed by Daniel Berlin <dan@dberlin.org>

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
#include "ggc.h"
#include "tree.h"
#include "basic-block.h"
#include "diagnostic.h"
#include "tree-inline.h"
#include "tree-flow.h"
#include "tree-gimple.h"
#include "tree-dump.h"
#include "timevar.h"
#include "fibheap.h"
#include "hashtab.h"
#include "tree-iterator.h"
#include "real.h"
#include "alloc-pool.h"
#include "tree-pass.h"
#include "flags.h"
#include "bitmap.h"
#include "langhooks.h"
#include "cfgloop.h"


typedef struct vn_tables_s
{
  htab_t unary;
  htab_t binary;
  htab_t phis;
  htab_t references;
  alloc_pool unary_op_pool;
  alloc_pool binary_op_pool;
  alloc_pool phis_pool;
  alloc_pool references_pool;
} *vn_tables_t;


typedef struct vn_binary_op_s
{
  tree op0;
  enum tree_code opcode;
  tree op1;
  hashval_t hashcode;
  tree result;
} *vn_binary_op_t;


typedef struct vn_unary_op_s
{
  tree op0;
  enum tree_code opcode;
  hashval_t hashcode;
  tree result;
} *vn_unary_op_t;

typedef struct vn_phi_s
{
  VEC(tree, heap) *phiargs;
#if 0
  tree orig_phi;
#endif
  basic_block block;
  hashval_t hashcode;
  tree result;
} *vn_phi_t;

typedef struct vn_reference_op_struct
{
  enum tree_code opcode;
  tree op0;
  tree op1;
  tree op2;
  tree op3;
  hashval_t hashcode;
} vn_reference_op_s;
typedef vn_reference_op_s *vn_reference_op_t;

DEF_VEC_O(vn_reference_op_s);
DEF_VEC_ALLOC_O(vn_reference_op_s, heap);

typedef struct vn_reference_s
{
  VEC(tree, heap) *vuses;
  VEC(vn_reference_op_s, heap) *operands;
  hashval_t hashcode;
  tree result;
} *vn_reference_t;

static vn_tables_t valid_info;
static vn_tables_t optimistic_info;
static vn_tables_t current_info;
static int *rpo_numbers;

typedef struct vn_ssa_aux
{
  /* SCC information.  */
  unsigned int dfsnum;
  bool visited;
  unsigned int low;
  bool on_sccstack;

  /* Value number.
     Note: This should always be an SSA_NAME or VN_TOP.
     Constants are not allowed.*/
  tree valnum;
  /* Representative expression, if not a direct constant. */
  tree expr;
  /* Whether the representative expression contains constants.  */
  bool has_constants;
} *vn_ssa_aux_t;

#define SSA_VAL(x) (VN_INFO ((x))->valnum)
static tree VN_TOP;

static unsigned int next_dfs_num;
static VEC (tree, heap) *sccstack;

DEF_VEC_P(vn_ssa_aux_t);
DEF_VEC_ALLOC_P(vn_ssa_aux_t, heap);
static VEC (vn_ssa_aux_t, heap) *vn_ssa_aux_table;
static inline vn_ssa_aux_t
VN_INFO (tree name)
{
  return VEC_index (vn_ssa_aux_t, vn_ssa_aux_table,
		    SSA_NAME_VERSION (name));
}

static inline void
VN_INFO_SET (tree name, vn_ssa_aux_t value)
{
  VEC_replace (vn_ssa_aux_t, vn_ssa_aux_table,
	       SSA_NAME_VERSION (name), value);
}

#if 0
static hashval_t
vn_reference_op_hash (const void *p1)
{
  const vn_reference_op_t vro1 = (vn_reference_op_t) p1;
  return vro1->hashcode;
}
#endif

static int
vn_reference_op_eq (const void *p1, const void *p2)
{
  const vn_reference_op_t vro1 = (vn_reference_op_t) p1;
  const vn_reference_op_t vro2 = (vn_reference_op_t) p2;
  return vro1->opcode == vro2->opcode
    && expressions_equal_p (vro1->op0, vro2->op0)
    && expressions_equal_p (vro1->op1, vro2->op1)
    && expressions_equal_p (vro1->op2, vro2->op2);
}

static hashval_t
vn_reference_op_compute_hash (const vn_reference_op_t vro1)
{
  return iterative_hash_expr (vro1->op0, vro1->opcode)
    + iterative_hash_expr (vro1->op1, vro1->opcode)
    + iterative_hash_expr (vro1->op2, vro1->opcode);
}

static hashval_t
vn_reference_hash (const void *p1)
{
  const vn_reference_t vr1 = (vn_reference_t) p1;
  return vr1->hashcode;
}

static inline hashval_t
vn_reference_compute_hash (const vn_reference_t vr1)
{
  hashval_t result = 0;
  tree v;
  int i;
  vn_reference_op_t vro;

  for (i = 0; VEC_iterate (tree, vr1->vuses, i, v); i++)
    result += iterative_hash_expr (v, 0);
  for (i = 0; VEC_iterate (vn_reference_op_s, vr1->operands, i, vro); i++)
    result += vn_reference_op_compute_hash (vro);

  return result;
}

static int
vn_reference_eq (const void *p1, const void *p2)
{
  tree v;
  int i;
  vn_reference_op_t vro;

  const vn_reference_t vr1 = (vn_reference_t) p1;
  const vn_reference_t vr2 = (vn_reference_t) p2;

  if (vr1->vuses == vr2->vuses
      && vr1->operands == vr2->operands)
    return true;

  if (VEC_length (tree, vr1->vuses) != VEC_length (tree, vr2->vuses))
    return false;

  if (VEC_length (vn_reference_op_s, vr1->operands)
      != VEC_length (vn_reference_op_s, vr2->operands))
    return false;

  for (i = 0; VEC_iterate (tree, vr1->vuses, i, v); i++)
    {
      if (VEC_index (tree, vr2->vuses, i) != v)
	return false;
    }

  for (i = 0; VEC_iterate (vn_reference_op_s, vr1->operands, i, vro); i++)
    {
      if (!vn_reference_op_eq (VEC_index (vn_reference_op_s, vr2->operands, i),
			       vro))
	return false;
    }
  return true;
}

/* Place the vuses from STMT into *result */

static inline void
vuses_to_vec (tree stmt, VEC (tree, heap) **result)
{
  ssa_op_iter iter;
  tree vuse;

  if (!stmt)
    return;

  FOR_EACH_SSA_TREE_OPERAND (vuse, stmt, iter, SSA_OP_VUSE)
    VEC_safe_push (tree, heap, *result, vuse);

  if (VEC_length (tree, *result) > 1)
    sort_vuses_heap (*result);
}


/* Copy the VUSE names in STMT into a vector, and return
   the vector.  */

static VEC (tree, heap) *
copy_vuses_from_stmt (tree stmt)
{
  VEC (tree, heap) *vuses = NULL;

  vuses_to_vec (stmt, &vuses);

  return vuses;
}

/* Place the vdefs from STMT into *result */

static inline void
vdefs_to_vec (tree stmt, VEC (tree, heap) **result)
{
  ssa_op_iter iter;
  tree vdef;

  if (!stmt)
    return;

  FOR_EACH_SSA_TREE_OPERAND (vdef, stmt, iter, SSA_OP_VIRTUAL_DEFS)
    VEC_safe_push (tree, heap, *result, vdef);

  if (VEC_length (tree, *result) > 1)
    sort_vuses_heap (*result);
}

/* Copy the names of vdef results in STMT into a vector, and return
   the vector.  */

static VEC (tree, heap) *
copy_vdefs_from_stmt (tree stmt)
{
  VEC (tree, heap) *vdefs = NULL;

  vdefs_to_vec (stmt, &vdefs);

  return vdefs;
}

/* Place for shared_vuses_from_stmt to shove vuses.  */
static VEC (tree, heap) *shared_lookup_vuses;

/* Copy the virtual uses from STMT into SHARED_LOOKUP_VUSES.
   This function will overwrite the current SHARED_LOOKUP_VUSES
   variable.  */

static VEC (tree, heap) *
shared_vuses_from_stmt (tree stmt)
{
  VEC_truncate (tree, shared_lookup_vuses, 0);
  vuses_to_vec (stmt, &shared_lookup_vuses);

  return shared_lookup_vuses;
}


/* Copy the names from virtual defs from STMT into SHARED_LOOKUP_VUSES.
   This function will overwrite the current SHARED_LOOKUP_VUSES
   variable.  */

static VEC (tree, heap) *
shared_vdefs_from_stmt (tree stmt)
{
  VEC_truncate (tree, shared_lookup_vuses, 0);
  vdefs_to_vec (stmt, &shared_lookup_vuses);

  return shared_lookup_vuses;
}


static void
copy_reference_ops_from_ref (tree ref, VEC(vn_reference_op_s, heap) **result)
{
  while (ref)
    {
      vn_reference_op_s temp;
      temp.opcode = TREE_CODE (ref);
      temp.op0 = NULL;
      temp.op1 = NULL;
      temp.op2 = NULL;

      switch (temp.opcode)
	{
	case BIT_FIELD_REF:
	case COMPONENT_REF:
	  temp.op0 = TREE_OPERAND (ref, 1);
	  temp.op1 = TREE_OPERAND (ref, 2);
	  break;
	case ARRAY_REF:
	  temp.op0 = TREE_OPERAND (ref, 1);
	  temp.op1 = TREE_OPERAND (ref, 2);
	  temp.op2 = TREE_OPERAND (ref, 3);
	  break;
	case VAR_DECL:
	case PARM_DECL:
	case CONST_DECL:
	case RESULT_DECL:
	case SSA_NAME:
	  temp.op0 = ref;
	  break;
	default:
	  break;
	}
      VEC_safe_push (vn_reference_op_s, heap, *result, &temp);

      if (TREE_CODE_CLASS (temp.opcode) == tcc_reference)
	ref = TREE_OPERAND (ref, 0);
      else
	ref = NULL_TREE;
    }
}

static VEC(vn_reference_op_s, heap) *
create_reference_ops_from_ref (tree ref)
{
  VEC (vn_reference_op_s, heap) *result = NULL;

  copy_reference_ops_from_ref (ref, &result);
  return result;
}

static VEC(vn_reference_op_s, heap) *shared_lookup_references;

static VEC(vn_reference_op_s, heap) *
shared_reference_ops_from_ref (tree ref)
{
  if (!ref)
    return NULL;
  VEC_truncate (vn_reference_op_s, shared_lookup_references, 0);
  copy_reference_ops_from_ref (ref, &shared_lookup_references);
  return shared_lookup_references;
}


static VEC (vn_reference_op_s, heap) *
valueize_refs (VEC (vn_reference_op_s, heap) *orig)
{
  vn_reference_op_t vro;
  int i;

  for (i = 0; VEC_iterate (vn_reference_op_s, orig, i, vro); i++)
    {
      if (vro->opcode == SSA_NAME)
	vro->op0 = SSA_VAL (vro->op0) ? SSA_VAL (vro->op0) : vro->op0;
    }

  return orig;
}

/* Lookup OP in the current hash table, and return the resulting
   value number if it exists in the hash table.  Return NULL_TREE if
   it does not exist in the hash table. */

static tree
vn_reference_lookup (tree op, VEC (tree, heap) *vuses)
{

  void **slot;
  struct vn_reference_s vr1;

  vr1.vuses = vuses;
  vr1.operands = valueize_refs (shared_reference_ops_from_ref (op));
  vr1.hashcode = vn_reference_compute_hash (&vr1);
  slot = htab_find_slot_with_hash (current_info->references, &vr1, vr1.hashcode,
				   NO_INSERT);
  if (!slot)
    return NULL_TREE;
  return ((vn_reference_t)*slot)->result;
}

/* Insert OP into the current hash table with a value number of
   RESULT.  */

static void
vn_reference_insert (tree op, tree result, VEC(tree, heap) *vuses)
{
  void **slot;
  vn_reference_t vr1 = (vn_reference_t) pool_alloc (current_info->references_pool);

  vr1->vuses = vuses;
  vr1->operands = valueize_refs (create_reference_ops_from_ref (op));
  vr1->hashcode = vn_reference_compute_hash (vr1);
  vr1->result = result;

  slot = htab_find_slot_with_hash (current_info->references, vr1, vr1->hashcode,
				   INSERT);
  gcc_assert (!*slot);

  *slot = vr1;
}

static hashval_t
vn_unary_op_hash (const void *p1)
{
  const vn_unary_op_t vuo1 = (vn_unary_op_t) p1;
  return vuo1->hashcode;
}

static inline hashval_t
vn_unary_op_compute_hash (const vn_unary_op_t vuo1)
{
  return iterative_hash_expr (vuo1->op0, vuo1->opcode);
}

static int
vn_unary_op_eq (const void *p1, const void *p2)
{
  const vn_unary_op_t vuo1 = (vn_unary_op_t) p1;
  const vn_unary_op_t vuo2 = (vn_unary_op_t) p2;
  return vuo1->opcode == vuo2->opcode
    && expressions_equal_p (vuo1->op0, vuo2->op0);
}

/* Lookup OP in the current hash table, and return the resulting
   value number if it exists in the hash table.  Return NULL_TREE if
   it does not exist in the hash table. */

static tree
vn_unary_op_lookup (tree op)
{
  void **slot;
  struct vn_unary_op_s vuo1;

  vuo1.opcode = TREE_CODE (op);
  vuo1.op0 = TREE_OPERAND (op, 0);

  if (TREE_CODE (vuo1.op0) == SSA_NAME)
    vuo1.op0 = SSA_VAL (vuo1.op0);

  vuo1.hashcode = vn_unary_op_compute_hash (&vuo1);
  slot = htab_find_slot_with_hash (current_info->unary, &vuo1, vuo1.hashcode,
				   NO_INSERT);
  if (!slot)
    return NULL_TREE;
  return ((vn_unary_op_t)*slot)->result;
}

/* Insert OP into the current hash table with a value number of
   RESULT.  */

static void
vn_unary_op_insert (tree op, tree result)
{
  void **slot;
  vn_unary_op_t vuo1 = (vn_unary_op_t) pool_alloc (current_info->unary_op_pool);

  vuo1->opcode = TREE_CODE (op);
  vuo1->op0 = TREE_OPERAND (op, 0);
  vuo1->result = result;

  if (TREE_CODE (vuo1->op0) == SSA_NAME)
    vuo1->op0 = SSA_VAL (vuo1->op0);

#if 0
  /* XXX: Shouldn't set this here. */
  if (is_gimple_min_invariant (vuo1->op0))
    VN_INFO (result)->has_constants = true;
#endif
  vuo1->hashcode = vn_unary_op_compute_hash (vuo1);
  slot = htab_find_slot_with_hash (current_info->unary, vuo1, vuo1->hashcode,
				   INSERT);
  gcc_assert (!*slot);
  *slot = vuo1;
}


static inline hashval_t
vn_binary_op_compute_hash (const vn_binary_op_t vbo1)
{
  return iterative_hash_expr (vbo1->op0, vbo1->opcode)
    + iterative_hash_expr (vbo1->op1, vbo1->opcode);
}

static hashval_t
vn_binary_op_hash (const void *p1)
{
  const vn_binary_op_t vbo1 = (vn_binary_op_t) p1;
  return vbo1->hashcode;
}

static int
vn_binary_op_eq (const void *p1, const void *p2)
{
  const vn_binary_op_t vbo1 = (vn_binary_op_t) p1;
  const vn_binary_op_t vbo2 = (vn_binary_op_t) p2;
  return vbo1->opcode == vbo2->opcode
    && expressions_equal_p (vbo1->op0, vbo2->op0)
    && expressions_equal_p (vbo1->op1, vbo2->op1);
}

/* Lookup OP in the current hash table, and return the resulting
   value number if it exists in the hash table.  Return NULL_TREE if
   it does not exist in the hash table. */

static tree
vn_binary_op_lookup (tree op)
{
  void **slot;
  struct vn_binary_op_s vbo1;

  vbo1.opcode = TREE_CODE (op);
  vbo1.op0 = TREE_OPERAND (op, 0);
  vbo1.op1 = TREE_OPERAND (op, 1);

  if (TREE_CODE (vbo1.op0) == SSA_NAME)
    vbo1.op0 = SSA_VAL (vbo1.op0);
  if (TREE_CODE (vbo1.op1) == SSA_NAME)
    vbo1.op1 = SSA_VAL (vbo1.op1);

  if (tree_swap_operands_p (vbo1.op0, vbo1.op1, false)
      && commutative_tree_code (vbo1.opcode))
    {
      tree temp = vbo1.op0;
      vbo1.op0 = vbo1.op1;
      vbo1.op1 = temp;
    }

  vbo1.hashcode = vn_binary_op_compute_hash (&vbo1);
  slot = htab_find_slot_with_hash (current_info->binary, &vbo1, vbo1.hashcode,
				   NO_INSERT);
  if (!slot)
    return NULL_TREE;
  return ((vn_binary_op_t)*slot)->result;
}

/* Insert OP into the current hash table with a value number of
   RESULT.  */

static void
vn_binary_op_insert (tree op, tree result)
{
  void **slot;
  vn_binary_op_t vbo1 = (vn_binary_op_t) pool_alloc (current_info->binary_op_pool);


  vbo1->opcode = TREE_CODE (op);
  vbo1->op0 = TREE_OPERAND (op, 0);
  vbo1->op1 = TREE_OPERAND (op, 1);
  vbo1->result = result;

  if (TREE_CODE (vbo1->op0) == SSA_NAME)
    vbo1->op0 = SSA_VAL (vbo1->op0);
  if (TREE_CODE (vbo1->op1) == SSA_NAME)
    vbo1->op1 = SSA_VAL (vbo1->op1);

#if 0
  /* XXX: Shouldn't set this here. */
  if (is_gimple_min_invariant (vbo1->op0)
      || is_gimple_min_invariant (vbo1->op1))
    VN_INFO (result)->has_constants = true;
#endif

  if (tree_swap_operands_p (vbo1->op0, vbo1->op1, false)
      && commutative_tree_code (vbo1->opcode))
    {
      tree temp = vbo1->op0;
      vbo1->op0 = vbo1->op1;
      vbo1->op1 = temp;
    }
  vbo1->hashcode = vn_binary_op_compute_hash (vbo1);
  slot = htab_find_slot_with_hash (current_info->binary, vbo1, vbo1->hashcode,
				   INSERT);
  gcc_assert (!*slot);

  *slot = vbo1;
}

static inline hashval_t
vn_phi_compute_hash (vn_phi_t vp1)
{
  hashval_t result = 0;
  int i;
  tree phi1op;

  result = vp1->block->index;
#if 1

  for (i = 0; VEC_iterate (tree, vp1->phiargs, i, phi1op); i++)
    {
      if (phi1op == VN_TOP)
	continue;
      result += iterative_hash_expr (phi1op, result);
    }
#endif
  return result;
}

static hashval_t
vn_phi_hash (const void *p1)
{
  const vn_phi_t vp1 = (vn_phi_t) p1;
  return vp1->hashcode;
}

/* Compare two phi entries for equality, ignoring VN_TOP arguments.  */
static int
vn_phi_eq (const void *p1, const void *p2)
{
  const vn_phi_t vp1 = (vn_phi_t) p1;
  const vn_phi_t vp2 = (vn_phi_t) p2;
#if 0
  if (vp1->orig_phi == vp2->orig_phi)
    return true;
#endif
  if (vp1->block == vp2->block)
    {
      int i;
      tree phi1op;

      for (i = 0; VEC_iterate (tree, vp1->phiargs, i, phi1op); i++)
	{
	  tree phi2op = VEC_index (tree, vp2->phiargs, i);
	  if (phi1op == VN_TOP || phi2op == VN_TOP)
	    continue;
	  if (!expressions_equal_p (phi1op, phi2op))
	    return false;
	}
      return true;
    }
  return false;
}

static VEC(tree, heap) *shared_lookup_phiargs;

/* Lookup PHI in the current hash table, and return the resulting
   value number if it exists in the hash table.  Return NULL_TREE if
   it does not exist in the hash table. */

static tree
vn_phi_lookup (tree phi)
{
  void **slot;
  struct vn_phi_s vp1;
  int i;

  VEC_truncate (tree, shared_lookup_phiargs, 0);
  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
    {
      tree def = PHI_ARG_DEF (phi, i);
      def = TREE_CODE (def) == SSA_NAME ? SSA_VAL (def) : def;
      VEC_safe_push (tree, heap, shared_lookup_phiargs, def);
    }
  vp1.phiargs = shared_lookup_phiargs;
#if 0
  vp1.orig_phi = phi;
#endif
  vp1.block = bb_for_stmt (phi);
  vp1.hashcode = vn_phi_compute_hash (&vp1);
  slot = htab_find_slot_with_hash (current_info->phis, &vp1, vp1.hashcode,
				   NO_INSERT);
  if (!slot)
    return NULL_TREE;
  return ((vn_phi_t)*slot)->result;
}

/* Insert PHI into the current hash table with a value number of
   RESULT.  */

static void
vn_phi_insert (tree phi, tree result)
{
  void **slot;
  vn_phi_t vp1 = (vn_phi_t) pool_alloc (current_info->phis_pool);
  int i;
  VEC (tree, heap) *args = NULL;

  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
    {
      tree def = PHI_ARG_DEF (phi, i);
      def = TREE_CODE (def) == SSA_NAME ? SSA_VAL (def) : def;
      VEC_safe_push (tree, heap, args, def);
    }
  vp1->phiargs = args;
#if 0
  vp1->orig_phi = phi;
#endif
  vp1->block = bb_for_stmt (phi);
  vp1->result = result;
  vp1->hashcode = vn_phi_compute_hash (vp1);

  slot = htab_find_slot_with_hash (current_info->phis, vp1, vp1->hashcode,
				   INSERT);
  /*  gcc_assert (!*slot); */
  *slot = vp1;
}


/* Print SCC to OUT. */

static void
print_scc (FILE *out, VEC (tree, heap) *scc)
{
  tree var;
  unsigned int i;

  fprintf (out, "SCC consists of: ");
  for (i = 0; VEC_iterate (tree, scc, i, var); i++)
    {
      print_generic_expr (out, var, 0);
      fprintf (out, " ");
    }
  fprintf (out, "\n");
}

/* Set the value number of FROM to TO, return true if it has changed
   as a result.  */

static inline bool
set_ssa_val_to (tree from, tree to)
{
  gcc_assert (to != NULL && is_gimple_reg (from));
  
/*   gcc_assert (TREE_CODE (to) == SSA_NAME || is_gimple_min_invariant (to));  */
  
  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Setting value number of ");
      print_generic_expr (dump_file, from, 0);
      fprintf (dump_file, " to ");
      print_generic_expr (dump_file, to, 0);
      fprintf (dump_file, "\n");
    }

  /* XXX: Should not be setting this here.  */
  if (is_gimple_min_invariant (to))
    {
      VN_INFO (from)->has_constants = true;
    }
  else
    {
      if (TREE_CODE (from) == SSA_NAME && from != to
	  && TREE_CODE (to) == SSA_NAME)
	{
	  VN_INFO (from)->has_constants = VN_INFO (to)->has_constants;
	  VN_INFO (from)->expr = VN_INFO (to)->expr;
	}
#if 0
      else
      VN_INFO (from)->has_constants = false;
#endif
    }

  if (SSA_VAL (from) != to)
    {
      SSA_VAL (from) = to;
      return true;
    }
  return false;
}

/* Set all definitions in STMT to value number to themselves.
   Return true if a value number changed. */

static bool
defs_to_varying (tree stmt)
{
  bool changed = false;
  ssa_op_iter iter;
  def_operand_p defp;

  FOR_EACH_SSA_DEF_OPERAND (defp, stmt, iter, SSA_OP_DEF)
    {
      tree def = DEF_FROM_PTR (defp);
      changed = set_ssa_val_to (def, def);
    }
  return changed;
}

/* Visit a copy between LHS and RHS, return true if the value number
   changed.  */

static bool
visit_copy (tree lhs, tree rhs)
{
  return set_ssa_val_to (lhs, rhs);
}

/* Visit a unary operator RHS, value number it, and return true if the
   value number of LHS has changed as a result.  */

static bool
visit_unary_op (tree lhs, tree op)
{
  bool changed = false;
  tree result = vn_unary_op_lookup (op);

  if (result)
    {
      changed = set_ssa_val_to (lhs, result);
    }
  else
    {
      changed = set_ssa_val_to (lhs, lhs);
      vn_unary_op_insert (op, lhs);
    }

  return changed;
}

/* Visit a binary operator RHS, value number it, and return true if the
   value number of LHS has changed as a result.  */

static bool
visit_binary_op (tree lhs, tree op)
{
  bool changed = false;
  tree result = vn_binary_op_lookup (op);

  if (result)
    {
      changed = set_ssa_val_to (lhs, result);
    }
  else
    {
      changed = set_ssa_val_to (lhs, lhs);
      vn_binary_op_insert (op, lhs);
    }

  return changed;
}

/* Visit a load from a reference operator RHS, part of STMT, value number it,
   and return true if the value number of the LHS has changed as a result.  */

static bool
visit_reference_op_load (tree lhs, tree op, tree stmt)
{
  bool changed = false;
  tree result = vn_reference_lookup (op, shared_vuses_from_stmt (stmt));

  if (result)
    {
      changed = set_ssa_val_to (lhs, result);
    }
  else
    {
      changed = set_ssa_val_to (lhs, lhs);
      vn_reference_insert (op, lhs, copy_vuses_from_stmt (stmt));
    }

  return changed;
}


/* Visit a store to a reference operator LHS, part of STMT, value number it,
   and return true if the value number of the LHS has changed as a result.  */

static bool
visit_reference_op_store (tree lhs, tree op, tree stmt)
{
  bool changed = false;
  tree result = vn_reference_lookup (lhs, shared_vdefs_from_stmt (stmt));

  if (!result)
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	{
	  fprintf (dump_file, "Value numbering store ");
	  print_generic_expr (dump_file, lhs, 0);
	  fprintf (dump_file, " to ");
	  print_generic_expr (dump_file, op, 0);
	  fprintf (dump_file, "\n");
	}
      vn_reference_insert (lhs, op, copy_vdefs_from_stmt (stmt));
    }

  return changed;
}

/* Visit and value number PHI, return true if the value number
   changed.  */

static bool
visit_phi (tree phi)
{
  bool changed = false;
  tree result;
  tree sameval = VN_TOP;
  bool allsame = true;
  int i;

  /* See if all non-TOP arguments have the same value.  If so, that is
     the value of the phi node, since TOP is equivalent to
     everything.  */
  for (i = 0; i < PHI_NUM_ARGS (phi); i++)
    {
      tree def = PHI_ARG_DEF (phi, i);

      if (TREE_CODE (def) == SSA_NAME)
	def = SSA_VAL (def);
      if (def == VN_TOP)
	continue;
      if (sameval == VN_TOP)
	{
	  sameval = def;
	}
      else
	{
	  if (!expressions_equal_p (def, sameval))
	    {
	      allsame = false;
	      break;
	    }
	}
    }

  if (allsame)
    {
      if (is_gimple_min_invariant (sameval))
	{
	  VN_INFO (PHI_RESULT (phi))->expr = sameval;
	  VN_INFO (PHI_RESULT (phi))->has_constants = true;
	}
      return set_ssa_val_to (PHI_RESULT (phi), sameval);
    }

  result = vn_phi_lookup (phi);
  if (result)
    changed = set_ssa_val_to (PHI_RESULT (phi), result);
  else
    {
      vn_phi_insert (phi, PHI_RESULT (phi));
      VN_INFO (PHI_RESULT (phi))->has_constants = false;
      VN_INFO (PHI_RESULT (phi))->expr = PHI_RESULT (phi);
      changed = set_ssa_val_to (PHI_RESULT (phi), PHI_RESULT (phi));
    }

  return changed;
}

/* Return true if EXPR contains constants.  */
static bool
expr_has_constants (tree expr)
{
  switch (TREE_CODE_CLASS (TREE_CODE (expr)))
    {
    case tcc_unary:
      return is_gimple_min_invariant (TREE_OPERAND (expr, 0));

    case tcc_binary:
      return is_gimple_min_invariant (TREE_OPERAND (expr, 0))
	|| is_gimple_min_invariant (TREE_OPERAND (expr, 1));
      /* XXX: We are such liars */
    case tcc_reference:
      return false;
    default:
      return is_gimple_min_invariant (expr);
    }
  return false;
}

/* Try to simplify RHS using equivalences, constant folding, and black
   magic. */

static tree
try_to_simplify (tree stmt, tree rhs)
{
  if (TREE_CODE (rhs) == SSA_NAME)
    {
      if (is_gimple_min_invariant (SSA_VAL (rhs)))
	return SSA_VAL (rhs);
      else if (VN_INFO (rhs)->has_constants)
	return VN_INFO (rhs)->expr;
    }
  else
    {
      switch (TREE_CODE_CLASS (TREE_CODE (rhs)))
	{
	case tcc_reference:
	  {
	    tree result = vn_reference_lookup (rhs, 
					       shared_vuses_from_stmt (stmt));
	    if (result)
	      return result;  
	  }
	  break;
	case tcc_unary:
	  {
	    tree result = NULL_TREE;
	    tree op0 = TREE_OPERAND (rhs, 0);
	    if (TREE_CODE (op0) == SSA_NAME && VN_INFO (op0)->has_constants)
	      result = fold_unary (TREE_CODE (rhs), TREE_TYPE (rhs),
				   VN_INFO (op0)->expr);
	    if (result && is_gimple_min_invariant (result))
	      return result;
	  }
	  break;
	case tcc_binary:
	  {
	    tree result = NULL_TREE;
	    tree op0 = TREE_OPERAND (rhs, 0);
	    tree op1 = TREE_OPERAND (rhs, 1);

	    if (TREE_CODE (op0) == SSA_NAME)
	      {
		if (VN_INFO (op0)->has_constants)
		  op0 = VN_INFO (op0)->expr;
		else if (VN_INFO (op0)->valnum != VN_TOP)
		  op0 = VN_INFO (op0)->valnum;
	      }

	    if (TREE_CODE (op1) == SSA_NAME)
	      {
		if (VN_INFO (op1)->has_constants)
		  op1 = VN_INFO (op1)->expr;
		else if (VN_INFO (op1)->valnum != VN_TOP)
		  op1 = VN_INFO (op1)->valnum;
	      }
	    result = fold_binary (TREE_CODE (rhs), TREE_TYPE (rhs), op0, op1);

	    /* Make sure result is not a complex expression consiting
	       of operators of operators (IE (a + b) + (a + c))
	       Otherwise, we will end up with unbounded expressions if
	       fold does anything at all.  */
	    if (result)
	      {
		if (is_gimple_min_invariant (result))
		  return result;
		if (EXPR_P (result))
		  {
		    switch (TREE_CODE_CLASS (TREE_CODE (result)))
		      {
		      case tcc_unary:
			{
			  tree op0 = TREE_OPERAND (result, 0);
			  if (!EXPR_P (op0))
			    return result;
			}
			break;
		      case tcc_binary:
			{
			  tree op0 = TREE_OPERAND (result, 0);
			  tree op1 = TREE_OPERAND (result, 1);
			  if (!EXPR_P (op0) && !EXPR_P (op1))
			    return result;
			}
			break;
		      default:
			break;
		      }
		  }
	      }
	  }
	  break;
	default:
	  break;
	}
    }
  return rhs;
}

/* Visit and value number USE, return true if the value number
   changed. */

static bool
visit_use (tree use)
{
  bool changed = false;
  tree stmt = SSA_NAME_DEF_STMT (use);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Value numbering ");
      print_generic_expr (dump_file, use, 0);
      fprintf (dump_file, "\n");
    }

  /* We don't bother trying to directly value number vuses or vdefs
     right now, only the operations the statements they are attached to
     have. This is because it is highly unlikely we can get an answer
     that is better than what aliasing has come up with anyway.

     This is the source of the is_gimple_reg checks. */
  if (IS_EMPTY_STMT (stmt))
    {
      if (is_gimple_reg (use))
	changed = set_ssa_val_to (use, use);
    }
  else
    {
      if (TREE_CODE (stmt) == PHI_NODE)
	{
	  if (is_gimple_reg (PHI_RESULT (stmt)))
	    changed = visit_phi (stmt);
	}
      else if (TREE_CODE (stmt) != GIMPLE_MODIFY_STMT
	       || stmt_ann (stmt)->has_volatile_ops)
	{
	  changed = defs_to_varying (stmt);
	}
      else
	{
	  tree lhs = GIMPLE_STMT_OPERAND (stmt, 0);
	  tree rhs = GIMPLE_STMT_OPERAND (stmt, 1);
	  tree simplified;

	  STRIP_USELESS_TYPE_CONVERSION (rhs);

	  simplified = try_to_simplify (stmt, rhs);
	  /* Setting value numbers to constants will screw up phi
	     congruence because constants are not uniquely
	     associated with a single ssa name that can be looked up.
	  */
	  if (simplified && is_gimple_min_invariant (simplified)
	      && TREE_CODE (lhs) == SSA_NAME
	      && simplified != rhs)
	    {
	      changed = set_ssa_val_to (lhs, simplified);
	      VN_INFO (lhs)->expr = simplified;
	      VN_INFO (lhs)->has_constants = true;
	      if (dump_file && (dump_flags & TDF_DETAILS))
		{
		  print_generic_expr (dump_file, lhs, 0);
		  fprintf (dump_file, " simplified to value ");
		  print_generic_expr (dump_file, simplified, 0);
		  fprintf (dump_file, "\n");
		}
	      goto done;
	    }
	  else if (simplified && TREE_CODE (simplified) == SSA_NAME
		   && TREE_CODE (lhs) == SSA_NAME)
	    {
	      changed = visit_copy (lhs, rhs);
	      goto done;
	    }
	  else if (simplified)
	    {

	      if (TREE_CODE (lhs) == SSA_NAME)
		{
		  VN_INFO (lhs)->has_constants = expr_has_constants (simplified);
		  VN_INFO (lhs)->expr = simplified;
		}
	      rhs = simplified;
	      if (dump_file && (dump_flags & TDF_DETAILS))
		{
		  fprintf (dump_file, "RHS ");
		  print_generic_expr (dump_file, rhs, 0);
		  fprintf (dump_file, " simplified to ");
		  print_generic_expr (dump_file, simplified, 0);
		  if (TREE_CODE (lhs) == SSA_NAME)
		    fprintf (dump_file, " has constants %d\n",
			     VN_INFO (lhs)->has_constants);
		  else
		    fprintf (dump_file, "\n");

		}
	    }

	  if (TREE_CODE (lhs) == SSA_NAME
	      && SSA_NAME_OCCURS_IN_ABNORMAL_PHI (lhs))
	    changed = defs_to_varying (stmt);
	  else if (TREE_CODE_CLASS (TREE_CODE (lhs)) == tcc_reference)
	    {
	      changed = visit_reference_op_store (lhs, rhs, stmt);
	    }
	  else if (TREE_CODE (lhs) == SSA_NAME)
	    {
	      if (is_gimple_min_invariant (rhs))
		changed = set_ssa_val_to (lhs, rhs);
	      else if (TREE_CODE (rhs) == SSA_NAME)
		changed = visit_copy (lhs, rhs);
	      else
		{
		  switch (TREE_CODE_CLASS (TREE_CODE (rhs)))
		    {
		    case tcc_unary:
		      changed = visit_unary_op (lhs, rhs);
		      break;
		    case tcc_binary:
		      changed = visit_binary_op (lhs, rhs);
		      break;
		    case tcc_reference:
		      changed = visit_reference_op_load (lhs, rhs, stmt);
		      break;
		    default:
		      changed = defs_to_varying (stmt);
		      break;
		    }
		}
	    }
	  else
	    changed = defs_to_varying (stmt);

	}
    }
 done:
  return changed;
}

/* Compare two operands by reverse postorder index */
static int
compare_ops (const void *pa, const void *pb)
{
  const tree opa = *((const tree *)pa);
  const tree opb = *((const tree *)pb);
  tree opstmta = SSA_NAME_DEF_STMT (opa);
  tree opstmtb = SSA_NAME_DEF_STMT (opb);
  basic_block bba;
  basic_block bbb;

  if (IS_EMPTY_STMT (opstmta) && IS_EMPTY_STMT (opstmtb))
    return 0;
  else if (IS_EMPTY_STMT (opstmta))
    return -1;
  else if (IS_EMPTY_STMT (opstmtb))
    return 1;

  bba = bb_for_stmt (opstmta);
  bbb = bb_for_stmt (opstmtb);

  if (!bba && !bbb)
    return 0;
  else if (!bba)
    return -1;
  else if (!bbb)
    return 1;
  
  if (bba == bbb)
    {
      if (TREE_CODE (opstmta) == PHI_NODE && TREE_CODE (opstmtb) == PHI_NODE)
	return 0;
      else if (TREE_CODE (opstmta) == PHI_NODE)
	return -1;
      else if (TREE_CODE (opstmtb) == PHI_NODE)
	return 1;
      return stmt_ann (opstmta)->uid - stmt_ann (opstmtb)->uid;
    }
  return rpo_numbers[bba->index] - rpo_numbers[bbb->index];
}

/* Sort an array containing members of a strongly connected component
   SCC so that the members are ordered by RPO number.
   This means that when the sort is complete, iterating through the
   array will give you the members in RPO order.  */

static void
sort_scc (VEC (tree, heap) *scc)
{
  qsort (VEC_address (tree, scc),
	 VEC_length (tree, scc),
	 sizeof (tree),
	 compare_ops);
}



/* Process a strongly connected component in the SSA graph.  */

static void
process_scc (VEC (tree, heap) *scc)
{
  /* If the SCC has a single member, just visit it.  */

  if (VEC_length (tree, scc) == 1)
    {
      visit_use (VEC_index (tree, scc, 0));
    }
  else
    {
      tree var;
      unsigned int i;
      unsigned int iterations = 0;
      bool changed = true;

      /* Iterate over the SCC with the optimistic table until it stops
	 changing.  */
      current_info = optimistic_info;
      while (changed)
	{
	  changed = false;
	  iterations++;
	  for (i = 0; VEC_iterate (tree, scc, i, var); i++)
	    changed |= visit_use (var);
	}

      if (dump_file && (dump_flags & TDF_STATS))
	fprintf (dump_file, "Processing SCC required %d iterations\n",
		 iterations);

      /* Finally, visit the SCC once using the valid table.  */
      current_info = valid_info;
      for (i = 0; VEC_iterate (tree, scc, i, var); i++)
	visit_use (var);
    }
}

/* Depth first search on NAME to discover and process SCC's in the SSA
   graph.
   Execution of this algorithm relies on the fact that the SCC's are
   popped off the stack in topological order.  */

static void
DFS (tree name)
{
  ssa_op_iter iter;
  use_operand_p usep;
  tree defstmt;

  /* SCC info */
  VN_INFO (name)->dfsnum = next_dfs_num++;
  VN_INFO (name)->visited = true;
  VN_INFO (name)->low = VN_INFO (name)->dfsnum;

  VEC_safe_push (tree, heap, sccstack, name);
  VN_INFO (name)->on_sccstack = true;
  defstmt = SSA_NAME_DEF_STMT (name);


  /* Recursively DFS on our operands, looking for SCC's.  */
  if (!IS_EMPTY_STMT (defstmt)
      && (TREE_CODE (defstmt) != PHI_NODE
	  || is_gimple_reg (PHI_RESULT (defstmt))))
    {
      FOR_EACH_PHI_OR_STMT_USE (usep, SSA_NAME_DEF_STMT (name),
				iter, SSA_OP_USE | SSA_OP_VIRTUAL_USES)
	{
	  tree use = USE_FROM_PTR (usep);

	  if (TREE_CODE (use) != SSA_NAME)
	    continue;

	  if (! (VN_INFO (use)->visited))
	    {
	      DFS (use);
	      VN_INFO (name)->low = MIN (VN_INFO (name)->low,
					 VN_INFO (use)->low);
	    }
	  if (VN_INFO (use)->dfsnum < VN_INFO (name)->dfsnum
	      && VN_INFO (use)->on_sccstack)
	    {
	      VN_INFO (name)->low = MIN (VN_INFO (use)->dfsnum,
					  VN_INFO (name)->low);
	    }
	}
    }

  /* See if we found an SCC.  */
  if (VN_INFO (name)->low == VN_INFO (name)->dfsnum)
    {
      VEC (tree, heap) *scc = NULL;
      tree x;

      /* Found an SCC, pop it off the SCC stack and process it.  */
      do
	{
	  x = VEC_pop (tree, sccstack);

	  VN_INFO (x)->on_sccstack = false;
	  VEC_safe_push (tree, heap, scc, x);
	} while (x != name);

      if (VEC_length (tree, scc) > 1)
	sort_scc (scc);
      if (dump_file && (dump_flags & TDF_DETAILS))
	print_scc (dump_file, scc);
      process_scc (scc);

      VEC_free (tree, heap, scc);
    }
}

static void
free_phi (void *vp)
{
  vn_phi_t phi = vp;
  VEC_free (tree, heap, phi->phiargs);
}


static void
free_reference (void *vp)
{
  vn_reference_t vr = vp;
  VEC_free (tree, heap, vr->vuses);
  VEC_free (vn_reference_op_s, heap, vr->operands);
}

/* Allocate a value number table.  */
static void
allocate_vn_table (vn_tables_t table)
{
  table->phis = htab_create (23, vn_phi_hash, vn_phi_eq, free_phi);
  table->unary = htab_create (23, vn_unary_op_hash, vn_unary_op_eq, NULL);
  table->binary = htab_create (23, vn_binary_op_hash, vn_binary_op_eq, NULL);
  table->references = htab_create (23, vn_reference_hash, vn_reference_eq,
				   free_reference);

  table->unary_op_pool = create_alloc_pool ("VN unary operations",
					    sizeof (struct vn_unary_op_s),
					    30);
  table->binary_op_pool = create_alloc_pool ("VN binary operations",
					     sizeof (struct vn_binary_op_s),
					     30);
  table->phis_pool = create_alloc_pool ("VN phis",
					sizeof (struct vn_phi_s),
					30);
  table->references_pool = create_alloc_pool ("VN references",
					      sizeof (struct vn_reference_s),
					      30);
}

/* Free a value number table.  */
static void
free_vn_table (vn_tables_t table)
{
  htab_delete (table->phis);
  htab_delete (table->unary);
  htab_delete (table->binary);
  htab_delete (table->references);
  free_alloc_pool (table->unary_op_pool);
  free_alloc_pool (table->binary_op_pool);
  free_alloc_pool (table->phis_pool);
  free_alloc_pool (table->references_pool);
}

static void
init_scc_vn (void)
{
  size_t i;
  int j;
  int *rpo_numbers_temp;
  basic_block bb;
  size_t id = 0;
  
  connect_infinite_loops_to_exit ();
  calculate_dominance_info (CDI_DOMINATORS);
  sccstack = NULL;
  next_dfs_num = 1;

  vn_ssa_aux_table = VEC_alloc (vn_ssa_aux_t, heap, num_ssa_names + 1);
  /* VEC_alloc doesn't actually grow it to the right size, it just
     preallocates the space to do so.  */
  VEC_safe_grow (vn_ssa_aux_t, heap, vn_ssa_aux_table, num_ssa_names + 1);
  shared_lookup_phiargs = NULL;
  shared_lookup_vuses = NULL;
  shared_lookup_references = NULL;
  rpo_numbers = XCNEWVEC (int, last_basic_block + NUM_FIXED_BLOCKS);
  rpo_numbers_temp = XCNEWVEC (int, last_basic_block + NUM_FIXED_BLOCKS);
  pre_and_rev_post_order_compute (NULL, rpo_numbers_temp, false);

  /* RPO numbers is an array of rpo ordering, rpo[i] = bb means that
     the i'th block in RPO order is bb.  We want to map bb's to RPO
     numbers, so we need to rearrange this array.  */
  for (j = 0; j < n_basic_blocks - NUM_FIXED_BLOCKS; j++)
    rpo_numbers[rpo_numbers_temp[j]] = j;

  free (rpo_numbers_temp);

  VN_TOP = create_tmp_var_raw (void_type_node, "vn_top");

  /* Create the VN_INFO structures, and initialize value numbers to
     TOP.  */
  for (i = 0; i < num_ssa_names; i++)
    {
      tree name = ssa_name (i);
      if (name)
	{
	  VN_INFO_SET (name, XCNEW (struct vn_ssa_aux));
	  VN_INFO (name)->valnum = VN_TOP;
	  VN_INFO (name)->expr = name;
	}
    }

  FOR_ALL_BB (bb)
    {
      block_stmt_iterator bsi;
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
	{
	  tree stmt = bsi_stmt (bsi);
	  stmt_ann (stmt)->uid = id++;
	}
    }
  
  /* Create the valid and optimistic value numbering tables.  */
  valid_info = XCNEW (struct vn_tables_s);
  allocate_vn_table (valid_info);
  optimistic_info = XCNEW (struct vn_tables_s);
  allocate_vn_table (optimistic_info);
}

static void
free_scc_vn (void)
{
  size_t i;

  VEC_free (tree, heap, shared_lookup_phiargs);
  VEC_free (tree, heap, shared_lookup_vuses);
  VEC_free (vn_reference_op_s, heap, shared_lookup_references);
  XDELETEVEC (rpo_numbers);
  remove_fake_exit_edges ();
  for (i = 0; i < num_ssa_names; i++)
    {
      tree name = ssa_name (i);
      if (name && is_gimple_reg (name))
	XDELETE (VN_INFO (name));
    }
  VEC_free (vn_ssa_aux_t, heap, vn_ssa_aux_table);
  VEC_free (tree, heap, sccstack);
  free_vn_table (valid_info);
  XDELETE (valid_info);
  free_vn_table (optimistic_info);
  XDELETE (optimistic_info);
}

static bool
gate_scc_vn (void)
{
  return 1;
}

/* Execute SCC value numbering and return the list of TODO flags
   needed to be set.  */
static unsigned int
execute_scc_vn (void)
{
  size_t i;
  tree param;

  init_scc_vn ();
  current_info = valid_info;

  for (param = DECL_ARGUMENTS (current_function_decl);
       param;
       param = TREE_CHAIN (param))
    {
      if (gimple_default_def (cfun, param) != NULL)
	{
	  tree def = gimple_default_def (cfun, param);
	  SSA_VAL (def) = def;
	}
    }

  for (i = 0; i < num_ssa_names; i++)
    {
      tree name = ssa_name (i);
      if (name && is_gimple_reg (name)
	  && VN_INFO (name)->visited == false)
	DFS (name);
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Value numbers:\n");
      for (i = 0; i < num_ssa_names; i++)
	{
	  tree name = ssa_name (i);
	  if (name && is_gimple_reg (name)
	      && VN_INFO (name)->visited
	      && (SSA_VAL (name) != name
		  || is_gimple_min_invariant (VN_INFO (name)->expr)))
	    {
	      print_generic_expr (dump_file, name, 0);
	      fprintf (dump_file, " = ");
	      if (is_gimple_min_invariant (VN_INFO (name)->expr))
		print_generic_expr (dump_file, VN_INFO (name)->expr, 0);
	      else
		print_generic_expr (dump_file, SSA_VAL (name), 0);
	      fprintf (dump_file, "\n");
	    }
	}
    }

  free_scc_vn ();
  return 0;
}

struct tree_opt_pass pass_scc_vn =
{
  "sccvn",				/* name */
  gate_scc_vn,				/* gate */
  execute_scc_vn,				/* execute */
  NULL,					/* sub */
  NULL,					/* next */
  0,					/* static_pass_number */
  TV_TREE_SCC_VN,				/* tv_id */
  PROP_cfg | PROP_ssa | PROP_alias,	/* properties_required */
  0,					/* properties_provided */
  0,					/* properties_destroyed */
  0,					/* todo_flags_start */
  TODO_dump_func | TODO_ggc_collect | TODO_verify_ssa, /* todo_flags_finish */
  0					/* letter */
};
