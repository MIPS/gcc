/*
 * Copyright (c) 2008 Google Inc. All rights reserved.
 *
 * $Header: $
 */
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "function.h"
#include "basic-block.h"
#include "tree-pass.h"
#include "target.h"	/* for targetm */
#include "output.h"	/* for asm_fprintf */

/* start taken from dwarf2out.c */
#ifndef FUNC_BEGIN_LABEL
  #define FUNC_BEGIN_LABEL	"LFB"
#endif

#ifndef FUNC_END_LABEL
  #define FUNC_END_LABEL	"LFE"
#endif
/* end taken from dwarf2out.c */

#include "icg.h"

static
void bad_register_argument(unsigned r, char type)
{
  fflush(stdout);
  fprintf(stderr, "bad argument to register format: r=%3d rtype='%c'\n",
    r, type);
  gcc_assert(false);
  /*NOTREACHED*/
}

/*
 * allowable types in argument t are:
 *    'b' an 8-bit QI general-purpose register (e.g., %al  and %r8b)
 *    'h' an 8-bit QI general-purpose register (e.g., %ah  and %bh)
 *    'w' a 16-bit HI general-purpose register (e.g., %ax  and %r8w)
 *    'd' a 32-bit SI general-purpose register (e.g., %eax and %r8d)
 *    'q' a 64-bit DI general-purpose register (e.g., %rax and %r8)
 *    'x' an xmm register (e.g., %xmm8)
 */

static
const char *register_format_str(char *buf,
    size_t buflen, unsigned r, char type)
{
  r = icg_path_compress(r);
  switch (r) {
  case REG_RAX:
    switch (type) {
    case 'b': return("%al");
    case 'h': return("%ah");
    case 'w': return("%ax");
    case 'l':
    case 'd': return("%eax");
    case 'q': return("%rax");
    default:
      bad_register_argument(r, type);
      /*NOTREACHED*/
      break;
    }
    break;

  case REG_RBX:
    switch (type) {
    case 'b': return("%bl");
    case 'h': return("%bh");
    case 'w': return("%bx");
    case 'l':
    case 'd': return("%ebx");
    case 'q': return("%rbx");
    default:
      bad_register_argument(r, type);
      break;
    }
    break;

  case REG_RCX:
    switch (type) {
    case 'b': return("%cl");
    case 'h': return("%ch");
    case 'w': return("%cx");
    case 'l':
    case 'd': return("%ecx");
    case 'q': return("%rcx");
    default:
      bad_register_argument(r, type);
      /*NOTREACHED*/
      break;
    }
    break;

  case REG_RDX:
    switch (type) {
    case 'b': return("%dl");
    case 'h': return("%dh");
    case 'w': return("%dx");
    case 'l':
    case 'd': return("%edx");
    case 'q': return("%rdx");
    default:
      bad_register_argument(r, type);
      /*NOTREACHED*/
      break;
    }
    break;

  case REG_RSI:
    switch (type) {
    case 'b': return("%sil");
    case 'w': return("%si");
    case 'l':
    case 'd': return("%esi");
    case 'q': return("%rsi");
    default:
      bad_register_argument(r, type);
      /*NOTREACHED*/
      break;
    }
    break;

  case REG_RDI:
    switch (type) {
    case 'b': return("%dil");
    case 'w': return("%di");
    case 'l':
    case 'd': return("%edi");
    case 'q': return("%rdi");
    default:
      bad_register_argument(r, type);
      /*NOTREACHED*/
      break;
    }
    break;

  case REG_RBP:
    switch (type) {
    case 'b': return("%bpl");
    case 'w': return("%bp");
    case 'l':
    case 'd': return("%ebp");
    case 'q': return("%rbp");
    default:
      bad_register_argument(r, type);
      /*NOTREACHED*/
      break;
    }
    break;

  case REG_RSP:
    switch (type) {
    case 'b': return("%spl");
    case 'w': return("%sp");
    case 'l':
    case 'd': return("%esp");
    case 'q': return("%rsp");
    default:
      bad_register_argument(r, type);
      /*NOTREACHED*/
      break;
    }
    break;

  /*
   * r8..r15
   */
  case 37: case 38: case 39: case 40: case 41: case 42: case 43: case 44:
    switch (type) {
    case 'b': snprintf(buf, buflen, "%%r%ub", r - 29); return buf;
    case 'w': snprintf(buf, buflen, "%%r%uw", r - 29); return buf;
    case 'l':
    case 'd': snprintf(buf, buflen, "%%r%ud", r - 29); return buf;
    case 'q': snprintf(buf, buflen, "%%r%u", r - 29); return buf;
    default:
      bad_register_argument(r, type);
      /*NOTREACHED*/
      break;
    }
    break;

  /*
   * xmm0 .. xmm7
   */
  case 21: case 22: case 23: case 24: case 25: case 26: case 27: case 28:
    switch (type) {
    case 'x':
      snprintf(buf, buflen, "%%xmm%u", r - 21); return buf;
    default:
      bad_register_argument(r, type);
      /*NOTREACHED*/
      break;
    }
    break;

  /*
   * xmm8..xmm15
   */
  case 45: case 46: case 47: case 48: case 49: case 50: case 51: case 52:
    switch (type) {
    case 'x':
      snprintf(buf, buflen, "%%xmm%u", r - 45 + 8); return buf;
    default:
      bad_register_argument(r, type);
      /*NOTREACHED*/
      break;
    }
    break;

  default:
    switch (type) {
    case 'b': snprintf(buf, buflen, "%%r%ub", r); return buf;
    case 'w': snprintf(buf, buflen, "%%r%uw", r); return buf;
    case 'l':
    case 'd': snprintf(buf, buflen, "%%r%ud", r); return buf;
    case 'q': snprintf(buf, buflen, "%%r%u", r); return buf;
    case 'x': snprintf(buf, buflen, "%%xmm%u", r); return buf;
    default:
      bad_register_argument(r, type);
      /*NOTREACHED*/
      break;
    }
    break;
  }
  return "?";
}

static
void register_format(unsigned r, char type)
{
  char buf[BUFSIZ];
  const char *str;
  buf[0] = 0;
  str = register_format_str(buf, sizeof(buf), r, type);
  fputs(str, dump_file);
}


static
void bid_format_struct(const amode a)
{
  if (a.string) {
    fprintf(dump_file, "%s", a.string);
    if (a.disp > 0) {
      fprintf(dump_file, "+");
    }
  }
  /*
   * Apparently due to a buglet in gcc4.3.1,
   * icg+rtl prints 0(%rbp) for some reason,
   * so we want to emulate that so we can diff assembly code output.
   */
  if (a.disp != 0 || a.base == REG_RBP) {
    fprintf(dump_file, "%d", a.disp);
  }
  fprintf(dump_file, "(");
  if (a.base_valid && !a.base_rip) {
    register_format(a.base, 'q');	/* pointers are always 64-bits */
  } else if (a.base_rip) {
    fprintf(dump_file, "%%rip");
  }
  if (a.scale > 0) {
    fprintf(dump_file, ",");
    register_format(a.index, 'q');
    if (a.scale > 1) {
      fprintf(dump_file, ",%d", a.scale);
    }
  }
  fprintf(dump_file, ")");
}

static
void address_format(icg_node *r)
{
  bid_format_struct(r->a);
}



static
void dump(const char *name)
{
  fprintf(dump_file, "\t%s\n", name);
}


/*
 * single register dst
 */
static
void dumpR(const char *name, unsigned dst, char dst_type)
{
  fprintf(dump_file, "\t%s\t", name);
  register_format(dst, dst_type);
  fprintf(dump_file, "\n");
}

/*
 * single memory
 */
static
void dumpM(const char *name, icg_node *a)
{
  fprintf(dump_file, "\t%s\t", name);
  address_format(a);
  fprintf(dump_file, "\n");
}


/*
 * 2 register operand
 */
static
void dumpRR(const char *name,
	    unsigned src, char src_type,
	    unsigned dst, char dst_type)
{
  fprintf(dump_file, "\t%s\t", name);
  register_format(src, src_type);
  fprintf(dump_file, ", ");
  register_format(dst, dst_type);
  fprintf(dump_file, "\n");
}


/*
 * 3 register operand
 */
static
void dumpRRR(const char *name,
	     unsigned src1, char src1_type,
	     unsigned src2, char src2_type,
	     unsigned dst, char dst_type)
{
  fprintf(dump_file, "\t%s\t", name);
  register_format(src1, src1_type);
  fprintf(dump_file, ", ");
  register_format(src2, src2_type);
  fprintf(dump_file, ", ");
  register_format(dst, dst_type);
  fprintf(dump_file, "\n");
}

static
void dumpasm_worker(FILE *dump_file,
  const char *template,
  const char *index_to_str[], size_t nindex_to_str)
{
  if (template) {
    const char *cp;
    for (cp = template; *cp; cp++) {
      switch (*cp) {
      case '%':
        cp += 1;
	if (*cp == '%') {
	  fputc('%', dump_file);
	  continue;
	} else if (ISDIGIT(*cp) && ((size_t)(*cp-'0')) < nindex_to_str) {
	  fputs(index_to_str[*cp - '0'], dump_file);
	} else {
	  icg_nyi("unknown or invalid asm escape character '%c' in ``%s''",
	    *cp, template);
	}
	break;
      default:
        fputc(*cp, dump_file);
	break;
      }
    }
  }
}

static
void dumpasmRRR(const char *template,
	     unsigned src1, char src1_type,
	     unsigned src2, char src2_type,
	     unsigned dst, char dst_type)
{
  char dst_fmt[BUFSIZ];
  char src1_fmt[BUFSIZ];
  char src2_fmt[BUFSIZ];
  const char *index_to_str[3];

  index_to_str[0] = register_format_str(dst_fmt,  sizeof(dst_fmt),   dst,  dst_type);
  index_to_str[1] = register_format_str(src1_fmt, sizeof(src1_fmt), src1, src1_type);
  index_to_str[2] = register_format_str(src2_fmt, sizeof(src2_fmt), src2, src2_type);

  fprintf(dump_file, "\t");
  dumpasm_worker(dump_file, template, index_to_str, 3);
  fprintf(dump_file, "\n");
}

/*
 * register src, memory dst
 */
static
void dumpRM(const char *name, unsigned src, char src_type, icg_node *a)
{
  (void) dumpRM;
  fprintf(dump_file, "\t%s\t", name);
  register_format(src, src_type);
  fprintf(dump_file, ", ");
  address_format(a);
  fprintf(dump_file, "\n");
}


static
void dumpRMbid(const char *name, unsigned src, char src_type,
	       unsigned base, bool base_valid,
	       unsigned index, int scale, int disp, const char *s)
	       {
  fprintf(dump_file, "\t%s\t", name);
  register_format(src, src_type);
  fprintf(dump_file, ", ");
  {
    amode a;
    a.base = base;
    a.base_valid = base_valid;
    a.base_rip = 0;
    a.index = index;
    a.scale = scale;
    a.disp = disp;
    a.string = s;
    bid_format_struct(a);
  }
  fprintf(dump_file, "\n");
}

/*
 * memory src, register dst
 */
static
void dumpMR(const char *name, icg_node *a, unsigned dst, char dst_type)
{
  fprintf(dump_file, "\t%s\t", name);
  address_format(a);
  fprintf(dump_file, ", ");
  register_format(dst, dst_type);
  fprintf(dump_file, "\n");
}


/*
 * memory src (bid format), register dst
 */
static
void dumpMbidR(const char *name,
	       unsigned base, bool base_valid,
	       unsigned index, int scale,
	       int disp, const char *s,
	       unsigned dst, char dst_type)
{
  fprintf(dump_file, "\t%s\t", name);
  {
    amode a;
    a.base = base;
    a.base_valid = base_valid;
    a.base_rip = 0;
    a.index = index;
    a.scale = scale;
    a.disp = disp;
    a.string = s;
    bid_format_struct(a);
  }
  fprintf(dump_file, ", ");
  register_format(dst, dst_type);
  fprintf(dump_file, "\n");
}



/*
 * immediate operand
 */
static
void imm_format(icg_node *imm)
{
  fprintf(dump_file, "$");
  if (imm->a.string) {
    fprintf(dump_file, "%s", imm->a.string);
    if (imm->val > 0) {
      fprintf(dump_file, "+%lld", imm->val);
    }
    else if (imm->val < 0) {
      fprintf(dump_file, "%lld", imm->val);
    }
  }
  else
    fprintf(dump_file, "%lld", imm->val);
}


/*
 * immediate src, register dst
 */
static
void dumpIR(const char *name, icg_node *imm, unsigned dst, char dst_type)
{
  fprintf(dump_file, "\t%s\t", name);
  imm_format(imm);
  fprintf(dump_file, ", ");
  register_format(dst, dst_type);
  fprintf(dump_file, "\n");
}


/*
 * operator with a count of 1 (used for salq, shrq and sarq)
 */
static
void dumpIR1(const char *name, unsigned dst, char dst_type)
{
  fprintf(dump_file, "\t%s\t$1,", name);
  register_format(dst, dst_type);
  fprintf(dump_file, "\n");
}


/*
 * operator with a count of -1 (used for sbbl, sometimes)
 */
static
void dumpIRm1(const char *name, unsigned dst, char dst_type)
{
  fprintf(dump_file, "\t%s\t$1,", name);
  register_format(dst, dst_type);
  fprintf(dump_file, "\n");
}


/*
 * immediate source n, register dst
 */
static
void dumpIRn(const char *name, int n, unsigned dst, char dst_type)
{
  fprintf(dump_file, "\t%s\t$%d,", name, n);
  register_format(dst, dst_type);
  fprintf(dump_file, "\n");
}


/*
 * immediate, src register, dst register
 */
static
void dumpIRR(const char *name, icg_node *imm,
    unsigned src, unsigned dst, char rt)
{
  fprintf(dump_file, "\t%s\t", name);
  imm_format(imm);
  fprintf(dump_file, ", ");
  register_format(src, rt);
  fprintf(dump_file, ", ");
  register_format(dst, rt);
  fprintf(dump_file, "\n");
}

/*
 * immediate memory
*/
static
void dumpIM(const char *name, icg_node *imm, icg_node *a)
{
  fprintf(dump_file, "\t%s\t", name);
  imm_format(imm);
  fprintf(dump_file, ", ");
  address_format(a);
  fprintf(dump_file, "\n");
}


/*
 * immediate memory register
 */
static
void dumpIMR(const char *name, icg_node *imm, icg_node *a, unsigned dst, char dst_type)
{
  fprintf(dump_file, "\t%s\t", name);
  imm_format(imm);
  fprintf(dump_file, ", ");
  address_format(a);
  fprintf(dump_file, ", ");
  register_format(dst, dst_type);
  fprintf(dump_file, "\n");
}


static
void dump_call(const char *s)
{
  fprintf(dump_file, "\t" "call\t%s\n", s);
}

/*
 * Call indirect through a register
 */
static
void dump_callR(unsigned r)
{
  const char dst_type = 'q';
  fprintf(dump_file, "\t" "call\t*");
  register_format(r, dst_type);
  fprintf(dump_file, "\n");
}

/*
 * Call indirect through a memory location
 */
static
void dump_callM(const amode a)
{
  fprintf(dump_file, "\t" "call\t*");
  bid_format_struct(a);
  fprintf(dump_file, "\n");
}

/*
 * dump register to register copy only if the copy is non trivial
 */
static
void dump_copy(const char *name, unsigned src, unsigned dst, char type)
{
  if (icg_path_compress(src) != icg_path_compress(dst)) {
    dumpRR(name, src, type, dst, type);
  }
}

static
void dump_jump(const char *opcode_name, icg_node *p)
{
  const rtx ref = XEXP (p->rtl, 0);
  const int ln = CODE_LABEL_NUMBER(ref);
  char buf[BUFSIZ];
  fprintf(dump_file, "\t" "%s" "\t", opcode_name);
  snprintf(buf, sizeof(buf), ".L%d", ln);
  ASM_OUTPUT_LABELREF (dump_file, buf);
  fprintf(dump_file, "\n");
}

static
void dump_jumpR(const char *opcode_name, unsigned src, char src_type)
{
  fprintf(dump_file, "\t" "%s" "\t", opcode_name);
  register_format(src, src_type);
  fprintf(dump_file, "\n");
}

static
void dump_jumpM(const char *opcode_name, icg_node *a)
{
  fprintf(dump_file, "\t" "%s" "\t", opcode_name);
  address_format(a);
  fprintf(dump_file, "\n");
}

/*
 * get an array data structure that maps rule number to grammar source locus
 */
#include "icg-rulemaps.c"

/*
 * Visit each node in the parse tree.
 * Print the burm rule if do_burm is non-zero.
 * Also print the assembly code.
 * Execute the debug portion of the rule as given in the plug spec.
 */
static
void reduce_debug(icg_node *p, NT goalNT, int do_burm, int depth)
{
  icg_node *kid[MAX_KIDS];
  const RuleNumber rule = icg_burm_rule(p->state_label, goalNT);
  const NT *nts = icg_burm_nts[rule];

  if (do_burm) {
    icg_trace_rule(dump_file, p, depth, goalNT);
  }
  icg_burm_kids(p, rule, kid);
  /*
   * The visit permutation is only valid for non-chain rules with >= 2 nonterminals
   */
  if (nts[0] && nts[1] && p->perm_kids >= 2) {
    int i;
    for (i = 0; i < p->perm_kids; i++) {
      const int k = p->perm[i];
      reduce_debug(kid[k], nts[k], do_burm, depth+1);
    }
  }
  else {
    int i;
    for (i = 0; nts[i]; i++) {
      reduce_debug(kid[i], nts[i], do_burm, depth+1);
    }
  }
  switch (rule) {
#include "icg-debug.cases"
  }
}

void icg_trace_rule(FILE *dump_file, const icg_node *p, int depth, NT goalNT)
{
  if (dump_file == 0) dump_file = stdout;
  if (p == 0) return;
  if (p->state_label) {
    const RuleNumber rule = icg_burm_rule(p->state_label, goalNT);
    int i;
    for (i = 0; i < depth; i++) {
      fputc(' ', dump_file);
    }
    fprintf(dump_file, "%s   (at %s:%d) {rule %d} <tree:%p>\n",
      icg_burm_string[rule],
      icg_rule_to_locus[rule].filename,
      icg_rule_to_locus[rule].line_number,
      rule,
      (const void *)p
    );
  }
}

static
void bbhook(void)
{
}

static
void dump_bb_label(basic_block bb)
{
  rtx label = BB_HEAD(bb);	/* do not call block_label, as it will make a new block */
  const int ln = CODE_LABEL_NUMBER(label);
  char buf[BUFSIZ];
  if (ln >= 0) {
    snprintf(buf, sizeof(buf), ".L%d", ln);
    ASM_OUTPUT_LABEL(dump_file, buf);
  } else {
    bbhook();
  }
}

/*
 * Dump out the entire function.
 * First, dump out the burm rules + assembly code.
 * Then, dump out just the assembly code.
 */
void
icg_debug(const char *whence) {
  (void) dumpRRR;	/* shut up gcc "defined but not used" message */
  (void) dumpRMbid;	/* shut up gcc "defined but not used" message */
  (void) dump_jumpM;	/* shut up gcc "defined but not used" message */
  if (dump_file) {
    {
      basic_block bb;
      fprintf(dump_file, "; START icg_debug part 1 %s {\n", whence);
      FOR_EACH_BB(bb) {
	rtx insn;
	dump_bb_label(bb);
	FOR_BB_INSNS(bb, insn) {
	  unsigned id = INSN_UID(insn);
	  icg_node *tree = icg_insn2tree[id];
	  if (tree)
	    reduce_debug(tree, /*stmt*/1, /*do_burm*/1, 0);
	}
      }
      fprintf(dump_file, ";   END icg_debug part 1 %s }\n", whence);
      fprintf(dump_file, "\n\n");
    }

    {
      basic_block bb;
      const char *aname = (IDENTIFIER_POINTER
		 (DECL_ASSEMBLER_NAME (current_function_decl)));
      fprintf(dump_file, "; START icg_debug part 2 %s {\n", whence);

      fprintf(dump_file, "\t" ".p2align 4,,15\n");
      fprintf(dump_file, ".globl %s\n", aname);
      fprintf(dump_file, "\t" ".type" "\t" "%s, @function\n", aname);
      fprintf(dump_file, "%s:\n", aname);
      /* see dwarf2out.c */
      ASM_OUTPUT_DEBUG_LABEL (dump_file, FUNC_BEGIN_LABEL,
			      current_function_funcdef_no);

      FOR_EACH_BB(bb) {
	rtx insn;
	dump_bb_label(bb);
	FOR_BB_INSNS(bb, insn) {
	  unsigned id = INSN_UID(insn);
	  icg_node *tree = icg_insn2tree[id];
	  if (tree)
	    reduce_debug(tree, /*stmt*/1, /*do_burm*/0, 0);
	}
      }
      fprintf(dump_file, "\t" "ret" "\n");
      /* see dwarf2out.c */
      ASM_OUTPUT_DEBUG_LABEL (dump_file, FUNC_END_LABEL,
			      current_function_funcdef_no);
      fprintf(dump_file, "\t" ".size" "\t" "%s, .-%s\n", aname, aname);
      fprintf(dump_file, ";   END icg_debug part 2 %s }\n", whence);
    }
  }
}
