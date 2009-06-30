/* Perform re-association according to extended Sethi-Ullmann
   numbering for every real INSN, for GNU compiler.
   Copyright (C) 2008  Free Software Foundation, Inc.


This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "obstack.h"
#include "icg-opcode.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "flags.h"
#include "function.h"
#include "basic-block.h"
#include "tree-pass.h"
#include "df.h"
#include "icg.h"


#define REASSOC_DEBUG false
#define ASSOC_OP_PLUS(OP) ((OP >= PLUS_DI) && (OP <= PLUS_SF))
#define ASSOC_OP_MULT(OP) ((OP >= MULT_DI) && (OP <= MULT_SF))
#define ASSOC_OP(OP) (ASSOC_OP_PLUS(OP) || ASSOC_OP_MULT(OP))
#define ICG_REG_REG(OP) ((OP >= REG_DI) && (OP<=REG_SF))
#define ICG_REG_REGX(OP) ((OP >= REGX_DI) && (OP<=REGX_SF))
#define ICG_REG(OP) ((OP>=REG_DI) && (OP<=REGX_SF))


/* Code to reassociate expressions in a tree based on the Extended
 * Sethi-Ullman numbering. A node in the expression tree is associative
 * if it is a part of an associative expression (+,*). A node in the
 * expression tree is said to be the root of an associative tree if
 * it is associative and starts a new associative expression. 
 */ 

/* This data structure stores the associative information for a
 * node in the expression tree.
 */

typedef struct _icg_su_reassoc
{
  /* This is the id of a icg_node which is its dfs number
   * in the original expression tree
   */
  unsigned dfs_number;

  /* Which rtl expr does this node belong to ? */
  icg_node *which_expr;

  /* Who was its original parent ? */
  icg_node *orig_parent;
  unsigned orig_parent_dfs;

  /* Who is its associative parent, if any ? */
  icg_node*assoc_parent;
  unsigned assoc_parent_dfs;

  /*
   * Pointers to all its associative operators,
   * defined only for roots of the assoc tree. This
   * is used while rewiring the expressions.
   */
  unsigned int *expr_children;
  unsigned int *orig_expr_children;
  unsigned int num_expr_children;

  /*
   * Pointers to all its associative children,
   * only for root of the assoc tree. Used to determine
   * the new order of evaluation of expressions.
   */
  unsigned int *assoc_children;
  unsigned int *orig_assoc_children;
  /* Integer to point to number of associative children. */
  unsigned num_assoc_children;

  /* Is it associative ? - Yes, if it belongs to an associative tree. */
  bool is_associative;

  /* Is it the associative root ? - Yes, if it starts an associative tree. */
  bool is_associative_root;

  /* This flag is set if a new associative order is formed. */
  bool association_changed;

  /* SU-pair values for this node */
  unsigned su_extra;
  unsigned su_freed;
}icg_su_reassoc;

static long long num_icg_expr=0;
static icg_su_reassoc *su_reassoc;

static int
dump_icg_construct(void)
{
  unsigned i=1;
  if(!dump_file)
    return 0;
  fprintf(dump_file, "\n" "Dumping icg_construct\n");
  fprintf(dump_file, "num_icg_expr=%lld\n", num_icg_expr);
  for(i=1;i<num_icg_expr+1;i++)
  {
    fprintf(dump_file, "%u : %u,%u,%u,%d\n", i, su_reassoc[i].dfs_number,
               su_reassoc[i].orig_parent_dfs, su_reassoc[i].assoc_parent_dfs,
                                    su_reassoc[i].which_expr->op);
    if(su_reassoc[i].is_associative_root)
    {
      unsigned int j;
      fprintf(dump_file, "Root : ");
      for(j=0;j<su_reassoc[i].num_assoc_children;j++)
      {
        unsigned k = su_reassoc[i].assoc_children[j];
        fprintf(dump_file, "(%u,%u,%u,%d)  ", su_reassoc[k].dfs_number,
                              su_reassoc[k].orig_parent_dfs, su_reassoc[k].assoc_parent_dfs,
                               su_reassoc[k].which_expr->op);

      }
      fprintf(dump_file, "\n");
      fprintf(dump_file, "assoc_children : [");
      for(j=0;j<su_reassoc[i].num_assoc_children;j++)
      {
        fprintf(dump_file, "(%u,%u) ", su_reassoc[i].assoc_children[j],
                                      su_reassoc[i].orig_assoc_children[j]);
      }
      fprintf(dump_file, " ]\n");
    }
  }
  fprintf(dump_file, "\n" "End of Dumping icg_construct\n");
  return 0;
}

/* This function counts the number of nodes in a expression tree.
 */
static long long
count_occurrences_of_icg_expr(icg_node *node)
{
  long long count=1;
  if(node->left) {
    count += count_occurrences_of_icg_expr(node->left);
  }
  if(node->right) {
    count += count_occurrences_of_icg_expr(node->right);
  }
  return count;
}

/* Rewiring of the expressions based on reassociation is done here.
 */
static bool
su_reconstruct_expr_tree(icg_su_reassoc *assoc_root)
{
  unsigned i, j;
  unsigned *assoc_children = assoc_root->assoc_children;
  unsigned num_assoc_children = assoc_root->num_assoc_children;

  unsigned *expr_children = assoc_root->expr_children;
  unsigned num_expr_children = assoc_root->num_expr_children;
  icg_node *expr_src = 0;

  if ((num_expr_children + 1)!= (num_assoc_children-1)) {
    fprintf(dump_file, "\n" "Inconsistency between expr and children.  ABORT!\n");
    exit(0);
  }

  /* Rewire the operators and operands */
  expr_src=assoc_root->which_expr;
  expr_src->right =
         su_reassoc[assoc_children[num_assoc_children-1]].which_expr;
  /* Rewire the operators first */
  for(i=0;i<num_expr_children;i++) {
    icg_node *expr_dest=su_reassoc[expr_children[i]].which_expr;
    expr_src->left=expr_dest;
    expr_src=expr_dest;
  }
  /* Wire the operands now */
  expr_src->left=su_reassoc[assoc_children[0]].which_expr;
  for(i=1, j=num_expr_children-1;i<=num_assoc_children-2;i++, j--) {
    expr_src=su_reassoc[expr_children[j]].which_expr;
    expr_src->right=su_reassoc[assoc_children[i]].which_expr;
  }
  return true;
}

/* The new order of expressions based on (extra,freed) tuple is
 * determined here.
 */
static bool
su_reassociate_expressions(icg_su_reassoc *assoc_root)
{
  unsigned i=0, j=0;
  unsigned *assoc_children = assoc_root->assoc_children;
  unsigned num_assoc_children = assoc_root->num_assoc_children;
  if(num_assoc_children <= 2) return false;
  /* Sort O(n^2) on extra in ascending order resolving ties through
   * freed in descending
   */
  for(i = 0;i < (num_assoc_children - 1); i++) {
    icg_su_reassoc *ptr1 = &su_reassoc[assoc_children[i]];
    for(j = i + 1;j < num_assoc_children;j++) {
       icg_su_reassoc *ptr2 = &su_reassoc[assoc_children[j]];
       if((ptr1->su_extra > ptr2->su_extra) ||
          ((ptr1->su_extra == ptr2->su_extra) &&
           (ptr1->su_freed < ptr2->su_freed))) {
         unsigned tmp = assoc_children[i];
         assoc_children[i] = assoc_children[j];
         assoc_children[j]=tmp;
         ptr1=ptr2;
       }
    }
  }
  /* Now, do placement, refer to wiki  */
  for(i=(num_assoc_children -1) ;i>=1;i--) {
    for(j=0;j<i;j++) {
      icg_su_reassoc *ptr=&su_reassoc[assoc_children[j]];
      if(ptr->su_freed > 0) break;
    }
    if(j==i) {
      unsigned tmp=assoc_children[i];
      assoc_children[i]=assoc_children[0];
      assoc_children[0]=tmp;
      break;
    }
  }
  su_reconstruct_expr_tree(assoc_root);
  return true;
}

static unsigned
for_each_icg_assoc_construct(icg_node *icg_expr, unsigned parent_dfs_number)
{
  /* Structures to hold su_pairs of children*/
  struct _su_pair_
  {
    unsigned su_extra;
    unsigned su_freed;
  } su_pair_kids[2];
  struct _su_pair_ *left_val, *right_val;
  unsigned cost_left, cost_right;  
  unsigned num_kids=0, child_dfs=0;

  static unsigned dfs_number=0;
  unsigned this_dfs_number;
  icg_su_reassoc *this_ptr=NULL;

  gcc_assert(icg_expr!=NULL);

  /* parent_dfs_number == 0 implies it is the root of the expression
   * tree.
   */ 
  if(parent_dfs_number==0)
  {
    dfs_number=0;
    parent_dfs_number=1;
  }

  dfs_number++;
  this_dfs_number=dfs_number;

  /*
   * The id of a node is its dfs number in the original tree, id may not
   * match the dfs  number if reassociation happens
   */
  this_ptr=&su_reassoc[this_dfs_number];
  icg_expr->id=this_dfs_number;

  this_ptr->which_expr=icg_expr;
  this_ptr->orig_parent=
                su_reassoc[parent_dfs_number].which_expr;
  this_ptr->orig_parent_dfs=parent_dfs_number;
  this_ptr->dfs_number=this_dfs_number;
  this_ptr->assoc_children=this_ptr->expr_children=NULL;
  this_ptr->orig_assoc_children=this_ptr->orig_expr_children=NULL;
  this_ptr->num_expr_children=this_ptr->num_assoc_children=0;
  this_ptr->su_extra=this_ptr->su_freed=0;

  /* Normally, I am my own associative parent & I dont belong
   *  to any associative tree
   */
  this_ptr->assoc_parent=icg_expr;
  this_ptr->assoc_parent_dfs=this_dfs_number;
  this_ptr->is_associative=false;
  this_ptr->is_associative_root=false;

  /* Find associative children and roots of associative trees */
  if(ASSOC_OP(icg_expr->op)) {
    this_ptr->is_associative=true;
    if(this_dfs_number == parent_dfs_number) {
      /* This is the root of the expression tree  */
      this_ptr->is_associative_root=true;
    }
    else  {
      /* If the opcode of this node does not match the opcode of its
       * parent it is an associative root.
       */
      if(icg_expr->op!=this_ptr->orig_parent->op)
        this_ptr->is_associative_root=true;
    }
  }

  /* Update associative parents which are roots of the associative
   * tree
   */
  if((this_dfs_number != parent_dfs_number) &&
            su_reassoc[parent_dfs_number].is_associative)  {
    if(su_reassoc[parent_dfs_number].is_associative_root) {
      this_ptr->assoc_parent=this_ptr->orig_parent;
      this_ptr->assoc_parent_dfs=parent_dfs_number;
    }
    else {
      this_ptr->assoc_parent=su_reassoc[parent_dfs_number].assoc_parent;
      this_ptr->assoc_parent_dfs=su_reassoc[parent_dfs_number].assoc_parent_dfs;
    }
  }

  /* Populate the expr_children (addresses of operators) of the 
   * associative root node. These will be later used when rewiring
   * the operands.
   */
  if(this_ptr->is_associative && !this_ptr->is_associative_root) {
    /* Add this as an expr child.*/
    unsigned int assoc_parent_dfs=this_ptr->assoc_parent_dfs;
    unsigned int *expr_children=
              su_reassoc[assoc_parent_dfs].expr_children;
    unsigned int *orig_expr_children=
              su_reassoc[assoc_parent_dfs].orig_expr_children;
    unsigned int *num_expr_children = NULL;
    if(expr_children==NULL) {
      expr_children=(unsigned *)icg_calloc((num_icg_expr+1) *
                                            sizeof(unsigned));
      orig_expr_children=(unsigned *)icg_calloc((num_icg_expr+1) *
                                                 sizeof(unsigned));
      su_reassoc[assoc_parent_dfs].expr_children=expr_children;
      su_reassoc[assoc_parent_dfs].orig_expr_children=orig_expr_children;
    }
    num_expr_children = &su_reassoc[assoc_parent_dfs].num_expr_children;
    expr_children[*num_expr_children]=this_dfs_number;
    orig_expr_children[*num_expr_children]=this_dfs_number;
    (*num_expr_children)++;
  }

  /* Populate the assoc_children of the associative_root parent node
   * if necessary
   */
  if(su_reassoc[parent_dfs_number].is_associative &&
    (!this_ptr->is_associative || this_ptr->is_associative_root)) {
    unsigned int *num_assoc_children=NULL;
    /* Add yourself as an assoc child to your associative parent */
    unsigned int assoc_parent_dfs=this_ptr->assoc_parent_dfs;
    unsigned int *assoc_children=
                  su_reassoc[assoc_parent_dfs].assoc_children;
    unsigned int *orig_assoc_children=
                  su_reassoc[assoc_parent_dfs].orig_assoc_children;
    if(assoc_children==NULL) {
      assoc_children=(unsigned *)icg_calloc((num_icg_expr+1) *
                                             sizeof(unsigned));
      orig_assoc_children=(unsigned *)icg_calloc((num_icg_expr+1) *
                                                  sizeof(unsigned));
      su_reassoc[assoc_parent_dfs].assoc_children=assoc_children;
      su_reassoc[assoc_parent_dfs].orig_assoc_children=orig_assoc_children;
    }
    num_assoc_children= &su_reassoc[assoc_parent_dfs].num_assoc_children;
    assoc_children[*num_assoc_children]=this_dfs_number;
    orig_assoc_children[*num_assoc_children]=this_dfs_number;
    (*num_assoc_children)++;
  }

  /* Recurse on the left and right children */
  if(icg_expr->left) {
     child_dfs=for_each_icg_assoc_construct(icg_expr->left, this_dfs_number);
     su_pair_kids[0].su_extra=su_reassoc[child_dfs].su_extra;
     su_pair_kids[0].su_freed=su_reassoc[child_dfs].su_freed;
     num_kids++;
  }
  if(icg_expr->right){
     child_dfs=for_each_icg_assoc_construct(icg_expr->right, this_dfs_number);
     su_pair_kids[1].su_extra=su_reassoc[child_dfs].su_extra;
     su_pair_kids[1].su_freed=su_reassoc[child_dfs].su_freed;
     num_kids++;
  }
  if(num_kids == 0) {   /* This is a leaf */
     /* If this is a register that need not be preserved 
      * then set free to be 1.
      */
     if(ICG_REG_REGX(icg_expr->op)) {
       this_ptr->su_freed = 1;
     }
     else if(this_dfs_number == (parent_dfs_number + 1)) {
       /* This is a left child of its parent */
       this_ptr->su_extra = 1;
     }
  }
  else if(num_kids == 1) {
     /* Simply propogate it upward after adjusting r-node*/
     left_val=&su_pair_kids[0];
     /* Adjust r-node to be (1, 0) */
     if(left_val->su_extra==0 && left_val->su_freed==0)
        left_val->su_extra=1;
     this_ptr->su_extra=left_val->su_extra;
     this_ptr->su_freed=left_val->su_freed;
  }
  else if(num_kids == 2) {
     /* Use Preston's algorithm in the wiki */
     left_val=&su_pair_kids[0];
     right_val=&su_pair_kids[1];
     /* Adjust r-node to be (1, 0) */
     if(left_val->su_extra==0 && left_val->su_freed==0)
        left_val->su_extra=1;
     cost_left =
        (left_val->su_extra + left_val->su_freed) >right_val->su_extra ?
        left_val->su_extra :
        (right_val->su_extra -left_val->su_freed + 1);
     cost_right =
        (right_val->su_extra + right_val->su_freed) > left_val->su_extra ?
        right_val->su_extra :
        (left_val->su_extra -right_val->su_freed + 1);
     this_ptr->su_extra = (cost_left <= cost_right)?cost_left : cost_right;
     this_ptr->su_freed = left_val->su_freed + right_val->su_freed;
  }
  if(this_ptr->is_associative_root) {
     this_ptr->association_changed =
        su_reassociate_expressions(this_ptr);
  }
  return this_dfs_number;
}

static int icg_assoc_tree_construct(icg_node *curr_node)
{
  if(dump_file && REASSOC_DEBUG) {
    fprintf(dump_file,"\n" "Before ReAssociation ---> \n");
    print_icg_tree (dump_file, curr_node, "\n" "icg_expr :");
  }
  num_icg_expr=count_occurrences_of_icg_expr(curr_node);
  su_reassoc=(icg_su_reassoc *)(icg_calloc)
                        ((num_icg_expr+1)*sizeof(icg_su_reassoc));
  for_each_icg_assoc_construct(curr_node, 0);
  if(dump_file && REASSOC_DEBUG)
    dump_icg_construct();
  return 0;
}

static void print_dot(FILE *dotfile, rtx curr_insn);

void icg_reassociate(void)
{
  basic_block bb;
  rtx curr_insn;
  static int callnum=0;
  char dotfile_name[100];
  FILE *dotfile;

  /* To turn on icg_reassociate do "touch reassociate" */
  char *do_reassociate = getenv("ICG_REASSOC");
  if(do_reassociate) {
    if(strcmp(do_reassociate,"true") !=0 ) {
       return;
     }
  }
  else {
    return;
  }
  
  if(dump_file)
    fprintf(dump_file,"Turning on reassociation \n"); 
  if(REASSOC_DEBUG)
    fprintf(stderr,"Turning on reassociation \n"); 

  sprintf(dotfile_name,"icg_assoc_%d.dot",callnum);

  dotfile=fopen(dotfile_name, "w");
  callnum++;
  if(dump_file && REASSOC_DEBUG)
    fprintf(dump_file, "\n\n" "ReAssociation Starts\n\n");

  FOR_EACH_BB(bb)
  {
    FOR_BB_INSNS(bb, curr_insn)
    {
      if(!INSN_P(curr_insn))
        continue;
      if (!icg_insn2dirty[INSN_UID(curr_insn)])
        continue;

      if(dump_file && REASSOC_DEBUG) {
        fprintf(dump_file, "\n" "****************Source file, line : %s, %d\n",
                                    insn_file(curr_insn), insn_line(curr_insn));
        print_inline_rtx(dump_file, PATTERN(curr_insn), 2);
      }
      icg_assoc_tree_construct(icg_insn2tree[INSN_UID(curr_insn)]);
      if(dump_file && REASSOC_DEBUG) {
        fprintf(dump_file, "\n" "After Reassociation ---> \n");
        print_icg_tree (dump_file, icg_insn2tree[INSN_UID(curr_insn)] , "\n" "icg_expr :");
        fprintf(dump_file, "\n");
        print_dot(dotfile, curr_insn);
      }
    }
  }
  if(dump_file && REASSOC_DEBUG) {
    fprintf(dump_file, "\n\n" "ReAssociation Completed\n\n");
  }
  fclose(dotfile);
}

/* Dot dumper for the associative tree */
static int
for_each_dot_icg (FILE *dotfile, icg_node *curr_node, int parent, int isleft)
{
  /* Is 100 characters enough ?? */
  char node_string[100];
  icg_su_reassoc *this_ptr=NULL;

  if (curr_node == NULL)
    return 0;
  this_ptr=&su_reassoc[curr_node->id];
  if (ASSOC_OP_PLUS(curr_node->op))
    snprintf(node_string, sizeof(node_string), "PLUS");
  else if (ASSOC_OP_MULT(curr_node->op))
    snprintf(node_string, sizeof(node_string), "MULT");
  else if (ICG_REG_REG(curr_node->op))
    snprintf(node_string, sizeof(node_string), "REG : %d ", curr_node->r);
  else if (ICG_REG_REGX(curr_node->op))
    snprintf(node_string, sizeof(node_string), "REGX : %d ", curr_node->rx);
  else
    snprintf(node_string, sizeof(node_string), "%s", get_icg_opcode_name(curr_node->op));

  if (this_ptr->is_associative_root) {
    const size_t lg = strlen(node_string);
    snprintf(node_string+lg, sizeof(node_string)-lg, "(E)");
  }

  {
    const size_t lg = strlen(node_string);
    snprintf(node_string+lg, sizeof(node_string)-lg, " (%d,%d)(%d) [%d]",
			     this_ptr->su_extra, this_ptr->su_freed,
			     this_ptr->num_assoc_children, curr_node->id);
  }

  if (this_ptr->is_associative && !this_ptr->is_associative_root) {
    fprintf(dotfile, "n%d [label=\"%s\", shape=box];\n",
      curr_node->id, node_string);
  }
  else if (this_ptr->is_associative_root && this_ptr->association_changed) {
    fprintf(dotfile, "n%d [label=\"%s\", shape=invtriangle];\n",
      curr_node->id, node_string);
  } else {
    fprintf(dotfile, "n%d [label=\"%s\"];\n", curr_node->id, node_string);
  }
  if (isleft < 2) {
    if (isleft) {
      fprintf(dotfile, "n%d -- n%d [style=bold, label=\"left\"];\n",
	parent, curr_node->id);
    } else {
      fprintf(dotfile, "n%d -- n%d [style=bold, label=\"right\"];\n",
	parent, curr_node->id);
    }
  }
  if (curr_node->left) {
    for_each_dot_icg(dotfile, curr_node->left, curr_node->id, 1);
  }
  if (curr_node->right) {
    for_each_dot_icg(dotfile, curr_node->right, curr_node->id, 0);
  }

  if (this_ptr->is_associative_root) {
    unsigned int i=0;
    unsigned int *expr_children=this_ptr->expr_children;
    unsigned int num_expr_children=this_ptr->num_expr_children;
    for(i=0;i < num_expr_children; i++) {
      unsigned child_id=expr_children[i];
      fprintf(dotfile, "n%d -- n%d [stlye=dotted, color=green, label=\"%d\"];\n",
                                                curr_node->id, child_id, i);
    }
  }
  if (this_ptr->is_associative_root) {
    unsigned int i=0;
    unsigned int *assoc_children=this_ptr->assoc_children;
    unsigned int *orig_assoc_children=this_ptr->orig_assoc_children;
    unsigned int num_assoc_children=this_ptr->num_assoc_children;
    for(i=0;i < num_assoc_children; i++) {
      unsigned child_id=assoc_children[i];
      unsigned orig_child_id=orig_assoc_children[i];
      fprintf(dotfile, "n%d -- n%d [stlye=dotted, color=red, label=\"%d\"];\n",
                                                curr_node->id, child_id, i);
      if (child_id !=orig_child_id) {
        fprintf(dotfile, "n%d -- n%d [stlye=dotted, color=blue, label=\"%d\"];\n",
                                                curr_node->id, orig_child_id, i);
      }
    }
  }

  return 0;
}

static void print_dot(FILE *dotfile, rtx  curr_insn)
{
  icg_node *curr_node = NULL;
  fprintf(dotfile, "\n" "graph graph_%d {\n", INSN_UID(curr_insn));
  curr_node=icg_insn2tree[INSN_UID(curr_insn)];
  for_each_dot_icg(dotfile, curr_node, 0, 2);
  fprintf(dotfile, "}\n");
}
