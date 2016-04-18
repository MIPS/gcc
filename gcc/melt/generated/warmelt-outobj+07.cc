/* GCC MELT GENERATED C++ FILE warmelt-outobj+07.cc - DO NOT EDIT - see http://gcc-melt.org/ */
/* secondary MELT generated C++ file of rank #7 */
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt_f7[] = MELT_RUN_HASHMD5 /* from melt-run.h */;


/**** warmelt-outobj+07.cc declarations ****/
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



/**** warmelt-outobj+07.cc implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7 fromline 1752 */

    /** start of frame for meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7 of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7// fromline 1568
        : public Melt_CallFrameWithValues<5>
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
        MeltFrame_meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<5> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7), clos) {};
        MeltFrame_meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7() //the constructor fromline 1642
            : Melt_CallFrameWithValues<5> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7)) {};
        MeltFrame_meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7


    /** end of frame for meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7 fromline 1697**/

    /* end of frame for routine meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7 fromline 1756 */

    /* classy proc frame meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7 */ MeltFrame_meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7 fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl7", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:7172:/ getarg");
    /*_.V__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7173:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:7173:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:7173:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7173;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "compile_list_sexpr:errorhandler v=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.V__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n.. modnamstr=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &(/*~MODNAMSTR*/ meltfclos->tabval[0]);
                            /*_.MELT_DEBUG_FUN__V4*/ meltfptr[3] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V3*/ meltfptr[2] = /*_.MELT_DEBUG_FUN__V4*/ meltfptr[3];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7173:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V4*/ meltfptr[3] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V3*/ meltfptr[2] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7173:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V3*/ meltfptr[2] = 0 ;
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
            MELT_LOCATION("warmelt-outobj.melt:7174:/ locexp");

#if MELT_HAVE_DEBUG
            if (melt_need_debug (0))
                melt_dbgshortbacktrace(( "compile_list_sexpr:errorhandler"), (15));
#endif
            ;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7175:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.V__V2*/ meltfptr[1];
            /*_.FATAL_COMPILE_ERROR__V5*/ meltfptr[3] =  melt_apply ((meltclosure_ptr_t)((/*!FATAL_COMPILE_ERROR*/ meltfrout->tabval[1])), (melt_ptr_t)((/*~MODNAMSTR*/ meltfclos->tabval[0])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7172:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.FATAL_COMPILE_ERROR__V5*/ meltfptr[3];;

        {
            MELT_LOCATION("warmelt-outobj.melt:7172:/ locexp");
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
        /*clear*/ /*_.FATAL_COMPILE_ERROR__V5*/ meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl7", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_134_WARMELTmiOUTOBJ_LAMBDA_cl7*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR fromline 1752 */

    /** start of frame for meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR// fromline 1568
        : public Melt_CallFrameWithValues<57>
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
        MeltFrame_meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<57> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR), clos) {};
        MeltFrame_meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR() //the constructor fromline 1642
            : Melt_CallFrameWithValues<57> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<57> (fil,lin, sizeof(MeltFrame_meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR)) {};
        MeltFrame_meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<57> (fil,lin, sizeof(MeltFrame_meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR


    /** end of frame for meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR fromline 1697**/

    /* end of frame for routine meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR fromline 1756 */

    /* classy proc frame meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR */ MeltFrame_meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:7213:/ getarg");
    /*_.LSEXP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODNAMSTR__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MODNAMSTR__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7214:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:7214:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:7214:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[8];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7214;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "compile_first_bootstrap_list_sexpr modnamstr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MODNAMSTR__V3*/ meltfptr[2];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " lsexp=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &(/*!DEBUG_LESS*/ meltfrout->tabval[1]);
                            /*^apply.arg*/
                            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.LSEXP__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V4*/ meltfptr[3] = /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7214:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:7214:/ locexp");
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
            MELT_LOCATION("warmelt-outobj.melt:7215:/ locexp");

#if MELT_HAVE_DEBUG
            if (melt_need_debug (0))
                melt_dbgshortbacktrace(( "compile_list_sexpr"), (10));
#endif
            ;
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7216:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_LIST__L3*/ meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.LSEXP__V2*/ meltfptr[1])) == MELTOBMAG_LIST);;
            MELT_LOCATION("warmelt-outobj.melt:7216:/ cond");
            /*cond*/ if (/*_#IS_LIST__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[3] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7216:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check lsexp";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7216;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.LSEXP__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V7*/ meltfptr[3] = /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7216:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V6*/ meltfptr[4] = /*_._IFELSE___V7*/ meltfptr[3];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7216:/ clear");
            /*clear*/ /*_#IS_LIST__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V7*/ meltfptr[3] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V6*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7217:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRING__L4*/ meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODNAMSTR__V3*/ meltfptr[2])) == MELTOBMAG_STRING);;
            MELT_LOCATION("warmelt-outobj.melt:7217:/ cond");
            /*cond*/ if (/*_#IS_STRING__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[3] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7217:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modnamstr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7217;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODNAMSTR__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V10*/ meltfptr[3] = /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7217:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V9*/ meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[3];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7217:/ clear");
            /*clear*/ /*_#IS_STRING__L4*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V10*/ meltfptr[3] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V9*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7218:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRING_DYNLOADED_SUFFIXED__L5*/ meltfnum[1] =
                /*string_dynloaded_suffixed:*/ (melt_string_is_ending ((melt_ptr_t)/*_.MODNAMSTR__V3*/ meltfptr[2],
                                                MELT_DYNLOADED_SUFFIX));;
            /*^compute*/
            /*_#NOT__L6*/ meltfnum[0] =
                (!(/*_#STRING_DYNLOADED_SUFFIXED__L5*/ meltfnum[1]));;
            MELT_LOCATION("warmelt-outobj.melt:7218:/ cond");
            /*cond*/ if (/*_#NOT__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V13*/ meltfptr[3] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7218:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "compile_first_bootstrap_list_sexpr modulename not ended with MELT_DYNLOADED_S\
UFFIX.";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7218;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODNAMSTR__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V14*/ meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V13*/ meltfptr[3] = /*_.MELT_ASSERT_FAILURE_FUN__V14*/ meltfptr[13];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7218:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V14*/ meltfptr[13] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V12*/ meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[3];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7218:/ clear");
            /*clear*/ /*_#STRING_DYNLOADED_SUFFIXED__L5*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_#NOT__L6*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V13*/ meltfptr[3] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V12*/ meltfptr[10] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7220:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRING_SUFFIXED__L7*/ meltfnum[1] =
                /*string_suffixed:*/(melt_string_is_ending ((melt_ptr_t)/*_.MODNAMSTR__V3*/ meltfptr[2],  ".melt"));;
            /*^compute*/
            /*_#NOT__L8*/ meltfnum[0] =
                (!(/*_#STRING_SUFFIXED__L7*/ meltfnum[1]));;
            MELT_LOCATION("warmelt-outobj.melt:7220:/ cond");
            /*cond*/ if (/*_#NOT__L8*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V16*/ meltfptr[3] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7220:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "compile_first_bootstrap_list_sexpr modulename not ended with .melt";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7220;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODNAMSTR__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V17*/ meltfptr[16] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V16*/ meltfptr[3] = /*_.MELT_ASSERT_FAILURE_FUN__V17*/ meltfptr[16];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7220:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V17*/ meltfptr[16] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V15*/ meltfptr[13] = /*_._IFELSE___V16*/ meltfptr[3];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7220:/ clear");
            /*clear*/ /*_#STRING_SUFFIXED__L7*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_#NOT__L8*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V16*/ meltfptr[3] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V15*/ meltfptr[13] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7222:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRING_SUFFIXED__L9*/ meltfnum[1] =
                /*string_suffixed:*/(melt_string_is_ending ((melt_ptr_t)/*_.MODNAMSTR__V3*/ meltfptr[2],  ".c"));;
            /*^compute*/
            /*_#NOT__L10*/ meltfnum[0] =
                (!(/*_#STRING_SUFFIXED__L9*/ meltfnum[1]));;
            MELT_LOCATION("warmelt-outobj.melt:7222:/ cond");
            /*cond*/ if (/*_#NOT__L10*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V19*/ meltfptr[3] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7222:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "compile_first_bootstrap_list_sexpr modulename not ended with .c";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7222;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODNAMSTR__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V19*/ meltfptr[3] = /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7222:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V18*/ meltfptr[16] = /*_._IFELSE___V19*/ meltfptr[3];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7222:/ clear");
            /*clear*/ /*_#STRING_SUFFIXED__L9*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_#NOT__L10*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V19*/ meltfptr[3] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V18*/ meltfptr[16] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7224:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRING_SUFFIXED__L11*/ meltfnum[1] =
                /*string_suffixed:*/(melt_string_is_ending ((melt_ptr_t)/*_.MODNAMSTR__V3*/ meltfptr[2],  ".cc"));;
            /*^compute*/
            /*_#NOT__L12*/ meltfnum[0] =
                (!(/*_#STRING_SUFFIXED__L11*/ meltfnum[1]));;
            MELT_LOCATION("warmelt-outobj.melt:7224:/ cond");
            /*cond*/ if (/*_#NOT__L12*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V22*/ meltfptr[3] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7224:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "compile_first_bootstrap_list_sexpr modulename not ended with .cc";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7224;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODNAMSTR__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V23*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V22*/ meltfptr[3] = /*_.MELT_ASSERT_FAILURE_FUN__V23*/ meltfptr[22];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7224:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V23*/ meltfptr[22] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V21*/ meltfptr[19] = /*_._IFELSE___V22*/ meltfptr[3];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7224:/ clear");
            /*clear*/ /*_#STRING_SUFFIXED__L11*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_#NOT__L12*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V22*/ meltfptr[3] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V21*/ meltfptr[19] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:7226:/ locexp");

            /* compile_first_bootstrap_list_sexpr CHECK_WARMELT_FIRST_BOOTSTRAPPING_CHUNK__2 */
            melt_checkmsg ("bootstrapping first file",
                           melt_flag_bootstrapping
                           && melt_string_str((melt_ptr_t) /*_.MODNAMSTR__V3*/ meltfptr[2])
                           && strstr(melt_string_str((melt_ptr_t) /*_.MODNAMSTR__V3*/ meltfptr[2]), "first")) ;
            ;
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:7234:/ quasiblock");


        /*_.MODNAKEDNAME__V25*/ meltfptr[3] =
            (meltgc_new_string_nakedbasename((meltobject_ptr_t)((/*!DISCR_STRING*/ meltfrout->tabval[3])),
                                             melt_string_str((melt_ptr_t)(/*_.MODNAMSTR__V3*/ meltfptr[2]))));;
        MELT_LOCATION("warmelt-outobj.melt:7236:/ blockmultialloc");
        /*multiallocblock*/
        {
            struct meltletrec_1_st
            {
                struct MELT_MULTIPLE_STRUCT(2) rtup_0__TUPLREC__x7;
                long meltletrec_1_endgap;
            } *meltletrec_1_ptr = 0;
            meltletrec_1_ptr = (struct meltletrec_1_st *) meltgc_allocate (sizeof (struct meltletrec_1_st), 0);
            /*^blockmultialloc.initfill*/
            /*inimult rtup_0__TUPLREC__x7*/
            /*_.TUPLREC___V27*/ meltfptr[26] = (melt_ptr_t) &meltletrec_1_ptr->rtup_0__TUPLREC__x7;
            meltletrec_1_ptr->rtup_0__TUPLREC__x7.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_MULTIPLE))));
            meltletrec_1_ptr->rtup_0__TUPLREC__x7.nbval = 2;


            /*^putuple*/
            /*putupl#15*/
            melt_assertmsg("putupl [:7236] #15 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V27*/ meltfptr[26]))== MELTOBMAG_MULTIPLE);
            melt_assertmsg("putupl [:7236] #15 checkoff", (0>=0 && 0< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V27*/ meltfptr[26]))));
            ((meltmultiple_ptr_t)(/*_.TUPLREC___V27*/ meltfptr[26]))->tabval[0] = (melt_ptr_t)((/*!konst_6_MACROmiFIRST*/ meltfrout->tabval[6]));
            ;
            /*^putuple*/
            /*putupl#16*/
            melt_assertmsg("putupl [:7236] #16 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V27*/ meltfptr[26]))== MELTOBMAG_MULTIPLE);
            melt_assertmsg("putupl [:7236] #16 checkoff", (1>=0 && 1< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V27*/ meltfptr[26]))));
            ((meltmultiple_ptr_t)(/*_.TUPLREC___V27*/ meltfptr[26]))->tabval[1] = (melt_ptr_t)(/*_.MODNAMSTR__V3*/ meltfptr[2]);
            ;
            /*^touch*/
            meltgc_touch(/*_.TUPLREC___V27*/ meltfptr[26]);
            ;
            /*_.TUPLE___V26*/ meltfptr[25] = /*_.TUPLREC___V27*/ meltfptr[26];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7236:/ clear");
            /*clear*/ /*_.TUPLREC___V27*/ meltfptr[26] = 0 ;
            /*^clear*/
            /*clear*/ /*_.TUPLREC___V27*/ meltfptr[26] = 0 ;
        } /*end multiallocblock*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.TUPLE___V26*/ meltfptr[25];
            /*_.MACROENV__V28*/ meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!FRESH_ENV*/ meltfrout->tabval[4])), (melt_ptr_t)((/*!INITIAL_ENVIRONMENT*/ meltfrout->tabval[5])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_.MAKE_MAPSTRING__V29*/ meltfptr[28] =
            (meltgc_new_mapstrings( (meltobject_ptr_t) ((/*!DISCR_MAP_STRINGS*/ meltfrout->tabval[8])), (490)));;
        /*^compute*/
        /*_.MAKE_MAPSTRING__V30*/ meltfptr[29] =
            (meltgc_new_mapstrings( (meltobject_ptr_t) ((/*!DISCR_MAP_STRINGS*/ meltfrout->tabval[8])), (240)));;
        /*^compute*/
        /*_.MAKE_INTEGERBOX__V31*/ meltfptr[30] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[9])), (0)));;
        /*^compute*/
        /*_.MAKE_LIST__V32*/ meltfptr[31] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[10]))));;
        /*^compute*/
        /*_.MAKE_LIST__V33*/ meltfptr[32] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[10]))));;
        /*^compute*/
        /*_.MAKE_LIST__V34*/ meltfptr[33] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[10]))));;
        /*^compute*/
        /*_.MAKE_LIST__V35*/ meltfptr[34] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[10]))));;
        MELT_LOCATION("warmelt-outobj.melt:7252:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V37*/ meltfptr[36] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_12*/ meltfrout->tabval[12])), (1));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V37*/ meltfptr[36])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V37*/ meltfptr[36])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V37*/ meltfptr[36])->tabval[0] = (melt_ptr_t)(/*_.MODNAMSTR__V3*/ meltfptr[2]);
        ;
        /*_.LAMBDA___V36*/ meltfptr[35] = /*_.LAMBDA___V37*/ meltfptr[36];;
        /*^compute*/
        /*_.MAKE_INTEGERBOX__V38*/ meltfptr[37] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[9])), (0)));;
        /*^compute*/
        /*_.MAKE_LIST__V39*/ meltfptr[38] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[10]))));;
        /*^compute*/
        /*_.MAKE_MAPSTRING__V40*/ meltfptr[39] =
            (meltgc_new_mapstrings( (meltobject_ptr_t) ((/*!DISCR_MAP_STRINGS*/ meltfrout->tabval[8])), (31)));;
        MELT_LOCATION("warmelt-outobj.melt:7237:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_FIRST_MODULE_CONTEXT*/ meltfrout->tabval[7])), (22), "CLASS_FIRST_MODULE_CONTEXT");
            /*_.INST__V42*/ meltfptr[41] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_MODULENAME", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (1), (/*_.MODNAKEDNAME__V25*/ meltfptr[3]), "MOCX_MODULENAME");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_EXPFIELDICT", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (2), (/*_.MAKE_MAPSTRING__V29*/ meltfptr[28]), "MOCX_EXPFIELDICT");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_EXPCLASSDICT", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (3), (/*_.MAKE_MAPSTRING__V30*/ meltfptr[29]), "MOCX_EXPCLASSDICT");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_INITIALENV", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (4), ((/*nil*/NULL)), "MOCX_INITIALENV");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_BASENV", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (6), ((/*!INITIAL_ENVIRONMENT*/ meltfrout->tabval[5])), "MOCX_BASENV");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_MACROENV", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (5), (/*_.MACROENV__V28*/ meltfptr[26]), "MOCX_MACROENV");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_FUNCOUNT", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (7), (/*_.MAKE_INTEGERBOX__V31*/ meltfptr[30]), "MOCX_FUNCOUNT");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_FILETUPLE", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (8), ((/*nil*/NULL)), "MOCX_FILETUPLE");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_CHEADERLIST", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (11), (/*_.MAKE_LIST__V32*/ meltfptr[31]), "MOCX_CHEADERLIST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_CIMPLEMENTLIST", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (12), (/*_.MAKE_LIST__V33*/ meltfptr[32]), "MOCX_CIMPLEMENTLIST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_PACKAGEPCLIST", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (15), (/*_.MAKE_LIST__V34*/ meltfptr[33]), "MOCX_PACKAGEPCLIST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_GENDEVLIST", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (14), (/*_.MAKE_LIST__V35*/ meltfptr[34]), "MOCX_GENDEVLIST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_ERRORHANDLER", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (16), (/*_.LAMBDA___V36*/ meltfptr[35]), "MOCX_ERRORHANDLER");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_VARCOUNT", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (17), (/*_.MAKE_INTEGERBOX__V38*/ meltfptr[37]), "MOCX_VARCOUNT");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_VARLIST", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (18), (/*_.MAKE_LIST__V39*/ meltfptr[38]), "MOCX_VARLIST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_HOOKDICT", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (19), (/*_.MAKE_MAPSTRING__V40*/ meltfptr[39]), "MOCX_HOOKDICT");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_MACROLIST", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (20), ((/*nil*/NULL)), "MOCX_MACROLIST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_ISGPLCOMPATIBLE", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (21), ((/*nil*/NULL)), "MOCX_ISGPLCOMPATIBLE");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V42*/ meltfptr[41], "newly made instance");
        ;
        /*_.MODCTX__V41*/ meltfptr[40] = /*_.INST__V42*/ meltfptr[41];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7259:/ apply");
        /*apply*/
        {
            /*_.NCX__V43*/ meltfptr[42] =  melt_apply ((meltclosure_ptr_t)((/*!CREATE_NORMCONTEXT*/ meltfrout->tabval[13])), (melt_ptr_t)(/*_.MODCTX__V41*/ meltfptr[40]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7261:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L13*/ meltfnum[1] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7261:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L13*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L14*/ meltfnum[0] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7261:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[9];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L14*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7261;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "compile_first_bootstrap_list_sexpr modctx=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &(/*!DEBUG_LESS*/ meltfrout->tabval[1]);
                            /*^apply.arg*/
                            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V41*/ meltfptr[40];
                            /*^apply.arg*/
                            argtab[6].meltbp_cstring =  "\n ncx=";
                            /*^apply.arg*/
                            argtab[7].meltbp_aptr = (melt_ptr_t*) &(/*!DEBUG_LESS*/ meltfrout->tabval[1]);
                            /*^apply.arg*/
                            argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.NCX__V43*/ meltfptr[42];
                            /*_.MELT_DEBUG_FUN__V45*/ meltfptr[44] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V44*/ meltfptr[43] = /*_.MELT_DEBUG_FUN__V45*/ meltfptr[44];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7261:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L14*/ meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V45*/ meltfptr[44] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V44*/ meltfptr[43] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7261:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L13*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V44*/ meltfptr[43] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7262:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L15*/ meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)((/*!INITIAL_ENVIRONMENT*/ meltfrout->tabval[5])), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[14])));;
            MELT_LOCATION("warmelt-outobj.melt:7262:/ cond");
            /*cond*/ if (/*_#IS_A__L15*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V47*/ meltfptr[43] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7262:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "compile_first_bootstrap_list_sexpr check initial_environment";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7262;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!INITIAL_ENVIRONMENT*/ meltfrout->tabval[5]);
                            /*_.MELT_ASSERT_FAILURE_FUN__V48*/ meltfptr[47] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V47*/ meltfptr[43] = /*_.MELT_ASSERT_FAILURE_FUN__V48*/ meltfptr[47];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7262:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V48*/ meltfptr[47] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V46*/ meltfptr[44] = /*_._IFELSE___V47*/ meltfptr[43];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7262:/ clear");
            /*clear*/ /*_#IS_A__L15*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V47*/ meltfptr[43] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V46*/ meltfptr[44] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7264:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L16*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.NCX__V43*/ meltfptr[42]), (melt_ptr_t)((/*!CLASS_NORMALIZATION_CONTEXT*/ meltfrout->tabval[15])));;
            MELT_LOCATION("warmelt-outobj.melt:7264:/ cond");
            /*cond*/ if (/*_#IS_A__L16*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V50*/ meltfptr[43] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7264:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check ncx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7264;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.NCX__V43*/ meltfptr[42];
                            /*_.MELT_ASSERT_FAILURE_FUN__V51*/ meltfptr[50] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V50*/ meltfptr[43] = /*_.MELT_ASSERT_FAILURE_FUN__V51*/ meltfptr[50];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7264:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V51*/ meltfptr[50] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V49*/ meltfptr[47] = /*_._IFELSE___V50*/ meltfptr[43];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7264:/ clear");
            /*clear*/ /*_#IS_A__L16*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V50*/ meltfptr[43] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V49*/ meltfptr[47] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:7265:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!INITIAL_ENVIRONMENT*/ meltfrout->tabval[5]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!MACROEXPAND_1*/ meltfrout->tabval[17]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V41*/ meltfptr[40];
            /*_.XLIST__V53*/ meltfptr[43] =  melt_apply ((meltclosure_ptr_t)((/*!MACROEXPAND_TOPLEVEL_LIST*/ meltfrout->tabval[16])), (melt_ptr_t)(/*_.LSEXP__V2*/ meltfptr[1]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_#LENXLIST__L17*/ meltfnum[0] =
            (melt_list_length((melt_ptr_t)(/*_.XLIST__V53*/ meltfptr[43])));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7268:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L18*/ meltfnum[1] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7268:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L18*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L19*/ meltfnum[18] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7268:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[6];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L19*/ meltfnum[18];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7268;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "compile_first_bootstrap_list_sexpr after macroexpansion and before\
 translation modctx=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &(/*!DEBUG_LESS*/ meltfrout->tabval[1]);
                            /*^apply.arg*/
                            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V41*/ meltfptr[40];
                            /*_.MELT_DEBUG_FUN__V55*/ meltfptr[54] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V54*/ meltfptr[53] = /*_.MELT_DEBUG_FUN__V55*/ meltfptr[54];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7268:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L19*/ meltfnum[18] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V55*/ meltfptr[54] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V54*/ meltfptr[53] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7268:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L18*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V54*/ meltfptr[53] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7269:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_cstring =  "compile first got $1 expanded expressions in $2";
            /*^apply.arg*/
            argtab[1].meltbp_long = /*_#LENXLIST__L17*/ meltfnum[0];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODNAMSTR__V3*/ meltfptr[2];
            /*_.INFORM_AT__V56*/ meltfptr[54] =  melt_apply ((meltclosure_ptr_t)((/*!INFORM_AT*/ meltfrout->tabval[18])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7271:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[6];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MODNAMSTR__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V41*/ meltfptr[40];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.NCX__V43*/ meltfptr[42];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*!INITIAL_ENVIRONMENT*/ meltfrout->tabval[5]);
            /*^apply.arg*/
            argtab[4].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^apply.arg*/
            argtab[5].meltbp_aptr = (melt_ptr_t*) &(/*!COMPILE2OBJ_INITPROC*/ meltfrout->tabval[20]);
            /*_.TRANSLATE_MACROEXPANDED_LIST__V57*/ meltfptr[53] =  melt_apply ((meltclosure_ptr_t)((/*!TRANSLATE_MACROEXPANDED_LIST*/ meltfrout->tabval[19])), (melt_ptr_t)(/*_.XLIST__V53*/ meltfptr[43]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.LET___V52*/ meltfptr[50] = /*_.TRANSLATE_MACROEXPANDED_LIST__V57*/ meltfptr[53];;

        MELT_LOCATION("warmelt-outobj.melt:7265:/ clear");
        /*clear*/ /*_.XLIST__V53*/ meltfptr[43] = 0 ;
        /*^clear*/
        /*clear*/ /*_#LENXLIST__L17*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_.INFORM_AT__V56*/ meltfptr[54] = 0 ;
        /*^clear*/
        /*clear*/ /*_.TRANSLATE_MACROEXPANDED_LIST__V57*/ meltfptr[53] = 0 ;
        /*_.LET___V24*/ meltfptr[22] = /*_.LET___V52*/ meltfptr[50];;

        MELT_LOCATION("warmelt-outobj.melt:7234:/ clear");
        /*clear*/ /*_.MODNAKEDNAME__V25*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.TUPLE___V26*/ meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MACROENV__V28*/ meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_MAPSTRING__V29*/ meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_MAPSTRING__V30*/ meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_INTEGERBOX__V31*/ meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_LIST__V32*/ meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_LIST__V33*/ meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_LIST__V34*/ meltfptr[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_LIST__V35*/ meltfptr[34] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LAMBDA___V36*/ meltfptr[35] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_INTEGERBOX__V38*/ meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_LIST__V39*/ meltfptr[38] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_MAPSTRING__V40*/ meltfptr[39] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MODCTX__V41*/ meltfptr[40] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NCX__V43*/ meltfptr[42] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V46*/ meltfptr[44] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V49*/ meltfptr[47] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V52*/ meltfptr[50] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7213:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V24*/ meltfptr[22];;

        {
            MELT_LOCATION("warmelt-outobj.melt:7213:/ locexp");
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
        /*clear*/ /*_.IFCPP___V6*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V9*/ meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V12*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V15*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V18*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V21*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V24*/ meltfptr[22] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_135_WARMELTmiOUTOBJ_COMPILE_FIRST_BOOTSTRAP_LIST_SEXPR*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8 fromline 1752 */

    /** start of frame for meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8 of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8// fromline 1568
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
        MeltFrame_meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<3> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8), clos) {};
        MeltFrame_meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8() //the constructor fromline 1642
            : Melt_CallFrameWithValues<3> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8)) {};
        MeltFrame_meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8


    /** end of frame for meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8 fromline 1697**/

    /* end of frame for routine meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8 fromline 1756 */

    /* classy proc frame meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8 */ MeltFrame_meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8 fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl8", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:7252:/ getarg");
    /*_.V__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
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
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.V__V2*/ meltfptr[1];
            /*_.FATAL_COMPILE_ERROR__V3*/ meltfptr[2] =  melt_apply ((meltclosure_ptr_t)((/*!FATAL_COMPILE_ERROR*/ meltfrout->tabval[0])), (melt_ptr_t)((/*~MODNAMSTR*/ meltfclos->tabval[0])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*^quasiblock*/


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.FATAL_COMPILE_ERROR__V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-outobj.melt:7252:/ locexp");
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
        /*clear*/ /*_.FATAL_COMPILE_ERROR__V3*/ meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl8", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_136_WARMELTmiOUTOBJ_LAMBDA_cl8*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN fromline 1752 */

    /** start of frame for meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN// fromline 1568
        : public Melt_CallFrameWithValues<109>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[23];
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
        MeltFrame_meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<109> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN), clos) {};
        MeltFrame_meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN() //the constructor fromline 1642
            : Melt_CallFrameWithValues<109> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<109> (fil,lin, sizeof(MeltFrame_meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN)) {};
        MeltFrame_meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<109> (fil,lin, sizeof(MeltFrame_meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN


    /** end of frame for meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN fromline 1697**/

    /* end of frame for routine meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN fromline 1756 */

    /* classy proc frame meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN */ MeltFrame_meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("AUTOBOX_NORMAL_RETURN", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:7288:/ getarg");
    /*_.NEXP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.CTYP__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.CTYP__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.NCX__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.NCX__V4*/ meltfptr[3])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7289:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:7289:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:7289:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[10];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7289;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "autobox_normal_return nexp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.NEXP__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " ctyp=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.CTYP__V3*/ meltfptr[2];
                            /*^apply.arg*/
                            argtab[7].meltbp_cstring =  " ncx=";
                            /*^apply.arg*/
                            argtab[8].meltbp_aptr = (melt_ptr_t*) &(/*!DEBUG_LESS*/ meltfrout->tabval[1]);
                            /*^apply.arg*/
                            argtab[9].meltbp_aptr = (melt_ptr_t*) &/*_.NCX__V4*/ meltfptr[3];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7289:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0 ;
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
                MELT_LOCATION("warmelt-outobj.melt:7289:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0 ;
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

        {
            MELT_LOCATION("warmelt-outobj.melt:7290:/ locexp");

#if MELT_HAVE_DEBUG
            if (melt_need_debug (0))
                melt_dbgshortbacktrace(( "autobox_normal_return"), (12));
#endif
            ;
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7291:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.CTYP__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-outobj.melt:7291:/ cond");
            /*cond*/ if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7291:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check ctyp";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7291;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CTYP__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7291:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7291:/ clear");
            /*clear*/ /*_#IS_A__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:7292:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            /*_.CSYM__V11*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!CLONE_SYMBOL*/ meltfrout->tabval[4])), (melt_ptr_t)((/*!konst_5_RETAUTOBOXVAL*/ meltfrout->tabval[5])), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7293:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_NORMAL_LET_BINDING*/ meltfrout->tabval[6])), (4), "CLASS_NORMAL_LET_BINDING");
            /*_.INST__V13*/ meltfptr[12] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LETBIND_LOC", melt_magic_discr((melt_ptr_t)(/*_.INST__V13*/ meltfptr[12])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V13*/ meltfptr[12]), (3), ((/*nil*/NULL)), "LETBIND_LOC");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @BINDER", melt_magic_discr((melt_ptr_t)(/*_.INST__V13*/ meltfptr[12])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V13*/ meltfptr[12]), (0), (/*_.CSYM__V11*/ meltfptr[4]), "BINDER");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LETBIND_TYPE", melt_magic_discr((melt_ptr_t)(/*_.INST__V13*/ meltfptr[12])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V13*/ meltfptr[12]), (1), ((/*!CTYPE_VALUE*/ meltfrout->tabval[7])), "LETBIND_TYPE");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LETBIND_EXPR", melt_magic_discr((melt_ptr_t)(/*_.INST__V13*/ meltfptr[12])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V13*/ meltfptr[12]), (2), ((/*nil*/NULL)), "LETBIND_EXPR");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V13*/ meltfptr[12], "newly made instance");
        ;
        /*_.CBIND__V12*/ meltfptr[11] = /*_.INST__V13*/ meltfptr[12];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7299:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_NREP_LOCSYMOCC*/ meltfrout->tabval[8])), (4), "CLASS_NREP_LOCSYMOCC");
            /*_.INST__V15*/ meltfptr[14] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @NREP_LOC", melt_magic_discr((melt_ptr_t)(/*_.INST__V15*/ meltfptr[14])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V15*/ meltfptr[14]), (0), ((/*nil*/NULL)), "NREP_LOC");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @NOCC_CTYP", melt_magic_discr((melt_ptr_t)(/*_.INST__V15*/ meltfptr[14])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V15*/ meltfptr[14]), (2), ((/*!CTYPE_VALUE*/ meltfrout->tabval[7])), "NOCC_CTYP");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @NOCC_SYMB", melt_magic_discr((melt_ptr_t)(/*_.INST__V15*/ meltfptr[14])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V15*/ meltfptr[14]), (1), (/*_.CSYM__V11*/ meltfptr[4]), "NOCC_SYMB");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @NOCC_BIND", melt_magic_discr((melt_ptr_t)(/*_.INST__V15*/ meltfptr[14])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V15*/ meltfptr[14]), (3), (/*_.CBIND__V12*/ meltfptr[11]), "NOCC_BIND");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V15*/ meltfptr[14], "newly made instance");
        ;
        /*_.CLOCC__V14*/ meltfptr[13] = /*_.INST__V15*/ meltfptr[14];;
        MELT_LOCATION("warmelt-outobj.melt:7304:/ blockmultialloc");
        /*multiallocblock*/
        {
            struct meltletrec_1_st
            {
                struct meltpair_st rpair_0__CBIND_x1;
                struct meltlist_st rlist_1__LIST_;
                long meltletrec_1_endgap;
            } *meltletrec_1_ptr = 0;
            meltletrec_1_ptr = (struct meltletrec_1_st *) meltgc_allocate (sizeof (struct meltletrec_1_st), 0);
            /*^blockmultialloc.initfill*/
            /*inipair rpair_0__CBIND_x1*/
            /*_.CBIND__V17*/ meltfptr[16] = (melt_ptr_t) &meltletrec_1_ptr->rpair_0__CBIND_x1;
            meltletrec_1_ptr->rpair_0__CBIND_x1.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_PAIR))));

            /*inilist rlist_1__LIST_*/
            /*_.LIST___V18*/ meltfptr[17] = (melt_ptr_t) &meltletrec_1_ptr->rlist_1__LIST_;
            meltletrec_1_ptr->rlist_1__LIST_.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_LIST))));



            /*^putpairhead*/
            /*putpairhead*/
            melt_assertmsg("putpairhead /1 checkpair", melt_magic_discr((melt_ptr_t)(/*_.CBIND__V17*/ meltfptr[16]))== MELTOBMAG_PAIR);
            ((meltpair_ptr_t)(/*_.CBIND__V17*/ meltfptr[16]))->hd = (melt_ptr_t) (/*_.CBIND__V12*/ meltfptr[11]);
            ;
            /*^touch*/
            meltgc_touch(/*_.CBIND__V17*/ meltfptr[16]);
            ;
            /*^putlist*/
            /*putlist*/
            melt_assertmsg("putlist checklist", melt_magic_discr((melt_ptr_t)(/*_.LIST___V18*/ meltfptr[17]))== MELTOBMAG_LIST);
            ((meltlist_ptr_t)(/*_.LIST___V18*/ meltfptr[17]))->first = (meltpair_ptr_t) (/*_.CBIND__V17*/ meltfptr[16]);
            ((meltlist_ptr_t)(/*_.LIST___V18*/ meltfptr[17]))->last = (meltpair_ptr_t) (/*_.CBIND__V17*/ meltfptr[16]);
            ;
            /*^touch*/
            meltgc_touch(/*_.LIST___V18*/ meltfptr[17]);
            ;
            /*_.NBINDS__V16*/ meltfptr[15] = /*_.LIST___V18*/ meltfptr[17];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7304:/ clear");
            /*clear*/ /*_.CBIND__V17*/ meltfptr[16] = 0 ;
            /*^clear*/
            /*clear*/ /*_.LIST___V18*/ meltfptr[17] = 0 ;
            /*^clear*/
            /*clear*/ /*_.CBIND__V17*/ meltfptr[16] = 0 ;
            /*^clear*/
            /*clear*/ /*_.LIST___V18*/ meltfptr[17] = 0 ;
        } /*end multiallocblock*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7306:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L4*/ meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7306:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L5*/ meltfnum[1] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7306:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[9];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L5*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7306;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "autobox_normal_return clocc=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CLOCC__V14*/ meltfptr[13];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " incomplete cbind=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.CBIND__V12*/ meltfptr[11];
                            /*^apply.arg*/
                            argtab[7].meltbp_cstring =  "\n.. nbinds=";
                            /*^apply.arg*/
                            argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.NBINDS__V16*/ meltfptr[15];
                            /*_.MELT_DEBUG_FUN__V20*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V19*/ meltfptr[16] = /*_.MELT_DEBUG_FUN__V20*/ meltfptr[17];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7306:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L5*/ meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V20*/ meltfptr[17] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V19*/ meltfptr[16] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7306:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L4*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V19*/ meltfptr[16] = 0 ;
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
        /*_#eqeq__L6*/ meltfnum[1] =
            ((/*_.CTYP__V3*/ meltfptr[2]) == ((/*!CTYPE_VALUE*/ meltfrout->tabval[7])));;
        MELT_LOCATION("warmelt-outobj.melt:7309:/ cond");
        /*cond*/ if (/*_#eqeq__L6*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_NOT_A__L7*/ meltfnum[0] =
                        !melt_is_instance_of((melt_ptr_t)(/*_.NEXP__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_NREP_RETURN*/ meltfrout->tabval[9])));;
                    MELT_LOCATION("warmelt-outobj.melt:7310:/ cond");
                    /*cond*/ if (/*_#IS_NOT_A__L7*/ meltfnum[0]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-outobj.melt:7311:/ quasiblock");



                                MELT_CHECK_SIGNAL();
                                ;
                                /*^quasiblock*/


                                /*^rawallocobj*/
                                /*rawallocobj*/
                                {
                                    melt_ptr_t newobj = 0;
                                    melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_NREP_RETURN*/ meltfrout->tabval[9])), (3), "CLASS_NREP_RETURN");
                                    /*_.INST__V25*/ meltfptr[24] =
                                        newobj;
                                };
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @NRET_MAIN", melt_magic_discr((melt_ptr_t)(/*_.INST__V25*/ meltfptr[24])) == MELTOBMAG_OBJECT);
                                melt_putfield_object((/*_.INST__V25*/ meltfptr[24]), (1), (/*_.CLOCC__V14*/ meltfptr[13]), "NRET_MAIN");
                                ;
                                /*^touchobj*/

                                melt_dbgtrace_written_object (/*_.INST__V25*/ meltfptr[24], "newly made instance");
                                ;
                                /*_.NRET__V24*/ meltfptr[23] = /*_.INST__V25*/ meltfptr[24];;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:7315:/ cond");
                                /*cond*/ if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CBIND__V12*/ meltfptr[11]),
                                                                  (melt_ptr_t)((/*!CLASS_LET_BINDING*/ meltfrout->tabval[10])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            /*^putslot*/
                                            /*putslot*/
                                            melt_assertmsg("checkobj putslot  _ @LETBIND_EXPR", melt_magic_discr((melt_ptr_t)(/*_.CBIND__V12*/ meltfptr[11])) == MELTOBMAG_OBJECT);
                                            melt_putfield_object((/*_.CBIND__V12*/ meltfptr[11]), (2), (/*_.NEXP__V2*/ meltfptr[1]), "LETBIND_EXPR");
                                            ;
                                            /*^touch*/
                                            meltgc_touch(/*_.CBIND__V12*/ meltfptr[11]);
                                            ;
                                            /*^touchobj*/

                                            melt_dbgtrace_written_object (/*_.CBIND__V12*/ meltfptr[11], "put-fields");
                                            ;
                                            /*epilog*/
                                        }
                                        ;
                                    } /*noelse*/
                                ;

#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:7316:/ cppif.then");
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
                                    MELT_LOCATION("warmelt-outobj.melt:7316:/ cond");
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
                                                MELT_LOCATION("warmelt-outobj.melt:7316:/ apply");
                                                /*apply*/
                                                {
                                                    union meltparam_un argtab[9];
                                                    memset(&argtab, 0, sizeof(argtab));
                                                    /*^apply.arg*/
                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[8];
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 7316;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_cstring =  "autobox_normal_return value gives nret=";
                                                    /*^apply.arg*/
                                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.NRET__V24*/ meltfptr[23];
                                                    /*^apply.arg*/
                                                    argtab[5].meltbp_cstring =  "\n.. nbinds=";
                                                    /*^apply.arg*/
                                                    argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.NBINDS__V16*/ meltfptr[15];
                                                    /*^apply.arg*/
                                                    argtab[7].meltbp_cstring =  "\n cbind=";
                                                    /*^apply.arg*/
                                                    argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.CBIND__V12*/ meltfptr[11];
                                                    /*_.MELT_DEBUG_FUN__V27*/ meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IF___V26*/ meltfptr[25] = /*_.MELT_DEBUG_FUN__V27*/ meltfptr[26];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:7316:/ clear");
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
                                        MELT_LOCATION("warmelt-outobj.melt:7316:/ locexp");
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

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:7317:/ quasiblock");


                                /*_._RETVAL___V1*/ meltfptr[0] = /*_.NRET__V24*/ meltfptr[23];;
                                MELT_LOCATION("warmelt-outobj.melt:7317:/ putxtraresult");
                                if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) (/*_.NBINDS__V16*/ meltfptr[15]);
                                ;
                                /*^finalreturn*/
                                ;
                                /*finalret*/ goto meltlabend_rout ;
                                /*_.LET___V23*/ meltfptr[22] = /*_.RETURN___V28*/ meltfptr[26];;

                                MELT_LOCATION("warmelt-outobj.melt:7311:/ clear");
                                /*clear*/ /*_.NRET__V24*/ meltfptr[23] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.RETURN___V28*/ meltfptr[26] = 0 ;
                                /*_._IFELSE___V22*/ meltfptr[16] = /*_.LET___V23*/ meltfptr[22];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:7310:/ clear");
                                /*clear*/ /*_.LET___V23*/ meltfptr[22] = 0 ;
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
                                MELT_LOCATION("warmelt-outobj.melt:7318:/ quasiblock");


                                /*_._RETVAL___V1*/ meltfptr[0] = /*_.NEXP__V2*/ meltfptr[1];;
                                MELT_LOCATION("warmelt-outobj.melt:7318:/ putxtraresult");
                                if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) (/*_.NBINDS__V16*/ meltfptr[15]);
                                ;
                                /*^finalreturn*/
                                ;
                                /*finalret*/ goto meltlabend_rout ;
                                /*_._IFELSE___V22*/ meltfptr[16] = /*_.RETURN___V29*/ meltfptr[25];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:7310:/ clear");
                                /*clear*/ /*_.RETURN___V29*/ meltfptr[25] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V21*/ meltfptr[17] = /*_._IFELSE___V22*/ meltfptr[16];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:7309:/ clear");
                    /*clear*/ /*_#IS_NOT_A__L7*/ meltfnum[0] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V22*/ meltfptr[16] = 0 ;
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
                    /*_#eqeq__L10*/ meltfnum[8] =
                        ((/*_.CTYP__V3*/ meltfptr[2]) == ((/*!CTYPE_LONG*/ meltfrout->tabval[11])));;
                    MELT_LOCATION("warmelt-outobj.melt:7320:/ cond");
                    /*cond*/ if (/*_#eqeq__L10*/ meltfnum[8]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-outobj.melt:7321:/ quasiblock");



                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:7322:/ apply");
                                /*apply*/
                                {
                                    /*_.LGSYM__V32*/ meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!CLONE_SYMBOL*/ meltfrout->tabval[4])), (melt_ptr_t)((/*!konst_12_LONGAUTOBOX*/ meltfrout->tabval[12])), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:7323:/ quasiblock");


                                /*^rawallocobj*/
                                /*rawallocobj*/
                                {
                                    melt_ptr_t newobj = 0;
                                    melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_NORMAL_LET_BINDING*/ meltfrout->tabval[6])), (4), "CLASS_NORMAL_LET_BINDING");
                                    /*_.INST__V34*/ meltfptr[16] =
                                        newobj;
                                };
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @LETBIND_LOC", melt_magic_discr((melt_ptr_t)(/*_.INST__V34*/ meltfptr[16])) == MELTOBMAG_OBJECT);
                                melt_putfield_object((/*_.INST__V34*/ meltfptr[16]), (3), ((/*nil*/NULL)), "LETBIND_LOC");
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @BINDER", melt_magic_discr((melt_ptr_t)(/*_.INST__V34*/ meltfptr[16])) == MELTOBMAG_OBJECT);
                                melt_putfield_object((/*_.INST__V34*/ meltfptr[16]), (0), (/*_.LGSYM__V32*/ meltfptr[22]), "BINDER");
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @LETBIND_TYPE", melt_magic_discr((melt_ptr_t)(/*_.INST__V34*/ meltfptr[16])) == MELTOBMAG_OBJECT);
                                melt_putfield_object((/*_.INST__V34*/ meltfptr[16]), (1), ((/*!CTYPE_LONG*/ meltfrout->tabval[11])), "LETBIND_TYPE");
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @LETBIND_EXPR", melt_magic_discr((melt_ptr_t)(/*_.INST__V34*/ meltfptr[16])) == MELTOBMAG_OBJECT);
                                melt_putfield_object((/*_.INST__V34*/ meltfptr[16]), (2), (/*_.NEXP__V2*/ meltfptr[1]), "LETBIND_EXPR");
                                ;
                                /*^touchobj*/

                                melt_dbgtrace_written_object (/*_.INST__V34*/ meltfptr[16], "newly made instance");
                                ;
                                /*_.LGBIND__V33*/ meltfptr[25] = /*_.INST__V34*/ meltfptr[16];;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:7329:/ quasiblock");


                                /*^rawallocobj*/
                                /*rawallocobj*/
                                {
                                    melt_ptr_t newobj = 0;
                                    melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_NREP_LOCSYMOCC*/ meltfrout->tabval[8])), (4), "CLASS_NREP_LOCSYMOCC");
                                    /*_.INST__V36*/ meltfptr[35] =
                                        newobj;
                                };
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @NREP_LOC", melt_magic_discr((melt_ptr_t)(/*_.INST__V36*/ meltfptr[35])) == MELTOBMAG_OBJECT);
                                melt_putfield_object((/*_.INST__V36*/ meltfptr[35]), (0), ((/*nil*/NULL)), "NREP_LOC");
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @NOCC_CTYP", melt_magic_discr((melt_ptr_t)(/*_.INST__V36*/ meltfptr[35])) == MELTOBMAG_OBJECT);
                                melt_putfield_object((/*_.INST__V36*/ meltfptr[35]), (2), ((/*!CTYPE_LONG*/ meltfrout->tabval[11])), "NOCC_CTYP");
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @NOCC_SYMB", melt_magic_discr((melt_ptr_t)(/*_.INST__V36*/ meltfptr[35])) == MELTOBMAG_OBJECT);
                                melt_putfield_object((/*_.INST__V36*/ meltfptr[35]), (1), (/*_.LGSYM__V32*/ meltfptr[22]), "NOCC_SYMB");
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @NOCC_BIND", melt_magic_discr((melt_ptr_t)(/*_.INST__V36*/ meltfptr[35])) == MELTOBMAG_OBJECT);
                                melt_putfield_object((/*_.INST__V36*/ meltfptr[35]), (3), (/*_.LGBIND__V33*/ meltfptr[25]), "NOCC_BIND");
                                ;
                                /*^touchobj*/

                                melt_dbgtrace_written_object (/*_.INST__V36*/ meltfptr[35], "newly made instance");
                                ;
                                /*_.LGLOCC__V35*/ meltfptr[34] = /*_.INST__V36*/ meltfptr[35];;

                                MELT_CHECK_SIGNAL();
                                ;
                                /*^compute*/
                                /*_.MAKE_STRINGCONST__V37*/ meltfptr[36] =
                                    (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[14])), ( "/*autobox long*/ meltgc_new_int((meltobject_ptr_t)MELT_PREDEF(DISCR_CONSTANT_\
INTEGER), ")));;
                                /*^compute*/
                                /*_.MAKE_STRINGCONST__V38*/ meltfptr[37] =
                                    (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[14])), ( ")")));;
                                MELT_LOCATION("warmelt-outobj.melt:7338:/ blockmultialloc");
                                /*multiallocblock*/
                                {
                                    struct meltletrec_2_st
                                    {
                                        struct MELT_MULTIPLE_STRUCT(3) rtup_0__TUPLREC__x8;
                                        long meltletrec_2_endgap;
                                    } *meltletrec_2_ptr = 0;
                                    meltletrec_2_ptr = (struct meltletrec_2_st *) meltgc_allocate (sizeof (struct meltletrec_2_st), 0);
                                    /*^blockmultialloc.initfill*/
                                    /*inimult rtup_0__TUPLREC__x8*/
                                    /*_.TUPLREC___V40*/ meltfptr[39] = (melt_ptr_t) &meltletrec_2_ptr->rtup_0__TUPLREC__x8;
                                    meltletrec_2_ptr->rtup_0__TUPLREC__x8.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_MULTIPLE))));
                                    meltletrec_2_ptr->rtup_0__TUPLREC__x8.nbval = 3;


                                    /*^putuple*/
                                    /*putupl#17*/
                                    melt_assertmsg("putupl [:7338] #17 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V40*/ meltfptr[39]))== MELTOBMAG_MULTIPLE);
                                    melt_assertmsg("putupl [:7338] #17 checkoff", (0>=0 && 0< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V40*/ meltfptr[39]))));
                                    ((meltmultiple_ptr_t)(/*_.TUPLREC___V40*/ meltfptr[39]))->tabval[0] = (melt_ptr_t)(/*_.MAKE_STRINGCONST__V37*/ meltfptr[36]);
                                    ;
                                    /*^putuple*/
                                    /*putupl#18*/
                                    melt_assertmsg("putupl [:7338] #18 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V40*/ meltfptr[39]))== MELTOBMAG_MULTIPLE);
                                    melt_assertmsg("putupl [:7338] #18 checkoff", (1>=0 && 1< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V40*/ meltfptr[39]))));
                                    ((meltmultiple_ptr_t)(/*_.TUPLREC___V40*/ meltfptr[39]))->tabval[1] = (melt_ptr_t)(/*_.LGLOCC__V35*/ meltfptr[34]);
                                    ;
                                    /*^putuple*/
                                    /*putupl#19*/
                                    melt_assertmsg("putupl [:7338] #19 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V40*/ meltfptr[39]))== MELTOBMAG_MULTIPLE);
                                    melt_assertmsg("putupl [:7338] #19 checkoff", (2>=0 && 2< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V40*/ meltfptr[39]))));
                                    ((meltmultiple_ptr_t)(/*_.TUPLREC___V40*/ meltfptr[39]))->tabval[2] = (melt_ptr_t)(/*_.MAKE_STRINGCONST__V38*/ meltfptr[37]);
                                    ;
                                    /*^touch*/
                                    meltgc_touch(/*_.TUPLREC___V40*/ meltfptr[39]);
                                    ;
                                    /*_.TUPLE___V39*/ meltfptr[38] = /*_.TUPLREC___V40*/ meltfptr[39];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:7338:/ clear");
                                    /*clear*/ /*_.TUPLREC___V40*/ meltfptr[39] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.TUPLREC___V40*/ meltfptr[39] = 0 ;
                                } /*end multiallocblock*/
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:7334:/ quasiblock");


                                /*^rawallocobj*/
                                /*rawallocobj*/
                                {
                                    melt_ptr_t newobj = 0;
                                    melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_NREP_CHUNK*/ meltfrout->tabval[13])), (4), "CLASS_NREP_CHUNK");
                                    /*_.INST__V42*/ meltfptr[41] =
                                        newobj;
                                };
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @NREP_LOC", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
                                melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (0), ((/*nil*/NULL)), "NREP_LOC");
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @NCHUNK_EXPANSION", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
                                melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (2), (/*_.TUPLE___V39*/ meltfptr[38]), "NCHUNK_EXPANSION");
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @NCHUNK_OPER", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
                                melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (3), ((/*!konst_15_AUTOBOXLONG*/ meltfrout->tabval[15])), "NCHUNK_OPER");
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @NEXPR_CTYP", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[41])) == MELTOBMAG_OBJECT);
                                melt_putfield_object((/*_.INST__V42*/ meltfptr[41]), (1), ((/*!CTYPE_VALUE*/ meltfrout->tabval[7])), "NEXPR_CTYP");
                                ;
                                /*^touchobj*/

                                melt_dbgtrace_written_object (/*_.INST__V42*/ meltfptr[41], "newly made instance");
                                ;
                                /*_.NCHK__V41*/ meltfptr[39] = /*_.INST__V42*/ meltfptr[41];;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:7348:/ quasiblock");


                                /*^rawallocobj*/
                                /*rawallocobj*/
                                {
                                    melt_ptr_t newobj = 0;
                                    melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_NREP_RETURN*/ meltfrout->tabval[9])), (3), "CLASS_NREP_RETURN");
                                    /*_.INST__V44*/ meltfptr[43] =
                                        newobj;
                                };
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @NRET_MAIN", melt_magic_discr((melt_ptr_t)(/*_.INST__V44*/ meltfptr[43])) == MELTOBMAG_OBJECT);
                                melt_putfield_object((/*_.INST__V44*/ meltfptr[43]), (1), (/*_.CLOCC__V14*/ meltfptr[13]), "NRET_MAIN");
                                ;
                                /*^touchobj*/

                                melt_dbgtrace_written_object (/*_.INST__V44*/ meltfptr[43], "newly made instance");
                                ;
                                /*_.NRET__V43*/ meltfptr[42] = /*_.INST__V44*/ meltfptr[43];;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:7352:/ cond");
                                /*cond*/ if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CBIND__V12*/ meltfptr[11]),
                                                                  (melt_ptr_t)((/*!CLASS_LET_BINDING*/ meltfrout->tabval[10])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            /*^putslot*/
                                            /*putslot*/
                                            melt_assertmsg("checkobj putslot  _ @LETBIND_EXPR", melt_magic_discr((melt_ptr_t)(/*_.CBIND__V12*/ meltfptr[11])) == MELTOBMAG_OBJECT);
                                            melt_putfield_object((/*_.CBIND__V12*/ meltfptr[11]), (2), (/*_.NCHK__V41*/ meltfptr[39]), "LETBIND_EXPR");
                                            ;
                                            /*^touch*/
                                            meltgc_touch(/*_.CBIND__V12*/ meltfptr[11]);
                                            ;
                                            /*^touchobj*/

                                            melt_dbgtrace_written_object (/*_.CBIND__V12*/ meltfptr[11], "put-fields");
                                            ;
                                            /*epilog*/
                                        }
                                        ;
                                    } /*noelse*/
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:7353:/ locexp");
                                    meltgc_prepend_list((melt_ptr_t)(/*_.NBINDS__V16*/ meltfptr[15]), (melt_ptr_t)(/*_.LGBIND__V33*/ meltfptr[25]));
                                }
                                ;

#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:7354:/ cppif.then");
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
                                    /*_#MELT_NEED_DBG__L11*/ meltfnum[7] =
                                        /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                        (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                        0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                        ;;
                                    MELT_LOCATION("warmelt-outobj.melt:7354:/ cond");
                                    /*cond*/ if (/*_#MELT_NEED_DBG__L11*/ meltfnum[7]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*^block*/
                                            /*anyblock*/
                                            {

                                                /*_#MELT_CALLCOUNT__L12*/ meltfnum[0] =
                                                    /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                                    meltcallcount  /* melt_callcount debugging */
#else
                                                    0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                                    ;;

                                                MELT_CHECK_SIGNAL();
                                                ;
                                                MELT_LOCATION("warmelt-outobj.melt:7354:/ apply");
                                                /*apply*/
                                                {
                                                    union meltparam_un argtab[7];
                                                    memset(&argtab, 0, sizeof(argtab));
                                                    /*^apply.arg*/
                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[0];
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 7354;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_cstring =  "autobox_normal_return long return nret=";
                                                    /*^apply.arg*/
                                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.NRET__V43*/ meltfptr[42];
                                                    /*^apply.arg*/
                                                    argtab[5].meltbp_cstring =  "\n.. nbinds=";
                                                    /*^apply.arg*/
                                                    argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.NBINDS__V16*/ meltfptr[15];
                                                    /*_.MELT_DEBUG_FUN__V46*/ meltfptr[45] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IF___V45*/ meltfptr[44] = /*_.MELT_DEBUG_FUN__V46*/ meltfptr[45];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:7354:/ clear");
                                                /*clear*/ /*_#MELT_CALLCOUNT__L12*/ meltfnum[0] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_.MELT_DEBUG_FUN__V46*/ meltfptr[45] = 0 ;
                                            }
                                            ;
                                        }
                                    else    /*^cond.else*/
                                        {

                                            /*_._IF___V45*/ meltfptr[44] =  /*reallynil*/ NULL ;;
                                        }
                                    ;

                                    {
                                        MELT_LOCATION("warmelt-outobj.melt:7354:/ locexp");
                                        /*void*/(void)0;
                                    }
                                    ;
                                    /*^quasiblock*/


                                    /*epilog*/

                                    /*^clear*/
                                    /*clear*/ /*_#MELT_NEED_DBG__L11*/ meltfnum[7] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IF___V45*/ meltfptr[44] = 0 ;
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
                                MELT_LOCATION("warmelt-outobj.melt:7355:/ quasiblock");


                                /*_._RETVAL___V1*/ meltfptr[0] = /*_.NRET__V43*/ meltfptr[42];;
                                MELT_LOCATION("warmelt-outobj.melt:7355:/ putxtraresult");
                                if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) (/*_.NBINDS__V16*/ meltfptr[15]);
                                ;
                                /*^finalreturn*/
                                ;
                                /*finalret*/ goto meltlabend_rout ;
                                /*_.LET___V31*/ meltfptr[26] = /*_.RETURN___V47*/ meltfptr[45];;

                                MELT_LOCATION("warmelt-outobj.melt:7321:/ clear");
                                /*clear*/ /*_.LGSYM__V32*/ meltfptr[22] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.LGBIND__V33*/ meltfptr[25] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.LGLOCC__V35*/ meltfptr[34] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.MAKE_STRINGCONST__V37*/ meltfptr[36] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.MAKE_STRINGCONST__V38*/ meltfptr[37] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.TUPLE___V39*/ meltfptr[38] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.NCHK__V41*/ meltfptr[39] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.NRET__V43*/ meltfptr[42] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.RETURN___V47*/ meltfptr[45] = 0 ;
                                /*_._IFELSE___V30*/ meltfptr[23] = /*_.LET___V31*/ meltfptr[26];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:7320:/ clear");
                                /*clear*/ /*_.LET___V31*/ meltfptr[26] = 0 ;
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
                                /*_#eqeq__L13*/ meltfnum[0] =
                                    ((/*_.CTYP__V3*/ meltfptr[2]) == ((/*!CTYPE_VOID*/ meltfrout->tabval[16])));;
                                MELT_LOCATION("warmelt-outobj.melt:7358:/ cond");
                                /*cond*/ if (/*_#eqeq__L13*/ meltfnum[0]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-outobj.melt:7359:/ quasiblock");



                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*^quasiblock*/


                                            /*^rawallocobj*/
                                            /*rawallocobj*/
                                            {
                                                melt_ptr_t newobj = 0;
                                                melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_NREP_NIL*/ meltfrout->tabval[17])), (1), "CLASS_NREP_NIL");
                                                /*_.INST__V51*/ meltfptr[34] =
                                                    newobj;
                                            };
                                            ;
                                            /*^touchobj*/

                                            melt_dbgtrace_written_object (/*_.INST__V51*/ meltfptr[34], "newly made instance");
                                            ;
                                            /*_.NILR__V50*/ meltfptr[25] = /*_.INST__V51*/ meltfptr[34];;

                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-outobj.melt:7361:/ quasiblock");


                                            /*^rawallocobj*/
                                            /*rawallocobj*/
                                            {
                                                melt_ptr_t newobj = 0;
                                                melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_NREP_RETURN*/ meltfrout->tabval[9])), (3), "CLASS_NREP_RETURN");
                                                /*_.INST__V53*/ meltfptr[37] =
                                                    newobj;
                                            };
                                            ;
                                            /*^putslot*/
                                            /*putslot*/
                                            melt_assertmsg("checkobj putslot  _ @NRET_MAIN", melt_magic_discr((melt_ptr_t)(/*_.INST__V53*/ meltfptr[37])) == MELTOBMAG_OBJECT);
                                            melt_putfield_object((/*_.INST__V53*/ meltfptr[37]), (1), (/*_.CLOCC__V14*/ meltfptr[13]), "NRET_MAIN");
                                            ;
                                            /*^touchobj*/

                                            melt_dbgtrace_written_object (/*_.INST__V53*/ meltfptr[37], "newly made instance");
                                            ;
                                            /*_.NRET__V52*/ meltfptr[36] = /*_.INST__V53*/ meltfptr[37];;

                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-outobj.melt:7364:/ cond");
                                            /*cond*/ if (
                                                /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CBIND__V12*/ meltfptr[11]),
                                                                              (melt_ptr_t)((/*!CLASS_LET_BINDING*/ meltfrout->tabval[10])))
                                            ) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        /*^putslot*/
                                                        /*putslot*/
                                                        melt_assertmsg("checkobj putslot  _ @LETBIND_EXPR", melt_magic_discr((melt_ptr_t)(/*_.CBIND__V12*/ meltfptr[11])) == MELTOBMAG_OBJECT);
                                                        melt_putfield_object((/*_.CBIND__V12*/ meltfptr[11]), (2), (/*_.NILR__V50*/ meltfptr[25]), "LETBIND_EXPR");
                                                        ;
                                                        /*^touch*/
                                                        meltgc_touch(/*_.CBIND__V12*/ meltfptr[11]);
                                                        ;
                                                        /*^touchobj*/

                                                        melt_dbgtrace_written_object (/*_.CBIND__V12*/ meltfptr[11], "put-fields");
                                                        ;
                                                        /*epilog*/
                                                    }
                                                    ;
                                                } /*noelse*/
                                            ;

#if MELT_HAVE_DEBUG
                                            MELT_LOCATION("warmelt-outobj.melt:7365:/ cppif.then");
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
                                                /*_#MELT_NEED_DBG__L14*/ meltfnum[7] =
                                                    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                                    (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                                    0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                                    ;;
                                                MELT_LOCATION("warmelt-outobj.melt:7365:/ cond");
                                                /*cond*/ if (/*_#MELT_NEED_DBG__L14*/ meltfnum[7]) /*then*/
                                                    {
                                                        /*^cond.then*/
                                                        /*^block*/
                                                        /*anyblock*/
                                                        {

                                                            /*_#MELT_CALLCOUNT__L15*/ meltfnum[14] =
                                                                /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                                                meltcallcount  /* melt_callcount debugging */
#else
                                                                0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                                                ;;

                                                            MELT_CHECK_SIGNAL();
                                                            ;
                                                            MELT_LOCATION("warmelt-outobj.melt:7365:/ apply");
                                                            /*apply*/
                                                            {
                                                                union meltparam_un argtab[7];
                                                                memset(&argtab, 0, sizeof(argtab));
                                                                /*^apply.arg*/
                                                                argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L15*/ meltfnum[14];
                                                                /*^apply.arg*/
                                                                argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                                /*^apply.arg*/
                                                                argtab[2].meltbp_long = 7365;
                                                                /*^apply.arg*/
                                                                argtab[3].meltbp_cstring =  "autobox_normal_return void return nret=";
                                                                /*^apply.arg*/
                                                                argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.NRET__V52*/ meltfptr[36];
                                                                /*^apply.arg*/
                                                                argtab[5].meltbp_cstring =  " nbinds=";
                                                                /*^apply.arg*/
                                                                argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.NBINDS__V16*/ meltfptr[15];
                                                                /*_.MELT_DEBUG_FUN__V55*/ meltfptr[39] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                            }
                                                            ;
                                                            /*_._IF___V54*/ meltfptr[38] = /*_.MELT_DEBUG_FUN__V55*/ meltfptr[39];;
                                                            /*epilog*/

                                                            MELT_LOCATION("warmelt-outobj.melt:7365:/ clear");
                                                            /*clear*/ /*_#MELT_CALLCOUNT__L15*/ meltfnum[14] = 0 ;
                                                            /*^clear*/
                                                            /*clear*/ /*_.MELT_DEBUG_FUN__V55*/ meltfptr[39] = 0 ;
                                                        }
                                                        ;
                                                    }
                                                else    /*^cond.else*/
                                                    {

                                                        /*_._IF___V54*/ meltfptr[38] =  /*reallynil*/ NULL ;;
                                                    }
                                                ;

                                                {
                                                    MELT_LOCATION("warmelt-outobj.melt:7365:/ locexp");
                                                    /*void*/(void)0;
                                                }
                                                ;
                                                /*^quasiblock*/


                                                /*epilog*/

                                                /*^clear*/
                                                /*clear*/ /*_#MELT_NEED_DBG__L14*/ meltfnum[7] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_._IF___V54*/ meltfptr[38] = 0 ;
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
                                            MELT_LOCATION("warmelt-outobj.melt:7366:/ quasiblock");


                                            /*_._RETVAL___V1*/ meltfptr[0] = /*_.NRET__V52*/ meltfptr[36];;
                                            MELT_LOCATION("warmelt-outobj.melt:7366:/ putxtraresult");
                                            if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                            if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                            if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) (/*_.NBINDS__V16*/ meltfptr[15]);
                                            ;
                                            /*^finalreturn*/
                                            ;
                                            /*finalret*/ goto meltlabend_rout ;
                                            /*_.LET___V49*/ meltfptr[22] = /*_.RETURN___V56*/ meltfptr[42];;

                                            MELT_LOCATION("warmelt-outobj.melt:7359:/ clear");
                                            /*clear*/ /*_.NILR__V50*/ meltfptr[25] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.NRET__V52*/ meltfptr[36] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.RETURN___V56*/ meltfptr[42] = 0 ;
                                            /*_._IFELSE___V48*/ meltfptr[44] = /*_.LET___V49*/ meltfptr[22];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:7358:/ clear");
                                            /*clear*/ /*_.LET___V49*/ meltfptr[22] = 0 ;
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
                                            /*_#eqeq__L16*/ meltfnum[14] =
                                                ((/*_.CTYP__V3*/ meltfptr[2]) == ((/*!CTYPE_CSTRING*/ meltfrout->tabval[18])));;
                                            MELT_LOCATION("warmelt-outobj.melt:7369:/ cond");
                                            /*cond*/ if (/*_#eqeq__L16*/ meltfnum[14]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        MELT_LOCATION("warmelt-outobj.melt:7370:/ quasiblock");



                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:7371:/ apply");
                                                        /*apply*/
                                                        {
                                                            /*_.STSYM__V59*/ meltfptr[39] =  melt_apply ((meltclosure_ptr_t)((/*!CLONE_SYMBOL*/ meltfrout->tabval[4])), (melt_ptr_t)((/*!konst_19_STRINGAUTOBOX*/ meltfrout->tabval[19])), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                                                        }
                                                        ;

                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:7372:/ quasiblock");


                                                        /*^rawallocobj*/
                                                        /*rawallocobj*/
                                                        {
                                                            melt_ptr_t newobj = 0;
                                                            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_NORMAL_LET_BINDING*/ meltfrout->tabval[6])), (4), "CLASS_NORMAL_LET_BINDING");
                                                            /*_.INST__V61*/ meltfptr[25] =
                                                                newobj;
                                                        };
                                                        ;
                                                        /*^putslot*/
                                                        /*putslot*/
                                                        melt_assertmsg("checkobj putslot  _ @LETBIND_LOC", melt_magic_discr((melt_ptr_t)(/*_.INST__V61*/ meltfptr[25])) == MELTOBMAG_OBJECT);
                                                        melt_putfield_object((/*_.INST__V61*/ meltfptr[25]), (3), ((/*nil*/NULL)), "LETBIND_LOC");
                                                        ;
                                                        /*^putslot*/
                                                        /*putslot*/
                                                        melt_assertmsg("checkobj putslot  _ @BINDER", melt_magic_discr((melt_ptr_t)(/*_.INST__V61*/ meltfptr[25])) == MELTOBMAG_OBJECT);
                                                        melt_putfield_object((/*_.INST__V61*/ meltfptr[25]), (0), (/*_.STSYM__V59*/ meltfptr[39]), "BINDER");
                                                        ;
                                                        /*^putslot*/
                                                        /*putslot*/
                                                        melt_assertmsg("checkobj putslot  _ @LETBIND_TYPE", melt_magic_discr((melt_ptr_t)(/*_.INST__V61*/ meltfptr[25])) == MELTOBMAG_OBJECT);
                                                        melt_putfield_object((/*_.INST__V61*/ meltfptr[25]), (1), ((/*!CTYPE_CSTRING*/ meltfrout->tabval[18])), "LETBIND_TYPE");
                                                        ;
                                                        /*^putslot*/
                                                        /*putslot*/
                                                        melt_assertmsg("checkobj putslot  _ @LETBIND_EXPR", melt_magic_discr((melt_ptr_t)(/*_.INST__V61*/ meltfptr[25])) == MELTOBMAG_OBJECT);
                                                        melt_putfield_object((/*_.INST__V61*/ meltfptr[25]), (2), (/*_.NEXP__V2*/ meltfptr[1]), "LETBIND_EXPR");
                                                        ;
                                                        /*^touchobj*/

                                                        melt_dbgtrace_written_object (/*_.INST__V61*/ meltfptr[25], "newly made instance");
                                                        ;
                                                        /*_.STBIND__V60*/ meltfptr[38] = /*_.INST__V61*/ meltfptr[25];;

                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:7378:/ quasiblock");


                                                        /*^rawallocobj*/
                                                        /*rawallocobj*/
                                                        {
                                                            melt_ptr_t newobj = 0;
                                                            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_NREP_LOCSYMOCC*/ meltfrout->tabval[8])), (4), "CLASS_NREP_LOCSYMOCC");
                                                            /*_.INST__V63*/ meltfptr[42] =
                                                                newobj;
                                                        };
                                                        ;
                                                        /*^putslot*/
                                                        /*putslot*/
                                                        melt_assertmsg("checkobj putslot  _ @NREP_LOC", melt_magic_discr((melt_ptr_t)(/*_.INST__V63*/ meltfptr[42])) == MELTOBMAG_OBJECT);
                                                        melt_putfield_object((/*_.INST__V63*/ meltfptr[42]), (0), ((/*nil*/NULL)), "NREP_LOC");
                                                        ;
                                                        /*^putslot*/
                                                        /*putslot*/
                                                        melt_assertmsg("checkobj putslot  _ @NOCC_CTYP", melt_magic_discr((melt_ptr_t)(/*_.INST__V63*/ meltfptr[42])) == MELTOBMAG_OBJECT);
                                                        melt_putfield_object((/*_.INST__V63*/ meltfptr[42]), (2), ((/*!CTYPE_CSTRING*/ meltfrout->tabval[18])), "NOCC_CTYP");
                                                        ;
                                                        /*^putslot*/
                                                        /*putslot*/
                                                        melt_assertmsg("checkobj putslot  _ @NOCC_SYMB", melt_magic_discr((melt_ptr_t)(/*_.INST__V63*/ meltfptr[42])) == MELTOBMAG_OBJECT);
                                                        melt_putfield_object((/*_.INST__V63*/ meltfptr[42]), (1), (/*_.STSYM__V59*/ meltfptr[39]), "NOCC_SYMB");
                                                        ;
                                                        /*^putslot*/
                                                        /*putslot*/
                                                        melt_assertmsg("checkobj putslot  _ @NOCC_BIND", melt_magic_discr((melt_ptr_t)(/*_.INST__V63*/ meltfptr[42])) == MELTOBMAG_OBJECT);
                                                        melt_putfield_object((/*_.INST__V63*/ meltfptr[42]), (3), (/*_.STBIND__V60*/ meltfptr[38]), "NOCC_BIND");
                                                        ;
                                                        /*^touchobj*/

                                                        melt_dbgtrace_written_object (/*_.INST__V63*/ meltfptr[42], "newly made instance");
                                                        ;
                                                        /*_.STLOCC__V62*/ meltfptr[36] = /*_.INST__V63*/ meltfptr[42];;

                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        /*^compute*/
                                                        /*_.MAKE_STRINGCONST__V64*/ meltfptr[22] =
                                                            (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[14])), ( "/*autobox cstring*/ meltgc_new_stringdup((meltobject_ptr_t)MELT_PREDEF\
(DISCR_STRING), ")));;
                                                        /*^compute*/
                                                        /*_.MAKE_STRINGCONST__V65*/ meltfptr[64] =
                                                            (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[14])), ( ")")));;
                                                        MELT_LOCATION("warmelt-outobj.melt:7387:/ blockmultialloc");
                                                        /*multiallocblock*/
                                                        {
                                                            struct meltletrec_3_st
                                                            {
                                                                struct MELT_MULTIPLE_STRUCT(3) rtup_0__TUPLREC__x9;
                                                                long meltletrec_3_endgap;
                                                            } *meltletrec_3_ptr = 0;
                                                            meltletrec_3_ptr = (struct meltletrec_3_st *) meltgc_allocate (sizeof (struct meltletrec_3_st), 0);
                                                            /*^blockmultialloc.initfill*/
                                                            /*inimult rtup_0__TUPLREC__x9*/
                                                            /*_.TUPLREC___V67*/ meltfptr[66] = (melt_ptr_t) &meltletrec_3_ptr->rtup_0__TUPLREC__x9;
                                                            meltletrec_3_ptr->rtup_0__TUPLREC__x9.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_MULTIPLE))));
                                                            meltletrec_3_ptr->rtup_0__TUPLREC__x9.nbval = 3;


                                                            /*^putuple*/
                                                            /*putupl#20*/
                                                            melt_assertmsg("putupl [:7387] #20 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V67*/ meltfptr[66]))== MELTOBMAG_MULTIPLE);
                                                            melt_assertmsg("putupl [:7387] #20 checkoff", (0>=0 && 0< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V67*/ meltfptr[66]))));
                                                            ((meltmultiple_ptr_t)(/*_.TUPLREC___V67*/ meltfptr[66]))->tabval[0] = (melt_ptr_t)(/*_.MAKE_STRINGCONST__V64*/ meltfptr[22]);
                                                            ;
                                                            /*^putuple*/
                                                            /*putupl#21*/
                                                            melt_assertmsg("putupl [:7387] #21 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V67*/ meltfptr[66]))== MELTOBMAG_MULTIPLE);
                                                            melt_assertmsg("putupl [:7387] #21 checkoff", (1>=0 && 1< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V67*/ meltfptr[66]))));
                                                            ((meltmultiple_ptr_t)(/*_.TUPLREC___V67*/ meltfptr[66]))->tabval[1] = (melt_ptr_t)(/*_.STLOCC__V62*/ meltfptr[36]);
                                                            ;
                                                            /*^putuple*/
                                                            /*putupl#22*/
                                                            melt_assertmsg("putupl [:7387] #22 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V67*/ meltfptr[66]))== MELTOBMAG_MULTIPLE);
                                                            melt_assertmsg("putupl [:7387] #22 checkoff", (2>=0 && 2< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V67*/ meltfptr[66]))));
                                                            ((meltmultiple_ptr_t)(/*_.TUPLREC___V67*/ meltfptr[66]))->tabval[2] = (melt_ptr_t)(/*_.MAKE_STRINGCONST__V65*/ meltfptr[64]);
                                                            ;
                                                            /*^touch*/
                                                            meltgc_touch(/*_.TUPLREC___V67*/ meltfptr[66]);
                                                            ;
                                                            /*_.TUPLE___V66*/ meltfptr[65] = /*_.TUPLREC___V67*/ meltfptr[66];;
                                                            /*epilog*/

                                                            MELT_LOCATION("warmelt-outobj.melt:7387:/ clear");
                                                            /*clear*/ /*_.TUPLREC___V67*/ meltfptr[66] = 0 ;
                                                            /*^clear*/
                                                            /*clear*/ /*_.TUPLREC___V67*/ meltfptr[66] = 0 ;
                                                        } /*end multiallocblock*/
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:7383:/ quasiblock");


                                                        /*^rawallocobj*/
                                                        /*rawallocobj*/
                                                        {
                                                            melt_ptr_t newobj = 0;
                                                            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_NREP_CHUNK*/ meltfrout->tabval[13])), (4), "CLASS_NREP_CHUNK");
                                                            /*_.INST__V69*/ meltfptr[68] =
                                                                newobj;
                                                        };
                                                        ;
                                                        /*^putslot*/
                                                        /*putslot*/
                                                        melt_assertmsg("checkobj putslot  _ @NREP_LOC", melt_magic_discr((melt_ptr_t)(/*_.INST__V69*/ meltfptr[68])) == MELTOBMAG_OBJECT);
                                                        melt_putfield_object((/*_.INST__V69*/ meltfptr[68]), (0), ((/*nil*/NULL)), "NREP_LOC");
                                                        ;
                                                        /*^putslot*/
                                                        /*putslot*/
                                                        melt_assertmsg("checkobj putslot  _ @NCHUNK_EXPANSION", melt_magic_discr((melt_ptr_t)(/*_.INST__V69*/ meltfptr[68])) == MELTOBMAG_OBJECT);
                                                        melt_putfield_object((/*_.INST__V69*/ meltfptr[68]), (2), (/*_.TUPLE___V66*/ meltfptr[65]), "NCHUNK_EXPANSION");
                                                        ;
                                                        /*^putslot*/
                                                        /*putslot*/
                                                        melt_assertmsg("checkobj putslot  _ @NCHUNK_OPER", melt_magic_discr((melt_ptr_t)(/*_.INST__V69*/ meltfptr[68])) == MELTOBMAG_OBJECT);
                                                        melt_putfield_object((/*_.INST__V69*/ meltfptr[68]), (3), ((/*!konst_20_AUTOBOXSTRING*/ meltfrout->tabval[20])), "NCHUNK_OPER");
                                                        ;
                                                        /*^putslot*/
                                                        /*putslot*/
                                                        melt_assertmsg("checkobj putslot  _ @NEXPR_CTYP", melt_magic_discr((melt_ptr_t)(/*_.INST__V69*/ meltfptr[68])) == MELTOBMAG_OBJECT);
                                                        melt_putfield_object((/*_.INST__V69*/ meltfptr[68]), (1), ((/*!CTYPE_VALUE*/ meltfrout->tabval[7])), "NEXPR_CTYP");
                                                        ;
                                                        /*^touchobj*/

                                                        melt_dbgtrace_written_object (/*_.INST__V69*/ meltfptr[68], "newly made instance");
                                                        ;
                                                        /*_.NCHK__V68*/ meltfptr[66] = /*_.INST__V69*/ meltfptr[68];;

                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:7397:/ quasiblock");


                                                        /*^rawallocobj*/
                                                        /*rawallocobj*/
                                                        {
                                                            melt_ptr_t newobj = 0;
                                                            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_NREP_RETURN*/ meltfrout->tabval[9])), (3), "CLASS_NREP_RETURN");
                                                            /*_.INST__V71*/ meltfptr[70] =
                                                                newobj;
                                                        };
                                                        ;
                                                        /*^putslot*/
                                                        /*putslot*/
                                                        melt_assertmsg("checkobj putslot  _ @NRET_MAIN", melt_magic_discr((melt_ptr_t)(/*_.INST__V71*/ meltfptr[70])) == MELTOBMAG_OBJECT);
                                                        melt_putfield_object((/*_.INST__V71*/ meltfptr[70]), (1), (/*_.CLOCC__V14*/ meltfptr[13]), "NRET_MAIN");
                                                        ;
                                                        /*^touchobj*/

                                                        melt_dbgtrace_written_object (/*_.INST__V71*/ meltfptr[70], "newly made instance");
                                                        ;
                                                        /*_.NRET__V70*/ meltfptr[69] = /*_.INST__V71*/ meltfptr[70];;

                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:7401:/ cond");
                                                        /*cond*/ if (
                                                            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CBIND__V12*/ meltfptr[11]),
                                                                                          (melt_ptr_t)((/*!CLASS_LET_BINDING*/ meltfrout->tabval[10])))
                                                        ) /*then*/
                                                            {
                                                                /*^cond.then*/
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {

                                                                    /*^putslot*/
                                                                    /*putslot*/
                                                                    melt_assertmsg("checkobj putslot  _ @LETBIND_EXPR", melt_magic_discr((melt_ptr_t)(/*_.CBIND__V12*/ meltfptr[11])) == MELTOBMAG_OBJECT);
                                                                    melt_putfield_object((/*_.CBIND__V12*/ meltfptr[11]), (2), (/*_.NCHK__V68*/ meltfptr[66]), "LETBIND_EXPR");
                                                                    ;
                                                                    /*^touch*/
                                                                    meltgc_touch(/*_.CBIND__V12*/ meltfptr[11]);
                                                                    ;
                                                                    /*^touchobj*/

                                                                    melt_dbgtrace_written_object (/*_.CBIND__V12*/ meltfptr[11], "put-fields");
                                                                    ;
                                                                    /*epilog*/
                                                                }
                                                                ;
                                                            } /*noelse*/
                                                        ;

                                                        {
                                                            MELT_LOCATION("warmelt-outobj.melt:7402:/ locexp");
                                                            meltgc_prepend_list((melt_ptr_t)(/*_.NBINDS__V16*/ meltfptr[15]), (melt_ptr_t)(/*_.STBIND__V60*/ meltfptr[38]));
                                                        }
                                                        ;

#if MELT_HAVE_DEBUG
                                                        MELT_LOCATION("warmelt-outobj.melt:7403:/ cppif.then");
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
                                                            /*_#MELT_NEED_DBG__L17*/ meltfnum[7] =
                                                                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                                                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                                                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                                                ;;
                                                            MELT_LOCATION("warmelt-outobj.melt:7403:/ cond");
                                                            /*cond*/ if (/*_#MELT_NEED_DBG__L17*/ meltfnum[7]) /*then*/
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
                                                                        MELT_LOCATION("warmelt-outobj.melt:7403:/ apply");
                                                                        /*apply*/
                                                                        {
                                                                            union meltparam_un argtab[7];
                                                                            memset(&argtab, 0, sizeof(argtab));
                                                                            /*^apply.arg*/
                                                                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L18*/ meltfnum[17];
                                                                            /*^apply.arg*/
                                                                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                                            /*^apply.arg*/
                                                                            argtab[2].meltbp_long = 7403;
                                                                            /*^apply.arg*/
                                                                            argtab[3].meltbp_cstring =  "autobox_normal_return cstring return nret=";
                                                                            /*^apply.arg*/
                                                                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.NRET__V70*/ meltfptr[69];
                                                                            /*^apply.arg*/
                                                                            argtab[5].meltbp_cstring =  " nbinds=";
                                                                            /*^apply.arg*/
                                                                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.NBINDS__V16*/ meltfptr[15];
                                                                            /*_.MELT_DEBUG_FUN__V73*/ meltfptr[72] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                        }
                                                                        ;
                                                                        /*_._IF___V72*/ meltfptr[71] = /*_.MELT_DEBUG_FUN__V73*/ meltfptr[72];;
                                                                        /*epilog*/

                                                                        MELT_LOCATION("warmelt-outobj.melt:7403:/ clear");
                                                                        /*clear*/ /*_#MELT_CALLCOUNT__L18*/ meltfnum[17] = 0 ;
                                                                        /*^clear*/
                                                                        /*clear*/ /*_.MELT_DEBUG_FUN__V73*/ meltfptr[72] = 0 ;
                                                                    }
                                                                    ;
                                                                }
                                                            else    /*^cond.else*/
                                                                {

                                                                    /*_._IF___V72*/ meltfptr[71] =  /*reallynil*/ NULL ;;
                                                                }
                                                            ;

                                                            {
                                                                MELT_LOCATION("warmelt-outobj.melt:7403:/ locexp");
                                                                /*void*/(void)0;
                                                            }
                                                            ;
                                                            /*^quasiblock*/


                                                            /*epilog*/

                                                            /*^clear*/
                                                            /*clear*/ /*_#MELT_NEED_DBG__L17*/ meltfnum[7] = 0 ;
                                                            /*^clear*/
                                                            /*clear*/ /*_._IF___V72*/ meltfptr[71] = 0 ;
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
                                                        MELT_LOCATION("warmelt-outobj.melt:7404:/ quasiblock");


                                                        /*_._RETVAL___V1*/ meltfptr[0] = /*_.NRET__V70*/ meltfptr[69];;
                                                        MELT_LOCATION("warmelt-outobj.melt:7404:/ putxtraresult");
                                                        if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                                        if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                                        if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) (/*_.NBINDS__V16*/ meltfptr[15]);
                                                        ;
                                                        /*^finalreturn*/
                                                        ;
                                                        /*finalret*/ goto meltlabend_rout ;
                                                        /*_.LET___V58*/ meltfptr[26] = /*_.RETURN___V74*/ meltfptr[72];;

                                                        MELT_LOCATION("warmelt-outobj.melt:7370:/ clear");
                                                        /*clear*/ /*_.STSYM__V59*/ meltfptr[39] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.STBIND__V60*/ meltfptr[38] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.STLOCC__V62*/ meltfptr[36] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.MAKE_STRINGCONST__V64*/ meltfptr[22] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.MAKE_STRINGCONST__V65*/ meltfptr[64] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.TUPLE___V66*/ meltfptr[65] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.NCHK__V68*/ meltfptr[66] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.NRET__V70*/ meltfptr[69] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.RETURN___V74*/ meltfptr[72] = 0 ;
                                                        /*_._IFELSE___V57*/ meltfptr[45] = /*_.LET___V58*/ meltfptr[26];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-outobj.melt:7369:/ clear");
                                                        /*clear*/ /*_.LET___V58*/ meltfptr[26] = 0 ;
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
                                                        /*_#IS_A__L19*/ meltfnum[17] =
                                                            melt_is_instance_of((melt_ptr_t)(/*_.CTYP__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_CTYPE_GTY*/ meltfrout->tabval[21])));;
                                                        MELT_LOCATION("warmelt-outobj.melt:7407:/ cond");
                                                        /*cond*/ if (/*_#IS_A__L19*/ meltfnum[17]) /*then*/
                                                            {
                                                                /*^cond.then*/
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {

                                                                    MELT_LOCATION("warmelt-outobj.melt:7408:/ quasiblock");


                                                                    /*^cond*/
                                                                    /*cond*/ if (
                                                                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CTYP__V3*/ meltfptr[2]),
                                                                                                      (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[2])))
                                                                    ) /*then*/
                                                                        {
                                                                            /*^cond.then*/
                                                                            /*^getslot*/
                                                                            {
                                                                                melt_ptr_t slot=NULL, obj=NULL;
                                                                                obj = (melt_ptr_t)(/*_.CTYP__V3*/ meltfptr[2]) /*=obj*/;
                                                                                melt_object_get_field(slot,obj, 11, "CTYPE_AUTOBOXDISCR");
                                                                                /*_.DIS__V77*/ meltfptr[38] = slot;
                                                                            };
                                                                            ;
                                                                        }
                                                                    else    /*^cond.else*/
                                                                        {

                                                                            /*_.DIS__V77*/ meltfptr[38] =  /*reallynil*/ NULL ;;
                                                                        }
                                                                    ;
                                                                    MELT_LOCATION("warmelt-outobj.melt:7409:/ cond");
                                                                    /*cond*/ if (
                                                                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CTYP__V3*/ meltfptr[2]),
                                                                                                      (melt_ptr_t)((/*!CLASS_CTYPE_GTY*/ meltfrout->tabval[21])))
                                                                    ) /*then*/
                                                                        {
                                                                            /*^cond.then*/
                                                                            /*^getslot*/
                                                                            {
                                                                                melt_ptr_t slot=NULL, obj=NULL;
                                                                                obj = (melt_ptr_t)(/*_.CTYP__V3*/ meltfptr[2]) /*=obj*/;
                                                                                melt_object_get_field(slot,obj, 21, "CTYPG_BOXFUN");
                                                                                /*_.BOXF__V78*/ meltfptr[36] = slot;
                                                                            };
                                                                            ;
                                                                        }
                                                                    else    /*^cond.else*/
                                                                        {

                                                                            /*_.BOXF__V78*/ meltfptr[36] =  /*reallynil*/ NULL ;;
                                                                        }
                                                                    ;
                                                                    MELT_LOCATION("warmelt-outobj.melt:7410:/ cond");
                                                                    /*cond*/ if (
                                                                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CTYP__V3*/ meltfptr[2]),
                                                                                                      (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[22])))
                                                                    ) /*then*/
                                                                        {
                                                                            /*^cond.then*/
                                                                            /*^getslot*/
                                                                            {
                                                                                melt_ptr_t slot=NULL, obj=NULL;
                                                                                obj = (melt_ptr_t)(/*_.CTYP__V3*/ meltfptr[2]) /*=obj*/;
                                                                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                                                /*_.CTYNAM__V79*/ meltfptr[22] = slot;
                                                                            };
                                                                            ;
                                                                        }
                                                                    else    /*^cond.else*/
                                                                        {

                                                                            /*_.CTYNAM__V79*/ meltfptr[22] =  /*reallynil*/ NULL ;;
                                                                        }
                                                                    ;

                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    MELT_LOCATION("warmelt-outobj.melt:7411:/ apply");
                                                                    /*apply*/
                                                                    {
                                                                        union meltparam_un argtab[2];
                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                        /*^apply.arg*/
                                                                        argtab[0].meltbp_cstring =  "AUTOBOXING_";
                                                                        /*^apply.arg*/
                                                                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.CTYNAM__V79*/ meltfptr[22];
                                                                        /*_.STRING4OUT__V80*/ meltfptr[64] =  melt_apply ((meltclosure_ptr_t)((/*!STRING4OUT*/ meltfrout->tabval[23])), (melt_ptr_t)((/*!DISCR_STRING*/ meltfrout->tabval[24])), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                    }
                                                                    ;

                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    /*^apply*/
                                                                    /*apply*/
                                                                    {
                                                                        /*_.GTSYM__V81*/ meltfptr[65] =  melt_apply ((meltclosure_ptr_t)((/*!CLONE_SYMBOL*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.STRING4OUT__V80*/ meltfptr[64]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                                                                    }
                                                                    ;

                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    MELT_LOCATION("warmelt-outobj.melt:7412:/ quasiblock");


                                                                    /*^rawallocobj*/
                                                                    /*rawallocobj*/
                                                                    {
                                                                        melt_ptr_t newobj = 0;
                                                                        melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_NORMAL_LET_BINDING*/ meltfrout->tabval[6])), (4), "CLASS_NORMAL_LET_BINDING");
                                                                        /*_.INST__V83*/ meltfptr[69] =
                                                                            newobj;
                                                                    };
                                                                    ;
                                                                    /*^putslot*/
                                                                    /*putslot*/
                                                                    melt_assertmsg("checkobj putslot  _ @LETBIND_LOC", melt_magic_discr((melt_ptr_t)(/*_.INST__V83*/ meltfptr[69])) == MELTOBMAG_OBJECT);
                                                                    melt_putfield_object((/*_.INST__V83*/ meltfptr[69]), (3), ((/*nil*/NULL)), "LETBIND_LOC");
                                                                    ;
                                                                    /*^putslot*/
                                                                    /*putslot*/
                                                                    melt_assertmsg("checkobj putslot  _ @BINDER", melt_magic_discr((melt_ptr_t)(/*_.INST__V83*/ meltfptr[69])) == MELTOBMAG_OBJECT);
                                                                    melt_putfield_object((/*_.INST__V83*/ meltfptr[69]), (0), (/*_.GTSYM__V81*/ meltfptr[65]), "BINDER");
                                                                    ;
                                                                    /*^putslot*/
                                                                    /*putslot*/
                                                                    melt_assertmsg("checkobj putslot  _ @LETBIND_TYPE", melt_magic_discr((melt_ptr_t)(/*_.INST__V83*/ meltfptr[69])) == MELTOBMAG_OBJECT);
                                                                    melt_putfield_object((/*_.INST__V83*/ meltfptr[69]), (1), ((/*!CTYPE_CSTRING*/ meltfrout->tabval[18])), "LETBIND_TYPE");
                                                                    ;
                                                                    /*^putslot*/
                                                                    /*putslot*/
                                                                    melt_assertmsg("checkobj putslot  _ @LETBIND_EXPR", melt_magic_discr((melt_ptr_t)(/*_.INST__V83*/ meltfptr[69])) == MELTOBMAG_OBJECT);
                                                                    melt_putfield_object((/*_.INST__V83*/ meltfptr[69]), (2), (/*_.NEXP__V2*/ meltfptr[1]), "LETBIND_EXPR");
                                                                    ;
                                                                    /*^touchobj*/

                                                                    melt_dbgtrace_written_object (/*_.INST__V83*/ meltfptr[69], "newly made instance");
                                                                    ;
                                                                    /*_.GTBIND__V82*/ meltfptr[66] = /*_.INST__V83*/ meltfptr[69];;

                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    MELT_LOCATION("warmelt-outobj.melt:7418:/ quasiblock");


                                                                    /*^rawallocobj*/
                                                                    /*rawallocobj*/
                                                                    {
                                                                        melt_ptr_t newobj = 0;
                                                                        melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_NREP_LOCSYMOCC*/ meltfrout->tabval[8])), (4), "CLASS_NREP_LOCSYMOCC");
                                                                        /*_.INST__V85*/ meltfptr[26] =
                                                                            newobj;
                                                                    };
                                                                    ;
                                                                    /*^putslot*/
                                                                    /*putslot*/
                                                                    melt_assertmsg("checkobj putslot  _ @NREP_LOC", melt_magic_discr((melt_ptr_t)(/*_.INST__V85*/ meltfptr[26])) == MELTOBMAG_OBJECT);
                                                                    melt_putfield_object((/*_.INST__V85*/ meltfptr[26]), (0), ((/*nil*/NULL)), "NREP_LOC");
                                                                    ;
                                                                    /*^putslot*/
                                                                    /*putslot*/
                                                                    melt_assertmsg("checkobj putslot  _ @NOCC_CTYP", melt_magic_discr((melt_ptr_t)(/*_.INST__V85*/ meltfptr[26])) == MELTOBMAG_OBJECT);
                                                                    melt_putfield_object((/*_.INST__V85*/ meltfptr[26]), (2), (/*_.CTYP__V3*/ meltfptr[2]), "NOCC_CTYP");
                                                                    ;
                                                                    /*^putslot*/
                                                                    /*putslot*/
                                                                    melt_assertmsg("checkobj putslot  _ @NOCC_SYMB", melt_magic_discr((melt_ptr_t)(/*_.INST__V85*/ meltfptr[26])) == MELTOBMAG_OBJECT);
                                                                    melt_putfield_object((/*_.INST__V85*/ meltfptr[26]), (1), (/*_.GTSYM__V81*/ meltfptr[65]), "NOCC_SYMB");
                                                                    ;
                                                                    /*^putslot*/
                                                                    /*putslot*/
                                                                    melt_assertmsg("checkobj putslot  _ @NOCC_BIND", melt_magic_discr((melt_ptr_t)(/*_.INST__V85*/ meltfptr[26])) == MELTOBMAG_OBJECT);
                                                                    melt_putfield_object((/*_.INST__V85*/ meltfptr[26]), (3), (/*_.GTBIND__V82*/ meltfptr[66]), "NOCC_BIND");
                                                                    ;
                                                                    /*^touchobj*/

                                                                    melt_dbgtrace_written_object (/*_.INST__V85*/ meltfptr[26], "newly made instance");
                                                                    ;
                                                                    /*_.GTLOCC__V84*/ meltfptr[72] = /*_.INST__V85*/ meltfptr[26];;

                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    /*^compute*/
                                                                    /*_.MAKE_STRINGCONST__V86*/ meltfptr[85] =
                                                                        (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[14])), ( "/*autobox-gty ")));;
                                                                    /*^compute*/
                                                                    /*_.MAKE_STRING__V87*/ meltfptr[86] =
                                                                        (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[14])),
                                                                                              melt_string_str((melt_ptr_t)(/*_.CTYNAM__V79*/ meltfptr[22]))));;
                                                                    /*^compute*/
                                                                    /*_.MAKE_STRINGCONST__V88*/ meltfptr[87] =
                                                                        (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[14])), ( "*/ ")));;
                                                                    /*^compute*/
                                                                    /*_.MAKE_STRING__V89*/ meltfptr[88] =
                                                                        (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[14])),
                                                                                              melt_string_str((melt_ptr_t)(/*_.BOXF__V78*/ meltfptr[36]))));;
                                                                    /*^compute*/
                                                                    /*_.MAKE_STRINGCONST__V90*/ meltfptr[89] =
                                                                        (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[14])), ( " ((meltobject_ptr_t)MELT_PREDEF (")));;
                                                                    /*^compute*/
                                                                    /*_.MAKE_STRING__V91*/ meltfptr[90] =
                                                                        (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[14])),
                                                                                              melt_string_str((melt_ptr_t)(/*_.DIS__V77*/ meltfptr[38]))));;
                                                                    /*^compute*/
                                                                    /*_.MAKE_STRINGCONST__V92*/ meltfptr[91] =
                                                                        (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[14])), ( "), ")));;
                                                                    /*^compute*/
                                                                    /*_.MAKE_STRINGCONST__V93*/ meltfptr[92] =
                                                                        (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[14])), ( ")")));;
                                                                    MELT_LOCATION("warmelt-outobj.melt:7427:/ blockmultialloc");
                                                                    /*multiallocblock*/
                                                                    {
                                                                        struct meltletrec_4_st
                                                                        {
                                                                            struct MELT_MULTIPLE_STRUCT(9) rtup_0__TUPLREC__x10;
                                                                            long meltletrec_4_endgap;
                                                                        } *meltletrec_4_ptr = 0;
                                                                        meltletrec_4_ptr = (struct meltletrec_4_st *) meltgc_allocate (sizeof (struct meltletrec_4_st), 0);
                                                                        /*^blockmultialloc.initfill*/
                                                                        /*inimult rtup_0__TUPLREC__x10*/
                                                                        /*_.TUPLREC___V95*/ meltfptr[94] = (melt_ptr_t) &meltletrec_4_ptr->rtup_0__TUPLREC__x10;
                                                                        meltletrec_4_ptr->rtup_0__TUPLREC__x10.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_MULTIPLE))));
                                                                        meltletrec_4_ptr->rtup_0__TUPLREC__x10.nbval = 9;


                                                                        /*^putuple*/
                                                                        /*putupl#23*/
                                                                        melt_assertmsg("putupl [:7427] #23 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))== MELTOBMAG_MULTIPLE);
                                                                        melt_assertmsg("putupl [:7427] #23 checkoff", (0>=0 && 0< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))));
                                                                        ((meltmultiple_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))->tabval[0] = (melt_ptr_t)(/*_.MAKE_STRINGCONST__V86*/ meltfptr[85]);
                                                                        ;
                                                                        /*^putuple*/
                                                                        /*putupl#24*/
                                                                        melt_assertmsg("putupl [:7427] #24 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))== MELTOBMAG_MULTIPLE);
                                                                        melt_assertmsg("putupl [:7427] #24 checkoff", (1>=0 && 1< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))));
                                                                        ((meltmultiple_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))->tabval[1] = (melt_ptr_t)(/*_.MAKE_STRING__V87*/ meltfptr[86]);
                                                                        ;
                                                                        /*^putuple*/
                                                                        /*putupl#25*/
                                                                        melt_assertmsg("putupl [:7427] #25 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))== MELTOBMAG_MULTIPLE);
                                                                        melt_assertmsg("putupl [:7427] #25 checkoff", (2>=0 && 2< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))));
                                                                        ((meltmultiple_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))->tabval[2] = (melt_ptr_t)(/*_.MAKE_STRINGCONST__V88*/ meltfptr[87]);
                                                                        ;
                                                                        /*^putuple*/
                                                                        /*putupl#26*/
                                                                        melt_assertmsg("putupl [:7427] #26 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))== MELTOBMAG_MULTIPLE);
                                                                        melt_assertmsg("putupl [:7427] #26 checkoff", (3>=0 && 3< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))));
                                                                        ((meltmultiple_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))->tabval[3] = (melt_ptr_t)(/*_.MAKE_STRING__V89*/ meltfptr[88]);
                                                                        ;
                                                                        /*^putuple*/
                                                                        /*putupl#27*/
                                                                        melt_assertmsg("putupl [:7427] #27 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))== MELTOBMAG_MULTIPLE);
                                                                        melt_assertmsg("putupl [:7427] #27 checkoff", (4>=0 && 4< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))));
                                                                        ((meltmultiple_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))->tabval[4] = (melt_ptr_t)(/*_.MAKE_STRINGCONST__V90*/ meltfptr[89]);
                                                                        ;
                                                                        /*^putuple*/
                                                                        /*putupl#28*/
                                                                        melt_assertmsg("putupl [:7427] #28 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))== MELTOBMAG_MULTIPLE);
                                                                        melt_assertmsg("putupl [:7427] #28 checkoff", (5>=0 && 5< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))));
                                                                        ((meltmultiple_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))->tabval[5] = (melt_ptr_t)(/*_.MAKE_STRING__V91*/ meltfptr[90]);
                                                                        ;
                                                                        /*^putuple*/
                                                                        /*putupl#29*/
                                                                        melt_assertmsg("putupl [:7427] #29 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))== MELTOBMAG_MULTIPLE);
                                                                        melt_assertmsg("putupl [:7427] #29 checkoff", (6>=0 && 6< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))));
                                                                        ((meltmultiple_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))->tabval[6] = (melt_ptr_t)(/*_.MAKE_STRINGCONST__V92*/ meltfptr[91]);
                                                                        ;
                                                                        /*^putuple*/
                                                                        /*putupl#30*/
                                                                        melt_assertmsg("putupl [:7427] #30 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))== MELTOBMAG_MULTIPLE);
                                                                        melt_assertmsg("putupl [:7427] #30 checkoff", (7>=0 && 7< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))));
                                                                        ((meltmultiple_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))->tabval[7] = (melt_ptr_t)(/*_.GTLOCC__V84*/ meltfptr[72]);
                                                                        ;
                                                                        /*^putuple*/
                                                                        /*putupl#31*/
                                                                        melt_assertmsg("putupl [:7427] #31 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))== MELTOBMAG_MULTIPLE);
                                                                        melt_assertmsg("putupl [:7427] #31 checkoff", (8>=0 && 8< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))));
                                                                        ((meltmultiple_ptr_t)(/*_.TUPLREC___V95*/ meltfptr[94]))->tabval[8] = (melt_ptr_t)(/*_.MAKE_STRINGCONST__V93*/ meltfptr[92]);
                                                                        ;
                                                                        /*^touch*/
                                                                        meltgc_touch(/*_.TUPLREC___V95*/ meltfptr[94]);
                                                                        ;
                                                                        /*_.TUPLE___V94*/ meltfptr[93] = /*_.TUPLREC___V95*/ meltfptr[94];;
                                                                        /*epilog*/

                                                                        MELT_LOCATION("warmelt-outobj.melt:7427:/ clear");
                                                                        /*clear*/ /*_.TUPLREC___V95*/ meltfptr[94] = 0 ;
                                                                        /*^clear*/
                                                                        /*clear*/ /*_.TUPLREC___V95*/ meltfptr[94] = 0 ;
                                                                    } /*end multiallocblock*/
                                                                    ;
                                                                    MELT_LOCATION("warmelt-outobj.melt:7423:/ quasiblock");


                                                                    /*^rawallocobj*/
                                                                    /*rawallocobj*/
                                                                    {
                                                                        melt_ptr_t newobj = 0;
                                                                        melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_NREP_CHUNK*/ meltfrout->tabval[13])), (4), "CLASS_NREP_CHUNK");
                                                                        /*_.INST__V97*/ meltfptr[96] =
                                                                            newobj;
                                                                    };
                                                                    ;
                                                                    /*^putslot*/
                                                                    /*putslot*/
                                                                    melt_assertmsg("checkobj putslot  _ @NREP_LOC", melt_magic_discr((melt_ptr_t)(/*_.INST__V97*/ meltfptr[96])) == MELTOBMAG_OBJECT);
                                                                    melt_putfield_object((/*_.INST__V97*/ meltfptr[96]), (0), ((/*nil*/NULL)), "NREP_LOC");
                                                                    ;
                                                                    /*^putslot*/
                                                                    /*putslot*/
                                                                    melt_assertmsg("checkobj putslot  _ @NCHUNK_EXPANSION", melt_magic_discr((melt_ptr_t)(/*_.INST__V97*/ meltfptr[96])) == MELTOBMAG_OBJECT);
                                                                    melt_putfield_object((/*_.INST__V97*/ meltfptr[96]), (2), (/*_.TUPLE___V94*/ meltfptr[93]), "NCHUNK_EXPANSION");
                                                                    ;
                                                                    /*^putslot*/
                                                                    /*putslot*/
                                                                    melt_assertmsg("checkobj putslot  _ @NCHUNK_OPER", melt_magic_discr((melt_ptr_t)(/*_.INST__V97*/ meltfptr[96])) == MELTOBMAG_OBJECT);
                                                                    melt_putfield_object((/*_.INST__V97*/ meltfptr[96]), (3), ((/*!konst_25_AUTOBOXGTY*/ meltfrout->tabval[25])), "NCHUNK_OPER");
                                                                    ;
                                                                    /*^putslot*/
                                                                    /*putslot*/
                                                                    melt_assertmsg("checkobj putslot  _ @NEXPR_CTYP", melt_magic_discr((melt_ptr_t)(/*_.INST__V97*/ meltfptr[96])) == MELTOBMAG_OBJECT);
                                                                    melt_putfield_object((/*_.INST__V97*/ meltfptr[96]), (1), ((/*!CTYPE_VALUE*/ meltfrout->tabval[7])), "NEXPR_CTYP");
                                                                    ;
                                                                    /*^touchobj*/

                                                                    melt_dbgtrace_written_object (/*_.INST__V97*/ meltfptr[96], "newly made instance");
                                                                    ;
                                                                    /*_.NCHK__V96*/ meltfptr[94] = /*_.INST__V97*/ meltfptr[96];;

                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    MELT_LOCATION("warmelt-outobj.melt:7449:/ quasiblock");


                                                                    /*^rawallocobj*/
                                                                    /*rawallocobj*/
                                                                    {
                                                                        melt_ptr_t newobj = 0;
                                                                        melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_NREP_RETURN*/ meltfrout->tabval[9])), (3), "CLASS_NREP_RETURN");
                                                                        /*_.INST__V99*/ meltfptr[98] =
                                                                            newobj;
                                                                    };
                                                                    ;
                                                                    /*^putslot*/
                                                                    /*putslot*/
                                                                    melt_assertmsg("checkobj putslot  _ @NRET_MAIN", melt_magic_discr((melt_ptr_t)(/*_.INST__V99*/ meltfptr[98])) == MELTOBMAG_OBJECT);
                                                                    melt_putfield_object((/*_.INST__V99*/ meltfptr[98]), (1), (/*_.CLOCC__V14*/ meltfptr[13]), "NRET_MAIN");
                                                                    ;
                                                                    /*^touchobj*/

                                                                    melt_dbgtrace_written_object (/*_.INST__V99*/ meltfptr[98], "newly made instance");
                                                                    ;
                                                                    /*_.NRET__V98*/ meltfptr[97] = /*_.INST__V99*/ meltfptr[98];;

                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    MELT_LOCATION("warmelt-outobj.melt:7453:/ cond");
                                                                    /*cond*/ if (
                                                                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CBIND__V12*/ meltfptr[11]),
                                                                                                      (melt_ptr_t)((/*!CLASS_LET_BINDING*/ meltfrout->tabval[10])))
                                                                    ) /*then*/
                                                                        {
                                                                            /*^cond.then*/
                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {

                                                                                /*^putslot*/
                                                                                /*putslot*/
                                                                                melt_assertmsg("checkobj putslot  _ @LETBIND_EXPR", melt_magic_discr((melt_ptr_t)(/*_.CBIND__V12*/ meltfptr[11])) == MELTOBMAG_OBJECT);
                                                                                melt_putfield_object((/*_.CBIND__V12*/ meltfptr[11]), (2), (/*_.NCHK__V96*/ meltfptr[94]), "LETBIND_EXPR");
                                                                                ;
                                                                                /*^touch*/
                                                                                meltgc_touch(/*_.CBIND__V12*/ meltfptr[11]);
                                                                                ;
                                                                                /*^touchobj*/

                                                                                melt_dbgtrace_written_object (/*_.CBIND__V12*/ meltfptr[11], "put-fields");
                                                                                ;
                                                                                /*epilog*/
                                                                            }
                                                                            ;
                                                                        } /*noelse*/
                                                                    ;

                                                                    {
                                                                        MELT_LOCATION("warmelt-outobj.melt:7454:/ locexp");
                                                                        meltgc_prepend_list((melt_ptr_t)(/*_.NBINDS__V16*/ meltfptr[15]), (melt_ptr_t)(/*_.GTBIND__V82*/ meltfptr[66]));
                                                                    }
                                                                    ;

#if MELT_HAVE_DEBUG
                                                                    MELT_LOCATION("warmelt-outobj.melt:7455:/ cppif.then");
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
                                                                        /*_#MELT_NEED_DBG__L20*/ meltfnum[7] =
                                                                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                                                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                                                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                                                            ;;
                                                                        MELT_LOCATION("warmelt-outobj.melt:7455:/ cond");
                                                                        /*cond*/ if (/*_#MELT_NEED_DBG__L20*/ meltfnum[7]) /*then*/
                                                                            {
                                                                                /*^cond.then*/
                                                                                /*^block*/
                                                                                /*anyblock*/
                                                                                {

                                                                                    /*_#MELT_CALLCOUNT__L21*/ meltfnum[20] =
                                                                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                                                                        meltcallcount  /* melt_callcount debugging */
#else
                                                                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                                                                        ;;

                                                                                    MELT_CHECK_SIGNAL();
                                                                                    ;
                                                                                    MELT_LOCATION("warmelt-outobj.melt:7455:/ apply");
                                                                                    /*apply*/
                                                                                    {
                                                                                        union meltparam_un argtab[7];
                                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                                        /*^apply.arg*/
                                                                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L21*/ meltfnum[20];
                                                                                        /*^apply.arg*/
                                                                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                                                        /*^apply.arg*/
                                                                                        argtab[2].meltbp_long = 7455;
                                                                                        /*^apply.arg*/
                                                                                        argtab[3].meltbp_cstring =  "autobox_normal_return gtyctype return nret=";
                                                                                        /*^apply.arg*/
                                                                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.NRET__V98*/ meltfptr[97];
                                                                                        /*^apply.arg*/
                                                                                        argtab[5].meltbp_cstring =  " nbinds=";
                                                                                        /*^apply.arg*/
                                                                                        argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.NBINDS__V16*/ meltfptr[15];
                                                                                        /*_.MELT_DEBUG_FUN__V101*/ meltfptr[100] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                    }
                                                                                    ;
                                                                                    /*_._IF___V100*/ meltfptr[99] = /*_.MELT_DEBUG_FUN__V101*/ meltfptr[100];;
                                                                                    /*epilog*/

                                                                                    MELT_LOCATION("warmelt-outobj.melt:7455:/ clear");
                                                                                    /*clear*/ /*_#MELT_CALLCOUNT__L21*/ meltfnum[20] = 0 ;
                                                                                    /*^clear*/
                                                                                    /*clear*/ /*_.MELT_DEBUG_FUN__V101*/ meltfptr[100] = 0 ;
                                                                                }
                                                                                ;
                                                                            }
                                                                        else    /*^cond.else*/
                                                                            {

                                                                                /*_._IF___V100*/ meltfptr[99] =  /*reallynil*/ NULL ;;
                                                                            }
                                                                        ;

                                                                        {
                                                                            MELT_LOCATION("warmelt-outobj.melt:7455:/ locexp");
                                                                            /*void*/(void)0;
                                                                        }
                                                                        ;
                                                                        /*^quasiblock*/


                                                                        /*epilog*/

                                                                        /*^clear*/
                                                                        /*clear*/ /*_#MELT_NEED_DBG__L20*/ meltfnum[7] = 0 ;
                                                                        /*^clear*/
                                                                        /*clear*/ /*_._IF___V100*/ meltfptr[99] = 0 ;
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
                                                                    MELT_LOCATION("warmelt-outobj.melt:7456:/ quasiblock");


                                                                    /*_._RETVAL___V1*/ meltfptr[0] = /*_.NRET__V98*/ meltfptr[97];;
                                                                    MELT_LOCATION("warmelt-outobj.melt:7456:/ putxtraresult");
                                                                    if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                                                    if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                                                    if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) (/*_.NBINDS__V16*/ meltfptr[15]);
                                                                    ;
                                                                    /*^finalreturn*/
                                                                    ;
                                                                    /*finalret*/ goto meltlabend_rout ;
                                                                    /*_.LET___V76*/ meltfptr[39] = /*_.RETURN___V102*/ meltfptr[100];;

                                                                    MELT_LOCATION("warmelt-outobj.melt:7408:/ clear");
                                                                    /*clear*/ /*_.DIS__V77*/ meltfptr[38] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.BOXF__V78*/ meltfptr[36] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.CTYNAM__V79*/ meltfptr[22] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.STRING4OUT__V80*/ meltfptr[64] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.GTSYM__V81*/ meltfptr[65] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.GTBIND__V82*/ meltfptr[66] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.GTLOCC__V84*/ meltfptr[72] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.MAKE_STRINGCONST__V86*/ meltfptr[85] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.MAKE_STRING__V87*/ meltfptr[86] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.MAKE_STRINGCONST__V88*/ meltfptr[87] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.MAKE_STRING__V89*/ meltfptr[88] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.MAKE_STRINGCONST__V90*/ meltfptr[89] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.MAKE_STRING__V91*/ meltfptr[90] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.MAKE_STRINGCONST__V92*/ meltfptr[91] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.MAKE_STRINGCONST__V93*/ meltfptr[92] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.TUPLE___V94*/ meltfptr[93] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.NCHK__V96*/ meltfptr[94] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.NRET__V98*/ meltfptr[97] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.RETURN___V102*/ meltfptr[100] = 0 ;
                                                                    /*_._IFELSE___V75*/ meltfptr[71] = /*_.LET___V76*/ meltfptr[39];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-outobj.melt:7407:/ clear");
                                                                    /*clear*/ /*_.LET___V76*/ meltfptr[39] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        else    /*^cond.else*/
                                                            {

                                                                /*^block*/
                                                                /*anyblock*/
                                                                {


#if MELT_HAVE_DEBUG
                                                                    MELT_LOCATION("warmelt-outobj.melt:7460:/ cppif.then");
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
                                                                        /*_#MELT_NEED_DBG__L22*/ meltfnum[20] =
                                                                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                                                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                                                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                                                            ;;
                                                                        MELT_LOCATION("warmelt-outobj.melt:7460:/ cond");
                                                                        /*cond*/ if (/*_#MELT_NEED_DBG__L22*/ meltfnum[20]) /*then*/
                                                                            {
                                                                                /*^cond.then*/
                                                                                /*^block*/
                                                                                /*anyblock*/
                                                                                {

                                                                                    /*_#MELT_CALLCOUNT__L23*/ meltfnum[7] =
                                                                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                                                                        meltcallcount  /* melt_callcount debugging */
#else
                                                                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                                                                        ;;
                                                                                    /*^compute*/
                                                                                    /*_.DISCRIM__V104*/ meltfptr[38] =
                                                                                        ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.CTYP__V3*/ meltfptr[2]))));;

                                                                                    MELT_CHECK_SIGNAL();
                                                                                    ;
                                                                                    MELT_LOCATION("warmelt-outobj.melt:7460:/ apply");
                                                                                    /*apply*/
                                                                                    {
                                                                                        union meltparam_un argtab[7];
                                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                                        /*^apply.arg*/
                                                                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L23*/ meltfnum[7];
                                                                                        /*^apply.arg*/
                                                                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                                                        /*^apply.arg*/
                                                                                        argtab[2].meltbp_long = 7460;
                                                                                        /*^apply.arg*/
                                                                                        argtab[3].meltbp_cstring =  "autobox_normal_return unexpected ctyp=";
                                                                                        /*^apply.arg*/
                                                                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CTYP__V3*/ meltfptr[2];
                                                                                        /*^apply.arg*/
                                                                                        argtab[5].meltbp_cstring =  "\n of discrim ";
                                                                                        /*^apply.arg*/
                                                                                        argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.DISCRIM__V104*/ meltfptr[38];
                                                                                        /*_.MELT_DEBUG_FUN__V105*/ meltfptr[36] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                    }
                                                                                    ;
                                                                                    /*_._IF___V103*/ meltfptr[99] = /*_.MELT_DEBUG_FUN__V105*/ meltfptr[36];;
                                                                                    /*epilog*/

                                                                                    MELT_LOCATION("warmelt-outobj.melt:7460:/ clear");
                                                                                    /*clear*/ /*_#MELT_CALLCOUNT__L23*/ meltfnum[7] = 0 ;
                                                                                    /*^clear*/
                                                                                    /*clear*/ /*_.DISCRIM__V104*/ meltfptr[38] = 0 ;
                                                                                    /*^clear*/
                                                                                    /*clear*/ /*_.MELT_DEBUG_FUN__V105*/ meltfptr[36] = 0 ;
                                                                                }
                                                                                ;
                                                                            }
                                                                        else    /*^cond.else*/
                                                                            {

                                                                                /*_._IF___V103*/ meltfptr[99] =  /*reallynil*/ NULL ;;
                                                                            }
                                                                        ;

                                                                        {
                                                                            MELT_LOCATION("warmelt-outobj.melt:7460:/ locexp");
                                                                            /*void*/(void)0;
                                                                        }
                                                                        ;
                                                                        /*^quasiblock*/


                                                                        /*epilog*/

                                                                        /*^clear*/
                                                                        /*clear*/ /*_#MELT_NEED_DBG__L22*/ meltfnum[20] = 0 ;
                                                                        /*^clear*/
                                                                        /*clear*/ /*_._IF___V103*/ meltfptr[99] = 0 ;
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
                                                                    MELT_LOCATION("warmelt-outobj.melt:7461:/ cppif.then");
                                                                    /*^block*/
                                                                    /*anyblock*/
                                                                    {


                                                                        MELT_CHECK_SIGNAL();
                                                                        ;
                                                                        /*^cond*/
                                                                        /*cond*/ if ((/*nil*/NULL)) /*then*/
                                                                            {
                                                                                /*^cond.then*/
                                                                                /*_._IFELSE___V107*/ meltfptr[64] = (/*nil*/NULL);;
                                                                            }
                                                                        else
                                                                            {
                                                                                MELT_LOCATION("warmelt-outobj.melt:7461:/ cond.else");

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
                                                                                        argtab[0].meltbp_cstring =  "autobox_normal_return unexpected ctype";
                                                                                        /*^apply.arg*/
                                                                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                                                        /*^apply.arg*/
                                                                                        argtab[2].meltbp_long = 7461;
                                                                                        /*^apply.arg*/
                                                                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CTYP__V3*/ meltfptr[2];
                                                                                        /*_.MELT_ASSERT_FAILURE_FUN__V108*/ meltfptr[65] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                    }
                                                                                    ;
                                                                                    /*_._IFELSE___V107*/ meltfptr[64] = /*_.MELT_ASSERT_FAILURE_FUN__V108*/ meltfptr[65];;
                                                                                    /*epilog*/

                                                                                    MELT_LOCATION("warmelt-outobj.melt:7461:/ clear");
                                                                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V108*/ meltfptr[65] = 0 ;
                                                                                }
                                                                                ;
                                                                            }
                                                                        ;
                                                                        /*_.IFCPP___V106*/ meltfptr[22] = /*_._IFELSE___V107*/ meltfptr[64];;
                                                                        /*epilog*/

                                                                        MELT_LOCATION("warmelt-outobj.melt:7461:/ clear");
                                                                        /*clear*/ /*_._IFELSE___V107*/ meltfptr[64] = 0 ;
                                                                    }

#else /*MELT_HAVE_DEBUG*/
                                                                    /*^cppif.else*/
                                                                    /*_.IFCPP___V106*/ meltfptr[22] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                                                    ;
                                                                    MELT_LOCATION("warmelt-outobj.melt:7459:/ quasiblock");


                                                                    /*_.PROGN___V109*/ meltfptr[66] = /*_.IFCPP___V106*/ meltfptr[22];;
                                                                    /*^compute*/
                                                                    /*_._IFELSE___V75*/ meltfptr[71] = /*_.PROGN___V109*/ meltfptr[66];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-outobj.melt:7407:/ clear");
                                                                    /*clear*/ /*_.IFCPP___V106*/ meltfptr[22] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.PROGN___V109*/ meltfptr[66] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        ;
                                                        /*_._IFELSE___V57*/ meltfptr[45] = /*_._IFELSE___V75*/ meltfptr[71];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-outobj.melt:7369:/ clear");
                                                        /*clear*/ /*_#IS_A__L19*/ meltfnum[17] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_._IFELSE___V75*/ meltfptr[71] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;
                                            /*_._IFELSE___V48*/ meltfptr[44] = /*_._IFELSE___V57*/ meltfptr[45];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:7358:/ clear");
                                            /*clear*/ /*_#eqeq__L16*/ meltfnum[14] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_._IFELSE___V57*/ meltfptr[45] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V30*/ meltfptr[23] = /*_._IFELSE___V48*/ meltfptr[44];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:7320:/ clear");
                                /*clear*/ /*_#eqeq__L13*/ meltfnum[0] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V48*/ meltfptr[44] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V21*/ meltfptr[17] = /*_._IFELSE___V30*/ meltfptr[23];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:7309:/ clear");
                    /*clear*/ /*_#eqeq__L10*/ meltfnum[8] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V30*/ meltfptr[23] = 0 ;
                }
                ;
            }
        ;
        /*_.LET___V10*/ meltfptr[8] = /*_._IFELSE___V21*/ meltfptr[17];;

        MELT_LOCATION("warmelt-outobj.melt:7292:/ clear");
        /*clear*/ /*_.CSYM__V11*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CBIND__V12*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CLOCC__V14*/ meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NBINDS__V16*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_#eqeq__L6*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V21*/ meltfptr[17] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7288:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V10*/ meltfptr[8];;

        {
            MELT_LOCATION("warmelt-outobj.melt:7288:/ locexp");
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
    melt_trace_end("AUTOBOX_NORMAL_RETURN", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_137_WARMELTmiOUTOBJ_AUTOBOX_NORMAL_RETURN*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS fromline 1752 */

    /** start of frame for meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS// fromline 1568
        : public Melt_CallFrameWithValues<146>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[70];
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
        MeltFrame_meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<146> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS), clos) {};
        MeltFrame_meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS() //the constructor fromline 1642
            : Melt_CallFrameWithValues<146> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<146> (fil,lin, sizeof(MeltFrame_meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS)) {};
        MeltFrame_meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<146> (fil,lin, sizeof(MeltFrame_meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS


    /** end of frame for meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS fromline 1697**/

    /* end of frame for routine meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS fromline 1756 */

    /* classy proc frame meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS */ MeltFrame_meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("TRANSLATE_RUN_MELT_EXPRESSIONS", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:7468:/ getarg");
    /*_.EXPRS__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7473:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:7473:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:7473:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[8];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7473;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_run_melt_expressions start exprs=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.EXPRS__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n*env=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &(/*!DEBUG_MORE*/ meltfrout->tabval[1]);
                            /*^apply.arg*/
                            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V4*/ meltfptr[3] = /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7473:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:7473:/ locexp");
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
            MELT_LOCATION("warmelt-outobj.melt:7474:/ locexp");

#if MELT_HAVE_DEBUG
            if (melt_need_debug (0))
                melt_dbgshortbacktrace(( "translate_run_melt_expressions"), (12));
#endif
            ;
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7475:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-outobj.melt:7475:/ cond");
            /*cond*/ if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[3] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7475:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check env";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7475;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V7*/ meltfptr[3] = /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7475:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V6*/ meltfptr[4] = /*_._IFELSE___V7*/ meltfptr[3];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7475:/ clear");
            /*clear*/ /*_#IS_A__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V7*/ meltfptr[3] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V6*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_LIST__L4*/ meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.EXPRS__V2*/ meltfptr[1])) == MELTOBMAG_LIST);;
        /*^compute*/
        /*_#NOT__L5*/ meltfnum[1] =
            (!(/*_#IS_LIST__L4*/ meltfnum[0]));;
        MELT_LOCATION("warmelt-outobj.melt:7476:/ cond");
        /*cond*/ if (/*_#NOT__L5*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:7477:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L6*/ meltfnum[5] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:7477:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L6*/ meltfnum[5]) /*then*/
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
                                    MELT_LOCATION("warmelt-outobj.melt:7477:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[6];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[6];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 7477;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "translate_run_melt_expressions gets non-list ";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.EXPRS__V2*/ meltfptr[1];
                                        /*^apply.arg*/
                                        argtab[5].meltbp_cstring =  " and gives null";
                                        /*_.MELT_DEBUG_FUN__V11*/ meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V10*/ meltfptr[3] = /*_.MELT_DEBUG_FUN__V11*/ meltfptr[10];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:7477:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L7*/ meltfnum[6] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V11*/ meltfptr[10] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V10*/ meltfptr[3] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:7477:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L6*/ meltfnum[5] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V10*/ meltfptr[3] = 0 ;
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
                    MELT_LOCATION("warmelt-outobj.melt:7478:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:7478:/ locexp");
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
                    MELT_LOCATION("warmelt-outobj.melt:7476:/ quasiblock");


                    /*_.PROGN___V13*/ meltfptr[3] = /*_.RETURN___V12*/ meltfptr[10];;
                    /*^compute*/
                    /*_._IF___V9*/ meltfptr[7] = /*_.PROGN___V13*/ meltfptr[3];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:7476:/ clear");
                    /*clear*/ /*_.RETURN___V12*/ meltfptr[10] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V13*/ meltfptr[3] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V9*/ meltfptr[7] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_.LIST_FIRST__V14*/ meltfptr[10] =
            (melt_list_first((melt_ptr_t)(/*_.EXPRS__V2*/ meltfptr[1])));;
        /*^compute*/
        /*_#NULL__L8*/ meltfnum[6] =
            (/*null*/(/*_.LIST_FIRST__V14*/ meltfptr[10]) == NULL);;
        MELT_LOCATION("warmelt-outobj.melt:7479:/ cond");
        /*cond*/ if (/*_#NULL__L8*/ meltfnum[6]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:7480:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L9*/ meltfnum[5] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:7480:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L9*/ meltfnum[5]) /*then*/
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
                                    MELT_LOCATION("warmelt-outobj.melt:7480:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[4];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L10*/ meltfnum[9];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 7480;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "translate_run_melt_expressions gets empty list and gives null";
                                        /*_.MELT_DEBUG_FUN__V17*/ meltfptr[16] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V16*/ meltfptr[15] = /*_.MELT_DEBUG_FUN__V17*/ meltfptr[16];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:7480:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L10*/ meltfnum[9] = 0 ;
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
                            MELT_LOCATION("warmelt-outobj.melt:7480:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L9*/ meltfnum[5] = 0 ;
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
                    MELT_LOCATION("warmelt-outobj.melt:7481:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:7481:/ locexp");
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
                    MELT_LOCATION("warmelt-outobj.melt:7479:/ quasiblock");


                    /*_.PROGN___V19*/ meltfptr[15] = /*_.RETURN___V18*/ meltfptr[16];;
                    /*^compute*/
                    /*_._IF___V15*/ meltfptr[3] = /*_.PROGN___V19*/ meltfptr[15];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:7479:/ clear");
                    /*clear*/ /*_.RETURN___V18*/ meltfptr[16] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V19*/ meltfptr[15] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V15*/ meltfptr[3] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:7482:/ quasiblock");


        /*_#NUM__L11*/ meltfnum[9] = 0;;
        /*^compute*/
        /*_#STARTERRCOUNT__L12*/ meltfnum[5] =
            melt_error_counter;;
        /*^compute*/
        /*_.NAKEDBASNAM__V21*/ meltfptr[15] = (/*nil*/NULL);;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7485:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[4])), (1), "CLASS_REFERENCE");
            /*_.INST__V23*/ meltfptr[22] =
                newobj;
        };
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V23*/ meltfptr[22], "newly made instance");
        ;
        /*_.REFERR__V22*/ meltfptr[21] = /*_.INST__V23*/ meltfptr[22];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7487:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[4])), (1), "CLASS_REFERENCE");
            /*_.INST__V25*/ meltfptr[24] =
                newobj;
        };
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V25*/ meltfptr[24], "newly made instance");
        ;
        /*_.REFNORMLIST__V24*/ meltfptr[23] = /*_.INST__V25*/ meltfptr[24];;

        {
            MELT_LOCATION("warmelt-outobj.melt:7489:/ locexp");
            /* translate_run_melt_expressions NUMCHK__1 */
            char basbuf_NUMCHK__1[64] ;
            long num_NUMCHK__1 = 0 ;
            memset (basbuf_NUMCHK__1, 0, sizeof(basbuf_NUMCHK__1)) ;
            melt_count_runtime_extensions++ ;
            num_NUMCHK__1 = melt_count_runtime_extensions ;
            if (melt_count_runtime_extensions
                    > MELT_MAX_RUNTIME_EXTENSIONS)
                melt_fatal_error ("too many %ld MELT runtime extensions",
                                  num_NUMCHK__1) ;
            /* In translate_run_melt_expressions NUMCHK__1 using getpid to ensure unicity
            of generated naked base name */
            const char* runinfix = melt_argument("run-infix");
            if (runinfix && runinfix[0])
                {
                    static long informruncounter;
                    /* inform only once. */
                    if (informruncounter++ == 0)
                        inform (UNKNOWN_LOCATION,
                                "MELT using the run infix %s for temporary modules",
                                runinfix);
                    if (strlen(runinfix) > 32)
                        melt_fatal_error ("too long run infix %s, should be less than thirty-two bytes", runinfix);
                    for (const char*pr = runinfix; *pr; pr++)
                        if (!ISALNUM(*pr) && *pr != '_')
                            melt_fatal_error ("invalid run infix %s, should have only alphanumerical or _",
                                              runinfix);
                    snprintf (basbuf_NUMCHK__1, sizeof(basbuf_NUMCHK__1),
                              "meltruni_%.32s_run%04lx",
                              runinfix, num_NUMCHK__1);
                }
            else
                snprintf (basbuf_NUMCHK__1, sizeof(basbuf_NUMCHK__1),
                          "melt_p%d_run%04lx", (int)getpid(), num_NUMCHK__1) ;
            /*_.NAKEDBASNAM__V21*/ meltfptr[15] = meltgc_new_string_raw_len ((meltobject_ptr_t)MELT_PREDEF (DISCR_STRING),
                                                  basbuf_NUMCHK__1, -1) ;
            ;
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7526:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L13*/ meltfnum[12] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7526:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L13*/ meltfnum[12]) /*then*/
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
                        MELT_LOCATION("warmelt-outobj.melt:7526:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[9];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L14*/ meltfnum[13];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7526;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_run_melt_expressions nakedbasnam=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.NAKEDBASNAM__V21*/ meltfptr[15];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " num=";
                            /*^apply.arg*/
                            argtab[6].meltbp_long = /*_#NUM__L11*/ meltfnum[9];
                            /*^apply.arg*/
                            argtab[7].meltbp_cstring =  " starterrcount=";
                            /*^apply.arg*/
                            argtab[8].meltbp_long = /*_#STARTERRCOUNT__L12*/ meltfnum[5];
                            /*_.MELT_DEBUG_FUN__V27*/ meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V26*/ meltfptr[25] = /*_.MELT_DEBUG_FUN__V27*/ meltfptr[26];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7526:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L14*/ meltfnum[13] = 0 ;
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
                MELT_LOCATION("warmelt-outobj.melt:7526:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L13*/ meltfnum[12] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7527:/ quasiblock");


        MELT_LOCATION("warmelt-outobj.melt:7529:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V30*/ meltfptr[29] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_7*/ meltfrout->tabval[7])), (2));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V30*/ meltfptr[29])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V30*/ meltfptr[29])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V30*/ meltfptr[29])->tabval[0] = (melt_ptr_t)(/*_.NAKEDBASNAM__V21*/ meltfptr[15]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V30*/ meltfptr[29])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 1>= 0 && 1< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V30*/ meltfptr[29])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V30*/ meltfptr[29])->tabval[1] = (melt_ptr_t)(/*_.REFERR__V22*/ meltfptr[21]);
        ;
        /*_.RUNERRORHDLR__V29*/ meltfptr[25] = /*_.LAMBDA___V30*/ meltfptr[29];;
        MELT_LOCATION("warmelt-outobj.melt:7548:/ blockmultialloc");
        /*multiallocblock*/
        {
            struct meltletrec_1_st
            {
                struct meltpair_st rpair_0___PAIROFLIST_x1;
                struct meltlist_st rlist_1__LIST_;
                long meltletrec_1_endgap;
            } *meltletrec_1_ptr = 0;
            meltletrec_1_ptr = (struct meltletrec_1_st *) meltgc_allocate (sizeof (struct meltletrec_1_st), 0);
            /*^blockmultialloc.initfill*/
            /*inipair rpair_0___PAIROFLIST_x1*/
            /*_._PAIROFLIST__V32*/ meltfptr[31] = (melt_ptr_t) &meltletrec_1_ptr->rpair_0___PAIROFLIST_x1;
            meltletrec_1_ptr->rpair_0___PAIROFLIST_x1.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_PAIR))));

            /*inilist rlist_1__LIST_*/
            /*_.LIST___V33*/ meltfptr[32] = (melt_ptr_t) &meltletrec_1_ptr->rlist_1__LIST_;
            meltletrec_1_ptr->rlist_1__LIST_.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_LIST))));



            /*^putpairhead*/
            /*putpairhead*/
            melt_assertmsg("putpairhead /2 checkpair", melt_magic_discr((melt_ptr_t)(/*_._PAIROFLIST__V32*/ meltfptr[31]))== MELTOBMAG_PAIR);
            ((meltpair_ptr_t)(/*_._PAIROFLIST__V32*/ meltfptr[31]))->hd = (melt_ptr_t) ((/*nil*/NULL));
            ;
            /*^touch*/
            meltgc_touch(/*_._PAIROFLIST__V32*/ meltfptr[31]);
            ;
            /*^putlist*/
            /*putlist*/
            melt_assertmsg("putlist checklist", melt_magic_discr((melt_ptr_t)(/*_.LIST___V33*/ meltfptr[32]))== MELTOBMAG_LIST);
            ((meltlist_ptr_t)(/*_.LIST___V33*/ meltfptr[32]))->first = (meltpair_ptr_t) (/*_._PAIROFLIST__V32*/ meltfptr[31]);
            ((meltlist_ptr_t)(/*_.LIST___V33*/ meltfptr[32]))->last = (meltpair_ptr_t) (/*_._PAIROFLIST__V32*/ meltfptr[31]);
            ;
            /*^touch*/
            meltgc_touch(/*_.LIST___V33*/ meltfptr[32]);
            ;
            /*_.LITVALIST__V31*/ meltfptr[30] = /*_.LIST___V33*/ meltfptr[32];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7548:/ clear");
            /*clear*/ /*_._PAIROFLIST__V32*/ meltfptr[31] = 0 ;
            /*^clear*/
            /*clear*/ /*_.LIST___V33*/ meltfptr[32] = 0 ;
            /*^clear*/
            /*clear*/ /*_._PAIROFLIST__V32*/ meltfptr[31] = 0 ;
            /*^clear*/
            /*clear*/ /*_.LIST___V33*/ meltfptr[32] = 0 ;
        } /*end multiallocblock*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:7550:/ quasiblock");


        /*^blockmultialloc*/
        /*multiallocblock*/
        {
            struct meltletrec_2_st
            {
                struct MELT_MULTIPLE_STRUCT(2) rtup_0__TUPLREC__x11;
                long meltletrec_2_endgap;
            } *meltletrec_2_ptr = 0;
            meltletrec_2_ptr = (struct meltletrec_2_st *) meltgc_allocate (sizeof (struct meltletrec_2_st), 0);
            /*^blockmultialloc.initfill*/
            /*inimult rtup_0__TUPLREC__x11*/
            /*_.TUPLREC___V36*/ meltfptr[35] = (melt_ptr_t) &meltletrec_2_ptr->rtup_0__TUPLREC__x11;
            meltletrec_2_ptr->rtup_0__TUPLREC__x11.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_MULTIPLE))));
            meltletrec_2_ptr->rtup_0__TUPLREC__x11.nbval = 2;


            /*^putuple*/
            /*putupl#32*/
            melt_assertmsg("putupl [:7550] #32 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V36*/ meltfptr[35]))== MELTOBMAG_MULTIPLE);
            melt_assertmsg("putupl [:7550] #32 checkoff", (0>=0 && 0< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V36*/ meltfptr[35]))));
            ((meltmultiple_ptr_t)(/*_.TUPLREC___V36*/ meltfptr[35]))->tabval[0] = (melt_ptr_t)((/*!konst_9_MACROmiEXTENSION*/ meltfrout->tabval[9]));
            ;
            /*^putuple*/
            /*putupl#33*/
            melt_assertmsg("putupl [:7550] #33 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V36*/ meltfptr[35]))== MELTOBMAG_MULTIPLE);
            melt_assertmsg("putupl [:7550] #33 checkoff", (1>=0 && 1< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V36*/ meltfptr[35]))));
            ((meltmultiple_ptr_t)(/*_.TUPLREC___V36*/ meltfptr[35]))->tabval[1] = (melt_ptr_t)(/*_.NAKEDBASNAM__V21*/ meltfptr[15]);
            ;
            /*^touch*/
            meltgc_touch(/*_.TUPLREC___V36*/ meltfptr[35]);
            ;
            /*_.TUPLE___V35*/ meltfptr[32] = /*_.TUPLREC___V36*/ meltfptr[35];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7550:/ clear");
            /*clear*/ /*_.TUPLREC___V36*/ meltfptr[35] = 0 ;
            /*^clear*/
            /*clear*/ /*_.TUPLREC___V36*/ meltfptr[35] = 0 ;
        } /*end multiallocblock*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.TUPLE___V35*/ meltfptr[32];
            /*_.MENV__V37*/ meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!FRESH_ENV*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7552:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L15*/ meltfnum[13] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7552:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L15*/ meltfnum[13]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L16*/ meltfnum[12] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7552:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[6];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L16*/ meltfnum[12];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7552;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_run_melt_expressions macroenv=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &(/*!DEBUG_MORE*/ meltfrout->tabval[1]);
                            /*^apply.arg*/
                            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.MENV__V37*/ meltfptr[35];
                            /*_.MELT_DEBUG_FUN__V39*/ meltfptr[38] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V38*/ meltfptr[37] = /*_.MELT_DEBUG_FUN__V39*/ meltfptr[38];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7552:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L16*/ meltfnum[12] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V39*/ meltfptr[38] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V38*/ meltfptr[37] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7552:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L15*/ meltfnum[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V38*/ meltfptr[37] = 0 ;
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
        /*_.LET___V34*/ meltfptr[31] = /*_.MENV__V37*/ meltfptr[35];;

        MELT_LOCATION("warmelt-outobj.melt:7550:/ clear");
        /*clear*/ /*_.TUPLE___V35*/ meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MENV__V37*/ meltfptr[35] = 0 ;
        /*_.MACROENV__V40*/ meltfptr[38] = /*_.LET___V34*/ meltfptr[31];;
        MELT_LOCATION("warmelt-outobj.melt:7555:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*_.MAKE_MAPSTRING__V42*/ meltfptr[32] =
            (meltgc_new_mapstrings( (meltobject_ptr_t) ((/*!DISCR_MAP_STRINGS*/ meltfrout->tabval[11])), (71)));;
        /*^compute*/
        /*_.MAKE_MAPSTRING__V43*/ meltfptr[35] =
            (meltgc_new_mapstrings( (meltobject_ptr_t) ((/*!DISCR_MAP_STRINGS*/ meltfrout->tabval[11])), (19)));;
        /*^compute*/
        /*_.MAKE_INTEGERBOX__V44*/ meltfptr[43] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[12])), (0)));;
        /*^compute*/
        /*_.MAKE_LIST__V45*/ meltfptr[44] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[13]))));;
        /*^compute*/
        /*_.MAKE_LIST__V46*/ meltfptr[45] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[13]))));;
        /*^compute*/
        /*_.MAKE_INTEGERBOX__V47*/ meltfptr[46] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[12])), (0)));;
        /*^compute*/
        /*_.MAKE_LIST__V48*/ meltfptr[47] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[13]))));;
        /*^compute*/
        /*_.MAKE_MAPSTRING__V49*/ meltfptr[48] =
            (meltgc_new_mapstrings( (meltobject_ptr_t) ((/*!DISCR_MAP_STRINGS*/ meltfrout->tabval[11])), (31)));;
        /*^compute*/
        /*_.MAKE_LIST__V50*/ meltfptr[49] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[13]))));;
        /*^compute*/
        /*_.MAKE_LIST__V51*/ meltfptr[50] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[13]))));;
        /*^compute*/
        /*_.MAKE_LIST__V52*/ meltfptr[51] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[13]))));;
        /*^compute*/
        /*_.MAKE_INTEGERBOX__V53*/ meltfptr[52] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[12])), (1)));;
        /*^compute*/
        /*_.MAKE_MAPOBJECT__V54*/ meltfptr[53] =
            (meltgc_new_mapobjects( (meltobject_ptr_t) ((/*!DISCR_MAP_OBJECTS*/ meltfrout->tabval[14])), (53)));;
        MELT_LOCATION("warmelt-outobj.melt:7555:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_RUNNING_EXTENSION_MODULE_CONTEXT*/ meltfrout->tabval[10])), (25), "CLASS_RUNNING_EXTENSION_MODULE_CONTEXT");
            /*_.INST__V56*/ meltfptr[55] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_MODULENAME", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (1), (/*_.NAKEDBASNAM__V21*/ meltfptr[15]), "MOCX_MODULENAME");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_EXPFIELDICT", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (2), (/*_.MAKE_MAPSTRING__V42*/ meltfptr[32]), "MOCX_EXPFIELDICT");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_EXPCLASSDICT", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (3), (/*_.MAKE_MAPSTRING__V43*/ meltfptr[35]), "MOCX_EXPCLASSDICT");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_INITIALENV", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (4), (/*_.ENV__V3*/ meltfptr[2]), "MOCX_INITIALENV");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_BASENV", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (6), (/*_.ENV__V3*/ meltfptr[2]), "MOCX_BASENV");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_MACROENV", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (5), (/*_.MACROENV__V40*/ meltfptr[38]), "MOCX_MACROENV");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_FUNCOUNT", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (7), (/*_.MAKE_INTEGERBOX__V44*/ meltfptr[43]), "MOCX_FUNCOUNT");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_FILETUPLE", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (8), ((/*nil*/NULL)), "MOCX_FILETUPLE");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_CHEADERLIST", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (11), (/*_.MAKE_LIST__V45*/ meltfptr[44]), "MOCX_CHEADERLIST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_CIMPLEMENTLIST", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (12), (/*_.MAKE_LIST__V46*/ meltfptr[45]), "MOCX_CIMPLEMENTLIST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_VARCOUNT", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (17), (/*_.MAKE_INTEGERBOX__V47*/ meltfptr[46]), "MOCX_VARCOUNT");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_VARLIST", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (18), (/*_.MAKE_LIST__V48*/ meltfptr[47]), "MOCX_VARLIST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_HOOKDICT", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (19), (/*_.MAKE_MAPSTRING__V49*/ meltfptr[48]), "MOCX_HOOKDICT");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_PACKAGEPCLIST", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (15), (/*_.MAKE_LIST__V50*/ meltfptr[49]), "MOCX_PACKAGEPCLIST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_GENDEVLIST", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (14), (/*_.MAKE_LIST__V51*/ meltfptr[50]), "MOCX_GENDEVLIST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_MACROLIST", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (20), (/*_.MAKE_LIST__V52*/ meltfptr[51]), "MOCX_MACROLIST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_ERRORHANDLER", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (16), (/*_.RUNERRORHDLR__V29*/ meltfptr[25]), "MOCX_ERRORHANDLER");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_ISGPLCOMPATIBLE", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (21), ((/*nil*/NULL)), "MOCX_ISGPLCOMPATIBLE");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MORCX_LITERVALIST", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (22), (/*_.LITVALIST__V31*/ meltfptr[30]), "MORCX_LITERVALIST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MORCX_COUNTLITVAL", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (24), (/*_.MAKE_INTEGERBOX__V53*/ meltfptr[52]), "MORCX_COUNTLITVAL");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MORCX_LITEROBJMAP", melt_magic_discr((melt_ptr_t)(/*_.INST__V56*/ meltfptr[55])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V56*/ meltfptr[55]), (23), (/*_.MAKE_MAPOBJECT__V54*/ meltfptr[53]), "MORCX_LITEROBJMAP");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V56*/ meltfptr[55], "newly made instance");
        ;
        /*_.M__V55*/ meltfptr[54] = /*_.INST__V56*/ meltfptr[55];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7581:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L17*/ meltfnum[12] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7581:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L17*/ meltfnum[12]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L18*/ meltfnum[13] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7581:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L18*/ meltfnum[13];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7581;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_run_melt_expressions modctx=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.M__V55*/ meltfptr[54];
                            /*_.MELT_DEBUG_FUN__V58*/ meltfptr[57] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V57*/ meltfptr[56] = /*_.MELT_DEBUG_FUN__V58*/ meltfptr[57];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7581:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L18*/ meltfnum[13] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V58*/ meltfptr[57] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V57*/ meltfptr[56] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7581:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L17*/ meltfnum[12] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V57*/ meltfptr[56] = 0 ;
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
        /*_.LET___V41*/ meltfptr[37] = /*_.M__V55*/ meltfptr[54];;

        MELT_LOCATION("warmelt-outobj.melt:7555:/ clear");
        /*clear*/ /*_.MAKE_MAPSTRING__V42*/ meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_MAPSTRING__V43*/ meltfptr[35] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_INTEGERBOX__V44*/ meltfptr[43] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_LIST__V45*/ meltfptr[44] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_LIST__V46*/ meltfptr[45] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_INTEGERBOX__V47*/ meltfptr[46] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_LIST__V48*/ meltfptr[47] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_MAPSTRING__V49*/ meltfptr[48] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_LIST__V50*/ meltfptr[49] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_LIST__V51*/ meltfptr[50] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_LIST__V52*/ meltfptr[51] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_INTEGERBOX__V53*/ meltfptr[52] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_MAPOBJECT__V54*/ meltfptr[53] = 0 ;
        /*^clear*/
        /*clear*/ /*_.M__V55*/ meltfptr[54] = 0 ;
        /*_.MODCTX__V59*/ meltfptr[57] = /*_.LET___V41*/ meltfptr[37];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7584:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*_.NCX__V60*/ meltfptr[56] =  melt_apply ((meltclosure_ptr_t)((/*!CREATE_NORMAL_EXTENDING_CONTEXT*/ meltfrout->tabval[15])), (melt_ptr_t)(/*_.MODCTX__V59*/ meltfptr[57]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:7586:/ quasiblock");


        /*_.LS__V62*/ meltfptr[35] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[13]))));;
        MELT_LOCATION("warmelt-outobj.melt:7589:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V64*/ meltfptr[44] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_20*/ meltfrout->tabval[20])), (3));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V64*/ meltfptr[44])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V64*/ meltfptr[44])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V64*/ meltfptr[44])->tabval[0] = (melt_ptr_t)(/*_.LS__V62*/ meltfptr[35]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V64*/ meltfptr[44])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 1>= 0 && 1< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V64*/ meltfptr[44])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V64*/ meltfptr[44])->tabval[1] = (melt_ptr_t)(/*_.NAKEDBASNAM__V21*/ meltfptr[15]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V64*/ meltfptr[44])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 2>= 0 && 2< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V64*/ meltfptr[44])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V64*/ meltfptr[44])->tabval[2] = (melt_ptr_t)(/*_.RUNERRORHDLR__V29*/ meltfptr[25]);
        ;
        /*_.ADDEXP__V63*/ meltfptr[43] = /*_.LAMBDA___V64*/ meltfptr[44];;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L19*/ meltfnum[13] =
            (/*null*/(/*_.EXPRS__V2*/ meltfptr[1]) == NULL);;
        MELT_LOCATION("warmelt-outobj.melt:7621:/ cond");
        /*cond*/ if (/*_#NULL__L19*/ meltfnum[13]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-outobj.melt:7622:/ locexp");
                        /*void*/(void)0;
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V65*/ meltfptr[45] = 0 ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:7621:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_MULTIPLE__L20*/ meltfnum[12] =
                        (melt_magic_discr((melt_ptr_t)(/*_.EXPRS__V2*/ meltfptr[1])) == MELTOBMAG_MULTIPLE);;
                    MELT_LOCATION("warmelt-outobj.melt:7623:/ cond");
                    /*cond*/ if (/*_#IS_MULTIPLE__L20*/ meltfnum[12]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*citerblock FOREACH_IN_MULTIPLE*/
                                {
                                    /* start foreach_in_multiple meltcit1__EACHTUP */
                                    long  meltcit1__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.EXPRS__V2*/ meltfptr[1]);
                                    for (/*_#IX__L21*/ meltfnum[20] = 0;
                                                       (/*_#IX__L21*/ meltfnum[20] >= 0) && (/*_#IX__L21*/ meltfnum[20] <  meltcit1__EACHTUP_ln);
                                                       /*_#IX__L21*/ meltfnum[20]++)
                                        {
                                            /*_.CUREXP__V67*/ meltfptr[47] = melt_multiple_nth((melt_ptr_t)(/*_.EXPRS__V2*/ meltfptr[1]),  /*_#IX__L21*/ meltfnum[20]);




                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-outobj.melt:7627:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[1];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_long = /*_#IX__L21*/ meltfnum[20];
                                                /*_.ADDEXP__V68*/ meltfptr[48] =  melt_apply ((meltclosure_ptr_t)(/*_.ADDEXP__V63*/ meltfptr[43]), (melt_ptr_t)(/*_.CUREXP__V67*/ meltfptr[47]), (MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                            /*_._IFELSE___V66*/ meltfptr[46] = /*_.ADDEXP__V68*/ meltfptr[48];;
                                            if (/*_#IX__L21*/ meltfnum[20]<0) break;
                                        } /* end  foreach_in_multiple meltcit1__EACHTUP */

                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:7624:/ clear");
                                    /*clear*/ /*_.CUREXP__V67*/ meltfptr[47] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#IX__L21*/ meltfnum[20] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.ADDEXP__V68*/ meltfptr[48] = 0 ;
                                } /*endciterblock FOREACH_IN_MULTIPLE*/
                                ;
                                /*epilog*/
                            }
                            ;
                        }
                    else
                        {
                            MELT_LOCATION("warmelt-outobj.melt:7623:/ cond.else");

                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                /*_#IS_LIST__L22*/ meltfnum[21] =
                                    (melt_magic_discr((melt_ptr_t)(/*_.EXPRS__V2*/ meltfptr[1])) == MELTOBMAG_LIST);;
                                MELT_LOCATION("warmelt-outobj.melt:7629:/ cond");
                                /*cond*/ if (/*_#IS_LIST__L22*/ meltfnum[21]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-outobj.melt:7630:/ quasiblock");


                                            /*_#IX__L23*/ meltfnum[22] = 0;;
                                            /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
                                            {
                                                /* start foreach_pair_component_in_list meltcit2__EACHLIST */
                                                for (/*_.CURPAIR__V70*/ meltfptr[50] = melt_list_first( (melt_ptr_t)/*_.EXPRS__V2*/ meltfptr[1]);
                                                                        melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V70*/ meltfptr[50]) == MELTOBMAG_PAIR;
                                                                        /*_.CURPAIR__V70*/ meltfptr[50] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V70*/ meltfptr[50]))
                                                    {
                                                        /*_.CUREXP__V71*/ meltfptr[51] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V70*/ meltfptr[50]);


                                                        /*_#miI__L24*/ meltfnum[23] =
                                                            ((/*_#IX__L23*/ meltfnum[22]) - (1));;
                                                        MELT_LOCATION("warmelt-outobj.melt:7634:/ compute");
                                                        /*_#IX__L23*/ meltfnum[22] = /*_#SETQ___L25*/ meltfnum[24] = /*_#miI__L24*/ meltfnum[23];;

                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:7635:/ apply");
                                                        /*apply*/
                                                        {
                                                            union meltparam_un argtab[1];
                                                            memset(&argtab, 0, sizeof(argtab));
                                                            /*^apply.arg*/
                                                            argtab[0].meltbp_long = /*_#IX__L23*/ meltfnum[22];
                                                            /*_.ADDEXP__V72*/ meltfptr[52] =  melt_apply ((meltclosure_ptr_t)(/*_.ADDEXP__V63*/ meltfptr[43]), (melt_ptr_t)(/*_.CUREXP__V71*/ meltfptr[51]), (MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                        }
                                                        ;
                                                        /*_._IFELSE___V69*/ meltfptr[49] = /*_.ADDEXP__V72*/ meltfptr[52];;
                                                    } /* end foreach_pair_component_in_list meltcit2__EACHLIST */
                                                /*_.CURPAIR__V70*/ meltfptr[50] = NULL;
                                                /*_.CUREXP__V71*/ meltfptr[51] = NULL;


                                                /*citerepilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:7631:/ clear");
                                                /*clear*/ /*_.CURPAIR__V70*/ meltfptr[50] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_.CUREXP__V71*/ meltfptr[51] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_#miI__L24*/ meltfnum[23] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_#SETQ___L25*/ meltfnum[24] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_.ADDEXP__V72*/ meltfptr[52] = 0 ;
                                            } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
                                            ;

                                            MELT_LOCATION("warmelt-outobj.melt:7630:/ clear");
                                            /*clear*/ /*_#IX__L23*/ meltfnum[22] = 0 ;
                                            /*epilog*/
                                        }
                                        ;
                                    }
                                else
                                    {
                                        MELT_LOCATION("warmelt-outobj.melt:7629:/ cond.else");

                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*_#IS_A__L26*/ meltfnum[22] =
                                                melt_is_instance_of((melt_ptr_t)(/*_.EXPRS__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[21])));;
                                            MELT_LOCATION("warmelt-outobj.melt:7636:/ cond");
                                            /*cond*/ if (/*_#IS_A__L26*/ meltfnum[22]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:7637:/ apply");
                                                        /*apply*/
                                                        {
                                                            union meltparam_un argtab[1];
                                                            memset(&argtab, 0, sizeof(argtab));
                                                            /*^apply.arg*/
                                                            argtab[0].meltbp_long = -999999;
                                                            /*_.ADDEXP__V74*/ meltfptr[54] =  melt_apply ((meltclosure_ptr_t)(/*_.ADDEXP__V63*/ meltfptr[43]), (melt_ptr_t)(/*_.EXPRS__V2*/ meltfptr[1]), (MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                        }
                                                        ;
                                                        /*_._IFELSE___V73*/ meltfptr[53] = /*_.ADDEXP__V74*/ meltfptr[54];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-outobj.melt:7636:/ clear");
                                                        /*clear*/ /*_.ADDEXP__V74*/ meltfptr[54] = 0 ;
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
                                                        MELT_LOCATION("warmelt-outobj.melt:7639:/ apply");
                                                        /*apply*/
                                                        {
                                                            /*_.RUNERRORHDLR__V75*/ meltfptr[54] =  melt_apply ((meltclosure_ptr_t)(/*_.RUNERRORHDLR__V29*/ meltfptr[25]), (melt_ptr_t)((/*!konst_22*/ meltfrout->tabval[22])), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                                                        }
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:7638:/ quasiblock");


                                                        /*_.PROGN___V76*/ meltfptr[75] = /*_.RUNERRORHDLR__V75*/ meltfptr[54];;
                                                        /*^compute*/
                                                        /*_._IFELSE___V73*/ meltfptr[53] = /*_.PROGN___V76*/ meltfptr[75];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-outobj.melt:7636:/ clear");
                                                        /*clear*/ /*_.RUNERRORHDLR__V75*/ meltfptr[54] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.PROGN___V76*/ meltfptr[75] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;
                                            /*_._IFELSE___V69*/ meltfptr[49] = /*_._IFELSE___V73*/ meltfptr[53];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:7629:/ clear");
                                            /*clear*/ /*_#IS_A__L26*/ meltfnum[22] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_._IFELSE___V73*/ meltfptr[53] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V66*/ meltfptr[46] = /*_._IFELSE___V69*/ meltfptr[49];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:7623:/ clear");
                                /*clear*/ /*_#IS_LIST__L22*/ meltfnum[21] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V69*/ meltfptr[49] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V65*/ meltfptr[45] = /*_._IFELSE___V66*/ meltfptr[46];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:7621:/ clear");
                    /*clear*/ /*_#IS_MULTIPLE__L20*/ meltfnum[12] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V66*/ meltfptr[46] = 0 ;
                }
                ;
            }
        ;
        /*_.LET___V61*/ meltfptr[32] = /*_.LS__V62*/ meltfptr[35];;

        MELT_LOCATION("warmelt-outobj.melt:7586:/ clear");
        /*clear*/ /*_.LS__V62*/ meltfptr[35] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ADDEXP__V63*/ meltfptr[43] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L19*/ meltfnum[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V65*/ meltfptr[45] = 0 ;
        /*_.LSEXP__V77*/ meltfptr[54] = /*_.LET___V61*/ meltfptr[32];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7643:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L27*/ meltfnum[22] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7643:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L27*/ meltfnum[22]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L28*/ meltfnum[21] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7643:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[9];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L28*/ meltfnum[21];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7643;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_run_melt_expressions modctx=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V59*/ meltfptr[57];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n ncx=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.NCX__V60*/ meltfptr[56];
                            /*^apply.arg*/
                            argtab[7].meltbp_cstring =  "\n lsexp=";
                            /*^apply.arg*/
                            argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.LSEXP__V77*/ meltfptr[54];
                            /*_.MELT_DEBUG_FUN__V79*/ meltfptr[53] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V78*/ meltfptr[75] = /*_.MELT_DEBUG_FUN__V79*/ meltfptr[53];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7643:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L28*/ meltfnum[21] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V79*/ meltfptr[53] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V78*/ meltfptr[75] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7643:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L27*/ meltfnum[22] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V78*/ meltfptr[75] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7644:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.REFERR__V22*/ meltfptr[21]),
                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.REFERR__V22*/ meltfptr[21]) /*=obj*/;
                    melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                    /*_.REFERENCED_VALUE__V80*/ meltfptr[49] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.REFERENCED_VALUE__V80*/ meltfptr[49] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:7644:/ cond");
        /*cond*/ if (/*_.REFERENCED_VALUE__V80*/ meltfptr[49]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:7645:/ apply");
                    /*apply*/
                    {
                        /*_.RUNERRORHDLR__V82*/ meltfptr[35] =  melt_apply ((meltclosure_ptr_t)(/*_.RUNERRORHDLR__V29*/ meltfptr[25]), (melt_ptr_t)((/*!konst_23*/ meltfrout->tabval[23])), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:7646:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] = (/*nil*/NULL);;
                    MELT_LOCATION("warmelt-outobj.melt:7646:/ putxtraresult");
                    if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                    if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                    if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                    ;
                    /*^finalreturn*/
                    ;
                    /*finalret*/ goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-outobj.melt:7644:/ quasiblock");


                    /*_.PROGN___V84*/ meltfptr[45] = /*_.RETURN___V83*/ meltfptr[43];;
                    /*^compute*/
                    /*_._IF___V81*/ meltfptr[46] = /*_.PROGN___V84*/ meltfptr[45];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:7644:/ clear");
                    /*clear*/ /*_.RUNERRORHDLR__V82*/ meltfptr[35] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V83*/ meltfptr[43] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V84*/ meltfptr[45] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V81*/ meltfptr[46] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:7647:/ quasiblock");


        MELT_LOCATION("warmelt-outobj.melt:7648:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*!MACROEXPAND_1*/ meltfrout->tabval[25]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V59*/ meltfptr[57];
            /*_.XL__V87*/ meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!MACROEXPAND_TOPLEVEL_LIST*/ meltfrout->tabval[24])), (melt_ptr_t)(/*_.LSEXP__V77*/ meltfptr[54]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7650:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L29*/ meltfnum[12] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7650:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L29*/ meltfnum[12]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L30*/ meltfnum[13] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7650:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L30*/ meltfnum[13];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7650;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_run_melt_expressions xlist=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XL__V87*/ meltfptr[35];
                            /*_.MELT_DEBUG_FUN__V89*/ meltfptr[45] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V88*/ meltfptr[43] = /*_.MELT_DEBUG_FUN__V89*/ meltfptr[45];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7650:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L30*/ meltfnum[13] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V89*/ meltfptr[45] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V88*/ meltfptr[43] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7650:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L29*/ meltfnum[12] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V88*/ meltfptr[43] = 0 ;
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
        /*_.LET___V86*/ meltfptr[75] = /*_.XL__V87*/ meltfptr[35];;

        MELT_LOCATION("warmelt-outobj.melt:7648:/ clear");
        /*clear*/ /*_.XL__V87*/ meltfptr[35] = 0 ;
        /*_.XLIST__V90*/ meltfptr[45] = /*_.LET___V86*/ meltfptr[75];;
        /*^compute*/
        /*_#LENXLIST__L31*/ meltfnum[21] =
            (melt_list_length((melt_ptr_t)(/*_.XLIST__V90*/ meltfptr[45])));;
        MELT_LOCATION("warmelt-outobj.melt:7655:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V92*/ meltfptr[35] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_38*/ meltfrout->tabval[38])), (1));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V92*/ meltfptr[35])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V92*/ meltfptr[35])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V92*/ meltfptr[35])->tabval[0] = (melt_ptr_t)(/*_.REFNORMLIST__V24*/ meltfptr[23]);
        ;
        /*_.NORMALEXTEND__V91*/ meltfptr[43] = /*_.LAMBDA___V92*/ meltfptr[35];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7718:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L32*/ meltfnum[22] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7718:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L32*/ meltfnum[22]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L33*/ meltfnum[13] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7718:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L33*/ meltfnum[13];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7718;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_run_melt_expressions xlist=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XLIST__V90*/ meltfptr[45];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n... normalextend=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.NORMALEXTEND__V91*/ meltfptr[43];
                            /*_.MELT_DEBUG_FUN__V94*/ meltfptr[93] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V93*/ meltfptr[92] = /*_.MELT_DEBUG_FUN__V94*/ meltfptr[93];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7718:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L33*/ meltfnum[13] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V94*/ meltfptr[93] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V93*/ meltfptr[92] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7718:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L32*/ meltfnum[22] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V93*/ meltfptr[92] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7719:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.REFERR__V22*/ meltfptr[21]),
                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.REFERR__V22*/ meltfptr[21]) /*=obj*/;
                    melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                    /*_.REFERENCED_VALUE__V95*/ meltfptr[93] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.REFERENCED_VALUE__V95*/ meltfptr[93] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#NOTNULL__L34*/ meltfnum[12] =
            ((/*_.REFERENCED_VALUE__V95*/ meltfptr[93]) != NULL);;
        MELT_LOCATION("warmelt-outobj.melt:7719:/ cond");
        /*cond*/ if (/*_#NOTNULL__L34*/ meltfnum[12]) /*then*/
            {
                /*^cond.then*/
                /*_#OR___L35*/ meltfnum[13] = /*_#NOTNULL__L34*/ meltfnum[12];;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:7719:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    /*_#MELT_ERROR_COUNTER__L36*/ meltfnum[22] =
                        melt_error_counter;;
                    /*^compute*/
                    /*_#gtI__L37*/ meltfnum[36] =
                        ((/*_#MELT_ERROR_COUNTER__L36*/ meltfnum[22]) > (/*_#STARTERRCOUNT__L12*/ meltfnum[5]));;
                    /*^compute*/
                    /*_#OR___L35*/ meltfnum[13] = /*_#gtI__L37*/ meltfnum[36];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:7719:/ clear");
                    /*clear*/ /*_#MELT_ERROR_COUNTER__L36*/ meltfnum[22] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#gtI__L37*/ meltfnum[36] = 0 ;
                }
                ;
            }
        ;
        /*^cond*/
        /*cond*/ if (/*_#OR___L35*/ meltfnum[13]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:7720:/ apply");
                    /*apply*/
                    {
                        /*_.RUNERRORHDLR__V97*/ meltfptr[96] =  melt_apply ((meltclosure_ptr_t)(/*_.RUNERRORHDLR__V29*/ meltfptr[25]), (melt_ptr_t)((/*!konst_39*/ meltfrout->tabval[39])), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:7721:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] = (/*nil*/NULL);;
                    MELT_LOCATION("warmelt-outobj.melt:7721:/ putxtraresult");
                    if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                    if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                    if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                    ;
                    /*^finalreturn*/
                    ;
                    /*finalret*/ goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-outobj.melt:7719:/ quasiblock");


                    /*_.PROGN___V99*/ meltfptr[98] = /*_.RETURN___V98*/ meltfptr[97];;
                    /*^compute*/
                    /*_._IF___V96*/ meltfptr[92] = /*_.PROGN___V99*/ meltfptr[98];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:7719:/ clear");
                    /*clear*/ /*_.RUNERRORHDLR__V97*/ meltfptr[96] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V98*/ meltfptr[97] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V99*/ meltfptr[98] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V96*/ meltfptr[92] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:7722:/ quasiblock");


        /*_.BASENAME__V101*/ meltfptr[97] =
            (meltgc_new_string_tempname_suffixed((meltobject_ptr_t)((/*!DISCR_STRING*/ meltfrout->tabval[40])),
                    melt_string_str((melt_ptr_t)(/*_.NAKEDBASNAM__V21*/ meltfptr[15])),
                    ( "_eXt")));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7724:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L38*/ meltfnum[22] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7724:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L38*/ meltfnum[22]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L39*/ meltfnum[36] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7724:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L39*/ meltfnum[36];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7724;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_run_melt_expressions basename=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.BASENAME__V101*/ meltfptr[97];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " refnormlist=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.REFNORMLIST__V24*/ meltfptr[23];
                            /*_.MELT_DEBUG_FUN__V103*/ meltfptr[102] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V102*/ meltfptr[98] = /*_.MELT_DEBUG_FUN__V103*/ meltfptr[102];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7724:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L39*/ meltfnum[36] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V103*/ meltfptr[102] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V102*/ meltfptr[98] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7724:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L38*/ meltfnum[22] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V102*/ meltfptr[98] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7727:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[6];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.BASENAME__V101*/ meltfptr[97];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V59*/ meltfptr[57];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.NCX__V60*/ meltfptr[56];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.NORMALEXTEND__V91*/ meltfptr[43];
            /*^apply.arg*/
            argtab[5].meltbp_aptr = (melt_ptr_t*) &(/*!COMPILE2OBJ_INITEXTENDPROC*/ meltfrout->tabval[42]);
            /*_.TRANSLATE_MACROEXPANDED_LIST__V104*/ meltfptr[102] =  melt_apply ((meltclosure_ptr_t)((/*!TRANSLATE_MACROEXPANDED_LIST*/ meltfrout->tabval[41])), (melt_ptr_t)(/*_.XLIST__V90*/ meltfptr[45]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7729:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L40*/ meltfnum[36] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7729:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L40*/ meltfnum[36]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L41*/ meltfnum[22] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7729:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[8];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L41*/ meltfnum[22];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7729;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_run_melt_expressions after translation";
                            /*^apply.arg*/
                            argtab[4].meltbp_cstring =  " refnormlist=";
                            /*^apply.arg*/
                            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.REFNORMLIST__V24*/ meltfptr[23];
                            /*^apply.arg*/
                            argtab[6].meltbp_cstring =  " to nakedbasnam=";
                            /*^apply.arg*/
                            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.NAKEDBASNAM__V21*/ meltfptr[15];
                            /*_.MELT_DEBUG_FUN__V106*/ meltfptr[105] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V105*/ meltfptr[98] = /*_.MELT_DEBUG_FUN__V106*/ meltfptr[105];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7729:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L41*/ meltfnum[22] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V106*/ meltfptr[105] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V105*/ meltfptr[98] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7729:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L40*/ meltfnum[36] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V105*/ meltfptr[98] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7732:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.REFERR__V22*/ meltfptr[21]),
                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.REFERR__V22*/ meltfptr[21]) /*=obj*/;
                    melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                    /*_.REFERENCED_VALUE__V107*/ meltfptr[105] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.REFERENCED_VALUE__V107*/ meltfptr[105] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#NOTNULL__L42*/ meltfnum[22] =
            ((/*_.REFERENCED_VALUE__V107*/ meltfptr[105]) != NULL);;
        MELT_LOCATION("warmelt-outobj.melt:7732:/ cond");
        /*cond*/ if (/*_#NOTNULL__L42*/ meltfnum[22]) /*then*/
            {
                /*^cond.then*/
                /*_#OR___L43*/ meltfnum[36] = /*_#NOTNULL__L42*/ meltfnum[22];;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:7732:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    /*_#MELT_ERROR_COUNTER__L44*/ meltfnum[43] =
                        melt_error_counter;;
                    /*^compute*/
                    /*_#gtI__L45*/ meltfnum[44] =
                        ((/*_#MELT_ERROR_COUNTER__L44*/ meltfnum[43]) > (/*_#STARTERRCOUNT__L12*/ meltfnum[5]));;
                    /*^compute*/
                    /*_#OR___L43*/ meltfnum[36] = /*_#gtI__L45*/ meltfnum[44];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:7732:/ clear");
                    /*clear*/ /*_#MELT_ERROR_COUNTER__L44*/ meltfnum[43] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#gtI__L45*/ meltfnum[44] = 0 ;
                }
                ;
            }
        ;
        /*^cond*/
        /*cond*/ if (/*_#OR___L43*/ meltfnum[36]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:7733:/ apply");
                    /*apply*/
                    {
                        /*_.RUNERRORHDLR__V109*/ meltfptr[108] =  melt_apply ((meltclosure_ptr_t)(/*_.RUNERRORHDLR__V29*/ meltfptr[25]), (melt_ptr_t)((/*!konst_43*/ meltfrout->tabval[43])), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:7734:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] = (/*nil*/NULL);;
                    MELT_LOCATION("warmelt-outobj.melt:7734:/ putxtraresult");
                    if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                    if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                    if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                    ;
                    /*^finalreturn*/
                    ;
                    /*finalret*/ goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-outobj.melt:7732:/ quasiblock");


                    /*_.PROGN___V111*/ meltfptr[110] = /*_.RETURN___V110*/ meltfptr[109];;
                    /*^compute*/
                    /*_._IF___V108*/ meltfptr[98] = /*_.PROGN___V111*/ meltfptr[110];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:7732:/ clear");
                    /*clear*/ /*_.RUNERRORHDLR__V109*/ meltfptr[108] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V110*/ meltfptr[109] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V111*/ meltfptr[110] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V108*/ meltfptr[98] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7736:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L46*/ meltfnum[43] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7736:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L46*/ meltfnum[43]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L47*/ meltfnum[44] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7736:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L47*/ meltfnum[44];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7736;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_run_melt_expressions before compiling runextend flavor of\
 basename=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.BASENAME__V101*/ meltfptr[97];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n modctx=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V59*/ meltfptr[57];
                            /*_.MELT_DEBUG_FUN__V113*/ meltfptr[109] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V112*/ meltfptr[108] = /*_.MELT_DEBUG_FUN__V113*/ meltfptr[109];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7736:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L47*/ meltfnum[44] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V113*/ meltfptr[109] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V112*/ meltfptr[108] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7736:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L46*/ meltfnum[43] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V112*/ meltfptr[108] = 0 ;
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
            MELT_LOCATION("warmelt-outobj.melt:7737:/ locexp");
            /*generate_flavored_melt_module*/ melt_compile_source (melt_string_str ((melt_ptr_t)/*_.BASENAME__V101*/ meltfptr[97]),
                    melt_string_str ((melt_ptr_t)/*_.BASENAME__V101*/ meltfptr[97]),
                    NULL,
                    melt_string_str ((melt_ptr_t)(/*!konst_44*/ meltfrout->tabval[44]))); /*generate_flavored_melt_module*/
            ;
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7738:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L48*/ meltfnum[44] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7738:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L48*/ meltfnum[44]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L49*/ meltfnum[43] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7738:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L49*/ meltfnum[43];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7738;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_run_melt_expressions after compiling runextend flavor of\
 basename=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.BASENAME__V101*/ meltfptr[97];
                            /*_.MELT_DEBUG_FUN__V115*/ meltfptr[109] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V114*/ meltfptr[110] = /*_.MELT_DEBUG_FUN__V115*/ meltfptr[109];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7738:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L49*/ meltfnum[43] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V115*/ meltfptr[109] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V114*/ meltfptr[110] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7738:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L48*/ meltfnum[44] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V114*/ meltfptr[110] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7740:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_LIST__L50*/ meltfnum[43] =
                (melt_magic_discr((melt_ptr_t)(/*_.LITVALIST__V31*/ meltfptr[30])) == MELTOBMAG_LIST);;
            MELT_LOCATION("warmelt-outobj.melt:7740:/ cond");
            /*cond*/ if (/*_#IS_LIST__L50*/ meltfnum[43]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V117*/ meltfptr[109] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7740:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check litvalist";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7740;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.LITVALIST__V31*/ meltfptr[30];
                            /*_.MELT_ASSERT_FAILURE_FUN__V118*/ meltfptr[110] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V117*/ meltfptr[109] = /*_.MELT_ASSERT_FAILURE_FUN__V118*/ meltfptr[110];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7740:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V118*/ meltfptr[110] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V116*/ meltfptr[108] = /*_._IFELSE___V117*/ meltfptr[109];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7740:/ clear");
            /*clear*/ /*_#IS_LIST__L50*/ meltfnum[43] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V117*/ meltfptr[109] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V116*/ meltfptr[108] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7741:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L51*/ meltfnum[44] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7741:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L51*/ meltfnum[44]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L52*/ meltfnum[43] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7741:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L52*/ meltfnum[43];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7741;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_run_melt_expressions after generation litvalist=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.LITVALIST__V31*/ meltfptr[30];
                            /*_.MELT_DEBUG_FUN__V120*/ meltfptr[109] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V119*/ meltfptr[110] = /*_.MELT_DEBUG_FUN__V120*/ meltfptr[109];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7741:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L52*/ meltfnum[43] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V120*/ meltfptr[109] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V119*/ meltfptr[110] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7741:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L51*/ meltfnum[44] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V119*/ meltfptr[110] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7742:/ quasiblock");


        /*_#NBLITVAL__L53*/ meltfnum[43] =
            (melt_list_length((melt_ptr_t)(/*_.LITVALIST__V31*/ meltfptr[30])));;
        /*^compute*/
        /*_.TUPLITVAL__V122*/ meltfptr[110] =
            (meltgc_new_multiple((meltobject_ptr_t)((/*!DISCR_MULTIPLE*/ meltfrout->tabval[45])), (/*_#NBLITVAL__L53*/ meltfnum[43])));;
        /*^compute*/
        /*_#IX__L54*/ meltfnum[44] = 0;;
        MELT_LOCATION("warmelt-outobj.melt:7745:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.MODCTX__V59*/ meltfptr[57]),
                                          (melt_ptr_t)((/*!CLASS_ANY_MODULE_CONTEXT*/ meltfrout->tabval[46])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.MODCTX__V59*/ meltfptr[57]) /*=obj*/;
                    melt_object_get_field(slot,obj, 8, "MOCX_FILETUPLE");
                    /*_.FILTUP__V123*/ meltfptr[122] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.FILTUP__V123*/ meltfptr[122] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#NBFILE__L55*/ meltfnum[54] =
            (melt_multiple_length((melt_ptr_t)(/*_.FILTUP__V123*/ meltfptr[122])));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7748:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L56*/ meltfnum[55] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7748:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L56*/ meltfnum[55]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L57*/ meltfnum[56] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7748:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L57*/ meltfnum[56];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7748;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_run_melt_expressions filtup=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.FILTUP__V123*/ meltfptr[122];
                            /*_.MELT_DEBUG_FUN__V125*/ meltfptr[124] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V124*/ meltfptr[123] = /*_.MELT_DEBUG_FUN__V125*/ meltfptr[124];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7748:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L57*/ meltfnum[56] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V125*/ meltfptr[124] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V124*/ meltfptr[123] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7748:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L56*/ meltfnum[55] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V124*/ meltfptr[123] = 0 ;
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
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit3__EACHLIST */
            for (/*_.CURPAIR__V126*/ meltfptr[124] = melt_list_first( (melt_ptr_t)/*_.LITVALIST__V31*/ meltfptr[30]);
                                     melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V126*/ meltfptr[124]) == MELTOBMAG_PAIR;
                                     /*_.CURPAIR__V126*/ meltfptr[124] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V126*/ meltfptr[124]))
                {
                    /*_.CURLITVAL__V127*/ meltfptr[123] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V126*/ meltfptr[124]);



#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:7752:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L58*/ meltfnum[56] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:7752:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L58*/ meltfnum[56]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L59*/ meltfnum[55] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:7752:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[7];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L59*/ meltfnum[55];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 7752;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "translate_run_melt_expressions ix=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_long = /*_#IX__L54*/ meltfnum[44];
                                        /*^apply.arg*/
                                        argtab[5].meltbp_cstring =  " curlitval=";
                                        /*^apply.arg*/
                                        argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.CURLITVAL__V127*/ meltfptr[123];
                                        /*_.MELT_DEBUG_FUN__V129*/ meltfptr[128] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V128*/ meltfptr[127] = /*_.MELT_DEBUG_FUN__V129*/ meltfptr[128];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:7752:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L59*/ meltfnum[55] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V129*/ meltfptr[128] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V128*/ meltfptr[127] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:7752:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L58*/ meltfnum[56] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V128*/ meltfptr[127] = 0 ;
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
                    MELT_LOCATION("warmelt-outobj.melt:7754:/ cond");
                    /*cond*/ if (/*_.CURLITVAL__V127*/ meltfptr[123]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:7755:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*_#IS_A__L60*/ meltfnum[55] =
                                        melt_is_instance_of((melt_ptr_t)(/*_.CURLITVAL__V127*/ meltfptr[123]), (melt_ptr_t)((/*!CLASS_LITERAL_VALUE*/ meltfrout->tabval[47])));;
                                    MELT_LOCATION("warmelt-outobj.melt:7755:/ cond");
                                    /*cond*/ if (/*_#IS_A__L60*/ meltfnum[55]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*_._IFELSE___V131*/ meltfptr[127] = (/*nil*/NULL);;
                                        }
                                    else
                                        {
                                            MELT_LOCATION("warmelt-outobj.melt:7755:/ cond.else");

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
                                                    argtab[0].meltbp_cstring =  "check curlitval";
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 7755;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CURLITVAL__V127*/ meltfptr[123];
                                                    /*_.MELT_ASSERT_FAILURE_FUN__V132*/ meltfptr[131] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IFELSE___V131*/ meltfptr[127] = /*_.MELT_ASSERT_FAILURE_FUN__V132*/ meltfptr[131];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:7755:/ clear");
                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V132*/ meltfptr[131] = 0 ;
                                            }
                                            ;
                                        }
                                    ;
                                    /*_.IFCPP___V130*/ meltfptr[128] = /*_._IFELSE___V131*/ meltfptr[127];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:7755:/ clear");
                                    /*clear*/ /*_#IS_A__L60*/ meltfnum[55] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IFELSE___V131*/ meltfptr[127] = 0 ;
                                }

#else /*MELT_HAVE_DEBUG*/
                                /*^cppif.else*/
                                /*_.IFCPP___V130*/ meltfptr[128] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:7757:/ quasiblock");


                                /*^cond*/
                                /*cond*/ if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CURLITVAL__V127*/ meltfptr[123]),
                                                                  (melt_ptr_t)((/*!CLASS_LITERAL_VALUE*/ meltfrout->tabval[47])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CURLITVAL__V127*/ meltfptr[123]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "LITV_VALUE");
                                            /*_.VAL__V133*/ meltfptr[131] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.VAL__V133*/ meltfptr[131] =  /*reallynil*/ NULL ;;
                                    }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:7758:/ cond");
                                /*cond*/ if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CURLITVAL__V127*/ meltfptr[123]),
                                                                  (melt_ptr_t)((/*!CLASS_LITERAL_VALUE*/ meltfrout->tabval[47])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CURLITVAL__V127*/ meltfptr[123]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 2, "LITV_RANK");
                                            /*_.BXRK__V134*/ meltfptr[127] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.BXRK__V134*/ meltfptr[127] =  /*reallynil*/ NULL ;;
                                    }
                                ;

#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:7760:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*_#GET_INT__L61*/ meltfnum[56] =
                                        (melt_get_int((melt_ptr_t)(/*_.BXRK__V134*/ meltfptr[127])));;
                                    /*^compute*/
                                    /*_#eqeqI__L62*/ meltfnum[55] =
                                        ((/*_#GET_INT__L61*/ meltfnum[56]) == (/*_#IX__L54*/ meltfnum[44]));;
                                    MELT_LOCATION("warmelt-outobj.melt:7760:/ cond");
                                    /*cond*/ if (/*_#eqeqI__L62*/ meltfnum[55]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*_._IFELSE___V136*/ meltfptr[135] = (/*nil*/NULL);;
                                        }
                                    else
                                        {
                                            MELT_LOCATION("warmelt-outobj.melt:7760:/ cond.else");

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
                                                    argtab[0].meltbp_cstring =  "check bxrk";
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 7760;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.BXRK__V134*/ meltfptr[127];
                                                    /*^apply.arg*/
                                                    argtab[4].meltbp_long = /*_#IX__L54*/ meltfnum[44];
                                                    /*_.MELT_ASSERT_FAILURE_FUN__V137*/ meltfptr[136] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IFELSE___V136*/ meltfptr[135] = /*_.MELT_ASSERT_FAILURE_FUN__V137*/ meltfptr[136];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:7760:/ clear");
                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V137*/ meltfptr[136] = 0 ;
                                            }
                                            ;
                                        }
                                    ;
                                    /*_.IFCPP___V135*/ meltfptr[134] = /*_._IFELSE___V136*/ meltfptr[135];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:7760:/ clear");
                                    /*clear*/ /*_#GET_INT__L61*/ meltfnum[56] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#eqeqI__L62*/ meltfnum[55] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IFELSE___V136*/ meltfptr[135] = 0 ;
                                }

#else /*MELT_HAVE_DEBUG*/
                                /*^cppif.else*/
                                /*_.IFCPP___V135*/ meltfptr[134] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:7761:/ locexp");
                                    meltgc_multiple_put_nth((melt_ptr_t)(/*_.TUPLITVAL__V122*/ meltfptr[110]), (/*_#IX__L54*/ meltfnum[44]), (melt_ptr_t)(/*_.VAL__V133*/ meltfptr[131]));
                                }
                                ;

                                MELT_LOCATION("warmelt-outobj.melt:7757:/ clear");
                                /*clear*/ /*_.VAL__V133*/ meltfptr[131] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.BXRK__V134*/ meltfptr[127] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.IFCPP___V135*/ meltfptr[134] = 0 ;
                                MELT_LOCATION("warmelt-outobj.melt:7754:/ quasiblock");


                                /*epilog*/

                                /*^clear*/
                                /*clear*/ /*_.IFCPP___V130*/ meltfptr[128] = 0 ;
                            }
                            ;
                        } /*noelse*/
                    ;
                    /*_#plI__L63*/ meltfnum[56] =
                        ((/*_#IX__L54*/ meltfnum[44]) + (1));;
                    MELT_LOCATION("warmelt-outobj.melt:7763:/ compute");
                    /*_#IX__L54*/ meltfnum[44] = /*_#SETQ___L64*/ meltfnum[55] = /*_#plI__L63*/ meltfnum[56];;
                } /* end foreach_pair_component_in_list meltcit3__EACHLIST */
            /*_.CURPAIR__V126*/ meltfptr[124] = NULL;
            /*_.CURLITVAL__V127*/ meltfptr[123] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:7749:/ clear");
            /*clear*/ /*_.CURPAIR__V126*/ meltfptr[124] = 0 ;
            /*^clear*/
            /*clear*/ /*_.CURLITVAL__V127*/ meltfptr[123] = 0 ;
            /*^clear*/
            /*clear*/ /*_#plI__L63*/ meltfnum[56] = 0 ;
            /*^clear*/
            /*clear*/ /*_#SETQ___L64*/ meltfnum[55] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7764:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L65*/ meltfnum[64] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7764:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L65*/ meltfnum[64]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L66*/ meltfnum[65] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7764:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L66*/ meltfnum[65];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7764;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_run_melt_expressions tuplitval=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.TUPLITVAL__V122*/ meltfptr[110];
                            /*_.MELT_DEBUG_FUN__V139*/ meltfptr[135] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V138*/ meltfptr[136] = /*_.MELT_DEBUG_FUN__V139*/ meltfptr[135];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7764:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L66*/ meltfnum[65] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V139*/ meltfptr[135] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V138*/ meltfptr[136] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7764:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L65*/ meltfnum[64] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V138*/ meltfptr[136] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7765:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L67*/ meltfnum[65] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7765:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L67*/ meltfnum[65]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L68*/ meltfnum[64] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7765:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[9];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L68*/ meltfnum[64];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7765;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "before melt_run_extension basename=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.BASENAME__V101*/ meltfptr[97];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n* env=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*^apply.arg*/
                            argtab[7].meltbp_cstring =  "\n* tuplitval=";
                            /*^apply.arg*/
                            argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.TUPLITVAL__V122*/ meltfptr[110];
                            /*_.MELT_DEBUG_FUN__V141*/ meltfptr[127] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V140*/ meltfptr[131] = /*_.MELT_DEBUG_FUN__V141*/ meltfptr[127];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7765:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L68*/ meltfnum[64] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V141*/ meltfptr[127] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V140*/ meltfptr[131] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7765:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L67*/ meltfnum[65] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V140*/ meltfptr[131] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7768:/ quasiblock");


        /*_.RESULT__V143*/ meltfptr[128] =
            /* melt_run_extension */ meltgc_run_cc_extension
            ((melt_ptr_t) /*_.BASENAME__V101*/ meltfptr[97],
             (melt_ptr_t) /*_.ENV__V3*/ meltfptr[2],
             (melt_ptr_t) /*_.TUPLITVAL__V122*/ meltfptr[110]) ;;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7770:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L69*/ meltfnum[64] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7770:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L69*/ meltfnum[64]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L70*/ meltfnum[65] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7770:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L70*/ meltfnum[65];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7770;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "after melt_run_extension basename=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.BASENAME__V101*/ meltfptr[97];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n* translate_run_melt_expressions result=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.RESULT__V143*/ meltfptr[128];
                            /*_.MELT_DEBUG_FUN__V145*/ meltfptr[136] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V144*/ meltfptr[135] = /*_.MELT_DEBUG_FUN__V145*/ meltfptr[136];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7770:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L70*/ meltfnum[65] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V145*/ meltfptr[136] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V144*/ meltfptr[135] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7770:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L69*/ meltfnum[64] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V144*/ meltfptr[135] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7772:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.RESULT__V143*/ meltfptr[128];;

        {
            MELT_LOCATION("warmelt-outobj.melt:7772:/ locexp");
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
        /*_.LET___V142*/ meltfptr[134] = /*_.RETURN___V146*/ meltfptr[127];;

        MELT_LOCATION("warmelt-outobj.melt:7768:/ clear");
        /*clear*/ /*_.RESULT__V143*/ meltfptr[128] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V146*/ meltfptr[127] = 0 ;
        /*_.LET___V121*/ meltfptr[109] = /*_.LET___V142*/ meltfptr[134];;

        MELT_LOCATION("warmelt-outobj.melt:7742:/ clear");
        /*clear*/ /*_#NBLITVAL__L53*/ meltfnum[43] = 0 ;
        /*^clear*/
        /*clear*/ /*_.TUPLITVAL__V122*/ meltfptr[110] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IX__L54*/ meltfnum[44] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FILTUP__V123*/ meltfptr[122] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NBFILE__L55*/ meltfnum[54] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V142*/ meltfptr[134] = 0 ;
        /*_.LET___V100*/ meltfptr[96] = /*_.LET___V121*/ meltfptr[109];;

        MELT_LOCATION("warmelt-outobj.melt:7722:/ clear");
        /*clear*/ /*_.BASENAME__V101*/ meltfptr[97] = 0 ;
        /*^clear*/
        /*clear*/ /*_.TRANSLATE_MACROEXPANDED_LIST__V104*/ meltfptr[102] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REFERENCED_VALUE__V107*/ meltfptr[105] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOTNULL__L42*/ meltfnum[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_#OR___L43*/ meltfnum[36] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V108*/ meltfptr[98] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V116*/ meltfptr[108] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V121*/ meltfptr[109] = 0 ;
        /*_.LET___V85*/ meltfptr[53] = /*_.LET___V100*/ meltfptr[96];;

        MELT_LOCATION("warmelt-outobj.melt:7647:/ clear");
        /*clear*/ /*_.LET___V86*/ meltfptr[75] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XLIST__V90*/ meltfptr[45] = 0 ;
        /*^clear*/
        /*clear*/ /*_#LENXLIST__L31*/ meltfnum[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NORMALEXTEND__V91*/ meltfptr[43] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REFERENCED_VALUE__V95*/ meltfptr[93] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOTNULL__L34*/ meltfnum[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_#OR___L35*/ meltfnum[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V96*/ meltfptr[92] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V100*/ meltfptr[96] = 0 ;
        /*_.LET___V28*/ meltfptr[26] = /*_.LET___V85*/ meltfptr[53];;

        MELT_LOCATION("warmelt-outobj.melt:7527:/ clear");
        /*clear*/ /*_.RUNERRORHDLR__V29*/ meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LITVALIST__V31*/ meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V34*/ meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MACROENV__V40*/ meltfptr[38] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V41*/ meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MODCTX__V59*/ meltfptr[57] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NCX__V60*/ meltfptr[56] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V61*/ meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LSEXP__V77*/ meltfptr[54] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REFERENCED_VALUE__V80*/ meltfptr[49] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V81*/ meltfptr[46] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V85*/ meltfptr[53] = 0 ;
        /*_.LET___V20*/ meltfptr[16] = /*_.LET___V28*/ meltfptr[26];;

        MELT_LOCATION("warmelt-outobj.melt:7482:/ clear");
        /*clear*/ /*_#NUM__L11*/ meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_#STARTERRCOUNT__L12*/ meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NAKEDBASNAM__V21*/ meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REFERR__V22*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REFNORMLIST__V24*/ meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V28*/ meltfptr[26] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7468:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V20*/ meltfptr[16];;

        {
            MELT_LOCATION("warmelt-outobj.melt:7468:/ locexp");
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
        /*clear*/ /*_.IFCPP___V6*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_LIST__L4*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L5*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V9*/ meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V14*/ meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L8*/ meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V15*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V20*/ meltfptr[16] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("TRANSLATE_RUN_MELT_EXPRESSIONS", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_138_WARMELTmiOUTOBJ_TRANSLATE_RUN_MELT_EXPRESSIONS*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9 fromline 1752 */

    /** start of frame for meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9 of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9// fromline 1568
        : public Melt_CallFrameWithValues<5>
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
        MeltFrame_meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<5> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9), clos) {};
        MeltFrame_meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9() //the constructor fromline 1642
            : Melt_CallFrameWithValues<5> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9)) {};
        MeltFrame_meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9


    /** end of frame for meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9 fromline 1697**/

    /* end of frame for routine meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9 fromline 1756 */

    /* classy proc frame meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9 */ MeltFrame_meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9 fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl9", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:7529:/ getarg");
    /*_.V__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7530:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:7530:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:7530:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7530;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "runerrorhdlr v=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.V__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V4*/ meltfptr[3] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V3*/ meltfptr[2] = /*_.MELT_DEBUG_FUN__V4*/ meltfptr[3];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7530:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V4*/ meltfptr[3] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V3*/ meltfptr[2] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7530:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V3*/ meltfptr[2] = 0 ;
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
        /*_#IS_STRING__L3*/ meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.V__V2*/ meltfptr[1])) == MELTOBMAG_STRING);;
        MELT_LOCATION("warmelt-outobj.melt:7532:/ cond");
        /*cond*/ if (/*_#IS_STRING__L3*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:7533:/ locexp");
                        /* translate_run_melt_expressions WARNRUNSTR_CHK__1 */
                        warning (0, "MELT running expression {%s} error - %s",
                                 melt_string_str ((melt_ptr_t) (/*~NAKEDBASNAM*/ meltfclos->tabval[0])),
                                 melt_string_str ((melt_ptr_t) /*_.V__V2*/ meltfptr[1])) ; ;
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:7532:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:7540:/ locexp");
                        /* translate_run_melt_expressions WARNRUN_CHK__1 */
                        warning (0, "MELT running expression {%s} errored",
                                 melt_string_str ((melt_ptr_t) (/*~NAKEDBASNAM*/ meltfclos->tabval[0]))) ; ;
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:7539:/ quasiblock");


                    /*epilog*/
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7546:/ cond");
        /*cond*/ if (/*_.V__V2*/ meltfptr[1]) /*then*/
            {
                /*^cond.then*/
                /*_.OR___V5*/ meltfptr[3] = /*_.V__V2*/ meltfptr[1];;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:7546:/ cond.else");

                /*_.OR___V5*/ meltfptr[3] = (/*!konst_1_TRUE*/ meltfrout->tabval[1]);;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:7546:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*~REFERR*/ meltfclos->tabval[1])),
                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[2])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @REFERENCED_VALUE", melt_magic_discr((melt_ptr_t)((/*~REFERR*/ meltfclos->tabval[1]))) == MELTOBMAG_OBJECT);
                    melt_putfield_object(((/*~REFERR*/ meltfclos->tabval[1])), (0), (/*_.OR___V5*/ meltfptr[3]), "REFERENCED_VALUE");
                    ;
                    /*^touch*/
                    meltgc_touch((/*~REFERR*/ meltfclos->tabval[1]));
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object ((/*~REFERR*/ meltfclos->tabval[1]), "put-fields");
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:7529:/ clear");
        /*clear*/ /*_#IS_STRING__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OR___V5*/ meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl9", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_139_WARMELTmiOUTOBJ_LAMBDA_cl9*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10 fromline 1752 */

    /** start of frame for meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10 of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10// fromline 1568
        : public Melt_CallFrameWithValues<19>
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
        MeltFrame_meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<19> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10), clos) {};
        MeltFrame_meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10() //the constructor fromline 1642
            : Melt_CallFrameWithValues<19> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10)) {};
        MeltFrame_meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<19> (fil,lin, sizeof(MeltFrame_meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10


    /** end of frame for meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10 fromline 1697**/

    /* end of frame for routine meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10 fromline 1756 */

    /* classy proc frame meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10 */ MeltFrame_meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10 fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl10", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:7589:/ getarg");
    /*_.CUREXP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#IX__L1*/ meltfnum[0] = meltxargtab_[0].meltbp_long;
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
        /*_#NULL__L2*/ meltfnum[1] =
            (/*null*/(/*_.CUREXP__V2*/ meltfptr[1]) == NULL);;
        MELT_LOCATION("warmelt-outobj.melt:7591:/ cond");
        /*cond*/ if (/*_#NULL__L2*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-outobj.melt:7592:/ locexp");
                        /*void*/(void)0;
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V3*/ meltfptr[2] = 0 ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:7591:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_A__L3*/ meltfnum[2] =
                        melt_is_instance_of((melt_ptr_t)(/*_.CUREXP__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
                    MELT_LOCATION("warmelt-outobj.melt:7593:/ cond");
                    /*cond*/ if (/*_#IS_A__L3*/ meltfnum[2]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {




                                {
                                    MELT_LOCATION("warmelt-outobj.melt:7594:/ locexp");
                                    meltgc_append_list((melt_ptr_t)((/*~LS*/ meltfclos->tabval[0])), (melt_ptr_t)(/*_.CUREXP__V2*/ meltfptr[1]));
                                }
                                ;
                                /*clear*/ /*_._IFELSE___V4*/ meltfptr[3] = 0 ;
                                /*epilog*/
                            }
                            ;
                        }
                    else
                        {
                            MELT_LOCATION("warmelt-outobj.melt:7593:/ cond.else");

                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                /*_#IS_A__L4*/ meltfnum[3] =
                                    melt_is_instance_of((melt_ptr_t)(/*_.CUREXP__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[1])));;
                                MELT_LOCATION("warmelt-outobj.melt:7595:/ cond");
                                /*cond*/ if (/*_#IS_A__L4*/ meltfnum[3]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {




                                            {
                                                MELT_LOCATION("warmelt-outobj.melt:7596:/ locexp");
                                                meltgc_append_list((melt_ptr_t)((/*~LS*/ meltfclos->tabval[0])), (melt_ptr_t)(/*_.CUREXP__V2*/ meltfptr[1]));
                                            }
                                            ;
                                            /*clear*/ /*_._IFELSE___V5*/ meltfptr[4] = 0 ;
                                            /*epilog*/
                                        }
                                        ;
                                    }
                                else
                                    {
                                        MELT_LOCATION("warmelt-outobj.melt:7595:/ cond.else");

                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*_#IS_STRING__L5*/ meltfnum[4] =
                                                (melt_magic_discr((melt_ptr_t)(/*_.CUREXP__V2*/ meltfptr[1])) == MELTOBMAG_STRING);;
                                            MELT_LOCATION("warmelt-outobj.melt:7597:/ cond");
                                            /*cond*/ if (/*_#IS_STRING__L5*/ meltfnum[4]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {




                                                        {
                                                            MELT_LOCATION("warmelt-outobj.melt:7598:/ locexp");
                                                            meltgc_append_list((melt_ptr_t)((/*~LS*/ meltfclos->tabval[0])), (melt_ptr_t)(/*_.CUREXP__V2*/ meltfptr[1]));
                                                        }
                                                        ;
                                                        /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
                                                        /*epilog*/
                                                    }
                                                    ;
                                                }
                                            else
                                                {
                                                    MELT_LOCATION("warmelt-outobj.melt:7597:/ cond.else");

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        /*_#IS_INTEGERBOX__L6*/ meltfnum[5] =
                                                            (melt_magic_discr((melt_ptr_t)(/*_.CUREXP__V2*/ meltfptr[1])) == MELTOBMAG_INT);;
                                                        MELT_LOCATION("warmelt-outobj.melt:7599:/ cond");
                                                        /*cond*/ if (/*_#IS_INTEGERBOX__L6*/ meltfnum[5]) /*then*/
                                                            {
                                                                /*^cond.then*/
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {




                                                                    {
                                                                        MELT_LOCATION("warmelt-outobj.melt:7600:/ locexp");
                                                                        meltgc_append_list((melt_ptr_t)((/*~LS*/ meltfclos->tabval[0])), (melt_ptr_t)(/*_.CUREXP__V2*/ meltfptr[1]));
                                                                    }
                                                                    ;
                                                                    /*clear*/ /*_._IFELSE___V7*/ meltfptr[6] = 0 ;
                                                                    /*epilog*/
                                                                }
                                                                ;
                                                            }
                                                        else
                                                            {
                                                                MELT_LOCATION("warmelt-outobj.melt:7599:/ cond.else");

                                                                /*^block*/
                                                                /*anyblock*/
                                                                {


                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    /*_#IS_CLOSURE__L7*/ meltfnum[6] =
                                                                        (melt_magic_discr((melt_ptr_t)(/*_.CUREXP__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
                                                                    MELT_LOCATION("warmelt-outobj.melt:7601:/ cond");
                                                                    /*cond*/ if (/*_#IS_CLOSURE__L7*/ meltfnum[6]) /*then*/
                                                                        {
                                                                            /*^cond.then*/
                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {


#if MELT_HAVE_DEBUG
                                                                                MELT_LOCATION("warmelt-outobj.melt:7602:/ cppif.then");
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
                                                                                    MELT_LOCATION("warmelt-outobj.melt:7602:/ cond");
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
                                                                                                MELT_LOCATION("warmelt-outobj.melt:7602:/ apply");
                                                                                                /*apply*/
                                                                                                {
                                                                                                    union meltparam_un argtab[7];
                                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[8];
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[2].meltbp_long = 7602;
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[3].meltbp_cstring =  "translate_run_melt_expressions closure curexp=";
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CUREXP__V2*/ meltfptr[1];
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[5].meltbp_cstring =  " ix=";
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[6].meltbp_long = /*_#IX__L1*/ meltfnum[0];
                                                                                                    /*_.MELT_DEBUG_FUN__V10*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                                                                }
                                                                                                ;
                                                                                                /*_._IF___V9*/ meltfptr[8] = /*_.MELT_DEBUG_FUN__V10*/ meltfptr[9];;
                                                                                                /*epilog*/

                                                                                                MELT_LOCATION("warmelt-outobj.melt:7602:/ clear");
                                                                                                /*clear*/ /*_#MELT_CALLCOUNT__L9*/ meltfnum[8] = 0 ;
                                                                                                /*^clear*/
                                                                                                /*clear*/ /*_.MELT_DEBUG_FUN__V10*/ meltfptr[9] = 0 ;
                                                                                            }
                                                                                            ;
                                                                                        }
                                                                                    else    /*^cond.else*/
                                                                                        {

                                                                                            /*_._IF___V9*/ meltfptr[8] =  /*reallynil*/ NULL ;;
                                                                                        }
                                                                                    ;

                                                                                    {
                                                                                        MELT_LOCATION("warmelt-outobj.melt:7602:/ locexp");
                                                                                        /*void*/(void)0;
                                                                                    }
                                                                                    ;
                                                                                    /*^quasiblock*/


                                                                                    /*epilog*/

                                                                                    /*^clear*/
                                                                                    /*clear*/ /*_#MELT_NEED_DBG__L8*/ meltfnum[7] = 0 ;
                                                                                    /*^clear*/
                                                                                    /*clear*/ /*_._IF___V9*/ meltfptr[8] = 0 ;
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
                                                                                MELT_LOCATION("warmelt-outobj.melt:7603:/ apply");
                                                                                /*apply*/
                                                                                {
                                                                                    /*_.CUREXP__V11*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)(/*_.CUREXP__V2*/ meltfptr[1]), (melt_ptr_t)((/*~LS*/ meltfclos->tabval[0])), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                                                                                }
                                                                                ;
                                                                                MELT_LOCATION("warmelt-outobj.melt:7601:/ quasiblock");


                                                                                /*_.PROGN___V12*/ meltfptr[8] = /*_.CUREXP__V11*/ meltfptr[9];;
                                                                                /*^compute*/
                                                                                /*_._IFELSE___V8*/ meltfptr[7] = /*_.PROGN___V12*/ meltfptr[8];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-outobj.melt:7601:/ clear");
                                                                                /*clear*/ /*_.CUREXP__V11*/ meltfptr[9] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.PROGN___V12*/ meltfptr[8] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    else    /*^cond.else*/
                                                                        {

                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {


#if MELT_HAVE_DEBUG
                                                                                MELT_LOCATION("warmelt-outobj.melt:7605:/ cppif.then");
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
                                                                                    /*_#MELT_NEED_DBG__L10*/ meltfnum[8] =
                                                                                        /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                                                                        (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                                                                        0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                                                                        ;;
                                                                                    MELT_LOCATION("warmelt-outobj.melt:7605:/ cond");
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
                                                                                                MELT_LOCATION("warmelt-outobj.melt:7605:/ apply");
                                                                                                /*apply*/
                                                                                                {
                                                                                                    union meltparam_un argtab[7];
                                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[7];
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[2].meltbp_long = 7605;
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[3].meltbp_cstring =  "translate_run_melt_expressions bad curexp=";
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CUREXP__V2*/ meltfptr[1];
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[5].meltbp_cstring =  " ix=";
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[6].meltbp_long = /*_#IX__L1*/ meltfnum[0];
                                                                                                    /*_.MELT_DEBUG_FUN__V14*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                                                                }
                                                                                                ;
                                                                                                /*_._IF___V13*/ meltfptr[9] = /*_.MELT_DEBUG_FUN__V14*/ meltfptr[8];;
                                                                                                /*epilog*/

                                                                                                MELT_LOCATION("warmelt-outobj.melt:7605:/ clear");
                                                                                                /*clear*/ /*_#MELT_CALLCOUNT__L11*/ meltfnum[7] = 0 ;
                                                                                                /*^clear*/
                                                                                                /*clear*/ /*_.MELT_DEBUG_FUN__V14*/ meltfptr[8] = 0 ;
                                                                                            }
                                                                                            ;
                                                                                        }
                                                                                    else    /*^cond.else*/
                                                                                        {

                                                                                            /*_._IF___V13*/ meltfptr[9] =  /*reallynil*/ NULL ;;
                                                                                        }
                                                                                    ;

                                                                                    {
                                                                                        MELT_LOCATION("warmelt-outobj.melt:7605:/ locexp");
                                                                                        /*void*/(void)0;
                                                                                    }
                                                                                    ;
                                                                                    /*^quasiblock*/


                                                                                    /*epilog*/

                                                                                    /*^clear*/
                                                                                    /*clear*/ /*_#MELT_NEED_DBG__L10*/ meltfnum[8] = 0 ;
                                                                                    /*^clear*/
                                                                                    /*clear*/ /*_._IF___V13*/ meltfptr[9] = 0 ;
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
                                                                                MELT_LOCATION("warmelt-outobj.melt:7606:/ quasiblock");


                                                                                /*_.CURDIS__V16*/ meltfptr[9] =
                                                                                    ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.CUREXP__V2*/ meltfptr[1]))));;
                                                                                MELT_LOCATION("warmelt-outobj.melt:7607:/ cond");
                                                                                /*cond*/ if (
                                                                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CURDIS__V16*/ meltfptr[9]),
                                                                                                                  (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[3])))
                                                                                ) /*then*/
                                                                                    {
                                                                                        /*^cond.then*/
                                                                                        /*^getslot*/
                                                                                        {
                                                                                            melt_ptr_t slot=NULL, obj=NULL;
                                                                                            obj = (melt_ptr_t)(/*_.CURDIS__V16*/ meltfptr[9]) /*=obj*/;
                                                                                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                                                            /*_.CURDISNAME__V17*/ meltfptr[16] = slot;
                                                                                        };
                                                                                        ;
                                                                                    }
                                                                                else    /*^cond.else*/
                                                                                    {

                                                                                        /*_.CURDISNAME__V17*/ meltfptr[16] =  /*reallynil*/ NULL ;;
                                                                                    }
                                                                                ;

                                                                                {
                                                                                    MELT_LOCATION("warmelt-outobj.melt:7609:/ locexp");
                                                                                    /* translate_run_melt_expressions BADEXP_CHK__1 */
                                                                                    warning (0, "MELT run %s expression #%d has bad disciminant %s",
                                                                                             melt_string_str ((melt_ptr_t) (/*~NAKEDBASNAM*/ meltfclos->tabval[1])),
                                                                                             1+(int) /*_#IX__L1*/ meltfnum[0],
                                                                                             melt_string_str ((melt_ptr_t) /*_.CURDISNAME__V17*/ meltfptr[16])) ;
                                                                                    ;
                                                                                }
                                                                                ;

                                                                                MELT_CHECK_SIGNAL();
                                                                                ;
                                                                                MELT_LOCATION("warmelt-outobj.melt:7616:/ apply");
                                                                                /*apply*/
                                                                                {
                                                                                    /*_.RUNERRORHDLR__V18*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*~RUNERRORHDLR*/ meltfclos->tabval[2])), (melt_ptr_t)((/*!konst_4*/ meltfrout->tabval[4])), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                                                                                }
                                                                                ;
                                                                                /*_.LET___V15*/ meltfptr[8] = /*_.RUNERRORHDLR__V18*/ meltfptr[17];;

                                                                                MELT_LOCATION("warmelt-outobj.melt:7606:/ clear");
                                                                                /*clear*/ /*_.CURDIS__V16*/ meltfptr[9] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.CURDISNAME__V17*/ meltfptr[16] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.RUNERRORHDLR__V18*/ meltfptr[17] = 0 ;
                                                                                MELT_LOCATION("warmelt-outobj.melt:7604:/ quasiblock");


                                                                                /*_.PROGN___V19*/ meltfptr[9] = /*_.LET___V15*/ meltfptr[8];;
                                                                                /*^compute*/
                                                                                /*_._IFELSE___V8*/ meltfptr[7] = /*_.PROGN___V19*/ meltfptr[9];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-outobj.melt:7601:/ clear");
                                                                                /*clear*/ /*_.LET___V15*/ meltfptr[8] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.PROGN___V19*/ meltfptr[9] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    ;
                                                                    /*_._IFELSE___V7*/ meltfptr[6] = /*_._IFELSE___V8*/ meltfptr[7];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-outobj.melt:7599:/ clear");
                                                                    /*clear*/ /*_#IS_CLOSURE__L7*/ meltfnum[6] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_._IFELSE___V8*/ meltfptr[7] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        ;
                                                        /*_._IFELSE___V6*/ meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-outobj.melt:7597:/ clear");
                                                        /*clear*/ /*_#IS_INTEGERBOX__L6*/ meltfnum[5] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_._IFELSE___V7*/ meltfptr[6] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;
                                            /*_._IFELSE___V5*/ meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:7595:/ clear");
                                            /*clear*/ /*_#IS_STRING__L5*/ meltfnum[4] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_._IFELSE___V6*/ meltfptr[5] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V4*/ meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:7593:/ clear");
                                /*clear*/ /*_#IS_A__L4*/ meltfnum[3] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V5*/ meltfptr[4] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V3*/ meltfptr[2] = /*_._IFELSE___V4*/ meltfptr[3];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:7591:/ clear");
                    /*clear*/ /*_#IS_A__L3*/ meltfnum[2] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V4*/ meltfptr[3] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:7589:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_._IFELSE___V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-outobj.melt:7589:/ locexp");
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
        /*clear*/ /*_#NULL__L2*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V3*/ meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl10", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_140_WARMELTmiOUTOBJ_LAMBDA_cl10*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11 fromline 1752 */

    /** start of frame for meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11 of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11// fromline 1568
        : public Melt_CallFrameWithValues<62>
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
        MeltFrame_meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<62> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11), clos) {};
        MeltFrame_meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11() //the constructor fromline 1642
            : Melt_CallFrameWithValues<62> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<62> (fil,lin, sizeof(MeltFrame_meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11)) {};
        MeltFrame_meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<62> (fil,lin, sizeof(MeltFrame_meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11


    /** end of frame for meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11 fromline 1697**/

    /* end of frame for routine meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11 fromline 1756 */

    /* classy proc frame meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11 */ MeltFrame_meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11 fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl11", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:7655:/ getarg");
    /*_.NORMLIST__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MODCTX__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.NCX__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.NCX__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.INIENV__V5*/ meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.INIENV__V5*/ meltfptr[4])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7656:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:7656:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:7657:/ cond");
                        /*cond*/ if (
                            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*~REFNORMLIST*/ meltfclos->tabval[0])),
                                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[1])))
                        ) /*then*/
                            {
                                /*^cond.then*/
                                /*^getslot*/
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)((/*~REFNORMLIST*/ meltfclos->tabval[0])) /*=obj*/;
                                    melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                                    /*_.REFERENCED_VALUE__V7*/ meltfptr[6] = slot;
                                };
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_.REFERENCED_VALUE__V7*/ meltfptr[6] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7656:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[9];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7656;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_run_melt_expressions/normalextend normlist=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.NORMLIST__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n old !refnormlist=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.REFERENCED_VALUE__V7*/ meltfptr[6];
                            /*^apply.arg*/
                            argtab[7].meltbp_cstring =  "\n ncx=";
                            /*^apply.arg*/
                            argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.NCX__V4*/ meltfptr[3];
                            /*_.MELT_DEBUG_FUN__V8*/ meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/ meltfptr[5] = /*_.MELT_DEBUG_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7656:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.REFERENCED_VALUE__V7*/ meltfptr[6] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V8*/ meltfptr[7] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7656:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V6*/ meltfptr[5] = 0 ;
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
            MELT_LOCATION("warmelt-outobj.melt:7659:/ locexp");

#if MELT_HAVE_DEBUG
            if (melt_need_debug (0))
                melt_dbgshortbacktrace(( "translate_run_melt_expressions/normalextend"), (13));
#endif
            ;
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7660:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*^cond*/
            /*cond*/ if (
                /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*~REFNORMLIST*/ meltfclos->tabval[0])),
                                              (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[1])))
            ) /*then*/
                {
                    /*^cond.then*/
                    /*^getslot*/
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)((/*~REFNORMLIST*/ meltfclos->tabval[0])) /*=obj*/;
                        melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                        /*_.REFERENCED_VALUE__V10*/ meltfptr[7] = slot;
                    };
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_.REFERENCED_VALUE__V10*/ meltfptr[7] =  /*reallynil*/ NULL ;;
                }
            ;
            /*^compute*/
            /*_#NULL__L3*/ meltfnum[1] =
                (/*null*/(/*_.REFERENCED_VALUE__V10*/ meltfptr[7]) == NULL);;
            MELT_LOCATION("warmelt-outobj.melt:7660:/ cond");
            /*cond*/ if (/*_#NULL__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7660:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check null !refnormlist";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7660;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &(/*~REFNORMLIST*/ meltfclos->tabval[0]);
                            /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V11*/ meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7660:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V9*/ meltfptr[6] = /*_._IFELSE___V11*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7660:/ clear");
            /*clear*/ /*_.REFERENCED_VALUE__V10*/ meltfptr[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_#NULL__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V11*/ meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V9*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:7661:/ quasiblock");


        /*_.LASTNORMPAIR__V14*/ meltfptr[7] =
            (melt_list_last((melt_ptr_t)(/*_.NORMLIST__V2*/ meltfptr[1])));;
        MELT_LOCATION("warmelt-outobj.melt:7662:/ quasiblock");


        /*_.LN__V16*/ meltfptr[15] =
            (melt_pair_head (melt_list_last((melt_ptr_t)(/*_.NORMLIST__V2*/ meltfptr[1]))));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7664:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L4*/ meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7664:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L5*/ meltfnum[1] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7664:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L5*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7664;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_run_melt_expressions/normalextend lastnorm=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.LN__V16*/ meltfptr[15];
                            /*_.MELT_DEBUG_FUN__V18*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V17*/ meltfptr[16] = /*_.MELT_DEBUG_FUN__V18*/ meltfptr[17];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7664:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L5*/ meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V18*/ meltfptr[17] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V17*/ meltfptr[16] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7664:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L4*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V17*/ meltfptr[16] = 0 ;
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
        /*_.LET___V15*/ meltfptr[5] = /*_.LN__V16*/ meltfptr[15];;

        MELT_LOCATION("warmelt-outobj.melt:7662:/ clear");
        /*clear*/ /*_.LN__V16*/ meltfptr[15] = 0 ;
        /*_.LASTNORM__V19*/ meltfptr[17] = /*_.LET___V15*/ meltfptr[5];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7666:/ msend");
        /*msend*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^ojbmsend.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.INIENV__V5*/ meltfptr[4];
            /*_.LASTCTYP__V20*/ meltfptr[16] = meltgc_send((melt_ptr_t)(/*_.LASTNORM__V19*/ meltfptr[17]), (melt_ptr_t)((/*!GET_CTYPE*/ meltfrout->tabval[3])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7668:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L6*/ meltfnum[1] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7668:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L6*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L7*/ meltfnum[0] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7668:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[9];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7668;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_run_melt_expressions/normalextend lastnorm=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.LASTNORM__V19*/ meltfptr[17];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " lastnormpair=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.LASTNORMPAIR__V14*/ meltfptr[7];
                            /*^apply.arg*/
                            argtab[7].meltbp_cstring =  " lastctyp=";
                            /*^apply.arg*/
                            argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.LASTCTYP__V20*/ meltfptr[16];
                            /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V21*/ meltfptr[15] = /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7668:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L7*/ meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V21*/ meltfptr[15] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7668:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L6*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V21*/ meltfptr[15] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7671:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L8*/ meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.LASTCTYP__V20*/ meltfptr[16]), (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[4])));;
            MELT_LOCATION("warmelt-outobj.melt:7671:/ cond");
            /*cond*/ if (/*_#IS_A__L8*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V24*/ meltfptr[15] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7671:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check lastctyp";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7671;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.LASTCTYP__V20*/ meltfptr[16];
                            /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V24*/ meltfptr[15] = /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7671:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V25*/ meltfptr[24] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V23*/ meltfptr[21] = /*_._IFELSE___V24*/ meltfptr[15];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7671:/ clear");
            /*clear*/ /*_#IS_A__L8*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V24*/ meltfptr[15] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V23*/ meltfptr[21] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#eqeq__L9*/ meltfnum[1] =
            ((/*_.LASTCTYP__V20*/ meltfptr[16]) == ((/*!CTYPE_VALUE*/ meltfrout->tabval[5])));;
        MELT_LOCATION("warmelt-outobj.melt:7673:/ cond");
        /*cond*/ if (/*_#eqeq__L9*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:7674:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L10*/ meltfnum[0] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:7674:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L10*/ meltfnum[0]) /*then*/
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
                                    /*^compute*/
                                    /*_.DISCRIM__V28*/ meltfptr[27] =
                                        ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.LASTNORM__V19*/ meltfptr[17]))));;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:7674:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[11];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[10];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 7674;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "translate_run_melt_expressions/normalextend value lastnorm=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.LASTNORM__V19*/ meltfptr[17];
                                        /*^apply.arg*/
                                        argtab[5].meltbp_cstring =  "\n of discrim=";
                                        /*^apply.arg*/
                                        argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.DISCRIM__V28*/ meltfptr[27];
                                        /*^apply.arg*/
                                        argtab[7].meltbp_cstring =  "\n lastnormpair=";
                                        /*^apply.arg*/
                                        argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.LASTNORMPAIR__V14*/ meltfptr[7];
                                        /*^apply.arg*/
                                        argtab[9].meltbp_cstring =  "\n normlist=";
                                        /*^apply.arg*/
                                        argtab[10].meltbp_aptr = (melt_ptr_t*) &/*_.NORMLIST__V2*/ meltfptr[1];
                                        /*_.MELT_DEBUG_FUN__V29*/ meltfptr[28] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V27*/ meltfptr[15] = /*_.MELT_DEBUG_FUN__V29*/ meltfptr[28];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:7674:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L11*/ meltfnum[10] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.DISCRIM__V28*/ meltfptr[27] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V29*/ meltfptr[28] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V27*/ meltfptr[15] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:7674:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L10*/ meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V27*/ meltfptr[15] = 0 ;
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
                    /*_#IS_NOT_A__L12*/ meltfnum[10] =
                        !melt_is_instance_of((melt_ptr_t)(/*_.LASTNORM__V19*/ meltfptr[17]), (melt_ptr_t)((/*!CLASS_NREP_RETURN*/ meltfrout->tabval[6])));;
                    MELT_LOCATION("warmelt-outobj.melt:7677:/ cond");
                    /*cond*/ if (/*_#IS_NOT_A__L12*/ meltfnum[10]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-outobj.melt:7678:/ quasiblock");



                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:7680:/ cond");
                                /*cond*/ if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.LASTNORM__V19*/ meltfptr[17]),
                                                                  (melt_ptr_t)((/*!CLASS_NREP*/ meltfrout->tabval[7])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.LASTNORM__V19*/ meltfptr[17]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 0, "NREP_LOC");
                                            /*_.NREP_LOC__V32*/ meltfptr[15] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.NREP_LOC__V32*/ meltfptr[15] =  /*reallynil*/ NULL ;;
                                    }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:7678:/ quasiblock");


                                /*^rawallocobj*/
                                /*rawallocobj*/
                                {
                                    melt_ptr_t newobj = 0;
                                    melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_NREP_RETURN*/ meltfrout->tabval[6])), (3), "CLASS_NREP_RETURN");
                                    /*_.INST__V34*/ meltfptr[33] =
                                        newobj;
                                };
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @NRET_MAIN", melt_magic_discr((melt_ptr_t)(/*_.INST__V34*/ meltfptr[33])) == MELTOBMAG_OBJECT);
                                melt_putfield_object((/*_.INST__V34*/ meltfptr[33]), (1), (/*_.LASTNORM__V19*/ meltfptr[17]), "NRET_MAIN");
                                ;
                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @NREP_LOC", melt_magic_discr((melt_ptr_t)(/*_.INST__V34*/ meltfptr[33])) == MELTOBMAG_OBJECT);
                                melt_putfield_object((/*_.INST__V34*/ meltfptr[33]), (0), (/*_.NREP_LOC__V32*/ meltfptr[15]), "NREP_LOC");
                                ;
                                /*^touchobj*/

                                melt_dbgtrace_written_object (/*_.INST__V34*/ meltfptr[33], "newly made instance");
                                ;
                                /*_.NRET__V33*/ meltfptr[32] = /*_.INST__V34*/ meltfptr[33];;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:7683:/ locexp");
                                    meltgc_pair_set_head((melt_ptr_t)(/*_.LASTNORMPAIR__V14*/ meltfptr[7]), (/*_.NRET__V33*/ meltfptr[32]));
                                }
                                ;

#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:7684:/ cppif.then");
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
                                    /*_#MELT_NEED_DBG__L13*/ meltfnum[0] =
                                        /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                        (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                        0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                        ;;
                                    MELT_LOCATION("warmelt-outobj.melt:7684:/ cond");
                                    /*cond*/ if (/*_#MELT_NEED_DBG__L13*/ meltfnum[0]) /*then*/
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
                                                MELT_LOCATION("warmelt-outobj.melt:7684:/ apply");
                                                /*apply*/
                                                {
                                                    union meltparam_un argtab[9];
                                                    memset(&argtab, 0, sizeof(argtab));
                                                    /*^apply.arg*/
                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L14*/ meltfnum[13];
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 7684;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_cstring =  "translate_run_melt_expressions/normalextend replaced lastnorm=";
                                                    /*^apply.arg*/
                                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.LASTNORM__V19*/ meltfptr[17];
                                                    /*^apply.arg*/
                                                    argtab[5].meltbp_cstring =  "\n with nret=";
                                                    /*^apply.arg*/
                                                    argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.NRET__V33*/ meltfptr[32];
                                                    /*^apply.arg*/
                                                    argtab[7].meltbp_cstring =  "\n in lastnormpair=";
                                                    /*^apply.arg*/
                                                    argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.LASTNORMPAIR__V14*/ meltfptr[7];
                                                    /*_.MELT_DEBUG_FUN__V36*/ meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IF___V35*/ meltfptr[34] = /*_.MELT_DEBUG_FUN__V36*/ meltfptr[35];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:7684:/ clear");
                                                /*clear*/ /*_#MELT_CALLCOUNT__L14*/ meltfnum[13] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_.MELT_DEBUG_FUN__V36*/ meltfptr[35] = 0 ;
                                            }
                                            ;
                                        }
                                    else    /*^cond.else*/
                                        {

                                            /*_._IF___V35*/ meltfptr[34] =  /*reallynil*/ NULL ;;
                                        }
                                    ;

                                    {
                                        MELT_LOCATION("warmelt-outobj.melt:7684:/ locexp");
                                        /*void*/(void)0;
                                    }
                                    ;
                                    /*^quasiblock*/


                                    /*epilog*/

                                    /*^clear*/
                                    /*clear*/ /*_#MELT_NEED_DBG__L13*/ meltfnum[0] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IF___V35*/ meltfptr[34] = 0 ;
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
                                MELT_LOCATION("warmelt-outobj.melt:7687:/ compute");
                                /*_.LASTNORM__V19*/ meltfptr[17] = /*_.SETQ___V37*/ meltfptr[35] = /*_.NRET__V33*/ meltfptr[32];;
                                /*_.LET___V31*/ meltfptr[28] = /*_.SETQ___V37*/ meltfptr[35];;

                                MELT_LOCATION("warmelt-outobj.melt:7678:/ clear");
                                /*clear*/ /*_.NREP_LOC__V32*/ meltfptr[15] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.NRET__V33*/ meltfptr[32] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.SETQ___V37*/ meltfptr[35] = 0 ;
                                MELT_LOCATION("warmelt-outobj.melt:7677:/ quasiblock");


                                /*_.PROGN___V38*/ meltfptr[34] = /*_.LET___V31*/ meltfptr[28];;
                                /*^compute*/
                                /*_._IF___V30*/ meltfptr[27] = /*_.PROGN___V38*/ meltfptr[34];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:7677:/ clear");
                                /*clear*/ /*_.LET___V31*/ meltfptr[28] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V38*/ meltfptr[34] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IF___V30*/ meltfptr[27] =  /*reallynil*/ NULL ;;
                        }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:7673:/ quasiblock");


                    /*_.PROGN___V39*/ meltfptr[15] = /*_._IF___V30*/ meltfptr[27];;
                    /*^compute*/
                    /*_._IFELSE___V26*/ meltfptr[24] = /*_.PROGN___V39*/ meltfptr[15];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:7673:/ clear");
                    /*clear*/ /*_#IS_NOT_A__L12*/ meltfnum[10] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IF___V30*/ meltfptr[27] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V39*/ meltfptr[15] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:7690:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L15*/ meltfnum[13] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:7690:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L15*/ meltfnum[13]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L16*/ meltfnum[0] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;
                                    /*^compute*/
                                    /*_.DISCRIM__V41*/ meltfptr[35] =
                                        ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.LASTNORM__V19*/ meltfptr[17]))));;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:7690:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[9];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L16*/ meltfnum[0];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 7690;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "translate_run_melt_expressions/normalextend nonvalue lastnorm=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.LASTNORM__V19*/ meltfptr[17];
                                        /*^apply.arg*/
                                        argtab[5].meltbp_cstring =  " of discrim ";
                                        /*^apply.arg*/
                                        argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.DISCRIM__V41*/ meltfptr[35];
                                        /*^apply.arg*/
                                        argtab[7].meltbp_cstring =  "\n lastctyp=";
                                        /*^apply.arg*/
                                        argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.LASTCTYP__V20*/ meltfptr[16];
                                        /*_.MELT_DEBUG_FUN__V42*/ meltfptr[28] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V40*/ meltfptr[32] = /*_.MELT_DEBUG_FUN__V42*/ meltfptr[28];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:7690:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L16*/ meltfnum[0] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.DISCRIM__V41*/ meltfptr[35] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V42*/ meltfptr[28] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V40*/ meltfptr[32] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:7690:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L15*/ meltfnum[13] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V40*/ meltfptr[32] = 0 ;
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
                    MELT_LOCATION("warmelt-outobj.melt:7694:/ cond");
                    /*cond*/ if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.LASTCTYP__V20*/ meltfptr[16]),
                                                      (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[9])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.LASTCTYP__V20*/ meltfptr[16]) /*=obj*/;
                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                /*_.NAMED_NAME__V43*/ meltfptr[34] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.NAMED_NAME__V43*/ meltfptr[34] =  /*reallynil*/ NULL ;;
                        }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:7692:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[2];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "runtime expressions ending with a non-value expression of $1, auto\
-boxing it.";
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.NAMED_NAME__V43*/ meltfptr[34];
                        /*_.WARNING_AT__V44*/ meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!WARNING_AT*/ meltfrout->tabval[8])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:7695:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L17*/ meltfnum[10] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:7695:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L17*/ meltfnum[10]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L18*/ meltfnum[0] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:7695:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L18*/ meltfnum[0];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 7695;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "translate_run_melt_expressions/normalextend before autoboxing lastnorm=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.LASTNORM__V19*/ meltfptr[17];
                                        /*_.MELT_DEBUG_FUN__V46*/ meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V45*/ meltfptr[15] = /*_.MELT_DEBUG_FUN__V46*/ meltfptr[35];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:7695:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L18*/ meltfnum[0] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V46*/ meltfptr[35] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V45*/ meltfptr[15] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:7695:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L17*/ meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V45*/ meltfptr[15] = 0 ;
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
                    MELT_LOCATION("warmelt-outobj.melt:7696:/ quasiblock");


                    /*^multiapply*/
                    /*multiapply 3args, 1x.res*/
                    {
                        union meltparam_un argtab[2];

                        union meltparam_un restab[1];
                        memset(&restab, 0, sizeof(restab));
                        memset(&argtab, 0, sizeof(argtab));
                        /*^multiapply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LASTCTYP__V20*/ meltfptr[16];/*^multiapply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.NCX__V4*/ meltfptr[3];
                        /*^multiapply.xres*/
                        restab[0].meltbp_aptr = (melt_ptr_t*) &/*_.NEWBINDS__V49*/ meltfptr[35];
                        /*^multiapply.appl*/
                        /*_.NEWEXP__V48*/ meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!AUTOBOX_NORMAL_RETURN*/ meltfrout->tabval[10])), (melt_ptr_t)(/*_.LASTNORM__V19*/ meltfptr[17]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, (MELTBPARSTR_PTR ""), restab);
                    }
                    ;
                    /*^quasiblock*/



#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:7699:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L19*/ meltfnum[13] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:7699:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L19*/ meltfnum[13]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L20*/ meltfnum[0] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:7699:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[7];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L20*/ meltfnum[0];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 7699;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "translate_run_melt_expressions/normalextend autoboxed newexp=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.NEWEXP__V48*/ meltfptr[32];
                                        /*^apply.arg*/
                                        argtab[5].meltbp_cstring =  "\n.. newbinds=";
                                        /*^apply.arg*/
                                        argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.NEWBINDS__V49*/ meltfptr[35];
                                        /*_.MELT_DEBUG_FUN__V51*/ meltfptr[50] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V50*/ meltfptr[15] = /*_.MELT_DEBUG_FUN__V51*/ meltfptr[50];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:7699:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L20*/ meltfnum[0] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V51*/ meltfptr[50] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V50*/ meltfptr[15] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:7699:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L19*/ meltfnum[13] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V50*/ meltfptr[15] = 0 ;
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
                    MELT_LOCATION("warmelt-outobj.melt:7701:/ quasiblock");



                    MELT_CHECK_SIGNAL();
                    ;
                    /*^apply*/
                    /*apply*/
                    {
                        union meltparam_un argtab[2];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.NEWBINDS__V49*/ meltfptr[35];
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
                        /*_.NEWRAPLAST__V53*/ meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!WRAP_NORMAL_LET1*/ meltfrout->tabval[11])), (melt_ptr_t)(/*_.NEWEXP__V48*/ meltfptr[32]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:7703:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L21*/ meltfnum[10] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:7703:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L21*/ meltfnum[10]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L22*/ meltfnum[0] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:7703:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L22*/ meltfnum[0];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 7703;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "translate_run_melt_expressions/normalextend newraplast=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.NEWRAPLAST__V53*/ meltfptr[15];
                                        /*_.MELT_DEBUG_FUN__V55*/ meltfptr[54] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V54*/ meltfptr[53] = /*_.MELT_DEBUG_FUN__V55*/ meltfptr[54];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:7703:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L22*/ meltfnum[0] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V55*/ meltfptr[54] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V54*/ meltfptr[53] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:7703:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L21*/ meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V54*/ meltfptr[53] = 0 ;
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
                        MELT_LOCATION("warmelt-outobj.melt:7705:/ locexp");
                        meltgc_pair_set_head((melt_ptr_t)(/*_.LASTNORMPAIR__V14*/ meltfptr[7]), (/*_.NEWRAPLAST__V53*/ meltfptr[15]));
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:7706:/ compute");
                    /*_.LASTNORM__V19*/ meltfptr[17] = /*_.SETQ___V56*/ meltfptr[54] = /*_.NEWRAPLAST__V53*/ meltfptr[15];;
                    /*_.LET___V52*/ meltfptr[50] = /*_.SETQ___V56*/ meltfptr[54];;

                    MELT_LOCATION("warmelt-outobj.melt:7701:/ clear");
                    /*clear*/ /*_.NEWRAPLAST__V53*/ meltfptr[15] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V56*/ meltfptr[54] = 0 ;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_.MULTI___V47*/ meltfptr[28] = /*_.LET___V52*/ meltfptr[50];;

                    MELT_LOCATION("warmelt-outobj.melt:7696:/ clear");
                    /*clear*/ /*_.LET___V52*/ meltfptr[50] = 0 ;

                    /*^clear*/
                    /*clear*/ /*_.NEWBINDS__V49*/ meltfptr[35] = 0 ;
                    MELT_LOCATION("warmelt-outobj.melt:7689:/ quasiblock");


                    /*_.PROGN___V57*/ meltfptr[53] = /*_.MULTI___V47*/ meltfptr[28];;
                    /*^compute*/
                    /*_._IFELSE___V26*/ meltfptr[24] = /*_.PROGN___V57*/ meltfptr[53];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:7673:/ clear");
                    /*clear*/ /*_.NAMED_NAME__V43*/ meltfptr[34] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.WARNING_AT__V44*/ meltfptr[27] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.MULTI___V47*/ meltfptr[28] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V57*/ meltfptr[53] = 0 ;
                }
                ;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7709:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L23*/ meltfnum[13] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7709:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L23*/ meltfnum[13]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L24*/ meltfnum[0] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7709:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[9];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L24*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7709;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_run_melt_expressions/normalextend lastnorm=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.LASTNORM__V19*/ meltfptr[17];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n lastnormpair=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.LASTNORMPAIR__V14*/ meltfptr[7];
                            /*^apply.arg*/
                            argtab[7].meltbp_cstring =  "\n normlist=";
                            /*^apply.arg*/
                            argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.NORMLIST__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V59*/ meltfptr[54] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V58*/ meltfptr[15] = /*_.MELT_DEBUG_FUN__V59*/ meltfptr[54];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7709:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L24*/ meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V59*/ meltfptr[54] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V58*/ meltfptr[15] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7709:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L23*/ meltfnum[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V58*/ meltfptr[15] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7712:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*~REFNORMLIST*/ meltfclos->tabval[0])),
                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[1])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @REFERENCED_VALUE", melt_magic_discr((melt_ptr_t)((/*~REFNORMLIST*/ meltfclos->tabval[0]))) == MELTOBMAG_OBJECT);
                    melt_putfield_object(((/*~REFNORMLIST*/ meltfclos->tabval[0])), (0), (/*_.NORMLIST__V2*/ meltfptr[1]), "REFERENCED_VALUE");
                    ;
                    /*^touch*/
                    meltgc_touch((/*~REFNORMLIST*/ meltfclos->tabval[0]));
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object ((/*~REFNORMLIST*/ meltfclos->tabval[0]), "put-fields");
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7713:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L25*/ meltfnum[10] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7713:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L25*/ meltfnum[10]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L26*/ meltfnum[0] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7713:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L26*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7713;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_run_melt_expressions/normalextend final normlist=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.NORMLIST__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n updated refnormlist=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &(/*~REFNORMLIST*/ meltfclos->tabval[0]);
                            /*_.MELT_DEBUG_FUN__V61*/ meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V60*/ meltfptr[50] = /*_.MELT_DEBUG_FUN__V61*/ meltfptr[32];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7713:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L26*/ meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V61*/ meltfptr[32] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V60*/ meltfptr[50] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7713:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L25*/ meltfnum[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V60*/ meltfptr[50] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7715:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

        {
            MELT_LOCATION("warmelt-outobj.melt:7715:/ locexp");
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
        /*_.LET___V13*/ meltfptr[11] = /*_.RETURN___V62*/ meltfptr[35];;

        MELT_LOCATION("warmelt-outobj.melt:7661:/ clear");
        /*clear*/ /*_.LASTNORMPAIR__V14*/ meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V15*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LASTNORM__V19*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LASTCTYP__V20*/ meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V23*/ meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_#eqeq__L9*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V26*/ meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V62*/ meltfptr[35] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7655:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V13*/ meltfptr[11];;

        {
            MELT_LOCATION("warmelt-outobj.melt:7655:/ locexp");
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
        /*clear*/ /*_.IFCPP___V9*/ meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V13*/ meltfptr[11] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl11", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_141_WARMELTmiOUTOBJ_LAMBDA_cl11*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST fromline 1752 */

    /** start of frame for meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST// fromline 1568
        : public Melt_CallFrameWithValues<108>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[52];
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
        MeltFrame_meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<108> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST), clos) {};
        MeltFrame_meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST() //the constructor fromline 1642
            : Melt_CallFrameWithValues<108> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<108> (fil,lin, sizeof(MeltFrame_meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST)) {};
        MeltFrame_meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<108> (fil,lin, sizeof(MeltFrame_meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST


    /** end of frame for meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST fromline 1697**/

    /* end of frame for routine meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST fromline 1756 */

    /* classy proc frame meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST */ MeltFrame_meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:7777:/ getarg");
    /*_.LISMEXP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.PARMODCTX__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.PARMODCTX__V4*/ meltfptr[3])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7781:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:7781:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:7781:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[11];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7781;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "melt_translate_run_macroexpansions_list";
                            /*^apply.arg*/
                            argtab[4].meltbp_cstring =  " start lismexp=";
                            /*^apply.arg*/
                            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.LISMEXP__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[6].meltbp_cstring =  "\n.. env=";
                            /*^apply.arg*/
                            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*^apply.arg*/
                            argtab[8].meltbp_cstring =  "\n.. parmodctx=";
                            /*^apply.arg*/
                            argtab[9].meltbp_aptr = (melt_ptr_t*) &/*_.PARMODCTX__V4*/ meltfptr[3];
                            /*^apply.arg*/
                            argtab[10].meltbp_cstring =  "\n";
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7781:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0 ;
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
                MELT_LOCATION("warmelt-outobj.melt:7781:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0 ;
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

        {
            MELT_LOCATION("warmelt-outobj.melt:7784:/ locexp");

#if MELT_HAVE_DEBUG
            if (melt_need_debug (0))
                melt_dbgshortbacktrace(( "melt_translate_run_macroexpansions_list"), (20));
#endif
            ;
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7785:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:7785:/ cond");
            /*cond*/ if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7785:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check env";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7785;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7785:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7785:/ clear");
            /*clear*/ /*_#IS_A__L3*/ meltfnum[1] = 0 ;
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
        /*^compute*/
        /*_#NULL__L4*/ meltfnum[0] =
            (/*null*/(/*_.LISMEXP__V2*/ meltfptr[1]) == NULL);;
        MELT_LOCATION("warmelt-outobj.melt:7786:/ cond");
        /*cond*/ if (/*_#NULL__L4*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:7787:/ cppif.then");
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
                        MELT_LOCATION("warmelt-outobj.melt:7787:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L5*/ meltfnum[1]) /*then*/
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
                                    MELT_LOCATION("warmelt-outobj.melt:7787:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[6];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L6*/ meltfnum[5];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 7787;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "melt_translate_run_macroexpansions_list";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_cstring =  " null lismexp=";
                                        /*^apply.arg*/
                                        argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.LISMEXP__V2*/ meltfptr[1];
                                        /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V11*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:7787:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L6*/ meltfnum[5] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V11*/ meltfptr[4] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:7787:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L5*/ meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V11*/ meltfptr[4] = 0 ;
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
                    MELT_LOCATION("warmelt-outobj.melt:7788:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:7788:/ locexp");
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
                    MELT_LOCATION("warmelt-outobj.melt:7786:/ quasiblock");


                    /*_.PROGN___V14*/ meltfptr[4] = /*_.RETURN___V13*/ meltfptr[11];;
                    /*^compute*/
                    /*_._IF___V10*/ meltfptr[8] = /*_.PROGN___V14*/ meltfptr[4];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:7786:/ clear");
                    /*clear*/ /*_.RETURN___V13*/ meltfptr[11] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V14*/ meltfptr[4] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V10*/ meltfptr[8] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_LIST__L7*/ meltfnum[5] =
            (melt_magic_discr((melt_ptr_t)(/*_.LISMEXP__V2*/ meltfptr[1])) == MELTOBMAG_LIST);;
        /*^compute*/
        /*_#NOT__L8*/ meltfnum[1] =
            (!(/*_#IS_LIST__L7*/ meltfnum[5]));;
        MELT_LOCATION("warmelt-outobj.melt:7789:/ cond");
        /*cond*/ if (/*_#NOT__L8*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:7790:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L9*/ meltfnum[8] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:7790:/ cond");
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
                                    MELT_LOCATION("warmelt-outobj.melt:7790:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[6];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L10*/ meltfnum[9];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 7790;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "melt_translate_run_macroexpansions_list";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_cstring =  " bad lismexp=";
                                        /*^apply.arg*/
                                        argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.LISMEXP__V2*/ meltfptr[1];
                                        /*_.MELT_DEBUG_FUN__V17*/ meltfptr[16] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V16*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V17*/ meltfptr[16];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:7790:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L10*/ meltfnum[9] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V17*/ meltfptr[16] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V16*/ meltfptr[4] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:7790:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L9*/ meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V16*/ meltfptr[4] = 0 ;
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
                    MELT_LOCATION("warmelt-outobj.melt:7791:/ cppif.then");
                    /*^block*/
                    /*anyblock*/
                    {


                        MELT_CHECK_SIGNAL();
                        ;
                        /*^cond*/
                        /*cond*/ if ((/*nil*/NULL)) /*then*/
                            {
                                /*^cond.then*/
                                /*_._IFELSE___V19*/ meltfptr[4] = (/*nil*/NULL);;
                            }
                        else
                            {
                                MELT_LOCATION("warmelt-outobj.melt:7791:/ cond.else");

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
                                        argtab[0].meltbp_cstring =  "invalid lismexp";
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 7791;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.LISMEXP__V2*/ meltfptr[1];
                                        /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IFELSE___V19*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:7791:/ clear");
                                    /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V20*/ meltfptr[19] = 0 ;
                                }
                                ;
                            }
                        ;
                        /*_.IFCPP___V18*/ meltfptr[16] = /*_._IFELSE___V19*/ meltfptr[4];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7791:/ clear");
                        /*clear*/ /*_._IFELSE___V19*/ meltfptr[4] = 0 ;
                    }

#else /*MELT_HAVE_DEBUG*/
                    /*^cppif.else*/
                    /*_.IFCPP___V18*/ meltfptr[16] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:7792:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:7792:/ locexp");
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
                    MELT_LOCATION("warmelt-outobj.melt:7789:/ quasiblock");


                    /*_.PROGN___V22*/ meltfptr[4] = /*_.RETURN___V21*/ meltfptr[19];;
                    /*^compute*/
                    /*_._IF___V15*/ meltfptr[11] = /*_.PROGN___V22*/ meltfptr[4];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:7789:/ clear");
                    /*clear*/ /*_.IFCPP___V18*/ meltfptr[16] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V21*/ meltfptr[19] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V22*/ meltfptr[4] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V15*/ meltfptr[11] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:7793:/ quasiblock");


        /*_#NUM__L11*/ meltfnum[9] = 0;;
        /*^compute*/
        /*_#STARTERRCOUNT__L12*/ meltfnum[8] =
            melt_error_counter;;
        /*^compute*/
        /*_.NAKEDBASNAM__V24*/ meltfptr[19] = (/*nil*/NULL);;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7796:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[3])), (1), "CLASS_REFERENCE");
            /*_.INST__V26*/ meltfptr[25] =
                newobj;
        };
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V26*/ meltfptr[25], "newly made instance");
        ;
        /*_.REFERR__V25*/ meltfptr[4] = /*_.INST__V26*/ meltfptr[25];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7798:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[3])), (1), "CLASS_REFERENCE");
            /*_.INST__V28*/ meltfptr[27] =
                newobj;
        };
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V28*/ meltfptr[27], "newly made instance");
        ;
        /*_.REFNORMLIST__V27*/ meltfptr[26] = /*_.INST__V28*/ meltfptr[27];;
        MELT_LOCATION("warmelt-outobj.melt:7799:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PARMODCTX__V4*/ meltfptr[3]),
                                          (melt_ptr_t)((/*!CLASS_ANY_MODULE_CONTEXT*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PARMODCTX__V4*/ meltfptr[3]) /*=obj*/;
                    melt_object_get_field(slot,obj, 1, "MOCX_MODULENAME");
                    /*_.MODNAM__V29*/ meltfptr[28] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.MODNAM__V29*/ meltfptr[28] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:7800:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.PARMODCTX__V4*/ meltfptr[3]),
                                          (melt_ptr_t)((/*!CLASS_ANY_MODULE_CONTEXT*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.PARMODCTX__V4*/ meltfptr[3]) /*=obj*/;
                    melt_object_get_field(slot,obj, 5, "MOCX_MACROENV");
                    /*_.MACROENV__V30*/ meltfptr[29] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.MACROENV__V30*/ meltfptr[29] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7802:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L13*/ meltfnum[12] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7802:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L13*/ meltfnum[12]) /*then*/
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
                        MELT_LOCATION("warmelt-outobj.melt:7802:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[8];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L14*/ meltfnum[13];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7802;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "melt_translate_run_macroexpansions_list";
                            /*^apply.arg*/
                            argtab[4].meltbp_cstring =  " env=";
                            /*^apply.arg*/
                            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*^apply.arg*/
                            argtab[6].meltbp_cstring =  "\n.. macroenv=";
                            /*^apply.arg*/
                            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.MACROENV__V30*/ meltfptr[29];
                            /*_.MELT_DEBUG_FUN__V32*/ meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V31*/ meltfptr[30] = /*_.MELT_DEBUG_FUN__V32*/ meltfptr[31];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7802:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L14*/ meltfnum[13] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V32*/ meltfptr[31] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V31*/ meltfptr[30] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7802:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L13*/ meltfnum[12] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V31*/ meltfptr[30] = 0 ;
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
            MELT_LOCATION("warmelt-outobj.melt:7804:/ locexp");
            /* melt_translate_run_macroexpansions_list NUMCHK__2 */
            char basbuf_NUMCHK__2[64] ;
            long num_NUMCHK__2 = 0 ;
            memset (basbuf_NUMCHK__2, 0, sizeof(basbuf_NUMCHK__2)) ;
            melt_count_runtime_extensions++ ;
            num_NUMCHK__2 = melt_count_runtime_extensions ;
            if (melt_count_runtime_extensions
                    > MELT_MAX_RUNTIME_EXTENSIONS)
                melt_fatal_error ("too many %ld MELT runtime extensions for macroexpansion",
                                  num_NUMCHK__2) ;
            /* In melt_translate_run_macroexpansions_list NUMCHK__2 using getpid to
            ensure unicity of generated naked base name */
            if (melt_flag_bootstrapping && melt_magic_discr(/*_.MODNAM__V29*/ meltfptr[28]) == MELTOBMAG_STRING)
                snprintf (basbuf_NUMCHK__2, sizeof(basbuf_NUMCHK__2),
                          "melt_%s_macro%05lx",
                          melt_string_str(/*_.MODNAM__V29*/ meltfptr[28]), num_NUMCHK__2) ;
            else
                snprintf (basbuf_NUMCHK__2, sizeof(basbuf_NUMCHK__2),
                          "melt_p%d_macro%05lx",
                          (int)getpid(), num_NUMCHK__2) ;
            /*_.NAKEDBASNAM__V24*/ meltfptr[19] =
                meltgc_new_string_raw_len ((meltobject_ptr_t)MELT_PREDEF (DISCR_STRING),
                                           basbuf_NUMCHK__2, -1) ;
            /* end melt_translate_run_macroexpansions_list NUMCHK__2 */
            ;
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7829:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L15*/ meltfnum[13] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7829:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L15*/ meltfnum[13]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L16*/ meltfnum[12] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7829:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[9];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L16*/ meltfnum[12];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7829;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "melt_translate_run_macroexpansions_list nakedbasnam=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.NAKEDBASNAM__V24*/ meltfptr[19];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " num=";
                            /*^apply.arg*/
                            argtab[6].meltbp_long = /*_#NUM__L11*/ meltfnum[9];
                            /*^apply.arg*/
                            argtab[7].meltbp_cstring =  " starterrcount=";
                            /*^apply.arg*/
                            argtab[8].meltbp_long = /*_#STARTERRCOUNT__L12*/ meltfnum[8];
                            /*_.MELT_DEBUG_FUN__V34*/ meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V33*/ meltfptr[31] = /*_.MELT_DEBUG_FUN__V34*/ meltfptr[30];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7829:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L16*/ meltfnum[12] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V34*/ meltfptr[30] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V33*/ meltfptr[31] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7829:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L15*/ meltfnum[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V33*/ meltfptr[31] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7831:/ quasiblock");


        MELT_LOCATION("warmelt-outobj.melt:7833:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V37*/ meltfptr[36] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_7*/ meltfrout->tabval[7])), (2));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V37*/ meltfptr[36])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V37*/ meltfptr[36])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V37*/ meltfptr[36])->tabval[0] = (melt_ptr_t)(/*_.NAKEDBASNAM__V24*/ meltfptr[19]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V37*/ meltfptr[36])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 1>= 0 && 1< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V37*/ meltfptr[36])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V37*/ meltfptr[36])->tabval[1] = (melt_ptr_t)(/*_.REFERR__V25*/ meltfptr[4]);
        ;
        /*_.RUNERRORHDLR__V36*/ meltfptr[31] = /*_.LAMBDA___V37*/ meltfptr[36];;
        MELT_LOCATION("warmelt-outobj.melt:7853:/ blockmultialloc");
        /*multiallocblock*/
        {
            struct meltletrec_1_st
            {
                struct meltpair_st rpair_0___PAIROFLIST_x2;
                struct meltlist_st rlist_1__LIST_;
                long meltletrec_1_endgap;
            } *meltletrec_1_ptr = 0;
            meltletrec_1_ptr = (struct meltletrec_1_st *) meltgc_allocate (sizeof (struct meltletrec_1_st), 0);
            /*^blockmultialloc.initfill*/
            /*inipair rpair_0___PAIROFLIST_x2*/
            /*_._PAIROFLIST__V39*/ meltfptr[38] = (melt_ptr_t) &meltletrec_1_ptr->rpair_0___PAIROFLIST_x2;
            meltletrec_1_ptr->rpair_0___PAIROFLIST_x2.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_PAIR))));

            /*inilist rlist_1__LIST_*/
            /*_.LIST___V40*/ meltfptr[39] = (melt_ptr_t) &meltletrec_1_ptr->rlist_1__LIST_;
            meltletrec_1_ptr->rlist_1__LIST_.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_LIST))));



            /*^putpairhead*/
            /*putpairhead*/
            melt_assertmsg("putpairhead /3 checkpair", melt_magic_discr((melt_ptr_t)(/*_._PAIROFLIST__V39*/ meltfptr[38]))== MELTOBMAG_PAIR);
            ((meltpair_ptr_t)(/*_._PAIROFLIST__V39*/ meltfptr[38]))->hd = (melt_ptr_t) ((/*nil*/NULL));
            ;
            /*^touch*/
            meltgc_touch(/*_._PAIROFLIST__V39*/ meltfptr[38]);
            ;
            /*^putlist*/
            /*putlist*/
            melt_assertmsg("putlist checklist", melt_magic_discr((melt_ptr_t)(/*_.LIST___V40*/ meltfptr[39]))== MELTOBMAG_LIST);
            ((meltlist_ptr_t)(/*_.LIST___V40*/ meltfptr[39]))->first = (meltpair_ptr_t) (/*_._PAIROFLIST__V39*/ meltfptr[38]);
            ((meltlist_ptr_t)(/*_.LIST___V40*/ meltfptr[39]))->last = (meltpair_ptr_t) (/*_._PAIROFLIST__V39*/ meltfptr[38]);
            ;
            /*^touch*/
            meltgc_touch(/*_.LIST___V40*/ meltfptr[39]);
            ;
            /*_.LITVALIST__V38*/ meltfptr[37] = /*_.LIST___V40*/ meltfptr[39];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7853:/ clear");
            /*clear*/ /*_._PAIROFLIST__V39*/ meltfptr[38] = 0 ;
            /*^clear*/
            /*clear*/ /*_.LIST___V40*/ meltfptr[39] = 0 ;
            /*^clear*/
            /*clear*/ /*_._PAIROFLIST__V39*/ meltfptr[38] = 0 ;
            /*^clear*/
            /*clear*/ /*_.LIST___V40*/ meltfptr[39] = 0 ;
        } /*end multiallocblock*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:7855:/ blockmultialloc");
        /*multiallocblock*/
        {
            struct meltletrec_2_st
            {
                struct MELT_MULTIPLE_STRUCT(2) rtup_0__TUPLREC__x12;
                long meltletrec_2_endgap;
            } *meltletrec_2_ptr = 0;
            meltletrec_2_ptr = (struct meltletrec_2_st *) meltgc_allocate (sizeof (struct meltletrec_2_st), 0);
            /*^blockmultialloc.initfill*/
            /*inimult rtup_0__TUPLREC__x12*/
            /*_.TUPLREC___V42*/ meltfptr[39] = (melt_ptr_t) &meltletrec_2_ptr->rtup_0__TUPLREC__x12;
            meltletrec_2_ptr->rtup_0__TUPLREC__x12.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_MULTIPLE))));
            meltletrec_2_ptr->rtup_0__TUPLREC__x12.nbval = 2;


            /*^putuple*/
            /*putupl#34*/
            melt_assertmsg("putupl [:7855] #34 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V42*/ meltfptr[39]))== MELTOBMAG_MULTIPLE);
            melt_assertmsg("putupl [:7855] #34 checkoff", (0>=0 && 0< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V42*/ meltfptr[39]))));
            ((meltmultiple_ptr_t)(/*_.TUPLREC___V42*/ meltfptr[39]))->tabval[0] = (melt_ptr_t)((/*!konst_9_MACROmiEXTENSION*/ meltfrout->tabval[9]));
            ;
            /*^putuple*/
            /*putupl#35*/
            melt_assertmsg("putupl [:7855] #35 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V42*/ meltfptr[39]))== MELTOBMAG_MULTIPLE);
            melt_assertmsg("putupl [:7855] #35 checkoff", (1>=0 && 1< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V42*/ meltfptr[39]))));
            ((meltmultiple_ptr_t)(/*_.TUPLREC___V42*/ meltfptr[39]))->tabval[1] = (melt_ptr_t)(/*_.NAKEDBASNAM__V24*/ meltfptr[19]);
            ;
            /*^touch*/
            meltgc_touch(/*_.TUPLREC___V42*/ meltfptr[39]);
            ;
            /*_.TUPLE___V41*/ meltfptr[38] = /*_.TUPLREC___V42*/ meltfptr[39];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7855:/ clear");
            /*clear*/ /*_.TUPLREC___V42*/ meltfptr[39] = 0 ;
            /*^clear*/
            /*clear*/ /*_.TUPLREC___V42*/ meltfptr[39] = 0 ;
        } /*end multiallocblock*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.TUPLE___V41*/ meltfptr[38];
            /*_.MACROFRESHENV__V43*/ meltfptr[39] =  melt_apply ((meltclosure_ptr_t)((/*!FRESH_ENV*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_.MAKE_MAPSTRING__V44*/ meltfptr[43] =
            (meltgc_new_mapstrings( (meltobject_ptr_t) ((/*!DISCR_MAP_STRINGS*/ meltfrout->tabval[11])), (71)));;
        /*^compute*/
        /*_.MAKE_MAPSTRING__V45*/ meltfptr[44] =
            (meltgc_new_mapstrings( (meltobject_ptr_t) ((/*!DISCR_MAP_STRINGS*/ meltfrout->tabval[11])), (19)));;
        /*^compute*/
        /*_.MAKE_INTEGERBOX__V46*/ meltfptr[45] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[12])), (0)));;
        /*^compute*/
        /*_.MAKE_LIST__V47*/ meltfptr[46] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[13]))));;
        /*^compute*/
        /*_.MAKE_LIST__V48*/ meltfptr[47] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[13]))));;
        /*^compute*/
        /*_.MAKE_INTEGERBOX__V49*/ meltfptr[48] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[12])), (0)));;
        /*^compute*/
        /*_.MAKE_LIST__V50*/ meltfptr[49] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[13]))));;
        /*^compute*/
        /*_.MAKE_MAPSTRING__V51*/ meltfptr[50] =
            (meltgc_new_mapstrings( (meltobject_ptr_t) ((/*!DISCR_MAP_STRINGS*/ meltfrout->tabval[11])), (31)));;
        /*^compute*/
        /*_.MAKE_LIST__V52*/ meltfptr[51] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[13]))));;
        /*^compute*/
        /*_.MAKE_LIST__V53*/ meltfptr[52] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[13]))));;
        /*^compute*/
        /*_.MAKE_LIST__V54*/ meltfptr[53] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[13]))));;
        /*^compute*/
        /*_.MAKE_INTEGERBOX__V55*/ meltfptr[54] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_INTEGER*/ meltfrout->tabval[12])), (1)));;
        /*^compute*/
        /*_.MAKE_MAPOBJECT__V56*/ meltfptr[55] =
            (meltgc_new_mapobjects( (meltobject_ptr_t) ((/*!DISCR_MAP_OBJECTS*/ meltfrout->tabval[14])), (53)));;
        MELT_LOCATION("warmelt-outobj.melt:7857:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_RUNNING_EXTENSION_MODULE_CONTEXT*/ meltfrout->tabval[10])), (25), "CLASS_RUNNING_EXTENSION_MODULE_CONTEXT");
            /*_.INST__V58*/ meltfptr[57] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_MODULENAME", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (1), (/*_.NAKEDBASNAM__V24*/ meltfptr[19]), "MOCX_MODULENAME");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_EXPFIELDICT", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (2), (/*_.MAKE_MAPSTRING__V44*/ meltfptr[43]), "MOCX_EXPFIELDICT");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_EXPCLASSDICT", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (3), (/*_.MAKE_MAPSTRING__V45*/ meltfptr[44]), "MOCX_EXPCLASSDICT");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_INITIALENV", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (4), (/*_.ENV__V3*/ meltfptr[2]), "MOCX_INITIALENV");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_BASENV", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (6), (/*_.ENV__V3*/ meltfptr[2]), "MOCX_BASENV");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_MACROENV", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (5), (/*_.MACROFRESHENV__V43*/ meltfptr[39]), "MOCX_MACROENV");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_FUNCOUNT", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (7), (/*_.MAKE_INTEGERBOX__V46*/ meltfptr[45]), "MOCX_FUNCOUNT");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_FILETUPLE", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (8), ((/*nil*/NULL)), "MOCX_FILETUPLE");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_CHEADERLIST", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (11), (/*_.MAKE_LIST__V47*/ meltfptr[46]), "MOCX_CHEADERLIST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_CIMPLEMENTLIST", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (12), (/*_.MAKE_LIST__V48*/ meltfptr[47]), "MOCX_CIMPLEMENTLIST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_VARCOUNT", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (17), (/*_.MAKE_INTEGERBOX__V49*/ meltfptr[48]), "MOCX_VARCOUNT");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_VARLIST", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (18), (/*_.MAKE_LIST__V50*/ meltfptr[49]), "MOCX_VARLIST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_HOOKDICT", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (19), (/*_.MAKE_MAPSTRING__V51*/ meltfptr[50]), "MOCX_HOOKDICT");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_MACROLIST", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (20), (/*_.MAKE_LIST__V52*/ meltfptr[51]), "MOCX_MACROLIST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_ISGPLCOMPATIBLE", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (21), ((/*nil*/NULL)), "MOCX_ISGPLCOMPATIBLE");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_PACKAGEPCLIST", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (15), (/*_.MAKE_LIST__V53*/ meltfptr[52]), "MOCX_PACKAGEPCLIST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_GENDEVLIST", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (14), (/*_.MAKE_LIST__V54*/ meltfptr[53]), "MOCX_GENDEVLIST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MOCX_ERRORHANDLER", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (16), (/*_.RUNERRORHDLR__V36*/ meltfptr[31]), "MOCX_ERRORHANDLER");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MORCX_LITERVALIST", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (22), (/*_.LITVALIST__V38*/ meltfptr[37]), "MORCX_LITERVALIST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MORCX_COUNTLITVAL", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (24), (/*_.MAKE_INTEGERBOX__V55*/ meltfptr[54]), "MORCX_COUNTLITVAL");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @MORCX_LITEROBJMAP", melt_magic_discr((melt_ptr_t)(/*_.INST__V58*/ meltfptr[57])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V58*/ meltfptr[57]), (23), (/*_.MAKE_MAPOBJECT__V56*/ meltfptr[55]), "MORCX_LITEROBJMAP");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V58*/ meltfptr[57], "newly made instance");
        ;
        /*_.NEWMODCTX__V57*/ meltfptr[56] = /*_.INST__V58*/ meltfptr[57];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7883:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*_.NCX__V59*/ meltfptr[58] =  melt_apply ((meltclosure_ptr_t)((/*!CREATE_NORMAL_EXTENDING_CONTEXT*/ meltfrout->tabval[15])), (melt_ptr_t)(/*_.NEWMODCTX__V57*/ meltfptr[56]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_#LENXLIST__L17*/ meltfnum[12] =
            (melt_list_length((melt_ptr_t)(/*_.LISMEXP__V2*/ meltfptr[1])));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7887:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L18*/ meltfnum[13] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7887:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L18*/ meltfnum[13]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L19*/ meltfnum[18] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7887:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[8];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L19*/ meltfnum[18];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7887;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "melt_translate_run_macroexpansions_list here lismexp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.LISMEXP__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n.. macroenv=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &(/*!DEBUG_LESS*/ meltfrout->tabval[16]);
                            /*^apply.arg*/
                            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.MACROENV__V30*/ meltfptr[29];
                            /*_.MELT_DEBUG_FUN__V61*/ meltfptr[60] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V60*/ meltfptr[59] = /*_.MELT_DEBUG_FUN__V61*/ meltfptr[60];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7887:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L19*/ meltfnum[18] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V61*/ meltfptr[60] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V60*/ meltfptr[59] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7887:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L18*/ meltfnum[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V60*/ meltfptr[59] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7889:/ quasiblock");


        /*_.BASENAME__V63*/ meltfptr[59] =
            (meltgc_new_string_tempname_suffixed((meltobject_ptr_t)((/*!DISCR_STRING*/ meltfrout->tabval[17])),
                    melt_string_str((melt_ptr_t)(/*_.NAKEDBASNAM__V24*/ meltfptr[19])),
                    ( "_eXt")));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7891:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L20*/ meltfnum[18] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7891:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L20*/ meltfnum[18]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L21*/ meltfnum[13] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7891:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L21*/ meltfnum[13];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7891;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "melt_translate_run_macroexpansions_list basename=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.BASENAME__V63*/ meltfptr[59];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n.. refnormlist=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.REFNORMLIST__V27*/ meltfptr[26];
                            /*_.MELT_DEBUG_FUN__V65*/ meltfptr[64] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V64*/ meltfptr[63] = /*_.MELT_DEBUG_FUN__V65*/ meltfptr[64];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7891:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L21*/ meltfnum[13] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V65*/ meltfptr[64] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V64*/ meltfptr[63] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7891:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L20*/ meltfnum[18] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V64*/ meltfptr[63] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7894:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[6];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.BASENAME__V63*/ meltfptr[59];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.NEWMODCTX__V57*/ meltfptr[56];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.NCX__V59*/ meltfptr[58];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MACROENV__V30*/ meltfptr[29];
            /*^apply.arg*/
            argtab[4].meltbp_aptr = /*nil*/(melt_ptr_t*)NULL;
            /*^apply.arg*/
            argtab[5].meltbp_aptr = (melt_ptr_t*) &(/*!COMPILE2OBJ_INITEXTENDPROC*/ meltfrout->tabval[19]);
            /*_.TRANSLATE_MACROEXPANDED_LIST__V66*/ meltfptr[64] =  melt_apply ((meltclosure_ptr_t)((/*!TRANSLATE_MACROEXPANDED_LIST*/ meltfrout->tabval[18])), (melt_ptr_t)(/*_.LISMEXP__V2*/ meltfptr[1]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7896:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L22*/ meltfnum[13] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7896:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L22*/ meltfnum[13]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L23*/ meltfnum[18] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7896:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[8];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L23*/ meltfnum[18];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7896;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "melt_translate_run_macroexpansions_list after translation";
                            /*^apply.arg*/
                            argtab[4].meltbp_cstring =  " refnormlist=";
                            /*^apply.arg*/
                            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.REFNORMLIST__V27*/ meltfptr[26];
                            /*^apply.arg*/
                            argtab[6].meltbp_cstring =  " to nakedbasnam=";
                            /*^apply.arg*/
                            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.NAKEDBASNAM__V24*/ meltfptr[19];
                            /*_.MELT_DEBUG_FUN__V68*/ meltfptr[67] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V67*/ meltfptr[63] = /*_.MELT_DEBUG_FUN__V68*/ meltfptr[67];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7896:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L23*/ meltfnum[18] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V68*/ meltfptr[67] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V67*/ meltfptr[63] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7896:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L22*/ meltfnum[13] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V67*/ meltfptr[63] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7899:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.REFERR__V25*/ meltfptr[4]),
                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[3])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.REFERR__V25*/ meltfptr[4]) /*=obj*/;
                    melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                    /*_.REFERENCED_VALUE__V69*/ meltfptr[67] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.REFERENCED_VALUE__V69*/ meltfptr[67] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#NOTNULL__L24*/ meltfnum[18] =
            ((/*_.REFERENCED_VALUE__V69*/ meltfptr[67]) != NULL);;
        MELT_LOCATION("warmelt-outobj.melt:7899:/ cond");
        /*cond*/ if (/*_#NOTNULL__L24*/ meltfnum[18]) /*then*/
            {
                /*^cond.then*/
                /*_#OR___L25*/ meltfnum[13] = /*_#NOTNULL__L24*/ meltfnum[18];;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:7899:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    /*_#MELT_ERROR_COUNTER__L26*/ meltfnum[25] =
                        melt_error_counter;;
                    /*^compute*/
                    /*_#gtI__L27*/ meltfnum[26] =
                        ((/*_#MELT_ERROR_COUNTER__L26*/ meltfnum[25]) > (/*_#STARTERRCOUNT__L12*/ meltfnum[8]));;
                    /*^compute*/
                    /*_#OR___L25*/ meltfnum[13] = /*_#gtI__L27*/ meltfnum[26];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:7899:/ clear");
                    /*clear*/ /*_#MELT_ERROR_COUNTER__L26*/ meltfnum[25] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#gtI__L27*/ meltfnum[26] = 0 ;
                }
                ;
            }
        ;
        /*^cond*/
        /*cond*/ if (/*_#OR___L25*/ meltfnum[13]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:7900:/ apply");
                    /*apply*/
                    {
                        /*_.RUNERRORHDLR__V71*/ meltfptr[70] =  melt_apply ((meltclosure_ptr_t)(/*_.RUNERRORHDLR__V36*/ meltfptr[31]), (melt_ptr_t)((/*!konst_20*/ meltfrout->tabval[20])), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:7901:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] = (/*nil*/NULL);;
                    MELT_LOCATION("warmelt-outobj.melt:7901:/ putxtraresult");
                    if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                    if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                    if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                    ;
                    /*^finalreturn*/
                    ;
                    /*finalret*/ goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-outobj.melt:7899:/ quasiblock");


                    /*_.PROGN___V73*/ meltfptr[72] = /*_.RETURN___V72*/ meltfptr[71];;
                    /*^compute*/
                    /*_._IF___V70*/ meltfptr[63] = /*_.PROGN___V73*/ meltfptr[72];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:7899:/ clear");
                    /*clear*/ /*_.RUNERRORHDLR__V71*/ meltfptr[70] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V72*/ meltfptr[71] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V73*/ meltfptr[72] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V70*/ meltfptr[63] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7902:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L28*/ meltfnum[25] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7902:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L28*/ meltfnum[25]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L29*/ meltfnum[26] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7902:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[9];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L29*/ meltfnum[26];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7902;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "melt_translate_run_macroexpansions_list before compiling runextend\
 flavor of basename=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.BASENAME__V63*/ meltfptr[59];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n.. newmodctx=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.NEWMODCTX__V57*/ meltfptr[56];
                            /*^apply.arg*/
                            argtab[7].meltbp_cstring =  "\n.. parmodctx=";
                            /*^apply.arg*/
                            argtab[8].meltbp_aptr = (melt_ptr_t*) &/*_.PARMODCTX__V4*/ meltfptr[3];
                            /*_.MELT_DEBUG_FUN__V75*/ meltfptr[71] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V74*/ meltfptr[70] = /*_.MELT_DEBUG_FUN__V75*/ meltfptr[71];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7902:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L29*/ meltfnum[26] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V75*/ meltfptr[71] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V74*/ meltfptr[70] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7902:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L28*/ meltfnum[25] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V74*/ meltfptr[70] = 0 ;
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
            MELT_LOCATION("warmelt-outobj.melt:7903:/ locexp");
            /*generate_flavored_melt_module*/ melt_compile_source (melt_string_str ((melt_ptr_t)/*_.BASENAME__V63*/ meltfptr[59]),
                    melt_string_str ((melt_ptr_t)/*_.BASENAME__V63*/ meltfptr[59]),
                    NULL,
                    melt_string_str ((melt_ptr_t)(/*!konst_21*/ meltfrout->tabval[21]))); /*generate_flavored_melt_module*/
            ;
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7904:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L30*/ meltfnum[26] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7904:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L30*/ meltfnum[26]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L31*/ meltfnum[25] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7904:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L31*/ meltfnum[25];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7904;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "melt_translate_run_macroexpansions_list after compiling runextend flavor\
 of basename=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.BASENAME__V63*/ meltfptr[59];
                            /*_.MELT_DEBUG_FUN__V77*/ meltfptr[71] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V76*/ meltfptr[72] = /*_.MELT_DEBUG_FUN__V77*/ meltfptr[71];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7904:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L31*/ meltfnum[25] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V77*/ meltfptr[71] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V76*/ meltfptr[72] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7904:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L30*/ meltfnum[26] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V76*/ meltfptr[72] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7906:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_LIST__L32*/ meltfnum[25] =
                (melt_magic_discr((melt_ptr_t)(/*_.LITVALIST__V38*/ meltfptr[37])) == MELTOBMAG_LIST);;
            MELT_LOCATION("warmelt-outobj.melt:7906:/ cond");
            /*cond*/ if (/*_#IS_LIST__L32*/ meltfnum[25]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V79*/ meltfptr[71] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7906:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check litvalist";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7906;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.LITVALIST__V38*/ meltfptr[37];
                            /*_.MELT_ASSERT_FAILURE_FUN__V80*/ meltfptr[72] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V79*/ meltfptr[71] = /*_.MELT_ASSERT_FAILURE_FUN__V80*/ meltfptr[72];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7906:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V80*/ meltfptr[72] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V78*/ meltfptr[70] = /*_._IFELSE___V79*/ meltfptr[71];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7906:/ clear");
            /*clear*/ /*_#IS_LIST__L32*/ meltfnum[25] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V79*/ meltfptr[71] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V78*/ meltfptr[70] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7907:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L33*/ meltfnum[26] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7907:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L33*/ meltfnum[26]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L34*/ meltfnum[25] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7907:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L34*/ meltfnum[25];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7907;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "melt_translate_run_macroexpansions_list after generation litvalist=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.LITVALIST__V38*/ meltfptr[37];
                            /*_.MELT_DEBUG_FUN__V82*/ meltfptr[71] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V81*/ meltfptr[72] = /*_.MELT_DEBUG_FUN__V82*/ meltfptr[71];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7907:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L34*/ meltfnum[25] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V82*/ meltfptr[71] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V81*/ meltfptr[72] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7907:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L33*/ meltfnum[26] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V81*/ meltfptr[72] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7908:/ quasiblock");


        /*_#NBLITVAL__L35*/ meltfnum[25] =
            (melt_list_length((melt_ptr_t)(/*_.LITVALIST__V38*/ meltfptr[37])));;
        /*^compute*/
        /*_.TUPLITVAL__V84*/ meltfptr[72] =
            (meltgc_new_multiple((meltobject_ptr_t)((/*!DISCR_MULTIPLE*/ meltfrout->tabval[22])), (/*_#NBLITVAL__L35*/ meltfnum[25])));;
        /*^compute*/
        /*_#IX__L36*/ meltfnum[26] = 0;;
        MELT_LOCATION("warmelt-outobj.melt:7911:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.NEWMODCTX__V57*/ meltfptr[56]),
                                          (melt_ptr_t)((/*!CLASS_ANY_MODULE_CONTEXT*/ meltfrout->tabval[4])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.NEWMODCTX__V57*/ meltfptr[56]) /*=obj*/;
                    melt_object_get_field(slot,obj, 8, "MOCX_FILETUPLE");
                    /*_.FILTUP__V85*/ meltfptr[84] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.FILTUP__V85*/ meltfptr[84] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#NBFILE__L37*/ meltfnum[36] =
            (melt_multiple_length((melt_ptr_t)(/*_.FILTUP__V85*/ meltfptr[84])));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7914:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L38*/ meltfnum[37] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7914:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L38*/ meltfnum[37]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L39*/ meltfnum[38] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7914:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L39*/ meltfnum[38];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7914;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "melt_translate_run_macroexpansions_list filtup=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.FILTUP__V85*/ meltfptr[84];
                            /*_.MELT_DEBUG_FUN__V87*/ meltfptr[86] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V86*/ meltfptr[85] = /*_.MELT_DEBUG_FUN__V87*/ meltfptr[86];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7914:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L39*/ meltfnum[38] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V87*/ meltfptr[86] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V86*/ meltfptr[85] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7914:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L38*/ meltfnum[37] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V86*/ meltfptr[85] = 0 ;
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
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit1__EACHLIST */
            for (/*_.CURPAIR__V88*/ meltfptr[86] = melt_list_first( (melt_ptr_t)/*_.LITVALIST__V38*/ meltfptr[37]);
                                    melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V88*/ meltfptr[86]) == MELTOBMAG_PAIR;
                                    /*_.CURPAIR__V88*/ meltfptr[86] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V88*/ meltfptr[86]))
                {
                    /*_.CURLITVAL__V89*/ meltfptr[85] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V88*/ meltfptr[86]);



#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:7918:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L40*/ meltfnum[38] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:7918:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L40*/ meltfnum[38]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L41*/ meltfnum[37] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:7918:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[7];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L41*/ meltfnum[37];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 7918;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "melt_translate_run_macroexpansions_list ix#";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_long = /*_#IX__L36*/ meltfnum[26];
                                        /*^apply.arg*/
                                        argtab[5].meltbp_cstring =  " curlitval=";
                                        /*^apply.arg*/
                                        argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.CURLITVAL__V89*/ meltfptr[85];
                                        /*_.MELT_DEBUG_FUN__V91*/ meltfptr[90] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V90*/ meltfptr[89] = /*_.MELT_DEBUG_FUN__V91*/ meltfptr[90];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:7918:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L41*/ meltfnum[37] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V91*/ meltfptr[90] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V90*/ meltfptr[89] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:7918:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L40*/ meltfnum[38] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V90*/ meltfptr[89] = 0 ;
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
                    MELT_LOCATION("warmelt-outobj.melt:7919:/ cond");
                    /*cond*/ if (/*_.CURLITVAL__V89*/ meltfptr[85]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:7920:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*_#IS_A__L42*/ meltfnum[37] =
                                        melt_is_instance_of((melt_ptr_t)(/*_.CURLITVAL__V89*/ meltfptr[85]), (melt_ptr_t)((/*!CLASS_LITERAL_VALUE*/ meltfrout->tabval[23])));;
                                    MELT_LOCATION("warmelt-outobj.melt:7920:/ cond");
                                    /*cond*/ if (/*_#IS_A__L42*/ meltfnum[37]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*_._IFELSE___V93*/ meltfptr[89] = (/*nil*/NULL);;
                                        }
                                    else
                                        {
                                            MELT_LOCATION("warmelt-outobj.melt:7920:/ cond.else");

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
                                                    argtab[0].meltbp_cstring =  "check curlitval";
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 7920;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CURLITVAL__V89*/ meltfptr[85];
                                                    /*_.MELT_ASSERT_FAILURE_FUN__V94*/ meltfptr[93] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IFELSE___V93*/ meltfptr[89] = /*_.MELT_ASSERT_FAILURE_FUN__V94*/ meltfptr[93];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:7920:/ clear");
                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V94*/ meltfptr[93] = 0 ;
                                            }
                                            ;
                                        }
                                    ;
                                    /*_.IFCPP___V92*/ meltfptr[90] = /*_._IFELSE___V93*/ meltfptr[89];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:7920:/ clear");
                                    /*clear*/ /*_#IS_A__L42*/ meltfnum[37] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IFELSE___V93*/ meltfptr[89] = 0 ;
                                }

#else /*MELT_HAVE_DEBUG*/
                                /*^cppif.else*/
                                /*_.IFCPP___V92*/ meltfptr[90] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:7921:/ quasiblock");


                                /*^cond*/
                                /*cond*/ if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CURLITVAL__V89*/ meltfptr[85]),
                                                                  (melt_ptr_t)((/*!CLASS_LITERAL_VALUE*/ meltfrout->tabval[23])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CURLITVAL__V89*/ meltfptr[85]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "LITV_VALUE");
                                            /*_.VAL__V95*/ meltfptr[93] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.VAL__V95*/ meltfptr[93] =  /*reallynil*/ NULL ;;
                                    }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:7922:/ cond");
                                /*cond*/ if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CURLITVAL__V89*/ meltfptr[85]),
                                                                  (melt_ptr_t)((/*!CLASS_LITERAL_VALUE*/ meltfrout->tabval[23])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CURLITVAL__V89*/ meltfptr[85]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 2, "LITV_RANK");
                                            /*_.BXRK__V96*/ meltfptr[89] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.BXRK__V96*/ meltfptr[89] =  /*reallynil*/ NULL ;;
                                    }
                                ;

#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-outobj.melt:7924:/ cppif.then");
                                /*^block*/
                                /*anyblock*/
                                {


                                    MELT_CHECK_SIGNAL();
                                    ;
                                    /*_#GET_INT__L43*/ meltfnum[38] =
                                        (melt_get_int((melt_ptr_t)(/*_.BXRK__V96*/ meltfptr[89])));;
                                    /*^compute*/
                                    /*_#eqeqI__L44*/ meltfnum[37] =
                                        ((/*_#GET_INT__L43*/ meltfnum[38]) == (/*_#IX__L36*/ meltfnum[26]));;
                                    MELT_LOCATION("warmelt-outobj.melt:7924:/ cond");
                                    /*cond*/ if (/*_#eqeqI__L44*/ meltfnum[37]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*_._IFELSE___V98*/ meltfptr[97] = (/*nil*/NULL);;
                                        }
                                    else
                                        {
                                            MELT_LOCATION("warmelt-outobj.melt:7924:/ cond.else");

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
                                                    argtab[0].meltbp_cstring =  "check bxrk";
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 7924;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.BXRK__V96*/ meltfptr[89];
                                                    /*_.MELT_ASSERT_FAILURE_FUN__V99*/ meltfptr[98] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IFELSE___V98*/ meltfptr[97] = /*_.MELT_ASSERT_FAILURE_FUN__V99*/ meltfptr[98];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:7924:/ clear");
                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V99*/ meltfptr[98] = 0 ;
                                            }
                                            ;
                                        }
                                    ;
                                    /*_.IFCPP___V97*/ meltfptr[96] = /*_._IFELSE___V98*/ meltfptr[97];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:7924:/ clear");
                                    /*clear*/ /*_#GET_INT__L43*/ meltfnum[38] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#eqeqI__L44*/ meltfnum[37] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IFELSE___V98*/ meltfptr[97] = 0 ;
                                }

#else /*MELT_HAVE_DEBUG*/
                                /*^cppif.else*/
                                /*_.IFCPP___V97*/ meltfptr[96] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                ;

                                {
                                    MELT_LOCATION("warmelt-outobj.melt:7925:/ locexp");
                                    meltgc_multiple_put_nth((melt_ptr_t)(/*_.TUPLITVAL__V84*/ meltfptr[72]), (/*_#IX__L36*/ meltfnum[26]), (melt_ptr_t)(/*_.VAL__V95*/ meltfptr[93]));
                                }
                                ;

                                MELT_LOCATION("warmelt-outobj.melt:7921:/ clear");
                                /*clear*/ /*_.VAL__V95*/ meltfptr[93] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.BXRK__V96*/ meltfptr[89] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.IFCPP___V97*/ meltfptr[96] = 0 ;
                                MELT_LOCATION("warmelt-outobj.melt:7919:/ quasiblock");


                                /*epilog*/

                                /*^clear*/
                                /*clear*/ /*_.IFCPP___V92*/ meltfptr[90] = 0 ;
                            }
                            ;
                        } /*noelse*/
                    ;
                    /*_#plI__L45*/ meltfnum[38] =
                        ((/*_#IX__L36*/ meltfnum[26]) + (1));;
                    MELT_LOCATION("warmelt-outobj.melt:7927:/ compute");
                    /*_#IX__L36*/ meltfnum[26] = /*_#SETQ___L46*/ meltfnum[37] = /*_#plI__L45*/ meltfnum[38];;
                } /* end foreach_pair_component_in_list meltcit1__EACHLIST */
            /*_.CURPAIR__V88*/ meltfptr[86] = NULL;
            /*_.CURLITVAL__V89*/ meltfptr[85] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-outobj.melt:7915:/ clear");
            /*clear*/ /*_.CURPAIR__V88*/ meltfptr[86] = 0 ;
            /*^clear*/
            /*clear*/ /*_.CURLITVAL__V89*/ meltfptr[85] = 0 ;
            /*^clear*/
            /*clear*/ /*_#plI__L45*/ meltfnum[38] = 0 ;
            /*^clear*/
            /*clear*/ /*_#SETQ___L46*/ meltfnum[37] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7928:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L47*/ meltfnum[46] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7928:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L47*/ meltfnum[46]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L48*/ meltfnum[47] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7928:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L48*/ meltfnum[47];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7928;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "melt_translate_run_macroexpansions_list tuplitval=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.TUPLITVAL__V84*/ meltfptr[72];
                            /*_.MELT_DEBUG_FUN__V101*/ meltfptr[97] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V100*/ meltfptr[98] = /*_.MELT_DEBUG_FUN__V101*/ meltfptr[97];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7928:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L48*/ meltfnum[47] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V101*/ meltfptr[97] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V100*/ meltfptr[98] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7928:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L47*/ meltfnum[46] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V100*/ meltfptr[98] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7929:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L49*/ meltfnum[47] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7929:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L49*/ meltfnum[47]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L50*/ meltfnum[46] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7929:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[10];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L50*/ meltfnum[46];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7929;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "melt_translate_run_macroexpansions_list before melt_run_extension basename=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.BASENAME__V63*/ meltfptr[59];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n* env=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &(/*!DEBUG_MORE*/ meltfrout->tabval[24]);
                            /*^apply.arg*/
                            argtab[7].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*^apply.arg*/
                            argtab[8].meltbp_cstring =  "\n* tuplitval=";
                            /*^apply.arg*/
                            argtab[9].meltbp_aptr = (melt_ptr_t*) &/*_.TUPLITVAL__V84*/ meltfptr[72];
                            /*_.MELT_DEBUG_FUN__V103*/ meltfptr[89] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V102*/ meltfptr[93] = /*_.MELT_DEBUG_FUN__V103*/ meltfptr[89];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7929:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L50*/ meltfnum[46] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V103*/ meltfptr[89] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V102*/ meltfptr[93] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7929:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L49*/ meltfnum[47] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V102*/ meltfptr[93] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7932:/ quasiblock");


        /*_.RESULT__V105*/ meltfptr[90] =
            /* melt_run_extension */ meltgc_run_cc_extension
            ((melt_ptr_t) /*_.BASENAME__V63*/ meltfptr[59],
             (melt_ptr_t) /*_.ENV__V3*/ meltfptr[2],
             (melt_ptr_t) /*_.TUPLITVAL__V84*/ meltfptr[72]) ;;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7934:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L51*/ meltfnum[46] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:7934:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L51*/ meltfnum[46]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L52*/ meltfnum[47] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7934:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L52*/ meltfnum[47];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7934;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "melt_translate_run_macroexpansions_list after melt_run_extension basename=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.BASENAME__V63*/ meltfptr[59];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n*  result=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.RESULT__V105*/ meltfptr[90];
                            /*_.MELT_DEBUG_FUN__V107*/ meltfptr[98] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V106*/ meltfptr[97] = /*_.MELT_DEBUG_FUN__V107*/ meltfptr[98];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7934:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L52*/ meltfnum[47] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V107*/ meltfptr[98] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V106*/ meltfptr[97] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7934:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L51*/ meltfnum[46] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V106*/ meltfptr[97] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7936:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.RESULT__V105*/ meltfptr[90];;

        {
            MELT_LOCATION("warmelt-outobj.melt:7936:/ locexp");
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
        /*_.LET___V104*/ meltfptr[96] = /*_.RETURN___V108*/ meltfptr[89];;

        MELT_LOCATION("warmelt-outobj.melt:7932:/ clear");
        /*clear*/ /*_.RESULT__V105*/ meltfptr[90] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V108*/ meltfptr[89] = 0 ;
        /*_.LET___V83*/ meltfptr[71] = /*_.LET___V104*/ meltfptr[96];;

        MELT_LOCATION("warmelt-outobj.melt:7908:/ clear");
        /*clear*/ /*_#NBLITVAL__L35*/ meltfnum[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.TUPLITVAL__V84*/ meltfptr[72] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IX__L36*/ meltfnum[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FILTUP__V85*/ meltfptr[84] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NBFILE__L37*/ meltfnum[36] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V104*/ meltfptr[96] = 0 ;
        /*_.LET___V62*/ meltfptr[60] = /*_.LET___V83*/ meltfptr[71];;

        MELT_LOCATION("warmelt-outobj.melt:7889:/ clear");
        /*clear*/ /*_.BASENAME__V63*/ meltfptr[59] = 0 ;
        /*^clear*/
        /*clear*/ /*_.TRANSLATE_MACROEXPANDED_LIST__V66*/ meltfptr[64] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REFERENCED_VALUE__V69*/ meltfptr[67] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOTNULL__L24*/ meltfnum[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_#OR___L25*/ meltfnum[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V70*/ meltfptr[63] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V78*/ meltfptr[70] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V83*/ meltfptr[71] = 0 ;
        /*_.LET___V35*/ meltfptr[30] = /*_.LET___V62*/ meltfptr[60];;

        MELT_LOCATION("warmelt-outobj.melt:7831:/ clear");
        /*clear*/ /*_.RUNERRORHDLR__V36*/ meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LITVALIST__V38*/ meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_.TUPLE___V41*/ meltfptr[38] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MACROFRESHENV__V43*/ meltfptr[39] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_MAPSTRING__V44*/ meltfptr[43] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_MAPSTRING__V45*/ meltfptr[44] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_INTEGERBOX__V46*/ meltfptr[45] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_LIST__V47*/ meltfptr[46] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_LIST__V48*/ meltfptr[47] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_INTEGERBOX__V49*/ meltfptr[48] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_LIST__V50*/ meltfptr[49] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_MAPSTRING__V51*/ meltfptr[50] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_LIST__V52*/ meltfptr[51] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_LIST__V53*/ meltfptr[52] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_LIST__V54*/ meltfptr[53] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_INTEGERBOX__V55*/ meltfptr[54] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_MAPOBJECT__V56*/ meltfptr[55] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NEWMODCTX__V57*/ meltfptr[56] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NCX__V59*/ meltfptr[58] = 0 ;
        /*^clear*/
        /*clear*/ /*_#LENXLIST__L17*/ meltfnum[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V62*/ meltfptr[60] = 0 ;
        /*_.LET___V23*/ meltfptr[16] = /*_.LET___V35*/ meltfptr[30];;

        MELT_LOCATION("warmelt-outobj.melt:7793:/ clear");
        /*clear*/ /*_#NUM__L11*/ meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_#STARTERRCOUNT__L12*/ meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NAKEDBASNAM__V24*/ meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REFERR__V25*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.REFNORMLIST__V27*/ meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MODNAM__V29*/ meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MACROENV__V30*/ meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V35*/ meltfptr[30] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7777:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V23*/ meltfptr[16];;

        {
            MELT_LOCATION("warmelt-outobj.melt:7777:/ locexp");
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
        /*clear*/ /*_#NULL__L4*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V10*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_LIST__L7*/ meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L8*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V15*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V23*/ meltfptr[16] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_142_WARMELTmiOUTOBJ_MELT_TRANSLATE_RUN_MACROEXPANSIONS_LIST*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12 fromline 1752 */

    /** start of frame for meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12 of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12// fromline 1568
        : public Melt_CallFrameWithValues<5>
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
        MeltFrame_meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<5> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12), clos) {};
        MeltFrame_meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12() //the constructor fromline 1642
            : Melt_CallFrameWithValues<5> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12)) {};
        MeltFrame_meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<5> (fil,lin, sizeof(MeltFrame_meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12


    /** end of frame for meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12 fromline 1697**/

    /* end of frame for routine meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12 fromline 1756 */

    /* classy proc frame meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12 */ MeltFrame_meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12 fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl12", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:7833:/ getarg");
    /*_.V__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7834:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:7834:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:7834:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7834;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "melt_translate_run_macroexpansions_list/macrorunerrorhdlr v=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.V__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V4*/ meltfptr[3] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V3*/ meltfptr[2] = /*_.MELT_DEBUG_FUN__V4*/ meltfptr[3];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7834:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V4*/ meltfptr[3] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V3*/ meltfptr[2] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:7834:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V3*/ meltfptr[2] = 0 ;
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
            MELT_LOCATION("warmelt-outobj.melt:7835:/ locexp");

#if MELT_HAVE_DEBUG
            if (melt_need_debug (0))
                melt_dbgshortbacktrace(( "melt_translate_run_macroexpansions_list/macrorunerrorhdlr"), (14));
#endif
            ;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_STRING__L3*/ meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.V__V2*/ meltfptr[1])) == MELTOBMAG_STRING);;
        MELT_LOCATION("warmelt-outobj.melt:7837:/ cond");
        /*cond*/ if (/*_#IS_STRING__L3*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:7838:/ locexp");
                        /* melt_translate_run_macroexpansions_list WARNRUNSTR_CHK__2 */
                        warning (0, "MELT running macro expansions {%s} error - %s",
                                 melt_string_str ((melt_ptr_t) (/*~NAKEDBASNAM*/ meltfclos->tabval[0])),
                                 melt_string_str ((melt_ptr_t) /*_.V__V2*/ meltfptr[1])) ; ;
                    }
                    ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:7837:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:7845:/ locexp");
                        /* melt_translate_run_macroexpansions_list WARNRUN_CHK__2 */
                        warning (0, "MELT running macro expansions {%s} errored",
                                 melt_string_str ((melt_ptr_t) (/*~NAKEDBASNAM*/ meltfclos->tabval[0]))) ; ;
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:7844:/ quasiblock");


                    /*epilog*/
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7851:/ cond");
        /*cond*/ if (/*_.V__V2*/ meltfptr[1]) /*then*/
            {
                /*^cond.then*/
                /*_.OR___V5*/ meltfptr[3] = /*_.V__V2*/ meltfptr[1];;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:7851:/ cond.else");

                /*_.OR___V5*/ meltfptr[3] = (/*!konst_1_TRUE*/ meltfrout->tabval[1]);;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:7851:/ cond");
        /*cond*/ if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*~REFERR*/ meltfclos->tabval[1])),
                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[2])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @REFERENCED_VALUE", melt_magic_discr((melt_ptr_t)((/*~REFERR*/ meltfclos->tabval[1]))) == MELTOBMAG_OBJECT);
                    melt_putfield_object(((/*~REFERR*/ meltfclos->tabval[1])), (0), (/*_.OR___V5*/ meltfptr[3]), "REFERENCED_VALUE");
                    ;
                    /*^touch*/
                    meltgc_touch((/*~REFERR*/ meltfclos->tabval[1]));
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object ((/*~REFERR*/ meltfclos->tabval[1]), "put-fields");
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:7833:/ clear");
        /*clear*/ /*_#IS_STRING__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OR___V5*/ meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl12", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_143_WARMELTmiOUTOBJ_LAMBDA_cl12*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS fromline 1752 */

    /** start of frame for meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS// fromline 1568
        : public Melt_CallFrameWithValues<39>
    {
    public: /* fromline 1572*/
        long mcfr_varnum[22];
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
        MeltFrame_meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<39> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS), clos) {};
        MeltFrame_meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS() //the constructor fromline 1642
            : Melt_CallFrameWithValues<39> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<39> (fil,lin, sizeof(MeltFrame_meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS)) {};
        MeltFrame_meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<39> (fil,lin, sizeof(MeltFrame_meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS


    /** end of frame for meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS fromline 1697**/

    /* end of frame for routine meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS fromline 1756 */

    /* classy proc frame meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS */ MeltFrame_meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("READ_MELT_EXPRESSIONS", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:7946:/ getarg");
    /*_.SRC__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.RLIST__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.RLIST__V3*/ meltfptr[2])) != NULL);


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


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7947:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:7947:/ cond");
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
                        /*_#LIST_LENGTH__L4*/ meltfnum[3] =
                            (melt_list_length((melt_ptr_t)(/*_.RLIST__V3*/ meltfptr[2])));;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:7947:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[9];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7947;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "read_melt_expressions src=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SRC__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " depth=";
                            /*^apply.arg*/
                            argtab[6].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[7].meltbp_cstring =  " rlist.len=";
                            /*^apply.arg*/
                            argtab[8].meltbp_long = /*_#LIST_LENGTH__L4*/ meltfnum[3];
                            /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V4*/ meltfptr[3] = /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7947:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L3*/ meltfnum[2] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_#LIST_LENGTH__L4*/ meltfnum[3] = 0 ;
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
                MELT_LOCATION("warmelt-outobj.melt:7947:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L2*/ meltfnum[1] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7948:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_LIST__L5*/ meltfnum[2] =
                (melt_magic_discr((melt_ptr_t)(/*_.RLIST__V3*/ meltfptr[2])) == MELTOBMAG_LIST);;
            MELT_LOCATION("warmelt-outobj.melt:7948:/ cond");
            /*cond*/ if (/*_#IS_LIST__L5*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[3] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7948:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check rlist";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7948;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.RLIST__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V7*/ meltfptr[3] = /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7948:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V6*/ meltfptr[4] = /*_._IFELSE___V7*/ meltfptr[3];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7948:/ clear");
            /*clear*/ /*_#IS_LIST__L5*/ meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V7*/ meltfptr[3] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V6*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7949:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#ltI__L6*/ meltfnum[3] =
                ((/*_#DEPTH__L1*/ meltfnum[0]) < (100));;
            MELT_LOCATION("warmelt-outobj.melt:7949:/ cond");
            /*cond*/ if (/*_#ltI__L6*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[3] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7949:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check depth";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7949;
                            /*^apply.arg*/
                            argtab[3].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                            /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V10*/ meltfptr[3] = /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7949:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V9*/ meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[3];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7949:/ clear");
            /*clear*/ /*_#ltI__L6*/ meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V10*/ meltfptr[3] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V9*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L7*/ meltfnum[1] =
            (/*null*/(/*_.SRC__V2*/ meltfptr[1]) == NULL);;
        MELT_LOCATION("warmelt-outobj.melt:7951:/ cond");
        /*cond*/ if (/*_#NULL__L7*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-outobj.melt:7952:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:7952:/ locexp");
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
                    /*_._IFELSE___V12*/ meltfptr[10] = /*_.RETURN___V13*/ meltfptr[3];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:7951:/ clear");
                    /*clear*/ /*_.RETURN___V13*/ meltfptr[3] = 0 ;
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
                    /*_#IS_STRING__L8*/ meltfnum[2] =
                        (melt_magic_discr((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1])) == MELTOBMAG_STRING);;
                    MELT_LOCATION("warmelt-outobj.melt:7953:/ cond");
                    /*cond*/ if (/*_#IS_STRING__L8*/ meltfnum[2]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:7954:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[2];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_cstring =  "reading from MELT source file $1";
                                    /*^apply.arg*/
                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.SRC__V2*/ meltfptr[1];
                                    /*_.INFORM_AT__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!INFORM_AT*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                /*_.READ_FILE__V16*/ meltfptr[15] =
                                    (meltgc_read_file (melt_string_str((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1])), (char*)0));;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:7955:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[1];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.READ_FILE__V16*/ meltfptr[15];
                                    /*_.LIST_APPEND2LIST__V17*/ meltfptr[16] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_APPEND2LIST*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.RLIST__V3*/ meltfptr[2]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:7953:/ quasiblock");


                                /*_.PROGN___V18*/ meltfptr[17] = /*_.LIST_APPEND2LIST__V17*/ meltfptr[16];;
                                /*^compute*/
                                /*_._IFELSE___V14*/ meltfptr[3] = /*_.PROGN___V18*/ meltfptr[17];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:7953:/ clear");
                                /*clear*/ /*_.INFORM_AT__V15*/ meltfptr[14] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.READ_FILE__V16*/ meltfptr[15] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.LIST_APPEND2LIST__V17*/ meltfptr[16] = 0 ;
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
                                /*_#IS_LIST__L9*/ meltfnum[3] =
                                    (melt_magic_discr((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1])) == MELTOBMAG_LIST);;
                                MELT_LOCATION("warmelt-outobj.melt:7956:/ cond");
                                /*cond*/ if (/*_#IS_LIST__L9*/ meltfnum[3]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
                                            {
                                                /* start foreach_pair_component_in_list meltcit1__EACHLIST */
                                                for (/*_.CURPAIR__V20*/ meltfptr[15] = melt_list_first( (melt_ptr_t)/*_.SRC__V2*/ meltfptr[1]);
                                                                        melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V20*/ meltfptr[15]) == MELTOBMAG_PAIR;
                                                                        /*_.CURPAIR__V20*/ meltfptr[15] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V20*/ meltfptr[15]))
                                                    {
                                                        /*_.CURSRC__V21*/ meltfptr[16] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V20*/ meltfptr[15]);


                                                        /*_#plI__L10*/ meltfnum[9] =
                                                            ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-outobj.melt:7960:/ apply");
                                                        /*apply*/
                                                        {
                                                            union meltparam_un argtab[2];
                                                            memset(&argtab, 0, sizeof(argtab));
                                                            /*^apply.arg*/
                                                            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.RLIST__V3*/ meltfptr[2];
                                                            /*^apply.arg*/
                                                            argtab[1].meltbp_long = /*_#plI__L10*/ meltfnum[9];
                                                            /*_.READ_MELT_EXPRESSIONS__V22*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!READ_MELT_EXPRESSIONS*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.CURSRC__V21*/ meltfptr[16]), (MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                        }
                                                        ;
                                                        /*_._IFELSE___V19*/ meltfptr[14] = /*_.READ_MELT_EXPRESSIONS__V22*/ meltfptr[17];;
                                                    } /* end foreach_pair_component_in_list meltcit1__EACHLIST */
                                                /*_.CURPAIR__V20*/ meltfptr[15] = NULL;
                                                /*_.CURSRC__V21*/ meltfptr[16] = NULL;


                                                /*citerepilog*/

                                                MELT_LOCATION("warmelt-outobj.melt:7957:/ clear");
                                                /*clear*/ /*_.CURPAIR__V20*/ meltfptr[15] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_.CURSRC__V21*/ meltfptr[16] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_#plI__L10*/ meltfnum[9] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_.READ_MELT_EXPRESSIONS__V22*/ meltfptr[17] = 0 ;
                                            } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
                                            ;
                                            /*epilog*/
                                        }
                                        ;
                                    }
                                else
                                    {
                                        MELT_LOCATION("warmelt-outobj.melt:7956:/ cond.else");

                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*_#IS_MULTIPLE__L11*/ meltfnum[10] =
                                                (melt_magic_discr((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1])) == MELTOBMAG_MULTIPLE);;
                                            MELT_LOCATION("warmelt-outobj.melt:7961:/ cond");
                                            /*cond*/ if (/*_#IS_MULTIPLE__L11*/ meltfnum[10]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        /*citerblock FOREACH_IN_MULTIPLE*/
                                                        {
                                                            /* start foreach_in_multiple meltcit2__EACHTUP */
                                                            long  meltcit2__EACHTUP_ln = melt_multiple_length((melt_ptr_t)/*_.SRC__V2*/ meltfptr[1]);
                                                            for (/*_#SIX__L12*/ meltfnum[11] = 0;
                                                                                (/*_#SIX__L12*/ meltfnum[11] >= 0) && (/*_#SIX__L12*/ meltfnum[11] <  meltcit2__EACHTUP_ln);
                                                                                /*_#SIX__L12*/ meltfnum[11]++)
                                                                {
                                                                    /*_.CURSRC__V24*/ meltfptr[23] = melt_multiple_nth((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1]),  /*_#SIX__L12*/ meltfnum[11]);



                                                                    /*_#plI__L13*/ meltfnum[12] =
                                                                        ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    MELT_LOCATION("warmelt-outobj.melt:7965:/ apply");
                                                                    /*apply*/
                                                                    {
                                                                        union meltparam_un argtab[2];
                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                        /*^apply.arg*/
                                                                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.RLIST__V3*/ meltfptr[2];
                                                                        /*^apply.arg*/
                                                                        argtab[1].meltbp_long = /*_#plI__L13*/ meltfnum[12];
                                                                        /*_.READ_MELT_EXPRESSIONS__V25*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!READ_MELT_EXPRESSIONS*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.CURSRC__V24*/ meltfptr[23]), (MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                                    }
                                                                    ;
                                                                    /*_._IFELSE___V23*/ meltfptr[22] = /*_.READ_MELT_EXPRESSIONS__V25*/ meltfptr[24];;
                                                                    if (/*_#SIX__L12*/ meltfnum[11]<0) break;
                                                                } /* end  foreach_in_multiple meltcit2__EACHTUP */

                                                            /*citerepilog*/

                                                            MELT_LOCATION("warmelt-outobj.melt:7962:/ clear");
                                                            /*clear*/ /*_.CURSRC__V24*/ meltfptr[23] = 0 ;
                                                            /*^clear*/
                                                            /*clear*/ /*_#SIX__L12*/ meltfnum[11] = 0 ;
                                                            /*^clear*/
                                                            /*clear*/ /*_#plI__L13*/ meltfnum[12] = 0 ;
                                                            /*^clear*/
                                                            /*clear*/ /*_.READ_MELT_EXPRESSIONS__V25*/ meltfptr[24] = 0 ;
                                                        } /*endciterblock FOREACH_IN_MULTIPLE*/
                                                        ;
                                                        /*epilog*/
                                                    }
                                                    ;
                                                }
                                            else
                                                {
                                                    MELT_LOCATION("warmelt-outobj.melt:7961:/ cond.else");

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        /*_#IS_OBJECT__L14*/ meltfnum[13] =
                                                            (melt_magic_discr((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1])) == MELTOBMAG_OBJECT);;
                                                        MELT_LOCATION("warmelt-outobj.melt:7966:/ cond");
                                                        /*cond*/ if (/*_#IS_OBJECT__L14*/ meltfnum[13]) /*then*/
                                                            {
                                                                /*^cond.then*/
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {




                                                                    {
                                                                        MELT_LOCATION("warmelt-outobj.melt:7967:/ locexp");
                                                                        meltgc_append_list((melt_ptr_t)(/*_.RLIST__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1]));
                                                                    }
                                                                    ;
                                                                    /*clear*/ /*_._IFELSE___V26*/ meltfptr[25] = 0 ;
                                                                    /*epilog*/
                                                                }
                                                                ;
                                                            }
                                                        else
                                                            {
                                                                MELT_LOCATION("warmelt-outobj.melt:7966:/ cond.else");

                                                                /*^block*/
                                                                /*anyblock*/
                                                                {


                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    /*_#IS_CLOSURE__L15*/ meltfnum[14] =
                                                                        (melt_magic_discr((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1])) == MELTOBMAG_CLOSURE);;
                                                                    MELT_LOCATION("warmelt-outobj.melt:7968:/ cond");
                                                                    /*cond*/ if (/*_#IS_CLOSURE__L15*/ meltfnum[14]) /*then*/
                                                                        {
                                                                            /*^cond.then*/
                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {

                                                                                MELT_LOCATION("warmelt-outobj.melt:7969:/ quasiblock");


                                                                                /*_#plI__L16*/ meltfnum[15] =
                                                                                    ((/*_#DEPTH__L1*/ meltfnum[0]) + (1));;

                                                                                MELT_CHECK_SIGNAL();
                                                                                ;
                                                                                MELT_LOCATION("warmelt-outobj.melt:7969:/ apply");
                                                                                /*apply*/
                                                                                {
                                                                                    union meltparam_un argtab[1];
                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                    /*^apply.arg*/
                                                                                    argtab[0].meltbp_long = /*_#plI__L16*/ meltfnum[15];
                                                                                    /*_.XSRC__V28*/ meltfptr[27] =  melt_apply ((meltclosure_ptr_t)(/*_.SRC__V2*/ meltfptr[1]), (melt_ptr_t)(/*_.RLIST__V3*/ meltfptr[2]), (MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                                                }
                                                                                ;

#if MELT_HAVE_DEBUG
                                                                                MELT_LOCATION("warmelt-outobj.melt:7971:/ cppif.then");
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
                                                                                    MELT_LOCATION("warmelt-outobj.melt:7971:/ cond");
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
                                                                                                MELT_LOCATION("warmelt-outobj.melt:7971:/ apply");
                                                                                                /*apply*/
                                                                                                {
                                                                                                    union meltparam_un argtab[7];
                                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L18*/ meltfnum[17];
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[2].meltbp_long = 7971;
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[3].meltbp_cstring =  "read_melt_expressions xsrc=";
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XSRC__V28*/ meltfptr[27];
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[5].meltbp_cstring =  " depth=";
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[6].meltbp_long = /*_#DEPTH__L1*/ meltfnum[0];
                                                                                                    /*_.MELT_DEBUG_FUN__V30*/ meltfptr[29] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
                                                                                                }
                                                                                                ;
                                                                                                /*_._IF___V29*/ meltfptr[28] = /*_.MELT_DEBUG_FUN__V30*/ meltfptr[29];;
                                                                                                /*epilog*/

                                                                                                MELT_LOCATION("warmelt-outobj.melt:7971:/ clear");
                                                                                                /*clear*/ /*_#MELT_CALLCOUNT__L18*/ meltfnum[17] = 0 ;
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
                                                                                        MELT_LOCATION("warmelt-outobj.melt:7971:/ locexp");
                                                                                        /*void*/(void)0;
                                                                                    }
                                                                                    ;
                                                                                    /*^quasiblock*/


                                                                                    /*epilog*/

                                                                                    /*^clear*/
                                                                                    /*clear*/ /*_#MELT_NEED_DBG__L17*/ meltfnum[16] = 0 ;
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

                                                                                MELT_CHECK_SIGNAL();
                                                                                ;
                                                                                /*_#NULL__L19*/ meltfnum[17] =
                                                                                    (/*null*/(/*_.XSRC__V28*/ meltfptr[27]) == NULL);;
                                                                                MELT_LOCATION("warmelt-outobj.melt:7972:/ cond");
                                                                                /*cond*/ if (/*_#NULL__L19*/ meltfnum[17]) /*then*/
                                                                                    {
                                                                                        /*^cond.then*/
                                                                                        /*^block*/
                                                                                        /*anyblock*/
                                                                                        {

                                                                                            /*^quasiblock*/


                                                                                            /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                                                                                            {
                                                                                                MELT_LOCATION("warmelt-outobj.melt:7972:/ locexp");
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
                                                                                            /*_._IF___V31*/ meltfptr[29] = /*_.RETURN___V32*/ meltfptr[28];;
                                                                                            /*epilog*/

                                                                                            MELT_LOCATION("warmelt-outobj.melt:7972:/ clear");
                                                                                            /*clear*/ /*_.RETURN___V32*/ meltfptr[28] = 0 ;
                                                                                        }
                                                                                        ;
                                                                                    }
                                                                                else    /*^cond.else*/
                                                                                    {

                                                                                        /*_._IF___V31*/ meltfptr[29] =  /*reallynil*/ NULL ;;
                                                                                    }
                                                                                ;

#if MELT_HAVE_DEBUG
                                                                                MELT_LOCATION("warmelt-outobj.melt:7973:/ cppif.then");
                                                                                /*^block*/
                                                                                /*anyblock*/
                                                                                {


                                                                                    MELT_CHECK_SIGNAL();
                                                                                    ;
                                                                                    /*_#IS_A__L20*/ meltfnum[16] =
                                                                                        melt_is_instance_of((melt_ptr_t)(/*_.XSRC__V28*/ meltfptr[27]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[6])));;
                                                                                    MELT_LOCATION("warmelt-outobj.melt:7973:/ cond");
                                                                                    /*cond*/ if (/*_#IS_A__L20*/ meltfnum[16]) /*then*/
                                                                                        {
                                                                                            /*^cond.then*/
                                                                                            /*_#OR___L21*/ meltfnum[20] = /*_#IS_A__L20*/ meltfnum[16];;
                                                                                        }
                                                                                    else
                                                                                        {
                                                                                            MELT_LOCATION("warmelt-outobj.melt:7973:/ cond.else");

                                                                                            /*^block*/
                                                                                            /*anyblock*/
                                                                                            {

                                                                                                /*_#IS_A__L22*/ meltfnum[21] =
                                                                                                    melt_is_instance_of((melt_ptr_t)(/*_.XSRC__V28*/ meltfptr[27]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[5])));;
                                                                                                /*^compute*/
                                                                                                /*_#OR___L21*/ meltfnum[20] = /*_#IS_A__L22*/ meltfnum[21];;
                                                                                                /*epilog*/

                                                                                                MELT_LOCATION("warmelt-outobj.melt:7973:/ clear");
                                                                                                /*clear*/ /*_#IS_A__L22*/ meltfnum[21] = 0 ;
                                                                                            }
                                                                                            ;
                                                                                        }
                                                                                    ;
                                                                                    /*^cond*/
                                                                                    /*cond*/ if (/*_#OR___L21*/ meltfnum[20]) /*then*/
                                                                                        {
                                                                                            /*^cond.then*/
                                                                                            /*_._IFELSE___V34*/ meltfptr[33] = (/*nil*/NULL);;
                                                                                        }
                                                                                    else
                                                                                        {
                                                                                            MELT_LOCATION("warmelt-outobj.melt:7973:/ cond.else");

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
                                                                                                    argtab[0].meltbp_cstring =  "check xsrc";
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[2].meltbp_long = 7973;
                                                                                                    /*^apply.arg*/
                                                                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.XSRC__V28*/ meltfptr[27];
                                                                                                    /*_.MELT_ASSERT_FAILURE_FUN__V35*/ meltfptr[34] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                                }
                                                                                                ;
                                                                                                /*_._IFELSE___V34*/ meltfptr[33] = /*_.MELT_ASSERT_FAILURE_FUN__V35*/ meltfptr[34];;
                                                                                                /*epilog*/

                                                                                                MELT_LOCATION("warmelt-outobj.melt:7973:/ clear");
                                                                                                /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V35*/ meltfptr[34] = 0 ;
                                                                                            }
                                                                                            ;
                                                                                        }
                                                                                    ;
                                                                                    /*_.IFCPP___V33*/ meltfptr[28] = /*_._IFELSE___V34*/ meltfptr[33];;
                                                                                    /*epilog*/

                                                                                    MELT_LOCATION("warmelt-outobj.melt:7973:/ clear");
                                                                                    /*clear*/ /*_#IS_A__L20*/ meltfnum[16] = 0 ;
                                                                                    /*^clear*/
                                                                                    /*clear*/ /*_#OR___L21*/ meltfnum[20] = 0 ;
                                                                                    /*^clear*/
                                                                                    /*clear*/ /*_._IFELSE___V34*/ meltfptr[33] = 0 ;
                                                                                }

#else /*MELT_HAVE_DEBUG*/
                                                                                /*^cppif.else*/
                                                                                /*_.IFCPP___V33*/ meltfptr[28] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
                                                                                ;

                                                                                {
                                                                                    MELT_LOCATION("warmelt-outobj.melt:7974:/ locexp");
                                                                                    meltgc_append_list((melt_ptr_t)(/*_.RLIST__V3*/ meltfptr[2]), (melt_ptr_t)(/*_.XSRC__V28*/ meltfptr[27]));
                                                                                }
                                                                                ;
                                                                                /*_._IFELSE___V27*/ meltfptr[26] = /*_.IFCPP___V33*/ meltfptr[28];;

                                                                                MELT_LOCATION("warmelt-outobj.melt:7969:/ clear");
                                                                                /*clear*/ /*_#plI__L16*/ meltfnum[15] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.XSRC__V28*/ meltfptr[27] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_#NULL__L19*/ meltfnum[17] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_._IF___V31*/ meltfptr[29] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.IFCPP___V33*/ meltfptr[28] = 0 ;
                                                                                /*epilog*/
                                                                            }
                                                                            ;
                                                                        }
                                                                    else
                                                                        {
                                                                            MELT_LOCATION("warmelt-outobj.melt:7968:/ cond.else");

                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {

                                                                                /*_.DISCRIM__V36*/ meltfptr[34] =
                                                                                    ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.SRC__V2*/ meltfptr[1]))));;
                                                                                MELT_LOCATION("warmelt-outobj.melt:7977:/ cond");
                                                                                /*cond*/ if (
                                                                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.DISCRIM__V36*/ meltfptr[34]),
                                                                                                                  (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[8])))
                                                                                ) /*then*/
                                                                                    {
                                                                                        /*^cond.then*/
                                                                                        /*^getslot*/
                                                                                        {
                                                                                            melt_ptr_t slot=NULL, obj=NULL;
                                                                                            obj = (melt_ptr_t)(/*_.DISCRIM__V36*/ meltfptr[34]) /*=obj*/;
                                                                                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                                                                            /*_.NAMED_NAME__V37*/ meltfptr[33] = slot;
                                                                                        };
                                                                                        ;
                                                                                    }
                                                                                else    /*^cond.else*/
                                                                                    {

                                                                                        /*_.NAMED_NAME__V37*/ meltfptr[33] =  /*reallynil*/ NULL ;;
                                                                                    }
                                                                                ;

                                                                                MELT_CHECK_SIGNAL();
                                                                                ;
                                                                                MELT_LOCATION("warmelt-outobj.melt:7976:/ apply");
                                                                                /*apply*/
                                                                                {
                                                                                    union meltparam_un argtab[3];
                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                    /*^apply.arg*/
                                                                                    argtab[0].meltbp_cstring =  "invalid MELT expression $1 to read, with discriminant $2";
                                                                                    /*^apply.arg*/
                                                                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.SRC__V2*/ meltfptr[1];
                                                                                    /*^apply.arg*/
                                                                                    argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.NAMED_NAME__V37*/ meltfptr[33];
                                                                                    /*_.ERROR_AT__V38*/ meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!ERROR_AT*/ meltfrout->tabval[7])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                }
                                                                                ;
                                                                                MELT_LOCATION("warmelt-outobj.melt:7975:/ quasiblock");


                                                                                /*_.PROGN___V39*/ meltfptr[29] = /*_.ERROR_AT__V38*/ meltfptr[27];;
                                                                                /*^compute*/
                                                                                /*_._IFELSE___V27*/ meltfptr[26] = /*_.PROGN___V39*/ meltfptr[29];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-outobj.melt:7968:/ clear");
                                                                                /*clear*/ /*_.DISCRIM__V36*/ meltfptr[34] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.NAMED_NAME__V37*/ meltfptr[33] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.ERROR_AT__V38*/ meltfptr[27] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.PROGN___V39*/ meltfptr[29] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    ;
                                                                    /*_._IFELSE___V26*/ meltfptr[25] = /*_._IFELSE___V27*/ meltfptr[26];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-outobj.melt:7966:/ clear");
                                                                    /*clear*/ /*_#IS_CLOSURE__L15*/ meltfnum[14] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_._IFELSE___V27*/ meltfptr[26] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        ;
                                                        /*_._IFELSE___V23*/ meltfptr[22] = /*_._IFELSE___V26*/ meltfptr[25];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-outobj.melt:7961:/ clear");
                                                        /*clear*/ /*_#IS_OBJECT__L14*/ meltfnum[13] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_._IFELSE___V26*/ meltfptr[25] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;
                                            /*_._IFELSE___V19*/ meltfptr[14] = /*_._IFELSE___V23*/ meltfptr[22];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:7956:/ clear");
                                            /*clear*/ /*_#IS_MULTIPLE__L11*/ meltfnum[10] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_._IFELSE___V23*/ meltfptr[22] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V14*/ meltfptr[3] = /*_._IFELSE___V19*/ meltfptr[14];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:7953:/ clear");
                                /*clear*/ /*_#IS_LIST__L9*/ meltfnum[3] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V19*/ meltfptr[14] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V12*/ meltfptr[10] = /*_._IFELSE___V14*/ meltfptr[3];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:7951:/ clear");
                    /*clear*/ /*_#IS_STRING__L8*/ meltfnum[2] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V14*/ meltfptr[3] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:7946:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_._IFELSE___V12*/ meltfptr[10];;

        {
            MELT_LOCATION("warmelt-outobj.melt:7946:/ locexp");
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
        /*clear*/ /*_.IFCPP___V6*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V9*/ meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L7*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V12*/ meltfptr[10] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("READ_MELT_EXPRESSIONS", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_144_WARMELTmiOUTOBJ_READ_MELT_EXPRESSIONS*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES fromline 1752 */

    /** start of frame for meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES// fromline 1568
        : public Melt_CallFrameWithValues<40>
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
        MeltFrame_meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<40> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES), clos) {};
        MeltFrame_meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES() //the constructor fromline 1642
            : Melt_CallFrameWithValues<40> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<40> (fil,lin, sizeof(MeltFrame_meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES)) {};
        MeltFrame_meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<40> (fil,lin, sizeof(MeltFrame_meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES


    /** end of frame for meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES fromline 1697**/

    /* end of frame for routine meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES fromline 1756 */

    /* classy proc frame meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES */ MeltFrame_meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("TRANSLATE_TO_CC_MODULE_MELT_SOURCES", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:7984:/ getarg");
    /*_.SOURCES__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODSRCNAME__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MODSRCNAME__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.CURENV__V4*/ meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.CURENV__V4*/ meltfptr[3])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7985:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:7985:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:7985:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7985;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_to_cc_module_melt_sources sources=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SOURCES__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " modsrcname=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.MODSRCNAME__V3*/ meltfptr[2];
                            /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/ meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7985:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/ meltfnum[1] = 0 ;
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
                MELT_LOCATION("warmelt-outobj.melt:7985:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/ meltfnum[0] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:7986:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/ meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.CURENV__V4*/ meltfptr[3]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-outobj.melt:7986:/ cond");
            /*cond*/ if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7986:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check curenv";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7986;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.CURENV__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7986:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/ meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7986:/ clear");
            /*clear*/ /*_#IS_A__L3*/ meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7987:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRING__L4*/ meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODSRCNAME__V3*/ meltfptr[2])) == MELTOBMAG_STRING);;
            MELT_LOCATION("warmelt-outobj.melt:7987:/ cond");
            /*cond*/ if (/*_#IS_STRING__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7987:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modsrcname";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7987;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODSRCNAME__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V11*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7987:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V10*/ meltfptr[8] = /*_._IFELSE___V11*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7987:/ clear");
            /*clear*/ /*_#IS_STRING__L4*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V11*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V10*/ meltfptr[8] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#STRING_DYNLOADED_SUFFIXED__L5*/ meltfnum[1] =
            /*string_dynloaded_suffixed:*/ (melt_string_is_ending ((melt_ptr_t)/*_.MODSRCNAME__V3*/ meltfptr[2],
                                            MELT_DYNLOADED_SUFFIX));;
        MELT_LOCATION("warmelt-outobj.melt:7988:/ cond");
        /*cond*/ if (/*_#STRING_DYNLOADED_SUFFIXED__L5*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*_#OR___L6*/ meltfnum[0] = /*_#STRING_DYNLOADED_SUFFIXED__L5*/ meltfnum[1];;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:7988:/ cond.else");

                /*^block*/
                /*anyblock*/
                {

                    /*_#STRING_SUFFIXED__L7*/ meltfnum[6] =
                        /*string_suffixed:*/(melt_string_is_ending ((melt_ptr_t)/*_.MODSRCNAME__V3*/ meltfptr[2],  ".melt"));;
                    MELT_LOCATION("warmelt-outobj.melt:7988:/ cond");
                    /*cond*/ if (/*_#STRING_SUFFIXED__L7*/ meltfnum[6]) /*then*/
                        {
                            /*^cond.then*/
                            /*_#OR___L8*/ meltfnum[7] = /*_#STRING_SUFFIXED__L7*/ meltfnum[6];;
                        }
                    else
                        {
                            MELT_LOCATION("warmelt-outobj.melt:7988:/ cond.else");

                            /*^block*/
                            /*anyblock*/
                            {

                                /*_#STRING_SUFFIXED__L9*/ meltfnum[8] =
                                    /*string_suffixed:*/(melt_string_is_ending ((melt_ptr_t)/*_.MODSRCNAME__V3*/ meltfptr[2],  ".c"));;
                                MELT_LOCATION("warmelt-outobj.melt:7988:/ cond");
                                /*cond*/ if (/*_#STRING_SUFFIXED__L9*/ meltfnum[8]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*_#OR___L10*/ meltfnum[9] = /*_#STRING_SUFFIXED__L9*/ meltfnum[8];;
                                    }
                                else
                                    {
                                        MELT_LOCATION("warmelt-outobj.melt:7988:/ cond.else");

                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            /*_#STRING_SUFFIXED__L11*/ meltfnum[10] =
                                                /*string_suffixed:*/(melt_string_is_ending ((melt_ptr_t)/*_.MODSRCNAME__V3*/ meltfptr[2],  ".cc"));;
                                            /*^compute*/
                                            /*_#OR___L10*/ meltfnum[9] = /*_#STRING_SUFFIXED__L11*/ meltfnum[10];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-outobj.melt:7988:/ clear");
                                            /*clear*/ /*_#STRING_SUFFIXED__L11*/ meltfnum[10] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_#OR___L8*/ meltfnum[7] = /*_#OR___L10*/ meltfnum[9];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:7988:/ clear");
                                /*clear*/ /*_#STRING_SUFFIXED__L9*/ meltfnum[8] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#OR___L10*/ meltfnum[9] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_#OR___L6*/ meltfnum[0] = /*_#OR___L8*/ meltfnum[7];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:7988:/ clear");
                    /*clear*/ /*_#STRING_SUFFIXED__L7*/ meltfnum[6] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#OR___L8*/ meltfnum[7] = 0 ;
                }
                ;
            }
        ;
        /*^cond*/
        /*cond*/ if (/*_#OR___L6*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:7992:/ locexp");
                        error ("MELT ERROR MSG [#%ld]::: %s - %s", melt_dbgcounter, ( "invalid MELT source name - should not be suffixed"),
                               melt_string_str((melt_ptr_t)(/*_.MODSRCNAME__V3*/ meltfptr[2])));
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7994:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRING_DYNLOADED_SUFFIXED__L12*/ meltfnum[10] =
                /*string_dynloaded_suffixed:*/ (melt_string_is_ending ((melt_ptr_t)/*_.MODSRCNAME__V3*/ meltfptr[2],
                                                MELT_DYNLOADED_SUFFIX));;
            /*^compute*/
            /*_#NOT__L13*/ meltfnum[8] =
                (!(/*_#STRING_DYNLOADED_SUFFIXED__L12*/ meltfnum[10]));;
            MELT_LOCATION("warmelt-outobj.melt:7994:/ cond");
            /*cond*/ if (/*_#NOT__L13*/ meltfnum[8]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V14*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7994:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "translate_to_cc_module_melt_sources modsrcname not ended with MELT_DYNLOADED_\
SUFFIX";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7994;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODSRCNAME__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V14*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7994:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V13*/ meltfptr[11] = /*_._IFELSE___V14*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7994:/ clear");
            /*clear*/ /*_#STRING_DYNLOADED_SUFFIXED__L12*/ meltfnum[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_#NOT__L13*/ meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V14*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V13*/ meltfptr[11] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7996:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRING_SUFFIXED__L14*/ meltfnum[9] =
                /*string_suffixed:*/(melt_string_is_ending ((melt_ptr_t)/*_.MODSRCNAME__V3*/ meltfptr[2],  ".melt"));;
            /*^compute*/
            /*_#NOT__L15*/ meltfnum[6] =
                (!(/*_#STRING_SUFFIXED__L14*/ meltfnum[9]));;
            MELT_LOCATION("warmelt-outobj.melt:7996:/ cond");
            /*cond*/ if (/*_#NOT__L15*/ meltfnum[6]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V17*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7996:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "translate_to_cc_module_melt_sources modulename not ended with .melt";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7996;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODSRCNAME__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V17*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7996:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V16*/ meltfptr[14] = /*_._IFELSE___V17*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7996:/ clear");
            /*clear*/ /*_#STRING_SUFFIXED__L14*/ meltfnum[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_#NOT__L15*/ meltfnum[6] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V17*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V16*/ meltfptr[14] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:7998:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRING_SUFFIXED__L16*/ meltfnum[7] =
                /*string_suffixed:*/(melt_string_is_ending ((melt_ptr_t)/*_.MODSRCNAME__V3*/ meltfptr[2],  ".c"));;
            /*^compute*/
            /*_#NOT__L17*/ meltfnum[10] =
                (!(/*_#STRING_SUFFIXED__L16*/ meltfnum[7]));;
            MELT_LOCATION("warmelt-outobj.melt:7998:/ cond");
            /*cond*/ if (/*_#NOT__L17*/ meltfnum[10]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V20*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:7998:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "translate_to_cc_module_melt_sources modulename not ended with .c";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 7998;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODSRCNAME__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V21*/ meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V20*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V21*/ meltfptr[20];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:7998:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V21*/ meltfptr[20] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V19*/ meltfptr[17] = /*_._IFELSE___V20*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:7998:/ clear");
            /*clear*/ /*_#STRING_SUFFIXED__L16*/ meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_#NOT__L17*/ meltfnum[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V20*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V19*/ meltfptr[17] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:8000:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#STRING_SUFFIXED__L18*/ meltfnum[8] =
                /*string_suffixed:*/(melt_string_is_ending ((melt_ptr_t)/*_.MODSRCNAME__V3*/ meltfptr[2],  ".cc"));;
            /*^compute*/
            /*_#NOT__L19*/ meltfnum[9] =
                (!(/*_#STRING_SUFFIXED__L18*/ meltfnum[8]));;
            MELT_LOCATION("warmelt-outobj.melt:8000:/ cond");
            /*cond*/ if (/*_#NOT__L19*/ meltfnum[9]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V23*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:8000:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "translate_to_cc_module_melt_sources modulename not ended with .cc";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 8000;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODSRCNAME__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V23*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[23];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:8000:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V24*/ meltfptr[23] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V22*/ meltfptr[20] = /*_._IFELSE___V23*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:8000:/ clear");
            /*clear*/ /*_#STRING_SUFFIXED__L18*/ meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_#NOT__L19*/ meltfnum[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V23*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V22*/ meltfptr[20] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-outobj.melt:8002:/ quasiblock");


        /*_#ISVALIDMODSRC__L20*/ meltfnum[6] = 0;;
        /*^compute*/
        /*_.RLIST__V25*/ meltfptr[23] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[3]))));;

        {
            MELT_LOCATION("warmelt-outobj.melt:8005:/ locexp");
            /* translate_to_cc_module_melt_sources SETISVALIDMODSRCCH__1 */
            {
                const char* modsrcstr = melt_string_str ((melt_ptr_t) /*_.MODSRCNAME__V3*/ meltfptr[2]);
                const char* modsrcbase = modsrcstr ? (lbasename (modsrcstr)) : NULL;
                if (modsrcbase)
                    /*_#ISVALIDMODSRC__L20*/ meltfnum[6] =
                        (strchr(modsrcbase,'.') == NULL) && (strchr(modsrcbase,'+') == NULL);
            } /* end translate_to_cc_module_melt_sources SETISVALIDMODSRCCH__1 */
            ;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#NOT__L21*/ meltfnum[7] =
            (!(/*_#ISVALIDMODSRC__L20*/ meltfnum[6]));;
        MELT_LOCATION("warmelt-outobj.melt:8015:/ cond");
        /*cond*/ if (/*_#NOT__L21*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:8016:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L22*/ meltfnum[10] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-outobj.melt:8016:/ cond");
                        /*cond*/ if (/*_#MELT_NEED_DBG__L22*/ meltfnum[10]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L23*/ meltfnum[8] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-outobj.melt:8016:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L23*/ meltfnum[8];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 8016;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "translate_to_cc_module_melt_sources bad generated modsrcname=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MODSRCNAME__V3*/ meltfptr[2];
                                        /*_.MELT_DEBUG_FUN__V28*/ meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V27*/ meltfptr[26] = /*_.MELT_DEBUG_FUN__V28*/ meltfptr[27];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:8016:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L23*/ meltfnum[8] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V28*/ meltfptr[27] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V27*/ meltfptr[26] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:8016:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L22*/ meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V27*/ meltfptr[26] = 0 ;
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
                    MELT_LOCATION("warmelt-outobj.melt:8017:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[2];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "invalid generated source name $1 [basename should have no dot or plus]";
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MODSRCNAME__V3*/ meltfptr[2];
                        /*_.ERROR_AT__V29*/ meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!ERROR_AT*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:8020:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:8020:/ locexp");
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
                    MELT_LOCATION("warmelt-outobj.melt:8015:/ quasiblock");


                    /*_.PROGN___V31*/ meltfptr[30] = /*_.RETURN___V30*/ meltfptr[26];;
                    /*^compute*/
                    /*_._IF___V26*/ meltfptr[4] = /*_.PROGN___V31*/ meltfptr[30];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:8015:/ clear");
                    /*clear*/ /*_.ERROR_AT__V29*/ meltfptr[27] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V30*/ meltfptr[26] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V31*/ meltfptr[30] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V26*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:8022:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.RLIST__V25*/ meltfptr[23];
            /*^apply.arg*/
            argtab[1].meltbp_long = 0;
            /*_.READ_MELT_EXPRESSIONS__V32*/ meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!READ_MELT_EXPRESSIONS*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.SOURCES__V2*/ meltfptr[1]), (MELTBPARSTR_PTR MELTBPARSTR_LONG ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:8023:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L24*/ meltfnum[9] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:8023:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L24*/ meltfnum[9]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L25*/ meltfnum[8] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:8023:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L25*/ meltfnum[8];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 8023;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "after read translate_to_cc_module_melt_sources rlist=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RLIST__V25*/ meltfptr[23];
                            /*_.MELT_DEBUG_FUN__V34*/ meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V33*/ meltfptr[26] = /*_.MELT_DEBUG_FUN__V34*/ meltfptr[30];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:8023:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L25*/ meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V34*/ meltfptr[30] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V33*/ meltfptr[26] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:8023:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L24*/ meltfnum[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V33*/ meltfptr[26] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:8024:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#LIST_LENGTH__L26*/ meltfnum[10] =
                (melt_list_length((melt_ptr_t)(/*_.RLIST__V25*/ meltfptr[23])));;
            /*^compute*/
            /*_#gtI__L27*/ meltfnum[8] =
                ((/*_#LIST_LENGTH__L26*/ meltfnum[10]) > (0));;
            MELT_LOCATION("warmelt-outobj.melt:8024:/ cond");
            /*cond*/ if (/*_#gtI__L27*/ meltfnum[8]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V36*/ meltfptr[26] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:8024:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check non empty rlist";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 8024;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.RLIST__V25*/ meltfptr[23];
                            /*_.MELT_ASSERT_FAILURE_FUN__V37*/ meltfptr[36] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V36*/ meltfptr[26] = /*_.MELT_ASSERT_FAILURE_FUN__V37*/ meltfptr[36];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:8024:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V37*/ meltfptr[36] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V35*/ meltfptr[30] = /*_._IFELSE___V36*/ meltfptr[26];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:8024:/ clear");
            /*clear*/ /*_#LIST_LENGTH__L26*/ meltfnum[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_#gtI__L27*/ meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V36*/ meltfptr[26] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V35*/ meltfptr[30] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:8025:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CURENV__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MODSRCNAME__V3*/ meltfptr[2];
            /*_.COMPILE_LIST_SEXPR__V38*/ meltfptr[36] =  melt_apply ((meltclosure_ptr_t)((/*!COMPILE_LIST_SEXPR*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.RLIST__V25*/ meltfptr[23]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:8026:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L28*/ meltfnum[9] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:8026:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L28*/ meltfnum[9]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L29*/ meltfnum[10] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:8026:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[6];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L29*/ meltfnum[10];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 8026;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "translate_to_cc_module_melt_sources done modsrcname=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MODSRCNAME__V3*/ meltfptr[2];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n";
                            /*_.MELT_DEBUG_FUN__V40*/ meltfptr[39] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V39*/ meltfptr[26] = /*_.MELT_DEBUG_FUN__V40*/ meltfptr[39];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:8026:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L29*/ meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V40*/ meltfptr[39] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V39*/ meltfptr[26] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:8026:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L28*/ meltfnum[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V39*/ meltfptr[26] = 0 ;
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

        MELT_LOCATION("warmelt-outobj.melt:8002:/ clear");
        /*clear*/ /*_#ISVALIDMODSRC__L20*/ meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RLIST__V25*/ meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L21*/ meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V26*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.READ_MELT_EXPRESSIONS__V32*/ meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V35*/ meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.COMPILE_LIST_SEXPR__V38*/ meltfptr[36] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:7984:/ clear");
        /*clear*/ /*_.IFCPP___V7*/ meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V10*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_#STRING_DYNLOADED_SUFFIXED__L5*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_#OR___L6*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V13*/ meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V16*/ meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V19*/ meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V22*/ meltfptr[20] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("TRANSLATE_TO_CC_MODULE_MELT_SOURCES", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_145_WARMELTmiOUTOBJ_TRANSLATE_TO_CC_MODULE_MELT_SOURCES*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_146_WARMELTmiOUTOBJ_EVAL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                  const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_146_WARMELTmiOUTOBJ_EVAL_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_146_WARMELTmiOUTOBJ_EVAL fromline 1752 */

    /** start of frame for meltrout_146_WARMELTmiOUTOBJ_EVAL of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_146_WARMELTmiOUTOBJ_EVAL// fromline 1568
        : public Melt_CallFrameWithValues<29>
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
        MeltFrame_meltrout_146_WARMELTmiOUTOBJ_EVAL(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<29> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_146_WARMELTmiOUTOBJ_EVAL), clos) {};
        MeltFrame_meltrout_146_WARMELTmiOUTOBJ_EVAL() //the constructor fromline 1642
            : Melt_CallFrameWithValues<29> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_146_WARMELTmiOUTOBJ_EVAL)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_146_WARMELTmiOUTOBJ_EVAL(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<29> (fil,lin, sizeof(MeltFrame_meltrout_146_WARMELTmiOUTOBJ_EVAL)) {};
        MeltFrame_meltrout_146_WARMELTmiOUTOBJ_EVAL(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<29> (fil,lin, sizeof(MeltFrame_meltrout_146_WARMELTmiOUTOBJ_EVAL), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_146_WARMELTmiOUTOBJ_EVAL


    /** end of frame for meltrout_146_WARMELTmiOUTOBJ_EVAL fromline 1697**/

    /* end of frame for routine meltrout_146_WARMELTmiOUTOBJ_EVAL fromline 1756 */

    /* classy proc frame meltrout_146_WARMELTmiOUTOBJ_EVAL */ MeltFrame_meltrout_146_WARMELTmiOUTOBJ_EVAL
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_146_WARMELTmiOUTOBJ_EVAL fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("EVAL", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:8030:/ getarg");
    /*_.EXP__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:8036:/ cppif.then");
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
            MELT_LOCATION("warmelt-outobj.melt:8036:/ cond");
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
                        MELT_LOCATION("warmelt-outobj.melt:8036:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 8036;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "eval exp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.EXP__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " env=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V4*/ meltfptr[3] = /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:8036:/ clear");
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
                MELT_LOCATION("warmelt-outobj.melt:8036:/ locexp");
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

        MELT_CHECK_SIGNAL();
        ;
        /*_#NULL__L3*/ meltfnum[1] =
            (/*null*/(/*_.ENV__V3*/ meltfptr[2]) == NULL);;
        MELT_LOCATION("warmelt-outobj.melt:8037:/ cond");
        /*cond*/ if (/*_#NULL__L3*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-outobj.melt:8038:/ compute");
                    /*_.ENV__V3*/ meltfptr[2] = /*_.SETQ___V7*/ meltfptr[3] = (/*!konst_1*/ meltfrout->tabval[1]);;
                    /*_._IF___V6*/ meltfptr[4] = /*_.SETQ___V7*/ meltfptr[3];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:8037:/ clear");
                    /*clear*/ /*_.SETQ___V7*/ meltfptr[3] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V6*/ meltfptr[4] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L4*/ meltfnum[0] =
            !melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
        MELT_LOCATION("warmelt-outobj.melt:8039:/ cond");
        /*cond*/ if (/*_#IS_NOT_A__L4*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-outobj.melt:8040:/ cppif.then");
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
                        MELT_LOCATION("warmelt-outobj.melt:8040:/ cond");
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
                                    MELT_LOCATION("warmelt-outobj.melt:8040:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L6*/ meltfnum[5];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 8040;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "eval returns nil with bad env=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                                        /*_.MELT_DEBUG_FUN__V10*/ meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V9*/ meltfptr[8] = /*_.MELT_DEBUG_FUN__V10*/ meltfptr[9];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:8040:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L6*/ meltfnum[5] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V10*/ meltfptr[9] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V9*/ meltfptr[8] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-outobj.melt:8040:/ locexp");
                            /*void*/(void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L5*/ meltfnum[4] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V9*/ meltfptr[8] = 0 ;
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
                    /*_.DISCRIM__V11*/ meltfptr[9] =
                        ((melt_ptr_t) (melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]))));;
                    MELT_LOCATION("warmelt-outobj.melt:8042:/ cond");
                    /*cond*/ if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.DISCRIM__V11*/ meltfptr[9]),
                                                      (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[4])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.DISCRIM__V11*/ meltfptr[9]) /*=obj*/;
                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                /*_.NAMED_NAME__V12*/ meltfptr[8] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.NAMED_NAME__V12*/ meltfptr[8] =  /*reallynil*/ NULL ;;
                        }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:8041:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[2];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "EVAL got a bad environment of $1";
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.NAMED_NAME__V12*/ meltfptr[8];
                        /*_.WARNING_AT__V13*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!WARNING_AT*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-outobj.melt:8043:/ quasiblock");


                    /*_._RETVAL___V1*/ meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-outobj.melt:8043:/ locexp");
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
                    MELT_LOCATION("warmelt-outobj.melt:8039:/ quasiblock");


                    /*_.PROGN___V15*/ meltfptr[14] = /*_.RETURN___V14*/ meltfptr[13];;
                    /*^compute*/
                    /*_._IF___V8*/ meltfptr[3] = /*_.PROGN___V15*/ meltfptr[14];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:8039:/ clear");
                    /*clear*/ /*_.DISCRIM__V11*/ meltfptr[9] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.NAMED_NAME__V12*/ meltfptr[8] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.WARNING_AT__V13*/ meltfptr[12] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V14*/ meltfptr[13] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V15*/ meltfptr[14] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V8*/ meltfptr[3] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-outobj.melt:8045:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_LIST__L7*/ meltfnum[5] =
            (melt_magic_discr((melt_ptr_t)(/*_.EXP__V2*/ meltfptr[1])) == MELTOBMAG_LIST);;
        MELT_LOCATION("warmelt-outobj.melt:8046:/ cond");
        /*cond*/ if (/*_#IS_LIST__L7*/ meltfnum[5]) /*then*/
            {
                /*^cond.then*/
                /*_.LISTEXPS__V17*/ meltfptr[8] = /*_.EXP__V2*/ meltfptr[1];;
            }
        else
            {
                MELT_LOCATION("warmelt-outobj.melt:8046:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_MULTIPLE__L8*/ meltfnum[4] =
                        (melt_magic_discr((melt_ptr_t)(/*_.EXP__V2*/ meltfptr[1])) == MELTOBMAG_MULTIPLE);;
                    MELT_LOCATION("warmelt-outobj.melt:8048:/ cond");
                    /*cond*/ if (/*_#IS_MULTIPLE__L8*/ meltfnum[4]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:8049:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[1];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_LIST*/ meltfrout->tabval[6]);
                                    /*_.MULTIPLE_TO_LIST__V19*/ meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!MULTIPLE_TO_LIST*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.EXP__V2*/ meltfptr[1]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                /*_._IFELSE___V18*/ meltfptr[12] = /*_.MULTIPLE_TO_LIST__V19*/ meltfptr[13];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:8048:/ clear");
                                /*clear*/ /*_.MULTIPLE_TO_LIST__V19*/ meltfptr[13] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-outobj.melt:8052:/ blockmultialloc");
                                /*multiallocblock*/
                                {
                                    struct meltletrec_1_st
                                    {
                                        struct meltpair_st rpair_0__EXP_x1;
                                        struct meltlist_st rlist_1__LIST_;
                                        long meltletrec_1_endgap;
                                    } *meltletrec_1_ptr = 0;
                                    meltletrec_1_ptr = (struct meltletrec_1_st *) meltgc_allocate (sizeof (struct meltletrec_1_st), 0);
                                    /*^blockmultialloc.initfill*/
                                    /*inipair rpair_0__EXP_x1*/
                                    /*_.EXP__V21*/ meltfptr[13] = (melt_ptr_t) &meltletrec_1_ptr->rpair_0__EXP_x1;
                                    meltletrec_1_ptr->rpair_0__EXP_x1.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_PAIR))));

                                    /*inilist rlist_1__LIST_*/
                                    /*_.LIST___V22*/ meltfptr[21] = (melt_ptr_t) &meltletrec_1_ptr->rlist_1__LIST_;
                                    meltletrec_1_ptr->rlist_1__LIST_.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_LIST))));



                                    /*^putpairhead*/
                                    /*putpairhead*/
                                    melt_assertmsg("putpairhead /4 checkpair", melt_magic_discr((melt_ptr_t)(/*_.EXP__V21*/ meltfptr[13]))== MELTOBMAG_PAIR);
                                    ((meltpair_ptr_t)(/*_.EXP__V21*/ meltfptr[13]))->hd = (melt_ptr_t) (/*_.EXP__V2*/ meltfptr[1]);
                                    ;
                                    /*^touch*/
                                    meltgc_touch(/*_.EXP__V21*/ meltfptr[13]);
                                    ;
                                    /*^putlist*/
                                    /*putlist*/
                                    melt_assertmsg("putlist checklist", melt_magic_discr((melt_ptr_t)(/*_.LIST___V22*/ meltfptr[21]))== MELTOBMAG_LIST);
                                    ((meltlist_ptr_t)(/*_.LIST___V22*/ meltfptr[21]))->first = (meltpair_ptr_t) (/*_.EXP__V21*/ meltfptr[13]);
                                    ((meltlist_ptr_t)(/*_.LIST___V22*/ meltfptr[21]))->last = (meltpair_ptr_t) (/*_.EXP__V21*/ meltfptr[13]);
                                    ;
                                    /*^touch*/
                                    meltgc_touch(/*_.LIST___V22*/ meltfptr[21]);
                                    ;
                                    /*_.LIST___V20*/ meltfptr[14] = /*_.LIST___V22*/ meltfptr[21];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-outobj.melt:8052:/ clear");
                                    /*clear*/ /*_.EXP__V21*/ meltfptr[13] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.LIST___V22*/ meltfptr[21] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.EXP__V21*/ meltfptr[13] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.LIST___V22*/ meltfptr[21] = 0 ;
                                } /*end multiallocblock*/
                                ;
                                MELT_LOCATION("warmelt-outobj.melt:8051:/ quasiblock");


                                /*_.PROGN___V23*/ meltfptr[13] = /*_.LIST___V20*/ meltfptr[14];;
                                /*^compute*/
                                /*_._IFELSE___V18*/ meltfptr[12] = /*_.PROGN___V23*/ meltfptr[13];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-outobj.melt:8048:/ clear");
                                /*clear*/ /*_.LIST___V20*/ meltfptr[14] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V23*/ meltfptr[13] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_.LISTEXPS__V17*/ meltfptr[8] = /*_._IFELSE___V18*/ meltfptr[12];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-outobj.melt:8046:/ clear");
                    /*clear*/ /*_#IS_MULTIPLE__L8*/ meltfnum[4] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V18*/ meltfptr[12] = 0 ;
                }
                ;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:8054:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L9*/ meltfnum[4] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:8054:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L9*/ meltfnum[4]) /*then*/
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
                        MELT_LOCATION("warmelt-outobj.melt:8054:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L10*/ meltfnum[9];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 8054;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "eval listexps=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.LISTEXPS__V17*/ meltfptr[8];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " updated env=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_DEBUG_FUN__V25*/ meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V24*/ meltfptr[21] = /*_.MELT_DEBUG_FUN__V25*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:8054:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L10*/ meltfnum[9] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V25*/ meltfptr[14] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V24*/ meltfptr[21] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:8054:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L9*/ meltfnum[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V24*/ meltfptr[21] = 0 ;
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
        MELT_LOCATION("warmelt-outobj.melt:8055:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*_.RESEVAL__V27*/ meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!TRANSLATE_RUN_MELT_EXPRESSIONS*/ meltfrout->tabval[7])), (melt_ptr_t)(/*_.LISTEXPS__V17*/ meltfptr[8]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:8057:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L11*/ meltfnum[9] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-outobj.melt:8057:/ cond");
            /*cond*/ if (/*_#MELT_NEED_DBG__L11*/ meltfnum[9]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L12*/ meltfnum[4] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-outobj.melt:8057:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[4];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-outobj.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 8057;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "eval gives reseval=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RESEVAL__V27*/ meltfptr[12];
                            /*_.MELT_DEBUG_FUN__V29*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V28*/ meltfptr[14] = /*_.MELT_DEBUG_FUN__V29*/ meltfptr[21];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:8057:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L12*/ meltfnum[4] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V29*/ meltfptr[21] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V28*/ meltfptr[14] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-outobj.melt:8057:/ locexp");
                /*void*/(void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L11*/ meltfnum[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V28*/ meltfptr[14] = 0 ;
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
        /*_.LET___V26*/ meltfptr[13] = /*_.RESEVAL__V27*/ meltfptr[12];;

        MELT_LOCATION("warmelt-outobj.melt:8055:/ clear");
        /*clear*/ /*_.RESEVAL__V27*/ meltfptr[12] = 0 ;
        /*_.LET___V16*/ meltfptr[9] = /*_.LET___V26*/ meltfptr[13];;

        MELT_LOCATION("warmelt-outobj.melt:8045:/ clear");
        /*clear*/ /*_#IS_LIST__L7*/ meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LISTEXPS__V17*/ meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V26*/ meltfptr[13] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-outobj.melt:8030:/ quasiblock");


        /*_._RETVAL___V1*/ meltfptr[0] = /*_.LET___V16*/ meltfptr[9];;

        {
            MELT_LOCATION("warmelt-outobj.melt:8030:/ locexp");
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
        /*clear*/ /*_#NULL__L3*/ meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V6*/ meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L4*/ meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V8*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V16*/ meltfptr[9] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("EVAL", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_146_WARMELTmiOUTOBJ_EVAL_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_146_WARMELTmiOUTOBJ_EVAL*/






melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT fromline 1752 */

    /** start of frame for meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT of CLASS_PROCROUTINEOBJ from 1555**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT// fromline 1568
        : public Melt_CallFrameWithValues<6>
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
        MeltFrame_meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT(meltclosure_ptr_t clos) //the closure constructor fromline 1630
            : Melt_CallFrameWithValues<6> (
#if MELT_HAVE_DEBUG /*fromline 1634*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1638*/
                sizeof(MeltFrame_meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT), clos) {};
        MeltFrame_meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT() //the constructor fromline 1642
            : Melt_CallFrameWithValues<6> (
#if MELT_HAVE_DEBUG /*fromline 1646*/
                __FILE__, __LINE__,
#endif /* MELT_HAVE_DEBUG fromline 1650*/
                sizeof(MeltFrame_meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT)) {};
#if MELT_HAVE_DEBUG /*fromline 1654*/
        MeltFrame_meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT(const char*fil, int lin) //the constructor fromline 1656
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT)) {};
        MeltFrame_meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1661
            : Melt_CallFrameWithValues<6> (fil,lin, sizeof(MeltFrame_meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT), clos) {};
#endif /* MELT_HAVE_DEBUG fromline 1666*/

    }; // end  class MeltFrame_meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT


    /** end of frame for meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT fromline 1697**/

    /* end of frame for routine meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT fromline 1756 */

    /* classy proc frame meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT */ MeltFrame_meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT
    meltfram__ /*declfrastruct fromline 1780*/
    /*classyprocarg meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT fromline 1785*/
#if MELT_HAVE_DEBUG
    (__FILE__, __LINE__, meltclosp_) /* fromline 1789*/
#else /*MELT_HAVE_DEBUG */
    (meltclosp_) /* fromline 1793*/
#endif /*MELT_HAVE_DEBUG */

    ;
#define meltframe meltfram__

    melt_trace_start("GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT", meltcallcount);
    /*use arguments*/ (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-outobj.melt:8062:/ getarg");
    /*_.SBUF__V2*/ meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.NAME__V3*/ meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.NAME__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-outobj.melt:8065:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRBUF__L1*/ meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.SBUF__V2*/ meltfptr[1])) == MELTOBMAG_STRBUF);;
            MELT_LOCATION("warmelt-outobj.melt:8065:/ cond");
            /*cond*/ if (/*_#IS_STRBUF__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V5*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-outobj.melt:8065:/ cond.else");

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
                            argtab[2].meltbp_long = 8065;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SBUF__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V5*/ meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-outobj.melt:8065:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V6*/ meltfptr[5] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V4*/ meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-outobj.melt:8065:/ clear");
            /*clear*/ /*_#IS_STRBUF__L1*/ meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V5*/ meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V4*/ meltfptr[3] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:8066:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V2*/ meltfptr[1]), ( "/** Copyright (C) "));
        }
        ;
        MELT_LOCATION("warmelt-outobj.melt:8067:/ quasiblock");


        /*_#YEAR__L2*/ meltfnum[0] = 0;;

        {
            MELT_LOCATION("warmelt-outobj.melt:8069:/ locexp");
            /*+ generate_gplv3plus_copyright_notice_c_comment GETYEAR__1*/
            {
                time_t GETYEAR__1_now = 0;
                struct tm* GETYEAR__1_tm = NULL;
                /*_#YEAR__L2*/ meltfnum[0] = atol (__DATE__ + 7);
                time (&GETYEAR__1_now);
                if (GETYEAR__1_now > 0)
                    GETYEAR__1_tm = localtime (&GETYEAR__1_now);
                if (GETYEAR__1_tm)
                    /*_#YEAR__L2*/ meltfnum[0] = GETYEAR__1_tm->tm_year + 1900;
            } /*- GETYEAR__1*/;
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:8080:/ locexp");
            meltgc_add_strbuf_dec((melt_ptr_t)(/*_.SBUF__V2*/ meltfptr[1]), (/*_#YEAR__L2*/ meltfnum[0]));
        }
        ;

        MELT_LOCATION("warmelt-outobj.melt:8067:/ clear");
        /*clear*/ /*_#YEAR__L2*/ meltfnum[0] = 0 ;

        {
            MELT_LOCATION("warmelt-outobj.melt:8082:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V2*/ meltfptr[1]), ( " Free Software Foundation, Inc."));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:8083:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V2*/ meltfptr[1]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:8084:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V2*/ meltfptr[1]), ( "  This generated file "));
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_STRING__L3*/ meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.NAME__V3*/ meltfptr[2])) == MELTOBMAG_STRING);;
        MELT_LOCATION("warmelt-outobj.melt:8085:/ cond");
        /*cond*/ if (/*_#IS_STRING__L3*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-outobj.melt:8086:/ locexp");
                        /*ADDBASENAME__1 +*/
                        meltgc_add_strbuf( (melt_ptr_t)(/*_.SBUF__V2*/ meltfptr[1]),
                                           lbasename (melt_string_str((melt_ptr_t)(/*_.NAME__V3*/ meltfptr[2]))));
                        /*ADDBASENAME__1 -*/ ;
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:8093:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V2*/ meltfptr[1]), ( " is part of GCC."));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:8094:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V2*/ meltfptr[1]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:8095:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V2*/ meltfptr[1]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:8096:/ locexp");
            /*add2sbuf_strconst*/ meltgc_add_strbuf((melt_ptr_t)(/*_.SBUF__V2*/ meltfptr[1]), ( "                  [DON\'T EDIT THIS GENERATED FILE]\
\n\n    GCC is free software; you can redistribute it and/or modify\
\
\n    it under the terms of the GNU General Public License as published\
 by\n    the Free Software Foundation; either version 3, or (at your\
 option)\n    any later version.\n\
\n    GCC is distributed in the hope that it will be useful,\
\n    but WITHOUT ANY WARRANTY; without even the implied warranty of\
\
\n    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\
\
\n    GNU General Public License for more details.\
\n\n    You should have received a copy of the GNU General Public License\
\
\n    along with GCC; see the file COPYING3.  If not see\
\n    <http://www.gnu.org/licenses/>.\
\n**/"));
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:8112:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V2*/ meltfptr[1]), (0), 0);
        }
        ;

        {
            MELT_LOCATION("warmelt-outobj.melt:8113:/ locexp");
            meltgc_strbuf_add_indent((melt_ptr_t)(/*_.SBUF__V2*/ meltfptr[1]), (0), 0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-outobj.melt:8062:/ clear");
        /*clear*/ /*_.IFCPP___V4*/ meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_STRING__L3*/ meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_147_WARMELTmiOUTOBJ_GENERATE_GPLV3PLUS_COPYRIGHT_NOTICE_C_COMMENT*/




/**** end of warmelt-outobj+07.cc ****/

