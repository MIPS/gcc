/* lex.h -- Public #include File (module.h template V1.0)
   Copyright (C) 1995, 1999 Free Software Foundation, Inc.
   Contributed by James Craig Burley.

This file is part of GNU Fortran.

GNU Fortran is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Fortran is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Fortran; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.  */

#ifndef _H_f_lex
#define _H_f_lex

#include "where.h"

/* Describes the lexeme.  See lex.def.  */

typedef short int ffelexType;


/* Encapsulates source pointers to a contiguous chunk of text.

   For a given lexeme, zero or more of these are pointed to by
   that lexeme, as many as necessary to point to the source for
   each character.  */

typedef struct _ffelex_track_ {
  /* Number of characters in current chunk of characters.  */
  int len;

  /* Where the next chunk of characters begins.  */
  ffewherePt pt;
} *ffelexTrack;


/* Encapsulates the text forming a multi-character lexeme, including
   source pointers to each character in it.  */

typedef struct _ffelex_text_ {

  /* Length of text.  */
  int ffelex_text_len_;

  /* Where the text begins.  0 if it is all contiguous.  */
  ffewherePt ffelex_text_pt_;

  /* Tracking info on subsequent characters, if needed.  */
  ffelexTrack ffelex_text_track_;

  /* The text itself.  */
  char ffelex_text_p_[1];

} *ffelexText;


/* The lexeme encapsulation.  The center of the Fortran parsing universe.  */

typedef struct _ffelex_eme_ {

  /* For a simple lexeme, `pt' is the source pointer.  For a multi-character
     lexeme, `text' points to more information.  */
  union {
    ffewherePt pt;
    ffelexText text;
  } u;

  /* The lexeme itself.  Follows `u' since that might require stricter
     alignment.  */
  ffelexType type;

  /* Non-zero if whitespace or commentary preceded this lexeme.
     Helps the parser diagnose invalid spacing in free-form source.  */
  char spaced;

} *ffelexEme;


void ffelex_run (ffewhereFile wf, FILE *f, int free_form);
void ffelex_hash_kludge (FILE *f);
void ffelex_initialize (void);

#define ffelex_text_eme(t) ((t)->ffelex_eme_text_)
#define ffelex_type_eme(t) ((t)->type)
#define ffelex_pt_eme_simple(t) ((t)->u.pt)

#define ffelex_len_text_(t) ((t)->ffelex_text_length_)
#define ffelex_is_text_eq(t1,t2)					      \
  (((t1) == (t2)) ||							      \
   (((t1)->ffelex_text_len_ == (t2)->ffelex_text_len_)			      \
    && (strcmp ((t1)->p, (t2)->p))))

#endif
