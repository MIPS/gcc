/* Solaris 10 configuration.
   Copyright (C) 2004 Free Software Foundation, Inc.
   Contributed by CodeSourcery, LLC.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

#undef ASM_COMMENT_START
#define ASM_COMMENT_START "/"

#undef ASM_SPEC
#define ASM_SPEC "%{v:-V} %{Qy:} %{!Qn:-Qy} %{n} %{T} %{Ym,*} %{Yd,*} " \
		 "%{Wa,*:%*} %{m32:--32} %{m64:--64} -s %(asm_cpu)"

#undef NO_PROFILE_COUNTERS

#undef MCOUNT_NAME
#define MCOUNT_NAME "_mcount"

#undef WCHAR_TYPE
#define WCHAR_TYPE (TARGET_64BIT ? "int" : "long int")
#undef WCHAR_TYPE_SIZE
#define WCHAR_TYPE_SIZE 32

#define SUBTARGET_INSERT_ATTRIBUTES solaris_insert_attributes

#define REGISTER_TARGET_PRAGMAS()				\
  do {								\
    c_register_pragma (0, "align", solaris_pragma_align);	\
    c_register_pragma (0, "init", solaris_pragma_init);		\
    c_register_pragma (0, "fini", solaris_pragma_fini);		\
  } while (0)

/* This is how to declare the size of a function.  */
#undef ASM_DECLARE_FUNCTION_SIZE
#define ASM_DECLARE_FUNCTION_SIZE(FILE, FNAME, DECL)		\
  do								\
    {								\
      if (!flag_inhibit_size_directive)				\
        ASM_OUTPUT_MEASURED_SIZE (FILE, FNAME);			\
      solaris_output_init_fini (FILE, DECL);			\
    }								\
  while (0)

#define TARGET_INIT_FINI_ATTRIBUTES

#define SUBTARGET_OVERRIDE_OPTIONS				\
  do								\
    {								\
      if (flag_omit_frame_pointer == 2)				\
	flag_omit_frame_pointer = 0;				\
      if (flag_asynchronous_unwind_tables == 2)			\
	flag_asynchronous_unwind_tables = 0;			\
    }								\
  while (0)

#undef TARGET_SUBTARGET_DEFAULT
#define TARGET_SUBTARGET_DEFAULT (MASK_80387 | MASK_IEEE_FP	\
				  | MASK_FLOAT_RETURNS		\
				  | MASK_OMIT_LEAF_FRAME_POINTER)
