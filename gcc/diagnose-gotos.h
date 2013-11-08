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

#ifndef DIAGNOSE_GOTOS_H
#define DIAGNOSE_GOTOS_H

/*
   Diagnostic callback signature.
   GSI is a statement iterator of branch in question,
   BRANCH_CONTEXT is a gimple statement in wich context branch occures or NULL
      if outside of any region,
   LABEL_CONTEXT is a gimple statement in with context branch tragets or NULL
      if outside of any regions.
   This callback compares contexts and issues error and replace stmt 
      with NOP if needed
 */
typedef bool (*diagnose_context_callback) (gimple_stmt_iterator* gsi,
                              gimple branch_context, gimple label_context);
/*
   Recursive structure callback signature.
   CODE is a GIMPLE code and STMT is a statement in question,
   SEQ is a pointer to initialized vector.
   This callback must fill given vector with sequence pointers to substructures
   of STMT
 */
typedef void (*recursive_seq_callback) (enum gimple_code code, gimple stmt,
                                        vec<gimple_seq*>* seq);

extern void diagnose_gotos(vec<enum gimple_code>* codes,
                    recursive_seq_callback rcb, diagnose_context_callback dcb);

#endif
