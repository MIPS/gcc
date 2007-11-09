/* dwarf2out.h - Various declarations for functions found in dwarf2out.c
   Copyright (C) 1998, 1999, 2000, 2003, 2007
   Free Software Foundation, Inc.

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

extern void dwarf2out_decl (tree);
extern void dwarf2out_frame_debug (rtx, bool);

extern bool dwarf2_called_from_lto_p;

extern void debug_dwarf (void);
struct die_struct;
extern void debug_dwarf_die (struct die_struct *);
extern void dwarf2out_set_demangle_name_func (const char *(*) (const char *));
extern const char *dwarf_attr_name (unsigned);

/* The LTO representations for the bodies of functions may refer to
   the LTO representations of global functions, variables, and
   types.

   Because LTO operates on relocatable object files, using an ordinary
   label would require that the link-time optimizer perform much of
   the relocation processing normally performed by a linker.
   Therefore, the references used are pairs of the form (SECTION,
   OFFSET).

   The functions below indicate the OFFSET by using two labels.  The
   caller is responsible for emitting the offset as the difference
   between these two labels.  The labels are guaranteed to be in the
   same section.  The functions lto_{type,var,fn}_ref in the LTO front
   end are used to resolve these references in the LTO front end.  */

/* A reference to a global entity.  */
typedef struct lto_out_ref {
  /* The index of the compilation unit containing the entity.  */
  int64_t section;
  /* The label corresponding to the base of the DWARF 2 section
     containing this entity.  This string must not be freed by the
     caller.  */
  const char *base_label;
  /* The label for the entity itself.  This string must not be freed
     by the caller.  */
  const char *label;
} lto_out_ref;

/* Upon return, *REF contains a reference to TYPE, which must be a
   TYPE.  */
extern void lto_type_ref (tree type, lto_out_ref *ref);

/* Upon return, *REF contains a reference to VAR, which must be a
   VAR_DECL.  */
extern void lto_var_ref (tree var, lto_out_ref *ref);

/* Upon return, *REF contains a reference to FN, which must be a
   FUNCTION_DECL.  */
extern void lto_fn_ref (tree fn, lto_out_ref *ref);

/* Upon return, *REF contains a reference to FIELD, which must be a
   FIELD_DECL.  */
extern void lto_field_ref (tree field, lto_out_ref *ref);

/* Upon return, *REF contains a reference to TDECL, which must be a
   TYPE_DECL.  */
extern void lto_typedecl_ref (tree tdecl, lto_out_ref *ref);
