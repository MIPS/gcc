/* Predictive commoning.
   Copyright (C) 2005 Free Software Foundation, Inc.
   
This file is part of GCC.
   
GCC is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 2, or (at your option) any
later version.
   
GCC is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.
   
You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to the Free
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

/* This file implements the predictive commoning optimization.  Predictive
   commoning can be viewed as CSE around a loop, and with some improvements,
   as generalized strength reduction-- i.e., reusing values computed in
   earlier iterations of a loop in the later ones.  So far, the pass only
   handles the most useful case, that is, reusing values of memory references.
   If you think this is all just a special case of PRE, you are sort of right;
   however, concentrating on loops is simpler, and makes it possible to
   incorporate data dependence analysis to detect the opportunities, perform
   loop unrolling to avoid copies together with renaming immediately,
   and if needed, we could also take register pressure into account.

   Let us demonstrate what is done on an example:
   
   for (i = 0; i < 100; i++)
     {
       a[i+2] = a[i] + a[i+1];
       b[10] = b[10] + i;
       c[i] = c[99 - i];
       d[i] = d[i + 1];
     }

   1) We find data references in the loop, and split them to mutually
      independent groups (i.e., we find components of a data dependence
      graph).  We ignore read-read dependences whose distance is not constant.
      (TODO -- we could also ignore antidependences).  In this example, we
      find the following groups:

      a[i]{read}, a[i+1]{read}, a[i+2]{write}
      b[10]{read}, b[10]{write}
      c[99 - i]{read}, c[i]{write}
      d[i + 1]{read}, d[i]{write}

   2) Inside each of the group, we verify several conditions:
      a) all the references must differ in indices only, and the indices
	 must all have the same step
      b) the references must dominate loop latch (and thus, they must be
	 ordered by dominance relation).
      c) the distance of the indices must be a small multiple of the step
      We are then able to compute the difference of the references (# of
      iterations before they point to the same place as the first of them).
      In our example,

      a[i]{read,+0}, a[i+1]{read,-1}, a[i+2]{write,-2}
      b[10]{read,+0}, b[10]{write,+0}
      d[i + 1]{read,+0}, d[i]{write,+1}

   3) For each read, we determine the read or write whose value it reuses,
      together with the distance of this reuse.  I.e. we take the last
      reference before it with distance 0, or the last of the references
      with the smallest positive distance to the read.  Then, we remove
      the references that are not used in any of these chains, discard the
      empty groups, and propagate all the links so that they point to the
      single root reference of the chain (adjusting their distance 
      appropriately).  Some extra care needs to be taken for references with
      step 0.  In our example,

      a[i] --> (*) 2, a[i+1] --> (*) 1, a[i+2] (*)
      b[10] --> (*) 1, b[10] (*)

   4) For each root reference R, let N be maximum distance of a reference
      reusing its value.  Variables R0 upto RN are created, together
      with phi nodes that transfer values from R1 .. RN to R0 .. R(N-1).
      Initial values are loaded to R0..R(N-1) (in case not all references
      must necessarily be accessed and they may trap, we may fail here;
      TODO sometimes, the loads could be guarded by a check for the number
      of iterations).  Values loaded/stored in roots are also copied to
      RN.  Other reads are replaced with the appropriate variable Ri.
      Everything is put to SSA form.

      As a small improvement, if R0 is dead after the root (i.e., all uses of
      the value with the maximum distance dominate it), we can avoid creating
      RN and use R0 instead.

   On our example, we get (only the parts concerning a and b are shown):
   for (i = 0; i < 100; i++)
     {
       f = phi (a[0], s);
       s = phi (a[1], f);
       x = phi (b[10], x);

       f = f + s;
       a[i+2] = f;
       x = x + i;
       b[10] = x;
     }

   5) Factor F for unrolling is determined as the smallest common multiple of
      (N + 1) for each root reference (N for references for that we avoided
      creating RN).  If F and the loop is small enough, loop is unrolled F
      times.  In the original algorithm, the stores to RN (R0) in the copies
      of the loop body are periodically replaced with R0, R1, ... (R1, R2, ...),
      so that they can be coalesced and the copies can be eliminated.  We tried
      this (replacing the base variables of the ssa names), but it turned out
      to be useless:  after dom, the base variables we create are almost always
      lost and replaced with other temporaries.  Fortunately, out-of-ssa
      coalescing manages to create the code without assignments anyway.
      In our case, F = 2 and the (main loop of the) result is

      for (i = 0; i < ...; i += 2)
        {
          f = phi (a[0], f);
          s = phi (a[1], s);
          x = phi (b[10], x);

          f = f + s;
          a[i+2] = f;
          x = x + i;
          b[10] = x;

          s = s + f;
          a[i+3] = s;
          x = x + i;
          b[10] = x;
       }

   TODO -- stores killing other stores can be taken into account, e.g.,
   for (i = 0; i < n; i++)
     {
       a[i] = 1;
       a[i+2] = 2;
     }

   can be replaced with

   t0 = a[0];
   t1 = a[1];
   for (i = 0; i < n; i++)
     {
       a[i] = 1;
       t2 = 2;
       t0 = t1;
       t1 = t2;
     }
   a[n] = t0;
   a[n+1] = t1;

   The interesting part is that this would generalize store motion; still, since
   sm is performed elsewhere, it does not seem that important.

   Predictive commoning can be generalized for arbitrary computations (not
   just memory loads), and also nontrivial transfer functions (e.g., replacing
   i * i with ii_last + 2 * i + 1), to generalize strength reduction.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "tm_p.h"
#include "cfgloop.h"
#include "tree-flow.h"
#include "ggc.h"
#include "tree-data-ref.h"
#include "tree-scalar-evolution.h"
#include "tree-chrec.h"
#include "params.h"
#include "diagnostic.h"
#include "tree-pass.h"

/* The maximum number of iterations between the considered memory
   references.  */

#define MAX_DISTANCE (target_avail_regs < 16 ? 4 : 8)
   
/* Data references.  */

typedef struct dref
{
  /* The reference itself.  */
  struct data_reference *ref;

  /* The reference whose value we reuse, and distance to it.  For roots,
     distance is the maximum distance from reference using its value to
     it + 1.  */
  struct dref *chain;
  unsigned distance;

  /* For roots, the variables used to copy the value throughout iterations.  */
  VEC(tree,heap) *vars;

  /* For roots, true if it has an use with the maximal distance that comes
     after it in loop.  */
  bool has_max_use_after;

  /* Number of iterations offset from the first reference in the component.  */
  HOST_WIDE_INT offset;

  /* Number of the reference in a component, in dominance ordering.  */
  unsigned pos;
} dref;

DEF_VEC_O (dref);
DEF_VEC_ALLOC_O (dref, heap);

/* Describes the knowledge about the step of the memory references in
   the component.  */

enum ref_step_type
{
  /* The step is zero.  */
  RS_INVARIANT,

  /* The step is nonzero.  */
  RS_NONZERO,

  /* The step may or may not be nonzero.  */
  RS_ANY
};

/* Components of the data dependence graph.  */

struct component
{
  /* The references in the component.  */
  VEC(dref,heap) *refs;

  /* What we know about the step of the references in the component.  */
  enum ref_step_type comp_step;

  struct component *next;
};

/* Dumps data reference REF to FILE.  */

extern void dump_dref (FILE *, dref *);
void
dump_dref (FILE *file, dref *ref)
{
  bool is_root = (!ref->chain && ref->distance);
  unsigned i;
  tree var;

  fprintf (file, "  ");
  print_generic_expr (dump_file, DR_REF (ref->ref), TDF_SLIM);
  fprintf (file, " (id %d%s%s)\n", ref->pos,
	   DR_IS_READ (ref->ref) ? "" : ", write",
	   is_root ? ", root" : "");

  fprintf (file, "    offset %d\n", (int) ref->offset);

  if (ref->chain)
    fprintf (file, "    chain %d distance %d\n", ref->chain->pos,
	     ref->distance);

  if (is_root)
    {
      fprintf (file, "    max distance %d%s\n", (int) ref->distance,
	       ref->has_max_use_after ? "" : ", may reuse first");
      if (ref->vars)
	{
	  fprintf (file, "    vars");
	  for (i = 0; VEC_iterate (tree, ref->vars, i, var); i++)
	    {
	      fprintf (file, " ");
	      print_generic_expr (dump_file, var, TDF_SLIM);
	    }
	  fprintf (file, "\n");
	}
    }
}

/* Dumps COMP to FILE.  */

extern void dump_component (FILE *, struct component *);
void
dump_component (FILE *file, struct component *comp)
{
  dref *a;
  unsigned i;

  fprintf (file, "Component%s:\n",
	   comp->comp_step == RS_INVARIANT ? " (invariant)" : "");
  for (i = 0; VEC_iterate (dref, comp->refs, i, a); i++)
    dump_dref (file, a);
  fprintf (file, "\n");
}

/* Dumps COMPS to FILE.  */

extern void dump_components (FILE *, struct component *);
void
dump_components (FILE *file, struct component *comps)
{
  struct component *comp;

  for (comp = comps; comp; comp = comp->next)
    dump_component (file, comp);
}

/* Frees a component COMP.  */

static void
release_component (struct component *comp)
{
  unsigned i;
  dref *a;

  for (i = 0; VEC_iterate (dref, comp->refs, i, a); i++)
    if (a->vars)
      VEC_free (tree, heap, a->vars);

  VEC_free (dref, heap, comp->refs);
  free (comp);
}

/* Frees list of components COMPS.  */

static void
release_components (struct component *comps)
{
  struct component *act, *next;

  for (act = comps; act; act = next)
    {
      next = act->next;
      release_component (act);
    }
}

/* Finds a root of tree given by FATHERS containing A, and performs path
   shortening.  */

static unsigned
component_of (unsigned fathers[], unsigned a)
{
  unsigned root, n;

  for (root = a; root != fathers[root]; root = fathers[root])
    continue;

  for (; a != root; a = n)
    {
      n = fathers[a];
      fathers[a] = root;
    }

  return root;
}

/* Join operation for DFU.  FATHERS gives the tree, SIZES are sizes of the
   components, A and B are components to merge.  */

static void
merge_comps (unsigned fathers[], unsigned sizes[], unsigned a, unsigned b)
{
  unsigned ca = component_of (fathers, a);
  unsigned cb = component_of (fathers, b);

  if (ca == cb)
    return;

  if (sizes[ca] < sizes[cb])
    {
      sizes[cb] += sizes[ca];
      fathers[ca] = cb;
    }
  else
    {
      sizes[ca] += sizes[cb];
      fathers[cb] = ca;
    }
}

/* Similar to operand_equal_p, but handles the case that X and Y are NULL.  */

static bool
operand_eq_p (tree x, tree y)
{
  if (!x)
    return y == NULL_TREE;
  if (!y)
    return false;

  return operand_equal_p (x, y, 0);
}

/* The recursive part of suitable_reference_p.  Returns true if A is a reference
   that is suitable for predictive commoning in LOOP.  REF_STEP is set according to
   the step of the reference A.  */

static bool
suitable_reference_p_1 (struct loop *loop, tree a, enum ref_step_type *ref_step)
{
  enum tree_code code;
  affine_iv iv;
  enum ref_step_type sub_step = RS_INVARIANT;

  if (handled_component_p (a))
    {
      code = TREE_CODE (a);
      if (!suitable_reference_p_1 (loop, TREE_OPERAND (a, 0), &sub_step))
	return false;

      /* To be able to move the initialization of the reference from
	 the loop, the fields that we do not update must be invariant.  */
      if (TREE_CODE (a) == COMPONENT_REF
	  && !expr_invariant_in_loop_p (loop, component_ref_field_offset (a)))
	return false;
      if (TREE_CODE (a) == ARRAY_REF
	  && (!expr_invariant_in_loop_p (loop, array_ref_element_size (a))
	      || !expr_invariant_in_loop_p (loop, array_ref_low_bound (a))))
	return false;

      if (code != ARRAY_REF)
	{
	  *ref_step = sub_step;
	  return true;
	}

      a = TREE_OPERAND (a, 1);
    }
  else if (DECL_P (a))
    return true;
  else if (TREE_CODE (a) == INDIRECT_REF)
    a = TREE_OPERAND (a, 0);
  else
    return false;

  if (!simple_iv (loop, first_stmt (loop->header), a, &iv, true))
    return false;

  if (zero_p (iv.step))
    *ref_step = sub_step;
  else if (nonzero_p (iv.step))
    *ref_step = RS_NONZERO;
  else
    *ref_step = RS_ANY;

  return true;
}

/* Returns true if A is a reference that is suitable for predictive commoning
   in LOOP.  REF_STEP is set according to the step of the reference A.  */

static bool
suitable_reference_p (struct loop *loop, tree a, enum ref_step_type *ref_step)
{
  if (!is_gimple_reg_type (TREE_TYPE (a)))
    return false;

  return suitable_reference_p_1 (loop, a, ref_step);
}
/* Determines number of iterations of LOOP before B refers to exactly the
   same location as A and stores it to OFF.  If A and B do not have the same
   step, they never meet, or anything else fails, returns false, otherwise
   returns true.  Both A and B are assumed to satisfy suitable_reference_p.  */

static bool
determine_offset (struct loop *loop, tree a, tree b, HOST_WIDE_INT *off)
{
  enum tree_code code;
  affine_iv iva, ivb;
  tree type, diff, mult;
  HOST_WIDE_INT aoff;
  bool ok;

  *off = 0;

  if (TREE_TYPE (a) != TREE_TYPE (b))
    return false;

  if (handled_component_p (a))
    {
      code = TREE_CODE (a);
      if (code != TREE_CODE (b))
	return false;

      if (!determine_offset (loop, TREE_OPERAND (a, 0),
			     TREE_OPERAND (b, 0), off))
	return false;

      switch (code)
	{
	case ARRAY_RANGE_REF:
	  if (!operand_eq_p (TREE_OPERAND (a, 3), TREE_OPERAND (b, 3)))
	    return false;

	  /* Fallthru.  */

	case BIT_FIELD_REF:
	case COMPONENT_REF:
	  if (!operand_eq_p (TREE_OPERAND (a, 2), TREE_OPERAND (b, 2)))
	    return false;
	  if (!operand_eq_p (TREE_OPERAND (a, 1), TREE_OPERAND (b, 1)))
	    return false;

	  /* Fallthru.  */

	case VIEW_CONVERT_EXPR:
	case REALPART_EXPR:
	case IMAGPART_EXPR:
	  return true;

	case ARRAY_REF:
	  if (!operand_eq_p (TREE_OPERAND (a, 3), TREE_OPERAND (b, 3)))
	    return false;
	  if (!operand_eq_p (TREE_OPERAND (a, 2), TREE_OPERAND (b, 2)))
	    return false;

	  a = TREE_OPERAND (a, 1);
	  b = TREE_OPERAND (b, 1);
	  break;

	default:
	  gcc_unreachable ();
	}
    }
  else if (DECL_P (a))
    return operand_equal_p (a, b, 0);
  else if (TREE_CODE (a) == INDIRECT_REF && TREE_CODE (b) == INDIRECT_REF)
    {
      a = TREE_OPERAND (a, 0);
      b = TREE_OPERAND (b, 0);
    }
  else
    return false;

  /* Check whether a and b are indices with the same step.  */
  ok = (simple_iv (loop, first_stmt (loop->header), a, &iva, true)
	&& simple_iv (loop, first_stmt (loop->header), b, &ivb, true));
  gcc_assert (ok);

  if (!operand_eq_p (iva.step, ivb.step))
    return false;
  if (operand_equal_p (iva.base, ivb.base, 0))
    return true;
  if (zero_p (iva.step))
    return false;

  type = TREE_TYPE (iva.base);
  diff = fold_build2 (MINUS_EXPR, type,
		      iva.base, fold_convert (type, ivb.base));
  mult = constant_multiple_of (type, diff, iva.step);
  if (!mult || !cst_and_fits_in_hwi (mult))
    return false;
  aoff = int_cst_value (mult);

  if (*off != 0 && *off != aoff)
    return false;

  *off = aoff;
  return true;
}

/* Splits dependence graph on DATAREFS described by DEPENDS to components.  */

static struct component *
split_data_refs_to_components (struct loop *loop,
			       VEC (data_reference_p, heap) *datarefs,
			       VEC (ddr_p, heap) *depends)
{
  unsigned i, n = VEC_length (data_reference_p, datarefs);
  unsigned ca, ia, ib, bad;
  unsigned *comp_father = XNEWVEC (unsigned, n + 1);
  unsigned *comp_size = XNEWVEC (unsigned, n + 1);
  struct component **comps;
  struct data_reference *dr, *dra, *drb;
  struct data_dependence_relation *ddr;
  struct component *comp_list = NULL, *comp;
  dref dataref;
 
  for (i = 0; VEC_iterate (data_reference_p, datarefs, i, dr); i++)
    {
      if (!DR_REF (dr))
	{
	  /* A fake reference for call or asm_expr that may clobber memory;
	     just fail.  */
	  goto end;
	}
      dr->aux = i;
      comp_father[i] = i;
      comp_size[i] = 1;
    }

  /* A component reserved for the "bad" data references.  */
  comp_father[n] = n;
  comp_size[n] = 1;

  for (i = 0; VEC_iterate (data_reference_p, datarefs, i, dr); i++)
    {
      enum ref_step_type dummy;

      if (!suitable_reference_p (loop, DR_REF (dr), &dummy))
	{
	  ia = dr->aux;
	  merge_comps (comp_father, comp_size, n, ia);
	}
    }

  for (i = 0; VEC_iterate (ddr_p, depends, i, ddr); i++)
    {
      HOST_WIDE_INT dummy_off;

      if (DDR_ARE_DEPENDENT (ddr) == chrec_known)
	continue;

      dra = DDR_A (ddr);
      drb = DDR_B (ddr);
      ia = component_of (comp_father, dra->aux);
      ib = component_of (comp_father, drb->aux);
      if (ia == ib)
	continue;

      bad = component_of (comp_father, n);

      /* If both A and B are reads, we may ignore unsuitable dependences.  */
      if (DR_IS_READ (dra) && DR_IS_READ (drb)
	  && (ia == bad || ib == bad
	      || !determine_offset (loop, DR_REF (dra), DR_REF (drb),
				    &dummy_off)))
	continue;
	  
      merge_comps (comp_father, comp_size, ia, ib);
    }

  dataref.offset = 0;
  dataref.chain = NULL;
  dataref.distance = 0;
  dataref.has_max_use_after = false;
  dataref.vars = NULL;

  comps = XCNEWVEC (struct component *, n);
  bad = component_of (comp_father, n);
  for (i = 0; VEC_iterate (data_reference_p, datarefs, i, dr); i++)
    {
      ia = dr->aux;
      ca = component_of (comp_father, ia);
      if (ca == bad)
	continue;

      comp = comps[ca];
      if (!comp)
	{
	  comp = XCNEW (struct component);
	  comp->refs = VEC_alloc (dref, heap, comp_size[ca]);
	  comps[ca] = comp;
	}

      dataref.ref = dr;
      dataref.pos = VEC_length (dref, comp->refs);
      VEC_quick_push (dref, comp->refs, &dataref);
    }
  for (i = 0; i < n; i++)
    {
      comp = comps[i];
      if (comp)
	{
	  comp->next = comp_list;
	  comp_list = comp;
	}
    }
  free (comps);

end:
  free (comp_father);
  free (comp_size);
  return comp_list;
}

/* Returns true if the component COMP satisfies the conditions
   described in 2) at the begining of this file.  LOOP is the current
   loop.  */
      
static bool
suitable_component_p (struct loop *loop, struct component *comp)
{
  unsigned i;
  dref *a;
  basic_block ba, bp = loop->header;
  tree ref;
  bool ok, has_write = false;

  for (i = 0; VEC_iterate (dref, comp->refs, i, a); i++)
    {
      ba = bb_for_stmt (DR_STMT (a->ref));

      if (!just_once_each_iteration_p (loop, ba))
	return false;

      gcc_assert (dominated_by_p (CDI_DOMINATORS, ba, bp));
      bp = ba;

      if (!DR_IS_READ (a->ref))
	has_write = true;
    }

  a = VEC_index (dref, comp->refs, 0);
  ref = DR_REF (a->ref);
  ok = suitable_reference_p (loop, ref, &comp->comp_step);
  gcc_assert (ok);
  a->offset = 0;

  for (i = 1; VEC_iterate (dref, comp->refs, i, a); i++)
    {
      if (!determine_offset (loop, ref, DR_REF (a->ref), &a->offset))
	return false;

#ifdef ENABLE_CHECKING
      {
	enum ref_step_type a_step;
	ok = suitable_reference_p (loop, ref, &a_step);
	gcc_assert (ok && a_step == comp->comp_step);
      }
#endif
    }

  /* If there is a write inside the component, we must know whether the
     step is nonzero or not -- we would not otherwise be able to recognize
     whether the value accessed by reads comes from the OFFSET-th iteration
     or the previous one.  */
  if (has_write && comp->comp_step == RS_ANY)
    return false;

  return true;
}
      
/* Check the conditions on references inside each of components COMPS,
   and remove the unsuitable components from the list.  The new list
   of components is returned.  The conditions are described in 2) at
   the begining of this file.  LOOP is the current loop.  */

static struct component *
filter_suitable_components (struct loop *loop, struct component *comps)
{
  struct component **comp, *act;

  for (comp = &comps; *comp; )
    {
      act = *comp;
      if (suitable_component_p (loop, act))
	comp = &act->next;
      else
	{
	  *comp = act->next;
	  release_component (act);
	}
    }

  return comps;
}

/* Compares two drefs A and B by their offset and position.  Callback for
   qsort.  */

static int
order_drefs (const void *a, const void *b)
{
  const dref *da = a;
  const dref *db = b;

  if (da->offset < db->offset)
    return -1;
  if (da->offset > db->offset)
    return 1;

  return da->pos - db->pos;
}

/* Find roots of the values and determine distances in the component COMP.  */

static void
determine_roots_comp (struct component *comp)
{
  unsigned i, dist;
  dref *a, *root;

  /* Invariants are handled specially.  */
  if (comp->comp_step == RS_INVARIANT)
    return;

  qsort (VEC_address (dref, comp->refs), VEC_length (dref, comp->refs),
	 sizeof (dref), order_drefs);

  root = VEC_index (dref, comp->refs, 0);
  for (i = 1; VEC_iterate (dref, comp->refs, i, a); i++)
    {
      if (!DR_IS_READ (a->ref))
	{
	  root = a;
	  continue;
	}

      gcc_assert (root->offset <= a->offset);
      if (root->offset + MAX_DISTANCE <= a->offset)
	continue;

      dist = a->offset - root->offset;
      a->chain = root;
      a->distance = a->offset - root->offset;

      if (a->distance >= root->distance)
	{
	  root->distance = a->distance + 1;
	  root->has_max_use_after = false;
	}
      if (a->distance == root->distance - 1
	  && a->pos > root->pos)
	root->has_max_use_after = true;
    }
}

/* Find roots of the values and determine distances in components COMPS.  */

static void
determine_roots (struct component *comps)
{
  struct component *comp;

  for (comp = comps; comp; comp = comp->next)
    determine_roots_comp (comp);
}

/* Replace reference OLD in statement STMT with NEW.  If SET is true,
   NEW is instead initialized to the value of the reference in
   the statement.  */

static void
replace_ref_with (tree stmt, tree old, tree new, bool set)
{
  /* Since the reference is of gimple_reg type, it should only
     appear as lhs or rhs of modify statement.  */
  gcc_assert (TREE_CODE (stmt) == MODIFY_EXPR);

  if (set)
    {
      block_stmt_iterator bsi = bsi_for_stmt (stmt);
      tree new_stmt, rhs;

      if (TREE_OPERAND (stmt, 0) == old)
	rhs = TREE_OPERAND (stmt, 1);
      else
	{
	  gcc_assert (TREE_OPERAND (stmt, 1) == old);
	  rhs = TREE_OPERAND (stmt, 0);
	}

      new_stmt = build2 (MODIFY_EXPR, void_type_node, new, unshare_expr (rhs));
      bsi_insert_after (&bsi, new_stmt, BSI_NEW_STMT);
      SSA_NAME_DEF_STMT (new) = new_stmt;
    }
  else if (TREE_OPERAND (stmt, 1) == old)
    {
      TREE_OPERAND (stmt, 1) = new;
      update_stmt (stmt);
    }
  else
    gcc_unreachable ();
}

/* Specializes REF for the ITER-th iteration (ITER may be negative).  */

static void
ref_at_iteration (struct loop *loop, tree ref, HOST_WIDE_INT iter)
{
  tree *idx, type, val;
  affine_iv iv;
  bool ok;

  if (handled_component_p (ref))
    ref_at_iteration (loop, TREE_OPERAND (ref, 0), iter);

  if (TREE_CODE (ref) == ARRAY_REF)
    idx = &TREE_OPERAND (ref, 1);
  else if (TREE_CODE (ref) == INDIRECT_REF)
    idx = &TREE_OPERAND (ref, 0);
  else
    return;

  ok = simple_iv (loop, first_stmt (loop->header), *idx, &iv, true);
  gcc_assert (ok);
  if (zero_p (iv.step))
    {
      *idx = iv.base;
      return;
    }

  type = TREE_TYPE (iv.base);
  val = fold_build2 (MULT_EXPR, type, iv.step,
		     build_int_cst_type (type, iter));
  val = fold_build2 (PLUS_EXPR, type, iv.base, val);
  *idx = val;
}

/* Initializes variables r0 .. rN for ROOT and prepares phi nodes and
   initialization on entry to LOOP.  The ssa names for variables are
   stored to VARS.  If REUSE_FIRST is true, R0 a RN share a common variable.
   Returns false if the initialization cannot be performed because one of
   the loads might trap, false otherwise.  */

static bool
initialize_root_vars (struct loop *loop, dref *root, unsigned n,
		      bool reuse_first, VEC(tree, heap) **vars)
{
  unsigned i;
  /* N == 0 is the special case for load motion without any stores in the
     loop.  We still need to load the value in the beginning, but we do not
     need to create any phi nodes.  */
  unsigned ninit = n == 0 ? 1 : n;
  tree ref = DR_REF (root->ref), init, var, next, stmts;
  tree phi;
  edge entry = loop_preheader_edge (loop), latch = loop_latch_edge (loop);
  VEC(tree,heap) *inits = VEC_alloc (tree, heap, ninit);

  /* Find the initializers for the variables, and check that they cannot
     trap.  */
  for (i = 0; i < ninit; i++)
    {
      init = unshare_expr (ref);
      ref_at_iteration (loop, init, (int) i - n);
      if (tree_could_trap_p (init))
	{
	  VEC_free (tree, heap, inits);
	  return false;
	}
      VEC_quick_push (tree, inits, init);
    }

  *vars = VEC_alloc (tree, heap, n + 1);
  for (i = 0; i < n + (reuse_first ? 0 : 1); i++)
    {
      var = create_tmp_var (TREE_TYPE (ref), get_lsm_tmp_name (ref, i));
      add_referenced_var (var);
      VEC_quick_push (tree, *vars, var);
    }
  if (reuse_first)
    VEC_quick_push (tree, *vars, VEC_index (tree, *vars, 0));
  
  for (i = 0; VEC_iterate (tree, *vars, i, var); i++)
    VEC_replace (tree, *vars, i, make_ssa_name (var, NULL_TREE));

  if (n == 0)
    {
      /* Pure load motion, just initialize the variable.  */
      var = VEC_index (tree, *vars, 0);
      init = VEC_index (tree, inits, 0);
      init = force_gimple_operand (init, &stmts, false, NULL_TREE);
      if (stmts)
	bsi_insert_on_edge_immediate_loop (entry, stmts);

      init = build2 (MODIFY_EXPR, void_type_node, var, init);
      SSA_NAME_DEF_STMT (var) = init;
      bsi_insert_on_edge_immediate_loop (entry, init);
    }
  else
    {
      for (i = 0; i < n; i++)
	{
	  var = VEC_index (tree, *vars, i);
	  next = VEC_index (tree, *vars, i + 1);
	  init = VEC_index (tree, inits, i);

	  init = force_gimple_operand (init, &stmts, true, NULL_TREE);
	  if (stmts)
	    bsi_insert_on_edge_immediate_loop (entry, stmts);

	  phi = create_phi_node (var, loop->header);
	  SSA_NAME_DEF_STMT (var) = phi;
	  add_phi_arg (phi, init, entry);
	  add_phi_arg (phi, next, latch);
	}
    }

  VEC_free (tree, heap, inits);
  return true;
}

/* Create the variables and initialization statement for ROOT.  Return
   the suggested unroll factor.  */

static unsigned
initialize_root (struct loop *loop, dref *root)
{
  unsigned n = root->distance;

  if (!initialize_root_vars (loop, root, root->distance - 1,
			     !root->has_max_use_after, &root->vars))
    return 1;

  replace_ref_with (DR_STMT (root->ref), DR_REF (root->ref),
		    VEC_index (tree, root->vars, n - 1), true);

  return root->has_max_use_after ? n : n - 1;
}

/* Execute load motion for references in COMP.  */

static void
execute_load_motion (struct loop *loop, struct component *comp)
{
  VEC (tree, heap) *vars;
  dref *a;
  unsigned i, n_writes = 0, ridx;
  tree var;
  ssa_op_iter iter;
 
  for (i = 0; VEC_iterate (dref, comp->refs, i, a); i++)
    if (!DR_IS_READ (a->ref))
      n_writes++;

  a = VEC_index (dref, comp->refs, 0);
  if (!initialize_root_vars (loop, a, n_writes > 0 ? 1 : 0,
			     n_writes > 0, &vars))
    return;

  ridx = 0;
  for (i = 0; VEC_iterate (dref, comp->refs, i, a); i++)
    {
      FOR_EACH_SSA_TREE_OPERAND (var, DR_STMT (a->ref), iter, SSA_OP_ALL_VIRTUALS)
	mark_sym_for_renaming (SSA_NAME_VAR (var));

      if (!DR_IS_READ (a->ref))
	{
	  n_writes--;
	  if (n_writes)
	    {
	      var = VEC_index (tree, vars, 0);
	      var = make_ssa_name (SSA_NAME_VAR (var), NULL_TREE);
	      VEC_replace (tree, vars, 0, var);
	    }
	  else
	    ridx = 1;
	}
	  
      replace_ref_with (DR_STMT (a->ref), DR_REF (a->ref),
			VEC_index (tree, vars, ridx),
			!DR_IS_READ (a->ref));
    }

  VEC_free (tree, heap, vars);
}

/* Perform the predictive commoning optimization for a component COMP.
   Returns the suggested unroll factor.  */

static unsigned
execute_pred_commoning_comp (struct loop *loop, struct component *comp)
{
  unsigned i, n, factor = 1, af, nf;
  dref *a, *root;
  unsigned max = PARAM_VALUE (PARAM_MAX_UNROLL_TIMES);
  tree var;
  ssa_op_iter iter;

  for (i = 0; VEC_iterate (dref, comp->refs, i, a); i++)
    {
      root = a->chain;

      if (!root && !a->distance)
	continue;

      FOR_EACH_SSA_TREE_OPERAND (var, DR_STMT (a->ref), iter, SSA_OP_ALL_VIRTUALS)
	mark_sym_for_renaming (SSA_NAME_VAR (var));

      if (!root)
	{
	  af = initialize_root (loop, a);
      	  nf = af * factor / gcd (af, factor);
	  if (nf <= max)
	    factor = nf;
	  continue;
	}

      if (!root->vars)
	continue;

      gcc_assert (DR_IS_READ (a->ref));
      n = root->distance - 1;
      replace_ref_with (DR_STMT (a->ref), DR_REF (a->ref),
			VEC_index (tree, root->vars, n - a->distance),
			false);
    }

  return factor;
}

/* Perform the predictive commoning optimization for COMPS.  Returns
   the suggested unroll factor.  */

static unsigned
execute_pred_commoning (struct loop *loop, struct component *comps)
{
  struct component *comp;
  unsigned factor = 1, af, nfactor;
  unsigned max = PARAM_VALUE (PARAM_MAX_UNROLL_TIMES);

  for (comp = comps; comp; comp = comp->next)
    {
      if (comp->comp_step == RS_INVARIANT)
	execute_load_motion (loop, comp);
      else
	{
	  af = execute_pred_commoning_comp (loop, comp);
	  nfactor = factor * af / gcd (factor, af);
	  if (nfactor <= max)
	    factor = nfactor;
	}
    }

  return factor;
}

/* Returns true if we can and should unroll LOOP FACTOR times.  Number
   of iterations of the loop is returned in NITER.  */

static bool
should_unroll_loop_p (struct loop *loop, unsigned factor,
		      struct tree_niter_desc *niter)
{
  edge exit;

  if (factor == 1)
    return false;

  /* Check whether unrolling is possible.  We only want to unroll loops
     for that we are able to determine number of iterations.  We also
     want to split the extra iterations of the loop from its end,
     therefore we require that the loop has precisely one
     exit.  */

  exit = single_dom_exit (loop);
  if (!exit)
    return false;

  if (!number_of_iterations_exit (loop, exit, niter, false))
    return false;

  /* And of course, we must be able to duplicate the loop.  */
  if (!can_duplicate_loop_p (loop))
    return false;

  /* The final loop should be small enough.  */
  if (tree_num_loop_insns (loop) * factor
      > (unsigned) PARAM_VALUE (PARAM_MAX_UNROLLED_INSNS))
    return false;

  return true;
}

/* Performs predictive commoning for LOOP.  Returns true if LOOP was
   unrolled.  */

static bool
tree_predictive_commoning_loop (struct loops *loops, struct loop *loop)
{
  VEC (data_reference_p, heap) *datarefs;
  VEC (ddr_p, heap) *dependences;
  struct component *components;
  unsigned unroll_factor;
  struct tree_niter_desc desc;
  bool unrolled = false;

  datarefs = VEC_alloc (data_reference_p, heap, 10);
  dependences = VEC_alloc (ddr_p, heap, 10);
  compute_data_dependences_for_loop (loop, true, &datarefs, &dependences);

  components = split_data_refs_to_components (loop, datarefs, dependences);
  free_dependence_relations (dependences);
  if (!components)
    {
      free_data_refs (datarefs);
      return false;
    }

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Initial state:\n\n");
      dump_components (dump_file, components);
    }
  components = filter_suitable_components (loop, components);
  determine_roots (components);

  if (dump_file && (dump_flags & TDF_DETAILS))
    {
      fprintf (dump_file, "Before commoning:\n\n");
      dump_components (dump_file, components);
    }
  unroll_factor = execute_pred_commoning (loop, components);

  if (should_unroll_loop_p (loop, unroll_factor, &desc))
    {
      if (dump_file && (dump_flags & TDF_DETAILS))
	fprintf (dump_file, "Unrolling %u times.\n", unroll_factor);
      update_ssa (TODO_update_ssa_only_virtuals);
      tree_unroll_loop (loops, loop, unroll_factor, single_dom_exit (loop), &desc);
      unrolled = true;
    }

  release_components (components);
  free_data_refs (datarefs);
  return unrolled;
}

/* Runs predictive commoning over LOOPS.  */

void
tree_predictive_commoning (struct loops *loops)
{
  bool unrolled = false;
  struct loop *loop;
  unsigned i, n = loops->num;

  initialize_original_copy_tables ();
  for (i = 1; i < n; i++)
    {
      loop = loops->parray[i];
      if (loop && !loop->inner)
	unrolled |= tree_predictive_commoning_loop (loops, loop);
    }

  if (unrolled)
    {
      scev_reset ();
      cleanup_tree_cfg_loop ();
    }
  free_original_copy_tables ();
}
