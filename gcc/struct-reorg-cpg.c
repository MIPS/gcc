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
#include "params.h"
#include "intl.h"
#include "function.h"
#include "basic-block.h"
#include "tree-iterator.h"
#include "tree-pass.h"
#include "struct-reorg.h"
#include "math.h"

/* Different algorithm for calculating the CPG from the BB 
   field accesses.  */
/* An accurate and compile time efficient for the current 
   profiling information.  */
#define USE_COMPACT_FIELD_ACCESS_GRAPH 1
/* A gready non accurate calculation, but with a very small 
   compile time impact.  */
#define CPG_WITH_ZERO_DEPTH_FIELD_ACCESS 0
/* This is an addition to the compact field access graph 
   algorithm which makes the calculation further efficient 
   in case of loops, but still not fully functional.  */
#define USE_SPLIT_PRED_EDGES_IN_FACCG 0


/* This data structure holds the compact field accesses graph.  */
typedef struct faccg_node faccg_node_t;
typedef struct faccg_arc faccg_arc_t;

struct faccg_arc {
  int distance;
  gcov_type count;
  faccg_node_t *src, *dest;
  faccg_arc_t *pred_next, *succ_next;
  faccg_arc_t *collapsed_succs;
};


/* Index the FACCG nodes, for uniquenses in vcg dump.  */
static int facc_node_index = 0;

struct faccg_node {
  /* The accessed field index (-1 if doesn't exits).  */
  int f_index;
  /* The basic block index where the access belongs.  */
  int bb_index;
  int visited;
  int index;
  /* the list of predecessors/successors.  */
  faccg_arc_t *preds, *succs;
  faccg_node_t *next, *prev;
} ;

static void reorder_fields_of_struct (struct data_structure *);
static void split_data_structure (struct data_structure *ds);

/* Updates CPG cell [f1,f2] with the new distance and count.  */
static void
add_cp_relation (cpg_t *cpg, int f1, int f2, gcov_type count, int dist)
{
  int i1, i2;
  struct cpg_cell *cell;

  if (count == 0 || dist > STRUCT_REORG_DISTANCE_THRESHOLD)
    return;

  if (f1 < 0 || f2 < 0)
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

/* Allocates and initializes a BB_FIELD_ACCESS element.  */
static struct bb_field_access *
create_bb_field_access (int f_index, int bb_index, int distance,
			gcov_type count)
{
  struct bb_field_access *new
    = (struct bb_field_access *)
      xcalloc (1, sizeof (struct bb_field_access));

  new->f_index = f_index;
  new->distance_to_next = distance;
  new->count = count;
  new->bb_index = bb_index;
  return new;
}

/* Allocates memory for a new field access graph node,
   and initializes it.  */
static faccg_node_t *
create_facc_node (faccg_node_t **head_p, int f_index, int dist, 
		  int bb_index)
{
  faccg_node_t *head = *head_p;

  faccg_node_t *facc_node = xcalloc (1, sizeof (faccg_node_t));
  facc_node->f_index = f_index;
  facc_node->bb_index = bb_index;
  facc_node->succs = (faccg_arc_t *)xcalloc (1, sizeof (faccg_arc_t));
  facc_node->succs->distance = dist;
  facc_node->succs->src = facc_node;
  facc_node->next = head;
  if (head)
    head->prev = facc_node;
  *head_p = facc_node;
  return facc_node;
}

static faccg_node_t *
copy_faccg_node (faccg_node_t **head_p, faccg_node_t *node)
{
  faccg_node_t *head = *head_p;

  faccg_node_t *facc_node = xcalloc (1, sizeof (faccg_node_t));
  facc_node->f_index = node->f_index;
  facc_node->bb_index = node->bb_index;
  facc_node->next = head;
  head->prev = facc_node;
  *head_p = head;
  return facc_node;
}

static void 
remove_facc_node (faccg_node_t **head_p, faccg_node_t *node)
{
  faccg_node_t *head = *head_p; 

  if (node->prev)
    node->prev->next = node->next;
  if (node->next)
    node->next->prev = node->prev;
  if (head == node)
    *head_p = node->next;
  free (node);
}

/* Create an arc going out of SRC into DEST with COUNT and DISTANCE.  */
static faccg_arc_t *
create_facc_arc (faccg_node_t *src, faccg_node_t *dest, gcov_type count,
		 int distance)
{
  faccg_arc_t *arc;

  if (src->succs && ! src->succs->dest)
    arc = src->succs;
  else
    {
      arc = (faccg_arc_t *)xcalloc (1, sizeof (faccg_arc_t));
      arc->succ_next = src->succs;
      src->succs = arc;
    }
  arc->src = src;
  arc->count = count;
  arc->distance = distance;
  arc->dest = dest;
  arc->pred_next = dest->preds;
  dest->preds = arc;

  return arc;
}

/* Remove ARC from its source successors and its destination predecessors.  */
static void
remove_faccg_arc (faccg_arc_t *arc, bool from_src, bool from_dest)
{
  faccg_arc_t *crr;
  faccg_arc_t *prev = NULL;

  /* Remove ARC from its source successors.  */
  if (from_src && arc->src)
    {
      for (crr = arc->src->succs; crr && crr != arc; crr = crr->succ_next)
	prev = crr;

      if (! crr)
	abort ();
      if (prev)
	prev->succ_next = crr->succ_next;
      else
	arc->src->succs = crr->succ_next;
    }

  /* Remove ARC from its destination predecessors.  */
  if (from_dest && arc->dest)
    {
      prev = NULL;
      for (crr = arc->dest->preds; crr && crr != arc; crr = crr->pred_next)
	prev = crr;

      if (! crr)
	abort ();
      if (prev)
	prev->pred_next = crr->pred_next;
      else
	arc->dest->preds = crr->pred_next;
    }
}


/* Change the source of the ARC to be SRC and update its count and
   distance.  */
static void
redirect_arc_src (faccg_arc_t *arc, faccg_node_t *src, gcov_type count,
		   int dist)
{
  /* Remove the arc from its current pred list of its current src*/
  remove_faccg_arc (arc, true, false);

  arc->src = src;
  arc->succ_next = src->succs;
  src->succs = arc;
  arc->count = count;
  arc->distance = dist;
}

#if USE_SPLIT_PRED_EDGES_IN_FACCG
static void
redirect_arc_dest (faccg_arc_t *arc, faccg_node_t *dest)
{
  remove_faccg_arc (arc, false, true);
  arc->dest = dest;
  arc->pred_next = dest->preds;
  dest->preds = arc;
}

/* Returns true if there is a successor node for NODE.  */
static bool
successors_exist_p (faccg_node_t *node)
{
  faccg_arc_t *crr;

  for (crr = node->succs; crr; crr = crr->succ_next)
    if (crr->dest)
      return true;

  return false;
}
#endif /* USE_SPLIT_PRED_EDGES_IN_FACCG.  */

/* Add DEST to the collapsed successors of ARC.  */
static void
add_to_collapsed_succs (faccg_node_t **head_p, faccg_node_t *dest, 
			faccg_arc_t *arc, gcov_type count, int dist)
{
  faccg_arc_t *new_arc = (faccg_arc_t *)xcalloc (1, sizeof (faccg_arc_t));

  new_arc->dest = copy_faccg_node (head_p, dest);
  new_arc->count = count;
  new_arc->distance = dist;
  new_arc->succ_next = arc->collapsed_succs;
  arc->collapsed_succs = new_arc;
}

static void
add_cp_relation_for_collapsed_succs (cpg_t *cpg, faccg_arc_t *arc, 
				     int index)
{
  faccg_arc_t *crr;

  for (crr = arc->collapsed_succs; crr; crr = crr->succ_next)
    add_cp_relation (cpg, index, crr->dest->f_index, 
		     MIN (arc->count, crr->count), 
		     arc->distance + crr->distance);
 
}

/* ARC summarizes all the accessed field in the loop in its 
   COLLAPSED_SUCCS list.  This function adds CP_RELATION for 
   each couple of dields accessed inside the loop.  */
static void
update_cpg_for_faccg_loop (cpg_t *cpg, faccg_arc_t *arc) 
{
  faccg_arc_t *crr;

  if (arc->src != arc->dest)
    abort ();

  add_cp_relation (cpg, arc->src->f_index, arc->dest->f_index, 
		   arc->count, arc->distance);
  add_cp_relation_for_collapsed_succs (cpg, arc, arc->src->f_index);
  for (crr = arc->collapsed_succs; crr; crr = crr->succ_next)
     add_cp_relation_for_collapsed_succs (cpg, arc, crr->dest->f_index);
}

#if USE_SPLIT_PRED_EDGES_IN_FACCG
/* Foreach predecessor edge E of ARC->dest do the following: 
     1) make a new copy of ARC->dest  
     2) redirect E dest to the new copy
     3) Add a successor edge to the new copy for each successor
        of ARC->dest.  */
static void 
split_dest_predecessors (faccg_node_t **head_p, faccg_arc_t *arc)
{
  faccg_arc_t *crr, *crr2;
  faccg_node_t *new_dest, *dest = arc->dest;

  for (crr = dest->preds; crr; crr = crr->pred_next)
    {
      if (arc == crr)
	continue;

      new_dest = copy_faccg_node (head_p, dest); 
      redirect_arc_dest (crr, new_dest);
      for (crr2 = dest->succs; crr2; crr2 = crr2->succ_next)
	if (crr2->dest)
          create_facc_arc (new_dest, crr2->dest, crr2->count, crr2->distance);
    }
}
#endif /* USE_SPLIT_PRED_EDGES_IN_FACCG.  */

/* Knowing that ARC->dest->f_index == -1, and that ARC->dest has no other
   predecessors than ARC->src, we make all the successors of ARC->dest
   to be successors of ARC->src, remove ARC and ARC->dest.  In case of
   sequential successor the disntaces are accumulated and a minimum is
   taken on the counts.  In case of parallel successor the counts are
   accumulated and a wieghted average is taken on the distances.  */
static void
collapse_successor (faccg_node_t **head_p, faccg_arc_t *arc, cpg_t *cpg)
{
  faccg_arc_t *succ, *succ_next, *c_arc;
  faccg_node_t *dest = arc->dest;
  faccg_node_t *src = arc->src;
      

  /* Update the CPG with the ARC and add ARC->dest to the collapsed 
     successors of ARC so we can update them later with relation to 
     ARC->src predecessors.  */
  if (dest->f_index >= 0) 
    {
      /* Add CP relation between the collapsed successors of ARC and ARC->dest.  */
      for (c_arc = arc->collapsed_succs; c_arc; c_arc = c_arc->succ_next)
	add_cp_relation (cpg, dest->f_index, c_arc->dest->f_index, c_arc->count, c_arc->distance);
      if (src->f_index >= 0)
	add_cp_relation (cpg, src->f_index, dest->f_index, arc->count, arc->distance);
      add_to_collapsed_succs (head_p, dest, arc, arc->count, arc->distance);
    }
  
  /* Do not process sucessors of dest in the following two cases.  */
  if (! dest->succs || dest == src)
    {
      remove_faccg_arc (arc, false, true);
      arc->dest = NULL;
      if (! dest->preds)
	remove_facc_node (head_p, dest);
      return;
    }

  /* Update the edges from ARC->src to ARC->dest successors.  */
  for (succ = dest->succs; succ; succ = succ_next)
    {
      succ_next = succ->succ_next;

      /* Add CP_RELATION between SRC and the collapsed successors of SUCC.  
	 and update their distance/count fields.  */
      for (c_arc = succ->collapsed_succs; c_arc; c_arc = c_arc->succ_next)
	{
	  c_arc->count = MIN (c_arc->count, arc->count);
	  c_arc->distance += arc->distance;
	  if (arc->src->f_index >= 0)
	    add_cp_relation (cpg, arc->src->f_index, c_arc->dest->f_index, 
			     c_arc->count, c_arc->distance); 
	}

      /* Add the collapsed successors of ARC to SUCC.  */
      for ( c_arc = arc->collapsed_succs; c_arc; c_arc = c_arc->succ_next)
        add_to_collapsed_succs (head_p, c_arc->dest, succ, c_arc->count, 
				c_arc->distance);
      /* Make the arc to be from the ARC source to the DEST successor.  
	 This is the case of serial arcs thus: 
	 new count = minimum (count1, count2)
	 new distance = distance1 + distance2  */
      redirect_arc_src (succ, arc->src, MIN (arc->count, succ->count),
			arc->distance + succ->distance);
    }
  remove_faccg_arc (arc, true, true);
  free (arc);
  if (! dest->preds)
    remove_facc_node (head_p, dest);
}

/* Returns true if we can collapse the successor of arc.  */
static bool
can_collapse_successors_p (faccg_arc_t *arc)
{
  faccg_arc_t *crr;

  for (crr = arc->dest->preds; crr; crr = crr->pred_next)
    if (crr->src != arc->src)
      return false;

  return true;
}

/* A kind of BFS traversal of the field access graph, to compact it
   by replacing a sub-grapg of nodes that its f_index is -1 to one
   keeping the incoming and outgoing arcs but with updates count and
   distance according to collapse_successor.  */
static bool 
compact_faccg (faccg_node_t *root, faccg_node_t **head_p, cpg_t *cpg)
{
  bool change;
  faccg_arc_t *crr, *succ_next;

  if (root->visited)
    return false;

  root->visited = 1;

  do
    {
      change = false;
      for (crr = root->succs; crr; crr = succ_next)
	{
	  succ_next = crr->succ_next;
	  if (! crr->dest)
	    continue;
	  else if (crr->src == crr->dest)
	    {
	      update_cpg_for_faccg_loop (cpg, crr);
	      remove_faccg_arc (crr, false, true);
	      crr->dest = NULL;
	      change = true;
	    }
	  else if (crr->dest->preds 
		   && can_collapse_successors_p (crr))
	    {
	      collapse_successor (head_p, crr, cpg);
	      change = true;
            }
	}
      for (crr = root->succs; crr; crr = crr->succ_next)
	if (crr->dest && ! crr->dest->visited)
	  change |= compact_faccg (crr->dest, head_p, cpg);

      /* Try to split successors that have mupltiple predecessors
	 to be able to collapse them.  */
      if (! change)
	for (crr = root->succs; crr; crr = crr->succ_next)
	  if (crr->dest && crr->dest->preds 
	      && crr->dest->preds->pred_next)
	    {
#if USE_SPLIT_PRED_EDGES_IN_FACCG
	      split_dest_predecessors (head_p, crr);
	      change = true;
#endif /* USE_SPLIT_PRED_EDGES_IN_FACCG.  */
	    }
	  
    } while (change 
#if USE_SPLIT_PRED_EDGES_IN_FACCG
	     || successors_exist_p (root)
#endif /* USE_SPLIT_PRED_EDGES_IN_FACCG.  */
	    );
  return change;
}

static void
dump_faccg_node (FILE *dump_file, faccg_node_t *node, struct data_structure *ds)
{
  const char *f_name;
  faccg_arc_t *arc;

  if (node->f_index >= 0)
    f_name = IDENTIFIER_POINTER (DECL_NAME (ds->fields[node->f_index].decl));
  else
    f_name = "";

  fprintf (dump_file, "node: {title: \"%s_%d\" }\n", f_name, node->index);

  for (arc = node->succs; arc; arc = arc->succ_next)
    {
      const char *dest_name;

      if (! arc->dest)
	continue;

      if (arc->dest->f_index >= 0)
	dest_name = IDENTIFIER_POINTER (DECL_NAME (ds->fields[arc->dest->f_index].decl));
      else
	dest_name = "";

      fprintf (dump_file, "edge: { ");
      fprintf (dump_file, "sourcename: \"%s_%d\" ", f_name, node->index);
      fprintf (dump_file, "targetname: \"%s_%d\" ", dest_name, arc->dest->index);
      fprintf (dump_file, "label: \"%d_", arc->distance);
      fprintf (dump_file, HOST_WIDEST_INT_PRINT_DEC, arc->count);
      fprintf (dump_file, "\"}\n");
    }
}

static void 
dump_faccg (FILE *dump_file, faccg_node_t *head, struct data_structure *ds)
{
  faccg_node_t *node;

  for (node = head; node; node = node->next)
    dump_faccg_node (dump_file, node, ds);
}

static void
free_faccg (faccg_node_t *head)
{
  faccg_node_t *node, *node_next;
  faccg_arc_t *arc, *c_arc, *arc_next;

  for (node = head; node; node = node->next)
    {
      for (arc = node->succs; arc; arc = arc_next)
	{
	  while (arc->collapsed_succs)
	    {
	      c_arc = arc->collapsed_succs;
	      arc->collapsed_succs = c_arc->succ_next;
	      free (c_arc);
	    }
	  arc_next = arc->succ_next; 
	  remove_faccg_arc (arc, true, true);
	  free (arc);
	}
    }
  for (node = head; node; node = node_next)
    {
      node_next = node->next;
      free (node);
    }
}
/* Given the CFG of a function and the fields accesses in its BBs,
   it builds a compacted field access graph that represents the control
   flow of accesses to the fields of DS.  It traverses the CFG of F from
   the entry block in a BFS traversal.  When the BB has accesses to fields
   a node is created for each accesses and connected in the graph as it is
   in the accesses list of the block.  the first and last accesses are
   connected to the predecessors and successors respectively.  If a BB
   doesn't have field accesses its distance is added to its predecessors
   and edge with its count is aconsidered in calculating the minimum count
   of the appropriate outgoing edges of its the predecessor.  */
static faccg_node_t *
build_compact_field_access_graph (FILE *dump_file, struct data_structure *ds,
				  struct function *f, faccg_node_t **head_p)
{
  faccg_node_t **bb_faccg_nodes_first, **bb_faccg_nodes_last, 
	       *root, *crr;
  edge e;
  edge_iterator ei;
  basic_block bb;

  bb_faccg_nodes_first = xcalloc (n_basic_blocks_for_function(f),
				  sizeof (faccg_node_t *));
  bb_faccg_nodes_last = xcalloc (n_basic_blocks_for_function(f),
				 sizeof (faccg_node_t *));

  /* First we build the graph to contain also nodes with no accesses
     then we traverse it to compact it by removing these nodes.  */
  FOR_EACH_BB_FN(bb, f)
    {
      faccg_node_t *last, *first;
      struct bb_field_access *crr = ds->bbs_f_acc_lists[bb->index];

      if (! crr)
        abort ();

      first = create_facc_node (head_p, crr->f_index, 
				crr->distance_to_next, bb->index);
      bb_faccg_nodes_first[bb->index] = first;
      for (last = first, crr = crr->next; crr; crr = crr->next)
	{
          faccg_node_t *new = create_facc_node (head_p, crr->f_index, 
						crr->distance_to_next, 
						bb->index);

	  create_facc_arc (last, new, bb->count, last->succs->distance); 
	  last = new;
	}
      bb_faccg_nodes_last[bb->index] = last;
      FOR_EACH_EDGE (e, ei, bb->succs)
        if (e->dest != EXIT_BLOCK_PTR_FOR_FUNCTION (f)
	    && bb_faccg_nodes_first [e->dest->index])
            create_facc_arc (last, bb_faccg_nodes_first [e->dest->index],
			     e->count, last->succs->distance);
      FOR_EACH_EDGE (e, ei, bb->preds)
        {
          if (e->src != ENTRY_BLOCK_PTR_FOR_FUNCTION (f)
              && bb_faccg_nodes_last [e->src->index])
	    {
              faccg_node_t *last2 = bb_faccg_nodes_last[e->src->index];
	      create_facc_arc (last2, first, e->count,
			       last2->succs->distance);
	    }

        }

    }

  /* Add the root of the graph, this is done by connected the root to
     all the successors of the entry basic block field access nodes.  */
  root = (faccg_node_t *)xcalloc (1, sizeof (faccg_node_t));
  root->bb_index = ENTRY_BLOCK_PTR_FOR_FUNCTION (f)->index;
  root->f_index = -1;
  FOR_EACH_EDGE (e, ei, ENTRY_BLOCK_PTR_FOR_FUNCTION (f)->succs) 
    {
      if (! bb_faccg_nodes_first[e->dest->index])
        abort ();
      create_facc_arc (root, bb_faccg_nodes_first[e->dest->index],
                       e->count, 0);
    }

  free (bb_faccg_nodes_first);
  free (bb_faccg_nodes_last);

  for (crr = *head_p; crr; crr = crr->next)
    crr->index = facc_node_index++;

  /* Now compact the field access graph.  We look for a sub-graph in
     which all the nodes has f_index of -1.  */
  compact_faccg (root, head_p, ds->cpg);
  /* Clear the visited field of the nodes.  */
  for (crr = *head_p; crr; crr = crr->next)
    crr->visited = 0;
  if (dump_file)
    dump_faccg (dump_file, *head_p, ds);
  return root;
}


/* Add CP relation between NEXT and all the fields in LAF (Lately Accessed
   Fields) until distance passes the threshold.  */
static void
update_cpg_for_lately_accessed_fields (cpg_t *cpg,
				       struct bb_field_access *laf_start,
				       struct bb_field_access *laf_end,
				       int f_index, gcov_type count)
{
  struct bb_field_access *laf;
  int distance_to_latest = 0;

  for (laf = laf_start; laf != laf_end; laf = laf->prev)
    {
      distance_to_latest += laf->distance_to_next;
      /* We want the minimum count over the patch between
	 the two accesses.  */
      if (count > laf->count)
	count = laf->count;
      if (distance_to_latest > STRUCT_REORG_DISTANCE_THRESHOLD)
	break;
      add_cp_relation (cpg, laf->f_index, f_index,
           	       count, distance_to_latest);
    }
}

/* Given that BB_INDEX is the basic block index that closes a loop
   while traversing the CFG, add CP relations for all the lately
   accessed fields that came after that block was traversed.  */
static void
update_cpg_for_loop (cpg_t *cpg, struct bb_field_access *laf,
		     int  bb_index)
{
  bool last_bb_found = false;
  struct bb_field_access *crr, *laf_last;

  for (crr = laf; crr; crr = crr->prev)
    {
      if (last_bb_found && laf->bb_index != bb_index)
	break;
      if (laf->bb_index == bb_index)
	last_bb_found = true;
    }
  laf_last = crr;
  for (crr = laf; crr != laf_last; crr = crr->prev)
   update_cpg_for_lately_accessed_fields (cpg, crr, laf_last, crr->f_index,
					  crr->count);
}

#if CPG_WITH_ZERO_DEPTH_FIELD_ACCESS
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
  edge_iterator ei;
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
  FOR_EACH_EDGE (succ, ei, e->dest->succs)
     update_cpg_for_bb (cpg, succ, visited, cp, f1_indx, f);

}
#endif

#if USE_COMPACT_FIELD_ACCESS_GRAPH
/* Traverse the field access graph to and update the CPG.  */
static void
update_cpg_from_faccg (cpg_t *cpg, faccg_node_t *root,
		       struct bb_field_access *lately_accessed_fields,
		       struct function *f)
{
  faccg_arc_t *arc;
  basic_block bb;  
  struct bb_field_access *laf;
  
  if (root->bb_index == ENTRY_BLOCK_PTR_FOR_FUNCTION (f)->index)
    bb = ENTRY_BLOCK_PTR_FOR_FUNCTION (f);
  else if (root->bb_index == EXIT_BLOCK_PTR_FOR_FUNCTION (f)->index)
    bb = EXIT_BLOCK_PTR_FOR_FUNCTION (f);
  else 
    bb = BASIC_BLOCK_FOR_FUNCTION(f, root->bb_index);

  if (root->visited)
    {
      update_cpg_for_loop (cpg, lately_accessed_fields, bb->index);
      return;
    }
  
  root->visited = 1;
  laf = create_bb_field_access (root->f_index, bb->index, 0, bb->count);

  if (lately_accessed_fields && laf->f_index >= 0)
    update_cpg_for_lately_accessed_fields (cpg, laf, NULL, laf->f_index,
                                           bb->count);
  if (lately_accessed_fields)
    lately_accessed_fields->next = laf;
  laf->prev = lately_accessed_fields;

  for (arc = root->succs; arc; arc = arc->succ_next)
    {
      laf->count = arc->count;
      laf->distance_to_next = arc->distance;
      update_cpg_from_faccg (cpg, arc->dest, laf, f);
    }

  root->visited = 0;
  if (lately_accessed_fields)
    lately_accessed_fields->next = NULL;
  free (laf);
}
#else
/* Go over the fields accesses inside the block BB (by traversing
   BBS_F_ACC_LISTS) of the given block, add CP relations to the given CPG,
   and return an updated list of lately accessed fields (creates a new one
   if its empty). The firs element of the list is the latest accessed one.
   This function also removes fields from the lately accessed list if
   their distance goes above the distance threshold.  */
static void
update_cpg_for_bb (cpg_t *cpg, basic_block bb, sbitmap visited,
		   struct bb_field_access *lately_accessed_fields,
		   struct function *f)
{
  int static count = 0;
  struct bb_field_access *crr, *laf;
  edge e;
  edge_iterator ei;

  if ( bb == ENTRY_BLOCK_PTR_FOR_FUNCTION (f)
       || bb == EXIT_BLOCK_PTR_FOR_FUNCTION (f))
    return;

  if (TEST_BIT (visited, bb->index))
    {
      update_cpg_for_loop (cpg, lately_accessed_fields, bb->index);
      return;
    }
  SET_BIT (visited, bb->index);
  laf = lately_accessed_fields;

  for (crr = cpg->ds->bbs_f_acc_lists[bb->index]; crr; crr = crr->next)
    {
      new = create_bb_field_access (crr->f_index, bb->index,
				    crr->distance_to_next, bb->count);
      if (! laf)
	{
	  laf = new;
	  continue;
	}

      if (new->f_index >= 0)
        update_cpg_for_lately_accessed_fields (cpg, laf, NULL, new->f_index,
					       bb->count);

      new->prev = laf;
      laf->next = new;
      laf = new;
    } 
  FOR_EACH_EDGE (e, ei, bb->succs)
    {
      for (crr = laf; crr != lately_accessed_fields; crr = crr->prev)
	crr->count = e->count;
      update_cpg_for_bb (cpg, e->dest, visited, laf, f);
    }
  RESET_BIT (visited, bb->index);

  /* Remove the access of the block from the list of lately accessed
     fields.  */
  for (crr = laf; crr != lately_accessed_fields; )
    {
      laf = crr->prev;
      free (crr);
      if (laf)
	laf->next = NULL;
      crr = laf;
    }
}
#endif

/* Build the Close Proximity Graph for a given data structure.  */
void
update_cpg_for_structure (FILE *dump_file, struct data_structure *ds, 
			  struct function *f)
{
#if USE_COMPACT_FIELD_ACCESS_GRAPH
  faccg_node_t *root, *head = NULL;
#else
  edge e;
  sbitmap visited;
  edge_iterator ei;
#endif
  cpg_t *cpg;
  

  if (!f)
    return;

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
#if USE_COMPACT_FIELD_ACCESS_GRAPH
  root = build_compact_field_access_graph (dump_file, ds, f, &head),
  update_cpg_from_faccg (cpg, root, NULL, f);
  free_faccg (head);
 
#else
  visited = sbitmap_alloc (n_basic_blocks_for_function (f));
  sbitmap_zero (visited);

  FOR_EACH_EDGE (e, ei, ENTRY_BLOCK_PTR_FOR_FUNCTION (f)->succs)
    {
      update_cpg_for_bb (cpg, e->dest, visited, NULL, f);
    }
  sbitmap_free (visited);
#endif
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
cache_aware_data_reorganization (ATTRIBUTE_UNUSED struct data_structure *ds,
				 ATTRIBUTE_UNUSED bool reordering_only)
{
  sbitmap ones = sbitmap_alloc (ds->num_fields);

  sbitmap_ones (ones);
  if (! reordering_only)
    split_data_structure (ds);
  if (! ds->struct_clustering 
      || sbitmap_a_and_b_cg (ones, ones, 
			     ds->struct_clustering->fields_in_cluster))
    reorder_fields_of_struct (ds);
  return true;
}


/* Following code implements field reordering algorithm based on the
   WCP heuristic.  */

static HOST_WIDE_INT
field_size_in_bytes (tree decl)
{
  HOST_WIDE_INT f_size = int_size_in_bytes (TREE_TYPE (decl));

  /* We shouldn't be handling this structure if its fields doesn't
     have a known size at compile time.  */
  if (f_size < 0)
    abort ();
  return f_size;
}

struct field_order {
  int f_index;
  struct field_order *next, *prev;
};

static gcov_type 
cp_relation (struct data_structure *ds, int f1, int f2)
{
  
  int i1, i2;
  cpg_cell_t *cp;

  i1 = MIN (f1, f2);
  i2 = MAX (f1, f2);

  cp = ds->cpg->matrix [i1 + i2 * ds->num_fields]; 
  if (!cp || cp->distance > STRUCT_REORG_DISTANCE_THRESHOLD)
    return 0;
  else
    return cp->count;
}

#if 0
static HOST_WIDE_INT
fields_layout_distance (struct data_structure *ds,
			struct field_order *f1, int n)
{
  int i;
  struct field_order *crr;
  HOST_WIDE_INT size = 0;

  for (i = 0, crr = f1; i < n; i++, crr = crr->next)
    {
      if (! crr)
        break;

      size += field_size_in_bytes (ds->fields[crr->f_index].decl);
    }
  return size;
}
#endif

static gcov_type
field_wcp (struct data_structure *ds, struct field_order *first, int n,
           bool left)
{
  int i;
  HOST_WIDE_INT size = 1;
  struct field_order *crr = first;

  for (i = 0; i < n; i++)
    {
      crr = left ? first->next : first->prev;

      if (! crr)
	break;

      size *= field_size_in_bytes (ds->fields[crr->f_index].decl);
    }
  if (! crr)
    return 0;

  return cp_relation (ds, first->f_index, crr->f_index) / size;
}

#if 0
static gcov_type
data_structure_wcp (struct data_structure *ds, struct field_order *first)
{
  HOST_WIDE_INT remain_size;
  gcov_type ds_wcp = 0;
  struct field_order *left;
  int i,j;

  remain_size = CACHE_LINE_SIZE;
  for (j = 1, left = first; left; j++, left = left->next)
    for (i = 1; i < ds->num_fields - j; i++)
      ds_wcp += field_wcp (ds, left, i, true);
  return ds_wcp;

}
#endif

static gcov_type
wcp_left_right_contribution (struct data_structure *ds, int num_ordered,
			     struct field_order *first, int f, bool left)
{
  gcov_type ds_wcp = 0;
  struct field_order *new_f = xcalloc (1, sizeof (struct field_order));
  int i;

  new_f->f_index = f;

  if (left)
    {
      new_f->next = first;
      first->prev = new_f;
    }
  else
    {
      new_f->prev = first;
      first->next = new_f;
    }

  for (i = 1; i <= num_ordered ; i++)
    ds_wcp += field_wcp (ds, new_f, i, left);

  free (new_f);

  if (left)
    first->prev = NULL;
  else
    first->next = NULL;

  return ds_wcp;
}

/* Find the field with the maximum "fanout" and start the reordering
   with it.  */
static int
find_max_fanout_field (struct data_structure *ds,
		       sbitmap not_ordered_fields)
{
  int i, j;
  int f_max_fanout = -1;
  gcov_type max_fanout = 0;

  for (i = 0; i < ds->num_fields; i++)
    {
      gcov_type cp = 0;

      if (! TEST_BIT (not_ordered_fields, i))
	continue;

      for (j = 0; j < ds->num_fields; j++)
	if ((j != i) && TEST_BIT (not_ordered_fields, j))
	  cp += cp_relation (ds, i, j);

      if (cp > max_fanout)
        {
          max_fanout = cp;
          f_max_fanout = i;
        }
    }

  return f_max_fanout;
}


/* Perform the field reordering algorithm to find the "best" reordering
   of the structure fields (definition).  */
static void
reorder_fields_of_struct (struct data_structure *ds)
{
  int i, next_to_order, num_ordered = 0;
  sbitmap not_ordered_fields;
  struct field_order *order_left = NULL;
  struct field_order *order_right = NULL;
  struct field_order *crr;
  bool side = true;

  not_ordered_fields = sbitmap_alloc (ds->num_fields);
  sbitmap_ones (not_ordered_fields);

  /* Start witht the field with the maximum fanout, it has potential
     greater potential to contribute to the WCP of the ordered group
     so we want it int he middle.  */
  next_to_order = find_max_fanout_field (ds, not_ordered_fields);

  /* Loop while there is fields that aren't ordered yet.  */
  while ((sbitmap_first_set_bit (not_ordered_fields) >=0)
         && next_to_order >= 0)
    {
      gcov_type max_contrib = 0;
      gcov_type f_contrib;
      struct field_order *new_o;

      /* Now insert the NEXT_TO_ORDER field to the correct side in the
	 ordered list of fields.  */
      RESET_BIT (not_ordered_fields, next_to_order);
      new_o = (struct field_order *)
	      xcalloc (1, sizeof (struct field_order));
      new_o->f_index = next_to_order;
      if (! order_left)
        order_left = order_right = new_o;
      else if (side)
        {
          order_left->prev = new_o;
          new_o->next = order_left;
          order_left = new_o;
        }
      else
        {
          order_right->next = new_o;
          new_o->prev = order_right;
          order_right = new_o;
        }
        num_ordered ++;

      /* Find the field that gives the maximum contribution (to the left
	 or to the right), take also into acount the field fanout.  */
      for ( i = 0; i < ds->num_fields; i++)
        {
	  if (! TEST_BIT (not_ordered_fields, i))
	    continue;
          f_contrib = wcp_left_right_contribution (ds, num_ordered,
                                                   order_left, i, true);
          if (f_contrib > max_contrib)
            {
              max_contrib = f_contrib;
              next_to_order = i;
              side = true;
            }
          f_contrib = wcp_left_right_contribution (ds, num_ordered,
                                                   order_right, i, false);
          if (f_contrib > max_contrib)
            {
              max_contrib = f_contrib;
              next_to_order = i;
              side = false;
            }
        }

      /* This case is possible when the already ordered fields,
	 have no relation with the remaining fields, start again with
	 the field with the highest fanout from the remaining not
	 ordered fields.  */
      if (! TEST_BIT (not_ordered_fields, next_to_order))
     	{
	  next_to_order = find_max_fanout_field (ds, not_ordered_fields);
	  if (next_to_order < 0 )
	    next_to_order = sbitmap_first_set_bit (not_ordered_fields);
	  side = true;
        }
    }
  /* Now update the DS with the calculated ordering and free the
     ordering list.  */
  if (! ds->struct_clustering)
    ds->struct_clustering = (struct field_cluster *)
			    xcalloc (1, sizeof (struct field_cluster));
  ds->struct_clustering->fields_order = (int *)xcalloc (ds->num_fields,
                                                        sizeof (int));
  crr = order_left;
  i = 0;
  while (crr)
    {
      if (i >= ds->num_fields)
	abort ();
      ds->struct_clustering->fields_order[i++] = crr->f_index;
      if (crr->next)
        {
	  crr = crr->next;
	  free (crr->prev);
        }
      else
        {
          free (crr);
          break;
        }
    }
}

/* The total size of the group of fields in G.  */
static HOST_WIDE_INT
group_size_in_bytes (struct data_structure *ds, sbitmap g)
{
  HOST_WIDE_INT size_summ = 0;
  int i;

  for (i = 0; i < ds->num_fields; i++)
    {
      if (! TEST_BIT (g, i))
	continue;
      size_summ += field_size_in_bytes (ds->fields[i].decl);
    }

  return size_summ;
}

/* The average of the CP relation between field F and the fields in
   G.  */
static gcov_type 
average_cp_relation (struct data_structure *ds, sbitmap g, int f)
{
  int i, num = 0;
  gcov_type average_cp = 0;

  for (i = 0 ; i < ds->num_fields; i++)
    if (TEST_BIT (g, i))
      {
        average_cp = ((average_cp * num) + cp_relation (ds, i, f))/(num + 1);
	num++;
      }
  
  return average_cp;
}

/* The structure splitting algorithm
  Input:
    a group of fields G and its close proximity graph CPG(G).
  Output:
    a division of G into sub-groups g1 , g2 ,  , gn
  Definitions:
  1.  max_size - is the upper threshold on the sub-group size.
  2.  g[0] - the group of cold fields or fields that cannot gain from
             splitting to different groups.
  The Algorithm:
  1.  g[0] = {f ? G | access count (f) < Cold threshold access count }
  2.  G = G - g[0]
  3.  i=1
  4.  While G is no empty do
  5.    find the fields couple (fa, fb), such that CPG(fa, fb) is the highest.
  6.    if CPGcount( fa, fb) < access count threshold then
  7.        finish.
  8.    if size(fa) + size(fb) > max_size then
  9.      mark the edge (a,b) as not relevant.
  10.     continue.
  11.   endif
  12.   g[i] = {fa, fb}
  13.   find f in G such that the average CP(f,f`) where,
  14.   f` is in g[i] is the maximum and size(f) + size(g[i]) < max_size.
  15.   if average CP (f, g[i]) < access count threshold then
  16.     i = i + 1
  17.     continue.
  18.   endif
  11.   G = G - { f }
  12.   gi = gi + { f }
  13. end while.
*/

static void 
split_data_structure (struct data_structure *ds)
{
  int i, j;
  int max_size = STRUCT_REORG_CACHE_LINE_SIZE / 2; 
  int dynamic_threshold_n_updates = 0;
  gcov_type cold_field_threshold = 0;
  sbitmap remaining_fields = sbitmap_alloc (ds->num_fields); 
  struct field_cluster *crr_cluster, *cold_cluster;
  
  sbitmap_ones (remaining_fields);
  /* Find the cold field threshold.  */
  for (i = 0; i < ds->num_fields; i++)
    for (j = i; j < ds->num_fields; j++)
      if (cp_relation (ds, i, j) > cold_field_threshold)
        cold_field_threshold = cp_relation (ds, i, j);
  cold_field_threshold /= COLD_FIELD_RATIO;

  crr_cluster = NULL;
  while (sbitmap_first_set_bit (remaining_fields) >= 0)
    {
      gcov_type max_cp = 0;
      int max_i = -1; 
      int max_j = -1;

      /* Find the hotest CP relation in the CPG from the remaining fields.  
	 Here we want to favor hot self relation (a CP relation from one 
         a field to itself) over relations to other fields becuase it has
	 greater potential, if the groups size increases it will make less
	 instances of the sub-structure fit in the same cache line.  
	 The factor of favoring putting two fields together and seprating
	 them should be linear with the cache miss penalty.  */
      for (i = 0; i < ds->num_fields; i++)
	for (j = i; j < ds->num_fields; j++)
	  if (TEST_BIT (remaining_fields, i))
	    {
	      gcov_type cp = cp_relation (ds, i, j);
	      HOST_WIDE_INT size = field_size_in_bytes (ds->fields[i].decl);

	      if (TEST_BIT (remaining_fields, j))
		size += field_size_in_bytes (ds->fields[j].decl);  
	      /* Now consider the size of the fields.  */	
	      if (i != j)
		cp = cp / STRUCT_REORG_CACHE_MISS_PENALTY;
	      if (cp > max_cp && size <= max_size) 
		{
		  max_cp = cp;
		  max_i = i;
		  max_j = j;
		}
	    }
      /* If the maximum CP is less thanthe threshold, consider all the remianing
	 fields as cold.  */
      
      while (max_cp < cold_field_threshold)
	{
	  if (++dynamic_threshold_n_updates > STRUCT_REORG_DYNAMIC_THRESHOLD)
	    break;
	  cold_field_threshold = cold_field_threshold / COLD_FIELD_RATIO; 
	}
      if (max_cp < cold_field_threshold || max_i < 0 || max_j < 0 || ! cold_field_threshold)
	{
	  break;
	}

      /* Create a new cluster that contains MAX_I, MAX_J fields.  */
      ds->struct_clustering = (struct field_cluster *) xcalloc (1, sizeof (struct field_cluster));
      ds->struct_clustering->sibling = crr_cluster;
      crr_cluster = ds->struct_clustering;
      crr_cluster->fields_in_cluster = sbitmap_alloc (ds->num_fields);
      sbitmap_zero (crr_cluster->fields_in_cluster);
      SET_BIT (crr_cluster->fields_in_cluster, max_i);
      RESET_BIT (remaining_fields, max_i);
      if (TEST_BIT (remaining_fields, max_j))
	{
	  SET_BIT (crr_cluster->fields_in_cluster, max_j);
	  RESET_BIT (remaining_fields, max_j);
	}

      while (sbitmap_first_set_bit (remaining_fields) >= 0)
	{
	  gcov_type average_cp;
	  HOST_WIDE_INT f_size, g_size;

	  max_cp = 0;
	  for (i = 0; i < ds->num_fields; i++)
	    {
	      if (! TEST_BIT (remaining_fields, i))
		continue;
	      f_size = field_size_in_bytes (ds->fields[i].decl);
	      g_size = group_size_in_bytes (ds, crr_cluster->fields_in_cluster);
	      if ((g_size + f_size) > max_size)
		continue;
	      average_cp = average_cp_relation (ds, crr_cluster->fields_in_cluster, i);
	      /* average_cp = (average_cp * g_size)/(g_size + f_size);*/
	      average_cp = average_cp / STRUCT_REORG_CACHE_MISS_PENALTY;
	      if (average_cp > max_cp)
		{
		  max_i = i;
		  max_cp = average_cp;
		}
	    }
	  if (max_cp < cold_field_threshold)
	    break;
	  SET_BIT (crr_cluster->fields_in_cluster, max_i);
	  RESET_BIT (remaining_fields, max_i);
	}
    }
  if (sbitmap_first_set_bit (remaining_fields) >= 0)
    {
      /* Build g[0] - cold group.  */
      cold_cluster = (struct field_cluster *) xcalloc (1, sizeof (struct field_cluster));
      cold_cluster->sibling = ds->struct_clustering;
      ds->struct_clustering = cold_cluster;
      cold_cluster->fields_in_cluster = remaining_fields;
    }
  else
    sbitmap_free (remaining_fields);
  
}

