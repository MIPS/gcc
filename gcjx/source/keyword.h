/* ANSI-C code produced by gperf version 3.0.1 */
/* Command-line: gperf -L ANSI-C -C -F ', TOKEN_INVALID' -t -o -N find_keyword -k'1,2,$' source/keyword.gperf  */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 1 "source/keyword.gperf"

/* Copyright (C) 2004 Free Software Foundation, Inc.
  
   This file is part of GCC.
  
   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.
  
   GCC is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
  
   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING.  If not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.
*/
#line 22 "source/keyword.gperf"
struct keyword { const char *const name; enum token_value value; };
const struct keyword *find_keyword (const char *, unsigned int);

#define TOTAL_KEYWORDS 53
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 12
#define MIN_HASH_VALUE 9
#define MAX_HASH_VALUE 84
/* maximum key range = 76, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register unsigned int len)
{
  static const unsigned char asso_values[] =
    {
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 10, 55,  5,
      20, 15,  5, 50,  0, 15, 85,  0, 30, 50,
       0,  0, 25, 85,  5,  0,  5, 15, 20, 40,
      20,  5, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85
    };
  return len + asso_values[(unsigned char)str[1]] + asso_values[(unsigned char)str[0]] + asso_values[(unsigned char)str[len - 1]];
}

#ifdef __GNUC__
__inline
#endif
const struct keyword *
find_keyword (register const char *str, register unsigned int len)
{
  static const struct keyword wordlist[] =
    {
      {"", TOKEN_INVALID}, {"", TOKEN_INVALID},
      {"", TOKEN_INVALID}, {"", TOKEN_INVALID},
      {"", TOKEN_INVALID}, {"", TOKEN_INVALID},
      {"", TOKEN_INVALID}, {"", TOKEN_INVALID},
      {"", TOKEN_INVALID},
#line 73 "source/keyword.gperf"
      {"this", TOKEN_THIS},
#line 67 "source/keyword.gperf"
      {"short", TOKEN_SHORT},
#line 75 "source/keyword.gperf"
      {"throws", TOKEN_THROWS},
      {"", TOKEN_INVALID},
#line 48 "source/keyword.gperf"
      {"for", TOKEN_FOR},
#line 32 "source/keyword.gperf"
      {"char", TOKEN_CHAR},
#line 35 "source/keyword.gperf"
      {"const", TOKEN_CONST},
#line 68 "source/keyword.gperf"
      {"static", TOKEN_STATIC},
      {"", TOKEN_INVALID},
#line 79 "source/keyword.gperf"
      {"try", TOKEN_TRY},
      {"", TOKEN_INVALID},
#line 31 "source/keyword.gperf"
      {"catch", TOKEN_CATCH},
#line 26 "source/keyword.gperf"
      {"assert", TOKEN_ASSERT},
#line 38 "source/keyword.gperf"
      {"do", TOKEN_DO},
#line 55 "source/keyword.gperf"
      {"int", TOKEN_INT},
#line 76 "source/keyword.gperf"
      {"transient", TOKEN_TRANSIENT},
#line 70 "source/keyword.gperf"
      {"super", TOKEN_SUPER},
#line 66 "source/keyword.gperf"
      {"return", TOKEN_RETURN},
#line 51 "source/keyword.gperf"
      {"if", TOKEN_IF},
#line 36 "source/keyword.gperf"
      {"continue", TOKEN_CONTINUE},
#line 78 "source/keyword.gperf"
      {"true", TOKEN_TRUE},
#line 54 "source/keyword.gperf"
      {"instanceof", TOKEN_INSTANCEOF},
#line 58 "source/keyword.gperf"
      {"native", TOKEN_NATIVE},
#line 46 "source/keyword.gperf"
      {"finally", TOKEN_FINALLY},
      {"", TOKEN_INVALID},
#line 30 "source/keyword.gperf"
      {"case", TOKEN_CASE},
#line 44 "source/keyword.gperf"
      {"false", TOKEN_FALSE},
      {"", TOKEN_INVALID},
#line 72 "source/keyword.gperf"
      {"synchronized", TOKEN_SYNCHRONIZED},
#line 69 "source/keyword.gperf"
      {"strictfp", TOKEN_STRICT},
#line 56 "source/keyword.gperf"
      {"interface", TOKEN_INTERFACE},
#line 33 "source/keyword.gperf"
      {"class", TOKEN_CLASS},
#line 39 "source/keyword.gperf"
      {"double", TOKEN_DOUBLE},
#line 42 "source/keyword.gperf"
      {"extends", TOKEN_EXTENDS},
#line 81 "source/keyword.gperf"
      {"volatile", TOKEN_VOLATILE},
#line 80 "source/keyword.gperf"
      {"void", TOKEN_VOID},
#line 47 "source/keyword.gperf"
      {"float", TOKEN_FLOAT},
#line 71 "source/keyword.gperf"
      {"switch", TOKEN_SWITCH},
#line 37 "source/keyword.gperf"
      {"default", TOKEN_DEFAULT},
      {"", TOKEN_INVALID},
#line 61 "source/keyword.gperf"
      {"null", TOKEN_NULL},
#line 74 "source/keyword.gperf"
      {"throw", TOKEN_THROW},
#line 65 "source/keyword.gperf"
      {"public", TOKEN_PUBLIC},
#line 63 "source/keyword.gperf"
      {"private", TOKEN_PRIVATE},
      {"", TOKEN_INVALID},
#line 50 "source/keyword.gperf"
      {"goto", TOKEN_GOTO},
#line 45 "source/keyword.gperf"
      {"final", TOKEN_FINAL},
      {"", TOKEN_INVALID},
#line 62 "source/keyword.gperf"
      {"package", TOKEN_PACKAGE},
#line 59 "source/keyword.gperf"
      {"new", TOKEN_NEW},
#line 64 "source/keyword.gperf"
      {"protected", TOKEN_PROTECTED},
#line 82 "source/keyword.gperf"
      {"while", TOKEN_WHILE},
      {"", TOKEN_INVALID},
#line 27 "source/keyword.gperf"
      {"boolean", TOKEN_BOOLEAN},
      {"", TOKEN_INVALID},
#line 40 "source/keyword.gperf"
      {"else", TOKEN_ELSE},
#line 28 "source/keyword.gperf"
      {"break", TOKEN_BREAK},
      {"", TOKEN_INVALID}, {"", TOKEN_INVALID},
      {"", TOKEN_INVALID},
#line 41 "source/keyword.gperf"
      {"enum", TOKEN_ENUM},
      {"", TOKEN_INVALID}, {"", TOKEN_INVALID},
      {"", TOKEN_INVALID}, {"", TOKEN_INVALID},
      {"", TOKEN_INVALID},
#line 52 "source/keyword.gperf"
      {"implements", TOKEN_IMPLEMENTS},
#line 53 "source/keyword.gperf"
      {"import", TOKEN_IMPORT},
      {"", TOKEN_INVALID},
#line 25 "source/keyword.gperf"
      {"abstract", TOKEN_ABSTRACT},
#line 29 "source/keyword.gperf"
      {"byte", TOKEN_BYTE},
      {"", TOKEN_INVALID}, {"", TOKEN_INVALID},
      {"", TOKEN_INVALID}, {"", TOKEN_INVALID},
#line 57 "source/keyword.gperf"
      {"long", TOKEN_LONG}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
