/* -*- indented-text -*- */
/* Process source files and output type information.
   Copyright (C) 2002, 2003, 2004, 2005, 2007, 2008, 2009
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

%option noinput stack noyy_top_state

%{
#include "bconfig.h"
#include "system.h"

#define malloc xmalloc
#define realloc xrealloc

#include "gengtype.h"

#define YY_DECL int yylex (const char **yylval)
#define yyterminate() return EOF_TOKEN

struct fileloc lexer_line;
int lexer_toplevel_done;
int pp_nest, no_extra_nest;

static void 
update_lineno (const char *l, size_t len)
{
  while (len-- > 0)
    if (*l++ == '\n')
      lexer_line.line++;
}

%}

ID	[[:alpha:]_][[:alnum:]_]*
WS	[[:space:]]+
HWS	[ \t\r\v\f]*
IWORD	short|long|(un)?signed|char|int|HOST_WIDE_INT|HOST_WIDEST_INT|bool|size_t|BOOL_BITFIELD|CPPCHAR_SIGNED_T|ino_t|dev_t|HARD_REG_SET
ITYPE	{IWORD}({WS}{IWORD})*
EOID	[^[:alnum:]_]

%x in_struct in_comment start_pp_directive in_pp_directive pp_scanning
%option warn noyywrap nounput nodefault perf-report
%option 8bit never-interactive
%%
  /* Do this on entry to yylex():  */
  *yylval = 0;
  if (lexer_toplevel_done)
    {
      BEGIN(INITIAL);
      lexer_toplevel_done = 0;
    }


<INITIAL,in_struct,in_comment>^{HWS}"#"	{ yy_push_state (start_pp_directive); }
<pp_scanning>^{HWS}"#"			{ BEGIN (start_pp_directive); }

  /* Things we look for in skipping mode: */
<INITIAL>{
^{HWS}typedef/{EOID} {
  BEGIN(in_struct);
  return TYPEDEF;
}
^{HWS}struct/{EOID} {
  BEGIN(in_struct);
  return STRUCT;
}
^{HWS}union/{EOID} {
  BEGIN(in_struct);
  return UNION;
}
^{HWS}extern/{EOID} {
  BEGIN(in_struct);
  return EXTERN;
}
^{HWS}static/{EOID} {
  BEGIN(in_struct);
  return STATIC;
}

^{HWS}DEF_VEC_[OP]/{EOID} {
  BEGIN(in_struct);
  return DEFVEC_OP;
}
^{HWS}DEF_VEC_I/{EOID} {
  BEGIN(in_struct);
  return DEFVEC_I;
}
^{HWS}DEF_VEC_ALLOC_[IOP]/{EOID} {
  BEGIN(in_struct);
  return DEFVEC_ALLOC;
}
}

<*>\\\n				{ lexer_line.line++; }

<in_struct>{

"/*"				{ yy_push_state (in_comment); }

{WS}				{ update_lineno (yytext, yyleng); }

TARGET_SCOPE/{EOID}		/* don't care */
"const"/{EOID}			/* don't care */
"GTY"/{EOID}			{ return GTY_TOKEN; }
"VEC"/{EOID}			{ return VEC_TOKEN; }
"union"/{EOID}			{ return UNION; }
"struct"/{EOID}			{ return STRUCT; }
"enum"/{EOID}			{ return ENUM; }
"ptr_alias"/{EOID}	  	{ return PTR_ALIAS; }
"nested_ptr"/{EOID}		{ return NESTED_PTR; }
[0-9]+				{ return NUM; }
"param"[0-9]*"_is"/{EOID}		{
  *yylval = XDUPVAR (const char, yytext, yyleng, yyleng+1);
  return PARAM_IS;
}

{IWORD}({WS}{IWORD})*/{EOID}		|
"ENUM_BITFIELD"{WS}?"("{WS}?{ID}{WS}?")"	{
  size_t len;

  for (len = yyleng; ISSPACE (yytext[len-1]); len--)
    ;

  *yylval = XDUPVAR (const char, yytext, len, len+1);
  update_lineno (yytext, yyleng);
  return SCALAR;
}


{ID}/{EOID}			{
  *yylval = XDUPVAR (const char, yytext, yyleng, yyleng+1);
  return ID;
}

\"([^"\\]|\\.)*\"		{
  *yylval = XDUPVAR (const char, yytext+1, yyleng-2, yyleng-1);
  return STRING;
}
  /* This "terminal" avoids having to parse integer constant expressions.  */
"["[^\[\]]*"]"			{
  *yylval = XDUPVAR (const char, yytext+1, yyleng-2, yyleng-1);
  return ARRAY;
}
"'"("\\".|[^\\])"'"		{
  *yylval = XDUPVAR (const char, yytext+1, yyleng-2, yyleng);
  return CHAR;
}

"..."				{ return ELLIPSIS; }
[(){},*:<>;=%|-]		{ return yytext[0]; }

.				{
  error_at_line (&lexer_line, "unexpected character `%s'", yytext);
}
}

<pp_scanning>"GTY"/{EOID}	{
  gcc_assert (get_output_file_name (lexer_line.file)); }

{ID}			/* do nothing */
<INITIAL,in_struct,in_comment,pp_scanning>\n	{ lexer_line.line++; }
<INITIAL,start_pp_directive,in_pp_directive,pp_scanning>{
"/*"			{ yy_push_state (in_comment);}
"'"("\\".|[^\\])"'"	/* Skip character literal.  */
\"([^"\\]|\\.|\\\n)*\"	{ update_lineno (yytext, yyleng); /* String.  */}
"/"/[^*]		/* Do nothing for ordinary slash.  */
[^"/\n]			/* Do nothing.  */
}

<in_comment>{
[^*\n]{16}	|
[^*\n]		/* do nothing */
"*"/[^/]	/* do nothing */
"*/"		{ yy_pop_state (); } 
}

<INITIAL,start_pp_directive,in_pp_directive,pp_scanning>["/] |
<in_comment>"*"	{
  error_at_line (&lexer_line, 
		 "unterminated comment or string; unexpected EOF");
}

<start_pp_directive>{
{HWS}ifndef{HWS}EXTRA_TARGET[[:alnum:]_][^/"'\\\n]* {
  pp_nest++; BEGIN (in_pp_directive);
}
{HWS}ifndef{HWS}EXTRA_TARGET[^/"'\\\n]* {
  pp_nest++; BEGIN (in_pp_directive);
#ifdef EXTRA_TARGET
  no_extra_nest = pp_nest;
#endif
}
{HWS}if[^/"'\\\n]*	{ pp_nest++; BEGIN (in_pp_directive); }
{HWS}endif[^/"'\\\n]*	{
  if (pp_nest-- == no_extra_nest)
    no_extra_nest = 0;
  BEGIN (in_pp_directive);
}
{HWS}[^/"'\\\n]*	{ BEGIN (in_pp_directive); }
\n			{ lexer_line.line++; yy_pop_state (); }
}

<in_pp_directive>\n	{
  lexer_line.line++;
    if (no_extra_nest)
      BEGIN (pp_scanning);
    else
      yy_pop_state ();
} 

%%

void
yybegin (const char *fname)
{
  yyin = fopen (fname, "r");
  if (yyin == NULL)
    {
      perror (fname);
      exit (1);
    }
  lexer_line.file = fname;
  lexer_line.line = 1;
}

void
yyend (void)
{
  fclose (yyin);
}
