/* Subroutines for the C front end on the POWER and PowerPC architectures.
   Copyright (C) 2002, 2003, 2004
   Free Software Foundation, Inc.

   Contributed by Zack Weinberg <zack@codesourcery.com>

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING.  If not, write to the
   Free Software Foundation, 59 Temple Place - Suite 330, Boston,
   MA 02111-1307, USA.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "cpplib.h"
#include "tree.h"
#include "c-pragma.h"
#include "errors.h"
#include "tm_p.h"
/* APPLE LOCAL begin AltiVec */
#include "c-common.h"
#include "cpplib.h"
#include "cpphash.h"
#include "target.h"
/* APPLE LOCAL end AltiVec */

/* Handle the machine specific pragma longcall.  Its syntax is

   # pragma longcall ( TOGGLE )

   where TOGGLE is either 0 or 1.

   rs6000_default_long_calls is set to the value of TOGGLE, changing
   whether or not new function declarations receive a longcall
   attribute by default.  */

#define SYNTAX_ERROR(msgid) do {			\
  warning (msgid);					\
  warning ("ignoring malformed #pragma longcall");	\
  return;						\
} while (0)

void
rs6000_pragma_longcall (cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  tree x, n;

  /* If we get here, generic code has already scanned the directive
     leader and the word "longcall".  */

  if (c_lex (&x) != CPP_OPEN_PAREN)
    SYNTAX_ERROR ("missing open paren");
  if (c_lex (&n) != CPP_NUMBER)
    SYNTAX_ERROR ("missing number");
  if (c_lex (&x) != CPP_CLOSE_PAREN)
    SYNTAX_ERROR ("missing close paren");

  if (n != integer_zero_node && n != integer_one_node)
    SYNTAX_ERROR ("number must be 0 or 1");

  if (c_lex (&x) != CPP_EOF)
    warning ("junk at end of #pragma longcall");

  rs6000_default_long_calls = (n == integer_one_node);
}

/* Handle defining many CPP flags based on TARGET_xxx.  As a general
   policy, rather than trying to guess what flags a user might want a
   #define for, it's better to define a flag for everything.  */

#define builtin_define(TXT) cpp_define (pfile, TXT)
#define builtin_assert(TXT) cpp_assert (pfile, TXT)

/* APPLE LOCAL begin AltiVec */
/* Keep the AltiVec keywords handy for fast comparisons.  */
static GTY(()) cpp_hashnode *__vector_keyword;
static GTY(()) cpp_hashnode *vector_keyword;
static GTY(()) cpp_hashnode *__pixel_keyword;
static GTY(()) cpp_hashnode *pixel_keyword;
static GTY(()) cpp_hashnode *__bool_keyword;
static GTY(()) cpp_hashnode *bool_keyword;
static GTY(()) cpp_hashnode *_Bool_keyword;

/* Called to decide whether a conditional macro should be expanded.
   Since we have exactly one such macro (i.e, 'vector'), we do not
   need to examine the 'tok' parameter.  */

bool
rs6000_expand_macro_p (cpp_reader *pfile,
                      const cpp_token *tok ATTRIBUTE_UNUSED)
{
  bool expand_this = 0;
  cpp_token *peek = (cpp_token *) _cpp_peek_token (pfile, 0);

  if (peek->type == CPP_NAME)
    {
      cpp_hashnode *ident = peek->val.node;

      if (ident == pixel_keyword)
       {
         expand_this = 1;
         peek->val.node = __pixel_keyword;
       }
      else if (ident == bool_keyword || ident == _Bool_keyword)
       {
         expand_this = 1;
         peek->val.node = __bool_keyword;
       }
      else
       {
         enum rid rid_code = (enum rid)(ident->rid_code);

         if (rid_code == RID_UNSIGNED || rid_code == RID_LONG
             || rid_code == RID_SHORT || rid_code == RID_SIGNED
             || rid_code == RID_INT || rid_code == RID_CHAR
             || rid_code == RID_FLOAT)
           {
             expand_this = 1;
             /* If the next keyword is bool or pixel, it
                will need to be expanded as well.  */
             peek = (cpp_token *) _cpp_peek_token (pfile, 1);

             if (peek->type == CPP_NAME)
               {
                 ident = peek->val.node;

                 if (ident == pixel_keyword)
                   peek->val.node = __pixel_keyword;
                 else if (ident == bool_keyword || ident == _Bool_keyword)
                   peek->val.node = __bool_keyword;
               }
           }
       }
    }

  return expand_this;
}
/* APPLE LOCAL end AltiVec */

void
rs6000_cpu_cpp_builtins (cpp_reader *pfile)
{
  if (TARGET_POWER2)
    builtin_define ("_ARCH_PWR2");
  else if (TARGET_POWER)
    builtin_define ("_ARCH_PWR");
  if (TARGET_POWERPC)
    builtin_define ("_ARCH_PPC");
  if (TARGET_POWERPC64)
    builtin_define ("_ARCH_PPC64");
  if (! TARGET_POWER && ! TARGET_POWER2 && ! TARGET_POWERPC)
    builtin_define ("_ARCH_COM");
  if (TARGET_ALTIVEC)
    {
      /* APPLE LOCAL AltiVec */
      struct cpp_callbacks *cb = cpp_get_callbacks (pfile);

      builtin_define ("__ALTIVEC__");
      builtin_define ("__VEC__=10206");

      /* APPLE LOCAL begin AltiVec */
      /* Define the AltiVec syntactic elements.  */
      builtin_define ("__vector=__attribute__((altivec(vector__)))");
      __vector_keyword = cpp_lookup (pfile, DSC ("__vector"));
      builtin_define ("vector=__attribute__((altivec(vector__)))");
      vector_keyword = cpp_lookup (pfile, DSC ("vector"));
      vector_keyword->flags |= NODE_CONDITIONAL;

      builtin_define ("__pixel=__attribute__((altivec(pixel__))) unsigned short");
      __pixel_keyword = cpp_lookup (pfile, DSC ("__pixel"));
      pixel_keyword = cpp_lookup (pfile, DSC ("pixel"));

      builtin_define ("__bool=__attribute__((altivec(bool__))) unsigned");
      __bool_keyword = cpp_lookup (pfile, DSC ("__bool"));
      _Bool_keyword = cpp_lookup (pfile, DSC ("_Bool"));
      bool_keyword = cpp_lookup (pfile, DSC ("bool"));

      /* Enable context-sensitive macros.  */
      cb->expand_macro_p = rs6000_expand_macro_p;
      /* Enable '(vector signed int)(a, b, c, d)' vector literal notation.  */
      targetm.cast_expr_as_vector_init = true;

      /* Indicate that the compiler supports Apple AltiVec syntax, including context-
	 sensitive keywords.  */
      builtin_define ("__APPLE_ALTIVEC__");
      /* APPLE LOCAL end AltiVec */
    }
  if (TARGET_SPE)
    builtin_define ("__SPE__");
  if (TARGET_SOFT_FLOAT)
    builtin_define ("_SOFT_FLOAT");
  /* Used by lwarx/stwcx. errata work-around.  */
  if (rs6000_cpu == PROCESSOR_PPC405)
    builtin_define ("__PPC405__");

  /* May be overridden by target configuration.  */
  RS6000_CPU_CPP_ENDIAN_BUILTINS();

  if (TARGET_LONG_DOUBLE_128)
    builtin_define ("__LONG_DOUBLE_128__");

  switch (rs6000_current_abi)
    {
    case ABI_V4:
      builtin_define ("_CALL_SYSV");
      break;
    case ABI_AIX:
      builtin_define ("_CALL_AIXDESC");
      builtin_define ("_CALL_AIX");
      break;
    case ABI_DARWIN:
      builtin_define ("_CALL_DARWIN");
      break;
    default:
      break;
    }
}

