/* Contribution to the specs for the GNU Compiler Collection
   from GNU Fortran 95 compiler.
   Copyright (C) 2002 Free Software Foundation, Inc.

This file is licensed under the GPL.  */

/* This is the contribution to the `default_compilers' array in gcc.c
   for the f95 language.  */

{".f90", "@f95", 0},
{".f95", "@f95", 0},
{"@f95", "%{!E:f951 %i %<ffixed-format %(cc1_options) %{J*} %{I*}\
         %{!fsyntax-only:%(invoke_as)}}", 0}
,
