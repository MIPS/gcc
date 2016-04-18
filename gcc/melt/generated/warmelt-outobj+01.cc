/* GCC MELT GENERATED C++ FILE warmelt-outobj+01.cc - DO NOT EDIT - see http://gcc-melt.org/ */
/* secondary MELT generated C++ file of rank #1 */
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt_f1[] = MELT_RUN_HASHMD5 /* from melt-run.h */;


/**** warmelt-outobj+01.cc declarations ****/
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



/**** warmelt-outobj+01.cc implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM fromline 1752 */

    /** start of frame for meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM// fromline 1568
        : public Melt_CallFrameWithValues<37>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[21];
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
        MeltFrame_meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<37> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM), clos) {};
        MeltFrame_meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM() //the constructor fromline 1642
            : Melt_CallFrameWithValues<37> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<37> (fil,lin, sizeof(MeltFrame_meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM)) {};
        MeltFrame_meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<37> (fil,lin, sizeof(MeltFrame_meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM


    /** end of frame for meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM fromline 1697**/

    /* end of frame for routine meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM fromline 1756 */

    /* classy proc frame meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM */ MeltFrame_meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJINIELEM", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:166:/ getarg");
    /*_.OBIELEM__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:167:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBIELEM__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJINITELEM*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:167:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:167:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check obelem";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 167;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBIELEM__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:167:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:167:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:168:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBIELEM__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 4, "OIE_LOCVAR");
            /*_.OLOCVAR__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:169:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBIELEM__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OIE_CNAME");
            /*_.CNAM__V10*/ meltfptr[9] = slot;
        };
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:170:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRING__L3*/ meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.CNAM__V10*/ meltfptr[9])) == MELTOBMAG_STRING);;
            MELT_LOCATION("warmelt-outobj.melt:170:/ cond");
            /*cond*/ if (/*_#IS_STRING__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[11] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:170:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check cnam";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 170;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CNAM__V10*/ meltfptr[9];
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/ meltfptr[11] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:170:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/ meltfptr[10] = /*_._IFELSE___V12*/ meltfptr[11];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:170:/ clear");
            /*clear*/ /*_#IS_STRING__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V12*/ meltfptr[11] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V11*/ meltfptr[10] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:171:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*^cond*/
            /*cond*/ if (/*_.OLOCVAR__V9*/ meltfptr[5]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[11] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:171:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check olocvar";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 171;
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/ meltfptr[11] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:171:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[11];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:171:/ clear");
            /*clear*/ /*_._IFELSE___V15*/ meltfptr[11] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[12] = (/*nil*/NULL);
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OBIELEM__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V18*/ meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V17*/ meltfptr[15] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V18*/ meltfptr[11];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V18*/ meltfptr[11] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V17*/ meltfptr[15] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:173:/ msend");
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
            /*_.OUTPUT_C_CODE__V19*/ meltfptr[11] = meltgc_send((melt_ptr_t)(/*_.OLOCVAR__V9*/ meltfptr[5]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[3])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#STRBUF_USEDLENGTH__L4*/ meltfnum[1] =
            melt_strbuf_usedlength((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]));;
        MELT_LOCATION("warmelt-outobj.melt:175:/ cond");
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
        /*_#diIRAW__L6*/ meltfnum[5] =
            ((/*_#GET_INT__L5*/ meltfnum[4]) / (2));;
        /*^compute*/
        /*_#gtI__L7*/ meltfnum[6] =
            ((/*_#STRBUF_USEDLENGTH__L4*/ meltfnum[1]) > (/*_#diIRAW__L6*/ meltfnum[5]));;
        MELT_LOCATION("warmelt-outobj.melt:175:/ cond");
        /*cond*/ if (/*_#gtI__L7*/ meltfnum[6]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:176:/ locexp");

#if MELT_HAVE_DEBUG
                        if (melt_need_debug (0))
                            melt_dbgshortbacktrace(( "outpucod_objinielem huge declbuf"), (10));
#endif
                        ;
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:177:/ cppif.then");
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
                        MELT_LOCATION("warmelt-outobj.melt:177:/ cond");
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
                                    MELT_LOCATION("warmelt-outobj.melt:177:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[8];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 177;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "outpucod_objinielem huge declbuf=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                        /*_.MELT_DEBUG_FUN__V23*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[6])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V22*/ meltfptr[21] = /*_.MELT_DEBUG_FUN__V23*/ meltfptr[22];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:177:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L9*/ meltfnum[8] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V23*/ meltfptr[22] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V22*/ meltfptr[21] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:177:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L8*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V22*/ meltfptr[21] = 0 ;
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
                    MELT_LOCATION("warmelt-outobj.melt:178:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L10*/ meltfnum[8] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.DECLBUF__V3*/ meltfptr[2]));;
                        MELT_LOCATION("warmelt-outobj.melt:179:/ cond");
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
                                    /*_.REFERENCED_VALUE__V25*/ meltfptr[21] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V25*/ meltfptr[21] =  /*reallynil*/ NULL ;;
                            }
                        ;
                        /*^compute*/
                        /*_#GET_INT__L11*/ meltfnum[7] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V25*/ meltfptr[21])));;
                        /*^compute*/
                        /*_#ltI__L12*/ meltfnum[11] =
                            ((/*_#STRBUF_USEDLENGTH__L10*/ meltfnum[8]) < (/*_#GET_INT__L11*/ meltfnum[7]));;
                        MELT_LOCATION("warmelt-outobj.melt:178:/ cond");
                        /*cond*/ if (/*_#ltI__L12*/ meltfnum[11]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V26*/ meltfptr[25] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:178:/ cond.else");

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
                                        argtab[2].meltbp_long = 178;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V26*/ meltfptr[25] = /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:178:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V24*/ meltfptr[22] = /*_._IFELSE___V26*/ meltfptr[25];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:178:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L10*/ meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V25*/ meltfptr[21] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L11*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L12*/ meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V26*/ meltfptr[25] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V24*/ meltfptr[22] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:175:/ quasiblock");


                    /*_.PROGN___V28*/ meltfptr[26] = /*_.IFCPP___V24*/ meltfptr[22];;
                    /*^compute*/
                    /*_._IFELSE___V21*/ meltfptr[20] = /*_.PROGN___V28*/ meltfptr[26];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:175:/ clear");
                    /*clear*/ /*_.IFCPP___V24*/ meltfptr[22] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V28*/ meltfptr[26] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IFELSE___V21*/ meltfptr[20] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#STRBUF_USEDLENGTH__L13*/ meltfnum[8] =
            melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
        MELT_LOCATION("warmelt-outobj.melt:181:/ cond");
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
                    /*_.REFERENCED_VALUE__V29*/ meltfptr[21] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.REFERENCED_VALUE__V29*/ meltfptr[21] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#GET_INT__L14*/ meltfnum[7] =
            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V29*/ meltfptr[21])));;
        /*^compute*/
        /*_#diIRAW__L15*/ meltfnum[11] =
            ((/*_#GET_INT__L14*/ meltfnum[7]) / (2));;
        /*^compute*/
        /*_#gtI__L16*/ meltfnum[15] =
            ((/*_#STRBUF_USEDLENGTH__L13*/ meltfnum[8]) > (/*_#diIRAW__L15*/ meltfnum[11]));;
        MELT_LOCATION("warmelt-outobj.melt:181:/ cond");
        /*cond*/ if (/*_#gtI__L16*/ meltfnum[15]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:182:/ locexp");

#if MELT_HAVE_DEBUG
                        if (melt_need_debug (0))
                            melt_dbgshortbacktrace(( "outpucod_objinielem huge implbuf"), (10));
#endif
                        ;
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:183:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L17*/ meltfnum[16] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:183:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L17*/ meltfnum[16]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L18*/ meltfnum[17] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:183:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L18*/ meltfnum[17];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 183;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "outpucod_objinielem huge implbuf=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                        /*_.MELT_DEBUG_FUN__V32*/ meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[6])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V31*/ meltfptr[22] = /*_.MELT_DEBUG_FUN__V32*/ meltfptr[26];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:183:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L18*/ meltfnum[17] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V32*/ meltfptr[26] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V31*/ meltfptr[22] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:183:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L17*/ meltfnum[16] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V31*/ meltfptr[22] = 0 ;
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
                    MELT_LOCATION("warmelt-outobj.melt:184:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L19*/ meltfnum[17] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                        MELT_LOCATION("warmelt-outobj.melt:185:/ cond");
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
                                    /*_.REFERENCED_VALUE__V34*/ meltfptr[22] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V34*/ meltfptr[22] =  /*reallynil*/ NULL ;;
                            }
                        ;
                        /*^compute*/
                        /*_#GET_INT__L20*/ meltfnum[16] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V34*/ meltfptr[22])));;
                        /*^compute*/
                        /*_#ltI__L21*/ meltfnum[20] =
                            ((/*_#STRBUF_USEDLENGTH__L19*/ meltfnum[17]) < (/*_#GET_INT__L20*/ meltfnum[16]));;
                        MELT_LOCATION("warmelt-outobj.melt:184:/ cond");
                        /*cond*/ if (/*_#ltI__L21*/ meltfnum[20]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V35*/ meltfptr[34] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:184:/ cond.else");

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
                                        argtab[2].meltbp_long = 184;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V36*/ meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V35*/ meltfptr[34] = /*_.MELT_ASSERT_FAILURE_FUN__V36*/ meltfptr[35];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:184:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V36*/ meltfptr[35] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V33*/ meltfptr[26] = /*_._IFELSE___V35*/ meltfptr[34];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:184:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L19*/ meltfnum[17] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V34*/ meltfptr[22] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L20*/ meltfnum[16] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L21*/ meltfnum[20] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V35*/ meltfptr[34] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V33*/ meltfptr[26] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:181:/ quasiblock");


                    /*_.PROGN___V37*/ meltfptr[35] = /*_.IFCPP___V33*/ meltfptr[26];;
                    /*^compute*/
                    /*_._IFELSE___V30*/ meltfptr[25] = /*_.PROGN___V37*/ meltfptr[35];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:181:/ clear");
                    /*clear*/ /*_.IFCPP___V33*/ meltfptr[26] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V37*/ meltfptr[35] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IFELSE___V30*/ meltfptr[25] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_._IFELSE___V30*/ meltfptr[25];;

        MELT_LOCATION("warmelt-outobj.melt:168:/ clear");
        /*clear*/ /*_.OLOCVAR__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CNAM__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V14*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V17*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V19*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_#STRBUF_USEDLENGTH__L4*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REFERENCED_VALUE__V20*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L5*/ meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#diIRAW__L6*/ meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L7*/ meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V21*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_#STRBUF_USEDLENGTH__L13*/ meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REFERENCED_VALUE__V29*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L14*/ meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_#diIRAW__L15*/ meltfnum[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L16*/ meltfnum[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V30*/ meltfptr[25] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:166:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:166:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJINIELEM", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_10_WARMELTmiOUTOBJ_OUTPUCOD_OBJINIELEM*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT fromline 1752 */

    /** start of frame for meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT// fromline 1568
        : public Melt_CallFrameWithValues<9>
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
        MeltFrame_meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<9> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT), clos) {};
        MeltFrame_meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT() //the constructor fromline 1642
            : Melt_CallFrameWithValues<9> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<9> (fil,lin, sizeof(MeltFrame_meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT)) {};
        MeltFrame_meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<9> (fil,lin, sizeof(MeltFrame_meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT


    /** end of frame for meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT fromline 1697**/

    /* end of frame for routine meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT fromline 1756 */

    /* classy proc frame meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT */ MeltFrame_meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTCINITFILL_ROOT", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:190:/ getarg");
    /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.PTRSTR__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])) != NULL);


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
        MELT_LOCATION("warmelt-outobj.melt:191:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:191:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:191:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 191;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outcinitfill_root recv=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:191:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:191:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:192:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*^cond*/
            /*cond*/ if ((/*nil*/NULL)) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:192:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "outcinitfill_root unimplemented catchall";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 192;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:192:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:192:/ clear");
            /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:190:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.IFCPP___V7*/ meltfptr[5];;

        {
            MELT_LOCATION("warmelt-outobj.melt:190:/ locexp");
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
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTCINITFILL_ROOT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_11_WARMELTmiOUTOBJ_OUTCINITFILL_ROOT*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT fromline 1752 */

    /** start of frame for meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT// fromline 1568
        : public Melt_CallFrameWithValues<7>
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
        MeltFrame_meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<7> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT), clos) {};
        MeltFrame_meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT() //the constructor fromline 1642
            : Melt_CallFrameWithValues<7> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<7> (fil,lin, sizeof(MeltFrame_meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT)) {};
        MeltFrame_meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<7> (fil,lin, sizeof(MeltFrame_meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT


    /** end of frame for meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT fromline 1697**/

    /* end of frame for routine meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT fromline 1756 */

    /* classy proc frame meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT */ MeltFrame_meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTCINITPREDEF_ROOT", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:198:/ getarg");
    /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.SBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.PTRSTR__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])) != NULL);


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
        MELT_LOCATION("warmelt-outobj.melt:199:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:199:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:199:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 199;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outcinitfill_root recv=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:199:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:199:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:200:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

        {
            MELT_LOCATION("warmelt-outobj.melt:200:/ locexp");
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

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:198:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.RETURN___V7*/ meltfptr[5];;

        {
            MELT_LOCATION("warmelt-outobj.melt:198:/ locexp");
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
        /*clear*/ /*_.RETURN___V7*/ meltfptr[5] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTCINITPREDEF_ROOT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_12_WARMELTmiOUTOBJ_OUTCINITPREDEF_ROOT*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF fromline 1752 */

    /** start of frame for meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF// fromline 1568
        : public Melt_CallFrameWithValues<21>
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
        MeltFrame_meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<21> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF), clos) {};
        MeltFrame_meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF() //the constructor fromline 1642
            : Melt_CallFrameWithValues<21> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<21> (fil,lin, sizeof(MeltFrame_meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF)) {};
        MeltFrame_meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<21> (fil,lin, sizeof(MeltFrame_meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF


    /** end of frame for meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF fromline 1697**/

    /* end of frame for routine meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF fromline 1756 */

    /* classy proc frame meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF */ MeltFrame_meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUT_PREDEF", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:205:/ getarg");
    /*_.OBPR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.IMPLBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.IMPLBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#DEPTH__L1*/ meltfnum[0] = meltxargtab_[1].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_INTEGERBOX__L2*/ meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.OBPR__V2*/ meltfptr[1])) == MELTOBMAG_INT);;
        MELT_LOCATION("warmelt-outobj.melt:207:/ cond");
        /*cond*/ if (/*_#IS_INTEGERBOX__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:208:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V3*/ meltfptr[2]), ( "melt_fetch_predefined("));
                    }
                    ;
                    /*_#GET_INT__L3*/ meltfnum[2] =
                        (melt_get_int((melt_ptr_t)(/*_.OBPR__V2*/ meltfptr[1])));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:209:/ locexp");
                        meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V3*/ meltfptr[2]), (/*_#GET_INT__L3*/ meltfnum[2]));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:210:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V3*/ meltfptr[2]), ( ")"));
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:207:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_#GET_INT__L3*/ meltfnum[2] = 0 ;
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
                    /*_#IS_A__L4*/ meltfnum[2] =
                        melt_is_instance_of((melt_ptr_t)(/*_.OBPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[0])));;
                    MELT_LOCATION("warmelt-outobj.melt:212:/ cond");
                    /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-outobj.melt:213:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V3*/ meltfptr[2]), ( "((melt_ptr_t)(MELT_PREDEF("));
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:214:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.OBPR__V2*/ meltfptr[1]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                    /*_.NAMED_NAME__V6*/ meltfptr[5] = slot;
                                };
                                ;

                                {
                                    /*^locexp*/
                                    /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V3*/ meltfptr[2]),
                                                                          melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V6*/ meltfptr[5])));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:215:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V3*/ meltfptr[2]), ( ")))"));
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:212:/ quasiblock");


                                /*epilog*/

                                /*^clear*/
                                /*clear*/ /*_.NAMED_NAME__V6*/ meltfptr[5] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {


#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:218:/ cppif.then");
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
                                    /*_#MELT_NEED_DBG__L5*/ meltfnum[4] =
                                        /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                        (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                        0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                        ;;
                                    MELT_LOCATION("warmelt-outobj.melt:218:/ cond");
                                    /*cond*/ if (/*_#MELT_NEED_DBG__L5*/ meltfnum[4]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*^block*/
                                            /*anyblock*/
                                            {

                                                /*_#MELT_CALLCOUNT__L6*/ meltfnum[5] =
                                                    /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                                    meltcallcount  /* melt_callcount debugging */
#else
                                                    0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                                    ;;

                                                MELT_CHECK_SIGNAL();
                                                ;
                                                MELT_LOCATION("warmelt-outobj.melt:218:/ apply");
                                                /*apply*/
                                                {
                                                    union meltparam_un argtab[5];
                                                    memset(&argtab, 0, sizeof(argtab));
                                                    /*^apply.arg*/
                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L6*/ meltfnum[5];
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 218;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_cstring =  " output_predef bad obpr=";
                                                    /*^apply.arg*/
                                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OBPR__V2*/ meltfptr[1];
                                                    /*_.MELT_DEBUG_FUN__V8*/ meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IF___V7*/ meltfptr[5] = /*_.MELT_DEBUG_FUN__V8*/ meltfptr[7];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:218:/ clear");
                                                /*clear*/ /*_#MELT_CALLCOUNT__L6*/ meltfnum[5] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_.MELT_DEBUG_FUN__V8*/ meltfptr[7] = 0 ;
                                            }
                                            ;
                                        }
                                    else    /*^cond.else*/
                                        {

                                            /*_._IF___V7*/ meltfptr[5] =  /*reallynil*/ NULL ;;
                                        }
                                    ;

                                    {
                                        MELT_LOCATION("warmelt-outobj.melt:218:/ locexp");
                                        /*void*/(void)0;
                                    }
                                    ;
                                    /*^quasiblock*/


                                    /*epilog*/

                                    /*^clear*/
                                    /*clear*/ /*_#MELT_NEED_DBG__L5*/ meltfnum[4] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IF___V7*/ meltfptr[5] = 0 ;
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
                                MELT_LOCATION("warmelt-outobj.melt:219:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*^cond*/
                                    /*cond*/ if ((/*nil*/NULL)) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*_._IFELSE___V10*/ meltfptr[5] = (/*nil*/NULL);;
                                        }
                                    else
                                        {
                                            MELT_LOCATION("warmelt-outobj.melt:219:/ cond.else");

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
                                                    argtab[0].meltbp_cstring =  "invalid obpr";
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 219;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBPR__V2*/ meltfptr[1];
                                                    /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IFELSE___V10*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:219:/ clear");
                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10] = 0 ;
                                            }
                                            ;
                                        }
                                    ;
                                    /*_.IFCPP___V9*/ meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[5];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:219:/ clear");
                                    /*clear*/ /*_._IFELSE___V10*/ meltfptr[5] = 0 ;
                                }

#else /*MELT_HAVE_DEBUG*/
                                /*^cppif.else*/
                                /*_.IFCPP___V9*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:217:/ quasiblock");


                                /*_.PROGN___V12*/ meltfptr[10] = /*_.IFCPP___V9*/ meltfptr[7];;
                                /*^compute*/
                                /*_._IFELSE___V5*/ meltfptr[4] = /*_.PROGN___V12*/ meltfptr[10];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:212:/ clear");
                                /*clear*/ /*_.IFCPP___V9*/ meltfptr[7] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V12*/ meltfptr[10] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V4*/ meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:207:/ clear");
                    /*clear*/ /*_#IS_A__L4*/ meltfnum[2] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V5*/ meltfptr[4] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[5] =
            melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V3*/ meltfptr[2]));;
        MELT_LOCATION("warmelt-outobj.melt:223:/ cond");
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
                    /*_.REFERENCED_VALUE__V13*/ meltfptr[5] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.REFERENCED_VALUE__V13*/ meltfptr[5] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#GET_INT__L8*/ meltfnum[4] =
            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V13*/ meltfptr[5])));;
        /*^compute*/
        /*_#diIRAW__L9*/ meltfnum[2] =
            ((/*_#GET_INT__L8*/ meltfnum[4]) / (2));;
        /*^compute*/
        /*_#gtI__L10*/ meltfnum[9] =
            ((/*_#STRBUF_USEDLENGTH__L7*/ meltfnum[5]) > (/*_#diIRAW__L9*/ meltfnum[2]));;
        MELT_LOCATION("warmelt-outobj.melt:223:/ cond");
        /*cond*/ if (/*_#gtI__L10*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:224:/ locexp");

#if MELT_HAVE_DEBUG
                        if (melt_need_debug (0))
                            melt_dbgshortbacktrace(( "output_predef huge implbuf"), (10));
#endif
                        ;
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:225:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L11*/ meltfnum[10] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:225:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L11*/ meltfnum[10]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L12*/ meltfnum[11] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:225:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 225;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "output_predef huge implbuf=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V3*/ meltfptr[2];
                                        /*_.MELT_DEBUG_FUN__V16*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V15*/ meltfptr[10] = /*_.MELT_DEBUG_FUN__V16*/ meltfptr[4];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:225:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L12*/ meltfnum[11] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V16*/ meltfptr[4] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V15*/ meltfptr[10] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:225:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L11*/ meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V15*/ meltfptr[10] = 0 ;
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
                    MELT_LOCATION("warmelt-outobj.melt:226:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L13*/ meltfnum[11] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V3*/ meltfptr[2]));;
                        MELT_LOCATION("warmelt-outobj.melt:227:/ cond");
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
                                    /*_.REFERENCED_VALUE__V18*/ meltfptr[10] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V18*/ meltfptr[10] =  /*reallynil*/ NULL ;;
                            }
                        ;
                        /*^compute*/
                        /*_#GET_INT__L14*/ meltfnum[10] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V18*/ meltfptr[10])));;
                        /*^compute*/
                        /*_#ltI__L15*/ meltfnum[14] =
                            ((/*_#STRBUF_USEDLENGTH__L13*/ meltfnum[11]) < (/*_#GET_INT__L14*/ meltfnum[10]));;
                        MELT_LOCATION("warmelt-outobj.melt:226:/ cond");
                        /*cond*/ if (/*_#ltI__L15*/ meltfnum[14]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V19*/ meltfptr[18] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:226:/ cond.else");

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
                                        argtab[2].meltbp_long = 226;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V3*/ meltfptr[2];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V19*/ meltfptr[18] = /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:226:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V17*/ meltfptr[4] = /*_._IFELSE___V19*/ meltfptr[18];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:226:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L13*/ meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V18*/ meltfptr[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L14*/ meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L15*/ meltfnum[14] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V19*/ meltfptr[18] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V17*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:223:/ quasiblock");


                    /*_.PROGN___V21*/ meltfptr[19] = /*_.IFCPP___V17*/ meltfptr[4];;
                    /*^compute*/
                    /*_._IFELSE___V14*/ meltfptr[7] = /*_.PROGN___V21*/ meltfptr[19];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:223:/ clear");
                    /*clear*/ /*_.IFCPP___V17*/ meltfptr[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V21*/ meltfptr[19] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IFELSE___V14*/ meltfptr[7] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:205:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_._IFELSE___V14*/ meltfptr[7];;

        {
            MELT_LOCATION("warmelt-outobj.melt:205:/ locexp");
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
        /*clear*/ /*_#IS_INTEGERBOX__L2*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V4*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REFERENCED_VALUE__V13*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L8*/ meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#diIRAW__L9*/ meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L10*/ meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V14*/ meltfptr[7] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUT_PREDEF", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_13_WARMELTmiOUTOBJ_OUTPUT_PREDEF*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF fromline 1752 */

    /** start of frame for meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF// fromline 1568
        : public Melt_CallFrameWithValues<13>
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
        MeltFrame_meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<13> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF), clos) {};
        MeltFrame_meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF() //the constructor fromline 1642
            : Melt_CallFrameWithValues<13> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<13> (fil,lin, sizeof(MeltFrame_meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF)) {};
        MeltFrame_meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<13> (fil,lin, sizeof(MeltFrame_meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF


    /** end of frame for meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF fromline 1697**/

    /* end of frame for routine meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF fromline 1756 */

    /* classy proc frame meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF */ MeltFrame_meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_PREDEF", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:231:/ getarg");
    /*_.OBPRED__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:232:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBPRED__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJPREDEF*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:232:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:232:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check obpredef";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 232;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBPRED__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:232:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:232:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:233:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBPRED__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBPREDEF");
            /*_.OBPR__V9*/ meltfptr[5] = slot;
        };
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:234:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*^cond*/
            /*cond*/ if (/*_.OBPR__V9*/ meltfptr[5]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[10] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:234:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check obpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 234;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBPRED__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V11*/ meltfptr[10] = /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:234:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V10*/ meltfptr[9] = /*_._IFELSE___V11*/ meltfptr[10];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:234:/ clear");
            /*clear*/ /*_._IFELSE___V11*/ meltfptr[10] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V10*/ meltfptr[9] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:235:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_PREDEF__V13*/ meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_PREDEF*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.OBPR__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.LET___V8*/ meltfptr[6] = /*_.OUTPUT_PREDEF__V13*/ meltfptr[11];;

        MELT_LOCATION("warmelt-outobj.melt:233:/ clear");
        /*clear*/ /*_.OBPR__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_PREDEF__V13*/ meltfptr[11] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:231:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:231:/ locexp");
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
    melt_trace_end("OUTPUCOD_PREDEF", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_14_WARMELTmiOUTOBJ_OUTPUCOD_PREDEF*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL fromline 1752 */

    /** start of frame for meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL// fromline 1568
        : public Melt_CallFrameWithValues<7>
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
        MeltFrame_meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<7> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL), clos) {};
        MeltFrame_meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL() //the constructor fromline 1642
            : Melt_CallFrameWithValues<7> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<7> (fil,lin, sizeof(MeltFrame_meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL)) {};
        MeltFrame_meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<7> (fil,lin, sizeof(MeltFrame_meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL


    /** end of frame for meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL fromline 1697**/

    /* end of frame for routine meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL fromline 1756 */

    /* classy proc frame meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL */ MeltFrame_meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_NIL", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:239:/ getarg");
    /*_.OBNIL__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:240:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBNIL__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJNIL*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:240:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:240:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check obnil";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 240;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBNIL__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:240:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:240:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:241:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "(/*nil*/NULL)"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:239:/ clear");
        /*clear*/ /*_.IFCPP___V5*/ meltfptr[4] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_NIL", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_15_WARMELTmiOUTOBJ_OUTPUCOD_NIL*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL fromline 1752 */

    /** start of frame for meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL// fromline 1568
        : public Melt_CallFrameWithValues<42>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[15];
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
        MeltFrame_meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<42> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL), clos) {};
        MeltFrame_meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL() //the constructor fromline 1642
            : Melt_CallFrameWithValues<42> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<42> (fil,lin, sizeof(MeltFrame_meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL)) {};
        MeltFrame_meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<42> (fil,lin, sizeof(MeltFrame_meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL


    /** end of frame for meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL fromline 1697**/

    /* end of frame for routine meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL fromline 1756 */

    /* classy proc frame meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL */ MeltFrame_meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJEXPANDPUREVAL", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:245:/ getarg");
    /*_.OBX__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:246:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:246:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:246:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 246;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objexpandpureval obx=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OBX__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:246:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:246:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:247:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBX__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJEXPANDPUREVAL*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:247:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:247:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check obx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 247;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBX__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:247:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:247:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:248:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OBX__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJEXPANDPUREVAL*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OBX__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "OXPURVAL_LOC");
                    /*_.OLOC__V11*/ meltfptr[4] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OLOC__V11*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:249:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OBX__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJEXPANDPUREVAL*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OBX__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "OXPURVAL_COMM");
                    /*_.OCOMM__V12*/ meltfptr[11] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OCOMM__V12*/ meltfptr[11] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:250:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OBX__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJEXPANDPUREVAL*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OBX__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 3, "OXPURVAL_CONT");
                    /*_.OCONT__V13*/ meltfptr[12] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OCONT__V13*/ meltfptr[12] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:251:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OBX__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJVALUE*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OBX__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 0, "OBV_TYPE");
                    /*_.OTYP__V14*/ meltfptr[13] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OTYP__V14*/ meltfptr[13] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:253:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_MULTIPLE_OR_NULL__L5*/ meltfnum[1] =
                ((/*_.OCONT__V13*/ meltfptr[12]) == NULL || (melt_unsafe_magic_discr((melt_ptr_t)(/*_.OCONT__V13*/ meltfptr[12])) == MELTOBMAG_MULTIPLE));;
            MELT_LOCATION("warmelt-outobj.melt:253:/ cond");
            /*cond*/ if (/*_#IS_MULTIPLE_OR_NULL__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V16*/ meltfptr[15] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:253:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check ocont";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 253;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OCONT__V13*/ meltfptr[12];
                            /*_.MELT_ASSERT_FAILURE_FUN__V17*/ meltfptr[16] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V16*/ meltfptr[15] = /*_.MELT_ASSERT_FAILURE_FUN__V17*/ meltfptr[16];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:253:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V17*/ meltfptr[16] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V15*/ meltfptr[14] = /*_._IFELSE___V16*/ meltfptr[15];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:253:/ clear");
            /*clear*/ /*_#IS_MULTIPLE_OR_NULL__L5*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V16*/ meltfptr[15] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V15*/ meltfptr[14] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:254:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L6*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.OTYP__V14*/ meltfptr[13]), (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[4])));;
            MELT_LOCATION("warmelt-outobj.melt:254:/ cond");
            /*cond*/ if (/*_#IS_A__L6*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V19*/ meltfptr[15] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:254:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check otyp";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 254;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OTYP__V14*/ meltfptr[13];
                            /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V19*/ meltfptr[15] = /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:254:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V18*/ meltfptr[16] = /*_._IFELSE___V19*/ meltfptr[15];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:254:/ clear");
            /*clear*/ /*_#IS_A__L6*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V19*/ meltfptr[15] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V18*/ meltfptr[16] = (/*nil*/NULL);
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OBX__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V22*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V21*/ meltfptr[19] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V22*/ meltfptr[15];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V22*/ meltfptr[15] = 0 ;
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
        /*^compute*/
        /*_#eqeq__L7*/ meltfnum[1] =
            ((/*_.OTYP__V14*/ meltfptr[13]) == ((/*!CTYPE_VOID*/ meltfrout->tabval[6])));;
        MELT_LOCATION("warmelt-outobj.melt:257:/ cond");
        /*cond*/ if (/*_#eqeq__L7*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:258:/ locexp");

#if MELT_HAVE_DEBUG
                        if (melt_need_debug (0))
                            melt_dbgshortbacktrace(( "outpucod_objexpandpureval pure void"), (15));
#endif
                        ;
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:259:/ cond");
                    /*cond*/ if (/*_.OLOC__V11*/ meltfptr[4]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-outobj.melt:260:/ quasiblock");


                                /*_?*/ meltfram__.loc_CSTRING__o0 =
                                    (char*)0;;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:262:/ locexp");
                                    melt_warning_str(0, (melt_ptr_t)(/*_.OLOC__V11*/ meltfptr[4]), ( "strange pure value with :void ctype"), (melt_ptr_t)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:263:/ locexp");
                                    /* outpucod_objexpandpureval void FILLCOMSTR_CHK__1 */
                                    static char buf_FILLCOMSTR_CHK__1 [80] ;
                                    const char* comstr_FILLCOMSTR_CHK__1 =
                                        melt_string_str ((melt_ptr_t)/*_.OCOMM__V12*/ meltfptr[11]) ;
                                    if (comstr_FILLCOMSTR_CHK__1)
                                        snprintf(buf_FILLCOMSTR_CHK__1,
                                                 sizeof(buf_FILLCOMSTR_CHK__1),
                                                 "xpurvalvoid: %s", comstr_FILLCOMSTR_CHK__1) ;
                                    else
                                        snprintf(buf_FILLCOMSTR_CHK__1,
                                                 sizeof(buf_FILLCOMSTR_CHK__1),
                                                 "[xpurvalvoid/%lx]", melt_obj_hash ((melt_ptr_t) /*_.OBX__V2*/ meltfptr[1])) ;
                                    /*_?*/ meltfram__.loc_CSTRING__o0 = buf_FILLCOMSTR_CHK__1 ;
                                    /* outpucod_objexpandpureval void end FILLCOMSTR_CHK__1 */
                                    ;
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:280:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[3];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                    /*^apply.arg*/
                                    argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                                    /*^apply.arg*/
                                    argtab[2].meltbp_cstring = /*_?*/ meltfram__.loc_CSTRING__o0;
                                    /*_.OUTPUT_LOCATION__V26*/ meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[7])), (melt_ptr_t)(/*_.OLOC__V11*/ meltfptr[4]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                /*_.LET___V25*/ meltfptr[24] = /*_.OUTPUT_LOCATION__V26*/ meltfptr[25];;

                                MELT_LOCATION("warmelt-outobj.melt:260:/ clear");
                                /*clear*/ /*_?*/ meltfram__.loc_CSTRING__o0 = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.OUTPUT_LOCATION__V26*/ meltfptr[25] = 0 ;
                                /*_._IFELSE___V24*/ meltfptr[23] = /*_.LET___V25*/ meltfptr[24];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:259:/ clear");
                                /*clear*/ /*_.LET___V25*/ meltfptr[24] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {




                                {
                                    MELT_LOCATION("warmelt-outobj.melt:281:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                                }
                                ;
                                /*clear*/ /*_._IFELSE___V24*/ meltfptr[23] = 0 ;
                                /*epilog*/
                            }
                            ;
                        }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*^compute*/
                    /*_#NULL__L8*/ meltfnum[2] =
                        (/*null*/(/*_.OLOC__V11*/ meltfptr[4]) == NULL);;
                    MELT_LOCATION("warmelt-outobj.melt:283:/ cond");
                    /*cond*/ if (/*_#NULL__L8*/ meltfnum[2]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*_#IS_STRING__L10*/ meltfnum[9] =
                                    (melt_magic_discr((melt_ptr_t)(/*_.OCOMM__V12*/ meltfptr[11])) == MELTOBMAG_STRING);;
                                /*^compute*/
                                /*_#_IF___L9*/ meltfnum[8] = /*_#IS_STRING__L10*/ meltfnum[9];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:283:/ clear");
                                /*clear*/ /*_#IS_STRING__L10*/ meltfnum[9] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_#_IF___L9*/ meltfnum[8] = 0;;
                        }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:283:/ cond");
                    /*cond*/ if (/*_#_IF___L9*/ meltfnum[8]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:284:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[1];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_cstring =  "/*xpurval!";
                                    /*_.ADD2OUT__V28*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:285:/ locexp");
                                    meltgc_add_strbuf_ccomment((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                               melt_string_str((melt_ptr_t)(/*_.OCOMM__V12*/ meltfptr[11])));
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:286:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[1];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_cstring =  "*/ ";
                                    /*_.ADD2OUT__V29*/ meltfptr[28] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:283:/ quasiblock");


                                /*_.PROGN___V30*/ meltfptr[29] = /*_.ADD2OUT__V29*/ meltfptr[28];;
                                /*^compute*/
                                /*_._IF___V27*/ meltfptr[25] = /*_.PROGN___V30*/ meltfptr[29];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:283:/ clear");
                                /*clear*/ /*_.ADD2OUT__V28*/ meltfptr[24] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.ADD2OUT__V29*/ meltfptr[28] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V30*/ meltfptr[29] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IF___V27*/ meltfptr[25] =  /*reallynil*/ NULL ;;
                        }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:288:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "{";
                        /*_.ADD2OUT__V31*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*citerblock FOREACH_IN_MULTIPLE*/
                    {
                        /* start foreach_in_multiple meltcit1__EACHTUP */
                        long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.OCONT__V13*/ meltfptr[12]);
                        for (/*_#IX__L11*/ meltfnum[9] = 0;
                                           (/*_#IX__L11*/ meltfnum[9] >= 0) && (/*_#IX__L11*/ meltfnum[9] <  meltcit1__EACHTUP_ln);
                                           /*_#IX__L11*/ meltfnum[9]++)
                            {
                                /*_.COMP__V32*/ meltfptr[28] = melt_multiple_nth((melt_ptr_t)(/*_.OCONT__V13*/ meltfptr[12]),  /*_#IX__L11*/ meltfnum[9]);



                                /*_#plI__L12*/ meltfnum[11] =
                                    ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:292:/ msend");
                                /*msend*/
                                {
                                    union meltparam_un argtab[3];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^ojbmsend.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                    /*^ojbmsend.arg*/
                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                    /*^ojbmsend.arg*/
                                    argtab[2].meltbp_long = /*_#plI__L12*/ meltfnum[11];
                                    /*_.OUTPUT_C_CODE__V33*/ meltfptr[29] = meltgc_send((melt_ptr_t)(/*_.COMP__V32*/ meltfptr[28]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                if (/*_#IX__L11*/ meltfnum[9]<0) break;
                            } /* end  foreach_in_multiple meltcit1__EACHTUP */

                        /*citerepilog*/

                        MELT_LOCATION("warmelt-outobj.melt:289:/ clear");
                        /*clear*/ /*_.COMP__V32*/ meltfptr[28] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#IX__L11*/ meltfnum[9] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#plI__L12*/ meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.OUTPUT_C_CODE__V33*/ meltfptr[29] = 0 ;
                    } /*endciterblock FOREACH_IN_MULTIPLE*/
                    ;
                    /*_#plI__L13*/ meltfnum[12] =
                        ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:293:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L13*/ meltfnum[12]), 0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:294:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "}";
                        /*_.ADD2OUT__V34*/ meltfptr[33] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:295:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:257:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V24*/ meltfptr[23] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#NULL__L8*/ meltfnum[2] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#_IF___L9*/ meltfnum[8] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IF___V27*/ meltfptr[25] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ADD2OUT__V31*/ meltfptr[24] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#plI__L13*/ meltfnum[12] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ADD2OUT__V34*/ meltfptr[33] = 0 ;
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
                    MELT_LOCATION("warmelt-outobj.melt:299:/ cond");
                    /*cond*/ if (/*_.OLOC__V11*/ meltfptr[4]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-outobj.melt:300:/ quasiblock");


                                /*_?*/ meltfram__.loc_CSTRING__o1 =
                                    (char*)0;;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:302:/ locexp");
                                    /* outpucod_objexpandpureval nonvoid FILLCOMSTR_CHK__2 */
                                    static char buf_FILLCOMSTR_CHK__2 [80] ;
                                    const char* comstr_FILLCOMSTR_CHK__2
                                        = melt_string_str ((melt_ptr_t)/*_.OCOMM__V12*/ meltfptr[11]) ;
                                    if (comstr_FILLCOMSTR_CHK__2)
                                        snprintf(buf_FILLCOMSTR_CHK__2,
                                                 sizeof(buf_FILLCOMSTR_CHK__2),
                                                 "xpurval: %s", comstr_FILLCOMSTR_CHK__2) ;
                                    else
                                        snprintf(buf_FILLCOMSTR_CHK__2,
                                                 sizeof(buf_FILLCOMSTR_CHK__2),
                                                 "[xpurval/%lx]", melt_obj_hash ((melt_ptr_t) /*_.OBX__V2*/ meltfptr[1])) ;
                                    /*_?*/ meltfram__.loc_CSTRING__o1 = buf_FILLCOMSTR_CHK__2 ;
                                    /* outpucod_objexpandpureval nonvoid end FILLCOMSTR_CHK__2 */
                                    ;
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:319:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[3];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                    /*^apply.arg*/
                                    argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                                    /*^apply.arg*/
                                    argtab[2].meltbp_cstring = /*_?*/ meltfram__.loc_CSTRING__o1;
                                    /*_.OUTPUT_RAW_LOCATION__V37*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_RAW_LOCATION*/ meltfrout->tabval[10])), (melt_ptr_t)(/*_.OLOC__V11*/ meltfptr[4]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                /*_.LET___V36*/ meltfptr[25] = /*_.OUTPUT_RAW_LOCATION__V37*/ meltfptr[24];;

                                MELT_LOCATION("warmelt-outobj.melt:300:/ clear");
                                /*clear*/ /*_?*/ meltfram__.loc_CSTRING__o1 = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.OUTPUT_RAW_LOCATION__V37*/ meltfptr[24] = 0 ;
                                /*_._IF___V35*/ meltfptr[23] = /*_.LET___V36*/ meltfptr[25];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:299:/ clear");
                                /*clear*/ /*_.LET___V36*/ meltfptr[25] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IF___V35*/ meltfptr[23] =  /*reallynil*/ NULL ;;
                        }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:321:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "(";
                        /*_.ADD2OUT__V38*/ meltfptr[33] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*citerblock FOREACH_IN_MULTIPLE*/
                    {
                        /* start foreach_in_multiple meltcit2__EACHTUP */
                        long  meltcit2__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.OCONT__V13*/ meltfptr[12]);
                        for (/*_#IX__L14*/ meltfnum[2] = 0;
                                           (/*_#IX__L14*/ meltfnum[2] >= 0) && (/*_#IX__L14*/ meltfnum[2] <  meltcit2__EACHTUP_ln);
                                           /*_#IX__L14*/ meltfnum[2]++)
                            {
                                /*_.COMP__V39*/ meltfptr[24] = melt_multiple_nth((melt_ptr_t)(/*_.OCONT__V13*/ meltfptr[12]),  /*_#IX__L14*/ meltfnum[2]);



                                /*_#plI__L15*/ meltfnum[8] =
                                    ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:325:/ msend");
                                /*msend*/
                                {
                                    union meltparam_un argtab[3];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^ojbmsend.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                    /*^ojbmsend.arg*/
                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                    /*^ojbmsend.arg*/
                                    argtab[2].meltbp_long = /*_#plI__L15*/ meltfnum[8];
                                    /*_.OUTPUT_C_CODE__V40*/ meltfptr[25] = meltgc_send((melt_ptr_t)(/*_.COMP__V39*/ meltfptr[24]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                if (/*_#IX__L14*/ meltfnum[2]<0) break;
                            } /* end  foreach_in_multiple meltcit2__EACHTUP */

                        /*citerepilog*/

                        MELT_LOCATION("warmelt-outobj.melt:322:/ clear");
                        /*clear*/ /*_.COMP__V39*/ meltfptr[24] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#IX__L14*/ meltfnum[2] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#plI__L15*/ meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.OUTPUT_C_CODE__V40*/ meltfptr[25] = 0 ;
                    } /*endciterblock FOREACH_IN_MULTIPLE*/
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:326:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  ")";
                        /*_.ADD2OUT__V41*/ meltfptr[40] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:297:/ quasiblock");


                    /*_.PROGN___V42*/ meltfptr[41] = /*_.ADD2OUT__V41*/ meltfptr[40];;
                    /*^compute*/
                    /*_._IFELSE___V23*/ meltfptr[15] = /*_.PROGN___V42*/ meltfptr[41];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:257:/ clear");
                    /*clear*/ /*_._IF___V35*/ meltfptr[23] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ADD2OUT__V38*/ meltfptr[33] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ADD2OUT__V41*/ meltfptr[40] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V42*/ meltfptr[41] = 0 ;
                }
                ;
            }
        ;
        /*_.LET___V10*/ meltfptr[8] = /*_._IFELSE___V23*/ meltfptr[15];;

        MELT_LOCATION("warmelt-outobj.melt:248:/ clear");
        /*clear*/ /*_.OLOC__V11*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OCOMM__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OCONT__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OTYP__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V18*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V21*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_#eqeq__L7*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V23*/ meltfptr[15] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:245:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V10*/ meltfptr[8];;

        {
            MELT_LOCATION("warmelt-outobj.melt:245:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJEXPANDPUREVAL", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_16_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXPANDPUREVAL*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH fromline 1752 */

    /** start of frame for meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH// fromline 1568
        : public Melt_CallFrameWithValues<37>
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
        MeltFrame_meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<37> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH), clos) {};
        MeltFrame_meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH() //the constructor fromline 1642
            : Melt_CallFrameWithValues<37> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<37> (fil,lin, sizeof(MeltFrame_meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH)) {};
        MeltFrame_meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<37> (fil,lin, sizeof(MeltFrame_meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH


    /** end of frame for meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH fromline 1697**/

    /* end of frame for routine meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH fromline 1756 */

    /* classy proc frame meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH */ MeltFrame_meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJHOOKNTH", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:334:/ getarg");
    /*_.OBH__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:335:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:335:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:335:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 335;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objhooknth obh=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OBH__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:335:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:335:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:336:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBH__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJHOOKNTH*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:336:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:336:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check obh";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 336;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBH__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:336:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:336:/ clear");
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
        MELT_LOCATION("warmelt-outobj.melt:337:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*^cond*/
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OBH__V2*/ meltfptr[1]),
                                              (melt_ptr_t)((/*!CLASS_OBJVALUE*/ meltfrout->tabval[3])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.OBH__V2*/ meltfptr[1]) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "OBV_TYPE");
                        /*_.OBV_TYPE__V11*/ meltfptr[4] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.OBV_TYPE__V11*/ meltfptr[4] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#eqeq__L5*/ meltfnum[1] =
                ((/*_.OBV_TYPE__V11*/ meltfptr[4]) == ((/*!CTYPE_VALUE*/ meltfrout->tabval[4])));;
            MELT_LOCATION("warmelt-outobj.melt:337:/ cond");
            /*cond*/ if (/*_#eqeq__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[11] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:337:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check type";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 337;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBH__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/ meltfptr[11] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:337:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V10*/ meltfptr[8] = /*_._IFELSE___V12*/ meltfptr[11];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:337:/ clear");
            /*clear*/ /*_.OBV_TYPE__V11*/ meltfptr[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_#eqeq__L5*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V12*/ meltfptr[11] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V10*/ meltfptr[8] = (/*nil*/NULL);
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OBH__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V15*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V14*/ meltfptr[12] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V15*/ meltfptr[4];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V15*/ meltfptr[4] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V14*/ meltfptr[12] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:339:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBH__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OHKNTH_HOOK");
            /*_.OHOOK__V17*/ meltfptr[4] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:340:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBH__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OHKNTH_OFF");
            /*_.OFF__V18*/ meltfptr[17] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:341:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OHOOK__V17*/ meltfptr[4]),
                                          (melt_ptr_t)((/*!CLASS_OBJLOCV*/ meltfrout->tabval[7])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.OHOOK__V17*/ meltfptr[4]) /*=obj*/;
                    melt_object_get_field(slot,obj, 3, "OBL_CNAME");
                    /*_.OBL_CNAME__V19*/ meltfptr[18] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OBL_CNAME__V19*/ meltfptr[18] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:341:/ cond");
        /*cond*/ if (/*_.OBL_CNAME__V19*/ meltfptr[18]) /*then*/
            {
                /*^cond.then*/
                /*_.HKNAM__V20*/ meltfptr[19] = /*_.OBL_CNAME__V19*/ meltfptr[18];;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:341:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    /*^cond*/
                    /*cond*/ if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OHOOK__V17*/ meltfptr[4]),
                                                      (melt_ptr_t)((/*!CLASS_OBJINITELEM*/ meltfrout->tabval[6])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.OHOOK__V17*/ meltfptr[4]) /*=obj*/;
                                melt_object_get_field(slot,obj, 1, "OIE_CNAME");
                                /*_.OIE_CNAME__V21*/ meltfptr[20] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.OIE_CNAME__V21*/ meltfptr[20] =  /*reallynil*/ NULL ;;
                        }
                    ;
                    /*^compute*/
                    /*_.HKNAM__V20*/ meltfptr[19] = /*_.OIE_CNAME__V21*/ meltfptr[20];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:341:/ clear");
                    /*clear*/ /*_.OIE_CNAME__V21*/ meltfptr[20] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_STRING__L6*/ meltfnum[2] =
            (melt_magic_discr((melt_ptr_t)(/*_.HKNAM__V20*/ meltfptr[19])) == MELTOBMAG_STRING);;
        MELT_LOCATION("warmelt-outobj.melt:343:/ cond");
        /*cond*/ if (/*_#IS_STRING__L6*/ meltfnum[2]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:344:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "/*hooknth*/MELT_HOOK_NTH_SLOT(\" fromhook ";
                        /*_.ADD2OUT__V23*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:345:/ locexp");
                        meltgc_add_strbuf_cstr ((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                melt_string_str ((melt_ptr_t)(/*_.HKNAM__V20*/ meltfptr[19])));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_INTEGERBOX__L7*/ meltfnum[1] =
                        (melt_magic_discr((melt_ptr_t)(/*_.OFF__V18*/ meltfptr[17])) == MELTOBMAG_INT);;
                    MELT_LOCATION("warmelt-outobj.melt:346:/ cond");
                    /*cond*/ if (/*_#IS_INTEGERBOX__L7*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:347:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[2];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_cstring =  " #";
                                    /*^apply.arg*/
                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.OFF__V18*/ meltfptr[17];
                                    /*_.ADD2OUT__V25*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                /*_._IF___V24*/ meltfptr[23] = /*_.ADD2OUT__V25*/ meltfptr[24];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:346:/ clear");
                                /*clear*/ /*_.ADD2OUT__V25*/ meltfptr[24] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IF___V24*/ meltfptr[23] =  /*reallynil*/ NULL ;;
                        }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:348:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "\", (";
                        /*_.ADD2OUT__V26*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:349:/ msend");
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
                        /*_.OUTPUT_C_CODE__V27*/ meltfptr[26] = meltgc_send((melt_ptr_t)(/*_.OHOOK__V17*/ meltfptr[4]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:350:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "), /*rank=*/(";
                        /*_.ADD2OUT__V28*/ meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:351:/ msend");
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
                        /*_.OUTPUT_C_CODE__V29*/ meltfptr[28] = meltgc_send((melt_ptr_t)(/*_.OFF__V18*/ meltfptr[17]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:352:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  ")) ";
                        /*_.ADD2OUT__V30*/ meltfptr[29] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:343:/ quasiblock");


                    /*_.PROGN___V31*/ meltfptr[30] = /*_.ADD2OUT__V30*/ meltfptr[29];;
                    /*^compute*/
                    /*_._IFELSE___V22*/ meltfptr[20] = /*_.PROGN___V31*/ meltfptr[30];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:343:/ clear");
                    /*clear*/ /*_.ADD2OUT__V23*/ meltfptr[22] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#IS_INTEGERBOX__L7*/ meltfnum[1] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IF___V24*/ meltfptr[23] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ADD2OUT__V26*/ meltfptr[24] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_C_CODE__V27*/ meltfptr[26] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ADD2OUT__V28*/ meltfptr[27] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_C_CODE__V29*/ meltfptr[28] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ADD2OUT__V30*/ meltfptr[29] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V31*/ meltfptr[30] = 0 ;
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
                    MELT_LOCATION("warmelt-outobj.melt:354:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "/*hooknth*/MELT_HOOK_NTH_SLOT(\"anonhook\", (";
                        /*_.ADD2OUT__V32*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:355:/ msend");
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
                        /*_.OUTPUT_C_CODE__V33*/ meltfptr[23] = meltgc_send((melt_ptr_t)(/*_.OHOOK__V17*/ meltfptr[4]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:356:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "), /*rank=*/(";
                        /*_.ADD2OUT__V34*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:357:/ msend");
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
                        /*_.OUTPUT_C_CODE__V35*/ meltfptr[26] = meltgc_send((melt_ptr_t)(/*_.OFF__V18*/ meltfptr[17]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:358:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  ")) ";
                        /*_.ADD2OUT__V36*/ meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:353:/ quasiblock");


                    /*_.PROGN___V37*/ meltfptr[28] = /*_.ADD2OUT__V36*/ meltfptr[27];;
                    /*^compute*/
                    /*_._IFELSE___V22*/ meltfptr[20] = /*_.PROGN___V37*/ meltfptr[28];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:343:/ clear");
                    /*clear*/ /*_.ADD2OUT__V32*/ meltfptr[22] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_C_CODE__V33*/ meltfptr[23] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ADD2OUT__V34*/ meltfptr[24] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_C_CODE__V35*/ meltfptr[26] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ADD2OUT__V36*/ meltfptr[27] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V37*/ meltfptr[28] = 0 ;
                }
                ;
            }
        ;
        /*_.LET___V16*/ meltfptr[11] = /*_._IFELSE___V22*/ meltfptr[20];;

        MELT_LOCATION("warmelt-outobj.melt:339:/ clear");
        /*clear*/ /*_.OHOOK__V17*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OFF__V18*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OBL_CNAME__V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HKNAM__V20*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_STRING__L6*/ meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V22*/ meltfptr[20] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:334:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[11];;

        {
            MELT_LOCATION("warmelt-outobj.melt:334:/ locexp");
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
        /*clear*/ /*_.IFCPP___V10*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V14*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V16*/ meltfptr[11] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJHOOKNTH", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_17_WARMELTmiOUTOBJ_OUTPUCOD_OBJHOOKNTH*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT fromline 1752 */

    /** start of frame for meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT// fromline 1568
        : public Melt_CallFrameWithValues<4>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[1];
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
        MeltFrame_meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<4> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT), clos) {};
        MeltFrame_meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT() //the constructor fromline 1642
            : Melt_CallFrameWithValues<4> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<4> (fil,lin, sizeof(MeltFrame_meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT)) {};
        MeltFrame_meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<4> (fil,lin, sizeof(MeltFrame_meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT


    /** end of frame for meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT fromline 1697**/

    /* end of frame for routine meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT fromline 1756 */

    /* classy proc frame meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT */ MeltFrame_meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTDECLINIT_OBJINITOBJECT", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:367:/ getarg");
    /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.SBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


        {
            MELT_LOCATION("warmelt-outobj.melt:368:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " struct MELT_OBJECT_STRUCT("));
        }
        ;
        /*_#GET_INT__L1*/ meltfnum[0] =
            (melt_get_int((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1])));;

        {
            MELT_LOCATION("warmelt-outobj.melt:369:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#GET_INT__L1*/ meltfnum[0]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:370:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ") "));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:371:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
            /*_.NAMED_NAME__V4*/ meltfptr[3] = slot;
        };
        ;

        {
            /*^locexp*/
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V4*/ meltfptr[3])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:372:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ";"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:367:/ clear");
        /*clear*/ /*_#GET_INT__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NAMED_NAME__V4*/ meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTDECLINIT_OBJINITOBJECT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_18_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITOBJECT*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT fromline 1752 */

    /** start of frame for meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT// fromline 1568
        : public Melt_CallFrameWithValues<67>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[35];
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
        MeltFrame_meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<67> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT), clos) {};
        MeltFrame_meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT() //the constructor fromline 1642
            : Melt_CallFrameWithValues<67> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<67> (fil,lin, sizeof(MeltFrame_meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT)) {};
        MeltFrame_meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<67> (fil,lin, sizeof(MeltFrame_meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT


    /** end of frame for meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT fromline 1697**/

    /* end of frame for routine meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT fromline 1756 */

    /* classy proc frame meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT */ MeltFrame_meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTCINITFILL_OBJINITOBJECT", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:379:/ getarg");
    /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.SBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.PTRSTR__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])) != NULL);


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
        MELT_LOCATION("warmelt-outobj.melt:380:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:380:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:380:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 380;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outcinitfill_objinitobject recv=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " ptrstr=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:380:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:380:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:381:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJINITOBJECT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:381:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:381:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "outcinitfill_objinitobject check recv";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 381;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:381:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:381:/ clear");
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
        MELT_LOCATION("warmelt-outobj.melt:382:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRING__L5*/ meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])) == MELTOBMAG_STRING);;
            MELT_LOCATION("warmelt-outobj.melt:382:/ cond");
            /*cond*/ if (/*_#IS_STRING__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:382:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "outcinitfill_objinitobject check ptrstr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 382;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V11*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:382:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V10*/ meltfptr[8] = /*_._IFELSE___V11*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:382:/ clear");
            /*clear*/ /*_#IS_STRING__L5*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V11*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V10*/ meltfptr[8] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:383:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OIE_DATA");
            /*_.ODATA__V14*/ meltfptr[4] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:384:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OIE_DISCR");
            /*_.ODISCR__V15*/ meltfptr[14] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:385:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OIE_CNAME");
            /*_.ONAME__V16*/ meltfptr[15] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:386:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 4, "OIE_LOCVAR");
            /*_.OLOCVAR__V17*/ meltfptr[16] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:387:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 5, "OIO_PREDEF");
            /*_.OIOPREDEF__V18*/ meltfptr[17] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:388:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJINITOBJECT*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 6, "OIO_CLASS");
                    /*_.OCLASS__V19*/ meltfptr[18] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OCLASS__V19*/ meltfptr[18] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#DEPTHP1__L6*/ meltfnum[2] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:391:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L7*/ meltfnum[1] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:391:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L7*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L8*/ meltfnum[7] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:391:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[7];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 391;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outcinitfill_objinitobject odata=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.ODATA__V14*/ meltfptr[4];
                            /*_.MELT_DEBUG_FUN__V21*/ meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V20*/ meltfptr[19] = /*_.MELT_DEBUG_FUN__V21*/ meltfptr[20];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:391:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L8*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V21*/ meltfptr[20] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V20*/ meltfptr[19] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:391:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L7*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V20*/ meltfptr[19] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:392:/ cond");
        /*cond*/ if (/*_.ODATA__V14*/ meltfptr[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    /*^cppif.then*/
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#IS_A__L9*/ meltfnum[7] =
                        melt_is_instance_of((melt_ptr_t)(/*_.ODATA__V14*/ meltfptr[4]), (melt_ptr_t)((/*!CLASS_NREP_DATAINSTANCE*/ meltfrout->tabval[3])));;
                        MELT_LOCATION("warmelt-outobj.melt:392:/ cond");
                        /*cond*/ if (/*_#IS_A__L9*/ meltfnum[7]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V24*/ meltfptr[23] = (/*nil*/NULL);;
                            }
                        else {
                                MELT_LOCATION("warmelt-outobj.melt:392:/ cond.else");

                                /*^block*/
                                /*anyblock*/{


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*^apply*/
                                    /*apply*/{
                                        union meltparam_un argtab[4];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_cstring =  "check odata";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 392;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ODATA__V14*/ meltfptr[4];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V24*/ meltfptr[23] = /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:392:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V23*/ meltfptr[19] = /*_._IFELSE___V24*/ meltfptr[23];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:392:/ clear");
                        /*clear*/ /*_#IS_A__L9*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V24*/ meltfptr[23] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V23*/ meltfptr[19] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                    /*^compute*/
                    /*_._IF___V22*/ meltfptr[20] = /*_.IFCPP___V23*/ meltfptr[19];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:392:/ clear");
                    /*clear*/ /*_.IFCPP___V23*/ meltfptr[19] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V22*/ meltfptr[20] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:393:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^cond*/
        /*cond*/ if (/*_.ODATA__V14*/ meltfptr[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.ODATA__V14*/ meltfptr[4]) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "NREP_LOC");
                        /*_.NREP_LOC__V27*/ meltfptr[23] = slot;
                    };
                    ;
                    /*_.ODLOC__V26*/ meltfptr[24] = /*_.NREP_LOC__V27*/ meltfptr[23];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:393:/ clear");
                    /*clear*/ /*_.NREP_LOC__V27*/ meltfptr[23] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ODLOC__V26*/ meltfptr[24] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:394:/ cond");
        /*cond*/ if (/*_.ODATA__V14*/ meltfptr[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.ODATA__V14*/ meltfptr[4]) /*=obj*/;
                        melt_object_get_field(slot,obj, 7, "NINST_HASH");
                        /*_.NINST_HASH__V29*/ meltfptr[23] = slot;
                    };
                    ;
                    /*_.ODHASH__V28*/ meltfptr[19] = /*_.NINST_HASH__V29*/ meltfptr[23];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:394:/ clear");
                    /*clear*/ /*_.NINST_HASH__V29*/ meltfptr[23] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ODHASH__V28*/ meltfptr[19] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:395:/ cond");
        /*cond*/ if (/*_.ODATA__V14*/ meltfptr[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.ODATA__V14*/ meltfptr[4]) /*=obj*/;
                        melt_object_get_field(slot,obj, 8, "NINST_SLOTS");
                        /*_.NINST_SLOTS__V31*/ meltfptr[30] = slot;
                    };
                    ;
                    /*_.ODSLOTS__V30*/ meltfptr[23] = /*_.NINST_SLOTS__V31*/ meltfptr[30];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:395:/ clear");
                    /*clear*/ /*_.NINST_SLOTS__V31*/ meltfptr[30] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ODSLOTS__V30*/ meltfptr[23] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:396:/ cond");
        /*cond*/ if (/*_.ODATA__V14*/ meltfptr[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.ODATA__V14*/ meltfptr[4]) /*=obj*/;
                        melt_object_get_field(slot,obj, 5, "NINST_OBJNUM");
                        /*_.NINST_OBJNUM__V33*/ meltfptr[32] = slot;
                    };
                    ;
                    /*_.ODOBNUM__V32*/ meltfptr[30] = /*_.NINST_OBJNUM__V33*/ meltfptr[32];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:396:/ clear");
                    /*clear*/ /*_.NINST_OBJNUM__V33*/ meltfptr[32] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ODOBNUM__V32*/ meltfptr[30] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_MULTIPLE__L10*/ meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.ODSLOTS__V30*/ meltfptr[23])) == MELTOBMAG_MULTIPLE);;
        MELT_LOCATION("warmelt-outobj.melt:398:/ cond");
        /*cond*/ if (/*_#IS_MULTIPLE__L10*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_#MULTIPLE_LENGTH__L12*/ meltfnum[11] =
                        (melt_multiple_length((melt_ptr_t)(/*_.ODSLOTS__V30*/ meltfptr[23])));;
                    /*^compute*/
                    /*_#NBSLOTS__L11*/ meltfnum[7] = /*_#MULTIPLE_LENGTH__L12*/ meltfnum[11];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:398:/ clear");
                    /*clear*/ /*_#MULTIPLE_LENGTH__L12*/ meltfnum[11] = 0 ;
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
                    /*_#IS_A__L13*/ meltfnum[11] =
                        melt_is_instance_of((melt_ptr_t)(/*_.OCLASS__V19*/ meltfptr[18]), (melt_ptr_t)((/*!CLASS_CLASS*/ meltfrout->tabval[4])));;
                    MELT_LOCATION("warmelt-outobj.melt:400:/ cond");
                    /*cond*/ if (/*_#IS_A__L13*/ meltfnum[11]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-outobj.melt:401:/ cond");
                                /*cond*/ if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OCLASS__V19*/ meltfptr[18]),
                                                                  (melt_ptr_t)((/*!CLASS_CLASS*/ meltfrout->tabval[4])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.OCLASS__V19*/ meltfptr[18]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 6, "CLASS_FIELDS");
                                            /*_.CLASS_FIELDS__V34*/ meltfptr[32] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.CLASS_FIELDS__V34*/ meltfptr[32] =  /*reallynil*/ NULL ;;
                                    }
                                ;
                                /*^compute*/
                                /*_#MULTIPLE_LENGTH__L15*/ meltfnum[14] =
                                    (melt_multiple_length((melt_ptr_t)(/*_.CLASS_FIELDS__V34*/ meltfptr[32])));;
                                /*^compute*/
                                /*_#_IFELSE___L14*/ meltfnum[13] = /*_#MULTIPLE_LENGTH__L15*/ meltfnum[14];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:400:/ clear");
                                /*clear*/ /*_.CLASS_FIELDS__V34*/ meltfptr[32] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#MULTIPLE_LENGTH__L15*/ meltfnum[14] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {


#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:403:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*^cond*/
                                    /*cond*/ if ((/*nil*/NULL)) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*_._IFELSE___V36*/ meltfptr[35] = (/*nil*/NULL);;
                                        }
                                    else
                                        {
                                            MELT_LOCATION("warmelt-outobj.melt:403:/ cond.else");

                                            /*^block*/
                                            /*anyblock*/
                                            {


                                                MELT_CHECK_SIGNAL();
                                                ;
                                                /*^apply*/
                                                /*apply*/
                                                {
                                                    union meltparam_un argtab[5];
                                                    memset(&argtab, 0, sizeof(argtab));
                                                    /*^apply.arg*/
                                                    argtab[0].meltbp_cstring =  "outcinitfill_objinitobject cannot compute nbslots";
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 403;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ODSLOTS__V30*/ meltfptr[23];
                                                    /*^apply.arg*/
                                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OCLASS__V19*/ meltfptr[18];
                                                    /*_.MELT_ASSERT_FAILURE_FUN__V37*/ meltfptr[36] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IFELSE___V36*/ meltfptr[35] = /*_.MELT_ASSERT_FAILURE_FUN__V37*/ meltfptr[36];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:403:/ clear");
                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V37*/ meltfptr[36] = 0 ;
                                            }
                                            ;
                                        }
                                    ;
                                    /*_.IFCPP___V35*/ meltfptr[32] = /*_._IFELSE___V36*/ meltfptr[35];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:403:/ clear");
                                    /*clear*/ /*_._IFELSE___V36*/ meltfptr[35] = 0 ;
                                }

#else /*MELT_HAVE_DEBUG*/
                                /*^cppif.else*/
                                /*_.IFCPP___V35*/ meltfptr[32] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:402:/ quasiblock");


                                /*_#PROGN___L16*/ meltfnum[14] = 0;;
                                /*^compute*/
                                /*_#_IFELSE___L14*/ meltfnum[13] = /*_#PROGN___L16*/ meltfnum[14];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:400:/ clear");
                                /*clear*/ /*_.IFCPP___V35*/ meltfptr[32] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#PROGN___L16*/ meltfnum[14] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_#NBSLOTS__L11*/ meltfnum[7] = /*_#_IFELSE___L14*/ meltfnum[13];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:398:/ clear");
                    /*clear*/ /*_#IS_A__L13*/ meltfnum[11] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#_IFELSE___L14*/ meltfnum[13] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:406:/ cond");
        /*cond*/ if (/*_.ODLOC__V26*/ meltfptr[24]) /*then*/
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                        /*^apply.arg*/
                        argtab[1].meltbp_long = /*_#DEPTHP1__L6*/ meltfnum[2];
                        /*^apply.arg*/
                        argtab[2].meltbp_cstring =  "iniobj";
                        /*_.OUTPUT_LOCATION__V39*/ meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.ODLOC__V26*/ meltfptr[24]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V38*/ meltfptr[36] = /*_.OUTPUT_LOCATION__V39*/ meltfptr[35];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:406:/ clear");
                    /*clear*/ /*_.OUTPUT_LOCATION__V39*/ meltfptr[35] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V38*/ meltfptr[36] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:407:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:408:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "/*iniobj "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:409:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.ONAME__V16*/ meltfptr[15])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:410:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:411:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:412:/ cond");
        /*cond*/ if (/*_.OIOPREDEF__V18*/ meltfptr[17]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:414:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "if ("));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:415:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[2];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                        /*^apply.arg*/
                        argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                        /*_.OUTPUT_PREDEF__V40*/ meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_PREDEF*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.OIOPREDEF__V18*/ meltfptr[17]), (MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:416:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " != (melt_ptr_t)&"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:417:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:418:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "->"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:419:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.ONAME__V16*/ meltfptr[15])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:420:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ") {"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:421:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTHP1__L6*/ meltfnum[2]), 0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:422:/ cond");
                    /*cond*/ if (/*_#NBSLOTS__L11*/ meltfnum[7]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-outobj.melt:424:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "melt_assertmsg(\"check.predef length "));
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:425:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[2];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                                    /*^apply.arg*/
                                    argtab[1].meltbp_long = /*_#DEPTHP1__L6*/ meltfnum[2];
                                    /*_.OUTPUT_PREDEF__V41*/ meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_PREDEF*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.OIOPREDEF__V18*/ meltfptr[17]), (MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:426:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "\", melt_object_length((melt_ptr_t)("));
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:427:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[2];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                                    /*^apply.arg*/
                                    argtab[1].meltbp_long = /*_#DEPTHP1__L6*/ meltfnum[2];
                                    /*_.OUTPUT_PREDEF__V42*/ meltfptr[41] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_PREDEF*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.OIOPREDEF__V18*/ meltfptr[17]), (MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:428:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ")) >= "));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:429:/ locexp");
                                    meltgc_add_strbuf_dec((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#NBSLOTS__L11*/ meltfnum[7]));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:430:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ");"));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:431:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTHP1__L6*/ meltfnum[2]), 0);
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:423:/ quasiblock");


                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:422:/ clear");
                                /*clear*/ /*_.OUTPUT_PREDEF__V41*/ meltfptr[35] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.OUTPUT_PREDEF__V42*/ meltfptr[41] = 0 ;
                            }
                            ;
                        } /*noelse*/
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:432:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "};"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:433:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:434:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                        /*_.OUTPUT_C_CODE__V43*/ meltfptr[35] = meltgc_send((melt_ptr_t)(/*_.OLOCVAR__V17*/ meltfptr[16]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[7])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:435:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " = "));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:436:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[2];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                        /*^apply.arg*/
                        argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                        /*_.OUTPUT_PREDEF__V44*/ meltfptr[41] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_PREDEF*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.OIOPREDEF__V18*/ meltfptr[17]), (MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:437:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ";"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:438:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:413:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:412:/ clear");
                    /*clear*/ /*_.OUTPUT_PREDEF__V40*/ meltfptr[32] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_C_CODE__V43*/ meltfptr[35] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_PREDEF__V44*/ meltfptr[41] = 0 ;
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
                    /*_#IS_A__L17*/ meltfnum[14] =
                        melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJINITUNIQUEOBJECT*/ meltfrout->tabval[8])));;
                    MELT_LOCATION("warmelt-outobj.melt:443:/ cond");
                    /*cond*/ if (/*_#IS_A__L17*/ meltfnum[14]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-outobj.melt:445:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "/*uniqueobj*/ if (!"));
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:446:/ msend");
                                /*msend*/
                                {
                                    union meltparam_un argtab[3];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^ojbmsend.arg*/
                                    argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
                                    /*^ojbmsend.arg*/
                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                                    /*^ojbmsend.arg*/
                                    argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                                    /*_.OUTPUT_C_CODE__V45*/ meltfptr[32] = meltgc_send((melt_ptr_t)(/*_.OLOCVAR__V17*/ meltfptr[16]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[7])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:447:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ") "));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:448:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:444:/ quasiblock");


                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:443:/ clear");
                                /*clear*/ /*_.OUTPUT_C_CODE__V45*/ meltfptr[32] = 0 ;
                            }
                            ;
                        } /*noelse*/
                    ;
                    /*_#plI__L18*/ meltfnum[11] =
                        ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:450:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#plI__L18*/ meltfnum[11];
                        /*_.OUTPUT_C_CODE__V46*/ meltfptr[35] = meltgc_send((melt_ptr_t)(/*_.OLOCVAR__V17*/ meltfptr[16]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[7])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:451:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " = (melt_ptr_t )&"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:452:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:453:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "->"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:454:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.ONAME__V16*/ meltfptr[15])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:455:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ";"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:456:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:440:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:412:/ clear");
                    /*clear*/ /*_#IS_A__L17*/ meltfnum[14] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#plI__L18*/ meltfnum[11] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_C_CODE__V46*/ meltfptr[35] = 0 ;
                }
                ;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:462:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg(\"iniobj\
 check.discr isobj "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:463:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.ONAME__V16*/ meltfptr[15])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:464:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "\", melt_magic_discr ((melt_ptr_t) ("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:465:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V47*/ meltfptr[41] = meltgc_send((melt_ptr_t)(/*_.ODISCR__V15*/ meltfptr[14]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[7])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:466:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ")) == MELTOBMAG_OBJECT);"));
        }
        ;
        /*_#plI__L19*/ meltfnum[13] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        {
            MELT_LOCATION("warmelt-outobj.melt:467:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#plI__L19*/ meltfnum[13]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:469:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " if (MELT_LIKELY(!melt_prohibit_garbcoll)) melt_assertmsg(\"iniobj\
 check.discr objmagic "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:470:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.ONAME__V16*/ meltfptr[15])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:471:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "\", ((meltobject_ptr_t) ("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:472:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V48*/ meltfptr[32] = meltgc_send((melt_ptr_t)(/*_.ODISCR__V15*/ meltfptr[14]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[7])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:473:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "))->meltobj_magic == MELTOBMAG_OBJECT);"));
        }
        ;
        /*_#plI__L20*/ meltfnum[14] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        {
            MELT_LOCATION("warmelt-outobj.melt:474:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#plI__L20*/ meltfnum[14]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:476:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:477:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "->"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:478:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.ONAME__V16*/ meltfptr[15])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:479:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ".meltobj_class = (meltobject_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:480:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*_.OUTPUT_C_CODE__V49*/ meltfptr[35] = meltgc_send((melt_ptr_t)(/*_.ODISCR__V15*/ meltfptr[14]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[7])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:481:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:482:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:483:/ cond");
        /*cond*/ if (/*_.ODOBNUM__V32*/ meltfptr[30]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:485:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " "));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:486:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:487:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "->"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:488:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.ONAME__V16*/ meltfptr[15])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:489:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ".obj_num = "));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_INTEGERBOX__L21*/ meltfnum[11] =
                        (melt_magic_discr((melt_ptr_t)(/*_.ODOBNUM__V32*/ meltfptr[30])) == MELTOBMAG_INT);;
                    MELT_LOCATION("warmelt-outobj.melt:490:/ cond");
                    /*cond*/ if (/*_#IS_INTEGERBOX__L21*/ meltfnum[11]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*_#GET_INT__L22*/ meltfnum[21] =
                                    (melt_get_int((melt_ptr_t)(/*_.ODOBNUM__V32*/ meltfptr[30])));;



                                {
                                    MELT_LOCATION("warmelt-outobj.melt:491:/ locexp");
                                    meltgc_add_strbuf_dec((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#GET_INT__L22*/ meltfnum[21]));
                                }
                                ;
                                /*clear*/ /*_._IFELSE___V50*/ meltfptr[49] = 0 ;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:490:/ clear");
                                /*clear*/ /*_#GET_INT__L22*/ meltfnum[21] = 0 ;
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
                                /*_#IS_A__L23*/ meltfnum[21] =
                                    melt_is_instance_of((melt_ptr_t)(/*_.ODOBNUM__V32*/ meltfptr[30]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[9])));;
                                MELT_LOCATION("warmelt-outobj.melt:492:/ cond");
                                /*cond*/ if (/*_#IS_A__L23*/ meltfnum[21]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-outobj.melt:493:/ getslot");
                                            {
                                                melt_ptr_t slot=NULL, obj=NULL;
                                                obj = (melt_ptr_t)(/*_.ODOBNUM__V32*/ meltfptr[30]) /*=obj*/;
                                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                /*_.NAMED_NAME__V52*/ meltfptr[51] = slot;
                                            };
                                            ;



                                            {
                                                /*^locexp*/
                                                /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                                                      melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V52*/ meltfptr[51])));
                                            }
                                            ;
                                            /*clear*/ /*_._IFELSE___V51*/ meltfptr[50] = 0 ;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:492:/ clear");
                                            /*clear*/ /*_.NAMED_NAME__V52*/ meltfptr[51] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*^block*/
                                        /*anyblock*/
                                        {


#if MELT_HAVE_DEBUG
                                            MELT_LOCATION("warmelt-outobj.melt:495:/ cppif.then");
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
                                                /*_#MELT_NEED_DBG__L24*/ meltfnum[23] =
                                                    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                                    (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                                    0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                                    ;;
                                                MELT_LOCATION("warmelt-outobj.melt:495:/ cond");
                                                /*cond*/ if (/*_#MELT_NEED_DBG__L24*/ meltfnum[23]) /*then*/
                                                    {
                                                        /*^cond.then*/
                                                        /*^block*/
                                                        /*anyblock*/
                                                        {

                                                            /*_#MELT_CALLCOUNT__L25*/ meltfnum[24] =
                                                                /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                                                meltcallcount  /* melt_callcount debugging */
#else
                                                                0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                                                ;;

                                                            MELT_CHECK_SIGNAL();
                                                            ;
                                                            MELT_LOCATION("warmelt-outobj.melt:495:/ apply");
                                                            /*apply*/
                                                            {
                                                                union meltparam_un argtab[5];
                                                                memset(&argtab, 0, sizeof(argtab));
                                                                /*^apply.arg*/
                                                                argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L25*/ meltfnum[24];
                                                                /*^apply.arg*/
                                                                argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                                /*^apply.arg*/
                                                                argtab[2].meltbp_long = 495;
                                                                /*^apply.arg*/
                                                                argtab[3].meltbp_cstring =  "outcinitfill_objinitobject unexpected odobnum=";
                                                                /*^apply.arg*/
                                                                argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.ODOBNUM__V32*/ meltfptr[30];
                                                                /*_.MELT_DEBUG_FUN__V54*/ meltfptr[53] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                            }
                                                            ;
                                                            /*_._IF___V53*/ meltfptr[51] = /*_.MELT_DEBUG_FUN__V54*/ meltfptr[53];;
                                                            /*epilog*/

                                                            MELT_LOCATION("warmelt-outobj.melt:495:/ clear");
                                                            /*clear*/ /*_#MELT_CALLCOUNT__L25*/ meltfnum[24] = 0 ;
                                                            /*^clear*/
                                                            /*clear*/ /*_.MELT_DEBUG_FUN__V54*/ meltfptr[53] = 0 ;
                                                        }
                                                        ;
                                                    }
                                                else    /*^cond.else*/
                                                    {

                                                        /*_._IF___V53*/ meltfptr[51] =  /*reallynil*/ NULL ;;
                                                    }
                                                ;

                                                {
                                                    MELT_LOCATION("warmelt-outobj.melt:495:/ locexp");
                                                    /*void*/(void)0;
                                                }
                                                ;
                                                /*^quasiblock*/


                                                /*epilog*/

                                                /*^clear*/
                                                /*clear*/ /*_#MELT_NEED_DBG__L24*/ meltfnum[23] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_._IF___V53*/ meltfptr[51] = 0 ;
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
                                            MELT_LOCATION("warmelt-outobj.melt:496:/ cppif.then");
                                            /*^block*/
                                            /*anyblock*/
                                            {


                                                MELT_CHECK_SIGNAL();
                                                ;
                                                /*^cond*/
                                                /*cond*/ if ((/*nil*/NULL)) /*then*/
                                                    {
                                                        /*^cond.then*/
                                                        /*_._IFELSE___V56*/ meltfptr[51] = (/*nil*/NULL);;
                                                    }
                                                else
                                                    {
                                                        MELT_LOCATION("warmelt-outobj.melt:496:/ cond.else");

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
                                                                argtab[0].meltbp_cstring =  "outcinitfill_objinitobject unexpected odobnum";
                                                                /*^apply.arg*/
                                                                argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                                /*^apply.arg*/
                                                                argtab[2].meltbp_long = 496;
                                                                /*^apply.arg*/
                                                                argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ODOBNUM__V32*/ meltfptr[30];
                                                                /*_.MELT_ASSERT_FAILURE_FUN__V57*/ meltfptr[56] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                            }
                                                            ;
                                                            /*_._IFELSE___V56*/ meltfptr[51] = /*_.MELT_ASSERT_FAILURE_FUN__V57*/ meltfptr[56];;
                                                            /*epilog*/

                                                            MELT_LOCATION("warmelt-outobj.melt:496:/ clear");
                                                            /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V57*/ meltfptr[56] = 0 ;
                                                        }
                                                        ;
                                                    }
                                                ;
                                                /*_.IFCPP___V55*/ meltfptr[53] = /*_._IFELSE___V56*/ meltfptr[51];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:496:/ clear");
                                                /*clear*/ /*_._IFELSE___V56*/ meltfptr[51] = 0 ;
                                            }

#else /*MELT_HAVE_DEBUG*/
                                            /*^cppif.else*/
                                            /*_.IFCPP___V55*/ meltfptr[53] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                            ;
                                            MELT_LOCATION("warmelt-outobj.melt:494:/ quasiblock");


                                            /*_.PROGN___V58*/ meltfptr[56] = /*_.IFCPP___V55*/ meltfptr[53];;
                                            /*^compute*/
                                            /*_._IFELSE___V51*/ meltfptr[50] = /*_.PROGN___V58*/ meltfptr[56];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:492:/ clear");
                                            /*clear*/ /*_.IFCPP___V55*/ meltfptr[53] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.PROGN___V58*/ meltfptr[56] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V50*/ meltfptr[49] = /*_._IFELSE___V51*/ meltfptr[50];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:490:/ clear");
                                /*clear*/ /*_#IS_A__L23*/ meltfnum[21] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V51*/ meltfptr[50] = 0 ;
                            }
                            ;
                        }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:498:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ";"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:499:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:484:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:483:/ clear");
                    /*clear*/ /*_#IS_INTEGERBOX__L21*/ meltfnum[11] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V50*/ meltfptr[49] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:501:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:502:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:503:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "->"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:504:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.ONAME__V16*/ meltfptr[15])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:505:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ".obj_hash = "));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:506:/ cond");
        /*cond*/ if (/*_.ODHASH__V28*/ meltfptr[19]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_#GET_INT__L26*/ meltfnum[24] =
                        (melt_get_int((melt_ptr_t)(/*_.ODHASH__V28*/ meltfptr[19])));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:507:/ locexp");
                        meltgc_add_strbuf_dec((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#GET_INT__L26*/ meltfnum[24]));
                    }
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:506:/ clear");
                    /*clear*/ /*_#GET_INT__L26*/ meltfnum[24] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:508:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "melt_nonzerohash ()"));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:509:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ";"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:510:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:511:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:512:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:513:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "->"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:514:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.ONAME__V16*/ meltfptr[15])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:515:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ".obj_len = "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:516:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#NBSLOTS__L11*/ meltfnum[7]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:517:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ";"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:518:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_LOCATION("warmelt-outobj.melt:393:/ clear");
        /*clear*/ /*_.ODLOC__V26*/ meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ODHASH__V28*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ODSLOTS__V30*/ meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ODOBNUM__V32*/ meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_MULTIPLE__L10*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NBSLOTS__L11*/ meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V38*/ meltfptr[36] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V47*/ meltfptr[41] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L19*/ meltfnum[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V48*/ meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L20*/ meltfnum[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V49*/ meltfptr[35] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#STRBUF_USEDLENGTH__L27*/ meltfnum[23] =
            melt_strbuf_usedlength((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]));;
        MELT_LOCATION("warmelt-outobj.melt:521:/ cond");
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
                    /*_.REFERENCED_VALUE__V59*/ meltfptr[51] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.REFERENCED_VALUE__V59*/ meltfptr[51] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#GET_INT__L28*/ meltfnum[21] =
            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V59*/ meltfptr[51])));;
        /*^compute*/
        /*_#diIRAW__L29*/ meltfnum[11] =
            ((/*_#GET_INT__L28*/ meltfnum[21]) / (2));;
        /*^compute*/
        /*_#gtI__L30*/ meltfnum[24] =
            ((/*_#STRBUF_USEDLENGTH__L27*/ meltfnum[23]) > (/*_#diIRAW__L29*/ meltfnum[11]));;
        MELT_LOCATION("warmelt-outobj.melt:521:/ cond");
        /*cond*/ if (/*_#gtI__L30*/ meltfnum[24]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:522:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L31*/ meltfnum[1] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:522:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L31*/ meltfnum[1]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L32*/ meltfnum[7] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:522:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L32*/ meltfnum[7];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 522;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "outcinitfill_objinitobject huge sbuf=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                                        /*_.MELT_DEBUG_FUN__V62*/ meltfptr[50] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V61*/ meltfptr[56] = /*_.MELT_DEBUG_FUN__V62*/ meltfptr[50];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:522:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L32*/ meltfnum[7] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V62*/ meltfptr[50] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V61*/ meltfptr[56] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:522:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L31*/ meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V61*/ meltfptr[56] = 0 ;
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
                    MELT_LOCATION("warmelt-outobj.melt:523:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L33*/ meltfnum[13] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]));;
                        MELT_LOCATION("warmelt-outobj.melt:524:/ cond");
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
                                    /*_.REFERENCED_VALUE__V64*/ meltfptr[24] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V64*/ meltfptr[24] =  /*reallynil*/ NULL ;;
                            }
                        ;
                        /*^compute*/
                        /*_#GET_INT__L34*/ meltfnum[14] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V64*/ meltfptr[24])));;
                        /*^compute*/
                        /*_#ltI__L35*/ meltfnum[7] =
                            ((/*_#STRBUF_USEDLENGTH__L33*/ meltfnum[13]) < (/*_#GET_INT__L34*/ meltfnum[14]));;
                        MELT_LOCATION("warmelt-outobj.melt:523:/ cond");
                        /*cond*/ if (/*_#ltI__L35*/ meltfnum[7]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V65*/ meltfptr[19] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:523:/ cond.else");

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
                                        argtab[0].meltbp_cstring =  "check limited sbuf";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 523;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V66*/ meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V65*/ meltfptr[19] = /*_.MELT_ASSERT_FAILURE_FUN__V66*/ meltfptr[23];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:523:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V66*/ meltfptr[23] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V63*/ meltfptr[49] = /*_._IFELSE___V65*/ meltfptr[19];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:523:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L33*/ meltfnum[13] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V64*/ meltfptr[24] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L34*/ meltfnum[14] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L35*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V65*/ meltfptr[19] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V63*/ meltfptr[49] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:521:/ quasiblock");


                    /*_.PROGN___V67*/ meltfptr[30] = /*_.IFCPP___V63*/ meltfptr[49];;
                    /*^compute*/
                    /*_._IFELSE___V60*/ meltfptr[53] = /*_.PROGN___V67*/ meltfptr[30];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:521:/ clear");
                    /*clear*/ /*_.IFCPP___V63*/ meltfptr[49] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V67*/ meltfptr[30] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IFELSE___V60*/ meltfptr[53] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_.LET___V13*/ meltfptr[11] = /*_._IFELSE___V60*/ meltfptr[53];;

        MELT_LOCATION("warmelt-outobj.melt:383:/ clear");
        /*clear*/ /*_.ODATA__V14*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ODISCR__V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ONAME__V16*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OLOCVAR__V17*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OIOPREDEF__V18*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OCLASS__V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_#DEPTHP1__L6*/ meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V22*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_#STRBUF_USEDLENGTH__L27*/ meltfnum[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REFERENCED_VALUE__V59*/ meltfptr[51] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L28*/ meltfnum[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_#diIRAW__L29*/ meltfnum[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L30*/ meltfnum[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V60*/ meltfptr[53] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:379:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V13*/ meltfptr[11];;

        {
            MELT_LOCATION("warmelt-outobj.melt:379:/ locexp");
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
        /*clear*/ /*_.IFCPP___V10*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V13*/ meltfptr[11] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTCINITFILL_OBJINITOBJECT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_19_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITOBJECT*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT fromline 1752 */

    /** start of frame for meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT// fromline 1568
        : public Melt_CallFrameWithValues<56>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[29];
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
        MeltFrame_meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<56> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT), clos) {};
        MeltFrame_meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT() //the constructor fromline 1642
            : Melt_CallFrameWithValues<56> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<56> (fil,lin, sizeof(MeltFrame_meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT)) {};
        MeltFrame_meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<56> (fil,lin, sizeof(MeltFrame_meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT


    /** end of frame for meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT fromline 1697**/

    /* end of frame for routine meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT fromline 1756 */

    /* classy proc frame meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT */ MeltFrame_meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTCINITPREDEF_OBJINITOBJECT", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:532:/ getarg");
    /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.SBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.PTRSTR__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])) != NULL);


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
        MELT_LOCATION("warmelt-outobj.melt:533:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:533:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:533:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[8];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 533;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[4].meltbp_cstring =  "outcinitpredef_objinitobject recv=";
                            /*^apply.arg*/
                            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[6].meltbp_cstring =  " ptrstr=";
                            /*^apply.arg*/
                            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:533:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:533:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:534:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJINITOBJECT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:534:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:534:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check recv";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 534;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:534:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:534:/ clear");
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
        MELT_LOCATION("warmelt-outobj.melt:535:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRBUF__L5*/ meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2])) == MELTOBMAG_STRBUF);;
            MELT_LOCATION("warmelt-outobj.melt:535:/ cond");
            /*cond*/ if (/*_#IS_STRBUF__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:535:/ cond.else");

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
                            argtab[2].meltbp_long = 535;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V11*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:535:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V10*/ meltfptr[8] = /*_._IFELSE___V11*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:535:/ clear");
            /*clear*/ /*_#IS_STRBUF__L5*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V11*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V10*/ meltfptr[8] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:536:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRING__L6*/ meltfnum[2] =
                (melt_magic_discr((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])) == MELTOBMAG_STRING);;
            MELT_LOCATION("warmelt-outobj.melt:536:/ cond");
            /*cond*/ if (/*_#IS_STRING__L6*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V14*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:536:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check ptrstr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 536;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V14*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:536:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V13*/ meltfptr[11] = /*_._IFELSE___V14*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:536:/ clear");
            /*clear*/ /*_#IS_STRING__L6*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V14*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V13*/ meltfptr[11] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:537:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OIE_DATA");
            /*_.ODATA__V17*/ meltfptr[4] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:538:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OIE_DISCR");
            /*_.ODISCR__V18*/ meltfptr[17] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:539:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OIE_CNAME");
            /*_.ONAME__V19*/ meltfptr[18] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:540:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 4, "OIE_LOCVAR");
            /*_.OLOCVAR__V20*/ meltfptr[19] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:541:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 5, "OIO_PREDEF");
            /*_.OIOPREDEF__V21*/ meltfptr[20] = slot;
        };
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:543:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L7*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.ODATA__V17*/ meltfptr[4]), (melt_ptr_t)((/*!CLASS_NREP_DATAINSTANCE*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-outobj.melt:543:/ cond");
            /*cond*/ if (/*_#IS_A__L7*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V23*/ meltfptr[22] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:543:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check odata";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 543;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ODATA__V17*/ meltfptr[4];
                            /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V23*/ meltfptr[22] = /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[23];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:543:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[23] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V22*/ meltfptr[21] = /*_._IFELSE___V23*/ meltfptr[22];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:543:/ clear");
            /*clear*/ /*_#IS_A__L7*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V23*/ meltfptr[22] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V22*/ meltfptr[21] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:544:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L8*/ meltfnum[2] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:544:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L8*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L9*/ meltfnum[1] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:544:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 544;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outcinitpredef_objinitobject oiopredef=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OIOPREDEF__V21*/ meltfptr[20];
                            /*_.MELT_DEBUG_FUN__V26*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V25*/ meltfptr[23] = /*_.MELT_DEBUG_FUN__V26*/ meltfptr[22];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:544:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L9*/ meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V26*/ meltfptr[22] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V25*/ meltfptr[23] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:544:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L8*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V25*/ meltfptr[23] = 0 ;
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
        /*_#NULL__L10*/ meltfnum[1] =
            (/*null*/(/*_.OIOPREDEF__V21*/ meltfptr[20]) == NULL);;
        MELT_LOCATION("warmelt-outobj.melt:545:/ cond");
        /*cond*/ if (/*_#NULL__L10*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*^quasiblock*/


                    /*_._RETVAL___V1*/ meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:545:/ locexp");
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
                    /*_._IF___V27*/ meltfptr[22] = /*_.RETURN___V28*/ meltfptr[23];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:545:/ clear");
                    /*clear*/ /*_.RETURN___V28*/ meltfptr[23] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V27*/ meltfptr[22] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_A__L11*/ meltfnum[2] =
            melt_is_instance_of((melt_ptr_t)(/*_.OIOPREDEF__V21*/ meltfptr[20]), (melt_ptr_t)((/*!CLASS_NREP_NIL*/ meltfrout->tabval[4])));;
        MELT_LOCATION("warmelt-outobj.melt:546:/ cond");
        /*cond*/ if (/*_#IS_A__L11*/ meltfnum[2]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*^quasiblock*/


                    /*_._RETVAL___V1*/ meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:546:/ locexp");
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
                    /*_._IF___V29*/ meltfptr[23] = /*_.RETURN___V30*/ meltfptr[29];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:546:/ clear");
                    /*clear*/ /*_.RETURN___V30*/ meltfptr[29] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V29*/ meltfptr[23] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:547:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.ODATA__V17*/ meltfptr[4]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "NREP_LOC");
            /*_.ODLOC__V32*/ meltfptr[31] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:549:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "inipredef";
            /*_.OUTPUT_LOCATION__V33*/ meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.ODLOC__V32*/ meltfptr[31]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:550:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "/*inipredef "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:551:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.ONAME__V19*/ meltfptr[18])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:552:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:553:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#STRBUF_USEDLENGTH__L12*/ meltfnum[11] =
            melt_strbuf_usedlength((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]));;
        MELT_LOCATION("warmelt-outobj.melt:555:/ cond");
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
        /*_#GET_INT__L13*/ meltfnum[12] =
            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V34*/ meltfptr[33])));;
        /*^compute*/
        /*_#diIRAW__L14*/ meltfnum[13] =
            ((/*_#GET_INT__L13*/ meltfnum[12]) / (2));;
        /*^compute*/
        /*_#gtI__L15*/ meltfnum[14] =
            ((/*_#STRBUF_USEDLENGTH__L12*/ meltfnum[11]) > (/*_#diIRAW__L14*/ meltfnum[13]));;
        MELT_LOCATION("warmelt-outobj.melt:555:/ cond");
        /*cond*/ if (/*_#gtI__L15*/ meltfnum[14]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:556:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L16*/ meltfnum[15] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:556:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L16*/ meltfnum[15]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L17*/ meltfnum[16] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:556:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L17*/ meltfnum[16];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 556;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "outcinitpredef_objinitobject huge implbuf=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                                        /*_.MELT_DEBUG_FUN__V37*/ meltfptr[36] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V36*/ meltfptr[35] = /*_.MELT_DEBUG_FUN__V37*/ meltfptr[36];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:556:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L17*/ meltfnum[16] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V37*/ meltfptr[36] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V36*/ meltfptr[35] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:556:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L16*/ meltfnum[15] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V36*/ meltfptr[35] = 0 ;
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
                    MELT_LOCATION("warmelt-outobj.melt:557:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L18*/ meltfnum[16] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]));;
                        MELT_LOCATION("warmelt-outobj.melt:558:/ cond");
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
                                    /*_.REFERENCED_VALUE__V39*/ meltfptr[35] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V39*/ meltfptr[35] =  /*reallynil*/ NULL ;;
                            }
                        ;
                        /*^compute*/
                        /*_#GET_INT__L19*/ meltfnum[15] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V39*/ meltfptr[35])));;
                        /*^compute*/
                        /*_#ltI__L20*/ meltfnum[19] =
                            ((/*_#STRBUF_USEDLENGTH__L18*/ meltfnum[16]) < (/*_#GET_INT__L19*/ meltfnum[15]));;
                        MELT_LOCATION("warmelt-outobj.melt:557:/ cond");
                        /*cond*/ if (/*_#ltI__L20*/ meltfnum[19]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V40*/ meltfptr[39] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:557:/ cond.else");

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
                                        argtab[0].meltbp_cstring =  "check limited sbuf";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 557;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V41*/ meltfptr[40] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V40*/ meltfptr[39] = /*_.MELT_ASSERT_FAILURE_FUN__V41*/ meltfptr[40];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:557:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V41*/ meltfptr[40] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V38*/ meltfptr[36] = /*_._IFELSE___V40*/ meltfptr[39];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:557:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L18*/ meltfnum[16] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V39*/ meltfptr[35] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L19*/ meltfnum[15] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L20*/ meltfnum[19] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V40*/ meltfptr[39] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V38*/ meltfptr[36] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:555:/ quasiblock");


                    /*_.PROGN___V42*/ meltfptr[40] = /*_.IFCPP___V38*/ meltfptr[36];;
                    /*^compute*/
                    /*_._IFELSE___V35*/ meltfptr[34] = /*_.PROGN___V42*/ meltfptr[40];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:555:/ clear");
                    /*clear*/ /*_.IFCPP___V38*/ meltfptr[36] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V42*/ meltfptr[40] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IFELSE___V35*/ meltfptr[34] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_A__L21*/ meltfnum[16] =
            melt_is_instance_of((melt_ptr_t)(/*_.OIOPREDEF__V21*/ meltfptr[20]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[8])));;
        MELT_LOCATION("warmelt-outobj.melt:561:/ cond");
        /*cond*/ if (/*_#IS_A__L21*/ meltfnum[16]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:562:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "if (!MELT_PREDEF("));
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:563:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.OIOPREDEF__V21*/ meltfptr[20]) /*=obj*/;
                        melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                        /*_.NAMED_NAME__V44*/ meltfptr[39] = slot;
                    };
                    ;

                    {
                        /*^locexp*/
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V44*/ meltfptr[39])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:564:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ")) MELT_STORE_PREDEF("));
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:565:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.OIOPREDEF__V21*/ meltfptr[20]) /*=obj*/;
                        melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                        /*_.NAMED_NAME__V45*/ meltfptr[36] = slot;
                    };
                    ;

                    {
                        /*^locexp*/
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V45*/ meltfptr[36])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:566:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ", (melt_ptr_t)&"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:567:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:568:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "->"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:569:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.ONAME__V19*/ meltfptr[18])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:570:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ");"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:571:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (1), 0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:572:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "else {"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:573:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (2), 0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:574:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "MELTPREDEFIX(meltpredefinited,"));
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:575:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.OIOPREDEF__V21*/ meltfptr[20]) /*=obj*/;
                        melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                        /*_.NAMED_NAME__V46*/ meltfptr[40] = slot;
                    };
                    ;

                    {
                        /*^locexp*/
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V46*/ meltfptr[40])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:576:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ") = 1;"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:577:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (2), 0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:578:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "fnotice(stderr, \"MELT: predefined object %s already defined <%s:%d\
>\\n\", \""));
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:579:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.OIOPREDEF__V21*/ meltfptr[20]) /*=obj*/;
                        melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                        /*_.NAMED_NAME__V47*/ meltfptr[46] = slot;
                    };
                    ;

                    {
                        /*^locexp*/
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V47*/ meltfptr[46])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:580:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "\", __FILE__, __LINE__);"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:581:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (2), 0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:582:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "};"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:583:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (1), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:561:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_.NAMED_NAME__V44*/ meltfptr[39] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.NAMED_NAME__V45*/ meltfptr[36] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.NAMED_NAME__V46*/ meltfptr[40] = 0 ;
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


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_INTEGERBOX__L22*/ meltfnum[15] =
                        (melt_magic_discr((melt_ptr_t)(/*_.OIOPREDEF__V21*/ meltfptr[20])) == MELTOBMAG_INT);;
                    MELT_LOCATION("warmelt-outobj.melt:585:/ cond");
                    /*cond*/ if (/*_#IS_INTEGERBOX__L22*/ meltfnum[15]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-outobj.melt:586:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "if (!melt_fetch_predefined("));
                                }
                                ;
                                /*_#GET_INT__L23*/ meltfnum[19] =
                                    (melt_get_int((melt_ptr_t)(/*_.OIOPREDEF__V21*/ meltfptr[20])));;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:587:/ locexp");
                                    meltgc_add_strbuf_dec((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#GET_INT__L23*/ meltfnum[19]));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:588:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ")) melt_store_predefined("));
                                }
                                ;
                                /*_#GET_INT__L24*/ meltfnum[23] =
                                    (melt_get_int((melt_ptr_t)(/*_.OIOPREDEF__V21*/ meltfptr[20])));;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:589:/ locexp");
                                    meltgc_add_strbuf_dec((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#GET_INT__L24*/ meltfnum[23]));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:590:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ", (melt_ptr_t)&"));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:591:/ locexp");
                                    /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                                          melt_string_str((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:592:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "->"));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:593:/ locexp");
                                    /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                                          melt_string_str((melt_ptr_t)(/*_.ONAME__V19*/ meltfptr[18])));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:594:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ");"));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:595:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (1), 0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:596:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "else {"));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:597:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (2), 0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:598:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "meltpredefinited["));
                                }
                                ;
                                /*_#GET_INT__L25*/ meltfnum[24] =
                                    (melt_get_int((melt_ptr_t)(/*_.OIOPREDEF__V21*/ meltfptr[20])));;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:599:/ locexp");
                                    meltgc_add_strbuf_dec((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#GET_INT__L25*/ meltfnum[24]));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:600:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "] = 1;"));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:601:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (2), 0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:602:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "fnotice(\"MELT: predefined #%d already defined <%s:%d>\\n\", "));
                                }
                                ;
                                /*_#GET_INT__L26*/ meltfnum[25] =
                                    (melt_get_int((melt_ptr_t)(/*_.OIOPREDEF__V21*/ meltfptr[20])));;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:603:/ locexp");
                                    meltgc_add_strbuf_dec((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#GET_INT__L26*/ meltfnum[25]));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:604:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ", __FILE__, __LINE__);"));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:605:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (2), 0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:606:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "};"));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:607:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (1), 0);
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:585:/ quasiblock");


                                /*epilog*/

                                /*^clear*/
                                /*clear*/ /*_#GET_INT__L23*/ meltfnum[19] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#GET_INT__L24*/ meltfnum[23] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#GET_INT__L25*/ meltfnum[24] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#GET_INT__L26*/ meltfnum[25] = 0 ;
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
                                /*_#NULL__L27*/ meltfnum[19] =
                                    (/*null*/(/*_.OIOPREDEF__V21*/ meltfptr[20]) == NULL);;
                                MELT_LOCATION("warmelt-outobj.melt:610:/ cond");
                                /*cond*/ if (/*_#NULL__L27*/ meltfnum[19]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-outobj.melt:611:/ quasiblock");


                                            /*_._RETVAL___V1*/ meltfptr[0] = (/*nil*/NULL);;

                                            {
                                                MELT_LOCATION("warmelt-outobj.melt:611:/ locexp");
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
                                            /*_._IFELSE___V49*/ meltfptr[36] = /*_.RETURN___V50*/ meltfptr[40];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:610:/ clear");
                                            /*clear*/ /*_.RETURN___V50*/ meltfptr[40] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*^block*/
                                        /*anyblock*/
                                        {


#if MELT_HAVE_DEBUG
                                            MELT_LOCATION("warmelt-outobj.melt:613:/ cppif.then");
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
                                                /*_#MELT_NEED_DBG__L28*/ meltfnum[23] =
                                                    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                                    (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                                    0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                                    ;;
                                                MELT_LOCATION("warmelt-outobj.melt:613:/ cond");
                                                /*cond*/ if (/*_#MELT_NEED_DBG__L28*/ meltfnum[23]) /*then*/
                                                    {
                                                        /*^cond.then*/
                                                        /*^block*/
                                                        /*anyblock*/
                                                        {

                                                            /*_#MELT_CALLCOUNT__L29*/ meltfnum[24] =
                                                                /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                                                meltcallcount  /* melt_callcount debugging */
#else
                                                                0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                                                ;;

                                                            MELT_CHECK_SIGNAL();
                                                            ;
                                                            MELT_LOCATION("warmelt-outobj.melt:613:/ apply");
                                                            /*apply*/
                                                            {
                                                                union meltparam_un argtab[5];
                                                                memset(&argtab, 0, sizeof(argtab));
                                                                /*^apply.arg*/
                                                                argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L29*/ meltfnum[24];
                                                                /*^apply.arg*/
                                                                argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                                /*^apply.arg*/
                                                                argtab[2].meltbp_long = 613;
                                                                /*^apply.arg*/
                                                                argtab[3].meltbp_cstring =  "outcinitpredef_objinitobject unexpected oiopredef=";
                                                                /*^apply.arg*/
                                                                argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OIOPREDEF__V21*/ meltfptr[20];
                                                                /*_.MELT_DEBUG_FUN__V52*/ meltfptr[40] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                            }
                                                            ;
                                                            /*_._IF___V51*/ meltfptr[46] = /*_.MELT_DEBUG_FUN__V52*/ meltfptr[40];;
                                                            /*epilog*/

                                                            MELT_LOCATION("warmelt-outobj.melt:613:/ clear");
                                                            /*clear*/ /*_#MELT_CALLCOUNT__L29*/ meltfnum[24] = 0 ;
                                                            /*^clear*/
                                                            /*clear*/ /*_.MELT_DEBUG_FUN__V52*/ meltfptr[40] = 0 ;
                                                        }
                                                        ;
                                                    }
                                                else    /*^cond.else*/
                                                    {

                                                        /*_._IF___V51*/ meltfptr[46] =  /*reallynil*/ NULL ;;
                                                    }
                                                ;

                                                {
                                                    MELT_LOCATION("warmelt-outobj.melt:613:/ locexp");
                                                    /*void*/(void)0;
                                                }
                                                ;
                                                /*^quasiblock*/


                                                /*epilog*/

                                                /*^clear*/
                                                /*clear*/ /*_#MELT_NEED_DBG__L28*/ meltfnum[23] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_._IF___V51*/ meltfptr[46] = 0 ;
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
                                            MELT_LOCATION("warmelt-outobj.melt:614:/ cppif.then");
                                            /*^block*/
                                            /*anyblock*/
                                            {


                                                MELT_CHECK_SIGNAL();
                                                ;
                                                /*^cond*/
                                                /*cond*/ if ((/*nil*/NULL)) /*then*/
                                                    {
                                                        /*^cond.then*/
                                                        /*_._IFELSE___V54*/ meltfptr[46] = (/*nil*/NULL);;
                                                    }
                                                else
                                                    {
                                                        MELT_LOCATION("warmelt-outobj.melt:614:/ cond.else");

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
                                                                argtab[0].meltbp_cstring =  "outcinitpredef_objinitobject unexpected oiopredef";
                                                                /*^apply.arg*/
                                                                argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                                /*^apply.arg*/
                                                                argtab[2].meltbp_long = 614;
                                                                /*^apply.arg*/
                                                                argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OIOPREDEF__V21*/ meltfptr[20];
                                                                /*_.MELT_ASSERT_FAILURE_FUN__V55*/ meltfptr[54] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                            }
                                                            ;
                                                            /*_._IFELSE___V54*/ meltfptr[46] = /*_.MELT_ASSERT_FAILURE_FUN__V55*/ meltfptr[54];;
                                                            /*epilog*/

                                                            MELT_LOCATION("warmelt-outobj.melt:614:/ clear");
                                                            /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V55*/ meltfptr[54] = 0 ;
                                                        }
                                                        ;
                                                    }
                                                ;
                                                /*_.IFCPP___V53*/ meltfptr[40] = /*_._IFELSE___V54*/ meltfptr[46];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:614:/ clear");
                                                /*clear*/ /*_._IFELSE___V54*/ meltfptr[46] = 0 ;
                                            }

#else /*MELT_HAVE_DEBUG*/
                                            /*^cppif.else*/
                                            /*_.IFCPP___V53*/ meltfptr[40] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                            ;
                                            MELT_LOCATION("warmelt-outobj.melt:612:/ quasiblock");


                                            /*_.PROGN___V56*/ meltfptr[54] = /*_.IFCPP___V53*/ meltfptr[40];;
                                            /*^compute*/
                                            /*_._IFELSE___V49*/ meltfptr[36] = /*_.PROGN___V56*/ meltfptr[54];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:610:/ clear");
                                            /*clear*/ /*_.IFCPP___V53*/ meltfptr[40] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.PROGN___V56*/ meltfptr[54] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V48*/ meltfptr[39] = /*_._IFELSE___V49*/ meltfptr[36];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:585:/ clear");
                                /*clear*/ /*_#NULL__L27*/ meltfnum[19] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V49*/ meltfptr[36] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V43*/ meltfptr[35] = /*_._IFELSE___V48*/ meltfptr[39];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:561:/ clear");
                    /*clear*/ /*_#IS_INTEGERBOX__L22*/ meltfnum[15] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V48*/ meltfptr[39] = 0 ;
                }
                ;
            }
        ;
        /*_.LET___V31*/ meltfptr[29] = /*_._IFELSE___V43*/ meltfptr[35];;

        MELT_LOCATION("warmelt-outobj.melt:547:/ clear");
        /*clear*/ /*_.ODLOC__V32*/ meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V33*/ meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_#STRBUF_USEDLENGTH__L12*/ meltfnum[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REFERENCED_VALUE__V34*/ meltfptr[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L13*/ meltfnum[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_#diIRAW__L14*/ meltfnum[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L15*/ meltfnum[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V35*/ meltfptr[34] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L21*/ meltfnum[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V43*/ meltfptr[35] = 0 ;
        /*_.LET___V16*/ meltfptr[14] = /*_.LET___V31*/ meltfptr[29];;

        MELT_LOCATION("warmelt-outobj.melt:537:/ clear");
        /*clear*/ /*_.ODATA__V17*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ODISCR__V18*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ONAME__V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OLOCVAR__V20*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OIOPREDEF__V21*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V22*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L10*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V27*/ meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L11*/ meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V29*/ meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V31*/ meltfptr[29] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:532:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-outobj.melt:532:/ locexp");
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
        /*clear*/ /*_.IFCPP___V10*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V13*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V16*/ meltfptr[14] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTCINITPREDEF_OBJINITOBJECT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_20_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITOBJECT*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK fromline 1752 */

    /** start of frame for meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK// fromline 1568
        : public Melt_CallFrameWithValues<40>
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
        MeltFrame_meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<40> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK), clos) {};
        MeltFrame_meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK() //the constructor fromline 1642
            : Melt_CallFrameWithValues<40> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<40> (fil,lin, sizeof(MeltFrame_meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK)) {};
        MeltFrame_meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<40> (fil,lin, sizeof(MeltFrame_meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK


    /** end of frame for meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK fromline 1697**/

    /* end of frame for routine meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK fromline 1756 */

    /* classy proc frame meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK */ MeltFrame_meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTCINITPREDEF_OBJINITHOOK", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:622:/ getarg");
    /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.SBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.PTRSTR__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])) != NULL);


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
        MELT_LOCATION("warmelt-outobj.melt:623:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:623:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:623:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[8];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 623;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[4].meltbp_cstring =  "outcinitpredef_objinithook recv=";
                            /*^apply.arg*/
                            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[6].meltbp_cstring =  " ptrstr=";
                            /*^apply.arg*/
                            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:623:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:623:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:624:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJINITHOOK*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:624:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:624:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "outcinitpredef_objinithook check recv";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 624;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:624:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:624:/ clear");
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
        MELT_LOCATION("warmelt-outobj.melt:625:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRBUF__L5*/ meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2])) == MELTOBMAG_STRBUF);;
            MELT_LOCATION("warmelt-outobj.melt:625:/ cond");
            /*cond*/ if (/*_#IS_STRBUF__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:625:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "outcinitpredef_objinithook check sbuf";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 625;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V11*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:625:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V10*/ meltfptr[8] = /*_._IFELSE___V11*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:625:/ clear");
            /*clear*/ /*_#IS_STRBUF__L5*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V11*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V10*/ meltfptr[8] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:626:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRING__L6*/ meltfnum[2] =
                (melt_magic_discr((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])) == MELTOBMAG_STRING);;
            MELT_LOCATION("warmelt-outobj.melt:626:/ cond");
            /*cond*/ if (/*_#IS_STRING__L6*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V14*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:626:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "outcinitpredef_objinithook check ptrstr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 626;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V14*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:626:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V13*/ meltfptr[11] = /*_._IFELSE___V14*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:626:/ clear");
            /*clear*/ /*_#IS_STRING__L6*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V14*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V13*/ meltfptr[11] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:627:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJINITELEM*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "OIE_DATA");
                    /*_.ODATA__V17*/ meltfptr[4] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ODATA__V17*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:628:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJINITELEM*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 3, "OIE_DISCR");
                    /*_.ODISCR__V18*/ meltfptr[17] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ODISCR__V18*/ meltfptr[17] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:629:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJINITELEM*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "OIE_CNAME");
                    /*_.ONAME__V19*/ meltfptr[18] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ONAME__V19*/ meltfptr[18] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:630:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJINITELEM*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 4, "OIE_LOCVAR");
                    /*_.OLOCVAR__V20*/ meltfptr[19] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OLOCVAR__V20*/ meltfptr[19] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:631:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJINITHOOK*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 8, "OIHK_PREDEF");
                    /*_.OPREDEF__V21*/ meltfptr[20] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OPREDEF__V21*/ meltfptr[20] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:632:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJINITHOOK*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 9, "OIHK_MODVARBIND");
                    /*_.MODVARBIND__V22*/ meltfptr[21] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.MODVARBIND__V22*/ meltfptr[21] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:633:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.ODATA__V17*/ meltfptr[4]),
                                          (melt_ptr_t)((/*!CLASS_NREP*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.ODATA__V17*/ meltfptr[4]) /*=obj*/;
                    melt_object_get_field(slot,obj, 0, "NREP_LOC");
                    /*_.ODLOC__V23*/ meltfptr[22] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.ODLOC__V23*/ meltfptr[22] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:635:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L7*/ meltfnum[1] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:635:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L7*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L8*/ meltfnum[2] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:635:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[9];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 635;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outcinitpredef_objinithook opredef=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OPREDEF__V21*/ meltfptr[20];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n odata=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.ODATA__V17*/ meltfptr[4];
                            /*^apply.arg*/
                            argtab[7].meltbp_cstring =  "\n oname=";
                            /*^apply.arg*/
                            argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.ONAME__V19*/ meltfptr[18];
                            /*_.MELT_DEBUG_FUN__V25*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V24*/ meltfptr[23] = /*_.MELT_DEBUG_FUN__V25*/ meltfptr[24];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:635:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L8*/ meltfnum[2] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V25*/ meltfptr[24] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V24*/ meltfptr[23] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:635:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L7*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V24*/ meltfptr[23] = 0 ;
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
        /*_#NULL__L9*/ meltfnum[2] =
            (/*null*/(/*_.OPREDEF__V21*/ meltfptr[20]) == NULL);;
        MELT_LOCATION("warmelt-outobj.melt:636:/ cond");
        /*cond*/ if (/*_#NULL__L9*/ meltfnum[2]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*^quasiblock*/


                    /*_._RETVAL___V1*/ meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:636:/ locexp");
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
                    /*_._IF___V26*/ meltfptr[24] = /*_.RETURN___V27*/ meltfptr[23];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:636:/ clear");
                    /*clear*/ /*_.RETURN___V27*/ meltfptr[23] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:637:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "inipredefhook";
            /*_.OUTPUT_LOCATION__V28*/ meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.ODLOC__V23*/ meltfptr[22]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:638:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "/*inipredefhook ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.ONAME__V19*/ meltfptr[18];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " */";
            /*_.ADD2OUT__V29*/ meltfptr[28] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:639:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_A__L10*/ meltfnum[1] =
            melt_is_instance_of((melt_ptr_t)(/*_.OPREDEF__V21*/ meltfptr[20]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[7])));;
        MELT_LOCATION("warmelt-outobj.melt:641:/ cond");
        /*cond*/ if (/*_#IS_A__L10*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-outobj.melt:642:/ quasiblock");


                    MELT_LOCATION("warmelt-outobj.melt:643:/ cond");
                    /*cond*/ if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.OPREDEF__V21*/ meltfptr[20]),
                                                      (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[8])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.OPREDEF__V21*/ meltfptr[20]) /*=obj*/;
                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                /*_.OPREDNAME__V31*/ meltfptr[30] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.OPREDNAME__V31*/ meltfptr[30] =  /*reallynil*/ NULL ;;
                        }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:645:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[13];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "if (!MELT_PREDEF(";
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.OPREDNAME__V31*/ meltfptr[30];
                        /*^apply.arg*/
                        argtab[2].meltbp_cstring =  ")) \n\t\t  MELT_STORE_PREDEF(";
                        /*^apply.arg*/
                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OPREDNAME__V31*/ meltfptr[30];
                        /*^apply.arg*/
                        argtab[4].meltbp_cstring =  ", (melt_ptr_t)& ";
                        /*^apply.arg*/
                        argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[6].meltbp_cstring =  "->";
                        /*^apply.arg*/
                        argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.ONAME__V19*/ meltfptr[18];
                        /*^apply.arg*/
                        argtab[8].meltbp_cstring =  ") ;\n\t\t  else {\n\t\t  MELTPREDEFIX(meltpredefinited,";
                        /*^apply.arg*/
                        argtab[9].meltbp_aptr = (melt_ptr_t*) &/*_.OPREDNAME__V31*/ meltfptr[30];
                        /*^apply.arg*/
                        argtab[10].meltbp_cstring =  ") = 1\t;\n\t\t  fnotice(stderr, \"MELT: predefined hook %s already\
 defined <%s:%d>\\n\",\n\t\t\t\t  \"";
                        /*^apply.arg*/
                        argtab[11].meltbp_aptr = (melt_ptr_t*) &/*_.OPREDNAME__V31*/ meltfptr[30];
                        /*^apply.arg*/
                        argtab[12].meltbp_cstring =  "\", __FILE__, __LINE__) ;\n\t\t  }\t\t\t\t\t\t    ;\
\n\t\t  ";
                        /*_.ADD2OUT__V32*/ meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:654:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (1), 0);
                    }
                    ;
                    /*_._IFELSE___V30*/ meltfptr[29] = /*_.ADD2OUT__V32*/ meltfptr[31];;

                    MELT_LOCATION("warmelt-outobj.melt:642:/ clear");
                    /*clear*/ /*_.OPREDNAME__V31*/ meltfptr[30] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ADD2OUT__V32*/ meltfptr[31] = 0 ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:641:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_INTEGERBOX__L11*/ meltfnum[10] =
                        (melt_magic_discr((melt_ptr_t)(/*_.OPREDEF__V21*/ meltfptr[20])) == MELTOBMAG_INT);;
                    MELT_LOCATION("warmelt-outobj.melt:656:/ cond");
                    /*cond*/ if (/*_#IS_INTEGERBOX__L11*/ meltfnum[10]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-outobj.melt:657:/ quasiblock");


                                /*_#PREDEFNUM__L12*/ meltfnum[11] =
                                    (melt_get_int((melt_ptr_t)(/*_.OPREDEF__V21*/ meltfptr[20])));;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:659:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[13];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_cstring =  "if (!melt_fetch_predefined(";
                                    /*^apply.arg*/
                                    argtab[1].meltbp_long = /*_#PREDEFNUM__L12*/ meltfnum[11];
                                    /*^apply.arg*/
                                    argtab[2].meltbp_cstring =  "))\n\t\t  melt_store_predefined(";
                                    /*^apply.arg*/
                                    argtab[3].meltbp_long = /*_#PREDEFNUM__L12*/ meltfnum[11];
                                    /*^apply.arg*/
                                    argtab[4].meltbp_cstring =  ", (melt_ptr_t)& ";
                                    /*^apply.arg*/
                                    argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
                                    /*^apply.arg*/
                                    argtab[6].meltbp_cstring =  "->";
                                    /*^apply.arg*/
                                    argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.ONAME__V19*/ meltfptr[18];
                                    /*^apply.arg*/
                                    argtab[8].meltbp_cstring =  ") ;\n\t\t  else {\n\t\t  meltpredefinited[";
                                    /*^apply.arg*/
                                    argtab[9].meltbp_long = /*_#PREDEFNUM__L12*/ meltfnum[11];
                                    /*^apply.arg*/
                                    argtab[10].meltbp_cstring =  "] = 1 ;\n\t\t  fnotice(stderr, \"MELT: predefined hook #%d already\
 defined <%s:%d>\\n\",\n\t\t\t\t  ";
                                    /*^apply.arg*/
                                    argtab[11].meltbp_long = /*_#PREDEFNUM__L12*/ meltfnum[11];
                                    /*^apply.arg*/
                                    argtab[12].meltbp_cstring =  ", __FILE__, __LINE__) ;\n\t\t  }\n\t\t  ";
                                    /*_.ADD2OUT__V34*/ meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:668:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (1), 0);
                                }
                                ;
                                /*_._IFELSE___V33*/ meltfptr[30] = /*_.ADD2OUT__V34*/ meltfptr[31];;

                                MELT_LOCATION("warmelt-outobj.melt:657:/ clear");
                                /*clear*/ /*_#PREDEFNUM__L12*/ meltfnum[11] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.ADD2OUT__V34*/ meltfptr[31] = 0 ;
                                /*epilog*/
                            }
                            ;
                        }
                    else
                        {
                            MELT_LOCATION("warmelt-outobj.melt:656:/ cond.else");

                            /*^block*/
                            /*anyblock*/
                            {


#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:671:/ cppif.then");
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
                                    /*_#MELT_NEED_DBG__L13*/ meltfnum[11] =
                                        /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                        (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                        0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                        ;;
                                    MELT_LOCATION("warmelt-outobj.melt:671:/ cond");
                                    /*cond*/ if (/*_#MELT_NEED_DBG__L13*/ meltfnum[11]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*^block*/
                                            /*anyblock*/
                                            {

                                                /*_#MELT_CALLCOUNT__L14*/ meltfnum[13] =
                                                    /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                                    meltcallcount  /* melt_callcount debugging */
#else
                                                    0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                                    ;;

                                                MELT_CHECK_SIGNAL();
                                                ;
                                                MELT_LOCATION("warmelt-outobj.melt:671:/ apply");
                                                /*apply*/
                                                {
                                                    union meltparam_un argtab[5];
                                                    memset(&argtab, 0, sizeof(argtab));
                                                    /*^apply.arg*/
                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L14*/ meltfnum[13];
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 671;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_cstring =  "outcinitpredef_objinithook unexpected opredef=";
                                                    /*^apply.arg*/
                                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OPREDEF__V21*/ meltfptr[20];
                                                    /*_.MELT_DEBUG_FUN__V36*/ meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IF___V35*/ meltfptr[31] = /*_.MELT_DEBUG_FUN__V36*/ meltfptr[35];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:671:/ clear");
                                                /*clear*/ /*_#MELT_CALLCOUNT__L14*/ meltfnum[13] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_.MELT_DEBUG_FUN__V36*/ meltfptr[35] = 0 ;
                                            }
                                            ;
                                        }
                                    else    /*^cond.else*/
                                        {

                                            /*_._IF___V35*/ meltfptr[31] =  /*reallynil*/ NULL ;;
                                        }
                                    ;

                                    {
                                        MELT_LOCATION("warmelt-outobj.melt:671:/ locexp");
                                        /*void*/(void)0;
                                    }
                                    ;
                                    /*^quasiblock*/


                                    /*epilog*/

                                    /*^clear*/
                                    /*clear*/ /*_#MELT_NEED_DBG__L13*/ meltfnum[11] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IF___V35*/ meltfptr[31] = 0 ;
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
                                MELT_LOCATION("warmelt-outobj.melt:672:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*^cond*/
                                    /*cond*/ if ((/*nil*/NULL)) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*_._IFELSE___V38*/ meltfptr[31] = (/*nil*/NULL);;
                                        }
                                    else
                                        {
                                            MELT_LOCATION("warmelt-outobj.melt:672:/ cond.else");

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
                                                    argtab[0].meltbp_cstring =  "outcinitpredef_objinithook bad predefined";
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 672;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OPREDEF__V21*/ meltfptr[20];
                                                    /*_.MELT_ASSERT_FAILURE_FUN__V39*/ meltfptr[38] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IFELSE___V38*/ meltfptr[31] = /*_.MELT_ASSERT_FAILURE_FUN__V39*/ meltfptr[38];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:672:/ clear");
                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V39*/ meltfptr[38] = 0 ;
                                            }
                                            ;
                                        }
                                    ;
                                    /*_.IFCPP___V37*/ meltfptr[35] = /*_._IFELSE___V38*/ meltfptr[31];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:672:/ clear");
                                    /*clear*/ /*_._IFELSE___V38*/ meltfptr[31] = 0 ;
                                }

#else /*MELT_HAVE_DEBUG*/
                                /*^cppif.else*/
                                /*_.IFCPP___V37*/ meltfptr[35] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:670:/ quasiblock");


                                /*_.PROGN___V40*/ meltfptr[38] = /*_.IFCPP___V37*/ meltfptr[35];;
                                /*^compute*/
                                /*_._IFELSE___V33*/ meltfptr[30] = /*_.PROGN___V40*/ meltfptr[38];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:656:/ clear");
                                /*clear*/ /*_.IFCPP___V37*/ meltfptr[35] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V40*/ meltfptr[38] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V30*/ meltfptr[29] = /*_._IFELSE___V33*/ meltfptr[30];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:641:/ clear");
                    /*clear*/ /*_#IS_INTEGERBOX__L11*/ meltfnum[10] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V33*/ meltfptr[30] = 0 ;
                }
                ;
            }
        ;
        /*_.LET___V16*/ meltfptr[14] = /*_._IFELSE___V30*/ meltfptr[29];;

        MELT_LOCATION("warmelt-outobj.melt:627:/ clear");
        /*clear*/ /*_.ODATA__V17*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ODISCR__V18*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ONAME__V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OLOCVAR__V20*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OPREDEF__V21*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MODVARBIND__V22*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ODLOC__V23*/ meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L9*/ meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V26*/ meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V28*/ meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V29*/ meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L10*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V30*/ meltfptr[29] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:622:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-outobj.melt:622:/ locexp");
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
        /*clear*/ /*_.IFCPP___V10*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V13*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V16*/ meltfptr[14] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTCINITPREDEF_OBJINITHOOK", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_21_WARMELTmiOUTOBJ_OUTCINITPREDEF_OBJINITHOOK*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE fromline 1752 */

    /** start of frame for meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE// fromline 1568
        : public Melt_CallFrameWithValues<4>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[1];
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
        MeltFrame_meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<4> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE), clos) {};
        MeltFrame_meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE() //the constructor fromline 1642
            : Melt_CallFrameWithValues<4> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<4> (fil,lin, sizeof(MeltFrame_meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE)) {};
        MeltFrame_meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<4> (fil,lin, sizeof(MeltFrame_meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE


    /** end of frame for meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE fromline 1697**/

    /* end of frame for routine meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE fromline 1756 */

    /* classy proc frame meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE */ MeltFrame_meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTDECLINIT_OBJINITMULTIPLE", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:680:/ getarg");
    /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.SBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


        {
            MELT_LOCATION("warmelt-outobj.melt:681:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " struct MELT_MULTIPLE_STRUCT("));
        }
        ;
        /*_#GET_INT__L1*/ meltfnum[0] =
            (melt_get_int((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1])));;

        {
            MELT_LOCATION("warmelt-outobj.melt:682:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#GET_INT__L1*/ meltfnum[0]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:683:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ") "));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:684:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
            /*_.NAMED_NAME__V4*/ meltfptr[3] = slot;
        };
        ;

        {
            /*^locexp*/
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V4*/ meltfptr[3])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:685:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ";"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:680:/ clear");
        /*clear*/ /*_#GET_INT__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NAMED_NAME__V4*/ meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTDECLINIT_OBJINITMULTIPLE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_22_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITMULTIPLE*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE fromline 1752 */

    /** start of frame for meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE// fromline 1568
        : public Melt_CallFrameWithValues<27>
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
        MeltFrame_meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<27> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE), clos) {};
        MeltFrame_meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE() //the constructor fromline 1642
            : Melt_CallFrameWithValues<27> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<27> (fil,lin, sizeof(MeltFrame_meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE)) {};
        MeltFrame_meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<27> (fil,lin, sizeof(MeltFrame_meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE


    /** end of frame for meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE fromline 1697**/

    /* end of frame for routine meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE fromline 1756 */

    /* classy proc frame meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE */ MeltFrame_meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTCINITFILL_OBJINITMULTIPLE", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:690:/ getarg");
    /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.SBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.PTRSTR__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])) != NULL);


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
        MELT_LOCATION("warmelt-outobj.melt:691:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:691:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:691:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 691;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outcinitfill_objinitmultiple recv=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " ptrstr=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:691:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:691:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:692:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJINITMULTIPLE*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:692:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:692:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "outcinitfill_objinitmultiple check recv";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 692;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:692:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:692:/ clear");
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
        MELT_LOCATION("warmelt-outobj.melt:693:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRING__L5*/ meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])) == MELTOBMAG_STRING);;
            MELT_LOCATION("warmelt-outobj.melt:693:/ cond");
            /*cond*/ if (/*_#IS_STRING__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:693:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "outcinitfill_objinitmultiple check ptrstr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 693;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V11*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:693:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V10*/ meltfptr[8] = /*_._IFELSE___V11*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:693:/ clear");
            /*clear*/ /*_#IS_STRING__L5*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V11*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V10*/ meltfptr[8] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:694:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OIE_CNAME");
            /*_.CNAM__V14*/ meltfptr[4] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:695:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 4, "OIE_LOCVAR");
            /*_.OLOCVAR__V15*/ meltfptr[14] = slot;
        };
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:697:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "/*inimult "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:698:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.CNAM__V14*/ meltfptr[4])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:699:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:700:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (1), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:701:/ cond");
        /*cond*/ if (/*_.OLOCVAR__V15*/ meltfptr[14]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:703:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = 1;
                        /*_.OUTPUT_C_CODE__V16*/ meltfptr[15] = meltgc_send((melt_ptr_t)(/*_.OLOCVAR__V15*/ meltfptr[14]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[3])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:704:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " = (melt_ptr_t) &"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:705:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:706:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "->"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:707:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.CNAM__V14*/ meltfptr[4])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:708:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ";"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:709:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (1), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:702:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:701:/ clear");
                    /*clear*/ /*_.OUTPUT_C_CODE__V16*/ meltfptr[15] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:711:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:712:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:713:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "->"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:714:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.CNAM__V14*/ meltfptr[4])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:715:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ".discr = (meltobject_ptr_t)("));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:716:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OIE_DISCR");
            /*_.OIE_DISCR__V17*/ meltfptr[15] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^msend*/
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = 1;
            /*_.OUTPUT_C_CODE__V18*/ meltfptr[17] = meltgc_send((melt_ptr_t)(/*_.OIE_DISCR__V17*/ meltfptr[15]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[3])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:717:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:718:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (1), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:719:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:720:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:721:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "->"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:722:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.CNAM__V14*/ meltfptr[4])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:723:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ".nbval = "));
        }
        ;
        /*_#GET_INT__L6*/ meltfnum[2] =
            (melt_get_int((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1])));;

        {
            MELT_LOCATION("warmelt-outobj.melt:724:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#GET_INT__L6*/ meltfnum[2]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:725:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ";"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[1] =
            melt_strbuf_usedlength((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]));;
        MELT_LOCATION("warmelt-outobj.melt:727:/ cond");
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
                    /*_.REFERENCED_VALUE__V19*/ meltfptr[18] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.REFERENCED_VALUE__V19*/ meltfptr[18] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#GET_INT__L8*/ meltfnum[7] =
            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V19*/ meltfptr[18])));;
        /*^compute*/
        /*_#diIRAW__L9*/ meltfnum[8] =
            ((/*_#GET_INT__L8*/ meltfnum[7]) / (2));;
        /*^compute*/
        /*_#gtI__L10*/ meltfnum[9] =
            ((/*_#STRBUF_USEDLENGTH__L7*/ meltfnum[1]) > (/*_#diIRAW__L9*/ meltfnum[8]));;
        MELT_LOCATION("warmelt-outobj.melt:727:/ cond");
        /*cond*/ if (/*_#gtI__L10*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:728:/ locexp");

#if MELT_HAVE_DEBUG
                        if (melt_need_debug (0))
                            melt_dbgshortbacktrace(( "outcinitfill_objinitmultiple huge sbuf"), (15));
#endif
                        ;
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:729:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L11*/ meltfnum[10] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:729:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L11*/ meltfnum[10]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L12*/ meltfnum[11] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:729:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 729;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "outcinitfill_objinitmultiple huge sbuf=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                                        /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V21*/ meltfptr[20] = /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:729:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L12*/ meltfnum[11] = 0 ;
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
                            MELT_LOCATION("warmelt-outobj.melt:729:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L11*/ meltfnum[10] = 0 ;
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
                    MELT_LOCATION("warmelt-outobj.melt:730:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L13*/ meltfnum[11] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]));;
                        MELT_LOCATION("warmelt-outobj.melt:731:/ cond");
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
                                    /*_.REFERENCED_VALUE__V24*/ meltfptr[20] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V24*/ meltfptr[20] =  /*reallynil*/ NULL ;;
                            }
                        ;
                        /*^compute*/
                        /*_#GET_INT__L14*/ meltfnum[10] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V24*/ meltfptr[20])));;
                        /*^compute*/
                        /*_#ltI__L15*/ meltfnum[14] =
                            ((/*_#STRBUF_USEDLENGTH__L13*/ meltfnum[11]) < (/*_#GET_INT__L14*/ meltfnum[10]));;
                        MELT_LOCATION("warmelt-outobj.melt:730:/ cond");
                        /*cond*/ if (/*_#ltI__L15*/ meltfnum[14]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V25*/ meltfptr[24] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:730:/ cond.else");

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
                                        argtab[0].meltbp_cstring =  "check limited sbuf";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 730;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V26*/ meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V25*/ meltfptr[24] = /*_.MELT_ASSERT_FAILURE_FUN__V26*/ meltfptr[25];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:730:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V26*/ meltfptr[25] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V23*/ meltfptr[21] = /*_._IFELSE___V25*/ meltfptr[24];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:730:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L13*/ meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V24*/ meltfptr[20] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L14*/ meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L15*/ meltfnum[14] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V25*/ meltfptr[24] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V23*/ meltfptr[21] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:727:/ quasiblock");


                    /*_.PROGN___V27*/ meltfptr[25] = /*_.IFCPP___V23*/ meltfptr[21];;
                    /*^compute*/
                    /*_._IFELSE___V20*/ meltfptr[19] = /*_.PROGN___V27*/ meltfptr[25];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:727:/ clear");
                    /*clear*/ /*_.IFCPP___V23*/ meltfptr[21] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V27*/ meltfptr[25] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IFELSE___V20*/ meltfptr[19] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_.LET___V13*/ meltfptr[11] = /*_._IFELSE___V20*/ meltfptr[19];;

        MELT_LOCATION("warmelt-outobj.melt:694:/ clear");
        /*clear*/ /*_.CNAM__V14*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OLOCVAR__V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OIE_DISCR__V17*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V18*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L6*/ meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REFERENCED_VALUE__V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L8*/ meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_#diIRAW__L9*/ meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L10*/ meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V20*/ meltfptr[19] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:690:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V13*/ meltfptr[11];;

        {
            MELT_LOCATION("warmelt-outobj.melt:690:/ locexp");
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
        /*clear*/ /*_.IFCPP___V10*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V13*/ meltfptr[11] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTCINITFILL_OBJINITMULTIPLE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_23_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITMULTIPLE*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE fromline 1752 */

    /** start of frame for meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE// fromline 1568
        : public Melt_CallFrameWithValues<4>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[1];
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
        MeltFrame_meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<4> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE), clos) {};
        MeltFrame_meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE() //the constructor fromline 1642
            : Melt_CallFrameWithValues<4> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<4> (fil,lin, sizeof(MeltFrame_meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE)) {};
        MeltFrame_meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<4> (fil,lin, sizeof(MeltFrame_meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE


    /** end of frame for meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE fromline 1697**/

    /* end of frame for routine meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE fromline 1756 */

    /* classy proc frame meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE */ MeltFrame_meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTDECLINIT_OBJINITCLOSURE", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:735:/ getarg");
    /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.SBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


        {
            MELT_LOCATION("warmelt-outobj.melt:736:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " struct MELT_CLOSURE_STRUCT("));
        }
        ;
        /*_#GET_INT__L1*/ meltfnum[0] =
            (melt_get_int((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1])));;

        {
            MELT_LOCATION("warmelt-outobj.melt:737:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#GET_INT__L1*/ meltfnum[0]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:738:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ") "));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:739:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OIE_CNAME");
            /*_.OIE_CNAME__V4*/ meltfptr[3] = slot;
        };
        ;

        {
            /*^locexp*/
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.OIE_CNAME__V4*/ meltfptr[3])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:740:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ";"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:735:/ clear");
        /*clear*/ /*_#GET_INT__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OIE_CNAME__V4*/ meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTDECLINIT_OBJINITCLOSURE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_24_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITCLOSURE*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE fromline 1752 */

    /** start of frame for meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE// fromline 1568
        : public Melt_CallFrameWithValues<29>
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
        MeltFrame_meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<29> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE), clos) {};
        MeltFrame_meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE() //the constructor fromline 1642
            : Melt_CallFrameWithValues<29> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<29> (fil,lin, sizeof(MeltFrame_meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE)) {};
        MeltFrame_meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<29> (fil,lin, sizeof(MeltFrame_meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE


    /** end of frame for meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE fromline 1697**/

    /* end of frame for routine meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE fromline 1756 */

    /* classy proc frame meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE */ MeltFrame_meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTCINITFILL_OBJINITCLOSURE", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:744:/ getarg");
    /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.SBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.PTRSTR__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])) != NULL);


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
        MELT_LOCATION("warmelt-outobj.melt:745:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:745:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:745:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 745;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outcinitfill_objinitclosure recv=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " ptrstr=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:745:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:745:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:746:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJINITCLOSURE*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:746:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:746:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check recv";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 746;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:746:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:746:/ clear");
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
        MELT_LOCATION("warmelt-outobj.melt:747:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRING__L5*/ meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])) == MELTOBMAG_STRING);;
            MELT_LOCATION("warmelt-outobj.melt:747:/ cond");
            /*cond*/ if (/*_#IS_STRING__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:747:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check ptrstr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 747;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V11*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:747:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V10*/ meltfptr[8] = /*_._IFELSE___V11*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:747:/ clear");
            /*clear*/ /*_#IS_STRING__L5*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V11*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V10*/ meltfptr[8] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:748:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OIE_CNAME");
            /*_.CNAM__V14*/ meltfptr[4] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:749:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 4, "OIE_LOCVAR");
            /*_.OLOCVAR__V15*/ meltfptr[14] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:750:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 5, "OICLO_ROUT");
            /*_.OROUT__V16*/ meltfptr[15] = slot;
        };
        ;
        /*_#DEPTHP1__L6*/ meltfnum[2] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

        {
            MELT_LOCATION("warmelt-outobj.melt:753:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "/*iniclos "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:754:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.CNAM__V14*/ meltfptr[4])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:755:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:756:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTHP1__L6*/ meltfnum[2]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:757:/ cond");
        /*cond*/ if (/*_.OLOCVAR__V15*/ meltfptr[14]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:760:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#DEPTHP1__L6*/ meltfnum[2];
                        /*_.OUTPUT_C_CODE__V17*/ meltfptr[16] = meltgc_send((melt_ptr_t)(/*_.OLOCVAR__V15*/ meltfptr[14]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[3])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:761:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " = (melt_ptr_t) &"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:762:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:763:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "->"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:764:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.CNAM__V14*/ meltfptr[4])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:765:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ";"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:766:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTHP1__L6*/ meltfnum[2]), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:758:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:757:/ clear");
                    /*clear*/ /*_.OUTPUT_C_CODE__V17*/ meltfptr[16] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:768:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:769:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:770:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "->"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:771:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.CNAM__V14*/ meltfptr[4])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:772:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ".discr = (meltobject_ptr_t)("));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:773:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OIE_DISCR");
            /*_.OIE_DISCR__V18*/ meltfptr[16] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^msend*/
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTHP1__L6*/ meltfnum[2];
            /*_.OUTPUT_C_CODE__V19*/ meltfptr[18] = meltgc_send((melt_ptr_t)(/*_.OIE_DISCR__V18*/ meltfptr[16]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[3])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:774:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:775:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTHP1__L6*/ meltfnum[2]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:776:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:777:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:778:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "->"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:779:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.CNAM__V14*/ meltfptr[4])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:780:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ".nbval = "));
        }
        ;
        /*_#GET_INT__L7*/ meltfnum[1] =
            (melt_get_int((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1])));;

        {
            MELT_LOCATION("warmelt-outobj.melt:781:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#GET_INT__L7*/ meltfnum[1]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:782:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ";"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:783:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTHP1__L6*/ meltfnum[2]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:784:/ cond");
        /*cond*/ if (/*_.OROUT__V16*/ meltfptr[15]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:786:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " "));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:787:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:788:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "->"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:789:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.CNAM__V14*/ meltfptr[4])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:790:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ".rout = (meltroutine_ptr_t) ("));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:791:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#DEPTHP1__L6*/ meltfnum[2];
                        /*_.OUTPUT_C_CODE__V20*/ meltfptr[19] = meltgc_send((melt_ptr_t)(/*_.OROUT__V16*/ meltfptr[15]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[3])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:792:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ");"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:793:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTHP1__L6*/ meltfnum[2]), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:785:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:784:/ clear");
                    /*clear*/ /*_.OUTPUT_C_CODE__V20*/ meltfptr[19] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#STRBUF_USEDLENGTH__L8*/ meltfnum[7] =
            melt_strbuf_usedlength((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]));;
        MELT_LOCATION("warmelt-outobj.melt:796:/ cond");
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
                    /*_.REFERENCED_VALUE__V21*/ meltfptr[19] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.REFERENCED_VALUE__V21*/ meltfptr[19] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#GET_INT__L9*/ meltfnum[8] =
            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V21*/ meltfptr[19])));;
        /*^compute*/
        /*_#diIRAW__L10*/ meltfnum[9] =
            ((/*_#GET_INT__L9*/ meltfnum[8]) / (2));;
        /*^compute*/
        /*_#gtI__L11*/ meltfnum[10] =
            ((/*_#STRBUF_USEDLENGTH__L8*/ meltfnum[7]) > (/*_#diIRAW__L10*/ meltfnum[9]));;
        MELT_LOCATION("warmelt-outobj.melt:796:/ cond");
        /*cond*/ if (/*_#gtI__L11*/ meltfnum[10]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:797:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L12*/ meltfnum[11] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:797:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L12*/ meltfnum[11]) /*then*/
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
                                    MELT_LOCATION("warmelt-outobj.melt:797:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L13*/ meltfnum[12];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 797;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "outcinitfill_objinitclosure huge sbuf=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                                        /*_.MELT_DEBUG_FUN__V24*/ meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V23*/ meltfptr[22] = /*_.MELT_DEBUG_FUN__V24*/ meltfptr[23];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:797:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L13*/ meltfnum[12] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V24*/ meltfptr[23] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V23*/ meltfptr[22] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:797:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L12*/ meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V23*/ meltfptr[22] = 0 ;
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
                        MELT_LOCATION("warmelt-outobj.melt:798:/ locexp");

#if MELT_HAVE_DEBUG
                        if (melt_need_debug (0))
                            melt_dbgshortbacktrace(( "outcinitfill_objinitclosure huge sbuf"), (15));
#endif
                        ;
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:799:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L14*/ meltfnum[12] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]));;
                        MELT_LOCATION("warmelt-outobj.melt:800:/ cond");
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
                                    /*_.REFERENCED_VALUE__V26*/ meltfptr[22] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V26*/ meltfptr[22] =  /*reallynil*/ NULL ;;
                            }
                        ;
                        /*^compute*/
                        /*_#GET_INT__L15*/ meltfnum[11] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V26*/ meltfptr[22])));;
                        /*^compute*/
                        /*_#ltI__L16*/ meltfnum[15] =
                            ((/*_#STRBUF_USEDLENGTH__L14*/ meltfnum[12]) < (/*_#GET_INT__L15*/ meltfnum[11]));;
                        MELT_LOCATION("warmelt-outobj.melt:799:/ cond");
                        /*cond*/ if (/*_#ltI__L16*/ meltfnum[15]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V27*/ meltfptr[26] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:799:/ cond.else");

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
                                        argtab[0].meltbp_cstring =  "check limited sbuf";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 799;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V28*/ meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V27*/ meltfptr[26] = /*_.MELT_ASSERT_FAILURE_FUN__V28*/ meltfptr[27];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:799:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V28*/ meltfptr[27] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V25*/ meltfptr[23] = /*_._IFELSE___V27*/ meltfptr[26];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:799:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L14*/ meltfnum[12] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V26*/ meltfptr[22] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L15*/ meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L16*/ meltfnum[15] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V27*/ meltfptr[26] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V25*/ meltfptr[23] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:796:/ quasiblock");


                    /*_.PROGN___V29*/ meltfptr[27] = /*_.IFCPP___V25*/ meltfptr[23];;
                    /*^compute*/
                    /*_._IFELSE___V22*/ meltfptr[21] = /*_.PROGN___V29*/ meltfptr[27];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:796:/ clear");
                    /*clear*/ /*_.IFCPP___V25*/ meltfptr[23] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V29*/ meltfptr[27] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IFELSE___V22*/ meltfptr[21] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_.LET___V13*/ meltfptr[11] = /*_._IFELSE___V22*/ meltfptr[21];;

        MELT_LOCATION("warmelt-outobj.melt:748:/ clear");
        /*clear*/ /*_.CNAM__V14*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OLOCVAR__V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OROUT__V16*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_#DEPTHP1__L6*/ meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OIE_DISCR__V18*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L7*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_#STRBUF_USEDLENGTH__L8*/ meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REFERENCED_VALUE__V21*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L9*/ meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_#diIRAW__L10*/ meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L11*/ meltfnum[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V22*/ meltfptr[21] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:744:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V13*/ meltfptr[11];;

        {
            MELT_LOCATION("warmelt-outobj.melt:744:/ locexp");
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
        /*clear*/ /*_.IFCPP___V10*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V13*/ meltfptr[11] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTCINITFILL_OBJINITCLOSURE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_25_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITCLOSURE*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE fromline 1752 */

    /** start of frame for meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE// fromline 1568
        : public Melt_CallFrameWithValues<4>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[1];
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
        MeltFrame_meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<4> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE), clos) {};
        MeltFrame_meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE() //the constructor fromline 1642
            : Melt_CallFrameWithValues<4> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<4> (fil,lin, sizeof(MeltFrame_meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE)) {};
        MeltFrame_meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<4> (fil,lin, sizeof(MeltFrame_meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE


    /** end of frame for meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE fromline 1697**/

    /* end of frame for routine meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE fromline 1756 */

    /* classy proc frame meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE */ MeltFrame_meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTDECLINIT_OBJINITROUTINE", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:805:/ getarg");
    /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.SBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


        {
            MELT_LOCATION("warmelt-outobj.melt:806:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " struct MELT_ROUTINE_STRUCT("));
        }
        ;
        /*_#GET_INT__L1*/ meltfnum[0] =
            (melt_get_int((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1])));;

        {
            MELT_LOCATION("warmelt-outobj.melt:807:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#GET_INT__L1*/ meltfnum[0]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:808:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ") "));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:809:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OIE_CNAME");
            /*_.OIE_CNAME__V4*/ meltfptr[3] = slot;
        };
        ;

        {
            /*^locexp*/
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.OIE_CNAME__V4*/ meltfptr[3])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:810:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ";"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:805:/ clear");
        /*clear*/ /*_#GET_INT__L1*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OIE_CNAME__V4*/ meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTDECLINIT_OBJINITROUTINE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_26_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITROUTINE*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE fromline 1752 */

    /** start of frame for meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE// fromline 1568
        : public Melt_CallFrameWithValues<45>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[26];
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
        MeltFrame_meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<45> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE), clos) {};
        MeltFrame_meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE() //the constructor fromline 1642
            : Melt_CallFrameWithValues<45> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<45> (fil,lin, sizeof(MeltFrame_meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE)) {};
        MeltFrame_meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<45> (fil,lin, sizeof(MeltFrame_meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE


    /** end of frame for meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE fromline 1697**/

    /* end of frame for routine meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE fromline 1756 */

    /* classy proc frame meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE */ MeltFrame_meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTCINITFILL_OBJINITROUTINE", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:815:/ getarg");
    /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.SBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.PTRSTR__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])) != NULL);


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
        MELT_LOCATION("warmelt-outobj.melt:816:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:816:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:816:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[8];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 816;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[4].meltbp_cstring =  "outcinitfill_objinitroutine recv=";
                            /*^apply.arg*/
                            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[6].meltbp_cstring =  " ptrstr=";
                            /*^apply.arg*/
                            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:816:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:816:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:817:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJINITROUTINE*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:817:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:817:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "heck recv";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 817;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:817:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:817:/ clear");
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
        MELT_LOCATION("warmelt-outobj.melt:818:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRING__L5*/ meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])) == MELTOBMAG_STRING);;
            MELT_LOCATION("warmelt-outobj.melt:818:/ cond");
            /*cond*/ if (/*_#IS_STRING__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:818:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "heck ptrstr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 818;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V11*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:818:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V10*/ meltfptr[8] = /*_._IFELSE___V11*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:818:/ clear");
            /*clear*/ /*_#IS_STRING__L5*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V11*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V10*/ meltfptr[8] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:819:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OIE_CNAME");
            /*_.CNAM__V13*/ meltfptr[11] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:820:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 5, "OIR_PROCROUTINE");
            /*_.IPRO__V14*/ meltfptr[4] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:821:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 4, "OIE_LOCVAR");
            /*_.OLOCVAR__V15*/ meltfptr[14] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:822:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OIE_DATA");
            /*_.NDATR__V16*/ meltfptr[15] = slot;
        };
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:824:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "/*inirout "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:825:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.CNAM__V13*/ meltfptr[11])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:826:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:827:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (1), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:828:/ cond");
        /*cond*/ if (/*_.OLOCVAR__V15*/ meltfptr[14]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:829:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "/*iniroutlocvar*/ "));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:830:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = 1;
                        /*_.OUTPUT_C_CODE__V17*/ meltfptr[16] = meltgc_send((melt_ptr_t)(/*_.OLOCVAR__V15*/ meltfptr[14]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[3])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:831:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " = (melt_ptr_t) &"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:832:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:833:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "->"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:834:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.CNAM__V13*/ meltfptr[11])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:835:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ";"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:836:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (1), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:828:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_C_CODE__V17*/ meltfptr[16] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:838:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:839:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:840:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "->"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:841:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.CNAM__V13*/ meltfptr[11])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:842:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ".discr = (meltobject_ptr_t)("));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:843:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OIE_DISCR");
            /*_.OIE_DISCR__V18*/ meltfptr[16] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^msend*/
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = 1;
            /*_.OUTPUT_C_CODE__V19*/ meltfptr[18] = meltgc_send((melt_ptr_t)(/*_.OIE_DISCR__V18*/ meltfptr[16]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[3])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:844:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ");"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:845:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (1), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:846:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " strncpy ("));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:847:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:848:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "->"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:849:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.CNAM__V13*/ meltfptr[11])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:850:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ".routdescr, \""));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_A__L6*/ meltfnum[2] =
            melt_is_instance_of((melt_ptr_t)(/*_.NDATR__V16*/ meltfptr[15]), (melt_ptr_t)((/*!CLASS_NREP_DATAROUTINE*/ meltfrout->tabval[4])));;
        MELT_LOCATION("warmelt-outobj.melt:851:/ cond");
        /*cond*/ if (/*_#IS_A__L6*/ meltfnum[2]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-outobj.melt:852:/ quasiblock");


                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.NDATR__V16*/ meltfptr[15]) /*=obj*/;
                        melt_object_get_field(slot,obj, 1, "NDATA_NAME");
                        /*_.DNAM__V20*/ meltfptr[19] = slot;
                    };
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:853:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.NDATR__V16*/ meltfptr[15]) /*=obj*/;
                        melt_object_get_field(slot,obj, 5, "NDROU_PROC");
                        /*_.DPRO__V21*/ meltfptr[20] = slot;
                    };
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:855:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L7*/ meltfnum[1] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:855:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L7*/ meltfnum[1]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L8*/ meltfnum[7] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:855:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[8];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[7];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 855;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.NDATR__V16*/ meltfptr[15];
                                        /*^apply.arg*/
                                        argtab[4].meltbp_cstring =  "outcinitfill_objinitroutine ndatr=";
                                        /*^apply.arg*/
                                        argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.NDATR__V16*/ meltfptr[15];
                                        /*^apply.arg*/
                                        argtab[6].meltbp_cstring =  " dpro=";
                                        /*^apply.arg*/
                                        argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.DPRO__V21*/ meltfptr[20];
                                        /*_.MELT_DEBUG_FUN__V23*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V22*/ meltfptr[21] = /*_.MELT_DEBUG_FUN__V23*/ meltfptr[22];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:855:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L8*/ meltfnum[7] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V23*/ meltfptr[22] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V22*/ meltfptr[21] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:855:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L7*/ meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V22*/ meltfptr[21] = 0 ;
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
                    /*_#IS_A__L9*/ meltfnum[7] =
                        melt_is_instance_of((melt_ptr_t)(/*_.DNAM__V20*/ meltfptr[19]), (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[5])));;
                    MELT_LOCATION("warmelt-outobj.melt:856:/ cond");
                    /*cond*/ if (/*_#IS_A__L9*/ meltfnum[7]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-outobj.melt:857:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.DNAM__V20*/ meltfptr[19]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                    /*_.NAMED_NAME__V24*/ meltfptr[22] = slot;
                                };
                                ;

                                {
                                    /*^locexp*/
                                    meltgc_add_strbuf_cstr ((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                            melt_string_str ((melt_ptr_t)(/*_.NAMED_NAME__V24*/ meltfptr[22])));
                                }
                                ;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:856:/ clear");
                                /*clear*/ /*_.NAMED_NAME__V24*/ meltfptr[22] = 0 ;
                            }
                            ;
                        } /*noelse*/
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_A__L10*/ meltfnum[1] =
                        melt_is_instance_of((melt_ptr_t)(/*_.DPRO__V21*/ meltfptr[20]), (melt_ptr_t)((/*!CLASS_NREP_ROUTPROC*/ meltfrout->tabval[6])));;
                    MELT_LOCATION("warmelt-outobj.melt:858:/ cond");
                    /*cond*/ if (/*_#IS_A__L10*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-outobj.melt:859:/ quasiblock");


                                /*^getslot*/
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.DPRO__V21*/ meltfptr[20]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 0, "NREP_LOC");
                                    /*_.DLOC__V25*/ meltfptr[21] = slot;
                                };
                                ;
                                /*_.MIXINT_VAL__V26*/ meltfptr[22] =
                                    (melt_val_mixint((melt_ptr_t)(/*_.DLOC__V25*/ meltfptr[21])));;
                                MELT_LOCATION("warmelt-outobj.melt:860:/ cond");
                                /*cond*/ if (/*_.MIXINT_VAL__V26*/ meltfptr[22]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*_.LOCFIL__V27*/ meltfptr[26] = /*_.MIXINT_VAL__V26*/ meltfptr[22];;
                                    }
                                else
                                    {
                                        MELT_LOCATION("warmelt-outobj.melt:860:/ cond.else");

                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            /*_.MIXLOC_VAL__V28*/ meltfptr[27] =
                                                (melt_val_mixloc((melt_ptr_t)(/*_.DLOC__V25*/ meltfptr[21])));;
                                            /*^compute*/
                                            /*_.LOCFIL__V27*/ meltfptr[26] = /*_.MIXLOC_VAL__V28*/ meltfptr[27];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:860:/ clear");
                                            /*clear*/ /*_.MIXLOC_VAL__V28*/ meltfptr[27] = 0 ;
                                        }
                                        ;
                                    }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:862:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " @"));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:863:/ locexp");
                                    meltgc_add_strbuf_cstr ((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                            melt_string_str ((melt_ptr_t)(/*_.LOCFIL__V27*/ meltfptr[26])));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:864:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ":"));
                                }
                                ;
                                /*_#GET_INT__L11*/ meltfnum[10] =
                                    (melt_get_int((melt_ptr_t)(/*_.DLOC__V25*/ meltfptr[21])));;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:865:/ locexp");
                                    meltgc_add_strbuf_dec((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#GET_INT__L11*/ meltfnum[10]));
                                }
                                ;

                                MELT_LOCATION("warmelt-outobj.melt:859:/ clear");
                                /*clear*/ /*_.DLOC__V25*/ meltfptr[21] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.MIXINT_VAL__V26*/ meltfptr[22] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.LOCFIL__V27*/ meltfptr[26] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#GET_INT__L11*/ meltfnum[10] = 0 ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;

                    MELT_LOCATION("warmelt-outobj.melt:852:/ clear");
                    /*clear*/ /*_.DNAM__V20*/ meltfptr[19] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.DPRO__V21*/ meltfptr[20] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#IS_A__L9*/ meltfnum[7] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#IS_A__L10*/ meltfnum[1] = 0 ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:851:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:869:/ locexp");
                        meltgc_add_strbuf_cstr ((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                melt_string_str ((melt_ptr_t)(/*_.CNAM__V13*/ meltfptr[11])));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:870:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "\",  MELT_ROUTDESCR_LEN - 1);"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:871:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (1), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:872:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:873:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:874:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "->"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:875:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.CNAM__V13*/ meltfptr[11])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:876:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ".nbval = "));
        }
        ;
        /*_#GET_INT__L12*/ meltfnum[10] =
            (melt_get_int((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1])));;

        {
            MELT_LOCATION("warmelt-outobj.melt:877:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#GET_INT__L12*/ meltfnum[10]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:878:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ";"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:879:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (1), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:880:/ cond");
        /*cond*/ if (/*_.IPRO__V14*/ meltfptr[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:882:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L13*/ meltfnum[7] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:882:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L13*/ meltfnum[7]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L14*/ meltfnum[1] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:882:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L14*/ meltfnum[1];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 882;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "outcinitfill_objinitroutine ipro=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.IPRO__V14*/ meltfptr[4];
                                        /*_.MELT_DEBUG_FUN__V30*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V29*/ meltfptr[27] = /*_.MELT_DEBUG_FUN__V30*/ meltfptr[21];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:882:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L14*/ meltfnum[1] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V30*/ meltfptr[21] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V29*/ meltfptr[27] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:882:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L13*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V29*/ meltfptr[27] = 0 ;
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
                    MELT_LOCATION("warmelt-outobj.melt:883:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#IS_A__L15*/ meltfnum[1] =
                            melt_is_instance_of((melt_ptr_t)(/*_.IPRO__V14*/ meltfptr[4]), (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[5])));;
                        MELT_LOCATION("warmelt-outobj.melt:883:/ cond");
                        /*cond*/ if (/*_#IS_A__L15*/ meltfnum[1]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V32*/ meltfptr[26] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:883:/ cond.else");

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
                                        argtab[0].meltbp_cstring =  "check ipro";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 883;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IPRO__V14*/ meltfptr[4];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V32*/ meltfptr[26] = /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[19];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:883:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[19] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V31*/ meltfptr[22] = /*_._IFELSE___V32*/ meltfptr[26];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:883:/ clear");
                        /*clear*/ /*_#IS_A__L15*/ meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V32*/ meltfptr[26] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V31*/ meltfptr[22] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:884:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "MELT_ROUTINE_SET_ROUTCODE(&"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:885:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:886:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "->"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:887:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.CNAM__V13*/ meltfptr[11])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:888:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ", "));
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:889:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.IPRO__V14*/ meltfptr[4]) /*=obj*/;
                        melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                        /*_.NAMED_NAME__V34*/ meltfptr[20] = slot;
                    };
                    ;

                    {
                        /*^locexp*/
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.NAMED_NAME__V34*/ meltfptr[20])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:890:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ");"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:891:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (1), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:881:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:880:/ clear");
                    /*clear*/ /*_.IFCPP___V31*/ meltfptr[22] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.NAMED_NAME__V34*/ meltfptr[20] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:894:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L16*/ meltfnum[7] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:894:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L16*/ meltfnum[7]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L17*/ meltfnum[1] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:894:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L17*/ meltfnum[1];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 894;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "outcinitfill_objinitroutine (noipro) recv=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                                        /*_.MELT_DEBUG_FUN__V36*/ meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V35*/ meltfptr[21] = /*_.MELT_DEBUG_FUN__V36*/ meltfptr[27];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:894:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L17*/ meltfnum[1] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V36*/ meltfptr[27] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V35*/ meltfptr[21] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:894:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L16*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V35*/ meltfptr[21] = 0 ;
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
                        MELT_LOCATION("warmelt-outobj.melt:895:/ locexp");

#if MELT_HAVE_DEBUG
                        if (melt_need_debug (0))
                            melt_dbgshortbacktrace(( "outcinitfill_objinitroutine noipro"), (12));
#endif
                        ;
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:896:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( "#warning no procedure in objinitroutine "));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:897:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                              melt_string_str((melt_ptr_t)(/*_.CNAM__V13*/ meltfptr[11])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:898:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (1), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:893:/ quasiblock");


                    /*epilog*/
                }
                ;
            }
        ;

        MELT_LOCATION("warmelt-outobj.melt:819:/ clear");
        /*clear*/ /*_.CNAM__V13*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IPRO__V14*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OLOCVAR__V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NDATR__V16*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OIE_DISCR__V18*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L6*/ meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L12*/ meltfnum[10] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#STRBUF_USEDLENGTH__L18*/ meltfnum[1] =
            melt_strbuf_usedlength((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]));;
        MELT_LOCATION("warmelt-outobj.melt:903:/ cond");
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
                    /*_.REFERENCED_VALUE__V37*/ meltfptr[19] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.REFERENCED_VALUE__V37*/ meltfptr[19] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#GET_INT__L19*/ meltfnum[7] =
            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V37*/ meltfptr[19])));;
        /*^compute*/
        /*_#diIRAW__L20*/ meltfnum[2] =
            ((/*_#GET_INT__L19*/ meltfnum[7]) / (2));;
        /*^compute*/
        /*_#gtI__L21*/ meltfnum[10] =
            ((/*_#STRBUF_USEDLENGTH__L18*/ meltfnum[1]) > (/*_#diIRAW__L20*/ meltfnum[2]));;
        MELT_LOCATION("warmelt-outobj.melt:903:/ cond");
        /*cond*/ if (/*_#gtI__L21*/ meltfnum[10]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:904:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L22*/ meltfnum[21] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:904:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L22*/ meltfnum[21]) /*then*/
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

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:904:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L23*/ meltfnum[22];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 904;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "outcinitfill_objinitroutine huge sbuf=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                                        /*_.MELT_DEBUG_FUN__V40*/ meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V39*/ meltfptr[22] = /*_.MELT_DEBUG_FUN__V40*/ meltfptr[20];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:904:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L23*/ meltfnum[22] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V40*/ meltfptr[20] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V39*/ meltfptr[22] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:904:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L22*/ meltfnum[21] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V39*/ meltfptr[22] = 0 ;
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
                        MELT_LOCATION("warmelt-outobj.melt:905:/ locexp");

#if MELT_HAVE_DEBUG
                        if (melt_need_debug (0))
                            melt_dbgshortbacktrace(( "outcinitfill_objinitroutine huge sbuf"), (12));
#endif
                        ;
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:906:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L24*/ meltfnum[22] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]));;
                        MELT_LOCATION("warmelt-outobj.melt:907:/ cond");
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
                                    /*_.REFERENCED_VALUE__V42*/ meltfptr[21] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V42*/ meltfptr[21] =  /*reallynil*/ NULL ;;
                            }
                        ;
                        /*^compute*/
                        /*_#GET_INT__L25*/ meltfnum[21] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V42*/ meltfptr[21])));;
                        /*^compute*/
                        /*_#ltI__L26*/ meltfnum[25] =
                            ((/*_#STRBUF_USEDLENGTH__L24*/ meltfnum[22]) < (/*_#GET_INT__L25*/ meltfnum[21]));;
                        MELT_LOCATION("warmelt-outobj.melt:906:/ cond");
                        /*cond*/ if (/*_#ltI__L26*/ meltfnum[25]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V43*/ meltfptr[11] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:906:/ cond.else");

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
                                        argtab[0].meltbp_cstring =  "check limited sbuf";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 906;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V44*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V43*/ meltfptr[11] = /*_.MELT_ASSERT_FAILURE_FUN__V44*/ meltfptr[4];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:906:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V44*/ meltfptr[4] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V41*/ meltfptr[27] = /*_._IFELSE___V43*/ meltfptr[11];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:906:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L24*/ meltfnum[22] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V42*/ meltfptr[21] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L25*/ meltfnum[21] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L26*/ meltfnum[25] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V43*/ meltfptr[11] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V41*/ meltfptr[27] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:903:/ quasiblock");


                    /*_.PROGN___V45*/ meltfptr[14] = /*_.IFCPP___V41*/ meltfptr[27];;
                    /*^compute*/
                    /*_._IFELSE___V38*/ meltfptr[26] = /*_.PROGN___V45*/ meltfptr[14];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:903:/ clear");
                    /*clear*/ /*_.IFCPP___V41*/ meltfptr[27] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V45*/ meltfptr[14] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IFELSE___V38*/ meltfptr[26] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:815:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_._IFELSE___V38*/ meltfptr[26];;

        {
            MELT_LOCATION("warmelt-outobj.melt:815:/ locexp");
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
        /*clear*/ /*_.IFCPP___V10*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_#STRBUF_USEDLENGTH__L18*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REFERENCED_VALUE__V37*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L19*/ meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_#diIRAW__L20*/ meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L21*/ meltfnum[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V38*/ meltfptr[26] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTCINITFILL_OBJINITROUTINE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_27_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITROUTINE*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK fromline 1752 */

    /** start of frame for meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK// fromline 1568
        : public Melt_CallFrameWithValues<7>
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
        MeltFrame_meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<7> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK), clos) {};
        MeltFrame_meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK() //the constructor fromline 1642
            : Melt_CallFrameWithValues<7> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<7> (fil,lin, sizeof(MeltFrame_meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK)) {};
        MeltFrame_meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<7> (fil,lin, sizeof(MeltFrame_meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK


    /** end of frame for meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK fromline 1697**/

    /* end of frame for routine meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK fromline 1756 */

    /* classy proc frame meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK */ MeltFrame_meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTDECLINIT_OBJINITHOOK", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:914:/ getarg");
    /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.SBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:915:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L1*/ meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:915:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:915:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 915;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outdeclinit_objinithook recv=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V4*/ meltfptr[3] = /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:915:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:915:/ locexp");
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

        {
            MELT_LOCATION("warmelt-outobj.melt:916:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( " struct MELT_HOOK_STRUCT("));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:917:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJINITHOOK*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 7, "OIHK_CLOVAL");
                    /*_.OIHK_CLOVAL__V6*/ meltfptr[4] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OIHK_CLOVAL__V6*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#MULTIPLE_LENGTH__L3*/ meltfnum[1] =
            (melt_multiple_length((melt_ptr_t)(/*_.OIHK_CLOVAL__V6*/ meltfptr[4])));;

        {
            MELT_LOCATION("warmelt-outobj.melt:917:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#MULTIPLE_LENGTH__L3*/ meltfnum[1]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:918:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ") "));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:919:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJINITELEM*/ meltfrout->tabval[2])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "OIE_CNAME");
                    /*_.OIE_CNAME__V7*/ meltfptr[3] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OIE_CNAME__V7*/ meltfptr[3] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:919:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                                  melt_string_str((melt_ptr_t)(/*_.OIE_CNAME__V7*/ meltfptr[3])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:920:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), ( ";"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:914:/ clear");
        /*clear*/ /*_.OIHK_CLOVAL__V6*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#MULTIPLE_LENGTH__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OIE_CNAME__V7*/ meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTDECLINIT_OBJINITHOOK", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_28_WARMELTmiOUTOBJ_OUTDECLINIT_OBJINITHOOK*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK fromline 1752 */

    /** start of frame for meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK// fromline 1568
        : public Melt_CallFrameWithValues<37>
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
        MeltFrame_meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<37> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK), clos) {};
        MeltFrame_meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK() //the constructor fromline 1642
            : Melt_CallFrameWithValues<37> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<37> (fil,lin, sizeof(MeltFrame_meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK)) {};
        MeltFrame_meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<37> (fil,lin, sizeof(MeltFrame_meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK


    /** end of frame for meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK fromline 1697**/

    /* end of frame for routine meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK fromline 1756 */

    /* classy proc frame meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK */ MeltFrame_meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTCINITFILL_OBJINITHOOK", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:927:/ getarg");
    /*_.RECV__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.SBUF__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.PTRSTR__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.PTRSTR__V4*/ meltfptr[3])) != NULL);


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
        MELT_LOCATION("warmelt-outobj.melt:928:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:928:/ cond");
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
                            ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]))));;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:928:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[9];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 928;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outcinitfill_objinithook recv=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n.. of discrim:";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.DISCRIM__V6*/ meltfptr[5];
                            /*^apply.arg*/
                            argtab[7].meltbp_cstring =  "\n.. ptrstr=";
                            /*^apply.arg*/
                            argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
                            /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:928:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:928:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:930:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJINITHOOK*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:930:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:930:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check recv";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 930;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.RECV__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/ meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[4];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:930:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[4] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/ meltfptr[5] = /*_._IFELSE___V9*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:930:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/ meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:931:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRBUF__L5*/ meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2])) == MELTOBMAG_STRBUF);;
            MELT_LOCATION("warmelt-outobj.melt:931:/ cond");
            /*cond*/ if (/*_#IS_STRBUF__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:931:/ cond.else");

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
                            argtab[2].meltbp_long = 931;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/ meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:931:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/ meltfptr[4] = /*_._IFELSE___V12*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:931:/ clear");
            /*clear*/ /*_#IS_STRBUF__L5*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V12*/ meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V11*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:932:/ quasiblock");


        /*^cond*/
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJINITELEM*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "OIE_CNAME");
                    /*_.CNAME__V14*/ meltfptr[12] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.CNAME__V14*/ meltfptr[12] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:933:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 4, "OIE_LOCVAR");
            /*_.OLOCVAR__V15*/ meltfptr[6] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:934:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OIE_DATA");
            /*_.NDATR__V16*/ meltfptr[15] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:935:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJINITHOOK*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 6, "OIHK_DATA");
                    /*_.HKDATA__V17*/ meltfptr[16] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.HKDATA__V17*/ meltfptr[16] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:936:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJINITHOOK*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 5, "OIHK_PROCHOOK");
                    /*_.HKPROC__V18*/ meltfptr[17] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.HKPROC__V18*/ meltfptr[17] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:937:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJINITHOOK*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 7, "OIHK_CLOVAL");
                    /*_.OCLOVALTUP__V19*/ meltfptr[18] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OCLOVALTUP__V19*/ meltfptr[18] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:938:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJINITHOOK*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 8, "OIHK_PREDEF");
                    /*_.OPREDEF__V20*/ meltfptr[19] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OPREDEF__V20*/ meltfptr[19] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#NBCLOVAL__L6*/ meltfnum[2] =
            (melt_multiple_length((melt_ptr_t)(/*_.OCLOVALTUP__V19*/ meltfptr[18])));;
        MELT_LOCATION("warmelt-outobj.melt:940:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.HKPROC__V18*/ meltfptr[17]),
                                          (melt_ptr_t)((/*!CLASS_NREP_CLOSPROC*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.HKPROC__V18*/ meltfptr[17]) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "NRCLOP_NAME");
                    /*_.HKNAME__V21*/ meltfptr[20] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.HKNAME__V21*/ meltfptr[20] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:942:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L7*/ meltfnum[1] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:942:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L7*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L8*/ meltfnum[7] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:942:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[9];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[7];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 942;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outcinitfill_objinithook hkproc=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.HKPROC__V18*/ meltfptr[17];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n .. hkdata=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.HKDATA__V17*/ meltfptr[16];
                            /*^apply.arg*/
                            argtab[7].meltbp_cstring =  "\n .. opredef=";
                            /*^apply.arg*/
                            argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.OPREDEF__V20*/ meltfptr[19];
                            /*_.MELT_DEBUG_FUN__V23*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V22*/ meltfptr[21] = /*_.MELT_DEBUG_FUN__V23*/ meltfptr[22];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:942:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L8*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V23*/ meltfptr[22] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V22*/ meltfptr[21] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:942:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L7*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V22*/ meltfptr[21] = 0 ;
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
            MELT_LOCATION("warmelt-outobj.melt:944:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:945:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[5];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "/*inihook ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.CNAME__V14*/ meltfptr[12];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " fromline ";
            /*^apply.arg*/
            argtab[3].meltbp_long = 945;
            /*^apply.arg*/
            argtab[4].meltbp_cstring =  " */";
            /*_.ADD2OUT__V24*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_#plI__L9*/ meltfnum[7] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        {
            MELT_LOCATION("warmelt-outobj.melt:946:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#plI__L9*/ meltfnum[7]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:947:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[7];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  " ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "->";
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CNAME__V14*/ meltfptr[12];
            /*^apply.arg*/
            argtab[4].meltbp_cstring =  ".discr = /*fromline ";
            /*^apply.arg*/
            argtab[5].meltbp_long = 947;
            /*^apply.arg*/
            argtab[6].meltbp_cstring =  "*/ (meltobject_ptr_t)(";
            /*_.ADD2OUT__V25*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:948:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_OBJINITELEM*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.RECV__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 3, "OIE_DISCR");
                    /*_.OIE_DISCR__V26*/ meltfptr[25] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OIE_DISCR__V26*/ meltfptr[25] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#plI__L10*/ meltfnum[1] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:948:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#plI__L10*/ meltfnum[1];
            /*_.OUTPUT_C_CODE__V27*/ meltfptr[26] = meltgc_send((melt_ptr_t)(/*_.OIE_DISCR__V26*/ meltfptr[25]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[6])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:949:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  ");";
            /*_.ADD2OUT__V28*/ meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_#plI__L11*/ meltfnum[10] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        {
            MELT_LOCATION("warmelt-outobj.melt:950:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#plI__L11*/ meltfnum[10]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:951:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[21];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "   /*inihook ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.HKNAME__V21*/ meltfptr[20];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " data fromline ";
            /*^apply.arg*/
            argtab[3].meltbp_long = 953;
            /*^apply.arg*/
            argtab[4].meltbp_cstring =  " */\n      ";
            /*^apply.arg*/
            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[6].meltbp_cstring =  "->";
            /*^apply.arg*/
            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.CNAME__V14*/ meltfptr[12];
            /*^apply.arg*/
            argtab[8].meltbp_cstring =  ".nbval = ";
            /*^apply.arg*/
            argtab[9].meltbp_long = /*_#NBCLOVAL__L6*/ meltfnum[2];
            /*^apply.arg*/
            argtab[10].meltbp_cstring =  "\t\t\t\t ;\n      MELT_HOOK_SET_ROUTCODE (&";
            /*^apply.arg*/
            argtab[11].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[12].meltbp_cstring =  "->";
            /*^apply.arg*/
            argtab[13].meltbp_aptr = (melt_ptr_t*) &/*_.CNAME__V14*/ meltfptr[12];
            /*^apply.arg*/
            argtab[14].meltbp_cstring =  ", melthook_";
            /*^apply.arg*/
            argtab[15].meltbp_aptr = (melt_ptr_t*) &/*_.HKNAME__V21*/ meltfptr[20];
            /*^apply.arg*/
            argtab[16].meltbp_cstring =  ") ;\n      ";
            /*^apply.arg*/
            argtab[17].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[18].meltbp_cstring =  "->";
            /*^apply.arg*/
            argtab[19].meltbp_aptr = (melt_ptr_t*) &/*_.CNAME__V14*/ meltfptr[12];
            /*^apply.arg*/
            argtab[20].meltbp_cstring =  ".hookdata = ";
            /*_.ADD2OUT__V29*/ meltfptr[28] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:958:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = 1;
            /*_.OUTPUT_C_CODE__V30*/ meltfptr[29] = meltgc_send((melt_ptr_t)(/*_.HKDATA__V17*/ meltfptr[16]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[6])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:959:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  ";";
            /*_.ADD2OUT__V31*/ meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_#plI__L12*/ meltfnum[11] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        {
            MELT_LOCATION("warmelt-outobj.melt:960:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#plI__L12*/ meltfnum[11]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:961:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[5];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "strncpy (";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "->";
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CNAME__V14*/ meltfptr[12];
            /*^apply.arg*/
            argtab[4].meltbp_cstring =  ".hookname, \"";
            /*_.ADD2OUT__V32*/ meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:962:/ locexp");
            meltgc_add_strbuf_cstr ((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]),
                                    melt_string_str ((melt_ptr_t)(/*_.HKNAME__V21*/ meltfptr[20])));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:963:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[5];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "\", sizeof(";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "->";
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CNAME__V14*/ meltfptr[12];
            /*^apply.arg*/
            argtab[4].meltbp_cstring =  ".hookname)-1);";
            /*_.ADD2OUT__V33*/ meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:964:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:965:/ cond");
        /*cond*/ if (/*_.OPREDEF__V20*/ meltfptr[19]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:966:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[13];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "/*predefinedhook ";
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.OPREDEF__V20*/ meltfptr[19];
                        /*^apply.arg*/
                        argtab[2].meltbp_cstring =  " for ";
                        /*^apply.arg*/
                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CNAME__V14*/ meltfptr[12];
                        /*^apply.arg*/
                        argtab[4].meltbp_cstring =  "*/\n\tif (!MELT_PREDEF(";
                        /*^apply.arg*/
                        argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.OPREDEF__V20*/ meltfptr[19];
                        /*^apply.arg*/
                        argtab[6].meltbp_cstring =  "))\n\tMELT_STORE_PREDEF(";
                        /*^apply.arg*/
                        argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.OPREDEF__V20*/ meltfptr[19];
                        /*^apply.arg*/
                        argtab[8].meltbp_cstring =  ", (melt_ptr_t) &";
                        /*^apply.arg*/
                        argtab[9].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[10].meltbp_cstring =  "->";
                        /*^apply.arg*/
                        argtab[11].meltbp_aptr = (melt_ptr_t*) &/*_.CNAME__V14*/ meltfptr[12];
                        /*^apply.arg*/
                        argtab[12].meltbp_cstring =  ") ;\n\t";
                        /*_.ADD2OUT__V34*/ meltfptr[33] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:972:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:965:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_.ADD2OUT__V34*/ meltfptr[33] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:974:/ cond");
        /*cond*/ if (/*_.OLOCVAR__V15*/ meltfptr[6]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:975:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = 1;
                        /*_.OUTPUT_C_CODE__V35*/ meltfptr[33] = meltgc_send((melt_ptr_t)(/*_.OLOCVAR__V15*/ meltfptr[6]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[6])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:976:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[7];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  " = /*inihooklocvar ";
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.HKNAME__V21*/ meltfptr[20];
                        /*^apply.arg*/
                        argtab[2].meltbp_cstring =  "*/ (melt_ptr_t) &";
                        /*^apply.arg*/
                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.PTRSTR__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[4].meltbp_cstring =  "->";
                        /*^apply.arg*/
                        argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.CNAME__V14*/ meltfptr[12];
                        /*^apply.arg*/
                        argtab[6].meltbp_cstring =  " ;\n\t\t";
                        /*_.ADD2OUT__V36*/ meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:979:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (1), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:974:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_C_CODE__V35*/ meltfptr[33] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.ADD2OUT__V36*/ meltfptr[35] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:981:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "/*done inihook ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.CNAME__V14*/ meltfptr[12];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " */";
            /*_.ADD2OUT__V37*/ meltfptr[33] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:982:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V3*/ meltfptr[2]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_LOCATION("warmelt-outobj.melt:932:/ clear");
        /*clear*/ /*_.CNAME__V14*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OLOCVAR__V15*/ meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NDATR__V16*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HKDATA__V17*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HKPROC__V18*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OCLOVALTUP__V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OPREDEF__V20*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NBCLOVAL__L6*/ meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_.HKNAME__V21*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V24*/ meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L9*/ meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V25*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OIE_DISCR__V26*/ meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L10*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V27*/ meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V28*/ meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L11*/ meltfnum[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V29*/ meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V30*/ meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V31*/ meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L12*/ meltfnum[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V32*/ meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V33*/ meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V37*/ meltfptr[33] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:927:/ clear");
        /*clear*/ /*_.IFCPP___V8*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V11*/ meltfptr[4] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTCINITFILL_OBJINITHOOK", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_29_WARMELTmiOUTOBJ_OUTCINITFILL_OBJINITHOOK*/




/**** end of warmelt-outobj+01.cc ****/

