#ifndef CONFIG_ASM_H
#define CONFIG_ASM_H

#include "cpplib.h"

/* We use a small state machine to inform the lexer when to start
   returning tokens marking the beginning of each asm line.  */
enum cw_asm_states {
  /* Normal code.  */
  cw_asm_none,
  /* '{' of asm block seen, decls may appear.  */
  cw_asm_decls,
  /* No more decls, in asm block proper, '}' not seen yet.  */
  cw_asm_asm
};

extern enum cw_asm_states cw_asm_state;
extern int cw_asm_in_decl;
extern int inside_cw_asm_block;
extern int cw_asm_at_bol;
extern int cw_asm_in_operands;
extern const cpp_token *cw_split_next;
void cw_insert_saved_token (void);

/* Maximum number of arguments.  */
#define CW_MAX_ARG 20

#ifndef TARGET_CW_EXTRA_INFO
#define TARGET_CW_EXTRA_INFO
#endif

struct cw_md_Extra_info {
  /* Number of operands to the ASM_expr.  Note, this can be different
     from the number of operands to the instruction, in cases like:

        mov 0(foo,bar,4), $42

	where foo and bar are C expressions.  */
  int num;

  /* Constraints for all operands to the ASM_EXPR.  */
  const char *constraints[CW_MAX_ARG];

  TARGET_CW_EXTRA_INFO
};
typedef struct cw_md_Extra_info cw_md_extra_info;

void print_cw_asm_operand (char *buf, tree arg, unsigned argnum,
			   tree *uses, tree *label,
			   bool must_be_reg, bool must_not_be_reg, cw_md_extra_info *e);
#endif
