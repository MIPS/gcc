/* GCC MELT GENERATED C++ FILE warmelt-outobj+04.cc - DO NOT EDIT - see http://gcc-melt.org/ */
/* secondary MELT generated C++ file of rank #4 */
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt_f4[] = MELT_RUN_HASHMD5 /* from melt-run.h */;


/**** warmelt-outobj+04.cc declarations ****/
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



/**** warmelt-outobj+04.cc implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR fromline 1752 */

    /** start of frame for meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR// fromline 1568
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
        MeltFrame_meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<19> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR), clos) {};
        MeltFrame_meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR() //the constructor fromline 1642
            : Melt_CallFrameWithValues<19> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR)) {};
        MeltFrame_meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR


    /** end of frame for meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR fromline 1697**/

    /* end of frame for routine meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR fromline 1756 */

    /* classy proc frame meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR */ MeltFrame_meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJCOMMENTINSTR", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3406:/ getarg");
    /*_.OBCI__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3407:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBCI__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJCOMMENTINSTR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3407:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3407:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check obci";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3407;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBCI__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3407:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3407:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3408:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBCI__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.OLOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3409:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBCI__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBCI_COMMENT");
            /*_.COMS__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3410:/ quasiblock");


        /*_.SBU__V12*/ meltfptr[11] =
            (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[2])), (const char*)0);;

        {
            MELT_LOCATION("warmelt-outobj.melt:3411:/ locexp");
            meltgc_add_strbuf_ccomment((melt_ptr_t)(/*_.SBU__V12*/ meltfptr[11]),
                                       melt_string_str((melt_ptr_t)(/*_.COMS__V10*/ meltfptr[9])));
        }
        ;
        /*_.STRBUF2STRING__V13*/ meltfptr[12] =
            (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_STRING*/ meltfrout->tabval[3])), melt_strbuf_str((melt_ptr_t)(/*_.SBU__V12*/ meltfptr[11]))));;
        /*^compute*/
        /*_.LET___V11*/ meltfptr[10] = /*_.STRBUF2STRING__V13*/ meltfptr[12];;

        MELT_LOCATION("warmelt-outobj.melt:3410:/ clear");
        /*clear*/ /*_.SBU__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.STRBUF2STRING__V13*/ meltfptr[12] = 0 ;
        /*_.COMSTR__V14*/ meltfptr[11] = /*_.LET___V11*/ meltfptr[10];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3415:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "comment";
            /*_.OUTPUT_LOCATION__V15*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3416:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/**COMMENT: "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3417:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.COMSTR__V14*/ meltfptr[11])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3418:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " **/;"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3419:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3420:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L3*/ meltfnum[1] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3421:/ cond");
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
            /*_#GET_INT__L4*/ meltfnum[3] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V17*/ meltfptr[16])));;
            /*^compute*/
            /*_#ltI__L5*/ meltfnum[4] =
                ((/*_#STRBUF_USEDLENGTH__L3*/ meltfnum[1]) < (/*_#GET_INT__L4*/ meltfnum[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3420:/ cond");
            /*cond*/ if (/*_#ltI__L5*/ meltfnum[4]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V18*/ meltfptr[17] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3420:/ cond.else");

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
                            argtab[2].meltbp_long = 3420;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V18*/ meltfptr[17] = /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3420:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V16*/ meltfptr[15] = /*_._IFELSE___V18*/ meltfptr[17];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3420:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V17*/ meltfptr[16] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L4*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L5*/ meltfnum[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V18*/ meltfptr[17] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V16*/ meltfptr[15] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V16*/ meltfptr[15];;

        MELT_LOCATION("warmelt-outobj.melt:3408:/ clear");
        /*clear*/ /*_.OLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.COMS__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.COMSTR__V14*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V15*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V16*/ meltfptr[15] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3406:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3406:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJCOMMENTINSTR", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_72_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTINSTR*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK fromline 1752 */

    /** start of frame for meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK// fromline 1568
        : public Melt_CallFrameWithValues<27>
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
        MeltFrame_meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<27> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK), clos) {};
        MeltFrame_meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK() //the constructor fromline 1642
            : Melt_CallFrameWithValues<27> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<27> (fil,lin, sizeof(MeltFrame_meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK)) {};
        MeltFrame_meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<27> (fil,lin, sizeof(MeltFrame_meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK


    /** end of frame for meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK fromline 1697**/

    /* end of frame for routine meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK fromline 1756 */

    /* classy proc frame meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK */ MeltFrame_meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJCOMMENTEDBLOCK", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3427:/ getarg");
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
        MELT_LOCATION("warmelt-outobj.melt:3428:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJCOMMENTEDBLOCK*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3428:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3428:/ cond.else");

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
                            argtab[2].meltbp_long = 3428;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBLO__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3428:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3428:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3429:/ getslot");
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
        MELT_LOCATION("warmelt-outobj.melt:3430:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBLO_BODYL");
            /*_.BODYL__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3431:/ getslot");
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
        MELT_LOCATION("warmelt-outobj.melt:3433:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OCOMBLO_COMMENT");
            /*_.COMS__V14*/ meltfptr[13] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3434:/ quasiblock");


        /*_.SBU__V16*/ meltfptr[15] =
            (melt_ptr_t) meltgc_new_strbuf((meltobject_ptr_t)((/*!DISCR_STRBUF*/ meltfrout->tabval[4])), (const char*)0);;

        {
            MELT_LOCATION("warmelt-outobj.melt:3435:/ locexp");
            meltgc_add_strbuf_ccomment((melt_ptr_t)(/*_.SBU__V16*/ meltfptr[15]),
                                       melt_string_str((melt_ptr_t)(/*_.COMS__V14*/ meltfptr[13])));
        }
        ;
        /*_.STRBUF2STRING__V17*/ meltfptr[16] =
            (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_STRING*/ meltfrout->tabval[5])), melt_strbuf_str((melt_ptr_t)(/*_.SBU__V16*/ meltfptr[15]))));;
        /*^compute*/
        /*_.LET___V15*/ meltfptr[14] = /*_.STRBUF2STRING__V17*/ meltfptr[16];;

        MELT_LOCATION("warmelt-outobj.melt:3434:/ clear");
        /*clear*/ /*_.SBU__V16*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.STRBUF2STRING__V17*/ meltfptr[16] = 0 ;
        /*_.COMSTR__V18*/ meltfptr[15] = /*_.LET___V15*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3439:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*com.block:"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3440:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.COMSTR__V18*/ meltfptr[15])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3441:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*/{"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_LIST__L4*/ meltfnum[3] =
            (melt_magic_discr((melt_ptr_t)(/*_.BODYL__V11*/ meltfptr[10])) == MELTOBMAG_LIST);;
        MELT_LOCATION("warmelt-outobj.melt:3442:/ cond");
        /*cond*/ if (/*_#IS_LIST__L4*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3443:/ apply");
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
                        /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V20*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_CODE_INSTRUCTIONS_LIST*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.BODYL__V11*/ meltfptr[10]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V19*/ meltfptr[16] = /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V20*/ meltfptr[19];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3442:/ clear");
                    /*clear*/ /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V20*/ meltfptr[19] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V19*/ meltfptr[16] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_LIST__L5*/ meltfnum[4] =
            (melt_magic_discr((melt_ptr_t)(/*_.EPIL__V12*/ meltfptr[11])) == MELTOBMAG_LIST);;
        MELT_LOCATION("warmelt-outobj.melt:3444:/ cond");
        /*cond*/ if (/*_#IS_LIST__L5*/ meltfnum[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_#GET_INT__L6*/ meltfnum[5] =
                        (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V13*/ meltfptr[12])));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3445:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L6*/ meltfnum[5]), 0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3446:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*comp.epilog:"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3447:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                              melt_string_str((melt_ptr_t)(/*_.COMSTR__V18*/ meltfptr[15])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3448:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*/"));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3449:/ apply");
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
                        /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V22*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_CODE_INSTRUCTIONS_LIST*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.EPIL__V12*/ meltfptr[11]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3444:/ quasiblock");


                    /*_.PROGN___V23*/ meltfptr[22] = /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V22*/ meltfptr[21];;
                    /*^compute*/
                    /*_._IF___V21*/ meltfptr[19] = /*_.PROGN___V23*/ meltfptr[22];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3444:/ clear");
                    /*clear*/ /*_#GET_INT__L6*/ meltfnum[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_CODE_INSTRUCTIONS_LIST__V22*/ meltfptr[21] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V23*/ meltfptr[22] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V21*/ meltfptr[19] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3450:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "}"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3451:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*com.end block:"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3452:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.COMSTR__V18*/ meltfptr[15])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3453:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3454:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3455:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[5] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3456:/ cond");
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
                        /*_.REFERENCED_VALUE__V25*/ meltfptr[22] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V25*/ meltfptr[22] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L8*/ meltfnum[7] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V25*/ meltfptr[22])));;
            /*^compute*/
            /*_#ltI__L9*/ meltfnum[8] =
                ((/*_#STRBUF_USEDLENGTH__L7*/ meltfnum[5]) < (/*_#GET_INT__L8*/ meltfnum[7]));;
            MELT_LOCATION("warmelt-outobj.melt:3455:/ cond");
            /*cond*/ if (/*_#ltI__L9*/ meltfnum[8]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V26*/ meltfptr[25] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3455:/ cond.else");

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
                            argtab[2].meltbp_long = 3455;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V26*/ meltfptr[25] = /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3455:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[26] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V24*/ meltfptr[21] = /*_._IFELSE___V26*/ meltfptr[25];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3455:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V25*/ meltfptr[22] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L8*/ meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L9*/ meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V26*/ meltfptr[25] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V24*/ meltfptr[21] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V10*/ meltfptr[9] = /*_.IFCPP___V24*/ meltfptr[21];;

        MELT_LOCATION("warmelt-outobj.melt:3430:/ clear");
        /*clear*/ /*_.BODYL__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.EPIL__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BOXDEPTHP1__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.COMS__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.COMSTR__V18*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_LIST__L4*/ meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V19*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_LIST__L5*/ meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V21*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V24*/ meltfptr[21] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3427:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V10*/ meltfptr[9];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3427:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJCOMMENTEDBLOCK", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_73_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMMENTEDBLOCK*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR fromline 1752 */

    /** start of frame for meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR// fromline 1568
        : public Melt_CallFrameWithValues<24>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[11];
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
        MeltFrame_meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<24> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR), clos) {};
        MeltFrame_meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR() //the constructor fromline 1642
            : Melt_CallFrameWithValues<24> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<24> (fil,lin, sizeof(MeltFrame_meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR)) {};
        MeltFrame_meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<24> (fil,lin, sizeof(MeltFrame_meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR


    /** end of frame for meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR fromline 1697**/

    /* end of frame for routine meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR fromline 1756 */

    /* classy proc frame meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR */ MeltFrame_meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJLABELINSTR", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3465:/ getarg");
    /*_.OBLAB__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3466:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:3466:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:3466:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3466;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "outpucod_objlabelinstr oblab=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.OBLAB__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3466:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:3466:/ locexp");
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
        MELT_LOCATION("warmelt-outobj.melt:3467:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[2] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBLAB__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJLABELINSTR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:3467:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3467:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oblab";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3467;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBLAB__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3467:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3467:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3468:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3469:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*objlabel*/ "));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:3470:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBLAB__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBLAB_PREFIX");
            /*_.OPREFIX__V10*/ meltfptr[8] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3471:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBLAB__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBLAB_RANK");
            /*_.OBRANK__V11*/ meltfptr[4] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3472:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!LABELCOUNTREF*/ meltfrout->tabval[3])),
                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)((/*!LABELCOUNTREF*/ meltfrout->tabval[3])) /*=obj*/;
                    melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                    /*_.REFERENCED_VALUE__V12*/ meltfptr[11] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.REFERENCED_VALUE__V12*/ meltfptr[11] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#GET_INT__L5*/ meltfnum[1] =
            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V12*/ meltfptr[11])));;
        /*^compute*/
        /*_#COUNT__L6*/ meltfnum[2] =
            ((1) + (/*_#GET_INT__L5*/ meltfnum[1]));;
        /*^compute*/
        /*_.BOX__V13*/ meltfptr[12] =
            /*full constboxing*/ /*boxing ctype_long*/ meltgc_new_int((meltobject_ptr_t) (((melt_ptr_t)(MELT_PREDEF(DISCR_CONSTANT_INTEGER)))), (/*_#COUNT__L6*/ meltfnum[2]));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3474:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.BOX__V13*/ meltfptr[12];
            /*_.SET_CONTENT__V14*/ meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!SET_CONTENT*/ meltfrout->tabval[5])), (melt_ptr_t)((/*!LABELCOUNTREF*/ meltfrout->tabval[3])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3475:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.OPREFIX__V10*/ meltfptr[8])));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3476:/ cond");
        /*cond*/ if (/*_.OBRANK__V11*/ meltfptr[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_#GET_INT__L7*/ meltfnum[6] =
                        (melt_get_int((melt_ptr_t)(/*_.OBRANK__V11*/ meltfptr[4])));;



                    {
                        MELT_LOCATION("warmelt-outobj.melt:3477:/ locexp");
                        meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L7*/ meltfnum[6]));
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V15*/ meltfptr[14] = 0 ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3476:/ clear");
                    /*clear*/ /*_#GET_INT__L7*/ meltfnum[6] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {

                    /*_#OBJ_HASH__L8*/ meltfnum[6] =
                        (melt_obj_hash((melt_ptr_t)(/*_.OBLAB__V2*/ meltfptr[1])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3478:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[2];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "_h";
                        /*^apply.arg*/
                        argtab[1].meltbp_long = /*_#OBJ_HASH__L8*/ meltfnum[6];
                        /*_.STRING4OUT__V16*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!STRING4OUT*/ meltfrout->tabval[6])), (melt_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[7])), (MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*^compute*/
                    /*_.OBRANK__V11*/ meltfptr[4] = /*_.SETQ___V17*/ meltfptr[16] = /*_.STRING4OUT__V16*/ meltfptr[15];;
                    /*_._IFELSE___V15*/ meltfptr[14] = /*_.SETQ___V17*/ meltfptr[16];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3476:/ clear");
                    /*clear*/ /*_#OBJ_HASH__L8*/ meltfnum[6] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.STRING4OUT__V16*/ meltfptr[15] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V17*/ meltfptr[16] = 0 ;
                }
                ;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3480:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ": ;"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3481:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3482:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L9*/ meltfnum[6] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3483:/ cond");
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[8])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[4])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[8])) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                        /*_.REFERENCED_VALUE__V19*/ meltfptr[16] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V19*/ meltfptr[16] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L10*/ meltfnum[9] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V19*/ meltfptr[16])));;
            /*^compute*/
            /*_#ltI__L11*/ meltfnum[10] =
                ((/*_#STRBUF_USEDLENGTH__L9*/ meltfnum[6]) < (/*_#GET_INT__L10*/ meltfnum[9]));;
            MELT_LOCATION("warmelt-outobj.melt:3482:/ cond");
            /*cond*/ if (/*_#ltI__L11*/ meltfnum[10]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V20*/ meltfptr[19] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3482:/ cond.else");

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
                            argtab[2].meltbp_long = 3482;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V21*/ meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V20*/ meltfptr[19] = /*_.MELT_ASSERT_FAILURE_FUN__V21*/ meltfptr[20];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3482:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V21*/ meltfptr[20] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V18*/ meltfptr[15] = /*_._IFELSE___V20*/ meltfptr[19];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3482:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L9*/ meltfnum[6] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V19*/ meltfptr[16] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L10*/ meltfnum[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L11*/ meltfnum[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V20*/ meltfptr[19] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V18*/ meltfptr[15] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3484:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBLAB__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.OBI_LOC__V22*/ meltfptr[20] = slot;
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
            argtab[2].meltbp_cstring =  "objlabel";
            /*_.OUTPUT_LOCATION__V23*/ meltfptr[16] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.OBI_LOC__V22*/ meltfptr[20]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3485:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[14];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  " /* objlabel */\n     #if MELTDEBUG_MATCHING\
\n     static long meltlab_count_";
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#COUNT__L6*/ meltfnum[2];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  ";   \n     meltlab_count_";
            /*^apply.arg*/
            argtab[3].meltbp_long = /*_#COUNT__L6*/ meltfnum[2];
            /*^apply.arg*/
            argtab[4].meltbp_cstring =  "++;\n     debugeprintf(\"objlabel_";
            /*^apply.arg*/
            argtab[5].meltbp_long = /*_#COUNT__L6*/ meltfnum[2];
            /*^apply.arg*/
            argtab[6].meltbp_cstring =  " ";
            /*^apply.arg*/
            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.OPREFIX__V10*/ meltfptr[8];
            /*^apply.arg*/
            argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.OBRANK__V11*/ meltfptr[4];
            /*^apply.arg*/
            argtab[9].meltbp_cstring =  " ";
            /*^apply.arg*/
            argtab[10].meltbp_aptr = (melt_ptr_t*) &/*_.OBLAB__V2*/ meltfptr[1];
            /*^apply.arg*/
            argtab[11].meltbp_cstring =  " %ld\", meltlab_count_";
            /*^apply.arg*/
            argtab[12].meltbp_long = /*_#COUNT__L6*/ meltfnum[2];
            /*^apply.arg*/
            argtab[13].meltbp_cstring =  ");\n     #endif\n     ";
            /*_.ADD2OUT__V24*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[10])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3493:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_LOCATION("warmelt-outobj.melt:3470:/ clear");
        /*clear*/ /*_.OPREFIX__V10*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OBRANK__V11*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REFERENCED_VALUE__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L5*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_#COUNT__L6*/ meltfnum[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BOX__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SET_CONTENT__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V18*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OBI_LOC__V22*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V23*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V24*/ meltfptr[19] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:3465:/ clear");
        /*clear*/ /*_.IFCPP___V7*/ meltfptr[5] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJLABELINSTR", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_74_WARMELTmiOUTOBJ_OUTPUCOD_OBJLABELINSTR*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR fromline 1752 */

    /** start of frame for meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR// fromline 1568
        : public Melt_CallFrameWithValues<15>
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
        MeltFrame_meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<15> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR), clos) {};
        MeltFrame_meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR() //the constructor fromline 1642
            : Melt_CallFrameWithValues<15> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<15> (fil,lin, sizeof(MeltFrame_meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR)) {};
        MeltFrame_meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<15> (fil,lin, sizeof(MeltFrame_meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR


    /** end of frame for meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR fromline 1697**/

    /* end of frame for routine meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR fromline 1756 */

    /* classy proc frame meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR */ MeltFrame_meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJGOTOINSTR", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3500:/ getarg");
    /*_.OBGOTO__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3501:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBGOTO__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJGOTOINSTR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3501:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3501:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check obgoto";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3501;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBGOTO__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3501:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3501:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3502:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBGOTO__V2*/ meltfptr[1]) /*=obj*/;
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
            argtab[2].meltbp_cstring =  "objgoto";
            /*_.OUTPUT_LOCATION__V9*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.OBI_LOC__V8*/ meltfptr[6]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3503:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*objgoto*/ goto "));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:3504:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBGOTO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBGOTO_PREFIX");
            /*_.OBGOTO_PREFIX__V10*/ meltfptr[9] = slot;
        };
        ;

        {
            /*^locexp*/
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.OBGOTO_PREFIX__V10*/ meltfptr[9])));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:3505:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBGOTO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBGOTO_RANK");
            /*_.OBRANK__V11*/ meltfptr[10] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3506:/ cond");
        /*cond*/ if (/*_.OBRANK__V11*/ meltfptr[10]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_#GET_INT__L3*/ meltfnum[1] =
                        (melt_get_int((melt_ptr_t)(/*_.OBRANK__V11*/ meltfptr[10])));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3507:/ locexp");
                        meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L3*/ meltfnum[1]));
                    }
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3506:/ clear");
                    /*clear*/ /*_#GET_INT__L3*/ meltfnum[1] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_LOCATION("warmelt-outobj.melt:3505:/ clear");
        /*clear*/ /*_.OBRANK__V11*/ meltfptr[10] = 0 ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3508:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3509:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3510:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L4*/ meltfnum[1] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3511:/ cond");
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
                        /*_.REFERENCED_VALUE__V13*/ meltfptr[12] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V13*/ meltfptr[12] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L5*/ meltfnum[4] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V13*/ meltfptr[12])));;
            /*^compute*/
            /*_#ltI__L6*/ meltfnum[5] =
                ((/*_#STRBUF_USEDLENGTH__L4*/ meltfnum[1]) < (/*_#GET_INT__L5*/ meltfnum[4]));;
            MELT_LOCATION("warmelt-outobj.melt:3510:/ cond");
            /*cond*/ if (/*_#ltI__L6*/ meltfnum[5]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V14*/ meltfptr[13] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3510:/ cond.else");

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
                            argtab[2].meltbp_long = 3510;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V14*/ meltfptr[13] = /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3510:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V12*/ meltfptr[10] = /*_._IFELSE___V14*/ meltfptr[13];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3510:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L4*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V13*/ meltfptr[12] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L5*/ meltfnum[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L6*/ meltfnum[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V14*/ meltfptr[13] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V12*/ meltfptr[10] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3500:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.IFCPP___V12*/ meltfptr[10];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3500:/ locexp");
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
        /*clear*/ /*_.OBGOTO_PREFIX__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V12*/ meltfptr[10] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("OUTPUCOD_OBJGOTOINSTR", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_75_WARMELTmiOUTOBJ_OUTPUCOD_OBJGOTOINSTR*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM fromline 1752 */

    /** start of frame for meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM// fromline 1568
        : public Melt_CallFrameWithValues<16>
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
        MeltFrame_meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<16> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM), clos) {};
        MeltFrame_meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM() //the constructor fromline 1642
            : Melt_CallFrameWithValues<16> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<16> (fil,lin, sizeof(MeltFrame_meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM)) {};
        MeltFrame_meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<16> (fil,lin, sizeof(MeltFrame_meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM


    /** end of frame for meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM fromline 1697**/

    /* end of frame for routine meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM fromline 1756 */

    /* classy proc frame meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM */ MeltFrame_meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("ADD2SBUF_CLONSYM", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3517:/ getarg");
    /*_.SBUF__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.CSY__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.CSY__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3518:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRBUF__L1*/ meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.SBUF__V2*/ meltfptr[1])) == MELTOBMAG_STRBUF);;
            MELT_LOCATION("warmelt-outobj.melt:3518:/ cond");
            /*cond*/ if (/*_#IS_STRBUF__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V5*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3518:/ cond.else");

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
                            argtab[2].meltbp_long = 3518;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V5*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3518:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V4*/ meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3518:/ clear");
            /*clear*/ /*_#IS_STRBUF__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V5*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V4*/ meltfptr[3] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3519:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.CSY__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_CLONED_SYMBOL*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:3519:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3519:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check csy";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3519;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CSY__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3519:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3519:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3520:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.CSY__V3*/ meltfptr[2]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
            /*_.CNAM__V11*/ meltfptr[4] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3521:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.CSY__V3*/ meltfptr[2]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "CSYM_URANK");
            /*_.CSYM_URANK__V12*/ meltfptr[11] = slot;
        };
        ;
        /*_#RK__L3*/ meltfnum[0] =
            (melt_get_int((melt_ptr_t)(/*_.CSYM_URANK__V12*/ meltfptr[11])));;

        {
            MELT_LOCATION("warmelt-outobj.melt:3522:/ locexp");
            meltgc_add_strbuf_cident((melt_ptr_t)(/*_.SBUF__V2*/ meltfptr[1]),
                                     melt_string_str((melt_ptr_t)(/*_.CNAM__V11*/ meltfptr[4])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3523:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V2*/ meltfptr[1]), ( "_"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3524:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.SBUF__V2*/ meltfptr[1]), (/*_#RK__L3*/ meltfnum[0]));
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3525:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L4*/ meltfnum[3] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.SBUF__V2*/ meltfptr[1]));;
            MELT_LOCATION("warmelt-outobj.melt:3526:/ cond");
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[2])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[3])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[2])) /*=obj*/;
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
            /*_#GET_INT__L5*/ meltfnum[4] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V14*/ meltfptr[13])));;
            /*^compute*/
            /*_#ltI__L6*/ meltfnum[5] =
                ((/*_#STRBUF_USEDLENGTH__L4*/ meltfnum[3]) < (/*_#GET_INT__L5*/ meltfnum[4]));;
            MELT_LOCATION("warmelt-outobj.melt:3525:/ cond");
            /*cond*/ if (/*_#ltI__L6*/ meltfnum[5]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3525:/ cond.else");

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
                            argtab[2].meltbp_long = 3525;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/ meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3525:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V13*/ meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3525:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L4*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V14*/ meltfptr[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L5*/ meltfnum[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L6*/ meltfnum[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/ meltfptr[14] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V13*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V10*/ meltfptr[8] = /*_.IFCPP___V13*/ meltfptr[12];;

        MELT_LOCATION("warmelt-outobj.melt:3520:/ clear");
        /*clear*/ /*_.CNAM__V11*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CSYM_URANK__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_#RK__L3*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V13*/ meltfptr[12] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3517:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V10*/ meltfptr[8];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3517:/ locexp");
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
        /*clear*/ /*_.IFCPP___V4*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V7*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V10*/ meltfptr[8] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("ADD2SBUF_CLONSYM", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_76_WARMELTmiOUTOBJ_ADD2SBUF_CLONSYM*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP fromline 1752 */

    /** start of frame for meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP// fromline 1568
        : public Melt_CallFrameWithValues<38>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[24];
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
        MeltFrame_meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<38> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP), clos) {};
        MeltFrame_meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP() //the constructor fromline 1642
            : Melt_CallFrameWithValues<38> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<38> (fil,lin, sizeof(MeltFrame_meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP)) {};
        MeltFrame_meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<38> (fil,lin, sizeof(MeltFrame_meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP


    /** end of frame for meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP fromline 1697**/

    /* end of frame for routine meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP fromline 1756 */

    /* classy proc frame meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP */ MeltFrame_meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJLOOP", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3530:/ getarg");
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
        MELT_LOCATION("warmelt-outobj.melt:3531:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJLOOP*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3531:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3531:/ cond.else");

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
                            argtab[2].meltbp_long = 3531;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBLO__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3531:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3531:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3532:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBLO_BODYL");
            /*_.BODYL__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3533:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBLO_EPIL");
            /*_.EPIL__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3534:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OBLOOP_LABEL");
            /*_.LAB__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3535:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBLO__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.OLOC__V12*/ meltfptr[11] = slot;
        };
        ;
        /*_#plI__L3*/ meltfnum[1] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;
        /*^compute*/
        /*_.BOXDEPTHP1__V13*/ meltfptr[12] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[2])), (/*_#plI__L3*/ meltfnum[1])));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3538:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[3] =
                melt_is_instance_of((melt_ptr_t)(/*_.LAB__V11*/ meltfptr[10]), (melt_ptr_t)((/*!CLASS_CLONED_SYMBOL*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-outobj.melt:3538:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3538:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check lab";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3538;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.LAB__V11*/ meltfptr[10];
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/ meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3538:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/ meltfptr[13] = /*_._IFELSE___V15*/ meltfptr[14];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3538:/ clear");
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
        MELT_LOCATION("warmelt-outobj.melt:3539:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "loop";
            /*_.OUTPUT_LOCATION__V17*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.OLOC__V12*/ meltfptr[11]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3540:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*loop*/{ meltlabloop_"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3541:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LAB__V11*/ meltfptr[10];
            /*_.ADD2SBUF_CLONSYM__V18*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2SBUF_CLONSYM*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3542:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ":;"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_LIST__L5*/ meltfnum[3] =
            (melt_magic_discr((melt_ptr_t)(/*_.BODYL__V9*/ meltfptr[5])) == MELTOBMAG_LIST);;
        MELT_LOCATION("warmelt-outobj.melt:3543:/ cond");
        /*cond*/ if (/*_#IS_LIST__L5*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3544:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                        /*^apply.arg*/
                        argtab[2].meltbp_cstring =  "loopbody";
                        /*_.OUTPUT_LOCATION__V19*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.OLOC__V12*/ meltfptr[11]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_#plI__L6*/ meltfnum[5] =
                        ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3545:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L6*/ meltfnum[5]), 0);
                    }
                    ;
                    /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
                    {
                        /* start foreach_pair_component_in_list meltcit1__EACHLIST */
                        for (/*_.CURPAIR__V20*/ meltfptr[19] = melt_list_first( (melt_ptr_t)/*_.BODYL__V9*/ meltfptr[5]);
                                                melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V20*/ meltfptr[19]) == MELTOBMAG_PAIR;
                                                /*_.CURPAIR__V20*/ meltfptr[19] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V20*/ meltfptr[19]))
                            {
                                /*_.CURBODY__V21*/ meltfptr[20] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V20*/ meltfptr[19]);


                                MELT_LOCATION("warmelt-outobj.melt:3549:/ quasiblock");


                                /*_#DEPTHP1__L7*/ meltfnum[6] =
                                    (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V13*/ meltfptr[12])));;

                                MELT_CHECK_SIGNAL();
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:3550:/ cond");
                                /*cond*/ if (/*_.CURBODY__V21*/ meltfptr[20]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            /*_#IS_A__L9*/ meltfnum[8] =
                                                melt_is_instance_of((melt_ptr_t)(/*_.CURBODY__V21*/ meltfptr[20]), (melt_ptr_t)((/*!CLASS_OBJPUREVALUE*/ meltfrout->tabval[6])));;
                                            /*^compute*/
                                            /*_#NOT__L10*/ meltfnum[9] =
                                                (!(/*_#IS_A__L9*/ meltfnum[8]));;
                                            /*^compute*/
                                            /*_#_IF___L8*/ meltfnum[7] = /*_#NOT__L10*/ meltfnum[9];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:3550:/ clear");
                                            /*clear*/ /*_#IS_A__L9*/ meltfnum[8] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_#NOT__L10*/ meltfnum[9] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_#_IF___L8*/ meltfnum[7] = 0;;
                                    }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:3550:/ cond");
                                /*cond*/ if (/*_#_IF___L8*/ meltfnum[7]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-outobj.melt:3551:/ msend");
                                            /*msend*/
                                            {
                                                union meltparam_un argtab[3];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^ojbmsend.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                                /*^ojbmsend.arg*/
                                                argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                /*^ojbmsend.arg*/
                                                argtab[2].meltbp_long = /*_#DEPTHP1__L7*/ meltfnum[6];
                                                /*_.OUTPUT_C_CODE__V23*/ meltfptr[22] = meltgc_send((melt_ptr_t)(/*_.CURBODY__V21*/ meltfptr[20]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[7])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                            /*_._IF___V22*/ meltfptr[21] = /*_.OUTPUT_C_CODE__V23*/ meltfptr[22];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:3550:/ clear");
                                            /*clear*/ /*_.OUTPUT_C_CODE__V23*/ meltfptr[22] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_._IF___V22*/ meltfptr[21] =  /*reallynil*/ NULL ;;
                                    }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:3552:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:3553:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTHP1__L7*/ meltfnum[6]), 0);
                                }
                                ;

                                MELT_LOCATION("warmelt-outobj.melt:3549:/ clear");
                                /*clear*/ /*_#DEPTHP1__L7*/ meltfnum[6] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#_IF___L8*/ meltfnum[7] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IF___V22*/ meltfptr[21] = 0 ;
                            } /* end foreach_pair_component_in_list meltcit1__EACHLIST */
                        /*_.CURPAIR__V20*/ meltfptr[19] = NULL;
                        /*_.CURBODY__V21*/ meltfptr[20] = NULL;


                        /*citerepilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3546:/ clear");
                        /*clear*/ /*_.CURPAIR__V20*/ meltfptr[19] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.CURBODY__V21*/ meltfptr[20] = 0 ;
                    } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3543:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_LOCATION__V19*/ meltfptr[18] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#plI__L6*/ meltfnum[5] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3554:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
        }
        ;
        /*_#plI__L11*/ meltfnum[8] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        {
            MELT_LOCATION("warmelt-outobj.melt:3555:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L11*/ meltfnum[8]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3556:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " goto meltlabloop_"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3557:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LAB__V11*/ meltfptr[10];
            /*_.ADD2SBUF_CLONSYM__V24*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2SBUF_CLONSYM*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3558:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
        }
        ;
        /*_#plI__L12*/ meltfnum[9] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        {
            MELT_LOCATION("warmelt-outobj.melt:3559:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L12*/ meltfnum[9]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3560:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " meltlabexit_"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3561:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LAB__V11*/ meltfptr[10];
            /*_.ADD2SBUF_CLONSYM__V25*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2SBUF_CLONSYM*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3562:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ":;"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_LIST__L13*/ meltfnum[6] =
            (melt_magic_discr((melt_ptr_t)(/*_.EPIL__V10*/ meltfptr[9])) == MELTOBMAG_LIST);;
        MELT_LOCATION("warmelt-outobj.melt:3563:/ cond");
        /*cond*/ if (/*_#IS_LIST__L13*/ meltfnum[6]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3564:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                        /*^apply.arg*/
                        argtab[2].meltbp_cstring =  "loopepilog";
                        /*_.OUTPUT_LOCATION__V26*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.OLOC__V12*/ meltfptr[11]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3565:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*loopepilog*/"));
                    }
                    ;
                    /*_#plI__L14*/ meltfnum[7] =
                        ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3566:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L14*/ meltfnum[7]), 0);
                    }
                    ;
                    /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
                    {
                        /* start foreach_pair_component_in_list meltcit2__EACHLIST */
                        for (/*_.CURPAIR__V27*/ meltfptr[26] = melt_list_first( (melt_ptr_t)/*_.EPIL__V10*/ meltfptr[9]);
                                                melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V27*/ meltfptr[26]) == MELTOBMAG_PAIR;
                                                /*_.CURPAIR__V27*/ meltfptr[26] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V27*/ meltfptr[26]))
                            {
                                /*_.CUREPIL__V28*/ meltfptr[27] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V27*/ meltfptr[26]);


                                MELT_LOCATION("warmelt-outobj.melt:3570:/ quasiblock");


                                /*_#DEPTHP1__L15*/ meltfnum[5] =
                                    (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V13*/ meltfptr[12])));;

                                MELT_CHECK_SIGNAL();
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:3571:/ cond");
                                /*cond*/ if (/*_.CUREPIL__V28*/ meltfptr[27]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            /*_#IS_A__L17*/ meltfnum[16] =
                                                melt_is_instance_of((melt_ptr_t)(/*_.CUREPIL__V28*/ meltfptr[27]), (melt_ptr_t)((/*!CLASS_OBJPUREVALUE*/ meltfrout->tabval[6])));;
                                            /*^compute*/
                                            /*_#NOT__L18*/ meltfnum[17] =
                                                (!(/*_#IS_A__L17*/ meltfnum[16]));;
                                            /*^compute*/
                                            /*_#_IF___L16*/ meltfnum[15] = /*_#NOT__L18*/ meltfnum[17];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:3571:/ clear");
                                            /*clear*/ /*_#IS_A__L17*/ meltfnum[16] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_#NOT__L18*/ meltfnum[17] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_#_IF___L16*/ meltfnum[15] = 0;;
                                    }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:3571:/ cond");
                                /*cond*/ if (/*_#_IF___L16*/ meltfnum[15]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-outobj.melt:3572:/ msend");
                                            /*msend*/
                                            {
                                                union meltparam_un argtab[3];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^ojbmsend.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                                /*^ojbmsend.arg*/
                                                argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                /*^ojbmsend.arg*/
                                                argtab[2].meltbp_long = /*_#DEPTHP1__L15*/ meltfnum[5];
                                                /*_.OUTPUT_C_CODE__V30*/ meltfptr[29] = meltgc_send((melt_ptr_t)(/*_.CUREPIL__V28*/ meltfptr[27]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[7])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                            /*_._IF___V29*/ meltfptr[28] = /*_.OUTPUT_C_CODE__V30*/ meltfptr[29];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:3571:/ clear");
                                            /*clear*/ /*_.OUTPUT_C_CODE__V30*/ meltfptr[29] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_._IF___V29*/ meltfptr[28] =  /*reallynil*/ NULL ;;
                                    }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:3573:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
                                }
                                ;

#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:3574:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*_#STRBUF_USEDLENGTH__L19*/ meltfnum[16] =
                                        melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                                    MELT_LOCATION("warmelt-outobj.melt:3575:/ cond");
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
                                    /*_#GET_INT__L20*/ meltfnum[17] =
                                        (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V32*/ meltfptr[31])));;
                                    /*^compute*/
                                    /*_#ltI__L21*/ meltfnum[20] =
                                        ((/*_#STRBUF_USEDLENGTH__L19*/ meltfnum[16]) < (/*_#GET_INT__L20*/ meltfnum[17]));;
                                    MELT_LOCATION("warmelt-outobj.melt:3574:/ cond");
                                    /*cond*/ if (/*_#ltI__L21*/ meltfnum[20]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*_._IFELSE___V33*/ meltfptr[32] = (/*nil*/NULL);;
                                        }
                                    else
                                        {
                                            MELT_LOCATION("warmelt-outobj.melt:3574:/ cond.else");

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
                                                    argtab[2].meltbp_long = 3574;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                    /*_.MELT_ASSERT_FAILURE_FUN__V34*/ meltfptr[33] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IFELSE___V33*/ meltfptr[32] = /*_.MELT_ASSERT_FAILURE_FUN__V34*/ meltfptr[33];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:3574:/ clear");
                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V34*/ meltfptr[33] = 0 ;
                                            }
                                            ;
                                        }
                                    ;
                                    /*_.IFCPP___V31*/ meltfptr[29] = /*_._IFELSE___V33*/ meltfptr[32];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:3574:/ clear");
                                    /*clear*/ /*_#STRBUF_USEDLENGTH__L19*/ meltfnum[16] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.REFERENCED_VALUE__V32*/ meltfptr[31] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#GET_INT__L20*/ meltfnum[17] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#ltI__L21*/ meltfnum[20] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IFELSE___V33*/ meltfptr[32] = 0 ;
                                }

#else /*MELT_HAVE_DEBUG*/
                                /*^cppif.else*/
                                /*_.IFCPP___V31*/ meltfptr[29] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:3576:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTHP1__L15*/ meltfnum[5]), 0);
                                }
                                ;

                                MELT_LOCATION("warmelt-outobj.melt:3570:/ clear");
                                /*clear*/ /*_#DEPTHP1__L15*/ meltfnum[5] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#_IF___L16*/ meltfnum[15] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IF___V29*/ meltfptr[28] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.IFCPP___V31*/ meltfptr[29] = 0 ;
                            } /* end foreach_pair_component_in_list meltcit2__EACHLIST */
                        /*_.CURPAIR__V27*/ meltfptr[26] = NULL;
                        /*_.CUREPIL__V28*/ meltfptr[27] = NULL;


                        /*citerepilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3567:/ clear");
                        /*clear*/ /*_.CURPAIR__V27*/ meltfptr[26] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.CUREPIL__V28*/ meltfptr[27] = 0 ;
                    } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3563:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_LOCATION__V26*/ meltfptr[18] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#plI__L14*/ meltfnum[7] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3577:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "}"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3578:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3579:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L22*/ meltfnum[16] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3580:/ cond");
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
                        /*_.REFERENCED_VALUE__V36*/ meltfptr[31] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V36*/ meltfptr[31] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L23*/ meltfnum[17] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V36*/ meltfptr[31])));;
            /*^compute*/
            /*_#ltI__L24*/ meltfnum[20] =
                ((/*_#STRBUF_USEDLENGTH__L22*/ meltfnum[16]) < (/*_#GET_INT__L23*/ meltfnum[17]));;
            MELT_LOCATION("warmelt-outobj.melt:3579:/ cond");
            /*cond*/ if (/*_#ltI__L24*/ meltfnum[20]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V37*/ meltfptr[32] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3579:/ cond.else");

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
                            argtab[2].meltbp_long = 3579;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V38*/ meltfptr[28] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V37*/ meltfptr[32] = /*_.MELT_ASSERT_FAILURE_FUN__V38*/ meltfptr[28];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3579:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V38*/ meltfptr[28] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V35*/ meltfptr[33] = /*_._IFELSE___V37*/ meltfptr[32];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3579:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L22*/ meltfnum[16] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V36*/ meltfptr[31] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L23*/ meltfnum[17] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L24*/ meltfnum[20] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V37*/ meltfptr[32] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V35*/ meltfptr[33] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V35*/ meltfptr[33];;

        MELT_LOCATION("warmelt-outobj.melt:3532:/ clear");
        /*clear*/ /*_.BODYL__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.EPIL__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LAB__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OLOC__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BOXDEPTHP1__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V17*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2SBUF_CLONSYM__V18*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_LIST__L5*/ meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L11*/ meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2SBUF_CLONSYM__V24*/ meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L12*/ meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2SBUF_CLONSYM__V25*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_LIST__L13*/ meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V35*/ meltfptr[33] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3530:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3530:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJLOOP", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_77_WARMELTmiOUTOBJ_OUTPUCOD_OBJLOOP*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT fromline 1752 */

    /** start of frame for meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT// fromline 1568
        : public Melt_CallFrameWithValues<19>
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
        MeltFrame_meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<19> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT), clos) {};
        MeltFrame_meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT() //the constructor fromline 1642
            : Melt_CallFrameWithValues<19> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT)) {};
        MeltFrame_meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT


    /** end of frame for meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT fromline 1697**/

    /* end of frame for routine meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT fromline 1756 */

    /* classy proc frame meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT */ MeltFrame_meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJEXIT", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3587:/ getarg");
    /*_.OBXI__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3588:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBXI__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJEXIT*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3588:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3588:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check obxi";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3588;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBXI__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3588:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3588:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3589:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBXI__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBEXIT_LABEL");
            /*_.OLAB__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3590:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBXI__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.LOC__V10*/ meltfptr[9] = slot;
        };
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3592:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OLAB__V9*/ meltfptr[5]), (melt_ptr_t)((/*!CLASS_CLONED_SYMBOL*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-outobj.melt:3592:/ cond");
            /*cond*/ if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[11] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3592:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check olab";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3592;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OLAB__V9*/ meltfptr[5];
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/ meltfptr[11] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3592:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/ meltfptr[10] = /*_._IFELSE___V12*/ meltfptr[11];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3592:/ clear");
            /*clear*/ /*_#IS_A__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V12*/ meltfptr[11] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V11*/ meltfptr[10] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3593:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "exit";
            /*_.OUTPUT_LOCATION__V14*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.LOC__V10*/ meltfptr[9]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3594:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*exit*/{"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3595:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3596:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " goto meltlabexit_"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3597:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OLAB__V9*/ meltfptr[5];
            /*_.ADD2SBUF_CLONSYM__V15*/ meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2SBUF_CLONSYM*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3598:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";}"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3599:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3600:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L4*/ meltfnum[1] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3601:/ cond");
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
            /*_#GET_INT__L5*/ meltfnum[4] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V17*/ meltfptr[16])));;
            /*^compute*/
            /*_#ltI__L6*/ meltfnum[5] =
                ((/*_#STRBUF_USEDLENGTH__L4*/ meltfnum[1]) < (/*_#GET_INT__L5*/ meltfnum[4]));;
            MELT_LOCATION("warmelt-outobj.melt:3600:/ cond");
            /*cond*/ if (/*_#ltI__L6*/ meltfnum[5]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V18*/ meltfptr[17] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3600:/ cond.else");

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
                            argtab[2].meltbp_long = 3600;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V18*/ meltfptr[17] = /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3600:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V16*/ meltfptr[15] = /*_._IFELSE___V18*/ meltfptr[17];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3600:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L4*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V17*/ meltfptr[16] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L5*/ meltfnum[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L6*/ meltfnum[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V18*/ meltfptr[17] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V16*/ meltfptr[15] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V16*/ meltfptr[15];;

        MELT_LOCATION("warmelt-outobj.melt:3589:/ clear");
        /*clear*/ /*_.OLAB__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V14*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2SBUF_CLONSYM__V15*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V16*/ meltfptr[15] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3587:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3587:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJEXIT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_78_WARMELTmiOUTOBJ_OUTPUCOD_OBJEXIT*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN fromline 1752 */

    /** start of frame for meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN// fromline 1568
        : public Melt_CallFrameWithValues<19>
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
        MeltFrame_meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<19> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN), clos) {};
        MeltFrame_meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN() //the constructor fromline 1642
            : Melt_CallFrameWithValues<19> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN)) {};
        MeltFrame_meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN


    /** end of frame for meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN fromline 1697**/

    /* end of frame for routine meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN fromline 1756 */

    /* classy proc frame meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN */ MeltFrame_meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJAGAIN", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3607:/ getarg");
    /*_.OBAG__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3608:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBAG__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJAGAIN*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3608:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3608:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check obag";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3608;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBAG__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3608:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3608:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3609:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBAG__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBAGAIN_LABEL");
            /*_.OLAB__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3610:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBAG__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.LOC__V10*/ meltfptr[9] = slot;
        };
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3612:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OLAB__V9*/ meltfptr[5]), (melt_ptr_t)((/*!CLASS_CLONED_SYMBOL*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-outobj.melt:3612:/ cond");
            /*cond*/ if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[11] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3612:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check olab";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3612;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OLAB__V9*/ meltfptr[5];
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/ meltfptr[11] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3612:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/ meltfptr[10] = /*_._IFELSE___V12*/ meltfptr[11];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3612:/ clear");
            /*clear*/ /*_#IS_A__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V12*/ meltfptr[11] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V11*/ meltfptr[10] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3613:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "again";
            /*_.OUTPUT_LOCATION__V14*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.LOC__V10*/ meltfptr[9]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3614:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*again*/{"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3615:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3616:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " goto meltlabloop_"));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3617:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OLAB__V9*/ meltfptr[5];
            /*_.ADD2SBUF_CLONSYM__V15*/ meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2SBUF_CLONSYM*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3618:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";}"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3619:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3620:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L4*/ meltfnum[1] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3621:/ cond");
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
            /*_#GET_INT__L5*/ meltfnum[4] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V17*/ meltfptr[16])));;
            /*^compute*/
            /*_#ltI__L6*/ meltfnum[5] =
                ((/*_#STRBUF_USEDLENGTH__L4*/ meltfnum[1]) < (/*_#GET_INT__L5*/ meltfnum[4]));;
            MELT_LOCATION("warmelt-outobj.melt:3620:/ cond");
            /*cond*/ if (/*_#ltI__L6*/ meltfnum[5]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V18*/ meltfptr[17] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3620:/ cond.else");

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
                            argtab[2].meltbp_long = 3620;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V18*/ meltfptr[17] = /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3620:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V16*/ meltfptr[15] = /*_._IFELSE___V18*/ meltfptr[17];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3620:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L4*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V17*/ meltfptr[16] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L5*/ meltfnum[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L6*/ meltfnum[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V18*/ meltfptr[17] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V16*/ meltfptr[15] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V16*/ meltfptr[15];;

        MELT_LOCATION("warmelt-outobj.melt:3609:/ clear");
        /*clear*/ /*_.OLAB__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V14*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2SBUF_CLONSYM__V15*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V16*/ meltfptr[15] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3607:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3607:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJAGAIN", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_79_WARMELTmiOUTOBJ_OUTPUCOD_OBJAGAIN*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE fromline 1752 */

    /** start of frame for meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE// fromline 1568
        : public Melt_CallFrameWithValues<39>
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
        MeltFrame_meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<39> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE), clos) {};
        MeltFrame_meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE() //the constructor fromline 1642
            : Melt_CallFrameWithValues<39> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<39> (fil,lin, sizeof(MeltFrame_meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE)) {};
        MeltFrame_meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<39> (fil,lin, sizeof(MeltFrame_meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE


    /** end of frame for meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE fromline 1697**/

    /* end of frame for routine meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE fromline 1756 */

    /* classy proc frame meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE */ MeltFrame_meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJCOMPUTE", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3626:/ getarg");
    /*_.OBCOMP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3627:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OBCOMP__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJCOMPUTE*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3627:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3627:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check obcomp";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3627;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OBCOMP__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3627:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3627:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3628:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBCOMP__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBDI_DESTLIST");
            /*_.CDEST__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3629:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBCOMP__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.CLOC__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3630:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OBCOMP__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBCPT_EXPR");
            /*_.CEXP__V11*/ meltfptr[10] = slot;
        };
        ;
        /*_#plI__L3*/ meltfnum[1] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;
        /*^compute*/
        /*_.BOXDEPTHP1__V12*/ meltfptr[11] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[2])), (/*_#plI__L3*/ meltfnum[1])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3633:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "compute";
            /*_.OUTPUT_LOCATION__V13*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.CLOC__V10*/ meltfptr[9]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OBCOMP__V2*/ meltfptr[1];
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

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_LIST__L4*/ meltfnum[3] =
            (melt_magic_discr((melt_ptr_t)(/*_.CDEST__V9*/ meltfptr[5])) == MELTOBMAG_LIST);;
        MELT_LOCATION("warmelt-outobj.melt:3635:/ cond");
        /*cond*/ if (/*_#IS_LIST__L4*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
                    {
                        /* start foreach_pair_component_in_list meltcit1__EACHLIST */
                        for (/*_.CURPAIR__V16*/ meltfptr[14] = melt_list_first( (melt_ptr_t)/*_.CDEST__V9*/ meltfptr[5]);
                                                melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V16*/ meltfptr[14]) == MELTOBMAG_PAIR;
                                                /*_.CURPAIR__V16*/ meltfptr[14] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V16*/ meltfptr[14]))
                            {
                                /*_.DESTCUR__V17*/ meltfptr[16] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V16*/ meltfptr[14]);


                                /*_#GET_INT__L5*/ meltfnum[4] =
                                    (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V12*/ meltfptr[11])));;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:3639:/ msend");
                                /*msend*/
                                {
                                    union meltparam_un argtab[3];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^ojbmsend.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                    /*^ojbmsend.arg*/
                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                    /*^ojbmsend.arg*/
                                    argtab[2].meltbp_long = /*_#GET_INT__L5*/ meltfnum[4];
                                    /*_.OUTPUT_C_CODE__V18*/ meltfptr[17] = meltgc_send((melt_ptr_t)(/*_.DESTCUR__V17*/ meltfptr[16]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:3640:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = "));
                                }
                                ;
                            } /* end foreach_pair_component_in_list meltcit1__EACHLIST */
                        /*_.CURPAIR__V16*/ meltfptr[14] = NULL;
                        /*_.DESTCUR__V17*/ meltfptr[16] = NULL;


                        /*citerepilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3636:/ clear");
                        /*clear*/ /*_.CURPAIR__V16*/ meltfptr[14] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.DESTCUR__V17*/ meltfptr[16] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L5*/ meltfnum[4] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.OUTPUT_C_CODE__V18*/ meltfptr[17] = 0 ;
                    } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_LIST__L6*/ meltfnum[5] =
            (melt_magic_discr((melt_ptr_t)(/*_.CEXP__V11*/ meltfptr[10])) == MELTOBMAG_LIST);;
        MELT_LOCATION("warmelt-outobj.melt:3642:/ cond");
        /*cond*/ if (/*_#IS_LIST__L6*/ meltfnum[5]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#LIST_LENGTH__L7*/ meltfnum[6] =
                        (melt_list_length((melt_ptr_t)(/*_.CEXP__V11*/ meltfptr[10])));;
                    /*^compute*/
                    /*_#gtI__L8*/ meltfnum[7] =
                        ((/*_#LIST_LENGTH__L7*/ meltfnum[6]) > (2));;
                    MELT_LOCATION("warmelt-outobj.melt:3643:/ cond");
                    /*cond*/ if (/*_#gtI__L8*/ meltfnum[7]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*_#plI__L9*/ meltfnum[8] =
                                    ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:3644:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L9*/ meltfnum[8]), 0);
                                }
                                ;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:3643:/ clear");
                                /*clear*/ /*_#plI__L9*/ meltfnum[8] = 0 ;
                            }
                            ;
                        } /*noelse*/
                    ;
                    /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
                    {
                        /* start foreach_pair_component_in_list meltcit2__EACHLIST */
                        for (/*_.CURPAIR__V20*/ meltfptr[19] = melt_list_first( (melt_ptr_t)/*_.CEXP__V11*/ meltfptr[10]);
                                                melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V20*/ meltfptr[19]) == MELTOBMAG_PAIR;
                                                /*_.CURPAIR__V20*/ meltfptr[19] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V20*/ meltfptr[19]))
                            {
                                /*_.EXPCUR__V21*/ meltfptr[20] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V20*/ meltfptr[19]);


                                /*_#GET_INT__L10*/ meltfnum[8] =
                                    (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V12*/ meltfptr[11])));;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:3648:/ msend");
                                /*msend*/
                                {
                                    union meltparam_un argtab[3];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^ojbmsend.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                    /*^ojbmsend.arg*/
                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                    /*^ojbmsend.arg*/
                                    argtab[2].meltbp_long = /*_#GET_INT__L10*/ meltfnum[8];
                                    /*_.OUTPUT_C_CODE__V22*/ meltfptr[21] = meltgc_send((melt_ptr_t)(/*_.EXPCUR__V21*/ meltfptr[20]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:3649:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*_#STRBUF_USEDLENGTH__L11*/ meltfnum[10] =
                                        melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                                    MELT_LOCATION("warmelt-outobj.melt:3650:/ cond");
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
                                    /*_#GET_INT__L12*/ meltfnum[11] =
                                        (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V24*/ meltfptr[23])));;
                                    /*^compute*/
                                    /*_#ltI__L13*/ meltfnum[12] =
                                        ((/*_#STRBUF_USEDLENGTH__L11*/ meltfnum[10]) < (/*_#GET_INT__L12*/ meltfnum[11]));;
                                    MELT_LOCATION("warmelt-outobj.melt:3649:/ cond");
                                    /*cond*/ if (/*_#ltI__L13*/ meltfnum[12]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*_._IFELSE___V25*/ meltfptr[24] = (/*nil*/NULL);;
                                        }
                                    else
                                        {
                                            MELT_LOCATION("warmelt-outobj.melt:3649:/ cond.else");

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
                                                    argtab[0].meltbp_cstring =  "check limited implbuf";
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 3649;
                                                    /*_.MELT_ASSERT_FAILURE_FUN__V26*/ meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IFELSE___V25*/ meltfptr[24] = /*_.MELT_ASSERT_FAILURE_FUN__V26*/ meltfptr[25];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:3649:/ clear");
                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V26*/ meltfptr[25] = 0 ;
                                            }
                                            ;
                                        }
                                    ;
                                    /*_.IFCPP___V23*/ meltfptr[22] = /*_._IFELSE___V25*/ meltfptr[24];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:3649:/ clear");
                                    /*clear*/ /*_#STRBUF_USEDLENGTH__L11*/ meltfnum[10] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.REFERENCED_VALUE__V24*/ meltfptr[23] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#GET_INT__L12*/ meltfnum[11] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#ltI__L13*/ meltfnum[12] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IFELSE___V25*/ meltfptr[24] = 0 ;
                                }

#else /*MELT_HAVE_DEBUG*/
                                /*^cppif.else*/
                                /*_.IFCPP___V23*/ meltfptr[22] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                ;
                            } /* end foreach_pair_component_in_list meltcit2__EACHLIST */
                        /*_.CURPAIR__V20*/ meltfptr[19] = NULL;
                        /*_.EXPCUR__V21*/ meltfptr[20] = NULL;


                        /*citerepilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3645:/ clear");
                        /*clear*/ /*_.CURPAIR__V20*/ meltfptr[19] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.EXPCUR__V21*/ meltfptr[20] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L10*/ meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.OUTPUT_C_CODE__V22*/ meltfptr[21] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.IFCPP___V23*/ meltfptr[22] = 0 ;
                    } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3642:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_#LIST_LENGTH__L7*/ meltfnum[6] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#gtI__L8*/ meltfnum[7] = 0 ;
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
                    /*_#IS_MULTIPLE__L14*/ meltfnum[10] =
                        (melt_magic_discr((melt_ptr_t)(/*_.CEXP__V11*/ meltfptr[10])) == MELTOBMAG_MULTIPLE);;
                    MELT_LOCATION("warmelt-outobj.melt:3652:/ cond");
                    /*cond*/ if (/*_#IS_MULTIPLE__L14*/ meltfnum[10]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                /*_#MULTIPLE_LENGTH__L15*/ meltfnum[11] =
                                    (melt_multiple_length((melt_ptr_t)(/*_.CEXP__V11*/ meltfptr[10])));;
                                /*^compute*/
                                /*_#gtI__L16*/ meltfnum[12] =
                                    ((/*_#MULTIPLE_LENGTH__L15*/ meltfnum[11]) > (2));;
                                MELT_LOCATION("warmelt-outobj.melt:3653:/ cond");
                                /*cond*/ if (/*_#gtI__L16*/ meltfnum[12]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            /*_#plI__L17*/ meltfnum[6] =
                                                ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                                            {
                                                MELT_LOCATION("warmelt-outobj.melt:3654:/ locexp");
                                                meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L17*/ meltfnum[6]), 0);
                                            }
                                            ;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:3653:/ clear");
                                            /*clear*/ /*_#plI__L17*/ meltfnum[6] = 0 ;
                                        }
                                        ;
                                    } /*noelse*/
                                ;
                                /*citerblock FOREACH_IN_MULTIPLE*/
                                {
                                    /* start foreach_in_multiple meltcit3__EACHTUP */
                                    long  meltcit3__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.CEXP__V11*/ meltfptr[10]);
                                    for (/*_#IX__L18*/ meltfnum[7] = 0;
                                                       (/*_#IX__L18*/ meltfnum[7] >= 0) && (/*_#IX__L18*/ meltfnum[7] <  meltcit3__EACHTUP_ln);
                                                       /*_#IX__L18*/ meltfnum[7]++)
                                        {
                                            /*_.EXPCUR__V28*/ meltfptr[23] = melt_multiple_nth((melt_ptr_t)(/*_.CEXP__V11*/ meltfptr[10]),  /*_#IX__L18*/ meltfnum[7]);



                                            /*_#GET_INT__L19*/ meltfnum[6] =
                                                (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V12*/ meltfptr[11])));;

                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-outobj.melt:3658:/ msend");
                                            /*msend*/
                                            {
                                                union meltparam_un argtab[3];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^ojbmsend.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                                /*^ojbmsend.arg*/
                                                argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                /*^ojbmsend.arg*/
                                                argtab[2].meltbp_long = /*_#GET_INT__L19*/ meltfnum[6];
                                                /*_.OUTPUT_C_CODE__V29*/ meltfptr[24] = meltgc_send((melt_ptr_t)(/*_.EXPCUR__V28*/ meltfptr[23]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;

#if MELT_HAVE_DEBUG
                                            MELT_LOCATION("warmelt-outobj.melt:3659:/ cppif.then");
                                            /*^block*/
                                            /*anyblock*/
                                            {


                                                MELT_CHECK_SIGNAL();
                                                ;
                                                /*_#STRBUF_USEDLENGTH__L20*/ meltfnum[19] =
                                                    melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                                                MELT_LOCATION("warmelt-outobj.melt:3660:/ cond");
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
                                                            /*_.REFERENCED_VALUE__V31*/ meltfptr[30] = slot;
                                                        };
                                                        ;
                                                    }
                                                else    /*^cond.else*/
                                                    {

                                                        /*_.REFERENCED_VALUE__V31*/ meltfptr[30] =  /*reallynil*/ NULL ;;
                                                    }
                                                ;
                                                /*^compute*/
                                                /*_#GET_INT__L21*/ meltfnum[20] =
                                                    (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V31*/ meltfptr[30])));;
                                                /*^compute*/
                                                /*_#ltI__L22*/ meltfnum[21] =
                                                    ((/*_#STRBUF_USEDLENGTH__L20*/ meltfnum[19]) < (/*_#GET_INT__L21*/ meltfnum[20]));;
                                                MELT_LOCATION("warmelt-outobj.melt:3659:/ cond");
                                                /*cond*/ if (/*_#ltI__L22*/ meltfnum[21]) /*then*/
                                                    {
                                                        /*^cond.then*/
                                                        /*_._IFELSE___V32*/ meltfptr[31] = (/*nil*/NULL);;
                                                    }
                                                else
                                                    {
                                                        MELT_LOCATION("warmelt-outobj.melt:3659:/ cond.else");

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
                                                                argtab[2].meltbp_long = 3659;
                                                                /*^apply.arg*/
                                                                argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                                /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                            }
                                                            ;
                                                            /*_._IFELSE___V32*/ meltfptr[31] = /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[32];;
                                                            /*epilog*/

                                                            MELT_LOCATION("warmelt-outobj.melt:3659:/ clear");
                                                            /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[32] = 0 ;
                                                        }
                                                        ;
                                                    }
                                                ;
                                                /*_.IFCPP___V30*/ meltfptr[29] = /*_._IFELSE___V32*/ meltfptr[31];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:3659:/ clear");
                                                /*clear*/ /*_#STRBUF_USEDLENGTH__L20*/ meltfnum[19] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_.REFERENCED_VALUE__V31*/ meltfptr[30] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_#GET_INT__L21*/ meltfnum[20] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_#ltI__L22*/ meltfnum[21] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_._IFELSE___V32*/ meltfptr[31] = 0 ;
                                            }

#else /*MELT_HAVE_DEBUG*/
                                            /*^cppif.else*/
                                            /*_.IFCPP___V30*/ meltfptr[29] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                            ;
                                            if (/*_#IX__L18*/ meltfnum[7]<0) break;
                                        } /* end  foreach_in_multiple meltcit3__EACHTUP */

                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:3655:/ clear");
                                    /*clear*/ /*_.EXPCUR__V28*/ meltfptr[23] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#IX__L18*/ meltfnum[7] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#GET_INT__L19*/ meltfnum[6] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.OUTPUT_C_CODE__V29*/ meltfptr[24] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.IFCPP___V30*/ meltfptr[29] = 0 ;
                                } /*endciterblock FOREACH_IN_MULTIPLE*/
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:3652:/ quasiblock");


                                /*epilog*/

                                /*^clear*/
                                /*clear*/ /*_#MULTIPLE_LENGTH__L15*/ meltfnum[11] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#gtI__L16*/ meltfnum[12] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {

                                /*_#plI__L23*/ meltfnum[19] =
                                    ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:3663:/ msend");
                                /*msend*/
                                {
                                    union meltparam_un argtab[3];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^ojbmsend.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                    /*^ojbmsend.arg*/
                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                    /*^ojbmsend.arg*/
                                    argtab[2].meltbp_long = /*_#plI__L23*/ meltfnum[19];
                                    /*_.OUTPUT_C_CODE__V34*/ meltfptr[32] = meltgc_send((melt_ptr_t)(/*_.CEXP__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:3662:/ quasiblock");


                                /*_.PROGN___V35*/ meltfptr[30] = /*_.OUTPUT_C_CODE__V34*/ meltfptr[32];;
                                /*^compute*/
                                /*_._IFELSE___V27*/ meltfptr[25] = /*_.PROGN___V35*/ meltfptr[30];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:3652:/ clear");
                                /*clear*/ /*_#plI__L23*/ meltfnum[19] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.OUTPUT_C_CODE__V34*/ meltfptr[32] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V35*/ meltfptr[30] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V19*/ meltfptr[18] = /*_._IFELSE___V27*/ meltfptr[25];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3642:/ clear");
                    /*clear*/ /*_#IS_MULTIPLE__L14*/ meltfnum[10] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V27*/ meltfptr[25] = 0 ;
                }
                ;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3665:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3666:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L24*/ meltfnum[20] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3667:/ cond");
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
                        /*_.REFERENCED_VALUE__V37*/ meltfptr[32] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V37*/ meltfptr[32] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L25*/ meltfnum[21] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V37*/ meltfptr[32])));;
            /*^compute*/
            /*_#ltI__L26*/ meltfnum[11] =
                ((/*_#STRBUF_USEDLENGTH__L24*/ meltfnum[20]) < (/*_#GET_INT__L25*/ meltfnum[21]));;
            MELT_LOCATION("warmelt-outobj.melt:3666:/ cond");
            /*cond*/ if (/*_#ltI__L26*/ meltfnum[11]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V38*/ meltfptr[30] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3666:/ cond.else");

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
                            argtab[2].meltbp_long = 3666;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V39*/ meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V38*/ meltfptr[30] = /*_.MELT_ASSERT_FAILURE_FUN__V39*/ meltfptr[25];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3666:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V39*/ meltfptr[25] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V36*/ meltfptr[31] = /*_._IFELSE___V38*/ meltfptr[30];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3666:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L24*/ meltfnum[20] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V37*/ meltfptr[32] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L25*/ meltfnum[21] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L26*/ meltfnum[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V38*/ meltfptr[30] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V36*/ meltfptr[31] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V36*/ meltfptr[31];;

        MELT_LOCATION("warmelt-outobj.melt:3628:/ clear");
        /*clear*/ /*_.CDEST__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CLOC__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CEXP__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BOXDEPTHP1__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_LIST__L4*/ meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_LIST__L6*/ meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V36*/ meltfptr[31] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3626:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3626:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJCOMPUTE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_80_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOMPUTE*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND fromline 1752 */

    /** start of frame for meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND// fromline 1568
        : public Melt_CallFrameWithValues<25>
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
        MeltFrame_meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<25> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND), clos) {};
        MeltFrame_meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND() //the constructor fromline 1642
            : Melt_CallFrameWithValues<25> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<25> (fil,lin, sizeof(MeltFrame_meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND)) {};
        MeltFrame_meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<25> (fil,lin, sizeof(MeltFrame_meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND


    /** end of frame for meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND fromline 1697**/

    /* end of frame for routine meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND fromline 1756 */

    /* classy proc frame meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND */ MeltFrame_meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJCOND", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3673:/ getarg");
    /*_.OCOND__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3674:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OCOND__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJCOND*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3674:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3674:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check ocond";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3674;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OCOND__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3674:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3674:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3675:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OCOND__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.CLOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3676:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OCOND__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBCOND_TEST");
            /*_.CTEST__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3677:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OCOND__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBCOND_THEN");
            /*_.CTHEN__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3678:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OCOND__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OBCOND_ELSE");
            /*_.CELSE__V12*/ meltfptr[11] = slot;
        };
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3680:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#NOTNULL__L3*/ meltfnum[1] =
                ((/*_.CTEST__V10*/ meltfptr[9]) != NULL);;
            MELT_LOCATION("warmelt-outobj.melt:3680:/ cond");
            /*cond*/ if (/*_#NOTNULL__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V14*/ meltfptr[13] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3680:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check ctest";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3680;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CTEST__V10*/ meltfptr[9];
                            /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V14*/ meltfptr[13] = /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3680:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V13*/ meltfptr[12] = /*_._IFELSE___V14*/ meltfptr[13];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3680:/ clear");
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
        MELT_LOCATION("warmelt-outobj.melt:3681:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "cond";
            /*_.OUTPUT_LOCATION__V16*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.CLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3682:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*cond*/ if ("));
        }
        ;
        /*_#plI__L4*/ meltfnum[1] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3683:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#plI__L4*/ meltfnum[1];
            /*_.OUTPUT_C_CODE__V17*/ meltfptr[13] = meltgc_send((melt_ptr_t)(/*_.CTEST__V10*/ meltfptr[9]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[3])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3684:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ") /*then*/ {"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3685:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3686:/ cond");
        /*cond*/ if (/*_.CTHEN__V11*/ meltfptr[10]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_#IS_A__L6*/ meltfnum[5] =
                        melt_is_instance_of((melt_ptr_t)(/*_.CTHEN__V11*/ meltfptr[10]), (melt_ptr_t)((/*!CLASS_OBJPUREVALUE*/ meltfrout->tabval[4])));;
                    /*^compute*/
                    /*_#NOT__L7*/ meltfnum[6] =
                        (!(/*_#IS_A__L6*/ meltfnum[5]));;
                    /*^compute*/
                    /*_#_IF___L5*/ meltfnum[4] = /*_#NOT__L7*/ meltfnum[6];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3686:/ clear");
                    /*clear*/ /*_#IS_A__L6*/ meltfnum[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#NOT__L7*/ meltfnum[6] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_#_IF___L5*/ meltfnum[4] = 0;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:3686:/ cond");
        /*cond*/ if (/*_#_IF___L5*/ meltfnum[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3687:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                        /*^apply.arg*/
                        argtab[2].meltbp_cstring =  "cond.then";
                        /*_.OUTPUT_LOCATION__V18*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.CLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_#plI__L8*/ meltfnum[5] =
                        ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3688:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#plI__L8*/ meltfnum[5];
                        /*_.OUTPUT_C_CODE__V19*/ meltfptr[18] = meltgc_send((melt_ptr_t)(/*_.CTHEN__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[3])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3689:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3690:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3686:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_LOCATION__V18*/ meltfptr[17] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#plI__L8*/ meltfnum[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_C_CODE__V19*/ meltfptr[18] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3692:/ cond");
        /*cond*/ if (/*_.CELSE__V12*/ meltfptr[11]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_#IS_A__L10*/ meltfnum[5] =
                        melt_is_instance_of((melt_ptr_t)(/*_.CELSE__V12*/ meltfptr[11]), (melt_ptr_t)((/*!CLASS_OBJPUREVALUE*/ meltfrout->tabval[4])));;
                    /*^compute*/
                    /*_#NOT__L11*/ meltfnum[10] =
                        (!(/*_#IS_A__L10*/ meltfnum[5]));;
                    /*^compute*/
                    /*_#_IF___L9*/ meltfnum[6] = /*_#NOT__L11*/ meltfnum[10];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3692:/ clear");
                    /*clear*/ /*_#IS_A__L10*/ meltfnum[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#NOT__L11*/ meltfnum[10] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_#_IF___L9*/ meltfnum[6] = 0;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:3692:/ cond");
        /*cond*/ if (/*_#_IF___L9*/ meltfnum[6]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:3694:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "} else {"));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3695:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                        /*^apply.arg*/
                        argtab[2].meltbp_cstring =  "cond.else";
                        /*_.OUTPUT_LOCATION__V20*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.CLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_#plI__L12*/ meltfnum[5] =
                        ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3696:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L12*/ meltfnum[5]), 0);
                    }
                    ;
                    /*_#plI__L13*/ meltfnum[10] =
                        ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3697:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#plI__L13*/ meltfnum[10];
                        /*_.OUTPUT_C_CODE__V21*/ meltfptr[18] = meltgc_send((melt_ptr_t)(/*_.CELSE__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[3])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3698:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
                    }
                    ;
                    /*_#plI__L14*/ meltfnum[13] =
                        ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3699:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L14*/ meltfnum[13]), 0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3700:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "}"));
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3693:/ quasiblock");


                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3692:/ clear");
                    /*clear*/ /*_.OUTPUT_LOCATION__V20*/ meltfptr[17] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#plI__L12*/ meltfnum[5] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#plI__L13*/ meltfnum[10] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.OUTPUT_C_CODE__V21*/ meltfptr[18] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#plI__L14*/ meltfnum[13] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:3702:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "} /*noelse*/"));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3704:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3705:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L15*/ meltfnum[5] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3706:/ cond");
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
                        /*_.REFERENCED_VALUE__V23*/ meltfptr[18] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V23*/ meltfptr[18] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L16*/ meltfnum[10] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V23*/ meltfptr[18])));;
            /*^compute*/
            /*_#ltI__L17*/ meltfnum[13] =
                ((/*_#STRBUF_USEDLENGTH__L15*/ meltfnum[5]) < (/*_#GET_INT__L16*/ meltfnum[10]));;
            MELT_LOCATION("warmelt-outobj.melt:3705:/ cond");
            /*cond*/ if (/*_#ltI__L17*/ meltfnum[13]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V24*/ meltfptr[23] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3705:/ cond.else");

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
                            argtab[2].meltbp_long = 3705;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V24*/ meltfptr[23] = /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3705:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V22*/ meltfptr[17] = /*_._IFELSE___V24*/ meltfptr[23];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3705:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L15*/ meltfnum[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V23*/ meltfptr[18] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L16*/ meltfnum[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L17*/ meltfnum[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V24*/ meltfptr[23] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V22*/ meltfptr[17] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V22*/ meltfptr[17];;

        MELT_LOCATION("warmelt-outobj.melt:3675:/ clear");
        /*clear*/ /*_.CLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CTEST__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CTHEN__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CELSE__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V16*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L4*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V17*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_#_IF___L5*/ meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#_IF___L9*/ meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V22*/ meltfptr[17] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3673:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3673:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJCOND", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_81_WARMELTmiOUTOBJ_OUTPUCOD_OBJCOND*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF fromline 1752 */

    /** start of frame for meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF// fromline 1568
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
        MeltFrame_meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<24> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF), clos) {};
        MeltFrame_meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF() //the constructor fromline 1642
            : Melt_CallFrameWithValues<24> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<24> (fil,lin, sizeof(MeltFrame_meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF)) {};
        MeltFrame_meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<24> (fil,lin, sizeof(MeltFrame_meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF


    /** end of frame for meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF fromline 1697**/

    /* end of frame for routine meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF fromline 1756 */

    /* classy proc frame meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF */ MeltFrame_meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJCPPIF", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3713:/ getarg");
    /*_.OPIF__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3714:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OPIF__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJCPPIF*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3714:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3714:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check opif";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3714;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OPIF__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3714:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3714:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3715:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPIF__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.CLOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3716:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPIF__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBIFP_COND");
            /*_.CCOND__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3717:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPIF__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBIFP_THEN");
            /*_.CTHEN__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3718:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OPIF__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OBIFP_ELSE");
            /*_.CELSE__V12*/ meltfptr[11] = slot;
        };
        ;
        /*_#DEPTHP1__L3*/ meltfnum[1] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3721:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRING__L4*/ meltfnum[3] =
                (melt_magic_discr((melt_ptr_t)(/*_.CCOND__V10*/ meltfptr[9])) == MELTOBMAG_STRING);;
            MELT_LOCATION("warmelt-outobj.melt:3721:/ cond");
            /*cond*/ if (/*_#IS_STRING__L4*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V14*/ meltfptr[13] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3721:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check ccond";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3721;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CCOND__V10*/ meltfptr[9];
                            /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V14*/ meltfptr[13] = /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3721:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V13*/ meltfptr[12] = /*_._IFELSE___V14*/ meltfptr[13];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3721:/ clear");
            /*clear*/ /*_#IS_STRING__L4*/ meltfnum[3] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:3722:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "cppif";
            /*_.OUTPUT_RAW_LOCATION__V16*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_RAW_LOCATION*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.CLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3723:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "#if "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3724:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.CCOND__V10*/ meltfptr[9])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3725:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTHP1__L3*/ meltfnum[1]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3726:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "cppif.then";
            /*_.OUTPUT_LOCATION__V17*/ meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.CLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3727:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTHP1__L3*/ meltfnum[1];
            /*_.OUTPUT_C_CODE__V18*/ meltfptr[17] = meltgc_send((melt_ptr_t)(/*_.CTHEN__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3728:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTHP1__L3*/ meltfnum[1]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3729:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "#else /*"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3730:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.CCOND__V10*/ meltfptr[9])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3731:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3732:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTHP1__L3*/ meltfnum[1]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3733:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "cppif.else";
            /*_.OUTPUT_LOCATION__V19*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.CLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3734:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#DEPTHP1__L3*/ meltfnum[1];
            /*_.OUTPUT_C_CODE__V20*/ meltfptr[19] = meltgc_send((melt_ptr_t)(/*_.CELSE__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3735:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTHP1__L3*/ meltfnum[1]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3736:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "#endif /*"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3737:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.CCOND__V10*/ meltfptr[9])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3738:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3739:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTHP1__L3*/ meltfnum[1]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3740:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L5*/ meltfnum[3] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3741:/ cond");
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
                ((/*_#STRBUF_USEDLENGTH__L5*/ meltfnum[3]) < (/*_#GET_INT__L6*/ meltfnum[5]));;
            MELT_LOCATION("warmelt-outobj.melt:3740:/ cond");
            /*cond*/ if (/*_#ltI__L7*/ meltfnum[6]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V23*/ meltfptr[22] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3740:/ cond.else");

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
                            argtab[2].meltbp_long = 3740;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V23*/ meltfptr[22] = /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[23];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3740:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[23] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V21*/ meltfptr[20] = /*_._IFELSE___V23*/ meltfptr[22];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3740:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L5*/ meltfnum[3] = 0 ;
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
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V21*/ meltfptr[20];;

        MELT_LOCATION("warmelt-outobj.melt:3715:/ clear");
        /*clear*/ /*_.CLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CCOND__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CTHEN__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CELSE__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_#DEPTHP1__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_RAW_LOCATION__V16*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V17*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V18*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V20*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V21*/ meltfptr[20] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3713:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3713:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJCPPIF", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_82_WARMELTmiOUTOBJ_OUTPUCOD_OBJCPPIF*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL fromline 1752 */

    /** start of frame for meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL// fromline 1568
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
        MeltFrame_meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<27> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL), clos) {};
        MeltFrame_meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL() //the constructor fromline 1642
            : Melt_CallFrameWithValues<27> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<27> (fil,lin, sizeof(MeltFrame_meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL)) {};
        MeltFrame_meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<27> (fil,lin, sizeof(MeltFrame_meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL


    /** end of frame for meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL fromline 1697**/

    /* end of frame for routine meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL fromline 1756 */

    /* classy proc frame meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL */ MeltFrame_meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJINTERNSYMBOL", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3746:/ getarg");
    /*_.OISY__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3747:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OISY__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJINTERNSYMBOL*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3747:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3747:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oisy";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3747;
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3747:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3747:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3748:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OISY__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.CLOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3749:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OISY__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBINTERN_IOBJ");
            /*_.OIOBJ__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3750:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OIOBJ__V10*/ meltfptr[9]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OIE_DATA");
            /*_.OIDAT__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3751:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OIOBJ__V10*/ meltfptr[9]) /*=obj*/;
            melt_object_get_field(slot,obj, 4, "OIE_LOCVAR");
            /*_.OILOCV__V12*/ meltfptr[11] = slot;
        };
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3753:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OIOBJ__V10*/ meltfptr[9]), (melt_ptr_t)((/*!CLASS_OBJINITOBJECT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-outobj.melt:3753:/ cond");
            /*cond*/ if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V14*/ meltfptr[13] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3753:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oiobj";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3753;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OIOBJ__V10*/ meltfptr[9];
                            /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V14*/ meltfptr[13] = /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3753:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V13*/ meltfptr[12] = /*_._IFELSE___V14*/ meltfptr[13];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3753:/ clear");
            /*clear*/ /*_#IS_A__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V14*/ meltfptr[13] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V13*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3754:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OIDAT__V11*/ meltfptr[10]), (melt_ptr_t)((/*!CLASS_NREP_DATASYMBOL*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-outobj.melt:3754:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V17*/ meltfptr[13] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3754:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oidat";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3754;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OIDAT__V11*/ meltfptr[10];
                            /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V17*/ meltfptr[13] = /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3754:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V16*/ meltfptr[14] = /*_._IFELSE___V17*/ meltfptr[13];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3754:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V17*/ meltfptr[13] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V16*/ meltfptr[14] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3755:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OIDAT__V11*/ meltfptr[10]) /*=obj*/;
            melt_object_get_field(slot,obj, 9, "NDSY_NAMESTR");
            /*_.NSY__V19*/ meltfptr[17] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3756:/ cond");
        /*cond*/ if (/*_.CLOC__V9*/ meltfptr[5]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V20*/ meltfptr[13] = /*_.CLOC__V9*/ meltfptr[5];;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:3756:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.OIDAT__V11*/ meltfptr[10]) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "NREP_LOC");
                        /*_.NREP_LOC__V21*/ meltfptr[20] = slot;
                    };
                    ;
                    /*_._IFELSE___V20*/ meltfptr[13] = /*_.NREP_LOC__V21*/ meltfptr[20];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3756:/ clear");
                    /*clear*/ /*_.NREP_LOC__V21*/ meltfptr[20] = 0 ;
                }
                ;
            }
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
            argtab[1].meltbp_long = 1;
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "internsymbol";
            /*_.OUTPUT_LOCATION__V22*/ meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_._IFELSE___V20*/ meltfptr[13]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3757:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*internsym:"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3758:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.NSY__V19*/ meltfptr[17])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3759:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3760:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3761:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "(void) melthookproc_HOOK_INTERN_SYMBOL ((melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3762:/ msend");
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
            /*_.OUTPUT_C_CODE__V23*/ meltfptr[22] = meltgc_send((melt_ptr_t)(/*_.OILOCV__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3763:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "));"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3764:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_LOCATION("warmelt-outobj.melt:3755:/ clear");
        /*clear*/ /*_.NSY__V19*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V20*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V22*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V23*/ meltfptr[22] = 0 ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3765:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L5*/ meltfnum[1] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3766:/ cond");
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
                        /*_.REFERENCED_VALUE__V25*/ meltfptr[13] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V25*/ meltfptr[13] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L6*/ meltfnum[5] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V25*/ meltfptr[13])));;
            /*^compute*/
            /*_#ltI__L7*/ meltfnum[6] =
                ((/*_#STRBUF_USEDLENGTH__L5*/ meltfnum[1]) < (/*_#GET_INT__L6*/ meltfnum[5]));;
            MELT_LOCATION("warmelt-outobj.melt:3765:/ cond");
            /*cond*/ if (/*_#ltI__L7*/ meltfnum[6]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V26*/ meltfptr[20] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3765:/ cond.else");

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
                            argtab[2].meltbp_long = 3765;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V26*/ meltfptr[20] = /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[22];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3765:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V27*/ meltfptr[22] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V24*/ meltfptr[17] = /*_._IFELSE___V26*/ meltfptr[20];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3765:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L5*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V25*/ meltfptr[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L6*/ meltfnum[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L7*/ meltfnum[6] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V26*/ meltfptr[20] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V24*/ meltfptr[17] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V24*/ meltfptr[17];;

        MELT_LOCATION("warmelt-outobj.melt:3748:/ clear");
        /*clear*/ /*_.CLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OIOBJ__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OIDAT__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OILOCV__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V16*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V24*/ meltfptr[17] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3746:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3746:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJINTERNSYMBOL", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_83_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNSYMBOL*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD fromline 1752 */

    /** start of frame for meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD// fromline 1568
        : public Melt_CallFrameWithValues<24>
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
        MeltFrame_meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<24> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD), clos) {};
        MeltFrame_meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD() //the constructor fromline 1642
            : Melt_CallFrameWithValues<24> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<24> (fil,lin, sizeof(MeltFrame_meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD)) {};
        MeltFrame_meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<24> (fil,lin, sizeof(MeltFrame_meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD


    /** end of frame for meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD fromline 1697**/

    /* end of frame for routine meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD fromline 1756 */

    /* classy proc frame meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD */ MeltFrame_meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJINTERNKEYWORD", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3771:/ getarg");
    /*_.OIKW__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3772:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OIKW__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJINTERNKEYWORD*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3772:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3772:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oikw";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3772;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OIKW__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3772:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3772:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3773:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OIKW__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.CLOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3774:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OIKW__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBINTERN_IOBJ");
            /*_.OIOBJ__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3775:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OIOBJ__V10*/ meltfptr[9]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OIE_DATA");
            /*_.OIDAT__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3776:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OIOBJ__V10*/ meltfptr[9]) /*=obj*/;
            melt_object_get_field(slot,obj, 4, "OIE_LOCVAR");
            /*_.OILOCV__V12*/ meltfptr[11] = slot;
        };
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3778:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OIDAT__V11*/ meltfptr[10]), (melt_ptr_t)((/*!CLASS_NREP_DATAKEYWORD*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-outobj.melt:3778:/ cond");
            /*cond*/ if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V14*/ meltfptr[13] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3778:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oidat";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3778;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OIDAT__V11*/ meltfptr[10];
                            /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V14*/ meltfptr[13] = /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3778:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V13*/ meltfptr[12] = /*_._IFELSE___V14*/ meltfptr[13];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3778:/ clear");
            /*clear*/ /*_#IS_A__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V14*/ meltfptr[13] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V13*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3779:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OIDAT__V11*/ meltfptr[10]) /*=obj*/;
            melt_object_get_field(slot,obj, 9, "NDSY_NAMESTR");
            /*_.NSY__V16*/ meltfptr[14] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3780:/ cond");
        /*cond*/ if (/*_.CLOC__V9*/ meltfptr[5]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V17*/ meltfptr[13] = /*_.CLOC__V9*/ meltfptr[5];;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:3780:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.OIDAT__V11*/ meltfptr[10]) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "NREP_LOC");
                        /*_.NREP_LOC__V18*/ meltfptr[17] = slot;
                    };
                    ;
                    /*_._IFELSE___V17*/ meltfptr[13] = /*_.NREP_LOC__V18*/ meltfptr[17];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3780:/ clear");
                    /*clear*/ /*_.NREP_LOC__V18*/ meltfptr[17] = 0 ;
                }
                ;
            }
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
            argtab[2].meltbp_cstring =  "internkeyword";
            /*_.OUTPUT_LOCATION__V19*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_._IFELSE___V17*/ meltfptr[13]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3781:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*internkeyw:"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3782:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.NSY__V16*/ meltfptr[14])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3783:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3784:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3785:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "(void) melthookproc_HOOK_INTERN_KEYWORD ((melt_ptr_t)("));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3786:/ msend");
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
            /*_.OUTPUT_C_CODE__V20*/ meltfptr[19] = meltgc_send((melt_ptr_t)(/*_.OILOCV__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3787:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "));"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3788:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        MELT_LOCATION("warmelt-outobj.melt:3779:/ clear");
        /*clear*/ /*_.NSY__V16*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V17*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V19*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V20*/ meltfptr[19] = 0 ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3789:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L4*/ meltfnum[1] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3790:/ cond");
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
                        /*_.REFERENCED_VALUE__V22*/ meltfptr[13] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V22*/ meltfptr[13] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L5*/ meltfnum[4] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V22*/ meltfptr[13])));;
            /*^compute*/
            /*_#ltI__L6*/ meltfnum[5] =
                ((/*_#STRBUF_USEDLENGTH__L4*/ meltfnum[1]) < (/*_#GET_INT__L5*/ meltfnum[4]));;
            MELT_LOCATION("warmelt-outobj.melt:3789:/ cond");
            /*cond*/ if (/*_#ltI__L6*/ meltfnum[5]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V23*/ meltfptr[17] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3789:/ cond.else");

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
                            argtab[2].meltbp_long = 3789;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V23*/ meltfptr[17] = /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[19];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3789:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[19] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V21*/ meltfptr[14] = /*_._IFELSE___V23*/ meltfptr[17];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3789:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L4*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V22*/ meltfptr[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L5*/ meltfnum[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L6*/ meltfnum[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V23*/ meltfptr[17] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V21*/ meltfptr[14] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V21*/ meltfptr[14];;

        MELT_LOCATION("warmelt-outobj.melt:3773:/ clear");
        /*clear*/ /*_.CLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OIOBJ__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OIDAT__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OILOCV__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V21*/ meltfptr[14] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3771:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3771:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJINTERNKEYWORD", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_84_WARMELTmiOUTOBJ_OUTPUCOD_OBJINTERNKEYWORD*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL fromline 1752 */

    /** start of frame for meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL// fromline 1568
        : public Melt_CallFrameWithValues<30>
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
        MeltFrame_meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<30> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL), clos) {};
        MeltFrame_meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL() //the constructor fromline 1642
            : Melt_CallFrameWithValues<30> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<30> (fil,lin, sizeof(MeltFrame_meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL)) {};
        MeltFrame_meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<30> (fil,lin, sizeof(MeltFrame_meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL


    /** end of frame for meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL fromline 1697**/

    /* end of frame for routine meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL fromline 1756 */

    /* classy proc frame meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL */ MeltFrame_meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJGETNAMEDSYMBOL", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3795:/ getarg");
    /*_.OGSY__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3796:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OGSY__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJGETNAMEDSYMBOL*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3796:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3796:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check ogsy";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3796;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OGSY__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3796:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3796:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3797:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OGSY__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.CLOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3798:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OGSY__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBGNAMED_IOBJ");
            /*_.OIOBJ__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3799:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OIOBJ__V10*/ meltfptr[9]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OIE_DATA");
            /*_.OGDAT__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3800:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OIOBJ__V10*/ meltfptr[9]) /*=obj*/;
            melt_object_get_field(slot,obj, 4, "OIE_LOCVAR");
            /*_.OILOCV__V12*/ meltfptr[11] = slot;
        };
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3802:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OIOBJ__V10*/ meltfptr[9]), (melt_ptr_t)((/*!CLASS_OBJINITOBJECT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-outobj.melt:3802:/ cond");
            /*cond*/ if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V14*/ meltfptr[13] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3802:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oiobj";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3802;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OIOBJ__V10*/ meltfptr[9];
                            /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V14*/ meltfptr[13] = /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3802:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V13*/ meltfptr[12] = /*_._IFELSE___V14*/ meltfptr[13];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3802:/ clear");
            /*clear*/ /*_#IS_A__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V14*/ meltfptr[13] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V13*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3803:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OGDAT__V11*/ meltfptr[10]), (melt_ptr_t)((/*!CLASS_NREP_DATASYMBOL*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-outobj.melt:3803:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V17*/ meltfptr[13] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3803:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check ogdat";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3803;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OGDAT__V11*/ meltfptr[10];
                            /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V17*/ meltfptr[13] = /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3803:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V16*/ meltfptr[14] = /*_._IFELSE___V17*/ meltfptr[13];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3803:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V17*/ meltfptr[13] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V16*/ meltfptr[14] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3804:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OGDAT__V11*/ meltfptr[10]) /*=obj*/;
            melt_object_get_field(slot,obj, 9, "NDSY_NAMESTR");
            /*_.NSY__V20*/ meltfptr[13] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3806:/ cond");
        /*cond*/ if (/*_.CLOC__V9*/ meltfptr[5]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V21*/ meltfptr[20] = /*_.CLOC__V9*/ meltfptr[5];;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:3806:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.OGDAT__V11*/ meltfptr[10]) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "NREP_LOC");
                        /*_.NREP_LOC__V22*/ meltfptr[21] = slot;
                    };
                    ;
                    /*_._IFELSE___V21*/ meltfptr[20] = /*_.NREP_LOC__V22*/ meltfptr[21];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3806:/ clear");
                    /*clear*/ /*_.NREP_LOC__V22*/ meltfptr[21] = 0 ;
                }
                ;
            }
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
            argtab[2].meltbp_cstring =  "getnamedsymbol";
            /*_.OUTPUT_LOCATION__V23*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_._IFELSE___V21*/ meltfptr[20]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3807:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*getnamedsym:"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3808:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.NSY__V20*/ meltfptr[13])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3809:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3810:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;
        /*_#plI__L5*/ meltfnum[1] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        {
            MELT_LOCATION("warmelt-outobj.melt:3811:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L5*/ meltfnum[1]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3812:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "if (NULL == "));
        }
        ;
        /*_#plI__L6*/ meltfnum[5] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3813:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#plI__L6*/ meltfnum[5];
            /*_.OUTPUT_C_CODE__V24*/ meltfptr[23] = meltgc_send((melt_ptr_t)(/*_.OILOCV__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3814:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ") {"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3815:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3818:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "#if !MELT_HAS_INITIAL_ENVIRONMENT"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3819:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3820:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "#error MELT getting named symbol ";
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.NSY__V20*/ meltfptr[13];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  " without initial environment";
            /*_.ADD2OUT__V25*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3821:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3822:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "#endif /*!MELT_HAS_INITIAL_ENVIRONMENT*/"));
        }
        ;
        /*_#plI__L7*/ meltfnum[6] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (2));;

        {
            MELT_LOCATION("warmelt-outobj.melt:3823:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L7*/ meltfnum[6]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3824:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " melt_ptr_t meltsy_"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3825:/ locexp");
            meltgc_add_strbuf_cident((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                     melt_string_str((melt_ptr_t)(/*_.NSY__V20*/ meltfptr[13])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3826:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = melthookproc_HOOK_NAMED_SYMBOL (\""));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3827:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.NSY__V20*/ meltfptr[13])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3828:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "\", (long) MELT_GET);"));
        }
        ;
        /*_#plI__L8*/ meltfnum[7] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (2));;

        {
            MELT_LOCATION("warmelt-outobj.melt:3829:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L8*/ meltfnum[7]), 0);
        }
        ;
        /*_#plI__L9*/ meltfnum[8] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (3));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3830:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#plI__L9*/ meltfnum[8];
            /*_.OUTPUT_C_CODE__V26*/ meltfptr[25] = meltgc_send((melt_ptr_t)(/*_.OILOCV__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3831:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = (melt_ptr_t) meltsy_"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3832:/ locexp");
            meltgc_add_strbuf_cident((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                     melt_string_str((melt_ptr_t)(/*_.NSY__V20*/ meltfptr[13])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3833:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3834:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3835:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "} /*endgetnamedsym "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3836:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.NSY__V20*/ meltfptr[13])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3837:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3838:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3839:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3840:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L10*/ meltfnum[9] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3841:/ cond");
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
            /*_#GET_INT__L11*/ meltfnum[10] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V28*/ meltfptr[27])));;
            /*^compute*/
            /*_#ltI__L12*/ meltfnum[11] =
                ((/*_#STRBUF_USEDLENGTH__L10*/ meltfnum[9]) < (/*_#GET_INT__L11*/ meltfnum[10]));;
            MELT_LOCATION("warmelt-outobj.melt:3840:/ cond");
            /*cond*/ if (/*_#ltI__L12*/ meltfnum[11]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V29*/ meltfptr[28] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3840:/ cond.else");

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
                            argtab[2].meltbp_long = 3840;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V30*/ meltfptr[29] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V29*/ meltfptr[28] = /*_.MELT_ASSERT_FAILURE_FUN__V30*/ meltfptr[29];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3840:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V30*/ meltfptr[29] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V27*/ meltfptr[26] = /*_._IFELSE___V29*/ meltfptr[28];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3840:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L10*/ meltfnum[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V28*/ meltfptr[27] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L11*/ meltfnum[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L12*/ meltfnum[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V29*/ meltfptr[28] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V27*/ meltfptr[26] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V19*/ meltfptr[17] = /*_.IFCPP___V27*/ meltfptr[26];;

        MELT_LOCATION("warmelt-outobj.melt:3804:/ clear");
        /*clear*/ /*_.NSY__V20*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V21*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V23*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L5*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L6*/ meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V24*/ meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADD2OUT__V25*/ meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L7*/ meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L8*/ meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L9*/ meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V26*/ meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V27*/ meltfptr[26] = 0 ;
        /*_.LET___V8*/ meltfptr[6] = /*_.LET___V19*/ meltfptr[17];;

        MELT_LOCATION("warmelt-outobj.melt:3797:/ clear");
        /*clear*/ /*_.CLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OIOBJ__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OGDAT__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OILOCV__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V16*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V19*/ meltfptr[17] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3795:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3795:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJGETNAMEDSYMBOL", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_85_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDSYMBOL*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD fromline 1752 */

    /** start of frame for meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD// fromline 1568
        : public Melt_CallFrameWithValues<30>
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
        MeltFrame_meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<30> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD), clos) {};
        MeltFrame_meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD() //the constructor fromline 1642
            : Melt_CallFrameWithValues<30> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<30> (fil,lin, sizeof(MeltFrame_meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD)) {};
        MeltFrame_meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<30> (fil,lin, sizeof(MeltFrame_meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD


    /** end of frame for meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD fromline 1697**/

    /* end of frame for routine meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD fromline 1756 */

    /* classy proc frame meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD */ MeltFrame_meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJGETNAMEDKEYWORD", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3846:/ getarg");
    /*_.OGKW__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3847:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OGKW__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJGETNAMEDKEYWORD*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3847:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3847:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check ogkw";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3847;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OGKW__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3847:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3847:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3848:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OGKW__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.CLOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3849:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OGKW__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBGNAMED_IOBJ");
            /*_.OIOBJ__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3850:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OIOBJ__V10*/ meltfptr[9]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OIE_DATA");
            /*_.OGDAT__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3851:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OIOBJ__V10*/ meltfptr[9]) /*=obj*/;
            melt_object_get_field(slot,obj, 4, "OIE_LOCVAR");
            /*_.OILOCV__V12*/ meltfptr[11] = slot;
        };
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3853:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OIOBJ__V10*/ meltfptr[9]), (melt_ptr_t)((/*!CLASS_OBJINITOBJECT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-outobj.melt:3853:/ cond");
            /*cond*/ if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V14*/ meltfptr[13] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3853:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oiobj";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3853;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OIOBJ__V10*/ meltfptr[9];
                            /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V14*/ meltfptr[13] = /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3853:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V13*/ meltfptr[12] = /*_._IFELSE___V14*/ meltfptr[13];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3853:/ clear");
            /*clear*/ /*_#IS_A__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V14*/ meltfptr[13] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V13*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3854:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OGDAT__V11*/ meltfptr[10]), (melt_ptr_t)((/*!CLASS_NREP_DATAKEYWORD*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-outobj.melt:3854:/ cond");
            /*cond*/ if (/*_#IS_A__L4*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V17*/ meltfptr[13] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3854:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check ogdat";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3854;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OGDAT__V11*/ meltfptr[10];
                            /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V17*/ meltfptr[13] = /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3854:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V16*/ meltfptr[14] = /*_._IFELSE___V17*/ meltfptr[13];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3854:/ clear");
            /*clear*/ /*_#IS_A__L4*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V17*/ meltfptr[13] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V16*/ meltfptr[14] = (/*nil*/NULL);
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OGKW__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V20*/ meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V19*/ meltfptr[17] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V20*/ meltfptr[13];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V20*/ meltfptr[13] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V19*/ meltfptr[17] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:3856:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OGDAT__V11*/ meltfptr[10]) /*=obj*/;
            melt_object_get_field(slot,obj, 9, "NDSY_NAMESTR");
            /*_.NKW__V22*/ meltfptr[21] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3857:/ cond");
        /*cond*/ if (/*_.CLOC__V9*/ meltfptr[5]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V23*/ meltfptr[22] = /*_.CLOC__V9*/ meltfptr[5];;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:3857:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.OGDAT__V11*/ meltfptr[10]) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "NREP_LOC");
                        /*_.NREP_LOC__V24*/ meltfptr[23] = slot;
                    };
                    ;
                    /*_._IFELSE___V23*/ meltfptr[22] = /*_.NREP_LOC__V24*/ meltfptr[23];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:3857:/ clear");
                    /*clear*/ /*_.NREP_LOC__V24*/ meltfptr[23] = 0 ;
                }
                ;
            }
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
            argtab[2].meltbp_cstring =  "getnamedkeyword";
            /*_.OUTPUT_LOCATION__V25*/ meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_._IFELSE___V23*/ meltfptr[22]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3858:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*getnamedkeyw:"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3859:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.NKW__V22*/ meltfptr[21])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3860:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "*/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3861:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3862:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "{ melt_ptr_t kw_"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3863:/ locexp");
            meltgc_add_strbuf_cident((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                     melt_string_str((melt_ptr_t)(/*_.NKW__V22*/ meltfptr[21])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3864:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = melthookproc_HOOK_NAMED_KEYWORD (\""));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3865:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                  melt_string_str((melt_ptr_t)(/*_.NKW__V22*/ meltfptr[21])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3866:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "\", (long) MELT_GET);"));
        }
        ;
        /*_#plI__L5*/ meltfnum[1] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        {
            MELT_LOCATION("warmelt-outobj.melt:3867:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L5*/ meltfnum[1]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3868:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "if (kw_"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3869:/ locexp");
            meltgc_add_strbuf_cident((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                     melt_string_str((melt_ptr_t)(/*_.NKW__V22*/ meltfptr[21])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3870:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ") "));
        }
        ;
        /*_#plI__L6*/ meltfnum[5] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3871:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#plI__L6*/ meltfnum[5];
            /*_.OUTPUT_C_CODE__V26*/ meltfptr[25] = meltgc_send((melt_ptr_t)(/*_.OILOCV__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[6])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3872:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = (melt_ptr_t) kw_"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3873:/ locexp");
            meltgc_add_strbuf_cident((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                     melt_string_str((melt_ptr_t)(/*_.NKW__V22*/ meltfptr[21])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3874:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "; }"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3875:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3876:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[6] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3877:/ cond");
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
                ((/*_#STRBUF_USEDLENGTH__L7*/ meltfnum[6]) < (/*_#GET_INT__L8*/ meltfnum[7]));;
            MELT_LOCATION("warmelt-outobj.melt:3876:/ cond");
            /*cond*/ if (/*_#ltI__L9*/ meltfnum[8]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V29*/ meltfptr[28] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3876:/ cond.else");

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
                            argtab[2].meltbp_long = 3876;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V30*/ meltfptr[29] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V29*/ meltfptr[28] = /*_.MELT_ASSERT_FAILURE_FUN__V30*/ meltfptr[29];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3876:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V30*/ meltfptr[29] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V27*/ meltfptr[26] = /*_._IFELSE___V29*/ meltfptr[28];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3876:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L7*/ meltfnum[6] = 0 ;
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
        /*_.IFCPP___V27*/ meltfptr[26] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V21*/ meltfptr[13] = /*_.IFCPP___V27*/ meltfptr[26];;

        MELT_LOCATION("warmelt-outobj.melt:3856:/ clear");
        /*clear*/ /*_.NKW__V22*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V23*/ meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V25*/ meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L5*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L6*/ meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V26*/ meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V27*/ meltfptr[26] = 0 ;
        /*_.LET___V8*/ meltfptr[6] = /*_.LET___V21*/ meltfptr[13];;

        MELT_LOCATION("warmelt-outobj.melt:3848:/ clear");
        /*clear*/ /*_.CLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OIOBJ__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OGDAT__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OILOCV__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V16*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V19*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V21*/ meltfptr[13] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3846:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3846:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJGETNAMEDKEYWORD", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_86_WARMELTmiOUTOBJ_OUTPUCOD_OBJGETNAMEDKEYWORD*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY fromline 1752 */

    /** start of frame for meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY// fromline 1568
        : public Melt_CallFrameWithValues<62>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[45];
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
        MeltFrame_meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<62> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY), clos) {};
        MeltFrame_meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY() //the constructor fromline 1642
            : Melt_CallFrameWithValues<62> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<62> (fil,lin, sizeof(MeltFrame_meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY)) {};
        MeltFrame_meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<62> (fil,lin, sizeof(MeltFrame_meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY


    /** end of frame for meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY fromline 1697**/

    /* end of frame for routine meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY fromline 1756 */

    /* classy proc frame meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY */ MeltFrame_meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJAPPLY", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3883:/ getarg");
    /*_.OAPP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3884:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OAPP__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJAPPLY*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3884:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3884:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oapp";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3884;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OAPP__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3884:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3884:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3885:/ quasiblock");


        MELT_LOCATION("warmelt-outobj.melt:3886:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OAPP__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.ALOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3887:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OAPP__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBDI_DESTLIST");
            /*_.ADEST__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3888:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OAPP__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBAPP_CLOS");
            /*_.OCLOS__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:3889:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OAPP__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OBAPP_ARGS");
            /*_.OARGS__V12*/ meltfptr[11] = slot;
        };
        ;
        /*_#NBARG__L3*/ meltfnum[1] =
            (melt_multiple_length((melt_ptr_t)(/*_.OARGS__V12*/ meltfptr[11])));;
        /*^compute*/
        /*_.PARAMDESCLIST__V13*/ meltfptr[12] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
        /*^compute*/
        /*_#plI__L4*/ meltfnum[3] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;
        /*^compute*/
        /*_.BOXDEPTHP1__V14*/ meltfptr[13] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[3])), (/*_#plI__L4*/ meltfnum[3])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3894:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "apply";
            /*_.OUTPUT_LOCATION__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.ALOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OAPP__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V17*/ meltfptr[16] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
            MELT_LOCATION("warmelt-outobj.melt:3896:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*apply*/{"));
        }
        ;
        /*_#plI__L5*/ meltfnum[4] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

        {
            MELT_LOCATION("warmelt-outobj.melt:3897:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L5*/ meltfnum[4]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#gtI__L6*/ meltfnum[5] =
            ((/*_#NBARG__L3*/ meltfnum[1]) > (1));;
        MELT_LOCATION("warmelt-outobj.melt:3898:/ cond");
        /*cond*/ if (/*_#gtI__L6*/ meltfnum[5]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:3899:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "union meltparam_un argtab["));
                    }
                    ;
                    /*_#miI__L7*/ meltfnum[6] =
                        ((/*_#NBARG__L3*/ meltfnum[1]) - (1));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3900:/ locexp");
                        meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#miI__L7*/ meltfnum[6]));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3901:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "];"));
                    }
                    ;
                    /*_#plI__L8*/ meltfnum[7] =
                        ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3902:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L8*/ meltfnum[7]), 0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3903:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "memset(&argtab, 0, sizeof(argtab));"));
                    }
                    ;
                    /*_#plI__L9*/ meltfnum[8] =
                        ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3904:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L9*/ meltfnum[8]), 0);
                    }
                    ;
                    /*citerblock FOREACH_IN_MULTIPLE*/
                    {
                        /* start foreach_in_multiple meltcit1__EACHTUP */
                        long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.OARGS__V12*/ meltfptr[11]);
                        for (/*_#CURANK__L10*/ meltfnum[9] = 0;
                                               (/*_#CURANK__L10*/ meltfnum[9] >= 0) && (/*_#CURANK__L10*/ meltfnum[9] <  meltcit1__EACHTUP_ln);
                                               /*_#CURANK__L10*/ meltfnum[9]++)
                            {
                                /*_.CURARG__V18*/ meltfptr[16] = melt_multiple_nth((melt_ptr_t)(/*_.OARGS__V12*/ meltfptr[11]),  /*_#CURANK__L10*/ meltfnum[9]);




#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:3909:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*_#IS_NOT_A__L11*/ meltfnum[10] =
                                        !melt_is_instance_of((melt_ptr_t)(/*_.CURARG__V18*/ meltfptr[16]), (melt_ptr_t)((/*!CLASS_OBJINSTR*/ meltfrout->tabval[6])));;
                                    MELT_LOCATION("warmelt-outobj.melt:3909:/ cond");
                                    /*cond*/ if (/*_#IS_NOT_A__L11*/ meltfnum[10]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*_._IFELSE___V20*/ meltfptr[19] = (/*nil*/NULL);;
                                        }
                                    else
                                        {
                                            MELT_LOCATION("warmelt-outobj.melt:3909:/ cond.else");

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
                                                    argtab[0].meltbp_cstring =  "outputcod_objapply check curarg not objinstr";
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 3909;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CURARG__V18*/ meltfptr[16];
                                                    /*_.MELT_ASSERT_FAILURE_FUN__V21*/ meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IFELSE___V20*/ meltfptr[19] = /*_.MELT_ASSERT_FAILURE_FUN__V21*/ meltfptr[20];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:3909:/ clear");
                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V21*/ meltfptr[20] = 0 ;
                                            }
                                            ;
                                        }
                                    ;
                                    /*_.IFCPP___V19*/ meltfptr[18] = /*_._IFELSE___V20*/ meltfptr[19];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:3909:/ clear");
                                    /*clear*/ /*_#IS_NOT_A__L11*/ meltfnum[10] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IFELSE___V20*/ meltfptr[19] = 0 ;
                                }

#else /*MELT_HAVE_DEBUG*/
                                /*^cppif.else*/
                                /*_.IFCPP___V19*/ meltfptr[18] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                /*^compute*/
                                /*_#gtI__L12*/ meltfnum[10] =
                                    ((/*_#CURANK__L10*/ meltfnum[9]) > (0));;
                                MELT_LOCATION("warmelt-outobj.melt:3910:/ cond");
                                /*cond*/ if (/*_#gtI__L12*/ meltfnum[10]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-outobj.melt:3911:/ quasiblock");



                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*^msend*/
                                            /*msend*/
                                            {
                                                union meltparam_un argtab[1];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^ojbmsend.arg*/
                                                argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
                                                /*_.CURCTYP__V22*/ meltfptr[20] = meltgc_send((melt_ptr_t)(/*_.CURARG__V18*/ meltfptr[16]), (melt_ptr_t)((/*!GET_CTYPE*/ meltfrout->tabval[7])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;

#if MELT_HAVE_DEBUG
                                            MELT_LOCATION("warmelt-outobj.melt:3912:/ cppif.then");
                                            /*^block*/
                                            /*anyblock*/
                                            {


                                                MELT_CHECK_SIGNAL();
                                                ;
                                                /*_#IS_A__L13*/ meltfnum[12] =
                                                    melt_is_instance_of((melt_ptr_t)(/*_.CURCTYP__V22*/ meltfptr[20]), (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[8])));;
                                                MELT_LOCATION("warmelt-outobj.melt:3912:/ cond");
                                                /*cond*/ if (/*_#IS_A__L13*/ meltfnum[12]) /*then*/
                                                    {
                                                        /*^cond.then*/
                                                        /*_._IFELSE___V24*/ meltfptr[23] = (/*nil*/NULL);;
                                                    }
                                                else
                                                    {
                                                        MELT_LOCATION("warmelt-outobj.melt:3912:/ cond.else");

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
                                                                argtab[0].meltbp_cstring =  "check curctyp";
                                                                /*^apply.arg*/
                                                                argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                                /*^apply.arg*/
                                                                argtab[2].meltbp_long = 3912;
                                                                /*^apply.arg*/
                                                                argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CURCTYP__V22*/ meltfptr[20];
                                                                /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                            }
                                                            ;
                                                            /*_._IFELSE___V24*/ meltfptr[23] = /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24];;
                                                            /*epilog*/

                                                            MELT_LOCATION("warmelt-outobj.melt:3912:/ clear");
                                                            /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] = 0 ;
                                                        }
                                                        ;
                                                    }
                                                ;
                                                /*_.IFCPP___V23*/ meltfptr[19] = /*_._IFELSE___V24*/ meltfptr[23];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:3912:/ clear");
                                                /*clear*/ /*_#IS_A__L13*/ meltfnum[12] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_._IFELSE___V24*/ meltfptr[23] = 0 ;
                                            }

#else /*MELT_HAVE_DEBUG*/
                                            /*^cppif.else*/
                                            /*_.IFCPP___V23*/ meltfptr[19] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                            ;
                                            /*^compute*/
                                            /*_#GET_INT__L14*/ meltfnum[12] =
                                                (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V14*/ meltfptr[13])));;

                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-outobj.melt:3913:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[3];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                /*^apply.arg*/
                                                argtab[1].meltbp_long = /*_#GET_INT__L14*/ meltfnum[12];
                                                /*^apply.arg*/
                                                argtab[2].meltbp_cstring =  "apply.arg";
                                                /*_.OUTPUT_LOCATION__V26*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.ALOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                            MELT_LOCATION("warmelt-outobj.melt:3914:/ getslot");
                                            {
                                                melt_ptr_t slot=NULL, obj=NULL;
                                                obj = (melt_ptr_t)(/*_.CURCTYP__V22*/ meltfptr[20]) /*=obj*/;
                                                melt_object_get_field(slot,obj, 6, "CTYPE_PARSTRING");
                                                /*_.CTYPE_PARSTRING__V27*/ meltfptr[23] = slot;
                                            };
                                            ;

                                            {
                                                /*^locexp*/
                                                meltgc_append_list((melt_ptr_t)(/*_.PARAMDESCLIST__V13*/ meltfptr[12]), (melt_ptr_t)(/*_.CTYPE_PARSTRING__V27*/ meltfptr[23]));
                                            }
                                            ;

                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*_#NULL__L15*/ meltfnum[14] =
                                                (/*null*/(/*_.CURARG__V18*/ meltfptr[16]) == NULL);;
                                            MELT_LOCATION("warmelt-outobj.melt:3915:/ cond");
                                            /*cond*/ if (/*_#NULL__L15*/ meltfnum[14]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        {
                                                            MELT_LOCATION("warmelt-outobj.melt:3916:/ locexp");
                                                            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "argtab["));
                                                        }
                                                        ;
                                                        /*_#miI__L16*/ meltfnum[15] =
                                                            ((/*_#CURANK__L10*/ meltfnum[9]) - (1));;

                                                        {
                                                            MELT_LOCATION("warmelt-outobj.melt:3917:/ locexp");
                                                            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#miI__L16*/ meltfnum[15]));
                                                        }
                                                        ;

                                                        {
                                                            MELT_LOCATION("warmelt-outobj.melt:3918:/ locexp");
                                                            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "]."));
                                                        }
                                                        ;

                                                        {
                                                            MELT_LOCATION("warmelt-outobj.melt:3919:/ locexp");
                                                            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "meltbp_aptr = (melt_ptr_t*)NULL"));
                                                        }
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:3915:/ quasiblock");


                                                        /*epilog*/

                                                        /*^clear*/
                                                        /*clear*/ /*_#miI__L16*/ meltfnum[15] = 0 ;
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
                                                        /*_#IS_A__L17*/ meltfnum[15] =
                                                            melt_is_instance_of((melt_ptr_t)(/*_.CURARG__V18*/ meltfptr[16]), (melt_ptr_t)((/*!CLASS_OBJNIL*/ meltfrout->tabval[9])));;
                                                        MELT_LOCATION("warmelt-outobj.melt:3920:/ cond");
                                                        /*cond*/ if (/*_#IS_A__L17*/ meltfnum[15]) /*then*/
                                                            {
                                                                /*^cond.then*/
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {


                                                                    {
                                                                        MELT_LOCATION("warmelt-outobj.melt:3921:/ locexp");
                                                                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "argtab["));
                                                                    }
                                                                    ;
                                                                    /*_#miI__L18*/ meltfnum[17] =
                                                                        ((/*_#CURANK__L10*/ meltfnum[9]) - (1));;

                                                                    {
                                                                        MELT_LOCATION("warmelt-outobj.melt:3922:/ locexp");
                                                                        meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#miI__L18*/ meltfnum[17]));
                                                                    }
                                                                    ;

                                                                    {
                                                                        MELT_LOCATION("warmelt-outobj.melt:3923:/ locexp");
                                                                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "]."));
                                                                    }
                                                                    ;

                                                                    {
                                                                        MELT_LOCATION("warmelt-outobj.melt:3924:/ locexp");
                                                                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "meltbp_aptr = /*nil*/(melt_ptr_t*)NULL"));
                                                                    }
                                                                    ;
                                                                    MELT_LOCATION("warmelt-outobj.melt:3920:/ quasiblock");


                                                                    /*epilog*/

                                                                    /*^clear*/
                                                                    /*clear*/ /*_#miI__L18*/ meltfnum[17] = 0 ;
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
                                                                    /*_#eqeq__L19*/ meltfnum[17] =
                                                                        ((/*_.CURCTYP__V22*/ meltfptr[20]) == ((/*!CTYPE_VALUE*/ meltfrout->tabval[10])));;
                                                                    MELT_LOCATION("warmelt-outobj.melt:3925:/ cond");
                                                                    /*cond*/ if (/*_#eqeq__L19*/ meltfnum[17]) /*then*/
                                                                        {
                                                                            /*^cond.then*/
                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {


                                                                                MELT_CHECK_SIGNAL();
                                                                                ;

                                                                                MELT_CHECK_SIGNAL();
                                                                                ;
                                                                                /*_#IS_NOT_A__L20*/ meltfnum[19] =
                                                                                    !melt_is_instance_of((melt_ptr_t)(/*_.CURARG__V18*/ meltfptr[16]), (melt_ptr_t)((/*!CLASS_OBJLOCV*/ meltfrout->tabval[11])));;
                                                                                MELT_LOCATION("warmelt-outobj.melt:3926:/ cond");
                                                                                /*cond*/ if (/*_#IS_NOT_A__L20*/ meltfnum[19]) /*then*/
                                                                                    {
                                                                                        /*^cond.then*/
                                                                                        /*^block*/
                                                                                        /*anyblock*/
                                                                                        {


                                                                                            MELT_CHECK_SIGNAL();
                                                                                            ;
                                                                                            /*_#IS_NOT_A__L22*/ meltfnum[21] =
                                                                                                !melt_is_instance_of((melt_ptr_t)(/*_.CURARG__V18*/ meltfptr[16]), (melt_ptr_t)((/*!CLASS_OBJCLOCCV*/ meltfrout->tabval[12])));;
                                                                                            MELT_LOCATION("warmelt-outobj.melt:3926:/ cond");
                                                                                            /*cond*/ if (/*_#IS_NOT_A__L22*/ meltfnum[21]) /*then*/
                                                                                                {
                                                                                                    /*^cond.then*/
                                                                                                    /*^block*/
                                                                                                    /*anyblock*/
                                                                                                    {

                                                                                                        /*_#IS_NOT_A__L24*/ meltfnum[23] =
                                                                                                            !melt_is_instance_of((melt_ptr_t)(/*_.CURARG__V18*/ meltfptr[16]), (melt_ptr_t)((/*!CLASS_OBJINITELEM*/ meltfrout->tabval[13])));;
                                                                                                        /*^compute*/
                                                                                                        /*_#_IF___L23*/ meltfnum[22] = /*_#IS_NOT_A__L24*/ meltfnum[23];;
                                                                                                        /*epilog*/

                                                                                                        MELT_LOCATION("warmelt-outobj.melt:3926:/ clear");
                                                                                                        /*clear*/ /*_#IS_NOT_A__L24*/ meltfnum[23] = 0 ;
                                                                                                    }
                                                                                                    ;
                                                                                                }
                                                                                            else    /*^cond.else*/
                                                                                                {

                                                                                                    /*_#_IF___L23*/ meltfnum[22] = 0;;
                                                                                                }
                                                                                            ;
                                                                                            /*^compute*/
                                                                                            /*_#_IF___L21*/ meltfnum[20] = /*_#_IF___L23*/ meltfnum[22];;
                                                                                            /*epilog*/

                                                                                            MELT_LOCATION("warmelt-outobj.melt:3926:/ clear");
                                                                                            /*clear*/ /*_#IS_NOT_A__L22*/ meltfnum[21] = 0 ;
                                                                                            /*^clear*/
                                                                                            /*clear*/ /*_#_IF___L23*/ meltfnum[22] = 0 ;
                                                                                        }
                                                                                        ;
                                                                                    }
                                                                                else    /*^cond.else*/
                                                                                    {

                                                                                        /*_#_IF___L21*/ meltfnum[20] = 0;;
                                                                                    }
                                                                                ;
                                                                                MELT_LOCATION("warmelt-outobj.melt:3926:/ cond");
                                                                                /*cond*/ if (/*_#_IF___L21*/ meltfnum[20]) /*then*/
                                                                                    {
                                                                                        /*^cond.then*/
                                                                                        /*^block*/
                                                                                        /*anyblock*/
                                                                                        {


                                                                                            {
                                                                                                MELT_LOCATION("warmelt-outobj.melt:3929:/ locexp");
                                                                                                /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "if (("));
                                                                                            }
                                                                                            ;
                                                                                            /*_#GET_INT__L25*/ meltfnum[23] =
                                                                                                (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V14*/ meltfptr[13])));;

                                                                                            MELT_CHECK_SIGNAL();
                                                                                            ;
                                                                                            MELT_LOCATION("warmelt-outobj.melt:3930:/ msend");
                                                                                            /*msend*/
                                                                                            {
                                                                                                union meltparam_un argtab[3];
                                                                                                memset(&argtab, 0, sizeof(argtab));
                                                                                                /*^ojbmsend.arg*/
                                                                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                                                                                /*^ojbmsend.arg*/
                                                                                                argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                                                                /*^ojbmsend.arg*/
                                                                                                argtab[2].meltbp_long = /*_#GET_INT__L25*/ meltfnum[23];
                                                                                                /*_.OUTPUT_C_CODE__V31*/ meltfptr[30] = meltgc_send((melt_ptr_t)(/*_.CURARG__V18*/ meltfptr[16]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[14])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                                                            }
                                                                                            ;
                                                                                            /*_.DISCRIM__V32*/ meltfptr[31] =
                                                                                                ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.CURARG__V18*/ meltfptr[16]))));;
                                                                                            MELT_LOCATION("warmelt-outobj.melt:3931:/ cond");
                                                                                            /*cond*/ if (
                                                                                                /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.DISCRIM__V32*/ meltfptr[31]),
                                                                                                                              (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[16])))
                                                                                            ) /*then*/
                                                                                                {
                                                                                                    /*^cond.then*/
                                                                                                    /*^getslot*/
                                                                                                    {
                                                                                                        melt_ptr_t slot=NULL, obj=NULL;
                                                                                                        obj = (melt_ptr_t)(/*_.DISCRIM__V32*/ meltfptr[31]) /*=obj*/;
                                                                                                        melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                                                                        /*_.NAMED_NAME__V33*/ meltfptr[32] = slot;
                                                                                                    };
                                                                                                    ;
                                                                                                }
                                                                                            else    /*^cond.else*/
                                                                                                {

                                                                                                    /*_.NAMED_NAME__V33*/ meltfptr[32] =  /*reallynil*/ NULL ;;
                                                                                                }
                                                                                            ;

                                                                                            MELT_CHECK_SIGNAL();
                                                                                            ;
                                                                                            MELT_LOCATION("warmelt-outobj.melt:3931:/ apply");
                                                                                            /*apply*/
                                                                                            {
                                                                                                union meltparam_un argtab[3];
                                                                                                memset(&argtab, 0, sizeof(argtab));
                                                                                                /*^apply.arg*/
                                                                                                argtab[0].meltbp_cstring =  ") != NULL) /*objapply no nullval ";
                                                                                                /*^apply.arg*/
                                                                                                argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.NAMED_NAME__V33*/ meltfptr[32];
                                                                                                /*^apply.arg*/
                                                                                                argtab[2].meltbp_cstring =  "*/";
                                                                                                /*_.ADD2OUT__V34*/ meltfptr[33] =  melt_apply ((meltclosure_ptr_t)((/*!ADD2OUT*/ meltfrout->tabval[15])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                                                                            }
                                                                                            ;
                                                                                            /*_#plI__L26*/ meltfnum[21] =
                                                                                                ((2) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                                                                                            {
                                                                                                MELT_LOCATION("warmelt-outobj.melt:3932:/ locexp");
                                                                                                meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L26*/ meltfnum[21]), 0);
                                                                                            }
                                                                                            ;
                                                                                            MELT_LOCATION("warmelt-outobj.melt:3926:/ quasiblock");


                                                                                            /*epilog*/

                                                                                            /*^clear*/
                                                                                            /*clear*/ /*_#GET_INT__L25*/ meltfnum[23] = 0 ;
                                                                                            /*^clear*/
                                                                                            /*clear*/ /*_.OUTPUT_C_CODE__V31*/ meltfptr[30] = 0 ;
                                                                                            /*^clear*/
                                                                                            /*clear*/ /*_.DISCRIM__V32*/ meltfptr[31] = 0 ;
                                                                                            /*^clear*/
                                                                                            /*clear*/ /*_.NAMED_NAME__V33*/ meltfptr[32] = 0 ;
                                                                                            /*^clear*/
                                                                                            /*clear*/ /*_.ADD2OUT__V34*/ meltfptr[33] = 0 ;
                                                                                            /*^clear*/
                                                                                            /*clear*/ /*_#plI__L26*/ meltfnum[21] = 0 ;
                                                                                        }
                                                                                        ;
                                                                                    } /*noelse*/
                                                                                ;

                                                                                {
                                                                                    MELT_LOCATION("warmelt-outobj.melt:3933:/ locexp");
                                                                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "argtab["));
                                                                                }
                                                                                ;
                                                                                /*_#miI__L27*/ meltfnum[22] =
                                                                                    ((/*_#CURANK__L10*/ meltfnum[9]) - (1));;

                                                                                {
                                                                                    MELT_LOCATION("warmelt-outobj.melt:3934:/ locexp");
                                                                                    meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#miI__L27*/ meltfnum[22]));
                                                                                }
                                                                                ;

                                                                                {
                                                                                    MELT_LOCATION("warmelt-outobj.melt:3935:/ locexp");
                                                                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "]."));
                                                                                }
                                                                                ;

                                                                                {
                                                                                    MELT_LOCATION("warmelt-outobj.melt:3936:/ locexp");
                                                                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "meltbp_aptr = (melt_ptr_t*) &"));
                                                                                }
                                                                                ;
                                                                                /*_#GET_INT__L28*/ meltfnum[23] =
                                                                                    (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V14*/ meltfptr[13])));;

                                                                                MELT_CHECK_SIGNAL();
                                                                                ;
                                                                                MELT_LOCATION("warmelt-outobj.melt:3937:/ msend");
                                                                                /*msend*/
                                                                                {
                                                                                    union meltparam_un argtab[3];
                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                    /*^ojbmsend.arg*/
                                                                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                                                                    /*^ojbmsend.arg*/
                                                                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                                                    /*^ojbmsend.arg*/
                                                                                    argtab[2].meltbp_long = /*_#GET_INT__L28*/ meltfnum[23];
                                                                                    /*_.OUTPUT_C_CODE__V35*/ meltfptr[30] = meltgc_send((melt_ptr_t)(/*_.CURARG__V18*/ meltfptr[16]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[14])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                                                }
                                                                                ;
                                                                                MELT_LOCATION("warmelt-outobj.melt:3925:/ quasiblock");


                                                                                /*_.PROGN___V36*/ meltfptr[31] = /*_.OUTPUT_C_CODE__V35*/ meltfptr[30];;
                                                                                /*^compute*/
                                                                                /*_._IFELSE___V30*/ meltfptr[29] = /*_.PROGN___V36*/ meltfptr[31];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-outobj.melt:3925:/ clear");
                                                                                /*clear*/ /*_#IS_NOT_A__L20*/ meltfnum[19] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_#_IF___L21*/ meltfnum[20] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_#miI__L27*/ meltfnum[22] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_#GET_INT__L28*/ meltfnum[23] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.OUTPUT_C_CODE__V35*/ meltfptr[30] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.PROGN___V36*/ meltfptr[31] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    else    /*^cond.else*/
                                                                        {

                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {


                                                                                {
                                                                                    MELT_LOCATION("warmelt-outobj.melt:3940:/ locexp");
                                                                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "argtab["));
                                                                                }
                                                                                ;
                                                                                /*_#miI__L29*/ meltfnum[21] =
                                                                                    ((/*_#CURANK__L10*/ meltfnum[9]) - (1));;

                                                                                {
                                                                                    MELT_LOCATION("warmelt-outobj.melt:3941:/ locexp");
                                                                                    meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#miI__L29*/ meltfnum[21]));
                                                                                }
                                                                                ;

                                                                                {
                                                                                    MELT_LOCATION("warmelt-outobj.melt:3942:/ locexp");
                                                                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "]."));
                                                                                }
                                                                                ;
                                                                                MELT_LOCATION("warmelt-outobj.melt:3943:/ getslot");
                                                                                {
                                                                                    melt_ptr_t slot=NULL, obj=NULL;
                                                                                    obj = (melt_ptr_t)(/*_.CURCTYP__V22*/ meltfptr[20]) /*=obj*/;
                                                                                    melt_object_get_field(slot,obj, 7, "CTYPE_ARGFIELD");
                                                                                    /*_.CTYPE_ARGFIELD__V37*/ meltfptr[32] = slot;
                                                                                };
                                                                                ;

                                                                                {
                                                                                    /*^locexp*/
                                                                                    /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                                                                                          melt_string_str((melt_ptr_t)(/*_.CTYPE_ARGFIELD__V37*/ meltfptr[32])));
                                                                                }
                                                                                ;

                                                                                {
                                                                                    MELT_LOCATION("warmelt-outobj.melt:3944:/ locexp");
                                                                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = "));
                                                                                }
                                                                                ;
                                                                                /*_#GET_INT__L30*/ meltfnum[19] =
                                                                                    (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V14*/ meltfptr[13])));;

                                                                                MELT_CHECK_SIGNAL();
                                                                                ;
                                                                                MELT_LOCATION("warmelt-outobj.melt:3945:/ msend");
                                                                                /*msend*/
                                                                                {
                                                                                    union meltparam_un argtab[3];
                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                    /*^ojbmsend.arg*/
                                                                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                                                                    /*^ojbmsend.arg*/
                                                                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                                                    /*^ojbmsend.arg*/
                                                                                    argtab[2].meltbp_long = /*_#GET_INT__L30*/ meltfnum[19];
                                                                                    /*_.OUTPUT_C_CODE__V38*/ meltfptr[33] = meltgc_send((melt_ptr_t)(/*_.CURARG__V18*/ meltfptr[16]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[14])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                                                }
                                                                                ;
                                                                                MELT_LOCATION("warmelt-outobj.melt:3939:/ quasiblock");


                                                                                /*_.PROGN___V39*/ meltfptr[30] = /*_.OUTPUT_C_CODE__V38*/ meltfptr[33];;
                                                                                /*^compute*/
                                                                                /*_._IFELSE___V30*/ meltfptr[29] = /*_.PROGN___V39*/ meltfptr[30];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-outobj.melt:3925:/ clear");
                                                                                /*clear*/ /*_#miI__L29*/ meltfnum[21] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.CTYPE_ARGFIELD__V37*/ meltfptr[32] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_#GET_INT__L30*/ meltfnum[19] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.OUTPUT_C_CODE__V38*/ meltfptr[33] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.PROGN___V39*/ meltfptr[30] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    ;
                                                                    /*_._IFELSE___V29*/ meltfptr[28] = /*_._IFELSE___V30*/ meltfptr[29];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-outobj.melt:3920:/ clear");
                                                                    /*clear*/ /*_#eqeq__L19*/ meltfnum[17] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_._IFELSE___V30*/ meltfptr[29] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        ;
                                                        /*_._IFELSE___V28*/ meltfptr[27] = /*_._IFELSE___V29*/ meltfptr[28];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-outobj.melt:3915:/ clear");
                                                        /*clear*/ /*_#IS_A__L17*/ meltfnum[15] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_._IFELSE___V29*/ meltfptr[28] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;

                                            {
                                                MELT_LOCATION("warmelt-outobj.melt:3947:/ locexp");
                                                /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
                                            }
                                            ;
                                            /*_#GET_INT__L31*/ meltfnum[20] =
                                                (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V14*/ meltfptr[13])));;

                                            {
                                                MELT_LOCATION("warmelt-outobj.melt:3948:/ locexp");
                                                meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L31*/ meltfnum[20]), 0);
                                            }
                                            ;

                                            MELT_LOCATION("warmelt-outobj.melt:3911:/ clear");
                                            /*clear*/ /*_.CURCTYP__V22*/ meltfptr[20] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.IFCPP___V23*/ meltfptr[19] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_#GET_INT__L14*/ meltfnum[12] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.OUTPUT_LOCATION__V26*/ meltfptr[24] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.CTYPE_PARSTRING__V27*/ meltfptr[23] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_#NULL__L15*/ meltfnum[14] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_._IFELSE___V28*/ meltfptr[27] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_#GET_INT__L31*/ meltfnum[20] = 0 ;
                                            /*epilog*/
                                        }
                                        ;
                                    } /*noelse*/
                                ;

#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:3950:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*_#STRBUF_USEDLENGTH__L32*/ meltfnum[22] =
                                        melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                                    MELT_LOCATION("warmelt-outobj.melt:3951:/ cond");
                                    /*cond*/ if (
                                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[17])),
                                                                      (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[18])))
                                    ) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*^getslot*/
                                            {
                                                melt_ptr_t slot=NULL, obj=NULL;
                                                obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[17])) /*=obj*/;
                                                melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                                                /*_.REFERENCED_VALUE__V41*/ meltfptr[32] = slot;
                                            };
                                            ;
                                        }
                                    else    /*^cond.else*/
                                        {

                                            /*_.REFERENCED_VALUE__V41*/ meltfptr[32] =  /*reallynil*/ NULL ;;
                                        }
                                    ;
                                    /*^compute*/
                                    /*_#GET_INT__L33*/ meltfnum[23] =
                                        (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V41*/ meltfptr[32])));;
                                    /*^compute*/
                                    /*_#ltI__L34*/ meltfnum[21] =
                                        ((/*_#STRBUF_USEDLENGTH__L32*/ meltfnum[22]) < (/*_#GET_INT__L33*/ meltfnum[23]));;
                                    MELT_LOCATION("warmelt-outobj.melt:3950:/ cond");
                                    /*cond*/ if (/*_#ltI__L34*/ meltfnum[21]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*_._IFELSE___V42*/ meltfptr[33] = (/*nil*/NULL);;
                                        }
                                    else
                                        {
                                            MELT_LOCATION("warmelt-outobj.melt:3950:/ cond.else");

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
                                                    argtab[2].meltbp_long = 3950;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                    /*_.MELT_ASSERT_FAILURE_FUN__V43*/ meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IFELSE___V42*/ meltfptr[33] = /*_.MELT_ASSERT_FAILURE_FUN__V43*/ meltfptr[30];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:3950:/ clear");
                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V43*/ meltfptr[30] = 0 ;
                                            }
                                            ;
                                        }
                                    ;
                                    /*_.IFCPP___V40*/ meltfptr[31] = /*_._IFELSE___V42*/ meltfptr[33];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:3950:/ clear");
                                    /*clear*/ /*_#STRBUF_USEDLENGTH__L32*/ meltfnum[22] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.REFERENCED_VALUE__V41*/ meltfptr[32] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#GET_INT__L33*/ meltfnum[23] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#ltI__L34*/ meltfnum[21] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IFELSE___V42*/ meltfptr[33] = 0 ;
                                }

#else /*MELT_HAVE_DEBUG*/
                                /*^cppif.else*/
                                /*_.IFCPP___V40*/ meltfptr[31] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                ;
                                if (/*_#CURANK__L10*/ meltfnum[9]<0) break;
                            } /* end  foreach_in_multiple meltcit1__EACHTUP */

                        /*citerepilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3906:/ clear");
                        /*clear*/ /*_.CURARG__V18*/ meltfptr[16] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#CURANK__L10*/ meltfnum[9] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.IFCPP___V19*/ meltfptr[18] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#gtI__L12*/ meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.IFCPP___V40*/ meltfptr[31] = 0 ;
                    } /*endciterblock FOREACH_IN_MULTIPLE*/
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3898:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_#miI__L7*/ meltfnum[6] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#plI__L8*/ meltfnum[7] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#plI__L9*/ meltfnum[8] = 0 ;
                }
                ;
            } /*noelse*/
        ;
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit2__EACHLIST */
            for (/*_.CURPAIR__V44*/ meltfptr[29] = melt_list_first( (melt_ptr_t)/*_.ADEST__V10*/ meltfptr[9]);
                                    melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V44*/ meltfptr[29]) == MELTOBMAG_PAIR;
                                    /*_.CURPAIR__V44*/ meltfptr[29] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V44*/ meltfptr[29]))
                {
                    /*_.CURDEST__V45*/ meltfptr[28] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V44*/ meltfptr[29]);


                    /*_#GET_INT__L35*/ meltfnum[19] =
                        (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V14*/ meltfptr[13])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:3958:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#GET_INT__L35*/ meltfnum[19];
                        /*_.OUTPUT_C_CODE__V46*/ meltfptr[20] = meltgc_send((melt_ptr_t)(/*_.CURDEST__V45*/ meltfptr[28]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[14])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:3959:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L36*/ meltfnum[17] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                        MELT_LOCATION("warmelt-outobj.melt:3960:/ cond");
                        /*cond*/ if (
                            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[17])),
                                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[18])))
                        ) /*then*/
                            {
                                /*^cond.then*/
                                /*^getslot*/
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[17])) /*=obj*/;
                                    melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                                    /*_.REFERENCED_VALUE__V48*/ meltfptr[24] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V48*/ meltfptr[24] =  /*reallynil*/ NULL ;;
                            }
                        ;
                        /*^compute*/
                        /*_#GET_INT__L37*/ meltfnum[15] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V48*/ meltfptr[24])));;
                        /*^compute*/
                        /*_#ltI__L38*/ meltfnum[12] =
                            ((/*_#STRBUF_USEDLENGTH__L36*/ meltfnum[17]) < (/*_#GET_INT__L37*/ meltfnum[15]));;
                        MELT_LOCATION("warmelt-outobj.melt:3959:/ cond");
                        /*cond*/ if (/*_#ltI__L38*/ meltfnum[12]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V49*/ meltfptr[23] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:3959:/ cond.else");

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
                                        argtab[0].meltbp_cstring =  "check limited implbuf";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 3959;
                                        /*_.MELT_ASSERT_FAILURE_FUN__V50*/ meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V49*/ meltfptr[23] = /*_.MELT_ASSERT_FAILURE_FUN__V50*/ meltfptr[27];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:3959:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V50*/ meltfptr[27] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V47*/ meltfptr[19] = /*_._IFELSE___V49*/ meltfptr[23];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3959:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L36*/ meltfnum[17] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V48*/ meltfptr[24] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L37*/ meltfnum[15] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L38*/ meltfnum[12] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V49*/ meltfptr[23] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V47*/ meltfptr[19] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3961:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = "));
                    }
                    ;
                } /* end foreach_pair_component_in_list meltcit2__EACHLIST */
            /*_.CURPAIR__V44*/ meltfptr[29] = NULL;
            /*_.CURDEST__V45*/ meltfptr[28] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:3955:/ clear");
            /*clear*/ /*_.CURPAIR__V44*/ meltfptr[29] = 0 ;
            /*^clear*/
            /*clear*/ /*_.CURDEST__V45*/ meltfptr[28] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L35*/ meltfnum[19] = 0 ;
            /*^clear*/
            /*clear*/ /*_.OUTPUT_C_CODE__V46*/ meltfptr[20] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V47*/ meltfptr[19] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OAPP__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V52*/ meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V51*/ meltfptr[30] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V52*/ meltfptr[32];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V52*/ meltfptr[32] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V51*/ meltfptr[30] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3964:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " melt_apply ((meltclosure_ptr_t)("));
        }
        ;
        /*_#plI__L39*/ meltfnum[14] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3965:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#plI__L39*/ meltfnum[14];
            /*_.OUTPUT_C_CODE__V53*/ meltfptr[33] = meltgc_send((melt_ptr_t)(/*_.OCLOS__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[14])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3966:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "), (melt_ptr_t)("));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:3968:/ quasiblock");


        /*_.FIRSTARG__V55*/ meltfptr[24] =
            (melt_multiple_nth((melt_ptr_t)(/*_.OARGS__V12*/ meltfptr[11]), (0)));;
        /*^compute*/
        /*_#plI__L40*/ meltfnum[20] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3969:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#plI__L40*/ meltfnum[20];
            /*_.OUTPUT_C_CODE__V56*/ meltfptr[23] = meltgc_send((melt_ptr_t)(/*_.FIRSTARG__V55*/ meltfptr[24]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[14])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.LET___V54*/ meltfptr[27] = /*_.OUTPUT_C_CODE__V56*/ meltfptr[23];;

        MELT_LOCATION("warmelt-outobj.melt:3968:/ clear");
        /*clear*/ /*_.FIRSTARG__V55*/ meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L40*/ meltfnum[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V56*/ meltfptr[23] = 0 ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3971:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "), ("));
        }
        ;
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit3__EACHLIST */
            for (/*_.CURPAIR__V57*/ meltfptr[32] = melt_list_first( (melt_ptr_t)/*_.PARAMDESCLIST__V13*/ meltfptr[12]);
                                    melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V57*/ meltfptr[32]) == MELTOBMAG_PAIR;
                                    /*_.CURPAIR__V57*/ meltfptr[32] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V57*/ meltfptr[32]))
                {
                    /*_.PARD__V58*/ meltfptr[24] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V57*/ meltfptr[32]);



                    {
                        MELT_LOCATION("warmelt-outobj.melt:3976:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                              melt_string_str((melt_ptr_t)(/*_.PARD__V58*/ meltfptr[24])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:3977:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " "));
                    }
                    ;
                } /* end foreach_pair_component_in_list meltcit3__EACHLIST */
            /*_.CURPAIR__V57*/ meltfptr[32] = NULL;
            /*_.PARD__V58*/ meltfptr[24] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:3973:/ clear");
            /*clear*/ /*_.CURPAIR__V57*/ meltfptr[32] = 0 ;
            /*^clear*/
            /*clear*/ /*_.PARD__V58*/ meltfptr[24] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3978:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "\"\"), "));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#gtI__L41*/ meltfnum[22] =
            ((/*_#NBARG__L3*/ meltfnum[1]) > (1));;
        MELT_LOCATION("warmelt-outobj.melt:3980:/ cond");
        /*cond*/ if (/*_#gtI__L41*/ meltfnum[22]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:3981:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "argtab,"));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:3980:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:3982:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "(union meltparam_un*)0,"));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3984:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " \"\", (union meltparam_un*)0"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3985:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
        }
        ;
        /*_#plI__L42*/ meltfnum[23] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

        {
            MELT_LOCATION("warmelt-outobj.melt:3986:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L42*/ meltfnum[23]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3987:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "}"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:3988:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:3989:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L43*/ meltfnum[21] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:3990:/ cond");
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[17])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[18])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*!BUFFER_LIMIT_CONT*/ meltfrout->tabval[17])) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                        /*_.REFERENCED_VALUE__V60*/ meltfptr[59] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V60*/ meltfptr[59] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L44*/ meltfnum[6] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V60*/ meltfptr[59])));;
            /*^compute*/
            /*_#ltI__L45*/ meltfnum[7] =
                ((/*_#STRBUF_USEDLENGTH__L43*/ meltfnum[21]) < (/*_#GET_INT__L44*/ meltfnum[6]));;
            MELT_LOCATION("warmelt-outobj.melt:3989:/ cond");
            /*cond*/ if (/*_#ltI__L45*/ meltfnum[7]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V61*/ meltfptr[60] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3989:/ cond.else");

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
                            argtab[2].meltbp_long = 3989;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V62*/ meltfptr[61] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V61*/ meltfptr[60] = /*_.MELT_ASSERT_FAILURE_FUN__V62*/ meltfptr[61];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3989:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V62*/ meltfptr[61] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V59*/ meltfptr[23] = /*_._IFELSE___V61*/ meltfptr[60];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3989:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L43*/ meltfnum[21] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V60*/ meltfptr[59] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L44*/ meltfnum[6] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L45*/ meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V61*/ meltfptr[60] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V59*/ meltfptr[23] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V59*/ meltfptr[23];;

        MELT_LOCATION("warmelt-outobj.melt:3885:/ clear");
        /*clear*/ /*_.ALOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADEST__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OCLOS__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OARGS__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NBARG__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PARAMDESCLIST__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L4*/ meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BOXDEPTHP1__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V16*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L5*/ meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L6*/ meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V51*/ meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L39*/ meltfnum[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V53*/ meltfptr[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V54*/ meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L41*/ meltfnum[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L42*/ meltfnum[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V59*/ meltfptr[23] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3883:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3883:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJAPPLY", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_87_WARMELTmiOUTOBJ_OUTPUCOD_OBJAPPLY*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND fromline 1752 */

    /** start of frame for meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND// fromline 1568
        : public Melt_CallFrameWithValues<61>
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
        MeltFrame_meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<61> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND), clos) {};
        MeltFrame_meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND() //the constructor fromline 1642
            : Melt_CallFrameWithValues<61> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<61> (fil,lin, sizeof(MeltFrame_meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND)) {};
        MeltFrame_meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<61> (fil,lin, sizeof(MeltFrame_meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND


    /** end of frame for meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND fromline 1697**/

    /* end of frame for routine meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND fromline 1756 */

    /* classy proc frame meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND */ MeltFrame_meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJMSEND", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:3997:/ getarg");
    /*_.OMSEND__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:3998:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OMSEND__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJMSEND*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:3998:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:3998:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check omsend";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 3998;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OMSEND__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:3998:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:3998:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:3999:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OMSEND__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.OLOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4000:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OMSEND__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBDI_DESTLIST");
            /*_.ODEST__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4001:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OMSEND__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBMSND_SEL");
            /*_.OSEL__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4002:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OMSEND__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OBMSND_RECV");
            /*_.ORECV__V12*/ meltfptr[11] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4003:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OMSEND__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 4, "OBMSND_ARGS");
            /*_.OARGS__V13*/ meltfptr[12] = slot;
        };
        ;
        /*_#NBARG__L3*/ meltfnum[1] =
            (melt_multiple_length((melt_ptr_t)(/*_.OARGS__V13*/ meltfptr[12])));;
        /*^compute*/
        /*_.PARAMDESCLIST__V14*/ meltfptr[13] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
        /*^compute*/
        /*_#plI__L4*/ meltfnum[3] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;
        /*^compute*/
        /*_.BOXDEPTHP1__V15*/ meltfptr[14] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[3])), (/*_#plI__L4*/ meltfnum[3])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4008:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "msend";
            /*_.OUTPUT_LOCATION__V16*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OMSEND__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V18*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V17*/ meltfptr[16] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V18*/ meltfptr[17];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V18*/ meltfptr[17] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V17*/ meltfptr[16] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4010:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*msend*/{"));
        }
        ;
        /*_#plI__L5*/ meltfnum[4] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

        {
            MELT_LOCATION("warmelt-outobj.melt:4011:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L5*/ meltfnum[4]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#gtI__L6*/ meltfnum[5] =
            ((/*_#NBARG__L3*/ meltfnum[1]) > (0));;
        MELT_LOCATION("warmelt-outobj.melt:4012:/ cond");
        /*cond*/ if (/*_#gtI__L6*/ meltfnum[5]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4016:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "union meltparam_un argtab["));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4017:/ locexp");
                        meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#NBARG__L3*/ meltfnum[1]));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4018:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "];"));
                    }
                    ;
                    /*_#plI__L7*/ meltfnum[6] =
                        ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4019:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L7*/ meltfnum[6]), 0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4020:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "memset(&argtab, 0, sizeof(argtab));"));
                    }
                    ;
                    /*_#plI__L8*/ meltfnum[7] =
                        ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4021:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L8*/ meltfnum[7]), 0);
                    }
                    ;
                    /*citerblock FOREACH_IN_MULTIPLE*/
                    {
                        /* start foreach_in_multiple meltcit1__EACHTUP */
                        long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.OARGS__V13*/ meltfptr[12]);
                        for (/*_#CURANK__L9*/ meltfnum[8] = 0;
                                              (/*_#CURANK__L9*/ meltfnum[8] >= 0) && (/*_#CURANK__L9*/ meltfnum[8] <  meltcit1__EACHTUP_ln);
                                              /*_#CURANK__L9*/ meltfnum[8]++)
                            {
                                /*_.CURARG__V19*/ meltfptr[17] = melt_multiple_nth((melt_ptr_t)(/*_.OARGS__V13*/ meltfptr[12]),  /*_#CURANK__L9*/ meltfnum[8]);



                                MELT_LOCATION("warmelt-outobj.melt:4026:/ quasiblock");



                                MELT_CHECK_SIGNAL();
                                ;
                                /*^msend*/
                                /*msend*/
                                {
                                    union meltparam_un argtab[1];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^ojbmsend.arg*/
                                    argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
                                    /*_.CURCTYP__V20*/ meltfptr[19] = meltgc_send((melt_ptr_t)(/*_.CURARG__V19*/ meltfptr[17]), (melt_ptr_t)((/*!GET_CTYPE*/ meltfrout->tabval[6])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:4027:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*_#IS_A__L10*/ meltfnum[9] =
                                        melt_is_instance_of((melt_ptr_t)(/*_.CURCTYP__V20*/ meltfptr[19]), (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[7])));;
                                    MELT_LOCATION("warmelt-outobj.melt:4027:/ cond");
                                    /*cond*/ if (/*_#IS_A__L10*/ meltfnum[9]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*_._IFELSE___V22*/ meltfptr[21] = (/*nil*/NULL);;
                                        }
                                    else
                                        {
                                            MELT_LOCATION("warmelt-outobj.melt:4027:/ cond.else");

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
                                                    argtab[0].meltbp_cstring =  "check curctyp";
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 4027;
                                                    /*_.MELT_ASSERT_FAILURE_FUN__V23*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IFELSE___V22*/ meltfptr[21] = /*_.MELT_ASSERT_FAILURE_FUN__V23*/ meltfptr[22];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:4027:/ clear");
                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V23*/ meltfptr[22] = 0 ;
                                            }
                                            ;
                                        }
                                    ;
                                    /*_.IFCPP___V21*/ meltfptr[20] = /*_._IFELSE___V22*/ meltfptr[21];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:4027:/ clear");
                                    /*clear*/ /*_#IS_A__L10*/ meltfnum[9] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IFELSE___V22*/ meltfptr[21] = 0 ;
                                }

#else /*MELT_HAVE_DEBUG*/
                                /*^cppif.else*/
                                /*_.IFCPP___V21*/ meltfptr[20] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                ;
                                /*^compute*/
                                /*_#GET_INT__L11*/ meltfnum[9] =
                                    (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V15*/ meltfptr[14])));;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:4028:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[3];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                    /*^apply.arg*/
                                    argtab[1].meltbp_long = /*_#GET_INT__L11*/ meltfnum[9];
                                    /*^apply.arg*/
                                    argtab[2].meltbp_cstring =  "ojbmsend.arg";
                                    /*_.OUTPUT_LOCATION__V24*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:4029:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "argtab["));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:4030:/ locexp");
                                    meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#CURANK__L9*/ meltfnum[8]));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:4031:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "]."));
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:4032:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.CURCTYP__V20*/ meltfptr[19]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 6, "CTYPE_PARSTRING");
                                    /*_.CTYPE_PARSTRING__V25*/ meltfptr[21] = slot;
                                };
                                ;

                                {
                                    /*^locexp*/
                                    meltgc_append_list((melt_ptr_t)(/*_.PARAMDESCLIST__V14*/ meltfptr[13]), (melt_ptr_t)(/*_.CTYPE_PARSTRING__V25*/ meltfptr[21]));
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                /*_#NULL__L12*/ meltfnum[11] =
                                    (/*null*/(/*_.CURARG__V19*/ meltfptr[17]) == NULL);;
                                MELT_LOCATION("warmelt-outobj.melt:4033:/ cond");
                                /*cond*/ if (/*_#NULL__L12*/ meltfnum[11]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {




                                            {
                                                MELT_LOCATION("warmelt-outobj.melt:4034:/ locexp");
                                                /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "meltbp_aptr = (melt_ptr_t*)NULL"));
                                            }
                                            ;
                                            /*clear*/ /*_._IFELSE___V26*/ meltfptr[25] = 0 ;
                                            /*epilog*/
                                        }
                                        ;
                                    }
                                else
                                    {
                                        MELT_LOCATION("warmelt-outobj.melt:4033:/ cond.else");

                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*_#IS_A__L13*/ meltfnum[12] =
                                                melt_is_instance_of((melt_ptr_t)(/*_.CURARG__V19*/ meltfptr[17]), (melt_ptr_t)((/*!CLASS_OBJNIL*/ meltfrout->tabval[8])));;
                                            MELT_LOCATION("warmelt-outobj.melt:4036:/ cond");
                                            /*cond*/ if (/*_#IS_A__L13*/ meltfnum[12]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {




                                                        {
                                                            MELT_LOCATION("warmelt-outobj.melt:4037:/ locexp");
                                                            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "meltbp_aptr = /*nil*/(melt_ptr_t*)NULL"));
                                                        }
                                                        ;
                                                        /*clear*/ /*_._IFELSE___V27*/ meltfptr[26] = 0 ;
                                                        /*epilog*/
                                                    }
                                                    ;
                                                }
                                            else
                                                {
                                                    MELT_LOCATION("warmelt-outobj.melt:4036:/ cond.else");

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        /*_#eqeq__L14*/ meltfnum[13] =
                                                            ((/*_.CURCTYP__V20*/ meltfptr[19]) == ((/*!CTYPE_VALUE*/ meltfrout->tabval[9])));;
                                                        MELT_LOCATION("warmelt-outobj.melt:4039:/ cond");
                                                        /*cond*/ if (/*_#eqeq__L14*/ meltfnum[13]) /*then*/
                                                            {
                                                                /*^cond.then*/
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {


                                                                    {
                                                                        MELT_LOCATION("warmelt-outobj.melt:4040:/ locexp");
                                                                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "meltbp_aptr = (melt_ptr_t*) &"));
                                                                    }
                                                                    ;
                                                                    /*_#GET_INT__L15*/ meltfnum[14] =
                                                                        (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V15*/ meltfptr[14])));;

                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    MELT_LOCATION("warmelt-outobj.melt:4041:/ msend");
                                                                    /*msend*/
                                                                    {
                                                                        union meltparam_un argtab[3];
                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                        /*^ojbmsend.arg*/
                                                                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                                                        /*^ojbmsend.arg*/
                                                                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                                        /*^ojbmsend.arg*/
                                                                        argtab[2].meltbp_long = /*_#GET_INT__L15*/ meltfnum[14];
                                                                        /*_.OUTPUT_C_CODE__V29*/ meltfptr[28] = meltgc_send((melt_ptr_t)(/*_.CURARG__V19*/ meltfptr[17]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[10])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                                    }
                                                                    ;
                                                                    MELT_LOCATION("warmelt-outobj.melt:4039:/ quasiblock");


                                                                    /*_.PROGN___V30*/ meltfptr[29] = /*_.OUTPUT_C_CODE__V29*/ meltfptr[28];;
                                                                    /*^compute*/
                                                                    /*_._IFELSE___V28*/ meltfptr[27] = /*_.PROGN___V30*/ meltfptr[29];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-outobj.melt:4039:/ clear");
                                                                    /*clear*/ /*_#GET_INT__L15*/ meltfnum[14] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.OUTPUT_C_CODE__V29*/ meltfptr[28] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.PROGN___V30*/ meltfptr[29] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        else    /*^cond.else*/
                                                            {

                                                                /*^block*/
                                                                /*anyblock*/
                                                                {


#if MELT_HAVE_DEBUG
                                                                    MELT_LOCATION("warmelt-outobj.melt:4044:/ cppif.then");
                                                                    /*^block*/
                                                                    /*anyblock*/
                                                                    {


                                                                        MELT_CHECK_SIGNAL();
                                                                        ;
                                                                        /*_#IS_MULTIPLE__L16*/ meltfnum[14] =
                                                                            (melt_magic_discr((melt_ptr_t)(/*_.CURARG__V19*/ meltfptr[17])) == MELTOBMAG_MULTIPLE);;
                                                                        /*^compute*/
                                                                        /*_#NOT__L17*/ meltfnum[16] =
                                                                            (!(/*_#IS_MULTIPLE__L16*/ meltfnum[14]));;
                                                                        MELT_LOCATION("warmelt-outobj.melt:4044:/ cond");
                                                                        /*cond*/ if (/*_#NOT__L17*/ meltfnum[16]) /*then*/
                                                                            {
                                                                                /*^cond.then*/
                                                                                /*_._IFELSE___V32*/ meltfptr[29] = (/*nil*/NULL);;
                                                                            }
                                                                        else
                                                                            {
                                                                                MELT_LOCATION("warmelt-outobj.melt:4044:/ cond.else");

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
                                                                                        argtab[0].meltbp_cstring =  "check curarg is not multiple";
                                                                                        /*^apply.arg*/
                                                                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                                                        /*^apply.arg*/
                                                                                        argtab[2].meltbp_long = 4044;
                                                                                        /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                                                    }
                                                                                    ;
                                                                                    /*_._IFELSE___V32*/ meltfptr[29] = /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[32];;
                                                                                    /*epilog*/

                                                                                    MELT_LOCATION("warmelt-outobj.melt:4044:/ clear");
                                                                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[32] = 0 ;
                                                                                }
                                                                                ;
                                                                            }
                                                                        ;
                                                                        /*_.IFCPP___V31*/ meltfptr[28] = /*_._IFELSE___V32*/ meltfptr[29];;
                                                                        /*epilog*/

                                                                        MELT_LOCATION("warmelt-outobj.melt:4044:/ clear");
                                                                        /*clear*/ /*_#IS_MULTIPLE__L16*/ meltfnum[14] = 0 ;
                                                                        /*^clear*/
                                                                        /*clear*/ /*_#NOT__L17*/ meltfnum[16] = 0 ;
                                                                        /*^clear*/
                                                                        /*clear*/ /*_._IFELSE___V32*/ meltfptr[29] = 0 ;
                                                                    }

#else /*MELT_HAVE_DEBUG*/
                                                                    /*^cppif.else*/
                                                                    /*_.IFCPP___V31*/ meltfptr[28] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                                                    ;
                                                                    MELT_LOCATION("warmelt-outobj.melt:4045:/ getslot");
                                                                    {
                                                                        melt_ptr_t slot=NULL, obj=NULL;
                                                                        obj = (melt_ptr_t)(/*_.CURCTYP__V20*/ meltfptr[19]) /*=obj*/;
                                                                        melt_object_get_field(slot,obj, 7, "CTYPE_ARGFIELD");
                                                                        /*_.CTYPE_ARGFIELD__V34*/ meltfptr[32] = slot;
                                                                    };
                                                                    ;

                                                                    {
                                                                        /*^locexp*/
                                                                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                                                                              melt_string_str((melt_ptr_t)(/*_.CTYPE_ARGFIELD__V34*/ meltfptr[32])));
                                                                    }
                                                                    ;

                                                                    {
                                                                        MELT_LOCATION("warmelt-outobj.melt:4046:/ locexp");
                                                                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = "));
                                                                    }
                                                                    ;
                                                                    /*_#GET_INT__L18*/ meltfnum[14] =
                                                                        (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V15*/ meltfptr[14])));;

                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    MELT_LOCATION("warmelt-outobj.melt:4047:/ msend");
                                                                    /*msend*/
                                                                    {
                                                                        union meltparam_un argtab[3];
                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                        /*^ojbmsend.arg*/
                                                                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                                                        /*^ojbmsend.arg*/
                                                                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                                        /*^ojbmsend.arg*/
                                                                        argtab[2].meltbp_long = /*_#GET_INT__L18*/ meltfnum[14];
                                                                        /*_.OUTPUT_C_CODE__V35*/ meltfptr[29] = meltgc_send((melt_ptr_t)(/*_.CURARG__V19*/ meltfptr[17]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[10])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                                    }
                                                                    ;
                                                                    MELT_LOCATION("warmelt-outobj.melt:4043:/ quasiblock");


                                                                    /*_.PROGN___V36*/ meltfptr[35] = /*_.OUTPUT_C_CODE__V35*/ meltfptr[29];;
                                                                    /*^compute*/
                                                                    /*_._IFELSE___V28*/ meltfptr[27] = /*_.PROGN___V36*/ meltfptr[35];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-outobj.melt:4039:/ clear");
                                                                    /*clear*/ /*_.IFCPP___V31*/ meltfptr[28] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.CTYPE_ARGFIELD__V34*/ meltfptr[32] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_#GET_INT__L18*/ meltfnum[14] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.OUTPUT_C_CODE__V35*/ meltfptr[29] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.PROGN___V36*/ meltfptr[35] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        ;
                                                        /*_._IFELSE___V27*/ meltfptr[26] = /*_._IFELSE___V28*/ meltfptr[27];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-outobj.melt:4036:/ clear");
                                                        /*clear*/ /*_#eqeq__L14*/ meltfnum[13] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_._IFELSE___V28*/ meltfptr[27] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;
                                            /*_._IFELSE___V26*/ meltfptr[25] = /*_._IFELSE___V27*/ meltfptr[26];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:4033:/ clear");
                                            /*clear*/ /*_#IS_A__L13*/ meltfnum[12] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_._IFELSE___V27*/ meltfptr[26] = 0 ;
                                        }
                                        ;
                                    }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:4049:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
                                }
                                ;

#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:4050:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*_#STRBUF_USEDLENGTH__L19*/ meltfnum[16] =
                                        melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                                    MELT_LOCATION("warmelt-outobj.melt:4051:/ cond");
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
                                                /*_.REFERENCED_VALUE__V38*/ meltfptr[32] = slot;
                                            };
                                            ;
                                        }
                                    else    /*^cond.else*/
                                        {

                                            /*_.REFERENCED_VALUE__V38*/ meltfptr[32] =  /*reallynil*/ NULL ;;
                                        }
                                    ;
                                    /*^compute*/
                                    /*_#GET_INT__L20*/ meltfnum[14] =
                                        (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V38*/ meltfptr[32])));;
                                    /*^compute*/
                                    /*_#ltI__L21*/ meltfnum[13] =
                                        ((/*_#STRBUF_USEDLENGTH__L19*/ meltfnum[16]) < (/*_#GET_INT__L20*/ meltfnum[14]));;
                                    MELT_LOCATION("warmelt-outobj.melt:4050:/ cond");
                                    /*cond*/ if (/*_#ltI__L21*/ meltfnum[13]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*_._IFELSE___V39*/ meltfptr[29] = (/*nil*/NULL);;
                                        }
                                    else
                                        {
                                            MELT_LOCATION("warmelt-outobj.melt:4050:/ cond.else");

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
                                                    argtab[2].meltbp_long = 4050;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                    /*_.MELT_ASSERT_FAILURE_FUN__V40*/ meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IFELSE___V39*/ meltfptr[29] = /*_.MELT_ASSERT_FAILURE_FUN__V40*/ meltfptr[35];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:4050:/ clear");
                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V40*/ meltfptr[35] = 0 ;
                                            }
                                            ;
                                        }
                                    ;
                                    /*_.IFCPP___V37*/ meltfptr[28] = /*_._IFELSE___V39*/ meltfptr[29];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:4050:/ clear");
                                    /*clear*/ /*_#STRBUF_USEDLENGTH__L19*/ meltfnum[16] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.REFERENCED_VALUE__V38*/ meltfptr[32] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#GET_INT__L20*/ meltfnum[14] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#ltI__L21*/ meltfnum[13] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IFELSE___V39*/ meltfptr[29] = 0 ;
                                }

#else /*MELT_HAVE_DEBUG*/
                                /*^cppif.else*/
                                /*_.IFCPP___V37*/ meltfptr[28] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                ;
                                /*^compute*/
                                /*_#GET_INT__L22*/ meltfnum[12] =
                                    (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V15*/ meltfptr[14])));;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:4052:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L22*/ meltfnum[12]), 0);
                                }
                                ;

                                MELT_LOCATION("warmelt-outobj.melt:4026:/ clear");
                                /*clear*/ /*_.CURCTYP__V20*/ meltfptr[19] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.IFCPP___V21*/ meltfptr[20] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#GET_INT__L11*/ meltfnum[9] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.OUTPUT_LOCATION__V24*/ meltfptr[22] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.CTYPE_PARSTRING__V25*/ meltfptr[21] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#NULL__L12*/ meltfnum[11] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V26*/ meltfptr[25] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.IFCPP___V37*/ meltfptr[28] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#GET_INT__L22*/ meltfnum[12] = 0 ;
                                if (/*_#CURANK__L9*/ meltfnum[8]<0) break;
                            } /* end  foreach_in_multiple meltcit1__EACHTUP */

                        /*citerepilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4023:/ clear");
                        /*clear*/ /*_.CURARG__V19*/ meltfptr[17] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#CURANK__L9*/ meltfnum[8] = 0 ;
                    } /*endciterblock FOREACH_IN_MULTIPLE*/
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4012:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_#plI__L7*/ meltfnum[6] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#plI__L8*/ meltfnum[7] = 0 ;
                }
                ;
            } /*noelse*/
        ;
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit2__EACHLIST */
            for (/*_.CURPAIR__V41*/ meltfptr[27] = melt_list_first( (melt_ptr_t)/*_.ODEST__V10*/ meltfptr[9]);
                                    melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V41*/ meltfptr[27]) == MELTOBMAG_PAIR;
                                    /*_.CURPAIR__V41*/ meltfptr[27] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V41*/ meltfptr[27]))
                {
                    /*_.CURDEST__V42*/ meltfptr[26] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V41*/ meltfptr[27]);


                    /*_#GET_INT__L23*/ meltfnum[16] =
                        (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V15*/ meltfptr[14])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4059:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#GET_INT__L23*/ meltfnum[16];
                        /*_.OUTPUT_C_CODE__V43*/ meltfptr[35] = meltgc_send((melt_ptr_t)(/*_.CURDEST__V42*/ meltfptr[26]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[10])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:4060:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#STRBUF_USEDLENGTH__L24*/ meltfnum[14] =
                            melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                        MELT_LOCATION("warmelt-outobj.melt:4061:/ cond");
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
                                    /*_.REFERENCED_VALUE__V45*/ meltfptr[29] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V45*/ meltfptr[29] =  /*reallynil*/ NULL ;;
                            }
                        ;
                        /*^compute*/
                        /*_#GET_INT__L25*/ meltfnum[13] =
                            (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V45*/ meltfptr[29])));;
                        /*^compute*/
                        /*_#ltI__L26*/ meltfnum[9] =
                            ((/*_#STRBUF_USEDLENGTH__L24*/ meltfnum[14]) < (/*_#GET_INT__L25*/ meltfnum[13]));;
                        MELT_LOCATION("warmelt-outobj.melt:4060:/ cond");
                        /*cond*/ if (/*_#ltI__L26*/ meltfnum[9]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V46*/ meltfptr[19] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:4060:/ cond.else");

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
                                        argtab[2].meltbp_long = 4060;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V47*/ meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V46*/ meltfptr[19] = /*_.MELT_ASSERT_FAILURE_FUN__V47*/ meltfptr[20];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:4060:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V47*/ meltfptr[20] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V44*/ meltfptr[32] = /*_._IFELSE___V46*/ meltfptr[19];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4060:/ clear");
                        /*clear*/ /*_#STRBUF_USEDLENGTH__L24*/ meltfnum[14] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V45*/ meltfptr[29] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#GET_INT__L25*/ meltfnum[13] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#ltI__L26*/ meltfnum[9] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V46*/ meltfptr[19] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V44*/ meltfptr[32] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4062:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = "));
                    }
                    ;
                } /* end foreach_pair_component_in_list meltcit2__EACHLIST */
            /*_.CURPAIR__V41*/ meltfptr[27] = NULL;
            /*_.CURDEST__V42*/ meltfptr[26] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:4056:/ clear");
            /*clear*/ /*_.CURPAIR__V41*/ meltfptr[27] = 0 ;
            /*^clear*/
            /*clear*/ /*_.CURDEST__V42*/ meltfptr[26] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L23*/ meltfnum[16] = 0 ;
            /*^clear*/
            /*clear*/ /*_.OUTPUT_C_CODE__V43*/ meltfptr[35] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V44*/ meltfptr[32] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4064:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L27*/ meltfnum[11] =
                (melt_magic_discr((melt_ptr_t)(/*_.ORECV__V12*/ meltfptr[11])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-outobj.melt:4064:/ cond");
            /*cond*/ if (/*_#IS_OBJECT__L27*/ meltfnum[11]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V49*/ meltfptr[21] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4064:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check orecv object";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4064;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ORECV__V12*/ meltfptr[11];
                            /*_.MELT_ASSERT_FAILURE_FUN__V50*/ meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V49*/ meltfptr[21] = /*_.MELT_ASSERT_FAILURE_FUN__V50*/ meltfptr[25];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4064:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V50*/ meltfptr[25] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V48*/ meltfptr[22] = /*_._IFELSE___V49*/ meltfptr[21];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4064:/ clear");
            /*clear*/ /*_#IS_OBJECT__L27*/ meltfnum[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V49*/ meltfptr[21] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V48*/ meltfptr[22] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4066:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "meltgc_send((melt_ptr_t)("));
        }
        ;
        /*_#plI__L28*/ meltfnum[12] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4067:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#plI__L28*/ meltfnum[12];
            /*_.OUTPUT_C_CODE__V51*/ meltfptr[28] = meltgc_send((melt_ptr_t)(/*_.ORECV__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[10])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4068:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "), (melt_ptr_t)("));
        }
        ;
        /*_#plI__L29*/ meltfnum[6] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4069:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#plI__L29*/ meltfnum[6];
            /*_.OUTPUT_C_CODE__V52*/ meltfptr[20] = meltgc_send((melt_ptr_t)(/*_.OSEL__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[10])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4070:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "), ("));
        }
        ;
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit3__EACHLIST */
            for (/*_.CURPAIR__V53*/ meltfptr[29] = melt_list_first( (melt_ptr_t)/*_.PARAMDESCLIST__V14*/ meltfptr[13]);
                                    melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V53*/ meltfptr[29]) == MELTOBMAG_PAIR;
                                    /*_.CURPAIR__V53*/ meltfptr[29] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V53*/ meltfptr[29]))
                {
                    /*_.PARD__V54*/ meltfptr[19] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V53*/ meltfptr[29]);



#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:4075:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*_#IS_STRING__L30*/ meltfnum[7] =
                            (melt_magic_discr((melt_ptr_t)(/*_.PARD__V54*/ meltfptr[19])) == MELTOBMAG_STRING);;
                        MELT_LOCATION("warmelt-outobj.melt:4075:/ cond");
                        /*cond*/ if (/*_#IS_STRING__L30*/ meltfnum[7]) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V56*/ meltfptr[21] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:4075:/ cond.else");

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
                                        argtab[0].meltbp_cstring =  "check pard";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 4075;
                                        /*_.MELT_ASSERT_FAILURE_FUN__V57*/ meltfptr[56] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V56*/ meltfptr[21] = /*_.MELT_ASSERT_FAILURE_FUN__V57*/ meltfptr[56];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:4075:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V57*/ meltfptr[56] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V55*/ meltfptr[25] = /*_._IFELSE___V56*/ meltfptr[21];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4075:/ clear");
                        /*clear*/ /*_#IS_STRING__L30*/ meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IFELSE___V56*/ meltfptr[21] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V55*/ meltfptr[25] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4076:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                              melt_string_str((melt_ptr_t)(/*_.PARD__V54*/ meltfptr[19])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4077:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " "));
                    }
                    ;
                } /* end foreach_pair_component_in_list meltcit3__EACHLIST */
            /*_.CURPAIR__V53*/ meltfptr[29] = NULL;
            /*_.PARD__V54*/ meltfptr[19] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:4072:/ clear");
            /*clear*/ /*_.CURPAIR__V53*/ meltfptr[29] = 0 ;
            /*^clear*/
            /*clear*/ /*_.PARD__V54*/ meltfptr[19] = 0 ;
            /*^clear*/
            /*clear*/ /*_.IFCPP___V55*/ meltfptr[25] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4078:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "\"\"), "));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#gtI__L31*/ meltfnum[14] =
            ((/*_#NBARG__L3*/ meltfnum[1]) > (0));;
        MELT_LOCATION("warmelt-outobj.melt:4079:/ cond");
        /*cond*/ if (/*_#gtI__L31*/ meltfnum[14]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4080:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "argtab,"));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:4079:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4081:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "(union meltparam_un*)0,"));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4083:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " \"\", (union meltparam_un*)0"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4084:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
        }
        ;
        /*_#plI__L32*/ meltfnum[13] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

        {
            MELT_LOCATION("warmelt-outobj.melt:4085:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L32*/ meltfnum[13]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4086:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "}"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4087:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4088:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L33*/ meltfnum[9] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:4089:/ cond");
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
                        /*_.REFERENCED_VALUE__V59*/ meltfptr[21] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V59*/ meltfptr[21] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L34*/ meltfnum[11] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V59*/ meltfptr[21])));;
            /*^compute*/
            /*_#ltI__L35*/ meltfnum[7] =
                ((/*_#STRBUF_USEDLENGTH__L33*/ meltfnum[9]) < (/*_#GET_INT__L34*/ meltfnum[11]));;
            MELT_LOCATION("warmelt-outobj.melt:4088:/ cond");
            /*cond*/ if (/*_#ltI__L35*/ meltfnum[7]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V60*/ meltfptr[59] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4088:/ cond.else");

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
                            argtab[2].meltbp_long = 4088;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V61*/ meltfptr[60] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V60*/ meltfptr[59] = /*_.MELT_ASSERT_FAILURE_FUN__V61*/ meltfptr[60];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4088:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V61*/ meltfptr[60] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V58*/ meltfptr[56] = /*_._IFELSE___V60*/ meltfptr[59];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4088:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L33*/ meltfnum[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V59*/ meltfptr[21] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L34*/ meltfnum[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L35*/ meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V60*/ meltfptr[59] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V58*/ meltfptr[56] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V58*/ meltfptr[56];;

        MELT_LOCATION("warmelt-outobj.melt:3999:/ clear");
        /*clear*/ /*_.OLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ODEST__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OSEL__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ORECV__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OARGS__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NBARG__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PARAMDESCLIST__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L4*/ meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BOXDEPTHP1__V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V16*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V17*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L5*/ meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L6*/ meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V48*/ meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L28*/ meltfnum[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V51*/ meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L29*/ meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V52*/ meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L31*/ meltfnum[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L32*/ meltfnum[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V58*/ meltfptr[56] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:3997:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:3997:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJMSEND", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_88_WARMELTmiOUTOBJ_OUTPUCOD_OBJMSEND*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY fromline 1752 */

    /** start of frame for meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY// fromline 1568
        : public Melt_CallFrameWithValues<76>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[51];
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
        MeltFrame_meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<76> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY), clos) {};
        MeltFrame_meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY() //the constructor fromline 1642
            : Melt_CallFrameWithValues<76> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<76> (fil,lin, sizeof(MeltFrame_meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY)) {};
        MeltFrame_meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<76> (fil,lin, sizeof(MeltFrame_meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY


    /** end of frame for meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY fromline 1697**/

    /* end of frame for routine meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY fromline 1756 */

    /* classy proc frame meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY */ MeltFrame_meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJMULTIAPPLY", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:4096:/ getarg");
    /*_.OAPP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:4097:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OAPP__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJMULTIAPPLY*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:4097:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4097:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oapp";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4097;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OAPP__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4097:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4097:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:4098:/ quasiblock");


        MELT_LOCATION("warmelt-outobj.melt:4099:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OAPP__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.ALOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4100:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OAPP__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBDI_DESTLIST");
            /*_.ADEST__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4101:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OAPP__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBAPP_CLOS");
            /*_.OCLOS__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4102:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OAPP__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OBAPP_ARGS");
            /*_.OARGS__V12*/ meltfptr[11] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4103:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OAPP__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 4, "OBMULTAPP_XRES");
            /*_.OXRES__V13*/ meltfptr[12] = slot;
        };
        ;
        /*_#NBARG__L3*/ meltfnum[1] =
            (melt_multiple_length((melt_ptr_t)(/*_.OARGS__V12*/ meltfptr[11])));;
        /*^compute*/
        /*_#NBXRES__L4*/ meltfnum[3] =
            (melt_multiple_length((melt_ptr_t)(/*_.OXRES__V13*/ meltfptr[12])));;
        /*^compute*/
        /*_.PARAMDESCLIST__V14*/ meltfptr[13] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
        /*^compute*/
        /*_.RESDESCLIST__V15*/ meltfptr[14] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
        /*^compute*/
        /*_#plI__L5*/ meltfnum[4] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;
        /*^compute*/
        /*_.BOXDEPTHP1__V16*/ meltfptr[15] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[3])), (/*_#plI__L5*/ meltfnum[4])));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4110:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_MULTIPLE_OR_NULL__L6*/ meltfnum[5] =
                ((/*_.OARGS__V12*/ meltfptr[11]) == NULL || (melt_unsafe_magic_discr((melt_ptr_t)(/*_.OARGS__V12*/ meltfptr[11])) == MELTOBMAG_MULTIPLE));;
            MELT_LOCATION("warmelt-outobj.melt:4110:/ cond");
            /*cond*/ if (/*_#IS_MULTIPLE_OR_NULL__L6*/ meltfnum[5]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V18*/ meltfptr[17] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4110:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oargs";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4110;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OARGS__V12*/ meltfptr[11];
                            /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V18*/ meltfptr[17] = /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4110:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V17*/ meltfptr[16] = /*_._IFELSE___V18*/ meltfptr[17];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4110:/ clear");
            /*clear*/ /*_#IS_MULTIPLE_OR_NULL__L6*/ meltfnum[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V18*/ meltfptr[17] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V17*/ meltfptr[16] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4111:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_MULTIPLE_OR_NULL__L7*/ meltfnum[5] =
                ((/*_.OXRES__V13*/ meltfptr[12]) == NULL || (melt_unsafe_magic_discr((melt_ptr_t)(/*_.OXRES__V13*/ meltfptr[12])) == MELTOBMAG_MULTIPLE));;
            MELT_LOCATION("warmelt-outobj.melt:4111:/ cond");
            /*cond*/ if (/*_#IS_MULTIPLE_OR_NULL__L7*/ meltfnum[5]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V21*/ meltfptr[17] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4111:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oxres";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4111;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OXRES__V13*/ meltfptr[12];
                            /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V21*/ meltfptr[17] = /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4111:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V22*/ meltfptr[21] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V20*/ meltfptr[18] = /*_._IFELSE___V21*/ meltfptr[17];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4111:/ clear");
            /*clear*/ /*_#IS_MULTIPLE_OR_NULL__L7*/ meltfnum[5] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:4112:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "multiapply";
            /*_.OUTPUT_LOCATION__V23*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.ALOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OAPP__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V25*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V24*/ meltfptr[17] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V25*/ meltfptr[24];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V25*/ meltfptr[24] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V24*/ meltfptr[17] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4114:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*multiapply "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4115:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#NBARG__L3*/ meltfnum[1]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4116:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "args, "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4117:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#NBXRES__L4*/ meltfnum[3]));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4118:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "x.res*/ "));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4119:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "{"));
        }
        ;
        /*_#plI__L8*/ meltfnum[5] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

        {
            MELT_LOCATION("warmelt-outobj.melt:4120:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L8*/ meltfnum[5]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#gtI__L9*/ meltfnum[8] =
            ((/*_#NBARG__L3*/ meltfnum[1]) > (1));;
        MELT_LOCATION("warmelt-outobj.melt:4121:/ cond");
        /*cond*/ if (/*_#gtI__L9*/ meltfnum[8]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4122:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "union meltparam_un argtab["));
                    }
                    ;
                    /*_#miI__L10*/ meltfnum[9] =
                        ((/*_#NBARG__L3*/ meltfnum[1]) - (1));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4123:/ locexp");
                        meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#miI__L10*/ meltfnum[9]));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4124:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "];"));
                    }
                    ;
                    /*_#plI__L11*/ meltfnum[10] =
                        ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4125:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L11*/ meltfnum[10]), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4121:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_#miI__L10*/ meltfnum[9] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#plI__L11*/ meltfnum[10] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#gtI__L12*/ meltfnum[9] =
            ((/*_#NBXRES__L4*/ meltfnum[3]) > (0));;
        MELT_LOCATION("warmelt-outobj.melt:4127:/ cond");
        /*cond*/ if (/*_#gtI__L12*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_#plI__L13*/ meltfnum[10] =
                        ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4128:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L13*/ meltfnum[10]), 0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4129:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "union meltparam_un restab["));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4130:/ locexp");
                        meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#NBXRES__L4*/ meltfnum[3]));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4131:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "];"));
                    }
                    ;
                    /*_#plI__L14*/ meltfnum[13] =
                        ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4132:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L14*/ meltfnum[13]), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4127:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_#plI__L13*/ meltfnum[10] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#plI__L14*/ meltfnum[13] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#gtI__L15*/ meltfnum[10] =
            ((/*_#NBXRES__L4*/ meltfnum[3]) > (0));;
        MELT_LOCATION("warmelt-outobj.melt:4134:/ cond");
        /*cond*/ if (/*_#gtI__L15*/ meltfnum[10]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4135:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "memset(&restab, 0, sizeof(restab));"));
                    }
                    ;
                    /*_#plI__L16*/ meltfnum[13] =
                        ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4136:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L16*/ meltfnum[13]), 0);
                    }
                    ;
                    /*citerblock FOREACH_IN_MULTIPLE*/
                    {
                        /* start foreach_in_multiple meltcit1__EACHTUP */
                        long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.OXRES__V13*/ meltfptr[12]);
                        for (/*_#CURANK__L17*/ meltfnum[16] = 0;
                                               (/*_#CURANK__L17*/ meltfnum[16] >= 0) && (/*_#CURANK__L17*/ meltfnum[16] <  meltcit1__EACHTUP_ln);
                                               /*_#CURANK__L17*/ meltfnum[16]++)
                            {
                                /*_.CURES__V26*/ meltfptr[24] = melt_multiple_nth((melt_ptr_t)(/*_.OXRES__V13*/ meltfptr[12]),  /*_#CURANK__L17*/ meltfnum[16]);



                                MELT_LOCATION("warmelt-outobj.melt:4141:/ quasiblock");



                                MELT_CHECK_SIGNAL();
                                ;
                                /*^msend*/
                                /*msend*/
                                {
                                    union meltparam_un argtab[1];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^ojbmsend.arg*/
                                    argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
                                    /*_.CURCTYP__V27*/ meltfptr[26] = meltgc_send((melt_ptr_t)(/*_.CURES__V26*/ meltfptr[24]), (melt_ptr_t)((/*!GET_CTYPE*/ meltfrout->tabval[6])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:4142:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.CURCTYP__V27*/ meltfptr[26]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 6, "CTYPE_PARSTRING");
                                    /*_.CTYPE_PARSTRING__V28*/ meltfptr[27] = slot;
                                };
                                ;

                                {
                                    /*^locexp*/
                                    meltgc_append_list((melt_ptr_t)(/*_.RESDESCLIST__V15*/ meltfptr[14]), (melt_ptr_t)(/*_.CTYPE_PARSTRING__V28*/ meltfptr[27]));
                                }
                                ;

                                MELT_LOCATION("warmelt-outobj.melt:4141:/ clear");
                                /*clear*/ /*_.CURCTYP__V27*/ meltfptr[26] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.CTYPE_PARSTRING__V28*/ meltfptr[27] = 0 ;
                                if (/*_#CURANK__L17*/ meltfnum[16]<0) break;
                            } /* end  foreach_in_multiple meltcit1__EACHTUP */

                        /*citerepilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4138:/ clear");
                        /*clear*/ /*_.CURES__V26*/ meltfptr[24] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#CURANK__L17*/ meltfnum[16] = 0 ;
                    } /*endciterblock FOREACH_IN_MULTIPLE*/
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4134:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_#plI__L16*/ meltfnum[13] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#gtI__L18*/ meltfnum[13] =
            ((/*_#NBARG__L3*/ meltfnum[1]) > (1));;
        MELT_LOCATION("warmelt-outobj.melt:4143:/ cond");
        /*cond*/ if (/*_#gtI__L18*/ meltfnum[13]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4144:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "memset(&argtab, 0, sizeof(argtab));"));
                    }
                    ;
                    /*_#plI__L19*/ meltfnum[18] =
                        ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4145:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L19*/ meltfnum[18]), 0);
                    }
                    ;
                    /*citerblock FOREACH_IN_MULTIPLE*/
                    {
                        /* start foreach_in_multiple meltcit2__EACHTUP */
                        long  meltcit2__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.OARGS__V12*/ meltfptr[11]);
                        for (/*_#CURANK__L20*/ meltfnum[19] = 0;
                                               (/*_#CURANK__L20*/ meltfnum[19] >= 0) && (/*_#CURANK__L20*/ meltfnum[19] <  meltcit2__EACHTUP_ln);
                                               /*_#CURANK__L20*/ meltfnum[19]++)
                            {
                                /*_.CURARG__V29*/ meltfptr[26] = melt_multiple_nth((melt_ptr_t)(/*_.OARGS__V12*/ meltfptr[11]),  /*_#CURANK__L20*/ meltfnum[19]);




                                MELT_CHECK_SIGNAL();
                                ;
                                /*_#gtI__L21*/ meltfnum[20] =
                                    ((/*_#CURANK__L20*/ meltfnum[19]) > (0));;
                                MELT_LOCATION("warmelt-outobj.melt:4150:/ cond");
                                /*cond*/ if (/*_#gtI__L21*/ meltfnum[20]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-outobj.melt:4151:/ quasiblock");



                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*^msend*/
                                            /*msend*/
                                            {
                                                union meltparam_un argtab[1];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^ojbmsend.arg*/
                                                argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
                                                /*_.CURCTYP__V30*/ meltfptr[27] = meltgc_send((melt_ptr_t)(/*_.CURARG__V29*/ meltfptr[26]), (melt_ptr_t)((/*!GET_CTYPE*/ meltfrout->tabval[6])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;

#if MELT_HAVE_DEBUG
                                            MELT_LOCATION("warmelt-outobj.melt:4152:/ cppif.then");
                                            /*^block*/
                                            /*anyblock*/
                                            {


                                                MELT_CHECK_SIGNAL();
                                                ;
                                                /*_#IS_A__L22*/ meltfnum[21] =
                                                    melt_is_instance_of((melt_ptr_t)(/*_.CURCTYP__V30*/ meltfptr[27]), (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[7])));;
                                                MELT_LOCATION("warmelt-outobj.melt:4152:/ cond");
                                                /*cond*/ if (/*_#IS_A__L22*/ meltfnum[21]) /*then*/
                                                    {
                                                        /*^cond.then*/
                                                        /*_._IFELSE___V32*/ meltfptr[31] = (/*nil*/NULL);;
                                                    }
                                                else
                                                    {
                                                        MELT_LOCATION("warmelt-outobj.melt:4152:/ cond.else");

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
                                                                argtab[0].meltbp_cstring =  "check curctyp";
                                                                /*^apply.arg*/
                                                                argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                                /*^apply.arg*/
                                                                argtab[2].meltbp_long = 4152;
                                                                /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                            }
                                                            ;
                                                            /*_._IFELSE___V32*/ meltfptr[31] = /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[32];;
                                                            /*epilog*/

                                                            MELT_LOCATION("warmelt-outobj.melt:4152:/ clear");
                                                            /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V33*/ meltfptr[32] = 0 ;
                                                        }
                                                        ;
                                                    }
                                                ;
                                                /*_.IFCPP___V31*/ meltfptr[30] = /*_._IFELSE___V32*/ meltfptr[31];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:4152:/ clear");
                                                /*clear*/ /*_#IS_A__L22*/ meltfnum[21] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_._IFELSE___V32*/ meltfptr[31] = 0 ;
                                            }

#else /*MELT_HAVE_DEBUG*/
                                            /*^cppif.else*/
                                            /*_.IFCPP___V31*/ meltfptr[30] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                            ;
                                            /*^compute*/
                                            /*_#GET_INT__L23*/ meltfnum[21] =
                                                (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V16*/ meltfptr[15])));;

                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-outobj.melt:4153:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[3];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                /*^apply.arg*/
                                                argtab[1].meltbp_long = /*_#GET_INT__L23*/ meltfnum[21];
                                                /*^apply.arg*/
                                                argtab[2].meltbp_cstring =  "multiapply.arg";
                                                /*_.OUTPUT_LOCATION__V34*/ meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.ALOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;

                                            {
                                                MELT_LOCATION("warmelt-outobj.melt:4154:/ locexp");
                                                /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "argtab["));
                                            }
                                            ;
                                            /*_#miI__L24*/ meltfnum[23] =
                                                ((/*_#CURANK__L20*/ meltfnum[19]) - (1));;

                                            {
                                                MELT_LOCATION("warmelt-outobj.melt:4155:/ locexp");
                                                meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#miI__L24*/ meltfnum[23]));
                                            }
                                            ;

                                            {
                                                MELT_LOCATION("warmelt-outobj.melt:4156:/ locexp");
                                                /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "]."));
                                            }
                                            ;
                                            MELT_LOCATION("warmelt-outobj.melt:4157:/ getslot");
                                            {
                                                melt_ptr_t slot=NULL, obj=NULL;
                                                obj = (melt_ptr_t)(/*_.CURCTYP__V30*/ meltfptr[27]) /*=obj*/;
                                                melt_object_get_field(slot,obj, 6, "CTYPE_PARSTRING");
                                                /*_.CTYPE_PARSTRING__V35*/ meltfptr[31] = slot;
                                            };
                                            ;

                                            {
                                                /*^locexp*/
                                                meltgc_append_list((melt_ptr_t)(/*_.PARAMDESCLIST__V14*/ meltfptr[13]), (melt_ptr_t)(/*_.CTYPE_PARSTRING__V35*/ meltfptr[31]));
                                            }
                                            ;

                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*_#NULL__L25*/ meltfnum[24] =
                                                (/*null*/(/*_.CURARG__V29*/ meltfptr[26]) == NULL);;
                                            MELT_LOCATION("warmelt-outobj.melt:4159:/ cond");
                                            /*cond*/ if (/*_#NULL__L25*/ meltfnum[24]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {




                                                        {
                                                            MELT_LOCATION("warmelt-outobj.melt:4160:/ locexp");
                                                            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "meltbp_aptr = (melt_ptr_t*)NULL"));
                                                        }
                                                        ;
                                                        /*clear*/ /*_._IFELSE___V36*/ meltfptr[35] = 0 ;
                                                        /*epilog*/
                                                    }
                                                    ;
                                                }
                                            else
                                                {
                                                    MELT_LOCATION("warmelt-outobj.melt:4159:/ cond.else");

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        /*_#eqeq__L26*/ meltfnum[25] =
                                                            ((/*_.CURCTYP__V30*/ meltfptr[27]) == ((/*!CTYPE_VALUE*/ meltfrout->tabval[8])));;
                                                        MELT_LOCATION("warmelt-outobj.melt:4161:/ cond");
                                                        /*cond*/ if (/*_#eqeq__L26*/ meltfnum[25]) /*then*/
                                                            {
                                                                /*^cond.then*/
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {


                                                                    {
                                                                        MELT_LOCATION("warmelt-outobj.melt:4162:/ locexp");
                                                                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "meltbp_aptr = (melt_ptr_t*) &"));
                                                                    }
                                                                    ;
                                                                    /*_#GET_INT__L27*/ meltfnum[26] =
                                                                        (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V16*/ meltfptr[15])));;

                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    MELT_LOCATION("warmelt-outobj.melt:4163:/ msend");
                                                                    /*msend*/
                                                                    {
                                                                        union meltparam_un argtab[3];
                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                        /*^ojbmsend.arg*/
                                                                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                                                        /*^ojbmsend.arg*/
                                                                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                                        /*^ojbmsend.arg*/
                                                                        argtab[2].meltbp_long = /*_#GET_INT__L27*/ meltfnum[26];
                                                                        /*_.OUTPUT_C_CODE__V38*/ meltfptr[37] = meltgc_send((melt_ptr_t)(/*_.CURARG__V29*/ meltfptr[26]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                                    }
                                                                    ;
                                                                    MELT_LOCATION("warmelt-outobj.melt:4161:/ quasiblock");


                                                                    /*_.PROGN___V39*/ meltfptr[38] = /*_.OUTPUT_C_CODE__V38*/ meltfptr[37];;
                                                                    /*^compute*/
                                                                    /*_._IFELSE___V37*/ meltfptr[36] = /*_.PROGN___V39*/ meltfptr[38];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-outobj.melt:4161:/ clear");
                                                                    /*clear*/ /*_#GET_INT__L27*/ meltfnum[26] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.OUTPUT_C_CODE__V38*/ meltfptr[37] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.PROGN___V39*/ meltfptr[38] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        else    /*^cond.else*/
                                                            {

                                                                /*^block*/
                                                                /*anyblock*/
                                                                {

                                                                    MELT_LOCATION("warmelt-outobj.melt:4166:/ getslot");
                                                                    {
                                                                        melt_ptr_t slot=NULL, obj=NULL;
                                                                        obj = (melt_ptr_t)(/*_.CURCTYP__V30*/ meltfptr[27]) /*=obj*/;
                                                                        melt_object_get_field(slot,obj, 7, "CTYPE_ARGFIELD");
                                                                        /*_.CTYPE_ARGFIELD__V40*/ meltfptr[37] = slot;
                                                                    };
                                                                    ;

                                                                    {
                                                                        /*^locexp*/
                                                                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                                                                              melt_string_str((melt_ptr_t)(/*_.CTYPE_ARGFIELD__V40*/ meltfptr[37])));
                                                                    }
                                                                    ;

                                                                    {
                                                                        MELT_LOCATION("warmelt-outobj.melt:4167:/ locexp");
                                                                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = "));
                                                                    }
                                                                    ;
                                                                    /*_#GET_INT__L28*/ meltfnum[26] =
                                                                        (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V16*/ meltfptr[15])));;

                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    MELT_LOCATION("warmelt-outobj.melt:4168:/ msend");
                                                                    /*msend*/
                                                                    {
                                                                        union meltparam_un argtab[3];
                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                        /*^ojbmsend.arg*/
                                                                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                                                        /*^ojbmsend.arg*/
                                                                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                                        /*^ojbmsend.arg*/
                                                                        argtab[2].meltbp_long = /*_#GET_INT__L28*/ meltfnum[26];
                                                                        /*_.OUTPUT_C_CODE__V41*/ meltfptr[38] = meltgc_send((melt_ptr_t)(/*_.CURARG__V29*/ meltfptr[26]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                                    }
                                                                    ;
                                                                    MELT_LOCATION("warmelt-outobj.melt:4165:/ quasiblock");


                                                                    /*_.PROGN___V42*/ meltfptr[41] = /*_.OUTPUT_C_CODE__V41*/ meltfptr[38];;
                                                                    /*^compute*/
                                                                    /*_._IFELSE___V37*/ meltfptr[36] = /*_.PROGN___V42*/ meltfptr[41];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-outobj.melt:4161:/ clear");
                                                                    /*clear*/ /*_.CTYPE_ARGFIELD__V40*/ meltfptr[37] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_#GET_INT__L28*/ meltfnum[26] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.OUTPUT_C_CODE__V41*/ meltfptr[38] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.PROGN___V42*/ meltfptr[41] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        ;
                                                        /*_._IFELSE___V36*/ meltfptr[35] = /*_._IFELSE___V37*/ meltfptr[36];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-outobj.melt:4159:/ clear");
                                                        /*clear*/ /*_#eqeq__L26*/ meltfnum[25] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_._IFELSE___V37*/ meltfptr[36] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;

                                            {
                                                MELT_LOCATION("warmelt-outobj.melt:4170:/ locexp");
                                                /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
                                            }
                                            ;

                                            MELT_LOCATION("warmelt-outobj.melt:4151:/ clear");
                                            /*clear*/ /*_.CURCTYP__V30*/ meltfptr[27] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.IFCPP___V31*/ meltfptr[30] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_#GET_INT__L23*/ meltfnum[21] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.OUTPUT_LOCATION__V34*/ meltfptr[32] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_#miI__L24*/ meltfnum[23] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.CTYPE_PARSTRING__V35*/ meltfptr[31] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_#NULL__L25*/ meltfnum[24] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_._IFELSE___V36*/ meltfptr[35] = 0 ;
                                            /*epilog*/
                                        }
                                        ;
                                    } /*noelse*/
                                ;

#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:4172:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*_#STRBUF_USEDLENGTH__L29*/ meltfnum[26] =
                                        melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                                    MELT_LOCATION("warmelt-outobj.melt:4173:/ cond");
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
                                                /*_.REFERENCED_VALUE__V44*/ meltfptr[38] = slot;
                                            };
                                            ;
                                        }
                                    else    /*^cond.else*/
                                        {

                                            /*_.REFERENCED_VALUE__V44*/ meltfptr[38] =  /*reallynil*/ NULL ;;
                                        }
                                    ;
                                    /*^compute*/
                                    /*_#GET_INT__L30*/ meltfnum[25] =
                                        (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V44*/ meltfptr[38])));;
                                    /*^compute*/
                                    /*_#ltI__L31*/ meltfnum[21] =
                                        ((/*_#STRBUF_USEDLENGTH__L29*/ meltfnum[26]) < (/*_#GET_INT__L30*/ meltfnum[25]));;
                                    MELT_LOCATION("warmelt-outobj.melt:4172:/ cond");
                                    /*cond*/ if (/*_#ltI__L31*/ meltfnum[21]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*_._IFELSE___V45*/ meltfptr[41] = (/*nil*/NULL);;
                                        }
                                    else
                                        {
                                            MELT_LOCATION("warmelt-outobj.melt:4172:/ cond.else");

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
                                                    argtab[2].meltbp_long = 4172;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                    /*_.MELT_ASSERT_FAILURE_FUN__V46*/ meltfptr[36] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IFELSE___V45*/ meltfptr[41] = /*_.MELT_ASSERT_FAILURE_FUN__V46*/ meltfptr[36];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:4172:/ clear");
                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V46*/ meltfptr[36] = 0 ;
                                            }
                                            ;
                                        }
                                    ;
                                    /*_.IFCPP___V43*/ meltfptr[37] = /*_._IFELSE___V45*/ meltfptr[41];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:4172:/ clear");
                                    /*clear*/ /*_#STRBUF_USEDLENGTH__L29*/ meltfnum[26] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.REFERENCED_VALUE__V44*/ meltfptr[38] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#GET_INT__L30*/ meltfnum[25] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#ltI__L31*/ meltfnum[21] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IFELSE___V45*/ meltfptr[41] = 0 ;
                                }

#else /*MELT_HAVE_DEBUG*/
                                /*^cppif.else*/
                                /*_.IFCPP___V43*/ meltfptr[37] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                ;
                                if (/*_#CURANK__L20*/ meltfnum[19]<0) break;
                            } /* end  foreach_in_multiple meltcit2__EACHTUP */

                        /*citerepilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4147:/ clear");
                        /*clear*/ /*_.CURARG__V29*/ meltfptr[26] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#CURANK__L20*/ meltfnum[19] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#gtI__L21*/ meltfnum[20] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.IFCPP___V43*/ meltfptr[37] = 0 ;
                    } /*endciterblock FOREACH_IN_MULTIPLE*/
                    ;
                    /*_#GET_INT__L32*/ meltfnum[23] =
                        (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V16*/ meltfptr[15])));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4175:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L32*/ meltfnum[23]), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4143:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_#plI__L19*/ meltfnum[18] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#GET_INT__L32*/ meltfnum[23] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#gtI__L33*/ meltfnum[24] =
            ((/*_#NBXRES__L4*/ meltfnum[3]) > (0));;
        MELT_LOCATION("warmelt-outobj.melt:4178:/ cond");
        /*cond*/ if (/*_#gtI__L33*/ meltfnum[24]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*citerblock FOREACH_IN_MULTIPLE*/
                    {
                        /* start foreach_in_multiple meltcit3__EACHTUP */
                        long  meltcit3__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.OXRES__V13*/ meltfptr[12]);
                        for (/*_#CURANK__L34*/ meltfnum[26] = 0;
                                               (/*_#CURANK__L34*/ meltfnum[26] >= 0) && (/*_#CURANK__L34*/ meltfnum[26] <  meltcit3__EACHTUP_ln);
                                               /*_#CURANK__L34*/ meltfnum[26]++)
                            {
                                /*_.CURES__V47*/ meltfptr[27] = melt_multiple_nth((melt_ptr_t)(/*_.OXRES__V13*/ meltfptr[12]),  /*_#CURANK__L34*/ meltfnum[26]);



                                MELT_LOCATION("warmelt-outobj.melt:4182:/ quasiblock");



                                MELT_CHECK_SIGNAL();
                                ;
                                /*^msend*/
                                /*msend*/
                                {
                                    union meltparam_un argtab[1];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^ojbmsend.arg*/
                                    argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
                                    /*_.CURESTYP__V48*/ meltfptr[30] = meltgc_send((melt_ptr_t)(/*_.CURES__V47*/ meltfptr[27]), (melt_ptr_t)((/*!GET_CTYPE*/ meltfrout->tabval[6])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:4183:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*_#IS_A__L35*/ meltfnum[25] =
                                        melt_is_instance_of((melt_ptr_t)(/*_.CURESTYP__V48*/ meltfptr[30]), (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[7])));;
                                    MELT_LOCATION("warmelt-outobj.melt:4183:/ cond");
                                    /*cond*/ if (/*_#IS_A__L35*/ meltfnum[25]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*_._IFELSE___V50*/ meltfptr[31] = (/*nil*/NULL);;
                                        }
                                    else
                                        {
                                            MELT_LOCATION("warmelt-outobj.melt:4183:/ cond.else");

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
                                                    argtab[0].meltbp_cstring =  "check curestyp";
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 4183;
                                                    /*_.MELT_ASSERT_FAILURE_FUN__V51*/ meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IFELSE___V50*/ meltfptr[31] = /*_.MELT_ASSERT_FAILURE_FUN__V51*/ meltfptr[35];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:4183:/ clear");
                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V51*/ meltfptr[35] = 0 ;
                                            }
                                            ;
                                        }
                                    ;
                                    /*_.IFCPP___V49*/ meltfptr[32] = /*_._IFELSE___V50*/ meltfptr[31];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:4183:/ clear");
                                    /*clear*/ /*_#IS_A__L35*/ meltfnum[25] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IFELSE___V50*/ meltfptr[31] = 0 ;
                                }

#else /*MELT_HAVE_DEBUG*/
                                /*^cppif.else*/
                                /*_.IFCPP___V49*/ meltfptr[32] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                ;
                                /*^compute*/
                                /*_#GET_INT__L36*/ meltfnum[21] =
                                    (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V16*/ meltfptr[15])));;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:4184:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[3];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                    /*^apply.arg*/
                                    argtab[1].meltbp_long = /*_#GET_INT__L36*/ meltfnum[21];
                                    /*^apply.arg*/
                                    argtab[2].meltbp_cstring =  "multiapply.xres";
                                    /*_.OUTPUT_LOCATION__V52*/ meltfptr[36] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.ALOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:4185:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "restab["));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:4186:/ locexp");
                                    meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#CURANK__L34*/ meltfnum[26]));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:4187:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "]."));
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                /*_#NULL__L37*/ meltfnum[18] =
                                    (/*null*/(/*_.CURES__V47*/ meltfptr[27]) == NULL);;
                                MELT_LOCATION("warmelt-outobj.melt:4189:/ cond");
                                /*cond*/ if (/*_#NULL__L37*/ meltfnum[18]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {




                                            {
                                                MELT_LOCATION("warmelt-outobj.melt:4190:/ locexp");
                                                /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "meltbp_aptr = (melt_ptr_t*)NULL"));
                                            }
                                            ;
                                            /*clear*/ /*_._IFELSE___V53*/ meltfptr[38] = 0 ;
                                            /*epilog*/
                                        }
                                        ;
                                    }
                                else
                                    {
                                        MELT_LOCATION("warmelt-outobj.melt:4189:/ cond.else");

                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*_#eqeq__L38*/ meltfnum[23] =
                                                ((/*_.CURESTYP__V48*/ meltfptr[30]) == ((/*!CTYPE_VALUE*/ meltfrout->tabval[8])));;
                                            MELT_LOCATION("warmelt-outobj.melt:4191:/ cond");
                                            /*cond*/ if (/*_#eqeq__L38*/ meltfnum[23]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        {
                                                            MELT_LOCATION("warmelt-outobj.melt:4192:/ locexp");
                                                            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "meltbp_aptr = (melt_ptr_t*) &"));
                                                        }
                                                        ;
                                                        /*_#GET_INT__L39*/ meltfnum[25] =
                                                            (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V16*/ meltfptr[15])));;

                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:4193:/ msend");
                                                        /*msend*/
                                                        {
                                                            union meltparam_un argtab[3];
                                                            memset(&argtab, 0, sizeof(argtab));
                                                            /*^ojbmsend.arg*/
                                                            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                                            /*^ojbmsend.arg*/
                                                            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                            /*^ojbmsend.arg*/
                                                            argtab[2].meltbp_long = /*_#GET_INT__L39*/ meltfnum[25];
                                                            /*_.OUTPUT_C_CODE__V55*/ meltfptr[35] = meltgc_send((melt_ptr_t)(/*_.CURES__V47*/ meltfptr[27]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                        }
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:4191:/ quasiblock");


                                                        /*_.PROGN___V56*/ meltfptr[31] = /*_.OUTPUT_C_CODE__V55*/ meltfptr[35];;
                                                        /*^compute*/
                                                        /*_._IFELSE___V54*/ meltfptr[41] = /*_.PROGN___V56*/ meltfptr[31];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-outobj.melt:4191:/ clear");
                                                        /*clear*/ /*_#GET_INT__L39*/ meltfnum[25] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.OUTPUT_C_CODE__V55*/ meltfptr[35] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.PROGN___V56*/ meltfptr[31] = 0 ;
                                                    }
                                                    ;
                                                }
                                            else    /*^cond.else*/
                                                {

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        MELT_LOCATION("warmelt-outobj.melt:4196:/ getslot");
                                                        {
                                                            melt_ptr_t slot=NULL, obj=NULL;
                                                            obj = (melt_ptr_t)(/*_.CURESTYP__V48*/ meltfptr[30]) /*=obj*/;
                                                            melt_object_get_field(slot,obj, 8, "CTYPE_RESFIELD");
                                                            /*_.CTYPE_RESFIELD__V57*/ meltfptr[35] = slot;
                                                        };
                                                        ;

                                                        {
                                                            /*^locexp*/
                                                            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                                                                  melt_string_str((melt_ptr_t)(/*_.CTYPE_RESFIELD__V57*/ meltfptr[35])));
                                                        }
                                                        ;

                                                        {
                                                            MELT_LOCATION("warmelt-outobj.melt:4197:/ locexp");
                                                            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " =  & "));
                                                        }
                                                        ;
                                                        /*_#GET_INT__L40*/ meltfnum[25] =
                                                            (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V16*/ meltfptr[15])));;

                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:4198:/ msend");
                                                        /*msend*/
                                                        {
                                                            union meltparam_un argtab[3];
                                                            memset(&argtab, 0, sizeof(argtab));
                                                            /*^ojbmsend.arg*/
                                                            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                                            /*^ojbmsend.arg*/
                                                            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                            /*^ojbmsend.arg*/
                                                            argtab[2].meltbp_long = /*_#GET_INT__L40*/ meltfnum[25];
                                                            /*_.OUTPUT_C_CODE__V58*/ meltfptr[31] = meltgc_send((melt_ptr_t)(/*_.CURES__V47*/ meltfptr[27]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                        }
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:4195:/ quasiblock");


                                                        /*_.PROGN___V59*/ meltfptr[58] = /*_.OUTPUT_C_CODE__V58*/ meltfptr[31];;
                                                        /*^compute*/
                                                        /*_._IFELSE___V54*/ meltfptr[41] = /*_.PROGN___V59*/ meltfptr[58];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-outobj.melt:4191:/ clear");
                                                        /*clear*/ /*_.CTYPE_RESFIELD__V57*/ meltfptr[35] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_#GET_INT__L40*/ meltfnum[25] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.OUTPUT_C_CODE__V58*/ meltfptr[31] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.PROGN___V59*/ meltfptr[58] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;
                                            /*_._IFELSE___V53*/ meltfptr[38] = /*_._IFELSE___V54*/ meltfptr[41];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:4189:/ clear");
                                            /*clear*/ /*_#eqeq__L38*/ meltfnum[23] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_._IFELSE___V54*/ meltfptr[41] = 0 ;
                                        }
                                        ;
                                    }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:4200:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
                                }
                                ;
                                /*_#GET_INT__L41*/ meltfnum[25] =
                                    (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V16*/ meltfptr[15])));;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:4201:/ locexp");
                                    meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L41*/ meltfnum[25]), 0);
                                }
                                ;

                                MELT_LOCATION("warmelt-outobj.melt:4182:/ clear");
                                /*clear*/ /*_.CURESTYP__V48*/ meltfptr[30] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.IFCPP___V49*/ meltfptr[32] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#GET_INT__L36*/ meltfnum[21] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.OUTPUT_LOCATION__V52*/ meltfptr[36] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#NULL__L37*/ meltfnum[18] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V53*/ meltfptr[38] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#GET_INT__L41*/ meltfnum[25] = 0 ;
                                if (/*_#CURANK__L34*/ meltfnum[26]<0) break;
                            } /* end  foreach_in_multiple meltcit3__EACHTUP */

                        /*citerepilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4179:/ clear");
                        /*clear*/ /*_.CURES__V47*/ meltfptr[27] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#CURANK__L34*/ meltfnum[26] = 0 ;
                    } /*endciterblock FOREACH_IN_MULTIPLE*/
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4178:/ quasiblock");


                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*_#GET_INT__L42*/ meltfnum[23] =
            (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V16*/ meltfptr[15])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4204:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#GET_INT__L42*/ meltfnum[23];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "multiapply.appl";
            /*_.OUTPUT_LOCATION__V60*/ meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.ALOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit4__EACHLIST */
            for (/*_.CURPAIR__V61*/ meltfptr[31] = melt_list_first( (melt_ptr_t)/*_.ADEST__V10*/ meltfptr[9]);
                                    melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V61*/ meltfptr[31]) == MELTOBMAG_PAIR;
                                    /*_.CURPAIR__V61*/ meltfptr[31] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V61*/ meltfptr[31]))
                {
                    /*_.CURDEST__V62*/ meltfptr[58] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V61*/ meltfptr[31]);


                    /*_#GET_INT__L43*/ meltfnum[21] =
                        (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V16*/ meltfptr[15])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4209:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#GET_INT__L43*/ meltfnum[21];
                        /*_.OUTPUT_C_CODE__V63*/ meltfptr[41] = meltgc_send((melt_ptr_t)(/*_.CURDEST__V62*/ meltfptr[58]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4210:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = "));
                    }
                    ;
                } /* end foreach_pair_component_in_list meltcit4__EACHLIST */
            /*_.CURPAIR__V61*/ meltfptr[31] = NULL;
            /*_.CURDEST__V62*/ meltfptr[58] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:4206:/ clear");
            /*clear*/ /*_.CURPAIR__V61*/ meltfptr[31] = 0 ;
            /*^clear*/
            /*clear*/ /*_.CURDEST__V62*/ meltfptr[58] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L43*/ meltfnum[21] = 0 ;
            /*^clear*/
            /*clear*/ /*_.OUTPUT_C_CODE__V63*/ meltfptr[41] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4212:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " melt_apply ((meltclosure_ptr_t)("));
        }
        ;
        /*_#plI__L44*/ meltfnum[18] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4213:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#plI__L44*/ meltfnum[18];
            /*_.OUTPUT_C_CODE__V64*/ meltfptr[30] = meltgc_send((melt_ptr_t)(/*_.OCLOS__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4214:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "), (melt_ptr_t)("));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:4216:/ quasiblock");


        /*_.FIRSTARG__V66*/ meltfptr[36] =
            (melt_multiple_nth((melt_ptr_t)(/*_.OARGS__V12*/ meltfptr[11]), (0)));;
        /*^compute*/
        /*_#plI__L45*/ meltfnum[25] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4217:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#plI__L45*/ meltfnum[25];
            /*_.OUTPUT_C_CODE__V67*/ meltfptr[38] = meltgc_send((melt_ptr_t)(/*_.FIRSTARG__V66*/ meltfptr[36]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.LET___V65*/ meltfptr[32] = /*_.OUTPUT_C_CODE__V67*/ meltfptr[38];;

        MELT_LOCATION("warmelt-outobj.melt:4216:/ clear");
        /*clear*/ /*_.FIRSTARG__V66*/ meltfptr[36] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L45*/ meltfnum[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V67*/ meltfptr[38] = 0 ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4219:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "), ("));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:4223:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V69*/ meltfptr[38] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_13*/ meltfrout->tabval[13])), (1));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V69*/ meltfptr[38])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V69*/ meltfptr[38])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V69*/ meltfptr[38])->tabval[0] = (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]);
        ;
        /*_.LAMBDA___V68*/ meltfptr[36] = /*_.LAMBDA___V69*/ meltfptr[38];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4221:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LAMBDA___V68*/ meltfptr[36];
            /*_.LIST_EVERY__V70*/ meltfptr[69] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_EVERY*/ meltfrout->tabval[12])), (melt_ptr_t)(/*_.PARAMDESCLIST__V14*/ meltfptr[13]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4226:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "\"\"), "));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#gtI__L46*/ meltfnum[25] =
            ((/*_#NBARG__L3*/ meltfnum[1]) > (1));;
        MELT_LOCATION("warmelt-outobj.melt:4228:/ cond");
        /*cond*/ if (/*_#gtI__L46*/ meltfnum[25]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4229:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "argtab, ("));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:4228:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4230:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "(union meltparam_un*)0, ("));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit5__EACHLIST */
            for (/*_.CURPAIR__V71*/ meltfptr[70] = melt_list_first( (melt_ptr_t)/*_.RESDESCLIST__V15*/ meltfptr[14]);
                                    melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V71*/ meltfptr[70]) == MELTOBMAG_PAIR;
                                    /*_.CURPAIR__V71*/ meltfptr[70] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V71*/ meltfptr[70]))
                {
                    /*_.RESD__V72*/ meltfptr[71] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V71*/ meltfptr[70]);



                    {
                        MELT_LOCATION("warmelt-outobj.melt:4235:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                              melt_string_str((melt_ptr_t)(/*_.RESD__V72*/ meltfptr[71])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4236:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " "));
                    }
                    ;
                } /* end foreach_pair_component_in_list meltcit5__EACHLIST */
            /*_.CURPAIR__V71*/ meltfptr[70] = NULL;
            /*_.RESD__V72*/ meltfptr[71] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:4232:/ clear");
            /*clear*/ /*_.CURPAIR__V71*/ meltfptr[70] = 0 ;
            /*^clear*/
            /*clear*/ /*_.RESD__V72*/ meltfptr[71] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4237:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "\"\"), "));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#gtI__L47*/ meltfnum[46] =
            ((/*_#NBXRES__L4*/ meltfnum[3]) > (0));;
        MELT_LOCATION("warmelt-outobj.melt:4239:/ cond");
        /*cond*/ if (/*_#gtI__L47*/ meltfnum[46]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4240:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "restab"));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:4239:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4241:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "(union meltparam_un*)0"));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4242:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
        }
        ;
        /*_#plI__L48*/ meltfnum[47] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

        {
            MELT_LOCATION("warmelt-outobj.melt:4243:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L48*/ meltfnum[47]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4244:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "}"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4245:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4246:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L49*/ meltfnum[48] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:4247:/ cond");
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
                        /*_.REFERENCED_VALUE__V74*/ meltfptr[73] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V74*/ meltfptr[73] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L50*/ meltfnum[49] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V74*/ meltfptr[73])));;
            /*^compute*/
            /*_#ltI__L51*/ meltfnum[50] =
                ((/*_#STRBUF_USEDLENGTH__L49*/ meltfnum[48]) < (/*_#GET_INT__L50*/ meltfnum[49]));;
            MELT_LOCATION("warmelt-outobj.melt:4246:/ cond");
            /*cond*/ if (/*_#ltI__L51*/ meltfnum[50]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V75*/ meltfptr[74] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4246:/ cond.else");

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
                            argtab[2].meltbp_long = 4246;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V76*/ meltfptr[75] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V75*/ meltfptr[74] = /*_.MELT_ASSERT_FAILURE_FUN__V76*/ meltfptr[75];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4246:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V76*/ meltfptr[75] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V73*/ meltfptr[72] = /*_._IFELSE___V75*/ meltfptr[74];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4246:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L49*/ meltfnum[48] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V74*/ meltfptr[73] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L50*/ meltfnum[49] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L51*/ meltfnum[50] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V75*/ meltfptr[74] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V73*/ meltfptr[72] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V73*/ meltfptr[72];;

        MELT_LOCATION("warmelt-outobj.melt:4098:/ clear");
        /*clear*/ /*_.ALOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADEST__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OCLOS__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OARGS__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OXRES__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NBARG__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NBXRES__L4*/ meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PARAMDESCLIST__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RESDESCLIST__V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L5*/ meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BOXDEPTHP1__V16*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V17*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V20*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V23*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V24*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L8*/ meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L9*/ meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L12*/ meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L15*/ meltfnum[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L18*/ meltfnum[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L33*/ meltfnum[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L42*/ meltfnum[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V60*/ meltfptr[35] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L44*/ meltfnum[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V64*/ meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V65*/ meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LAMBDA___V68*/ meltfptr[36] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_EVERY__V70*/ meltfptr[69] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L46*/ meltfnum[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L47*/ meltfnum[46] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L48*/ meltfnum[47] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V73*/ meltfptr[72] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4096:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:4096:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJMULTIAPPLY", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_89_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIAPPLY*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5 fromline 1752 */

    /** start of frame for meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5 of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5// fromline 1568
        : public Melt_CallFrameWithValues<2>
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
        MeltFrame_meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<2> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5), clos) {};
        MeltFrame_meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5() //the constructor fromline 1642
            : Melt_CallFrameWithValues<2> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5)) {};
        MeltFrame_meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5


    /** end of frame for meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5 fromline 1697**/

    /* end of frame for routine meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5 fromline 1756 */

    /* classy proc frame meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5 */ MeltFrame_meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5 fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl5", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:4223:/ getarg");
    /*_.PARD__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


        {
            MELT_LOCATION("warmelt-outobj.melt:4224:/ locexp");
            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)((/*~IMPLBUF*/ meltfclos->tabval[0])),
                                                  melt_string_str((melt_ptr_t)(/*_.PARD__V2*/ meltfptr[1])));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4225:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)((/*~IMPLBUF*/ meltfclos->tabval[0])), ( " "));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl5", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_90_WARMELTmiOUTOBJ_LAMBDA_cl5*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND fromline 1752 */

    /** start of frame for meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND// fromline 1568
        : public Melt_CallFrameWithValues<68>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[45];
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
        MeltFrame_meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<68> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND), clos) {};
        MeltFrame_meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND() //the constructor fromline 1642
            : Melt_CallFrameWithValues<68> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<68> (fil,lin, sizeof(MeltFrame_meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND)) {};
        MeltFrame_meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<68> (fil,lin, sizeof(MeltFrame_meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND


    /** end of frame for meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND fromline 1697**/

    /* end of frame for routine meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND fromline 1756 */

    /* classy proc frame meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND */ MeltFrame_meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJMULTIMSEND", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:4256:/ getarg");
    /*_.OMSND__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:4257:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OMSND__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJMULTIMSEND*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:4257:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4257:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check omsnd";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4257;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OMSND__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4257:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4257:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:4258:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OMSND__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.OLOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4259:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OMSND__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OBDI_DESTLIST");
            /*_.ODEST__V10*/ meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4260:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OMSND__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "OBMSND_SEL");
            /*_.OSEL__V11*/ meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4261:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OMSND__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "OBMSND_RECV");
            /*_.ORECV__V12*/ meltfptr[11] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4262:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OMSND__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 4, "OBMSND_ARGS");
            /*_.OARGS__V13*/ meltfptr[12] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4263:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OMSND__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 5, "OBMULTSND_XRES");
            /*_.OXRES__V14*/ meltfptr[13] = slot;
        };
        ;
        /*_#NBARG__L3*/ meltfnum[1] =
            (melt_multiple_length((melt_ptr_t)(/*_.OARGS__V13*/ meltfptr[12])));;
        /*^compute*/
        /*_#NBXRES__L4*/ meltfnum[3] =
            (melt_multiple_length((melt_ptr_t)(/*_.OXRES__V14*/ meltfptr[13])));;
        /*^compute*/
        /*_.PARAMDESCLIST__V15*/ meltfptr[14] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
        /*^compute*/
        /*_.RESDESCLIST__V16*/ meltfptr[15] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
        /*^compute*/
        /*_#plI__L5*/ meltfnum[4] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;
        /*^compute*/
        /*_.BOXDEPTHP1__V17*/ meltfptr[16] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[3])), (/*_#plI__L5*/ meltfnum[4])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4270:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "multimsend";
            /*_.OUTPUT_LOCATION__V18*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OMSND__V2*/ meltfptr[1];
                        /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V20*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!DO_OUTPUT_DESCRIPTIVE_COMMENT*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_._IF___V19*/ meltfptr[18] = /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V20*/ meltfptr[19];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:73:/ clear");
                    /*clear*/ /*_.DO_OUTPUT_DESCRIPTIVE_COMMENT__V20*/ meltfptr[19] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V19*/ meltfptr[18] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4272:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*multimsend*/{"));
        }
        ;
        /*_#plI__L6*/ meltfnum[5] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

        {
            MELT_LOCATION("warmelt-outobj.melt:4273:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L6*/ meltfnum[5]), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#gtI__L7*/ meltfnum[6] =
            ((/*_#NBARG__L3*/ meltfnum[1]) > (0));;
        MELT_LOCATION("warmelt-outobj.melt:4274:/ cond");
        /*cond*/ if (/*_#gtI__L7*/ meltfnum[6]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4275:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "union meltparam_un argtab["));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4276:/ locexp");
                        meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#NBARG__L3*/ meltfnum[1]));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4277:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "];"));
                    }
                    ;
                    /*_#plI__L8*/ meltfnum[7] =
                        ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4278:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L8*/ meltfnum[7]), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4274:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_#plI__L8*/ meltfnum[7] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#gtI__L9*/ meltfnum[7] =
            ((/*_#NBXRES__L4*/ meltfnum[3]) > (0));;
        MELT_LOCATION("warmelt-outobj.melt:4280:/ cond");
        /*cond*/ if (/*_#gtI__L9*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4281:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "union meltparam_un restab["));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4282:/ locexp");
                        meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#NBXRES__L4*/ meltfnum[3]));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4283:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "];"));
                    }
                    ;
                    /*_#plI__L10*/ meltfnum[9] =
                        ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4284:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L10*/ meltfnum[9]), 0);
                    }
                    ;
                    /*citerblock FOREACH_IN_MULTIPLE*/
                    {
                        /* start foreach_in_multiple meltcit1__EACHTUP */
                        long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.OXRES__V14*/ meltfptr[13]);
                        for (/*_#CURANK__L11*/ meltfnum[10] = 0;
                                               (/*_#CURANK__L11*/ meltfnum[10] >= 0) && (/*_#CURANK__L11*/ meltfnum[10] <  meltcit1__EACHTUP_ln);
                                               /*_#CURANK__L11*/ meltfnum[10]++)
                            {
                                /*_.CURES__V21*/ meltfptr[19] = melt_multiple_nth((melt_ptr_t)(/*_.OXRES__V14*/ meltfptr[13]),  /*_#CURANK__L11*/ meltfnum[10]);



                                MELT_LOCATION("warmelt-outobj.melt:4289:/ quasiblock");



                                MELT_CHECK_SIGNAL();
                                ;
                                /*^msend*/
                                /*msend*/
                                {
                                    union meltparam_un argtab[1];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^ojbmsend.arg*/
                                    argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
                                    /*_.CURESTYP__V22*/ meltfptr[21] = meltgc_send((melt_ptr_t)(/*_.CURES__V21*/ meltfptr[19]), (melt_ptr_t)((/*!GET_CTYPE*/ meltfrout->tabval[6])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:4290:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.CURESTYP__V22*/ meltfptr[21]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 6, "CTYPE_PARSTRING");
                                    /*_.CTYPE_PARSTRING__V23*/ meltfptr[22] = slot;
                                };
                                ;

                                {
                                    /*^locexp*/
                                    meltgc_append_list((melt_ptr_t)(/*_.RESDESCLIST__V16*/ meltfptr[15]), (melt_ptr_t)(/*_.CTYPE_PARSTRING__V23*/ meltfptr[22]));
                                }
                                ;

                                MELT_LOCATION("warmelt-outobj.melt:4289:/ clear");
                                /*clear*/ /*_.CURESTYP__V22*/ meltfptr[21] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.CTYPE_PARSTRING__V23*/ meltfptr[22] = 0 ;
                                if (/*_#CURANK__L11*/ meltfnum[10]<0) break;
                            } /* end  foreach_in_multiple meltcit1__EACHTUP */

                        /*citerepilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4286:/ clear");
                        /*clear*/ /*_.CURES__V21*/ meltfptr[19] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#CURANK__L11*/ meltfnum[10] = 0 ;
                    } /*endciterblock FOREACH_IN_MULTIPLE*/
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4280:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_#plI__L10*/ meltfnum[9] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#gtI__L12*/ meltfnum[9] =
            ((/*_#NBARG__L3*/ meltfnum[1]) > (0));;
        MELT_LOCATION("warmelt-outobj.melt:4292:/ cond");
        /*cond*/ if (/*_#gtI__L12*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4293:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "memset(&argtab, 0, sizeof(argtab));"));
                    }
                    ;
                    /*_#plI__L13*/ meltfnum[12] =
                        ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4294:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L13*/ meltfnum[12]), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4292:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_#plI__L13*/ meltfnum[12] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#gtI__L14*/ meltfnum[12] =
            ((/*_#NBXRES__L4*/ meltfnum[3]) > (0));;
        MELT_LOCATION("warmelt-outobj.melt:4295:/ cond");
        /*cond*/ if (/*_#gtI__L14*/ meltfnum[12]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4296:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "memset(&restab, 0, sizeof(restab));"));
                    }
                    ;
                    /*_#plI__L15*/ meltfnum[14] =
                        ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4297:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L15*/ meltfnum[14]), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4295:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_#plI__L15*/ meltfnum[14] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#gtI__L16*/ meltfnum[14] =
            ((/*_#NBARG__L3*/ meltfnum[1]) > (0));;
        MELT_LOCATION("warmelt-outobj.melt:4299:/ cond");
        /*cond*/ if (/*_#gtI__L16*/ meltfnum[14]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*citerblock FOREACH_IN_MULTIPLE*/
                    {
                        /* start foreach_in_multiple meltcit2__EACHTUP */
                        long  meltcit2__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.OARGS__V13*/ meltfptr[12]);
                        for (/*_#CURANK__L17*/ meltfnum[16] = 0;
                                               (/*_#CURANK__L17*/ meltfnum[16] >= 0) && (/*_#CURANK__L17*/ meltfnum[16] <  meltcit2__EACHTUP_ln);
                                               /*_#CURANK__L17*/ meltfnum[16]++)
                            {
                                /*_.CURARG__V24*/ meltfptr[21] = melt_multiple_nth((melt_ptr_t)(/*_.OARGS__V13*/ meltfptr[12]),  /*_#CURANK__L17*/ meltfnum[16]);



                                MELT_LOCATION("warmelt-outobj.melt:4304:/ quasiblock");



                                MELT_CHECK_SIGNAL();
                                ;
                                /*^msend*/
                                /*msend*/
                                {
                                    union meltparam_un argtab[1];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^ojbmsend.arg*/
                                    argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
                                    /*_.CURCTYP__V25*/ meltfptr[22] = meltgc_send((melt_ptr_t)(/*_.CURARG__V24*/ meltfptr[21]), (melt_ptr_t)((/*!GET_CTYPE*/ meltfrout->tabval[6])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:4305:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*_#IS_A__L18*/ meltfnum[17] =
                                        melt_is_instance_of((melt_ptr_t)(/*_.CURCTYP__V25*/ meltfptr[22]), (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[7])));;
                                    MELT_LOCATION("warmelt-outobj.melt:4305:/ cond");
                                    /*cond*/ if (/*_#IS_A__L18*/ meltfnum[17]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*_._IFELSE___V27*/ meltfptr[26] = (/*nil*/NULL);;
                                        }
                                    else
                                        {
                                            MELT_LOCATION("warmelt-outobj.melt:4305:/ cond.else");

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
                                                    argtab[0].meltbp_cstring =  "check curctyp";
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 4305;
                                                    /*_.MELT_ASSERT_FAILURE_FUN__V28*/ meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IFELSE___V27*/ meltfptr[26] = /*_.MELT_ASSERT_FAILURE_FUN__V28*/ meltfptr[27];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:4305:/ clear");
                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V28*/ meltfptr[27] = 0 ;
                                            }
                                            ;
                                        }
                                    ;
                                    /*_.IFCPP___V26*/ meltfptr[25] = /*_._IFELSE___V27*/ meltfptr[26];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:4305:/ clear");
                                    /*clear*/ /*_#IS_A__L18*/ meltfnum[17] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IFELSE___V27*/ meltfptr[26] = 0 ;
                                }

#else /*MELT_HAVE_DEBUG*/
                                /*^cppif.else*/
                                /*_.IFCPP___V26*/ meltfptr[25] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                ;
                                /*^compute*/
                                /*_#GET_INT__L19*/ meltfnum[17] =
                                    (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V17*/ meltfptr[16])));;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:4306:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[3];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                    /*^apply.arg*/
                                    argtab[1].meltbp_long = /*_#GET_INT__L19*/ meltfnum[17];
                                    /*^apply.arg*/
                                    argtab[2].meltbp_cstring =  "multimsend.arg";
                                    /*_.OUTPUT_LOCATION__V29*/ meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:4307:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "argtab["));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:4308:/ locexp");
                                    meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#CURANK__L17*/ meltfnum[16]));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:4309:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "]."));
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:4310:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.CURCTYP__V25*/ meltfptr[22]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 6, "CTYPE_PARSTRING");
                                    /*_.CTYPE_PARSTRING__V30*/ meltfptr[26] = slot;
                                };
                                ;

                                {
                                    /*^locexp*/
                                    meltgc_append_list((melt_ptr_t)(/*_.PARAMDESCLIST__V15*/ meltfptr[14]), (melt_ptr_t)(/*_.CTYPE_PARSTRING__V30*/ meltfptr[26]));
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                /*_#NULL__L20*/ meltfnum[19] =
                                    (/*null*/(/*_.CURARG__V24*/ meltfptr[21]) == NULL);;
                                MELT_LOCATION("warmelt-outobj.melt:4312:/ cond");
                                /*cond*/ if (/*_#NULL__L20*/ meltfnum[19]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {




                                            {
                                                MELT_LOCATION("warmelt-outobj.melt:4313:/ locexp");
                                                /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "meltbp_aptr = (melt_ptr_t*)NULL"));
                                            }
                                            ;
                                            /*clear*/ /*_._IFELSE___V31*/ meltfptr[30] = 0 ;
                                            /*epilog*/
                                        }
                                        ;
                                    }
                                else
                                    {
                                        MELT_LOCATION("warmelt-outobj.melt:4312:/ cond.else");

                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*_#eqeq__L21*/ meltfnum[20] =
                                                ((/*_.CURCTYP__V25*/ meltfptr[22]) == ((/*!CTYPE_VALUE*/ meltfrout->tabval[8])));;
                                            MELT_LOCATION("warmelt-outobj.melt:4314:/ cond");
                                            /*cond*/ if (/*_#eqeq__L21*/ meltfnum[20]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        {
                                                            MELT_LOCATION("warmelt-outobj.melt:4315:/ locexp");
                                                            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "meltbp_aptr = (melt_ptr_t*) &"));
                                                        }
                                                        ;
                                                        /*_#GET_INT__L22*/ meltfnum[21] =
                                                            (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V17*/ meltfptr[16])));;

                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:4316:/ msend");
                                                        /*msend*/
                                                        {
                                                            union meltparam_un argtab[3];
                                                            memset(&argtab, 0, sizeof(argtab));
                                                            /*^ojbmsend.arg*/
                                                            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                                            /*^ojbmsend.arg*/
                                                            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                            /*^ojbmsend.arg*/
                                                            argtab[2].meltbp_long = /*_#GET_INT__L22*/ meltfnum[21];
                                                            /*_.OUTPUT_C_CODE__V33*/ meltfptr[32] = meltgc_send((melt_ptr_t)(/*_.CURARG__V24*/ meltfptr[21]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                        }
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:4314:/ quasiblock");


                                                        /*_.PROGN___V34*/ meltfptr[33] = /*_.OUTPUT_C_CODE__V33*/ meltfptr[32];;
                                                        /*^compute*/
                                                        /*_._IFELSE___V32*/ meltfptr[31] = /*_.PROGN___V34*/ meltfptr[33];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-outobj.melt:4314:/ clear");
                                                        /*clear*/ /*_#GET_INT__L22*/ meltfnum[21] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.OUTPUT_C_CODE__V33*/ meltfptr[32] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.PROGN___V34*/ meltfptr[33] = 0 ;
                                                    }
                                                    ;
                                                }
                                            else    /*^cond.else*/
                                                {

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        MELT_LOCATION("warmelt-outobj.melt:4319:/ getslot");
                                                        {
                                                            melt_ptr_t slot=NULL, obj=NULL;
                                                            obj = (melt_ptr_t)(/*_.CURCTYP__V25*/ meltfptr[22]) /*=obj*/;
                                                            melt_object_get_field(slot,obj, 7, "CTYPE_ARGFIELD");
                                                            /*_.CTYPE_ARGFIELD__V35*/ meltfptr[32] = slot;
                                                        };
                                                        ;

                                                        {
                                                            /*^locexp*/
                                                            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                                                                  melt_string_str((melt_ptr_t)(/*_.CTYPE_ARGFIELD__V35*/ meltfptr[32])));
                                                        }
                                                        ;

                                                        {
                                                            MELT_LOCATION("warmelt-outobj.melt:4320:/ locexp");
                                                            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = "));
                                                        }
                                                        ;
                                                        /*_#GET_INT__L23*/ meltfnum[21] =
                                                            (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V17*/ meltfptr[16])));;

                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:4321:/ msend");
                                                        /*msend*/
                                                        {
                                                            union meltparam_un argtab[3];
                                                            memset(&argtab, 0, sizeof(argtab));
                                                            /*^ojbmsend.arg*/
                                                            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                                            /*^ojbmsend.arg*/
                                                            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                            /*^ojbmsend.arg*/
                                                            argtab[2].meltbp_long = /*_#GET_INT__L23*/ meltfnum[21];
                                                            /*_.OUTPUT_C_CODE__V36*/ meltfptr[33] = meltgc_send((melt_ptr_t)(/*_.CURARG__V24*/ meltfptr[21]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                        }
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:4318:/ quasiblock");


                                                        /*_.PROGN___V37*/ meltfptr[36] = /*_.OUTPUT_C_CODE__V36*/ meltfptr[33];;
                                                        /*^compute*/
                                                        /*_._IFELSE___V32*/ meltfptr[31] = /*_.PROGN___V37*/ meltfptr[36];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-outobj.melt:4314:/ clear");
                                                        /*clear*/ /*_.CTYPE_ARGFIELD__V35*/ meltfptr[32] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_#GET_INT__L23*/ meltfnum[21] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.OUTPUT_C_CODE__V36*/ meltfptr[33] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.PROGN___V37*/ meltfptr[36] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;
                                            /*_._IFELSE___V31*/ meltfptr[30] = /*_._IFELSE___V32*/ meltfptr[31];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:4312:/ clear");
                                            /*clear*/ /*_#eqeq__L21*/ meltfnum[20] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_._IFELSE___V32*/ meltfptr[31] = 0 ;
                                        }
                                        ;
                                    }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:4323:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
                                }
                                ;

                                MELT_LOCATION("warmelt-outobj.melt:4304:/ clear");
                                /*clear*/ /*_.CURCTYP__V25*/ meltfptr[22] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.IFCPP___V26*/ meltfptr[25] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#GET_INT__L19*/ meltfnum[17] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.OUTPUT_LOCATION__V29*/ meltfptr[27] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.CTYPE_PARSTRING__V30*/ meltfptr[26] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#NULL__L20*/ meltfnum[19] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V31*/ meltfptr[30] = 0 ;

#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:4325:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*_#STRBUF_USEDLENGTH__L24*/ meltfnum[21] =
                                        melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
                                    MELT_LOCATION("warmelt-outobj.melt:4326:/ cond");
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
                                                /*_.REFERENCED_VALUE__V39*/ meltfptr[33] = slot;
                                            };
                                            ;
                                        }
                                    else    /*^cond.else*/
                                        {

                                            /*_.REFERENCED_VALUE__V39*/ meltfptr[33] =  /*reallynil*/ NULL ;;
                                        }
                                    ;
                                    /*^compute*/
                                    /*_#GET_INT__L25*/ meltfnum[20] =
                                        (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V39*/ meltfptr[33])));;
                                    /*^compute*/
                                    /*_#ltI__L26*/ meltfnum[17] =
                                        ((/*_#STRBUF_USEDLENGTH__L24*/ meltfnum[21]) < (/*_#GET_INT__L25*/ meltfnum[20]));;
                                    MELT_LOCATION("warmelt-outobj.melt:4325:/ cond");
                                    /*cond*/ if (/*_#ltI__L26*/ meltfnum[17]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*_._IFELSE___V40*/ meltfptr[36] = (/*nil*/NULL);;
                                        }
                                    else
                                        {
                                            MELT_LOCATION("warmelt-outobj.melt:4325:/ cond.else");

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
                                                    argtab[0].meltbp_cstring =  "check limited implbuf";
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 4325;
                                                    /*_.MELT_ASSERT_FAILURE_FUN__V41*/ meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IFELSE___V40*/ meltfptr[36] = /*_.MELT_ASSERT_FAILURE_FUN__V41*/ meltfptr[31];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:4325:/ clear");
                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V41*/ meltfptr[31] = 0 ;
                                            }
                                            ;
                                        }
                                    ;
                                    /*_.IFCPP___V38*/ meltfptr[32] = /*_._IFELSE___V40*/ meltfptr[36];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:4325:/ clear");
                                    /*clear*/ /*_#STRBUF_USEDLENGTH__L24*/ meltfnum[21] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.REFERENCED_VALUE__V39*/ meltfptr[33] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#GET_INT__L25*/ meltfnum[20] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#ltI__L26*/ meltfnum[17] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IFELSE___V40*/ meltfptr[36] = 0 ;
                                }

#else /*MELT_HAVE_DEBUG*/
                                /*^cppif.else*/
                                /*_.IFCPP___V38*/ meltfptr[32] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                ;
                                if (/*_#CURANK__L17*/ meltfnum[16]<0) break;
                            } /* end  foreach_in_multiple meltcit2__EACHTUP */

                        /*citerepilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4301:/ clear");
                        /*clear*/ /*_.CURARG__V24*/ meltfptr[21] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#CURANK__L17*/ meltfnum[16] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.IFCPP___V38*/ meltfptr[32] = 0 ;
                    } /*endciterblock FOREACH_IN_MULTIPLE*/
                    ;
                    /*_#GET_INT__L27*/ meltfnum[19] =
                        (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V17*/ meltfptr[16])));;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4328:/ locexp");
                        meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#GET_INT__L27*/ meltfnum[19]), 0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4299:/ quasiblock");


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_#GET_INT__L27*/ meltfnum[19] = 0 ;
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#gtI__L28*/ meltfnum[21] =
            ((/*_#NBXRES__L4*/ meltfnum[3]) > (0));;
        MELT_LOCATION("warmelt-outobj.melt:4331:/ cond");
        /*cond*/ if (/*_#gtI__L28*/ meltfnum[21]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*citerblock FOREACH_IN_MULTIPLE*/
                    {
                        /* start foreach_in_multiple meltcit3__EACHTUP */
                        long  meltcit3__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.OXRES__V14*/ meltfptr[13]);
                        for (/*_#CURANK__L29*/ meltfnum[20] = 0;
                                               (/*_#CURANK__L29*/ meltfnum[20] >= 0) && (/*_#CURANK__L29*/ meltfnum[20] <  meltcit3__EACHTUP_ln);
                                               /*_#CURANK__L29*/ meltfnum[20]++)
                            {
                                /*_.CURES__V42*/ meltfptr[22] = melt_multiple_nth((melt_ptr_t)(/*_.OXRES__V14*/ meltfptr[13]),  /*_#CURANK__L29*/ meltfnum[20]);



                                MELT_LOCATION("warmelt-outobj.melt:4335:/ quasiblock");



                                MELT_CHECK_SIGNAL();
                                ;
                                /*^msend*/
                                /*msend*/
                                {
                                    union meltparam_un argtab[1];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^ojbmsend.arg*/
                                    argtab[0].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
                                    /*_.CURESTYP__V43*/ meltfptr[25] = meltgc_send((melt_ptr_t)(/*_.CURES__V42*/ meltfptr[22]), (melt_ptr_t)((/*!GET_CTYPE*/ meltfrout->tabval[6])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:4336:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*_#IS_A__L30*/ meltfnum[17] =
                                        melt_is_instance_of((melt_ptr_t)(/*_.CURESTYP__V43*/ meltfptr[25]), (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[7])));;
                                    MELT_LOCATION("warmelt-outobj.melt:4336:/ cond");
                                    /*cond*/ if (/*_#IS_A__L30*/ meltfnum[17]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*_._IFELSE___V45*/ meltfptr[26] = (/*nil*/NULL);;
                                        }
                                    else
                                        {
                                            MELT_LOCATION("warmelt-outobj.melt:4336:/ cond.else");

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
                                                    argtab[0].meltbp_cstring =  "check curestyp";
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 4336;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CURESTYP__V43*/ meltfptr[25];
                                                    /*_.MELT_ASSERT_FAILURE_FUN__V46*/ meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IFELSE___V45*/ meltfptr[26] = /*_.MELT_ASSERT_FAILURE_FUN__V46*/ meltfptr[30];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:4336:/ clear");
                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V46*/ meltfptr[30] = 0 ;
                                            }
                                            ;
                                        }
                                    ;
                                    /*_.IFCPP___V44*/ meltfptr[27] = /*_._IFELSE___V45*/ meltfptr[26];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:4336:/ clear");
                                    /*clear*/ /*_#IS_A__L30*/ meltfnum[17] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IFELSE___V45*/ meltfptr[26] = 0 ;
                                }

#else /*MELT_HAVE_DEBUG*/
                                /*^cppif.else*/
                                /*_.IFCPP___V44*/ meltfptr[27] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                ;
                                /*^compute*/
                                /*_#GET_INT__L31*/ meltfnum[19] =
                                    (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V17*/ meltfptr[16])));;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:4337:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[3];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                    /*^apply.arg*/
                                    argtab[1].meltbp_long = /*_#GET_INT__L31*/ meltfnum[19];
                                    /*^apply.arg*/
                                    argtab[2].meltbp_cstring =  "multimsend.xres";
                                    /*_.OUTPUT_LOCATION__V47*/ meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:4338:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "restab["));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:4339:/ locexp");
                                    meltgc_add_strbuf_dec((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#CURANK__L29*/ meltfnum[20]));
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:4340:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "]."));
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                /*_#NULL__L32*/ meltfnum[17] =
                                    (/*null*/(/*_.CURES__V42*/ meltfptr[22]) == NULL);;
                                MELT_LOCATION("warmelt-outobj.melt:4342:/ cond");
                                /*cond*/ if (/*_#NULL__L32*/ meltfnum[17]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {




                                            {
                                                MELT_LOCATION("warmelt-outobj.melt:4343:/ locexp");
                                                /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "meltbp_aptr = (melt_ptr_t*)NULL"));
                                            }
                                            ;
                                            /*clear*/ /*_._IFELSE___V48*/ meltfptr[33] = 0 ;
                                            /*epilog*/
                                        }
                                        ;
                                    }
                                else
                                    {
                                        MELT_LOCATION("warmelt-outobj.melt:4342:/ cond.else");

                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*_#eqeq__L33*/ meltfnum[32] =
                                                ((/*_.CURESTYP__V43*/ meltfptr[25]) == ((/*!CTYPE_VALUE*/ meltfrout->tabval[8])));;
                                            MELT_LOCATION("warmelt-outobj.melt:4345:/ cond");
                                            /*cond*/ if (/*_#eqeq__L33*/ meltfnum[32]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        {
                                                            MELT_LOCATION("warmelt-outobj.melt:4346:/ locexp");
                                                            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "meltbp_aptr = (melt_ptr_t*) &"));
                                                        }
                                                        ;
                                                        /*_#GET_INT__L34*/ meltfnum[33] =
                                                            (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V17*/ meltfptr[16])));;

                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:4347:/ msend");
                                                        /*msend*/
                                                        {
                                                            union meltparam_un argtab[3];
                                                            memset(&argtab, 0, sizeof(argtab));
                                                            /*^ojbmsend.arg*/
                                                            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                                            /*^ojbmsend.arg*/
                                                            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                            /*^ojbmsend.arg*/
                                                            argtab[2].meltbp_long = /*_#GET_INT__L34*/ meltfnum[33];
                                                            /*_.OUTPUT_C_CODE__V50*/ meltfptr[30] = meltgc_send((melt_ptr_t)(/*_.CURES__V42*/ meltfptr[22]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                        }
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:4345:/ quasiblock");


                                                        /*_.PROGN___V51*/ meltfptr[26] = /*_.OUTPUT_C_CODE__V50*/ meltfptr[30];;
                                                        /*^compute*/
                                                        /*_._IFELSE___V49*/ meltfptr[36] = /*_.PROGN___V51*/ meltfptr[26];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-outobj.melt:4345:/ clear");
                                                        /*clear*/ /*_#GET_INT__L34*/ meltfnum[33] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.OUTPUT_C_CODE__V50*/ meltfptr[30] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.PROGN___V51*/ meltfptr[26] = 0 ;
                                                    }
                                                    ;
                                                }
                                            else    /*^cond.else*/
                                                {

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        MELT_LOCATION("warmelt-outobj.melt:4350:/ getslot");
                                                        {
                                                            melt_ptr_t slot=NULL, obj=NULL;
                                                            obj = (melt_ptr_t)(/*_.CURESTYP__V43*/ meltfptr[25]) /*=obj*/;
                                                            melt_object_get_field(slot,obj, 8, "CTYPE_RESFIELD");
                                                            /*_.CTYPE_RESFIELD__V52*/ meltfptr[30] = slot;
                                                        };
                                                        ;

                                                        {
                                                            /*^locexp*/
                                                            /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                                                                  melt_string_str((melt_ptr_t)(/*_.CTYPE_RESFIELD__V52*/ meltfptr[30])));
                                                        }
                                                        ;

                                                        {
                                                            MELT_LOCATION("warmelt-outobj.melt:4351:/ locexp");
                                                            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = "));
                                                        }
                                                        ;
                                                        /*_#GET_INT__L35*/ meltfnum[33] =
                                                            (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V17*/ meltfptr[16])));;

                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:4352:/ msend");
                                                        /*msend*/
                                                        {
                                                            union meltparam_un argtab[3];
                                                            memset(&argtab, 0, sizeof(argtab));
                                                            /*^ojbmsend.arg*/
                                                            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                                                            /*^ojbmsend.arg*/
                                                            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                                                            /*^ojbmsend.arg*/
                                                            argtab[2].meltbp_long = /*_#GET_INT__L35*/ meltfnum[33];
                                                            /*_.OUTPUT_C_CODE__V53*/ meltfptr[26] = meltgc_send((melt_ptr_t)(/*_.CURES__V42*/ meltfptr[22]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                        }
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:4349:/ quasiblock");


                                                        /*_.PROGN___V54*/ meltfptr[53] = /*_.OUTPUT_C_CODE__V53*/ meltfptr[26];;
                                                        /*^compute*/
                                                        /*_._IFELSE___V49*/ meltfptr[36] = /*_.PROGN___V54*/ meltfptr[53];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-outobj.melt:4345:/ clear");
                                                        /*clear*/ /*_.CTYPE_RESFIELD__V52*/ meltfptr[30] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_#GET_INT__L35*/ meltfnum[33] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.OUTPUT_C_CODE__V53*/ meltfptr[26] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.PROGN___V54*/ meltfptr[53] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;
                                            /*_._IFELSE___V48*/ meltfptr[33] = /*_._IFELSE___V49*/ meltfptr[36];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:4342:/ clear");
                                            /*clear*/ /*_#eqeq__L33*/ meltfnum[32] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_._IFELSE___V49*/ meltfptr[36] = 0 ;
                                        }
                                        ;
                                    }
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:4354:/ locexp");
                                    /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ";"));
                                }
                                ;

                                MELT_LOCATION("warmelt-outobj.melt:4335:/ clear");
                                /*clear*/ /*_.CURESTYP__V43*/ meltfptr[25] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.IFCPP___V44*/ meltfptr[27] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#GET_INT__L31*/ meltfnum[19] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.OUTPUT_LOCATION__V47*/ meltfptr[31] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#NULL__L32*/ meltfnum[17] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V48*/ meltfptr[33] = 0 ;
                                if (/*_#CURANK__L29*/ meltfnum[20]<0) break;
                            } /* end  foreach_in_multiple meltcit3__EACHTUP */

                        /*citerepilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4332:/ clear");
                        /*clear*/ /*_.CURES__V42*/ meltfptr[22] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#CURANK__L29*/ meltfnum[20] = 0 ;
                    } /*endciterblock FOREACH_IN_MULTIPLE*/
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4331:/ quasiblock");


                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*_#GET_INT__L36*/ meltfnum[33] =
            (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V17*/ meltfptr[16])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4358:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#GET_INT__L36*/ meltfnum[33];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "multimsend.send";
            /*_.OUTPUT_LOCATION__V55*/ meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.OLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit4__EACHLIST */
            for (/*_.CURPAIR__V56*/ meltfptr[26] = melt_list_first( (melt_ptr_t)/*_.ODEST__V10*/ meltfptr[9]);
                                    melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V56*/ meltfptr[26]) == MELTOBMAG_PAIR;
                                    /*_.CURPAIR__V56*/ meltfptr[26] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V56*/ meltfptr[26]))
                {
                    /*_.CURDEST__V57*/ meltfptr[53] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V56*/ meltfptr[26]);


                    /*_#GET_INT__L37*/ meltfnum[32] =
                        (melt_get_int((melt_ptr_t)(/*_.BOXDEPTHP1__V17*/ meltfptr[16])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:4363:/ msend");
                    /*msend*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^ojbmsend.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
                        /*^ojbmsend.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                        /*^ojbmsend.arg*/
                        argtab[2].meltbp_long = /*_#GET_INT__L37*/ meltfnum[32];
                        /*_.OUTPUT_C_CODE__V58*/ meltfptr[36] = meltgc_send((melt_ptr_t)(/*_.CURDEST__V57*/ meltfptr[53]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4364:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = "));
                    }
                    ;
                } /* end foreach_pair_component_in_list meltcit4__EACHLIST */
            /*_.CURPAIR__V56*/ meltfptr[26] = NULL;
            /*_.CURDEST__V57*/ meltfptr[53] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:4360:/ clear");
            /*clear*/ /*_.CURPAIR__V56*/ meltfptr[26] = 0 ;
            /*^clear*/
            /*clear*/ /*_.CURDEST__V57*/ meltfptr[53] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L37*/ meltfnum[32] = 0 ;
            /*^clear*/
            /*clear*/ /*_.OUTPUT_C_CODE__V58*/ meltfptr[36] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4366:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " meltgc_send ((melt_ptr_t)("));
        }
        ;
        /*_#plI__L38*/ meltfnum[19] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4367:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#plI__L38*/ meltfnum[19];
            /*_.OUTPUT_C_CODE__V59*/ meltfptr[25] = meltgc_send((melt_ptr_t)(/*_.ORECV__V12*/ meltfptr[11]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4368:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "), ((melt_ptr_t)("));
        }
        ;
        /*_#plI__L39*/ meltfnum[17] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4370:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#plI__L39*/ meltfnum[17];
            /*_.OUTPUT_C_CODE__V60*/ meltfptr[27] = meltgc_send((melt_ptr_t)(/*_.OSEL__V11*/ meltfptr[10]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[9])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4371:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ")), ("));
        }
        ;
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit5__EACHLIST */
            for (/*_.CURPAIR__V61*/ meltfptr[31] = melt_list_first( (melt_ptr_t)/*_.PARAMDESCLIST__V15*/ meltfptr[14]);
                                    melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V61*/ meltfptr[31]) == MELTOBMAG_PAIR;
                                    /*_.CURPAIR__V61*/ meltfptr[31] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V61*/ meltfptr[31]))
                {
                    /*_.PARD__V62*/ meltfptr[33] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V61*/ meltfptr[31]);



                    {
                        MELT_LOCATION("warmelt-outobj.melt:4376:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                              melt_string_str((melt_ptr_t)(/*_.PARD__V62*/ meltfptr[33])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4377:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " "));
                    }
                    ;
                } /* end foreach_pair_component_in_list meltcit5__EACHLIST */
            /*_.CURPAIR__V61*/ meltfptr[31] = NULL;
            /*_.PARD__V62*/ meltfptr[33] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:4373:/ clear");
            /*clear*/ /*_.CURPAIR__V61*/ meltfptr[31] = 0 ;
            /*^clear*/
            /*clear*/ /*_.PARD__V62*/ meltfptr[33] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4378:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "\"\"), "));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#gtI__L40*/ meltfnum[39] =
            ((/*_#NBARG__L3*/ meltfnum[1]) > (0));;
        MELT_LOCATION("warmelt-outobj.melt:4380:/ cond");
        /*cond*/ if (/*_#gtI__L40*/ meltfnum[39]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4381:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "argtab, ("));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:4380:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4382:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "(union meltparam_un*)0, ("));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit6__EACHLIST */
            for (/*_.CURPAIR__V63*/ meltfptr[62] = melt_list_first( (melt_ptr_t)/*_.RESDESCLIST__V16*/ meltfptr[15]);
                                    melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V63*/ meltfptr[62]) == MELTOBMAG_PAIR;
                                    /*_.CURPAIR__V63*/ meltfptr[62] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V63*/ meltfptr[62]))
                {
                    /*_.RESD__V64*/ meltfptr[63] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V63*/ meltfptr[62]);



                    {
                        MELT_LOCATION("warmelt-outobj.melt:4387:/ locexp");
                        /*add2sbuf_string*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]),
                                                              melt_string_str((melt_ptr_t)(/*_.RESD__V64*/ meltfptr[63])));
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:4388:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " "));
                    }
                    ;
                } /* end foreach_pair_component_in_list meltcit6__EACHLIST */
            /*_.CURPAIR__V63*/ meltfptr[62] = NULL;
            /*_.RESD__V64*/ meltfptr[63] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:4384:/ clear");
            /*clear*/ /*_.CURPAIR__V63*/ meltfptr[62] = 0 ;
            /*^clear*/
            /*clear*/ /*_.RESD__V64*/ meltfptr[63] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4389:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "\"\"), "));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#gtI__L41*/ meltfnum[40] =
            ((/*_#NBXRES__L4*/ meltfnum[3]) > (0));;
        MELT_LOCATION("warmelt-outobj.melt:4391:/ cond");
        /*cond*/ if (/*_#gtI__L41*/ meltfnum[40]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4392:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "restab"));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:4391:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:4393:/ locexp");
                        /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "(union meltparam_un*)0"));
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4394:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( ");"));
        }
        ;
        /*_#plI__L42*/ meltfnum[41] =
            ((1) + (/*_#DEPTH__L1*/ meltfnum[0]));;

        {
            MELT_LOCATION("warmelt-outobj.melt:4395:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#plI__L42*/ meltfnum[41]), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4396:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "}"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4397:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), (/*_#DEPTH__L1*/ meltfnum[0]), 0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4398:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L43*/ meltfnum[42] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:4399:/ cond");
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
                        /*_.REFERENCED_VALUE__V66*/ meltfptr[65] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V66*/ meltfptr[65] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#GET_INT__L44*/ meltfnum[43] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V66*/ meltfptr[65])));;
            /*^compute*/
            /*_#ltI__L45*/ meltfnum[44] =
                ((/*_#STRBUF_USEDLENGTH__L43*/ meltfnum[42]) < (/*_#GET_INT__L44*/ meltfnum[43]));;
            MELT_LOCATION("warmelt-outobj.melt:4398:/ cond");
            /*cond*/ if (/*_#ltI__L45*/ meltfnum[44]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V67*/ meltfptr[66] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4398:/ cond.else");

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
                            argtab[2].meltbp_long = 4398;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V68*/ meltfptr[67] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V67*/ meltfptr[66] = /*_.MELT_ASSERT_FAILURE_FUN__V68*/ meltfptr[67];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4398:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V68*/ meltfptr[67] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V65*/ meltfptr[64] = /*_._IFELSE___V67*/ meltfptr[66];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4398:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L43*/ meltfnum[42] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V66*/ meltfptr[65] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L44*/ meltfnum[43] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L45*/ meltfnum[44] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V67*/ meltfptr[66] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V65*/ meltfptr[64] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V65*/ meltfptr[64];;

        MELT_LOCATION("warmelt-outobj.melt:4258:/ clear");
        /*clear*/ /*_.OLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ODEST__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OSEL__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ORECV__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OARGS__V13*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OXRES__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NBARG__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NBXRES__L4*/ meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PARAMDESCLIST__V15*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RESDESCLIST__V16*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L5*/ meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BOXDEPTHP1__V17*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V18*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V19*/ meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L6*/ meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L7*/ meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L9*/ meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L12*/ meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L14*/ meltfnum[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L16*/ meltfnum[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L28*/ meltfnum[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_#GET_INT__L36*/ meltfnum[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V55*/ meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L38*/ meltfnum[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V59*/ meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L39*/ meltfnum[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V60*/ meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L40*/ meltfnum[39] = 0 ;
        /*^clear*/
        /*clear*/ /*_#gtI__L41*/ meltfnum[40] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L42*/ meltfnum[41] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V65*/ meltfptr[64] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4256:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:4256:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJMULTIMSEND", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_91_WARMELTmiOUTOBJ_OUTPUCOD_OBJMULTIMSEND*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR fromline 1752 */

    /** start of frame for meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR// fromline 1568
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
        MeltFrame_meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<18> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR), clos) {};
        MeltFrame_meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR() //the constructor fromline 1642
            : Melt_CallFrameWithValues<18> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<18> (fil,lin, sizeof(MeltFrame_meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR)) {};
        MeltFrame_meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<18> (fil,lin, sizeof(MeltFrame_meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR


    /** end of frame for meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR fromline 1697**/

    /* end of frame for routine meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR fromline 1756 */

    /* classy proc frame meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR */ MeltFrame_meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("OUTPUCOD_OBJCLEAR", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:4406:/ getarg");
    /*_.OCLEAR__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

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
        MELT_LOCATION("warmelt-outobj.melt:4407:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.OCLEAR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_OBJCLEAR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-outobj.melt:4407:/ cond");
            /*cond*/ if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4407:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check oclear";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4407;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.OCLEAR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4407:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4407:/ clear");
            /*clear*/ /*_#IS_A__L2*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:4408:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OCLEAR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 0, "OBI_LOC");
            /*_.CLOC__V9*/ meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-outobj.melt:4409:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.OCLEAR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "OCLR_VLOC");
            /*_.CVL__V10*/ meltfptr[9] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4411:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_cstring =  "clear";
            /*_.OUTPUT_LOCATION__V11*/ meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!OUTPUT_LOCATION*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.CLOC__V9*/ meltfptr[5]), (MELTBPARSTR_PTR MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
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
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.OCLEAR__V2*/ meltfptr[1];
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

        {
            MELT_LOCATION("warmelt-outobj.melt:4413:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( "/*clear*/ "));
        }
        ;
        /*_#plI__L3*/ meltfnum[1] =
            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4414:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.DECLBUF__V3*/ meltfptr[2];
            /*^ojbmsend.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
            /*^ojbmsend.arg*/
            argtab[2].meltbp_long = /*_#plI__L3*/ meltfnum[1];
            /*_.OUTPUT_C_CODE__V14*/ meltfptr[12] = meltgc_send((melt_ptr_t)(/*_.CVL__V10*/ meltfptr[9]), (melt_ptr_t)((/*!OUTPUT_C_CODE*/ meltfrout->tabval[4])), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:4415:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]), ( " = 0 "));
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:4416:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRBUF_USEDLENGTH__L4*/ meltfnum[3] =
                melt_strbuf_usedlength((melt_ptr_t)(/*_.IMPLBUF__V4*/ meltfptr[3]));;
            MELT_LOCATION("warmelt-outobj.melt:4417:/ cond");
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
            /*_#GET_INT__L5*/ meltfnum[4] =
                (melt_get_int((melt_ptr_t)(/*_.REFERENCED_VALUE__V16*/ meltfptr[15])));;
            /*^compute*/
            /*_#ltI__L6*/ meltfnum[5] =
                ((/*_#STRBUF_USEDLENGTH__L4*/ meltfnum[3]) < (/*_#GET_INT__L5*/ meltfnum[4]));;
            MELT_LOCATION("warmelt-outobj.melt:4416:/ cond");
            /*cond*/ if (/*_#ltI__L6*/ meltfnum[5]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V17*/ meltfptr[16] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:4416:/ cond.else");

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
                            argtab[2].meltbp_long = 4416;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.IMPLBUF__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V17*/ meltfptr[16] = /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:4416:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V15*/ meltfptr[14] = /*_._IFELSE___V17*/ meltfptr[16];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:4416:/ clear");
            /*clear*/ /*_#STRBUF_USEDLENGTH__L4*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_.REFERENCED_VALUE__V16*/ meltfptr[15] = 0 ;
            /*^clear*/
            /*clear*/ /*_#GET_INT__L5*/ meltfnum[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_#ltI__L6*/ meltfnum[5] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V17*/ meltfptr[16] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V15*/ meltfptr[14] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.LET___V8*/ meltfptr[6] = /*_.IFCPP___V15*/ meltfptr[14];;

        MELT_LOCATION("warmelt-outobj.melt:4408:/ clear");
        /*clear*/ /*_.CLOC__V9*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CVL__V10*/ meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_LOCATION__V11*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OUTPUT_C_CODE__V14*/ meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V15*/ meltfptr[14] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:4406:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V8*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-outobj.melt:4406:/ locexp");
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
    melt_trace_end("OUTPUCOD_OBJCLEAR", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_92_WARMELTmiOUTOBJ_OUTPUCOD_OBJCLEAR*/




/**** end of warmelt-outobj+04.cc ****/

