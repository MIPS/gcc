/* Integrated Register Allocator intercommunication header file.
   Copyright (C) 2006, 2007
   Free Software Foundation, Inc.
   Contributed by Vladimir Makarov <vmakarov@redhat.com>.

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

#include "cfgloop.h"
#include "ira.h"
#include "alloc-pool.h"

#ifdef ENABLE_CHECKING
#define ENABLE_IRA_CHECKING
#endif

#ifdef ENABLE_IRA_CHECKING
#define ira_assert(c) gcc_assert (c)
#else
#define ira_assert(c)
#endif

/* Compute register frequency from edge frequency FREQ.  It is
   analogous to REG_FREQ_FROM_BB.  When optimizing for size, or
   profile driven feedback is available and the function is never
   executed, frequency is always equivalent.  Otherwise rescale edge
   frequency.  */
#define REG_FREQ_FROM_EDGE_FREQ(freq)					      \
  (optimize_size || (flag_branch_probabilities && !ENTRY_BLOCK_PTR->count)    \
   ? REG_FREQ_MAX : (freq * REG_FREQ_MAX / BB_FREQ_MAX)			      \
   ? (freq * REG_FREQ_MAX / BB_FREQ_MAX) : 1)

/* All natural loops.  */
extern struct loops ira_loops;

/* The flag value used internally.  */
extern int internal_flag_ira_verbose;

/* Dump file of the allocator if it is not NULL.  */
extern FILE *ira_dump_file;

/* Pools for allocnos, copies, allocno live ranges.  */
extern alloc_pool allocno_pool, copy_pool, allocno_live_range_pool;

/* Allocno live range, allocno, and copy of allocnos.  */
typedef struct loop_tree_node *loop_tree_node_t;
typedef struct allocno_live_range *allocno_live_range_t;
typedef struct allocno *allocno_t;
typedef struct allocno_copy *copy_t;

/* The following structure describes loop tree node (block or loop).
   We need such tree because the loop tree from cfgloop.h is not
   convenient for the optimization (because basic blocks are not a
   part of the tree from cfgloop.h).  We also use the nodes for
   storing additional information about basic blocks/loops for the
   register allocation.  */
struct loop_tree_node
{
  /* The node represents basic block if inner == NULL.  */
  basic_block bb;    /* NULL for loop.  */
  struct loop *loop; /* NULL for BB.  */
  /* The next node on the same tree level.  */
  loop_tree_node_t next;
  /* The first node immediately inside the node.  */
  loop_tree_node_t inner;
  /* The node containing given node.  */
  loop_tree_node_t father;

  /* Loop level in range [0, ira_loop_tree_height).  */
  int level;

  /* Allocnos in loop corresponding to regnos.  If it is NULL the loop
     is not included in the loop tree (e.g. it has abnormal enter/exit
     edges).  */
  allocno_t *regno_allocno_map;

  /* Maximal register pressure inside loop for given class (defined
     only for cover_class).  */
  int reg_pressure [N_REG_CLASSES];

  /* Allocnos referred in the loop node.  */
  bitmap mentioned_allocnos;

  /* Regnos modified in the loop node (including its subloops).  */
  bitmap modified_regnos;

  /* Allocnos living at the loop borders.  */
  bitmap border_allocnos;

  /* Copies referred in the loop node.  */
  bitmap local_copies;
};

/* The root of the loop tree corresponding to the all function.  */
extern loop_tree_node_t ira_loop_tree_root;

/* Height of the loop tree.  */
extern int ira_loop_tree_height;

/* All basic block data are referred through the following array.  We
   can not use member `aux' for this because it is used for insertion
   of insns on edges.  */
extern loop_tree_node_t ira_bb_nodes;

/* Two access macros to the basic block data.  */
#if defined ENABLE_IRA_CHECKING && (GCC_VERSION >= 2007)
#define IRA_BB_NODE_BY_INDEX(index) __extension__			\
(({ loop_tree_node_t _node = (&ira_bb_nodes [index]);	\
     if (_node->inner != NULL || _node->loop != NULL || _node->bb == NULL)\
       {								\
         fprintf (stderr,						\
                  "\n%s: %d: error in %s: it is not a block node\n",	\
                  __FILE__, __LINE__, __FUNCTION__);			\
         exit (1);							\
       }								\
     _node; }))
#else
#define IRA_BB_NODE_BY_INDEX(index) (&ira_bb_nodes [index])
#endif

#define IRA_BB_NODE(bb) IRA_BB_NODE_BY_INDEX ((bb)->index)

/* All loop data are referred through the following array.  */
extern loop_tree_node_t ira_loop_nodes;

/* Two access macros to the loop data.  */
#if defined ENABLE_IRA_CHECKING && (GCC_VERSION >= 2007)
#define IRA_LOOP_NODE_BY_INDEX(index) __extension__			\
(({ loop_tree_node_t const _node = (&ira_loop_nodes [index]);\
     if (_node->inner == NULL || _node->bb != NULL || _node->loop == NULL)\
       {								\
         fprintf (stderr,						\
                  "\n%s: %d: error in %s: it is not a loop node\n",	\
                  __FILE__, __LINE__, __FUNCTION__);			\
         exit (1);							\
       }								\
     _node; }))
#else
#define IRA_LOOP_NODE_BY_INDEX(index) (&ira_loop_nodes [index])
#endif

#define IRA_LOOP_NODE(loop) IRA_LOOP_NODE_BY_INDEX ((loop)->num)



/* The structure describes program points where a given allocno
   lives.  */
struct allocno_live_range
{
  /* Allocno whose live range is described by given structure.  */
  allocno_t allocno;
  /* Program point range.  */
  int start, finish;
  /* Next structure describing program points where the allocno
     lives.  */
  allocno_live_range_t next;
  /* Pointer to structures with the same start/finish.  */
  allocno_live_range_t start_next, finish_next;
};

/* Program points are enumerated by number from range
   0..MAX_POINT-1.  */
extern int max_point;

/* Arrays of size MAX_POINT mapping a program point to the allocno
   live ranges with given start/finish point.  */
extern allocno_live_range_t *start_point_ranges, *finish_point_ranges;

/* Node representing allocnos (allocation entity).  */
struct allocno
{
  /* The allocno order number starting with 0.  */
  int num;
  /* Regno for allocno or cap.  */
  int regno;
  /* Mode of the allocno.  */
  enum machine_mode mode;
  /* Final rtx representation of the allocno.  */
  rtx reg;
  /* Hard register assigned to given allocno.  Negative value means
     that memory was allocated to the allocno.  */
  int hard_regno;
  /* Allocnos with the same regno are linked by the following member.
     Allocnos corresponding to inner loops are first in the list
     (depth-first tarverse).  */
  allocno_t next_regno_allocno;
  /* There may be different allocnos with the same regno.  They are
     bound to a loop tree node.  */
  loop_tree_node_t loop_tree_node;
  /* Accumulated usage references of the allocno.  */
  int nrefs;
  /* Accumulated frequency of usage of the allocno.  */
  int freq;
  /* Register class which should be used for allocation for given
     allocno.  NO_REGS means that we should use memory.  */
  enum reg_class cover_class;
  /* The biggest register class with minimal cost usage for given
     allocno.  */
  enum reg_class best_class;
  /* Minimal accumulated cost of usage register of the cover class for
     the allocno.  */
  int cover_class_cost;
  /* Minimal accumulated, and updated costs of memory for the
     allocno.  */
  int memory_cost, updated_memory_cost;
  /* Copies to other non-conflicting allocnos.  The copies can
     represent move insn or potential move insn usually because of two
     operand constraints.  */
  copy_t allocno_copies;
  /* It is a allocno (cap) representing given allocno on upper loop tree
     level.  */
  allocno_t cap;
  /* It is a link to allocno (cap) on lower loop level represented by
     given cap.  Null if it is not a cap.  */
  allocno_t cap_member;
  /* Coalesced allocnos form a cyclic list.  One allocno given by
     FIRST_COALESCED_ALLOCNO represents all coalesced allocnos.  The
     list is chained by NEXT_COALESCED_ALLOCNO.  */
  allocno_t first_coalesced_allocno;
  allocno_t next_coalesced_allocno;
  /* Pointer to structures describing at what program point the
     allocno lives.  We always maintain the condition that *ranges in
     the list are not intersected and ordered by decreasing their
     program points*.  */
  allocno_live_range_t live_ranges;
  /* Vector of conflicting allocnos with NULL end marker (first
     initial and then accumulated conflict allocnos).  Only allocnos
     with the same cover class are in the vector.  */
  allocno_t *conflict_allocno_vec;
  /* Allocated size of the previous array.  */
  int conflict_allocno_vec_size;
  /* Numbers of initial and total (with) accumulated conflicts in the
     previous array.  */
  int conflict_allocnos_num, total_conflict_allocnos_num;
  /* Initial and accumulated hard registers conflicting with this
     allocno and as a consequences can not be assigned to the
     allocno.  */
  HARD_REG_SET conflict_hard_regs, total_conflict_hard_regs;
  /* Accumulated frequency of calls which given allocno
     intersects.  */
  int call_freq;
  /* Start index of calls intersected by the allocno in
     regno_calls [regno].  */
  int calls_crossed_start;
  /* Length of the previous array (number of the intersected calls).  */
  int calls_crossed_num;
  /* Non NULL if we remove restoring value from given allocno to
     MEM_OPTIMIZED_DEST at the end of loop because the value is not
     changed in loop.  */
  allocno_t mem_optimized_dest;
  /* TRUE if the allocno was a destination of removed move at the end
     of loop because the value is not changed in loop.  */
  unsigned int mem_optimized_dest_p : 1;

#ifdef STACK_REGS
  /* Set to TRUE if allocno can't be allocated in the stack
     register.  */
  unsigned int no_stack_reg_p : 1, total_no_stack_reg_p : 1;
#endif
  /* TRUE value means than the allocno was not removed from the
     conflicting graph during colouring.  */
  unsigned int in_graph_p : 1;
  /* TRUE if a hard register or memory has been assigned to the
     allocno.  */
  unsigned int assigned_p : 1;
  /* TRUE if it is put on the stack to make other allocnos
     colorable.  */
  unsigned int may_be_spilled_p : 1;
  /* Array of additional costs (accumulated and the one updated during
     coloring) for hard regno of allocno cover class.  If given
     allocno represents a set of allocnos the current costs represents
     costs of the all set.  */
  int *hard_reg_costs, *updated_hard_reg_costs;
  /* Array of decreasing costs (accumulated and the one updated during
     coloring) for conflicting allocnos for hard regno of the allocno
     cover class.  The member values can be NULL if all costs are the
     same.  If given allocno represents a set of allocnos the current
     costs represents costs of the all set.  */
  int *conflict_hard_reg_costs, *updated_conflict_hard_reg_costs;
  /* Number of allocnos with TRUE in_graph_p value and conflicting with
     given allocno.  */
  int left_conflicts_num;
  /* Number of hard register of the allocno cover class really
     availiable for the allocno allocation.  */
  int available_regs_num;
  /* Allocnos in a bucket (used in coloring) chained by the following
     two members.  */
  allocno_t next_bucket_allocno;
  allocno_t prev_bucket_allocno;
  /* Used for temporary purposes.  */
  int temp;
};

/* All members of the allocno node should be accessed only through the
   following macros.  */
#define ALLOCNO_NUM(A) ((A)->num)
#define ALLOCNO_REGNO(A) ((A)->regno)
#define ALLOCNO_REG(A) ((A)->reg)
#define ALLOCNO_NEXT_REGNO_ALLOCNO(A) ((A)->next_regno_allocno)
#define ALLOCNO_LOOP_TREE_NODE(A) ((A)->loop_tree_node)
#define ALLOCNO_CAP(A) ((A)->cap)
#define ALLOCNO_CAP_MEMBER(A) ((A)->cap_member)
#define ALLOCNO_CONFLICT_ALLOCNO_VEC(A) ((A)->conflict_allocno_vec)
#define ALLOCNO_CONFLICT_ALLOCNO_VEC_SIZE(A) ((A)->conflict_allocno_vec_size)
#define ALLOCNO_CONFLICT_ALLOCNOS_NUM(A) ((A)->conflict_allocnos_num)
#define ALLOCNO_TOTAL_CONFLICT_ALLOCNOS_NUM(A) \
  ((A)->total_conflict_allocnos_num)
#define ALLOCNO_CONFLICT_HARD_REGS(A) ((A)->conflict_hard_regs)
#define ALLOCNO_TOTAL_CONFLICT_HARD_REGS(A) ((A)->total_conflict_hard_regs)
#define ALLOCNO_NREFS(A) ((A)->nrefs)
#define ALLOCNO_FREQ(A) ((A)->freq)
#define ALLOCNO_HARD_REGNO(A) ((A)->hard_regno)
#define ALLOCNO_CALL_FREQ(A) ((A)->call_freq)
#define ALLOCNO_CALLS_CROSSED_START(A) ((A)->calls_crossed_start)
#define ALLOCNO_CALLS_CROSSED_NUM(A) ((A)->calls_crossed_num)
#define ALLOCNO_MEM_OPTIMIZED_DEST(A) ((A)->mem_optimized_dest)
#define ALLOCNO_MEM_OPTIMIZED_DEST_P(A) ((A)->mem_optimized_dest_p)
#ifdef STACK_REGS
#define ALLOCNO_NO_STACK_REG_P(A) ((A)->no_stack_reg_p)
#define ALLOCNO_TOTAL_NO_STACK_REG_P(A) ((A)->total_no_stack_reg_p)
#endif
#define ALLOCNO_IN_GRAPH_P(A) ((A)->in_graph_p)
#define ALLOCNO_ASSIGNED_P(A) ((A)->assigned_p)
#define ALLOCNO_MAY_BE_SPILLED_P(A) ((A)->may_be_spilled_p)
#define ALLOCNO_MODE(A) ((A)->mode)
#define ALLOCNO_COPIES(A) ((A)->allocno_copies)
#define ALLOCNO_HARD_REG_COSTS(A) ((A)->hard_reg_costs)
#define ALLOCNO_UPDATED_HARD_REG_COSTS(A) ((A)->updated_hard_reg_costs)
#define ALLOCNO_CONFLICT_HARD_REG_COSTS(A) \
  ((A)->conflict_hard_reg_costs)
#define ALLOCNO_UPDATED_CONFLICT_HARD_REG_COSTS(A) \
  ((A)->updated_conflict_hard_reg_costs)
#define ALLOCNO_LEFT_CONFLICTS_NUM(A) ((A)->left_conflicts_num)
#define ALLOCNO_BEST_CLASS(A) ((A)->best_class)
#define ALLOCNO_COVER_CLASS(A) ((A)->cover_class)
#define ALLOCNO_COVER_CLASS_COST(A) ((A)->cover_class_cost)
#define ALLOCNO_MEMORY_COST(A) ((A)->memory_cost)
#define ALLOCNO_UPDATED_MEMORY_COST(A) ((A)->updated_memory_cost)
#define ALLOCNO_AVAILABLE_REGS_NUM(A) ((A)->available_regs_num)
#define ALLOCNO_NEXT_BUCKET_ALLOCNO(A) ((A)->next_bucket_allocno)
#define ALLOCNO_PREV_BUCKET_ALLOCNO(A) ((A)->prev_bucket_allocno)
#define ALLOCNO_TEMP(A) ((A)->temp)
#define ALLOCNO_FIRST_COALESCED_ALLOCNO(A) ((A)->first_coalesced_allocno)
#define ALLOCNO_NEXT_COALESCED_ALLOCNO(A) ((A)->next_coalesced_allocno)
#define ALLOCNO_LIVE_RANGES(A) ((A)->live_ranges)

/* Map regno -> allocnos.  */
extern allocno_t *regno_allocno_map;

/* Array of references to all allocnos.  The order number of the
   allocno corresponds to the index in the array.  */
extern allocno_t *allocnos;

/* Sizes of the previous array.  */
extern int allocnos_num;

/* The following structure represents a copy of given allocno to
   another allocno.  The copies represent move insns or potential move
   insns usually because of two operand constraints. */
struct allocno_copy
{
  /* The order number of the copy node starting with 0.  */
  int num;
  /* Allocno connected by the copy.  The first one should have smaller
     order number than the second one.  */
  allocno_t first, second;
  /* Execution frequency of the copy.  */
  int freq;
  /* It is a move insn if the copy represents it, potential move insn
     is represented by NULL.  */
  rtx move_insn;
  /* Copies with the same allocno as FIRST are linked by the two
     following members.  */
  copy_t prev_first_allocno_copy, next_first_allocno_copy;
  /* Copies with the same allocno as SECOND are linked by the two
     following members.  */
  copy_t prev_second_allocno_copy, next_second_allocno_copy;
  /* Region from which given copy is originated.  */
  loop_tree_node_t loop_tree_node;
};

/* Array of references to copies.  The order number of the copy
   corresponds to the index in the array.  */
extern copy_t *copies;

/* Size of the previous array.  */
extern int copies_num;

/* The following structure describes a stack slot used for spilled
   registers.  */
struct spilled_reg_stack_slot
{
  /* pseudo-registers have used the stack slot.  */
  regset_head spilled_regs;
  /* RTL representation of the stack slot.  */
  rtx mem;
  /* Size of the stack slot.  */
  unsigned int width;
};

/* The number of elements in the following array.  */
extern int spilled_reg_stack_slots_num;

/* The following array contains description of spilled registers stack
   slots have been used in current function so far.  */
extern struct spilled_reg_stack_slot *spilled_reg_stack_slots;

/* Correspondingly overall cost of the allocation, cost of hard
   register usage for the allocnos, cost of memory usage for the
   allocnos, cost of loads, stores and register move insns generated
   for register live range splitting.  */
extern int overall_cost;
extern int reg_cost, mem_cost;
extern int load_cost, store_cost, shuffle_cost;
extern int move_loops_num, additional_jumps_num;

/* Map: register class x machine mode -> number of hard registers of
   given class needed to store value of given mode.  If the number is
   different, the size will be negative.  */
extern int reg_class_nregs [N_REG_CLASSES] [MAX_MACHINE_MODE];

/* Maximal value of the previous array elements.  */
extern int max_nregs;

/* The number of bits in each element of `allocnos_live' and what
   type that element has.  We use the largest integer format on the
   host machine.  */
#define INT_BITS HOST_BITS_PER_WIDE_INT
#define INT_TYPE HOST_WIDE_INT

/* Set, clear or test bit number I in R, a bit vector indexed by
   allocno number.  */
#define SET_ALLOCNO_SET_BIT(R, I)				\
  ((R)[(unsigned) (I) / INT_BITS]				\
   |= ((INT_TYPE) 1 << ((unsigned) (I) % INT_BITS)))

#define CLEAR_ALLOCNO_SET_BIT(R, I)				\
  ((R) [(unsigned) (I) / INT_BITS]				\
   &= ~((INT_TYPE) 1 << ((unsigned) (I) % INT_BITS)))

#define TEST_ALLOCNO_SET_BIT(R, I)				\
  ((R) [(unsigned) (I) / INT_BITS]				\
   & ((INT_TYPE) 1 << ((unsigned) (I) % INT_BITS)))

/* For each allocno set in ALLOCNO_SET, set ALLOCNO to that
   allocno, and execute CODE.  */
#define EXECUTE_IF_SET_IN_ALLOCNO_SET(ALLOCNO_SET, ALLOCNO, CODE)	\
do {									\
  int i_;								\
  int allocno_;								\
  INT_TYPE *p_ = (ALLOCNO_SET);						\
									\
  for (i_ = allocno_set_words - 1, allocno_ = 0; i_ >= 0;		\
       i_--, allocno_ += INT_BITS)					\
    {									\
      unsigned INT_TYPE word_ = (unsigned INT_TYPE) *p_++;		\
									\
      for ((ALLOCNO) = allocno_; word_; word_ >>= 1, (ALLOCNO)++)	\
	{								\
	  if (word_ & 1)						\
	    {CODE;}							\
	}								\
    }									\
} while (0)

/* ira.c: */

/* Hard regsets whose all bits are correspondingly zero or one.  */
extern HARD_REG_SET zero_hard_reg_set;
extern HARD_REG_SET one_hard_reg_set;

/* A mode whose value is immediately contained in given mode
   value.  */
extern unsigned char mode_inner_mode [NUM_MACHINE_MODES];

/* Map hard regs X modes -> number registers for store value of given
   mode starting with given hard register.  */
extern HARD_REG_SET reg_mode_hard_regset
                    [FIRST_PSEUDO_REGISTER] [NUM_MACHINE_MODES];

/* Array analog of macros MEMORY_MOVE_COST and REGISTER_MOVE_COST.  */
extern int memory_move_cost [MAX_MACHINE_MODE] [N_REG_CLASSES] [2];
extern int register_move_cost [MAX_MACHINE_MODE] [N_REG_CLASSES]
                              [N_REG_CLASSES];

/* Register class subset relation.  */
extern int class_subset_p [N_REG_CLASSES] [N_REG_CLASSES];

/* The biggest class inside of intersection of the two classes.  */
extern enum reg_class reg_class_subintersect [N_REG_CLASSES] [N_REG_CLASSES];

/* Hard registers which can be used for the allocation of given
   register class.  */
extern short class_hard_regs [N_REG_CLASSES] [FIRST_PSEUDO_REGISTER];

/* The size of the above array for given register class.  */
extern int class_hard_regs_num [N_REG_CLASSES];

/* Index (in class_hard_regs) for given register class and hard
   register.  */
extern short class_hard_reg_index [N_REG_CLASSES] [FIRST_PSEUDO_REGISTER];

/* Hard registers can not be used for the register allocator.  */
extern HARD_REG_SET no_alloc_regs;

/* Number of class hard registers available for the register
   allocation for given classes.  */
extern int available_class_regs [N_REG_CLASSES];

/* Array whose values are hard regset of hard registers of given
   register class whose HARD_REGNO_MODE_OK values are zero.  */
extern HARD_REG_SET prohibited_class_mode_regs
                    [N_REG_CLASSES] [NUM_MACHINE_MODES];

/* Array whose values are hard regset of hard registers for which
   move of the hard register in given mode into itself is
   prohibited.  */
extern HARD_REG_SET prohibited_mode_move_regs [NUM_MACHINE_MODES];

/* Number of cover classes.  */
extern int reg_class_cover_size;

/* The array containing cover classes whose hard registers are used
   for the allocation.  */
extern enum reg_class reg_class_cover [N_REG_CLASSES];

/* The value is number of elements in the subsequent array.  */
extern int important_classes_num;

/* The array containing classes which are subclasses of cover
   classes.  */
extern enum reg_class important_classes [N_REG_CLASSES];

/* The array containing order numbers of important classes (they are
   subclasses of cover classes).  */
extern enum reg_class important_class_nums [N_REG_CLASSES];

/* Map of register classes to corresponding cover class containing the
   given class.  */
extern enum reg_class class_translate [N_REG_CLASSES];

extern void set_non_alloc_regs (int);
extern void *ira_allocate (size_t);
extern void *ira_reallocate (void *, size_t);
extern void ira_free (void *addr);
extern bitmap ira_allocate_bitmap (void);
extern void ira_free_bitmap (bitmap);
extern regset ira_allocate_regset (void);
extern void ira_free_regset (regset);
extern int hard_reg_not_in_set_p (int, enum machine_mode, HARD_REG_SET);
extern void print_disposition (FILE *);
extern void debug_disposition (void);
extern void debug_class_cover (void);

/* Regno invariant flags.  */
extern int *reg_equiv_invariant_p;

/* Regno equivalent constants.  */
extern rtx *reg_equiv_const;

/* ira-build.c */

/* The current loop tree node.  */
extern loop_tree_node_t ira_curr_loop_tree_node;
extern allocno_t *ira_curr_regno_allocno_map;
extern VEC(rtx, heap) **regno_calls;

extern int add_regno_call (int, rtx);

extern void traverse_loop_tree (int, loop_tree_node_t,
				void (*) (loop_tree_node_t),
				void (*) (loop_tree_node_t));
extern allocno_t create_allocno (int, int, loop_tree_node_t);
extern void allocate_allocno_conflicts (allocno_t, int);
extern int allocno_conflict_index (allocno_t, allocno_t);
extern void add_allocno_conflict (allocno_t, allocno_t);
extern void print_expanded_allocno (allocno_t);
extern allocno_live_range_t create_allocno_live_range (allocno_t, int, int,
						       allocno_live_range_t);
extern void finish_allocno_live_range (allocno_live_range_t);
extern copy_t create_copy (allocno_t, allocno_t, int, rtx, loop_tree_node_t);
extern void add_allocno_copy_to_list (copy_t);
extern void swap_allocno_copy_ends_if_necessary (copy_t);
extern void remove_allocno_copy_from_list (copy_t);
extern copy_t add_allocno_copy (allocno_t, allocno_t, int, rtx,
				loop_tree_node_t);

extern void ira_flattening (int, int);
extern int ira_build (int);
extern void ira_destroy (void);

/* ira-costs.c */
extern void init_ira_costs_once (void);
extern void finish_ira_costs_once (void);
extern void ira_costs (void);
extern void tune_allocno_costs_and_cover_classes (void);

/* ira-lives.c */

/* Number of ints required to hold allocnos_num bits.  */
extern int allocno_set_words;

extern void rebuild_start_finish_chains (void);
extern void print_live_range_list (FILE *, allocno_live_range_t);
extern void debug_live_range_list (allocno_live_range_t);
extern void debug_allocno_live_ranges (allocno_t);
extern void debug_live_ranges (void);
extern void create_allocno_live_ranges (void);
extern void finish_allocno_live_ranges (void);

/* ira-conflicts.c */
extern int allocno_conflict_p (allocno_t, allocno_t);
extern int allocno_reg_conflict_p (int, int);
extern void debug_conflicts (int);
extern void ira_build_conflicts (void);

/* ira-color.c */
extern int loop_edge_freq (loop_tree_node_t, int, int);
extern void reassign_conflict_allocnos (int, int);
extern void ira_color (void);

/* ira-emit.c */
extern void ira_emit (int);
