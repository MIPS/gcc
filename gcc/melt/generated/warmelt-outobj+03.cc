/* GCC MELT GENERATED C++ FILE warmelt-outobj+03.cc - DO NOT EDIT - see http://gcc-melt.org/ */
/* secondary MELT generated C++ file of rank #3 */
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt_f3[] = MELT_RUN_HASHMD5 /* from melt-run.h */;


/**** warmelt-outobj+03.cc declarations ****/
/**** MELT GENERATED DECLARATIONS for warmelt-outobj ** DO NOT EDIT ; see gcc-melt.org ****/

/****++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
***
    Copyright (C) 2008 - 2016  Free Software Foundation, Inc.
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



class Melt_InitialClassyFrame_WARMELTmiOUTOBJ_h761021192; // forward declaration fromline 6691
typedef Melt_InitialClassyFrame_WARMELTmiOUTOBJ_h761021192 Melt_InitialFrame;
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

/*** 4 MELT called hook declarations ***/

/*declare MELT called hook #0 HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_PREVENV, const char* meltinp1_MODULNAME);

/*declare MELT called hook #1 HOOK_MELT_MAKE_LOCATION **/
MELT_EXTERN melt_ptr_t melthook_HOOK_MELT_MAKE_LOCATION (melt_ptr_t melthookdatap, const char* meltinp0_FILENAME, long meltinp1_LINENO);

/*declare MELT called hook #2 HOOK_SYMBOL_IMPORTER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_SYMBOL_IMPORTER (melt_ptr_t melthookdatap, const char* meltinp0_SYMNAMESTR, const char* meltinp1_MODULENAMESTR, melt_ptr_t meltinp2_PARENV);

/*declare MELT called hook #3 HOOK_VALUE_EXPORTER **/
MELT_EXTERN void melthook_HOOK_VALUE_EXPORTER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_SYM, melt_ptr_t meltinp1_VAL, melt_ptr_t meltinp2_CONTENV);

/*** end of 4 MELT called hook declarations ***/

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


MELT_EXTERN MELT_LOW_OPTIMIZATION void* melt_start_this_module (void*); /*mandatory start of module*/

MELT_EXTERN const char melt_module_is_gpl_compatible[];

/*declare opaque initial frame: */

typedef Melt_InitialClassyFrame_WARMELTmiOUTOBJ_h761021192 /*opaqueinitialclassy*/ meltinitial_frame_t;


/* define different names when debugging or not */
#if MELT_HAVE_DEBUG
MELT_EXTERN const char meltmodule_WARMELTmiOUTOBJ__melt_have_debug_enabled[];
#define melt_have_debug_string meltmodule_WARMELTmiOUTOBJ__melt_have_debug_enabled
#else /*!MELT_HAVE_DEBUG*/
MELT_EXTERN const char meltmodule_WARMELTmiOUTOBJ__melt_have_debug_disabled[];
#define melt_have_debug_string meltmodule_WARMELTmiOUTOBJ__melt_have_debug_disabled
#endif /*!MELT_HAVE_DEBUG*/



void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_0 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_1 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_2 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_3 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_4 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_5 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_6 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_7 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_8 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_9 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_10 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_11 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_12 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_13 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_14 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_15 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_16 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_17 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_18 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_19 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_20 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_21 (meltinitial_frame_t*, char*); //declare

void MELT_MODULE_VISIBILITY MELT_LOW_OPTIMIZATION meltmod__WARMELTmiOUTOBJ__initialmeltchunk_22 (meltinitial_frame_t*, char*); //declare



/**** warmelt-outobj+03.cc implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3 fromline 1752 */

    /** start of frame for meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3 of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3// fromline 1568
        : public Melt_CallFrameWithValues<3>
    {
    public: /* fromline 1572*/
// no classy longs
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
        MeltFrame_meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<3> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3), clos) {};
        MeltFrame_meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3() //the constructor fromline 1642
            : Melt_CallFrameWithValues<3> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3)) {};
        MeltFrame_meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3


    /** end of frame for meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3 fromline 1697**/

    /* end of frame for routine meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3 fromline 1756 */

    /* classy proc frame meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3 */ MeltFrame_meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3 fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl3", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:2253:/ getarg");
    /*_.ROU__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DSBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DSBUF__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


        {
            MELT_LOCATION("warmelt-outobj.melt:2254:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.DSBUF__V3*/ meltfptr[2]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2255:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.DSBUF__V3*/ meltfptr[2]), ( "/*startiniframdecl*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2256:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.DSBUF__V3*/ meltfptr[2]), (0), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl3", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_50_WARMELTmiOUTOBJ_LAMBDA_cl3*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE fromline 1752 */

    /** start of frame for meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE// fromline 1568
        : public Melt_CallFrameWithValues<43>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[13];
        /*classy others*/
        const char* loc_CSTRING__o0;
        const char* loc_CSTRING__o1;
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<43> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE), clos) {};
        MeltFrame_meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE() //the constructor fromline 1642
            : Melt_CallFrameWithValues<43> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<43> (fil,lin, sizeof(MeltFrame_meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE)) {};
        MeltFrame_meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<43> (fil,lin, sizeof(MeltFrame_meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE


    /** end of frame for meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE fromline 1697**/

    /* end of frame for routine meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE fromline 1756 */

    /* classy proc frame meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE */ MeltFrame_meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUT_CENTRALDECL_HOOKROUTINE", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:2450:/ getarg");
    /*_.PHOOK__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.SBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_CSTRING) goto meltlab_endgetargs;
    /*_?*/ meltfram__.loc_CSTRING__o0 = meltxargtab_[1].meltbp_cstring;

    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_CSTRING) goto meltlab_endgetargs;
    /*_?*/ meltfram__.loc_CSTRING__o1 = meltxargtab_[2].meltbp_cstring;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2451:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:2451:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:2451:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2451;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "output_centraldecl_hookroutine start phook=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.PHOOK__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V4*/ meltfptr[3] = /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2451:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V4*/ meltfptr[3] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:2451:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V4*/ meltfptr[3] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:2452:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_HOOKROUTINEOBJ*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:2452:/ cond");
            /*cond*/ if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[3] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2452:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check phook";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2452;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.PHOOK__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V7*/ meltfptr[3] = /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2452:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V6*/ meltfptr[4] = /*_._IFELSE___V7*/ meltfptr[3];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2452:/ clear");
            /*clear*/ /*_#IS_A__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V7*/ meltfptr[3] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V6*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2453:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRBUF__L4*/ meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2])) == MELTOBMAG_STRBUF);;
            MELT_LOCATION("warmelt-outobj.melt:2453:/ cond");
            /*cond*/ if (/*_#IS_STRBUF__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[3] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2453:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sbuf";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2453;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V10*/ meltfptr[3] = /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2453:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V9*/ meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[3];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2453:/ clear");
            /*clear*/ /*_#IS_STRBUF__L4*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V10*/ meltfptr[3] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V9*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:2454:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                    /*_.HOOKNAME__V12*/ meltfptr[10] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.HOOKNAME__V12*/ meltfptr[10] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2455:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_ROUTINEOBJ*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 4, "OBROUT_PROC");
                    /*_.NHOOKPROC__V13*/ meltfptr[3] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.NHOOKPROC__V13*/ meltfptr[3] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2456:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_ROUTINEOBJ*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 9, "OBROUT_RETVAL");
                    /*_.ORETVAL__V14*/ meltfptr[13] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ORETVAL__V14*/ meltfptr[13] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2457:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.NHOOKPROC__V13*/ meltfptr[3]),
                                          (melt_ptr_t)((/*!CLASS_NREP_HOOKPROC*/ meltfrout->tabval[5])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.NHOOKPROC__V13*/ meltfptr[3]) /*=obj*/;
                    melt_object_get_field(slot,obj, 8, "NRHOOK_CTYPE");
                    /*_.ORETCTYPE__V15*/ meltfptr[14] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ORETCTYPE__V15*/ meltfptr[14] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2458:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.ORETCTYPE__V15*/ meltfptr[14]),
                                          (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[6])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.ORETCTYPE__V15*/ meltfptr[14]) /*=obj*/;
                    melt_object_get_field(slot,obj, 4, "CTYPE_CNAME");
                    /*_.ORETCTYCNAME__V16*/ meltfptr[15] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ORETCTYCNAME__V16*/ meltfptr[15] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2459:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_HOOKROUTINEOBJ*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 12, "OHOOKROUT_DATARG");
                    /*_.OHKDATARG__V17*/ meltfptr[16] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OHKDATARG__V17*/ meltfptr[16] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2460:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OHKDATARG__V17*/ meltfptr[16]),
                                          (melt_ptr_t)((/*!CLASS_OBJCFORMAL*/ meltfrout->tabval[7])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OHKDATARG__V17*/ meltfptr[16]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "OFORMAL_NAME");
                    /*_.OHKDATACNAME__V18*/ meltfptr[17] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OHKDATACNAME__V18*/ meltfptr[17] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2461:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_HOOKROUTINEOBJ*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 13, "OHOOKROUT_INARGS");
                    /*_.OHKINARGS__V19*/ meltfptr[18] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OHKINARGS__V19*/ meltfptr[18] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2462:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_HOOKROUTINEOBJ*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 14, "OHOOKROUT_OUTARGS");
                    /*_.OHKOUTARGS__V20*/ meltfptr[19] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OHKOUTARGS__V20*/ meltfptr[19] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2464:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
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
            MELT_LOCATION("warmelt-outobj.melt:2464:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L6*/ meltfnum[0] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:2464:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L6*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2464;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "output_centraldecl_hookroutine oretctype=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.ORETCTYPE__V15*/ meltfptr[14];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n.. oretval=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.ORETVAL__V14*/ meltfptr[13];
                            /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V21*/ meltfptr[20] = /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2464:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L6*/ meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V21*/ meltfptr[20] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:2464:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L5*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V21*/ meltfptr[20] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:2465:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L7*/ meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ORETCTYPE__V15*/ meltfptr[14]), (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[6])));;
            MELT_LOCATION("warmelt-outobj.melt:2465:/ cond");
            /*cond*/ if (/*_#IS_A__L7*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V24*/ meltfptr[20] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2465:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oretctype";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2465;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ORETCTYPE__V15*/ meltfptr[14];
                            /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V24*/ meltfptr[20] = /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2465:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V23*/ meltfptr[21] = /*_._IFELSE___V24*/ meltfptr[20];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2465:/ clear");
            /*clear*/ /*_#IS_A__L7*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V24*/ meltfptr[20] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V23*/ meltfptr[21] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2466:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (0), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2467:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[5];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "/** ";
            /*^apply.arg*/
            argtab[1].meltbp_cstring = /*_?*/ meltfram__.loc_CSTRING__o0;
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " hook ";
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V12*/ meltfptr[10];
            /*^apply.arg*/
            argtab[4].meltbp_cstring =  " */";
            /*_.ADD2OUT__V26*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2468:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (0), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2469:/ cond");
        /*cond*/ if (/*_?*/ meltfram__.loc_CSTRING__o1) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:2470:/ locexp");
                        meltgc_add_out((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_?*/ meltfram__.loc_CSTRING__o1));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2471:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (0), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:2469:/ quasiblock");


                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2473:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[5];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ORETCTYCNAME__V16*/ meltfptr[15];
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  " ";
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V12*/ meltfptr[10];
            /*^apply.arg*/
            argtab[3].meltbp_cstring =  "(melt_ptr_t ";
            /*^apply.arg*/
            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OHKDATACNAME__V18*/ meltfptr[17];
            /*_.ADD2OUT__V27*/ meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*citerblock FOREACH_IN_MULTIPLE*/
        {
            /* start foreach_in_multiple meltcit1__EACHTUP */
            long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.OHKINARGS__V19*/ meltfptr[18]);
            for (/*_#INIX__L8*/ meltfnum[1] = 0;
                                (/*_#INIX__L8*/ meltfnum[1] >= 0) && (/*_#INIX__L8*/ meltfnum[1] <  meltcit1__EACHTUP_ln);
                                /*_#INIX__L8*/ meltfnum[1]++)
                {
                    /*_.CURINARG__V28*/ meltfptr[27] = melt_multiple_nth((melt_ptr_t)(/*_.OHKINARGS__V19*/ meltfptr[18]),  /*_#INIX__L8*/ meltfnum[1]);




#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:2478:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        {
                            /*^locexp*/
                            /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                            melt_dbgcounter++;
#endif
                            ;
                        }
                        ;

                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#MELT_NEED_DBG__L9*/ meltfnum[0] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:2478:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L9*/ meltfnum[0]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L10*/ meltfnum[9] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:2478:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[7];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L10*/ meltfnum[9];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 2478;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "output_centraldecl_hookroutine curinarg=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CURINARG__V28*/ meltfptr[27];
                                        /*^apply.arg*/
                                        argtab[5].meltbp_cstring =  " inix=";
                                        /*^apply.arg*/
                                        argtab[6].meltbp_long = /*_#INIX__L8*/ meltfnum[1];
                                        /*_.MELT_DEBUG_FUN__V30*/ meltfptr[29] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V29*/ meltfptr[28] = /*_.MELT_DEBUG_FUN__V30*/ meltfptr[29];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:2478:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L10*/ meltfnum[9] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V30*/ meltfptr[29] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V29*/ meltfptr[28] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:2478:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L9*/ meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V29*/ meltfptr[28] = 0 ;
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

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2479:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ","));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2480:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (20), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:2481:/ quasiblock");


                    /*^cond*/
                    /*cond*/ if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CURINARG__V28*/ meltfptr[27]),
                                                      (melt_ptr_t)((/*!CLASS_OBJVALUE*/ meltfrout->tabval[9])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.CURINARG__V28*/ meltfptr[27]) /*=obj*/;
                                melt_object_get_field(slot,obj, 0, "OBV_TYPE");
                                /*_.CURINCTYP__V32*/ meltfptr[28] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.CURINCTYP__V32*/ meltfptr[28] =  /*reallynil*/ NULL ;;
                        }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:2482:/ cond");
                    /*cond*/ if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CURINARG__V28*/ meltfptr[27]),
                                                      (melt_ptr_t)((/*!CLASS_OBJCFORMAL*/ meltfrout->tabval[7])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.CURINARG__V28*/ meltfptr[27]) /*=obj*/;
                                melt_object_get_field(slot,obj, 1, "OFORMAL_NAME");
                                /*_.CURFORMALNAME__V33*/ meltfptr[32] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.CURFORMALNAME__V33*/ meltfptr[32] =  /*reallynil*/ NULL ;;
                        }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:2483:/ cond");
                    /*cond*/ if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CURINCTYP__V32*/ meltfptr[28]),
                                                      (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[6])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.CURINCTYP__V32*/ meltfptr[28]) /*=obj*/;
                                melt_object_get_field(slot,obj, 4, "CTYPE_CNAME");
                                /*_.CURINCTYCNAME__V34*/ meltfptr[33] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.CURINCTYCNAME__V34*/ meltfptr[33] =  /*reallynil*/ NULL ;;
                        }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:2485:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CURINCTYCNAME__V34*/ meltfptr[33];
                        /*^apply.arg*/
                        argtab[1].meltbp_cstring =  " ";
                        /*^apply.arg*/
                        argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.CURFORMALNAME__V33*/ meltfptr[32];
                        /*_.ADD2OUT__V35*/ meltfptr[34] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_.LET___V31*/ meltfptr[29] = /*_.ADD2OUT__V35*/ meltfptr[34];;

                    MELT_LOCATION("warmelt-outobj.melt:2481:/ clear");
                    /*clear*/ /*_.CURINCTYP__V32*/ meltfptr[28] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.CURFORMALNAME__V33*/ meltfptr[32] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.CURINCTYCNAME__V34*/ meltfptr[33] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ADD2OUT__V35*/ meltfptr[34] = 0 ;
                    if (/*_#INIX__L8*/ meltfnum[1]<0) break;
                } /* end  foreach_in_multiple meltcit1__EACHTUP */

            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:2475:/ clear");
            /*clear*/ /*_.CURINARG__V28*/ meltfptr[27] = 0 ;
            /*^clear*/
            /*clear*/ /*_#INIX__L8*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_.LET___V31*/ meltfptr[29] = 0 ;
        } /*endciterblock FOREACH_IN_MULTIPLE*/
        ;
        /*citerblock FOREACH_IN_MULTIPLE*/
        {
            /* start foreach_in_multiple meltcit2__EACHTUP */
            long  meltcit2__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.OHKOUTARGS__V20*/ meltfptr[19]);
            for (/*_#OUTIX__L11*/ meltfnum[9] = 0;
                                  (/*_#OUTIX__L11*/ meltfnum[9] >= 0) && (/*_#OUTIX__L11*/ meltfnum[9] <  meltcit2__EACHTUP_ln);
                                  /*_#OUTIX__L11*/ meltfnum[9]++)
                {
                    /*_.CUROUTARG__V36*/ meltfptr[28] = melt_multiple_nth((melt_ptr_t)(/*_.OHKOUTARGS__V20*/ meltfptr[19]),  /*_#OUTIX__L11*/ meltfnum[9]);




#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:2491:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        {
                            /*^locexp*/
                            /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                            melt_dbgcounter++;
#endif
                            ;
                        }
                        ;

                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#MELT_NEED_DBG__L12*/ meltfnum[0] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:2491:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L12*/ meltfnum[0]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L13*/ meltfnum[12] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:2491:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[7];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L13*/ meltfnum[12];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 2491;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "output_centraldecl_hookroutine curoutarg=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CUROUTARG__V36*/ meltfptr[28];
                                        /*^apply.arg*/
                                        argtab[5].meltbp_cstring =  " outix=";
                                        /*^apply.arg*/
                                        argtab[6].meltbp_long = /*_#OUTIX__L11*/ meltfnum[9];
                                        /*_.MELT_DEBUG_FUN__V38*/ meltfptr[33] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V37*/ meltfptr[32] = /*_.MELT_DEBUG_FUN__V38*/ meltfptr[33];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:2491:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L13*/ meltfnum[12] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V38*/ meltfptr[33] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V37*/ meltfptr[32] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:2491:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L12*/ meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V37*/ meltfptr[32] = 0 ;
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

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2492:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ","));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2493:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (20), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:2494:/ quasiblock");


                    /*^cond*/
                    /*cond*/ if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CUROUTARG__V36*/ meltfptr[28]),
                                                      (melt_ptr_t)((/*!CLASS_OBJVALUE*/ meltfrout->tabval[9])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.CUROUTARG__V36*/ meltfptr[28]) /*=obj*/;
                                melt_object_get_field(slot,obj, 0, "OBV_TYPE");
                                /*_.CUROUTCTYP__V40*/ meltfptr[33] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.CUROUTCTYP__V40*/ meltfptr[33] =  /*reallynil*/ NULL ;;
                        }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:2495:/ cond");
                    /*cond*/ if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CUROUTARG__V36*/ meltfptr[28]),
                                                      (melt_ptr_t)((/*!CLASS_OBJCFORMAL*/ meltfrout->tabval[7])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.CUROUTARG__V36*/ meltfptr[28]) /*=obj*/;
                                melt_object_get_field(slot,obj, 1, "OFORMAL_NAME");
                                /*_.CURFORMALNAME__V41*/ meltfptr[32] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.CURFORMALNAME__V41*/ meltfptr[32] =  /*reallynil*/ NULL ;;
                        }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:2496:/ cond");
                    /*cond*/ if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CUROUTCTYP__V40*/ meltfptr[33]),
                                                      (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[6])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.CUROUTCTYP__V40*/ meltfptr[33]) /*=obj*/;
                                melt_object_get_field(slot,obj, 4, "CTYPE_CNAME");
                                /*_.CUROUTCTYCNAME__V42*/ meltfptr[41] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.CUROUTCTYCNAME__V42*/ meltfptr[41] =  /*reallynil*/ NULL ;;
                        }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:2498:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CUROUTCTYCNAME__V42*/ meltfptr[41];
                        /*^apply.arg*/
                        argtab[1].meltbp_cstring =  "* ";
                        /*^apply.arg*/
                        argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.CURFORMALNAME__V41*/ meltfptr[32];
                        /*_.ADD2OUT__V43*/ meltfptr[42] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_.LET___V39*/ meltfptr[34] = /*_.ADD2OUT__V43*/ meltfptr[42];;

                    MELT_LOCATION("warmelt-outobj.melt:2494:/ clear");
                    /*clear*/ /*_.CUROUTCTYP__V40*/ meltfptr[33] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.CURFORMALNAME__V41*/ meltfptr[32] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.CUROUTCTYCNAME__V42*/ meltfptr[41] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ADD2OUT__V43*/ meltfptr[42] = 0 ;
                    if (/*_#OUTIX__L11*/ meltfnum[9]<0) break;
                } /* end  foreach_in_multiple meltcit2__EACHTUP */

            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:2488:/ clear");
            /*clear*/ /*_.CUROUTARG__V36*/ meltfptr[28] = 0 ;
            /*^clear*/
            /*clear*/ /*_#OUTIX__L11*/ meltfnum[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_.LET___V39*/ meltfptr[34] = 0 ;
        } /*endciterblock FOREACH_IN_MULTIPLE*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2500:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ")"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2501:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (0), 0);
        }
        ;

        MELT_LOCATION("warmelt-outobj.melt:2454:/ clear");
        /*clear*/ /*_.HOOKNAME__V12*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NHOOKPROC__V13*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ORETVAL__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ORETCTYPE__V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ORETCTYCNAME__V16*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OHKDATARG__V17*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OHKDATACNAME__V18*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OHKINARGS__V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OHKOUTARGS__V20*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V23*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V26*/ meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V27*/ meltfptr[20] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:2450:/ clear");
        /*clear*/ /*_.IFCPP___V6*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V9*/ meltfptr[7] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUT_CENTRALDECL_HOOKROUTINE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_51_WARMELTmiOUTOBJ_OUTPUT_CENTRALDECL_HOOKROUTINE*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE fromline 1752 */

    /** start of frame for meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE// fromline 1568
        : public Melt_CallFrameWithValues<73>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[26];
        /*classy others*/
        const char* loc_CSTRING__o0;
        long _meltsparel;
        void melt_mark_stuff (void)
        {
        } /*end melt_mark_stuff*/
        virtual void melt_mark_ggc_data (void)
        {
            melt_mark_values ();
            melt_mark_stuff ();
        }; /*end melt_mark_ggc_data*/
        MeltFrame_meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<73> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE), clos) {};
        MeltFrame_meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE() //the constructor fromline 1642
            : Melt_CallFrameWithValues<73> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<73> (fil,lin, sizeof(MeltFrame_meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE)) {};
        MeltFrame_meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<73> (fil,lin, sizeof(MeltFrame_meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE


    /** end of frame for meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE fromline 1697**/

    /* end of frame for routine meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE fromline 1756 */

    /* classy proc frame meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE */ MeltFrame_meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_HOOKROUTINE", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:2508:/ getarg");
    /*_.PHOOK__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:2509:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
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
            MELT_LOCATION("warmelt-outobj.melt:2509:/ cond");
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
                        /*^compute*/
                        /*_.DISCRIM__V6*/ meltfptr[5] =
                            ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]))));;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:2509:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2509;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_hookroutine start phook=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.PHOOK__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n of discrim=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.DISCRIM__V6*/ meltfptr[5];
                            /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2509:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L3*/ meltfnum[2] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.DISCRIM__V6*/ meltfptr[5] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V5*/ meltfptr[4] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:2509:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:2511:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_HOOKROUTINEOBJ*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:2511:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2511:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check phook";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2511;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.PHOOK__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/ meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[4];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2511:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[4] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/ meltfptr[5] = /*_._IFELSE___V9*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2511:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/ meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:2512:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                    /*_.HOOKNAME__V11*/ meltfptr[4] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.HOOKNAME__V11*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2513:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_ROUTINEOBJ*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 4, "OBROUT_PROC");
                    /*_.NHOOKPROC__V12*/ meltfptr[6] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.NHOOKPROC__V12*/ meltfptr[6] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2514:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.NHOOKPROC__V12*/ meltfptr[6]),
                                          (melt_ptr_t)((/*!CLASS_NREP_CLOSPROC*/ meltfrout->tabval[5])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.NHOOKPROC__V12*/ meltfptr[6]) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "NRCLOP_NAME");
                    /*_.NRAWHOOKNAME__V13*/ meltfptr[12] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.NRAWHOOKNAME__V13*/ meltfptr[12] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2515:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_ROUTINEOBJ*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 5, "OBROUT_BODY");
                    /*_.OBODY__V14*/ meltfptr[13] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OBODY__V14*/ meltfptr[13] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2516:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_ROUTINEOBJ*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 6, "OBROUT_NBVAL");
                    /*_.NBVAL__V15*/ meltfptr[14] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.NBVAL__V15*/ meltfptr[14] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2517:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_ROUTINEOBJ*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 7, "OBROUT_NBLONG");
                    /*_.NBLONGS__V16*/ meltfptr[15] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.NBLONGS__V16*/ meltfptr[15] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2518:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_ROUTINEOBJ*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 8, "OBROUT_OTHERS");
                    /*_.OTHERVARS__V17*/ meltfptr[16] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OTHERVARS__V17*/ meltfptr[16] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2519:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_ROUTINEOBJ*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 9, "OBROUT_RETVAL");
                    /*_.ORETVAL__V18*/ meltfptr[17] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ORETVAL__V18*/ meltfptr[17] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2520:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.ORETVAL__V18*/ meltfptr[17]),
                                          (melt_ptr_t)((/*!CLASS_OBJVALUE*/ meltfrout->tabval[6])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.ORETVAL__V18*/ meltfptr[17]) /*=obj*/;
                    melt_object_get_field(slot,obj, 0, "OBV_TYPE");
                    /*_.ORETCTYPE__V19*/ meltfptr[18] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ORETCTYPE__V19*/ meltfptr[18] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2521:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.ORETCTYPE__V19*/ meltfptr[18]),
                                          (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[7])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.ORETCTYPE__V19*/ meltfptr[18]) /*=obj*/;
                    melt_object_get_field(slot,obj, 4, "CTYPE_CNAME");
                    /*_.ORETCTYCNAME__V20*/ meltfptr[19] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ORETCTYCNAME__V20*/ meltfptr[19] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2524:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_HOOKROUTINEOBJ*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 12, "OHOOKROUT_DATARG");
                    /*_.OHKDATARG__V21*/ meltfptr[20] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OHKDATARG__V21*/ meltfptr[20] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2525:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OHKDATARG__V21*/ meltfptr[20]),
                                          (melt_ptr_t)((/*!CLASS_OBJCFORMAL*/ meltfrout->tabval[8])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OHKDATARG__V21*/ meltfptr[20]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "OFORMAL_NAME");
                    /*_.OHKDATACNAME__V22*/ meltfptr[21] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OHKDATACNAME__V22*/ meltfptr[21] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2526:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_HOOKROUTINEOBJ*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 13, "OHOOKROUT_INARGS");
                    /*_.OHKINARGS__V23*/ meltfptr[22] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OHKINARGS__V23*/ meltfptr[22] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2527:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_HOOKROUTINEOBJ*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 14, "OHOOKROUT_OUTARGS");
                    /*_.OHKOUTARGS__V24*/ meltfptr[23] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OHKOUTARGS__V24*/ meltfptr[23] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2528:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_HOOKROUTINEOBJ*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 15, "OHOOKROUT_EPILOG");
                    /*_.OHKEPILOG__V25*/ meltfptr[24] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OHKEPILOG__V25*/ meltfptr[24] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#OBJ_HASH__L5*/ meltfnum[1] =
            (melt_obj_hash((melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1])));;
        /*^compute*/
        /*_#pcIRAW__L6*/ meltfnum[2] =
            ((/*_#OBJ_HASH__L5*/ meltfnum[1]) % (4096));;
        /*^compute*/
        /*_#MINIHASH__L7*/ meltfnum[6] =
            ((2) + (/*_#pcIRAW__L6*/ meltfnum[2]));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2531:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
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
            MELT_LOCATION("warmelt-outobj.melt:2531:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:2531:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[23];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[8];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2531;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_hookroutine hookname=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V11*/ meltfptr[4];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n nrawhookname=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.NRAWHOOKNAME__V13*/ meltfptr[12];
                            /*^apply.arg*/
                            argtab[7].meltbp_cstring =  "\n ohkdatarg=";
                            /*^apply.arg*/
                            argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.OHKDATARG__V21*/ meltfptr[20];
                            /*^apply.arg*/
                            argtab[9].meltbp_cstring =  "\n oretctype=";
                            /*^apply.arg*/
                            argtab[10].meltbp_aptr = (melt_ptr_t*) &/*_.ORETCTYPE__V19*/ meltfptr[18];
                            /*^apply.arg*/
                            argtab[11].meltbp_cstring =  "\n ohkinargs=";
                            /*^apply.arg*/
                            argtab[12].meltbp_aptr = (melt_ptr_t*) &/*_.OHKINARGS__V23*/ meltfptr[22];
                            /*^apply.arg*/
                            argtab[13].meltbp_cstring =  "\n ohkoutargs=";
                            /*^apply.arg*/
                            argtab[14].meltbp_aptr = (melt_ptr_t*) &/*_.OHKOUTARGS__V24*/ meltfptr[23];
                            /*^apply.arg*/
                            argtab[15].meltbp_cstring =  "\n oretval=";
                            /*^apply.arg*/
                            argtab[16].meltbp_aptr = (melt_ptr_t*) &/*_.ORETVAL__V18*/ meltfptr[17];
                            /*^apply.arg*/
                            argtab[17].meltbp_cstring =  "\n oretctype=";
                            /*^apply.arg*/
                            argtab[18].meltbp_aptr = (melt_ptr_t*) &/*_.ORETCTYPE__V19*/ meltfptr[18];
                            /*^apply.arg*/
                            argtab[19].meltbp_cstring =  "\n obody=";
                            /*^apply.arg*/
                            argtab[20].meltbp_aptr = (melt_ptr_t*) &/*_.OBODY__V14*/ meltfptr[13];
                            /*^apply.arg*/
                            argtab[21].meltbp_cstring =  "\n ohkepilog=";
                            /*^apply.arg*/
                            argtab[22].meltbp_aptr = (melt_ptr_t*) &/*_.OHKEPILOG__V25*/ meltfptr[24];
                            /*_.MELT_DEBUG_FUN__V27*/ meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V26*/ meltfptr[25] = /*_.MELT_DEBUG_FUN__V27*/ meltfptr[26];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2531:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L9*/ meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V27*/ meltfptr[26] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V26*/ meltfptr[25] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:2531:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L8*/ meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V26*/ meltfptr[25] = 0 ;
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

        {
            MELT_LOCATION("warmelt-outobj.melt:2541:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (0), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2542:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "/** start of declaration for hook ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V11*/ meltfptr[4];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "**/";
            /*_.ADD2OUT__V28*/ meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2543:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2544:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2545:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "/** start of implementation for hook ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V11*/ meltfptr[4];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "**/";
            /*_.ADD2OUT__V29*/ meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2546:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2548:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "declaration of";
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "MELT_EXTERN";
            /*_.OUTPUT_CENTRALDECL_HOOKROUTINE__V30*/ meltfptr[29] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_CENTRALDECL_HOOKROUTINE*/ meltfrout->tabval[10])), (melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2549:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), ( ";"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2550:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2551:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2552:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2553:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2554:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "/* frame for ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V11*/ meltfptr[4];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " hook: */";
            /*_.ADD2OUT__V31*/ meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2555:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*_.OUTPUT_CURFRAME_DECLSTRUCT__V32*/ meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_CURFRAME_DECLSTRUCT*/ meltfrout->tabval[11])), (melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2556:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2557:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "/* endframe for ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V11*/ meltfptr[4];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " hook */";
            /*_.ADD2OUT__V33*/ meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2558:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2559:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;
        /*_?*/ meltfram__.loc_CSTRING__o0 =
            (char*)0;;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2560:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_cstring =  "implementation of";
            /*^apply.arg*/
            argtab[2].meltbp_cstring = /*_?*/ meltfram__.loc_CSTRING__o0;
            /*_.OUTPUT_CENTRALDECL_HOOKROUTINE__V34*/ meltfptr[33] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_CENTRALDECL_HOOKROUTINE*/ meltfrout->tabval[10])), (melt_ptr_t)(/*_.PHOOK__V2*/ meltfptr[1]), (MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2561:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "{ /* body of hook ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V11*/ meltfptr[4];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " from outpucod_hookroutine */";
            /*_.ADD2OUT__V35*/ meltfptr[34] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2562:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2563:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OHKDATARG__V21*/ meltfptr[20]),
                                          (melt_ptr_t)((/*!CLASS_OBJCFORMAL*/ meltfrout->tabval[8])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OHKDATARG__V21*/ meltfptr[20]) /*=obj*/;
                    melt_object_get_field(slot,obj, 4, "OFORMAL_LOCV");
                    /*_.ODATALOCV__V36*/ meltfptr[35] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ODATALOCV__V36*/ meltfptr[35] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2565:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L10*/ meltfnum[8] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:2565:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L10*/ meltfnum[8]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L11*/ meltfnum[7] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:2565:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[9];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[7];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2565;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_hookroutine framing hookname=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V11*/ meltfptr[4];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n.. ohkdatarg=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.OHKDATARG__V21*/ meltfptr[20];
                            /*^apply.arg*/
                            argtab[7].meltbp_cstring =  "\n.. odatalocv=";
                            /*^apply.arg*/
                            argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.ODATALOCV__V36*/ meltfptr[35];
                            /*_.MELT_DEBUG_FUN__V38*/ meltfptr[37] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V37*/ meltfptr[36] = /*_.MELT_DEBUG_FUN__V38*/ meltfptr[37];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2565:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L11*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V38*/ meltfptr[37] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V37*/ meltfptr[36] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:2565:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L10*/ meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V37*/ meltfptr[36] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:2568:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[13];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "/* declare and initialize frame for ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V11*/ meltfptr[4];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " hook */ \n       #undef meltcallcount\
\n       #if MELT_HAVE_DEBUG\n       static long melthook_call_counter_";
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.NRAWHOOKNAME__V13*/ meltfptr[12];
            /*^apply.arg*/
            argtab[4].meltbp_cstring =  " ;\n       long melt_thishookcall_counter_";
            /*^apply.arg*/
            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.NRAWHOOKNAME__V13*/ meltfptr[12];
            /*^apply.arg*/
            argtab[6].meltbp_cstring =  " ATTRIBUTE_UNUSED \n       = melthook_call_counter_";
            /*^apply.arg*/
            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.NRAWHOOKNAME__V13*/ meltfptr[12];
            /*^apply.arg*/
            argtab[8].meltbp_cstring =  "++ ;\n       #define meltcallcount melt_thishookcall_counter_";
            /*^apply.arg*/
            argtab[9].meltbp_aptr = (melt_ptr_t*) &/*_.NRAWHOOKNAME__V13*/ meltfptr[12];
            /*^apply.arg*/
            argtab[10].meltbp_cstring =  "\n       #else\n       #define meltcallcount 0L\
\n       #endif\n       MeltFrame_";
            /*^apply.arg*/
            argtab[11].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V11*/ meltfptr[4];
            /*^apply.arg*/
            argtab[12].meltbp_cstring =  " meltfram__(meltcast_melthook_st(melthookdatap)) /*hook classy frame\
*/;\n       ";
            /*_.ADD2OUT__V39*/ meltfptr[37] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2583:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2584:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "#define meltfhook /*hook data of ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.NRAWHOOKNAME__V13*/ meltfptr[12];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "*/ ((melthook_ptr_t) (";
            /*_.ADD2OUT__V40*/ meltfptr[36] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2585:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = 0;
            /*_.OUTPUT_C_CODE__V41*/ meltfptr[40] = meltgc_send((melt_ptr_t)(/*_.ODATALOCV__V36*/ meltfptr[35]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[12])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2586:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "))"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2587:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (1), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2589:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "/* body of ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V11*/ meltfptr[4];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " hook */";
            /*_.ADD2OUT__V42*/ meltfptr[41] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2593:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (1), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2594:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L12*/ meltfnum[7] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:2594:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L12*/ meltfnum[7]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L13*/ meltfnum[8] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:2594:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L13*/ meltfnum[8];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2594;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_hookroutine obody=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OBODY__V14*/ meltfptr[13];
                            /*_.MELT_DEBUG_FUN__V44*/ meltfptr[43] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V43*/ meltfptr[42] = /*_.MELT_DEBUG_FUN__V44*/ meltfptr[43];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2594:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L13*/ meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V44*/ meltfptr[43] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V43*/ meltfptr[42] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:2594:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L12*/ meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V43*/ meltfptr[42] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:2595:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_LIST__L14*/ meltfnum[8] =
                (melt_magic_discr((melt_ptr_t)(/*_.OBODY__V14*/ meltfptr[13])) == MELTOBMAG_LIST);;
            MELT_LOCATION("warmelt-outobj.melt:2595:/ cond");
            /*cond*/ if (/*_#IS_LIST__L14*/ meltfnum[8]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V46*/ meltfptr[42] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2595:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check list body";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2595;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBODY__V14*/ meltfptr[13];
                            /*_.MELT_ASSERT_FAILURE_FUN__V47*/ meltfptr[46] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V46*/ meltfptr[42] = /*_.MELT_ASSERT_FAILURE_FUN__V47*/ meltfptr[46];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2595:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V47*/ meltfptr[46] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V45*/ meltfptr[43] = /*_._IFELSE___V46*/ meltfptr[42];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2595:/ clear");
            /*clear*/ /*_#IS_LIST__L14*/ meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V46*/ meltfptr[42] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V45*/ meltfptr[43] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit1__EACHLIST */
            for (/*_.CURBODYPAIR__V48*/ meltfptr[46] = melt_list_first( (melt_ptr_t)/*_.OBODY__V14*/ meltfptr[13]);
                                        melt_magic_discr((melt_ptr_t) /*_.CURBODYPAIR__V48*/ meltfptr[46]) == MELTOBMAG_PAIR;
                                        /*_.CURBODYPAIR__V48*/ meltfptr[46] = melt_pair_tail((melt_ptr_t) /*_.CURBODYPAIR__V48*/ meltfptr[46]))
                {
                    /*_.CURBODYO__V49*/ meltfptr[42] = melt_pair_head((melt_ptr_t) /*_.CURBODYPAIR__V48*/ meltfptr[46]);



#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:2599:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        {
                            /*^locexp*/
                            /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                            melt_dbgcounter++;
#endif
                            ;
                        }
                        ;

                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#MELT_NEED_DBG__L15*/ meltfnum[7] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:2599:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L15*/ meltfnum[7]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L16*/ meltfnum[8] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:2599:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L16*/ meltfnum[8];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 2599;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "outpucod_hookroutine curbodyo=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CURBODYO__V49*/ meltfptr[42];
                                        /*_.MELT_DEBUG_FUN__V51*/ meltfptr[50] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V50*/ meltfptr[49] = /*_.MELT_DEBUG_FUN__V51*/ meltfptr[50];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:2599:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L16*/ meltfnum[8] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V51*/ meltfptr[50] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V50*/ meltfptr[49] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:2599:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L15*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V50*/ meltfptr[49] = 0 ;
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
                    /*_#IS_NOT_A__L17*/ meltfnum[8] =
                        !melt_is_instance_of((melt_ptr_t)(/*_.CURBODYO__V49*/ meltfptr[42]), (melt_ptr_t)((/*!CLASS_OBJPUREVALUE*/ meltfrout->tabval[13])));;
                    MELT_LOCATION("warmelt-outobj.melt:2600:/ cond");
                    /*cond*/ if (/*_#IS_NOT_A__L17*/ meltfnum[8]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:2601:/ msend");
                                /*msend*/
                                {
                                    union meltparam_un argtab[3];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^ojbmsend.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                    /*^ojbmsend.arg*/
                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                    /*^ojbmsend.arg*/
                                    argtab[2].meltbp_long = 1;
                                    /*_.OUTPUT_C_CODE__V52*/ meltfptr[50] = meltgc_send((melt_ptr_t)(/*_.CURBODYO__V49*/ meltfptr[42]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[12])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2602:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2603:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (1), 0);
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:2600:/ quasiblock");


                                /*epilog*/

                                /*^clear*/
                                /*clear*/ /*_.OUTPUT_C_CODE__V52*/ meltfptr[50] = 0 ;
                            }
                            ;
                        } /*noelse*/
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:2604:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L18*/ meltfnum[7] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                        MELT_LOCATION("warmelt-outobj.melt:2605:/ cond");
                        /*cond*/ if (
                            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[14])),
                                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[15])))
                        ) /*then*/
                            {
                                /*^cond.then*/
                                /*^getslot*/
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[14])) /*=obj*/;
                                    melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                                    /*_.REFERENCED_VALUE__V54*/ meltfptr[50] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V54*/ meltfptr[50] =  /*reallynil*/ NULL ;;
                            }
                        ;
                        /*^compute*/
                        /*_#GET_INT__L19*/ meltfnum[18] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V54*/ meltfptr[50])));;
                        /*^compute*/
                        /*_#ltI__L20*/ meltfnum[19] =
                            ((/*_#STRBUF_USEDLENGTH__L18*/ meltfnum[7]) < (/*_#GET_INT__L19*/ meltfnum[18]));;
                        MELT_LOCATION("warmelt-outobj.melt:2604:/ cond");
                        /*cond*/ if (/*_#ltI__L20*/ meltfnum[19]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V55*/ meltfptr[54] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:2604:/ cond.else");

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
                                        argtab[2].meltbp_long = 2604;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V56*/ meltfptr[55] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V55*/ meltfptr[54] = /*_.MELT_ASSERT_FAILURE_FUN__V56*/ meltfptr[55];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:2604:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V56*/ meltfptr[55] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V53*/ meltfptr[49] = /*_._IFELSE___V55*/ meltfptr[54];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2604:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L18*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V54*/ meltfptr[50] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L19*/ meltfnum[18] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L20*/ meltfnum[19] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V55*/ meltfptr[54] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V53*/ meltfptr[49] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                } /* end foreach_pair_component_in_list meltcit1__EACHLIST */
            /*_.CURBODYPAIR__V48*/ meltfptr[46] = NULL;
            /*_.CURBODYO__V49*/ meltfptr[42] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:2596:/ clear");
            /*clear*/ /*_.CURBODYPAIR__V48*/ meltfptr[46] = 0 ;
            /*^clear*/
            /*clear*/ /*_.CURBODYO__V49*/ meltfptr[42] = 0 ;
            /*^clear*/
            /*clear*/ /*_#IS_NOT_A__L17*/ meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V53*/ meltfptr[49] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2608:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "/* epilogue of ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V11*/ meltfptr[4];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " hook */\n       goto  meltlabend_rout\t\t;\
\n       meltlabend_rout:\t\t\t;\n       ";
            /*_.ADD2OUT__V57*/ meltfptr[55] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2615:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (1), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2616:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L21*/ meltfnum[7] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:2616:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L21*/ meltfnum[7]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L22*/ meltfnum[18] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:2616:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L22*/ meltfnum[18];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2616;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_hookroutine ohkepilog=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OHKEPILOG__V25*/ meltfptr[24];
                            /*_.MELT_DEBUG_FUN__V59*/ meltfptr[54] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V58*/ meltfptr[50] = /*_.MELT_DEBUG_FUN__V59*/ meltfptr[54];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2616:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L22*/ meltfnum[18] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V59*/ meltfptr[54] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V58*/ meltfptr[50] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:2616:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L21*/ meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V58*/ meltfptr[50] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:2617:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_LIST__L23*/ meltfnum[19] =
                (melt_magic_discr((melt_ptr_t)(/*_.OHKEPILOG__V25*/ meltfptr[24])) == MELTOBMAG_LIST);;
            MELT_LOCATION("warmelt-outobj.melt:2617:/ cond");
            /*cond*/ if (/*_#IS_LIST__L23*/ meltfnum[19]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V61*/ meltfptr[50] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2617:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check epilog";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2617;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OHKEPILOG__V25*/ meltfptr[24];
                            /*_.MELT_ASSERT_FAILURE_FUN__V62*/ meltfptr[61] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V61*/ meltfptr[50] = /*_.MELT_ASSERT_FAILURE_FUN__V62*/ meltfptr[61];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2617:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V62*/ meltfptr[61] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V60*/ meltfptr[54] = /*_._IFELSE___V61*/ meltfptr[50];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2617:/ clear");
            /*clear*/ /*_#IS_LIST__L23*/ meltfnum[19] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V61*/ meltfptr[50] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V60*/ meltfptr[54] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit2__EACHLIST */
            for (/*_.CUREPIPAIR__V63*/ meltfptr[61] = melt_list_first( (melt_ptr_t)/*_.OHKEPILOG__V25*/ meltfptr[24]);
                                       melt_magic_discr((melt_ptr_t) /*_.CUREPIPAIR__V63*/ meltfptr[61]) == MELTOBMAG_PAIR;
                                       /*_.CUREPIPAIR__V63*/ meltfptr[61] = melt_pair_tail((melt_ptr_t) /*_.CUREPIPAIR__V63*/ meltfptr[61]))
                {
                    /*_.CUREPINS__V64*/ meltfptr[50] = melt_pair_head((melt_ptr_t) /*_.CUREPIPAIR__V63*/ meltfptr[61]);



#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:2621:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        {
                            /*^locexp*/
                            /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                            melt_dbgcounter++;
#endif
                            ;
                        }
                        ;

                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#MELT_NEED_DBG__L24*/ meltfnum[18] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:2621:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L24*/ meltfnum[18]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L25*/ meltfnum[7] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:2621:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L25*/ meltfnum[7];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 2621;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "outpucod_hookroutine curepins=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CUREPINS__V64*/ meltfptr[50];
                                        /*_.MELT_DEBUG_FUN__V66*/ meltfptr[65] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V65*/ meltfptr[64] = /*_.MELT_DEBUG_FUN__V66*/ meltfptr[65];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:2621:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L25*/ meltfnum[7] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V66*/ meltfptr[65] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V65*/ meltfptr[64] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:2621:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L24*/ meltfnum[18] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V65*/ meltfptr[64] = 0 ;
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
                    /*_#IS_NOT_A__L26*/ meltfnum[19] =
                        !melt_is_instance_of((melt_ptr_t)(/*_.CUREPINS__V64*/ meltfptr[50]), (melt_ptr_t)((/*!CLASS_OBJPUREVALUE*/ meltfrout->tabval[13])));;
                    MELT_LOCATION("warmelt-outobj.melt:2622:/ cond");
                    /*cond*/ if (/*_#IS_NOT_A__L26*/ meltfnum[19]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:2623:/ msend");
                                /*msend*/
                                {
                                    union meltparam_un argtab[3];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^ojbmsend.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                    /*^ojbmsend.arg*/
                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                    /*^ojbmsend.arg*/
                                    argtab[2].meltbp_long = 1;
                                    /*_.OUTPUT_C_CODE__V67*/ meltfptr[65] = meltgc_send((melt_ptr_t)(/*_.CUREPINS__V64*/ meltfptr[50]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[12])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2624:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2625:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (1), 0);
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:2622:/ quasiblock");


                                /*epilog*/

                                /*^clear*/
                                /*clear*/ /*_.OUTPUT_C_CODE__V67*/ meltfptr[65] = 0 ;
                            }
                            ;
                        } /*noelse*/
                    ;
                } /* end foreach_pair_component_in_list meltcit2__EACHLIST */
            /*_.CUREPIPAIR__V63*/ meltfptr[61] = NULL;
            /*_.CUREPINS__V64*/ meltfptr[50] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:2618:/ clear");
            /*clear*/ /*_.CUREPIPAIR__V63*/ meltfptr[61] = 0 ;
            /*^clear*/
            /*clear*/ /*_.CUREPINS__V64*/ meltfptr[50] = 0 ;
            /*^clear*/
            /*clear*/ /*_#IS_NOT_A__L26*/ meltfnum[19] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2627:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "/* final of ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V11*/ meltfptr[4];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " hook */\n       return ";
            /*_.ADD2OUT__V68*/ meltfptr[64] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2631:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (3), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2632:/ cond");
        /*cond*/ if (/*_.ORETVAL__V18*/ meltfptr[17]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:2633:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = 1;
                        /*_.OUTPUT_C_CODE__V70*/ meltfptr[69] = meltgc_send((melt_ptr_t)(/*_.ORETVAL__V18*/ meltfptr[17]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[12])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V69*/ meltfptr[65] = /*_.OUTPUT_C_CODE__V70*/ meltfptr[69];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:2632:/ clear");
                    /*clear*/ /*_.OUTPUT_C_CODE__V70*/ meltfptr[69] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V69*/ meltfptr[65] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2634:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2635:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2636:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "} /* end of ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V11*/ meltfptr[4];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " hook */";
            /*_.ADD2OUT__V71*/ meltfptr[69] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2637:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2638:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "#undef meltframe"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2639:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2640:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "#undef meltfram__"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2641:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2642:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "#undef meltfhook"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2643:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2644:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "#undef meltcallcount"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2645:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2646:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2647:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2648:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (0), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2649:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "/** end of declaration for hook ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V11*/ meltfptr[4];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "**/";
            /*_.ADD2OUT__V72*/ meltfptr[71] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2650:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2651:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2652:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2653:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2654:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "/** end of implementation for hook ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.HOOKNAME__V11*/ meltfptr[4];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "**/";
            /*_.ADD2OUT__V73*/ meltfptr[72] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2655:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2656:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2657:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        MELT_LOCATION("warmelt-outobj.melt:2563:/ clear");
        /*clear*/ /*_.ODATALOCV__V36*/ meltfptr[35] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V39*/ meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V40*/ meltfptr[36] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V41*/ meltfptr[40] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V42*/ meltfptr[41] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V45*/ meltfptr[43] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V57*/ meltfptr[55] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V60*/ meltfptr[54] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V68*/ meltfptr[64] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V69*/ meltfptr[65] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V71*/ meltfptr[69] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V72*/ meltfptr[71] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V73*/ meltfptr[72] = 0 ;

        MELT_LOCATION("warmelt-outobj.melt:2512:/ clear");
        /*clear*/ /*_.HOOKNAME__V11*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NHOOKPROC__V12*/ meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NRAWHOOKNAME__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OBODY__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NBVAL__V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NBLONGS__V16*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OTHERVARS__V17*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ORETVAL__V18*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ORETCTYPE__V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ORETCTYCNAME__V20*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OHKDATARG__V21*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OHKDATACNAME__V22*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OHKINARGS__V23*/ meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OHKOUTARGS__V24*/ meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OHKEPILOG__V25*/ meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_#OBJ_HASH__L5*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_#pcIRAW__L6*/ meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#MINIHASH__L7*/ meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V28*/ meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V29*/ meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_CENTRALDECL_HOOKROUTINE__V30*/ meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V31*/ meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_CURFRAME_DECLSTRUCT__V32*/ meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V33*/ meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_?*/ meltfram__.loc_CSTRING__o0 = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_CENTRALDECL_HOOKROUTINE__V34*/ meltfptr[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V35*/ meltfptr[34] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:2508:/ clear");
        /*clear*/ /*_.IFCPP___V8*/ meltfptr[5] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_HOOKROUTINE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_52_WARMELTmiOUTOBJ_OUTPUCOD_HOOKROUTINE*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE fromline 1752 */

    /** start of frame for meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE// fromline 1568
        : public Melt_CallFrameWithValues<79>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[27];
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
        MeltFrame_meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<79> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE), clos) {};
        MeltFrame_meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE() //the constructor fromline 1642
            : Melt_CallFrameWithValues<79> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<79> (fil,lin, sizeof(MeltFrame_meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE)) {};
        MeltFrame_meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<79> (fil,lin, sizeof(MeltFrame_meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE


    /** end of frame for meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE fromline 1697**/

    /* end of frame for routine meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE fromline 1756 */

    /* classy proc frame meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE */ MeltFrame_meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_INITIALEXTENSIONROUTINE", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:2665:/ getarg");
    /*_.PINI__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:2666:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
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
            MELT_LOCATION("warmelt-outobj.melt:2666:/ cond");
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
                        /*^compute*/
                        /*_.DISCRIM__V6*/ meltfptr[5] =
                            ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.PINI__V2*/ meltfptr[1]))));;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:2666:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2666;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_initialextensionroutine start pini=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.PINI__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n of discrim ";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.DISCRIM__V6*/ meltfptr[5];
                            /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2666:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L3*/ meltfnum[2] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.DISCRIM__V6*/ meltfptr[5] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V5*/ meltfptr[4] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:2666:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:2668:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.PINI__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_INITIAL_EXTENSION_ROUTINEOBJ*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:2668:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2668:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check pini";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2668;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.PINI__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/ meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[4];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2668:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[4] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/ meltfptr[5] = /*_._IFELSE___V9*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2668:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/ meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2669:/ locexp");

#if MELT_HAVE_DEBUG
            if (melt_need_debug (0))
                melt_dbgshortbacktrace(( "outpucod_initialextensionroutine"), (20));
#endif
            ;
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2670:/ quasiblock");


        MELT_LOCATION("warmelt-outobj.melt:2671:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PINI__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PINI__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                    /*_.ONAME__V11*/ meltfptr[4] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ONAME__V11*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2672:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PINI__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_INITIALROUTINEOBJ*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PINI__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 12, "OIROUT_DATA");
                    /*_.ODATUP__V12*/ meltfptr[6] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ODATUP__V12*/ meltfptr[6] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2673:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PINI__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_INITIALROUTINEOBJ*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PINI__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 14, "OIROUT_FILL");
                    /*_.ORFILL__V13*/ meltfptr[12] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ORFILL__V13*/ meltfptr[12] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2674:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PINI__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_INITIALROUTINEOBJ*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PINI__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 13, "OIROUT_PROLOG");
                    /*_.OPROLOG__V14*/ meltfptr[13] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OPROLOG__V14*/ meltfptr[13] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2675:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.PINI__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 15, "OIROUT_MODCTX");
            /*_.OMODU__V15*/ meltfptr[14] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:2676:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OMODU__V15*/ meltfptr[14]),
                                          (melt_ptr_t)((/*!CLASS_ANY_MODULE_CONTEXT*/ meltfrout->tabval[5])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OMODU__V15*/ meltfptr[14]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "MOCX_MODULENAME");
                    /*_.OMODNAM__V16*/ meltfptr[15] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OMODNAM__V16*/ meltfptr[15] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2677:/ quasiblock");


        /*_.SBUF__V18*/ meltfptr[17] =
            (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[6])), (const char*)0);;

        {
            MELT_LOCATION("warmelt-outobj.melt:2679:/ locexp");
            meltgc_add_strbuf_cident((melt_ptr_t)(/*_.SBUF__V18*/ meltfptr[17]),
                                     melt_string_str((melt_ptr_t)(/*_.OMODNAM__V16*/ meltfptr[15])));
        }
        ;
        /*_.STRBUF2STRING__V19*/ meltfptr[18] =
            (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[7])), melt_strbuf_str((melt_ptr_t)(/*_.SBUF__V18*/ meltfptr[17]))));;
        /*^compute*/
        /*_.LET___V17*/ meltfptr[16] = /*_.STRBUF2STRING__V19*/ meltfptr[18];;

        MELT_LOCATION("warmelt-outobj.melt:2677:/ clear");
        /*clear*/ /*_.SBUF__V18*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.STRBUF2STRING__V19*/ meltfptr[18] = 0 ;
        /*_.OMODCNAME__V20*/ meltfptr[17] = /*_.LET___V17*/ meltfptr[16];;
        MELT_LOCATION("warmelt-outobj.melt:2681:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PINI__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_ROUTINEOBJ*/ meltfrout->tabval[8])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PINI__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 5, "OBROUT_BODY");
                    /*_.OBODY__V21*/ meltfptr[18] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OBODY__V21*/ meltfptr[18] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2682:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PINI__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_ROUTINEOBJ*/ meltfrout->tabval[8])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PINI__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 6, "OBROUT_NBVAL");
                    /*_.ONBVAL__V22*/ meltfptr[21] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ONBVAL__V22*/ meltfptr[21] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#NBVAL__L5*/ meltfnum[1] =
            (melt_get_int((melt_ptr_t)(/*_.ONBVAL__V22*/ meltfptr[21])));;
        /*^compute*/
        /*_#OBJ_HASH__L6*/ meltfnum[2] =
            (melt_obj_hash((melt_ptr_t)(/*_.PINI__V2*/ meltfptr[1])));;
        /*^compute*/
        /*_#pcIRAW__L7*/ meltfnum[6] =
            ((/*_#OBJ_HASH__L6*/ meltfnum[2]) % (4096));;
        /*^compute*/
        /*_#MINIHASH__L8*/ meltfnum[7] =
            ((1) + (/*_#pcIRAW__L7*/ meltfnum[6]));;
        MELT_LOCATION("warmelt-outobj.melt:2685:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PINI__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_ROUTINEOBJ*/ meltfrout->tabval[8])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PINI__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 9, "OBROUT_RETVAL");
                    /*_.ORETVAL__V23*/ meltfptr[22] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ORETVAL__V23*/ meltfptr[22] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2687:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2688:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (0), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2689:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[15];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  " /** declare initial extension running routine ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.ONAME__V11*/ meltfptr[4];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " \n* of ";
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OMODNAM__V16*/ meltfptr[15];
            /*^apply.arg*/
            argtab[4].meltbp_cstring =  " [outpucod_initialextensionroutine] **/\
\nMELT_EXTERN melt_ptr_t ";
            /*^apply.arg*/
            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.ONAME__V11*/ meltfptr[4];
            /*^apply.arg*/
            argtab[6].meltbp_cstring =  " (melt_ptr_t meltarg_curenvbox_p, melt_ptr_t meltarg_tuplitval_p) \
;\n\n#ifdef MELT_HAVE_DEBUG\nMELT_EXTERN const char meltextend_";
            /*^apply.arg*/
            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.OMODCNAME__V20*/ meltfptr[17];
            /*^apply.arg*/
            argtab[8].meltbp_cstring =  "_have_debug_enabled[] ;\n#define melt_have_debug_string meltextend_";
            /*^apply.arg*/
            argtab[9].meltbp_aptr = (melt_ptr_t*) &/*_.OMODCNAME__V20*/ meltfptr[17];
            /*^apply.arg*/
            argtab[10].meltbp_cstring =  "_have_debug_enabled\n#else\nMELT_EXTERN const char meltextend_";
            /*^apply.arg*/
            argtab[11].meltbp_aptr = (melt_ptr_t*) &/*_.OMODCNAME__V20*/ meltfptr[17];
            /*^apply.arg*/
            argtab[12].meltbp_cstring =  "_have_debug_disabled[] ;\n#define melt_have_debug_string meltextend_";
            /*^apply.arg*/
            argtab[13].meltbp_aptr = (melt_ptr_t*) &/*_.OMODCNAME__V20*/ meltfptr[17];
            /*^apply.arg*/
            argtab[14].meltbp_cstring =  "_have_debug_disabled\n#endif /* MELT_HAVE_DEBUG */\
\n\n     ";
            /*_.ADD2OUT__V24*/ meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2704:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (0), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2705:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[17];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  " /* implement extension running routine ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.ONAME__V11*/ meltfptr[4];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " \nof ";
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OMODNAM__V16*/ meltfptr[15];
            /*^apply.arg*/
            argtab[4].meltbp_cstring =  " [outpucod_initialextensionroutine] **/\
\n#ifdef MELT_HAVE_DEBUG\nconst char meltextend_";
            /*^apply.arg*/
            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.OMODCNAME__V20*/ meltfptr[17];
            /*^apply.arg*/
            argtab[6].meltbp_cstring =  "_have_debug_enabled[]\n= \"MELT running extension ";
            /*^apply.arg*/
            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.OMODNAM__V16*/ meltfptr[15];
            /*^apply.arg*/
            argtab[8].meltbp_cstring =  " has debug enabled\" ;\n#else\nconst char meltextend_";
            /*^apply.arg*/
            argtab[9].meltbp_aptr = (melt_ptr_t*) &/*_.OMODCNAME__V20*/ meltfptr[17];
            /*^apply.arg*/
            argtab[10].meltbp_cstring =  "_have_debug_disabled[]\n= \"MELT running extension ";
            /*^apply.arg*/
            argtab[11].meltbp_aptr = (melt_ptr_t*) &/*_.OMODNAM__V16*/ meltfptr[15];
            /*^apply.arg*/
            argtab[12].meltbp_cstring =  " has debug disabled\" ;\n#endif /* MELT_HAVE_DEBUG */\
\n\n\n/******* starting initial extend run routine ";
            /*^apply.arg*/
            argtab[13].meltbp_aptr = (melt_ptr_t*) &/*_.OMODNAM__V16*/ meltfptr[15];
            /*^apply.arg*/
            argtab[14].meltbp_cstring =  " ******/\nmelt_ptr_t \n";
            /*^apply.arg*/
            argtab[15].meltbp_aptr = (melt_ptr_t*) &/*_.ONAME__V11*/ meltfptr[4];
            /*^apply.arg*/
            argtab[16].meltbp_cstring =  " (melt_ptr_t meltarg_curenvbox_p, melt_ptr_t meltarg_tuplitval_p) \
\n{\n";
            /*_.ADD2OUT__V25*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2727:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[5];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "// extension routine ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.OMODNAM__V16*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " fromline ";
            /*^apply.arg*/
            argtab[3].meltbp_long = 2727;
            /*^apply.arg*/
            argtab[4].meltbp_cstring =  "\n";
            /*_.ADD2OUT__V26*/ meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2728:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*_.OUTPUT_CURFRAME_DECLSTRUCT__V27*/ meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_CURFRAME_DECLSTRUCT*/ meltfrout->tabval[10])), (melt_ptr_t)(/*_.PINI__V2*/ meltfptr[1]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2729:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (1), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2730:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[5];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "/* extension routine ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.OMODNAM__V16*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " initial frame decl fromline ";
            /*^apply.arg*/
            argtab[3].meltbp_long = 2730;
            /*^apply.arg*/
            argtab[4].meltbp_cstring =  " */ typedef ";
            /*_.ADD2OUT__V28*/ meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2731:/ apply");
        /*apply*/
        {
            /*_.INITIAL_CLASSY_FRAME_NAME__V29*/ meltfptr[28] =  melt_apply ((meltclosure_ptr_t)((/*!INITIAL_CLASSY_FRAME_NAME*/ meltfrout->tabval[11])), (melt_ptr_t)(/*_.PINI__V2*/ meltfptr[1]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.INITIAL_CLASSY_FRAME_NAME__V29*/ meltfptr[28];
            /*_.ADD2OUT__V30*/ meltfptr[29] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2732:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[5];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  " meltrun_";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.OMODCNAME__V20*/ meltfptr[17];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "_initialext_frame_t /*declaring runextend ";
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OMODNAM__V16*/ meltfptr[15];
            /*^apply.arg*/
            argtab[4].meltbp_cstring =  " frame type*/;";
            /*_.ADD2OUT__V31*/ meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2734:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]), (1), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2737:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (1), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2738:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "/* extension routine ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.OMODNAM__V16*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " initial frame init */";
            /*_.ADD2OUT__V32*/ meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2740:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V34*/ meltfptr[33] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_14*/ meltfrout->tabval[14])), (1));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V34*/ meltfptr[33])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V34*/ meltfptr[33])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V34*/ meltfptr[33])->tabval[0] = (melt_ptr_t)(/*_.OMODNAM__V16*/ meltfptr[15]);
        ;
        /*_.LAMBDA___V33*/ meltfptr[32] = /*_.LAMBDA___V34*/ meltfptr[33];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2739:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.PINI__V2*/ meltfptr[1];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*_.OUTPUT_CURFRAME_DECLSTRUCT_INIT__V35*/ meltfptr[34] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_CURFRAME_DECLSTRUCT_INIT*/ meltfrout->tabval[12])), (melt_ptr_t)(/*_.LAMBDA___V33*/ meltfptr[32]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2744:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2745:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  " /* extension ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.OMODNAM__V16*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " initialized initial frame */";
            /*_.ADD2OUT__V36*/ meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2747:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2748:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  " /* extension ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.OMODNAM__V16*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " cdata struct */";
            /*_.ADD2OUT__V37*/ meltfptr[36] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2749:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*_.OUTPUT_CURFRAME_CDAT_STRUCT__V38*/ meltfptr[37] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_CURFRAME_CDAT_STRUCT*/ meltfrout->tabval[15])), (melt_ptr_t)(/*_.ODATUP__V12*/ meltfptr[6]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2750:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " *meltcdat = NULL;"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2751:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (1), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2753:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L9*/ meltfnum[8] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:2753:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L9*/ meltfnum[8]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L10*/ meltfnum[9] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:2753:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L10*/ meltfnum[9];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2753;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_initialextensionroutine oprolog=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OPROLOG__V14*/ meltfptr[13];
                            /*_.MELT_DEBUG_FUN__V40*/ meltfptr[39] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V39*/ meltfptr[38] = /*_.MELT_DEBUG_FUN__V40*/ meltfptr[39];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2753:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L10*/ meltfnum[9] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V40*/ meltfptr[39] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V39*/ meltfptr[38] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:2753:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L9*/ meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V39*/ meltfptr[38] = 0 ;
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

        {
            MELT_LOCATION("warmelt-outobj.melt:2754:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (1), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2755:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  " /* extension ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.OMODNAM__V16*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " prologue */";
            /*_.ADD2OUT__V41*/ meltfptr[39] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit1__EACHLIST */
            for (/*_.CURPAIR__V42*/ meltfptr[38] = melt_list_first( (melt_ptr_t)/*_.OPROLOG__V14*/ meltfptr[13]);
                                    melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V42*/ meltfptr[38]) == MELTOBMAG_PAIR;
                                    /*_.CURPAIR__V42*/ meltfptr[38] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V42*/ meltfptr[38]))
                {
                    /*_.CURPROL__V43*/ meltfptr[42] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V42*/ meltfptr[38]);



                    MELT_CHECK_SIGNAL();
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:2759:/ cond");
                    /*cond*/ if (/*_.CURPROL__V43*/ meltfptr[42]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*_#IS_A__L12*/ meltfnum[8] =
                                    melt_is_instance_of((melt_ptr_t)(/*_.CURPROL__V43*/ meltfptr[42]), (melt_ptr_t)((/*!CLASS_OBJPUREVALUE*/ meltfrout->tabval[16])));;
                                /*^compute*/
                                /*_#NOT__L13*/ meltfnum[12] =
                                    (!(/*_#IS_A__L12*/ meltfnum[8]));;
                                /*^compute*/
                                /*_#_IF___L11*/ meltfnum[9] = /*_#NOT__L13*/ meltfnum[12];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:2759:/ clear");
                                /*clear*/ /*_#IS_A__L12*/ meltfnum[8] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#NOT__L13*/ meltfnum[12] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_#_IF___L11*/ meltfnum[9] = 0;;
                        }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:2759:/ cond");
                    /*cond*/ if (/*_#_IF___L11*/ meltfnum[9]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2760:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (1), 0);
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:2761:/ msend");
                                /*msend*/
                                {
                                    union meltparam_un argtab[3];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^ojbmsend.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                    /*^ojbmsend.arg*/
                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                    /*^ojbmsend.arg*/
                                    argtab[2].meltbp_long = 1;
                                    /*_.OUTPUT_C_CODE__V45*/ meltfptr[44] = meltgc_send((melt_ptr_t)(/*_.CURPROL__V43*/ meltfptr[42]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[17])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:2759:/ quasiblock");


                                /*_.PROGN___V46*/ meltfptr[45] = /*_.OUTPUT_C_CODE__V45*/ meltfptr[44];;
                                /*^compute*/
                                /*_._IF___V44*/ meltfptr[43] = /*_.PROGN___V46*/ meltfptr[45];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:2759:/ clear");
                                /*clear*/ /*_.OUTPUT_C_CODE__V45*/ meltfptr[44] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V46*/ meltfptr[45] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IF___V44*/ meltfptr[43] =  /*reallynil*/ NULL ;;
                        }
                    ;
                } /* end foreach_pair_component_in_list meltcit1__EACHLIST */
            /*_.CURPAIR__V42*/ meltfptr[38] = NULL;
            /*_.CURPROL__V43*/ meltfptr[42] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:2756:/ clear");
            /*clear*/ /*_.CURPAIR__V42*/ meltfptr[38] = 0 ;
            /*^clear*/
            /*clear*/ /*_.CURPROL__V43*/ meltfptr[42] = 0 ;
            /*^clear*/
            /*clear*/ /*_#_IF___L11*/ meltfnum[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V44*/ meltfptr[43] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2763:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (1), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2764:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  " /* extension ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.OMODNAM__V16*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " cdata fill */";
            /*_.ADD2OUT__V47*/ meltfptr[44] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2765:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*_.OUTPUT_CURFRAME_CDAT_FILL__V48*/ meltfptr[45] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_CURFRAME_CDAT_FILL*/ meltfrout->tabval[18])), (melt_ptr_t)(/*_.ODATUP__V12*/ meltfptr[6]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2766:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (1), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2767:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  " /* extension ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.OMODNAM__V16*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " cdata done fill */";
            /*_.ADD2OUT__V49*/ meltfptr[48] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2768:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (1), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2770:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L14*/ meltfnum[8] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:2770:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L14*/ meltfnum[8]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L15*/ meltfnum[12] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:2770:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L15*/ meltfnum[12];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2770;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_initialextensionroutine obody=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OBODY__V21*/ meltfptr[18];
                            /*_.MELT_DEBUG_FUN__V51*/ meltfptr[50] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V50*/ meltfptr[49] = /*_.MELT_DEBUG_FUN__V51*/ meltfptr[50];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2770:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L15*/ meltfnum[12] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V51*/ meltfptr[50] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V50*/ meltfptr[49] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:2770:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L14*/ meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V50*/ meltfptr[49] = 0 ;
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

        {
            MELT_LOCATION("warmelt-outobj.melt:2771:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (1), 0);
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:2772:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_LIST__L16*/ meltfnum[12] =
            (melt_magic_discr((melt_ptr_t)(/*_.OBODY__V21*/ meltfptr[18])) == MELTOBMAG_LIST);;
        MELT_LOCATION("warmelt-outobj.melt:2774:/ cond");
        /*cond*/ if (/*_#IS_LIST__L16*/ meltfnum[12]) /*then*/
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[20]);
                        /*_.LIST_TO_MULTIPLE__V53*/ meltfptr[49] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[19])), (melt_ptr_t)(/*_.OBODY__V21*/ meltfptr[18]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_.BODTUP__V52*/ meltfptr[50] = /*_.LIST_TO_MULTIPLE__V53*/ meltfptr[49];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:2774:/ clear");
                    /*clear*/ /*_.LIST_TO_MULTIPLE__V53*/ meltfptr[49] = 0 ;
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
                    /*_#IS_MULTIPLE__L17*/ meltfnum[8] =
                        (melt_magic_discr((melt_ptr_t)(/*_.OBODY__V21*/ meltfptr[18])) == MELTOBMAG_MULTIPLE);;
                    MELT_LOCATION("warmelt-outobj.melt:2775:/ cond");
                    /*cond*/ if (/*_#IS_MULTIPLE__L17*/ meltfnum[8]) /*then*/
                        {
                            /*^cond.then*/
                            /*_._IFELSE___V54*/ meltfptr[49] = /*_.OBODY__V21*/ meltfptr[18];;
                        }
                    else
                        {
                            MELT_LOCATION("warmelt-outobj.melt:2775:/ cond.else");

                            /*^block*/
                            /*anyblock*/
                            {


#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:2777:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*^cond*/
                                    /*cond*/ if ((/*nil*/NULL)) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*_._IFELSE___V56*/ meltfptr[55] = (/*nil*/NULL);;
                                        }
                                    else
                                        {
                                            MELT_LOCATION("warmelt-outobj.melt:2777:/ cond.else");

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
                                                    argtab[0].meltbp_cstring =  "unexpected body";
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 2777;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBODY__V21*/ meltfptr[18];
                                                    /*_.MELT_ASSERT_FAILURE_FUN__V57*/ meltfptr[56] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IFELSE___V56*/ meltfptr[55] = /*_.MELT_ASSERT_FAILURE_FUN__V57*/ meltfptr[56];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:2777:/ clear");
                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V57*/ meltfptr[56] = 0 ;
                                            }
                                            ;
                                        }
                                    ;
                                    /*_.IFCPP___V55*/ meltfptr[54] = /*_._IFELSE___V56*/ meltfptr[55];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:2777:/ clear");
                                    /*clear*/ /*_._IFELSE___V56*/ meltfptr[55] = 0 ;
                                }

#else /*MELT_HAVE_DEBUG*/
                                /*^cppif.else*/
                                /*_.IFCPP___V55*/ meltfptr[54] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:2776:/ quasiblock");


                                /*_.PROGN___V58*/ meltfptr[56] = /*_.IFCPP___V55*/ meltfptr[54];;
                                /*^compute*/
                                /*_._IFELSE___V54*/ meltfptr[49] = /*_.PROGN___V58*/ meltfptr[56];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:2775:/ clear");
                                /*clear*/ /*_.IFCPP___V55*/ meltfptr[54] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V58*/ meltfptr[56] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_.BODTUP__V52*/ meltfptr[50] = /*_._IFELSE___V54*/ meltfptr[49];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:2774:/ clear");
                    /*clear*/ /*_#IS_MULTIPLE__L17*/ meltfnum[8] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V54*/ meltfptr[49] = 0 ;
                }
                ;
            }
        ;
        /*_#NBBODY__L18*/ meltfnum[8] =
            (melt_multiple_length((melt_ptr_t)(/*_.BODTUP__V52*/ meltfptr[50])));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2780:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L19*/ meltfnum[18] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:2780:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L19*/ meltfnum[18]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L20*/ meltfnum[19] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:2780:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L20*/ meltfnum[19];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2780;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_initialextensionroutine bodtup=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.BODTUP__V52*/ meltfptr[50];
                            /*_.MELT_DEBUG_FUN__V60*/ meltfptr[54] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V59*/ meltfptr[55] = /*_.MELT_DEBUG_FUN__V60*/ meltfptr[54];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2780:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L20*/ meltfnum[19] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V60*/ meltfptr[54] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V59*/ meltfptr[55] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:2780:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L19*/ meltfnum[18] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V59*/ meltfptr[55] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:2781:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[5];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  " /* extension ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.OMODNAM__V16*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " body of ";
            /*^apply.arg*/
            argtab[3].meltbp_long = /*_#NBBODY__L18*/ meltfnum[8];
            /*^apply.arg*/
            argtab[4].meltbp_cstring =  " instructions */";
            /*_.ADD2OUT__V61*/ meltfptr[56] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*citerblock FOREACH_IN_MULTIPLE*/
        {
            /* start foreach_in_multiple meltcit2__EACHTUP */
            long  meltcit2__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.BODTUP__V52*/ meltfptr[50]);
            for (/*_#BODIX__L21*/ meltfnum[19] = 0;
                                  (/*_#BODIX__L21*/ meltfnum[19] >= 0) && (/*_#BODIX__L21*/ meltfnum[19] <  meltcit2__EACHTUP_ln);
                                  /*_#BODIX__L21*/ meltfnum[19]++)
                {
                    /*_.CURBODY__V62*/ meltfptr[49] = melt_multiple_nth((melt_ptr_t)(/*_.BODTUP__V52*/ meltfptr[50]),  /*_#BODIX__L21*/ meltfnum[19]);




#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:2785:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        {
                            /*^locexp*/
                            /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                            melt_dbgcounter++;
#endif
                            ;
                        }
                        ;

                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#MELT_NEED_DBG__L22*/ meltfnum[18] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:2785:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L22*/ meltfnum[18]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L23*/ meltfnum[22] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;
                                    /*^compute*/
                                    /*_.DISCRIM__V64*/ meltfptr[55] =
                                        ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.CURBODY__V62*/ meltfptr[49]))));;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:2785:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[10];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L23*/ meltfnum[22];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 2785;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "outpucod_initialextensionroutine bodix=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_long = /*_#BODIX__L21*/ meltfnum[19];
                                        /*^apply.arg*/
                                        argtab[5].meltbp_cstring =  "\n curbody=";
                                        /*^apply.arg*/
                                        argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.CURBODY__V62*/ meltfptr[49];
                                        /*^apply.arg*/
                                        argtab[7].meltbp_cstring =  "\n of discrim=";
                                        /*^apply.arg*/
                                        argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.DISCRIM__V64*/ meltfptr[55];
                                        /*^apply.arg*/
                                        argtab[9].meltbp_cstring =  "\n";
                                        /*_.MELT_DEBUG_FUN__V65*/ meltfptr[64] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V63*/ meltfptr[54] = /*_.MELT_DEBUG_FUN__V65*/ meltfptr[64];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:2785:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L23*/ meltfnum[22] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.DISCRIM__V64*/ meltfptr[55] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V65*/ meltfptr[64] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V63*/ meltfptr[54] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:2785:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L22*/ meltfnum[18] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V63*/ meltfptr[54] = 0 ;
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

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:2788:/ cond");
                    /*cond*/ if (/*_.CURBODY__V62*/ meltfptr[49]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*_#IS_NOT_A__L25*/ meltfnum[18] =
                                    !melt_is_instance_of((melt_ptr_t)(/*_.CURBODY__V62*/ meltfptr[49]), (melt_ptr_t)((/*!CLASS_OBJPUREVALUE*/ meltfrout->tabval[16])));;
                                /*^compute*/
                                /*_#_IF___L24*/ meltfnum[22] = /*_#IS_NOT_A__L25*/ meltfnum[18];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:2788:/ clear");
                                /*clear*/ /*_#IS_NOT_A__L25*/ meltfnum[18] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_#_IF___L24*/ meltfnum[22] = 0;;
                        }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:2788:/ cond");
                    /*cond*/ if (/*_#_IF___L24*/ meltfnum[22]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:2789:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    {
                                        /*^locexp*/
                                        /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                                        melt_dbgcounter++;
#endif
                                        ;
                                    }
                                    ;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*_#MELT_NEED_DBG__L26*/ meltfnum[18] =
                                        /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                        (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                        0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                        ;;
                                    MELT_LOCATION("warmelt-outobj.melt:2789:/ cond");
                                    /*cond*/ if (/*_#MELT_NEED_DBG__L26*/ meltfnum[18]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*^block*/
                                            /*anyblock*/
                                            {

                                                /*_#MELT_CALLCOUNT__L27*/ meltfnum[26] =
                                                    /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                                    meltcallcount  /* melt_callcount debugging */
#else
                                                    0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                                    ;;

                                                MELT_CHECK_SIGNAL();
                                                ;
                                                MELT_LOCATION("warmelt-outobj.melt:2789:/ apply");
                                                /*apply*/
                                                {
                                                    union meltparam_un argtab[7];
                                                    memset(&argtab, 0, sizeof(argtab));
                                                    /*^apply.arg*/
                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L27*/ meltfnum[26];
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 2789;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_cstring =  "outpucod_initialextensionroutine good bodix=";
                                                    /*^apply.arg*/
                                                    argtab[4].meltbp_long = /*_#BODIX__L21*/ meltfnum[19];
                                                    /*^apply.arg*/
                                                    argtab[5].meltbp_cstring =  "\n real curbody=";
                                                    /*^apply.arg*/
                                                    argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.CURBODY__V62*/ meltfptr[49];
                                                    /*_.MELT_DEBUG_FUN__V68*/ meltfptr[54] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IF___V67*/ meltfptr[64] = /*_.MELT_DEBUG_FUN__V68*/ meltfptr[54];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:2789:/ clear");
                                                /*clear*/ /*_#MELT_CALLCOUNT__L27*/ meltfnum[26] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_.MELT_DEBUG_FUN__V68*/ meltfptr[54] = 0 ;
                                            }
                                            ;
                                        }
                                    else    /*^cond.else*/
                                        {

                                            /*_._IF___V67*/ meltfptr[64] =  /*reallynil*/ NULL ;;
                                        }
                                    ;

                                    {
                                        MELT_LOCATION("warmelt-outobj.melt:2789:/ locexp");
                                        /*void*/(void)0;
                                    }
                                    ;
                                    /*^quasiblock*/


                                    /*epilog*/

                                    /*^clear*/
                                    /*clear*/ /*_#MELT_NEED_DBG__L26*/ meltfnum[18] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IF___V67*/ meltfptr[64] = 0 ;
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

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2790:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (1), 0);
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:2791:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[5];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_cstring =  " /*sideffecting ";
                                    /*^apply.arg*/
                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.OMODNAM__V16*/ meltfptr[15];
                                    /*^apply.arg*/
                                    argtab[2].meltbp_cstring =  " extension body #";
                                    /*^apply.arg*/
                                    argtab[3].meltbp_long = /*_#BODIX__L21*/ meltfnum[19];
                                    /*^apply.arg*/
                                    argtab[4].meltbp_cstring =  " */";
                                    /*_.ADD2OUT__V69*/ meltfptr[54] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2792:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (1), 0);
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:2793:/ quasiblock");


                                /*^cond*/
                                /*cond*/ if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CURBODY__V62*/ meltfptr[49]),
                                                                  (melt_ptr_t)((/*!CLASS_OBJINSTR*/ meltfrout->tabval[21])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CURBODY__V62*/ meltfptr[49]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 0, "OBI_LOC");
                                            /*_.CURLOC__V71*/ meltfptr[70] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.CURLOC__V71*/ meltfptr[70] =  /*reallynil*/ NULL ;;
                                    }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:2795:/ cond");
                                /*cond*/ if (/*_.CURLOC__V71*/ meltfptr[70]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-outobj.melt:2796:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[3];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                /*^apply.arg*/
                                                argtab[1].meltbp_long = 1;
                                                /*^apply.arg*/
                                                argtab[2].meltbp_cstring =  "curbody";
                                                /*_.OUTPUT_LOCATION__V73*/ meltfptr[72] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[22])), (melt_ptr_t)(/*_.CURLOC__V71*/ meltfptr[70]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                            /*_._IF___V72*/ meltfptr[71] = /*_.OUTPUT_LOCATION__V73*/ meltfptr[72];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:2795:/ clear");
                                            /*clear*/ /*_.OUTPUT_LOCATION__V73*/ meltfptr[72] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_._IF___V72*/ meltfptr[71] =  /*reallynil*/ NULL ;;
                                    }
                                ;
                                /*^compute*/
                                /*_.LET___V70*/ meltfptr[64] = /*_._IF___V72*/ meltfptr[71];;

                                MELT_LOCATION("warmelt-outobj.melt:2793:/ clear");
                                /*clear*/ /*_.CURLOC__V71*/ meltfptr[70] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IF___V72*/ meltfptr[71] = 0 ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:2797:/ msend");
                                /*msend*/
                                {
                                    union meltparam_un argtab[3];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^ojbmsend.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                    /*^ojbmsend.arg*/
                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                    /*^ojbmsend.arg*/
                                    argtab[2].meltbp_long = 1;
                                    /*_.OUTPUT_C_CODE__V74*/ meltfptr[72] = meltgc_send((melt_ptr_t)(/*_.CURBODY__V62*/ meltfptr[49]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[17])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:2788:/ quasiblock");


                                /*_.PROGN___V75*/ meltfptr[70] = /*_.OUTPUT_C_CODE__V74*/ meltfptr[72];;
                                /*^compute*/
                                /*_._IF___V66*/ meltfptr[55] = /*_.PROGN___V75*/ meltfptr[70];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:2788:/ clear");
                                /*clear*/ /*_.ADD2OUT__V69*/ meltfptr[54] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.LET___V70*/ meltfptr[64] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.OUTPUT_C_CODE__V74*/ meltfptr[72] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V75*/ meltfptr[70] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IF___V66*/ meltfptr[55] =  /*reallynil*/ NULL ;;
                        }
                    ;
                    if (/*_#BODIX__L21*/ meltfnum[19]<0) break;
                } /* end  foreach_in_multiple meltcit2__EACHTUP */

            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:2782:/ clear");
            /*clear*/ /*_.CURBODY__V62*/ meltfptr[49] = 0 ;
            /*^clear*/
            /*clear*/ /*_#BODIX__L21*/ meltfnum[19] = 0 ;
            /*^clear*/
            /*clear*/ /*_#_IF___L24*/ meltfnum[22] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V66*/ meltfptr[55] = 0 ;
        } /*endciterblock FOREACH_IN_MULTIPLE*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2800:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (1), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2801:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  " /*-- ending run extension ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.OMODNAM__V16*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "  --*/";
            /*_.ADD2OUT__V76*/ meltfptr[71] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2802:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "\n goto meltlabend_rout;\nmeltlabend_rout:\
\n  return (melt_ptr_t)";
            /*_.ADD2OUT__V77*/ meltfptr[54] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2806:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = 1;
            /*_.OUTPUT_C_CODE__V78*/ meltfptr[64] = meltgc_send((melt_ptr_t)(/*_.ORETVAL__V23*/ meltfptr[22]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[17])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2807:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[5];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "; /*return value of extension ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.OMODNAM__V16*/ meltfptr[15];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " */\n} /* end extension ";
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ONAME__V11*/ meltfptr[4];
            /*^apply.arg*/
            argtab[4].meltbp_cstring =  " */";
            /*_.ADD2OUT__V79*/ meltfptr[72] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2809:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2810:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2811:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2812:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        MELT_LOCATION("warmelt-outobj.melt:2772:/ clear");
        /*clear*/ /*_#IS_LIST__L16*/ meltfnum[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BODTUP__V52*/ meltfptr[50] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NBBODY__L18*/ meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V61*/ meltfptr[56] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V76*/ meltfptr[71] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V77*/ meltfptr[54] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V78*/ meltfptr[64] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V79*/ meltfptr[72] = 0 ;

        MELT_LOCATION("warmelt-outobj.melt:2670:/ clear");
        /*clear*/ /*_.ONAME__V11*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ODATUP__V12*/ meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ORFILL__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OPROLOG__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OMODU__V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OMODNAM__V16*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V17*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OMODCNAME__V20*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OBODY__V21*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ONBVAL__V22*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NBVAL__L5*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_#OBJ_HASH__L6*/ meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#pcIRAW__L7*/ meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_#MINIHASH__L8*/ meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ORETVAL__V23*/ meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V24*/ meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V25*/ meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V26*/ meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_CURFRAME_DECLSTRUCT__V27*/ meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V28*/ meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_.INITIAL_CLASSY_FRAME_NAME__V29*/ meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V30*/ meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V31*/ meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V32*/ meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LAMBDA___V33*/ meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_CURFRAME_DECLSTRUCT_INIT__V35*/ meltfptr[34] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V36*/ meltfptr[35] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V37*/ meltfptr[36] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_CURFRAME_CDAT_STRUCT__V38*/ meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V41*/ meltfptr[39] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V47*/ meltfptr[44] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_CURFRAME_CDAT_FILL__V48*/ meltfptr[45] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V49*/ meltfptr[48] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:2665:/ clear");
        /*clear*/ /*_.IFCPP___V8*/ meltfptr[5] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_INITIALEXTENSIONROUTINE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_53_WARMELTmiOUTOBJ_OUTPUCOD_INITIALEXTENSIONROUTINE*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4 fromline 1752 */

    /** start of frame for meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4 of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4// fromline 1568
        : public Melt_CallFrameWithValues<4>
    {
    public: /* fromline 1572*/
// no classy longs
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
        MeltFrame_meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<4> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4), clos) {};
        MeltFrame_meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4() //the constructor fromline 1642
            : Melt_CallFrameWithValues<4> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<4> (fil,lin, sizeof(MeltFrame_meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4)) {};
        MeltFrame_meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<4> (fil,lin, sizeof(MeltFrame_meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4


    /** end of frame for meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4 fromline 1697**/

    /* end of frame for routine meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4 fromline 1756 */

    /* classy proc frame meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4 */ MeltFrame_meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4 fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl4", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:2740:/ getarg");
    /*_.ROU__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.DSBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.DSBUF__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


        {
            MELT_LOCATION("warmelt-outobj.melt:2741:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.DSBUF__V3*/ meltfptr[2]), (0), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2742:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[5];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  " /* extension routine ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*~OMODNAM*/ meltfclos->tabval[0]);
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " initializing frame fromline ";
            /*^apply.arg*/
            argtab[3].meltbp_long = 2742;
            /*^apply.arg*/
            argtab[4].meltbp_cstring =  " */ ";
            /*_.ADD2OUT__V4*/ meltfptr[3] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[0])), (melt_ptr_t)(/*_.DSBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2740:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.ADD2OUT__V4*/ meltfptr[3];;

        {
            MELT_LOCATION("warmelt-outobj.melt:2740:/ locexp");
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
        /*clear*/ /*_.ADD2OUT__V4*/ meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl4", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_54_WARMELTmiOUTOBJ_LAMBDA_cl4*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG fromline 1752 */

    /** start of frame for meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG// fromline 1568
        : public Melt_CallFrameWithValues<38>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[18];
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
        MeltFrame_meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<38> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG), clos) {};
        MeltFrame_meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG() //the constructor fromline 1642
            : Melt_CallFrameWithValues<38> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<38> (fil,lin, sizeof(MeltFrame_meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG)) {};
        MeltFrame_meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<38> (fil,lin, sizeof(MeltFrame_meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG


    /** end of frame for meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG fromline 1697**/

    /* end of frame for routine meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG fromline 1756 */

    /* classy proc frame meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG */ MeltFrame_meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_GETARG", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:2824:/ getarg");
    /*_.GARG__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:2825:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.GARG__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJGETARG*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:2825:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2825:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check garg";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2825;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.GARG__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2825:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2825:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:2826:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.GARG__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBARG_OBLOC");
            /*_.OLOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:2827:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.GARG__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.NLOC__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:2828:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.GARG__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBARG_BIND");
            /*_.OBIND__V11*/ meltfptr[10] = slot;
        };
        ;
        /*_#RKBIND__L3*/ meltfnum[1] =
            (melt_get_int((melt_ptr_t)(/*_.OBIND__V11*/ meltfptr[10])));;
        MELT_LOCATION("warmelt-outobj.melt:2830:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBIND__V11*/ meltfptr[10]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "FBIND_TYPE");
            /*_.CTYBIND__V12*/ meltfptr[11] = slot;
        };
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2832:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[3] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBIND__V11*/ meltfptr[10]), (melt_ptr_t)((/*!CLASS_FORMAL_BINDING*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-outobj.melt:2832:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V14*/ meltfptr[13] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2832:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check obind";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2832;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBIND__V11*/ meltfptr[10];
                            /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V14*/ meltfptr[13] = /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2832:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V13*/ meltfptr[12] = /*_._IFELSE___V14*/ meltfptr[13];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2832:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[3] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:2833:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "getarg";
            /*_.OUTPUT_LOCATION__V16*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.NLOC__V10*/ meltfptr[9]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2834:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L5*/ meltfnum[3] =
                melt_is_instance_of((melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (melt_ptr_t)((/*!CLASS_OBJLOCV*/ meltfrout->tabval[4])));;
            MELT_LOCATION("warmelt-outobj.melt:2834:/ cond");
            /*cond*/ if (/*_#IS_A__L5*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V18*/ meltfptr[17] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2834:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oloc";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2834;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OLOC__V9*/ meltfptr[5];
                            /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V18*/ meltfptr[17] = /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2834:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V17*/ meltfptr[13] = /*_._IFELSE___V18*/ meltfptr[17];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2834:/ clear");
            /*clear*/ /*_#IS_A__L5*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V18*/ meltfptr[17] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V17*/ meltfptr[13] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2835:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L6*/ meltfnum[3] =
                melt_is_instance_of((melt_ptr_t)(/*_.CTYBIND__V12*/ meltfptr[11]), (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[5])));;
            MELT_LOCATION("warmelt-outobj.melt:2835:/ cond");
            /*cond*/ if (/*_#IS_A__L6*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V21*/ meltfptr[17] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2835:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check ctybind";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2835;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CTYBIND__V12*/ meltfptr[11];
                            /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V21*/ meltfptr[17] = /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2835:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V20*/ meltfptr[18] = /*_._IFELSE___V21*/ meltfptr[17];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2835:/ clear");
            /*clear*/ /*_#IS_A__L6*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V21*/ meltfptr[17] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V20*/ meltfptr[18] = (/*nil*/NULL);
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.GARG__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V24*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V23*/ meltfptr[21] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V24*/ meltfptr[17];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V24*/ meltfptr[17] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V23*/ meltfptr[21] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#eqeqI__L7*/ meltfnum[3] =
            ((/*_#RKBIND__L3*/ meltfnum[1]) == (0));;
        MELT_LOCATION("warmelt-outobj.melt:2837:/ cond");
        /*cond*/ if (/*_#eqeqI__L7*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:2839:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#eqeq__L8*/ meltfnum[7] =
                            ((/*_.CTYBIND__V12*/ meltfptr[11]) == ((/*!CTYPE_VALUE*/ meltfrout->tabval[7])));;
                        MELT_LOCATION("warmelt-outobj.melt:2839:/ cond");
                        /*cond*/ if (/*_#eqeq__L8*/ meltfnum[7]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V26*/ meltfptr[25] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:2839:/ cond.else");

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
                                        argtab[0].meltbp_cstring =  "check ctybind first";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 2839;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CTYBIND__V12*/ meltfptr[11];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V26*/ meltfptr[25] = /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:2839:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V25*/ meltfptr[17] = /*_._IFELSE___V26*/ meltfptr[25];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2839:/ clear");
                        /*clear*/ /*_#eqeq__L8*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V26*/ meltfptr[25] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V25*/ meltfptr[17] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:2840:/ msend");
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
                        /*_.OUTPUT_C_CODE__V28*/ meltfptr[26] = meltgc_send((melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[8])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2841:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = (melt_ptr_t) meltfirstargp_;"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2842:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:2838:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:2837:/ clear");
                    /*clear*/ /*_.IFCPP___V25*/ meltfptr[17] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_C_CODE__V28*/ meltfptr[26] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-outobj.melt:2844:/ quasiblock");


                    MELT_LOCATION("warmelt-outobj.melt:2846:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.CTYBIND__V12*/ meltfptr[11]) /*=obj*/;
                        melt_object_get_field(slot,obj, 5, "CTYPE_PARCHAR");
                        /*_.PARC__V29*/ meltfptr[25] = slot;
                    };
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:2847:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.CTYBIND__V12*/ meltfptr[11]) /*=obj*/;
                        melt_object_get_field(slot,obj, 7, "CTYPE_ARGFIELD");
                        /*_.ARGF__V30*/ meltfptr[17] = slot;
                    };
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_STRING__L9*/ meltfnum[7] =
                        (melt_magic_discr((melt_ptr_t)(/*_.PARC__V29*/ meltfptr[25])) == MELTOBMAG_STRING);;
                    /*^compute*/
                    /*_#NOT__L10*/ meltfnum[9] =
                        (!(/*_#IS_STRING__L9*/ meltfnum[7]));;
                    MELT_LOCATION("warmelt-outobj.melt:2849:/ cond");
                    /*cond*/ if (/*_#NOT__L10*/ meltfnum[9]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-outobj.melt:2851:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.CTYBIND__V12*/ meltfptr[11]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                    /*_.NAMED_NAME__V31*/ meltfptr[26] = slot;
                                };
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2850:/ locexp");
                                    melt_error_str((melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), ( "impossible argument ctype"), (melt_ptr_t)(/*_.NAMED_NAME__V31*/ meltfptr[26]));
                                }
                                ;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:2849:/ clear");
                                /*clear*/ /*_.NAMED_NAME__V31*/ meltfptr[26] = 0 ;
                            }
                            ;
                        } /*noelse*/
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2852:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "if (meltxargdescr_["));
                    }
                    ;
                    /*_#miI__L11*/ meltfnum[10] =
                        ((/*_#RKBIND__L3*/ meltfnum[1]) - (1));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2853:/ locexp");
                        meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#miI__L11*/ meltfnum[10]));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2854:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "] != "));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2855:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                              melt_string_str((melt_ptr_t)(/*_.PARC__V29*/ meltfptr[25])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2856:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ") goto meltlab_endgetargs;"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2857:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#eqeq__L12*/ meltfnum[11] =
                        ((/*_.CTYBIND__V12*/ meltfptr[11]) == ((/*!CTYPE_VALUE*/ meltfrout->tabval[7])));;
                    MELT_LOCATION("warmelt-outobj.melt:2858:/ cond");
                    /*cond*/ if (/*_#eqeq__L12*/ meltfnum[11]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:2860:/ msend");
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
                                    /*_.OUTPUT_C_CODE__V32*/ meltfptr[26] = meltgc_send((melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[8])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2861:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = (meltxargtab_["));
                                }
                                ;
                                /*_#miI__L13*/ meltfnum[12] =
                                    ((/*_#RKBIND__L3*/ meltfnum[1]) - (1));;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2862:/ locexp");
                                    meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#miI__L13*/ meltfnum[12]));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2863:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "].meltbp_aptr) ? (*(meltxargtab_["));
                                }
                                ;
                                /*_#miI__L14*/ meltfnum[13] =
                                    ((/*_#RKBIND__L3*/ meltfnum[1]) - (1));;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2864:/ locexp");
                                    meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#miI__L14*/ meltfnum[13]));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2865:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "].meltbp_aptr)) : NULL;"));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2866:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2867:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "gcc_assert(melt_discr((melt_ptr_t)("));
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:2868:/ msend");
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
                                    /*_.OUTPUT_C_CODE__V33*/ meltfptr[32] = meltgc_send((melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[8])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2869:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ")) != NULL);"));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2870:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:2859:/ quasiblock");


                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:2858:/ clear");
                                /*clear*/ /*_.OUTPUT_C_CODE__V32*/ meltfptr[26] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#miI__L13*/ meltfnum[12] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#miI__L14*/ meltfnum[13] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.OUTPUT_C_CODE__V33*/ meltfptr[32] = 0 ;
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
                                MELT_LOCATION("warmelt-outobj.melt:2873:/ msend");
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
                                    /*_.OUTPUT_C_CODE__V34*/ meltfptr[26] = meltgc_send((melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[8])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2874:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = meltxargtab_["));
                                }
                                ;
                                /*_#miI__L15*/ meltfnum[12] =
                                    ((/*_#RKBIND__L3*/ meltfnum[1]) - (1));;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2875:/ locexp");
                                    meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#miI__L15*/ meltfnum[12]));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2876:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "]."));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2877:/ locexp");
                                    /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                                          melt_string_str((melt_ptr_t)(/*_.ARGF__V30*/ meltfptr[17])));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2878:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:2872:/ quasiblock");


                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:2858:/ clear");
                                /*clear*/ /*_.OUTPUT_C_CODE__V34*/ meltfptr[26] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#miI__L15*/ meltfnum[12] = 0 ;
                            }
                            ;
                        }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2881:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;

                    MELT_LOCATION("warmelt-outobj.melt:2844:/ clear");
                    /*clear*/ /*_.PARC__V29*/ meltfptr[25] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ARGF__V30*/ meltfptr[17] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#IS_STRING__L9*/ meltfnum[7] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#NOT__L10*/ meltfnum[9] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#miI__L11*/ meltfnum[10] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#eqeq__L12*/ meltfnum[11] = 0 ;
                    /*epilog*/
                }
                ;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2883:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L16*/ meltfnum[13] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:2884:/ cond");
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[9])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[10])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[9])) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                        /*_.REFERENCED_VALUE__V36*/ meltfptr[26] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V36*/ meltfptr[26] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L17*/ meltfnum[12] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V36*/ meltfptr[26])));;
            /*^compute*/
            /*_#ltI__L18*/ meltfnum[7] =
                ((/*_#STRBUF_USEDLENGTH__L16*/ meltfnum[13]) < (/*_#GET_INT__L17*/ meltfnum[12]));;
            MELT_LOCATION("warmelt-outobj.melt:2883:/ cond");
            /*cond*/ if (/*_#ltI__L18*/ meltfnum[7]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V37*/ meltfptr[25] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2883:/ cond.else");

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
                            argtab[2].meltbp_long = 2883;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V38*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V37*/ meltfptr[25] = /*_.MELT_ASSERT_FAILURE_FUN__V38*/ meltfptr[17];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2883:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V38*/ meltfptr[17] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V35*/ meltfptr[32] = /*_._IFELSE___V37*/ meltfptr[25];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2883:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L16*/ meltfnum[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V36*/ meltfptr[26] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L17*/ meltfnum[12] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L18*/ meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V37*/ meltfptr[25] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V35*/ meltfptr[32] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V35*/ meltfptr[32];;

        MELT_LOCATION("warmelt-outobj.melt:2826:/ clear");
        /*clear*/ /*_.OLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NLOC__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OBIND__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_#RKBIND__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CTYBIND__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V16*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V17*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V20*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V23*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_#eqeqI__L7*/ meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V35*/ meltfptr[32] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2824:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:2824:/ locexp");
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
    melt_trace_end("OUTPUCOD_GETARG", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_55_WARMELTmiOUTOBJ_OUTPUCOD_GETARG*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST fromline 1752 */

    /** start of frame for meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST// fromline 1568
        : public Melt_CallFrameWithValues<41>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[20];
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
        MeltFrame_meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<41> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST), clos) {};
        MeltFrame_meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<41> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<41> (fil,lin, sizeof(MeltFrame_meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST)) {};
        MeltFrame_meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<41> (fil,lin, sizeof(MeltFrame_meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST


    /** end of frame for meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST fromline 1697**/

    /* end of frame for routine meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST fromline 1756 */

    /* classy proc frame meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST */ MeltFrame_meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_GETARGREST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:2890:/ getarg");
    /*_.GARG__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:2891:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.GARG__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJGETARGREST*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:2891:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2891:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check garg";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2891;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.GARG__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2891:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2891:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:2892:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.GARG__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBARG_OBLOC");
            /*_.OLOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:2893:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.GARG__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.NLOC__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:2894:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.GARG__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBARG_BIND");
            /*_.OBIND__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:2895:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.GARG__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJGETARGREST*/ meltfrout->tabval[0])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.GARG__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 3, "OBARG_REST");
                    /*_.OREST__V12*/ meltfptr[11] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OREST__V12*/ meltfptr[11] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#RKBIND__L3*/ meltfnum[1] =
            (melt_get_int((melt_ptr_t)(/*_.OBIND__V11*/ meltfptr[10])));;
        MELT_LOCATION("warmelt-outobj.melt:2897:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBIND__V11*/ meltfptr[10]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "FBIND_TYPE");
            /*_.CTYBIND__V13*/ meltfptr[12] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2898:/ apply");
        /*apply*/
        {
            /*_.OVARIADICINDEX__V14*/ meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!VARIADIC_INDEX_IDSTR*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.OREST__V12*/ meltfptr[11]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2899:/ apply");
        /*apply*/
        {
            /*_.OVARIADICLENGTH__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!VARIADIC_LENGTH_IDSTR*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.OREST__V12*/ meltfptr[11]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2901:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[3] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBIND__V11*/ meltfptr[10]), (melt_ptr_t)((/*!CLASS_FORMAL_BINDING*/ meltfrout->tabval[4])));;
            MELT_LOCATION("warmelt-outobj.melt:2901:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V17*/ meltfptr[16] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2901:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check obind";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2901;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBIND__V11*/ meltfptr[10];
                            /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V17*/ meltfptr[16] = /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2901:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V16*/ meltfptr[15] = /*_._IFELSE___V17*/ meltfptr[16];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2901:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V17*/ meltfptr[16] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V16*/ meltfptr[15] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2902:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "getarg";
            /*_.OUTPUT_LOCATION__V19*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.NLOC__V10*/ meltfptr[9]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2903:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L5*/ meltfnum[3] =
                melt_is_instance_of((melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (melt_ptr_t)((/*!CLASS_OBJLOCV*/ meltfrout->tabval[6])));;
            MELT_LOCATION("warmelt-outobj.melt:2903:/ cond");
            /*cond*/ if (/*_#IS_A__L5*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V21*/ meltfptr[20] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2903:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oloc";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2903;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OLOC__V9*/ meltfptr[5];
                            /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V21*/ meltfptr[20] = /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2903:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V20*/ meltfptr[16] = /*_._IFELSE___V21*/ meltfptr[20];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2903:/ clear");
            /*clear*/ /*_#IS_A__L5*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V21*/ meltfptr[20] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V20*/ meltfptr[16] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2904:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L6*/ meltfnum[3] =
                melt_is_instance_of((melt_ptr_t)(/*_.CTYBIND__V13*/ meltfptr[12]), (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[7])));;
            MELT_LOCATION("warmelt-outobj.melt:2904:/ cond");
            /*cond*/ if (/*_#IS_A__L6*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V24*/ meltfptr[20] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2904:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check ctybind";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2904;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CTYBIND__V13*/ meltfptr[12];
                            /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V24*/ meltfptr[20] = /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2904:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V23*/ meltfptr[21] = /*_._IFELSE___V24*/ meltfptr[20];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2904:/ clear");
            /*clear*/ /*_#IS_A__L6*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V24*/ meltfptr[20] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V23*/ meltfptr[21] = (/*nil*/NULL);
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.GARG__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V27*/ meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V26*/ meltfptr[24] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V27*/ meltfptr[20];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V27*/ meltfptr[20] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V26*/ meltfptr[24] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#eqeqI__L7*/ meltfnum[3] =
            ((/*_#RKBIND__L3*/ meltfnum[1]) == (0));;
        MELT_LOCATION("warmelt-outobj.melt:2906:/ cond");
        /*cond*/ if (/*_#eqeqI__L7*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:2908:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#eqeq__L8*/ meltfnum[7] =
                            ((/*_.CTYBIND__V13*/ meltfptr[12]) == ((/*!CTYPE_VALUE*/ meltfrout->tabval[9])));;
                        MELT_LOCATION("warmelt-outobj.melt:2908:/ cond");
                        /*cond*/ if (/*_#eqeq__L8*/ meltfnum[7]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V29*/ meltfptr[28] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:2908:/ cond.else");

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
                                        argtab[0].meltbp_cstring =  "check ctybind first";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 2908;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CTYBIND__V13*/ meltfptr[12];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V30*/ meltfptr[29] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V29*/ meltfptr[28] = /*_.MELT_ASSERT_FAILURE_FUN__V30*/ meltfptr[29];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:2908:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V30*/ meltfptr[29] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V28*/ meltfptr[20] = /*_._IFELSE___V29*/ meltfptr[28];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2908:/ clear");
                        /*clear*/ /*_#eqeq__L8*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V29*/ meltfptr[28] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V28*/ meltfptr[20] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:2909:/ msend");
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
                        /*_.OUTPUT_C_CODE__V31*/ meltfptr[29] = meltgc_send((melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[10])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2910:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = (melt_ptr_t) meltfirstargp_;"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2911:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:2907:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:2906:/ clear");
                    /*clear*/ /*_.IFCPP___V28*/ meltfptr[20] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_C_CODE__V31*/ meltfptr[29] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-outobj.melt:2913:/ quasiblock");


                    MELT_LOCATION("warmelt-outobj.melt:2915:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.CTYBIND__V13*/ meltfptr[12]) /*=obj*/;
                        melt_object_get_field(slot,obj, 5, "CTYPE_PARCHAR");
                        /*_.PARC__V32*/ meltfptr[28] = slot;
                    };
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:2916:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.CTYBIND__V13*/ meltfptr[12]) /*=obj*/;
                        melt_object_get_field(slot,obj, 7, "CTYPE_ARGFIELD");
                        /*_.ARGF__V33*/ meltfptr[20] = slot;
                    };
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_STRING__L9*/ meltfnum[7] =
                        (melt_magic_discr((melt_ptr_t)(/*_.PARC__V32*/ meltfptr[28])) == MELTOBMAG_STRING);;
                    /*^compute*/
                    /*_#NOT__L10*/ meltfnum[9] =
                        (!(/*_#IS_STRING__L9*/ meltfnum[7]));;
                    MELT_LOCATION("warmelt-outobj.melt:2918:/ cond");
                    /*cond*/ if (/*_#NOT__L10*/ meltfnum[9]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-outobj.melt:2920:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.CTYBIND__V13*/ meltfptr[12]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                    /*_.NAMED_NAME__V34*/ meltfptr[29] = slot;
                                };
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2919:/ locexp");
                                    melt_error_str((melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), ( "impossible argument ctype"), (melt_ptr_t)(/*_.NAMED_NAME__V34*/ meltfptr[29]));
                                }
                                ;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:2918:/ clear");
                                /*clear*/ /*_.NAMED_NAME__V34*/ meltfptr[29] = 0 ;
                            }
                            ;
                        } /*noelse*/
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2921:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "if (meltxargdescr_["));
                    }
                    ;
                    /*_#miI__L11*/ meltfnum[10] =
                        ((/*_#RKBIND__L3*/ meltfnum[1]) - (1));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2922:/ locexp");
                        meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#miI__L11*/ meltfnum[10]));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2923:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "] != "));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2924:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                              melt_string_str((melt_ptr_t)(/*_.PARC__V32*/ meltfptr[28])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2925:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ") { /*getargrest*/"));
                    }
                    ;
                    /*_#plI__L12*/ meltfnum[11] =
                        ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2926:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L12*/ meltfnum[11]), 0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2927:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                              melt_string_str((melt_ptr_t)(/*_.OVARIADICINDEX__V14*/ meltfptr[13])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2928:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = "));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2929:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                              melt_string_str((melt_ptr_t)(/*_.OVARIADICLENGTH__V15*/ meltfptr[14])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2930:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
                    }
                    ;
                    /*_#plI__L13*/ meltfnum[12] =
                        ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2931:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L13*/ meltfnum[12]), 0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2932:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "goto meltlab_endgetargs;"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2933:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2934:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "}"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2935:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2936:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                              melt_string_str((melt_ptr_t)(/*_.OVARIADICINDEX__V14*/ meltfptr[13])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2937:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = "));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2938:/ locexp");
                        meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#RKBIND__L3*/ meltfnum[1]));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2939:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2940:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#eqeq__L14*/ meltfnum[13] =
                        ((/*_.CTYBIND__V13*/ meltfptr[12]) == ((/*!CTYPE_VALUE*/ meltfrout->tabval[9])));;
                    MELT_LOCATION("warmelt-outobj.melt:2941:/ cond");
                    /*cond*/ if (/*_#eqeq__L14*/ meltfnum[13]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:2943:/ msend");
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
                                    /*_.OUTPUT_C_CODE__V35*/ meltfptr[29] = meltgc_send((melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[10])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2944:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = (meltxargtab_["));
                                }
                                ;
                                /*_#miI__L15*/ meltfnum[14] =
                                    ((/*_#RKBIND__L3*/ meltfnum[1]) - (1));;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2945:/ locexp");
                                    meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#miI__L15*/ meltfnum[14]));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2946:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "].meltbp_aptr) ? (*(meltxargtab_["));
                                }
                                ;
                                /*_#miI__L16*/ meltfnum[15] =
                                    ((/*_#RKBIND__L3*/ meltfnum[1]) - (1));;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2947:/ locexp");
                                    meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#miI__L16*/ meltfnum[15]));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2948:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "].meltbp_aptr)) : NULL;"));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2949:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2950:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "gcc_assert(melt_discr((melt_ptr_t)("));
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:2951:/ msend");
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
                                    /*_.OUTPUT_C_CODE__V36*/ meltfptr[35] = meltgc_send((melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[10])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2952:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ")) != NULL);"));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2953:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:2942:/ quasiblock");


                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:2941:/ clear");
                                /*clear*/ /*_.OUTPUT_C_CODE__V35*/ meltfptr[29] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#miI__L15*/ meltfnum[14] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#miI__L16*/ meltfnum[15] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.OUTPUT_C_CODE__V36*/ meltfptr[35] = 0 ;
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
                                MELT_LOCATION("warmelt-outobj.melt:2956:/ msend");
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
                                    /*_.OUTPUT_C_CODE__V37*/ meltfptr[29] = meltgc_send((melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[10])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2957:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = meltxargtab_["));
                                }
                                ;
                                /*_#miI__L17*/ meltfnum[14] =
                                    ((/*_#RKBIND__L3*/ meltfnum[1]) - (1));;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2958:/ locexp");
                                    meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#miI__L17*/ meltfnum[14]));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2959:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "]."));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2960:/ locexp");
                                    /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                                          melt_string_str((melt_ptr_t)(/*_.ARGF__V33*/ meltfptr[20])));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:2961:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:2955:/ quasiblock");


                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:2941:/ clear");
                                /*clear*/ /*_.OUTPUT_C_CODE__V37*/ meltfptr[29] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#miI__L17*/ meltfnum[14] = 0 ;
                            }
                            ;
                        }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:2964:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;

                    MELT_LOCATION("warmelt-outobj.melt:2913:/ clear");
                    /*clear*/ /*_.PARC__V32*/ meltfptr[28] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ARGF__V33*/ meltfptr[20] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#IS_STRING__L9*/ meltfnum[7] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#NOT__L10*/ meltfnum[9] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#miI__L11*/ meltfnum[10] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#plI__L12*/ meltfnum[11] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#plI__L13*/ meltfnum[12] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#eqeq__L14*/ meltfnum[13] = 0 ;
                    /*epilog*/
                }
                ;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2966:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L18*/ meltfnum[15] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:2967:/ cond");
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[11])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[12])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[11])) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                        /*_.REFERENCED_VALUE__V39*/ meltfptr[29] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V39*/ meltfptr[29] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L19*/ meltfnum[14] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V39*/ meltfptr[29])));;
            /*^compute*/
            /*_#ltI__L20*/ meltfnum[7] =
                ((/*_#STRBUF_USEDLENGTH__L18*/ meltfnum[15]) < (/*_#GET_INT__L19*/ meltfnum[14]));;
            MELT_LOCATION("warmelt-outobj.melt:2966:/ cond");
            /*cond*/ if (/*_#ltI__L20*/ meltfnum[7]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V40*/ meltfptr[28] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2966:/ cond.else");

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
                            argtab[2].meltbp_long = 2966;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V41*/ meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V40*/ meltfptr[28] = /*_.MELT_ASSERT_FAILURE_FUN__V41*/ meltfptr[20];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2966:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V41*/ meltfptr[20] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V38*/ meltfptr[35] = /*_._IFELSE___V40*/ meltfptr[28];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2966:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L18*/ meltfnum[15] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V39*/ meltfptr[29] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L19*/ meltfnum[14] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L20*/ meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V40*/ meltfptr[28] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V38*/ meltfptr[35] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V38*/ meltfptr[35];;

        MELT_LOCATION("warmelt-outobj.melt:2892:/ clear");
        /*clear*/ /*_.OLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NLOC__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OBIND__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OREST__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_#RKBIND__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CTYBIND__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OVARIADICINDEX__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OVARIADICLENGTH__V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V16*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V19*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V20*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V23*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V26*/ meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_#eqeqI__L7*/ meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V38*/ meltfptr[35] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2890:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:2890:/ locexp");
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
    melt_trace_end("OUTPUCOD_GETARGREST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_56_WARMELTmiOUTOBJ_OUTPUCOD_GETARGREST*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL fromline 1752 */

    /** start of frame for meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL// fromline 1568
        : public Melt_CallFrameWithValues<18>
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
        MeltFrame_meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<18> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL), clos) {};
        MeltFrame_meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL() //the constructor fromline 1642
            : Melt_CallFrameWithValues<18> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<18> (fil,lin, sizeof(MeltFrame_meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL)) {};
        MeltFrame_meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<18> (fil,lin, sizeof(MeltFrame_meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL


    /** end of frame for meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL fromline 1697**/

    /* end of frame for routine meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL fromline 1756 */

    /* classy proc frame meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL */ MeltFrame_meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJCFORMAL", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:2974:/ getarg");
    /*_.CFORM__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:2975:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
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
            MELT_LOCATION("warmelt-outobj.melt:2975:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:2975:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2975;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objcformal cform=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CFORM__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2975:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:2975:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:2976:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.CFORM__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJCFORMAL*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:2976:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2976:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check cform";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2976;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CFORM__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2976:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2976:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2977:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRBUF__L5*/ meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3])) == MELTOBMAG_STRBUF);;
            MELT_LOCATION("warmelt-outobj.melt:2977:/ cond");
            /*cond*/ if (/*_#IS_STRBUF__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2977:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check implbuf";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2977;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V11*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2977:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V10*/ meltfptr[8] = /*_._IFELSE___V11*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2977:/ clear");
            /*clear*/ /*_#IS_STRBUF__L5*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V11*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V10*/ meltfptr[8] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:2978:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CFORM__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJCFORMAL*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.CFORM__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "OFORMAL_NAME");
                    /*_.FORNAM__V13*/ meltfptr[11] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.FORNAM__V13*/ meltfptr[11] =  /*reallynil*/ NULL ;;
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CFORM__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V14*/ meltfptr[4] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V15*/ meltfptr[14];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V15*/ meltfptr[14] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V14*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2981:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRING__L6*/ meltfnum[2] =
                (melt_magic_discr((melt_ptr_t)(/*_.FORNAM__V13*/ meltfptr[11])) == MELTOBMAG_STRING);;
            MELT_LOCATION("warmelt-outobj.melt:2981:/ cond");
            /*cond*/ if (/*_#IS_STRING__L6*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V17*/ meltfptr[16] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2981:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check formnam";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2981;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.FORNAM__V13*/ meltfptr[11];
                            /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V17*/ meltfptr[16] = /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2981:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V16*/ meltfptr[14] = /*_._IFELSE___V17*/ meltfptr[16];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2981:/ clear");
            /*clear*/ /*_#IS_STRING__L6*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V17*/ meltfptr[16] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V16*/ meltfptr[14] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2982:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:2983:/ locexp");
            meltgc_add_out((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), melt_string_str((melt_ptr_t)(/*_.FORNAM__V13*/ meltfptr[11])));
        }
        ;

        MELT_LOCATION("warmelt-outobj.melt:2978:/ clear");
        /*clear*/ /*_.FORNAM__V13*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V14*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V16*/ meltfptr[14] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:2974:/ clear");
        /*clear*/ /*_.IFCPP___V7*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V10*/ meltfptr[8] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJCFORMAL", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_57_WARMELTmiOUTOBJ_OUTPUCOD_OBJCFORMAL*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR fromline 1752 */

    /** start of frame for meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR// fromline 1568
        : public Melt_CallFrameWithValues<19>
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
        MeltFrame_meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<19> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR), clos) {};
        MeltFrame_meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR() //the constructor fromline 1642
            : Melt_CallFrameWithValues<19> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR)) {};
        MeltFrame_meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR


    /** end of frame for meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR fromline 1697**/

    /* end of frame for routine meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR fromline 1756 */

    /* classy proc frame meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR */ MeltFrame_meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJFETCHMODVAR", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:2989:/ getarg");
    /*_.FMODVAR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:2990:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
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
            MELT_LOCATION("warmelt-outobj.melt:2990:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:2990:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2990;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objfetchmodvar fmodvar=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.FMODVAR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2990:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:2990:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:2991:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.FMODVAR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJFETCHMODVAR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:2991:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2991:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check fmodvar";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2991;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.FMODVAR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2991:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2991:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:2992:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.FMODVAR__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJFETCHMODVAR*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.FMODVAR__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "OBFETCHMODVAR_BIND");
                    /*_.VARBIND__V11*/ meltfptr[4] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.VARBIND__V11*/ meltfptr[4] =  /*reallynil*/ NULL ;;
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.FMODVAR__V2*/ meltfptr[1];
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

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:2996:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L5*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.VARBIND__V11*/ meltfptr[4]), (melt_ptr_t)((/*!CLASS_NORMAL_MODULE_VARIABLE_BINDING*/ meltfrout->tabval[4])));;
            MELT_LOCATION("warmelt-outobj.melt:2996:/ cond");
            /*cond*/ if (/*_#IS_A__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:2996:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check varbind";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 2996;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VARBIND__V11*/ meltfptr[4];
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/ meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:2996:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:2996:/ clear");
            /*clear*/ /*_#IS_A__L5*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/ meltfptr[14] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2997:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  " (melt_module_var_fetch (";
            /*_.ADD2OUT__V17*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2998:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.VARBIND__V11*/ meltfptr[4];
            /*_.OUT_ENUMIX_MODVARBIND__V18*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!OUT_ENUMIX_MODVARBIND*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2999:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "))";
            /*_.ADD2OUT__V19*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.LET___V10*/ meltfptr[8] = /*_.ADD2OUT__V19*/ meltfptr[18];;

        MELT_LOCATION("warmelt-outobj.melt:2992:/ clear");
        /*clear*/ /*_.VARBIND__V11*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V14*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V17*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUT_ENUMIX_MODVARBIND__V18*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V19*/ meltfptr[18] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:2989:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V10*/ meltfptr[8];;

        {
            MELT_LOCATION("warmelt-outobj.melt:2989:/ locexp");
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
        /*clear*/ /*_.LET___V10*/ meltfptr[8] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJFETCHMODVAR", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_58_WARMELTmiOUTOBJ_OUTPUCOD_OBJFETCHMODVAR*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR fromline 1752 */

    /** start of frame for meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR// fromline 1568
        : public Melt_CallFrameWithValues<24>
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
        MeltFrame_meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<24> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR), clos) {};
        MeltFrame_meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR() //the constructor fromline 1642
            : Melt_CallFrameWithValues<24> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<24> (fil,lin, sizeof(MeltFrame_meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR)) {};
        MeltFrame_meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<24> (fil,lin, sizeof(MeltFrame_meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR


    /** end of frame for meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR fromline 1697**/

    /* end of frame for routine meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR fromline 1756 */

    /* classy proc frame meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR */ MeltFrame_meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJPUTMODVAR", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3004:/ getarg");
    /*_.PMODVAR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3005:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
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
            MELT_LOCATION("warmelt-outobj.melt:3005:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:3005:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3005;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objputmodvar pmodvar=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.PMODVAR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3005:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:3005:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:3006:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.PMODVAR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJPUTMODVAR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:3006:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3006:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check pmodvar";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3006;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.PMODVAR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3006:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3006:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3007:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PMODVAR__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJINSTR*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PMODVAR__V2*/ meltfptr[1]) /*=obj*/;
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
        MELT_LOCATION("warmelt-outobj.melt:3008:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PMODVAR__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJPUTMODVAR*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PMODVAR__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "OBPUTMODVAR_BIND");
                    /*_.VARBIND__V11*/ meltfptr[4] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.VARBIND__V11*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:3009:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PMODVAR__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJPUTMODVAR*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PMODVAR__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "OBPUTMODVAR_VAL");
                    /*_.OVAL__V12*/ meltfptr[11] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OVAL__V12*/ meltfptr[11] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3012:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
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
            MELT_LOCATION("warmelt-outobj.melt:3012:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:3012:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L6*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3012;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objputmodvar varbind=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.VARBIND__V11*/ meltfptr[4];
                            /*_.MELT_DEBUG_FUN__V14*/ meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V13*/ meltfptr[12] = /*_.MELT_DEBUG_FUN__V14*/ meltfptr[13];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3012:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L6*/ meltfnum[2] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V14*/ meltfptr[13] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V13*/ meltfptr[12] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:3012:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L5*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V13*/ meltfptr[12] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:3013:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L7*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.VARBIND__V11*/ meltfptr[4]), (melt_ptr_t)((/*!CLASS_NORMAL_MODULE_VARIABLE_BINDING*/ meltfrout->tabval[4])));;
            MELT_LOCATION("warmelt-outobj.melt:3013:/ cond");
            /*cond*/ if (/*_#IS_A__L7*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V16*/ meltfptr[12] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3013:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check varbind";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3013;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.VARBIND__V11*/ meltfptr[4];
                            /*_.MELT_ASSERT_FAILURE_FUN__V17*/ meltfptr[16] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V16*/ meltfptr[12] = /*_.MELT_ASSERT_FAILURE_FUN__V17*/ meltfptr[16];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3013:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V17*/ meltfptr[16] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V15*/ meltfptr[13] = /*_._IFELSE___V16*/ meltfptr[12];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3013:/ clear");
            /*clear*/ /*_#IS_A__L7*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V16*/ meltfptr[12] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V15*/ meltfptr[13] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3014:/ cond");
        /*cond*/ if (/*_.OLOC__V10*/ meltfptr[8]) /*then*/
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
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                        /*^apply.arg*/
                        argtab[2].meltbp_cstring =  "putmodvar";
                        /*_.OUTPUT_LOCATION__V19*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.OLOC__V10*/ meltfptr[8]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V18*/ meltfptr[16] = /*_.OUTPUT_LOCATION__V19*/ meltfptr[12];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3014:/ clear");
                    /*clear*/ /*_.OUTPUT_LOCATION__V19*/ meltfptr[12] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V18*/ meltfptr[16] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3015:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "melt_module_var_put (";
            /*_.ADD2OUT__V20*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3016:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.VARBIND__V11*/ meltfptr[4];
            /*_.OUT_ENUMIX_MODVARBIND__V21*/ meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!OUT_ENUMIX_MODVARBIND*/ meltfrout->tabval[7])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3017:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  ", (melt_ptr_t)(";
            /*_.ADD2OUT__V22*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3018:/ msend");
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
            /*_.OUTPUT_C_CODE__V23*/ meltfptr[22] = meltgc_send((melt_ptr_t)(/*_.OVAL__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[8])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3019:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "));";
            /*_.ADD2OUT__V24*/ meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3020:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_LOCATION("warmelt-outobj.melt:3007:/ clear");
        /*clear*/ /*_.OLOC__V10*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.VARBIND__V11*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OVAL__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V15*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V18*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V20*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUT_ENUMIX_MODVARBIND__V21*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V22*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V23*/ meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V24*/ meltfptr[23] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:3004:/ clear");
        /*clear*/ /*_.IFCPP___V7*/ meltfptr[5] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJPUTMODVAR", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_59_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTMODVAR*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG fromline 1752 */

    /** start of frame for meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG// fromline 1568
        : public Melt_CallFrameWithValues<25>
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
        MeltFrame_meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<25> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG), clos) {};
        MeltFrame_meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG() //the constructor fromline 1642
            : Melt_CallFrameWithValues<25> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<25> (fil,lin, sizeof(MeltFrame_meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG)) {};
        MeltFrame_meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<25> (fil,lin, sizeof(MeltFrame_meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG


    /** end of frame for meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG fromline 1697**/

    /* end of frame for routine meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG fromline 1756 */

    /* classy proc frame meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG */ MeltFrame_meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJPUTOUTARG", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3025:/ getarg");
    /*_.OUTA__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3026:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
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
            MELT_LOCATION("warmelt-outobj.melt:3026:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:3026:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3026;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objputoutarg outa=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OUTA__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3026:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:3026:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:3027:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.OUTA__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJPUTOUTARG*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:3027:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3027:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check outa";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3027;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OUTA__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3027:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3027:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3028:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OUTA__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJINSTR*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OUTA__V2*/ meltfptr[1]) /*=obj*/;
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
        MELT_LOCATION("warmelt-outobj.melt:3029:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OUTA__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJPUTOUTARG*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OUTA__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "OBOUTARG_FORMAL");
                    /*_.OFORMAL__V11*/ meltfptr[4] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OFORMAL__V11*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:3030:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OFORMAL__V11*/ meltfptr[4]),
                                          (melt_ptr_t)((/*!CLASS_OBJCFORMAL*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OFORMAL__V11*/ meltfptr[4]) /*=obj*/;
                    melt_object_get_field(slot,obj, 4, "OFORMAL_LOCV");
                    /*_.OLOCV__V12*/ meltfptr[11] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OLOCV__V12*/ meltfptr[11] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3032:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
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
            MELT_LOCATION("warmelt-outobj.melt:3032:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:3032:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L6*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3032;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objputoutarg oformal=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OFORMAL__V11*/ meltfptr[4];
                            /*_.MELT_DEBUG_FUN__V14*/ meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V13*/ meltfptr[12] = /*_.MELT_DEBUG_FUN__V14*/ meltfptr[13];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3032:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L6*/ meltfnum[2] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V14*/ meltfptr[13] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V13*/ meltfptr[12] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:3032:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L5*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V13*/ meltfptr[12] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:3033:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L7*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.OFORMAL__V11*/ meltfptr[4]), (melt_ptr_t)((/*!CLASS_OBJCFORMAL*/ meltfrout->tabval[4])));;
            MELT_LOCATION("warmelt-outobj.melt:3033:/ cond");
            /*cond*/ if (/*_#IS_A__L7*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V16*/ meltfptr[12] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3033:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oformal";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3033;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OFORMAL__V11*/ meltfptr[4];
                            /*_.MELT_ASSERT_FAILURE_FUN__V17*/ meltfptr[16] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V16*/ meltfptr[12] = /*_.MELT_ASSERT_FAILURE_FUN__V17*/ meltfptr[16];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3033:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V17*/ meltfptr[16] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V15*/ meltfptr[13] = /*_._IFELSE___V16*/ meltfptr[12];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3033:/ clear");
            /*clear*/ /*_#IS_A__L7*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V16*/ meltfptr[12] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V15*/ meltfptr[13] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3034:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L8*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OLOCV__V12*/ meltfptr[11]), (melt_ptr_t)((/*!CLASS_OBJLOCV*/ meltfrout->tabval[5])));;
            MELT_LOCATION("warmelt-outobj.melt:3034:/ cond");
            /*cond*/ if (/*_#IS_A__L8*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V19*/ meltfptr[12] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3034:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check olocv";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3034;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OLOCV__V12*/ meltfptr[11];
                            /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V19*/ meltfptr[12] = /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3034:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V18*/ meltfptr[16] = /*_._IFELSE___V19*/ meltfptr[12];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3034:/ clear");
            /*clear*/ /*_#IS_A__L8*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V19*/ meltfptr[12] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V18*/ meltfptr[16] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3035:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "putoutarg";
            /*_.OUTPUT_LOCATION__V21*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.OLOC__V10*/ meltfptr[8]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3036:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "if (("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3037:/ msend");
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
            /*_.OUTPUT_C_CODE__V22*/ meltfptr[12] = meltgc_send((melt_ptr_t)(/*_.OFORMAL__V11*/ meltfptr[4]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[7])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3038:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ") != NULL)"));
        }
        ;
        /*_#plI__L9*/ meltfnum[2] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        {
            MELT_LOCATION("warmelt-outobj.melt:3039:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L9*/ meltfnum[2]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3040:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "*";
            /*_.ADD2OUT__V23*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3041:/ msend");
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
            /*_.OUTPUT_C_CODE__V24*/ meltfptr[23] = meltgc_send((melt_ptr_t)(/*_.OFORMAL__V11*/ meltfptr[4]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[7])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3042:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " /*putout*/ = "));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3043:/ msend");
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
            /*_.OUTPUT_C_CODE__V25*/ meltfptr[24] = meltgc_send((melt_ptr_t)(/*_.OLOCV__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[7])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3044:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3045:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_LOCATION("warmelt-outobj.melt:3028:/ clear");
        /*clear*/ /*_.OLOC__V10*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OFORMAL__V11*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OLOCV__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V15*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V18*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V21*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V22*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L9*/ meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V23*/ meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V24*/ meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V25*/ meltfptr[24] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:3025:/ clear");
        /*clear*/ /*_.IFCPP___V7*/ meltfptr[5] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJPUTOUTARG", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_60_WARMELTmiOUTOBJ_OUTPUCOD_OBJPUTOUTARG*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV fromline 1752 */

    /** start of frame for meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV// fromline 1568
        : public Melt_CallFrameWithValues<16>
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
        MeltFrame_meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<16> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV), clos) {};
        MeltFrame_meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV() //the constructor fromline 1642
            : Melt_CallFrameWithValues<16> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<16> (fil,lin, sizeof(MeltFrame_meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV)) {};
        MeltFrame_meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<16> (fil,lin, sizeof(MeltFrame_meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV


    /** end of frame for meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV fromline 1697**/

    /* end of frame for routine meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV fromline 1756 */

    /* classy proc frame meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV */ MeltFrame_meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJLOCV", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3051:/ getarg");
    /*_.LOCV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3052:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.LOCV__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJLOCV*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3052:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3052:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check locv";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3052;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.LOCV__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3052:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3052:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3053:/ quasiblock");


        MELT_LOCATION("warmelt-outobj.melt:3054:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.LOCV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBV_TYPE");
            /*_.LTYP__V8*/ meltfptr[6] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3055:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.LOCV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBL_OFF");
            /*_.LOFF__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3056:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.LOCV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OBL_CNAME");
            /*_.LCNAM__V10*/ meltfptr[9] = slot;
        };
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LOCV__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V12*/ meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V11*/ meltfptr[10] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V12*/ meltfptr[11];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V12*/ meltfptr[11] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V11*/ meltfptr[10] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#eqeq__L3*/ meltfnum[1] =
            ((/*_.LTYP__V8*/ meltfptr[6]) == ((/*!CTYPE_VALUE*/ meltfrout->tabval[3])));;
        MELT_LOCATION("warmelt-outobj.melt:3060:/ cond");
        /*cond*/ if (/*_#eqeq__L3*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:3061:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*_."));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3062:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                              melt_string_str((melt_ptr_t)(/*_.LCNAM__V10*/ meltfptr[9])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3063:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*/ meltfptr["));
                    }
                    ;
                    /*_#GET_INT__L4*/ meltfnum[3] =
                        (melt_get_int((melt_ptr_t)(/*_.LOFF__V9*/ meltfptr[5])));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3064:/ locexp");
                        meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L4*/ meltfnum[3]));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3065:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "]"));
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3060:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_#GET_INT__L4*/ meltfnum[3] = 0 ;
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
                    /*_#eqeq__L5*/ meltfnum[3] =
                        ((/*_.LTYP__V8*/ meltfptr[6]) == ((/*!CTYPE_LONG*/ meltfrout->tabval[4])));;
                    MELT_LOCATION("warmelt-outobj.melt:3066:/ cond");
                    /*cond*/ if (/*_#eqeq__L5*/ meltfnum[3]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-outobj.melt:3067:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*_#"));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:3068:/ locexp");
                                    /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                                          melt_string_str((melt_ptr_t)(/*_.LCNAM__V10*/ meltfptr[9])));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:3069:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*/ meltfnum["));
                                }
                                ;
                                /*_#GET_INT__L6*/ meltfnum[5] =
                                    (melt_get_int((melt_ptr_t)(/*_.LOFF__V9*/ meltfptr[5])));;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:3070:/ locexp");
                                    meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L6*/ meltfnum[5]));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:3071:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "]"));
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:3066:/ quasiblock");


                                /*epilog*/

                                /*^clear*/
                                /*clear*/ /*_#GET_INT__L6*/ meltfnum[5] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-outobj.melt:3073:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*_?*/ meltfram__."));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:3074:/ locexp");
                                    /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                                          melt_string_str((melt_ptr_t)(/*_.LCNAM__V10*/ meltfptr[9])));
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:3072:/ quasiblock");


                                /*epilog*/
                            }
                            ;
                        }
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3060:/ clear");
                    /*clear*/ /*_#eqeq__L5*/ meltfnum[3] = 0 ;
                }
                ;
            }
        ;

        MELT_LOCATION("warmelt-outobj.melt:3053:/ clear");
        /*clear*/ /*_.LTYP__V8*/ meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOFF__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LCNAM__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_#eqeq__L3*/ meltfnum[1] = 0 ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3076:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[5] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3077:/ cond");
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
                        /*_.REFERENCED_VALUE__V14*/ meltfptr[6] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V14*/ meltfptr[6] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L8*/ meltfnum[3] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V14*/ meltfptr[6])));;
            /*^compute*/
            /*_#ltI__L9*/ meltfnum[1] =
                ((/*_#STRBUF_USEDLENGTH__L7*/ meltfnum[5]) < (/*_#GET_INT__L8*/ meltfnum[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3076:/ cond");
            /*cond*/ if (/*_#ltI__L9*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3076:/ cond.else");

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
                            argtab[2].meltbp_long = 3076;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3076:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V13*/ meltfptr[11] = /*_._IFELSE___V15*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3076:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V14*/ meltfptr[6] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L8*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L9*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V13*/ meltfptr[11] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3051:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.IFCPP___V13*/ meltfptr[11];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3051:/ locexp");
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
        /*clear*/ /*_.IFCPP___V13*/ meltfptr[11] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJLOCV", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_61_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOCV*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV fromline 1752 */

    /** start of frame for meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV// fromline 1568
        : public Melt_CallFrameWithValues<20>
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
        MeltFrame_meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<20> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV), clos) {};
        MeltFrame_meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV() //the constructor fromline 1642
            : Melt_CallFrameWithValues<20> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<20> (fil,lin, sizeof(MeltFrame_meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV)) {};
        MeltFrame_meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<20> (fil,lin, sizeof(MeltFrame_meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV


    /** end of frame for meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV fromline 1697**/

    /* end of frame for routine meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV fromline 1756 */

    /* classy proc frame meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV */ MeltFrame_meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJCLOCCV", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3083:/ getarg");
    /*_.OCCV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3084:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OCCV__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJCLOCCV*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3084:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3084:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check occv";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3084;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OCCV__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3084:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3084:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3085:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OCCV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBC_OFF");
            /*_.OOFF__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3086:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OCCV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OBC_NAME");
            /*_.ONAM__V10*/ meltfptr[9] = slot;
        };
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3087:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            MELT_LOCATION("warmelt-outobj.melt:3088:/ getslot");
            {
                melt_ptr_t slot=NULL, obj=NULL;
                obj = (melt_ptr_t)(/*_.OCCV__V2*/ meltfptr[1]) /*=obj*/;
                melt_object_get_field(slot,obj, 0, "OBV_TYPE");
                /*_.OBV_TYPE__V12*/ meltfptr[11] = slot;
            };
            ;
            /*_#eqeq__L3*/ meltfnum[1] =
                ((/*_.OBV_TYPE__V12*/ meltfptr[11]) == ((/*!CTYPE_VALUE*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-outobj.melt:3087:/ cond");
            /*cond*/ if (/*_#eqeq__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V13*/ meltfptr[12] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3087:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check valueness of closed occurrence";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3087;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OCCV__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V14*/ meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V13*/ meltfptr[12] = /*_.MELT_ASSERT_FAILURE_FUN__V14*/ meltfptr[13];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3087:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V14*/ meltfptr[13] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/ meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[12];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3087:/ clear");
            /*clear*/ /*_.OBV_TYPE__V12*/ meltfptr[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_#eqeq__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V13*/ meltfptr[12] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V11*/ meltfptr[10] = (/*nil*/NULL);
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OCCV__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V16*/ meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V15*/ meltfptr[13] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V16*/ meltfptr[11];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V16*/ meltfptr[11] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V15*/ meltfptr[13] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3090:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "(/*~"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3091:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.ONAM__V10*/ meltfptr[9])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3092:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*/ meltfclos->tabval["));
        }
        ;
        /*_#GET_INT__L4*/ meltfnum[1] =
            (melt_get_int((melt_ptr_t)(/*_.OOFF__V9*/ meltfptr[5])));;

        {
            MELT_LOCATION("warmelt-outobj.melt:3093:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L4*/ meltfnum[1]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3094:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "])"));
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3095:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L5*/ meltfnum[4] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3096:/ cond");
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
                        /*_.REFERENCED_VALUE__V18*/ meltfptr[11] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V18*/ meltfptr[11] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L6*/ meltfnum[5] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V18*/ meltfptr[11])));;
            /*^compute*/
            /*_#ltI__L7*/ meltfnum[6] =
                ((/*_#STRBUF_USEDLENGTH__L5*/ meltfnum[4]) < (/*_#GET_INT__L6*/ meltfnum[5]));;
            MELT_LOCATION("warmelt-outobj.melt:3095:/ cond");
            /*cond*/ if (/*_#ltI__L7*/ meltfnum[6]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V19*/ meltfptr[18] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3095:/ cond.else");

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
                            argtab[2].meltbp_long = 3095;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V19*/ meltfptr[18] = /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3095:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V17*/ meltfptr[12] = /*_._IFELSE___V19*/ meltfptr[18];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3095:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L5*/ meltfnum[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V18*/ meltfptr[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L6*/ meltfnum[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L7*/ meltfnum[6] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V19*/ meltfptr[18] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V17*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V17*/ meltfptr[12];;

        MELT_LOCATION("warmelt-outobj.melt:3085:/ clear");
        /*clear*/ /*_.OOFF__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ONAM__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V15*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L4*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V17*/ meltfptr[12] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3083:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3083:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJCLOCCV", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_62_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOCCV*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV fromline 1752 */

    /** start of frame for meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV// fromline 1568
        : public Melt_CallFrameWithValues<22>
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
        MeltFrame_meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<22> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV), clos) {};
        MeltFrame_meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV() //the constructor fromline 1642
            : Melt_CallFrameWithValues<22> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<22> (fil,lin, sizeof(MeltFrame_meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV)) {};
        MeltFrame_meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<22> (fil,lin, sizeof(MeltFrame_meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV


    /** end of frame for meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV fromline 1697**/

    /* end of frame for routine meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV fromline 1756 */

    /* classy proc frame meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV */ MeltFrame_meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJCLOHOOKV", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3102:/ getarg");
    /*_.OCCV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3103:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
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
            MELT_LOCATION("warmelt-outobj.melt:3103:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:3103:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3103;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objclohookv occv=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OCCV__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3103:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:3103:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:3104:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.OCCV__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJCLOHOOKV*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:3104:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3104:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check occv";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3104;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OCCV__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3104:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3104:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3105:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OCCV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBC_OFF");
            /*_.OOFF__V11*/ meltfptr[4] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3106:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OCCV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OBC_NAME");
            /*_.ONAM__V12*/ meltfptr[11] = slot;
        };
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3107:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            MELT_LOCATION("warmelt-outobj.melt:3108:/ getslot");
            {
                melt_ptr_t slot=NULL, obj=NULL;
                obj = (melt_ptr_t)(/*_.OCCV__V2*/ meltfptr[1]) /*=obj*/;
                melt_object_get_field(slot,obj, 0, "OBV_TYPE");
                /*_.OBV_TYPE__V14*/ meltfptr[13] = slot;
            };
            ;
            /*_#eqeq__L5*/ meltfnum[1] =
                ((/*_.OBV_TYPE__V14*/ meltfptr[13]) == ((/*!CTYPE_VALUE*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-outobj.melt:3107:/ cond");
            /*cond*/ if (/*_#eqeq__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3107:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check valueness of closed occurrence";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3107;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OCCV__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/ meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3107:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V13*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3107:/ clear");
            /*clear*/ /*_.OBV_TYPE__V14*/ meltfptr[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_#eqeq__L5*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/ meltfptr[14] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V13*/ meltfptr[12] = (/*nil*/NULL);
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OCCV__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V18*/ meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V17*/ meltfptr[15] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V18*/ meltfptr[13];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V18*/ meltfptr[13] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V17*/ meltfptr[15] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3110:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "(/*~"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3111:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.ONAM__V12*/ meltfptr[11])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3112:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*/ meltfhook->tabval["));
        }
        ;
        /*_#GET_INT__L6*/ meltfnum[2] =
            (melt_get_int((melt_ptr_t)(/*_.OOFF__V11*/ meltfptr[4])));;

        {
            MELT_LOCATION("warmelt-outobj.melt:3113:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L6*/ meltfnum[2]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3114:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "])"));
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3115:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[1] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3116:/ cond");
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
                        /*_.REFERENCED_VALUE__V20*/ meltfptr[13] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V20*/ meltfptr[13] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L8*/ meltfnum[7] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V20*/ meltfptr[13])));;
            /*^compute*/
            /*_#ltI__L9*/ meltfnum[8] =
                ((/*_#STRBUF_USEDLENGTH__L7*/ meltfnum[1]) < (/*_#GET_INT__L8*/ meltfnum[7]));;
            MELT_LOCATION("warmelt-outobj.melt:3115:/ cond");
            /*cond*/ if (/*_#ltI__L9*/ meltfnum[8]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V21*/ meltfptr[20] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3115:/ cond.else");

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
                            argtab[2].meltbp_long = 3115;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V21*/ meltfptr[20] = /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3115:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V19*/ meltfptr[14] = /*_._IFELSE___V21*/ meltfptr[20];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3115:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V20*/ meltfptr[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L8*/ meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L9*/ meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V21*/ meltfptr[20] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V19*/ meltfptr[14] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V10*/ meltfptr[8] = /*_.IFCPP___V19*/ meltfptr[14];;

        MELT_LOCATION("warmelt-outobj.melt:3105:/ clear");
        /*clear*/ /*_.OOFF__V11*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ONAM__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V17*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L6*/ meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V19*/ meltfptr[14] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3102:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V10*/ meltfptr[8];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3102:/ locexp");
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
        /*clear*/ /*_.LET___V10*/ meltfptr[8] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJCLOHOOKV", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_63_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLOHOOKV*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV fromline 1752 */

    /** start of frame for meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV// fromline 1568
        : public Melt_CallFrameWithValues<20>
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
        MeltFrame_meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<20> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV), clos) {};
        MeltFrame_meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV() //the constructor fromline 1642
            : Melt_CallFrameWithValues<20> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<20> (fil,lin, sizeof(MeltFrame_meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV)) {};
        MeltFrame_meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<20> (fil,lin, sizeof(MeltFrame_meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV


    /** end of frame for meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV fromline 1697**/

    /* end of frame for routine meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV fromline 1756 */

    /* classy proc frame meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV */ MeltFrame_meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJCONSTV", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3123:/ getarg");
    /*_.OCNSTV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3124:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OCNSTV__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJCONSTV*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3124:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3124:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check ocnstv";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3124;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OCNSTV__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3124:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3124:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3125:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OCNSTV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBC_OFF");
            /*_.OOFF__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3126:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OCNSTV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OBC_NAME");
            /*_.ONAM__V10*/ meltfptr[9] = slot;
        };
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3127:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            MELT_LOCATION("warmelt-outobj.melt:3128:/ getslot");
            {
                melt_ptr_t slot=NULL, obj=NULL;
                obj = (melt_ptr_t)(/*_.OCNSTV__V2*/ meltfptr[1]) /*=obj*/;
                melt_object_get_field(slot,obj, 0, "OBV_TYPE");
                /*_.OBV_TYPE__V12*/ meltfptr[11] = slot;
            };
            ;
            /*_#eqeq__L3*/ meltfnum[1] =
                ((/*_.OBV_TYPE__V12*/ meltfptr[11]) == ((/*!CTYPE_VALUE*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-outobj.melt:3127:/ cond");
            /*cond*/ if (/*_#eqeq__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V13*/ meltfptr[12] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3127:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check valueness of const occurrence";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3127;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OCNSTV__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V14*/ meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V13*/ meltfptr[12] = /*_.MELT_ASSERT_FAILURE_FUN__V14*/ meltfptr[13];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3127:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V14*/ meltfptr[13] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/ meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[12];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3127:/ clear");
            /*clear*/ /*_.OBV_TYPE__V12*/ meltfptr[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_#eqeq__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V13*/ meltfptr[12] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V11*/ meltfptr[10] = (/*nil*/NULL);
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OCNSTV__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V16*/ meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V15*/ meltfptr[13] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V16*/ meltfptr[11];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V16*/ meltfptr[11] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V15*/ meltfptr[13] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3130:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "(/*!"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3131:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.ONAM__V10*/ meltfptr[9])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3133:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*/ meltfrout->tabval["));
        }
        ;
        /*_#GET_INT__L4*/ meltfnum[1] =
            (melt_get_int((melt_ptr_t)(/*_.OOFF__V9*/ meltfptr[5])));;

        {
            MELT_LOCATION("warmelt-outobj.melt:3134:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L4*/ meltfnum[1]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3135:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "])"));
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3136:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L5*/ meltfnum[4] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3137:/ cond");
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
                        /*_.REFERENCED_VALUE__V18*/ meltfptr[11] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V18*/ meltfptr[11] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L6*/ meltfnum[5] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V18*/ meltfptr[11])));;
            /*^compute*/
            /*_#ltI__L7*/ meltfnum[6] =
                ((/*_#STRBUF_USEDLENGTH__L5*/ meltfnum[4]) < (/*_#GET_INT__L6*/ meltfnum[5]));;
            MELT_LOCATION("warmelt-outobj.melt:3136:/ cond");
            /*cond*/ if (/*_#ltI__L7*/ meltfnum[6]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V19*/ meltfptr[18] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3136:/ cond.else");

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
                            argtab[2].meltbp_long = 3136;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V19*/ meltfptr[18] = /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3136:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V17*/ meltfptr[12] = /*_._IFELSE___V19*/ meltfptr[18];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3136:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L5*/ meltfnum[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V18*/ meltfptr[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L6*/ meltfnum[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L7*/ meltfnum[6] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V19*/ meltfptr[18] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V17*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V17*/ meltfptr[12];;

        MELT_LOCATION("warmelt-outobj.melt:3125:/ clear");
        /*clear*/ /*_.OOFF__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ONAM__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V15*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L4*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V17*/ meltfptr[12] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3123:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3123:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJCONSTV", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_64_WARMELTmiOUTOBJ_OUTPUCOD_OBJCONSTV*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST fromline 1752 */

    /** start of frame for meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST// fromline 1568
        : public Melt_CallFrameWithValues<35>
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
        MeltFrame_meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<35> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST), clos) {};
        MeltFrame_meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<35> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<35> (fil,lin, sizeof(MeltFrame_meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST)) {};
        MeltFrame_meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<35> (fil,lin, sizeof(MeltFrame_meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST


    /** end of frame for meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST fromline 1697**/

    /* end of frame for routine meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST fromline 1756 */

    /* classy proc frame meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST */ MeltFrame_meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUT_CODE_INSTRUCTIONS_LIST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3143:/ getarg");
    /*_.LIS__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.BOXEDDEPTH__V5*/ meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.BOXEDDEPTH__V5*/ meltfptr[4])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3144:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_LIST_OR_NULL__L1*/ meltfnum[0] =
                ((/*_.LIS__V2*/ meltfptr[1]) == NULL
                 || (melt_unsafe_magic_discr((melt_ptr_t)(/*_.LIS__V2*/ meltfptr[1])) == MELTOBMAG_LIST));;
            MELT_LOCATION("warmelt-outobj.melt:3144:/ cond");
            /*cond*/ if (/*_#IS_LIST_OR_NULL__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3144:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check lis";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3144;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.LIS__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V7*/ meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3144:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V6*/ meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3144:/ clear");
            /*clear*/ /*_#IS_LIST_OR_NULL__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V7*/ meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V6*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3145:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_INTEGERBOX__L2*/ meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.BOXEDDEPTH__V5*/ meltfptr[4])) == MELTOBMAG_INT);;
            MELT_LOCATION("warmelt-outobj.melt:3145:/ cond");
            /*cond*/ if (/*_#IS_INTEGERBOX__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3145:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check boxeddepth";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3145;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.BOXEDDEPTH__V5*/ meltfptr[4];
                            /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V10*/ meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3145:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V9*/ meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3145:/ clear");
            /*clear*/ /*_#IS_INTEGERBOX__L2*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V10*/ meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V9*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3146:/ quasiblock");


        /*_#DEPTH__L3*/ meltfnum[0] =
            (melt_get_int((melt_ptr_t)(/*_.BOXEDDEPTH__V5*/ meltfptr[4])));;

        {
            MELT_LOCATION("warmelt-outobj.melt:3147:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L3*/ meltfnum[0]), 0);
        }
        ;
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit1__EACHLIST */
            for (/*_.PAIR__V12*/ meltfptr[10] = melt_list_first( (melt_ptr_t)/*_.LIS__V2*/ meltfptr[1]);
                                 melt_magic_discr((melt_ptr_t) /*_.PAIR__V12*/ meltfptr[10]) == MELTOBMAG_PAIR;
                                 /*_.PAIR__V12*/ meltfptr[10] = melt_pair_tail((melt_ptr_t) /*_.PAIR__V12*/ meltfptr[10]))
                {
                    /*_.CUR__V13*/ meltfptr[6] = melt_pair_head((melt_ptr_t) /*_.PAIR__V12*/ meltfptr[10]);



                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_A__L4*/ meltfnum[3] =
                        melt_is_instance_of((melt_ptr_t)(/*_.CUR__V13*/ meltfptr[6]), (melt_ptr_t)((/*!CLASS_OBJPLAINBLOCK*/ meltfrout->tabval[1])));;
                    MELT_LOCATION("warmelt-outobj.melt:3152:/ cond");
                    /*cond*/ if (/*_#IS_A__L4*/ meltfnum[3]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-outobj.melt:3153:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L3*/ meltfnum[0]), 0);
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:3154:/ quasiblock");


                                /*^getslot*/
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.CUR__V13*/ meltfptr[6]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 0, "OBI_LOC");
                                    /*_.BLOC__V16*/ meltfptr[15] = slot;
                                };
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:3155:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.CUR__V13*/ meltfptr[6]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 1, "OBLO_BODYL");
                                    /*_.BODYL__V17*/ meltfptr[16] = slot;
                                };
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:3156:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.CUR__V13*/ meltfptr[6]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 2, "OBLO_EPIL");
                                    /*_.EPIL__V18*/ meltfptr[17] = slot;
                                };
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:3158:/ cond");
                                /*cond*/ if (/*_.BLOC__V16*/ meltfptr[15]) /*then*/
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
                                                union meltparam_un argtab[3];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                /*^apply.arg*/
                                                argtab[1].meltbp_long = /*_#DEPTH__L3*/ meltfnum[0];
                                                /*^apply.arg*/
                                                argtab[2].meltbp_cstring =  "quasiblock";
                                                /*_.OUTPUT_LOCATION__V20*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.BLOC__V16*/ meltfptr[15]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                            /*_._IF___V19*/ meltfptr[18] = /*_.OUTPUT_LOCATION__V20*/ meltfptr[19];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:3158:/ clear");
                                            /*clear*/ /*_.OUTPUT_LOCATION__V20*/ meltfptr[19] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_._IF___V19*/ meltfptr[18] =  /*reallynil*/ NULL ;;
                                    }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:3159:/ cond");
                                /*cond*/ if (/*_.BODYL__V17*/ meltfptr[16]) /*then*/
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
                                                union meltparam_un argtab[3];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                                /*^apply.arg*/
                                                argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                /*^apply.arg*/
                                                argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.BOXEDDEPTH__V5*/ meltfptr[4];
                                                /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V22*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_CODE_INSTRUCTIONS_LIST*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.BODYL__V17*/ meltfptr[16]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                            /*_._IF___V21*/ meltfptr[19] = /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V22*/ meltfptr[21];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:3159:/ clear");
                                            /*clear*/ /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V22*/ meltfptr[21] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_._IF___V21*/ meltfptr[19] =  /*reallynil*/ NULL ;;
                                    }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:3160:/ cond");
                                /*cond*/ if (/*_.EPIL__V18*/ meltfptr[17]) /*then*/
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
                                                union meltparam_un argtab[3];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                                /*^apply.arg*/
                                                argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                /*^apply.arg*/
                                                argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.BOXEDDEPTH__V5*/ meltfptr[4];
                                                /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V24*/ meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_CODE_INSTRUCTIONS_LIST*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.EPIL__V18*/ meltfptr[17]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                            /*_._IF___V23*/ meltfptr[21] = /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V24*/ meltfptr[23];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:3160:/ clear");
                                            /*clear*/ /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V24*/ meltfptr[23] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_._IF___V23*/ meltfptr[21] =  /*reallynil*/ NULL ;;
                                    }
                                ;
                                /*^compute*/
                                /*_.LET___V15*/ meltfptr[14] = /*_._IF___V23*/ meltfptr[21];;

                                MELT_LOCATION("warmelt-outobj.melt:3154:/ clear");
                                /*clear*/ /*_.BLOC__V16*/ meltfptr[15] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.BODYL__V17*/ meltfptr[16] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.EPIL__V18*/ meltfptr[17] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IF___V19*/ meltfptr[18] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IF___V21*/ meltfptr[19] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IF___V23*/ meltfptr[21] = 0 ;
                                MELT_LOCATION("warmelt-outobj.melt:3152:/ quasiblock");


                                /*_.PROGN___V25*/ meltfptr[23] = /*_.LET___V15*/ meltfptr[14];;
                                /*^compute*/
                                /*_._IFELSE___V14*/ meltfptr[13] = /*_.PROGN___V25*/ meltfptr[23];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:3152:/ clear");
                                /*clear*/ /*_.LET___V15*/ meltfptr[14] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V25*/ meltfptr[23] = 0 ;
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

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:3163:/ cond");
                                /*cond*/ if (/*_.CUR__V13*/ meltfptr[6]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            /*_#IS_NOT_A__L6*/ meltfnum[5] =
                                                !melt_is_instance_of((melt_ptr_t)(/*_.CUR__V13*/ meltfptr[6]), (melt_ptr_t)((/*!CLASS_OBJPUREVALUE*/ meltfrout->tabval[4])));;
                                            /*^compute*/
                                            /*_#_IF___L5*/ meltfnum[4] = /*_#IS_NOT_A__L6*/ meltfnum[5];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:3163:/ clear");
                                            /*clear*/ /*_#IS_NOT_A__L6*/ meltfnum[5] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_#_IF___L5*/ meltfnum[4] = 0;;
                                    }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:3163:/ cond");
                                /*cond*/ if (/*_#_IF___L5*/ meltfnum[4]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            {
                                                MELT_LOCATION("warmelt-outobj.melt:3164:/ locexp");
                                                meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L3*/ meltfnum[0]), 0);
                                            }
                                            ;

                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-outobj.melt:3165:/ msend");
                                            /*msend*/
                                            {
                                                union meltparam_un argtab[3];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^ojbmsend.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                                /*^ojbmsend.arg*/
                                                argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                /*^ojbmsend.arg*/
                                                argtab[2].meltbp_long = /*_#DEPTH__L3*/ meltfnum[0];
                                                /*_.OUTPUT_C_CODE__V27*/ meltfptr[16] = meltgc_send((melt_ptr_t)(/*_.CUR__V13*/ meltfptr[6]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;

                                            {
                                                MELT_LOCATION("warmelt-outobj.melt:3166:/ locexp");
                                                /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
                                            }
                                            ;
                                            MELT_LOCATION("warmelt-outobj.melt:3163:/ quasiblock");


                                            /*epilog*/

                                            /*^clear*/
                                            /*clear*/ /*_.OUTPUT_C_CODE__V27*/ meltfptr[16] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_._IFELSE___V26*/ meltfptr[15] =  /*reallynil*/ NULL ;;
                                    }
                                ;
                                /*^compute*/
                                /*_._IFELSE___V14*/ meltfptr[13] = /*_._IFELSE___V26*/ meltfptr[15];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:3152:/ clear");
                                /*clear*/ /*_#_IF___L5*/ meltfnum[4] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V26*/ meltfptr[15] = 0 ;
                            }
                            ;
                        }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:3168:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[5] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                        MELT_LOCATION("warmelt-outobj.melt:3169:/ cond");
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
                                    /*_.REFERENCED_VALUE__V29*/ meltfptr[18] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V29*/ meltfptr[18] =  /*reallynil*/ NULL ;;
                            }
                        ;
                        /*^compute*/
                        /*_#GET_INT__L8*/ meltfnum[4] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V29*/ meltfptr[18])));;
                        /*^compute*/
                        /*_#ltI__L9*/ meltfnum[8] =
                            ((/*_#STRBUF_USEDLENGTH__L7*/ meltfnum[5]) < (/*_#GET_INT__L8*/ meltfnum[4]));;
                        MELT_LOCATION("warmelt-outobj.melt:3168:/ cond");
                        /*cond*/ if (/*_#ltI__L9*/ meltfnum[8]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V30*/ meltfptr[19] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:3168:/ cond.else");

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
                                        argtab[2].meltbp_long = 3168;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V31*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V30*/ meltfptr[19] = /*_.MELT_ASSERT_FAILURE_FUN__V31*/ meltfptr[21];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:3168:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V31*/ meltfptr[21] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V28*/ meltfptr[17] = /*_._IFELSE___V30*/ meltfptr[19];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3168:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[5] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V29*/ meltfptr[18] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L8*/ meltfnum[4] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L9*/ meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V30*/ meltfptr[19] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V28*/ meltfptr[17] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:3170:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L10*/ meltfnum[5] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]));;
                        MELT_LOCATION("warmelt-outobj.melt:3171:/ cond");
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
                                    /*_.REFERENCED_VALUE__V33*/ meltfptr[23] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V33*/ meltfptr[23] =  /*reallynil*/ NULL ;;
                            }
                        ;
                        /*^compute*/
                        /*_#GET_INT__L11*/ meltfnum[4] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V33*/ meltfptr[23])));;
                        /*^compute*/
                        /*_#ltI__L12*/ meltfnum[8] =
                            ((/*_#STRBUF_USEDLENGTH__L10*/ meltfnum[5]) < (/*_#GET_INT__L11*/ meltfnum[4]));;
                        MELT_LOCATION("warmelt-outobj.melt:3170:/ cond");
                        /*cond*/ if (/*_#ltI__L12*/ meltfnum[8]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V34*/ meltfptr[16] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:3170:/ cond.else");

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
                                        argtab[0].meltbp_cstring =  "check limited declbuf";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 3170;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V35*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V34*/ meltfptr[16] = /*_.MELT_ASSERT_FAILURE_FUN__V35*/ meltfptr[15];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:3170:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V35*/ meltfptr[15] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V32*/ meltfptr[14] = /*_._IFELSE___V34*/ meltfptr[16];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3170:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L10*/ meltfnum[5] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V33*/ meltfptr[23] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L11*/ meltfnum[4] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L12*/ meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V34*/ meltfptr[16] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V32*/ meltfptr[14] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                } /* end foreach_pair_component_in_list meltcit1__EACHLIST */
            /*_.PAIR__V12*/ meltfptr[10] = NULL;
            /*_.CUR__V13*/ meltfptr[6] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:3148:/ clear");
            /*clear*/ /*_.PAIR__V12*/ meltfptr[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_.CUR__V13*/ meltfptr[6] = 0 ;
            /*^clear*/
            /*clear*/ /*_#IS_A__L4*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V14*/ meltfptr[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V28*/ meltfptr[17] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V32*/ meltfptr[14] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;

        MELT_LOCATION("warmelt-outobj.melt:3146:/ clear");
        /*clear*/ /*_#DEPTH__L3*/ meltfnum[0] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:3143:/ clear");
        /*clear*/ /*_.IFCPP___V6*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V9*/ meltfptr[7] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUT_CODE_INSTRUCTIONS_LIST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_65_WARMELTmiOUTOBJ_OUTPUT_CODE_INSTRUCTIONS_LIST*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL fromline 1752 */

    /** start of frame for meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL// fromline 1568
        : public Melt_CallFrameWithValues<11>
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
        MeltFrame_meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<11> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL), clos) {};
        MeltFrame_meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL() //the constructor fromline 1642
            : Melt_CallFrameWithValues<11> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<11> (fil,lin, sizeof(MeltFrame_meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL)) {};
        MeltFrame_meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<11> (fil,lin, sizeof(MeltFrame_meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL


    /** end of frame for meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL fromline 1697**/

    /* end of frame for routine meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL fromline 1756 */

    /* classy proc frame meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL */ MeltFrame_meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJCHECKSIGNAL", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3177:/ getarg");
    /*_.OBCHI__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3178:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBCHI__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJCHECKSIGNAL*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3178:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3178:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check ochi";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3178;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBCHI__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3178:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3178:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3179:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBCHI__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.OLOC__V8*/ meltfptr[6] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3183:/ cond");
        /*cond*/ if (/*_.OLOC__V8*/ meltfptr[6]) /*then*/
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
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                        /*^apply.arg*/
                        argtab[2].meltbp_cstring =  "checksignal";
                        /*_.OUTPUT_RAW_LOCATION__V10*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_RAW_LOCATION*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.OLOC__V8*/ meltfptr[6]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V9*/ meltfptr[5] = /*_.OUTPUT_RAW_LOCATION__V10*/ meltfptr[9];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3183:/ clear");
                    /*clear*/ /*_.OUTPUT_RAW_LOCATION__V10*/ meltfptr[9] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V9*/ meltfptr[5] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3184:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  " MELT_CHECK_SIGNAL();";
            /*_.ADD2OUT__V11*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3185:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_LOCATION("warmelt-outobj.melt:3179:/ clear");
        /*clear*/ /*_.OLOC__V8*/ meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V11*/ meltfptr[9] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:3177:/ clear");
        /*clear*/ /*_.IFCPP___V5*/ meltfptr[4] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJCHECKSIGNAL", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_66_WARMELTmiOUTOBJ_OUTPUCOD_OBJCHECKSIGNAL*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK fromline 1752 */

    /** start of frame for meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK// fromline 1568
        : public Melt_CallFrameWithValues<24>
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
        MeltFrame_meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<24> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK), clos) {};
        MeltFrame_meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK() //the constructor fromline 1642
            : Melt_CallFrameWithValues<24> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<24> (fil,lin, sizeof(MeltFrame_meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK)) {};
        MeltFrame_meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<24> (fil,lin, sizeof(MeltFrame_meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK


    /** end of frame for meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK fromline 1697**/

    /* end of frame for routine meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK fromline 1756 */

    /* classy proc frame meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK */ MeltFrame_meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJANYBLOCK", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3190:/ getarg");
    /*_.OBLO__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3191:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJANYBLOCK*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3191:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3191:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oblo";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3191;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBLO__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3191:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3191:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3192:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.OBI_LOC__V8*/ meltfptr[6] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "block";
            /*_.OUTPUT_LOCATION__V9*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.OBI_LOC__V8*/ meltfptr[6]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:3193:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBLO_BODYL");
            /*_.BODYL__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3194:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBLO_EPIL");
            /*_.EPIL__V12*/ meltfptr[11] = slot;
        };
        ;
        /*_#plI__L3*/ meltfnum[1] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;
        /*^compute*/
        /*_.BOXDEPTHP1__V13*/ meltfptr[12] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[3])), (/*_#plI__L3*/ meltfnum[1])));;

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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OBLO__V2*/ meltfptr[1];
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

        {
            MELT_LOCATION("warmelt-outobj.melt:3198:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*anyblock*/{"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_LIST__L4*/ meltfnum[3] =
            (melt_magic_discr((melt_ptr_t)(/*_.BODYL__V11*/ meltfptr[10])) == MELTOBMAG_LIST);;
        MELT_LOCATION("warmelt-outobj.melt:3199:/ cond");
        /*cond*/ if (/*_#IS_LIST__L4*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3200:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.BOXDEPTHP1__V13*/ meltfptr[12];
                        /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V17*/ meltfptr[16] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_CODE_INSTRUCTIONS_LIST*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.BODYL__V11*/ meltfptr[10]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V16*/ meltfptr[14] = /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V17*/ meltfptr[16];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3199:/ clear");
                    /*clear*/ /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V17*/ meltfptr[16] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V16*/ meltfptr[14] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_LIST__L5*/ meltfnum[4] =
            (melt_magic_discr((melt_ptr_t)(/*_.EPIL__V12*/ meltfptr[11])) == MELTOBMAG_LIST);;
        MELT_LOCATION("warmelt-outobj.melt:3201:/ cond");
        /*cond*/ if (/*_#IS_LIST__L5*/ meltfnum[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_#GET_INT__L6*/ meltfnum[5] =
                        (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V13*/ meltfptr[12])));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3203:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L6*/ meltfnum[5]), 0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3204:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*epilog*/"));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3205:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.BOXDEPTHP1__V13*/ meltfptr[12];
                        /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V19*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_CODE_INSTRUCTIONS_LIST*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.EPIL__V12*/ meltfptr[11]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3202:/ quasiblock");


                    /*_.PROGN___V20*/ meltfptr[19] = /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V19*/ meltfptr[18];;
                    /*^compute*/
                    /*_._IF___V18*/ meltfptr[16] = /*_.PROGN___V20*/ meltfptr[19];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3201:/ clear");
                    /*clear*/ /*_#GET_INT__L6*/ meltfnum[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V19*/ meltfptr[18] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V20*/ meltfptr[19] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V18*/ meltfptr[16] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3206:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "}"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3207:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3208:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[5] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3209:/ cond");
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
                        /*_.REFERENCED_VALUE__V22*/ meltfptr[19] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V22*/ meltfptr[19] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L8*/ meltfnum[7] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V22*/ meltfptr[19])));;
            /*^compute*/
            /*_#ltI__L9*/ meltfnum[8] =
                ((/*_#STRBUF_USEDLENGTH__L7*/ meltfnum[5]) < (/*_#GET_INT__L8*/ meltfnum[7]));;
            MELT_LOCATION("warmelt-outobj.melt:3208:/ cond");
            /*cond*/ if (/*_#ltI__L9*/ meltfnum[8]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V23*/ meltfptr[22] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3208:/ cond.else");

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
                            argtab[2].meltbp_long = 3208;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V23*/ meltfptr[22] = /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[23];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3208:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[23] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V21*/ meltfptr[18] = /*_._IFELSE___V23*/ meltfptr[22];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3208:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V22*/ meltfptr[19] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L8*/ meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L9*/ meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V23*/ meltfptr[22] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V21*/ meltfptr[18] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V10*/ meltfptr[9] = /*_.IFCPP___V21*/ meltfptr[18];;

        MELT_LOCATION("warmelt-outobj.melt:3193:/ clear");
        /*clear*/ /*_.BODYL__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.EPIL__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BOXDEPTHP1__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_LIST__L4*/ meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V16*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_LIST__L5*/ meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V18*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V21*/ meltfptr[18] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3190:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V10*/ meltfptr[9];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3190:/ locexp");
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
        /*clear*/ /*_.OBI_LOC__V8*/ meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V10*/ meltfptr[9] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJANYBLOCK", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_67_WARMELTmiOUTOBJ_OUTPUCOD_OBJANYBLOCK*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG fromline 1752 */

    /** start of frame for meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG// fromline 1568
        : public Melt_CallFrameWithValues<15>
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
        MeltFrame_meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<15> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG), clos) {};
        MeltFrame_meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG() //the constructor fromline 1642
            : Melt_CallFrameWithValues<15> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<15> (fil,lin, sizeof(MeltFrame_meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG)) {};
        MeltFrame_meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<15> (fil,lin, sizeof(MeltFrame_meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG


    /** end of frame for meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG fromline 1697**/

    /* end of frame for routine meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG fromline 1756 */

    /* classy proc frame meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG */ MeltFrame_meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJFLAG", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3216:/ getarg");
    /*_.OFLAG__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3217:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
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
            MELT_LOCATION("warmelt-outobj.melt:3217:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:3217:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3217;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objflag oflag=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OFLAG__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3217:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:3217:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:3218:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.OFLAG__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJFLAG*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:3218:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3218:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oflag";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3218;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OFLAG__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3218:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3218:/ clear");
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OFLAG__V2*/ meltfptr[1];
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
        MELT_LOCATION("warmelt-outobj.melt:3220:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OFLAG__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJFLAG*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OFLAG__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "OBFLAG_NAME");
                    /*_.OFLNAME__V13*/ meltfptr[12] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OFLNAME__V13*/ meltfptr[12] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:3221:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OFLAG__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJFLAG*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OFLAG__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 3, "OBFLAG_RANK");
                    /*_.OFLRANK__V14*/ meltfptr[13] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OFLRANK__V14*/ meltfptr[13] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3223:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[5];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "meltmatchflag_";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.OFLNAME__V13*/ meltfptr[12];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "__rk";
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OFLRANK__V14*/ meltfptr[13];
            /*^apply.arg*/
            argtab[4].meltbp_cstring =  " ";
            /*_.ADD2OUT__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.LET___V12*/ meltfptr[4] = /*_.ADD2OUT__V15*/ meltfptr[14];;

        MELT_LOCATION("warmelt-outobj.melt:3220:/ clear");
        /*clear*/ /*_.OFLNAME__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OFLRANK__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V15*/ meltfptr[14] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3216:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V12*/ meltfptr[4];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3216:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJFLAG", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_68_WARMELTmiOUTOBJ_OUTPUCOD_OBJFLAG*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK fromline 1752 */

    /** start of frame for meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK// fromline 1568
        : public Melt_CallFrameWithValues<46>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[18];
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
        MeltFrame_meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<46> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK), clos) {};
        MeltFrame_meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK() //the constructor fromline 1642
            : Melt_CallFrameWithValues<46> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<46> (fil,lin, sizeof(MeltFrame_meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK)) {};
        MeltFrame_meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<46> (fil,lin, sizeof(MeltFrame_meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK


    /** end of frame for meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK fromline 1697**/

    /* end of frame for routine meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK fromline 1756 */

    /* classy proc frame meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK */ MeltFrame_meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJMATCHFLAGBLOCK", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3228:/ getarg");
    /*_.OBLO__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3229:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
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
            MELT_LOCATION("warmelt-outobj.melt:3229:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:3229:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3229;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objmatchflagblock oblo=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OBLO__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3229:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:3229:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:3230:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJMATCHFLAGBLOCK*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:3230:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3230:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oblo";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3230;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBLO__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3230:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3230:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3231:/ quasiblock");


        MELT_LOCATION("warmelt-outobj.melt:3232:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJINSTR*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]) /*=obj*/;
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
        MELT_LOCATION("warmelt-outobj.melt:3233:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJMATCHFLAGBLOCK*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 3, "OMCHFLGBLO_FLAGS");
                    /*_.OFLAGS__V11*/ meltfptr[4] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OFLAGS__V11*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:3234:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJANYBLOCK*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "OBLO_BODYL");
                    /*_.OBODY__V12*/ meltfptr[11] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OBODY__V12*/ meltfptr[11] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:3235:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBLO_EPIL");
            /*_.OEPIL__V13*/ meltfptr[12] = slot;
        };
        ;
        /*_#DEPTHSUCC__L5*/ meltfnum[1] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3238:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "matchflagblock";
            /*_.OUTPUT_LOCATION__V14*/ meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.OLOC__V10*/ meltfptr[8]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3239:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*matchflagblock*/ {"));
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3241:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L6*/ meltfnum[2] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:3241:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L6*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L7*/ meltfnum[6] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:3241:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[6];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3241;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objmatchflagblock oflags=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OFLAGS__V11*/ meltfptr[4];
                            /*_.MELT_DEBUG_FUN__V16*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V15*/ meltfptr[14] = /*_.MELT_DEBUG_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3241:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L7*/ meltfnum[6] = 0 ;
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
                MELT_LOCATION("warmelt-outobj.melt:3241:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L6*/ meltfnum[2] = 0 ;
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

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3242:/ cond");
        /*cond*/ if (/*_.OFLAGS__V11*/ meltfptr[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:3243:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#IS_MULTIPLE__L8*/ meltfnum[6] =
                            (melt_magic_discr((melt_ptr_t)(/*_.OFLAGS__V11*/ meltfptr[4])) == MELTOBMAG_MULTIPLE);;
                        MELT_LOCATION("warmelt-outobj.melt:3243:/ cond");
                        /*cond*/ if (/*_#IS_MULTIPLE__L8*/ meltfnum[6]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V18*/ meltfptr[14] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:3243:/ cond.else");

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
                                        argtab[0].meltbp_cstring =  "check oflags";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 3243;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OFLAGS__V11*/ meltfptr[4];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V18*/ meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:3243:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V17*/ meltfptr[15] = /*_._IFELSE___V18*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3243:/ clear");
                        /*clear*/ /*_#IS_MULTIPLE__L8*/ meltfnum[6] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V18*/ meltfptr[14] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V17*/ meltfptr[15] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                    /*citerblock FOREACH_IN_MULTIPLE*/
                    {
                        /* start foreach_in_multiple meltcit1__EACHTUP */
                        long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.OFLAGS__V11*/ meltfptr[4]);
                        for (/*_#IX__L9*/ meltfnum[2] = 0;
                                          (/*_#IX__L9*/ meltfnum[2] >= 0) && (/*_#IX__L9*/ meltfnum[2] <  meltcit1__EACHTUP_ln);
                                          /*_#IX__L9*/ meltfnum[2]++)
                            {
                                /*_.CUROFLAG__V20*/ meltfptr[18] = melt_multiple_nth((melt_ptr_t)(/*_.OFLAGS__V11*/ meltfptr[4]),  /*_#IX__L9*/ meltfnum[2]);




#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:3247:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    {
                                        /*^locexp*/
                                        /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                                        melt_dbgcounter++;
#endif
                                        ;
                                    }
                                    ;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*_#MELT_NEED_DBG__L10*/ meltfnum[6] =
                                        /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                        (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                        0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                        ;;
                                    MELT_LOCATION("warmelt-outobj.melt:3247:/ cond");
                                    /*cond*/ if (/*_#MELT_NEED_DBG__L10*/ meltfnum[6]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*^block*/
                                            /*anyblock*/
                                            {

                                                /*_#MELT_CALLCOUNT__L11*/ meltfnum[10] =
                                                    /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                                    meltcallcount  /* melt_callcount debugging */
#else
                                                    0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                                    ;;

                                                MELT_CHECK_SIGNAL();
                                                ;
                                                MELT_LOCATION("warmelt-outobj.melt:3247:/ apply");
                                                /*apply*/
                                                {
                                                    union meltparam_un argtab[7];
                                                    memset(&argtab, 0, sizeof(argtab));
                                                    /*^apply.arg*/
                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[10];
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 3247;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_cstring =  "outpucod_objmatchflagblock curoflag=";
                                                    /*^apply.arg*/
                                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CUROFLAG__V20*/ meltfptr[18];
                                                    /*^apply.arg*/
                                                    argtab[5].meltbp_cstring =  " ix=";
                                                    /*^apply.arg*/
                                                    argtab[6].meltbp_long = /*_#IX__L9*/ meltfnum[2];
                                                    /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IF___V21*/ meltfptr[14] = /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:3247:/ clear");
                                                /*clear*/ /*_#MELT_CALLCOUNT__L11*/ meltfnum[10] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21] = 0 ;
                                            }
                                            ;
                                        }
                                    else    /*^cond.else*/
                                        {

                                            /*_._IF___V21*/ meltfptr[14] =  /*reallynil*/ NULL ;;
                                        }
                                    ;

                                    {
                                        MELT_LOCATION("warmelt-outobj.melt:3247:/ locexp");
                                        /*void*/(void)0;
                                    }
                                    ;
                                    /*^quasiblock*/


                                    /*epilog*/

                                    /*^clear*/
                                    /*clear*/ /*_#MELT_NEED_DBG__L10*/ meltfnum[6] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IF___V21*/ meltfptr[14] = 0 ;
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
                                MELT_LOCATION("warmelt-outobj.melt:3248:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*_#IS_A__L12*/ meltfnum[10] =
                                        melt_is_instance_of((melt_ptr_t)(/*_.CUROFLAG__V20*/ meltfptr[18]), (melt_ptr_t)((/*!CLASS_OBJFLAG*/ meltfrout->tabval[6])));;
                                    MELT_LOCATION("warmelt-outobj.melt:3248:/ cond");
                                    /*cond*/ if (/*_#IS_A__L12*/ meltfnum[10]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*_._IFELSE___V24*/ meltfptr[14] = (/*nil*/NULL);;
                                        }
                                    else
                                        {
                                            MELT_LOCATION("warmelt-outobj.melt:3248:/ cond.else");

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
                                                    argtab[0].meltbp_cstring =  "check curoflag";
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 3248;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CUROFLAG__V20*/ meltfptr[18];
                                                    /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IFELSE___V24*/ meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:3248:/ clear");
                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] = 0 ;
                                            }
                                            ;
                                        }
                                    ;
                                    /*_.IFCPP___V23*/ meltfptr[21] = /*_._IFELSE___V24*/ meltfptr[14];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:3248:/ clear");
                                    /*clear*/ /*_#IS_A__L12*/ meltfnum[10] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IFELSE___V24*/ meltfptr[14] = 0 ;
                                }

#else /*MELT_HAVE_DEBUG*/
                                /*^cppif.else*/
                                /*_.IFCPP___V23*/ meltfptr[21] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:3249:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTHSUCC__L5*/ meltfnum[1]), 0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:3250:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "bool "));
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:3251:/ msend");
                                /*msend*/
                                {
                                    union meltparam_un argtab[3];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^ojbmsend.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                    /*^ojbmsend.arg*/
                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                    /*^ojbmsend.arg*/
                                    argtab[2].meltbp_long = /*_#DEPTHSUCC__L5*/ meltfnum[1];
                                    /*_.OUTPUT_C_CODE__V26*/ meltfptr[24] = meltgc_send((melt_ptr_t)(/*_.CUROFLAG__V20*/ meltfptr[18]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[7])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:3252:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = false;"));
                                }
                                ;
                                if (/*_#IX__L9*/ meltfnum[2]<0) break;
                            } /* end  foreach_in_multiple meltcit1__EACHTUP */

                        /*citerepilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3244:/ clear");
                        /*clear*/ /*_.CUROFLAG__V20*/ meltfptr[18] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#IX__L9*/ meltfnum[2] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.IFCPP___V23*/ meltfptr[21] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.OUTPUT_C_CODE__V26*/ meltfptr[24] = 0 ;
                    } /*endciterblock FOREACH_IN_MULTIPLE*/
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3242:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_.IFCPP___V17*/ meltfptr[15] = 0 ;
                }
                ;
            } /*noelse*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3256:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L13*/ meltfnum[6] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:3256:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L13*/ meltfnum[6]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L14*/ meltfnum[10] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:3256:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L14*/ meltfnum[10];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3256;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objmatchflagblock obody=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OBODY__V12*/ meltfptr[11];
                            /*_.MELT_DEBUG_FUN__V28*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V27*/ meltfptr[14] = /*_.MELT_DEBUG_FUN__V28*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3256:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L14*/ meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V28*/ meltfptr[15] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V27*/ meltfptr[14] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:3256:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L13*/ meltfnum[6] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V27*/ meltfptr[14] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:3257:/ cond");
        /*cond*/ if (/*_.OBODY__V12*/ meltfptr[11]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:3258:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTHSUCC__L5*/ meltfnum[1]), 0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3259:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                        /*^apply.arg*/
                        argtab[2].meltbp_cstring =  "matchflagblock.body";
                        /*_.OUTPUT_LOCATION__V30*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.OLOC__V10*/ meltfptr[8]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:3260:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#IS_LIST__L15*/ meltfnum[10] =
                            (melt_magic_discr((melt_ptr_t)(/*_.OBODY__V12*/ meltfptr[11])) == MELTOBMAG_LIST);;
                        MELT_LOCATION("warmelt-outobj.melt:3260:/ cond");
                        /*cond*/ if (/*_#IS_LIST__L15*/ meltfnum[10]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V32*/ meltfptr[31] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:3260:/ cond.else");

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
                                        argtab[0].meltbp_cstring =  "check obody";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 3260;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBODY__V12*/ meltfptr[11];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V32*/ meltfptr[31] = /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[32];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:3260:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[32] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V31*/ meltfptr[30] = /*_._IFELSE___V32*/ meltfptr[31];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3260:/ clear");
                        /*clear*/ /*_#IS_LIST__L15*/ meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V32*/ meltfptr[31] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V31*/ meltfptr[30] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                    /*^compute*/
                    /*_.BOX__V34*/ meltfptr[32] =
                        /*full constboxing*/ /*boxing ctype_long*/ meltgc_new_int((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(DISCR_CONSTANT_INTEGER)))), (/*_#DEPTHSUCC__L5*/ meltfnum[1]));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3261:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.BOX__V34*/ meltfptr[32];
                        /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V35*/ meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_CODE_INSTRUCTIONS_LIST*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.OBODY__V12*/ meltfptr[11]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3257:/ quasiblock");


                    /*_.PROGN___V36*/ meltfptr[35] = /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V35*/ meltfptr[31];;
                    /*^compute*/
                    /*_._IF___V29*/ meltfptr[15] = /*_.PROGN___V36*/ meltfptr[35];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3257:/ clear");
                    /*clear*/ /*_.OUTPUT_LOCATION__V30*/ meltfptr[14] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.IFCPP___V31*/ meltfptr[30] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.BOX__V34*/ meltfptr[32] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V35*/ meltfptr[31] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V36*/ meltfptr[35] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V29*/ meltfptr[15] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3265:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            {
                /*^locexp*/
                /*melt_increment_dbgcounter*/
#if MELT_HAVE_DEBUG > 0
                melt_dbgcounter++;
#endif
                ;
            }
            ;

            MELT_CHECK_SIGNAL();
            ;
            /*_#MELT_NEED_DBG__L16*/ meltfnum[6] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:3265:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L16*/ meltfnum[6]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L17*/ meltfnum[10] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:3265:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L17*/ meltfnum[10];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3265;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objmatchflagblock oepil=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OEPIL__V13*/ meltfptr[12];
                            /*_.MELT_DEBUG_FUN__V38*/ meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V37*/ meltfptr[14] = /*_.MELT_DEBUG_FUN__V38*/ meltfptr[30];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3265:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L17*/ meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V38*/ meltfptr[30] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V37*/ meltfptr[14] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:3265:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L16*/ meltfnum[6] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V37*/ meltfptr[14] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:3266:/ cond");
        /*cond*/ if (/*_.OEPIL__V13*/ meltfptr[12]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:3267:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTHSUCC__L5*/ meltfnum[1]), 0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3268:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                        /*^apply.arg*/
                        argtab[2].meltbp_cstring =  "matchflagblock.epilog";
                        /*_.OUTPUT_LOCATION__V40*/ meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.OLOC__V10*/ meltfptr[8]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:3269:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#IS_LIST__L18*/ meltfnum[10] =
                            (melt_magic_discr((melt_ptr_t)(/*_.OEPIL__V13*/ meltfptr[12])) == MELTOBMAG_LIST);;
                        MELT_LOCATION("warmelt-outobj.melt:3269:/ cond");
                        /*cond*/ if (/*_#IS_LIST__L18*/ meltfnum[10]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V42*/ meltfptr[30] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:3269:/ cond.else");

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
                                        argtab[0].meltbp_cstring =  "check oepil";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 3269;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OEPIL__V13*/ meltfptr[12];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V43*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V42*/ meltfptr[30] = /*_.MELT_ASSERT_FAILURE_FUN__V43*/ meltfptr[14];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:3269:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V43*/ meltfptr[14] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V41*/ meltfptr[35] = /*_._IFELSE___V42*/ meltfptr[30];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3269:/ clear");
                        /*clear*/ /*_#IS_LIST__L18*/ meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V42*/ meltfptr[30] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V41*/ meltfptr[35] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                    /*^compute*/
                    /*_.BOX__V44*/ meltfptr[14] =
                        /*full constboxing*/ /*boxing ctype_long*/ meltgc_new_int((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(DISCR_CONSTANT_INTEGER)))), (/*_#DEPTHSUCC__L5*/ meltfnum[1]));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3270:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.BOX__V44*/ meltfptr[14];
                        /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V45*/ meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_CODE_INSTRUCTIONS_LIST*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.OEPIL__V13*/ meltfptr[12]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3266:/ quasiblock");


                    /*_.PROGN___V46*/ meltfptr[45] = /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V45*/ meltfptr[30];;
                    /*^compute*/
                    /*_._IF___V39*/ meltfptr[32] = /*_.PROGN___V46*/ meltfptr[45];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3266:/ clear");
                    /*clear*/ /*_.OUTPUT_LOCATION__V40*/ meltfptr[31] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.IFCPP___V41*/ meltfptr[35] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.BOX__V44*/ meltfptr[14] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V45*/ meltfptr[30] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V46*/ meltfptr[45] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V39*/ meltfptr[32] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3273:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3274:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";} /*end.matchflagblock*/  "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3275:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_LOCATION("warmelt-outobj.melt:3231:/ clear");
        /*clear*/ /*_.OLOC__V10*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OFLAGS__V11*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OBODY__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OEPIL__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_#DEPTHSUCC__L5*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V29*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V39*/ meltfptr[32] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:3228:/ clear");
        /*clear*/ /*_.IFCPP___V7*/ meltfptr[5] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJMATCHFLAGBLOCK", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_69_WARMELTmiOUTOBJ_OUTPUCOD_OBJMATCHFLAGBLOCK*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK fromline 1752 */

    /** start of frame for meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK// fromline 1568
        : public Melt_CallFrameWithValues<46>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[19];
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
        MeltFrame_meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<46> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK), clos) {};
        MeltFrame_meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK() //the constructor fromline 1642
            : Melt_CallFrameWithValues<46> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<46> (fil,lin, sizeof(MeltFrame_meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK)) {};
        MeltFrame_meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<46> (fil,lin, sizeof(MeltFrame_meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK


    /** end of frame for meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK fromline 1697**/

    /* end of frame for routine meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK fromline 1756 */

    /* classy proc frame meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK */ MeltFrame_meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJMULTIALLOCBLOCK", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3280:/ getarg");
    /*_.OBLO__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3281:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJMULTIALLOCBLOCK*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3281:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3281:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oblo";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3281;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBLO__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3281:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3281:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3282:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.OLOC__V8*/ meltfptr[6] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3283:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OMALBLO_ALLSTRUCT");
            /*_.OALLSTRUCT__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3284:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 4, "OMALBLO_NAME");
            /*_.ONAME__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3285:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBLO_EPIL");
            /*_.EPIL__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3286:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBLO_BODYL");
            /*_.BODYL__V12*/ meltfptr[11] = slot;
        };
        ;
        /*_#DEPTHP1__L3*/ meltfnum[1] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;
        /*^compute*/
        /*_.BOXDEPTHP1__V13*/ meltfptr[12] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[2])), (/*_#DEPTHP1__L3*/ meltfnum[1])));;
        MELT_LOCATION("warmelt-outobj.melt:3289:/ quasiblock");


        /*_.PTRBUF__V15*/ meltfptr[14] =
            (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[3])), (const char*)0);;

        {
            MELT_LOCATION("warmelt-outobj.melt:3290:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.PTRBUF__V15*/ meltfptr[14]),
                                                  melt_string_str((melt_ptr_t)(/*_.ONAME__V10*/ meltfptr[9])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3291:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.PTRBUF__V15*/ meltfptr[14]), ( "_ptr"));
        }
        ;
        /*_.STRBUF2STRING__V16*/ meltfptr[15] =
            (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[4])), melt_strbuf_str((melt_ptr_t)(/*_.PTRBUF__V15*/ meltfptr[14]))));;
        /*^compute*/
        /*_.LET___V14*/ meltfptr[13] = /*_.STRBUF2STRING__V16*/ meltfptr[15];;

        MELT_LOCATION("warmelt-outobj.melt:3289:/ clear");
        /*clear*/ /*_.PTRBUF__V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.STRBUF2STRING__V16*/ meltfptr[15] = 0 ;
        /*_.ONAMEPTR__V17*/ meltfptr[14] = /*_.LET___V14*/ meltfptr[13];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3294:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "blockmultialloc";
            /*_.OUTPUT_LOCATION__V18*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.OLOC__V8*/ meltfptr[6]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3295:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_MULTIPLE_OR_NULL__L4*/ meltfnum[3] =
                ((/*_.OALLSTRUCT__V9*/ meltfptr[5]) == NULL || (melt_unsafe_magic_discr((melt_ptr_t)(/*_.OALLSTRUCT__V9*/ meltfptr[5])) == MELTOBMAG_MULTIPLE));;
            MELT_LOCATION("warmelt-outobj.melt:3295:/ cond");
            /*cond*/ if (/*_#IS_MULTIPLE_OR_NULL__L4*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V20*/ meltfptr[19] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3295:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oallstruct";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3295;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OALLSTRUCT__V9*/ meltfptr[5];
                            /*_.MELT_ASSERT_FAILURE_FUN__V21*/ meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V20*/ meltfptr[19] = /*_.MELT_ASSERT_FAILURE_FUN__V21*/ meltfptr[20];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3295:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V21*/ meltfptr[20] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V19*/ meltfptr[18] = /*_._IFELSE___V20*/ meltfptr[19];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3295:/ clear");
            /*clear*/ /*_#IS_MULTIPLE_OR_NULL__L4*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V20*/ meltfptr[19] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V19*/ meltfptr[18] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3296:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*multiallocblock*/{"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3297:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTHP1__L3*/ meltfnum[1]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3298:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "struct "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3299:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.ONAME__V10*/ meltfptr[9])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3300:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "_st {"));
        }
        ;
        /*citerblock FOREACH_IN_MULTIPLE*/
        {
            /* start foreach_in_multiple meltcit1__EACHTUP */
            long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.OALLSTRUCT__V9*/ meltfptr[5]);
            for (/*_#STRIX__L5*/ meltfnum[3] = 0;
                                 (/*_#STRIX__L5*/ meltfnum[3] >= 0) && (/*_#STRIX__L5*/ meltfnum[3] <  meltcit1__EACHTUP_ln);
                                 /*_#STRIX__L5*/ meltfnum[3]++)
                {
                    /*_.CURSTRU__V22*/ meltfptr[20] = melt_multiple_nth((melt_ptr_t)(/*_.OALLSTRUCT__V9*/ meltfptr[5]),  /*_#STRIX__L5*/ meltfnum[3]);




#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:3304:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#IS_A__L6*/ meltfnum[5] =
                            melt_is_instance_of((melt_ptr_t)(/*_.CURSTRU__V22*/ meltfptr[20]), (melt_ptr_t)((/*!CLASS_OBJINITELEM*/ meltfrout->tabval[6])));;
                        MELT_LOCATION("warmelt-outobj.melt:3304:/ cond");
                        /*cond*/ if (/*_#IS_A__L6*/ meltfnum[5]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V24*/ meltfptr[23] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:3304:/ cond.else");

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
                                        argtab[0].meltbp_cstring =  "check curstru";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 3304;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CURSTRU__V22*/ meltfptr[20];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V24*/ meltfptr[23] = /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:3304:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V23*/ meltfptr[19] = /*_._IFELSE___V24*/ meltfptr[23];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3304:/ clear");
                        /*clear*/ /*_#IS_A__L6*/ meltfnum[5] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V24*/ meltfptr[23] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V23*/ meltfptr[19] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3305:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTHP1__L3*/ meltfnum[1]), 0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3306:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*_.OUTPUT_C_DECLINIT__V26*/ meltfptr[24] = meltgc_send((melt_ptr_t)(/*_.CURSTRU__V22*/ meltfptr[20]), (melt_ptr_t)((/*!OUTPUT_C_DECLINIT*/ meltfrout->tabval[7])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:3307:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[5] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                        MELT_LOCATION("warmelt-outobj.melt:3308:/ cond");
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
                        /*_#GET_INT__L8*/ meltfnum[7] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V28*/ meltfptr[27])));;
                        /*^compute*/
                        /*_#ltI__L9*/ meltfnum[8] =
                            ((/*_#STRBUF_USEDLENGTH__L7*/ meltfnum[5]) < (/*_#GET_INT__L8*/ meltfnum[7]));;
                        MELT_LOCATION("warmelt-outobj.melt:3307:/ cond");
                        /*cond*/ if (/*_#ltI__L9*/ meltfnum[8]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V29*/ meltfptr[28] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:3307:/ cond.else");

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
                                        argtab[2].meltbp_long = 3307;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V30*/ meltfptr[29] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V29*/ meltfptr[28] = /*_.MELT_ASSERT_FAILURE_FUN__V30*/ meltfptr[29];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:3307:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V30*/ meltfptr[29] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V27*/ meltfptr[23] = /*_._IFELSE___V29*/ meltfptr[28];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3307:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[5] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V28*/ meltfptr[27] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L8*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L9*/ meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V29*/ meltfptr[28] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V27*/ meltfptr[23] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                    if (/*_#STRIX__L5*/ meltfnum[3]<0) break;
                } /* end  foreach_in_multiple meltcit1__EACHTUP */

            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:3301:/ clear");
            /*clear*/ /*_.CURSTRU__V22*/ meltfptr[20] = 0 ;
            /*^clear*/
            /*clear*/ /*_#STRIX__L5*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V23*/ meltfptr[19] = 0 ;
            /*^clear*/
            /*clear*/ /*_.OUTPUT_C_DECLINIT__V26*/ meltfptr[24] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V27*/ meltfptr[23] = 0 ;
        } /*endciterblock FOREACH_IN_MULTIPLE*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3310:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " long "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3311:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.ONAME__V10*/ meltfptr[9])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3312:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "_endgap; } *"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3313:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.ONAME__V10*/ meltfptr[9])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3314:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "_ptr = 0;"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3315:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTHP1__L3*/ meltfnum[1]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3316:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.ONAME__V10*/ meltfptr[9])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3317:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "_ptr = (struct "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3318:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.ONAME__V10*/ meltfptr[9])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3319:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "_st *) meltgc_allocate (sizeof (struct "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3320:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.ONAME__V10*/ meltfptr[9])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3321:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "_st), 0);"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3322:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTHP1__L3*/ meltfnum[1]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3324:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "blockmultialloc.initfill";
            /*_.OUTPUT_LOCATION__V31*/ meltfptr[29] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.OLOC__V8*/ meltfptr[6]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*citerblock FOREACH_IN_MULTIPLE*/
        {
            /* start foreach_in_multiple meltcit2__EACHTUP */
            long  meltcit2__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.OALLSTRUCT__V9*/ meltfptr[5]);
            for (/*_#STRIX__L10*/ meltfnum[5] = 0;
                                  (/*_#STRIX__L10*/ meltfnum[5] >= 0) && (/*_#STRIX__L10*/ meltfnum[5] <  meltcit2__EACHTUP_ln);
                                  /*_#STRIX__L10*/ meltfnum[5]++)
                {
                    /*_.CURSTRU__V32*/ meltfptr[27] = melt_multiple_nth((melt_ptr_t)(/*_.OALLSTRUCT__V9*/ meltfptr[5]),  /*_#STRIX__L10*/ meltfnum[5]);




                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3328:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.ONAMEPTR__V17*/ meltfptr[14];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#DEPTHP1__L3*/ meltfnum[1];
                        /*_.OUTPUT_C_INITIAL_FILL__V33*/ meltfptr[28] = meltgc_send((melt_ptr_t)(/*_.CURSTRU__V32*/ meltfptr[27]), (melt_ptr_t)((/*!OUTPUT_C_INITIAL_FILL*/ meltfrout->tabval[10])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:3329:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L11*/ meltfnum[7] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                        MELT_LOCATION("warmelt-outobj.melt:3330:/ cond");
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
                                    /*_.REFERENCED_VALUE__V35*/ meltfptr[34] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V35*/ meltfptr[34] =  /*reallynil*/ NULL ;;
                            }
                        ;
                        /*^compute*/
                        /*_#GET_INT__L12*/ meltfnum[8] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V35*/ meltfptr[34])));;
                        /*^compute*/
                        /*_#ltI__L13*/ meltfnum[12] =
                            ((/*_#STRBUF_USEDLENGTH__L11*/ meltfnum[7]) < (/*_#GET_INT__L12*/ meltfnum[8]));;
                        MELT_LOCATION("warmelt-outobj.melt:3329:/ cond");
                        /*cond*/ if (/*_#ltI__L13*/ meltfnum[12]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V36*/ meltfptr[35] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:3329:/ cond.else");

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
                                        argtab[2].meltbp_long = 3329;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V37*/ meltfptr[36] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V36*/ meltfptr[35] = /*_.MELT_ASSERT_FAILURE_FUN__V37*/ meltfptr[36];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:3329:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V37*/ meltfptr[36] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V34*/ meltfptr[33] = /*_._IFELSE___V36*/ meltfptr[35];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3329:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L11*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V35*/ meltfptr[34] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L12*/ meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L13*/ meltfnum[12] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V36*/ meltfptr[35] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V34*/ meltfptr[33] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3331:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTHP1__L3*/ meltfnum[1]), 0);
                    }
                    ;
                    if (/*_#STRIX__L10*/ meltfnum[5]<0) break;
                } /* end  foreach_in_multiple meltcit2__EACHTUP */

            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:3325:/ clear");
            /*clear*/ /*_.CURSTRU__V32*/ meltfptr[27] = 0 ;
            /*^clear*/
            /*clear*/ /*_#STRIX__L10*/ meltfnum[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_.OUTPUT_C_INITIAL_FILL__V33*/ meltfptr[28] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V34*/ meltfptr[33] = 0 ;
        } /*endciterblock FOREACH_IN_MULTIPLE*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_LIST__L14*/ meltfnum[7] =
            (melt_magic_discr((melt_ptr_t)(/*_.BODYL__V12*/ meltfptr[11])) == MELTOBMAG_LIST);;
        MELT_LOCATION("warmelt-outobj.melt:3334:/ cond");
        /*cond*/ if (/*_#IS_LIST__L14*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3335:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.BOXDEPTHP1__V13*/ meltfptr[12];
                        /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V39*/ meltfptr[34] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_CODE_INSTRUCTIONS_LIST*/ meltfrout->tabval[11])), (melt_ptr_t)(/*_.BODYL__V12*/ meltfptr[11]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V38*/ meltfptr[36] = /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V39*/ meltfptr[34];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3334:/ clear");
                    /*clear*/ /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V39*/ meltfptr[34] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V38*/ meltfptr[36] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_LIST__L15*/ meltfnum[8] =
            (melt_magic_discr((melt_ptr_t)(/*_.EPIL__V11*/ meltfptr[10])) == MELTOBMAG_LIST);;
        MELT_LOCATION("warmelt-outobj.melt:3337:/ cond");
        /*cond*/ if (/*_#IS_LIST__L15*/ meltfnum[8]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_#GET_INT__L16*/ meltfnum[12] =
                        (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V13*/ meltfptr[12])));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3339:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L16*/ meltfnum[12]), 0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3340:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*epilog*/"));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3341:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.BOXDEPTHP1__V13*/ meltfptr[12];
                        /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V41*/ meltfptr[34] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_CODE_INSTRUCTIONS_LIST*/ meltfrout->tabval[11])), (melt_ptr_t)(/*_.EPIL__V11*/ meltfptr[10]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3338:/ quasiblock");


                    /*_.PROGN___V42*/ meltfptr[41] = /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V41*/ meltfptr[34];;
                    /*^compute*/
                    /*_._IF___V40*/ meltfptr[35] = /*_.PROGN___V42*/ meltfptr[41];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3337:/ clear");
                    /*clear*/ /*_#GET_INT__L16*/ meltfnum[12] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V41*/ meltfptr[34] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V42*/ meltfptr[41] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V40*/ meltfptr[35] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3342:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "} /*end multiallocblock*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3343:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_LOCATION("warmelt-outobj.melt:3282:/ clear");
        /*clear*/ /*_.OLOC__V8*/ meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OALLSTRUCT__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ONAME__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.EPIL__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BODYL__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_#DEPTHP1__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BOXDEPTHP1__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ONAMEPTR__V17*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V18*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V31*/ meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_LIST__L14*/ meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V38*/ meltfptr[36] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_LIST__L15*/ meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V40*/ meltfptr[35] = 0 ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3345:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L17*/ meltfnum[12] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3346:/ cond");
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
                        /*_.REFERENCED_VALUE__V44*/ meltfptr[41] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V44*/ meltfptr[41] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L18*/ meltfnum[1] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V44*/ meltfptr[41])));;
            /*^compute*/
            /*_#ltI__L19*/ meltfnum[7] =
                ((/*_#STRBUF_USEDLENGTH__L17*/ meltfnum[12]) < (/*_#GET_INT__L18*/ meltfnum[1]));;
            MELT_LOCATION("warmelt-outobj.melt:3345:/ cond");
            /*cond*/ if (/*_#ltI__L19*/ meltfnum[7]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V45*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3345:/ cond.else");

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
                            argtab[2].meltbp_long = 3345;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V46*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V45*/ meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V46*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3345:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V46*/ meltfptr[5] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V43*/ meltfptr[34] = /*_._IFELSE___V45*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3345:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L17*/ meltfnum[12] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V44*/ meltfptr[41] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L18*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L19*/ meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V45*/ meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V43*/ meltfptr[34] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3280:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.IFCPP___V43*/ meltfptr[34];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3280:/ locexp");
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
        /*clear*/ /*_.IFCPP___V43*/ meltfptr[34] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJMULTIALLOCBLOCK", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_70_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIALLOCBLOCK*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK fromline 1752 */

    /** start of frame for meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK// fromline 1568
        : public Melt_CallFrameWithValues<45>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[20];
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
        MeltFrame_meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<45> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK), clos) {};
        MeltFrame_meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK() //the constructor fromline 1642
            : Melt_CallFrameWithValues<45> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<45> (fil,lin, sizeof(MeltFrame_meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK)) {};
        MeltFrame_meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<45> (fil,lin, sizeof(MeltFrame_meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK


    /** end of frame for meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK fromline 1697**/

    /* end of frame for routine meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK fromline 1756 */

    /* classy proc frame meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK */ MeltFrame_meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJCITERBLOCK", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3351:/ getarg");
    /*_.OBCIT__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3352:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBCIT__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJCITERBLOCK*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3352:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3352:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check obcit";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3352;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBCIT__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3352:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3352:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3353:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBCIT__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.OLOC__V8*/ meltfptr[6] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3354:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBCIT__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBLO_BODYL");
            /*_.BODYL__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3355:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBCIT__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBLO_EPIL");
            /*_.EPIL__V10*/ meltfptr[9] = slot;
        };
        ;
        /*_#plI__L3*/ meltfnum[1] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;
        /*^compute*/
        /*_.BOXDEPTHP1__V11*/ meltfptr[10] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[2])), (/*_#plI__L3*/ meltfnum[1])));;
        MELT_LOCATION("warmelt-outobj.melt:3357:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBCIT__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 4, "OBCITER_BEFORE");
            /*_.OBEFORE__V12*/ meltfptr[11] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3358:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBCIT__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 5, "OBCITER_AFTER");
            /*_.OAFTER__V13*/ meltfptr[12] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3359:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBCIT__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OBCITER_CITER");
            /*_.CITER__V14*/ meltfptr[13] = slot;
        };
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3361:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[3] =
                melt_is_instance_of((melt_ptr_t)(/*_.CITER__V14*/ meltfptr[13]), (melt_ptr_t)((/*!CLASS_CITERATOR*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-outobj.melt:3361:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V16*/ meltfptr[15] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3361:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check citer";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3361;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CITER__V14*/ meltfptr[13];
                            /*_.MELT_ASSERT_FAILURE_FUN__V17*/ meltfptr[16] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V16*/ meltfptr[15] = /*_.MELT_ASSERT_FAILURE_FUN__V17*/ meltfptr[16];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3361:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V17*/ meltfptr[16] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V15*/ meltfptr[14] = /*_._IFELSE___V16*/ meltfptr[15];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3361:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V16*/ meltfptr[15] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V15*/ meltfptr[14] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3362:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "citerblock";
            /*_.OUTPUT_LOCATION__V18*/ meltfptr[16] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.OLOC__V8*/ meltfptr[6]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3363:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*citerblock "));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:3364:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.CITER__V14*/ meltfptr[13]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
            /*_.NAMED_NAME__V19*/ meltfptr[15] = slot;
        };
        ;

        {
            /*^locexp*/
            meltgc_add_strbuf_ccomment((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                       melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V19*/ meltfptr[15])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3365:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*/ {"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3366:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3367:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "citerbefore";
            /*_.OUTPUT_LOCATION__V20*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.OLOC__V8*/ meltfptr[6]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*citerblock FOREACH_IN_MULTIPLE*/
        {
            /* start foreach_in_multiple meltcit1__EACHTUP */
            long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.OBEFORE__V12*/ meltfptr[11]);
            for (/*_#IX__L5*/ meltfnum[3] = 0;
                              (/*_#IX__L5*/ meltfnum[3] >= 0) && (/*_#IX__L5*/ meltfnum[3] <  meltcit1__EACHTUP_ln);
                              /*_#IX__L5*/ meltfnum[3]++)
                {
                    /*_.OBEF__V21*/ meltfptr[20] = melt_multiple_nth((melt_ptr_t)(/*_.OBEFORE__V12*/ meltfptr[11]),  /*_#IX__L5*/ meltfnum[3]);



                    /*_#GET_INT__L6*/ meltfnum[5] =
                        (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V11*/ meltfptr[10])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3371:/ msend");
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
                        /*_.OUTPUT_C_CODE__V22*/ meltfptr[21] = meltgc_send((melt_ptr_t)(/*_.OBEF__V21*/ meltfptr[20]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:3372:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[6] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                        MELT_LOCATION("warmelt-outobj.melt:3373:/ cond");
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
                        /*_#GET_INT__L8*/ meltfnum[7] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V24*/ meltfptr[23])));;
                        /*^compute*/
                        /*_#ltI__L9*/ meltfnum[8] =
                            ((/*_#STRBUF_USEDLENGTH__L7*/ meltfnum[6]) < (/*_#GET_INT__L8*/ meltfnum[7]));;
                        MELT_LOCATION("warmelt-outobj.melt:3372:/ cond");
                        /*cond*/ if (/*_#ltI__L9*/ meltfnum[8]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V25*/ meltfptr[24] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:3372:/ cond.else");

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
                                        argtab[2].meltbp_long = 3372;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V26*/ meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V25*/ meltfptr[24] = /*_.MELT_ASSERT_FAILURE_FUN__V26*/ meltfptr[25];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:3372:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V26*/ meltfptr[25] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V23*/ meltfptr[22] = /*_._IFELSE___V25*/ meltfptr[24];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3372:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[6] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V24*/ meltfptr[23] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L8*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L9*/ meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V25*/ meltfptr[24] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V23*/ meltfptr[22] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                    if (/*_#IX__L5*/ meltfnum[3]<0) break;
                } /* end  foreach_in_multiple meltcit1__EACHTUP */

            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:3368:/ clear");
            /*clear*/ /*_.OBEF__V21*/ meltfptr[20] = 0 ;
            /*^clear*/
            /*clear*/ /*_#IX__L5*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L6*/ meltfnum[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_.OUTPUT_C_CODE__V22*/ meltfptr[21] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V23*/ meltfptr[22] = 0 ;
        } /*endciterblock FOREACH_IN_MULTIPLE*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3375:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3376:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "citerbody";
            /*_.OUTPUT_LOCATION__V27*/ meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.OLOC__V8*/ meltfptr[6]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_LIST__L10*/ meltfnum[6] =
            (melt_magic_discr((melt_ptr_t)(/*_.BODYL__V9*/ meltfptr[5])) == MELTOBMAG_LIST);;
        MELT_LOCATION("warmelt-outobj.melt:3377:/ cond");
        /*cond*/ if (/*_#IS_LIST__L10*/ meltfnum[6]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3378:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.BOXDEPTHP1__V11*/ meltfptr[10];
                        /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V29*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_CODE_INSTRUCTIONS_LIST*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.BODYL__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V28*/ meltfptr[23] = /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V29*/ meltfptr[24];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3377:/ clear");
                    /*clear*/ /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V29*/ meltfptr[24] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V28*/ meltfptr[23] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3379:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3380:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "citerafter";
            /*_.OUTPUT_LOCATION__V30*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.OLOC__V8*/ meltfptr[6]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*citerblock FOREACH_IN_MULTIPLE*/
        {
            /* start foreach_in_multiple meltcit2__EACHTUP */
            long  meltcit2__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.OAFTER__V13*/ meltfptr[12]);
            for (/*_#IX__L11*/ meltfnum[7] = 0;
                               (/*_#IX__L11*/ meltfnum[7] >= 0) && (/*_#IX__L11*/ meltfnum[7] <  meltcit2__EACHTUP_ln);
                               /*_#IX__L11*/ meltfnum[7]++)
                {
                    /*_.OAFT__V31*/ meltfptr[30] = melt_multiple_nth((melt_ptr_t)(/*_.OAFTER__V13*/ meltfptr[12]),  /*_#IX__L11*/ meltfnum[7]);



                    /*_#GET_INT__L12*/ meltfnum[8] =
                        (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V11*/ meltfptr[10])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3384:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#GET_INT__L12*/ meltfnum[8];
                        /*_.OUTPUT_C_CODE__V32*/ meltfptr[31] = meltgc_send((melt_ptr_t)(/*_.OAFT__V31*/ meltfptr[30]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:3385:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L13*/ meltfnum[12] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                        MELT_LOCATION("warmelt-outobj.melt:3386:/ cond");
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
                                    /*_.REFERENCED_VALUE__V34*/ meltfptr[33] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V34*/ meltfptr[33] =  /*reallynil*/ NULL ;;
                            }
                        ;
                        /*^compute*/
                        /*_#GET_INT__L14*/ meltfnum[13] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V34*/ meltfptr[33])));;
                        /*^compute*/
                        /*_#ltI__L15*/ meltfnum[14] =
                            ((/*_#STRBUF_USEDLENGTH__L13*/ meltfnum[12]) < (/*_#GET_INT__L14*/ meltfnum[13]));;
                        MELT_LOCATION("warmelt-outobj.melt:3385:/ cond");
                        /*cond*/ if (/*_#ltI__L15*/ meltfnum[14]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V35*/ meltfptr[34] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:3385:/ cond.else");

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
                                        argtab[2].meltbp_long = 3385;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V36*/ meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V35*/ meltfptr[34] = /*_.MELT_ASSERT_FAILURE_FUN__V36*/ meltfptr[35];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:3385:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V36*/ meltfptr[35] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V33*/ meltfptr[32] = /*_._IFELSE___V35*/ meltfptr[34];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3385:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L13*/ meltfnum[12] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V34*/ meltfptr[33] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L14*/ meltfnum[13] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L15*/ meltfnum[14] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V35*/ meltfptr[34] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V33*/ meltfptr[32] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                    if (/*_#IX__L11*/ meltfnum[7]<0) break;
                } /* end  foreach_in_multiple meltcit2__EACHTUP */

            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:3381:/ clear");
            /*clear*/ /*_.OAFT__V31*/ meltfptr[30] = 0 ;
            /*^clear*/
            /*clear*/ /*_#IX__L11*/ meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L12*/ meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_.OUTPUT_C_CODE__V32*/ meltfptr[31] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V33*/ meltfptr[32] = 0 ;
        } /*endciterblock FOREACH_IN_MULTIPLE*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3388:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3389:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "citerepil";
            /*_.OUTPUT_LOCATION__V37*/ meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.OLOC__V8*/ meltfptr[6]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_LIST__L16*/ meltfnum[12] =
            (melt_magic_discr((melt_ptr_t)(/*_.EPIL__V10*/ meltfptr[9])) == MELTOBMAG_LIST);;
        MELT_LOCATION("warmelt-outobj.melt:3390:/ cond");
        /*cond*/ if (/*_#IS_LIST__L16*/ meltfnum[12]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_#GET_INT__L17*/ meltfnum[13] =
                        (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V11*/ meltfptr[10])));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3392:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L17*/ meltfnum[13]), 0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3393:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*citerepilog*/"));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3394:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.BOXDEPTHP1__V11*/ meltfptr[10];
                        /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V39*/ meltfptr[34] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_CODE_INSTRUCTIONS_LIST*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.EPIL__V10*/ meltfptr[9]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3391:/ quasiblock");


                    /*_.PROGN___V40*/ meltfptr[39] = /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V39*/ meltfptr[34];;
                    /*^compute*/
                    /*_._IF___V38*/ meltfptr[33] = /*_.PROGN___V40*/ meltfptr[39];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3390:/ clear");
                    /*clear*/ /*_#GET_INT__L17*/ meltfnum[13] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V39*/ meltfptr[34] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V40*/ meltfptr[39] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V38*/ meltfptr[33] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3395:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "} /*endciterblock "));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:3396:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.CITER__V14*/ meltfptr[13]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
            /*_.NAMED_NAME__V41*/ meltfptr[34] = slot;
        };
        ;

        {
            /*^locexp*/
            meltgc_add_strbuf_ccomment((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                       melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V41*/ meltfptr[34])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3397:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3398:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_LOCATION("warmelt-outobj.melt:3353:/ clear");
        /*clear*/ /*_.OLOC__V8*/ meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BODYL__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.EPIL__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BOXDEPTHP1__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OBEFORE__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OAFTER__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CITER__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V18*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NAMED_NAME__V19*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V20*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V27*/ meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_LIST__L10*/ meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V28*/ meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V30*/ meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V37*/ meltfptr[35] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_LIST__L16*/ meltfnum[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V38*/ meltfptr[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NAMED_NAME__V41*/ meltfptr[34] = 0 ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3400:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L18*/ meltfnum[14] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3401:/ cond");
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
                        /*_.REFERENCED_VALUE__V43*/ meltfptr[6] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V43*/ meltfptr[6] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L19*/ meltfnum[13] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V43*/ meltfptr[6])));;
            /*^compute*/
            /*_#ltI__L20*/ meltfnum[1] =
                ((/*_#STRBUF_USEDLENGTH__L18*/ meltfnum[14]) < (/*_#GET_INT__L19*/ meltfnum[13]));;
            MELT_LOCATION("warmelt-outobj.melt:3400:/ cond");
            /*cond*/ if (/*_#ltI__L20*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V44*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3400:/ cond.else");

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
                            argtab[2].meltbp_long = 3400;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V45*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V44*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V45*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3400:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V45*/ meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V42*/ meltfptr[39] = /*_._IFELSE___V44*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3400:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L18*/ meltfnum[14] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V43*/ meltfptr[6] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L19*/ meltfnum[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L20*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V44*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V42*/ meltfptr[39] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3351:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.IFCPP___V42*/ meltfptr[39];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3351:/ locexp");
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
        /*clear*/ /*_.IFCPP___V42*/ meltfptr[39] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJCITERBLOCK", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_71_WARMELTmiOUTOBJ_OUTPUCOD_OBJCITERBLOCK*/




/**** end of warmelt-outobj+03.cc ****/

