#include "cost.h"

const int ix86_cost_model_size = sizeof (struct processor_costs);

/* PentiumPro has optimized rep instructions for blocks aligned by 8 bytes
   (we ensure the alignment).  For small blocks inline loop is still a
   noticeable win, for bigger blocks either rep movsl or rep movsb is
   way to go.  Rep movsb has apparently more expensive startup time in CPU,
   but after 4K the difference is down in the noise.  */
static stringop_algs pentiumpro_memcpy[2] = {
  {rep_prefix_4_byte, {{128, loop, false}, {1024, unrolled_loop, false},
                       {8192, rep_prefix_4_byte, false},
                       {-1, rep_prefix_1_byte, false}}},
  DUMMY_STRINGOP_ALGS};
static stringop_algs pentiumpro_memset[2] = {
  {rep_prefix_4_byte, {{1024, unrolled_loop, false},
                       {8192, rep_prefix_4_byte, false},
                       {-1, libcall, false}}},
  DUMMY_STRINGOP_ALGS};
const
struct processor_costs ix86_cost_model = {
  COSTS_N_INSNS (1),			/* cost of an add instruction */
  COSTS_N_INSNS (1),			/* cost of a lea instruction */
  COSTS_N_INSNS (1),			/* variable shift costs */
  COSTS_N_INSNS (1),			/* constant shift costs */
  {COSTS_N_INSNS (4),			/* cost of starting multiply for QI */
   COSTS_N_INSNS (4),			/*				 HI */
   COSTS_N_INSNS (4),			/*				 SI */
   COSTS_N_INSNS (4),			/*				 DI */
   COSTS_N_INSNS (4)},			/*			      other */
  0,					/* cost of multiply per each bit set */
  {COSTS_N_INSNS (17),			/* cost of a divide/mod for QI */
   COSTS_N_INSNS (17),			/*			    HI */
   COSTS_N_INSNS (17),			/*			    SI */
   COSTS_N_INSNS (17),			/*			    DI */
   COSTS_N_INSNS (17)},			/*			    other */
  COSTS_N_INSNS (1),			/* cost of movsx */
  COSTS_N_INSNS (1),			/* cost of movzx */
  8,					/* "large" insn */
  6,					/* MOVE_RATIO */
  2,				     /* cost for loading QImode using movzbl */
  {4, 4, 4},				/* cost of loading integer registers
					   in QImode, HImode and SImode.
					   Relative to reg-reg move (2).  */
  {2, 2, 2},				/* cost of storing integer registers */
  2,					/* cost of reg,reg fld/fst */
  {2, 2, 6},				/* cost of loading fp registers
					   in SFmode, DFmode and XFmode */
  {4, 4, 6},				/* cost of storing fp registers
					   in SFmode, DFmode and XFmode */
  2,					/* cost of moving MMX register */
  {2, 2},				/* cost of loading MMX registers
					   in SImode and DImode */
  {2, 2},				/* cost of storing MMX registers
					   in SImode and DImode */
  2,					/* cost of moving SSE register */
  {2, 2, 8},				/* cost of loading SSE registers
					   in SImode, DImode and TImode */
  {2, 2, 8},				/* cost of storing SSE registers
					   in SImode, DImode and TImode */
  3,					/* MMX or SSE register to integer */
  8,					/* size of l1 cache.  */
  256,					/* size of l2 cache  */
  32,					/* size of prefetch block */
  6,					/* number of parallel prefetches */
  2,					/* Branch cost */
  COSTS_N_INSNS (3),			/* cost of FADD and FSUB insns.  */
  COSTS_N_INSNS (5),			/* cost of FMUL instruction.  */
  COSTS_N_INSNS (56),			/* cost of FDIV instruction.  */
  COSTS_N_INSNS (2),			/* cost of FABS instruction.  */
  COSTS_N_INSNS (2),			/* cost of FCHS instruction.  */
  COSTS_N_INSNS (56),			/* cost of FSQRT instruction.  */
  pentiumpro_memcpy,
  pentiumpro_memset,
  1,					/* scalar_stmt_cost.  */
  1,					/* scalar load_cost.  */
  1,					/* scalar_store_cost.  */
  1,					/* vec_stmt_cost.  */
  1,					/* vec_to_scalar_cost.  */
  1,					/* scalar_to_vec_cost.  */
  1,					/* vec_align_load_cost.  */
  2,					/* vec_unalign_load_cost.  */
  1,					/* vec_store_cost.  */
  3,					/* cond_taken_branch_cost.  */
  1,					/* cond_not_taken_branch_cost.  */
};
