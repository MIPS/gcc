/* Yet Another Register Allocator intercommunication header file.
   Contributed by Vladimir Makarov.
   Copyright (C) 2005, 2006 Free Software Foundation, Inc.

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

#include "yara.h"

#define YARA_NO_GVN 1
#define YARA_NO_LOCAL_CAN 2
#define YARA_NO_UPDATE_EQUIV_REGS 4
#define YARA_NO_ALLOCNO_COALESCING 8
#define YARA_NO_COPY_SYNC 16
#define YARA_NO_MOVE_SYNC 32
#define YARA_REGCLASS_BEFORE 64
#define YARA_CLEAN_AFTER 128
#define YARA_PRIORITY_COLORING 256
#define YARA_NO_BIASED_COLORING 512
#define YARA_NO_FREQ_BUCKET_ORDER 1024
#define YARA_NREGS_BUCKET_ORDER 2048
#define YARA_BB_RELIEF 4096


#ifdef ENABLE_CHECKING
/*#define ENABLE_YARA_CHECKING*/
#endif

#ifdef ENABLE_YARA_CHECKING
#define yara_assert(c) gcc_assert (c)
#else
#define yara_assert(c)
#endif

/* True if X is a constant that can be forced into the constant pool.  */
#define CONST_POOL_OK_P(X)			\
  (CONSTANT_P (X)				\
   && GET_CODE (X) != HIGH			\
   && ! targetm.cannot_force_const_mem (X))

/* Information about all natural loops.  */
extern struct loops yara_loops;

/* Dump file of the allocator if it is not null.  */
extern FILE *yara_dump_file;

/* Max insn uid before the register allocator final pass.  */
extern int yara_max_uid;

/* Biggest size and align in which each pseudo reg is referred to
   (including via subreg).  */
extern int *reg_max_ref_size, *reg_max_ref_align;

/* ??? */
extern int equiv_memory_num, *reg_equiv_memory_index;

/* Element N is the list of insns that initialized reg N from its
   equivalent constant or memory slot.  */
extern rtx *reg_equiv_init;

/* This is structure used to store possible register elimination
   during register allocation.  */
struct reg_eliminate GTY(())
{
  /* Given register elimination of which is described this structure.  */
  int from;
  /* Substituting register for given register.  */
  int to;
  /* The following two members used to find possible eliminations when
     the registers in the elimination are modified.  Finally we have
     elimination with equal offset values in the table REG_ELIMINATE
     after building the table.  */
  HOST_WIDE_INT offset;
  HOST_WIDE_INT initial_offset;
  /* It is used temporary to calculate possible elimination.  Finally
     we have only eliminations with nonzero value in the table
     REG_ELIMINATE.  */
  bool can_eliminate:1;
  bool obligatory:1;
  /* Next possible elimination for given register FROM.  Finally we
     can have only one obligatory elimination for given register.  */
  struct reg_eliminate *from_next;
  /* All structures are chained by this member.  */
  struct reg_eliminate *next;
};

extern struct reg_eliminate *reg_eliminate [FIRST_PSEUDO_REGISTER];

typedef union allocno_node *allocno_t;
typedef struct copy *copy_t;
typedef struct can *can_t;

/* The following structure describes loop tree node (block or loop).
   We need such tree because the loop tree from cfgloop.h is not
   convenient for the optimization (because basic blocks are not a
   part of the tree from cfgloop.h).  We also use the nodes for
   storing additional information about basic blocks/loops for the
   register allocation.  */
struct yara_loop_tree_node
{
  /* The node represents basic block if inner == NULL.  */
  basic_block bb;    /* NULL for loop.  */
  struct loop *loop; /* NULL for BB.  */
  struct yara_loop_tree_node *next;
  struct yara_loop_tree_node *inner;
  struct yara_loop_tree_node *father;

  /* Registers referred in the node.  */
  bitmap regno_refs;

  /* Region allocnos living at start of the corresponding bb or
     loop.  */
  bitmap allocno_live_at_start;
  /* Cans living through the node (but not used).  */
  bitmap can_through;
  /* Maximal pressure in given node.  */
  int reg_pressure [N_REG_CLASSES];
  /* Temporary field.  Register pressure excess for the current reg
     class.  */
  int reg_pressure_excess;

  /* Map regno -> allocno for given loop.  */
  allocno_t *regno_allocno_map;

  /* The following members are only for basic blocks because there are
     much less loops than bbs we dont't care about space saving.  */

  /* Reverse top sort order number of the block.  We use it to forward
     data flow analysis.  */
  int rts_number;
};

/* The root of the loop tree corresponding to all function.  */
extern struct yara_loop_tree_node *yara_loop_tree_root;

/* All basic block data are refered through the following array.  We
   can not use member `aux' for this because it is used for insertion
   of insns on edges.  */
extern struct yara_loop_tree_node *yara_bb_nodes;

#if defined ENABLE_YARA_CHECKING && (GCC_VERSION >= 2007)
#define YARA_BB_NODE_BY_INDEX(index) __extension__			\
(({ struct yara_loop_tree_node *const _node = (&yara_bb_nodes [index]);	\
     if (_node->inner != NULL || _node->loop != NULL || _node->bb == NULL)\
       {								\
         fprintf (stderr,						\
                  "\n%s: %d: error in %s: it is not a block node\n",	\
                  __FILE__, __LINE__, __FUNCTION__);			\
         exit (1);							\
       }								\
     _node; }))
#else
#define YARA_BB_NODE_BY_INDEX(index) (&yara_bb_nodes [index])
#endif

#define YARA_BB_NODE(bb) YARA_BB_NODE_BY_INDEX ((bb)->index)

/* All loop data are refered through the following array.  */
extern struct yara_loop_tree_node *yara_loop_nodes;

#if defined ENABLE_YARA_CHECKING && (GCC_VERSION >= 2007)
#define YARA_LOOP_NODE_BY_INDEX(index) __extension__			\
(({ struct yara_loop_tree_node *const _node = (&yara_loop_nodes [index]);\
     if (_node->inner == NULL || _node->bb != NULL || _node->loop == NULL)\
       {								\
         fprintf (stderr,						\
                  "\n%s: %d: error in %s: it is not a loop node\n",	\
                  __FILE__, __LINE__, __FUNCTION__);			\
         exit (1);							\
       }								\
     _node; }))
#else
#define YARA_LOOP_NODE_BY_INDEX(index) (&yara_loop_nodes [index])
#endif

#define YARA_LOOP_NODE(loop) YARA_LOOP_NODE_BY_INDEX ((loop)->num)

/* The following macro iterates on loops (enclosed loops first)
   including yara_loops.tree_root.  */
#define YARA_FOR_ALL_LOOP_INDEX(index) \
   for ((index) = 0; (index) < (int) yara_loops.num; (index)++)



#define SKIP_TO_REG(res, x)					        \
  for (res = (x);							\
       GET_CODE (res) == STRICT_LOW_PART || GET_CODE (res) == SUBREG;)	\
    res = XEXP (res, 0)

#define SKIP_TO_SUBREG(res, x)					        \
  for (res = (x); GET_CODE (res) == STRICT_LOW_PART;)			\
    res = XEXP (res, 0)



typedef unsigned long alt_set_t;

#define MAX_ALT_SET_SIZE          ((int) sizeof (alt_set_t) * CHAR_BIT)

#define ZERO_ALT_SET              ((alt_set_t) 0)
#define SET_ALT_RANGE(alts, n, m) 			\
  ((alts) |= ((~ (alt_set_t) 0) >> (MAX_ALT_SET_SIZE - (m) + (n) - 1)) << (n))
#define TEST_ALT(alts, n)         (((alts) >> (n)) & 1)
#define CLEAR_ALT(alts, n)        ((alts) &= ~ ((alt_set_t) 1) << (n))
#define SET_ALT(alts, n)          ((alts) |= ((alt_set_t) 1) << (n))
#define CLEAR_ALT_SET(alts)       ((alts) = 0)
#define SET_ALT_SET(alts)         ((alts) = ~ ZERO_ALT_SET)
#define COPY_ALT_SET(to, from)    ((to) = (from))
#define OR_ALT_SET(to, from)      ((to) |= (from))
#define AND_ALT_SET(to, from)     ((to) &= (from))
#define EQ_ALT_SET(as1, as2)      ((as1) == (as2))
#define FOR_EACH_ALT(alts, N, CODE)			\
  do {							\
    alt_set_t a_;					\
    int num_;						\
 							\
    for (num_ = 0, a_ = (alts); a_; a_ >>= 1, num_++)	\
      if (a_ & 1)					\
        {						\
          (N) = num_;					\
          CODE;						\
        }						\
  } while (0)




/* o Insn allocno means pseudo-register occurence in an insn.
   o Region allocno means living pseudo-register between two insns
     inside a basic block or loop. */
enum allocno_type { INSN_ALLOCNO, REGION_ALLOCNO};

/* Fixed insn allocno type values.  */
#define BASE_REG 0
#define INDEX_REG 1
#define NON_OPERAND 2
#define OPERAND_BASE 3

struct memory_slot
{
  /* The number of allocnos with given regno allocated currently on
     the stack.  */
  int allocnos_num;
  /* Start address and size of stack slot relatively to the start/end
     (it depends on in what direction the stack grows) of the region
     of stack slots registers.  */
  short start, size;
  /* ??? Memory representation after the allocno allocation for the
     stack slot (NULL originally).  Equivalent memory in non stack
     slots.  */
  rtx mem;
  /* All above memory is enumerated.  */
  int mem_index;
  struct memory_slot *next_free_slot;
};

/* The following describes change log entry for memory slots.  */
struct memory_slot_log_entry
{
  struct memory_slot *memory_slot;
  int start;
};

/* The following structure contains allocno attributes which should be
   logged during transactions.  */
struct allocno_change
{
  /* This member is true only for pseudo-register which will be
     changed by equivalent constant.  */
  bool use_equiv_const_p : 1;
  /* Allocated start hard regno. -1 for memory or constant.  */
  short hard_regno;
  /* Pseudo-register could need several hard registers starting with
     hard_regno (if any).  So they are in the following set. */
  HARD_REG_SET hard_regset;
  
  /* Memory slot allocated to given allocno (when hard_regno < 0).  If
     the allocno is an insn allocno, the value is memory for its
     location, not the register).  */
  struct memory_slot *memory_slot;
  unsigned int memory_slot_offset;
};

/* The following structure describes common information about allocno
   representing a pseudo-register in a part of his live range.  */
struct allocno_common
{
  enum allocno_type type : 2;
  /* The following true if the allocno crosses a call.  */
  bool call_cross_p : 1;
  /* Very contradictory for subregs -- check and fix ????? Mode of the
     pseudo-register, hard register, scratch or non-register operand.
     For insn allocnos, it is taken from recog_data which is mode of
     operand or mode form the insn template if the former is of
     VOIDmode.  */
  enum machine_mode mode : 8;

  int allocno_num; /* ?!? if conflicts as arcs */

  /* The pseudo-register number.  It is -1 for scratch and
     non-register operands.  It could be also number of a hard
     register which will be temporarily reloaded into another hard
     register.  */
  int regno;
  
  /* Copies where the allocno is source or destination
     correspondingly.  */
  copy_t src_copies;
  copy_t dst_copies;

  /* The following is frequency of all copies in which given allocno
     occurs.  */
  int freq;

  /* The field value is allocated length without one of the subsequent
     vector.  */
  int conflict_vec_len;
  /* Vector of conflicting allocnos with NULL end marker.  */
  allocno_t *conflict_vec;
  /* The field value is allocated length without one of the subsequent
     vector.  */
  int copy_conflict_vec_len;
  /* Vector of conflicting copies with NULL end marker.  */
  copy_t *copy_conflict_vec;

  /* Set of hard regs conflicting with allocno N.  */
  HARD_REG_SET hard_reg_conflicts;

  /* Frequency of calls which given allocno intersects.  */
  int call_freq;

  /* Allocno attributes should be logged.  */
  struct allocno_change change;

  /* Can to which give allocno belongs.  */
  can_t can;
};

struct reg_eliminate;

/* The following structure contains insn allocno attributes which
   should be logged during transactions.  */
struct insn_allocno_change
{
  /* Alternatives possible for the current allocation.  */
  alt_set_t possible_alts;
  /* During building allocno conflicts, it is an output allocno in
     which given allocno is address.  After building conflicts if the
     following value is not NULL, we should use the same allocation as
     for TIED_ALLOCNO.  */
  allocno_t tied_allocno;
  /* The following member is defined only the previous member is not
     NULL.  True means that given allocno is the original (in this
     case the previous member refers to the duplicate), otherwise it
     is duplicate (in this case the previous member refers to the
     original).  */
  bool original_p : 1;
  /* This member is true only for constant which should be placed in
     const pool.  */
  bool const_pool_p : 1;
  /* This member is true only for operand which is used without
     change.  It is possible for memory, constant, or hard
     register.  */
  bool use_without_change_p : 1;
  /* The following is a hard register needed for elimination.  The
     negative value means we don't need a hard register for the
     elimination.  The elimination register mode is always Pmode.  */
  short interm_elimination_regno;
  /* The following is an elimination used.  If we do not do the
     elimination, the value is NULL.  */
  struct reg_eliminate *elimination;
  /* Hard regset for elimination register in Pmode.  */
  HARD_REG_SET interm_elimination_regset;
};

/* The following describes change log entry for allocnos.  */
struct allocno_log_entry
{
  allocno_t allocno;
  struct allocno_change change;
  struct insn_allocno_change insn_change;
};

struct insn_allocno
{
  struct allocno_common common;
  /* If the allocno LOC contains subreg or inside subreg, this is the
     biggest mode of subreg and subreg internal.  Otherwise it is just
     allocno mode (mode of LOC). */
  enum machine_mode biggest_mode : 8;
  enum op_type op_mode : 8;
  int type : 8; /* 0 - Base, 1 - Index, 2, 3, 4, ... - Operand #val - 2. */
  /* The following member value is true if there is early clobber flag
     for the corresponding operand.  Actually the clobber flag
     belongs to an alternative but it is hard to represent and use it
     for allocno conflicts.  Therefore we make it true for all
     alternatives if there is a flag.  */
  bool early_clobber : 1;
  /* The following is true if the allocno contains a register can be
     eliminated.  We ignore the registers in memory expression.  They
     will be processed in other allocnos.  */
  bool elimination_p : 1;
  /* The value is true if it is a constant being a part of expression
     which might be transformed by register elimination.  Such
     constant never gets a hard register or memory.  */
  bool elimination_part_const_p : 1;
  rtx insn;
  /* It is address of rtx of the allocno.  If the allocno corresponing
     an operand, it is an operand location.  */
  rtx *loc;
  /* The container location of allocno rtx.  For base and index
     register it is the corresponding memory or address expression
     top.  */
  rtx *container_loc;
  /* The following two members are used for operands. */
  allocno_t commutative; /* allocno of commutative operand */
  /* Insn allocnos of an insn is linked by the following member.  */
  allocno_t next_insn_allocno;
  /* Insn allocno attributes should be logged.  */
  struct insn_allocno_change change;
};

struct region_allocno
{
  struct allocno_common common;
  /* First or last insn where the region allocno connected to an insn
     allocno.  Null values mean that the allocno lives beyond basic
     block scope.  */
  rtx start_insn, stop_insn;
  struct yara_loop_tree_node *node;
};

union allocno_node
{
  struct allocno_common common;
  struct insn_allocno insn;
  struct region_allocno region;
};

#define ALLOCNO_TYPE(A) ((A)->common.type)

#if defined ENABLE_YARA_CHECKING && (GCC_VERSION >= 2007)

#define ALLOCNO_CHECK(A, TYPE) __extension__				\
({  const allocno_t __a = (A);						\
    if (ALLOCNO_TYPE (__a) != (TYPE))					\
      allocno_check_failed (__a, __FILE__, __LINE__, __FUNCTION__, 	\
			    (TYPE));					\
    __a; })


#else /* not ENABLE_YARA_CHECKING, or not gcc */

#define ALLOCNO_CHECK(A, TYPE)			(A)

#endif

/* Common allocno macros: */
#define ALLOCNO_NUM(A) ((A)->common.allocno_num)
#define ALLOCNO_REGNO(A) ((A)->common.regno)
#define ALLOCNO_MODE(A) ((A)->common.mode)
#define ALLOCNO_SRC_COPIES(A) ((A)->common.src_copies)
#define ALLOCNO_DST_COPIES(A) ((A)->common.dst_copies)
#define ALLOCNO_FREQ(A) ((A)->common.freq)

#define ALLOCNO_CONFLICT_VEC(A) ((A)->common.conflict_vec)
#define ALLOCNO_CONFLICT_VEC_LEN(A) ((A)->common.conflict_vec_len)
#define ALLOCNO_COPY_CONFLICT_VEC(A) ((A)->common.copy_conflict_vec)
#define ALLOCNO_COPY_CONFLICT_VEC_LEN(A) ((A)->common.copy_conflict_vec_len)
#define ALLOCNO_HARD_REG_CONFLICTS(A) ((A)->common.hard_reg_conflicts)
#define ALLOCNO_CALL_CROSS_P(A) ((A)->common.call_cross_p)
#define ALLOCNO_CALL_FREQ(A) ((A)->common.call_freq)
#define ALLOCNO_CHANGE(A) ((A)->common.change)
#define ALLOCNO_HARD_REGNO(A) ((A)->common.change.hard_regno)
#define ALLOCNO_HARD_REGSET(A) ((A)->common.change.hard_regset)
#define ALLOCNO_MEMORY_SLOT(A) ((A)->common.change.memory_slot)
#define ALLOCNO_MEMORY_SLOT_OFFSET(A) ((A)->common.change.memory_slot_offset)
#define ALLOCNO_USE_EQUIV_CONST_P(A) ((A)->common.change.use_equiv_const_p)
#define ALLOCNO_CAN(A) ((A)->common.can)

#define INSN_ALLOCNO_CHECK(A)	\
  ((struct insn_allocno *) ALLOCNO_CHECK (A, INSN_ALLOCNO))
#define REGION_ALLOCNO_CHECK(A)	\
  ((struct region_allocno *) ALLOCNO_CHECK (A, REGION_ALLOCNO))

/* Insn allocno specific macros: */
#define INSN_ALLOCNO_BIGGEST_MODE(A) (INSN_ALLOCNO_CHECK (A)->biggest_mode)
#define INSN_ALLOCNO_OP_MODE(A) (INSN_ALLOCNO_CHECK (A)->op_mode)
#define INSN_ALLOCNO_TYPE(A) (INSN_ALLOCNO_CHECK (A)->type)
#define INSN_ALLOCNO_INSN(A) (INSN_ALLOCNO_CHECK (A)->insn)
#define INSN_ALLOCNO_LOC(A) (INSN_ALLOCNO_CHECK (A)->loc)
#define INSN_ALLOCNO_CONTAINER_LOC(A) (INSN_ALLOCNO_CHECK (A)->container_loc)
#define INSN_ALLOCNO_COMMUTATIVE(A) (INSN_ALLOCNO_CHECK (A)->commutative)
#define INSN_ALLOCNO_EARLY_CLOBBER(A) (INSN_ALLOCNO_CHECK (A)->early_clobber)
#define INSN_ALLOCNO_ELIMINATION_P(A) (INSN_ALLOCNO_CHECK (A)->elimination_p)
#define INSN_ALLOCNO_ELIMINATION_PART_CONST_P(A) \
  (INSN_ALLOCNO_CHECK (A)->elimination_part_const_p)
#define INSN_ALLOCNO_NEXT(A) (INSN_ALLOCNO_CHECK (A)->next_insn_allocno)
#define INSN_ALLOCNO_CHANGE(A) (INSN_ALLOCNO_CHECK (A)->change)
#define INSN_ALLOCNO_POSSIBLE_ALTS(A) \
  (INSN_ALLOCNO_CHECK (A)->change.possible_alts)
#define INSN_ALLOCNO_TIED_ALLOCNO(A) \
  (INSN_ALLOCNO_CHECK (A)->change.tied_allocno)
#define INSN_ALLOCNO_ADDR_OUTPUT_ALLOCNO(A) INSN_ALLOCNO_TIED_ALLOCNO (A)
#define INSN_ALLOCNO_ORIGINAL_P(A) (INSN_ALLOCNO_CHECK (A)->change.original_p)
#define INSN_ALLOCNO_CONST_POOL_P(A) \
  (INSN_ALLOCNO_CHECK (A)->change.const_pool_p)
#define INSN_ALLOCNO_USE_WITHOUT_CHANGE_P(A) \
  (INSN_ALLOCNO_CHECK (A)->change.use_without_change_p)
#define INSN_ALLOCNO_ELIMINATION(A) \
  (INSN_ALLOCNO_CHECK (A)->change.elimination)
#define INSN_ALLOCNO_INTERM_ELIMINATION_REGNO(A) \
  (INSN_ALLOCNO_CHECK (A)->change.interm_elimination_regno)
#define INSN_ALLOCNO_INTERM_ELIMINATION_REGSET(A) \
  (INSN_ALLOCNO_CHECK (A)->change.interm_elimination_regset)

/* Region allocno specific macros: */
#define REGION_ALLOCNO_START_INSN(A) (REGION_ALLOCNO_CHECK (A)->start_insn)
#define REGION_ALLOCNO_STOP_INSN(A) (REGION_ALLOCNO_CHECK (A)->stop_insn)
#define REGION_ALLOCNO_NODE(A) (REGION_ALLOCNO_CHECK (A)->node)

/* Type of the point where copy insn to connect allocnos or insn
   rematerializing destination should be placed.  */
enum point_type {BEFORE_INSN, AFTER_INSN, AT_BB_START, AT_BB_END,
		 ON_EDGE_SRC, ON_EDGE_DST};

/* Point where copy insn to connect allocnos or insn rematerializing
   destination should be placed.  */
struct point
{
  enum point_type point_type;
  union
  {
    rtx insn; /* for BEFORE_INSN and AFTER_INSN */
    basic_block bb; /* for AT_BB_START and AT_BB_END */
    /* for ON_EDGE_SRC, ON_EDGE_DST: */
    struct
    {
      edge e;
      struct yara_loop_tree_node *loop_node;
    } e;
  } u;
};

/* Array of references to allocnos after forming all of them.  */
extern allocno_t *allocnos;

/* Size of the previous array.  */
extern int allocnos_num;

/* The folowing array implements a map insn uid -> list of the insn
   allocnos.  */
extern allocno_t *insn_allocnos;

/* The following array of arrays implements a map insn uid x operand
   number -> allocno corresponding the insn operand.  */
extern allocno_t **insn_op_allocnos;

/* The following structure contains copy attributes which should be
   logged during transactions.  */
struct copy_change
{
#ifdef HAVE_SECONDARY_RELOADS
  /* The following true value means that we use icode insn with src as
     an operand.  Otherwise we use dst as output of icode insn.  */
  bool in_p : 1;
  /* Code of reload insn used for copying, CODE_FOR_nothing if we use
     standard move patterns.  */
  enum insn_code icode : 15;
  enum machine_mode interm_mode : 8, scratch_mode : 8;
  /* Start intermediate hard register needed for a secondary reload.
     -1 means it is not necessary.  */
  short interm_regno;
  /* If we need secondary reload using a reload pattern, the value is
     start regno of the clobbered register in the pattern.  Otherwise
     the value is -1.  */
  short scratch_regno;
#endif
  /* The following hard register (in given mode) might be needed to
     load equivalent constant into the destination allocno.  */
  short interm_equiv_const_regno;
  enum machine_mode interm_equiv_const_mode : 8;
  /* The following two member values are used only when secondary
     memory is needed.  We don't need secondary memory if the two
     values are NULL.  */
#ifdef SECONDARY_MEMORY_NEEDED
  enum machine_mode memory_mode : 8;
  /* Value from SECONDARY_MEMORY_NEEDED_RTX, otherwise NULL.  */
  rtx user_defined_memory;
  /* Allocated memory used only the previous value is NULL.  */
  struct memory_slot *memory_slot;
#endif
  /* The following set contains all hard regnos of intermediate,
     scratch and intermidiate equiv const registers.  */
  HARD_REG_SET interm_scratch_hard_regset;
};

/* The following describes change log entry for copy.  */
struct copy_log_entry
{
  copy_t copy;
  struct copy_change *copy_change;
};

/* The following structure describes allocno connection.  If the
   allocnos get different hard registers, a copy insn should be
   generated.  */
struct copy
{
  int copy_num;
  /* Source and destination allocnos.  One field value could be NULL
     for non pseudo-register allocno.  */
  allocno_t src, dst;
  /* Next copy with the same source/destination allocno.  */
  copy_t next_src_copy;
  copy_t next_dst_copy;
  /* Order of copy (before/after insns or at start/end of basic blocks
     or edges) is important.  So they are ordered by the following
     member.  */
  copy_t next_copy;
  /* Point where the copy should be placed. */
  struct point point;

  short freq;
  /* Flag of that source and target are already synchronized.  */
  bool sync_p : 1;
  /* Use the hard regno (if it is not negative) instead of src
     location.  */
  short subst_src_hard_regno : 15;

  /* Set of hard regs conflicting with the copy.  */
  HARD_REG_SET hard_reg_conflicts;
  /* The field value is allocated length without one of the subsequent
     vector.  */
  int allocno_conflict_vec_len;
  /* Vector of conflicting allocnos with NULL end marker.  */
  allocno_t *allocno_conflict_vec;

  /* Copy attributes should be logged.  */
  struct copy_change *copy_change;
};

/* Array of references to copies after forming all of them.  */
extern copy_t *copies;

/* Size of the previous array.  */
extern int copies_num;

#define COPY_NUM(C) ((C)->copy_num)
#define COPY_SRC(C) ((C)->src)
#define COPY_DST(C) ((C)->dst)
#define COPY_NEXT_SRC_COPY(C) ((C)->next_src_copy)
#define COPY_NEXT_DST_COPY(C) ((C)->next_dst_copy)
#define COPY_NEXT_COPY(C) ((C)->next_copy)

#define COPY_POINT(C) ((C)->point)

#define COPY_CHANGE_ADDR(C) ((C)->copy_change)

#define COPY_ALLOCNO_CONFLICT_VEC(C) ((C)->allocno_conflict_vec)
#define COPY_ALLOCNO_CONFLICT_VEC_LEN(C) ((C)->allocno_conflict_vec_len)

#define COPY_HARD_REG_CONFLICTS(C) ((C)->hard_reg_conflicts)
#define COPY_FREQ(C) ((C)->freq)
#define COPY_SYNC_P(C) ((C)->sync_p)
#define COPY_SUBST_SRC_HARD_REGNO(C) ((C)->subst_src_hard_regno)
#define COPY_CHANGE(C) ((C)->change)
#ifdef HAVE_SECONDARY_RELOADS
#define COPY_ICODE(C) (COPY_CHANGE_ADDR (C)->icode)
#define COPY_INTERM_MODE(C) (COPY_CHANGE_ADDR (C)->interm_mode)
#define COPY_SCRATCH_MODE(C) (COPY_CHANGE_ADDR (C)->scratch_mode)
#define COPY_INTERM_REGNO(C) (COPY_CHANGE_ADDR (C)->interm_regno)
#define COPY_SCRATCH_REGNO(C) (COPY_CHANGE_ADDR (C)->scratch_regno)
#define COPY_IN_P(C) (COPY_CHANGE_ADDR (C)->in_p)
#endif
#define COPY_INTERM_SCRATCH_HARD_REGSET(C) \
  (COPY_CHANGE_ADDR (C)->interm_scratch_hard_regset)
#define COPY_INTERM_EQUIV_CONST_REGNO(C) \
  (COPY_CHANGE_ADDR (C)->interm_equiv_const_regno)
#define COPY_INTERM_EQUIV_CONST_MODE(C) \
  (COPY_CHANGE_ADDR (C)->interm_equiv_const_mode)
#ifdef SECONDARY_MEMORY_NEEDED
#define COPY_MEMORY_MODE(C) (COPY_CHANGE_ADDR (C)->memory_mode)
#define COPY_USER_DEFINED_MEMORY(C) \
  (COPY_CHANGE_ADDR (C)->user_defined_memory)
#define COPY_MEMORY_SLOT(C) (COPY_CHANGE_ADDR (C)->memory_slot)
#endif

/* Copies attached to the insns, basic blocks, edges in the same order
   as they will be in the modified code.  */
extern copy_t *before_insn_copies, *after_insn_copies;
extern copy_t *at_bb_start_copies, *at_bb_end_copies;
extern copy_t *at_edge_start_copies, *at_edge_end_copies;

/* Node representing connected allocnos.  */
struct can
{
  /* Node number which starts with 0.  */
  int can_num;
  /* The following is array of can allocno.  The vector end is NULL
     element.  */
  allocno_t *can_allocnos;
  /* Vector of conflicting cans with NULL end marker.  */
  can_t *conflict_can_vec;
  HARD_REG_SET conflict_hard_regs;
  HARD_REG_SET biased_hard_regs;
  /* Frequency of usage of can allocnos.  */
  int freq;
  /* Hard regno assigned to given can.  */
  int hard_regno;
  /* Cans with the same number is getting the same memory.  It is
     necessary to guarantee that after splitting a can, there will be
     no memory-memory copy.  */
  int slotno;
  /* Frequency of calls which given can intersects.  */
  int call_freq;
  /* True if an allocno of the can lives through a call.  */
  bool call_p;
  /* True value means than the can was not removed from the
     conflicting graph during colouring.  During assigning the false
     value means that we should ignore it during assigning a hard
     register to another conflicting can.  */
  bool in_graph_p;
  /* True value means that the can is global one, in other words it
     lives in more one BB.  */
  bool global_p;
  /* True if we already spilled the can during local allocation.  */  
  bool spill_p;
  /* True if hard register or memory has been assigned to the can.  */
  bool assigned_p;
  /* Mode of allocnos belonging to the can.  */
  enum machine_mode mode;
  /* Copies to other non-conflicting cans.  */
  struct can_copy *can_copies;
  /* Array of additional costs for hard regno of COVER_CLASS.  */
  int *hard_reg_costs;
  /* Number of cans with true in_graph_p value and conflicting with
     given can.  */
  int left_conflicts_num;
  /* Register class which should be used for allocation for given
     allocno.  NO_REGS means that we should use memory.  */
  enum reg_class cover_class;
  /* Cost of usage register of the cover class and memory for the
     can.  */
  int cover_class_cost, memory_cost;
  /* Cans in a bucket chained by the following two members.  */
  struct can *next_bucket_can;
  struct can *prev_bucket_can;
};

#define CAN_NUM(C) ((C)->can_num)
#define CAN_ALLOCNOS(C) ((C)->can_allocnos)
#define CAN_CONFLICT_CAN_VEC(C) ((C)->conflict_can_vec)
#define CAN_CONFLICT_HARD_REGS(C) ((C)->conflict_hard_regs)
#define CAN_BIASED_HARD_REGS(C) ((C)->biased_hard_regs)
#define CAN_FREQ(C) ((C)->freq)
#define CAN_HARD_REGNO(C) ((C)->hard_regno)
#define CAN_SLOTNO(C) ((C)->slotno)
#define CAN_CALL_FREQ(C) ((C)->call_freq)
#define CAN_CALL_P(C) ((C)->call_p)
#define CAN_IN_GRAPH_P(C) ((C)->in_graph_p)
#define CAN_GLOBAL_P(C) ((C)->global_p)
#define CAN_SPILL_P(C) ((C)->spill_p)
#define CAN_ASSIGNED_P(C) ((C)->assigned_p)
#define CAN_MODE(C) ((C)->mode)
#define CAN_COPIES(C) ((C)->can_copies)
#define CAN_HARD_REG_COSTS(C) ((C)->hard_reg_costs)
#define CAN_LEFT_CONFLICTS_NUM(C) ((C)->left_conflicts_num)
#define CAN_COVER_CLASS(C) ((C)->cover_class)
#define CAN_COVER_CLASS_COST(C) ((C)->cover_class_cost)
#define CAN_MEMORY_COST(C) ((C)->memory_cost)
#define CAN_NEXT_BUCKET_CAN(C) ((C)->next_bucket_can)
#define CAN_PREV_BUCKET_CAN(C) ((C)->prev_bucket_can)

/* Array of references to cans.  */
extern can_t *cans;

/* Size of the previous array.  */
extern int cans_num;

/* The following structure represents a copy of given can to/from
   another can.  The two cans can conflict with each other but the
   probability is small.  The two cans have cover class different from
   NO_REGS.  */
struct can_copy
{
  /* True value means that we are copying given can into the can given
     by the subsequent member.  */
  bool to_p;
  /* Another end copy can.  */
  can_t can;
  int freq;
  struct can_copy *prev_can_copy, *next_can_copy;
  struct can_copy *tied_can_copy;
};

/* Set of insn operands.  */
typedef unsigned long op_set_t;

#define MAX_OP_SET_SIZE         ((int) sizeof (op_set_t) * CHAR_BIT)

#define ZERO_OP_SET             ((op_set_t) 0)
#define SET_OP_RANGE(ops, n, m) 			\
  ((ops) |= ((~ (op_set_t) 0) >> (MAX_OP_SET_SIZE - (m) + (n) - 1)) << (n))
#define TEST_OP(ops, n)         (((ops) >> (n)) & 1)
#define CLEAR_OP(ops, n)        ((ops) &= ~ ((op_set_t) 1) << (n))
#define SET_OP(ops, n)          ((ops) |= ((op_set_t) 1) << (n))
#define CLEAR_OP_SET(ops)       ((ops) = 0)
#define SET_OP_SET(ops)         ((ops) = ~ ZERO_OP_SET)
#define COPY_OP_SET(to, from)   ((to) = (from))
#define OR_OP_SET(to, from)     ((to) |= (from))
#define AND_OP_SET(to, from)    ((to) &= (from))
#define EQ_OP_SET(as1, as2)     ((as1) == (as2))

/* Information about the insn of given code.  */
struct insn_op_info
{
  int code; /* insn code or -1.  */
  /* Array of n_operands x n_alts.  It might be NULL if there is no
     alternatives or operands  */
  char **op_constraints;
  int n_operands;
  int n_alts;
  /* The following field value is true if there are commutative
     operands in the insn.  */
  bool commutative_op_p;
  /* If an opernad can use only one hard_register, the corresponding
     bit will be set.  */
  op_set_t single_reg_op_set;
};

/* The following is a map: insn code -> insn operand info.  */
extern struct insn_op_info **insn_code_infos;

/* The following array implements a map: insn uid -> insn operand info.  */
extern struct insn_op_info **insn_infos;

/* Additinal factor should be used to calculate cost of a copy.  */
#define COST_FACTOR 2

/* yara-ir.c: */
extern bool stack_frame_pointer_can_be_eliminated_p;
extern bool obligatory_stack_frame_pointer_elimination_p;
extern int frame_stack_pointer_offset;
extern int frame_hard_frame_pointer_offset;

extern can_t *conflict_cans;
extern int conflict_cans_num;

extern HARD_REG_SET conflict_hard_regs;

extern bitmap *regno_conflicts;
#ifdef SECONDARY_MEMORY_NEEDED
extern bitmap *regno_copy_conflicts;
#endif

extern int **slotno_conflicts;

extern int *slotno_max_ref_align;
extern int *slotno_max_ref_size;

extern int reg_class_nregs [N_REG_CLASSES] [MAX_MACHINE_MODE];
extern enum machine_mode spill_class_mode [N_REG_CLASSES] [MAX_MACHINE_MODE];
extern enum machine_mode spill_mode [MAX_MACHINE_MODE];

extern void allocno_check_failed (const allocno_t, const char *, int,
				  const char *, int);
extern void update_elim_offsets (void);
extern int get_stack_align (enum machine_mode);
extern void print_point (FILE *, struct point *);
extern const char *allocno_type_name (allocno_t);
extern void debug_allocno (allocno_t);
extern void debug_allocnos (void);
extern bool conflict_p (allocno_t, allocno_t);
extern void make_const_last (rtx **, rtx **, rtx **);
extern void make_canon_plus (rtx **, rtx **);
extern bool decode_address (rtx *, rtx **, rtx **, rtx **, rtx **,
			    HOST_WIDE_INT *, bool);
extern copy_t find_post_insn_allocno_copy (allocno_t, rtx);
extern rtx *get_container_loc (rtx *, rtx *);
extern allocno_t insn_allocno (rtx, rtx);

extern can_t create_can (void);
extern void setup_can_call_info (can_t);
extern int can_freq (can_t);
extern void print_can (FILE *, can_t);
extern void debug_can (can_t);
extern void print_cans (FILE *);
extern void debug_cans (void);

extern void find_allocno_conflicting_cans (allocno_t, bool);

extern void create_can_conflicts (can_t);

extern void debug_slotno_conflict (int);
extern void debug_all_slotno_conflicts (void);

extern void yara_ir_init_once (void);
extern void yara_ir_init (void);
extern void yara_ir_finish (void);

/* yara-trans.c: */
extern int global_allocation_cost;
extern int slot_memory_size;
extern int slot_memory_alignment;
extern int minimal_memory_load_cost [];
extern int minimal_memory_store_cost [];

extern HARD_REG_SET base_regs [MAX_MACHINE_MODE];
extern HARD_REG_SET index_regs;

extern void yara_trans_init_once (void);
extern void yara_trans_init (void);
extern void yara_trans_finish (void);

/* yara-trans.c: */
extern HOST_WIDE_INT rounded_slot_memory_size (void);
extern HOST_WIDE_INT get_stack_memory_start_frame_offset (void);
extern void print_memory_slot (FILE *, const char *, int,
			       struct memory_slot *);
extern int get_paradoxical_subreg_memory_offset (int, enum machine_mode);
extern enum machine_mode get_copy_mode (copy_t);
extern void get_copy_loc (copy_t, bool, enum machine_mode *,
			  int *, struct memory_slot **, int *);
void compact_stack (void);
extern bool hard_reg_in_set_p (int, enum machine_mode, HARD_REG_SET);
extern bool hard_reg_not_in_set_p (int, enum machine_mode, HARD_REG_SET);

extern int allocno_copy_cost (allocno_t);
extern void unassign_copy (copy_t);
extern enum machine_mode get_allocation_mode (allocno_t);
extern int get_allocno_hard_regno (allocno_t, int);
extern int get_allocno_reg_hard_regno (allocno_t, int);
extern int get_maximal_part_start_hard_regno (int hard_regno, allocno_t);
extern bool check_insns_added_since (rtx);
extern bool assign_allocno (allocno_t, enum reg_class, HARD_REG_SET, int);
extern bool assign_elimination_reg (allocno_t, enum reg_class,
				    HARD_REG_SET, int);
extern void create_tie (allocno_t, allocno_t);
extern void break_tie (allocno_t a);
extern void unassign_allocno (allocno_t);
extern bool memory_slot_intersected (struct memory_slot *,
				     struct memory_slot *);

extern enum reg_class smallest_superset_class (HARD_REG_SET);
extern void start_transaction (void);
extern void undo_transaction (void);
extern void end_transaction (void);
extern bool push_conflict_allocnos_for_spill (allocno_t, allocno_t, int);
extern bool assign_hard_regno_to_allocno_with_unassign (allocno_t,
							enum reg_class,
							HARD_REG_SET, int);
extern bool assign_class_allocno_with_unassign (allocno_t, enum reg_class,
						HARD_REG_SET);

extern bool check_hard_regno_memory_on_constraint (allocno_t, bool, int);

extern struct reg_eliminate *check_elimination_in_addr (int, int,
							HOST_WIDE_INT,
							rtx *, rtx *, bool *);
extern bool eliminate_reg (allocno_t);
extern void uneliminate_reg (allocno_t a);
extern void get_equiv_const_addr_info (rtx, int *, HOST_WIDE_INT *);

extern void get_equiv_const_elimination_info (rtx, int *, HOST_WIDE_INT *);

extern void eliminate_virtual_registers (int (*) (allocno_t, enum reg_class,
						  HARD_REG_SET));

/* yara-insn.c:  */
extern void setup_possible_allocno_alternatives (struct insn_op_info *,
						  allocno_t, bool);
extern void setup_possible_alternatives (bool);
extern void make_commutative_exchange (allocno_t);
extern void allocate_insn_allocnos (rtx, bool (*) (allocno_t),
				    void (*) (allocno_t *, int, rtx, int),
				    int (*) (allocno_t, enum reg_class));
extern void yara_insn_init (void);
extern void yara_insn_finish (void);

/* yara-color.c: */
extern void debug_cover_classes (void);

extern int (*pseudo_reg_copy_cost_func) (copy_t);

extern void yara_color_init_once (void);
extern void yara_color (void);

/* yara-final.c: */
extern bool unnecessary_copy_p (copy_t);
extern void yara_rewrite (void);

/* yara.c: */
extern HARD_REG_SET zero_hard_reg_set;
extern HARD_REG_SET one_hard_reg_set;
extern unsigned char mode_inner_mode [NUM_MACHINE_MODES];
extern HARD_REG_SET reg_mode_hard_regset
                    [FIRST_PSEUDO_REGISTER] [NUM_MACHINE_MODES];
extern int memory_move_cost [MAX_MACHINE_MODE] [N_REG_CLASSES] [2];
extern int register_move_cost [MAX_MACHINE_MODE] [N_REG_CLASSES]
                              [N_REG_CLASSES];
extern bool class_subset_p [N_REG_CLASSES] [N_REG_CLASSES];
extern short class_hard_regs [N_REG_CLASSES] [FIRST_PSEUDO_REGISTER];
extern int class_hard_regs_num [N_REG_CLASSES];
extern short class_hard_reg_index [N_REG_CLASSES] [FIRST_PSEUDO_REGISTER];
extern HARD_REG_SET no_alloc_regs;
extern int available_class_regs [N_REG_CLASSES];

extern void set_non_alloc_regs (bool);
extern void *yara_allocate (size_t);
extern void yara_free (void *addr);
extern bitmap yara_allocate_bitmap (void);
extern void yara_free_bitmap (bitmap);
extern regset yara_allocate_regset (void);
extern void yara_free_regset (regset);
extern void debug_disposition (void);
