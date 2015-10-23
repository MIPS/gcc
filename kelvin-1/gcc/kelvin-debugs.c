
/*
 * I'm just going to include this source file into the module that I
 * want to debug.  Presumably, that outer module will provide all of
 * the necessary includes.

#include <stdio.h>


#include "system.h"
#include "coretypes.h"

#include "basic-block.h"
#include "et-forest.h"
#include "cfgloop.h"
*/

#include "pretty-print.h"
#include "et-forest.h"
#include "gimple.h"
#include "input.h"
#include "line-map.h"

typedef const rtx_insn *const_ins;


/* note: rtl.h declares:
 *  extern void print_insn(pretty_printer *, const_rtx, int)
 * but that resolves to:
 *  extern void print_insn(pretty_printer *, rtx_def const *, int)
 * and rtx_def is a superclass of rtx_insn.  This results in an
 * unresolved reference at link time.
 *
 * extern void print_insn(pretty_printer *, const rtx_insn *, int);
 */

static void kdn_dump_loop_id(FILE *, loop_p);
static void kdn_dump_loop(FILE *, loop_p);
static void kdn_dump_block_id(FILE *, basic_block);
static void kdn_dump_block(FILE *, basic_block);
static void kdn_dump_block_flow(FILE *, int, basic_block);
static void kdn_dump_bb_flags(FILE *, int);
static void kdn_dump_edge_flags(FILE *, int);
static void kdn_dump_edge(FILE *, edge, bool, bool);
static void kdn_dump_rtl_instruction_list(FILE *,
					  const_rtx, struct rtl_bb_info *);
static void kdn_dump_gimple_instruction_list(FILE *,
					     struct gimple_bb_info gimple);
static void kdn_dump_rtl_insns(FILE *, const_rtx);
static void kdn_dump_gimple_insns(FILE *, gimple_seq);
static void kdn_dump_loop_exits(FILE *stream, struct loop_exit *);
static void kdn_dump_niter_desc(FILE *, struct niter_desc *);
static void kdn_dump_dominator(FILE *, struct et_node *);
static void kdn_treedump_children(FILE *, int, struct et_node *);
static void kdn_dump_wide_int(FILE *stream, const widest_int &value);
static void kdn_dump_all_blocks(FILE *stream, loop_p loop_ptr);

static void kdn_dump_loop_id(FILE *stream, loop_p loop_ptr)
{
  if (loop_ptr == NULL)
    fprintf(stream, "NULL\n");
  else
    fprintf(stream, " %d\n", loop_ptr->num);
}

static void kdn_dump_all_blocks(FILE *stream, loop_p loop_ptr) {
  /* find some instruction contained within the loop */
  if (loop_ptr->header == NULL) {
    fprintf(stream,
	    "Could not dump all blocks because loop_ptr->header is NULL.\n");
  } else if (loop_ptr->num_nodes <= 0) {
    fprintf(stream,
	    "Could not dump all blocks because loop has no nodes.\n");
  } else {
    basic_block *body, a_block;
    const_rtx the_head;
    struct rtl_bb_info *the_rtl;
    const rtx_insn *an_instruction;

    body = get_loop_body(loop_ptr);
    a_block = body[0];

    the_head = a_block->il.x.head_;
    the_rtl = a_block->il.x.rtl;

    if (the_head != NULL) {
      fprintf(stream,
	      "Dumping all basic blocks of the method:\n");

      an_instruction = as_a <const rtx_insn *> (the_head);

      /* walk through the list of instructions until you find the first
       *  insruction in the method.  This is the one for which the previous
       *  index value is zero.
       */
      while (PREV_INSN(an_instruction) != NULL) {
	an_instruction = PREV_INSN(an_instruction);
      }

      /* At this point, an_instruction points to first instruction
       * of the method.
       */
      /* while our focus is not yet at the end of the method's instruction
       * list:
       *  print out the contents of one basic block
       *  advance the focus to the instruction that follows the
       *   basic block just printed out.
       */

      while (an_instruction != NULL) {
	int loop_is_done = FALSE;
	int seen_one_basic_block_note = FALSE;
	basic_block the_block;
	
	fprintf(stream, "\nContents of one basic block:\n");
	while ((an_instruction != NULL) && !loop_is_done) {
	  /*
	   * instead of printing out instructions one at a time,
	   * i'll just skip over the instructions of each basic block
	   * while remembering the basic block information.
	   *
	   * print_rtl_single(stream, an_instruction);
	   */
	  
	  the_block = BLOCK_FOR_INSN(an_instruction);
	  if (NOTE_INSN_BASIC_BLOCK_P(an_instruction)) {
	    seen_one_basic_block_note = TRUE;
	  }
	  else {
	    switch (GET_CODE(an_instruction)) {
	      
	    case JUMP_INSN:
	      loop_is_done = TRUE;
	      break;
	  
	    case CALL_INSN:  /* not sure if CALL_INSN terminates a bb */
	      /* this code was copied from cfgbuild.c */
	      /* Noreturn and sibling call instructions terminate the
	       * basic blocks (but only if they happen unconditionally).
	       */
	      if ((SIBLING_CALL_P (an_instruction)
		   || find_reg_note (an_instruction, REG_NORETURN, 0))
		  && GET_CODE (PATTERN (an_instruction)) != COND_EXEC) {
		loop_is_done = TRUE;
		break;
	      }
	      
	    default:			/* in other cases, do nothing */
	      break;
	    }
	  }
	  
	  an_instruction = NEXT_INSN(an_instruction);
	  
	  if (an_instruction != NULL) {
	    if (NOTE_INSN_BASIC_BLOCK_P(an_instruction)) {
	      if (seen_one_basic_block_note) {
		loop_is_done = TRUE;
	      }
	    } else if (GET_CODE(an_instruction) == CODE_LABEL) {
	      if (seen_one_basic_block_note) {
		loop_is_done = TRUE;
	      }
	    }
	  }
	}
	kdn_dump_block(stream, the_block);
	fprintf(stream, "End of this basic block.\n");
      }
      fprintf(stream, "End of basic block's head:\n");
    }
  }
}

static void kdn_dump_loop(FILE *stream, loop_p loop_ptr) {
  
  fprintf(stream, " %d\n", loop_ptr->num);
  fprintf(stream, " number of instructions: %d\n", loop_ptr->ninsns);
  fprintf(stream, " number of blocks within loop: %d\n", loop_ptr->num_nodes);
  fprintf(stream, " average instructions per iteration: %d\n",
	  loop_ptr->av_ninsns);
  
  if (loop_ptr->any_upper_bound) {
    fprintf(stream,
	    " upper bound on number of times loop latch executes: ");
    kdn_dump_wide_int(stream, loop_ptr->nb_iterations_upper_bound);
  }
  
  if (loop_ptr->any_estimate) {
    fprintf(stream,
	    " estimate on number of times loop latch executes: ");
    kdn_dump_wide_int(stream, loop_ptr->nb_iterations_estimate);
  }

  fprintf(stream, " loop can be parallel: %d\n", loop_ptr->can_be_parallel);
  fprintf(stream, " warned of aggressive optimizations: %d\n",
	  loop_ptr->warned_aggressive_loop_optimizations);

  fprintf(stream, " force_vectorize: %d, dont_vectorize: %d\n",
	  loop_ptr->force_vectorize, loop_ptr->dont_vectorize);

  if (loop_ptr->inner != NULL) {
    fprintf(stream, " inner_loop: ");
    kdn_dump_loop_id(stream, loop_ptr->inner);
  }

  fprintf(stream, " outer_loops:\n");
  if (loop_ptr->superloops) {
    for (unsigned int i = 0; i < vec_safe_length(loop_ptr->superloops); i++) {
      fprintf(stream, "   ");
      kdn_dump_loop_id(stream, (*loop_ptr->superloops)[i]);
    }
  }

  fprintf(stream, " next loop is: ");
  kdn_dump_loop_id(stream, loop_ptr->next);

  fprintf(stream, " loop header: ");
  kdn_dump_block_id(stream, loop_ptr->header);

  fprintf(stream, " former header: ");
  kdn_dump_block_id(stream, loop_ptr->former_header);

  fprintf(stream, " loop latch: ");
  kdn_dump_block_id(stream, loop_ptr->latch);

  fprintf(stream, " loop exits: ");
  kdn_dump_loop_exits(stream, loop_ptr->exits);

  fprintf(stream, " loop iteration information\n");
  kdn_dump_niter_desc(stream, loop_ptr->simple_loop_desc);

#define DUMP_ENTIRE_BLOCK
#ifdef DUMP_ENTIRE_BLOCK
  kdn_dump_all_blocks(stream, loop_ptr);
#endif

  fprintf(stream,
	  "Not reported: aux, nb_iterations, safelen, estimate_state,");
  fprintf(stream,
	  " simduid bounds, control_ivs, lpt_decision\n");
}

static void kdn_dump_block_id(FILE *stream, basic_block block) {
  if (block == NULL)
    fprintf(stream, "Null Basic Block\n");
  else
    fprintf(stream, "Basic Block No: %d\n", block->index);
}

/* Dump this block and all of its successor blocks transitively up
 * to the num_levels distance from block.
 */
static void kdn_dump_block_flow(FILE *stream,
				int num_levels, basic_block block) {
  if (num_levels > 0) {
    kdn_dump_block(stream, block);
    for (unsigned int i = 0; i < EDGE_COUNT(block->succs); i++) {
      kdn_dump_block_flow(stream, num_levels - 1, EDGE_SUCC(block, i)->dest);
    }
  }
}

/* Note: basic_block is "struct basic_block_def *" */
static void kdn_dump_block(FILE *stream, basic_block block) {
  unsigned int i;

  if (block == NULL)
    fprintf(stream, "NULL block\n");
  else {
    fprintf(stream, "Basic block no: %d\n", block->index);

    /* The discriminator for this block.  The discriminator distinguishes
       among several basic blocks that share a common locus, allowing for
       more accurate sample-based profiling.  */
    fprintf(stream, " discriminator: %d\n", block->discriminator);

    if (block->loop_father) {
      fprintf(stream, " inner-most enclosing loop: ");
      kdn_dump_loop_id(stream, block->loop_father);
    }

    /* enumerate from cfg-flags.def */
    fprintf(stream, "  flags: ");
    kdn_dump_bb_flags(stream, block->flags);
    fprintf(stream, "  count: %ld\n", (long int) block->count);
    fprintf(stream, "  frequency: %d\n", block->frequency);
    
    fprintf(stream, "  Dominator information\n");
    kdn_dump_dominator(stream, block->dom[0]);
    
    fprintf(stream, "  Post-dominator information\n");
    kdn_dump_dominator(stream, block->dom[1]);  

    fprintf(stream, " Predecessor edges:\n");
    for (i = 0; i < EDGE_COUNT(block->preds); i++) {
      fprintf(stream, " predecessor[%d]:\n", i);
      kdn_dump_edge(stream, EDGE_PRED(block, i), TRUE, FALSE);
    }
    
    fprintf(stream, " Successor edges:\n");
    for (i = 0; i < EDGE_COUNT(block->succs); i++) {
      fprintf(stream, " successor[%d]:\n", i);
      kdn_dump_edge(stream, EDGE_SUCC(block, i), FALSE, TRUE);
    }
    
    fprintf(stream, " body of block:\n");
    if (block->flags & BB_RTL)
      kdn_dump_rtl_instruction_list(stream,
				    block->il.x.head_, block->il.x.rtl);
    else
      kdn_dump_gimple_instruction_list(stream, block->il.gimple);

    fprintf(stream, "End of info for basic block no: %d\n", block->index);
  }
}

static void kdn_dump_bb_flags(FILE *stream, int flags) {

  /* see cfg-flags.def */
  if (flags & BB_NEW)
    fprintf(stream, "NEW ");
  if (flags & BB_REACHABLE)
    fprintf(stream, "REACHABLE ");
  if (flags & BB_IRREDUCIBLE_LOOP)
    fprintf(stream, "IRREDUCIBLE_LOOP ");
  if (flags & BB_SUPERBLOCK)
    fprintf(stream, "SUPERBLOCK ");
  if (flags & BB_DISABLE_SCHEDULE)
    fprintf(stream, "DISABLE_SCHEDULE ");
  if (flags & BB_HOT_PARTITION)
    fprintf(stream, "HOT_PARTITION ");
  if (flags & BB_COLD_PARTITION)
    fprintf(stream, "COLD_PARTITION ");
  if (flags & BB_DUPLICATED)
    fprintf(stream, "DUPLICATED ");
  if (flags & BB_NON_LOCAL_GOTO_TARGET)
    fprintf(stream, "NON_LOCAL_GOTO_TARGET ");
  if (flags & BB_RTL)
    fprintf(stream, "RTL ");
  if (flags & BB_FORWARDER_BLOCK)
    fprintf(stream, "FORWARDER_BLOCK ");
  if (flags & BB_NONTHREADABLE_BLOCK)
    fprintf(stream, "NONTHREADABLE_BLOCK ");
  if (flags & BB_MODIFIED)
    fprintf(stream, "MODIFIED ");
  if (flags & BB_VISITED)
    fprintf(stream, "VISITED ");
  if (flags & BB_IN_TRANSACTION)
    fprintf(stream, "IN_TRANSACTION ");

  fprintf(stream, "\n");
}

static void kdn_dump_edge_flags(FILE *stream, int flags) {

  /* see cfg-flags.def */
  if (flags & EDGE_FALLTHRU)
    fprintf(stream, "FALLTHRU ");
  if (flags & EDGE_ABNORMAL)
    fprintf(stream, "ABNORMAL ");
  if (flags & EDGE_ABNORMAL_CALL)
    fprintf(stream, "ABNORMAL_CALL ");
  if (flags & EDGE_EH)
    fprintf(stream, "EH ");
  if (flags & EDGE_PRESERVE)
    fprintf(stream, "PRESERVE ");
  if (flags & EDGE_FAKE)
    fprintf(stream, "FAKE ");
  if (flags & EDGE_DFS_BACK)
    fprintf(stream, "DFS_BACK ");
  if (flags & EDGE_IRREDUCIBLE_LOOP)
    fprintf(stream, "IRREDUCIBLE_LOOP ");
  if (flags & EDGE_TRUE_VALUE)
    fprintf(stream, "TRUE_VALUE ");
  if (flags & EDGE_FALSE_VALUE)
    fprintf(stream, "FALSE_VALUE ");
  if (flags & EDGE_EXECUTABLE)
    fprintf(stream, "EXECUTABLE ");
  if (flags & EDGE_CROSSING)
    fprintf(stream, "CROSSING ");
  if (flags & EDGE_SIBCALL)
    fprintf(stream, "SIBCALL ");
  if (flags & EDGE_CAN_FALLTHRU)
    fprintf(stream, "CAN_FALLTHRU ");
  if (flags & EDGE_LOOP_EXIT)
    fprintf(stream, "LOOP_EXIT ");
  if (flags & EDGE_TM_UNINSTRUMENTED)
    fprintf(stream, "TM_UNINSTRUMENTED ");
  if (flags & EDGE_TM_ABORT)
    fprintf(stream, "TM_ABORT ");
  fprintf(stream, "\n");
}

static void kdn_dump_edge(FILE *stream, edge an_edge,
		   bool display_src, bool display_dest) {
  /* note: an_edge is a pointer to struct edge_def */
  if (an_edge == NULL) {
    fprintf(stream, "Null Edge\n");
  } else {
    fprintf(stream,
	    "Edge (B%d->B%d)\n",
	    an_edge->src->index, an_edge->dest->index);
    fprintf(stream, "  flags: ");
    kdn_dump_edge_flags(stream, an_edge->flags);
    fprintf(stream, "  probability: %d\n", an_edge->probability);
    fprintf(stream, "  count: %ld\n", (long int) an_edge->count);

    /*
     * trying to output got_locus information, but it's not working yet.
     */
    fprintf(stream, " goto locus: (file: %s) ",
	    LOCATION_FILE(an_edge->goto_locus));
    fprintf(stream, "(line: %d) ", LOCATION_LINE(an_edge->goto_locus));
    fprintf(stream, "(column: %d)\n", LOCATION_COLUMN(an_edge->goto_locus));

    if (an_edge->flags & EDGE_EXECUTABLE) {
      fprintf(stream, "  GIMPLE instructions:\n");
      kdn_dump_gimple_insns(stream, an_edge->insns.g);
      fprintf(stream, "  End of GIMPLE instructions\n");
    }
    else if (an_edge->insns.r != NULL) {
      fprintf(stream, "  RTL instructions:\n");
      kdn_dump_rtl_insns(stream, an_edge->insns.r);
      fprintf(stream, "  End of RTL instructions\n");
    }
    
    fprintf(stream, "  not reported: dest_idx, aux\n");
  }
}

static void kdn_dump_rtl_instruction_list(FILE *stream,
					  const_rtx head,
					  struct rtl_bb_info *rtl) {
  /* see rtl.h for definition of "class rtx_insn"
   * supports operations such as 
   *  INSN_UID(), NEXT_INSN(), PREV_INSN()
   * Note that a const_rtx does not have a next field.  const_rtx is
   * the "super-class" of rtx_insn.
   */
  const_ins notes_and_jumptables;
  struct pretty_printer buffer;

  pp_needs_newline(&buffer) = true;
  buffer.buffer->stream = stream;

  if (rtl != NULL) {
    int loop_is_done = FALSE;
    int seen_one_basic_block_note = FALSE;

    fprintf(stream, "Basic block's header:\n");
    for (notes_and_jumptables = rtl->header_;
	 (notes_and_jumptables != NULL) && !loop_is_done; ) {

      print_rtl_single(stream, notes_and_jumptables);

      if (NOTE_INSN_BASIC_BLOCK_P(notes_and_jumptables)) {
	seen_one_basic_block_note = TRUE;
      }
      else {

	switch (GET_CODE(notes_and_jumptables)) {

	case JUMP_INSN:
	  loop_is_done = TRUE;
	  break;
	
	case CALL_INSN:
	  /* this code was copied from cfgbuild.c */
	  /* Noreturn and sibling call instructions terminate the basic blocks
	     (but only if they happen unconditionally).  */
	  if ((SIBLING_CALL_P (notes_and_jumptables)
	       || find_reg_note (notes_and_jumptables, REG_NORETURN, 0))
	      && GET_CODE (PATTERN (notes_and_jumptables)) != COND_EXEC) {
	    loop_is_done = TRUE;
	    break;
	  }
	  
	default:			/* in other cases, do nothing */
	  break;
	}
      }

      notes_and_jumptables = NEXT_INSN(notes_and_jumptables);

      if (notes_and_jumptables != NULL) {
	if (NOTE_INSN_BASIC_BLOCK_P(notes_and_jumptables)) {
	  if (seen_one_basic_block_note) {
	    loop_is_done = TRUE;
	  }
	} else if (GET_CODE(notes_and_jumptables) == CODE_LABEL) {
	  if (seen_one_basic_block_note) {
	    loop_is_done = TRUE;
	  }
	}
      }
    }
    fprintf(stream, "End of basic block's header\n");
  }
 
  fprintf(stream, "Basic block's head:\n");

  if (head != NULL) {
    /* note: print_rtl prints multiple basic blocks here.  not sure
     * whose fault this is.
     */
    int loop_is_done = FALSE;
    int seen_one_basic_block_note = FALSE;

#undef EXTRA_KDN_DEBUGGING
#ifdef EXTRA_KDN_DEBUGGING
    fprintf(stream,
	    "NOTE_INSN_BASIC_BLOCK: %d, JUMP_INSN: %d, CODE_LABEL: %d\n",
	    NOTE_INSN_BASIC_BLOCK, JUMP_INSN, CODE_LABEL);
#endif	    

    for (const rtx_insn *head_iterator = as_a <const rtx_insn *> (head);
	 (head_iterator != NULL) && !loop_is_done; ) {

#ifdef EXTRA_KDN_DEBUGGING
      fprintf(stream,
	      "printing single rtl, loop_is_done: %d, seen_one_bb: %d\n",
	      loop_is_done, seen_one_basic_block_note);
      fprintf(stream,
	       "  code of this insn is %d\n", GET_CODE(head_iterator));
#endif

      print_rtl_single(stream, head_iterator);
      
      if (NOTE_INSN_BASIC_BLOCK_P(head_iterator)) {
	seen_one_basic_block_note = TRUE;
      }
      else {
	switch (GET_CODE(head_iterator)) {
	  
	case JUMP_INSN:
	  loop_is_done = TRUE;
	  break;
	  
	case CALL_INSN:		/* not sure if CALL_INSN terminates a bb */
	  /* this code was copied from cfgbuild.c */
	  /* Noreturn and sibling call instructions terminate the basic blocks
	     (but only if they happen unconditionally).  */
	  if ((SIBLING_CALL_P (head_iterator)
	       || find_reg_note (head_iterator, REG_NORETURN, 0))
	      && GET_CODE (PATTERN (head_iterator)) != COND_EXEC) {
	    loop_is_done = TRUE;
	    break;
	  }
	  
	default:			/* in other cases, do nothing */
	  break;
	}
      }

      head_iterator = NEXT_INSN(head_iterator);

      if (head_iterator != NULL) {
	if (NOTE_INSN_BASIC_BLOCK_P(head_iterator)) {
	  if (seen_one_basic_block_note) {
	    loop_is_done = TRUE;
	  }
	} else if (GET_CODE(head_iterator) == CODE_LABEL) {
	  if (seen_one_basic_block_note) {
	    loop_is_done = TRUE;
	  }
	}
      }
    }
  }
  fprintf(stream, "End of basic block's head:\n");

  if (rtl != NULL) {
    fprintf(stream, "Basic block's footer:\n");
    int loop_is_done = FALSE;
    int seen_one_basic_block_note = FALSE;

    for (notes_and_jumptables = rtl->footer_;
	 (notes_and_jumptables != NULL) && !loop_is_done; ) {

      print_rtl_single(stream, notes_and_jumptables);

      if (NOTE_INSN_BASIC_BLOCK_P(notes_and_jumptables)) {
	seen_one_basic_block_note = TRUE;
      }
      else {
	switch (GET_CODE(notes_and_jumptables)) {
	  
	case JUMP_INSN:
	  loop_is_done = TRUE;
	  break;
	  
	case CALL_INSN:
	  /* this code was copied from cfgbuild.c */
	  /* Noreturn and sibling call instructions terminate the basic blocks
	     (but only if they happen unconditionally).  */
	  if ((SIBLING_CALL_P (notes_and_jumptables)
	       || find_reg_note (notes_and_jumptables, REG_NORETURN, 0))
	      && GET_CODE (PATTERN (notes_and_jumptables)) != COND_EXEC) {
	    loop_is_done = TRUE;
	    break;
	  }
	  
	default:			/* in other cases, do nothing */
	  break;
	}
      }

      notes_and_jumptables = NEXT_INSN(notes_and_jumptables);

      if (notes_and_jumptables != NULL) {
	if (NOTE_INSN_BASIC_BLOCK_P(notes_and_jumptables)) {
	  if (seen_one_basic_block_note) {
	    loop_is_done = TRUE;
	  }
	} else if (GET_CODE(notes_and_jumptables) == CODE_LABEL) {
	  if (seen_one_basic_block_note) {
	    loop_is_done = TRUE;
	  }
	}
      }
    }
    fprintf(stream, "End of basic block's footer\n");
  }	 
}

/* ignore warning message about gimple not used */
static void kdn_dump_gimple_instruction_list(FILE *stream,
					     struct gimple_bb_info gimple) {
  /* see gimple-pretty-print.c for relevant subroutines */
  /*  reference to visited silences warning about arg not used */
  fprintf(stream,
	  "Not Implemented: dumping of gimple instructions, visited: %d\n",
	  gimple.seq->visited);
}

static void kdn_dump_rtl_insns(FILE *stream, const_rtx rtx_insns) {
  if (rtx_insns != NULL) {
    struct pretty_printer buffer;
    pp_needs_newline(&buffer) = true;
    buffer.buffer->stream = stream;

    print_rtl(stream, rtx_insns);
  }
}

static void kdn_dump_gimple_insns(FILE *stream, gimple_seq gimp_insns) {
  /* see gimple-pretty-print.c for relevant subroutines */
  /*  reference to visited silences warning about arg not used */
  fprintf(stream,
	  "Not Implemented: dumping of gimple instructions, visited: %d\n",
	  gimp_insns->visited);
}

static void kdn_dump_loop_exits(FILE *stream, struct loop_exit *exits) {
  edge e;
  loop_exit *walker = exits;

  /* exits is as an infinite cycle and never equals null, apparently */
  fprintf(stream, "Loop exits consist of edges: ");
  do {
    e = exits->e;
    kdn_dump_edge(stream, e, TRUE, TRUE);
    walker = walker->next;
  } while (walker != exits);
}

static void kdn_dump_niter_desc(FILE *stream, struct niter_desc *loop_desc) {

  if (loop_desc == NULL) {
    fprintf(stream, "No iteration information for this loop\n");

  } else {

    fprintf(stream, "  Edge that leaves loop: ");
    kdn_dump_edge(stream, loop_desc->out_edge, TRUE, TRUE);

    fprintf(stream, "  Edge from same source that continues loop: ");
    kdn_dump_edge(stream, loop_desc->in_edge, TRUE, TRUE);

    fprintf(stream, "  What do we know? %s\n",
	    (loop_desc->simple_p)? "something": "nothing");
    
    fprintf(stream, "  Is number of iterations constant? %s\n",
	    (loop_desc->const_iter)? "yes": "no");
    
    if (loop_desc->const_iter) {
      fprintf(stream, "  Constant number of iterations: %ld\n",
	      (long) loop_desc->niter);
    }

    /*
    fprintf(stream, " Number of iterations:\n");
    kdn_dump_rtl_insns(stream, loop_desc->niter_expr);
    fprintf(stream, "\n");
    */
    fprintf(stream,
	    "  Not reported: assumptions, noloop_assumptions, infinite,");
    fprintf(stream, " niter_expr, mode, signed_p\n");
  }
}


static void kdn_dump_dominator(FILE *stream, struct et_node *dominator) {
  basic_block a_block;
  
  if (dominator == NULL) {
    fprintf(stream,
	    "   [Post?] Domination tree is empty\n");
  } else {
    
    fprintf(stream,
	    "   [Post?] Domination tree [dfs_num_in: %d, dfs_num_out: %d]\n",
	    dominator->dfs_num_in, dominator->dfs_num_out);

    /* Testing confirms that dominator->data points to a basic block */
    a_block = (basic_block) dominator->data;

    fprintf(stream, "      ");
    kdn_dump_block_id(stream, a_block);

    if (dominator->son != NULL) {
      kdn_treedump_children(stream, 8, dominator->son);
    }
  }
}

static void kdn_treedump_children(FILE *stream,
				  int indent_level, struct et_node *a_son) {
  /* assert: a_son != NULL */
  struct et_node *first_son = a_son;

  do {
    basic_block a_block = (basic_block) a_son->data;

    fprintf(stream, "%*s", indent_level, ": ");
    kdn_dump_block_id(stream, a_block);

    if (a_son->son != NULL) {
      kdn_treedump_children(stream, indent_level + 2, a_son->son);
    }
    a_son = a_son->left;
  } while (a_son != first_son);
}

static void kdn_dump_wide_int(FILE *stream, const widest_int &orig_big_no) {
  widest_int working_memory;
  widest_int wide_zero, wide_ten;
  int precision = orig_big_no.get_precision();
  /* ISO C++ prohibits variable length array declarations, so I need a
   * different way to allocate this array.
   *
   *  char buffer[precision+2];
   */
  char *buffer = (char *) alloca(precision+2);
  char *end_buffer = &buffer[precision+1];
  bool negative = FALSE;

  /* implicit invocation of widest_int constructors */
  wide_zero = 0;
  wide_ten = 10;

  wi::copy(working_memory, orig_big_no);
  if (wi::only_sign_bit_p(working_memory)) {
    wi::neg(working_memory);
    negative = TRUE;
  }

  *end_buffer-- = '\0';
  for (int i = 0; i < precision; i++) {
    widest_int remainder;
    unsigned int length;
    const HOST_WIDE_INT *value;

    remainder = wi::smod_trunc(working_memory, wide_ten);
    length = remainder.get_len();

    /* value gets a pointer to the insides of the remainder object */
    value = remainder.get_val();

    /* kelvin says: i think i want to assert that length > 0 */
    if (length > 0) {
      *end_buffer-- = '0' + value[length-1];
    }

    /* divide working_memory by ten, since we've captured the least
     * significant bit already.
     */
    working_memory = wi::sdiv_trunc(working_memory, wide_ten);

    if (wi::cmps(working_memory, wide_zero) == 0) {
      break;
    }
  }

  if (negative) {
    *end_buffer-- = '-';
  }
  end_buffer++;
  fprintf(stream, "[kdn big number] %s\n", end_buffer);
}
