/* Prototypes for the CIL dump.

   Copyright (C) 2006-2008 Free Software Foundation, Inc.

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
02110-1301, USA.

Authors:
   Andrea Ornstein
   Erven Rohou
   Gabriele Svelto

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Erven Rohou             <erven.rohou@st.com>
*/

#ifndef EMIT_CIL_H
#define EMIT_CIL_H

#include "config.h"
#include "coretypes.h"

/******************************************************************************
 * Function prototypes                                                        *
 ******************************************************************************/

void emit_cil_init (void);
void emit_cil_fini (void);
void emit_cil_decl (FILE *, tree);

void emit_vcg_init (void);
void emit_vcg_fini (void);

#endif /* EMIT_CIL_H */
