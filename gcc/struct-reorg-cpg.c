#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "rtl.h"
#include "tree-inline.h"
#include "tree-flow.h"
#include "tree-flow-inline.h"
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
#include "struct-reorg.h"

/*
 * Field access profiling for structure reorganization
    The structure reorganization optimization is profile directed.
    Following is the part responsible for extracting the field access
    profiling information out of the basic block profiling that is
    currently available in GCC.  And the part responsible for representing
    that information in a graph called the Close Proximity Graph or CPG
    in short.
 * Close proximity graph
    The CPG is a relation graph for fields of a structure that we are
    interested in reorganizing its definition.  A node in the graph
    represents a struct field.  An edge between two nodes represents the
    close proximity relation between two fields.  The close proximity
    between two fields is the couple (count, average distance).  The count
    is the number of times one field access is followed immediately (wrt
    other fields of same struct) by an access to the other field
    (symmetrically).  The distance is the number of memory access
    instructions (not to other fields) that comes in between the two
    consecutive accesses.  Function calls should also be taken into
    account, we should distinguish between function calls that have many
    accesses to memory and others that have only small number of accesses
    to memory.  Inter procedural analysis could be used to make accurate
    decisions.  The distances are normalized by the following equation to
    produce the average:
        ____
        \
        /    distance(i) x count(i)
        ----
         n
        ----------------------------
           ____
           \
           /    count(i)
           ----
            n
*/

/* An edge in the CPG (a cell in the matrix).  */
typedef struct cpg_cell {
  gcov_type count;
  int distance;
} cpg_cell_t;

typedef struct cpg_struct {
  /* Information about the data structure of our interest.  */
  struct data_structure *ds;
  /* A matrix of edges ds->NUM_FIELDS X ds->NUM_FIELDS nodes.  */
  struct cpg_cell **cpg;
} cpg_t;

/* Updates CPG cell [f1,f2] with the new distance and count.  */
static void
add_cp_relation (cpg_t *cpg, int f1, int f2, gcov_type count, int dist)
{
  int i1, i2, d0, d1;
  gcov_type c0;

  if (count == 0)
    return;

  i1 = MIN (f1,f2);
  i2 = MAX (f1,f2);
  d0 = cpg->cpg[i1 + i2 * cpg->ds->num_fields]->distance;
  c0 = cpg->cpg[i1 + i2 * cpg->ds->num_fields]->count;
  d1 = (d0*c0 + count*dist)/(c0 + count);
  cpg->cpg[i1 + i2 * cpg->ds->num_fields]->distance = d1;
  cpg->cpg[i1 + i2 * cpg->ds->num_fields]->count += count;
}

/* Given a basic block we search for possibly immediate field accesses
   for each one of the outgoing arcs (on the CFG). Once an access is found
   we add a CP relation with the appropriate disntance and count.  The
   distance is the summation of all the disntances in the "empty" bbs in
   between the accesses plus the disntance at the end of the current bb
   plus the disntance at the beginning of the bb having the access in the
   other side.   */
static void
build_cpg_for_bb (cpg_t *cpg, edge e, sbitmap visited,
                  cpg_cell_t cp, int f1_indx)
{
  edge pred;
  struct bb_field_access *first_acc;

  if ( TEST_BIT (visited, e->dest->index))
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
  for (pred = e->dest->pred; pred; pred = pred->pred_next)
     build_cpg_for_bb (cpg, pred, visited, cp, f1_indx);
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
static cpg_t *
build_cpg_for_structure (struct data_structure *ds)
{
  cpg_t *cpg = (cpg_t *)xcalloc (1, sizeof (cpg_t));
  basic_block bb;
  sbitmap visited = sbitmap_alloc (last_basic_block);

  cpg->ds = ds;

  /* Build the intra block close proximity relations for each
     basic block and put it in the CPG.  */
  FOR_EACH_BB (bb)
    {
      add_cp_relation_for_bb (cpg, bb);
    }


  /* Now handle the inter-block CP relations.  */
  FOR_EACH_BB (bb)
    {
      cpg_cell_t cp;
      edge e;
      struct bb_field_access *last = get_last_field_access (cpg->ds, bb);

      sbitmap_zero (visited);
      cp.distance = last->distance_to_next;
      for (e = bb->succ; e; e = e->succ_next)
        {
          cp.count = e->count;
          build_cpg_for_bb (cpg, e, visited, cp, last->f_indx);
        }
    }
  return cpg;
}

bool
cache_aware_data_reorganization (struct data_structure *ds, 
				 ATTRIBUTE_UNUSED bool reordering_only)
{
  build_cpg_for_structure (ds);
  return true;
}
