/* Handle OpenMP #pragma.
   Copyright (C) 2004 Free Software Foundation, Inc.
   Contributed by Sebastian Pop <pop@cri.ensmp.fr>

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

/* This file contains the functions that handle the parsing and
   validation of OpenMP pragmas.  */

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "tm.h"
#include "errors.h"
#include "ggc.h"
#include "tree.h"
#include "function.h"
#include "cpplib.h"
#include "c-pragma.h"
#include "c-tree.h"
#include "flags.h"
#include "toplev.h"
#include "ggc.h"
#include "c-common.h"
#include "output.h"
#include "tm_p.h"
#include "target.h"

#define NYI gomp_nyi ()

/* Inform that this is not yet implemented.  */

static void 
gomp_nyi (void)
{
  inform ("GOMP: Not implemented yet.");
}

/* Return true when fails to find an open parenthesis.  */

static bool
gomp_expect_open_parenthesis (void)
{
  tree name_t;
  enum cpp_ttype token = c_lex (&name_t);
  
  if (token != CPP_OPEN_PAREN)
    {
      error ("Expected an open parenthesis.");
      return true;
    }
  return false;
}


/* The following functions handle OpenMP clauses.  */

/* Return true when it fails.  Does the job for a private clause in a
   parallel directive.  */

static bool
c_gomp_parallel_private (void)
{
  tree name_t, name_decl;
  enum cpp_ttype token;

  if (gomp_expect_open_parenthesis ())
    return true;

  while (1)
    {
      token = c_lex (&name_t);
      switch (token)
	{
	case CPP_CLOSE_PAREN:
	  return false;

	case CPP_COMMA:
	  break;

	case CPP_NAME:
	  if (name_t == NULL_TREE 
	      || (name_decl = lookup_name (name_t)) == NULL_TREE
	      || TREE_CODE (name_decl) != VAR_DECL)
	    {
	      error ("Expected a variable.");
	      return true;
	    }
	  /* TODO: mark that variable as private...  */
	  break;

	default:
	  {
	    error ("Expected a list of variables.");
	    return true;
	  }
	}
    }
}


/* The following functions handle OpenMP directives.  */

/* Parse a "#pragma omp parallel [clause[ [, ]clause] ... ] new-line".  */

void
c_gomp_parallel_pragma (cpp_reader *pfile)
{
  tree name_t;

  while (1)
    {
      enum cpp_ttype token = c_lex (&name_t);
      
      switch (token)
	{
	case CPP_NAME:
	  {
	    if (!strcmp (IDENTIFIER_POINTER (name_t), "for"))
	      {
		c_gomp_parallel_for_pragma (pfile);
		return;
	      }

	    else if (!strcmp (IDENTIFIER_POINTER (name_t), "sections"))
	      {
		c_gomp_parallel_sections_pragma (pfile);
		return;
	      }

	    else if (!strcmp (IDENTIFIER_POINTER (name_t), "if"))
	      NYI;

	    else if (!strcmp (IDENTIFIER_POINTER (name_t), "private"))
	      {
		if (c_gomp_parallel_private ())
		  return;
	      }
	    else if (!strcmp (IDENTIFIER_POINTER (name_t), "firstprivate"))
	      NYI;

	    else if (!strcmp (IDENTIFIER_POINTER (name_t), "default"))
	      NYI;

	    else if (!strcmp (IDENTIFIER_POINTER (name_t), "shared"))
	      NYI;

	    else if (!strcmp (IDENTIFIER_POINTER (name_t), "copyin"))
	      NYI;

	    else if (!strcmp (IDENTIFIER_POINTER (name_t), "reduction"))
	      NYI;

	    else if (!strcmp (IDENTIFIER_POINTER (name_t), "num_threads"))
	      NYI;

	    else
	      error ("Unrecognized clause in pragma omp parallel.");

	    break;
	  }
	  
	case CPP_EOF:
	  /* New line.  */
	  goto done;
	  
	default:
	  error ("Unrecognized token in pragma omp parallel.");
	  return;
	}
    }

 done:;
  return;
}

/* Parse a "#pragma omp for [clause[ [, ]clause] ... ] new-line".  */

void
c_gomp_for_pragma (cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  NYI;
}

/* Parse a "#pragma omp parallel for [clause[ [, ]clause] ... ] new-line".
   This is a combined directive.  */

void
c_gomp_parallel_for_pragma (cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  NYI;
}

/* Parse a "#pragma omp parallel sections [clause[ [, ]clause] ... ] new-line".
   This is a combined directive.  */

void
c_gomp_parallel_sections_pragma (cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  NYI;
}

/* Parse a "#pragma omp sections [clause[ [, ]clause] ... ] new-line".  */

void
c_gomp_sections_pragma (cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  NYI;
}

/* Parse a "#pragma omp section [clause[ [, ]clause] ... ] new-line".  */

void
c_gomp_section_pragma (cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  NYI;
}

/* Parse a "#pragma omp single [clause[ [, ]clause] ... ] new-line".  */

void
c_gomp_single_pragma (cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  NYI;
}

/* Parse a "#pragma omp master new-line".  */

void
c_gomp_master_pragma (cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  NYI;
}

/* Parse a "#pragma omp critical [(name)] new-line".  */

void
c_gomp_critical_pragma (cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  NYI;
}

/* Parse a "#pragma omp barrier new-line".  */

void
c_gomp_barrier_pragma (cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  NYI;
}

/* Parse a "#pragma omp atomic new-line".  */

void
c_gomp_atomic_pragma (cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  NYI;
}

/* Parse a "#pragma omp flush [(variable-list)] new-line".  */

void
c_gomp_flush_pragma (cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  NYI;
}

/* Parse a "#pragma omp ordered new-line".  */

void
c_gomp_ordered_pragma (cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  NYI;
}

/* Parse a "#pragma omp threadprivate (variable-list) new-line".  */

void
c_gomp_threadprivate_pragma (cpp_reader *pfile ATTRIBUTE_UNUSED)
{
  NYI;
}

