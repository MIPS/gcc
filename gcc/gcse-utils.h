/* Global common subexpression elimination/Partial redundancy elimination
   and global constant/copy propagation for GNU compiler.
   Copyright (C) 1997, 1998, 1999, 2000, 2001, 2002
   Free Software Foundation, Inc.

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
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#ifndef GCC_GCSE_UTILS_H
#define GCC_GCSE_UTILS_H

/* Array of implicit set patterns indexed by basic block index.  */
extern rtx *implicit_sets;

/* Memory allocation.  */
extern char *gmalloc			PARAMS ((unsigned int));
extern char *grealloc			PARAMS ((char *, unsigned int));
extern char *gcse_alloc			PARAMS ((unsigned long));

/* Hash tables.  */
extern void compute_hash_table		PARAMS ((struct hash_table *));
extern void alloc_hash_table		PARAMS ((int, struct hash_table *, int));
extern void free_hash_table		PARAMS ((struct hash_table *));
extern void dump_hash_table		PARAMS ((FILE *, const char *,
					  struct hash_table *));
extern struct expr *lookup_expr		PARAMS ((rtx, struct hash_table *));
extern struct expr *lookup_set		PARAMS ((unsigned int, rtx,
					  struct hash_table *));
extern struct expr *next_set		PARAMS ((unsigned int, struct expr *));

/* For hash table generation, shared with other parts.  */
extern int want_to_gcse_p		PARAMS ((rtx));
extern void record_last_mem_set_info	PARAMS ((rtx));

/* Local properties computation.  */
extern void compute_local_properties	PARAMS ((sbitmap *, sbitmap *, sbitmap *,
					  struct hash_table *));

/* Operand checking.  */
extern void reset_opr_set_tables	PARAMS ((void));
extern int oprs_not_set_p		PARAMS ((rtx, rtx));
extern void mark_oprs_set		PARAMS ((rtx));

/* Modify mem tables.  */
extern void clear_modify_mem_tables	PARAMS ((void));
extern void free_modify_mem_tables	PARAMS ((void));

/* Ldst handling.  */
extern struct ls_expr * ldst_entry 	PARAMS ((rtx));
extern void free_ldst_mems		PARAMS ((void));
extern inline struct ls_expr * first_ls_expr PARAMS ((void));
extern inline struct ls_expr * next_ls_expr PARAMS ((struct ls_expr *));
extern void free_ldst_entry 		PARAMS ((struct ls_expr *));
extern void print_ldst_list 		PARAMS ((FILE *));
extern struct ls_expr * find_rtx_in_ldst PARAMS ((rtx));
extern int enumerate_ldsts		PARAMS ((void));

/* Register set information.  */
extern void alloc_reg_set_mem		PARAMS ((int));
extern void free_reg_set_mem		PARAMS ((void));
extern void record_one_set		PARAMS ((int, rtx));
extern void record_set_info		PARAMS ((rtx, rtx, void *));

/* Insn manipulation.  */
extern rtx gcse_emit_move_after		PARAMS ((rtx, rtx, rtx));
extern rtx process_insert_insn		PARAMS ((struct expr *));
extern void insert_insn_end_bb		PARAMS ((struct expr *, basic_block, int,
					  sbitmap *, sbitmap *));
extern void insert_insn_start_bb	PARAMS ((rtx, basic_block));

/* Miscelaneous.  */
extern int expr_equiv_p			PARAMS ((rtx, rtx));
extern int expr_killed_p		PARAMS ((rtx, basic_block));

extern void compute_sets		PARAMS ((rtx));
#endif
