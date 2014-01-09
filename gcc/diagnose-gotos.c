/*
   Diagnose branches to/from structured pragma regions
   based on pass_diagnose_omp_blocks

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

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "tree.h"
#include "alias.h"
#include "gimple.h"
#include "tree-iterator.h"
#include "gimple-iterator.h"
#include "gimple-walk.h"
#include "splay-tree.h"
#include "diagnose-gotos.h"

#define WALK_SUBSTMTS  \
  case GIMPLE_BIND: \
case GIMPLE_TRY: \
case GIMPLE_CATCH: \
case GIMPLE_EH_FILTER: \
case GIMPLE_TRANSACTION: \
/* The sub-statements for these should be walked.  */ \
*handled_ops_p = false; \
break;

static splay_tree all_labels;

struct diagnose_context
{
  gimple stmt;
  vec<enum gimple_code>* codes;
  recursive_seq_callback rcb;
  diagnose_context_callback dcb;
};

/* Pass 1: Create a map of each label definitions  */

static tree
diagnose_sb_1 (gimple_stmt_iterator *gsi_p, bool *handled_ops_p,
               struct walk_stmt_info *wi)
{
  struct diagnose_context* context = (struct diagnose_context*) wi->info;
  gimple save_context;
  gimple stmt = gsi_stmt (*gsi_p);

  *handled_ops_p = true;

  switch (gimple_code (stmt))
    {
      WALK_SUBSTMTS;

    case GIMPLE_LABEL:
      splay_tree_insert (all_labels, (splay_tree_key) gimple_label_label (stmt),
                         (splay_tree_value) context->stmt);
      break;

    default:
    {
      unsigned i, j;
      vec<enum gimple_code>* codes = context->codes;

      for(i = 0; i < codes->length(); ++i)
        {
          if((*codes)[i] == gimple_code(stmt))
            {
              break;
            }
        }
      if(i < codes->length())
        {
          vec<gimple_seq*> recurse;
          recurse.create(3);
          context->rcb((*codes)[i], stmt, &recurse);

          save_context = context->stmt;
          context->stmt = stmt;
          for(j = 0; j < recurse.length(); ++j)
            {
              gimple_seq seq = *recurse[j];
              walk_gimple_seq(seq, diagnose_sb_1, NULL, wi);
            }
          context->stmt = save_context;
        }
    }
    break;
    }

  return NULL_TREE;
}

/* Pass 2: Check each branch and see if its context differs from that of
   the destination label's context.  */

static tree
diagnose_sb_2 (gimple_stmt_iterator *gsi_p, bool *handled_ops_p,
               struct walk_stmt_info *wi)
{
  struct diagnose_context* context = (struct diagnose_context*) wi->info;
  gimple save_context;
  splay_tree_node n;
  gimple stmt = gsi_stmt (*gsi_p);

  *handled_ops_p = true;

  switch (gimple_code (stmt))
    {
      WALK_SUBSTMTS;

    case GIMPLE_COND:
    {
      tree lab = gimple_cond_true_label (stmt);
      if (lab)
        {
          n = splay_tree_lookup (all_labels,
                                 (splay_tree_key) lab);
          context->dcb (gsi_p, context->stmt,
                        n ? (gimple) n->value : NULL);
        }
      lab = gimple_cond_false_label (stmt);
      if (lab)
        {
          n = splay_tree_lookup (all_labels,
                                 (splay_tree_key) lab);
          context->dcb (gsi_p, context->stmt,
                        n ? (gimple) n->value : NULL);
        }
    }
    break;

    case GIMPLE_GOTO:
    {
      tree lab = gimple_goto_dest (stmt);
      if (TREE_CODE (lab) != LABEL_DECL)
        break;

      n = splay_tree_lookup (all_labels, (splay_tree_key) lab);
      context->dcb (gsi_p, context->stmt, n ? (gimple) n->value : NULL);
    }
    break;

    case GIMPLE_SWITCH:
    {
      unsigned int i;
      for (i = 0; i < gimple_switch_num_labels (stmt); ++i)
        {
          tree lab = CASE_LABEL (gimple_switch_label (stmt, i));
          n = splay_tree_lookup (all_labels, (splay_tree_key) lab);
          if (n && context->dcb (gsi_p, context->stmt, (gimple) n->value))
            break;
        }
    }
    break;

    case GIMPLE_RETURN:
      context->dcb (gsi_p, context->stmt, NULL);
      break;

    default:
    {
      unsigned i, j;
      vec<enum gimple_code>* codes = context->codes;

      for(i = 0; i < codes->length(); ++i)
        {
          if((*codes)[i] == gimple_code(stmt))
            {
              break;
            }
        }
      if(i < codes->length())
        {
          vec<gimple_seq*> recurse;
          recurse.create(3);
          context->rcb((*codes)[i], stmt, &recurse);

          save_context = context->stmt;
          context->stmt = stmt;
          for(j = 0; j < recurse.length(); ++j)
            {
              gimple_seq* seq = recurse[j];
              walk_gimple_seq_mod(seq, diagnose_sb_2, NULL, wi);
            }
          context->stmt = save_context;
        }
    }
    break;
    }

  return NULL_TREE;
}


/*
   Perform validation of branches to/from structured pragma blocks.
   CODES is a vec of interesting codes,
   RCB and DCB - callback pointers.
 */
void
diagnose_gotos(vec<enum gimple_code>* codes, recursive_seq_callback rcb,
               diagnose_context_callback dcb)
{
  struct diagnose_context ctx;
  struct walk_stmt_info wi;
  gimple_seq body = gimple_body (current_function_decl);

  all_labels = splay_tree_new (splay_tree_compare_pointers, 0, 0);

  memset(&ctx, 0, sizeof(ctx));
  ctx.codes = codes;
  ctx.rcb = rcb;
  ctx.dcb = dcb;

  memset (&wi, 0, sizeof (wi));
  wi.info = &ctx;
  walk_gimple_seq (body, diagnose_sb_1, NULL, &wi);

  memset (&wi, 0, sizeof (wi));
  wi.want_locations = true;
  wi.info = &ctx;
  walk_gimple_seq_mod (&body, diagnose_sb_2, NULL, &wi);

  gimple_set_body (current_function_decl, body);

  splay_tree_delete (all_labels);
  all_labels = NULL;
}

