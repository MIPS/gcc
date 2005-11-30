/* Procedural abstraction using IPA.
   Copyright (C) 2005 Free Software Foundation, Inc.  

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
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "obstack.h"
#include "basic-block.h"
#include "flags.h"
#include "tree.h"
#include "real.h"
#include "tree-flow.h"
#include "tree-inline.h"
#include "cgraph.h"
#include "tree-pass.h"
#include "diagnostic.h"
#include "toplev.h"

/* Walk througt VAR list (initalized by INIT).  */
#define FOR_EACH_ELEM(var, init) \
  for (var = init; var; var = var->next)

/* Do not merge those sequences which use more than MAX_PARAMS_NUMBER
   number of parameters.  */
#define MAX_PARAMS_NUMBER -1

/* Return true if the call to merged function is small enought compared to
   original sequence length.  */
#define COMPARE_LENGTH_PARAM(l,p) (l > p * 6)

/* The minimum number of the seuence list.  */
#define MIN_SEQLIST_NUMBER 2

/* The minimum length of abstractable sequences.  */
#define MIN_LENGTH	8

/* The prefix of created artificial functions and variables.  */
#define METHOD_PREFIX	"__ipa_procabstr_m"
#define VAR_PREFIX	"__ipa_procabstr_v"

/* Shift by this while computing hash code in statement.  */
#define HASH_SHIFT 10

/* Shift by this while computing hash code in statement list.  */
#define BSI_HASH_SHIFT 1000

/* Number of statement in the hash comutation.  */
#define LOOK_FORWARD 10

/* First parameter of the htab_create function call.  */
#define MAX_HASH 1023

/* Structure for variables.  */
typedef struct def_var
{
  /* The variable itself.  */
  tree var;

  /* The position of container statement in the sequence.  */
  int index;

  /* The variable position in the container statement.  */
  int pos;

  /* Use VAR as a temporal variable in the new function.  */
  bool may_temp;

  /* The refered temporal variable in the new function.  */
  tree local_temp;

  /* Use VAR as a global variable.  */
  bool may_global;

  /* Need to save the value of VAR before function call.  */
  bool need_save;

  /* Need to restore the value of VAR after function call.  */
  bool need_load;

  /* Create new pointer type variable for VAR.  */
  bool create_pointer;

  /* The position in the argument list.  */
  int argnum;

  /* The parameter of the function call.  */
  tree arg;

  /* Point to the same variable which will be the argument in the function
     decl.  */
  struct def_var *samearg;

  /* Point to the same variable which will be the same parameter in the
     function call.  */
  struct def_var *unique_arg;

  /* Chain iterators.  */
  struct def_var *prev;
  struct def_var *next;
} *p_var;

/* Structure for storing any kind of tree pairs.  */
typedef struct def_treepair
{
  /* The first element of the pair.  */
  tree element1;

  /* The second element of the pair.  */
  tree element2;

  /* Chain iterators.  */
  struct def_treepair *next;
  struct def_treepair *prev;
} *p_treepair;

/* The sequence structure.  */
typedef struct def_sequence
{
  /* The unique identifier of the structure.  */
  unsigned int uid;

  /* The starting point of the sequence.  */
  block_stmt_iterator starting;

  /* The length of sequence.  */
  int length;

  /* The cgraph node where the sequence resides.  */
  struct cgraph_node *node;

  /* These insns must be inserted before/after the call to the new
     function.  */
  p_treepair mod_insns;

  /* Variable list of the sequence.  */
  p_var varlist;

  /* Contains pointer type variables which are definied in the sequence.  */
  htab_t ptr_varlist;

  /* Chain iterators.  */
  struct def_sequence *next;
  struct def_sequence *prev;
} *p_sequence;

/* The sequences list structure.  */
typedef struct def_seqlist
{
  /* The unique identifier of the structure.  */
  unsigned int uid;

  /* The element of sequence list.  */
  p_sequence seq;

  /* Chain iterators.  */
  struct def_seqlist *next;
  struct def_seqlist *prev;
} *p_seqlist;

/* The container of the abstractable sequences.  */
typedef struct def_allabstrseq
{
  /* The unique identifier of the structure.  */
  unsigned int uid;

  /* The element of the structure.  */
  p_seqlist abstrseq;

  /* The gain of sequence abstraction.  */
  int gain;

  /* Non zero if the element is not going to abstract.  */
  int removed;

  /* The length of the sequences in the sequence list.  */
  int length;

  /* Function declaration for the current sequence list (merged function). */
  tree decl;

  /* Chain iterators.  */
  struct def_allabstrseq *next;
  struct def_allabstrseq *prev;
} *p_allabstrseq;

/* The hash buckets of statements.  */
typedef struct def_fp
{
  /* Hash code of statement.  */
  unsigned int code;

  /* List of statements which have same hash code.  */
  htab_t bucket;
} *p_fp;

/* Element of a hash bucket.  */
typedef struct def_subfp
{
  /* The statement.  */
  tree node;

  /* The container function node.  */
  struct cgraph_node *cfun;
} *p_subfp;

/* The structure of the variables uses.  */
typedef struct def_all_vars
{
  /* The var_decl variable.  */
  tree var;

  /* Statement list. The list of uses of VAR.  */
  htab_t stmts;
} *p_all_vars;

/* This structure points to the sequence starting node in IPA phase.  */
typedef struct def_sequence_in_row
{
  /* The statement.  */
  tree stmt;

  /* The container function of STMT statement.  */
  struct cgraph_node *cnode;
} *p_sequence_in_row;

/* This structure stores two similar sequences.  */
typedef struct def_seqpair
{
  /* The similar sequences.  */
  struct def_sequence_in_row sequence0;
  struct def_sequence_in_row sequence1;

  /* The length of sequences.  */
  int length;

  /* Auxiliary property for merging sequence pairs into sequence list.  */
  int marker;
} *p_seqpair;

/* This structure contains a sequence list (merged from DEF_SEQPAIR).  */
typedef struct def_seqrow
{
  /* The length of the sequence list.  */
  int length;

  /* The gain of the abstraction.  */
  int gain;

  /* Contains the sequences.  */
  htab_t seqs;

  /* Auxiliary property for removing overlaping sequence lists.  */
  int marker;
} *p_seqrow;

/* This structure is for compute the position of the VARS variables in the
   sequence.  */
typedef struct def_index_and_vars
{
  /* The variable list of the sequence.  */
  p_var *vars;

  /* The position of container statement in the sequence.  */
  int index;

  /* The variable position in the container statement.  */
  int pos;
} *p_index_and_vars;

/* This structure is for modify a VAR variable in the POS postion in a
   statement.  */
typedef struct def_data
{
  /* Position.  */
  int pos;

  /* Position iterator.  */
  int lpos;

  /* Variable itself.  */
  tree *var;
} *p_data;

/* Structure of the statistics.  */
static struct def_statistics
{
  /* Number of save and restore statements around the call to merged
     functions.  */
  int loadsave;

  /* Number of abstracted sequence list.  */
  int seqlist;

  /* The iteration number of the procedure abstraction.  */
  int iter;

  /* Number of TREE stmt before abstraction.  */
  int stmt_num_before;

  /* Number of TREE stmt after abstraction.  */
  int stmt_num_after;

  /* Number of functions in the compilation unit.  */
  int fn_num;
} stats;

/* The all_sequence contains all possibly sequences for abstraction.  */
static p_sequence all_sequence;

/* The all_abstrseq conatins the list of sequences list for abstraction.  */
static p_allabstrseq all_abstrseq;

/* The fingerprints of the statements.  */
static htab_t fp_ht;

/* Store all vars and their container statements.  */
static htab_t all_vars;

/* Htab hash function for def_all_vars structure.  */
static unsigned int
all_vars_hash (const void *p)
{
  return htab_hash_pointer (((p_all_vars) p)->var);
}

/* Htab equal function for def_all_vars structure.  */
static int
all_vars_eq (const void *p0, const void *p1)
{
  return htab_eq_pointer (((p_all_vars) p0)->var, ((p_all_vars) p1)->var);
}

/* Htab del function for def_all_vars structure.  */
static void
all_vars_del (void *p)
{
  p_all_vars elem = (p_all_vars) p;
  if (elem->stmts)
    htab_delete (elem->stmts);
  free (elem);
}

/* Htab hash function for def_fp structure.  */
static unsigned int
fp_hash (const void *p)
{
  return ((p_fp) p)->code;
}

/* Htab equal function for def_fp structure.  */
static int
fp_eq (const void *p0, const void *p1)
{
  return fp_hash (p0) == fp_hash (p1);
}

/* Htab del function for def_fp structure.  */
static void
fp_del (void *p)
{
  p_fp elem = (p_fp) p;
  if (elem->bucket)
    htab_delete (elem->bucket);
  free (elem);
}

/* Htab hash function for def_subfp structure.  */
static unsigned int
subfp_hash (const void *p)
{
  return htab_hash_pointer (((p_subfp) p)->node);
}

/* Htab equal function for def_subfp structure.  */
static int
subfp_eq (const void *p0, const void *p1)
{
  return ((p_subfp) p0)->node == ((p_subfp) p1)->node;
}

/* Htab del function for def_subfp structure.  */
static void
subfp_del (void *p)
{
  p_subfp elem = (p_subfp) p;
  free (elem);
}

/* Htab hash function for def_seqpair structure.  */
static unsigned int
seqpair_hash (const void *p)
{
  p_seqpair data = (p_seqpair) p;
  return htab_hash_pointer (data->sequence0.stmt) +
    htab_hash_pointer (data->sequence1.stmt) + data->length;
}

/* Htab equal function for def_seqpair structure.  */
static int
seqpair_eq (const void *p0, const void *p1)
{
  p_seqpair d0 = (p_seqpair) p0;
  p_seqpair d1 = (p_seqpair) p1;
  return d0->length == d1->length
    && ((d0->sequence0.stmt == d1->sequence0.stmt
         && d0->sequence1.stmt == d1->sequence1.stmt)
        || (d0->sequence0.stmt == d1->sequence1.stmt
	    && d0->sequence1.stmt == d1->sequence0.stmt));
}

/* Htab del function for def_seqpair structure.  */
static void
seqpair_del (void *p)
{
  p_seqpair d = (p_seqpair) p;
  free (d);
}

/* Htab hash function for def_sequence_in_row structure.  */
static unsigned int
sequence_in_row_hash (const void *p)
{
  p_sequence_in_row d = (p_sequence_in_row) p;
  return htab_hash_pointer (d->stmt);
}

/* Htab equal function for def_sequence_in_row structure.  */
static int
sequence_in_row_eq (const void *p0, const void *p1)
{
  p_sequence_in_row d0 = (p_sequence_in_row) p0;
  p_sequence_in_row d1 = (p_sequence_in_row) p1;
  return htab_eq_pointer (d0->stmt, d1->stmt);
}

/* Htab del function for def_sequence_in_row structure.  */
static void
sequence_in_row_del (void *p)
{
  p_sequence_in_row d = (p_sequence_in_row) p;
  free (d);
}

/* Htab del function for def_seqrow structure.  */
static void
seqrow_del (void *p)
{
  p_seqrow d = (p_seqrow) p;
  if (d->seqs)
    htab_delete (d->seqs);
  free (d);
}

/* Htab hash function for length_list structure in create_length_list
   function.  */
static unsigned int
length_hash (const void *p)
{
  return *((int *) p);
}

/* Htab equal function for length_list structure in create_length_list
   function.  */
static int
length_eq (const void *p0, const void *p1)
{
  return *((int *) p0) == *((int *) p1);
}

/* Htab del function for length_list structure in create_length_list
   function.  */
static void
length_del (void *p)
{
  int *d = (int *) p;
  free (d);
}

/* Initialize the iteration of procedural abstraction.  */
static void
init_procabstr (void)
{
  all_sequence = NULL;
  all_abstrseq = NULL;

  all_vars = htab_create (MAX_HASH, all_vars_hash, all_vars_eq, all_vars_del);
  fp_ht = htab_create (MAX_HASH, fp_hash, fp_eq, fp_del);
}

/* Cleanup the iteration of procedural abstraction.  */
static void
cleanup_procabstr (void)
{
  p_seqlist abstr;
  p_allabstrseq all;
  p_sequence seq;
  p_var var;
  p_treepair pair;

  while (all_sequence)
    {
      seq = all_sequence;
      all_sequence = seq->next;

      if (seq->ptr_varlist != NULL)
	htab_delete (seq->ptr_varlist);

      while (seq->varlist)
	{
	  var = seq->varlist;
	  seq->varlist = var->next;
	  free (var);
	}
      while (seq->mod_insns)
	{
	  pair = seq->mod_insns;
	  seq->mod_insns = pair->next;
	  free (pair);
	}
      free (seq);
    }

  while (all_abstrseq)
    {
      all = all_abstrseq;
      all_abstrseq = all->next;

      while (all->abstrseq)
	{
	  abstr = all->abstrseq;
	  all->abstrseq = abstr->next;
	  free (abstr);
	}
      free (all);
    }
  htab_delete (all_vars);
  htab_delete (fp_ht);
}

/* Add a sequence which start from BSI into LIST list. The length of sequence
   is LENGTH. The NODE is the callgraph node which holds the sequence. It
   returns the generated p_sequence structure of the sequence.  */
static p_sequence
addto_sequence (p_sequence * list, block_stmt_iterator bsi, int length,
		struct cgraph_node *node)
{
  p_sequence *seq;
  p_sequence last;
  block_stmt_iterator tmp;
  int i;

  seq = list;
  last = NULL;

  while (*seq)
    {
      last = *seq;
      seq = &((*seq)->next);
    }

  (*seq) = (p_sequence) xcalloc (1, sizeof (struct def_sequence));

  (*seq)->starting = bsi;
  (*seq)->length = length;
  (*seq)->node = node;
  (*seq)->mod_insns = NULL;
  (*seq)->varlist = NULL;
  (*seq)->ptr_varlist = NULL;
  (*seq)->next = NULL;
  (*seq)->prev = last;

  /* Generate UID from the UID of container function, the UID of the basic_block
     and the distance of the sequence from the begining of basic_block.  */
  for (i = 0, tmp = bsi_start (bsi.bb);
       !bsi_end_p (tmp) && bsi_stmt (tmp) != bsi_stmt (bsi);
       i++, bsi_next (&tmp));
  (*seq)->uid = (DECL_UID (node->decl) << 16) + (bsi.bb->index << 8) + i;

  return *seq;
}

/* It returns the sequence which starts from BSI with LENGTH length if
   all_sequence contains it. Otherwise It creates a new sequence and returns
   with it. The NODE is the callgraph node that holds the sequence.  */
static p_sequence
access_sequence (block_stmt_iterator bsi, int length,
		 struct cgraph_node *node)
{
  p_sequence seq;

  for (seq = all_sequence;
       seq && (bsi_stmt (bsi) != bsi_stmt (seq->starting)
	       || length != seq->length); seq = seq->next);

  if (!seq)
    seq = addto_sequence (&all_sequence, bsi, length, node);

  return seq;
}

/* The addto_seqlist inserts SEQ sequence into LIST list and returns with the
   sequence list.  */
static p_seqlist
addto_seqlist (p_seqlist * list, p_sequence seq)
{
  p_seqlist *seqlist;
  p_seqlist last;

  seqlist = list;
  last = NULL;

  while (*seqlist)
    {
      last = *seqlist;
      seqlist = &((*seqlist)->next);
    }

  (*seqlist) = (p_seqlist) xcalloc (1, sizeof (struct def_seqlist));

  (*seqlist)->seq = seq;
  (*seqlist)->prev = last;
  (*seqlist)->next = NULL;

  (*seqlist)->uid = 0;
  for (last = *seqlist; last; last = last->prev)
    if ((*seqlist)->uid == 0 || (*seqlist)->uid > last->seq->uid)
      (*seqlist)->uid = last->seq->uid;

  return *seqlist;
}

/* The addto_allabstrseq inserts SEQLIST sequence list into the global container
   of the abstractable sequences. It returns the modified list container.  */
static p_allabstrseq
addto_allabstrseq (p_allabstrseq * list, p_seqlist seqlist)
{
  p_allabstrseq *abstrlist;
  p_allabstrseq last;

  abstrlist = list;
  last = NULL;

  while (*abstrlist)
    {
      last = *abstrlist;
      abstrlist = &((*abstrlist)->next);
    }

  (*abstrlist) = (p_allabstrseq) xcalloc (1, sizeof (struct def_allabstrseq));

  (*abstrlist)->abstrseq = seqlist;
  (*abstrlist)->removed = 0;
  (*abstrlist)->gain = 0;
  (*abstrlist)->length = 0;
  (*abstrlist)->uid = (last != NULL) ? last->uid + 1 : 0;
  (*abstrlist)->decl = NULL_TREE;
  (*abstrlist)->prev = last;
  (*abstrlist)->next = NULL;

  return *abstrlist;
}

/* Cost function. Higher return value means P is more complexed node.  */
static int
compute_node_cost (tree p)
{
  int cost, i, n;

  if (!p)
    return 0;

  cost = 0;

  /* Do not count.  */
  switch (TREE_CODE (p))
    {
    case MODIFY_EXPR:
      switch (TREE_CODE (TREE_OPERAND (p, 1)))
	{
	case CONVERT_EXPR:
	  return 0;
	default:
	  break;
	}
    default:
      break;
    }

  switch (TREE_CODE_CLASS (TREE_CODE (p)))
    {
    case tcc_unary:
    case tcc_binary:
    case tcc_comparison:
    case tcc_expression:
    case tcc_reference:
    case tcc_statement:
      n = TREE_CODE_LENGTH (TREE_CODE (p));
      for (i = 0; i < n; i++)
	cost += compute_node_cost (TREE_OPERAND (p, i));
      if (n > 0)
	cost += n - 1;
      break;
    default:
      break;
    }
  return cost;
}

/* Returns true if T0 and T1 are similar statements. T0 and T1 are similar when
   they are identical in any sub-tree except their variables.  */
static int
stmt_equal_simple (tree t0, tree t1)
{
  enum tree_code c0, c1;
  int cmp;
  int i;

  /* If the operand fields empty (like ARRAY_REF).  */
  if (t0 == NULL_TREE && t1 == NULL_TREE)
    return 1;

  if (t0 == NULL_TREE || t1 == NULL_TREE)
    return 0;

  c0 = TREE_CODE (t0);
  c1 = TREE_CODE (t1);

  if (c0 != c1)
    return 0;

  /* Return 1 when T0 and T1 are the same tree node, but do not
     return in some cases when its need futher checks.  */
  if (t0 == t1
      && c0 != LABEL_DECL
      && c0 != VAR_DECL
      && c0 != PARM_DECL && (c0 != FUNCTION_DECL || !DECL_INLINE (t0)))
    return 1;

  switch (c0)
    {
      /* Return 1 if T0 and T1 tree code are VAR_DECL or PARAM_DECL, and
         their type is the same.  */
    case PARM_DECL:
    case VAR_DECL:
      return TREE_TYPE (t0) == TREE_TYPE (t1);

      /* Do not include this nodes in the sequences.  */
    case COND_EXPR:
    case SWITCH_EXPR:
    case RETURN_EXPR:
    case LABEL_EXPR:
    case LABEL_DECL:
    case CONST_DECL:
    case FIELD_DECL:
    case FUNCTION_DECL:
      return 0;

    default:
      break;
    }

  /* We haven't got language hook. So do not include this nodes in the
     sequences.  */
  if ((int) c0 >= (int) LAST_AND_UNUSED_TREE_CODE)
    return 0;

  switch (TREE_CODE_CLASS (c0))
    {
      /* Check all operands.  */
    case tcc_unary:
    case tcc_binary:
    case tcc_comparison:
    case tcc_expression:
    case tcc_reference:
    case tcc_statement:
      for (cmp = 1, i = 0; cmp == 1 && i < TREE_CODE_LENGTH (c0); i++)
	cmp = stmt_equal_simple (TREE_OPERAND (t0, i), TREE_OPERAND (t1, i));
      return cmp;

      /* Otherwise use identical matching.  */
    default:
      return simple_cst_equal (t0, t1);;
    }
}

/* Returns sequence pair if we can find similar sequences started from BSI0
   and BSI1 block statement iterator.  */
static p_seqpair
find_seqpair (block_stmt_iterator bsi0, block_stmt_iterator bsi1)
{
  tree stmt0 = NULL_TREE , stmt1 = NULL_TREE;
  int length;
  p_seqpair seqpair = NULL;

  /* Save the starting tree nodes for checking overlaping sequences.  */
  if (!bsi_end_p (bsi0) && !bsi_end_p (bsi1))
    {
      stmt0 = bsi_stmt (bsi0);
      stmt1 = bsi_stmt (bsi1);
    }

  /* Find similar sequences. */
  for (length = 0;
       /* Break before end of block.  */
       !bsi_end_p (bsi0) && !bsi_end_p (bsi1)
       /* Break before overlap eachother.  */
       && bsi_stmt (bsi0) != stmt1 && bsi_stmt (bsi1) != stmt0
       /* Break if not similar.  */
       && stmt_equal_simple (bsi_stmt (bsi0), bsi_stmt (bsi1));
       bsi_next (&bsi0), bsi_next (&bsi1), length++);

  if (length >= MIN_LENGTH)
    {
      seqpair = (p_seqpair) xcalloc (1, sizeof (struct def_seqpair));
      seqpair->sequence0.stmt = stmt0;
      seqpair->sequence1.stmt = stmt1;
      seqpair->length = length;
      seqpair->marker = 0;
    }

  return seqpair;
}

/* This function stores SEQPAIR sequence pair into the container list
   SEQPAIR_LIST.  */
static void
add_seqpair (htab_t seqpair_list, p_seqpair seqpair)
{
  void **slot = htab_find_slot (seqpair_list, seqpair, INSERT);
  *slot = seqpair;
}

/* Returns htab_t length set which contains every length from SEQPAIR_LIST
   sequence pair list.  */
static htab_t
create_length_list (htab_t seqpair_list)
{
  htab_iterator hti;
  htab_t length_list =
    htab_create (MAX_HASH, length_hash, length_eq, length_del);
  p_seqpair seqpair;
  void **slot;
  int *len;

  FOR_EACH_HTAB_ELEMENT (seqpair_list, seqpair, p_seqpair, hti)
  {
    int l = seqpair->length;
    if (!htab_find (length_list, &l))
      {
	len = (int *) xcalloc (1, sizeof (int));
	*len = l;
	slot = htab_find_slot (length_list, len, INSERT);
	*slot = len;
      }
  }

  return length_list;
}

/* Add SEQPAIR sequence pair to SEQROW sequence list.  */
static void
add_seqrow (p_seqrow * seqrow, p_seqpair seqpair)
{
  void **slot;
  p_sequence_in_row sr;
  struct def_sequence_in_row s0, s1;
  bool f0, f1;

  /* Allocate initial elements of sequence list.  */
  if (!*seqrow)
    {
      *seqrow = (p_seqrow) xcalloc (1, sizeof (struct def_seqrow));
      (*seqrow)->length = seqpair->length;
      (*seqrow)->seqs =
	htab_create (MAX_HASH, sequence_in_row_hash, sequence_in_row_eq,
		     sequence_in_row_del);
      (*seqrow)->marker = 0;
      (*seqrow)->gain = -1;
    }

  s0.stmt = seqpair->sequence0.stmt;
  f0 = (htab_find ((*seqrow)->seqs, &s0) == NULL);

  s1.stmt = seqpair->sequence1.stmt;
  f1 = (htab_find ((*seqrow)->seqs, &s1) == NULL);

  /* Add the first element of the sequence pair to the sequence list.  */
  if (f0)
    {
      sr =
        (p_sequence_in_row) xcalloc (1, sizeof (struct def_sequence_in_row));
      sr->stmt = seqpair->sequence0.stmt;
      sr->cnode = seqpair->sequence0.cnode;
      slot = htab_find_slot ((*seqrow)->seqs, &s0, INSERT);
      *slot = sr;
    }

  /* Add the second element of the sequence pair to the sequence list.  */
  if (f1)
    {
      sr =
	(p_sequence_in_row) xcalloc (1, sizeof (struct def_sequence_in_row));
      sr->stmt = seqpair->sequence1.stmt;
      sr->cnode = seqpair->sequence1.cnode;
      slot = htab_find_slot ((*seqrow)->seqs, &s1, INSERT);
      *slot = sr;
    }
}

/* Returns true if sequence - started from CROW_START tree node with LENGTH
   length - is overlap any sequence in the SEQROW sequence list.  */
static bool
is_over_seq (tree crow_start, int length, p_seqrow seqrow)
{
  p_sequence_in_row seqrow_start;
  htab_iterator hti1;
  block_stmt_iterator bsi;
  int i;

  FOR_EACH_HTAB_ELEMENT (seqrow->seqs, seqrow_start, p_sequence_in_row, hti1)
    if (bb_for_stmt (crow_start) == bb_for_stmt (seqrow_start->stmt))
    {
      /* Starting point of the sequence in SEQROW sequence list is in the
         sequence started from CROW_START.  */
      for (i = 0, bsi = bsi_for_stmt (crow_start); i < length;
	   bsi_next (&bsi), i++)
	if (bsi_stmt (bsi) == seqrow_start->stmt)
	  return true;
      /* The CROW_START statement is in the sequence from SEQROW sequence
         list.  */
      for (i = 0, bsi = bsi_for_stmt (seqrow_start->stmt); i < seqrow->length;
	   bsi_next (&bsi), i++)
	if (bsi_stmt (bsi) == crow_start)
	  return true;
    }
  return false;
}

/* Returns true if CROW sequence list overlap any sequence list in SEQROW_SET
   sequence list set.  */
static bool
is_over (p_seqrow crow, htab_t seqrow_set)
{
  p_sequence_in_row crow_start;
  htab_iterator hti1, hti2;
  p_seqrow seqrow;

  FOR_EACH_HTAB_ELEMENT (crow->seqs, crow_start, p_sequence_in_row, hti1)
    FOR_EACH_HTAB_ELEMENT (seqrow_set, seqrow, p_seqrow, hti2)
     if (is_over_seq (crow_start->stmt, crow->length, seqrow))
      return true;
  return false;
}

/* Returns true if we can add a new sequence pair from SEQPAIR_LIST list into
   SEQROW sequence list. MARKED stores the number of merged sequence pair from
   SEQPAIR_LIST sequence pair list.  */
static bool
find_next_seqpair (p_seqrow seqrow, htab_t seqpair_list, int *marked)
{
  htab_iterator hti;
  p_seqpair seqpair;
  bool found = false;

  FOR_EACH_HTAB_ELEMENT (seqpair_list, seqpair, p_seqpair, hti)
    if (!seqpair->marker && seqpair->length >= seqrow->length)
      {
        struct def_sequence_in_row s0, s1;
        bool is_s0, is_s1;
        s0.stmt = seqpair->sequence0.stmt;
        s1.stmt = seqpair->sequence1.stmt;

        is_s0 = htab_find (seqrow->seqs, &s0) != NULL;
        is_s1 = htab_find (seqrow->seqs, &s1) != NULL;
        if (is_s0 || is_s1)
          {
            seqpair->marker = 1;
            (*marked)++;
            if ((is_s0
                 && is_over_seq (seqpair->sequence1.stmt, seqpair->length,
                                 seqrow))
                || (is_s1
                    && is_over_seq (seqpair->sequence0.stmt, seqpair->length,
                                    seqrow)))
              continue;
            add_seqrow (&seqrow, seqpair);
            found = true;
          }
      }

  return found;
}

/* Returns the gain of the abstraction when merging SEQROW sequence list.  */
static int
compute_seqrow_gain (p_seqrow seqrow)
{
  int gain, i;
  block_stmt_iterator bsi;
  htab_iterator hti;
  p_sequence_in_row node;

  if (seqrow->gain >= 0)
    return seqrow->gain;
  gain = 0;

  FOR_EACH_HTAB_ELEMENT (seqrow->seqs, node, p_sequence_in_row, hti)
  {
    for (i = 0, bsi = bsi_for_stmt (node->stmt); i < seqrow->length;
	 i++, bsi_next (&bsi))
      gain += compute_node_cost (bsi_stmt (bsi));
    break;
  }

  /* Formula: gain = cost_of_sequence * (number_of_sequences - 1).  */
  gain = gain * (htab_elements (seqrow->seqs) - 1);
  seqrow->gain = gain;
  return gain;
}

/* Returns sequence list from SEQROW_LIST list which has the biggest gain
   value and SEQROW_SET set does not contain it.  */
static p_seqrow
find_max_gain (htab_t seqrow_list, htab_t seqrow_set)
{
  p_seqrow seqrow, max_seqrow = NULL;
  htab_iterator hti;
  int gain = 0, max_gain = 0;

  FOR_EACH_HTAB_ELEMENT (seqrow_list, seqrow, p_seqrow, hti)
    if (!seqrow->marker && !htab_find (seqrow_set, seqrow))
      {
        gain = compute_seqrow_gain (seqrow);
        if (gain > max_gain)
          {
            max_gain = gain;
            max_seqrow = seqrow;
          }
      }
  return max_seqrow;
}

/* Auxiliary function for walk_tree. This function gathers all variables from
   TP tree node an stores in a list which is passed by DATA.  */
static tree
find_all_vars (tree * tp, int *walk_subtrees ATTRIBUTE_UNUSED, void *data)
{
  p_index_and_vars iv = (p_index_and_vars) data;
  if (*tp && is_gimple_variable (*tp))
    {
      p_var *v;
      p_var last;

      v = iv->vars;
      last = NULL;

      while (*v)
	{
	  last = *v;
	  v = &((*v)->next);
	}

      (*v) = (p_var) xcalloc (1, sizeof (struct def_var));

      (*v)->var = *tp;
      (*v)->index = iv->index;
      (*v)->pos = iv->pos;
      (*v)->argnum = 0;
      (*v)->samearg = NULL;
      (*v)->unique_arg = NULL;
      (*v)->arg = NULL;
      (*v)->may_temp = DECL_ARTIFICIAL (*tp);
      (*v)->may_global = false;
      (*v)->need_save = true;
      (*v)->need_load = true;
      (*v)->local_temp = NULL_TREE;
      (*v)->create_pointer = !POINTER_TYPE_P (TREE_TYPE (*tp));
      (*v)->prev = last;
      (*v)->next = NULL;
      iv->pos = iv->pos + 1;
    }
  return NULL_TREE;
}

/* Convert SEQROW_SET sequence lists to a new data representation for
   merging.  */
static void
convert_seqrow (htab_t seqrow_set)
{
  htab_iterator hti, hti1;
  block_stmt_iterator bsi;
  p_seqrow seqrow;
  p_sequence_in_row node;
  int i;

  /* Walk each sequence list.  */
  FOR_EACH_HTAB_ELEMENT (seqrow_set, seqrow, p_seqrow, hti)
    {
      p_seqlist slist = NULL;

      /* Walk each sequence.  */
      FOR_EACH_HTAB_ELEMENT (seqrow->seqs, node, p_sequence_in_row, hti1)
        {
          p_sequence seq;
          p_var vars = NULL;

          seq = access_sequence (bsi_for_stmt (node->stmt), seqrow->length,
                                 node->cnode);

          /* Walk sequence for finding variables.  */
          for (i = 0, bsi = bsi_for_stmt (node->stmt);
               !bsi_end_p (bsi) && i < seqrow->length;
               i++, bsi_next (&bsi))
            {
              struct def_index_and_vars iv;
              tree node;

              iv.vars = &vars;
              iv.index = i;
              iv.pos = 0;
              node = bsi_stmt (bsi);
              walk_tree (&node, find_all_vars, &iv, NULL);
            }

          seq->varlist = vars;
          addto_seqlist (&slist, seq);
        }

      addto_allabstrseq (&all_abstrseq, slist);
    }
}

/* This function produces the abstractable sequence lists from SEQROW_LIST
   list.  */
static void
find_max_seqrow (htab_t seqrow_list)
{
  p_seqrow seqrow, max_seqrow;
  htab_t seqrow_set;
  htab_iterator hti;
  void **slot;

  /* Contains those sequence list which have the biggest gain and do not
     overlap eachother.  */
  seqrow_set =
    htab_create (MAX_HASH, htab_hash_pointer, htab_eq_pointer, NULL);

  FOR_EACH_HTAB_ELEMENT (seqrow_list, seqrow, p_seqrow, hti)
    seqrow->marker = 0;

  /* Find sequence list which has the biggest gain and add into SEQROW_SET
     set.  */
  for (max_seqrow = find_max_gain (seqrow_list, seqrow_set);
       max_seqrow;
       max_seqrow = find_max_gain (seqrow_list, seqrow_set))
    {
      max_seqrow->marker = 1;
      if (is_over (max_seqrow, seqrow_set))
	continue;
      is_over (max_seqrow, seqrow_set);
      slot = htab_find_slot (seqrow_set, max_seqrow, INSERT);
      *slot = max_seqrow;
    }

  convert_seqrow (seqrow_set);

  htab_delete (seqrow_set);
}

/* This function find similar sequence for merging into new function.  */
static void
find_sequences (void)
{
  htab_iterator hti_base, hti0, hti1;
  p_fp res;
  p_subfp sub0, sub1;
  tree stmt0, stmt1;
  block_stmt_iterator bsi0, bsi1;
  void **slot;
  p_seqpair seqpair;
  p_seqrow seqrow;
  int *seqlen;
  htab_t seqrow_list;

  seqrow_list =
    htab_create (MAX_HASH, htab_hash_pointer, htab_eq_pointer, seqrow_del);

  /* Walk the hash table.  */
  FOR_EACH_HTAB_ELEMENT (fp_ht, res, p_fp, hti_base)
    {
      htab_t seqpair_list;
      htab_t length_list;

      seqpair_list =
        htab_create (MAX_HASH, seqpair_hash, seqpair_eq, seqpair_del);

      /* Compare each element of hash bucket to eachother.  */
      FOR_EACH_HTAB_ELEMENT (res->bucket, sub0, p_subfp, hti0)
        {
          stmt0 = sub0->node;
          bsi0 = bsi_for_stmt (stmt0);
          FOR_EACH_HTAB_ELEMENT (res->bucket, sub1, p_subfp, hti1)
            {
              stmt1 = sub1->node;
              if (stmt0 == stmt1)
                continue;
              bsi1 = bsi_for_stmt (stmt1);

              /* Look for identical or same sequence pair.  */
              seqpair = find_seqpair (bsi0, bsi1);
              if (seqpair)
                {
                  seqpair->sequence0.cnode = sub0->cfun;
                  seqpair->sequence1.cnode = sub1->cfun;
                  add_seqpair (seqpair_list, seqpair);
                }
            }
        }

      if (htab_elements (seqpair_list) == 0)
        continue;

      /* Find different lengths.  */
      length_list = create_length_list (seqpair_list);

      /* Walk length list.  */
      FOR_EACH_HTAB_ELEMENT (length_list, seqlen, int *, hti0)
        {
          int marked, max;

          max = htab_elements (seqpair_list);
          marked = 0;
          FOR_EACH_HTAB_ELEMENT (seqpair_list, seqpair, p_seqpair, hti1)
            seqpair->marker = 0;

          /* Walk all element of LENGTH_LIST list.  */
          while (marked < max)
            {
              seqrow = NULL;

              /* Look for the inital elements of sequence list.  */
              FOR_EACH_HTAB_ELEMENT (seqpair_list, seqpair, p_seqpair, hti1)
                if (!seqpair->marker && seqpair->length <= *seqlen)
                {
                  marked++;
                  seqpair->marker = 1;

                  /* Find the first sequence pair for the new sequence list.  */
                  if (seqpair->length == *seqlen)
                    {
                      add_seqrow (&seqrow, seqpair);
                      break;
                    }
                }
              if (!seqrow)
                break;

              /* Attach more sequence into the sequence list.  */
              while (find_next_seqpair (seqrow, seqpair_list, &marked));

              /* Store the sequence list.  */
              slot = htab_find_slot (seqrow_list, seqrow, INSERT);
              *slot = seqrow;
            }
        }

      htab_delete (length_list);
      htab_delete (seqpair_list);
    }

  /* Clean overlaping.  */
  find_max_seqrow (seqrow_list);

  htab_delete (seqrow_list);
}

/* Auxiliary function for walk_tree. This function returns with the definied
   variable if it is any.  */
static tree
find_decl_def (tree * tp, int *walk_subtrees, void *data ATTRIBUTE_UNUSED)
{
  tree op;
  switch (TREE_CODE (*tp))
    {
    case MODIFY_EXPR:
      op = TREE_OPERAND (*tp, 0);
      if (is_gimple_variable (op))
	return op;
      break;
    default:
      *walk_subtrees = 0;
      break;
    }
  return NULL_TREE;
}

/* Auxiliary function for walk_tree. This function find all variable usage
   for sequence.  */
static tree
find_decl_use (tree * tp, int *walk_subtrees, void *data)
{
  tree op0, op1;
  htab_t *set = (htab_t *) data;

  if (!*tp)
    return NULL_TREE;

  switch (TREE_CODE (*tp))
    {
    case MODIFY_EXPR:
      op0 = TREE_OPERAND (*tp, 0);
      op1 = TREE_OPERAND (*tp, 1);
      if (is_gimple_variable (op0))
	{
	  *walk_subtrees = 0;
	  return walk_tree (&op1, find_decl_use, data, NULL);
	}
      break;
    default:
      break;
    }
  if (is_gimple_variable (*tp))
    {
      void **slot = htab_find_slot (*set, *tp, INSERT);
      *slot = *tp;
    }
  return NULL_TREE;
}

/* Auxiliary function for walk_tree. This function find all variable usage
   for entire compilation unit.  */
static tree
find_all_uses (tree * tp, int *walk_subtrees ATTRIBUTE_UNUSED, void *data)
{
  if (is_gimple_variable (*tp))
    {
      struct def_all_vars tmp;
      p_all_vars elem;
      void **slot;
      tree stmt = (tree) data;

      /* Store this variable if it is not defined.  */
      if (*tp == walk_tree (&stmt, find_decl_def, NULL, NULL))
	return NULL_TREE;

      tmp.var = *tp;
      elem = (p_all_vars) htab_find (all_vars, &tmp);
      if (!elem)
	{
	  elem = (p_all_vars) xcalloc (1, sizeof (struct def_all_vars));
	  elem->var = *tp;
	  elem->stmts =
	    htab_create (MAX_HASH, htab_hash_pointer, htab_eq_pointer, NULL);
	  slot = htab_find_slot (all_vars, &tmp, INSERT);
	  *slot = elem;
	}
      slot = htab_find_slot (elem->stmts, data, INSERT);
      *slot = data;
      *slot = data;
    }
  return NULL_TREE;
}

/* Returns true if we can find the definition of the variable from the usage
   of it. This function is used for find locally temporal variable.  */
static bool
find_dec_def_from_use (tree use, tree decl, p_sequence seq, int length)
{
  block_stmt_iterator bsi, end;
  basic_block bb;
  tree end_stmt;
  int i;

  bb = bb_for_stmt (use);
  /* Removed after merging sequences.  */
  if (bb == NULL)
    return true;

  bsi = bsi_for_stmt (use);
  end = bsi_start (bb);
  /* The usage is in the sequence.  */
  if (seq->starting.bb == bb)
    {
      for (i = 0, end = seq->starting; i < length && !bsi_end_p (end);
	   i++, bsi_next (&end))
	if (use == bsi_stmt (end))
	  {
	    for (bsi = end; !bsi_end_p (bsi); bsi_prev (&bsi), i--)
	      {
		tree stmt, def;

		stmt = bsi_stmt (bsi);
		def = walk_tree (&stmt, find_decl_def, NULL, NULL);

		/* Definition is in the sequence.  */
		if (def == decl)
		  return true;

		/* Definition is not in the sequence. Return false. */
		if (i == 0)
		  return false;
	      }
	    return false;
	  }
      if (!bsi_end_p (end))
	bsi_prev (&end);
    }
  if (bsi_end_p (end))
    end_stmt = NULL_TREE;
  else
    end_stmt = bsi_stmt (end);

  /* Find definition in the same basic block.  */
  for (; !bsi_end_p (bsi) && bsi_stmt (bsi) != end_stmt; bsi_prev (&bsi))
    {
      tree stmt = bsi_stmt (bsi);
      if (decl == walk_tree (&stmt, find_decl_def, NULL, NULL))
	return true;
    }
  return false;
}

/* This function find locally temporal variable for SEQ sequence with LENGTH
   length.  */
static void
find_temp_vars (p_sequence seq, int length)
{
  p_var vars;
  struct def_all_vars tmp;
  p_all_vars uses;
  htab_iterator hti;
  tree stmt;
  bool may_temp;

  FOR_EACH_ELEM (vars, seq->varlist)
  {
    int uid;
    if (vars->index >= length)
      continue;

    uid = DECL_UID (vars->var);
    may_temp = true;
    tmp.var = vars->var;
    uses = htab_find (all_vars, &tmp);

    /* If this variable has usage.  */
    if (uses)
      {
	/* Walk each variable usage.  */
	FOR_EACH_HTAB_ELEMENT (uses->stmts, stmt, tree, hti)
	  if (!find_dec_def_from_use (stmt, vars->var, seq, length))
	  {
	    may_temp = false;
	    break;
	  }
      }

    /* Global variable can't be temporal.  */
    if (is_global_var (vars->var))
      may_temp = false;

    /* If we find all definition of the usage and in the sequence there is a
       definition before this usage, this variable is marked to be locally
       temporal.  */
    vars->may_temp = may_temp;
  }
}

/* This function produces a list of variables where the variables are definied
   and their types are a pointer type.  */
static void
find_ptr_def (p_sequence seq, int length)
{
  block_stmt_iterator bsi;
  int i;
  for (i = 0, bsi = seq->starting; i < length && !bsi_end_p (bsi);
       i++, bsi_next (&bsi))
    {
      tree stmt;
      tree decl;
      stmt = bsi_stmt (bsi);
      decl = walk_tree (&stmt, find_decl_def, NULL, NULL);
      if (decl
	  && is_gimple_variable (decl) && POINTER_TYPE_P (TREE_TYPE (decl)))
	{
	  void **slot = htab_find_slot (seq->ptr_varlist, decl, INSERT);
	  *slot = decl;
	}
    }
}

/* This function merges the temporal variables information.  */
static void
merge_temp_vars (p_seqlist seqlist)
{
  bool is_temp;
  p_var vars, nextvars, samevars;
  p_seqlist slist;

  FOR_EACH_ELEM (vars, seqlist->seq->varlist)
    {
      int uid;
      if (vars->index >= seqlist->seq->length)
        continue;

      uid = DECL_UID (vars->var);
      is_temp = vars->may_temp;
      FOR_EACH_ELEM (slist, seqlist->next)
        {
          FOR_EACH_ELEM (nextvars, slist->seq->varlist)
            if (vars->index == nextvars->index && vars->pos == nextvars->pos)
              {
                is_temp = nextvars->may_temp;
                break;
              }
          if (!is_temp)
            break;
        }

      if (is_temp)
        {
          int index = vars->index;
          int pos = vars->pos;
          htab_t last, curr;

          last = NULL;
          curr =
            htab_create (MAX_HASH, htab_hash_pointer, htab_eq_pointer, NULL);

          FOR_EACH_ELEM (slist, seqlist)
            {
              FOR_EACH_ELEM (nextvars, slist->seq->varlist)
                if (nextvars->index == index
                    && nextvars->pos == pos)
                  {
                    int hash =
                      nextvars->index * BSI_HASH_SHIFT + nextvars->pos;
                    void **slot = htab_find_slot (curr, (void *) hash, INSERT);
                    *slot = (void *) hash;
                    break;
                  }

              FOR_EACH_ELEM (samevars, slist->seq->varlist)
                if (nextvars
                    && nextvars->var == samevars->var
                    && nextvars != samevars)
                  {
                    int hash =
                      samevars->index * BSI_HASH_SHIFT + samevars->pos;
                    void **slot = htab_find_slot (curr, (void *) hash, INSERT);
                    *slot = (void *) hash;
                  }

              if (last)
                {
                  htab_iterator hti;
                  tree decl;

                  if (htab_elements (last) != htab_elements (curr))
                    {
                      is_temp = false;
                      break;
                    }

                  FOR_EACH_HTAB_ELEMENT (curr, decl, tree, hti)
                    if (!htab_find (last, decl))
                      {
                        is_temp = false;
                        break;
                      }
                  if (!is_temp)
                    break;
                }

              if (last)
                htab_delete (last);
              last = curr;
              curr =
                htab_create (MAX_HASH, htab_hash_pointer, htab_eq_pointer,
                             NULL);
            }

            if (last)
              htab_delete (last);
            htab_delete (curr);
          }

      FOR_EACH_ELEM (slist, seqlist)
        FOR_EACH_ELEM (nextvars, slist->seq->varlist)
          if (vars->index == nextvars->index && vars->pos == nextvars->pos)
            {
              nextvars->may_temp = is_temp;
              break;
            }
    }
}

/* This function merges pointer information of the variable.  */
static void
merge_ptr_def (p_seqlist seqlist)
{
  bool is_ptr;
  p_var vars, nextvars;
  p_seqlist slist;

  FOR_EACH_ELEM (vars, seqlist->seq->varlist)
    {
      int uid;
      if (vars->index >= seqlist->seq->length)
        continue;

      uid = DECL_UID (vars->var);
      is_ptr = (vars->create_pointer
                || htab_find (seqlist->seq->ptr_varlist, vars->var) != NULL);
      FOR_EACH_ELEM (slist, seqlist->next)
        {
          FOR_EACH_ELEM (nextvars, slist->seq->varlist)
            if (vars->index == nextvars->index
                && vars->pos == nextvars->pos)
              {
                is_ptr = (nextvars->create_pointer
                          || htab_find (slist->seq->ptr_varlist,
                                        nextvars->var) != NULL);
                break;
              }
          if (is_ptr)
            break;
        }

      FOR_EACH_ELEM (slist, seqlist)
        FOR_EACH_ELEM (nextvars, slist->seq->varlist)
          if (vars->index == nextvars->index
              && vars->pos == nextvars->pos)
            {
              nextvars->create_pointer = is_ptr;
              break;
            }
    }
}

/* This function find and mark those global variables which are identical in
   each sequences.  */
static void
find_may_global (p_seqlist seqlist)
{
  bool is_global;
  p_var vars, nextvars;
  p_seqlist slist;

  FOR_EACH_ELEM (vars, seqlist->seq->varlist)
    {
      if (vars->index >= seqlist->seq->length)
        continue;

      is_global = is_global_var (vars->var);
      FOR_EACH_ELEM (slist, seqlist->next)
        {
          FOR_EACH_ELEM (nextvars, slist->seq->varlist)
            if (vars->index == nextvars->index
                && vars->pos == nextvars->pos)
              {
                is_global = (vars->var == nextvars->var);
                break;
              }
          if (!is_global)
            break;
        }

      if (!is_global)
        FOR_EACH_ELEM (slist, seqlist)
          FOR_EACH_ELEM (nextvars, slist->seq->varlist)
            if (vars->index == nextvars->index
                && vars->pos == nextvars->pos)
              {
                nextvars->may_global = is_global;
                break;
              }
    }
}

/* This function find those variables which need to be saved before the
   function call.  */
static void
find_save (p_seqlist seqlist)
{
  p_var vars, nextvars;
  p_seqlist slist;
  bool may_save;

  FOR_EACH_ELEM (slist, seqlist)
    FOR_EACH_ELEM (vars, slist->seq->varlist)
      {
        if (!vars->may_temp && !vars->may_global)
          {
            block_stmt_iterator bsi;
            int i;
            bool may_save = false;
            htab_t set =
              htab_create (MAX_HASH, htab_hash_pointer, htab_eq_pointer, NULL);

            for (i = 0, bsi = slist->seq->starting;
                i < slist->seq->length && i <= vars->index && !bsi_end_p (bsi);
                i++, bsi_next (&bsi))
              {
                tree stmt;
                tree decl;
                stmt = bsi_stmt (bsi);
                walk_tree (&stmt, find_decl_use, &set, NULL);
                if (htab_find (set, vars->var))
                  {
                    may_save = true;
                    break;
                  }
                decl = walk_tree (&stmt, find_decl_def, NULL, NULL);
                if (decl && decl == vars->var && is_gimple_variable (decl))
                  break;
              }
            htab_delete (set);
            vars->need_save = may_save;
          }
      }

  FOR_EACH_ELEM (vars, seqlist->seq->varlist)
    {
      if (vars->index >= seqlist->seq->length)
        continue;

      may_save = vars->need_save;
      FOR_EACH_ELEM (slist, seqlist->next)
        {
          FOR_EACH_ELEM (nextvars, slist->seq->varlist)
            if (vars->index == nextvars->index
                && vars->pos == nextvars->pos)
              {
                may_save = vars->need_save;
                break;
              }
          if (may_save)
            break;
        }

      if (may_save)
        FOR_EACH_ELEM (slist, seqlist)
          FOR_EACH_ELEM (nextvars, slist->seq->varlist)
            if (vars->index == nextvars->index
                && vars->pos == nextvars->pos)
              {
                nextvars->need_save = may_save;
                break;
              }
    }
}

/* This function find those variables which need to be restored after the
   function call.  */
static void
find_load (p_seqlist seqlist)
{
  p_var vars;
  p_seqlist slist;
  htab_t set =
    htab_create (MAX_HASH, htab_hash_pointer, htab_eq_pointer, NULL);

  FOR_EACH_ELEM (slist, seqlist)
    {
      block_stmt_iterator bsi;
      int i;

      htab_empty (set);
      set->n_elements = 0;
      for (i = 0, bsi = slist->seq->starting;
          i < slist->seq->length && !bsi_end_p (bsi); i++, bsi_next (&bsi))
        {
          tree stmt;
          tree decl;
          stmt = bsi_stmt (bsi);
          decl = walk_tree (&stmt, find_decl_def, NULL, NULL);
          if (decl && !htab_find (set, decl))
            {
              void **slot = htab_find_slot (set, decl, INSERT);
              *slot = decl;
            }
        }

      FOR_EACH_ELEM (vars, slist->seq->varlist)
        if (!htab_find (set, vars->var))
          vars->need_load = false;

    }
  htab_delete (set);
}

/* This function find identical parameters for each new functions and merge
   them.  */
static void
optimize_params (p_seqlist seqlist)
{
  p_var vars, samevars, seq_vars, seq_samevars;
  p_seqlist slist;
  bool all_same;

  FOR_EACH_ELEM (vars, seqlist->seq->varlist)
    {
      if (vars->samearg)
        continue;
      FOR_EACH_ELEM (samevars, vars->next)
        if (vars->var == samevars->var
            && !samevars->samearg
            && samevars->index < seqlist->seq->length)
          {
            all_same = false;
            FOR_EACH_ELEM (slist, seqlist->next)
              {
                all_same = false;
                FOR_EACH_ELEM (seq_vars, slist->seq->varlist)
                  if (seq_vars->index == vars->index
                      && seq_vars->pos == vars->pos)
                    break;

                if (!seq_vars)
                  break;

                FOR_EACH_ELEM (seq_samevars, seq_vars->next)
                  if (seq_samevars
                      && seq_samevars->index == samevars->index
                      && seq_samevars->pos == samevars->pos)
                    {
                      all_same = (seq_vars->var == seq_samevars->var);
                      break;
                    }

                if (!all_same)
                  break;
              }

            if (all_same)
              FOR_EACH_ELEM (slist, seqlist)
                {
                  FOR_EACH_ELEM (seq_vars, slist->seq->varlist)
                    if (seq_vars->index == vars->index
                        && seq_vars->pos == vars->pos)
                      break;

                  if (!seq_vars)
                    break;

                  FOR_EACH_ELEM (seq_samevars, seq_vars->next)
                    if (seq_samevars
                        && seq_samevars->index == samevars->index
                        && seq_samevars->pos == samevars->pos)
                      {
                        seq_samevars->samearg = seq_vars;
                        break;
                      }
                }
          }
    }
}

/* This function find the identical arguments for each function calls and
   merge them.  */
static void
unique_params (p_seqlist seqlist)
{
  p_seqlist slist;
  p_var vars, nextvars;

  FOR_EACH_ELEM (slist, seqlist)
    FOR_EACH_ELEM (vars, slist->seq->varlist)
      FOR_EACH_ELEM (nextvars, vars->next)
        if (!nextvars->unique_arg && vars->var == nextvars->var)
          nextvars->unique_arg = vars;
}

/* This function set up auxiliary properties for merging. */
static void
set_abstrseq_info (p_allabstrseq * abstrseq)
{
  int length;
  p_seqlist slist;

  length = 0;
  FOR_EACH_ELEM (slist, (*abstrseq)->abstrseq)
    {
      if (length == 0)
        length = slist->seq->length;
      else if (length > slist->seq->length)
        length = slist->seq->length;
    }
  FOR_EACH_ELEM (slist, (*abstrseq)->abstrseq)
    slist->seq->length = length;

  (*abstrseq)->length = length;

  FOR_EACH_ELEM (slist, (*abstrseq)->abstrseq)
    {
      slist->seq->ptr_varlist =
        htab_create (MAX_HASH, htab_hash_pointer, htab_eq_pointer, NULL);
      find_temp_vars (slist->seq, length);
      find_ptr_def (slist->seq, length);
    }
  merge_temp_vars ((*abstrseq)->abstrseq);
  merge_ptr_def ((*abstrseq)->abstrseq);
  find_may_global ((*abstrseq)->abstrseq);

  find_save ((*abstrseq)->abstrseq);
  find_load ((*abstrseq)->abstrseq);

  optimize_params ((*abstrseq)->abstrseq);
  unique_params ((*abstrseq)->abstrseq);
}

/* Return true if DECL is a function definition.  */
static bool
is_fn_definition (tree decl)
{
  return (TREE_CODE (decl) == FUNCTION_DECL
	  && DECL_INITIAL (decl) != NULL_TREE
	  && DECL_SAVED_TREE (decl) != NULL_TREE);
}

/* Returns true if the global parameters of the procedural abstraction is
   not reached for the ABSTRSEQ abstractable sequence list.  */
static bool
check_params_no (p_allabstrseq abstrseq)
{
  p_sequence seq = abstrseq->abstrseq->seq;
  p_seqlist slist;
  p_var vars;
  int count = 0;
  int loadsave = 0;
  int sc = 0;

  FOR_EACH_ELEM (slist, abstrseq->abstrseq)
    sc++;

  FOR_EACH_ELEM (vars, seq->varlist)
    if (vars->index < abstrseq->length
        && !vars->may_temp)
      count++;

  FOR_EACH_ELEM (vars, seq->varlist)
    if (vars->index < abstrseq->length
	&& !vars->may_temp
	&& !vars->may_global
	&& !vars->samearg)
      {
	if (vars->need_save)
	  loadsave++;
	if (vars->need_load)
	  loadsave++;
      }
  return ((MAX_PARAMS_NUMBER < 0
           || count <= MAX_PARAMS_NUMBER)
	  && sc >= MIN_SEQLIST_NUMBER
	  && COMPARE_LENGTH_PARAM (abstrseq->length, loadsave));
}

/* Print debug info.  */
static void
dump_abstrseq (p_allabstrseq abstrseq)
{
  p_seqlist seqlist;

  if (!dump_file || !abstrseq)
    return;

  fprintf (dump_file, "Sequence list (al: %d)\n", abstrseq->length);

  FOR_EACH_ELEM (seqlist, abstrseq->abstrseq)
    {
      block_stmt_iterator bsi = seqlist->seq->starting;
      int length = seqlist->seq->length;
      p_var v0;
      tree res;
      htab_iterator hti;

      fprintf (dump_file, "\tSequence (function: %d, len: %d) from ",
              seqlist->seq->node->uid, seqlist->seq->length);
      print_generic_expr (dump_file, seqlist->seq->node->decl, dump_flags);
      fprintf (dump_file, "\n");

      for (; length > 0; bsi_next (&bsi), length--)
        {
          fprintf (dump_file, "\t\t");
          print_generic_stmt (dump_file, bsi_stmt (bsi), 2);
        }

      fprintf (dump_file, "\t\t---\n");
      fprintf (dump_file, "\t\t idx pos par glb sav loa tmp ptr sag uag\n");
      FOR_EACH_ELEM (v0, seqlist->seq->varlist)
        {
          fprintf (dump_file, "\t\t");

          fprintf (dump_file, "%4d", v0->index);
          fprintf (dump_file, "%4d ", v0->pos);
          if (v0->may_temp || v0->may_global || v0->samearg)
            fprintf (dump_file, "no ");
          else
            fprintf (dump_file, "yes");

          fprintf (dump_file, "%s", v0->may_global ? " yes" : " no ");
          fprintf (dump_file, "%s", v0->need_save ? " yes" : " no ");
          fprintf (dump_file, "%s", v0->need_load ? " yes" : " no ");
          fprintf (dump_file, "%s", v0->may_temp ? " yes" : " no ");
          fprintf (dump_file, "%s", v0->create_pointer ? " yes" : " no ");
          fprintf (dump_file, "%s", v0->samearg ? " yes" : " no ");
          fprintf (dump_file, "%s", v0->unique_arg ? " yes" : " no ");
          fprintf (dump_file, "  var(%d): ", DECL_UID (v0->var));
          print_generic_expr (dump_file, TREE_TYPE (v0->var), dump_flags);
          fprintf (dump_file, " ");
          print_generic_expr (dump_file, v0->var, dump_flags);
          fprintf (dump_file, "\n");
        }

      fprintf (dump_file, "\t\t---\n\t\tptr_varlist: ");
      FOR_EACH_HTAB_ELEMENT (seqlist->seq->ptr_varlist, res, tree, hti)
        {
          print_generic_expr (dump_file, res, dump_flags);
          fprintf (dump_file, " ");
        }
      fprintf (dump_file, "\n\n");
    }
  fprintf (dump_file, "\n");
}

/* Auxiliary function for walk_tree. This function returns with a variable in
   a certain position in a statement. */
static tree
find_positioned_var (tree * tp, int *walk_subtrees ATTRIBUTE_UNUSED,
		     void *data)
{
  p_data d = (p_data) data;
  if (*tp && is_gimple_variable (*tp))
    {
      if (d->pos == d->lpos)
	{
	  d->var = tp;
	  return *tp;
	}
      d->lpos++;
    }
  return NULL_TREE;
}

/* Returns the new function body of the merge ABSTRSEQ sequences list.  */
static tree
merge_sequences (p_allabstrseq * abstrseq)
{
  p_seqlist seqlist = (*abstrseq)->abstrseq;
  int seqlength = (*abstrseq)->length;
  int i;
  block_stmt_iterator bsi;
  tree body = NULL_TREE;

  for (bsi = seqlist->seq->starting, i = 0;
       !bsi_end_p (bsi) && i < seqlength; bsi_next (&bsi), i++)
    {
      tree stmt = bsi_stmt (bsi);
      tree call, decl;
      p_var var;

      /* Copy the node.  */
      tree new_node = copy_node (stmt);

      /* Substitute variables.  */
      FOR_EACH_ELEM (var, seqlist->seq->varlist) if (var->index == i)
	{
	  struct def_data data;
	  tree arg;
	  int j;
	  p_var ref;

	  if (var->samearg)
	    ref = var->samearg;
	  else
	    ref = var;

	  if (ref->may_global)
	    continue;

	  for (j = 1, arg = DECL_ARGUMENTS (current_function_decl);
	       arg; arg = TREE_CHAIN (arg), j++)
	    if (j == ref->argnum)
	      break;
	  data.pos = var->pos;
	  data.lpos = 0;
	  data.var = NULL;
	  walk_tree (&new_node, find_positioned_var, &data, NULL);
	  gcc_assert (data.var);
	  if (data.var)
	    {
	      if (ref->may_temp)
		{
		  if (!ref->local_temp)
		    ref->local_temp =
		      create_tmp_var (TREE_TYPE (var->var), VAR_PREFIX);
		  *(data.var) = ref->local_temp;
		}
	      else
		*(data.var) =
		  ref->create_pointer ? build_fold_indirect_ref (arg) : arg;
	    }
	  gcc_assert (*data.var);
	}

      append_to_statement_list (new_node, &body);

      /* Create call graph edges if new_node contains a CALL_EXPR.  */
      call = get_call_expr_in (new_node);
      if (call && (decl = get_callee_fndecl (call)))
        cgraph_create_edge (cgraph_node (current_function_decl),
                            cgraph_node (decl), new_node, 0, 0);

    }

  if (dump_file)
    {
      tree_stmt_iterator tsi;
      tree arg;

      fprintf (dump_file, "new function merged:\n");
      fprintf (dump_file, "declname: ");
      print_generic_stmt (dump_file, current_function_decl, TDF_SLIM);
      fprintf (dump_file, "args:\n");
      for (arg = DECL_ARGUMENTS (current_function_decl); arg;
	   arg = TREE_CHAIN (arg))
	{
	  print_generic_expr (dump_file, TREE_TYPE (arg), dump_flags);
	  fprintf (dump_file, " ");
	  print_generic_expr (dump_file, arg, dump_flags);
	  fprintf (dump_file, ", ");
	}
      fprintf (dump_file, "\nbody:\n");
      for (tsi = tsi_start (body); !tsi_end_p (tsi); tsi_next (&tsi))
	print_generic_stmt (dump_file, tsi_stmt (tsi), TDF_SLIM);
      fprintf (dump_file, "\n");
    }

  return body;
}

/* Returns with the parameter list of the new function.  */
static tree
create_function_parm_decls (p_allabstrseq * abstrseq)
{
  p_var var;
  tree arglist = NULL_TREE;
  int argnum = 0;

  if (!(*abstrseq))
    return NULL_TREE;

  FOR_EACH_ELEM (var, (*abstrseq)->abstrseq->seq->varlist)
    {
      tree old_var = var->var;
      tree new_var;
      tree new_type;

      if (var->index >= (*abstrseq)->length
          || var->may_temp
          || var->may_global
          || var->samearg)
        continue;

      gcc_assert (TREE_CODE (old_var) == VAR_DECL
                  || TREE_CODE (old_var) == PARM_DECL);

      /* The parameter will be a pointer type of the type before.  */
      new_type = var->create_pointer ?
        build_pointer_type (TREE_TYPE (old_var)) :
        TREE_TYPE (old_var);

      /* Build the new variable.  */
      new_var = build_decl (PARM_DECL, create_tmp_var_name ("param"),
                            new_type);

      /* We have to set DECL_ARG_TYPE for PARM_DECL.  */
      DECL_ARG_TYPE (new_var) = new_type;
      DECL_ARTIFICIAL (new_var) = 1;
      DECL_ABSTRACT (new_var) = 0;
      DECL_IGNORED_P (new_var) = 1;
      TREE_USED (new_var) = 1;

      /* Set the context of declaration to the new function if it's not a
         global variable.  */
      if (DECL_CONTEXT (old_var))
        DECL_CONTEXT (new_var) = current_function_decl;

      /* Set DECL_SIZE and DECL_MODE.  */
      DECL_SIZE (new_var) = 0;
      DECL_SIZE_UNIT (new_var) = 0;
      DECL_MODE (new_var) = VOIDmode;
      layout_decl (new_var, 0);

      if (CODE_CONTAINS_STRUCT (TREE_CODE (new_var), TS_DECL_WRTL)
          && !TREE_STATIC (new_var)
          && !DECL_EXTERNAL (new_var))
        SET_DECL_RTL (new_var, NULL_RTX);

      /* Set the context of declaration to the new function if it's not a
         global variable.  */
      if (DECL_CONTEXT (old_var))
        DECL_CONTEXT (new_var) = current_function_decl;

      gcc_assert (new_var);

      /* Chain new_var to the argumentlist.  */
      TREE_CHAIN (new_var) = arglist;
      arglist = new_var;

      /* Set argnum for variable in varlist.  */
      argnum++;
      var->argnum = argnum;
    }

  return nreverse (arglist);
}

/* Build a flowgraph for the statement_list STMT_LIST.  */
static void
make_blocks (tree stmt_list)
{
  tree_stmt_iterator i = tsi_start (stmt_list);
  tree stmt = NULL;
  bool start_new_block = true;
  bool first_stmt_of_list = true;
  basic_block bb = ENTRY_BLOCK_PTR;

  while (!tsi_end_p (i))
    {
      tree prev_stmt;

      prev_stmt = stmt;
      stmt = tsi_stmt (i);

      /* If the statement starts a new basic block or if we have determined
         in a previous pass that we need to create a new block for STMT, do
         so now.  */
      if (start_new_block)
	{
	  if (!first_stmt_of_list)
	    stmt_list = tsi_split_statement_list_before (&i);
	  bb = create_basic_block (stmt_list, NULL, bb);
	  start_new_block = false;
	}

      /* Now add STMT to BB and create the subgraphs for special statement
         codes.  */
      set_bb_for_stmt (stmt, bb);

      /* If STMT is a basic block terminator, set START_NEW_BLOCK for the
         next iteration.  */
      if (stmt_ends_bb_p (stmt))
	start_new_block = true;

      tsi_next (&i);
      first_stmt_of_list = false;
    }
}

/* This function creates a new function for ABSTRSEQ sequence list.  */
static void
create_function_decl (p_allabstrseq * abstrseq)
{
  tree func, args, res, body, tmp_retn;
  char name_buf[32];
  p_var var;

  args = NULL_TREE;
  FOR_EACH_ELEM (var, (*abstrseq ? (*abstrseq)->abstrseq->seq->varlist : NULL))
    {
      /* We don't need to do anything with those VAR_DECL or PARM_DECL nodes
        which are out of the abstractable sequence's border (built by the
        uniform length for each sequence lists).  */
      if (var->index >= (*abstrseq)->length
          || var->may_temp
          || var->may_global
          || var->samearg)
        continue;

      args =
        tree_cons (NULL_TREE, var->create_pointer ?
                              build_pointer_type (TREE_TYPE (var->var)) :
                              TREE_TYPE (var->var), args);
    }

  args = args == NULL_TREE ? void_list_node : nreverse (args);

  sprintf (name_buf, "%s%d_i%d", METHOD_PREFIX, (*abstrseq)->uid, stats.iter);
  func = get_file_function_name_long (name_buf);

  args = build_function_type (void_type_node, args);
  func = build_decl (FUNCTION_DECL, func, args);

  current_function_decl = func;

  res = build_decl (RESULT_DECL, NULL_TREE, void_type_node);
  DECL_ARTIFICIAL (res) = 1;
  DECL_IGNORED_P (res) = 1;
  DECL_RESULT (func) = res;

  allocate_struct_function (func);

  TREE_STATIC (func) = 1;
  TREE_USED (func) = 1;
  DECL_ARTIFICIAL (func) = 1;
  DECL_IGNORED_P (func) = 1;
  DECL_NO_INSTRUMENT_FUNCTION_ENTRY_EXIT (func) = 1;
  DECL_ARGUMENTS (func) = create_function_parm_decls (abstrseq);

  body = merge_sequences (abstrseq);

  tmp_retn = build (RETURN_EXPR, void_type_node, NULL_TREE);
  append_to_statement_list (tmp_retn, &body);

  DECL_SAVED_TREE (func) = body;
  TREE_PUBLIC (func) = 1;
  DECL_UNINLINABLE (func) = 1;
  DECL_INITIAL (func) = make_node (BLOCK);
  TREE_USED (DECL_INITIAL (func)) = 1;

  gimplify_function_tree (func);
  cgraph_finalize_function (func, true);
  init_empty_tree_cfg ();
  make_blocks (body);

  (*abstrseq)->decl = func;
  current_function_decl = NULL_TREE;
  cfun = NULL;
}

/* Auxiliary function for creating tree node pairs.  */
static void
add_tree_pair (p_treepair * treepair, tree t1, tree t2)
{
  p_treepair *pair;
  p_treepair last;

  pair = treepair;
  last = NULL;

  while (*pair)
    {
      last = *pair;
      pair = &((*pair)->next);
    }

  (*pair) = (p_treepair) xcalloc (1, sizeof (struct def_treepair));

  (*pair)->element1 = t1;
  (*pair)->element2 = t2;
  (*pair)->prev = last;
  (*pair)->next = NULL;
}

/* Returns the cgraph node of the DECL function.  */
static struct cgraph_node *
get_node_for_function (tree decl)
{
  struct cgraph_node *node;

  FOR_EACH_ELEM (node, cgraph_nodes)
    if (DECL_UID (node->decl) == DECL_UID (decl))
      return node;

  return NULL;
}

/* This function creates the function calls.  */
static void
create_function_calls (p_allabstrseq * abstrseq)
{
  p_var var;
  p_seqlist seqlist;

  FOR_EACH_ELEM (seqlist, (*abstrseq)->abstrseq)
    {
      block_stmt_iterator bsi;
      tree call;
      tree args;
      p_treepair pair;

      args = NULL_TREE;
      current_function_decl = seqlist->seq->node->decl;
      cfun = DECL_STRUCT_FUNCTION (current_function_decl);

      FOR_EACH_ELEM (var, seqlist->seq->varlist)
        {
          tree v;
          tree tmp_var = NULL_TREE;
          tree save_modexpr;
          tree restore_modexpr;

          /* We don't need to do anything with those VAR_DECL or PARM_DECL nodes
            which are out of the abstractable sequence's border (built by the
            uniform length for each sequence lists).  */
          if (var->index >= (*abstrseq)->length
              || var->may_temp
              || var->may_global
              || var->samearg)
            continue;

          v = var->var;

          /* Not pointer type variables.  */
          if (!var->unique_arg
              && (!POINTER_TYPE_P (TREE_TYPE (v))
                  || var->create_pointer))
            {
              /* Addressable variables.  */
              if (TREE_ADDRESSABLE (v))
                {
                  v =
                    var->create_pointer ? build_addr (v,
                                                      seqlist->seq->
                                                      node->decl) : v;
                }
              /* Not addressable variables.  */
              else
                {
                  tmp_var = create_tmp_var (TREE_TYPE (v), VAR_PREFIX);
                  SET_DECL_ASSEMBLER_NAME (tmp_var, DECL_NAME (tmp_var));

                  if (var->need_save)
                    {
                      save_modexpr =
                        build (MODIFY_EXPR, void_type_node, tmp_var, v);
                      stats.loadsave++;
                    }
                  else
                    save_modexpr = NULL;

                  if (var->need_load)
                    {
                      restore_modexpr =
                        build (MODIFY_EXPR, void_type_node, v, tmp_var);
                      stats.loadsave++;
                    }
                  else
                    restore_modexpr = NULL;

                  add_tree_pair (&seqlist->seq->mod_insns, save_modexpr,
                                restore_modexpr);
                  v = var->create_pointer ?
                      build_addr (tmp_var, seqlist->seq->node->decl) :
                      tmp_var;
                }
              var->arg = v;
            }
          else if (var->unique_arg
                  && (!POINTER_TYPE_P (TREE_TYPE (var->unique_arg->var))
                      || var->unique_arg->create_pointer))
            v = var->unique_arg->arg;

          gcc_assert (v);

          args = tree_cons (NULL_TREE, v, args);
        }

      if (args != NULL_TREE)
        args = nreverse (args);

      /* Build a call expression to our new function.  */
      call = build_function_call_expr ((*abstrseq)->decl, args);
      bsi = seqlist->seq->starting;

      /* Save the values of the original variables.  */
      FOR_EACH_ELEM (pair, seqlist->seq->mod_insns) if (pair->element2)
        bsi_insert_before (&bsi, pair->element2, BSI_NEW_STMT);

      /* Insert the call.  */
      bsi_insert_before (&bsi, call, BSI_NEW_STMT);

      /* Restore the values of the original variables.  */
      FOR_EACH_ELEM (pair, seqlist->seq->mod_insns) if (pair->element1)
        bsi_insert_before (&bsi, pair->element1, BSI_NEW_STMT);

      /* Create a new callgraph edge, that represents a call to our new
        function. The usage of DECL_NAME and DECL_ASSEMBLER_NAME macros should
        be reviewed.  */
      cgraph_create_edge (seqlist->seq->node,
                          get_node_for_function ((*abstrseq)->decl), call,
                          0, 0);

      current_function_decl = NULL_TREE;
      cfun = NULL;
    }
}

/* This function removes the original sequences.  */
static void
remove_sequence_stmts (p_allabstrseq * abstrseq)
{
  block_stmt_iterator bsi;
  p_seqlist seqlist;
  int seqlength = (*abstrseq)->length;
  int i;

  FOR_EACH_ELEM (seqlist, (*abstrseq)->abstrseq)
    {
      for (bsi = seqlist->seq->starting, i = 0;
           !bsi_end_p (bsi) && i < seqlength; i++)
        {
          tree stmt = bsi_stmt (bsi);
          tree call = get_call_expr_in (stmt);

          /* Remove callgraph edges.  */
          if (call)
            {
              struct cgraph_edge *edge =
                cgraph_edge (seqlist->seq->node, stmt);
              if (edge)
                cgraph_remove_edge (edge);
            }

          /* Remove statement.  */
          bsi_remove (&bsi);
        }
    }
}

/* Returns the hash value of the STMT statement.  */
static unsigned int
stmt_hash (tree stmt)
{
  unsigned int res = 0;
  int i;

  if (!stmt)
    return res;
  switch (TREE_CODE_CLASS (TREE_CODE (stmt)))
    {
    case tcc_unary:
    case tcc_binary:
    case tcc_comparison:
    case tcc_expression:
    case tcc_reference:
    case tcc_statement:
      for (i = 0; i < TREE_CODE_LENGTH (TREE_CODE (stmt)); i++)
	if (TREE_OPERAND (stmt, i))
	  res = res * HASH_SHIFT + TREE_CODE (TREE_OPERAND (stmt, i));
      break;
    default:
      break;
    }

  return res * HASH_SHIFT + TREE_CODE (stmt);
}

/* Returns with the hash number of FIRST bsi.  */
static unsigned int
bsi_hash (block_stmt_iterator first)
{
  unsigned int res = 0;
  int i;
  block_stmt_iterator bsi;

  for (i = 0, bsi = first;
       i < LOOK_FORWARD && !bsi_end_p (bsi);
       i++, bsi_next (&bsi))
    res = res * BSI_HASH_SHIFT + stmt_hash (bsi_stmt (bsi));

  return i == LOOK_FORWARD ? res : 0;
}

/* This funtion creates the hash table for faster sequence finding method.  */
static void
create_stmt_hash (void)
{
  struct cgraph_node *node;
  FOR_EACH_ELEM (node, cgraph_nodes)
  {
    basic_block bb;
    block_stmt_iterator bsi;
    if (!is_fn_definition (node->decl)
        || !DECL_SAVED_TREE (node->decl))
      continue;
    FOR_EACH_BB_FN (bb, DECL_STRUCT_FUNCTION (node->decl))
      for (bsi = bsi_start (bb); !bsi_end_p (bsi); bsi_next (&bsi))
        {
          tree stmt = bsi_stmt (bsi);
          p_fp elem = NULL;
          p_subfp sub = NULL;
          struct def_fp tmp;
          struct def_subfp subtmp;
          void **slot;

          walk_tree (&stmt, find_all_uses, stmt, NULL);

          switch (TREE_CODE (stmt))
            {
            case LABEL_EXPR:
            case RETURN_EXPR:
            case GOTO_EXPR:
              continue;
            default:
              break;
            }

          tmp.code = bsi_hash (bsi);
          if (tmp.code)
            {
              elem = htab_find (fp_ht, &tmp);
              if (!elem)
                {
                  elem = (p_fp) xcalloc (1, sizeof (struct def_fp));
                  elem->code = tmp.code;
                  elem->bucket =
                    htab_create (MAX_HASH, subfp_hash, subfp_eq, subfp_del);

                  slot = htab_find_slot (fp_ht, elem, INSERT);
                  *slot = elem;
                }

              subtmp.node = stmt;
              sub = htab_find (elem->bucket, &subtmp);
              if (!sub)
                {
                  sub = (p_subfp) xcalloc (1, sizeof (struct def_subfp));
                  sub->node = stmt;
                  sub->cfun = node;

                  slot = htab_find_slot (elem->bucket, stmt, INSERT);
                  *slot = sub;
                }
            }
        }
  }
}

/* Main function of procedural abstraction.  */
static void
ipa_procabstr (void)
{
  p_allabstrseq abstrseq;
  bool found;

  stats.iter = 0;
  stats.seqlist = 0;
  stats.loadsave = 0;
  stats.stmt_num_before = 0;
  stats.stmt_num_after = 0;
  stats.fn_num = 0;

  found = true;
  while (found)
    {
      found = false;

      init_procabstr ();
      create_stmt_hash ();
      find_sequences ();

      FOR_EACH_ELEM (abstrseq, all_abstrseq)
        if (!abstrseq->removed)
	{
	  set_abstrseq_info (&abstrseq);
	  if (!check_params_no (abstrseq))
	    {
	      abstrseq->removed = 1;
	      continue;
	    }
	  found = true;
	  stats.seqlist++;

	  dump_abstrseq (abstrseq);
	  create_function_decl (&abstrseq);
	  create_function_calls (&abstrseq);
	  remove_sequence_stmts (&abstrseq);
	}

      cleanup_procabstr ();
      stats.iter++;
    }
}

/* The gate of procedural abstraction.  */
static bool
is_procabstr (void)
{
  return (flag_ipa_procabstr == 1);
}

struct tree_opt_pass pass_ipa_procabstr = {
  "procabstr",			/* name  */
  is_procabstr,			/* gate  */
  ipa_procabstr,		/* execute  */
  NULL,				/* sub  */
  NULL,				/* next  */
  0,				/* static_pass_number  */
  0,				/* tv_id  */
  0,				/* properties_required  */
  0,				/* properties_provided  */
  0,				/* properties_destroyed  */
  0,				/* todo_flags_start  */
  TODO_dump_cgraph | TODO_dump_func,	/* todo_flags_finish  */
  0				/* letter  */
};
