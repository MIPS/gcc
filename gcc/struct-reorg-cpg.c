#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "tree-inline.h"
#include "tree-flow.h"
#include "tree-flow-inline.h"
#include "tree-dump.h"
#include "langhooks.h"
#include "hashtab.h"
#include "toplev.h"
#include "flags.h"
#include "ggc.h"
#include "debug.h"
#include "target.h"
#include "cgraph.h"
#include "diagnostic.h"
#include "timevar.h"
#include "params.h"
#include "fibheap.h"
#include "c-common.h"
#include "intl.h"
#include "function.h"
#include "basic-block.h"
#include "tree-iterator.h"
#include "tree-pass.h"
#include "struct-reorg.h"

/* Updates CPG cell [f1,f2] with the new distance and count.  */
static void
add_cp_relation (cpg_t *cpg, int f1, int f2, gcov_type count, int dist)
{
  int i1, i2;
  struct cpg_cell *cell;

  if (count == 0)
    return;

  i1 = MIN (f1,f2);
  i2 = MAX (f1,f2);

  if (! cpg->matrix[i1 + i2 * cpg->ds->num_fields])
    {
      cell = (struct cpg_cell *) xcalloc (1, sizeof (struct cpg_cell));
      cpg->matrix[i1 + i2 * cpg->ds->num_fields] = cell;
    }
  else
    cell = cpg->matrix[i1 + i2 * cpg->ds->num_fields];
  cell->distance
    = (cell->distance * cell->count + count*dist)/(cell->count + count);
  cell->count += count;
}

/* Given a basic block we search for possibly immediate field accesses
   for each one of the outgoing arcs (on the CFG). Once an access is found
   we add a CP relation with the appropriate disntance and count.  The
   distance is the summation of all the disntances in the "empty" bbs in
   between the accesses plus the disntance at the end of the current bb
   plus the disntance at the beginning of the bb having the access in the
   other side.   */
static void
update_cpg_for_bb (cpg_t *cpg, edge e, sbitmap visited, cpg_cell_t cp, 
		   int f1_indx, struct function *f)
{
  edge succ;
  struct bb_field_access *first_acc;

  if ( e->dest == ENTRY_BLOCK_PTR_FOR_FUNCTION (f) 
       || e->dest == EXIT_BLOCK_PTR_FOR_FUNCTION (f)
       || TEST_BIT (visited, e->dest->index))
    return;

  first_acc = cpg->ds->bbs_f_acc_lists [e->dest->index];
  cp.distance += first_acc->distance_to_next;
  cp.count = MIN (e->count, cp.count);

  if (first_acc->next)
    {
      add_cp_relation (cpg, f1_indx, first_acc->next->f_indx,
                       cp.count, cp.distance);
      return;
    }

  SET_BIT (visited, e->dest->index);
  for (succ = e->dest->succ; succ; succ = succ->succ_next)
     update_cpg_for_bb (cpg, succ, visited, cp, f1_indx, f);
}

/* Create intra-block CP relations (edges).  */
static void
add_cp_relation_for_bb (cpg_t *cpg, basic_block bb)
{
  struct bb_field_access *crr = cpg->ds->bbs_f_acc_lists[bb->index];

  for (crr = crr->next; crr && crr->next; crr = crr->next)
    {
      add_cp_relation (cpg, crr->f_indx, crr->next->f_indx,
                       bb->count, crr->distance_to_next);
    }
}


/* Build the Close Proximity Graph for a given data structure.  */
void 
update_cpg_for_structure (struct data_structure *ds, struct function *f)
{
  basic_block bb;
  sbitmap visited; 
  cpg_t *cpg;

  if (!f)
    return;

  visited = sbitmap_alloc (n_basic_blocks_for_function (f));

  if (! ds->cpg)
    {
      cpg = (cpg_t *)xcalloc (1, sizeof (cpg_t));
      cpg->ds = ds;
      ds->cpg = cpg;
      cpg->matrix = (struct cpg_cell **)xcalloc (ds->num_fields
 					         * ds->num_fields, 
					         sizeof (struct cpg_cell));
    }
  else
    cpg = ds->cpg;

  /* Build the intra block close proximity relations for each
     basic block and put it in the CPG.  */
  FOR_EACH_BB_FN (bb, f)
    {
      add_cp_relation_for_bb (cpg, bb);
    }


  /* Now handle the inter-block CP relations.  */
  FOR_EACH_BB_FN (bb, f)
    {
      cpg_cell_t cp;
      edge e;
      struct bb_field_access *last = get_last_field_access (cpg->ds, bb);

      /* No fields accessed in this block.  */
      if (last->f_indx < 0)
	continue;

      sbitmap_zero (visited);
      cp.distance = last->distance_to_next;
      for (e = bb->succ; e; e = e->succ_next)
        {
          cp.count = e->count;
          update_cpg_for_bb (cpg, e, visited, cp, last->f_indx, f);
        }
    }
  sbitmap_free (visited);
}

/* Dump the Close Proximity Graph, we print the edges that connect the
   different fields.  In the following format:
	struct1 :
	  field1 <-- (dist, count) --> field2
          ...
 */
void
dump_cpg (FILE *dump_file, cpg_t *cpg)
{
  int i, j;
  bool first_time = true;
  tree struct_id = TYPE_NAME (cpg->ds->decl);
 
  if (! struct_id)
    return;

  if (TREE_CODE (struct_id) == TYPE_DECL)
    struct_id = DECL_NAME (struct_id);

  for (i = 0; i < cpg->ds->num_fields; i++)
    for (j = i; j < cpg->ds->num_fields; j++)
      {
        struct data_field_entry *f1 = &cpg->ds->fields[i];
        struct data_field_entry *f2 = &cpg->ds->fields[j];
	struct cpg_cell *cell = cpg->matrix [i + j*cpg->ds->num_fields];

	if (! cell)
          continue;
	if (first_time)
	  {
	    first_time = false;
	    if (struct_id)
	      fprintf (dump_file, "%s:\n", IDENTIFIER_POINTER (struct_id)); 
	  }
        fprintf (dump_file, "\t%s <-- (%d, ", 
		 IDENTIFIER_POINTER (DECL_NAME (f1->decl)),
		 cell->distance);
        fprintf (dump_file, HOST_WIDEST_INT_PRINT_DEC, cell->count);
        fprintf (dump_file, ") --> %s\n", 
		 IDENTIFIER_POINTER (DECL_NAME (f2->decl)));
      }
}

/* Frees the memory allocated for CPG.  */
void
free_cpg (cpg_t *cpg)
{
  free (cpg->matrix);
  free (cpg);
}

/* Stage 2 (profile based clustering):
   Given a DATA_STRUCTURE with the following data initialized properly:
   DECL, NUM_FIELDS, FIELDS, ALLOC_SITES. The FIELDS array contains entry
   for each one of the fields of the structure, a field could be an
   atomic type or a complex type. In any case it must contain all the
   fields of the data structure. When stage 1 has a complex field it can
   choose to refer to it as a complete one field or a separated fields
   and allocate entry for each one of its fields. The later case means
   that stage 2 can separate fields of that sub-structure from each other.
   An example when this is not possible is when the complex field address
   is taken - In such a case stage 1 must add one entry for the complex
   field.
   This function performs the algorithm for profile based cache aware
   data reorganization and represents its results in STRUCT_CLUSTERING
   field of the given DATA_STRUCTURE STR.  This function should
   be called for each one of the structure that stage 1 has found relevant
   for clustering or field reordering. If only field reordering is
   applicable then the parameter REORDERING_ONLY should be true, other
   wise we assume that any clustering of the structure is legal.
   If the parameter REORDERING_ONLY is true no clustering will be
   performed and only a cache aware field reordering will be made.
   return false in unexpected failure, true otherwise.

   EXAMPLE:
        struct s1 {
          struct s11
          { int f11, f12, f13} f1;
          int f2, f3,f4;
          struct s2 *f5;
        };
        struct s2 {
          int f1, f2, f3, f2;
        }

   Stage 1 can construct one of two possible arrays for FIELDS:
   {f1,f2,f3,f4,f5} or {f11,f12,f13,f2,f3,f4,f5}
   In the first case fields f11, f12, f13 will always be adjacent.
   In the later case the preferred clustering may separate them.

   CURRENT STATUS: we just build the cpg and print it to the dump file.
*/
bool
cache_aware_data_reorganization (struct data_structure *ds,
				 ATTRIBUTE_UNUSED bool reordering_only)
{
  return true;
}
