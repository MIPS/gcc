/* -*- indented-text -*- */
/* Process source files and output type information.
   Copyright (C) 2002 Free Software Foundation, Inc.

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

%{
#include "hconfig.h"
#include "system.h"
#include "gengtype.h"
#define YYERROR_VERBOSE
%}

%union {
  type_p t;
  pair_p p;
  options_p o;
  const char *s;
}

%token <t>ENT_TYPEDEF_STRUCT
%token <t>ENT_STRUCT
%token ENT_EXTERNSTATIC
%token ENT_YACCUNION
%token GTY_TOKEN "GTY"
%token UNION "union"
%token STRUCT "struct"
%token ENUM "enum"
%token VARRAY_TYPE "varray_type"
%token NUM
%token PERCENTPERCENT "%%"
%token <t>SCALAR
%token <s>ID
%token <s>STRING
%token <s>ARRAY
%token <s>PERCENT_ID
%token <s>CHAR

%type <p> struct_fields yacc_ids yacc_typematch
%type <t> type lasttype
%type <o> optionsopt options option optionseq optionseqopt

%%

start: /* empty */
       | typedef_struct start
       | externstatic start
       | yacc_union start

typedef_struct: ENT_TYPEDEF_STRUCT options 
		   { 
		     $1->u.s.opt = $2;
		     $1->u.s.line = lexer_line;
		   }
		 '{' struct_fields '}' ID
		   { 
		     $1->u.s.fields = $5;
		     do_typedef ($7, $1, &lexer_line);
		     lexer_toplevel_done = 1;
		   }
		 ';'
		| ENT_STRUCT options 
		   { 
		     $1->u.s.opt = $2;
		     $1->u.s.line = lexer_line;
		   }
		  '{' struct_fields '}'
		   { 
		     $1->u.s.fields = $5;
		     lexer_toplevel_done = 1;
		   }
		 ';'

externstatic: ENT_EXTERNSTATIC options lasttype ID semiequal
	         {
	           note_variable ($4, $3, $2, &lexer_line);
	         }
	      | ENT_EXTERNSTATIC options lasttype ID ARRAY semiequal
	         {
	           note_variable ($4, create_array ($3, $5),
	      		    $2, &lexer_line);
	         }
	      | ENT_EXTERNSTATIC options lasttype ID ARRAY ARRAY semiequal
	         {
	           note_variable ($4, create_array (create_array ($3, $6),
	      				      $5),
	      		    $2, &lexer_line);
	         }

lasttype: type
	    { 
	      lexer_toplevel_done = 1;
	      $$ = $1;
	    }

semiequal: ';'
	   | '='
	   ;

yacc_union: ENT_YACCUNION options struct_fields '}' yacc_typematch PERCENTPERCENT
	      {
	        note_yacc_type ($2, $3, $5, &lexer_line);
	      }

yacc_typematch: /* empty */
		   { $$ = NULL; }
		| yacc_typematch PERCENT_ID yacc_ids
		   { 
		     pair_p p;
		     for (p = $3; p->next != NULL; p = p->next)
		       {
		         p->name = NULL;
			 p->type = NULL;
		       }
		     p->name = NULL;
		     p->type = NULL;
		     p->next = $1;
		     $$ = $3;
		   }
		| yacc_typematch PERCENT_ID '<' ID '>' yacc_ids
		   {
		     pair_p p;
		     type_p newtype = NULL;
		     if (strcmp ($2, "type") == 0)
		       newtype = (type_p) 1;
		     for (p = $6; p->next != NULL; p = p->next)
		       {
		         p->name = $4;
		         p->type = newtype;
		       }
		     p->name = $4;
		     p->next = $1;
		     p->type = newtype;
		     $$ = $6;
		   }
		;

yacc_ids: /* empty */
	{ $$ = NULL; }
     | yacc_ids ID
        { 
	  pair_p p = xcalloc (1, sizeof (*p));
	  p->next = $1;
	  p->line = lexer_line;
	  p->opt = xmalloc (sizeof (*(p->opt)));
	  p->opt->name = "tag";
	  p->opt->next = NULL;
	  p->opt->info = $2;
	  $$ = p;
	}
     | yacc_ids CHAR
        {
	  pair_p p = xcalloc (1, sizeof (*p));
	  p->next = $1;
	  p->line = lexer_line;
	  p->opt = xmalloc (sizeof (*(p->opt)));
	  p->opt->name = "tag";
	  p->opt->next = NULL;
	  p->opt->info = xmalloc (3 + strlen ($2));
	  sprintf (p->opt->info, "'%s'", $2);
	  $$ = p;
	}

struct_fields: { $$ = NULL; }
	       | type optionsopt ID bitfieldopt ';' struct_fields
	          {
	            pair_p p = xmalloc (sizeof (*p));
		    p->type = adjust_field_type ($1, $2);
		    p->opt = $2;
		    p->name = $3;
		    p->next = $6;
		    p->line = lexer_line;
		    $$ = p;
		  }
	       | type optionsopt ID ARRAY ';' struct_fields
	          {
	            pair_p p = xmalloc (sizeof (*p));
		    p->type = create_array ($1, $4);
		    p->opt = $2;
		    p->name = $3;
		    p->next = $6;
		    p->line = lexer_line;
		    $$ = p;
		  }
	       | type optionsopt ID ARRAY ARRAY ';' struct_fields
	          {
	            pair_p p = xmalloc (sizeof (*p));
		    p->type = create_array (create_array ($1, $5), $4);
		    p->opt = $2;
		    p->name = $3;
		    p->next = $7;
		    p->line = lexer_line;
		    $$ = p;
		  }

bitfieldopt: /* empty */
	     | ':' NUM

type: SCALAR
         { $$ = $1; }
      | VARRAY_TYPE options
         {
	   if ($2 == NULL || $2->next != NULL 
	       || strcmp ($2->name, "varray_type") != 0)
	     {
	       error_at_line (&lexer_line, 
			      "expected only `varray_type' option");
	       $$ = create_varray (&string_type);
	     }
	   else
	     $$ = create_varray ((type_p) $2->info);
	 }
      | ID
         { $$ = resolve_typedef ($1, &lexer_line); }
      | type '*'
         { $$ = create_pointer ($1); }
      | STRUCT ID '{' struct_fields '}'
         {
	   type_p t = find_structure ($2, 0, &lexer_line);
	   t->u.s.fields = $4;
	   if (t->u.s.line.file)
	     {
	       error_at_line (&lexer_line, "duplicate structure definition");
	       error_at_line (&t->u.s.line, "previous definition here");
	     }
	   t->u.s.line = lexer_line;
	   $$ = t;
	 }
      | STRUCT ID
         { $$ = find_structure ($2, 0, &lexer_line); }
      | UNION ID '{' struct_fields '}'
         {
	   type_p t = find_structure ($2, 1, &lexer_line);
	   t->u.s.fields = $4;
	   if (t->u.s.line.file)
	     {
	       error_at_line (&lexer_line, "duplicate structure definition");
	       error_at_line (&t->u.s.line, "previous definition here");
	     }
	   t->u.s.line = lexer_line;
	   $$ = t;
	 }
      | UNION ID
         { $$ = find_structure ($2, 1, &lexer_line); }
      | ENUM ID
         { $$ = create_scalar_type ($2, strlen ($2)); }
      | ENUM ID '{' enum_items '}'
         { $$ = create_scalar_type ($2, strlen ($2)); }

enum_items: /* empty */
	    | ID '=' NUM ',' enum_items
	      { }
	    | ID ',' enum_items
	      { }
	    | ID enum_items
	      { }
	    ;

optionsopt: { $$ = NULL; }
	    | options { $$ = $1; }

options: GTY_TOKEN '(' '(' optionseqopt ')' ')' { $$ = $4; }

option:	VARRAY_TYPE '(' type ')'
	   {
	     options_p o = xmalloc (sizeof (*o));
	     o->name = "varray_type";
	     o->info = $3;
	     $$ = o;
	   }
	| ID '(' STRING ')'
	   { 
	     options_p o = xmalloc (sizeof (*o));
	     o->name = $1;
	     o->info = (void *)$3;
	     $$ = o;
	   }

optionseq: option
	      {
	        $1->next = NULL;
		$$ = $1;
	      }
	    | optionseq ',' option
	      {
	        $3->next = $1;
		$$ = $3;
	      }

optionseqopt: { $$ = NULL }
	      | optionseq { $$ = $1; }

%%
