/* GCC MELT GENERATED C++ FILE warmelt-outobj+05.cc - DO NOT EDIT - see http://gcc-melt.org/ */
/* secondary MELT generated C++ file of rank #5 */
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt_f5[] = MELT_RUN_HASHMD5 /* from melt-run.h */;


/**** warmelt-outobj+05.cc declarations ****/
/**** MELT GENERATED DECLARATIONS for warmelt-outobj ** DO NOT EDIT ; see gcc-melt.org ****/

/****++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
***
    Copyright (C) 2008 - 2014  Free Software Foundation, Inc.
    Contributed by Basile Starynkevitch <basile@starynkevitch.net>

    This file is part of GCC.

    GCC is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3, or (at your option)
    any later version.

    GCC is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GCC; see the file COPYING3.  If not see
    <http://www.gnu.org/licenses/>.
***
----------------------------------------------------------------****/


/** ordinary MELT module meltbuild-sources/warmelt-outobj**/
#define MELT_HAS_INITIAL_ENVIRONMENT 1 /*usualmodule*/



class Melt_InitialClassyFrame_WARMELTmiOUTOBJ_h993045092; // forward declaration fromline 6660
typedef Melt_InitialClassyFrame_WARMELTmiOUTOBJ_h993045092 Melt_InitialFrame;
/*** 1 MELT module variables declarations ****/
MELT_EXTERN void melt_forwarding_module_data (void);
MELT_EXTERN void melt_marking_module_data (void);

MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY melt_WARMELTmiOUTOBJ_module_var_ptr_tab[16];
MELT_EXTERN bool MELT_MODULE_VISIBILITY melt_WARMELTmiOUTOBJ_module_var_flags[1];

static inline melt_ptr_t
melt_module_var_fetch (int ix)
{
    return (ix > 0  &&  ix <= 1)?melt_WARMELTmiOUTOBJ_module_var_ptr_tab[ix]:NULL;
}

static inline void
melt_module_var_put (int ix, melt_ptr_t val)
{
    if (ix > 0  &&  ix <= 1)
        {
            melt_WARMELTmiOUTOBJ_module_var_ptr_tab[ix]= val;
            melt_WARMELTmiOUTOBJ_module_var_flags[ix/16] = true;
        }
}

/* MELT module variables indexes */
enum
{
    meltmodatix_none,
    meltmodvarix_FLAGVAL_OUTPUT_DESCR_COMMENT_symb = 1,
    meltmodatix_last
}; /* end MELT module variables indexes */

/*** 5 MELT called hook declarations ***/

/*declare MELT called hook #0 HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_PREVENV, const char* meltinp1_MODULNAME);

/*declare MELT called hook #1 HOOK_MELT_MAKE_LOCATION **/
MELT_EXTERN melt_ptr_t melthook_HOOK_MELT_MAKE_LOCATION (melt_ptr_t melthookdatap, const char* meltinp0_FILENAME, long meltinp1_LINENO);

/*declare MELT called hook #2 HOOK_SYMBOL_IMPORTER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_SYMBOL_IMPORTER (melt_ptr_t melthookdatap, const char* meltinp0_SYMNAMESTR, const char* meltinp1_MODULENAMESTR, melt_ptr_t meltinp2_PARENV);

/*declare MELT called hook #3 HOOK_SYMBOL_IMPORTER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_SYMBOL_IMPORTER (melt_ptr_t melthookdatap, const char* meltinp0_SYMNAMESTR, const char* meltinp1_MODULENAMESTR, melt_ptr_t meltinp2_PARENV);

/*declare MELT called hook #4 HOOK_VALUE_EXPORTER **/
MELT_EXTERN void melthook_HOOK_VALUE_EXPORTER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_SYM, melt_ptr_t meltinp1_VAL, melt_ptr_t meltinp2_CONTENV);

/*** end of 5 MELT called hook declarations ***/

/*** no extra MELT c-headers ***/




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_1_WARMELTmiOUTOBJ_DO_OUTPUT_DESCRIPTIVE_COMMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_2_WARMELTmiOUTOBJ_OUTPUT_DESCRIPTIVE_COMMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_3_WARMELTmiOUTOBJ_GET_CODE_BUFFER_LIMIT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_4_WARMELTmiOUTOBJ_PUT_CODE_BUFFER_LIMIT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_5_WARMELTmiOUTOBJ_OUT_ENUMIX_MODVARBIND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_6_WARMELTmiOUTOBJ_LAMBDA_cl1(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_7_WARMELTmiOUTOBJ_INITIAL_CLASSY_FRAME_NAME(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_8_WARMELTmiOUTOBJ_LAMBDA_cl2(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_9_WARMELTmiOUTOBJ_OUTDECLINIT_ROOT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_30_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITSTRING(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_31_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITSTRING(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_32_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITBOXEDINTEGER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_33_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITBOXEDINTEGER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_34_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITPAIR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_35_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITPAIR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_36_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITLIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_37_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITLIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_38_WARMELTmiOUTOBJ_OUTPUCOD_ANYDISCR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_39_WARMELTmiOUTOBJ_OUTPUCOD_NULL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_40_WARMELTmiOUTOBJ_OUTPUCOD_CATCHALL_ROOT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_41_WARMELTmiOUTOBJ_OUTPUT_RAW_LOCATION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_42_WARMELTmiOUTOBJ_LINE_AND_FILE_OF_LOCATION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_43_WARMELTmiOUTOBJ_OUTPUT_LOCATION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_44_WARMELTmiOUTOBJ_OUTPUT_CURFRAME_DECLSTRUCT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_45_WARMELTmiOUTOBJ_OUTPUT_CURFRAME_DECLSTRUCT_INIT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_46_WARMELTmiOUTOBJ_OUTPUCOD_PROCROUTINE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_47_WARMELTmiOUTOBJ_OUTPUT_CURFRAME_CDAT_STRUCT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_48_WARMELTmiOUTOBJ_OUTPUT_CURFRAME_CDAT_FILL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_49_WARMELTmiOUTOBJ_OUTPUCOD_INITIALMODULEROUTINE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_113_WARMELTmiOUTOBJ_OUTPUCOD_VERBATIMSTRING(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_114_WARMELTmiOUTOBJ_OUTPUCOD_STRING(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_115_WARMELTmiOUTOBJ_OUTPUCOD_INTEGER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_116_WARMELTmiOUTOBJ_OUTPUCOD_FINALRETURN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_117_WARMELTmiOUTOBJ_SORTED_NAMED_DICT_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_118_WARMELTmiOUTOBJ_OUTPUT_EXPORTED_OFFSETS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_119_WARMELTmiOUTOBJ_NTH_SECUNDARY_FILE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_120_WARMELTmiOUTOBJ_OUTPUT_MELT_DESCRIPTOR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_121_WARMELTmiOUTOBJ_SYNTESTGEN_ANY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_122_WARMELTmiOUTOBJ_SUBSTITUTE_FORMALS_FOR_SYNTEST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_123_WARMELTmiOUTOBJ_EXPAND_TUPLE_FOR_SYNTEST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_124_WARMELTmiOUTOBJ_SYNTESTGEN_PRIMITIVE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_125_WARMELTmiOUTOBJ_SYNTESTGEN_CITERATOR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_126_WARMELTmiOUTOBJ_SYNTESTGEN_CMATCHER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_127_WARMELTmiOUTOBJ_EMIT_SYNTAX_TESTING_ROUTINE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_128_WARMELTmiOUTOBJ_GIVE_NORMALIZED_MACROEXPANDED_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_129_WARMELTmiOUTOBJ_OUTPUT_HOOKS_WITH_MODVAR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_130_WARMELTmiOUTOBJ_TRANSLATE_MACROEXPANDED_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_131_WARMELTmiOUTOBJ_LAMBDA_cl6(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_132_WARMELTmiOUTOBJ_FATAL_COMPILE_ERROR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_133_WARMELTmiOUTOBJ_COMPILE_LIST_SEXPR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_146_WARMELTmiOUTOBJ_EVAL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);


MELT_EXTERN void* melt_start_this_module (void*); /*mandatory start of module*/

MELT_EXTERN const char melt_module_is_gpl_compatible[];

/*declare opaque initial frame: */

typedef Melt_InitialClassyFrame_WARMELTmiOUTOBJ_h993045092 /*opaqueinitialclassy*/ meltinitial_frame_t;


/* define different names when debugging or not */
#if MELT_HAVE_DEBUG
MELT_EXTERN const char meltmodule_WARMELTmiOUTOBJ__melt_have_debug_enabled[];
#define melt_have_debug_string meltmodule_WARMELTmiOUTOBJ__melt_have_debug_enabled
#else /*!MELT_HAVE_DEBUG*/
MELT_EXTERN const char meltmodule_WARMELTmiOUTOBJ__melt_have_debug_disabled[];
#define melt_have_debug_string meltmodule_WARMELTmiOUTOBJ__melt_have_debug_disabled
#endif /*!MELT_HAVE_DEBUG*/



void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_0 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_1 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_2 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_3 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_4 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_5 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_6 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_7 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_8 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_9 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_10 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_11 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_12 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_13 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_14 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_15 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_16 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_17 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_18 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_19 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_20 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_21 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiOUTOBJ__initialmeltchunk_22 (meltinitial_frame_t*, char*);


/**** warmelt-outobj+05.cc implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ fromline 1752 */

    /** start of frame for meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ// fromline 1568
        : public Melt_CallFrameWithValues<32>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[16];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<32> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ), clos) {};
        MeltFrame_meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ() //the constructor fromline 1642
            : Melt_CallFrameWithValues<32> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<32> (fil,lin, sizeof(MeltFrame_meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ)) {};
        MeltFrame_meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<32> (fil,lin, sizeof(MeltFrame_meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ


    /** end of frame for meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ fromline 1697**/

    /* end of frame for routine meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ fromline 1756 */

    /* classy proc frame meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ */ MeltFrame_meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJRAWALLOCOBJ", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:4419:/ getarg");
    /*_.ORALOB__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DECLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[2].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4420:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.ORALOB__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJRAWALLOCOBJ*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:4420:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4420:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check oralob";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4420;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ORALOB__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4420:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4420:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:4421:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.ORALOB__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.ILOC__V8*/ meltfptr[6] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4422:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.ORALOB__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBRALLOBJ_CLASS");
            /*_.ICLASS__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4423:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.ORALOB__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 4, "OBRALLOBJ_CLASSNAME");
            /*_.ICLANAME__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4424:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.ORALOB__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OBRALLOBJ_LEN");
            /*_.ILEN__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4425:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.ORALOB__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBDI_DESTLIST");
            /*_.DESTLIST__V12*/ meltfptr[11] = slot;
        };
        ;
        /*_#plI__L3*/ meltfnum[1] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;
        /*^compute*/
        /*_.BOXDEPTHP1__V13*/ meltfptr[12] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[2])), (/*_#plI__L3*/ meltfnum[1])));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4428:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[3] =
                melt_is_instance_of((melt_ptr_t)(/*_.ICLASS__V9*/ meltfptr[5]), (melt_ptr_t)((/*!CLASS_OBJVALUE*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-outobj.melt:4428:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4428:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[3];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "outpucod_objrawallocobj check iclass";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4428;
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/ meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4428:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/ meltfptr[13] = /*_._IFELSE___V15*/ meltfptr[14];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4428:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/ meltfptr[14] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[13] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4429:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "rawallocobj";
            /*_.OUTPUT_LOCATION__V17*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.ILOC__V8*/ meltfptr[6]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:73:/ cond");
        /*cond*/ if ( (melt_module_var_fetch (meltmodvarix_FLAGVAL_OUTPUT_DESCR_COMMENT_symb))) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*^apply*/
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ORALOB__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V19*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V18*/ meltfptr[14] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V19*/ meltfptr[18];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V19*/ meltfptr[18] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V18*/ meltfptr[14] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4431:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*rawallocobj*/ { melt_ptr_t newobj = 0;"));
        }
        ;
        /*_#plI__L5*/ meltfnum[3] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        {
            MELT_LOCATION("warmelt-outobj.melt:4432:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L5*/ meltfnum[3]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4433:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "melt_raw_object_create(newobj,(melt_ptr_t)("));
        }
        ;
        /*_#GET_INT__L6*/ meltfnum[5] =
            (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V13*/ meltfptr[12])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4434:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#GET_INT__L6*/ meltfnum[5];
            /*_.OUTPUT_C_CODE__V20*/ meltfptr[18] = meltgc_send((melt_ptr_t)(/*_.ICLASS__V9*/ meltfptr[5]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[6])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4435:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "), ("));
        }
        ;
        /*_#GET_INT__L7*/ meltfnum[6] =
            (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V13*/ meltfptr[12])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4436:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#GET_INT__L7*/ meltfnum[6];
            /*_.OUTPUT_C_CODE__V21*/ meltfptr[20] = meltgc_send((melt_ptr_t)(/*_.ILEN__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[6])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4437:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "), \""));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4438:/ locexp");
            meltgc_add_strbuf_cstr ((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                    melt_string_str ((melt_ptr_t)(/*_.ICLANAME__V10*/ meltfptr[9])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4439:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "\");"));
        }
        ;
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit1__EACHLIST */
            for (/*_.DSTPAIR__V22*/ meltfptr[21] = melt_list_first( (melt_ptr_t)/*_.DESTLIST__V12*/ meltfptr[11]);
                                    melt_magic_discr((melt_ptr_t) /*_.DSTPAIR__V22*/ meltfptr[21]) == MELTOBMAG_PAIR;
                                    /*_.DSTPAIR__V22*/ meltfptr[21] = melt_pair_tail((melt_ptr_t) /*_.DSTPAIR__V22*/ meltfptr[21]))
                {
                    /*_.DST__V23*/ meltfptr[22] = melt_pair_head((melt_ptr_t) /*_.DSTPAIR__V22*/ meltfptr[21]);


                    /*_#plI__L8*/ meltfnum[7] =
                        ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4443:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L8*/ meltfnum[7]), 0);
                    }
                    ;
                    /*_#GET_INT__L9*/ meltfnum[8] =
                        (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V13*/ meltfptr[12])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4444:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#GET_INT__L9*/ meltfnum[8];
                        /*_.OUTPUT_C_CODE__V24*/ meltfptr[23] = meltgc_send((melt_ptr_t)(/*_.DST__V23*/ meltfptr[22]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[6])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:4445:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L10*/ meltfnum[9] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                        MELT_LOCATION("warmelt-outobj.melt:4446:/ cond");
                        /*cond*/ if (
                            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[7])),
                                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[8])))
                        ) /*then*/
                            {
                                /*^cond.then*/
                                /*^getslot*/
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[7])) /*=obj*/;
                                    melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                                    /*_.REFERENCED_VALUE__V26*/ meltfptr[25] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V26*/ meltfptr[25] =  /*reallynil*/ NULL ;;
                            }
                        ;
                        /*^compute*/
                        /*_#GET_INT__L11*/ meltfnum[10] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V26*/ meltfptr[25])));;
                        /*^compute*/
                        /*_#ltI__L12*/ meltfnum[11] =
                            ((/*_#STRBUF_USEDLENGTH__L10*/ meltfnum[9]) < (/*_#GET_INT__L11*/ meltfnum[10]));;
                        MELT_LOCATION("warmelt-outobj.melt:4445:/ cond");
                        /*cond*/ if (/*_#ltI__L12*/ meltfnum[11]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V27*/ meltfptr[26] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:4445:/ cond.else");

                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*^apply*/
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[4];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_cstring =  "check limited implbuf";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 4445;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V28*/ meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V27*/ meltfptr[26] = /*_.MELT_ASSERT_FAILURE_FUN__V28*/ meltfptr[27];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:4445:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V28*/ meltfptr[27] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V25*/ meltfptr[24] = /*_._IFELSE___V27*/ meltfptr[26];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4445:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L10*/ meltfnum[9] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V26*/ meltfptr[25] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L11*/ meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L12*/ meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V27*/ meltfptr[26] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V25*/ meltfptr[24] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4447:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " ="));
                    }
                    ;
                } /* end foreach_pair_component_in_list meltcit1__EACHLIST */
            /*_.DSTPAIR__V22*/ meltfptr[21] = NULL;
            /*_.DST__V23*/ meltfptr[22] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:4440:/ clear");
            /*clear*/ /*_.DSTPAIR__V22*/ meltfptr[21] = 0 ;
            /*^clear*/
            /*clear*/ /*_.DST__V23*/ meltfptr[22] = 0 ;
            /*^clear*/
            /*clear*/ /*_#plI__L8*/ meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L9*/ meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_.OUTPUT_C_CODE__V24*/ meltfptr[23] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V25*/ meltfptr[24] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;

        MELT_LOCATION("warmelt-outobj.melt:4421:/ clear");
        /*clear*/ /*_.ILOC__V8*/ meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ICLASS__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ICLANAME__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ILEN__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.DESTLIST__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BOXDEPTHP1__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V17*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V18*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L5*/ meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L6*/ meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V20*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L7*/ meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V21*/ meltfptr[20] = 0 ;
        /*_#plI__L13*/ meltfnum[9] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        {
            MELT_LOCATION("warmelt-outobj.melt:4448:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L13*/ meltfnum[9]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4449:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "newobj; };"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4450:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4451:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L14*/ meltfnum[10] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:4452:/ cond");
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[7])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[8])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[7])) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                        /*_.REFERENCED_VALUE__V30*/ meltfptr[25] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V30*/ meltfptr[25] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L15*/ meltfnum[11] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V30*/ meltfptr[25])));;
            /*^compute*/
            /*_#ltI__L16*/ meltfnum[1] =
                ((/*_#STRBUF_USEDLENGTH__L14*/ meltfnum[10]) < (/*_#GET_INT__L15*/ meltfnum[11]));;
            MELT_LOCATION("warmelt-outobj.melt:4451:/ cond");
            /*cond*/ if (/*_#ltI__L16*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V31*/ meltfptr[26] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4451:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check limited implbuf";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4451;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V32*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V31*/ meltfptr[26] = /*_.MELT_ASSERT_FAILURE_FUN__V32*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4451:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V32*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V29*/ meltfptr[27] = /*_._IFELSE___V31*/ meltfptr[26];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4451:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L14*/ meltfnum[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V30*/ meltfptr[25] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L15*/ meltfnum[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L16*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V31*/ meltfptr[26] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V29*/ meltfptr[27] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4419:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.IFCPP___V29*/ meltfptr[27];;

        {
            MELT_LOCATION("warmelt-outobj.melt:4419:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V5*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L13*/ meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V29*/ meltfptr[27] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJRAWALLOCOBJ", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_93_WARMELTmiOUTOBJ_OUTPUCOD_OBJRAWALLOCOBJ*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE fromline 1752 */

    /** start of frame for meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE// fromline 1568
        : public Melt_CallFrameWithValues<31>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[14];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<31> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE), clos) {};
        MeltFrame_meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE() //the constructor fromline 1642
            : Melt_CallFrameWithValues<31> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<31> (fil,lin, sizeof(MeltFrame_meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE)) {};
        MeltFrame_meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<31> (fil,lin, sizeof(MeltFrame_meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE


    /** end of frame for meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE fromline 1697**/

    /* end of frame for routine meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE fromline 1756 */

    /* classy proc frame meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE */ MeltFrame_meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJNEWCLOSURE", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:4458:/ getarg");
    /*_.OBNCLO__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DECLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[2].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4459:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBNCLO__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJNEWCLOSURE*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:4459:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4459:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check oralob";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4459;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBNCLO__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4459:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4459:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:4460:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBNCLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.ILOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4461:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBNCLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBNCLO_DISCR");
            /*_.ODISCR__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4462:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBNCLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OBNCLO_ROUT");
            /*_.OROUT__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4463:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBNCLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 4, "OBNCLO_LEN");
            /*_.OLEN__V12*/ meltfptr[11] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4464:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBNCLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBDI_DESTLIST");
            /*_.DESTLIST__V13*/ meltfptr[12] = slot;
        };
        ;
        /*_#plI__L3*/ meltfnum[1] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;
        /*^compute*/
        /*_.BOXDEPTHP1__V14*/ meltfptr[13] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[2])), (/*_#plI__L3*/ meltfnum[1])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4467:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "newclosure";
            /*_.OUTPUT_LOCATION__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.ILOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:73:/ cond");
        /*cond*/ if ( (melt_module_var_fetch (meltmodvarix_FLAGVAL_OUTPUT_DESCR_COMMENT_symb))) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*^apply*/
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OBNCLO__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V17*/ meltfptr[16] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V16*/ meltfptr[15] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V17*/ meltfptr[16];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V17*/ meltfptr[16] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V16*/ meltfptr[15] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4469:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " /*newclosure*/ "));
        }
        ;
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit1__EACHLIST */
            for (/*_.CURPAIR__V18*/ meltfptr[16] = melt_list_first( (melt_ptr_t)/*_.DESTLIST__V13*/ meltfptr[12]);
                                    melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V18*/ meltfptr[16]) == MELTOBMAG_PAIR;
                                    /*_.CURPAIR__V18*/ meltfptr[16] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V18*/ meltfptr[16]))
                {
                    /*_.DST__V19*/ meltfptr[18] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V18*/ meltfptr[16]);


                    /*_#GET_INT__L4*/ meltfnum[3] =
                        (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V14*/ meltfptr[13])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4473:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#GET_INT__L4*/ meltfnum[3];
                        /*_.OUTPUT_C_CODE__V20*/ meltfptr[19] = meltgc_send((melt_ptr_t)(/*_.DST__V19*/ meltfptr[18]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:4474:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L5*/ meltfnum[4] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                        MELT_LOCATION("warmelt-outobj.melt:4475:/ cond");
                        /*cond*/ if (
                            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[6])),
                                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[7])))
                        ) /*then*/
                            {
                                /*^cond.then*/
                                /*^getslot*/
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[6])) /*=obj*/;
                                    melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                                    /*_.REFERENCED_VALUE__V22*/ meltfptr[21] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V22*/ meltfptr[21] =  /*reallynil*/ NULL ;;
                            }
                        ;
                        /*^compute*/
                        /*_#GET_INT__L6*/ meltfnum[5] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V22*/ meltfptr[21])));;
                        /*^compute*/
                        /*_#ltI__L7*/ meltfnum[6] =
                            ((/*_#STRBUF_USEDLENGTH__L5*/ meltfnum[4]) < (/*_#GET_INT__L6*/ meltfnum[5]));;
                        MELT_LOCATION("warmelt-outobj.melt:4474:/ cond");
                        /*cond*/ if (/*_#ltI__L7*/ meltfnum[6]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V23*/ meltfptr[22] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:4474:/ cond.else");

                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*^apply*/
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[4];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_cstring =  "check limited implbuf";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 4474;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V23*/ meltfptr[22] = /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[23];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:4474:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[23] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V21*/ meltfptr[20] = /*_._IFELSE___V23*/ meltfptr[22];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4474:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L5*/ meltfnum[4] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V22*/ meltfptr[21] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L6*/ meltfnum[5] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L7*/ meltfnum[6] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V23*/ meltfptr[22] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V21*/ meltfptr[20] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4476:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " ="));
                    }
                    ;
                } /* end foreach_pair_component_in_list meltcit1__EACHLIST */
            /*_.CURPAIR__V18*/ meltfptr[16] = NULL;
            /*_.DST__V19*/ meltfptr[18] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:4470:/ clear");
            /*clear*/ /*_.CURPAIR__V18*/ meltfptr[16] = 0 ;
            /*^clear*/
            /*clear*/ /*_.DST__V19*/ meltfptr[18] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L4*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_.OUTPUT_C_CODE__V20*/ meltfptr[19] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V21*/ meltfptr[20] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;
        /*_#plI__L8*/ meltfnum[4] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        {
            MELT_LOCATION("warmelt-outobj.melt:4477:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L8*/ meltfnum[4]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4478:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "(melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)("));
        }
        ;
        /*_#GET_INT__L9*/ meltfnum[5] =
            (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V14*/ meltfptr[13])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4479:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#GET_INT__L9*/ meltfnum[5];
            /*_.OUTPUT_C_CODE__V25*/ meltfptr[23] = meltgc_send((melt_ptr_t)(/*_.ODISCR__V10*/ meltfptr[9]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4480:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "), (meltroutine_ptr_t)("));
        }
        ;
        /*_#GET_INT__L10*/ meltfnum[6] =
            (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V14*/ meltfptr[13])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4481:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#GET_INT__L10*/ meltfnum[6];
            /*_.OUTPUT_C_CODE__V26*/ meltfptr[21] = meltgc_send((melt_ptr_t)(/*_.OROUT__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4482:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "), ("));
        }
        ;
        /*_#GET_INT__L11*/ meltfnum[10] =
            (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V14*/ meltfptr[13])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4483:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#GET_INT__L11*/ meltfnum[10];
            /*_.OUTPUT_C_CODE__V27*/ meltfptr[22] = meltgc_send((melt_ptr_t)(/*_.OLEN__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4484:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "));"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4485:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4486:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L12*/ meltfnum[11] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:4487:/ cond");
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[6])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[7])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[6])) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                        /*_.REFERENCED_VALUE__V29*/ meltfptr[28] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V29*/ meltfptr[28] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L13*/ meltfnum[12] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V29*/ meltfptr[28])));;
            /*^compute*/
            /*_#ltI__L14*/ meltfnum[13] =
                ((/*_#STRBUF_USEDLENGTH__L12*/ meltfnum[11]) < (/*_#GET_INT__L13*/ meltfnum[12]));;
            MELT_LOCATION("warmelt-outobj.melt:4486:/ cond");
            /*cond*/ if (/*_#ltI__L14*/ meltfnum[13]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V30*/ meltfptr[29] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4486:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check limited implbuf";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4486;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V31*/ meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V30*/ meltfptr[29] = /*_.MELT_ASSERT_FAILURE_FUN__V31*/ meltfptr[30];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4486:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V31*/ meltfptr[30] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V28*/ meltfptr[27] = /*_._IFELSE___V30*/ meltfptr[29];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4486:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L12*/ meltfnum[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V29*/ meltfptr[28] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L13*/ meltfnum[12] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L14*/ meltfnum[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V30*/ meltfptr[29] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V28*/ meltfptr[27] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V28*/ meltfptr[27];;

        MELT_LOCATION("warmelt-outobj.melt:4460:/ clear");
        /*clear*/ /*_.ILOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ODISCR__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OROUT__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OLEN__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.DESTLIST__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BOXDEPTHP1__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V16*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L8*/ meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L9*/ meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V25*/ meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L10*/ meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V26*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L11*/ meltfnum[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V27*/ meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V28*/ meltfptr[27] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4458:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:4458:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V5*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V8*/ meltfptr[6] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJNEWCLOSURE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_94_WARMELTmiOUTOBJ_OUTPUCOD_OBJNEWCLOSURE*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH fromline 1752 */

    /** start of frame for meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH// fromline 1568
        : public Melt_CallFrameWithValues<14>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[2];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<14> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH), clos) {};
        MeltFrame_meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH() //the constructor fromline 1642
            : Melt_CallFrameWithValues<14> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<14> (fil,lin, sizeof(MeltFrame_meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH)) {};
        MeltFrame_meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<14> (fil,lin, sizeof(MeltFrame_meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH


    /** end of frame for meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH fromline 1697**/

    /* end of frame for routine meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH fromline 1756 */

    /* classy proc frame meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH */ MeltFrame_meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJTOUCH", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:4492:/ getarg");
    /*_.OTOUCH__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DECLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[2].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4493:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OTOUCH__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJTOUCH*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:4493:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4493:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[3];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check otouch";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4493;
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4493:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4493:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:4494:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OTOUCH__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.ILOC__V8*/ meltfptr[6] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4495:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OTOUCH__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OTOUCH_VAL");
            /*_.TOUCHED__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4496:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OTOUCH__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OTOUCH_COMMENT");
            /*_.COMM__V10*/ meltfptr[9] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4498:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "touch";
            /*_.OUTPUT_LOCATION__V11*/ meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.ILOC__V8*/ meltfptr[6]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:73:/ cond");
        /*cond*/ if ( (melt_module_var_fetch (meltmodvarix_FLAGVAL_OUTPUT_DESCR_COMMENT_symb))) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*^apply*/
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OTOUCH__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V13*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V12*/ meltfptr[11] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V13*/ meltfptr[12];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V13*/ meltfptr[12] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V12*/ meltfptr[11] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4500:/ cond");
        /*cond*/ if (/*_.COMM__V10*/ meltfptr[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4501:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*touch:"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4502:/ locexp");
                        meltgc_add_strbuf_cident((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                 melt_string_str((melt_ptr_t)(/*_.COMM__V10*/ meltfptr[9])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4503:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*/"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4504:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4500:/ quasiblock");


                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4506:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "meltgc_touch("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4507:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V14*/ meltfptr[12] = meltgc_send((melt_ptr_t)(/*_.TOUCHED__V9*/ meltfptr[5]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4508:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4509:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_LOCATION("warmelt-outobj.melt:4494:/ clear");
        /*clear*/ /*_.ILOC__V8*/ meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.TOUCHED__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.COMM__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V14*/ meltfptr[12] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:4492:/ clear");
        /*clear*/ /*_.IFCPP___V5*/ meltfptr[4] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJTOUCH", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_95_WARMELTmiOUTOBJ_OUTPUCOD_OBJTOUCH*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ fromline 1752 */

    /** start of frame for meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ// fromline 1568
        : public Melt_CallFrameWithValues<12>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[3];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<12> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ), clos) {};
        MeltFrame_meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ() //the constructor fromline 1642
            : Melt_CallFrameWithValues<12> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<12> (fil,lin, sizeof(MeltFrame_meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ)) {};
        MeltFrame_meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<12> (fil,lin, sizeof(MeltFrame_meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ


    /** end of frame for meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ fromline 1697**/

    /* end of frame for routine meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ fromline 1756 */

    /* classy proc frame meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ */ MeltFrame_meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_DBGTRACEWRITEOBJ", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:4517:/ getarg");
    /*_.OTWRO__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DECLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[2].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4518:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OTWRO__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJDBGTRACEWRITEOBJ*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:4518:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4518:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check otwro";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4518;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OTWRO__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4518:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4518:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:4519:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OTWRO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.ILOC__V8*/ meltfptr[6] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4520:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OTWRO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBDTW_WRITTENOBJ");
            /*_.OWRITTEN__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4521:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OTWRO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBDTW_MESSAGE");
            /*_.MSG__V10*/ meltfptr[9] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4523:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "touchobj";
            /*_.OUTPUT_LOCATION__V11*/ meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.ILOC__V8*/ meltfptr[6]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4524:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4525:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "melt_dbgtrace_written_object ("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4526:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V12*/ meltfptr[11] = meltgc_send((melt_ptr_t)(/*_.OWRITTEN__V9*/ meltfptr[5]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[3])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4527:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ", \""));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_STRING__L3*/ meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.MSG__V10*/ meltfptr[9])) == MELTOBMAG_STRING);;
        MELT_LOCATION("warmelt-outobj.melt:4528:/ cond");
        /*cond*/ if (/*_#IS_STRING__L3*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4529:/ locexp");
                        meltgc_add_strbuf_cstr ((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                melt_string_str ((melt_ptr_t)(/*_.MSG__V10*/ meltfptr[9])));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:4528:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4530:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*written object*"));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4531:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "\");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4532:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_LOCATION("warmelt-outobj.melt:4519:/ clear");
        /*clear*/ /*_.ILOC__V8*/ meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OWRITTEN__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MSG__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_STRING__L3*/ meltfnum[1] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:4517:/ clear");
        /*clear*/ /*_.IFCPP___V5*/ meltfptr[4] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_DBGTRACEWRITEOBJ", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_96_WARMELTmiOUTOBJ_OUTPUCOD_DBGTRACEWRITEOBJ*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE fromline 1752 */

    /** start of frame for meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE// fromline 1568
        : public Melt_CallFrameWithValues<27>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[12];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<27> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE), clos) {};
        MeltFrame_meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE() //the constructor fromline 1642
            : Melt_CallFrameWithValues<27> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<27> (fil,lin, sizeof(MeltFrame_meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE)) {};
        MeltFrame_meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<27> (fil,lin, sizeof(MeltFrame_meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE


    /** end of frame for meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE fromline 1697**/

    /* end of frame for routine meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE fromline 1756 */

    /* classy proc frame meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE */ MeltFrame_meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJPUTUPLE", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:4537:/ getarg");
    /*_.OPTUP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DECLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[2].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4538:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OPTUP__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJPUTUPLE*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:4538:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4538:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check optyp";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4538;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OPTUP__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4538:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4538:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:4539:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPTUP__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.ILOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4540:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPTUP__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OPUTU_TUPLED");
            /*_.OTUP__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4541:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPTUP__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OPUTU_OFFSET");
            /*_.OOFF__V11*/ meltfptr[10] = slot;
        };
        ;
        /*_#UNIQRANK__L3*/ meltfnum[1] = 0;;
        MELT_LOCATION("warmelt-outobj.melt:4543:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPTUP__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OPUTU_VALUE");
            /*_.OVAL__V12*/ meltfptr[11] = slot;
        };
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4545:/ locexp");
            {
                /* outpucod_objputuple UNIQRANKSET__1 */
                static long UNIQRANKSET__1_cnt ;
                UNIQRANKSET__1_cnt++		 ;
                /*_#UNIQRANK__L3*/ meltfnum[1] = UNIQRANKSET__1_cnt	 ;
            } /* end  outpucod_objputuple UNIQRANKSET__1 */ ;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4551:/ quasiblock");


        /*^multiapply*/
        /*multiapply 1args, 1x.res*/
        {

            union meltparam_un restab[1];
            memset(&restab, 0, sizeof(restab));
            /*^multiapply.xres*/
            restab[0].meltbp_aptr = (melt_ptr_t*) &/*_.FILEV__V15*/ meltfptr[14];
            /*^multiapply.appl*/
            /*_.LINEV__V14*/ meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!LINE_AND_FILE_OF_LOCATION*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.ILOC__V9*/ meltfptr[5]), (""), (union meltparam_un*)0, (MELTBPARSTR_PTR ""), restab);
        }
        ;
        /*^quasiblock*/



        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4554:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "putuple";
            /*_.OUTPUT_LOCATION__V16*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.ILOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4555:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*putupl"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4556:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "#"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4557:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#UNIQRANK__L3*/ meltfnum[1]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4558:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4559:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4560:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "melt_assertmsg(\"putupl "));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#GET_INT__L4*/ meltfnum[3] =
            (melt_get_int((melt_ptr_t)(/*_.LINEV__V14*/ meltfptr[13])));;
        /*^compute*/
        /*_#gtI__L5*/ meltfnum[4] =
            ((/*_#GET_INT__L4*/ meltfnum[3]) > (0));;
        MELT_LOCATION("warmelt-outobj.melt:4561:/ cond");
        /*cond*/ if (/*_#gtI__L5*/ meltfnum[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4563:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "["));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4564:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                              melt_string_str((melt_ptr_t)(/*_.FILEV__V15*/ meltfptr[14])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4565:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ":"));
                    }
                    ;
                    /*_#GET_INT__L6*/ meltfnum[5] =
                        (melt_get_int((melt_ptr_t)(/*_.LINEV__V14*/ meltfptr[13])));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4566:/ locexp");
                        meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L6*/ meltfnum[5]));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4567:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "] "));
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4562:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:4561:/ clear");
                    /*clear*/ /*_#GET_INT__L6*/ meltfnum[5] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4569:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "#"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4570:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#UNIQRANK__L3*/ meltfnum[1]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4571:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " checktup\", melt_magic_discr((melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4572:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V17*/ meltfptr[16] = meltgc_send((melt_ptr_t)(/*_.OTUP__V10*/ meltfptr[9]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4573:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "))== MELTOBMAG_MULTIPLE);"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4574:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4575:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "melt_assertmsg(\"putupl "));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#GET_INT__L7*/ meltfnum[5] =
            (melt_get_int((melt_ptr_t)(/*_.LINEV__V14*/ meltfptr[13])));;
        /*^compute*/
        /*_#gtI__L8*/ meltfnum[7] =
            ((/*_#GET_INT__L7*/ meltfnum[5]) > (0));;
        MELT_LOCATION("warmelt-outobj.melt:4576:/ cond");
        /*cond*/ if (/*_#gtI__L8*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4578:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "["));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4579:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                              melt_string_str((melt_ptr_t)(/*_.FILEV__V15*/ meltfptr[14])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4580:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ":"));
                    }
                    ;
                    /*_#GET_INT__L9*/ meltfnum[8] =
                        (melt_get_int((melt_ptr_t)(/*_.LINEV__V14*/ meltfptr[13])));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4581:/ locexp");
                        meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L9*/ meltfnum[8]));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4582:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "] "));
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4577:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:4576:/ clear");
                    /*clear*/ /*_#GET_INT__L9*/ meltfnum[8] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4584:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "#"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4585:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#UNIQRANK__L3*/ meltfnum[1]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4586:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " checkoff\", ("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4587:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V18*/ meltfptr[17] = meltgc_send((melt_ptr_t)(/*_.OOFF__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4588:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ">=0 && "));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4589:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V19*/ meltfptr[18] = meltgc_send((melt_ptr_t)(/*_.OOFF__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4590:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "< melt_multiple_length((melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4591:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V20*/ meltfptr[19] = meltgc_send((melt_ptr_t)(/*_.OTUP__V10*/ meltfptr[9]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4592:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "))));"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4593:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4594:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "((meltmultiple_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4595:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V21*/ meltfptr[20] = meltgc_send((melt_ptr_t)(/*_.OTUP__V10*/ meltfptr[9]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4596:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "))->tabval["));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4597:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V22*/ meltfptr[21] = meltgc_send((melt_ptr_t)(/*_.OOFF__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4598:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "] = (melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4599:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V23*/ meltfptr[22] = meltgc_send((melt_ptr_t)(/*_.OVAL__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4600:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4601:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4602:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L10*/ meltfnum[8] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:4603:/ cond");
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[5])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[6])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[5])) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                        /*_.REFERENCED_VALUE__V25*/ meltfptr[24] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V25*/ meltfptr[24] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L11*/ meltfnum[10] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V25*/ meltfptr[24])));;
            /*^compute*/
            /*_#ltI__L12*/ meltfnum[11] =
                ((/*_#STRBUF_USEDLENGTH__L10*/ meltfnum[8]) < (/*_#GET_INT__L11*/ meltfnum[10]));;
            MELT_LOCATION("warmelt-outobj.melt:4602:/ cond");
            /*cond*/ if (/*_#ltI__L12*/ meltfnum[11]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V26*/ meltfptr[25] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4602:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check limited implbuf";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4602;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V26*/ meltfptr[25] = /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4602:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V24*/ meltfptr[23] = /*_._IFELSE___V26*/ meltfptr[25];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4602:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L10*/ meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V25*/ meltfptr[24] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L11*/ meltfnum[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L12*/ meltfnum[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V26*/ meltfptr[25] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V24*/ meltfptr[23] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_.MULTI___V13*/ meltfptr[12] = /*_.IFCPP___V24*/ meltfptr[23];;

        MELT_LOCATION("warmelt-outobj.melt:4551:/ clear");
        /*clear*/ /*_.OUTPUT_LOCATION__V16*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L4*/ meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L5*/ meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V17*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L7*/ meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L8*/ meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V18*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V20*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V21*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V22*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V23*/ meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V24*/ meltfptr[23] = 0 ;

        /*^clear*/
        /*clear*/ /*_.FILEV__V15*/ meltfptr[14] = 0 ;
        /*_.LET___V8*/ meltfptr[6] = /*_.MULTI___V13*/ meltfptr[12];;

        MELT_LOCATION("warmelt-outobj.melt:4539:/ clear");
        /*clear*/ /*_.ILOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OTUP__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OOFF__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_#UNIQRANK__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OVAL__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MULTI___V13*/ meltfptr[12] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4537:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:4537:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V5*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V8*/ meltfptr[6] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJPUTUPLE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_97_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTUPLE*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD fromline 1752 */

    /** start of frame for meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD// fromline 1568
        : public Melt_CallFrameWithValues<18>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[5];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<18> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD), clos) {};
        MeltFrame_meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD() //the constructor fromline 1642
            : Melt_CallFrameWithValues<18> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<18> (fil,lin, sizeof(MeltFrame_meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD)) {};
        MeltFrame_meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<18> (fil,lin, sizeof(MeltFrame_meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD


    /** end of frame for meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD fromline 1697**/

    /* end of frame for routine meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD fromline 1756 */

    /* classy proc frame meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD */ MeltFrame_meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJPUTPAIRHEAD", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:4612:/ getarg");
    /*_.OPUT__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DECLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[2].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-outobj.melt:4613:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPUT__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.OLOC__V6*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4614:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OPUT__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJPUTPAIR*/ meltfrout->tabval[0])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OPUT__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "OPUTP_PAIR");
                    /*_.OPAIR__V7*/ meltfptr[6] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OPAIR__V7*/ meltfptr[6] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:4615:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OPUT__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJPUTPAIRHEAD*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OPUT__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "OPUTP_HEAD");
                    /*_.OHEAD__V8*/ meltfptr[7] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OHEAD__V8*/ meltfptr[7] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:4616:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!OBJPUTPAIRHEAD_COUNTER*/ meltfrout->tabval[2])),
                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)((/*!OBJPUTPAIRHEAD_COUNTER*/ meltfrout->tabval[2])) /*=obj*/;
                    melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                    /*_.OLDCOUNT__V9*/ meltfptr[8] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OLDCOUNT__V9*/ meltfptr[8] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_.NEWCOUNT__V10*/ meltfptr[9] =
            /*+ivi*/ ((melt_magic_discr((melt_ptr_t) /*_.OLDCOUNT__V9*/ meltfptr[8])==MELTOBMAG_INT)?
                      (meltgc_new_int
                       ((meltobject_ptr_t) MELT_PREDEF (DISCR_CONSTANT_INTEGER),
                        (melt_unsafe_boxed_int_value((melt_ptr_t) /*_.OLDCOUNT__V9*/ meltfptr[8]) + (1))))
                      :(melt_ptr_t)NULL);;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4619:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "putpairhead";
            /*_.OUTPUT_LOCATION__V11*/ meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.OLOC__V6*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4620:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*putpairhead*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4621:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4622:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!OBJPUTPAIRHEAD_COUNTER*/ meltfrout->tabval[2])),
                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @REFERENCED_VALUE", melt_magic_discr((melt_ptr_t)((/*!OBJPUTPAIRHEAD_COUNTER*/ meltfrout->tabval[2]))) == MELTOBMAG_OBJECT);
                    melt_putfield_object(((/*!OBJPUTPAIRHEAD_COUNTER*/ meltfrout->tabval[2])), (0), (/*_.NEWCOUNT__V10*/ meltfptr[9]), "REFERENCED_VALUE");
                    ;
                    /*^touch*/
                    meltgc_touch((/*!OBJPUTPAIRHEAD_COUNTER*/ meltfrout->tabval[2]));
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object ((/*!OBJPUTPAIRHEAD_COUNTER*/ meltfrout->tabval[2]), "put-fields");
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4623:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "melt_assertmsg(\"putpairhead /"));
        }
        ;
        /*_#GET_INT__L2*/ meltfnum[1] =
            (melt_get_int((melt_ptr_t)(/*_.NEWCOUNT__V10*/ meltfptr[9])));;

        {
            MELT_LOCATION("warmelt-outobj.melt:4624:/ locexp");
            meltgc_add_strbuf_hex((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L2*/ meltfnum[1]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4625:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " checkpair\", melt_magic_discr((melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4626:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V12*/ meltfptr[11] = meltgc_send((melt_ptr_t)(/*_.OPAIR__V7*/ meltfptr[6]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4627:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "))== MELTOBMAG_PAIR);"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4628:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4629:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "((meltpair_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4630:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V13*/ meltfptr[12] = meltgc_send((melt_ptr_t)(/*_.OPAIR__V7*/ meltfptr[6]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4631:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "))->hd = (melt_ptr_t) ("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4632:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V14*/ meltfptr[13] = meltgc_send((melt_ptr_t)(/*_.OHEAD__V8*/ meltfptr[7]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4633:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4634:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4635:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L3*/ meltfnum[2] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:4636:/ cond");
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[6])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[3])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[6])) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                        /*_.REFERENCED_VALUE__V16*/ meltfptr[15] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V16*/ meltfptr[15] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L4*/ meltfnum[3] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V16*/ meltfptr[15])));;
            /*^compute*/
            /*_#ltI__L5*/ meltfnum[4] =
                ((/*_#STRBUF_USEDLENGTH__L3*/ meltfnum[2]) < (/*_#GET_INT__L4*/ meltfnum[3]));;
            MELT_LOCATION("warmelt-outobj.melt:4635:/ cond");
            /*cond*/ if (/*_#ltI__L5*/ meltfnum[4]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V17*/ meltfptr[16] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4635:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check limited implbuf";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4635;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[7])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V17*/ meltfptr[16] = /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4635:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V15*/ meltfptr[14] = /*_._IFELSE___V17*/ meltfptr[16];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4635:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L3*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V16*/ meltfptr[15] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L4*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L5*/ meltfnum[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V17*/ meltfptr[16] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V15*/ meltfptr[14] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V5*/ meltfptr[4] = /*_.IFCPP___V15*/ meltfptr[14];;

        MELT_LOCATION("warmelt-outobj.melt:4613:/ clear");
        /*clear*/ /*_.OLOC__V6*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OPAIR__V7*/ meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OHEAD__V8*/ meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OLDCOUNT__V9*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NEWCOUNT__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L2*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V15*/ meltfptr[14] = 0 ;
        MELT_LOCATION("warmelt-outobj.melt:4612:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V5*/ meltfptr[4];;

        {
            MELT_LOCATION("warmelt-outobj.melt:4612:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.LET___V5*/ meltfptr[4] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJPUTPAIRHEAD", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_98_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRHEAD*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL fromline 1752 */

    /** start of frame for meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL// fromline 1568
        : public Melt_CallFrameWithValues<16>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[5];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<16> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL), clos) {};
        MeltFrame_meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL() //the constructor fromline 1642
            : Melt_CallFrameWithValues<16> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<16> (fil,lin, sizeof(MeltFrame_meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL)) {};
        MeltFrame_meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<16> (fil,lin, sizeof(MeltFrame_meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL


    /** end of frame for meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL fromline 1697**/

    /* end of frame for routine meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL fromline 1756 */

    /* classy proc frame meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL */ MeltFrame_meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJPUTPAIRTAIL", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:4641:/ getarg");
    /*_.OPUT__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DECLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[2].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-outobj.melt:4642:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPUT__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.OLOC__V6*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4643:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OPUT__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJPUTPAIR*/ meltfrout->tabval[0])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OPUT__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "OPUTP_PAIR");
                    /*_.OPAIR__V7*/ meltfptr[6] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OPAIR__V7*/ meltfptr[6] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:4644:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OPUT__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJPUTPAIRTAIL*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OPUT__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "OPUTP_TAIL");
                    /*_.OTAIL__V8*/ meltfptr[7] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OTAIL__V8*/ meltfptr[7] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4646:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "putpairtail";
            /*_.OUTPUT_LOCATION__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.OLOC__V6*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4647:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*putpairtail*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4648:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4649:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "melt_assertmsg(\"putpairtail /"));
        }
        ;
        /*_#OBJ_HASH__L2*/ meltfnum[1] =
            (melt_obj_hash((melt_ptr_t)(/*_.OPUT__V2*/ meltfptr[1])));;

        {
            MELT_LOCATION("warmelt-outobj.melt:4650:/ locexp");
            meltgc_add_strbuf_hex((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#OBJ_HASH__L2*/ meltfnum[1]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4651:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " checkpair\", melt_magic_discr((melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4652:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V10*/ meltfptr[9] = meltgc_send((melt_ptr_t)(/*_.OPAIR__V7*/ meltfptr[6]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[3])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4653:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "))== MELTOBMAG_PAIR);"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4654:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4655:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "((meltpair_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4656:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V11*/ meltfptr[10] = meltgc_send((melt_ptr_t)(/*_.OPAIR__V7*/ meltfptr[6]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[3])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4657:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "))->tl = (meltpair_ptr_t) ("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4658:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V12*/ meltfptr[11] = meltgc_send((melt_ptr_t)(/*_.OTAIL__V8*/ meltfptr[7]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[3])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4659:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4660:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4661:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L3*/ meltfnum[2] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:4662:/ cond");
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[4])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[5])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[4])) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                        /*_.REFERENCED_VALUE__V14*/ meltfptr[13] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V14*/ meltfptr[13] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L4*/ meltfnum[3] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V14*/ meltfptr[13])));;
            /*^compute*/
            /*_#ltI__L5*/ meltfnum[4] =
                ((/*_#STRBUF_USEDLENGTH__L3*/ meltfnum[2]) < (/*_#GET_INT__L4*/ meltfnum[3]));;
            MELT_LOCATION("warmelt-outobj.melt:4661:/ cond");
            /*cond*/ if (/*_#ltI__L5*/ meltfnum[4]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4661:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check limited implbuf";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4661;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[6])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/ meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4661:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V13*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4661:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L3*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V14*/ meltfptr[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L4*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L5*/ meltfnum[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/ meltfptr[14] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V13*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V5*/ meltfptr[4] = /*_.IFCPP___V13*/ meltfptr[12];;

        MELT_LOCATION("warmelt-outobj.melt:4642:/ clear");
        /*clear*/ /*_.OLOC__V6*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OPAIR__V7*/ meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OTAIL__V8*/ meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V9*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_#OBJ_HASH__L2*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V13*/ meltfptr[12] = 0 ;
        MELT_LOCATION("warmelt-outobj.melt:4641:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V5*/ meltfptr[4];;

        {
            MELT_LOCATION("warmelt-outobj.melt:4641:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.LET___V5*/ meltfptr[4] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJPUTPAIRTAIL", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_99_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTPAIRTAIL*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST fromline 1752 */

    /** start of frame for meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST// fromline 1568
        : public Melt_CallFrameWithValues<19>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[4];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<19> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST), clos) {};
        MeltFrame_meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<19> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST)) {};
        MeltFrame_meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST


    /** end of frame for meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST fromline 1697**/

    /* end of frame for routine meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST fromline 1756 */

    /* classy proc frame meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST */ MeltFrame_meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJPUTLIST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:4667:/ getarg");
    /*_.OPUT__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DECLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[2].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-outobj.melt:4668:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPUT__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.OLOC__V6*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4669:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OPUT__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJPUTLIST*/ meltfrout->tabval[0])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OPUT__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "OPUTL_LIST");
                    /*_.OLIST__V7*/ meltfptr[6] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OLIST__V7*/ meltfptr[6] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:4670:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OPUT__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJPUTLIST*/ meltfrout->tabval[0])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OPUT__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "OPUTL_FIRST");
                    /*_.OFIRST__V8*/ meltfptr[7] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OFIRST__V8*/ meltfptr[7] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:4671:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OPUT__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJPUTLIST*/ meltfrout->tabval[0])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OPUT__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 3, "OPUTL_LAST");
                    /*_.OLAST__V9*/ meltfptr[8] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OLAST__V9*/ meltfptr[8] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4673:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "putlist";
            /*_.OUTPUT_LOCATION__V10*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[1])), (melt_ptr_t)(/*_.OLOC__V6*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4674:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*putlist*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4675:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4676:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "melt_assertmsg(\"putlist checklist\", melt_magic_discr((melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4677:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V11*/ meltfptr[10] = meltgc_send((melt_ptr_t)(/*_.OLIST__V7*/ meltfptr[6]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[2])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4678:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "))== MELTOBMAG_LIST);"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4679:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4680:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "((meltlist_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4681:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V12*/ meltfptr[11] = meltgc_send((melt_ptr_t)(/*_.OLIST__V7*/ meltfptr[6]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[2])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4682:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "))->first = (meltpair_ptr_t) ("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4683:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V13*/ meltfptr[12] = meltgc_send((melt_ptr_t)(/*_.OFIRST__V8*/ meltfptr[7]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[2])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4684:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4685:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4686:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "((meltlist_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4687:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V14*/ meltfptr[13] = meltgc_send((melt_ptr_t)(/*_.OLIST__V7*/ meltfptr[6]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[2])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4688:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "))->last = (meltpair_ptr_t) ("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4689:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V15*/ meltfptr[14] = meltgc_send((melt_ptr_t)(/*_.OLAST__V9*/ meltfptr[8]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[2])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4690:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4691:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4692:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L2*/ meltfnum[1] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:4693:/ cond");
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[3])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[4])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[3])) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                        /*_.REFERENCED_VALUE__V17*/ meltfptr[16] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V17*/ meltfptr[16] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L3*/ meltfnum[2] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V17*/ meltfptr[16])));;
            /*^compute*/
            /*_#ltI__L4*/ meltfnum[3] =
                ((/*_#STRBUF_USEDLENGTH__L2*/ meltfnum[1]) < (/*_#GET_INT__L3*/ meltfnum[2]));;
            MELT_LOCATION("warmelt-outobj.melt:4692:/ cond");
            /*cond*/ if (/*_#ltI__L4*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V18*/ meltfptr[17] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4692:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check limited implbuf";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4692;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[5])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V18*/ meltfptr[17] = /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4692:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V16*/ meltfptr[15] = /*_._IFELSE___V18*/ meltfptr[17];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4692:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V17*/ meltfptr[16] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L3*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L4*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V18*/ meltfptr[17] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V16*/ meltfptr[15] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V5*/ meltfptr[4] = /*_.IFCPP___V16*/ meltfptr[15];;

        MELT_LOCATION("warmelt-outobj.melt:4668:/ clear");
        /*clear*/ /*_.OLOC__V6*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OLIST__V7*/ meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OFIRST__V8*/ meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OLAST__V9*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V16*/ meltfptr[15] = 0 ;
        MELT_LOCATION("warmelt-outobj.melt:4667:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V5*/ meltfptr[4];;

        {
            MELT_LOCATION("warmelt-outobj.melt:4667:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.LET___V5*/ meltfptr[4] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJPUTLIST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_100_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTLIST*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT fromline 1752 */

    /** start of frame for meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT// fromline 1568
        : public Melt_CallFrameWithValues<30>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[15];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<30> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT), clos) {};
        MeltFrame_meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT() //the constructor fromline 1642
            : Melt_CallFrameWithValues<30> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<30> (fil,lin, sizeof(MeltFrame_meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT)) {};
        MeltFrame_meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<30> (fil,lin, sizeof(MeltFrame_meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT


    /** end of frame for meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT fromline 1697**/

    /* end of frame for routine meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT fromline 1756 */

    /* classy proc frame meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT */ MeltFrame_meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJGETSLOT", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:4698:/ getarg");
    /*_.OGSL__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DECLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[2].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4699:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OGSL__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJGETSLOT*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:4699:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4699:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check ogsl";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4699;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OGSL__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4699:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4699:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:4700:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OGSL__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.OLOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4701:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OGSL__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBDI_DESTLIST");
            /*_.DESTLIST__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4702:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OGSL__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OGETSL_OBJ");
            /*_.OOBJ__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4703:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OGSL__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OGETSL_FIELD");
            /*_.OFIELD__V12*/ meltfptr[11] = slot;
        };
        ;
        /*_#plI__L3*/ meltfnum[1] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;
        /*^compute*/
        /*_.BOXDEPTHP1__V13*/ meltfptr[12] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[2])), (/*_#plI__L3*/ meltfnum[1])));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4706:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[3] =
                melt_is_instance_of((melt_ptr_t)(/*_.OFIELD__V12*/ meltfptr[11]), (melt_ptr_t)((/*!CLASS_FIELD*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-outobj.melt:4706:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4706:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check ofield";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4706;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OFIELD__V12*/ meltfptr[11];
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/ meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4706:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/ meltfptr[13] = /*_._IFELSE___V15*/ meltfptr[14];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4706:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/ meltfptr[14] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[13] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4707:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "getslot";
            /*_.OUTPUT_LOCATION__V17*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4708:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "{ melt_ptr_t slot=NULL, obj=NULL;"));
        }
        ;
        /*_#plI__L5*/ meltfnum[3] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

        {
            MELT_LOCATION("warmelt-outobj.melt:4709:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L5*/ meltfnum[3]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4710:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "obj = (melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4711:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V18*/ meltfptr[14] = meltgc_send((melt_ptr_t)(/*_.OOBJ__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4712:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ") /*=obj*/;"));
        }
        ;
        /*_#plI__L6*/ meltfnum[5] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

        {
            MELT_LOCATION("warmelt-outobj.melt:4713:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L6*/ meltfnum[5]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4714:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "melt_object_get_field(slot,obj, "));
        }
        ;
        /*_#GET_INT__L7*/ meltfnum[6] =
            (melt_get_int((melt_ptr_t)(/*_.OFIELD__V12*/ meltfptr[11])));;

        {
            MELT_LOCATION("warmelt-outobj.melt:4715:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L7*/ meltfnum[6]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4716:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ", \""));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:4717:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OFIELD__V12*/ meltfptr[11]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
            /*_.NAMED_NAME__V19*/ meltfptr[18] = slot;
        };
        ;

        {
            /*^locexp*/
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V19*/ meltfptr[18])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4718:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "\");"));
        }
        ;
        /*_#plI__L8*/ meltfnum[7] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

        {
            MELT_LOCATION("warmelt-outobj.melt:4719:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L8*/ meltfnum[7]), 0);
        }
        ;
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit1__EACHLIST */
            for (/*_.CURPAIR__V20*/ meltfptr[19] = melt_list_first( (melt_ptr_t)/*_.DESTLIST__V10*/ meltfptr[9]);
                                    melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V20*/ meltfptr[19]) == MELTOBMAG_PAIR;
                                    /*_.CURPAIR__V20*/ meltfptr[19] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V20*/ meltfptr[19]))
                {
                    /*_.DST__V21*/ meltfptr[20] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V20*/ meltfptr[19]);


                    /*_#GET_INT__L9*/ meltfnum[8] =
                        (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V13*/ meltfptr[12])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4723:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#GET_INT__L9*/ meltfnum[8];
                        /*_.OUTPUT_C_CODE__V22*/ meltfptr[21] = meltgc_send((melt_ptr_t)(/*_.DST__V21*/ meltfptr[20]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:4724:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L10*/ meltfnum[9] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                        MELT_LOCATION("warmelt-outobj.melt:4725:/ cond");
                        /*cond*/ if (
                            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[6])),
                                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[7])))
                        ) /*then*/
                            {
                                /*^cond.then*/
                                /*^getslot*/
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[6])) /*=obj*/;
                                    melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                                    /*_.REFERENCED_VALUE__V24*/ meltfptr[23] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V24*/ meltfptr[23] =  /*reallynil*/ NULL ;;
                            }
                        ;
                        /*^compute*/
                        /*_#GET_INT__L11*/ meltfnum[10] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V24*/ meltfptr[23])));;
                        /*^compute*/
                        /*_#ltI__L12*/ meltfnum[11] =
                            ((/*_#STRBUF_USEDLENGTH__L10*/ meltfnum[9]) < (/*_#GET_INT__L11*/ meltfnum[10]));;
                        MELT_LOCATION("warmelt-outobj.melt:4724:/ cond");
                        /*cond*/ if (/*_#ltI__L12*/ meltfnum[11]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V25*/ meltfptr[24] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:4724:/ cond.else");

                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*^apply*/
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[4];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_cstring =  "check limited implbuf";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 4724;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V26*/ meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V25*/ meltfptr[24] = /*_.MELT_ASSERT_FAILURE_FUN__V26*/ meltfptr[25];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:4724:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V26*/ meltfptr[25] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V23*/ meltfptr[22] = /*_._IFELSE___V25*/ meltfptr[24];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4724:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L10*/ meltfnum[9] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V24*/ meltfptr[23] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L11*/ meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L12*/ meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V25*/ meltfptr[24] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V23*/ meltfptr[22] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4726:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = "));
                    }
                    ;
                } /* end foreach_pair_component_in_list meltcit1__EACHLIST */
            /*_.CURPAIR__V20*/ meltfptr[19] = NULL;
            /*_.DST__V21*/ meltfptr[20] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:4720:/ clear");
            /*clear*/ /*_.CURPAIR__V20*/ meltfptr[19] = 0 ;
            /*^clear*/
            /*clear*/ /*_.DST__V21*/ meltfptr[20] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L9*/ meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_.OUTPUT_C_CODE__V22*/ meltfptr[21] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V23*/ meltfptr[22] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4727:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "slot; };"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4728:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4729:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L13*/ meltfnum[9] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:4730:/ cond");
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[6])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[7])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[6])) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                        /*_.REFERENCED_VALUE__V28*/ meltfptr[23] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V28*/ meltfptr[23] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L14*/ meltfnum[10] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V28*/ meltfptr[23])));;
            /*^compute*/
            /*_#ltI__L15*/ meltfnum[11] =
                ((/*_#STRBUF_USEDLENGTH__L13*/ meltfnum[9]) < (/*_#GET_INT__L14*/ meltfnum[10]));;
            MELT_LOCATION("warmelt-outobj.melt:4729:/ cond");
            /*cond*/ if (/*_#ltI__L15*/ meltfnum[11]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V29*/ meltfptr[24] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4729:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check limited implbuf";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4729;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V30*/ meltfptr[29] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V29*/ meltfptr[24] = /*_.MELT_ASSERT_FAILURE_FUN__V30*/ meltfptr[29];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4729:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V30*/ meltfptr[29] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V27*/ meltfptr[25] = /*_._IFELSE___V29*/ meltfptr[24];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4729:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L13*/ meltfnum[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V28*/ meltfptr[23] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L14*/ meltfnum[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L15*/ meltfnum[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V29*/ meltfptr[24] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V27*/ meltfptr[25] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V27*/ meltfptr[25];;

        MELT_LOCATION("warmelt-outobj.melt:4700:/ clear");
        /*clear*/ /*_.OLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.DESTLIST__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OOBJ__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OFIELD__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BOXDEPTHP1__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V17*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L5*/ meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V18*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L6*/ meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L7*/ meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NAMED_NAME__V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L8*/ meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V27*/ meltfptr[25] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4698:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:4698:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V5*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V8*/ meltfptr[6] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJGETSLOT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_101_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETSLOT*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT fromline 1752 */

    /** start of frame for meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT// fromline 1568
        : public Melt_CallFrameWithValues<58>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[17];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<58> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT), clos) {};
        MeltFrame_meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT() //the constructor fromline 1642
            : Melt_CallFrameWithValues<58> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<58> (fil,lin, sizeof(MeltFrame_meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT)) {};
        MeltFrame_meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<58> (fil,lin, sizeof(MeltFrame_meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT


    /** end of frame for meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT fromline 1697**/

    /* end of frame for routine meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT fromline 1756 */

    /* classy proc frame meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT */ MeltFrame_meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJPUTSLOT", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:4737:/ getarg");
    /*_.OPSLO__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DECLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[2].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4738:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L2*/ meltfnum[1] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:4738:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L3*/ meltfnum[2] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:4738:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4738;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objputslot opslo=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OPSLO__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4738:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L3*/ meltfnum[2] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V5*/ meltfptr[4] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:4738:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V5*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*void*/(void)0;
            }
            ;
            /*epilog*/
        }

#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4739:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.OPSLO__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJPUTSLOT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:4739:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4739:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check opslo";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4739;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OPSLO__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4739:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4739:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:73:/ cond");
        /*cond*/ if ( (melt_module_var_fetch (meltmodvarix_FLAGVAL_OUTPUT_DESCR_COMMENT_symb))) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*^apply*/
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OPSLO__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V11*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V10*/ meltfptr[8] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V11*/ meltfptr[4];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V11*/ meltfptr[4] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V10*/ meltfptr[8] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:4741:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPSLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.ILOC__V13*/ meltfptr[12] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4742:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPSLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OSLOT_ODATA");
            /*_.ODATA__V14*/ meltfptr[13] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4743:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPSLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OSLOT_OFFSET");
            /*_.OOFF__V15*/ meltfptr[14] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4744:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPSLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OSLOT_FIELD");
            /*_.OFIELD__V16*/ meltfptr[15] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4745:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPSLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 4, "OSLOT_VALUE");
            /*_.OVAL__V17*/ meltfptr[16] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4746:/ quasiblock");


        /*_.MSGBUF__V19*/ meltfptr[18] =
            (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[4])), (const char*)0);;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4748:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "putslot ";
            /*_.ADD2OUT__V20*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.MSGBUF__V19*/ meltfptr[18]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_A__L5*/ meltfnum[1] =
            melt_is_instance_of((melt_ptr_t)(/*_.ODATA__V14*/ meltfptr[13]), (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[6])));;
        MELT_LOCATION("warmelt-outobj.melt:4749:/ cond");
        /*cond*/ if (/*_#IS_A__L5*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-outobj.melt:4750:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.ODATA__V14*/ meltfptr[13]) /*=obj*/;
                        melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                        /*_.NAMED_NAME__V22*/ meltfptr[21] = slot;
                    };
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*^apply*/
                    /*apply*/
                    {
                        union meltparam_un argtab[2];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  " ";
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.NAMED_NAME__V22*/ meltfptr[21];
                        /*_.ADD2OUT__V23*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.MSGBUF__V19*/ meltfptr[18]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IFELSE___V21*/ meltfptr[20] = /*_.ADD2OUT__V23*/ meltfptr[22];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:4749:/ clear");
                    /*clear*/ /*_.NAMED_NAME__V22*/ meltfptr[21] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ADD2OUT__V23*/ meltfptr[22] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_A__L6*/ meltfnum[2] =
                        melt_is_instance_of((melt_ptr_t)(/*_.ODATA__V14*/ meltfptr[13]), (melt_ptr_t)((/*!CLASS_OBJINITELEM*/ meltfrout->tabval[7])));;
                    MELT_LOCATION("warmelt-outobj.melt:4752:/ cond");
                    /*cond*/ if (/*_#IS_A__L6*/ meltfnum[2]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-outobj.melt:4753:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.ODATA__V14*/ meltfptr[13]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 1, "OIE_CNAME");
                                    /*_.OIE_CNAME__V25*/ meltfptr[22] = slot;
                                };
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                /*^apply*/
                                /*apply*/
                                {
                                    union meltparam_un argtab[2];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_cstring =  " ~";
                                    /*^apply.arg*/
                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.OIE_CNAME__V25*/ meltfptr[22];
                                    /*_.ADD2OUT__V26*/ meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.MSGBUF__V19*/ meltfptr[18]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                /*_._IFELSE___V24*/ meltfptr[21] = /*_.ADD2OUT__V26*/ meltfptr[25];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:4752:/ clear");
                                /*clear*/ /*_.OIE_CNAME__V25*/ meltfptr[22] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.ADD2OUT__V26*/ meltfptr[25] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:4755:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[1];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_cstring =  " _";
                                    /*_.ADD2OUT__V27*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.MSGBUF__V19*/ meltfptr[18]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:4754:/ quasiblock");


                                /*_.PROGN___V28*/ meltfptr[25] = /*_.ADD2OUT__V27*/ meltfptr[22];;
                                /*^compute*/
                                /*_._IFELSE___V24*/ meltfptr[21] = /*_.PROGN___V28*/ meltfptr[25];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:4752:/ clear");
                                /*clear*/ /*_.ADD2OUT__V27*/ meltfptr[22] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V28*/ meltfptr[25] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V21*/ meltfptr[20] = /*_._IFELSE___V24*/ meltfptr[21];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:4749:/ clear");
                    /*clear*/ /*_#IS_A__L6*/ meltfnum[2] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V24*/ meltfptr[21] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_A__L7*/ meltfnum[2] =
            melt_is_instance_of((melt_ptr_t)(/*_.OFIELD__V16*/ meltfptr[15]), (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[6])));;
        MELT_LOCATION("warmelt-outobj.melt:4756:/ cond");
        /*cond*/ if (/*_#IS_A__L7*/ meltfnum[2]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-outobj.melt:4757:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.OFIELD__V16*/ meltfptr[15]) /*=obj*/;
                        melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                        /*_.NAMED_NAME__V30*/ meltfptr[25] = slot;
                    };
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*^apply*/
                    /*apply*/
                    {
                        union meltparam_un argtab[2];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  " @";
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.NAMED_NAME__V30*/ meltfptr[25];
                        /*_.ADD2OUT__V31*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.MSGBUF__V19*/ meltfptr[18]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IFELSE___V29*/ meltfptr[22] = /*_.ADD2OUT__V31*/ meltfptr[21];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:4756:/ clear");
                    /*clear*/ /*_.NAMED_NAME__V30*/ meltfptr[25] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ADD2OUT__V31*/ meltfptr[21] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4759:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[2];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  " @#";
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.OOFF__V15*/ meltfptr[14];
                        /*_.ADD2OUT__V32*/ meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.MSGBUF__V19*/ meltfptr[18]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4758:/ quasiblock");


                    /*_.PROGN___V33*/ meltfptr[21] = /*_.ADD2OUT__V32*/ meltfptr[25];;
                    /*^compute*/
                    /*_._IFELSE___V29*/ meltfptr[22] = /*_.PROGN___V33*/ meltfptr[21];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:4756:/ clear");
                    /*clear*/ /*_.ADD2OUT__V32*/ meltfptr[25] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V33*/ meltfptr[21] = 0 ;
                }
                ;
            }
        ;
        /*_.STRBUF2STRING__V34*/ meltfptr[25] =
            (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[8])), melt_strbuf_str((melt_ptr_t)(/*_.MSGBUF__V19*/ meltfptr[18]))));;
        /*^compute*/
        /*_.LET___V18*/ meltfptr[17] = /*_.STRBUF2STRING__V34*/ meltfptr[25];;

        MELT_LOCATION("warmelt-outobj.melt:4746:/ clear");
        /*clear*/ /*_.MSGBUF__V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V20*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L5*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V21*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L7*/ meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V29*/ meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.STRBUF2STRING__V34*/ meltfptr[25] = 0 ;
        /*_.MSG__V35*/ meltfptr[21] = /*_.LET___V18*/ meltfptr[17];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4762:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L8*/ meltfnum[1] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:4762:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L8*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L9*/ meltfnum[2] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:4762:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4762;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objputslot msg=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MSG__V35*/ meltfptr[21];
                            /*_.MELT_DEBUG_FUN__V37*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V36*/ meltfptr[18] = /*_.MELT_DEBUG_FUN__V37*/ meltfptr[19];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4762:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L9*/ meltfnum[2] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V37*/ meltfptr[19] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V36*/ meltfptr[18] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:4762:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L8*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V36*/ meltfptr[18] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*void*/(void)0;
            }
            ;
            /*epilog*/
        }

#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4763:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_NOT_A__L10*/ meltfnum[2] =
                !melt_is_instance_of((melt_ptr_t)(/*_.OVAL__V17*/ meltfptr[16]), (melt_ptr_t)((/*!CLASS_NREP*/ meltfrout->tabval[9])));;
            MELT_LOCATION("warmelt-outobj.melt:4763:/ cond");
            /*cond*/ if (/*_#IS_NOT_A__L10*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V39*/ meltfptr[22] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4763:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "outpucod_objputslot check oval not nrep";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4763;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OVAL__V17*/ meltfptr[16];
                            /*_.MELT_ASSERT_FAILURE_FUN__V40*/ meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V39*/ meltfptr[22] = /*_.MELT_ASSERT_FAILURE_FUN__V40*/ meltfptr[25];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4763:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V40*/ meltfptr[25] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V38*/ meltfptr[20] = /*_._IFELSE___V39*/ meltfptr[22];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4763:/ clear");
            /*clear*/ /*_#IS_NOT_A__L10*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V39*/ meltfptr[22] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V38*/ meltfptr[20] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4764:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "putslot";
            /*_.OUTPUT_LOCATION__V41*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[10])), (melt_ptr_t)(/*_.ILOC__V13*/ meltfptr[12]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4765:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*putslot*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4766:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4767:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "melt_assertmsg(\"checkobj ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MSG__V35*/ meltfptr[21];
            /*_.ADD2OUT__V42*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4768:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "\", melt_magic_discr((melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4769:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V43*/ meltfptr[25] = meltgc_send((melt_ptr_t)(/*_.ODATA__V14*/ meltfptr[13]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[11])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4770:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ")) == MELTOBMAG_OBJECT);"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4771:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_A__L11*/ meltfnum[1] =
            melt_is_instance_of((melt_ptr_t)(/*_.OFIELD__V16*/ meltfptr[15]), (melt_ptr_t)((/*!CLASS_FIELD*/ meltfrout->tabval[12])));;
        MELT_LOCATION("warmelt-outobj.melt:4772:/ cond");
        /*cond*/ if (/*_#IS_A__L11*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4774:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "melt_putfield_object(("));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4775:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                        /*_.OUTPUT_C_CODE__V44*/ meltfptr[22] = meltgc_send((melt_ptr_t)(/*_.ODATA__V14*/ meltfptr[13]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[11])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4776:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "), ("));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4777:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                        /*_.OUTPUT_C_CODE__V45*/ meltfptr[44] = meltgc_send((melt_ptr_t)(/*_.OOFF__V15*/ meltfptr[14]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[11])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4778:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "), ("));
                    }
                    ;
                    /*_#plI__L12*/ meltfnum[2] =
                        ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4779:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#plI__L12*/ meltfnum[2];
                        /*_.OUTPUT_C_CODE__V46*/ meltfptr[45] = meltgc_send((melt_ptr_t)(/*_.OVAL__V17*/ meltfptr[16]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[11])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4780:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "), \""));
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4781:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.OFIELD__V16*/ meltfptr[15]) /*=obj*/;
                        melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                        /*_.NAMED_NAME__V47*/ meltfptr[46] = slot;
                    };
                    ;

                    {
                        /*^locexp*/
                        meltgc_add_strbuf_cident((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                 melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V47*/ meltfptr[46])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4782:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "\");"));
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4773:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:4772:/ clear");
                    /*clear*/ /*_.OUTPUT_C_CODE__V44*/ meltfptr[22] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_C_CODE__V45*/ meltfptr[44] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#plI__L12*/ meltfnum[2] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_C_CODE__V46*/ meltfptr[45] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.NAMED_NAME__V47*/ meltfptr[46] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4786:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4787:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[2];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "melt_assertmsg(\"checkoff ";
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MSG__V35*/ meltfptr[21];
                        /*_.ADD2OUT__V48*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4788:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "\", ("));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4789:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                        /*_.OUTPUT_C_CODE__V49*/ meltfptr[44] = meltgc_send((melt_ptr_t)(/*_.OOFF__V15*/ meltfptr[14]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[11])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4790:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ">=0 && "));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4791:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                        /*_.OUTPUT_C_CODE__V50*/ meltfptr[45] = meltgc_send((melt_ptr_t)(/*_.OOFF__V15*/ meltfptr[14]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[11])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4792:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "< melt_object_length((melt_ptr_t)("));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4793:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                        /*_.OUTPUT_C_CODE__V51*/ meltfptr[46] = meltgc_send((melt_ptr_t)(/*_.ODATA__V14*/ meltfptr[13]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[11])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4794:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "))));"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4795:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4796:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "((meltobject_ptr_t)("));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4797:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                        /*_.OUTPUT_C_CODE__V52*/ meltfptr[51] = meltgc_send((melt_ptr_t)(/*_.ODATA__V14*/ meltfptr[13]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[11])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4798:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "))->obj_vartab["));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4799:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                        /*_.OUTPUT_C_CODE__V53*/ meltfptr[52] = meltgc_send((melt_ptr_t)(/*_.OOFF__V15*/ meltfptr[14]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[11])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4800:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "] = (melt_ptr_t)("));
                    }
                    ;
                    /*_#plI__L13*/ meltfnum[2] =
                        ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4801:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L13*/ meltfnum[2]), 0);
                    }
                    ;
                    /*_#plI__L14*/ meltfnum[13] =
                        ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4802:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#plI__L14*/ meltfnum[13];
                        /*_.OUTPUT_C_CODE__V54*/ meltfptr[53] = meltgc_send((melt_ptr_t)(/*_.OVAL__V17*/ meltfptr[16]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[11])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4803:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4784:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:4772:/ clear");
                    /*clear*/ /*_.ADD2OUT__V48*/ meltfptr[22] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_C_CODE__V49*/ meltfptr[44] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_C_CODE__V50*/ meltfptr[45] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_C_CODE__V51*/ meltfptr[46] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_C_CODE__V52*/ meltfptr[51] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_C_CODE__V53*/ meltfptr[52] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#plI__L13*/ meltfnum[2] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#plI__L14*/ meltfnum[13] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_C_CODE__V54*/ meltfptr[53] = 0 ;
                }
                ;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4806:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4807:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L15*/ meltfnum[2] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:4808:/ cond");
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[13])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[14])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[13])) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                        /*_.REFERENCED_VALUE__V56*/ meltfptr[44] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V56*/ meltfptr[44] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L16*/ meltfnum[13] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V56*/ meltfptr[44])));;
            /*^compute*/
            /*_#ltI__L17*/ meltfnum[16] =
                ((/*_#STRBUF_USEDLENGTH__L15*/ meltfnum[2]) < (/*_#GET_INT__L16*/ meltfnum[13]));;
            MELT_LOCATION("warmelt-outobj.melt:4807:/ cond");
            /*cond*/ if (/*_#ltI__L17*/ meltfnum[16]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V57*/ meltfptr[45] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4807:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check limited implbuf";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4807;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V58*/ meltfptr[46] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V57*/ meltfptr[45] = /*_.MELT_ASSERT_FAILURE_FUN__V58*/ meltfptr[46];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4807:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V58*/ meltfptr[46] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V55*/ meltfptr[22] = /*_._IFELSE___V57*/ meltfptr[45];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4807:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L15*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V56*/ meltfptr[44] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L16*/ meltfnum[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L17*/ meltfnum[16] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V57*/ meltfptr[45] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V55*/ meltfptr[22] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V12*/ meltfptr[4] = /*_.IFCPP___V55*/ meltfptr[22];;

        MELT_LOCATION("warmelt-outobj.melt:4741:/ clear");
        /*clear*/ /*_.ILOC__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ODATA__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OOFF__V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OFIELD__V16*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OVAL__V17*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V18*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MSG__V35*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V38*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V41*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V42*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V43*/ meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L11*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V55*/ meltfptr[22] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4737:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V12*/ meltfptr[4];;

        {
            MELT_LOCATION("warmelt-outobj.melt:4737:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V7*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V10*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V12*/ meltfptr[4] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJPUTSLOT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_102_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTSLOT*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT fromline 1752 */

    /** start of frame for meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT// fromline 1568
        : public Melt_CallFrameWithValues<22>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[6];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<22> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT), clos) {};
        MeltFrame_meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT() //the constructor fromline 1642
            : Melt_CallFrameWithValues<22> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<22> (fil,lin, sizeof(MeltFrame_meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT)) {};
        MeltFrame_meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<22> (fil,lin, sizeof(MeltFrame_meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT


    /** end of frame for meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT fromline 1697**/

    /* end of frame for routine meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT fromline 1756 */

    /* classy proc frame meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT */ MeltFrame_meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJPUTCLOSUROUT", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:4815:/ getarg");
    /*_.OPCLOR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DECLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[2].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4816:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OPCLOR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJPUTCLOSUROUT*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:4816:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4816:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check opclor";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4816;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OPCLOR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4816:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4816:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:4817:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPCLOR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.OLOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4818:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPCLOR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OPCLOR_CLOS");
            /*_.OCLOS__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4819:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPCLOR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OPCLOR_ROUT");
            /*_.OROUT__V11*/ meltfptr[10] = slot;
        };
        ;
        /*_#CNT__L3*/ meltfnum[1] = 0;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:73:/ cond");
        /*cond*/ if ( (melt_module_var_fetch (meltmodvarix_FLAGVAL_OUTPUT_DESCR_COMMENT_symb))) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*^apply*/
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OPCLOR__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V13*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V12*/ meltfptr[11] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V13*/ meltfptr[12];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V13*/ meltfptr[12] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V12*/ meltfptr[11] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4823:/ locexp");
            /* GETCNTCHK__1 in outpucod_objputclosurout */
            {
                static long GETCNTCHK__1_cnt;
                GETCNTCHK__1_cnt++;
                /*_#CNT__L3*/ meltfnum[1] = GETCNTCHK__1_cnt;
            } ;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4830:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "putclosurout";
            /*_.OUTPUT_LOCATION__V14*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4831:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*putclosurout#"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4832:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#CNT__L3*/ meltfnum[1]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4833:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4834:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4835:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "melt_assertmsg(\"putclosrout#"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4836:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#CNT__L3*/ meltfnum[1]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4837:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " checkclo\", melt_magic_discr((melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4838:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V15*/ meltfptr[14] = meltgc_send((melt_ptr_t)(/*_.OCLOS__V10*/ meltfptr[9]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4839:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ")) == MELTOBMAG_CLOSURE);"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4840:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4841:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "melt_assertmsg(\"putclosrout#"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4842:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#CNT__L3*/ meltfnum[1]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4843:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " checkrout\", melt_magic_discr((melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4844:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V16*/ meltfptr[15] = meltgc_send((melt_ptr_t)(/*_.OROUT__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4845:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ")) == MELTOBMAG_ROUTINE);"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4846:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4847:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "((meltclosure_ptr_t)"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4848:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V17*/ meltfptr[16] = meltgc_send((melt_ptr_t)(/*_.OCLOS__V10*/ meltfptr[9]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4849:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ")->rout = (meltroutine_ptr_t) ("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4850:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V18*/ meltfptr[17] = meltgc_send((melt_ptr_t)(/*_.OROUT__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4851:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4852:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4853:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L4*/ meltfnum[3] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:4854:/ cond");
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[5])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[6])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[5])) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                        /*_.REFERENCED_VALUE__V20*/ meltfptr[19] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V20*/ meltfptr[19] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L5*/ meltfnum[4] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V20*/ meltfptr[19])));;
            /*^compute*/
            /*_#ltI__L6*/ meltfnum[5] =
                ((/*_#STRBUF_USEDLENGTH__L4*/ meltfnum[3]) < (/*_#GET_INT__L5*/ meltfnum[4]));;
            MELT_LOCATION("warmelt-outobj.melt:4853:/ cond");
            /*cond*/ if (/*_#ltI__L6*/ meltfnum[5]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V21*/ meltfptr[20] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4853:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check limited implbuf";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4853;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V21*/ meltfptr[20] = /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4853:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V19*/ meltfptr[18] = /*_._IFELSE___V21*/ meltfptr[20];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4853:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L4*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V20*/ meltfptr[19] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L5*/ meltfnum[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L6*/ meltfnum[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V21*/ meltfptr[20] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V19*/ meltfptr[18] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V19*/ meltfptr[18];;

        MELT_LOCATION("warmelt-outobj.melt:4817:/ clear");
        /*clear*/ /*_.OLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OCLOS__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OROUT__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_#CNT__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V14*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V16*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V17*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V18*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V19*/ meltfptr[18] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4815:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:4815:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V5*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V8*/ meltfptr[6] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJPUTCLOSUROUT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_103_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSUROUT*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV fromline 1752 */

    /** start of frame for meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV// fromline 1568
        : public Melt_CallFrameWithValues<26>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[5];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<26> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV), clos) {};
        MeltFrame_meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV() //the constructor fromline 1642
            : Melt_CallFrameWithValues<26> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<26> (fil,lin, sizeof(MeltFrame_meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV)) {};
        MeltFrame_meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<26> (fil,lin, sizeof(MeltFrame_meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV


    /** end of frame for meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV fromline 1697**/

    /* end of frame for routine meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV fromline 1756 */

    /* classy proc frame meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV */ MeltFrame_meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJPUTCLOSEDV", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:4860:/ getarg");
    /*_.OPCLOV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DECLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[2].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4861:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OPCLOV__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJPUTCLOSEDV*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:4861:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4861:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check opclov";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4861;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OPCLOV__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4861:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4861:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:4862:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPCLOV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.OLOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4863:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPCLOV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OPCLOV_CLOS");
            /*_.OCLOS__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4864:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPCLOV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OPCLOV_OFF");
            /*_.OOFF__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4865:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPCLOV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OPCLOV_CVAL");
            /*_.OCVAL__V12*/ meltfptr[11] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4867:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "putclosedv";
            /*_.OUTPUT_LOCATION__V13*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:73:/ cond");
        /*cond*/ if ( (melt_module_var_fetch (meltmodvarix_FLAGVAL_OUTPUT_DESCR_COMMENT_symb))) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*^apply*/
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OPCLOV__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V14*/ meltfptr[13] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V15*/ meltfptr[14];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V15*/ meltfptr[14] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V14*/ meltfptr[13] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4869:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*putclosv*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4870:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4871:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "melt_assertmsg(\"putclosv checkclo\", melt_magic_discr((melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4872:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V16*/ meltfptr[14] = meltgc_send((melt_ptr_t)(/*_.OCLOS__V10*/ meltfptr[9]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4873:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ")) == MELTOBMAG_CLOSURE);"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4874:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4875:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "melt_assertmsg(\"putclosv checkoff\", "));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4876:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V17*/ meltfptr[16] = meltgc_send((melt_ptr_t)(/*_.OOFF__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4877:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ">= 0 && "));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4878:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V18*/ meltfptr[17] = meltgc_send((melt_ptr_t)(/*_.OOFF__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4879:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "< melt_closure_size((melt_ptr_t) ("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4880:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V19*/ meltfptr[18] = meltgc_send((melt_ptr_t)(/*_.OCLOS__V10*/ meltfptr[9]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4881:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ")));"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4882:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4883:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "((meltclosure_ptr_t)"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4884:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V20*/ meltfptr[19] = meltgc_send((melt_ptr_t)(/*_.OCLOS__V10*/ meltfptr[9]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4885:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ")->tabval["));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4886:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V21*/ meltfptr[20] = meltgc_send((melt_ptr_t)(/*_.OOFF__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4887:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "] = (melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4888:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V22*/ meltfptr[21] = meltgc_send((melt_ptr_t)(/*_.OCVAL__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4889:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4890:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4891:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L3*/ meltfnum[1] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:4892:/ cond");
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[5])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[6])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[5])) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                        /*_.REFERENCED_VALUE__V24*/ meltfptr[23] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V24*/ meltfptr[23] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L4*/ meltfnum[3] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V24*/ meltfptr[23])));;
            /*^compute*/
            /*_#ltI__L5*/ meltfnum[4] =
                ((/*_#STRBUF_USEDLENGTH__L3*/ meltfnum[1]) < (/*_#GET_INT__L4*/ meltfnum[3]));;
            MELT_LOCATION("warmelt-outobj.melt:4891:/ cond");
            /*cond*/ if (/*_#ltI__L5*/ meltfnum[4]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V25*/ meltfptr[24] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4891:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check limited implbuf";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4891;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V26*/ meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V25*/ meltfptr[24] = /*_.MELT_ASSERT_FAILURE_FUN__V26*/ meltfptr[25];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4891:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V26*/ meltfptr[25] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V23*/ meltfptr[22] = /*_._IFELSE___V25*/ meltfptr[24];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4891:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V24*/ meltfptr[23] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L4*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L5*/ meltfnum[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V25*/ meltfptr[24] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V23*/ meltfptr[22] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V23*/ meltfptr[22];;

        MELT_LOCATION("warmelt-outobj.melt:4862:/ clear");
        /*clear*/ /*_.OLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OCLOS__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OOFF__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OCVAL__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V16*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V17*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V18*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V20*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V21*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V22*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V23*/ meltfptr[22] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4860:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:4860:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V5*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V8*/ meltfptr[6] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJPUTCLOSEDV", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_104_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDV*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV fromline 1752 */

    /** start of frame for meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV// fromline 1568
        : public Melt_CallFrameWithValues<27>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[5];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<27> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV), clos) {};
        MeltFrame_meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV() //the constructor fromline 1642
            : Melt_CallFrameWithValues<27> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<27> (fil,lin, sizeof(MeltFrame_meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV)) {};
        MeltFrame_meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<27> (fil,lin, sizeof(MeltFrame_meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV


    /** end of frame for meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV fromline 1697**/

    /* end of frame for routine meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV fromline 1756 */

    /* classy proc frame meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV */ MeltFrame_meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJPUTCLOSEDNOTNULLV", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:4898:/ getarg");
    /*_.OPCLOV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DECLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[2].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4899:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OPCLOV__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJPUTCLOSEDNOTNULLV*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:4899:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4899:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check opclov";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4899;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OPCLOV__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4899:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4899:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:4900:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPCLOV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.OLOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4901:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPCLOV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OPCLOV_CLOS");
            /*_.OCLOS__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4902:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPCLOV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OPCLOV_OFF");
            /*_.OOFF__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4903:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPCLOV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OPCLOV_CVAL");
            /*_.OCVAL__V12*/ meltfptr[11] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4904:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "putclosednotnullv";
            /*_.OUTPUT_LOCATION__V13*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:73:/ cond");
        /*cond*/ if ( (melt_module_var_fetch (meltmodvarix_FLAGVAL_OUTPUT_DESCR_COMMENT_symb))) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*^apply*/
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OPCLOV__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V14*/ meltfptr[13] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V15*/ meltfptr[14];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V15*/ meltfptr[14] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V14*/ meltfptr[13] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4906:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*putclosvnotnull*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4907:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4908:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "melt_assertmsg(\"putclosvnotnull checkclo\", melt_magic_discr((melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4909:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V16*/ meltfptr[14] = meltgc_send((melt_ptr_t)(/*_.OCLOS__V10*/ meltfptr[9]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4910:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ")) == MELTOBMAG_CLOSURE);"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4911:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4912:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "melt_assertmsg(\"putclosvnotnull checknotnullval\", NULL != "));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4913:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V17*/ meltfptr[16] = meltgc_send((melt_ptr_t)(/*_.OCVAL__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4914:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4915:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4916:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "melt_assertmsg(\"putclosvnotnull checkoff\", "));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4917:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V18*/ meltfptr[17] = meltgc_send((melt_ptr_t)(/*_.OOFF__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4918:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ">= 0 && "));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4919:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V19*/ meltfptr[18] = meltgc_send((melt_ptr_t)(/*_.OOFF__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4920:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "< melt_closure_size((melt_ptr_t) ("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4921:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V20*/ meltfptr[19] = meltgc_send((melt_ptr_t)(/*_.OCLOS__V10*/ meltfptr[9]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4922:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ")));"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4923:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4924:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "((meltclosure_ptr_t)"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4925:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V21*/ meltfptr[20] = meltgc_send((melt_ptr_t)(/*_.OCLOS__V10*/ meltfptr[9]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4926:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ")->tabval["));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4927:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V22*/ meltfptr[21] = meltgc_send((melt_ptr_t)(/*_.OOFF__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4928:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "] = (melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4929:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V23*/ meltfptr[22] = meltgc_send((melt_ptr_t)(/*_.OCVAL__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4930:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4931:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4932:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L3*/ meltfnum[1] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:4933:/ cond");
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[5])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[6])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[5])) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                        /*_.REFERENCED_VALUE__V25*/ meltfptr[24] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V25*/ meltfptr[24] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L4*/ meltfnum[3] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V25*/ meltfptr[24])));;
            /*^compute*/
            /*_#ltI__L5*/ meltfnum[4] =
                ((/*_#STRBUF_USEDLENGTH__L3*/ meltfnum[1]) < (/*_#GET_INT__L4*/ meltfnum[3]));;
            MELT_LOCATION("warmelt-outobj.melt:4932:/ cond");
            /*cond*/ if (/*_#ltI__L5*/ meltfnum[4]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V26*/ meltfptr[25] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4932:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check limited implbuf";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4932;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V26*/ meltfptr[25] = /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4932:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V24*/ meltfptr[23] = /*_._IFELSE___V26*/ meltfptr[25];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4932:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V25*/ meltfptr[24] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L4*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L5*/ meltfnum[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V26*/ meltfptr[25] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V24*/ meltfptr[23] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V24*/ meltfptr[23];;

        MELT_LOCATION("warmelt-outobj.melt:4900:/ clear");
        /*clear*/ /*_.OLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OCLOS__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OOFF__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OCVAL__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V16*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V17*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V18*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V20*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V21*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V22*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V23*/ meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V24*/ meltfptr[23] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4898:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:4898:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V5*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V8*/ meltfptr[6] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJPUTCLOSEDNOTNULLV", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_105_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTCLOSEDNOTNULLV*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST fromline 1752 */

    /** start of frame for meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST// fromline 1568
        : public Melt_CallFrameWithValues<27>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[7];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<27> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST), clos) {};
        MeltFrame_meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<27> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<27> (fil,lin, sizeof(MeltFrame_meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST)) {};
        MeltFrame_meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<27> (fil,lin, sizeof(MeltFrame_meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST


    /** end of frame for meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST fromline 1697**/

    /* end of frame for routine meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST fromline 1756 */

    /* classy proc frame meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST */ MeltFrame_meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJPUTROUTCONST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:4939:/ getarg");
    /*_.OPRCONST__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DECLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[2].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4940:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OPRCONST__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJPUTROUTCONST*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:4940:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4940:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check oprconst";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4940;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OPRCONST__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4940:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4940:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:4941:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPRCONST__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.OLOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4942:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPRCONST__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OPRCONST_ROUT");
            /*_.OROUT__V10*/ meltfptr[9] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_A__L3*/ meltfnum[1] =
            melt_is_instance_of((melt_ptr_t)(/*_.OROUT__V10*/ meltfptr[9]), (melt_ptr_t)((/*!CLASS_OBJINITROUTINE*/ meltfrout->tabval[2])));;
        MELT_LOCATION("warmelt-outobj.melt:4943:/ cond");
        /*cond*/ if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.OROUT__V10*/ meltfptr[9]) /*=obj*/;
                        melt_object_get_field(slot,obj, 1, "OIE_CNAME");
                        /*_.OIE_CNAME__V12*/ meltfptr[11] = slot;
                    };
                    ;
                    /*_.OROUTNAM__V11*/ meltfptr[10] = /*_.OIE_CNAME__V12*/ meltfptr[11];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:4943:/ clear");
                    /*clear*/ /*_.OIE_CNAME__V12*/ meltfptr[11] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OROUTNAM__V11*/ meltfptr[10] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:4944:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPRCONST__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OPRCONST_OFF");
            /*_.OOFF__V13*/ meltfptr[11] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4945:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPRCONST__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OPRCONST_CVAL");
            /*_.OCVAL__V14*/ meltfptr[13] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4946:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "putroutconst";
            /*_.OUTPUT_LOCATION__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:73:/ cond");
        /*cond*/ if ( (melt_module_var_fetch (meltmodvarix_FLAGVAL_OUTPUT_DESCR_COMMENT_symb))) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*^apply*/
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OPRCONST__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V17*/ meltfptr[16] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V16*/ meltfptr[15] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V17*/ meltfptr[16];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V17*/ meltfptr[16] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V16*/ meltfptr[15] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4948:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*putroutconst*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4949:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4950:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "if (MELT_HAS_INITIAL_ENVIRONMENT) melt_assertmsg(\"putroutconst checkrout\
\", melt_magic_discr((melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4951:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V18*/ meltfptr[16] = meltgc_send((melt_ptr_t)(/*_.OROUT__V10*/ meltfptr[9]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4952:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ")) == MELTOBMAG_ROUTINE);"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4953:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4955:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "if (MELT_HAS_INITIAL_ENVIRONMENT) melt_checkmsg(\"putroutconst constnull."));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_STRING__L4*/ meltfnum[3] =
            (melt_magic_discr((melt_ptr_t)(/*_.OROUTNAM__V11*/ meltfptr[10])) == MELTOBMAG_STRING);;
        MELT_LOCATION("warmelt-outobj.melt:4956:/ cond");
        /*cond*/ if (/*_#IS_STRING__L4*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        /*^locexp*/
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                              melt_string_str((melt_ptr_t)(/*_.OROUTNAM__V11*/ meltfptr[10])));
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4957:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "#"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4958:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V19*/ meltfptr[18] = meltgc_send((melt_ptr_t)(/*_.OOFF__V13*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4959:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "\", NULL != ("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4960:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V20*/ meltfptr[19] = meltgc_send((melt_ptr_t)(/*_.OCVAL__V14*/ meltfptr[13]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4961:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "));"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4962:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4964:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "((meltroutine_ptr_t)"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4965:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V21*/ meltfptr[20] = meltgc_send((melt_ptr_t)(/*_.OROUT__V10*/ meltfptr[9]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4966:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ")->tabval["));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4967:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V22*/ meltfptr[21] = meltgc_send((melt_ptr_t)(/*_.OOFF__V13*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4968:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "] = (melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4969:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V23*/ meltfptr[22] = meltgc_send((melt_ptr_t)(/*_.OCVAL__V14*/ meltfptr[13]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4970:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4971:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4972:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L5*/ meltfnum[4] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:4973:/ cond");
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[6])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[7])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[6])) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                        /*_.REFERENCED_VALUE__V25*/ meltfptr[24] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V25*/ meltfptr[24] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L6*/ meltfnum[5] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V25*/ meltfptr[24])));;
            /*^compute*/
            /*_#ltI__L7*/ meltfnum[6] =
                ((/*_#STRBUF_USEDLENGTH__L5*/ meltfnum[4]) < (/*_#GET_INT__L6*/ meltfnum[5]));;
            MELT_LOCATION("warmelt-outobj.melt:4972:/ cond");
            /*cond*/ if (/*_#ltI__L7*/ meltfnum[6]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V26*/ meltfptr[25] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4972:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check limited implbuf";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4972;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V26*/ meltfptr[25] = /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4972:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V24*/ meltfptr[23] = /*_._IFELSE___V26*/ meltfptr[25];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4972:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L5*/ meltfnum[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V25*/ meltfptr[24] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L6*/ meltfnum[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L7*/ meltfnum[6] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V26*/ meltfptr[25] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V24*/ meltfptr[23] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V24*/ meltfptr[23];;

        MELT_LOCATION("warmelt-outobj.melt:4941:/ clear");
        /*clear*/ /*_.OLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OROUT__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OROUTNAM__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OOFF__V13*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OCVAL__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V16*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V18*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_STRING__L4*/ meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V20*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V21*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V22*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V23*/ meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V24*/ meltfptr[23] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4939:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:4939:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V5*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V8*/ meltfptr[6] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJPUTROUTCONST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_106_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONST*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL fromline 1752 */

    /** start of frame for meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL// fromline 1568
        : public Melt_CallFrameWithValues<27>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[6];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<27> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL), clos) {};
        MeltFrame_meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL() //the constructor fromline 1642
            : Melt_CallFrameWithValues<27> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<27> (fil,lin, sizeof(MeltFrame_meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL)) {};
        MeltFrame_meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<27> (fil,lin, sizeof(MeltFrame_meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL


    /** end of frame for meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL fromline 1697**/

    /* end of frame for routine meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL fromline 1756 */

    /* classy proc frame meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL */ MeltFrame_meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJPUTROUTCONSTNOTNULL", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:4979:/ getarg");
    /*_.OPRCONST__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DECLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[2].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4980:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OPRCONST__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJPUTROUTCONSTNOTNULL*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:4980:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4980:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check oprconst";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4980;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OPRCONST__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4980:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4980:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:4981:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPRCONST__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.OLOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4982:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPRCONST__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OPRCONST_ROUT");
            /*_.OROUT__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4983:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPRCONST__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OPRCONST_OFF");
            /*_.OOFF__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4984:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPRCONST__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OPRCONST_CVAL");
            /*_.OCVAL__V12*/ meltfptr[11] = slot;
        };
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4986:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#NOTNULL__L3*/ meltfnum[1] =
                ((/*_.OCVAL__V12*/ meltfptr[11]) != NULL);;
            MELT_LOCATION("warmelt-outobj.melt:4986:/ cond");
            /*cond*/ if (/*_#NOTNULL__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V14*/ meltfptr[13] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4986:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[3];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check notnull ocval";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4986;
                            /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V14*/ meltfptr[13] = /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4986:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V13*/ meltfptr[12] = /*_._IFELSE___V14*/ meltfptr[13];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4986:/ clear");
            /*clear*/ /*_#NOTNULL__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V14*/ meltfptr[13] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V13*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4987:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "putroutconstnotnull";
            /*_.OUTPUT_LOCATION__V16*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:73:/ cond");
        /*cond*/ if ( (melt_module_var_fetch (meltmodvarix_FLAGVAL_OUTPUT_DESCR_COMMENT_symb))) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*^apply*/
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OPRCONST__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V18*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V17*/ meltfptr[13] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V18*/ meltfptr[17];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V18*/ meltfptr[17] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V17*/ meltfptr[13] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4989:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*putroutconstnotnull*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4990:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4991:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "melt_assertmsg(\"putroutconstnotnull checkrout\", melt_magic_discr\
((melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4992:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V19*/ meltfptr[17] = meltgc_send((melt_ptr_t)(/*_.OROUT__V10*/ meltfptr[9]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4993:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ")) == MELTOBMAG_ROUTINE);"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4994:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4995:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "melt_assertmsg(\"putroutconstnotnull notnullconst\", NULL != "));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4996:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V20*/ meltfptr[19] = meltgc_send((melt_ptr_t)(/*_.OCVAL__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4997:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4998:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4999:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "((meltroutine_ptr_t)"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5000:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V21*/ meltfptr[20] = meltgc_send((melt_ptr_t)(/*_.OROUT__V10*/ meltfptr[9]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5001:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ")->tabval["));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5002:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V22*/ meltfptr[21] = meltgc_send((melt_ptr_t)(/*_.OOFF__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5003:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "] = (melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5004:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V23*/ meltfptr[22] = meltgc_send((melt_ptr_t)(/*_.OCVAL__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5005:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5006:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:5007:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L4*/ meltfnum[1] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:5008:/ cond");
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[5])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[6])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[5])) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                        /*_.REFERENCED_VALUE__V25*/ meltfptr[24] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V25*/ meltfptr[24] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L5*/ meltfnum[4] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V25*/ meltfptr[24])));;
            /*^compute*/
            /*_#ltI__L6*/ meltfnum[5] =
                ((/*_#STRBUF_USEDLENGTH__L4*/ meltfnum[1]) < (/*_#GET_INT__L5*/ meltfnum[4]));;
            MELT_LOCATION("warmelt-outobj.melt:5007:/ cond");
            /*cond*/ if (/*_#ltI__L6*/ meltfnum[5]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V26*/ meltfptr[25] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:5007:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check limited implbuf";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5007;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V26*/ meltfptr[25] = /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5007:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V24*/ meltfptr[23] = /*_._IFELSE___V26*/ meltfptr[25];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:5007:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L4*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V25*/ meltfptr[24] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L5*/ meltfnum[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L6*/ meltfnum[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V26*/ meltfptr[25] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V24*/ meltfptr[23] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V24*/ meltfptr[23];;

        MELT_LOCATION("warmelt-outobj.melt:4981:/ clear");
        /*clear*/ /*_.OLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OROUT__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OOFF__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OCVAL__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V16*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V17*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V19*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V20*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V21*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V22*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V23*/ meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V24*/ meltfptr[23] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4979:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:4979:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V5*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V8*/ meltfptr[6] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJPUTROUTCONSTNOTNULL", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_107_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTROUTCONSTNOTNULL*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST fromline 1752 */

    /** start of frame for meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST// fromline 1568
        : public Melt_CallFrameWithValues<36>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[8];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<36> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST), clos) {};
        MeltFrame_meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<36> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<36> (fil,lin, sizeof(MeltFrame_meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST)) {};
        MeltFrame_meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<36> (fil,lin, sizeof(MeltFrame_meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST


    /** end of frame for meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST fromline 1697**/

    /* end of frame for routine meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST fromline 1756 */

    /* classy proc frame meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST */ MeltFrame_meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJPUTHOOKCONST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:5014:/ getarg");
    /*_.OPHCONST__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DECLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[2].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:5015:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L2*/ meltfnum[1] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:5015:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L3*/ meltfnum[2] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:5015:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5015;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objputhookconst ophconst=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OPHCONST__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5015:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L3*/ meltfnum[2] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V5*/ meltfptr[4] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:5015:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V5*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*void*/(void)0;
            }
            ;
            /*epilog*/
        }

#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:5016:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.OPHCONST__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJPUTHOOKCONST*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:5016:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:5016:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check ophconst";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5016;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OPHCONST__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5016:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:5016:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:5017:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OPHCONST__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJINSTR*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OPHCONST__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 0, "OBI_LOC");
                    /*_.OLOC__V10*/ meltfptr[8] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OLOC__V10*/ meltfptr[8] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:5018:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OPHCONST__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJPUTHOOKCONST*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OPHCONST__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "OPHCONST_HOOK");
                    /*_.OHOOK__V11*/ meltfptr[4] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OHOOK__V11*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:5019:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OPHCONST__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJPUTHOOKCONST*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OPHCONST__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "OPHCONST_OFF");
                    /*_.OHOFF__V12*/ meltfptr[11] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OHOFF__V12*/ meltfptr[11] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:5020:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OPHCONST__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJPUTHOOKCONST*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OPHCONST__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 3, "OPHCONST_CVAL");
                    /*_.OVAL__V13*/ meltfptr[12] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OVAL__V13*/ meltfptr[12] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:5021:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OHOOK__V11*/ meltfptr[4]),
                                          (melt_ptr_t)((/*!CLASS_OBJINITELEM*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OHOOK__V11*/ meltfptr[4]) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "OIE_DATA");
                    /*_.NHOOK__V14*/ meltfptr[13] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.NHOOK__V14*/ meltfptr[13] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:5022:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.NHOOK__V14*/ meltfptr[13]),
                                          (melt_ptr_t)((/*!CLASS_NREP_BOUND_DATA*/ meltfrout->tabval[5])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.NHOOK__V14*/ meltfptr[13]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "NDATA_NAME");
                    /*_.HOOKNAME__V15*/ meltfptr[14] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.HOOKNAME__V15*/ meltfptr[14] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:5024:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L5*/ meltfnum[1] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:5024:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L6*/ meltfnum[2] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:5024:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[11];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L6*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5024;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objputhookconst ohook=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OHOOK__V11*/ meltfptr[4];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n.. ohoff=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.OHOFF__V12*/ meltfptr[11];
                            /*^apply.arg*/
                            argtab[7].meltbp_cstring =  "\n.. oval=";
                            /*^apply.arg*/
                            argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.OVAL__V13*/ meltfptr[12];
                            /*^apply.arg*/
                            argtab[9].meltbp_cstring =  "\n.. hookname=";
                            /*^apply.arg*/
                            argtab[10].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V15*/ meltfptr[14];
                            /*_.MELT_DEBUG_FUN__V17*/ meltfptr[16] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V16*/ meltfptr[15] = /*_.MELT_DEBUG_FUN__V17*/ meltfptr[16];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5024:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L6*/ meltfnum[2] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V17*/ meltfptr[16] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V16*/ meltfptr[15] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:5024:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L5*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V16*/ meltfptr[15] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*void*/(void)0;
            }
            ;
            /*epilog*/
        }

#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5025:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "puthookconst";
            /*_.OUTPUT_LOCATION__V18*/ meltfptr[16] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.OLOC__V10*/ meltfptr[8]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:73:/ cond");
        /*cond*/ if ( (melt_module_var_fetch (meltmodvarix_FLAGVAL_OUTPUT_DESCR_COMMENT_symb))) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*^apply*/
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OPHCONST__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V20*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[7])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V19*/ meltfptr[15] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V20*/ meltfptr[19];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V20*/ meltfptr[19] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V19*/ meltfptr[15] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5027:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "/*put inside hook ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V15*/ meltfptr[14];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " */";
            /*_.ADD2OUT__V21*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5028:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5029:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "melt_assertmsg(\"puthookconst checkhook ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V15*/ meltfptr[14];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "\", melt_magic_discr((melt_ptr_t)(";
            /*_.ADD2OUT__V22*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5030:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V23*/ meltfptr[22] = meltgc_send((melt_ptr_t)(/*_.OHOOK__V11*/ meltfptr[4]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5031:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  ")) == MELTOBMAG_HOOK);";
            /*_.ADD2OUT__V24*/ meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5032:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5033:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "melt_assertmsg(\"puthookconst checkhook ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V15*/ meltfptr[14];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "\",";
            /*_.ADD2OUT__V25*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_#plI__L7*/ meltfnum[2] =
            ((10) + (/*_#DEPTH__L1*/ meltfnum[0]));;

        {
            MELT_LOCATION("warmelt-outobj.melt:5035:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L7*/ meltfnum[2]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5036:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "(";
            /*_.ADD2OUT__V26*/ meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5037:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V27*/ meltfptr[26] = meltgc_send((melt_ptr_t)(/*_.OHOFF__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5038:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  ") >= 0";
            /*_.ADD2OUT__V28*/ meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_#plI__L8*/ meltfnum[1] =
            ((12) + (/*_#DEPTH__L1*/ meltfnum[0]));;

        {
            MELT_LOCATION("warmelt-outobj.melt:5039:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L8*/ meltfnum[1]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5040:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "&& (";
            /*_.ADD2OUT__V29*/ meltfptr[28] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5041:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V30*/ meltfptr[29] = meltgc_send((melt_ptr_t)(/*_.OHOFF__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5042:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  ") < melt_hook_size ((melt_ptr_t)";
            /*_.ADD2OUT__V31*/ meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5043:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V32*/ meltfptr[31] = meltgc_send((melt_ptr_t)(/*_.OHOOK__V11*/ meltfptr[4]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5044:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "));";
            /*_.ADD2OUT__V33*/ meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5045:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5046:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "((melthook_ptr_t)"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5047:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V34*/ meltfptr[33] = meltgc_send((melt_ptr_t)(/*_.OHOOK__V11*/ meltfptr[4]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5048:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ")->tabval["));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5049:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V35*/ meltfptr[34] = meltgc_send((melt_ptr_t)(/*_.OHOFF__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5050:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "] = (melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5051:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V36*/ meltfptr[35] = meltgc_send((melt_ptr_t)(/*_.OVAL__V13*/ meltfptr[12]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5052:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5053:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_LOCATION("warmelt-outobj.melt:5017:/ clear");
        /*clear*/ /*_.OLOC__V10*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OHOOK__V11*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OHOFF__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OVAL__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NHOOK__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOKNAME__V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V18*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V19*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V21*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V22*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V23*/ meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V24*/ meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V25*/ meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L7*/ meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V26*/ meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V27*/ meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V28*/ meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L8*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V29*/ meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V30*/ meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V31*/ meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V32*/ meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V33*/ meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V34*/ meltfptr[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V35*/ meltfptr[34] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V36*/ meltfptr[35] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:5014:/ clear");
        /*clear*/ /*_.IFCPP___V7*/ meltfptr[5] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJPUTHOOKCONST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_108_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKCONST*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA fromline 1752 */

    /** start of frame for meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA// fromline 1568
        : public Melt_CallFrameWithValues<28>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[7];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<28> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA), clos) {};
        MeltFrame_meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA() //the constructor fromline 1642
            : Melt_CallFrameWithValues<28> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<28> (fil,lin, sizeof(MeltFrame_meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA)) {};
        MeltFrame_meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<28> (fil,lin, sizeof(MeltFrame_meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA


    /** end of frame for meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA fromline 1697**/

    /* end of frame for routine meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA fromline 1756 */

    /* classy proc frame meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA */ MeltFrame_meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJPUTHOOKDATA", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:5059:/ getarg");
    /*_.OPHD__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DECLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[2].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:5060:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L2*/ meltfnum[1] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:5060:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L3*/ meltfnum[2] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:5060:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5060;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objputhookdata ophd=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OPHD__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5060:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L3*/ meltfnum[2] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V5*/ meltfptr[4] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:5060:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V5*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*void*/(void)0;
            }
            ;
            /*epilog*/
        }

#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:5061:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.OPHD__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJPUTHOOKDATA*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:5061:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:5061:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check ophconst";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5061;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OPHD__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5061:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:5061:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:5062:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OPHD__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJINSTR*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OPHD__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 0, "OBI_LOC");
                    /*_.OLOC__V10*/ meltfptr[8] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OLOC__V10*/ meltfptr[8] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:5063:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OPHD__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJPUTHOOKDATA*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OPHD__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "OPHKDATA_HOOK");
                    /*_.OHOOK__V11*/ meltfptr[4] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OHOOK__V11*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:5064:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OPHD__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJPUTHOOKDATA*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OPHD__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "OPHKDATA_DATA");
                    /*_.ODATA__V12*/ meltfptr[11] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ODATA__V12*/ meltfptr[11] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:5065:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OHOOK__V11*/ meltfptr[4]),
                                          (melt_ptr_t)((/*!CLASS_OBJINITELEM*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OHOOK__V11*/ meltfptr[4]) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "OIE_DATA");
                    /*_.NHOOK__V13*/ meltfptr[12] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.NHOOK__V13*/ meltfptr[12] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:5066:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.NHOOK__V13*/ meltfptr[12]),
                                          (melt_ptr_t)((/*!CLASS_NREP_BOUND_DATA*/ meltfrout->tabval[5])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.NHOOK__V13*/ meltfptr[12]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "NDATA_NAME");
                    /*_.HOOKNAME__V14*/ meltfptr[13] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.HOOKNAME__V14*/ meltfptr[13] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:5068:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L5*/ meltfnum[1] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:5068:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L6*/ meltfnum[2] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:5068:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[11];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L6*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5068;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objputhookdata ophd=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OPHD__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n.. ohook=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.OHOOK__V11*/ meltfptr[4];
                            /*^apply.arg*/
                            argtab[7].meltbp_cstring =  "\n.. odata=";
                            /*^apply.arg*/
                            argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.ODATA__V12*/ meltfptr[11];
                            /*^apply.arg*/
                            argtab[9].meltbp_cstring =  "\n nhook=";
                            /*^apply.arg*/
                            argtab[10].meltbp_aptr = (melt_ptr_t*) &/*_.NHOOK__V13*/ meltfptr[12];
                            /*_.MELT_DEBUG_FUN__V16*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V15*/ meltfptr[14] = /*_.MELT_DEBUG_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5068:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L6*/ meltfnum[2] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V16*/ meltfptr[15] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V15*/ meltfptr[14] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:5068:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L5*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V15*/ meltfptr[14] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*void*/(void)0;
            }
            ;
            /*epilog*/
        }

#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:5069:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L7*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.OHOOK__V11*/ meltfptr[4]), (melt_ptr_t)((/*!CLASS_OBJINITHOOK*/ meltfrout->tabval[6])));;
            MELT_LOCATION("warmelt-outobj.melt:5069:/ cond");
            /*cond*/ if (/*_#IS_A__L7*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V18*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:5069:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check ohook";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5069;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OHOOK__V11*/ meltfptr[4];
                            /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V18*/ meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5069:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V17*/ meltfptr[15] = /*_._IFELSE___V18*/ meltfptr[14];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:5069:/ clear");
            /*clear*/ /*_#IS_A__L7*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V18*/ meltfptr[14] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V17*/ meltfptr[15] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5070:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "puthookdata";
            /*_.OUTPUT_LOCATION__V20*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[7])), (melt_ptr_t)(/*_.OLOC__V10*/ meltfptr[8]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:73:/ cond");
        /*cond*/ if ( (melt_module_var_fetch (meltmodvarix_FLAGVAL_OUTPUT_DESCR_COMMENT_symb))) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*^apply*/
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OPHD__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V22*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V21*/ meltfptr[14] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V22*/ meltfptr[21];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V22*/ meltfptr[21] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V21*/ meltfptr[14] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5072:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "/*put data in hook ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V14*/ meltfptr[13];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " */";
            /*_.ADD2OUT__V23*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5073:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5074:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "melt_assertmsg(\"puthookdata checkhook ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V14*/ meltfptr[13];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "\", melt_magic_discr((melt_ptr_t)(";
            /*_.ADD2OUT__V24*/ meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5075:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V25*/ meltfptr[24] = meltgc_send((melt_ptr_t)(/*_.OHOOK__V11*/ meltfptr[4]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[10])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5076:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  ")) == MELTOBMAG_HOOK);";
            /*_.ADD2OUT__V26*/ meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5077:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5078:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "((melthook_ptr_t)"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5079:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V27*/ meltfptr[26] = meltgc_send((melt_ptr_t)(/*_.OHOOK__V11*/ meltfptr[4]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[10])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5080:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ")->hookdata = (melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5081:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V28*/ meltfptr[27] = meltgc_send((melt_ptr_t)(/*_.ODATA__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[10])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5082:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5083:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_LOCATION("warmelt-outobj.melt:5062:/ clear");
        /*clear*/ /*_.OLOC__V10*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OHOOK__V11*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ODATA__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NHOOK__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HOOKNAME__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V17*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V20*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V21*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V23*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V24*/ meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V25*/ meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V26*/ meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V27*/ meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V28*/ meltfptr[27] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:5059:/ clear");
        /*clear*/ /*_.IFCPP___V7*/ meltfptr[5] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJPUTHOOKDATA", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_109_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTHOOKDATA*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT fromline 1752 */

    /** start of frame for meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT// fromline 1568
        : public Melt_CallFrameWithValues<34>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[14];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<34> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT), clos) {};
        MeltFrame_meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT() //the constructor fromline 1642
            : Melt_CallFrameWithValues<34> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<34> (fil,lin, sizeof(MeltFrame_meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT)) {};
        MeltFrame_meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<34> (fil,lin, sizeof(MeltFrame_meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT


    /** end of frame for meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT fromline 1697**/

    /* end of frame for routine meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT fromline 1756 */

    /* classy proc frame meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT */ MeltFrame_meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJPUTXTRARESULT", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:5089:/ getarg");
    /*_.OPUTX__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DECLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[2].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:5090:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OPUTX__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJPUTXTRARESULT*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:5090:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:5090:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check oputx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5090;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OPUTX__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5090:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:5090:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:5091:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPUTX__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.OLOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:5092:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPUTX__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBXRES_RANK");
            /*_.ORANK__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:5093:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPUTX__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBXRES_OBLOC");
            /*_.OVLOC__V11*/ meltfptr[10] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5094:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*_.OCTYP__V12*/ meltfptr[11] = meltgc_send((melt_ptr_t)(/*_.OVLOC__V11*/ meltfptr[10]), (melt_ptr_t)((/*!GET_CTYPE*/ meltfrout->tabval[2])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5096:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "putxtraresult";
            /*_.OUTPUT_LOCATION__V13*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:73:/ cond");
        /*cond*/ if ( (melt_module_var_fetch (meltmodvarix_FLAGVAL_OUTPUT_DESCR_COMMENT_symb))) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*^apply*/
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OPUTX__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V14*/ meltfptr[13] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V15*/ meltfptr[14];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V15*/ meltfptr[14] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V14*/ meltfptr[13] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:5098:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OCTYP__V12*/ meltfptr[11]), (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[5])));;
            MELT_LOCATION("warmelt-outobj.melt:5098:/ cond");
            /*cond*/ if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V17*/ meltfptr[16] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:5098:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check octyp";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5098;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OCTYP__V12*/ meltfptr[11];
                            /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V17*/ meltfptr[16] = /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5098:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V16*/ meltfptr[14] = /*_._IFELSE___V17*/ meltfptr[16];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:5098:/ clear");
            /*clear*/ /*_#IS_A__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V17*/ meltfptr[16] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V16*/ meltfptr[14] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:5099:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_INTEGERBOX__L4*/ meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.ORANK__V10*/ meltfptr[9])) == MELTOBMAG_INT);;
            MELT_LOCATION("warmelt-outobj.melt:5099:/ cond");
            /*cond*/ if (/*_#IS_INTEGERBOX__L4*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V20*/ meltfptr[16] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:5099:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check orank";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5099;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ORANK__V10*/ meltfptr[9];
                            /*_.MELT_ASSERT_FAILURE_FUN__V21*/ meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V20*/ meltfptr[16] = /*_.MELT_ASSERT_FAILURE_FUN__V21*/ meltfptr[20];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5099:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V21*/ meltfptr[20] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V19*/ meltfptr[17] = /*_._IFELSE___V20*/ meltfptr[16];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:5099:/ clear");
            /*clear*/ /*_#IS_INTEGERBOX__L4*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V20*/ meltfptr[16] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V19*/ meltfptr[17] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5100:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5101:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5102:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "if (meltxresdescr_["));
        }
        ;
        /*_#GET_INT__L5*/ meltfnum[1] =
            (melt_get_int((melt_ptr_t)(/*_.ORANK__V10*/ meltfptr[9])));;

        {
            MELT_LOCATION("warmelt-outobj.melt:5103:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L5*/ meltfnum[1]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5104:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "] != "));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:5105:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OCTYP__V12*/ meltfptr[11]) /*=obj*/;
            melt_object_get_field(slot,obj, 5, "CTYPE_PARCHAR");
            /*_.CTYPE_PARCHAR__V22*/ meltfptr[20] = slot;
        };
        ;

        {
            /*^locexp*/
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.CTYPE_PARCHAR__V22*/ meltfptr[20])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5106:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ") goto meltlabend_rout;"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5107:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5108:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "if (meltxrestab_["));
        }
        ;
        /*_#GET_INT__L6*/ meltfnum[5] =
            (melt_get_int((melt_ptr_t)(/*_.ORANK__V10*/ meltfptr[9])));;

        {
            MELT_LOCATION("warmelt-outobj.melt:5109:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L6*/ meltfnum[5]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5110:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "]."));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5111:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OCTYP__V12*/ meltfptr[11]),
                                          (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[5])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OCTYP__V12*/ meltfptr[11]) /*=obj*/;
                    melt_object_get_field(slot,obj, 8, "CTYPE_RESFIELD");
                    /*_.CTYPE_RESFIELD__V23*/ meltfptr[16] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.CTYPE_RESFIELD__V23*/ meltfptr[16] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#IS_STRING__L7*/ meltfnum[6] =
            (melt_magic_discr((melt_ptr_t)(/*_.CTYPE_RESFIELD__V23*/ meltfptr[16])) == MELTOBMAG_STRING);;
        MELT_LOCATION("warmelt-outobj.melt:5111:/ cond");
        /*cond*/ if (/*_#IS_STRING__L7*/ meltfnum[6]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V24*/ meltfptr[23] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:5111:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:5112:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        {
                            /*^locexp*/
                            /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG
                            melt_dbgcounter++;
#endif
                            ;
                        }
                        ;

                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#MELT_NEED_DBG__L8*/ meltfnum[7] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:5112:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L8*/ meltfnum[7]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L9*/ meltfnum[8] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:5112:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[7];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[8];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 5112;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "outpucod_objputxtraresult bad octyp";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OCTYP__V12*/ meltfptr[11];
                                        /*^apply.arg*/
                                        argtab[5].meltbp_cstring =  " oputx=";
                                        /*^apply.arg*/
                                        argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.OPUTX__V2*/ meltfptr[1];
                                        /*_.MELT_DEBUG_FUN__V26*/ meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[6])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V25*/ meltfptr[24] = /*_.MELT_DEBUG_FUN__V26*/ meltfptr[25];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:5112:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L9*/ meltfnum[8] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V26*/ meltfptr[25] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V25*/ meltfptr[24] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:5112:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L8*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V25*/ meltfptr[24] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*^block*/
                    /*anyblock*/
                    {


                        {
                            /*^locexp*/
                            /*void*/(void)0;
                        }
                        ;
                        /*epilog*/
                    }

#endif /*MELT_HAVE_DEBUG*/
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:5113:/ cond");
                    /*cond*/ if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OCTYP__V12*/ meltfptr[11]),
                                                      (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[7])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.OCTYP__V12*/ meltfptr[11]) /*=obj*/;
                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                /*_.NAMED_NAME__V27*/ meltfptr[25] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.NAMED_NAME__V27*/ meltfptr[25] =  /*reallynil*/ NULL ;;
                        }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:5113:/ locexp");
                        melt_error_str((melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), ( "impossible secondary result type"), (melt_ptr_t)(/*_.NAMED_NAME__V27*/ meltfptr[25]));
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:5111:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_.NAMED_NAME__V27*/ meltfptr[25] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:5115:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OCTYP__V12*/ meltfptr[11]) /*=obj*/;
            melt_object_get_field(slot,obj, 8, "CTYPE_RESFIELD");
            /*_.CTYPE_RESFIELD__V28*/ meltfptr[24] = slot;
        };
        ;

        {
            /*^locexp*/
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.CTYPE_RESFIELD__V28*/ meltfptr[24])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5116:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ") *(meltxrestab_["));
        }
        ;
        /*_#GET_INT__L10*/ meltfnum[8] =
            (melt_get_int((melt_ptr_t)(/*_.ORANK__V10*/ meltfptr[9])));;

        {
            MELT_LOCATION("warmelt-outobj.melt:5117:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L10*/ meltfnum[8]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5118:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "]."));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:5119:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OCTYP__V12*/ meltfptr[11]) /*=obj*/;
            melt_object_get_field(slot,obj, 8, "CTYPE_RESFIELD");
            /*_.CTYPE_RESFIELD__V29*/ meltfptr[25] = slot;
        };
        ;

        {
            /*^locexp*/
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.CTYPE_RESFIELD__V29*/ meltfptr[25])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5120:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ") = ("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#eqeq__L11*/ meltfnum[7] =
            ((/*_.OCTYP__V12*/ meltfptr[11]) == ((/*!CTYPE_VALUE*/ meltfrout->tabval[8])));;
        MELT_LOCATION("warmelt-outobj.melt:5121:/ cond");
        /*cond*/ if (/*_#eqeq__L11*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        /*^locexp*/
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "melt_ptr_t) ("));
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5122:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V30*/ meltfptr[29] = meltgc_send((melt_ptr_t)(/*_.OVLOC__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5123:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:5124:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:5125:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L12*/ meltfnum[11] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:5126:/ cond");
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[10])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[11])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[10])) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                        /*_.REFERENCED_VALUE__V32*/ meltfptr[31] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V32*/ meltfptr[31] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L13*/ meltfnum[12] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V32*/ meltfptr[31])));;
            /*^compute*/
            /*_#ltI__L14*/ meltfnum[13] =
                ((/*_#STRBUF_USEDLENGTH__L12*/ meltfnum[11]) < (/*_#GET_INT__L13*/ meltfnum[12]));;
            MELT_LOCATION("warmelt-outobj.melt:5125:/ cond");
            /*cond*/ if (/*_#ltI__L14*/ meltfnum[13]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V33*/ meltfptr[32] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:5125:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check limited implbuf";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5125;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V34*/ meltfptr[33] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V33*/ meltfptr[32] = /*_.MELT_ASSERT_FAILURE_FUN__V34*/ meltfptr[33];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5125:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V34*/ meltfptr[33] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V31*/ meltfptr[30] = /*_._IFELSE___V33*/ meltfptr[32];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:5125:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L12*/ meltfnum[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V32*/ meltfptr[31] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L13*/ meltfnum[12] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L14*/ meltfnum[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V33*/ meltfptr[32] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V31*/ meltfptr[30] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V31*/ meltfptr[30];;

        MELT_LOCATION("warmelt-outobj.melt:5091:/ clear");
        /*clear*/ /*_.OLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ORANK__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OVLOC__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OCTYP__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V16*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V19*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L5*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CTYPE_PARCHAR__V22*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L6*/ meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CTYPE_RESFIELD__V23*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_STRING__L7*/ meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V24*/ meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CTYPE_RESFIELD__V28*/ meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L10*/ meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CTYPE_RESFIELD__V29*/ meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_#eqeq__L11*/ meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V30*/ meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V31*/ meltfptr[30] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5089:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:5089:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V5*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V8*/ meltfptr[6] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJPUTXTRARESULT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_110_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTXTRARESULT*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV fromline 1752 */

    /** start of frame for meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV// fromline 1568
        : public Melt_CallFrameWithValues<20>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[9];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<20> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV), clos) {};
        MeltFrame_meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV() //the constructor fromline 1642
            : Melt_CallFrameWithValues<20> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<20> (fil,lin, sizeof(MeltFrame_meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV)) {};
        MeltFrame_meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<20> (fil,lin, sizeof(MeltFrame_meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV


    /** end of frame for meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV fromline 1697**/

    /* end of frame for routine meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV fromline 1756 */

    /* classy proc frame meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV */ MeltFrame_meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJEXPV", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:5132:/ getarg");
    /*_.OEXP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DECLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[2].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:5133:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OEXP__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJEXPV*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:5133:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:5133:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[3];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check oexp";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5133;
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5133:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:5133:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:5134:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OEXP__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBX_CONT");
            /*_.CONT__V8*/ meltfptr[6] = slot;
        };
        ;
        /*_#plI__L3*/ meltfnum[1] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;
        /*^compute*/
        /*_.BOXDEPTHP1__V9*/ meltfptr[5] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[2])), (/*_#plI__L3*/ meltfnum[1])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:73:/ cond");
        /*cond*/ if ( (melt_module_var_fetch (meltmodvarix_FLAGVAL_OUTPUT_DESCR_COMMENT_symb))) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*^apply*/
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OEXP__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V11*/ meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V10*/ meltfptr[9] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V11*/ meltfptr[10];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V11*/ meltfptr[10] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V10*/ meltfptr[9] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:5138:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_MULTIPLE__L4*/ meltfnum[3] =
                (melt_magic_discr((melt_ptr_t)(/*_.CONT__V8*/ meltfptr[6])) == MELTOBMAG_MULTIPLE);;
            MELT_LOCATION("warmelt-outobj.melt:5138:/ cond");
            /*cond*/ if (/*_#IS_MULTIPLE__L4*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V13*/ meltfptr[12] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:5138:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check cont";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5138;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CONT__V8*/ meltfptr[6];
                            /*_.MELT_ASSERT_FAILURE_FUN__V14*/ meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V13*/ meltfptr[12] = /*_.MELT_ASSERT_FAILURE_FUN__V14*/ meltfptr[13];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5138:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V14*/ meltfptr[13] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V12*/ meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[12];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:5138:/ clear");
            /*clear*/ /*_#IS_MULTIPLE__L4*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V13*/ meltfptr[12] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V12*/ meltfptr[10] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*citerblock FOREACH_IN_MULTIPLE*/
        {
            /* start foreach_in_multiple meltcit1__EACHTUP */
            long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.CONT__V8*/ meltfptr[6]);
            for (/*_#IX__L5*/ meltfnum[3] = 0;
                              (/*_#IX__L5*/ meltfnum[3] >= 0) && (/*_#IX__L5*/ meltfnum[3] <  meltcit1__EACHTUP_ln);
                              /*_#IX__L5*/ meltfnum[3]++)
                {
                    /*_.COMP__V15*/ meltfptr[13] = melt_multiple_nth((melt_ptr_t)(/*_.CONT__V8*/ meltfptr[6]),  /*_#IX__L5*/ meltfnum[3]);



                    /*_#GET_INT__L6*/ meltfnum[5] =
                        (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V9*/ meltfptr[5])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:5142:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#GET_INT__L6*/ meltfnum[5];
                        /*_.OUTPUT_C_CODE__V16*/ meltfptr[12] = meltgc_send((melt_ptr_t)(/*_.COMP__V15*/ meltfptr[13]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:5143:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[6] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                        MELT_LOCATION("warmelt-outobj.melt:5144:/ cond");
                        /*cond*/ if (
                            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[5])),
                                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[6])))
                        ) /*then*/
                            {
                                /*^cond.then*/
                                /*^getslot*/
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[5])) /*=obj*/;
                                    melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                                    /*_.REFERENCED_VALUE__V18*/ meltfptr[17] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V18*/ meltfptr[17] =  /*reallynil*/ NULL ;;
                            }
                        ;
                        /*^compute*/
                        /*_#GET_INT__L8*/ meltfnum[7] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V18*/ meltfptr[17])));;
                        /*^compute*/
                        /*_#ltI__L9*/ meltfnum[8] =
                            ((/*_#STRBUF_USEDLENGTH__L7*/ meltfnum[6]) < (/*_#GET_INT__L8*/ meltfnum[7]));;
                        MELT_LOCATION("warmelt-outobj.melt:5143:/ cond");
                        /*cond*/ if (/*_#ltI__L9*/ meltfnum[8]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V19*/ meltfptr[18] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:5143:/ cond.else");

                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*^apply*/
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[4];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_cstring =  "check limited implbuf";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 5143;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V19*/ meltfptr[18] = /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:5143:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V17*/ meltfptr[16] = /*_._IFELSE___V19*/ meltfptr[18];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5143:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[6] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V18*/ meltfptr[17] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L8*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L9*/ meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V19*/ meltfptr[18] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V17*/ meltfptr[16] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                    if (/*_#IX__L5*/ meltfnum[3]<0) break;
                } /* end  foreach_in_multiple meltcit1__EACHTUP */

            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:5139:/ clear");
            /*clear*/ /*_.COMP__V15*/ meltfptr[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_#IX__L5*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L6*/ meltfnum[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_.OUTPUT_C_CODE__V16*/ meltfptr[12] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V17*/ meltfptr[16] = 0 ;
        } /*endciterblock FOREACH_IN_MULTIPLE*/
        ;

        MELT_LOCATION("warmelt-outobj.melt:5134:/ clear");
        /*clear*/ /*_.CONT__V8*/ meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BOXDEPTHP1__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V12*/ meltfptr[10] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:5132:/ clear");
        /*clear*/ /*_.IFCPP___V5*/ meltfptr[4] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJEXPV", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_111_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPV*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1538

    /* start of frame for routine meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV fromline 1752 */

    /** start of frame for meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV// fromline 1568
        : public Melt_CallFrameWithValues<34>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[17];
        /*classy others*/
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<34> (
#if ENABLE_CHECKING /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1638*/
                sizeof(MeltFrame_meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV), clos) {};
        MeltFrame_meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV() //the constructor fromline 1642
            : Melt_CallFrameWithValues<34> (
#if ENABLE_CHECKING /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1650*/
                sizeof(MeltFrame_meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV)) {};
#if ENABLE_CHECKING /*fromline 1654*/
        MeltFrame_meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<34> (fil,lin, sizeof(MeltFrame_meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV)) {};
        MeltFrame_meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<34> (fil,lin, sizeof(MeltFrame_meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV), clos) {};
#endif /* ENABLE_CHECKING fromline 1666*/

    }; // end  class MeltFrame_meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV


    /** end of frame for meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV fromline 1697**/

    /* end of frame for routine meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV fromline 1756 */

    /* classy proc frame meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV */ MeltFrame_meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV fromline 1785*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1793*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJLOCATEDEXPV", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:5149:/ getarg");
    /*_.OEXP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DECLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[2].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:5150:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L2*/ meltfnum[1] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:5150:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L3*/ meltfnum[2] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:5150:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5150;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objlocatedexpv oexp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OEXP__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5150:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L3*/ meltfnum[2] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V5*/ meltfptr[4] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:5150:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V5*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*void*/(void)0;
            }
            ;
            /*epilog*/
        }

#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:5151:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.OEXP__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJLOCATEDEXPV*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:5151:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:5151:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check oexp";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5151;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OEXP__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5151:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:5151:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:5152:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OEXP__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBX_CONT");
            /*_.CONT__V10*/ meltfptr[8] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:5153:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OEXP__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBCX_LOC");
            /*_.OLOC__V11*/ meltfptr[4] = slot;
        };
        ;
        /*_#DEPTHP1__L5*/ meltfnum[1] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;
        MELT_LOCATION("warmelt-outobj.melt:5155:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OEXP__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBV_TYPE");
            /*_.OTYP__V12*/ meltfptr[11] = slot;
        };
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:5158:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_MULTIPLE_OR_NULL__L6*/ meltfnum[2] =
                ((/*_.CONT__V10*/ meltfptr[8]) == NULL || (melt_unsafe_magic_discr((melt_ptr_t)(/*_.CONT__V10*/ meltfptr[8])) == MELTOBMAG_MULTIPLE));;
            MELT_LOCATION("warmelt-outobj.melt:5158:/ cond");
            /*cond*/ if (/*_#IS_MULTIPLE_OR_NULL__L6*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V14*/ meltfptr[13] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:5158:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check cont";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5158;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CONT__V10*/ meltfptr[8];
                            /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V14*/ meltfptr[13] = /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5158:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V13*/ meltfptr[12] = /*_._IFELSE___V14*/ meltfptr[13];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:5158:/ clear");
            /*clear*/ /*_#IS_MULTIPLE_OR_NULL__L6*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V14*/ meltfptr[13] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V13*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#eqeq__L7*/ meltfnum[2] =
            ((/*_.OTYP__V12*/ meltfptr[11]) == ((/*!CTYPE_VOID*/ meltfrout->tabval[3])));;
        MELT_LOCATION("warmelt-outobj.melt:5159:/ cond");
        /*cond*/ if (/*_#eqeq__L7*/ meltfnum[2]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:5161:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:5162:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "{"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:5163:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:5164:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                        /*^apply.arg*/
                        argtab[2].meltbp_cstring =  "locexp";
                        /*_.OUTPUT_LOCATION__V17*/ meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.OLOC__V11*/ meltfptr[4]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:5160:/ quasiblock");


                    /*_.PROGN___V18*/ meltfptr[17] = /*_.OUTPUT_LOCATION__V17*/ meltfptr[13];;
                    /*^compute*/
                    /*_._IFELSE___V16*/ meltfptr[14] = /*_.PROGN___V18*/ meltfptr[17];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:5159:/ clear");
                    /*clear*/ /*_.OUTPUT_LOCATION__V17*/ meltfptr[13] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V18*/ meltfptr[17] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:5166:/ cond");
                    /*cond*/ if (/*_.OLOC__V11*/ meltfptr[4]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:5167:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[3];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                    /*^apply.arg*/
                                    argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                                    /*^apply.arg*/
                                    argtab[2].meltbp_cstring =  "expr";
                                    /*_.OUTPUT_RAW_LOCATION__V20*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_RAW_LOCATION*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.OLOC__V11*/ meltfptr[4]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                /*_._IF___V19*/ meltfptr[13] = /*_.OUTPUT_RAW_LOCATION__V20*/ meltfptr[17];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:5166:/ clear");
                                /*clear*/ /*_.OUTPUT_RAW_LOCATION__V20*/ meltfptr[17] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IF___V19*/ meltfptr[13] =  /*reallynil*/ NULL ;;
                        }
                    ;
                    /*^compute*/
                    /*_._IFELSE___V16*/ meltfptr[14] = /*_._IF___V19*/ meltfptr[13];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:5159:/ clear");
                    /*clear*/ /*_._IF___V19*/ meltfptr[13] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:73:/ cond");
        /*cond*/ if ( (melt_module_var_fetch (meltmodvarix_FLAGVAL_OUTPUT_DESCR_COMMENT_symb))) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*^apply*/
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OEXP__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V22*/ meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V21*/ meltfptr[17] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V22*/ meltfptr[13];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V22*/ meltfptr[13] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V21*/ meltfptr[17] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:5172:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L8*/ meltfnum[7] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:5172:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L8*/ meltfnum[7]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L9*/ meltfnum[8] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:5172:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[8];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5172;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objlocatedexpv cont=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CONT__V10*/ meltfptr[8];
                            /*_.MELT_DEBUG_FUN__V24*/ meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V23*/ meltfptr[13] = /*_.MELT_DEBUG_FUN__V24*/ meltfptr[23];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5172:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L9*/ meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V24*/ meltfptr[23] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V23*/ meltfptr[13] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:5172:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L8*/ meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V23*/ meltfptr[13] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*void*/(void)0;
            }
            ;
            /*epilog*/
        }

#endif /*MELT_HAVE_DEBUG*/
        ;
        /*citerblock FOREACH_IN_MULTIPLE*/
        {
            /* start foreach_in_multiple meltcit1__EACHTUP */
            long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.CONT__V10*/ meltfptr[8]);
            for (/*_#IX__L10*/ meltfnum[8] = 0;
                               (/*_#IX__L10*/ meltfnum[8] >= 0) && (/*_#IX__L10*/ meltfnum[8] <  meltcit1__EACHTUP_ln);
                               /*_#IX__L10*/ meltfnum[8]++)
                {
                    /*_.COMP__V25*/ meltfptr[23] = melt_multiple_nth((melt_ptr_t)(/*_.CONT__V10*/ meltfptr[8]),  /*_#IX__L10*/ meltfnum[8]);




                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:5176:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#DEPTHP1__L5*/ meltfnum[1];
                        /*_.OUTPUT_C_CODE__V26*/ meltfptr[13] = meltgc_send((melt_ptr_t)(/*_.COMP__V25*/ meltfptr[23]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[7])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:5177:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L11*/ meltfnum[7] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                        MELT_LOCATION("warmelt-outobj.melt:5178:/ cond");
                        /*cond*/ if (
                            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[8])),
                                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[9])))
                        ) /*then*/
                            {
                                /*^cond.then*/
                                /*^getslot*/
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[8])) /*=obj*/;
                                    melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                                    /*_.REFERENCED_VALUE__V28*/ meltfptr[27] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V28*/ meltfptr[27] =  /*reallynil*/ NULL ;;
                            }
                        ;
                        /*^compute*/
                        /*_#GET_INT__L12*/ meltfnum[11] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V28*/ meltfptr[27])));;
                        /*^compute*/
                        /*_#ltI__L13*/ meltfnum[12] =
                            ((/*_#STRBUF_USEDLENGTH__L11*/ meltfnum[7]) < (/*_#GET_INT__L12*/ meltfnum[11]));;
                        MELT_LOCATION("warmelt-outobj.melt:5177:/ cond");
                        /*cond*/ if (/*_#ltI__L13*/ meltfnum[12]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V29*/ meltfptr[28] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:5177:/ cond.else");

                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*^apply*/
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[4];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_cstring =  "check limited implbuf";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 5177;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V30*/ meltfptr[29] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V29*/ meltfptr[28] = /*_.MELT_ASSERT_FAILURE_FUN__V30*/ meltfptr[29];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:5177:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V30*/ meltfptr[29] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V27*/ meltfptr[26] = /*_._IFELSE___V29*/ meltfptr[28];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5177:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L11*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V28*/ meltfptr[27] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L12*/ meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L13*/ meltfnum[12] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V29*/ meltfptr[28] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V27*/ meltfptr[26] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                    if (/*_#IX__L10*/ meltfnum[8]<0) break;
                } /* end  foreach_in_multiple meltcit1__EACHTUP */

            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:5173:/ clear");
            /*clear*/ /*_.COMP__V25*/ meltfptr[23] = 0 ;
            /*^clear*/
            /*clear*/ /*_#IX__L10*/ meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_.OUTPUT_C_CODE__V26*/ meltfptr[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V27*/ meltfptr[26] = 0 ;
        } /*endciterblock FOREACH_IN_MULTIPLE*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#eqeq__L14*/ meltfnum[7] =
            ((/*_.OTYP__V12*/ meltfptr[11]) == ((/*!CTYPE_VOID*/ meltfrout->tabval[3])));;
        MELT_LOCATION("warmelt-outobj.melt:5180:/ cond");
        /*cond*/ if (/*_#eqeq__L14*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:5181:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";}"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:5182:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:5180:/ quasiblock");


                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_LOCATION("warmelt-outobj.melt:5152:/ clear");
        /*clear*/ /*_.CONT__V10*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OLOC__V11*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#DEPTHP1__L5*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OTYP__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_#eqeq__L7*/ meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V16*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V21*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_#eqeq__L14*/ meltfnum[7] = 0 ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:5184:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L15*/ meltfnum[11] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:5185:/ cond");
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[8])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[9])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[8])) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                        /*_.REFERENCED_VALUE__V32*/ meltfptr[27] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V32*/ meltfptr[27] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L16*/ meltfnum[12] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V32*/ meltfptr[27])));;
            /*^compute*/
            /*_#ltI__L17*/ meltfnum[1] =
                ((/*_#STRBUF_USEDLENGTH__L15*/ meltfnum[11]) < (/*_#GET_INT__L16*/ meltfnum[12]));;
            MELT_LOCATION("warmelt-outobj.melt:5184:/ cond");
            /*cond*/ if (/*_#ltI__L17*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V33*/ meltfptr[28] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:5184:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^apply*/
                        /*apply*/
                        {
                            union meltparam_un argtab[4];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_cstring =  "check limited implbuf";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5184;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V34*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V33*/ meltfptr[28] = /*_.MELT_ASSERT_FAILURE_FUN__V34*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:5184:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V34*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V31*/ meltfptr[29] = /*_._IFELSE___V33*/ meltfptr[28];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:5184:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L15*/ meltfnum[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V32*/ meltfptr[27] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L16*/ meltfnum[12] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L17*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V33*/ meltfptr[28] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V31*/ meltfptr[29] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:5149:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.IFCPP___V31*/ meltfptr[29];;

        {
            MELT_LOCATION("warmelt-outobj.melt:5149:/ locexp");
            /*ochecknores compilobj_nrep_return*/
#if MELT_HAVE_DEBUG
            if (meltxresdescr_ && meltxresdescr_[0] && meltxrestab_)
                melt_warn_for_no_expected_secondary_results();
            /* we warned when secondary results are expected but not returned. */
#endif /*MELT_HAVE_DEBUG*/
            ;
        }
        ;
        /*^finalreturn*/
        ;
        /*finalret*/ goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V7*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V31*/ meltfptr[29] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJLOCATEDEXPV", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_112_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCATEDEXPV*/




/**** end of warmelt-outobj+05.cc ****/
