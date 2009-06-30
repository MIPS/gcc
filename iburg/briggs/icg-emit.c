/*
 *    Copyright (C) 2008  Free Software Foundation, Inc.
 *
 *    This file is part of GCC.
 *
 *    GCC is free software; you can redistribute it and/or modify it under
 *    the terms of the GNU General Public License as published by the Free
 *    Software Foundation; either version 3, or (at your option) any later
 *    version.
 *
 *    GCC is distributed in the hope that it will be useful, but WITHOUT ANY
 *    WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *    for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with GCC; see the file COPYING3.  If not see
 *    <http://www.gnu.org/licenses/>.
 *
 *
 *    This file implements the out of ICG generation of RTL.
 */

#include "obstack.h"
#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "rtl.h"
#include "flags.h"
#include "function.h"
#include "basic-block.h"
#include "tree-pass.h"
#include "expr.h"
#include "icg.h"
#include "icg-opcode.h"


/* ===============================================================
   Helper Functions
   =============================================================== */

/* Bit Scan right, in terms of the Count Leading Zero rtl opcode */
static rtx
gen_rtx_BSR_DI(enum machine_mode mode, rtx l)
{
  return gen_rtx_MINUS(mode, GEN_INT(63), gen_rtx_CLZ(mode, l));
}
static rtx
gen_rtx_BSR_SI(enum machine_mode mode, rtx l)
{
  return gen_rtx_MINUS(mode, GEN_INT(31), gen_rtx_CLZ(mode, l));
}
static rtx
gen_rtx_BSR_HI(enum machine_mode mode, rtx l)
{
  return gen_rtx_MINUS(mode, GEN_INT(15), gen_rtx_CLZ(mode, l));
}

static rtx
gen_rtx_ANDNOT(enum machine_mode mode, rtx l, rtx r)
{
  return gen_rtx_AND(mode, gen_rtx_NOT(mode, l), r);
}

static rtx
gen_rtx_LT_swap(enum machine_mode mode, rtx l, rtx r)
{
  return gen_rtx_LT(mode, r, l);
}

static rtx
gen_rtx_LE_swap(enum machine_mode mode, rtx l, rtx r)
{
  return gen_rtx_LE(mode, r, l);
}

static rtx
gen_rtx_COMPARE_assist(bool is_test_variant,
  enum machine_mode cc_mode, enum machine_mode rtx_mode,
  rtx l, rtx r)
{
  if (is_test_variant) {
    return gen_rtx_COMPARE(cc_mode, gen_rtx_AND(rtx_mode, l, r), GEN_INT(0));
  } else {
    return gen_rtx_COMPARE(cc_mode, l, r);
  }
}

static rtx
gen_rtx_DIV_helper(bool issigned, enum machine_mode mode, rtx l, rtx r)
{
  if (issigned) {
    return  gen_rtx_DIV(mode, l, r);
  } else {
    return gen_rtx_UDIV(mode, l, r);
  }
}

static rtx
gen_rtx_MOD_helper(bool issigned, enum machine_mode mode, rtx l, rtx r)
{
  if (issigned) {
    return  gen_rtx_MOD(mode, l, r);
  } else {
    return gen_rtx_UMOD(mode, l, r);
  }
}

static rtx
gen_rtx_IEEE_MAX_helper(enum machine_mode mode, rtx l, rtx r)
{
  return gen_rtx_UNSPEC(mode, gen_rtvec(2, l, r), UNSPEC_IEEE_MAX);
}

static rtx
gen_rtx_IEEE_MIN_helper(enum machine_mode mode, rtx l, rtx r)
{
  return gen_rtx_UNSPEC(mode, gen_rtvec(2, l, r), UNSPEC_IEEE_MIN);
}

static rtx
gen_rtx_ADD_CARRY(enum machine_mode mode, rtx l, rtx r)
{
  return gen_rtx_UNSPEC(mode, gen_rtvec(2, l, r), UNSPEC_ADD_CARRY);
}

/*
 * gen_rtx_addr - generate an rtx for
 *    base-reg + index-reg * scale + disp
 *
 * if disp == 0, there is no displacement
 * if scale== 0, there is no index register.
 * if (!valid), there is no base-reg
 */
static rtx
gen_rtx_addr(enum machine_mode mode, rtx base, rtx index, rtx disp)
{
  return gen_rtx_PLUS(mode, gen_rtx_PLUS(mode, index, disp), base);
}

static rtx
gen_rtx_imm_constant(long long int val, const char *string, rtx original_rtx)
{
  if (string) {
    rtx symbol = 0;
    const enum rtx_code expr_code = GET_CODE(original_rtx);
    if (expr_code == CONST) {
      return original_rtx;
    }
    /*
     * Make a copy of the original_rtx, so that the handles to the constant
     * pool will be preserved.
     * Symbol is almost surely going to be original_rtx.
     */
    symbol = copy_rtx(original_rtx);
    if (val) {
      return gen_rtx_CONST(DImode,
	gen_rtx_PLUS(DImode, symbol, GEN_INT(val)));
    } else {
      return symbol;
    }
  } else {
    return GEN_INT(val);
  }
}

static rtx
gen_rtx_imm(long long int val, const char *string)
{
  return gen_rtx_imm_constant(val, string, 0);
}


static void
dump_insn(rtx node, const char *fname, int lineno)
{
  if (dump_file) {
    fprintf(dump_file, " RTL: %s:%d: ", fname ? fname : "", lineno);
    print_inline_rtx(dump_file, node, 0);
    fprintf(dump_file, "\n");
  }
}


/*
 * target instructions built from emit_plain do not kill the condition codes
 */
static void
icg_emit_plain(rtx guts)
{
  dump_insn(emit_insn(guts), 0, 0);
}
static void
icg_emit_plain_tagged(rtx guts, const char *fname, int lineno)
{
  rtx emit_rtx = emit_insn(guts);
  dump_insn(emit_rtx, fname, lineno);
}


/*
 * target instructions built from emit_plain do     kill the condition codes
 */
static void
icg_emit_clobber(rtx guts)
{
  rtx clobber  = gen_rtx_CLOBBER (VOIDmode, gen_rtx_REG(CCmode, FLAGS_REG));
  rtx parallel = gen_rtx_PARALLEL(VOIDmode, gen_rtvec(2, guts, clobber));
  icg_emit_plain(parallel);
}

/*
 * This is the preferred entry point.  Pass in file name and line number
 * so that we can emit more accurate error messages if need be
 */
static void
icg_emit_clobber_tagged(rtx guts, const char *fname, int lineno)
{
  rtx clobber  = gen_rtx_CLOBBER (VOIDmode, gen_rtx_REG(CCmode, FLAGS_REG));
  rtx parallel = gen_rtx_PARALLEL(VOIDmode, gen_rtvec(2, guts, clobber));
  icg_emit_plain_tagged(parallel, fname, lineno);
}


/* =============================================================== */
#include "icg-ruleinfo.h"
unsigned icg_rulecount[PLUG_NRULES];
void icg_print_rulecount(FILE *dump_file, int dump_code)
{
  int rn;
  if (dump_file == 0) {
    dump_file = stdout;
  }
  {
    const char *msg;
    if (dump_code < 0) {
      msg = "All rules.";
    } else if (dump_code == 0) {
      msg = "Rules that were NOT used";
    } else {
      msg = "Rules that were used";
    }
    fprintf(dump_file, "# %s\n", msg);
  }
  for (rn = 0; rn < PLUG_NRULES; rn++) {
    if (0
    || (dump_code <  0)
    || (dump_code == 0 && icg_rulecount[rn] == 0)
    || (dump_code  > 0 && icg_rulecount[rn]  > 0)
    ) {
      if (icg_burm_string[rn]) {
	fprintf(dump_file, "%5d %4d %s\n",
	  rn,
	  icg_rulecount[rn],
	  icg_burm_string[rn]);
      }
    }
  }
}

void icg_read_rulecount(FILE *read_file)
{
  char buf[BUFSIZ];
  int rn = 0;
  int count = 0;
  while (fgets(buf, sizeof(buf), read_file) == buf) {
    if (buf[0] == '#') {
      continue;
    }
    if (sscanf(buf, "%d %d", &rn, &count) == 2) {
      icg_rulecount[rn] = count;
    }
  }
}
void icg_dump_rulecount(void)
{
  icg_print_rulecount(stdout, 1);
}


static const char *icg_rulecount_fname = "icg_rulecount.dump";
static
void end_reduce_emit_rtl(void)
{
  FILE *icg_rulecount_file = fopen(icg_rulecount_fname, "w");
  if (icg_rulecount_file) {
    if (1) {
      icg_print_rulecount(icg_rulecount_file, -1);
    } else {
      /*const size_t nitems =*/ fwrite(icg_rulecount,
	sizeof(icg_rulecount[0]), PLUG_NRULES, icg_rulecount_file);
      fclose(icg_rulecount_file); icg_rulecount_file = 0;
    }
  }
}

static
void start_reduce_emit_rtl(void)
{
  FILE *icg_rulecount_file = fopen(icg_rulecount_fname, "r");
  if (icg_rulecount_file) {
    if (1) {
      icg_read_rulecount(icg_rulecount_file);
    } else {
      struct stat sbuf;
      if (stat(icg_rulecount_fname, &sbuf) < 0
      || (sbuf.st_size != sizeof(icg_rulecount))) {
	/*VOID*/
      } else {
	/*const size_t nitems =*/ fread(icg_rulecount,
	  sizeof(icg_rulecount[0]), PLUG_NRULES, icg_rulecount_file);
	fclose(icg_rulecount_file); icg_rulecount_file = 0;
      }
    }
  }
  atexit(end_reduce_emit_rtl);
}

static
void reduce_emit_rtl(icg_node *p, NT goalNT)
{
  icg_node *kid[MAX_KIDS];

  const RuleNumber rule = icg_burm_rule(p->state_label, goalNT);
  const NT *nts = icg_burm_nts[rule];
  p->goalNT = goalNT;

  {
    /*
     * profile rule number usage
     */
    static int did_reduce_emit_rtl = 0;
    if (!did_reduce_emit_rtl) {
      start_reduce_emit_rtl();
    }
    did_reduce_emit_rtl += 1;
    icg_rulecount[rule] += 1;
  }

  icg_burm_kids(p, rule, kid);
  /*
   * The visit permutation is only valid for non-chain rules with >= 2 nonterminals
   */
  if (nts[0] && nts[1] && p->perm_kids >= 2) {
    int i;
    for (i = 0; i < p->perm_kids; i++) {
      const int k = p->perm[i];
      reduce_emit_rtl(kid[k], nts[k]);
    }
  }
  else {
    int i;
    for (i = 0; nts[i]; i++) {
      reduce_emit_rtl(kid[i], nts[i]);
    }
  }

  switch (rule) {
#include "icg-emit.cases"
  }
}

void icg_emit(void)
{
  basic_block bb;
  (void)gen_rtx_LT_swap;
  (void)gen_rtx_LE_swap;
  (void)gen_rtx_imm;
  (void)gen_rtx_ADD_CARRY;

  if (dump_file) {
    fprintf(dump_file, "===== start Emit ===== {\n");
  }

  FOR_EACH_BB(bb) {
    rtx insn, curr;
    FOR_BB_INSNS_SAFE(bb, insn, curr) {
      const unsigned id = INSN_UID(insn);

      icg_node *p = icg_insn2tree[id];
      if (p) {
	rtx current_insn;
	icg_insn2goalNT[id] = p->goalNT;
	if (dump_file) {
	  fprintf (dump_file, "Emit: <tree:%p>", (void *)p);
	  if (0) {
	    /* this is not too useful, as it is only for the root */
	    icg_trace_rule (dump_file, p, 0, p->goalNT);
	  }
	  print_inline_rtx (dump_file, insn, 0);
	  fprintf (dump_file, "\n");
	}

	start_sequence();
	reduce_emit_rtl(p, burm_goal_NT);
	current_insn = get_insns();
	end_sequence();

	if (current_insn) {
	  emit_insn_before(current_insn, insn);
	  delete_insn(insn);
	}
      }
    }
  }
  free_dominance_info (CDI_DOMINATORS);
  free_dominance_info (CDI_POST_DOMINATORS);

  if (dump_file) {
    fprintf(dump_file, "=====   end Emit ===== }\n");
  }

}
