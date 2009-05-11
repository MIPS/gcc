/*

   Copyright (C) 2007-2008 Free Software Foundation, Inc.
   Contributed by STMicroelectronics

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
Software Foundation, 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.  */

#ifndef __INTTYPES_H__
#define __INTTYPES_H__

#include <__cdefs.h>

#include <stddef.h>
#include <stdint.h>

__BEGIN_EXTERN_C

#if !defined __cplusplus || defined __STDC_FORMAT_MACROS

#define PRId8		"hhd"
#define PRId16		"hd"
#define PRId32		"d"
#define PRId40		"ld"
#define PRId64		"lld"

#define PRIdLEAST8	"hhd"
#define PRIdLEAST16	"hd"
#define PRIdLEAST32	"d"
#define PRIdLEAST40	"ld"
#define PRIdLEAST64	"lld"

#define PRIdFAST8	"hhd"
#define PRIdFAST16	"hd"
#define PRIdFAST32	"d"
#define PRIdFAST40	"ld"
#define PRIdFAST64	"lld"

#define PRIdMAX		"lld"
#define PRIdPTR		"p"

#define PRIi8		"hhi"
#define PRIi16		"hi"
#define PRIi32		"i"
#define PRIi40		"li"
#define PRIi64		"lli"

#define PRIiLEAST8	"hhi"
#define PRIiLEAST16	"hi"
#define PRIiLEAST32	"i"
#define PRIiLEAST40	"li"
#define PRIiLEAST64	"lli"

#define PRIiFAST8	"hhi"
#define PRIiFAST16	"hi"
#define PRIiFAST32	"i"
#define PRIiFAST40	"li"
#define PRIiFAST64	"lli"

#define PRIiMAX		"lli"
#define PRIiPTR		"p"

#define PRIo8		"hho"
#define PRIo16		"ho"
#define PRIo32		"o"
#define PRIo40		"lo"
#define PRIo64		"llo"

#define PRIoLEAST8	"hho"
#define PRIoLEAST16	"ho"
#define PRIoLEAST32	"o"
#define PRIoLEAST40	"lo"
#define PRIoLEAST64	"llo"

#define PRIoFAST8	"hho"
#define PRIoFAST16	"ho"
#define PRIoFAST32	"o"
#define PRIoFAST40	"lo"
#define PRIoFAST64	"llo"

#define PRIoMAX		"llo"
#define PRIoPTR		"p"

#define PRIu8		"hhu"
#define PRIu16		"hu"
#define PRIu32		"u"
#define PRIu40		"lu"
#define PRIu64		"llu"

#define PRIuLEAST8	"hhu"
#define PRIuLEAST16	"hu"
#define PRIuLEAST32	"u"
#define PRIuLEAST40	"lu"
#define PRIuLEAST64	"llu"

#define PRIuFAST8	"hhu"
#define PRIuFAST16	"hu"
#define PRIuFAST32	"u"
#define PRIuFAST40	"lu"
#define PRIuFAST64	"llu"

#define PRIuMAX		"llu"
#define PRIuPTR		"p"

#define PRIx8		"hhx"
#define PRIx16		"hx"
#define PRIx32		"x"
#define PRIx40		"lx"
#define PRIx64		"llx"

#define PRIxLEAST8	"hhx"
#define PRIxLEAST16	"hx"
#define PRIxLEAST32	"x"
#define PRIxLEAST40	"lx"
#define PRIxLEAST64	"llx"

#define PRIxFAST8	"hhx"
#define PRIxFAST16	"hx"
#define PRIxFAST32	"x"
#define PRIxFAST40	"lx"
#define PRIxFAST64	"llx"

#define PRIxMAX		"llx"
#define PRIxPTR		"p"

#define PRIX8		"hhX"
#define PRIX16		"hX"
#define PRIX32		"X"
#define PRIX40		"lX"
#define PRIX64		"llX"

#define PRIXLEAST8	"hhX"
#define PRIXLEAST16	"hX"
#define PRIXLEAST32	"X"
#define PRIXLEAST40	"lX"
#define PRIXLEAST64	"llX"

#define PRIXFAST8	"hhX"
#define PRIXFAST16	"hX"
#define PRIXFAST32	"X"
#define PRIXFAST40	"lX"
#define PRIXFAST64	"llX"

#define PRIXMAX		"llX"
#define PRIXPTR		"p"

#define SCNd8		"hhd"
#define SCNd16		"hd"
#define SCNd32		"d"
#define SCNd40		"ld"
#define SCNd64		"lld"

#define SCNdLEAST8	"hhd"
#define SCNdLEAST16	"hd"
#define SCNdLEAST32	"d"
#define SCNdLEAST40	"ld"
#define SCNdLEAST64	"lld"

#define SCNdFAST8	"hhd"
#define SCNdFAST16	"hd"
#define SCNdFAST32	"d"
#define SCNdFAST40	"ld"
#define SCNdFAST64	"lld"

#define SCNdMAX		"lld"
#define SCNdPTR		"p"

#define SCNi8		"hhi"
#define SCNi16		"hi"
#define SCNi32		"i"
#define SCNi40		"li"
#define SCNi64		"lli"

#define SCNiLEAST8	"hhi"
#define SCNiLEAST16	"hi"
#define SCNiLEAST32	"i"
#define SCNiLEAST40	"li"
#define SCNiLEAST64	"lli"

#define SCNiFAST8	"hhi"
#define SCNiFAST16	"hi"
#define SCNiFAST32	"i"
#define SCNiFAST40	"li"
#define SCNiFAST64	"lli"

#define SCNiMAX		"lli"
#define SCNiPTR		"p"

#define SCNo8		"hho"
#define SCNo16		"ho"
#define SCNo32		"o"
#define SCNo40		"lo"
#define SCNo64		"llo"

#define SCNoLEAST8	"hho"
#define SCNoLEAST16	"ho"
#define SCNoLEAST32	"o"
#define SCNoLEAST40	"lo"
#define SCNoLEAST64	"llo"

#define SCNoFAST8	"hho"
#define SCNoFAST16	"ho"
#define SCNoFAST32	"o"
#define SCNoFAST40	"lo"
#define SCNoFAST64	"llo"

#define SCNoMAX		"llo"
#define SCNoPTR		"p"

#define SCNu8		"hhu"
#define SCNu16		"hu"
#define SCNu32		"u"
#define SCNu40		"lu"
#define SCNu64		"llu"

#define SCNuLEAST8	"hhu"
#define SCNuLEAST16	"hu"
#define SCNuLEAST32	"u"
#define SCNuLEAST40	"lu"
#define SCNuLEAST64	"llu"

#define SCNuFAST8	"hhu"
#define SCNuFAST16	"hu"
#define SCNuFAST32	"u"
#define SCNuFAST40	"lu"
#define SCNuFAST64	"llu"

#define SCNuMAX		"llu"
#define SCNuPTR		"p"

#define SCNx8		"hhx"
#define SCNx16		"hx"
#define SCNx32		"x"
#define SCNx40		"lx"
#define SCNx64		"llx"

#define SCNxLEAST8	"hhx"
#define SCNxLEAST16	"hx"
#define SCNxLEAST32	"x"
#define SCNxLEAST40	"lx"
#define SCNxLEAST64	"llx"

#define SCNxFAST8	"hhx"
#define SCNxFAST16	"hx"
#define SCNxFAST32	"x"
#define SCNxFAST40	"lx"
#define SCNxFAST64	"llx"

#define SCNxMAX		"llx"
#define SCNxPTR		"p"

#define SCNX8		"hhX"
#define SCNX16		"hX"
#define SCNX32		"X"
#define SCNX40		"lX"
#define SCNX64		"llX"

#define SCNXLEAST8	"hhX"
#define SCNXLEAST16	"hX"
#define SCNXLEAST32	"X"
#define SCNXLEAST40	"lX"
#define SCNXLEAST64	"llX"

#define SCNXFAST8	"hhX"
#define SCNXFAST16	"hX"
#define SCNXFAST32	"X"
#define SCNXFAST40	"lX"
#define SCNXFAST64	"llX"

#define SCNXMAX		"llX"
#define SCNXPTR		"p"

#endif

typedef struct  {
  intmax_t quot;
  intmax_t rem;
} imaxdiv_t;

LIBSTD_LPROTO(imaxdiv_t, imaxdiv, intmax_t numer, intmax_t denom);
LIBSTD_LPROTO(intmax_t, imaxabs, intmax_t j);

LIBSTD_LPROTO(intmax_t, strtoimax, const char *restrict, char **restrict, int);
LIBSTD_LPROTO(uintmax_t, strtoumax, const char *restrict, char **restrict, int);

LIBSTD_LPROTO(intmax_t, wcstoimax, const wchar_t *restrict, wchar_t **restrict, int);
LIBSTD_LPROTO(uintmax_t, wcstoumax, const wchar_t *restrict, wchar_t **restrict, int);

__END_EXTERN_C

#endif /*__INTTYPES_H__*/
