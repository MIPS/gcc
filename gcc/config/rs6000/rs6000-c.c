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

/* Called to decide whether a conditional macro should be expanded.  */

bool
rs6000_expand_macro_p (cpp_reader *pfile, const cpp_token *tok)
{
  static bool expand_bool_pixel = 0;  /* Preserved across calls.  */
  bool expand_this = 0;
  const cpp_hashnode *ident = tok->val.node;

  if (ident == vector_keyword)
    {
      tok = _cpp_peek_token (pfile, 0);

      if (tok->type == CPP_NAME)
	{
	  ident = tok->val.node;
	  if (ident == pixel_keyword || ident == __pixel_keyword
	      || ident == bool_keyword || ident == __bool_keyword
	      || ident == _Bool_keyword)
	    expand_this = expand_bool_pixel = 1;
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
		  tok = _cpp_peek_token (pfile, 1);

		  if (tok->type == CPP_NAME)
		    {
		      ident = tok->val.node;
		      if (ident == pixel_keyword || ident == __pixel_keyword
			  || ident == bool_keyword || ident == __bool_keyword
			  || ident == _Bool_keyword)
			expand_bool_pixel = 1;
		    }
		}
	    }
	}
    }
  else if (ident == pixel_keyword || ident == bool_keyword
           || ident == _Bool_keyword)
    {
      if (expand_bool_pixel)
	{
	  expand_this = 1;
	  expand_bool_pixel = 0;
	}
    }

  return expand_this;
}

static void
cb_define_conditional_macro (cpp_reader *pfile ATTRIBUTE_UNUSED,
			     unsigned int n ATTRIBUTE_UNUSED,
			     cpp_hashnode *node) {
  const unsigned char *name = node->ident.str;
  bool underscore = (name[1] == '_');
  char kwd = (underscore ? name[2] : name[0]);
  cpp_hashnode **kwd_node = 0;

  if (!underscore)			/* macros without two leading underscores */
    node->flags |= NODE_CONDITIONAL;	/* shall be conditional */

  switch (kwd)
    {
      case 'v': kwd_node = (underscore ? &__vector_keyword : &vector_keyword); break;
      case 'p': kwd_node = (underscore ? &__pixel_keyword : &pixel_keyword); break;
      case 'b': kwd_node = (underscore ? &__bool_keyword : &bool_keyword); break;
      case '_': kwd_node = &_Bool_keyword; break;
      default: abort ();
    }
  *kwd_node = node;
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
      /* APPLE LOCAL begin AltiVec */
      struct cpp_callbacks *cb = cpp_get_callbacks (pfile);
      void (*old_cb_define) (cpp_reader *, unsigned int, cpp_hashnode *)
	= cb->define;
      /* APPLE LOCAL end AltiVec */

      builtin_define ("__ALTIVEC__");
      builtin_define ("__VEC__=10206");

      /* Define the AltiVec syntactic elements.  */

      /* APPLE LOCAL AltiVec */
      cb->define = cb_define_conditional_macro;

      builtin_define ("__vector=__attribute__((altivec(vector__)))");
      builtin_define ("__pixel=__attribute__((altivec(pixel__))) unsigned short");
      builtin_define ("__bool=__attribute__((altivec(bool__))) unsigned");

      /* APPLE LOCAL begin AltiVec */
      /* Keywords without two leading underscores are context-sensitive, and hence
	 implemented as conditional macros, controlled by the rs6000_expand_macro_p()
	 predicate above.  */
      builtin_define ("vector=__attribute__((altivec(vector__)))");
      builtin_define ("pixel=__attribute__((altivec(pixel__))) unsigned short");
      builtin_define ("bool=__attribute__((altivec(bool__))) unsigned");
      builtin_define ("_Bool=__attribute__((altivec(bool__))) unsigned");
      cb->define = old_cb_define;

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

