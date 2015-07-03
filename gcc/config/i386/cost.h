/* Return the right cost to give to an operation
   to make the cost of the corresponding register-to-register instruction
   N times that of a fast register-to-register instruction.  */
#define COSTS_N_INSNS(N) ((N) * 4)

#define DUMMY_STRINGOP_ALGS {libcall, {{-1, libcall, false}}}

#define false 0
#define true 1

/* Algorithm to expand string function with.  */
enum stringop_alg
{
#undef DEF_ENUM
#define DEF_ENUM

#undef DEF_ALG
#define DEF_ALG(alg, name) alg, 

#include "stringop.def"
last_alg

#undef DEF_ENUM
#undef DEF_ALG
};

#define MAX_STRINGOP_ALGS 4

/* Specify what algorithm to use for stringops on known size.
   When size is unknown, the UNKNOWN_SIZE alg is used.  When size is
   known at compile time or estimated via feedback, the SIZE array
   is walked in order until MAX is greater then the estimate (or -1
   means infinity).  Corresponding ALG is used then.
   When NOALIGN is true the code guaranting the alignment of the memory
   block is skipped.

   For example initializer:
    {{256, loop}, {-1, rep_prefix_4_byte}}
   will use loop for blocks smaller or equal to 256 bytes, rep prefix will
   be used otherwise.  */
typedef struct stringop_algs
{
  const enum stringop_alg unknown_size;
  const struct stringop_strategy {
    const int max;
    const enum stringop_alg alg;
    int noalign;
  } size [MAX_STRINGOP_ALGS];
} stringop_algs;

/* Define the specific costs for a given cpu */

struct processor_costs {
  const int add;		/* cost of an add instruction */
  const int lea;		/* cost of a lea instruction */
  const int shift_var;		/* variable shift costs */
  const int shift_const;	/* constant shift costs */
  const int mult_init[5];	/* cost of starting a multiply
				   in QImode, HImode, SImode, DImode, TImode*/
  const int mult_bit;		/* cost of multiply per each bit set */
  const int divide[5];		/* cost of a divide/mod
				   in QImode, HImode, SImode, DImode, TImode*/
  int movsx;			/* The cost of movsx operation.  */
  int movzx;			/* The cost of movzx operation.  */
  const int large_insn;		/* insns larger than this cost more */
  const int move_ratio;		/* The threshold of number of scalar
				   memory-to-memory move insns.  */
  const int movzbl_load;	/* cost of loading using movzbl */
  const int int_load[3];	/* cost of loading integer registers
				   in QImode, HImode and SImode relative
				   to reg-reg move (2).  */
  const int int_store[3];	/* cost of storing integer register
				   in QImode, HImode and SImode */
  const int fp_move;		/* cost of reg,reg fld/fst */
  const int fp_load[3];		/* cost of loading FP register
				   in SFmode, DFmode and XFmode */
  const int fp_store[3];	/* cost of storing FP register
				   in SFmode, DFmode and XFmode */
  const int mmx_move;		/* cost of moving MMX register.  */
  const int mmx_load[2];	/* cost of loading MMX register
				   in SImode and DImode */
  const int mmx_store[2];	/* cost of storing MMX register
				   in SImode and DImode */
  const int sse_move;		/* cost of moving SSE register.  */
  const int sse_load[3];	/* cost of loading SSE register
				   in SImode, DImode and TImode*/
  const int sse_store[3];	/* cost of storing SSE register
				   in SImode, DImode and TImode*/
  const int mmxsse_to_integer;	/* cost of moving mmxsse register to
				   integer and vice versa.  */
  const int l1_cache_size;	/* size of l1 cache, in kilobytes.  */
  const int l2_cache_size;	/* size of l2 cache, in kilobytes.  */
  const int prefetch_block;	/* bytes moved to cache for prefetch.  */
  const int simultaneous_prefetches; /* number of parallel prefetch
				   operations.  */
  const int branch_cost;	/* Default value for BRANCH_COST.  */
  const int fadd;		/* cost of FADD and FSUB instructions.  */
  const int fmul;		/* cost of FMUL instruction.  */
  const int fdiv;		/* cost of FDIV instruction.  */
  const int fabs;		/* cost of FABS instruction.  */
  const int fchs;		/* cost of FCHS instruction.  */
  const int fsqrt;		/* cost of FSQRT instruction.  */
				/* Specify what algorithm
				   to use for stringops on unknown size.  */
  struct stringop_algs *memcpy, *memset;
  const int scalar_stmt_cost;   /* Cost of any scalar operation, excluding
				   load and store.  */
  const int scalar_load_cost;   /* Cost of scalar load.  */
  const int scalar_store_cost;  /* Cost of scalar store.  */
  const int vec_stmt_cost;      /* Cost of any vector operation, excluding
                                   load, store, vector-to-scalar and
                                   scalar-to-vector operation.  */
  const int vec_to_scalar_cost;    /* Cost of vect-to-scalar operation.  */
  const int scalar_to_vec_cost;    /* Cost of scalar-to-vector operation.  */
  const int vec_align_load_cost;   /* Cost of aligned vector load.  */
  const int vec_unalign_load_cost; /* Cost of unaligned vector load.  */
  const int vec_store_cost;        /* Cost of vector store.  */
  const int cond_taken_branch_cost;    /* Cost of taken branch for vectorizer
					  cost model.  */
  const int cond_not_taken_branch_cost;/* Cost of not taken branch for
					  vectorizer cost model.  */
};
