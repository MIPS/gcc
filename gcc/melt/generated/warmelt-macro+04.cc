/* GCC MELT GENERATED C++ FILE warmelt-macro+04.cc - DO NOT EDIT - see http://gcc-melt.org/ */
/* secondary MELT generated C++ file of rank #4 */
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt_f4[] = MELT_RUN_HASHMD5 /* from melt-run.h */;


/**** warmelt-macro+04.cc declarations ****/
/**** MELT GENERATED DECLARATIONS for warmelt-macro ** DO NOT EDIT ; see gcc-melt.org ****/

/****++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
***
    Copyright 2008 - 2014  Free Software Foundation, Inc.
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


/** ordinary MELT module meltbuild-sources/warmelt-macro**/
#define MELT_HAS_INITIAL_ENVIRONMENT 1 /*usualmodule*/



class Melt_InitialClassyFrame_WARMELTmiMACRO_h550734951; // forward declaration fromline 6566
typedef Melt_InitialClassyFrame_WARMELTmiMACRO_h550734951 Melt_InitialFrame;
/*** 6 MELT module variables declarations ****/
MELT_EXTERN void melt_forwarding_module_data (void);
MELT_EXTERN void melt_marking_module_data (void);

MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY melt_WARMELTmiMACRO_module_var_ptr_tab[16];
MELT_EXTERN bool MELT_MODULE_VISIBILITY melt_WARMELTmiMACRO_module_var_flags[1];

static inline melt_ptr_t
melt_module_var_fetch (int ix)
{
    return (ix > 0  &&  ix <= 6)?melt_WARMELTmiMACRO_module_var_ptr_tab[ix]:NULL;
}

static inline void
melt_module_var_put (int ix, melt_ptr_t val)
{
    if (ix > 0  &&  ix <= 6)
        {
            melt_WARMELTmiMACRO_module_var_ptr_tab[ix]= val;
            melt_WARMELTmiMACRO_module_var_flags[ix/16] = true;
        }
}

/* MELT module variables indexes */
enum
{
    meltmodatix_none,
    meltmodvarix_DIAGV_GIMPLE_symb = 1,
    meltmodvarix_DIAGV_GIMPLE_SEQ_symb = 2,
    meltmodvarix_DIAGV_TREE_symb = 3,
    meltmodvarix_DIAGV_BASIC_BLOCK_symb = 4,
    meltmodvarix_DIAGV_EDGE_symb = 5,
    meltmodvarix_MELT_TRANSRUNMACRO_CLOS_symb = 6,
    meltmodatix_last
}; /* end MELT module variables indexes */

/*** 7 MELT called hook declarations ***/

/*declare MELT called hook #0 HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_PREVENV, const char* meltinp1_MODULNAME);

/*declare MELT called hook #1 HOOK_MACRO_EXPORTER **/
MELT_EXTERN void melthook_HOOK_MACRO_EXPORTER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_SYM, melt_ptr_t meltinp1_VAL, melt_ptr_t meltinp2_CONTENV);

/*declare MELT called hook #2 HOOK_MELT_MAKE_LOCATION **/
MELT_EXTERN melt_ptr_t melthook_HOOK_MELT_MAKE_LOCATION (melt_ptr_t melthookdatap, const char* meltinp0_FILENAME, long meltinp1_LINENO);

/*declare MELT called hook #3 HOOK_MELT_MAKE_LOCATION **/
MELT_EXTERN melt_ptr_t melthook_HOOK_MELT_MAKE_LOCATION (melt_ptr_t melthookdatap, const char* meltinp0_FILENAME, long meltinp1_LINENO);

/*declare MELT called hook #4 HOOK_PATMACRO_EXPORTER **/
MELT_EXTERN void melthook_HOOK_PATMACRO_EXPORTER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_SYM, melt_ptr_t meltinp1_MACVAL, melt_ptr_t meltinp2_PATVAL, melt_ptr_t meltinp3_CONTENV);

/*declare MELT called hook #5 HOOK_SYMBOL_IMPORTER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_SYMBOL_IMPORTER (melt_ptr_t melthookdatap, const char* meltinp0_SYMNAMESTR, const char* meltinp1_MODULENAMESTR, melt_ptr_t meltinp2_PARENV);

/*declare MELT called hook #6 HOOK_VALUE_EXPORTER **/
MELT_EXTERN void melthook_HOOK_VALUE_EXPORTER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_SYM, melt_ptr_t meltinp1_VAL, melt_ptr_t meltinp2_CONTENV);

/*** end of 7 MELT called hook declarations ***/

/*** no extra MELT c-headers ***/




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_1_WARMELTmiMACRO_REGISTER_DIAG_GIMPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_2_WARMELTmiMACRO_REGISTER_DIAG_GIMPLE_SEQ(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_3_WARMELTmiMACRO_REGISTER_DIAG_TREE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_4_WARMELTmiMACRO_REGISTER_DIAG_BASIC_BLOCK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_5_WARMELTmiMACRO_REGISTER_DIAG_EDGE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_6_WARMELTmiMACRO_DIAGNOSTIC_ARGS_MACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_7_WARMELTmiMACRO_DIAGNOSTIC_EXPAND_MESSAGE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_8_WARMELTmiMACRO_ERROR_AT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_9_WARMELTmiMACRO_FATAL_ERROR_AT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_10_WARMELTmiMACRO_WARNING_AT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_11_WARMELTmiMACRO_INFORM_AT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_12_WARMELTmiMACRO_S_EXPR_WEIGHT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_13_WARMELTmiMACRO_LAMBDA_cl1(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_14_WARMELTmiMACRO_EXPAND_RESTLIST_AS_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_15_WARMELTmiMACRO_EXPAND_RESTLIST_AS_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_16_WARMELTmiMACRO_EXPAND_PAIRLIST_AS_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_17_WARMELTmiMACRO_EXPAND_PAIRLIST_AS_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_18_WARMELTmiMACRO_EXPAND_TUPLE_SLICE_AS_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_19_WARMELTmiMACRO_REGISTER_GENERATOR_DEVICE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_20_WARMELTmiMACRO_EXPAND_APPLY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_21_WARMELTmiMACRO_EXPAND_MSEND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_22_WARMELTmiMACRO_EXPAND_FIELDEXPR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_23_WARMELTmiMACRO_EXPAND_CITERATION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_24_WARMELTmiMACRO_EXPAND_CMATCHEXPR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_25_WARMELTmiMACRO_EXPAND_FUNMATCHEXPR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_26_WARMELTmiMACRO_EXPAND_KEYWORDFUN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_27_WARMELTmiMACRO_MACROEXPAND_1(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_28_WARMELTmiMACRO_LAMBDA_cl2(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_29_WARMELTmiMACRO_EXPAND_PRIMITIVE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_30_WARMELTmiMACRO_EXPAND_HOOK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_31_WARMELTmiMACRO_PATTERNEXPAND_PAIRLIST_AS_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_32_WARMELTmiMACRO_LAMBDA_cl3(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_33_WARMELTmiMACRO_PATMACEXPAND_FOR_MATCHER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_34_WARMELTmiMACRO_LAMBDA_cl4(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_35_WARMELTmiMACRO_LAMBDA_cl5(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_36_WARMELTmiMACRO_PATTERN_WEIGHT_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_37_WARMELTmiMACRO_LAMBDA_cl6(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_38_WARMELTmiMACRO_PATTERNEXPAND_EXPR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_39_WARMELTmiMACRO_PATTERNEXPAND_1(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_40_WARMELTmiMACRO_MACROEXPAND_TOPLEVEL_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_41_WARMELTmiMACRO_LAMBDA_ARG_BINDINGS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_42_WARMELTmiMACRO_INSTALL_INITIAL_MACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_43_WARMELTmiMACRO_INSTALL_INITIAL_PATMACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_44_WARMELTmiMACRO_WARN_IF_REDEFINED(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_45_WARMELTmiMACRO_FLATTEN_FOR_C_CODE_EXPANSION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_46_WARMELTmiMACRO_PARSE_PAIRLIST_C_CODE_EXPANSION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_47_WARMELTmiMACRO_CHECK_C_EXPANSION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_48_WARMELTmiMACRO_MELT_REGISTER_TRANSLATOR_RUNNER_MACROEXPANSIONS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_49_WARMELTmiMACRO_MELT_INVOKE_TRANSLATOR_RUNNER_MACROEXPANSIONS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_50_WARMELTmiMACRO_MEXPAND_DEFPRIMITIVE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_51_WARMELTmiMACRO_MEXPAND_DEFCITERATOR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_52_WARMELTmiMACRO_MEXPAND_DEFCMATCHER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_53_WARMELTmiMACRO_MEXPAND_DEFUNMATCHER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_54_WARMELTmiMACRO_MEXPAND_DEFUN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_55_WARMELTmiMACRO_MELT_DELAYED_MACRO_EXPANDER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_56_WARMELTmiMACRO_MEXPAND_DEFMACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_57_WARMELTmiMACRO_LAMBDA_cl7(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_MACRO_INSTALLER**/

/** declaration of hook melthook_HOOK_MACRO_INSTALLER */
MELT_EXTERN
void melthook_HOOK_MACRO_INSTALLER(melt_ptr_t melthookdatap,
                                   melt_ptr_t meltinp0_MBIND,
                                   melt_ptr_t meltinp1_MEXPCLOS)
;



/** end of declaration for hook melthook_HOOK_MACRO_INSTALLER**/






MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_59_WARMELTmiMACRO_MEXPAND_AT_MACRO_EXPANSION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_60_WARMELTmiMACRO_MEXPAND_DEFVAR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_61_WARMELTmiMACRO_MEXPAND_DEFHOOK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_62_WARMELTmiMACRO_MEXPAND_DEFINE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_63_WARMELTmiMACRO_SCAN_DEFCLASS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_64_WARMELTmiMACRO_MEXPAND_DEFCLASS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_65_WARMELTmiMACRO_PARSE_FIELD_ASSIGNMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_66_WARMELTmiMACRO_LAMBDA_cl8(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_67_WARMELTmiMACRO_MEXPAND_DEFINSTANCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_68_WARMELTmiMACRO_MEXPAND_DEFSELECTOR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_69_WARMELTmiMACRO_MEXPAND_INSTANCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_70_WARMELTmiMACRO_MEXPAND_LOAD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_71_WARMELTmiMACRO_PARSE_FIELD_PATTERN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_72_WARMELTmiMACRO_LAMBDA_cl9(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_73_WARMELTmiMACRO_PATEXPAND_INSTANCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_74_WARMELTmiMACRO_LAMBDA_cl10(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_75_WARMELTmiMACRO_PATEXPAND_OBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_76_WARMELTmiMACRO_LAMBDA_cl11(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_77_WARMELTmiMACRO_MEXPAND_OBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_78_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_79_WARMELTmiMACRO_LAMBDA_cl12(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_80_WARMELTmiMACRO_MEXPAND_CODE_CHUNK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_87_WARMELTmiMACRO_LAMBDA_cl13(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_89_WARMELTmiMACRO_MEXPAND_IF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_93_WARMELTmiMACRO_LAMBDA_cl14(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_95_WARMELTmiMACRO_MEXPAND_COND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_96_WARMELTmiMACRO_LAMBDA_cl15(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_97_WARMELTmiMACRO_MEXPAND_AND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_98_WARMELTmiMACRO_LAMBDA_cl16(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_99_WARMELTmiMACRO_PATEXPAND_AS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_100_WARMELTmiMACRO_MEXPAND_AS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_102_WARMELTmiMACRO_PATEXPAND_AND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_103_WARMELTmiMACRO_MEXPAND_OR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_104_WARMELTmiMACRO_LAMBDA_cl17(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_105_WARMELTmiMACRO_PATEXPAND_OR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_106_WARMELTmiMACRO_MEXPAND_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_107_WARMELTmiMACRO_PATEXPAND_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_108_WARMELTmiMACRO_MEXPAND_DEREF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_109_WARMELTmiMACRO_MEXPANDOBSOLETE_CONTENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_110_WARMELTmiMACRO_MEXPAND_SET_REF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_111_WARMELTmiMACRO_MEXPAND_PLUS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_112_WARMELTmiMACRO_MEXPAND_MINUS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_113_WARMELTmiMACRO_MEXPAND_TIMES(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_114_WARMELTmiMACRO_MEXPAND_DIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_115_WARMELTmiMACRO_MEXPAND_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_116_WARMELTmiMACRO_PATEXPAND_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_117_WARMELTmiMACRO_MEXPAND_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_118_WARMELTmiMACRO_PATEXPAND_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_119_WARMELTmiMACRO_MEXPAND_MATCH(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_120_WARMELTmiMACRO_LAMBDA_cl18(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_121_WARMELTmiMACRO_MEXPAND_MATCHALT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_122_WARMELTmiMACRO_LAMBDA_cl19(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_123_WARMELTmiMACRO_MEXPAND_LETBINDING(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_124_WARMELTmiMACRO_MEXPAND_LET(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_125_WARMELTmiMACRO_LAMBDA_cl20(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_126_WARMELTmiMACRO_LAMBDA_cl21(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_127_WARMELTmiMACRO_YES_RECURSIVELY_CONSTRUCTIBLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_128_WARMELTmiMACRO_MEXPAND_LETREC(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_129_WARMELTmiMACRO_LAMBDA_cl22(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_130_WARMELTmiMACRO_LAMBDA_cl23(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_131_WARMELTmiMACRO_MEXPAND_LAMBDA(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_132_WARMELTmiMACRO_LAMBDA_cl24(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_133_WARMELTmiMACRO_MEXPAND_VARIADIC(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_134_WARMELTmiMACRO_LAMBDA_cl25(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_135_WARMELTmiMACRO_LAMBDA_cl26(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_136_WARMELTmiMACRO_LAMBDA_cl27(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_137_WARMELTmiMACRO_LAMBDA_cl28(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_138_WARMELTmiMACRO_MEXPAND_MULTICALL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_139_WARMELTmiMACRO_LAMBDA_cl29(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_140_WARMELTmiMACRO_LAMBDA_cl30(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_141_WARMELTmiMACRO_MEXPAND_BOX(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_142_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_143_WARMELTmiMACRO_MEXPAND_UNBOX(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_144_WARMELTmiMACRO_MEXPAND_COMMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_145_WARMELTmiMACRO_MEXPAND_CHEADER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_146_WARMELTmiMACRO_MEXPAND_CIMPLEMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_147_WARMELTmiMACRO_MEXPAND_MODULE_IS_GPL_COMPATIBLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_148_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_149_WARMELTmiMACRO_PAIRLIST_TO_RETURN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_150_WARMELTmiMACRO_LAMBDA_cl31(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_151_WARMELTmiMACRO_MEXPAND_PROGN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_152_WARMELTmiMACRO_MEXPAND_RETURN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_153_WARMELTmiMACRO_MEXPAND_FOREVER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_154_WARMELTmiMACRO_LAMBDA_cl32(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_155_WARMELTmiMACRO_MEXPAND_EXIT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_156_WARMELTmiMACRO_LAMBDA_cl33(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_157_WARMELTmiMACRO_MEXPAND_AGAIN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_158_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_159_WARMELTmiMACRO_MEXPAND_ASSERT_MSG(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_160_WARMELTmiMACRO_MEXPAND_THIS_LINE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_FILE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_162_WARMELTmiMACRO_MEXPAND_DEBUG(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_163_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_164_WARMELTmiMACRO_LAMBDA_cl34(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_165_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_CLASS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_168_WARMELTmiMACRO_LAMBDA_cl35(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_169_WARMELTmiMACRO_MEXPAND_EXPORT_SYNONYM(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_170_WARMELTmiMACRO_MEXPAND_CURRENT_MODULE_ENVIRONMENT_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_171_WARMELTmiMACRO_MEXPAND_PARENT_MODULE_ENVIRONMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_172_WARMELTmiMACRO_MEXPAND_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_173_WARMELTmiMACRO_MEXPAND_FETCH_PREDEFINED(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY meltrout_174_WARMELTmiMACRO_MEXPAND_STORE_PREDEFINED(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);


MELT_EXTERN void* melt_start_this_module (void*); /*mandatory start of module*/

MELT_EXTERN const char melt_module_is_gpl_compatible[];

/*declare opaque initial frame: */

typedef Melt_InitialClassyFrame_WARMELTmiMACRO_h550734951 /*opaqueinitialclassy*/ meltinitial_frame_t;


/* define different names when debugging or not */
#if MELT_HAVE_DEBUG
MELT_EXTERN const char meltmodule_WARMELTmiMACRO__melt_have_debug_enabled[];
#define melt_have_debug_string meltmodule_WARMELTmiMACRO__melt_have_debug_enabled
#else /*!MELT_HAVE_DEBUG*/
MELT_EXTERN const char meltmodule_WARMELTmiMACRO__melt_have_debug_disabled[];
#define melt_have_debug_string meltmodule_WARMELTmiMACRO__melt_have_debug_disabled
#endif /*!MELT_HAVE_DEBUG*/



void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_0 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_1 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_2 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_3 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_4 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_5 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_6 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_7 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_8 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_9 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_10 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_11 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_12 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_13 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_14 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_15 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_16 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_17 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_18 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_19 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_20 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_21 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_22 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_23 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_24 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_25 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_26 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_27 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_28 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_29 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_30 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_31 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_32 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_33 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_34 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_35 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_36 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_37 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_38 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_39 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_40 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_41 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_42 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_43 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_44 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_45 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_46 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_47 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_48 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_49 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_50 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_51 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_52 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_53 (meltinitial_frame_t*, char*);
void MELT_MODULE_VISIBILITY meltmod__WARMELTmiMACRO__initialmeltchunk_54 (meltinitial_frame_t*, char*);


/**** warmelt-macro+04.cc implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK fromline 1715 */

    /** start of frame for meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK// fromline 1531
        : public Melt_CallFrameWithValues<63>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<63> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK), clos) {};
        MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK() //the constructor fromline 1605
            : Melt_CallFrameWithValues<63> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<63> (fil,lin, sizeof(MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK)) {};
        MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<63> (fil,lin, sizeof(MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK


    /** end of frame for meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK fromline 1660**/

    /* end of frame for routine meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK fromline 1719 */

    /* classy proc frame meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK */
    MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_EXPR_CHUNK", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5236:/ getarg");
    /*_.SEXPR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V5*/
    meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5237:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5237:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L1*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5237:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5237;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_expr_chunk sexpr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5237:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/
                        meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V7*/
                        meltfptr[6] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5237:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V6*/
            meltfptr[5] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5238:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:5238:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5238:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5238;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5238:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5238:/ clear");
            /*clear*/ /*_#IS_A__L3*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5239:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:5239:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5239:/ cond.else");

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
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5239;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5239:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5239:/ clear");
            /*clear*/ /*_#IS_A__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V12*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V11*/ meltfptr[9] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5240:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V15*/
            meltfptr[5] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5241:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.SLOC__V16*/
            meltfptr[15] = slot;
        };
        ;
        /*_.LIST_FIRST__V17*/
        meltfptr[16] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V15*/ meltfptr[5])));;
        /*^compute*/
        /*_.CURPAIR__V18*/
        meltfptr[17] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V17*/ meltfptr[16])));;
        /*^compute*/
        /*_.CURCHK__V19*/
        meltfptr[18] = (/*nil*/NULL);;
        /*^compute*/
        /*_.GSYM__V20*/
        meltfptr[19] = (/*nil*/NULL);;
        /*^compute*/
        /*_.KCTY__V21*/
        meltfptr[20] = (/*nil*/NULL);;
        /*^compute*/
        /*_.CUREL__V22*/
        meltfptr[21] = (/*nil*/NULL);;
        /*^compute*/
        /*_.PAIR_HEAD__V23*/
        meltfptr[22] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])));;
        MELT_LOCATION("warmelt-macro.melt:5248:/ compute");
        /*_.CUREL__V22*/
        meltfptr[21] = /*_.SETQ___V24*/ meltfptr[23] = /*_.PAIR_HEAD__V23*/ meltfptr[22];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_A__L5*/
        meltfnum[1] =
            melt_is_instance_of((melt_ptr_t)(/*_.CUREL__V22*/ meltfptr[21]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[4])));;
        MELT_LOCATION("warmelt-macro.melt:5249:/ cond");
        /*cond*/
        if (/*_#IS_A__L5*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:5250:/ compute");
                    /*_.GSYM__V20*/
                    meltfptr[19] = /*_.SETQ___V26*/ meltfptr[25] = /*_.CUREL__V22*/ meltfptr[21];;
                    /*_._IFELSE___V25*/
                    meltfptr[24] = /*_.SETQ___V26*/ meltfptr[25];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5249:/ clear");
                    /*clear*/ /*_.SETQ___V26*/
                    meltfptr[25] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-macro.melt:5251:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.SLOC__V16*/ meltfptr[15]), ( "missing symbol in (EXPR_CHUNK <state-symbol> <ctype> <chunk>...)"), (melt_ptr_t)0);
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V25*/
                    meltfptr[24] = 0 ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V27*/
        meltfptr[25] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])));;
        MELT_LOCATION("warmelt-macro.melt:5252:/ compute");
        /*_.CURPAIR__V18*/
        meltfptr[17] = /*_.SETQ___V28*/ meltfptr[27] = /*_.PAIR_TAIL__V27*/ meltfptr[25];;
        /*_.PAIR_HEAD__V29*/
        meltfptr[28] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])));;
        MELT_LOCATION("warmelt-macro.melt:5253:/ compute");
        /*_.KCTY__V21*/
        meltfptr[20] = /*_.SETQ___V30*/ meltfptr[29] = /*_.PAIR_HEAD__V29*/ meltfptr[28];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_NOT_A__L6*/
        meltfnum[0] =
            !melt_is_instance_of((melt_ptr_t)(/*_.KCTY__V21*/ meltfptr[20]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[5])));;
        MELT_LOCATION("warmelt-macro.melt:5254:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L6*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5255:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.SLOC__V16*/ meltfptr[15]), ( "missing type keyword in (EXPR_CHUNK <state-symbol> <ctype> <chunk>...)"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5256:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:5256:/ locexp");
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
                    /*finalret*/
                    goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-macro.melt:5254:/ quasiblock");


                    /*_.PROGN___V33*/
                    meltfptr[32] = /*_.RETURN___V32*/ meltfptr[31];;
                    /*^compute*/
                    /*_._IF___V31*/
                    meltfptr[30] = /*_.PROGN___V33*/ meltfptr[32];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5254:/ clear");
                    /*clear*/ /*_.RETURN___V32*/
                    meltfptr[31] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V33*/
                    meltfptr[32] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V31*/ meltfptr[30] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V34*/
        meltfptr[31] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])));;
        MELT_LOCATION("warmelt-macro.melt:5257:/ compute");
        /*_.CURPAIR__V18*/
        meltfptr[17] = /*_.SETQ___V35*/ meltfptr[32] = /*_.PAIR_TAIL__V34*/ meltfptr[31];;
        MELT_LOCATION("warmelt-macro.melt:5258:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.KCTY__V21*/ meltfptr[20]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SYMB_DATA");
            /*_.CTY__V37*/
            meltfptr[36] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5259:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.KCTY__V21*/ meltfptr[20]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
            /*_.TYPKNAM__V38*/
            meltfptr[37] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_NOT_A__L7*/
        meltfnum[6] =
            !melt_is_instance_of((melt_ptr_t)(/*_.CTY__V37*/ meltfptr[36]), (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[6])));;
        MELT_LOCATION("warmelt-macro.melt:5261:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L7*/ meltfnum[6]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:5262:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L8*/
                        meltfnum[7] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-macro.melt:5262:/ cond");
                        /*cond*/
                        if (/*_#MELT_NEED_DBG__L8*/ meltfnum[7]) /*then*/
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
                                    MELT_LOCATION("warmelt-macro.melt:5262:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[8];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 5262;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "mexpand_expr_chunk bad kcty=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.KCTY__V21*/ meltfptr[20];
                                        /*_.MELT_DEBUG_FUN__V41*/
                                        meltfptr[40] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V40*/
                                    meltfptr[39] = /*_.MELT_DEBUG_FUN__V41*/ meltfptr[40];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:5262:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L9*/
                                    meltfnum[8] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V41*/
                                    meltfptr[40] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V40*/ meltfptr[39] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-macro.melt:5262:/ locexp");
                            /*void*/
                            (void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L8*/
                        meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V40*/
                        meltfptr[39] = 0 ;
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
                    MELT_LOCATION("warmelt-macro.melt:5263:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[2];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_cstring =  "bad type keyword for EXPR_CHUNK";
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.TYPKNAM__V38*/ meltfptr[37];
                        /*_.ERROR_AT__V42*/
                        meltfptr[40] =  melt_apply ((meltclosure_ptr_t)((/*!ERROR_AT*/ meltfrout->tabval[7])), (melt_ptr_t)(/*_.SLOC__V16*/ meltfptr[15]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5264:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:5264:/ locexp");
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
                    /*finalret*/
                    goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-macro.melt:5261:/ quasiblock");


                    /*_.PROGN___V44*/
                    meltfptr[43] = /*_.RETURN___V43*/ meltfptr[39];;
                    /*^compute*/
                    /*_._IF___V39*/
                    meltfptr[38] = /*_.PROGN___V44*/ meltfptr[43];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5261:/ clear");
                    /*clear*/ /*_.ERROR_AT__V42*/
                    meltfptr[40] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V43*/
                    meltfptr[39] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V44*/
                    meltfptr[43] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V39*/ meltfptr[38] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5266:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.CTY__V37*/ meltfptr[36]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "CTYPE_KEYWORD");
            /*_.CTYPE_KEYWORD__V45*/
            meltfptr[40] = slot;
        };
        ;
        /*_#eqeq__L10*/
        meltfnum[8] =
            ((/*_.CTYPE_KEYWORD__V45*/ meltfptr[40]) == (/*_.KCTY__V21*/ meltfptr[20]));;
        MELT_LOCATION("warmelt-macro.melt:5266:/ cond");
        /*cond*/
        if (/*_#eqeq__L10*/ meltfnum[8]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V46*/ meltfptr[39] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-macro.melt:5266:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5269:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.CTY__V37*/ meltfptr[36]) /*=obj*/;
                        melt_object_get_field(slot,obj, 10, "CTYPE_ALTKEYWORD");
                        /*_.CTYPE_ALTKEYWORD__V47*/
                        meltfptr[43] = slot;
                    };
                    ;
                    /*_#eqeq__L11*/
                    meltfnum[7] =
                        ((/*_.CTYPE_ALTKEYWORD__V47*/ meltfptr[43]) == (/*_.KCTY__V21*/ meltfptr[20]));;
                    MELT_LOCATION("warmelt-macro.melt:5269:/ cond");
                    /*cond*/
                    if (/*_#eqeq__L11*/ meltfnum[7]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:5271:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CTY__V37*/ meltfptr[36]),
                                                                  (melt_ptr_t)((/*!CLASS_QUASI_CTYPE*/ meltfrout->tabval[9])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CTY__V37*/ meltfptr[36]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 2, "CTYPE_KEYWORD");
                                            /*_.CTYPE_KEYWORD__V49*/
                                            meltfptr[48] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.CTYPE_KEYWORD__V49*/ meltfptr[48] =  /*reallynil*/ NULL ;;
                                    }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:5271:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CTYPE_KEYWORD__V49*/ meltfptr[48]),
                                                                  (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[10])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CTYPE_KEYWORD__V49*/ meltfptr[48]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                            /*_.NAMED_NAME__V50*/
                                            meltfptr[49] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.NAMED_NAME__V50*/ meltfptr[49] =  /*reallynil*/ NULL ;;
                                    }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-macro.melt:5270:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[3];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_cstring =  "using obsolete ctype keyword $1 in EXPR_CHUNK, preferring $2";
                                    /*^apply.arg*/
                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.TYPKNAM__V38*/ meltfptr[37];
                                    /*^apply.arg*/
                                    argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.NAMED_NAME__V50*/ meltfptr[49];
                                    /*_.WARNING_AT__V51*/
                                    meltfptr[50] =  melt_apply ((meltclosure_ptr_t)((/*!WARNING_AT*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.SLOC__V16*/ meltfptr[15]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                /*_._IFELSE___V48*/
                                meltfptr[47] = /*_.WARNING_AT__V51*/ meltfptr[50];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:5269:/ clear");
                                /*clear*/ /*_.CTYPE_KEYWORD__V49*/
                                meltfptr[48] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.NAMED_NAME__V50*/
                                meltfptr[49] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.WARNING_AT__V51*/
                                meltfptr[50] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {


#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-macro.melt:5274:/ cppif.then");
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
                                    /*_#MELT_NEED_DBG__L12*/
                                    meltfnum[11] =
                                        /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                        (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                        0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                        ;;
                                    MELT_LOCATION("warmelt-macro.melt:5274:/ cond");
                                    /*cond*/
                                    if (/*_#MELT_NEED_DBG__L12*/ meltfnum[11]) /*then*/
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
                                                MELT_LOCATION("warmelt-macro.melt:5274:/ apply");
                                                /*apply*/
                                                {
                                                    union meltparam_un argtab[5];
                                                    memset(&argtab, 0, sizeof(argtab));
                                                    /*^apply.arg*/
                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L13*/ meltfnum[12];
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 5274;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_cstring =  "mexpand_expr_chunk strange kcty=";
                                                    /*^apply.arg*/
                                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.KCTY__V21*/ meltfptr[20];
                                                    /*_.MELT_DEBUG_FUN__V53*/
                                                    meltfptr[49] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IF___V52*/
                                                meltfptr[48] = /*_.MELT_DEBUG_FUN__V53*/ meltfptr[49];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-macro.melt:5274:/ clear");
                                                /*clear*/ /*_#MELT_CALLCOUNT__L13*/
                                                meltfnum[12] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_.MELT_DEBUG_FUN__V53*/
                                                meltfptr[49] = 0 ;
                                            }
                                            ;
                                        }
                                    else    /*^cond.else*/
                                        {

                                            /*_._IF___V52*/ meltfptr[48] =  /*reallynil*/ NULL ;;
                                        }
                                    ;

                                    {
                                        MELT_LOCATION("warmelt-macro.melt:5274:/ locexp");
                                        /*void*/
                                        (void)0;
                                    }
                                    ;
                                    /*^quasiblock*/


                                    /*epilog*/

                                    /*^clear*/
                                    /*clear*/ /*_#MELT_NEED_DBG__L12*/
                                    meltfnum[11] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IF___V52*/
                                    meltfptr[48] = 0 ;
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
                                MELT_LOCATION("warmelt-macro.melt:5275:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[2];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_cstring =  "invalid type keyword $1 for EXPR_CHUNK";
                                    /*^apply.arg*/
                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.TYPKNAM__V38*/ meltfptr[37];
                                    /*_.ERROR_AT__V54*/
                                    meltfptr[50] =  melt_apply ((meltclosure_ptr_t)((/*!ERROR_AT*/ meltfrout->tabval[7])), (melt_ptr_t)(/*_.SLOC__V16*/ meltfptr[15]), (MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-macro.melt:5277:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] = (/*nil*/NULL);;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:5277:/ locexp");
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
                                /*finalret*/
                                goto meltlabend_rout ;
                                MELT_LOCATION("warmelt-macro.melt:5273:/ quasiblock");


                                /*_.PROGN___V56*/
                                meltfptr[48] = /*_.RETURN___V55*/ meltfptr[49];;
                                /*^compute*/
                                /*_._IFELSE___V48*/
                                meltfptr[47] = /*_.PROGN___V56*/ meltfptr[48];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:5269:/ clear");
                                /*clear*/ /*_.ERROR_AT__V54*/
                                meltfptr[50] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.RETURN___V55*/
                                meltfptr[49] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V56*/
                                meltfptr[48] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V46*/
                    meltfptr[39] = /*_._IFELSE___V48*/ meltfptr[47];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5266:/ clear");
                    /*clear*/ /*_.CTYPE_ALTKEYWORD__V47*/
                    meltfptr[43] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#eqeq__L11*/
                    meltfnum[7] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V48*/
                    meltfptr[47] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:5279:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5280:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.SLOC__V16*/ meltfptr[15];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.TUP__V58*/
            meltfptr[49] =  melt_apply ((meltclosure_ptr_t)((/*!EXPAND_MACROSTRING_PAIRS*/ meltfrout->tabval[11])), (melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5281:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_EXPRCHUNK*/ meltfrout->tabval[12])), (5), "CLASS_SOURCE_EXPRCHUNK");
            /*_.INST__V60*/
            meltfptr[43] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V60*/ meltfptr[43])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V60*/ meltfptr[43]), (1), (/*_.SLOC__V16*/ meltfptr[15]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SCH_GENSYM", melt_magic_discr((melt_ptr_t)(/*_.INST__V60*/ meltfptr[43])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V60*/ meltfptr[43]), (2), (/*_.GSYM__V20*/ meltfptr[19]), "SCH_GENSYM");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SCH_CHUNKS", melt_magic_discr((melt_ptr_t)(/*_.INST__V60*/ meltfptr[43])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V60*/ meltfptr[43]), (3), (/*_.TUP__V58*/ meltfptr[49]), "SCH_CHUNKS");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SXCH_CTYPE", melt_magic_discr((melt_ptr_t)(/*_.INST__V60*/ meltfptr[43])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V60*/ meltfptr[43]), (4), (/*_.CTY__V37*/ meltfptr[36]), "SXCH_CTYPE");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V60*/ meltfptr[43], "newly made instance");
        ;
        /*_.MRES__V59*/
        meltfptr[48] = /*_.INST__V60*/ meltfptr[43];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5288:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L14*/
            meltfnum[12] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5288:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L14*/ meltfnum[12]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L15*/ meltfnum[11] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:5288:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L15*/ meltfnum[11];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5288;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_expr_chunk return mres=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MRES__V59*/ meltfptr[48];
                            /*_.MELT_DEBUG_FUN__V62*/
                            meltfptr[61] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V61*/
                        meltfptr[47] = /*_.MELT_DEBUG_FUN__V62*/ meltfptr[61];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5288:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L15*/
                        meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V62*/
                        meltfptr[61] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V61*/ meltfptr[47] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5288:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L14*/
            meltfnum[12] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V61*/
            meltfptr[47] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5289:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.MRES__V59*/ meltfptr[48];;

        {
            MELT_LOCATION("warmelt-macro.melt:5289:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*_.LET___V57*/
        meltfptr[50] = /*_.RETURN___V63*/ meltfptr[61];;

        MELT_LOCATION("warmelt-macro.melt:5279:/ clear");
        /*clear*/ /*_.TUP__V58*/
        meltfptr[49] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MRES__V59*/
        meltfptr[48] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V63*/
        meltfptr[61] = 0 ;
        /*_.LET___V36*/
        meltfptr[35] = /*_.LET___V57*/ meltfptr[50];;

        MELT_LOCATION("warmelt-macro.melt:5258:/ clear");
        /*clear*/ /*_.CTY__V37*/
        meltfptr[36] = 0 ;
        /*^clear*/
        /*clear*/ /*_.TYPKNAM__V38*/
        meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L7*/
        meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V39*/
        meltfptr[38] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CTYPE_KEYWORD__V45*/
        meltfptr[40] = 0 ;
        /*^clear*/
        /*clear*/ /*_#eqeq__L10*/
        meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V46*/
        meltfptr[39] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V57*/
        meltfptr[50] = 0 ;
        /*_.LET___V14*/
        meltfptr[12] = /*_.LET___V36*/ meltfptr[35];;

        MELT_LOCATION("warmelt-macro.melt:5240:/ clear");
        /*clear*/ /*_.CONT__V15*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SLOC__V16*/
        meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V17*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURCHK__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.GSYM__V20*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.KCTY__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CUREL__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_HEAD__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L5*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V27*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_HEAD__V29*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V30*/
        meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L6*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V31*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V34*/
        meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V35*/
        meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V36*/
        meltfptr[35] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5236:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V14*/ meltfptr[12];;

        {
            MELT_LOCATION("warmelt-macro.melt:5236:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V8*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V11*/
        meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V14*/
        meltfptr[12] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_EXPR_CHUNK", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_81_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS fromline 1715 */

    /** start of frame for meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS// fromline 1531
        : public Melt_CallFrameWithValues<46>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<46> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS), clos) {};
        MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS() //the constructor fromline 1605
            : Melt_CallFrameWithValues<46> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<46> (fil,lin, sizeof(MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS)) {};
        MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<46> (fil,lin, sizeof(MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS


    /** end of frame for meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS fromline 1660**/

    /* end of frame for routine meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS fromline 1719 */

    /* classy proc frame meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS */
    MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_UNSAFE_PUT_FIELDS", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5305:/ getarg");
    /*_.SEXPR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V5*/
    meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5306:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:5306:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5306:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5306;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                            meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V7*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5306:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                        meltfptr[7] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V6*/
            meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5306:/ clear");
            /*clear*/ /*_#IS_A__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V7*/
            meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V6*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5307:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:5307:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5307:/ cond.else");

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
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5307;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                            meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V10*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5307:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                        meltfptr[10] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V9*/
            meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5307:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V10*/
            meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V9*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L3*/
        meltfnum[0] =
            ((/*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:5308:/ cond");
        /*cond*/
        if (/*_#NULL__L3*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V13*/ meltfptr[6] = (/*!MACROEXPAND_1*/ meltfrout->tabval[3]);;
                    /*_._IF___V12*/
                    meltfptr[10] = /*_.SETQ___V13*/ meltfptr[6];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5308:/ clear");
                    /*clear*/ /*_.SETQ___V13*/
                    meltfptr[6] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V12*/ meltfptr[10] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5309:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_CLOSURE__L4*/
            meltfnum[3] =
                (melt_magic_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) == MELTOBMAG_CLOSURE);;
            MELT_LOCATION("warmelt-macro.melt:5309:/ cond");
            /*cond*/
            if (/*_#IS_CLOSURE__L4*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5309:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check mexpander";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5309;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/
                        meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5309:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                        meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/
            meltfptr[6] = /*_._IFELSE___V15*/ meltfptr[14];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5309:/ clear");
            /*clear*/ /*_#IS_CLOSURE__L4*/
            meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[14] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5310:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L5*/
            meltfnum[3] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:5310:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L5*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V18*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5310:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5310;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                            /*_.MELT_ASSERT_FAILURE_FUN__V19*/
                            meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V18*/
                        meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V19*/ meltfptr[18];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5310:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V19*/
                        meltfptr[18] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V17*/
            meltfptr[15] = /*_._IFELSE___V18*/ meltfptr[14];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5310:/ clear");
            /*clear*/ /*_#IS_OBJECT__L5*/
            meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V18*/
            meltfptr[14] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V17*/ meltfptr[15] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5311:/ quasiblock");


        /*_.FIELDS__V21*/
        meltfptr[14] = (/*nil*/NULL);;
        /*^compute*/
        /*_.FIELDNAMS__V22*/
        meltfptr[21] = (/*nil*/NULL);;
        MELT_LOCATION("warmelt-macro.melt:5313:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V23*/
            meltfptr[22] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5314:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V24*/
            meltfptr[23] = slot;
        };
        ;
        /*_.LIST_FIRST__V25*/
        meltfptr[24] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V23*/ meltfptr[22])));;
        /*^compute*/
        /*_.CURPAIR__V26*/
        meltfptr[25] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V25*/ meltfptr[24])));;
        /*^compute*/
        /*_.OBJSRC__V27*/
        meltfptr[26] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V26*/ meltfptr[25])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_A__L6*/
        meltfnum[3] =
            melt_is_instance_of((melt_ptr_t)(/*_.OBJSRC__V27*/ meltfptr[26]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
        MELT_LOCATION("warmelt-macro.melt:5318:/ cond");
        /*cond*/
        if (/*_#IS_A__L6*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5319:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                        /*_.MEXPANDER__V29*/
                        meltfptr[28] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.OBJSRC__V27*/ meltfptr[26]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_.OBJEXP__V28*/
                    meltfptr[27] = /*_.MEXPANDER__V29*/ meltfptr[28];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5318:/ clear");
                    /*clear*/ /*_.MEXPANDER__V29*/
                    meltfptr[28] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OBJEXP__V28*/ meltfptr[27] = /*_.OBJSRC__V27*/ meltfptr[26];;
            }
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V30*/
        meltfptr[28] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V26*/ meltfptr[25])));;
        MELT_LOCATION("warmelt-macro.melt:5322:/ compute");
        /*_.CURPAIR__V26*/
        meltfptr[25] = /*_.SETQ___V31*/ meltfptr[30] = /*_.PAIR_TAIL__V30*/ meltfptr[28];;
        MELT_LOCATION("warmelt-macro.melt:5323:/ quasiblock");


        /*_.FLDLIST__V33*/
        meltfptr[32] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[4]))));;
        MELT_LOCATION("warmelt-macro.melt:5325:/ loop");
        /*loop*/
        {
meltlabloop_INSLOOP_6:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;

                MELT_CHECK_SIGNAL();
                ;
                /*_#IS_PAIR__L7*/
                meltfnum[6] =
                    (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V26*/ meltfptr[25])) == MELTOBMAG_PAIR);;
                /*^compute*/
                /*_#NOT__L8*/
                meltfnum[7] =
                    (!(/*_#IS_PAIR__L7*/ meltfnum[6]));;
                MELT_LOCATION("warmelt-macro.melt:5326:/ cond");
                /*cond*/
                if (/*_#NOT__L8*/ meltfnum[7]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^quasiblock*/


                            /*^compute*/
                            /*_.INSLOOP__V35*/ meltfptr[34] =  /*reallynil*/ NULL ;;

                            /*^exit*/
                            /*exit*/
                            {
                                goto meltlabexit_INSLOOP_6;
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    } /*noelse*/
                ;
                MELT_LOCATION("warmelt-macro.melt:5327:/ quasiblock");


                /*_.CURFKW__V36*/
                meltfptr[35] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V26*/ meltfptr[25])));;

                MELT_CHECK_SIGNAL();
                ;
                /*^compute*/
                /*_#IS_NOT_A__L9*/
                meltfnum[8] =
                    !melt_is_instance_of((melt_ptr_t)(/*_.CURFKW__V36*/ meltfptr[35]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[5])));;
                MELT_LOCATION("warmelt-macro.melt:5328:/ cond");
                /*cond*/
                if (/*_#IS_NOT_A__L9*/ meltfnum[8]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:5329:/ locexp");
                                /* error_plain */
                                melt_error_str((melt_ptr_t)(/*_.LOC__V24*/ meltfptr[23]), ( "expecting heyword in UNSAFE_PUT_FIELDS"), (melt_ptr_t)0);
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    } /*noelse*/
                ;
                /*_.PAIR_TAIL__V37*/
                meltfptr[36] =
                    (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V26*/ meltfptr[25])));;
                MELT_LOCATION("warmelt-macro.melt:5330:/ compute");
                /*_.CURPAIR__V26*/
                meltfptr[25] = /*_.SETQ___V38*/ meltfptr[37] = /*_.PAIR_TAIL__V37*/ meltfptr[36];;
                MELT_LOCATION("warmelt-macro.melt:5331:/ quasiblock");


                /*_.CUREXP__V39*/
                meltfptr[38] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V26*/ meltfptr[25])));;
                MELT_LOCATION("warmelt-macro.melt:5332:/ quasiblock");



                MELT_CHECK_SIGNAL();
                ;
                /*^apply*/
                /*apply*/
                {
                    union meltparam_un argtab[6];
                    memset(&argtab, 0, sizeof(argtab));
                    /*^apply.arg*/
                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LOC__V24*/ meltfptr[23];
                    /*^apply.arg*/
                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.CURFKW__V36*/ meltfptr[35];
                    /*^apply.arg*/
                    argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.CUREXP__V39*/ meltfptr[38];
                    /*^apply.arg*/
                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                    /*^apply.arg*/
                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
                    /*^apply.arg*/
                    argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                    /*_.FLDA__V40*/
                    meltfptr[39] =  melt_apply ((meltclosure_ptr_t)((/*!PARSE_FIELD_ASSIGNMENT*/ meltfrout->tabval[6])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                }
                ;

                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-macro.melt:5333:/ cond");
                /*cond*/
                if (/*_.FLDA__V40*/ meltfptr[39]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:5334:/ locexp");
                                meltgc_append_list((melt_ptr_t)(/*_.FLDLIST__V33*/ meltfptr[32]), (melt_ptr_t)(/*_.FLDA__V40*/ meltfptr[39]));
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    } /*noelse*/
                ;

                MELT_LOCATION("warmelt-macro.melt:5332:/ clear");
                /*clear*/ /*_.FLDA__V40*/
                meltfptr[39] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:5331:/ clear");
                /*clear*/ /*_.CUREXP__V39*/
                meltfptr[38] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:5327:/ clear");
                /*clear*/ /*_.CURFKW__V36*/
                meltfptr[35] = 0 ;
                /*^clear*/
                /*clear*/ /*_#IS_NOT_A__L9*/
                meltfnum[8] = 0 ;
                /*^clear*/
                /*clear*/ /*_.PAIR_TAIL__V37*/
                meltfptr[36] = 0 ;
                /*^clear*/
                /*clear*/ /*_.SETQ___V38*/
                meltfptr[37] = 0 ;
                /*_.PAIR_TAIL__V41*/
                meltfptr[39] =
                    (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V26*/ meltfptr[25])));;
                MELT_LOCATION("warmelt-macro.melt:5336:/ compute");
                /*_.CURPAIR__V26*/
                meltfptr[25] = /*_.SETQ___V42*/ meltfptr[38] = /*_.PAIR_TAIL__V41*/ meltfptr[39];;

                MELT_CHECK_SIGNAL();
                ;
                /*epilog*/

                MELT_LOCATION("warmelt-macro.melt:5325:/ clear");
                /*clear*/ /*_#IS_PAIR__L7*/
                meltfnum[6] = 0 ;
                /*^clear*/
                /*clear*/ /*_#NOT__L8*/
                meltfnum[7] = 0 ;
                /*^clear*/
                /*clear*/ /*_.PAIR_TAIL__V41*/
                meltfptr[39] = 0 ;
                /*^clear*/
                /*clear*/ /*_.SETQ___V42*/
                meltfptr[38] = 0 ;
            }
            ;
            ;
            goto meltlabloop_INSLOOP_6;
meltlabexit_INSLOOP_6:;/*^loopepilog*/
            /*loopepilog*/
            /*_.FOREVER___V34*/
            meltfptr[33] = /*_.INSLOOP__V35*/ meltfptr[34];;
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:5338:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[8]);
            /*_.FASTUP__V44*/
            meltfptr[36] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[7])), (melt_ptr_t)(/*_.FLDLIST__V33*/ meltfptr[32]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5339:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_UNSAFE_PUT_FIELDS*/ meltfrout->tabval[9])), (4), "CLASS_SOURCE_UNSAFE_PUT_FIELDS");
            /*_.INST__V46*/
            meltfptr[39] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V46*/ meltfptr[39])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V46*/ meltfptr[39]), (1), (/*_.LOC__V24*/ meltfptr[23]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SUPUT_OBJ", melt_magic_discr((melt_ptr_t)(/*_.INST__V46*/ meltfptr[39])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V46*/ meltfptr[39]), (2), (/*_.OBJEXP__V28*/ meltfptr[27]), "SUPUT_OBJ");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SUPUT_FIELDS", melt_magic_discr((melt_ptr_t)(/*_.INST__V46*/ meltfptr[39])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V46*/ meltfptr[39]), (3), (/*_.FASTUP__V44*/ meltfptr[36]), "SUPUT_FIELDS");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V46*/ meltfptr[39], "newly made instance");
        ;
        /*_.INST___V45*/
        meltfptr[37] = /*_.INST__V46*/ meltfptr[39];;
        /*^compute*/
        /*_.LET___V43*/
        meltfptr[35] = /*_.INST___V45*/ meltfptr[37];;

        MELT_LOCATION("warmelt-macro.melt:5338:/ clear");
        /*clear*/ /*_.FASTUP__V44*/
        meltfptr[36] = 0 ;
        /*^clear*/
        /*clear*/ /*_.INST___V45*/
        meltfptr[37] = 0 ;
        /*_.LET___V32*/
        meltfptr[31] = /*_.LET___V43*/ meltfptr[35];;

        MELT_LOCATION("warmelt-macro.melt:5323:/ clear");
        /*clear*/ /*_.FLDLIST__V33*/
        meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V34*/
        meltfptr[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V43*/
        meltfptr[35] = 0 ;
        /*_.LET___V20*/
        meltfptr[18] = /*_.LET___V32*/ meltfptr[31];;

        MELT_LOCATION("warmelt-macro.melt:5311:/ clear");
        /*clear*/ /*_.FIELDS__V21*/
        meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FIELDNAMS__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CONT__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OBJSRC__V27*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L6*/
        meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OBJEXP__V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V30*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V31*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V32*/
        meltfptr[31] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5305:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V20*/ meltfptr[18];;

        {
            MELT_LOCATION("warmelt-macro.melt:5305:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V6*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V9*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L3*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V12*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V17*/
        meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V20*/
        meltfptr[18] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_UNSAFE_PUT_FIELDS", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_82_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS fromline 1715 */

    /** start of frame for meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS// fromline 1531
        : public Melt_CallFrameWithValues<40>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<40> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS), clos) {};
        MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS() //the constructor fromline 1605
            : Melt_CallFrameWithValues<40> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<40> (fil,lin, sizeof(MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS)) {};
        MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<40> (fil,lin, sizeof(MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS


    /** end of frame for meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS fromline 1660**/

    /* end of frame for routine meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS fromline 1719 */

    /* classy proc frame meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS */
    MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_PUT_FIELDS", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5353:/ getarg");
    /*_.SEXPR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V5*/
    meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5354:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:5354:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5354:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5354;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                            meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V7*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5354:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                        meltfptr[7] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V6*/
            meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5354:/ clear");
            /*clear*/ /*_#IS_A__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V7*/
            meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V6*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5355:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:5355:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5355:/ cond.else");

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
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5355;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                            meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V10*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5355:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                        meltfptr[10] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V9*/
            meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5355:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V10*/
            meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V9*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L3*/
        meltfnum[0] =
            ((/*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:5356:/ cond");
        /*cond*/
        if (/*_#NULL__L3*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V13*/ meltfptr[6] = (/*!MACROEXPAND_1*/ meltfrout->tabval[3]);;
                    /*_._IF___V12*/
                    meltfptr[10] = /*_.SETQ___V13*/ meltfptr[6];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5356:/ clear");
                    /*clear*/ /*_.SETQ___V13*/
                    meltfptr[6] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V12*/ meltfptr[10] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:5357:/ quasiblock");


        /*_.FIELDS__V15*/
        meltfptr[14] = (/*nil*/NULL);;
        /*^compute*/
        /*_.FIELDNAMS__V16*/
        meltfptr[15] = (/*nil*/NULL);;
        MELT_LOCATION("warmelt-macro.melt:5359:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V17*/
            meltfptr[16] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5360:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V18*/
            meltfptr[17] = slot;
        };
        ;
        /*_.LIST_FIRST__V19*/
        meltfptr[18] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V17*/ meltfptr[16])));;
        /*^compute*/
        /*_.CURPAIR__V20*/
        meltfptr[19] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V19*/ meltfptr[18])));;
        /*^compute*/
        /*_.OBJSRC__V21*/
        meltfptr[20] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_A__L4*/
        meltfnum[3] =
            melt_is_instance_of((melt_ptr_t)(/*_.OBJSRC__V21*/ meltfptr[20]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
        MELT_LOCATION("warmelt-macro.melt:5364:/ cond");
        /*cond*/
        if (/*_#IS_A__L4*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5365:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                        /*_.MEXPANDER__V23*/
                        meltfptr[22] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.OBJSRC__V21*/ meltfptr[20]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_.OBJEXP__V22*/
                    meltfptr[21] = /*_.MEXPANDER__V23*/ meltfptr[22];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5364:/ clear");
                    /*clear*/ /*_.MEXPANDER__V23*/
                    meltfptr[22] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OBJEXP__V22*/ meltfptr[21] = /*_.OBJSRC__V21*/ meltfptr[20];;
            }
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V24*/
        meltfptr[22] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
        MELT_LOCATION("warmelt-macro.melt:5368:/ compute");
        /*_.CURPAIR__V20*/
        meltfptr[19] = /*_.SETQ___V25*/ meltfptr[24] = /*_.PAIR_TAIL__V24*/ meltfptr[22];;
        MELT_LOCATION("warmelt-macro.melt:5369:/ quasiblock");


        /*_.FLDLIST__V27*/
        meltfptr[26] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[4]))));;
        MELT_LOCATION("warmelt-macro.melt:5371:/ loop");
        /*loop*/
        {
meltlabloop_INSLOOP_7:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;

                MELT_CHECK_SIGNAL();
                ;
                /*_#IS_PAIR__L5*/
                meltfnum[4] =
                    (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])) == MELTOBMAG_PAIR);;
                /*^compute*/
                /*_#NOT__L6*/
                meltfnum[5] =
                    (!(/*_#IS_PAIR__L5*/ meltfnum[4]));;
                MELT_LOCATION("warmelt-macro.melt:5372:/ cond");
                /*cond*/
                if (/*_#NOT__L6*/ meltfnum[5]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^quasiblock*/


                            /*^compute*/
                            /*_.INSLOOP__V29*/ meltfptr[28] =  /*reallynil*/ NULL ;;

                            /*^exit*/
                            /*exit*/
                            {
                                goto meltlabexit_INSLOOP_7;
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    } /*noelse*/
                ;
                MELT_LOCATION("warmelt-macro.melt:5373:/ quasiblock");


                /*_.CURFKW__V30*/
                meltfptr[29] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;

                MELT_CHECK_SIGNAL();
                ;
                /*^compute*/
                /*_#IS_NOT_A__L7*/
                meltfnum[6] =
                    !melt_is_instance_of((melt_ptr_t)(/*_.CURFKW__V30*/ meltfptr[29]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[5])));;
                MELT_LOCATION("warmelt-macro.melt:5374:/ cond");
                /*cond*/
                if (/*_#IS_NOT_A__L7*/ meltfnum[6]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:5375:/ locexp");
                                /* error_plain */
                                melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "expecting heyword in PUT_FIELDS"), (melt_ptr_t)0);
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    } /*noelse*/
                ;
                /*_.PAIR_TAIL__V31*/
                meltfptr[30] =
                    (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
                MELT_LOCATION("warmelt-macro.melt:5376:/ compute");
                /*_.CURPAIR__V20*/
                meltfptr[19] = /*_.SETQ___V32*/ meltfptr[31] = /*_.PAIR_TAIL__V31*/ meltfptr[30];;
                MELT_LOCATION("warmelt-macro.melt:5377:/ quasiblock");


                /*_.CUREXP__V33*/
                meltfptr[32] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
                MELT_LOCATION("warmelt-macro.melt:5378:/ quasiblock");



                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-macro.melt:5379:/ apply");
                /*apply*/
                {
                    union meltparam_un argtab[6];
                    memset(&argtab, 0, sizeof(argtab));
                    /*^apply.arg*/
                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LOC__V18*/ meltfptr[17];
                    /*^apply.arg*/
                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.CURFKW__V30*/ meltfptr[29];
                    /*^apply.arg*/
                    argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.CUREXP__V33*/ meltfptr[32];
                    /*^apply.arg*/
                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                    /*^apply.arg*/
                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
                    /*^apply.arg*/
                    argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                    /*_.FLDA__V34*/
                    meltfptr[33] =  melt_apply ((meltclosure_ptr_t)((/*!PARSE_FIELD_ASSIGNMENT*/ meltfrout->tabval[6])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                }
                ;

                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-macro.melt:5380:/ cond");
                /*cond*/
                if (/*_.FLDA__V34*/ meltfptr[33]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:5381:/ locexp");
                                meltgc_append_list((melt_ptr_t)(/*_.FLDLIST__V27*/ meltfptr[26]), (melt_ptr_t)(/*_.FLDA__V34*/ meltfptr[33]));
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    } /*noelse*/
                ;

                MELT_LOCATION("warmelt-macro.melt:5378:/ clear");
                /*clear*/ /*_.FLDA__V34*/
                meltfptr[33] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:5377:/ clear");
                /*clear*/ /*_.CUREXP__V33*/
                meltfptr[32] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:5373:/ clear");
                /*clear*/ /*_.CURFKW__V30*/
                meltfptr[29] = 0 ;
                /*^clear*/
                /*clear*/ /*_#IS_NOT_A__L7*/
                meltfnum[6] = 0 ;
                /*^clear*/
                /*clear*/ /*_.PAIR_TAIL__V31*/
                meltfptr[30] = 0 ;
                /*^clear*/
                /*clear*/ /*_.SETQ___V32*/
                meltfptr[31] = 0 ;
                /*_.PAIR_TAIL__V35*/
                meltfptr[33] =
                    (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
                MELT_LOCATION("warmelt-macro.melt:5383:/ compute");
                /*_.CURPAIR__V20*/
                meltfptr[19] = /*_.SETQ___V36*/ meltfptr[32] = /*_.PAIR_TAIL__V35*/ meltfptr[33];;

                MELT_CHECK_SIGNAL();
                ;
                /*epilog*/

                MELT_LOCATION("warmelt-macro.melt:5371:/ clear");
                /*clear*/ /*_#IS_PAIR__L5*/
                meltfnum[4] = 0 ;
                /*^clear*/
                /*clear*/ /*_#NOT__L6*/
                meltfnum[5] = 0 ;
                /*^clear*/
                /*clear*/ /*_.PAIR_TAIL__V35*/
                meltfptr[33] = 0 ;
                /*^clear*/
                /*clear*/ /*_.SETQ___V36*/
                meltfptr[32] = 0 ;
            }
            ;
            ;
            goto meltlabloop_INSLOOP_7;
meltlabexit_INSLOOP_7:;/*^loopepilog*/
            /*loopepilog*/
            /*_.FOREVER___V28*/
            meltfptr[27] = /*_.INSLOOP__V29*/ meltfptr[28];;
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:5385:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[8]);
            /*_.FASTUP__V38*/
            meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[7])), (melt_ptr_t)(/*_.FLDLIST__V27*/ meltfptr[26]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5386:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_PUT_FIELDS*/ meltfrout->tabval[9])), (4), "CLASS_SOURCE_PUT_FIELDS");
            /*_.INST__V40*/
            meltfptr[33] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V40*/ meltfptr[33])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V40*/ meltfptr[33]), (1), (/*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SUPUT_OBJ", melt_magic_discr((melt_ptr_t)(/*_.INST__V40*/ meltfptr[33])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V40*/ meltfptr[33]), (2), (/*_.OBJEXP__V22*/ meltfptr[21]), "SUPUT_OBJ");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SUPUT_FIELDS", melt_magic_discr((melt_ptr_t)(/*_.INST__V40*/ meltfptr[33])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V40*/ meltfptr[33]), (3), (/*_.FASTUP__V38*/ meltfptr[30]), "SUPUT_FIELDS");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V40*/ meltfptr[33], "newly made instance");
        ;
        /*_.INST___V39*/
        meltfptr[31] = /*_.INST__V40*/ meltfptr[33];;
        /*^compute*/
        /*_.LET___V37*/
        meltfptr[29] = /*_.INST___V39*/ meltfptr[31];;

        MELT_LOCATION("warmelt-macro.melt:5385:/ clear");
        /*clear*/ /*_.FASTUP__V38*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.INST___V39*/
        meltfptr[31] = 0 ;
        /*_.LET___V26*/
        meltfptr[25] = /*_.LET___V37*/ meltfptr[29];;

        MELT_LOCATION("warmelt-macro.melt:5369:/ clear");
        /*clear*/ /*_.FLDLIST__V27*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V37*/
        meltfptr[29] = 0 ;
        /*_.LET___V14*/
        meltfptr[6] = /*_.LET___V26*/ meltfptr[25];;

        MELT_LOCATION("warmelt-macro.melt:5357:/ clear");
        /*clear*/ /*_.FIELDS__V15*/
        meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FIELDNAMS__V16*/
        meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CONT__V17*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V20*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OBJSRC__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L4*/
        meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OBJEXP__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V24*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V26*/
        meltfptr[25] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5353:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V14*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-macro.melt:5353:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V6*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V9*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L3*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V12*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V14*/
        meltfptr[6] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_PUT_FIELDS", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_83_WARMELTmiMACRO_MEXPAND_PUT_FIELDS*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD fromline 1715 */

    /** start of frame for meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD// fromline 1531
        : public Melt_CallFrameWithValues<36>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<36> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD), clos) {};
        MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD() //the constructor fromline 1605
            : Melt_CallFrameWithValues<36> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<36> (fil,lin, sizeof(MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD)) {};
        MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<36> (fil,lin, sizeof(MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD


    /** end of frame for meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD fromline 1660**/

    /* end of frame for routine meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD fromline 1719 */

    /* classy proc frame meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD */
    MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_UNSAFE_GET_FIELD", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5400:/ getarg");
    /*_.SEXPR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V5*/
    meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5401:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:5401:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5401:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5401;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                            meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V7*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5401:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                        meltfptr[7] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V6*/
            meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5401:/ clear");
            /*clear*/ /*_#IS_A__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V7*/
            meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V6*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5402:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:5402:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5402:/ cond.else");

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
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5402;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                            meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V10*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5402:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                        meltfptr[10] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V9*/
            meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5402:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V10*/
            meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V9*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5403:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V13*/
            meltfptr[6] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5404:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V14*/
            meltfptr[13] = slot;
        };
        ;
        /*_.LIST_FIRST__V15*/
        meltfptr[14] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V13*/ meltfptr[6])));;
        /*^compute*/
        /*_.CURPAIR__V16*/
        meltfptr[15] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V15*/ meltfptr[14])));;
        /*^compute*/
        /*_.CURFKW__V17*/
        meltfptr[16] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L3*/
        meltfnum[0] =
            !melt_is_instance_of((melt_ptr_t)(/*_.CURFKW__V17*/ meltfptr[16]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[3])));;
        MELT_LOCATION("warmelt-macro.melt:5408:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L3*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5410:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V14*/ meltfptr[13]), ( "field keyword expected in UNSAFE_GET_FIELD"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5411:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:5411:/ locexp");
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
                    /*finalret*/
                    goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-macro.melt:5409:/ quasiblock");


                    /*_.PROGN___V20*/
                    meltfptr[19] = /*_.RETURN___V19*/ meltfptr[18];;
                    /*^compute*/
                    /*_._IF___V18*/
                    meltfptr[17] = /*_.PROGN___V20*/ meltfptr[19];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5408:/ clear");
                    /*clear*/ /*_.RETURN___V19*/
                    meltfptr[18] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V20*/
                    meltfptr[19] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V18*/ meltfptr[17] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V21*/
        meltfptr[18] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;
        MELT_LOCATION("warmelt-macro.melt:5412:/ compute");
        /*_.CURPAIR__V16*/
        meltfptr[15] = /*_.SETQ___V22*/ meltfptr[19] = /*_.PAIR_TAIL__V21*/ meltfptr[18];;
        MELT_LOCATION("warmelt-macro.melt:5413:/ quasiblock");


        /*_.CUREXP__V24*/
        meltfptr[23] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;
        /*^compute*/
        /*_.PAIR_TAIL__V25*/
        meltfptr[24] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;
        MELT_LOCATION("warmelt-macro.melt:5414:/ compute");
        /*_.CURPAIR__V16*/
        meltfptr[15] = /*_.SETQ___V26*/ meltfptr[25] = /*_.PAIR_TAIL__V25*/ meltfptr[24];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5415:/ cond");
        /*cond*/
        if (/*_.CURPAIR__V16*/ meltfptr[15]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        /*^locexp*/
                        /* error_plain */ melt_error_str((melt_ptr_t)(/*_.LOC__V14*/ meltfptr[13]), ( "UNSAFE_GET_FIELD with more than two sons"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5418:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[6];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LOC__V14*/ meltfptr[13];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.CURFKW__V17*/ meltfptr[16];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.CUREXP__V24*/ meltfptr[23];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.FLDA__V28*/
            meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!PARSE_FIELD_ASSIGNMENT*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_A__L4*/
        meltfnum[3] =
            melt_is_instance_of((melt_ptr_t)(/*_.FLDA__V28*/ meltfptr[27]), (melt_ptr_t)((/*!CLASS_SOURCE_FIELDASSIGN*/ meltfrout->tabval[5])));;
        /*^compute*/
        /*_#NOT__L5*/
        meltfnum[4] =
            (!(/*_#IS_A__L4*/ meltfnum[3]));;
        MELT_LOCATION("warmelt-macro.melt:5419:/ cond");
        /*cond*/
        if (/*_#NOT__L5*/ meltfnum[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5421:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V14*/ meltfptr[13]), ( "bad field and expression in UNSAFE_GET_FIELD"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5422:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:5422:/ locexp");
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
                    /*finalret*/
                    goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-macro.melt:5420:/ quasiblock");


                    /*_.PROGN___V31*/
                    meltfptr[30] = /*_.RETURN___V30*/ meltfptr[29];;
                    /*^compute*/
                    /*_._IF___V29*/
                    meltfptr[28] = /*_.PROGN___V31*/ meltfptr[30];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5419:/ clear");
                    /*clear*/ /*_.RETURN___V30*/
                    meltfptr[29] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V31*/
                    meltfptr[30] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V29*/ meltfptr[28] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:5423:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.FLDA__V28*/ meltfptr[27]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SFLA_FIELD");
            /*_.FLD__V33*/
            meltfptr[30] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5424:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.FLDA__V28*/ meltfptr[27]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "SFLA_EXPR");
            /*_.EXP__V34*/
            meltfptr[33] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#NULL__L6*/
        meltfnum[5] =
            ((/*_.EXP__V34*/ meltfptr[33]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:5425:/ cond");
        /*cond*/
        if (/*_#NULL__L6*/ meltfnum[5]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5426:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V14*/ meltfptr[13]), ( "missing object expression for (UNSAFE_GET_FIELD :field objexpr)"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5427:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_UNSAFE_GET_FIELD*/ meltfrout->tabval[6])), (4), "CLASS_SOURCE_UNSAFE_GET_FIELD");
            /*_.INST__V36*/
            meltfptr[35] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V36*/ meltfptr[35])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V36*/ meltfptr[35]), (1), (/*_.LOC__V14*/ meltfptr[13]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SUGET_OBJ", melt_magic_discr((melt_ptr_t)(/*_.INST__V36*/ meltfptr[35])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V36*/ meltfptr[35]), (2), (/*_.EXP__V34*/ meltfptr[33]), "SUGET_OBJ");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SUGET_FIELD", melt_magic_discr((melt_ptr_t)(/*_.INST__V36*/ meltfptr[35])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V36*/ meltfptr[35]), (3), (/*_.FLD__V33*/ meltfptr[30]), "SUGET_FIELD");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V36*/ meltfptr[35], "newly made instance");
        ;
        /*_.INST___V35*/
        meltfptr[34] = /*_.INST__V36*/ meltfptr[35];;
        /*^compute*/
        /*_.LET___V32*/
        meltfptr[29] = /*_.INST___V35*/ meltfptr[34];;

        MELT_LOCATION("warmelt-macro.melt:5423:/ clear");
        /*clear*/ /*_.FLD__V33*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.EXP__V34*/
        meltfptr[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L6*/
        meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.INST___V35*/
        meltfptr[34] = 0 ;
        /*_.LET___V27*/
        meltfptr[26] = /*_.LET___V32*/ meltfptr[29];;

        MELT_LOCATION("warmelt-macro.melt:5418:/ clear");
        /*clear*/ /*_.FLDA__V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L4*/
        meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L5*/
        meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V29*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V32*/
        meltfptr[29] = 0 ;
        /*_.LET___V23*/
        meltfptr[22] = /*_.LET___V27*/ meltfptr[26];;

        MELT_LOCATION("warmelt-macro.melt:5413:/ clear");
        /*clear*/ /*_.CUREXP__V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V27*/
        meltfptr[26] = 0 ;
        /*_.LET___V12*/
        meltfptr[10] = /*_.LET___V23*/ meltfptr[22];;

        MELT_LOCATION("warmelt-macro.melt:5403:/ clear");
        /*clear*/ /*_.CONT__V13*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V14*/
        meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V15*/
        meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V16*/
        meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURFKW__V17*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L3*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V21*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V22*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V23*/
        meltfptr[22] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5400:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V12*/ meltfptr[10];;

        {
            MELT_LOCATION("warmelt-macro.melt:5400:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V6*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V9*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V12*/
        meltfptr[10] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_UNSAFE_GET_FIELD", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_84_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD fromline 1715 */

    /** start of frame for meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD// fromline 1531
        : public Melt_CallFrameWithValues<36>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<36> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD), clos) {};
        MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD() //the constructor fromline 1605
            : Melt_CallFrameWithValues<36> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<36> (fil,lin, sizeof(MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD)) {};
        MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<36> (fil,lin, sizeof(MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD


    /** end of frame for meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD fromline 1660**/

    /* end of frame for routine meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD fromline 1719 */

    /* classy proc frame meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD */
    MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_GET_FIELD", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5441:/ getarg");
    /*_.SEXPR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V5*/
    meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5442:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:5442:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5442:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5442;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                            meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V7*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5442:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                        meltfptr[7] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V6*/
            meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5442:/ clear");
            /*clear*/ /*_#IS_A__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V7*/
            meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V6*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5443:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:5443:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5443:/ cond.else");

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
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5443;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                            meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V10*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5443:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                        meltfptr[10] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V9*/
            meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5443:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V10*/
            meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V9*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5444:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V13*/
            meltfptr[6] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5445:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V14*/
            meltfptr[13] = slot;
        };
        ;
        /*_.LIST_FIRST__V15*/
        meltfptr[14] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V13*/ meltfptr[6])));;
        /*^compute*/
        /*_.CURPAIR__V16*/
        meltfptr[15] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V15*/ meltfptr[14])));;
        /*^compute*/
        /*_.CURFKW__V17*/
        meltfptr[16] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L3*/
        meltfnum[0] =
            !melt_is_instance_of((melt_ptr_t)(/*_.CURFKW__V17*/ meltfptr[16]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[3])));;
        MELT_LOCATION("warmelt-macro.melt:5449:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L3*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5451:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V14*/ meltfptr[13]), ( "field keyword expected in GET_FIELD"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5452:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:5452:/ locexp");
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
                    /*finalret*/
                    goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-macro.melt:5450:/ quasiblock");


                    /*_.PROGN___V20*/
                    meltfptr[19] = /*_.RETURN___V19*/ meltfptr[18];;
                    /*^compute*/
                    /*_._IF___V18*/
                    meltfptr[17] = /*_.PROGN___V20*/ meltfptr[19];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5449:/ clear");
                    /*clear*/ /*_.RETURN___V19*/
                    meltfptr[18] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V20*/
                    meltfptr[19] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V18*/ meltfptr[17] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V21*/
        meltfptr[18] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;
        MELT_LOCATION("warmelt-macro.melt:5453:/ compute");
        /*_.CURPAIR__V16*/
        meltfptr[15] = /*_.SETQ___V22*/ meltfptr[19] = /*_.PAIR_TAIL__V21*/ meltfptr[18];;
        MELT_LOCATION("warmelt-macro.melt:5454:/ quasiblock");


        /*_.CUREXP__V24*/
        meltfptr[23] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;
        /*^compute*/
        /*_.PAIR_TAIL__V25*/
        meltfptr[24] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;
        MELT_LOCATION("warmelt-macro.melt:5455:/ compute");
        /*_.CURPAIR__V16*/
        meltfptr[15] = /*_.SETQ___V26*/ meltfptr[25] = /*_.PAIR_TAIL__V25*/ meltfptr[24];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5456:/ cond");
        /*cond*/
        if (/*_.CURPAIR__V16*/ meltfptr[15]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        /*^locexp*/
                        /* error_plain */ melt_error_str((melt_ptr_t)(/*_.LOC__V14*/ meltfptr[13]), ( "UNSAFE_GET_FIELD with more than two sons"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5459:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[6];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LOC__V14*/ meltfptr[13];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.CURFKW__V17*/ meltfptr[16];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.CUREXP__V24*/ meltfptr[23];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.FLDA__V28*/
            meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!PARSE_FIELD_ASSIGNMENT*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_A__L4*/
        meltfnum[3] =
            melt_is_instance_of((melt_ptr_t)(/*_.FLDA__V28*/ meltfptr[27]), (melt_ptr_t)((/*!CLASS_SOURCE_FIELDASSIGN*/ meltfrout->tabval[5])));;
        /*^compute*/
        /*_#NOT__L5*/
        meltfnum[4] =
            (!(/*_#IS_A__L4*/ meltfnum[3]));;
        MELT_LOCATION("warmelt-macro.melt:5460:/ cond");
        /*cond*/
        if (/*_#NOT__L5*/ meltfnum[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5462:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V14*/ meltfptr[13]), ( "bad field and expression in GET_FIELD"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5463:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:5463:/ locexp");
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
                    /*finalret*/
                    goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-macro.melt:5461:/ quasiblock");


                    /*_.PROGN___V31*/
                    meltfptr[30] = /*_.RETURN___V30*/ meltfptr[29];;
                    /*^compute*/
                    /*_._IF___V29*/
                    meltfptr[28] = /*_.PROGN___V31*/ meltfptr[30];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5460:/ clear");
                    /*clear*/ /*_.RETURN___V30*/
                    meltfptr[29] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V31*/
                    meltfptr[30] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V29*/ meltfptr[28] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:5464:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.FLDA__V28*/ meltfptr[27]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SFLA_FIELD");
            /*_.FLD__V33*/
            meltfptr[30] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5465:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.FLDA__V28*/ meltfptr[27]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "SFLA_EXPR");
            /*_.EXP__V34*/
            meltfptr[33] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#NULL__L6*/
        meltfnum[5] =
            ((/*_.EXP__V34*/ meltfptr[33]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:5466:/ cond");
        /*cond*/
        if (/*_#NULL__L6*/ meltfnum[5]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5467:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V14*/ meltfptr[13]), ( "missing object expression for (GET_FIELD :field objexpr)"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5468:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_GET_FIELD*/ meltfrout->tabval[6])), (4), "CLASS_SOURCE_GET_FIELD");
            /*_.INST__V36*/
            meltfptr[35] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V36*/ meltfptr[35])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V36*/ meltfptr[35]), (1), (/*_.LOC__V14*/ meltfptr[13]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SUGET_OBJ", melt_magic_discr((melt_ptr_t)(/*_.INST__V36*/ meltfptr[35])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V36*/ meltfptr[35]), (2), (/*_.EXP__V34*/ meltfptr[33]), "SUGET_OBJ");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SUGET_FIELD", melt_magic_discr((melt_ptr_t)(/*_.INST__V36*/ meltfptr[35])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V36*/ meltfptr[35]), (3), (/*_.FLD__V33*/ meltfptr[30]), "SUGET_FIELD");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V36*/ meltfptr[35], "newly made instance");
        ;
        /*_.INST___V35*/
        meltfptr[34] = /*_.INST__V36*/ meltfptr[35];;
        /*^compute*/
        /*_.LET___V32*/
        meltfptr[29] = /*_.INST___V35*/ meltfptr[34];;

        MELT_LOCATION("warmelt-macro.melt:5464:/ clear");
        /*clear*/ /*_.FLD__V33*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.EXP__V34*/
        meltfptr[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L6*/
        meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.INST___V35*/
        meltfptr[34] = 0 ;
        /*_.LET___V27*/
        meltfptr[26] = /*_.LET___V32*/ meltfptr[29];;

        MELT_LOCATION("warmelt-macro.melt:5459:/ clear");
        /*clear*/ /*_.FLDA__V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L4*/
        meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L5*/
        meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V29*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V32*/
        meltfptr[29] = 0 ;
        /*_.LET___V23*/
        meltfptr[22] = /*_.LET___V27*/ meltfptr[26];;

        MELT_LOCATION("warmelt-macro.melt:5454:/ clear");
        /*clear*/ /*_.CUREXP__V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V27*/
        meltfptr[26] = 0 ;
        /*_.LET___V12*/
        meltfptr[10] = /*_.LET___V23*/ meltfptr[22];;

        MELT_LOCATION("warmelt-macro.melt:5444:/ clear");
        /*clear*/ /*_.CONT__V13*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V14*/
        meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V15*/
        meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V16*/
        meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURFKW__V17*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L3*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V21*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V22*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V23*/
        meltfptr[22] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5441:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V12*/ meltfptr[10];;

        {
            MELT_LOCATION("warmelt-macro.melt:5441:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V6*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V9*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V12*/
        meltfptr[10] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_GET_FIELD", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_85_WARMELTmiMACRO_MEXPAND_GET_FIELD*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN fromline 1715 */

    /** start of frame for meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN// fromline 1531
        : public Melt_CallFrameWithValues<26>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<26> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN), clos) {};
        MeltFrame_meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN() //the constructor fromline 1605
            : Melt_CallFrameWithValues<26> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<26> (fil,lin, sizeof(MeltFrame_meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN)) {};
        MeltFrame_meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<26> (fil,lin, sizeof(MeltFrame_meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN


    /** end of frame for meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN fromline 1660**/

    /* end of frame for routine meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN fromline 1719 */

    /* classy proc frame meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN */
    MeltFrame_meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("PAIRLIST_TO_PROGN", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5482:/ getarg");
    /*_.PAIR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.LOC__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.LOC__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V5*/
    meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V5*/ meltfptr[4])) != NULL);


    /*getarg#4*/
    /*^getarg*/
    if (meltxargdescr_[3] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V6*/
    meltfptr[5] = (meltxargtab_[3].meltbp_aptr) ? (*(meltxargtab_[3].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MODCTX__V6*/ meltfptr[5])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5483:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5483:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L1*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5483:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5483;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "pairlist_to_progn pair=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.PAIR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V8*/
                            meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V7*/
                        meltfptr[6] = /*_.MELT_DEBUG_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5483:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/
                        meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V8*/
                        meltfptr[7] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V7*/ meltfptr[6] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5483:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V7*/
            meltfptr[6] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5484:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V4*/ meltfptr[3]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:5484:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5484:/ cond.else");

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
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5484;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                            meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V10*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5484:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                        meltfptr[10] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V9*/
            meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5484:/ clear");
            /*clear*/ /*_#IS_A__L3*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V10*/
            meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V9*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5485:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_PAIR__L4*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.PAIR__V2*/ meltfptr[1])) == MELTOBMAG_PAIR);;
            MELT_LOCATION("warmelt-macro.melt:5485:/ cond");
            /*cond*/
            if (/*_#IS_PAIR__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V13*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5485:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check_pair";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5485;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.PAIR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V14*/
                            meltfptr[13] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V13*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V14*/ meltfptr[13];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5485:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V14*/
                        meltfptr[13] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V12*/
            meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5485:/ clear");
            /*clear*/ /*_#IS_PAIR__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V13*/
            meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V12*/ meltfptr[10] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5486:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L5*/
            meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V6*/ meltfptr[5])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:5486:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V16*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5486:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5486;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V6*/ meltfptr[5];
                            /*_.MELT_ASSERT_FAILURE_FUN__V17*/
                            meltfptr[16] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V16*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V17*/ meltfptr[16];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5486:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V17*/
                        meltfptr[16] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V15*/
            meltfptr[13] = /*_._IFELSE___V16*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5486:/ clear");
            /*clear*/ /*_#IS_OBJECT__L5*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V16*/
            meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V15*/ meltfptr[13] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5487:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:5490:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V20*/
        meltfptr[19] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_5*/ meltfrout->tabval[5])), (3));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V20*/ meltfptr[19])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V20*/ meltfptr[19])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V20*/ meltfptr[19])->tabval[0] = (melt_ptr_t)(/*_.MEXPANDER__V5*/ meltfptr[4]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V20*/ meltfptr[19])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 1>= 0 && 1< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V20*/ meltfptr[19])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V20*/ meltfptr[19])->tabval[1] = (melt_ptr_t)(/*_.ENV__V4*/ meltfptr[3]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V20*/ meltfptr[19])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 2>= 0 && 2< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V20*/ meltfptr[19])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V20*/ meltfptr[19])->tabval[2] = (melt_ptr_t)(/*_.MODCTX__V6*/ meltfptr[5]);
        ;
        /*_.LAMBDA___V19*/
        meltfptr[6] = /*_.LAMBDA___V20*/ meltfptr[19];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5487:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[4]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.LAMBDA___V19*/ meltfptr[6];
            /*_.BODYTUP__V21*/
            meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!PAIRLIST_TO_MULTIPLE*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.PAIR__V2*/ meltfptr[1]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5492:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_PROGN*/ meltfrout->tabval[6])), (3), "CLASS_SOURCE_PROGN");
            /*_.INST__V23*/
            meltfptr[22] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V23*/ meltfptr[22])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V23*/ meltfptr[22]), (1), (/*_.LOC__V3*/ meltfptr[2]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SPROGN_BODY", melt_magic_discr((melt_ptr_t)(/*_.INST__V23*/ meltfptr[22])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V23*/ meltfptr[22]), (2), (/*_.BODYTUP__V21*/ meltfptr[20]), "SPROGN_BODY");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V23*/ meltfptr[22], "newly made instance");
        ;
        /*_.SPROGN__V22*/
        meltfptr[21] = /*_.INST__V23*/ meltfptr[22];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5496:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L6*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5496:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L7*/ meltfnum[1] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:5496:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5496;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "pairlist_to_progn sprogn";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SPROGN__V22*/ meltfptr[21];
                            /*_.MELT_DEBUG_FUN__V25*/
                            meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V24*/
                        meltfptr[23] = /*_.MELT_DEBUG_FUN__V25*/ meltfptr[24];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5496:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L7*/
                        meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V25*/
                        meltfptr[24] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V24*/ meltfptr[23] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5496:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L6*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V24*/
            meltfptr[23] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5497:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.SPROGN__V22*/ meltfptr[21];;

        {
            MELT_LOCATION("warmelt-macro.melt:5497:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*_.LET___V18*/
        meltfptr[16] = /*_.RETURN___V26*/ meltfptr[24];;

        MELT_LOCATION("warmelt-macro.melt:5487:/ clear");
        /*clear*/ /*_.LAMBDA___V19*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BODYTUP__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SPROGN__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V26*/
        meltfptr[24] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5482:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V18*/ meltfptr[16];;

        {
            MELT_LOCATION("warmelt-macro.melt:5482:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V9*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V12*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V15*/
        meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V18*/
        meltfptr[16] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("PAIRLIST_TO_PROGN", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_86_WARMELTmiMACRO_PAIRLIST_TO_PROGN*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_87_WARMELTmiMACRO_LAMBDA_cl13(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_87_WARMELTmiMACRO_LAMBDA_cl13_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_87_WARMELTmiMACRO_LAMBDA_cl13 fromline 1715 */

    /** start of frame for meltrout_87_WARMELTmiMACRO_LAMBDA_cl13 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_87_WARMELTmiMACRO_LAMBDA_cl13// fromline 1531
        : public Melt_CallFrameWithValues<3>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_87_WARMELTmiMACRO_LAMBDA_cl13(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_87_WARMELTmiMACRO_LAMBDA_cl13), clos) {};
        MeltFrame_meltrout_87_WARMELTmiMACRO_LAMBDA_cl13() //the constructor fromline 1605
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_87_WARMELTmiMACRO_LAMBDA_cl13)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_87_WARMELTmiMACRO_LAMBDA_cl13(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_87_WARMELTmiMACRO_LAMBDA_cl13)) {};
        MeltFrame_meltrout_87_WARMELTmiMACRO_LAMBDA_cl13(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_87_WARMELTmiMACRO_LAMBDA_cl13), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_87_WARMELTmiMACRO_LAMBDA_cl13


    /** end of frame for meltrout_87_WARMELTmiMACRO_LAMBDA_cl13 fromline 1660**/

    /* end of frame for routine meltrout_87_WARMELTmiMACRO_LAMBDA_cl13 fromline 1719 */

    /* classy proc frame meltrout_87_WARMELTmiMACRO_LAMBDA_cl13 */
    MeltFrame_meltrout_87_WARMELTmiMACRO_LAMBDA_cl13
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_87_WARMELTmiMACRO_LAMBDA_cl13 fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl13", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5490:/ getarg");
    /*_.E__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
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
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*~ENV*/ meltfclos->tabval[1]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*~MEXPANDER*/ meltfclos->tabval[0]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*~MODCTX*/ meltfclos->tabval[2]);
            /*_.MEXPANDER__V3*/
            meltfptr[2] =  melt_apply ((meltclosure_ptr_t)((/*~MEXPANDER*/ meltfclos->tabval[0])), (melt_ptr_t)(/*_.E__V2*/ meltfptr[1]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*^quasiblock*/


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.MEXPANDER__V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-macro.melt:5490:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.MEXPANDER__V3*/
        meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl13", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_87_WARMELTmiMACRO_LAMBDA_cl13_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_87_WARMELTmiMACRO_LAMBDA_cl13*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ fromline 1715 */

    /** start of frame for meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ// fromline 1531
        : public Melt_CallFrameWithValues<34>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<34> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ), clos) {};
        MeltFrame_meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ() //the constructor fromline 1605
            : Melt_CallFrameWithValues<34> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<34> (fil,lin, sizeof(MeltFrame_meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ)) {};
        MeltFrame_meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<34> (fil,lin, sizeof(MeltFrame_meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ


    /** end of frame for meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ fromline 1660**/

    /* end of frame for routine meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ fromline 1719 */

    /* classy proc frame meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ */
    MeltFrame_meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_SETQ", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5501:/ getarg");
    /*_.SEXPR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V5*/
    meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5502:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:5502:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5502:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5502;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                            meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V7*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5502:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                        meltfptr[7] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V6*/
            meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5502:/ clear");
            /*clear*/ /*_#IS_A__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V7*/
            meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V6*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5503:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:5503:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5503:/ cond.else");

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
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5503;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                            meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V10*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5503:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                        meltfptr[10] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V9*/
            meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5503:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V10*/
            meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V9*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L3*/
        meltfnum[0] =
            ((/*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:5504:/ cond");
        /*cond*/
        if (/*_#NULL__L3*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V13*/ meltfptr[6] = (/*!MACROEXPAND_1*/ meltfrout->tabval[3]);;
                    /*_._IF___V12*/
                    meltfptr[10] = /*_.SETQ___V13*/ meltfptr[6];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5504:/ clear");
                    /*clear*/ /*_.SETQ___V13*/
                    meltfptr[6] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V12*/ meltfptr[10] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5505:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L4*/
            meltfnum[3] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:5505:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L4*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5505:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5505;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/
                        meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5505:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                        meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/
            meltfptr[6] = /*_._IFELSE___V15*/ meltfptr[14];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5505:/ clear");
            /*clear*/ /*_#IS_OBJECT__L4*/
            meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[14] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5506:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V18*/
            meltfptr[14] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5507:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V19*/
            meltfptr[18] = slot;
        };
        ;
        /*_.LIST_FIRST__V20*/
        meltfptr[19] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V18*/ meltfptr[14])));;
        /*^compute*/
        /*_.CURPAIR__V21*/
        meltfptr[20] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V20*/ meltfptr[19])));;
        /*^compute*/
        /*_.CURSYM__V22*/
        meltfptr[21] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L5*/
        meltfnum[3] =
            !melt_is_instance_of((melt_ptr_t)(/*_.CURSYM__V22*/ meltfptr[21]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[4])));;
        MELT_LOCATION("warmelt-macro.melt:5511:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L5*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5513:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "var symbol name expected in SETQ"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5514:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:5514:/ locexp");
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
                    /*finalret*/
                    goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-macro.melt:5512:/ quasiblock");


                    /*_.PROGN___V25*/
                    meltfptr[24] = /*_.RETURN___V24*/ meltfptr[23];;
                    /*^compute*/
                    /*_._IF___V23*/
                    meltfptr[22] = /*_.PROGN___V25*/ meltfptr[24];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5511:/ clear");
                    /*clear*/ /*_.RETURN___V24*/
                    meltfptr[23] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V25*/
                    meltfptr[24] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V23*/ meltfptr[22] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V26*/
        meltfptr[23] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20])));;
        MELT_LOCATION("warmelt-macro.melt:5515:/ compute");
        /*_.CURPAIR__V21*/
        meltfptr[20] = /*_.SETQ___V27*/ meltfptr[24] = /*_.PAIR_TAIL__V26*/ meltfptr[23];;
        MELT_LOCATION("warmelt-macro.melt:5516:/ quasiblock");


        /*_.CUREXP__V29*/
        meltfptr[28] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20])));;
        /*^compute*/
        /*_.PAIR_TAIL__V30*/
        meltfptr[29] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V21*/ meltfptr[20])));;
        MELT_LOCATION("warmelt-macro.melt:5517:/ compute");
        /*_.CURPAIR__V21*/
        meltfptr[20] = /*_.SETQ___V31*/ meltfptr[30] = /*_.PAIR_TAIL__V30*/ meltfptr[29];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5518:/ cond");
        /*cond*/
        if (/*_.CURPAIR__V21*/ meltfptr[20]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        /*^locexp*/
                        /* error_plain */ melt_error_str((melt_ptr_t)(/*_.LOC__V19*/ meltfptr[18]), ( "SETQ with more than two sons"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5522:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.MEXPANDER__V32*/
            meltfptr[31] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CUREXP__V29*/ meltfptr[28]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:5519:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_SETQ*/ meltfrout->tabval[5])), (4), "CLASS_SOURCE_SETQ");
            /*_.INST__V34*/
            meltfptr[33] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V34*/ meltfptr[33])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V34*/ meltfptr[33]), (1), (/*_.LOC__V19*/ meltfptr[18]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SSTQ_VAR", melt_magic_discr((melt_ptr_t)(/*_.INST__V34*/ meltfptr[33])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V34*/ meltfptr[33]), (2), (/*_.CURSYM__V22*/ meltfptr[21]), "SSTQ_VAR");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SSTQ_EXPR", melt_magic_discr((melt_ptr_t)(/*_.INST__V34*/ meltfptr[33])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V34*/ meltfptr[33]), (3), (/*_.MEXPANDER__V32*/ meltfptr[31]), "SSTQ_EXPR");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V34*/ meltfptr[33], "newly made instance");
        ;
        /*_.INST___V33*/
        meltfptr[32] = /*_.INST__V34*/ meltfptr[33];;
        /*^compute*/
        /*_.LET___V28*/
        meltfptr[27] = /*_.INST___V33*/ meltfptr[32];;

        MELT_LOCATION("warmelt-macro.melt:5516:/ clear");
        /*clear*/ /*_.CUREXP__V29*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V30*/
        meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V31*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MEXPANDER__V32*/
        meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.INST___V33*/
        meltfptr[32] = 0 ;
        /*_.LET___V17*/
        meltfptr[15] = /*_.LET___V28*/ meltfptr[27];;

        MELT_LOCATION("warmelt-macro.melt:5506:/ clear");
        /*clear*/ /*_.CONT__V18*/
        meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V20*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURSYM__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L5*/
        meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V26*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V27*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V28*/
        meltfptr[27] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5501:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V17*/ meltfptr[15];;

        {
            MELT_LOCATION("warmelt-macro.melt:5501:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V6*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V9*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L3*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V12*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V17*/
        meltfptr[15] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_SETQ", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_88_WARMELTmiMACRO_MEXPAND_SETQ*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_89_WARMELTmiMACRO_MEXPAND_IF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                      const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_89_WARMELTmiMACRO_MEXPAND_IF_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_89_WARMELTmiMACRO_MEXPAND_IF fromline 1715 */

    /** start of frame for meltrout_89_WARMELTmiMACRO_MEXPAND_IF of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_89_WARMELTmiMACRO_MEXPAND_IF// fromline 1531
        : public Melt_CallFrameWithValues<58>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_89_WARMELTmiMACRO_MEXPAND_IF(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<58> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_89_WARMELTmiMACRO_MEXPAND_IF), clos) {};
        MeltFrame_meltrout_89_WARMELTmiMACRO_MEXPAND_IF() //the constructor fromline 1605
            : Melt_CallFrameWithValues<58> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_89_WARMELTmiMACRO_MEXPAND_IF)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_89_WARMELTmiMACRO_MEXPAND_IF(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<58> (fil,lin, sizeof(MeltFrame_meltrout_89_WARMELTmiMACRO_MEXPAND_IF)) {};
        MeltFrame_meltrout_89_WARMELTmiMACRO_MEXPAND_IF(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<58> (fil,lin, sizeof(MeltFrame_meltrout_89_WARMELTmiMACRO_MEXPAND_IF), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_89_WARMELTmiMACRO_MEXPAND_IF


    /** end of frame for meltrout_89_WARMELTmiMACRO_MEXPAND_IF fromline 1660**/

    /* end of frame for routine meltrout_89_WARMELTmiMACRO_MEXPAND_IF fromline 1719 */

    /* classy proc frame meltrout_89_WARMELTmiMACRO_MEXPAND_IF */
    MeltFrame_meltrout_89_WARMELTmiMACRO_MEXPAND_IF
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_89_WARMELTmiMACRO_MEXPAND_IF fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_IF", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5532:/ getarg");
    /*_.SEXPR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V5*/
    meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5533:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5533:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L1*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5533:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5533;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_if sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5533:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/
                        meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V7*/
                        meltfptr[6] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5533:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V6*/
            meltfptr[5] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5534:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:5534:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5534:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5534;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5534:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5534:/ clear");
            /*clear*/ /*_#IS_A__L3*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5535:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:5535:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5535:/ cond.else");

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
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5535;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5535:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5535:/ clear");
            /*clear*/ /*_#IS_A__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V12*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V11*/ meltfptr[9] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L5*/
        meltfnum[1] =
            ((/*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:5536:/ cond");
        /*cond*/
        if (/*_#NULL__L5*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V15*/ meltfptr[5] = (/*!MACROEXPAND_1*/ meltfrout->tabval[4]);;
                    /*_._IF___V14*/
                    meltfptr[12] = /*_.SETQ___V15*/ meltfptr[5];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5536:/ clear");
                    /*clear*/ /*_.SETQ___V15*/
                    meltfptr[5] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V14*/ meltfptr[12] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5537:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L6*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:5537:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V17*/ meltfptr[16] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5537:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5537;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                            /*_.MELT_ASSERT_FAILURE_FUN__V18*/
                            meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V17*/
                        meltfptr[16] = /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5537:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V18*/
                        meltfptr[17] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V16*/
            meltfptr[5] = /*_._IFELSE___V17*/ meltfptr[16];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5537:/ clear");
            /*clear*/ /*_#IS_OBJECT__L6*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V17*/
            meltfptr[16] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V16*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5538:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:5539:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V20*/
            meltfptr[16] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5540:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V21*/
            meltfptr[20] = slot;
        };
        ;
        /*_.LIST_FIRST__V22*/
        meltfptr[21] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V20*/ meltfptr[16])));;
        /*^compute*/
        /*_.CURPAIR__V23*/
        meltfptr[22] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V22*/ meltfptr[21])));;
        /*^compute*/
        /*_.CURIF__V24*/
        meltfptr[23] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_PAIR__L7*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])) == MELTOBMAG_PAIR);;
        /*^compute*/
        /*_#NOT__L8*/
        meltfnum[7] =
            (!(/*_#IS_PAIR__L7*/ meltfnum[0]));;
        MELT_LOCATION("warmelt-macro.melt:5543:/ cond");
        /*cond*/
        if (/*_#NOT__L8*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5544:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20]), ( "missing condition in IF"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*_.PAIR_TAIL__V25*/
        meltfptr[24] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;
        MELT_LOCATION("warmelt-macro.melt:5545:/ compute");
        /*_.CURPAIR__V23*/
        meltfptr[22] = /*_.SETQ___V26*/ meltfptr[25] = /*_.PAIR_TAIL__V25*/ meltfptr[24];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_PAIR__L9*/
        meltfnum[8] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])) == MELTOBMAG_PAIR);;
        /*^compute*/
        /*_#NOT__L10*/
        meltfnum[9] =
            (!(/*_#IS_PAIR__L9*/ meltfnum[8]));;
        MELT_LOCATION("warmelt-macro.melt:5546:/ cond");
        /*cond*/
        if (/*_#NOT__L10*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5547:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20]), ( "missing then in IF"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5548:/ quasiblock");



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
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.XCOND__V28*/
            meltfptr[27] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURIF__V24*/ meltfptr[23]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5549:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L11*/
            meltfnum[10] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5549:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L11*/ meltfnum[10]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5549:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5549;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_if xcond";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XCOND__V28*/ meltfptr[27];
                            /*_.MELT_DEBUG_FUN__V30*/
                            meltfptr[29] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V29*/
                        meltfptr[28] = /*_.MELT_DEBUG_FUN__V30*/ meltfptr[29];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5549:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L12*/
                        meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V30*/
                        meltfptr[29] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V29*/ meltfptr[28] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5549:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L11*/
            meltfnum[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V29*/
            meltfptr[28] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5550:/ quasiblock");


        /*_.CURTHEN__V32*/
        meltfptr[28] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;
        /*^compute*/
        /*_.PAIR_TAIL__V33*/
        meltfptr[32] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;
        MELT_LOCATION("warmelt-macro.melt:5551:/ compute");
        /*_.CURPAIR__V23*/
        meltfptr[22] = /*_.SETQ___V34*/ meltfptr[33] = /*_.PAIR_TAIL__V33*/ meltfptr[32];;
        MELT_LOCATION("warmelt-macro.melt:5552:/ quasiblock");



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
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.XTHEN__V36*/
            meltfptr[35] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURTHEN__V32*/ meltfptr[28]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5554:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L13*/
            meltfnum[11] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5554:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L13*/ meltfnum[11]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5554:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L14*/ meltfnum[10];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5554;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_if xthen";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XTHEN__V36*/ meltfptr[35];
                            /*_.MELT_DEBUG_FUN__V38*/
                            meltfptr[37] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V37*/
                        meltfptr[36] = /*_.MELT_DEBUG_FUN__V38*/ meltfptr[37];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5554:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L14*/
                        meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V38*/
                        meltfptr[37] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V37*/ meltfptr[36] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5554:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L13*/
            meltfnum[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V37*/
            meltfptr[36] = 0 ;
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
        /*_#IS_PAIR__L15*/
        meltfnum[10] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])) == MELTOBMAG_PAIR);;
        MELT_LOCATION("warmelt-macro.melt:5555:/ cond");
        /*cond*/
        if (/*_#IS_PAIR__L15*/ meltfnum[10]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:5556:/ quasiblock");


                    /*_.CURELSE__V41*/
                    meltfptr[40] =
                        (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5557:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                        /*_.XELSE__V42*/
                        meltfptr[41] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURELSE__V41*/ meltfptr[40]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:5558:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L16*/
                        meltfnum[11] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-macro.melt:5558:/ cond");
                        /*cond*/
                        if (/*_#MELT_NEED_DBG__L16*/ meltfnum[11]) /*then*/
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
                                    MELT_LOCATION("warmelt-macro.melt:5558:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L17*/ meltfnum[16];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 5558;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "mexpand_if xelse";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XELSE__V42*/ meltfptr[41];
                                        /*_.MELT_DEBUG_FUN__V44*/
                                        meltfptr[43] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V43*/
                                    meltfptr[42] = /*_.MELT_DEBUG_FUN__V44*/ meltfptr[43];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:5558:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L17*/
                                    meltfnum[16] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V44*/
                                    meltfptr[43] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V43*/ meltfptr[42] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-macro.melt:5558:/ locexp");
                            /*void*/
                            (void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L16*/
                        meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V43*/
                        meltfptr[42] = 0 ;
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
                    /*_.PAIR_TAIL__V45*/
                    meltfptr[43] =
                        (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;
                    MELT_LOCATION("warmelt-macro.melt:5559:/ compute");
                    /*_.CURPAIR__V23*/
                    meltfptr[22] = /*_.SETQ___V46*/ meltfptr[42] = /*_.PAIR_TAIL__V45*/ meltfptr[43];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_PAIR__L18*/
                    meltfnum[16] =
                        (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])) == MELTOBMAG_PAIR);;
                    MELT_LOCATION("warmelt-macro.melt:5560:/ cond");
                    /*cond*/
                    if (/*_#IS_PAIR__L18*/ meltfnum[16]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:5561:/ locexp");
                                    /* error_plain */
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20]), ( "IF with more than three sons"), (melt_ptr_t)0);
                                }
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5562:/ quasiblock");



                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5563:/ quasiblock");


                    /*^rawallocobj*/
                    /*rawallocobj*/
                    {
                        melt_ptr_t newobj = 0;
                        melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_IFELSE*/ meltfrout->tabval[5])), (5), "CLASS_SOURCE_IFELSE");
                        /*_.INST__V49*/
                        meltfptr[48] =
                            newobj;
                    };
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V49*/ meltfptr[48])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V49*/ meltfptr[48]), (1), (/*_.LOC__V21*/ meltfptr[20]), "LOCA_LOCATION");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SIF_TEST", melt_magic_discr((melt_ptr_t)(/*_.INST__V49*/ meltfptr[48])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V49*/ meltfptr[48]), (2), (/*_.XCOND__V28*/ meltfptr[27]), "SIF_TEST");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SIF_THEN", melt_magic_discr((melt_ptr_t)(/*_.INST__V49*/ meltfptr[48])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V49*/ meltfptr[48]), (3), (/*_.XTHEN__V36*/ meltfptr[35]), "SIF_THEN");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SIF_ELSE", melt_magic_discr((melt_ptr_t)(/*_.INST__V49*/ meltfptr[48])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V49*/ meltfptr[48]), (4), (/*_.XELSE__V42*/ meltfptr[41]), "SIF_ELSE");
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object (/*_.INST__V49*/ meltfptr[48], "newly made instance");
                    ;
                    /*_.RESE__V48*/
                    meltfptr[47] = /*_.INST__V49*/ meltfptr[48];;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:5569:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L19*/
                        meltfnum[11] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-macro.melt:5569:/ cond");
                        /*cond*/
                        if (/*_#MELT_NEED_DBG__L19*/ meltfnum[11]) /*then*/
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
                                    MELT_LOCATION("warmelt-macro.melt:5569:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L20*/ meltfnum[19];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 5569;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "mexpand_if with else return rese";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RESE__V48*/ meltfptr[47];
                                        /*_.MELT_DEBUG_FUN__V51*/
                                        meltfptr[50] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V50*/
                                    meltfptr[49] = /*_.MELT_DEBUG_FUN__V51*/ meltfptr[50];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:5569:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L20*/
                                    meltfnum[19] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V51*/
                                    meltfptr[50] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V50*/ meltfptr[49] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-macro.melt:5569:/ locexp");
                            /*void*/
                            (void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L19*/
                        meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V50*/
                        meltfptr[49] = 0 ;
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
                    MELT_LOCATION("warmelt-macro.melt:5570:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = /*_.RESE__V48*/ meltfptr[47];;

                    {
                        MELT_LOCATION("warmelt-macro.melt:5570:/ locexp");
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
                    /*finalret*/
                    goto meltlabend_rout ;
                    /*_.LET___V47*/
                    meltfptr[46] = /*_.RETURN___V52*/ meltfptr[50];;

                    MELT_LOCATION("warmelt-macro.melt:5562:/ clear");
                    /*clear*/ /*_.RESE__V48*/
                    meltfptr[47] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V52*/
                    meltfptr[50] = 0 ;
                    /*_.LET___V40*/
                    meltfptr[36] = /*_.LET___V47*/ meltfptr[46];;

                    MELT_LOCATION("warmelt-macro.melt:5556:/ clear");
                    /*clear*/ /*_.CURELSE__V41*/
                    meltfptr[40] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.XELSE__V42*/
                    meltfptr[41] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PAIR_TAIL__V45*/
                    meltfptr[43] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V46*/
                    meltfptr[42] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#IS_PAIR__L18*/
                    meltfnum[16] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.LET___V47*/
                    meltfptr[46] = 0 ;
                    /*_._IFELSE___V39*/
                    meltfptr[37] = /*_.LET___V40*/ meltfptr[36];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5555:/ clear");
                    /*clear*/ /*_.LET___V40*/
                    meltfptr[36] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:5571:/ quasiblock");



                    MELT_CHECK_SIGNAL();
                    ;
                    /*^quasiblock*/


                    /*^rawallocobj*/
                    /*rawallocobj*/
                    {
                        melt_ptr_t newobj = 0;
                        melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_IF*/ meltfrout->tabval[6])), (4), "CLASS_SOURCE_IF");
                        /*_.INST__V55*/
                        meltfptr[50] =
                            newobj;
                    };
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V55*/ meltfptr[50])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V55*/ meltfptr[50]), (1), (/*_.LOC__V21*/ meltfptr[20]), "LOCA_LOCATION");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SIF_TEST", melt_magic_discr((melt_ptr_t)(/*_.INST__V55*/ meltfptr[50])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V55*/ meltfptr[50]), (2), (/*_.XCOND__V28*/ meltfptr[27]), "SIF_TEST");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SIF_THEN", melt_magic_discr((melt_ptr_t)(/*_.INST__V55*/ meltfptr[50])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V55*/ meltfptr[50]), (3), (/*_.XTHEN__V36*/ meltfptr[35]), "SIF_THEN");
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object (/*_.INST__V55*/ meltfptr[50], "newly made instance");
                    ;
                    /*_.RESP__V54*/
                    meltfptr[47] = /*_.INST__V55*/ meltfptr[50];;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:5575:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L21*/
                        meltfnum[19] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-macro.melt:5575:/ cond");
                        /*cond*/
                        if (/*_#MELT_NEED_DBG__L21*/ meltfnum[19]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L22*/ meltfnum[11] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-macro.melt:5575:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L22*/ meltfnum[11];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 5575;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "mexpand_if plain return resp";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RESP__V54*/ meltfptr[47];
                                        /*_.MELT_DEBUG_FUN__V57*/
                                        meltfptr[41] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V56*/
                                    meltfptr[40] = /*_.MELT_DEBUG_FUN__V57*/ meltfptr[41];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:5575:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L22*/
                                    meltfnum[11] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V57*/
                                    meltfptr[41] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V56*/ meltfptr[40] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-macro.melt:5575:/ locexp");
                            /*void*/
                            (void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L21*/
                        meltfnum[19] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V56*/
                        meltfptr[40] = 0 ;
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
                    MELT_LOCATION("warmelt-macro.melt:5576:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = /*_.RESP__V54*/ meltfptr[47];;

                    {
                        MELT_LOCATION("warmelt-macro.melt:5576:/ locexp");
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
                    /*finalret*/
                    goto meltlabend_rout ;
                    /*_.LET___V53*/
                    meltfptr[49] = /*_.RETURN___V58*/ meltfptr[43];;

                    MELT_LOCATION("warmelt-macro.melt:5571:/ clear");
                    /*clear*/ /*_.RESP__V54*/
                    meltfptr[47] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V58*/
                    meltfptr[43] = 0 ;
                    /*_._IFELSE___V39*/
                    meltfptr[37] = /*_.LET___V53*/ meltfptr[49];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5555:/ clear");
                    /*clear*/ /*_.LET___V53*/
                    meltfptr[49] = 0 ;
                }
                ;
            }
        ;
        /*_.LET___V35*/
        meltfptr[34] = /*_._IFELSE___V39*/ meltfptr[37];;

        MELT_LOCATION("warmelt-macro.melt:5552:/ clear");
        /*clear*/ /*_.XTHEN__V36*/
        meltfptr[35] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L15*/
        meltfnum[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V39*/
        meltfptr[37] = 0 ;
        /*_.LET___V31*/
        meltfptr[29] = /*_.LET___V35*/ meltfptr[34];;

        MELT_LOCATION("warmelt-macro.melt:5550:/ clear");
        /*clear*/ /*_.CURTHEN__V32*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V33*/
        meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V34*/
        meltfptr[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V35*/
        meltfptr[34] = 0 ;
        /*_.LET___V27*/
        meltfptr[26] = /*_.LET___V31*/ meltfptr[29];;

        MELT_LOCATION("warmelt-macro.melt:5548:/ clear");
        /*clear*/ /*_.XCOND__V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V31*/
        meltfptr[29] = 0 ;
        /*_.LET___V19*/
        meltfptr[17] = /*_.LET___V27*/ meltfptr[26];;

        MELT_LOCATION("warmelt-macro.melt:5538:/ clear");
        /*clear*/ /*_.CONT__V20*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURIF__V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L7*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L8*/
        meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L9*/
        meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L10*/
        meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V27*/
        meltfptr[26] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5532:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V19*/ meltfptr[17];;

        {
            MELT_LOCATION("warmelt-macro.melt:5532:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V8*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V11*/
        meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L5*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V16*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V19*/
        meltfptr[17] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_IF", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_89_WARMELTmiMACRO_MEXPAND_IF_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_89_WARMELTmiMACRO_MEXPAND_IF*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN fromline 1715 */

    /** start of frame for meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN// fromline 1531
        : public Melt_CallFrameWithValues<34>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<34> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN), clos) {};
        MeltFrame_meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN() //the constructor fromline 1605
            : Melt_CallFrameWithValues<34> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<34> (fil,lin, sizeof(MeltFrame_meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN)) {};
        MeltFrame_meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<34> (fil,lin, sizeof(MeltFrame_meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN


    /** end of frame for meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN fromline 1660**/

    /* end of frame for routine meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN fromline 1719 */

    /* classy proc frame meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN */
    MeltFrame_meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_WHEN", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5586:/ getarg");
    /*_.SEXPR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V5*/
    meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5587:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5587:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L1*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5587:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5587;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_when sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5587:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/
                        meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V7*/
                        meltfptr[6] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5587:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V6*/
            meltfptr[5] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5588:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:5588:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5588:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5588;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5588:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5588:/ clear");
            /*clear*/ /*_#IS_A__L3*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5589:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:5589:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5589:/ cond.else");

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
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5589;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5589:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5589:/ clear");
            /*clear*/ /*_#IS_A__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V12*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V11*/ meltfptr[9] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L5*/
        meltfnum[1] =
            ((/*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:5590:/ cond");
        /*cond*/
        if (/*_#NULL__L5*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V15*/ meltfptr[5] = (/*!MACROEXPAND_1*/ meltfrout->tabval[4]);;
                    /*_._IF___V14*/
                    meltfptr[12] = /*_.SETQ___V15*/ meltfptr[5];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5590:/ clear");
                    /*clear*/ /*_.SETQ___V15*/
                    meltfptr[5] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V14*/ meltfptr[12] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5591:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L6*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:5591:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V17*/ meltfptr[16] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5591:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5591;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                            /*_.MELT_ASSERT_FAILURE_FUN__V18*/
                            meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V17*/
                        meltfptr[16] = /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5591:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V18*/
                        meltfptr[17] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V16*/
            meltfptr[5] = /*_._IFELSE___V17*/ meltfptr[16];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5591:/ clear");
            /*clear*/ /*_#IS_OBJECT__L6*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V17*/
            meltfptr[16] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V16*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5592:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:5593:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V20*/
            meltfptr[16] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5594:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V21*/
            meltfptr[20] = slot;
        };
        ;
        /*_.LIST_FIRST__V22*/
        meltfptr[21] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V20*/ meltfptr[16])));;
        /*^compute*/
        /*_.CURPAIR__V23*/
        meltfptr[22] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V22*/ meltfptr[21])));;
        /*^compute*/
        /*_.CURIF__V24*/
        meltfptr[23] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_PAIR__L7*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])) == MELTOBMAG_PAIR);;
        /*^compute*/
        /*_#NOT__L8*/
        meltfnum[7] =
            (!(/*_#IS_PAIR__L7*/ meltfnum[0]));;
        MELT_LOCATION("warmelt-macro.melt:5598:/ cond");
        /*cond*/
        if (/*_#NOT__L8*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5599:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20]), ( "missing condition in WHEN"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*_.PAIR_TAIL__V25*/
        meltfptr[24] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;
        MELT_LOCATION("warmelt-macro.melt:5600:/ compute");
        /*_.CURPAIR__V23*/
        meltfptr[22] = /*_.SETQ___V26*/ meltfptr[25] = /*_.PAIR_TAIL__V25*/ meltfptr[24];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_PAIR__L9*/
        meltfnum[8] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])) == MELTOBMAG_PAIR);;
        /*^compute*/
        /*_#NOT__L10*/
        meltfnum[9] =
            (!(/*_#IS_PAIR__L9*/ meltfnum[8]));;
        MELT_LOCATION("warmelt-macro.melt:5601:/ cond");
        /*cond*/
        if (/*_#NOT__L10*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5602:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20]), ( "missing body in WHEN"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5603:/ quasiblock");



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
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.XCOND__V28*/
            meltfptr[27] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURIF__V24*/ meltfptr[23]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5604:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LOC__V21*/ meltfptr[20];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.XPROGN__V29*/
            meltfptr[28] =  melt_apply ((meltclosure_ptr_t)((/*!PAIRLIST_TO_PROGN*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5605:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_IF*/ meltfrout->tabval[6])), (4), "CLASS_SOURCE_IF");
            /*_.INST__V31*/
            meltfptr[30] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V31*/ meltfptr[30])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V31*/ meltfptr[30]), (1), (/*_.LOC__V21*/ meltfptr[20]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIF_TEST", melt_magic_discr((melt_ptr_t)(/*_.INST__V31*/ meltfptr[30])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V31*/ meltfptr[30]), (2), (/*_.XCOND__V28*/ meltfptr[27]), "SIF_TEST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIF_THEN", melt_magic_discr((melt_ptr_t)(/*_.INST__V31*/ meltfptr[30])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V31*/ meltfptr[30]), (3), (/*_.XPROGN__V29*/ meltfptr[28]), "SIF_THEN");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V31*/ meltfptr[30], "newly made instance");
        ;
        /*_.XWHEN__V30*/
        meltfptr[29] = /*_.INST__V31*/ meltfptr[30];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5610:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L11*/
            meltfnum[10] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5610:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L11*/ meltfnum[10]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5610:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5610;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_when return xwhen=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XWHEN__V30*/ meltfptr[29];
                            /*_.MELT_DEBUG_FUN__V33*/
                            meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V32*/
                        meltfptr[31] = /*_.MELT_DEBUG_FUN__V33*/ meltfptr[32];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5610:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L12*/
                        meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V33*/
                        meltfptr[32] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V32*/ meltfptr[31] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5610:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L11*/
            meltfnum[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V32*/
            meltfptr[31] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5611:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.XWHEN__V30*/ meltfptr[29];;

        {
            MELT_LOCATION("warmelt-macro.melt:5611:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*_.LET___V27*/
        meltfptr[26] = /*_.RETURN___V34*/ meltfptr[32];;

        MELT_LOCATION("warmelt-macro.melt:5603:/ clear");
        /*clear*/ /*_.XCOND__V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XPROGN__V29*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XWHEN__V30*/
        meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V34*/
        meltfptr[32] = 0 ;
        /*_.LET___V19*/
        meltfptr[17] = /*_.LET___V27*/ meltfptr[26];;

        MELT_LOCATION("warmelt-macro.melt:5592:/ clear");
        /*clear*/ /*_.CONT__V20*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURIF__V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L7*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L8*/
        meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L9*/
        meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L10*/
        meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V27*/
        meltfptr[26] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5586:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V19*/ meltfptr[17];;

        {
            MELT_LOCATION("warmelt-macro.melt:5586:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V8*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V11*/
        meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L5*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V16*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V19*/
        meltfptr[17] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_WHEN", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_90_WARMELTmiMACRO_MEXPAND_WHEN*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS fromline 1715 */

    /** start of frame for meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS// fromline 1531
        : public Melt_CallFrameWithValues<34>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<34> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS), clos) {};
        MeltFrame_meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS() //the constructor fromline 1605
            : Melt_CallFrameWithValues<34> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<34> (fil,lin, sizeof(MeltFrame_meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS)) {};
        MeltFrame_meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<34> (fil,lin, sizeof(MeltFrame_meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS


    /** end of frame for meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS fromline 1660**/

    /* end of frame for routine meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS fromline 1719 */

    /* classy proc frame meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS */
    MeltFrame_meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_UNLESS", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5622:/ getarg");
    /*_.SEXPR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V5*/
    meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5623:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5623:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L1*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5623:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5623;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_unless sexpr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5623:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/
                        meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V7*/
                        meltfptr[6] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5623:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V6*/
            meltfptr[5] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5624:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:5624:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5624:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5624;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5624:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5624:/ clear");
            /*clear*/ /*_#IS_A__L3*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5625:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:5625:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5625:/ cond.else");

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
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5625;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5625:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5625:/ clear");
            /*clear*/ /*_#IS_A__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V12*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V11*/ meltfptr[9] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L5*/
        meltfnum[1] =
            ((/*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:5626:/ cond");
        /*cond*/
        if (/*_#NULL__L5*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V15*/ meltfptr[5] = (/*!MACROEXPAND_1*/ meltfrout->tabval[4]);;
                    /*_._IF___V14*/
                    meltfptr[12] = /*_.SETQ___V15*/ meltfptr[5];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5626:/ clear");
                    /*clear*/ /*_.SETQ___V15*/
                    meltfptr[5] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V14*/ meltfptr[12] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5627:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L6*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:5627:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V17*/ meltfptr[16] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5627:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5627;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                            /*_.MELT_ASSERT_FAILURE_FUN__V18*/
                            meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V17*/
                        meltfptr[16] = /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5627:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V18*/
                        meltfptr[17] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V16*/
            meltfptr[5] = /*_._IFELSE___V17*/ meltfptr[16];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5627:/ clear");
            /*clear*/ /*_#IS_OBJECT__L6*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V17*/
            meltfptr[16] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V16*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5628:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:5629:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V20*/
            meltfptr[16] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5630:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V21*/
            meltfptr[20] = slot;
        };
        ;
        /*_.LIST_FIRST__V22*/
        meltfptr[21] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V20*/ meltfptr[16])));;
        /*^compute*/
        /*_.CURPAIR__V23*/
        meltfptr[22] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V22*/ meltfptr[21])));;
        /*^compute*/
        /*_.CURIF__V24*/
        meltfptr[23] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_PAIR__L7*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])) == MELTOBMAG_PAIR);;
        /*^compute*/
        /*_#NOT__L8*/
        meltfnum[7] =
            (!(/*_#IS_PAIR__L7*/ meltfnum[0]));;
        MELT_LOCATION("warmelt-macro.melt:5634:/ cond");
        /*cond*/
        if (/*_#NOT__L8*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5635:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20]), ( "missing condition in UNLESS"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*_.PAIR_TAIL__V25*/
        meltfptr[24] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;
        MELT_LOCATION("warmelt-macro.melt:5636:/ compute");
        /*_.CURPAIR__V23*/
        meltfptr[22] = /*_.SETQ___V26*/ meltfptr[25] = /*_.PAIR_TAIL__V25*/ meltfptr[24];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_PAIR__L9*/
        meltfnum[8] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])) == MELTOBMAG_PAIR);;
        /*^compute*/
        /*_#NOT__L10*/
        meltfnum[9] =
            (!(/*_#IS_PAIR__L9*/ meltfnum[8]));;
        MELT_LOCATION("warmelt-macro.melt:5637:/ cond");
        /*cond*/
        if (/*_#NOT__L10*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5638:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20]), ( "missing body in UNLESS"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5639:/ quasiblock");



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
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.XCOND__V28*/
            meltfptr[27] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURIF__V24*/ meltfptr[23]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5640:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LOC__V21*/ meltfptr[20];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.XPROGN__V29*/
            meltfptr[28] =  melt_apply ((meltclosure_ptr_t)((/*!PAIRLIST_TO_PROGN*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5641:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_IFELSE*/ meltfrout->tabval[6])), (5), "CLASS_SOURCE_IFELSE");
            /*_.INST__V31*/
            meltfptr[30] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V31*/ meltfptr[30])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V31*/ meltfptr[30]), (1), (/*_.LOC__V21*/ meltfptr[20]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIF_TEST", melt_magic_discr((melt_ptr_t)(/*_.INST__V31*/ meltfptr[30])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V31*/ meltfptr[30]), (2), (/*_.XCOND__V28*/ meltfptr[27]), "SIF_TEST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIF_THEN", melt_magic_discr((melt_ptr_t)(/*_.INST__V31*/ meltfptr[30])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V31*/ meltfptr[30]), (3), ((/*nil*/NULL)), "SIF_THEN");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIF_ELSE", melt_magic_discr((melt_ptr_t)(/*_.INST__V31*/ meltfptr[30])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V31*/ meltfptr[30]), (4), (/*_.XPROGN__V29*/ meltfptr[28]), "SIF_ELSE");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V31*/ meltfptr[30], "newly made instance");
        ;
        /*_.XUNLESS__V30*/
        meltfptr[29] = /*_.INST__V31*/ meltfptr[30];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5647:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L11*/
            meltfnum[10] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5647:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L11*/ meltfnum[10]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5647:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5647;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_unless return xunless=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XUNLESS__V30*/ meltfptr[29];
                            /*_.MELT_DEBUG_FUN__V33*/
                            meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V32*/
                        meltfptr[31] = /*_.MELT_DEBUG_FUN__V33*/ meltfptr[32];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5647:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L12*/
                        meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V33*/
                        meltfptr[32] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V32*/ meltfptr[31] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5647:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L11*/
            meltfnum[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V32*/
            meltfptr[31] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5648:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.XUNLESS__V30*/ meltfptr[29];;

        {
            MELT_LOCATION("warmelt-macro.melt:5648:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*_.LET___V27*/
        meltfptr[26] = /*_.RETURN___V34*/ meltfptr[32];;

        MELT_LOCATION("warmelt-macro.melt:5639:/ clear");
        /*clear*/ /*_.XCOND__V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XPROGN__V29*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XUNLESS__V30*/
        meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V34*/
        meltfptr[32] = 0 ;
        /*_.LET___V19*/
        meltfptr[17] = /*_.LET___V27*/ meltfptr[26];;

        MELT_LOCATION("warmelt-macro.melt:5628:/ clear");
        /*clear*/ /*_.CONT__V20*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURIF__V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L7*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L8*/
        meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L9*/
        meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L10*/
        meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V27*/
        meltfptr[26] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5622:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V19*/ meltfptr[17];;

        {
            MELT_LOCATION("warmelt-macro.melt:5622:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V8*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V11*/
        meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L5*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V16*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V19*/
        meltfptr[17] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_UNLESS", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_91_WARMELTmiMACRO_MEXPAND_UNLESS*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB fromline 1715 */

    /** start of frame for meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB// fromline 1531
        : public Melt_CallFrameWithValues<56>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<56> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB), clos) {};
        MeltFrame_meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB() //the constructor fromline 1605
            : Melt_CallFrameWithValues<56> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<56> (fil,lin, sizeof(MeltFrame_meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB)) {};
        MeltFrame_meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<56> (fil,lin, sizeof(MeltFrame_meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB


    /** end of frame for meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB fromline 1660**/

    /* end of frame for routine meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB fromline 1719 */

    /* classy proc frame meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB */
    MeltFrame_meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_WITH_CLONED_SYMB", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5661:/ getarg");
    /*_.SEXPR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V5*/
    meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5662:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5662:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L1*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5662:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5662;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_with_cloned_symb sexpr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5662:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/
                        meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V7*/
                        meltfptr[6] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5662:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V6*/
            meltfptr[5] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5663:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:5663:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5663:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5663;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5663:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5663:/ clear");
            /*clear*/ /*_#IS_A__L3*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5664:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:5664:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5664:/ cond.else");

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
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5664;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5664:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5664:/ clear");
            /*clear*/ /*_#IS_A__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V12*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V11*/ meltfptr[9] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L5*/
        meltfnum[1] =
            ((/*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:5665:/ cond");
        /*cond*/
        if (/*_#NULL__L5*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V15*/ meltfptr[5] = (/*!MACROEXPAND_1*/ meltfrout->tabval[4]);;
                    /*_._IF___V14*/
                    meltfptr[12] = /*_.SETQ___V15*/ meltfptr[5];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5665:/ clear");
                    /*clear*/ /*_.SETQ___V15*/
                    meltfptr[5] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V14*/ meltfptr[12] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5666:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L6*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:5666:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V17*/ meltfptr[16] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5666:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5666;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                            /*_.MELT_ASSERT_FAILURE_FUN__V18*/
                            meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V17*/
                        meltfptr[16] = /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5666:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V18*/
                        meltfptr[17] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V16*/
            meltfptr[5] = /*_._IFELSE___V17*/ meltfptr[16];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5666:/ clear");
            /*clear*/ /*_#IS_OBJECT__L6*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V17*/
            meltfptr[16] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V16*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5667:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:5668:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V20*/
            meltfptr[16] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5669:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V21*/
            meltfptr[20] = slot;
        };
        ;
        /*_.LIST_FIRST__V22*/
        meltfptr[21] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V20*/ meltfptr[16])));;
        /*^compute*/
        /*_.CURPAIR__V23*/
        meltfptr[22] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V22*/ meltfptr[21])));;
        /*^compute*/
        /*_.SYMBS__V24*/
        meltfptr[23] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5672:/ apply");
        /*apply*/
        {
            /*_.NEWENV__V25*/ meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!FRESH_ENV*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
        }
        ;
        /*_.BINDLIST__V26*/
        meltfptr[25] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[6]))));;
        /*^compute*/
        /*_.BODYL__V27*/
        meltfptr[26] = (/*nil*/NULL);;
        MELT_LOCATION("warmelt-macro.melt:5676:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V29*/
        meltfptr[28] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_19*/ meltfrout->tabval[19])), (2));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V29*/ meltfptr[28])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V29*/ meltfptr[28])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V29*/ meltfptr[28])->tabval[0] = (melt_ptr_t)(/*_.NEWENV__V25*/ meltfptr[24]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V29*/ meltfptr[28])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 1>= 0 && 1< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V29*/ meltfptr[28])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V29*/ meltfptr[28])->tabval[1] = (melt_ptr_t)(/*_.BINDLIST__V26*/ meltfptr[25]);
        ;
        /*_.PROCESSLIST__V28*/
        meltfptr[27] = /*_.LAMBDA___V29*/ meltfptr[28];;
        /*^compute*/
        /*_.PAIR_TAIL__V30*/
        meltfptr[29] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;
        MELT_LOCATION("warmelt-macro.melt:5717:/ compute");
        /*_.CURPAIR__V23*/
        meltfptr[22] = /*_.SETQ___V31*/ meltfptr[30] = /*_.PAIR_TAIL__V30*/ meltfptr[29];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5719:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L7*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5719:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L7*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5719:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[7];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5719;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_with_cloned_symb symbs=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SYMBS__V24*/ meltfptr[23];
                            /*_.MELT_DEBUG_FUN__V33*/
                            meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V32*/
                        meltfptr[31] = /*_.MELT_DEBUG_FUN__V33*/ meltfptr[32];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5719:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L8*/
                        meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V33*/
                        meltfptr[32] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V32*/ meltfptr[31] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5719:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L7*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V32*/
            meltfptr[31] = 0 ;
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
        /*_#NULL__L9*/
        meltfnum[7] =
            ((/*_.SYMBS__V24*/ meltfptr[23]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:5720:/ cond");
        /*cond*/
        if (/*_#NULL__L9*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-macro.melt:5721:/ locexp");
                        /*void*/
                        (void)0;
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V34*/
                    meltfptr[32] = 0 ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-macro.melt:5720:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_A__L10*/
                    meltfnum[0] =
                        melt_is_instance_of((melt_ptr_t)(/*_.SYMBS__V24*/ meltfptr[23]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
                    MELT_LOCATION("warmelt-macro.melt:5723:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L10*/ meltfnum[0]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:5724:/ quasiblock");


                                /*^getslot*/
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.SYMBS__V24*/ meltfptr[23]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
                                    /*_.LOCA_LOCATION__V36*/
                                    meltfptr[35] = slot;
                                };
                                ;
                                /*^cond*/
                                /*cond*/
                                if (/*_.LOCA_LOCATION__V36*/ meltfptr[35]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*_.SYLOC__V37*/ meltfptr[36] = /*_.LOCA_LOCATION__V36*/ meltfptr[35];;
                                    }
                                else
                                    {
                                        MELT_LOCATION("warmelt-macro.melt:5724:/ cond.else");

                                        /*_.SYLOC__V37*/
                                        meltfptr[36] = /*_.LOC__V21*/ meltfptr[20];;
                                    }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:5725:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.SYMBS__V24*/ meltfptr[23]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
                                    /*_.SYLIST__V38*/
                                    meltfptr[37] = slot;
                                };
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-macro.melt:5727:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[1];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.SYLIST__V38*/ meltfptr[37];
                                    /*_.PROCESSLIST__V39*/
                                    meltfptr[38] =  melt_apply ((meltclosure_ptr_t)(/*_.PROCESSLIST__V28*/ meltfptr[27]), (melt_ptr_t)(/*_.SYLOC__V37*/ meltfptr[36]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:5728:/ locexp");
                                    /*void*/
                                    (void)0;
                                }
                                ;
                                /*_._IFELSE___V35*/
                                meltfptr[31] = /*_.PROCESSLIST__V39*/ meltfptr[38];;

                                MELT_LOCATION("warmelt-macro.melt:5724:/ clear");
                                /*clear*/ /*_.LOCA_LOCATION__V36*/
                                meltfptr[35] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.SYLOC__V37*/
                                meltfptr[36] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.SYLIST__V38*/
                                meltfptr[37] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROCESSLIST__V39*/
                                meltfptr[38] = 0 ;
                                /*epilog*/
                            }
                            ;
                        }
                    else
                        {
                            MELT_LOCATION("warmelt-macro.melt:5723:/ cond.else");

                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                /*_#IS_LIST__L11*/
                                meltfnum[10] =
                                    (melt_magic_discr((melt_ptr_t)(/*_.SYMBS__V24*/ meltfptr[23])) == MELTOBMAG_LIST);;
                                MELT_LOCATION("warmelt-macro.melt:5730:/ cond");
                                /*cond*/
                                if (/*_#IS_LIST__L11*/ meltfnum[10]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-macro.melt:5731:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[1];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.SYMBS__V24*/ meltfptr[23];
                                                /*_.PROCESSLIST__V41*/
                                                meltfptr[36] =  melt_apply ((meltclosure_ptr_t)(/*_.PROCESSLIST__V28*/ meltfptr[27]), (melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;

                                            {
                                                MELT_LOCATION("warmelt-macro.melt:5732:/ locexp");
                                                /*void*/
                                                (void)0;
                                            }
                                            ;
                                            MELT_LOCATION("warmelt-macro.melt:5730:/ quasiblock");


                                            /*epilog*/

                                            /*^clear*/
                                            /*clear*/ /*_.PROCESSLIST__V41*/
                                            meltfptr[36] = 0 ;
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
                                            /*_#IS_MULTIPLE__L12*/
                                            meltfnum[11] =
                                                (melt_magic_discr((melt_ptr_t)(/*_.SYMBS__V24*/ meltfptr[23])) == MELTOBMAG_MULTIPLE);;
                                            MELT_LOCATION("warmelt-macro.melt:5733:/ cond");
                                            /*cond*/
                                            if (/*_#IS_MULTIPLE__L12*/ meltfnum[11]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-macro.melt:5734:/ apply");
                                                        /*apply*/
                                                        {
                                                            union meltparam_un argtab[1];
                                                            memset(&argtab, 0, sizeof(argtab));
                                                            /*^apply.arg*/
                                                            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_LIST*/ meltfrout->tabval[6]);
                                                            /*_.MULTIPLE_TO_LIST__V43*/
                                                            meltfptr[38] =  melt_apply ((meltclosure_ptr_t)((/*!MULTIPLE_TO_LIST*/ meltfrout->tabval[20])), (melt_ptr_t)(/*_.SYMBS__V24*/ meltfptr[23]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
                                                            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MULTIPLE_TO_LIST__V43*/ meltfptr[38];
                                                            /*_.PROCESSLIST__V44*/
                                                            meltfptr[36] =  melt_apply ((meltclosure_ptr_t)(/*_.PROCESSLIST__V28*/ meltfptr[27]), (melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                        }
                                                        ;

                                                        {
                                                            MELT_LOCATION("warmelt-macro.melt:5735:/ locexp");
                                                            /*void*/
                                                            (void)0;
                                                        }
                                                        ;
                                                        MELT_LOCATION("warmelt-macro.melt:5733:/ quasiblock");


                                                        /*epilog*/

                                                        /*^clear*/
                                                        /*clear*/ /*_.MULTIPLE_TO_LIST__V43*/
                                                        meltfptr[38] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.PROCESSLIST__V44*/
                                                        meltfptr[36] = 0 ;
                                                    }
                                                    ;
                                                }
                                            else    /*^cond.else*/
                                                {

                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        {
                                                            MELT_LOCATION("warmelt-macro.melt:5737:/ locexp");
                                                            /* error_plain */
                                                            melt_error_str((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20]), ( "WITH_CLONED_SYMB wants a list of symbols as first argument"), (melt_ptr_t)0);
                                                        }
                                                        ;

                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-macro.melt:5738:/ quasiblock");


                                                        /*_._RETVAL___V1*/
                                                        meltfptr[0] = (/*nil*/NULL);;
                                                        MELT_LOCATION("warmelt-macro.melt:5738:/ putxtraresult");
                                                        if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                                        if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                                        if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                                                        ;
                                                        /*^finalreturn*/
                                                        ;
                                                        /*finalret*/
                                                        goto meltlabend_rout ;
                                                        MELT_LOCATION("warmelt-macro.melt:5736:/ quasiblock");


                                                        /*_.PROGN___V46*/
                                                        meltfptr[36] = /*_.RETURN___V45*/ meltfptr[38];;
                                                        /*^compute*/
                                                        /*_._IFELSE___V42*/
                                                        meltfptr[37] = /*_.PROGN___V46*/ meltfptr[36];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-macro.melt:5733:/ clear");
                                                        /*clear*/ /*_.RETURN___V45*/
                                                        meltfptr[38] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.PROGN___V46*/
                                                        meltfptr[36] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;
                                            /*_._IFELSE___V40*/
                                            meltfptr[35] = /*_._IFELSE___V42*/ meltfptr[37];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-macro.melt:5730:/ clear");
                                            /*clear*/ /*_#IS_MULTIPLE__L12*/
                                            meltfnum[11] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_._IFELSE___V42*/
                                            meltfptr[37] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V35*/
                                meltfptr[31] = /*_._IFELSE___V40*/ meltfptr[35];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:5723:/ clear");
                                /*clear*/ /*_#IS_LIST__L11*/
                                meltfnum[10] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V40*/
                                meltfptr[35] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V34*/
                    meltfptr[32] = /*_._IFELSE___V35*/ meltfptr[31];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5720:/ clear");
                    /*clear*/ /*_#IS_A__L10*/
                    meltfnum[0] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V35*/
                    meltfptr[31] = 0 ;
                }
                ;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5739:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L13*/
            meltfnum[11] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5739:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L13*/ meltfnum[11]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5739:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L14*/ meltfnum[10];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5739;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_with_cloned_symb bindlist=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.BINDLIST__V26*/ meltfptr[25];
                            /*_.MELT_DEBUG_FUN__V48*/
                            meltfptr[36] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V47*/
                        meltfptr[38] = /*_.MELT_DEBUG_FUN__V48*/ meltfptr[36];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5739:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L14*/
                        meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V48*/
                        meltfptr[36] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V47*/ meltfptr[38] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5739:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L13*/
            meltfnum[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V47*/
            meltfptr[38] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5741:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.NEWENV__V25*/ meltfptr[24];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.MBODY__V50*/
            meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!EXPAND_PAIRLIST_AS_TUPLE*/ meltfrout->tabval[21])), (melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5744:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[24]);
            /*_.LIST_TO_MULTIPLE__V51*/
            meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[23])), (melt_ptr_t)(/*_.BINDLIST__V26*/ meltfptr[25]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:5742:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_LET*/ meltfrout->tabval[22])), (4), "CLASS_SOURCE_LET");
            /*_.INST__V53*/
            meltfptr[38] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V53*/ meltfptr[38])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V53*/ meltfptr[38]), (1), (/*_.LOC__V21*/ meltfptr[20]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SLET_BINDINGS", melt_magic_discr((melt_ptr_t)(/*_.INST__V53*/ meltfptr[38])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V53*/ meltfptr[38]), (2), (/*_.LIST_TO_MULTIPLE__V51*/ meltfptr[31]), "SLET_BINDINGS");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SLET_BODY", melt_magic_discr((melt_ptr_t)(/*_.INST__V53*/ meltfptr[38])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V53*/ meltfptr[38]), (3), (/*_.MBODY__V50*/ meltfptr[35]), "SLET_BODY");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V53*/ meltfptr[38], "newly made instance");
        ;
        /*_.MLET__V52*/
        meltfptr[36] = /*_.INST__V53*/ meltfptr[38];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5748:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L15*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5748:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L15*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L16*/ meltfnum[10] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:5748:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L16*/ meltfnum[10];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5748;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_with_cloned_symb mbody=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MBODY__V50*/ meltfptr[35];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n.. result mlet=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.MLET__V52*/ meltfptr[36];
                            /*_.MELT_DEBUG_FUN__V55*/
                            meltfptr[54] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V54*/
                        meltfptr[53] = /*_.MELT_DEBUG_FUN__V55*/ meltfptr[54];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5748:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L16*/
                        meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V55*/
                        meltfptr[54] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V54*/ meltfptr[53] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5748:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L15*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V54*/
            meltfptr[53] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5749:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.MLET__V52*/ meltfptr[36];;

        {
            MELT_LOCATION("warmelt-macro.melt:5749:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*_.LET___V49*/
        meltfptr[37] = /*_.RETURN___V56*/ meltfptr[54];;

        MELT_LOCATION("warmelt-macro.melt:5741:/ clear");
        /*clear*/ /*_.MBODY__V50*/
        meltfptr[35] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_TO_MULTIPLE__V51*/
        meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MLET__V52*/
        meltfptr[36] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V56*/
        meltfptr[54] = 0 ;
        /*_.LET___V19*/
        meltfptr[17] = /*_.LET___V49*/ meltfptr[37];;

        MELT_LOCATION("warmelt-macro.melt:5667:/ clear");
        /*clear*/ /*_.CONT__V20*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SYMBS__V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NEWENV__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BINDLIST__V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BODYL__V27*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PROCESSLIST__V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V30*/
        meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V31*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L9*/
        meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V34*/
        meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V49*/
        meltfptr[37] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5661:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V19*/ meltfptr[17];;

        {
            MELT_LOCATION("warmelt-macro.melt:5661:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V8*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V11*/
        meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L5*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V16*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V19*/
        meltfptr[17] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_WITH_CLONED_SYMB", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_92_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_93_WARMELTmiMACRO_LAMBDA_cl14(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_93_WARMELTmiMACRO_LAMBDA_cl14_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_93_WARMELTmiMACRO_LAMBDA_cl14 fromline 1715 */

    /** start of frame for meltrout_93_WARMELTmiMACRO_LAMBDA_cl14 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_93_WARMELTmiMACRO_LAMBDA_cl14// fromline 1531
        : public Melt_CallFrameWithValues<29>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_93_WARMELTmiMACRO_LAMBDA_cl14(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<29> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_93_WARMELTmiMACRO_LAMBDA_cl14), clos) {};
        MeltFrame_meltrout_93_WARMELTmiMACRO_LAMBDA_cl14() //the constructor fromline 1605
            : Melt_CallFrameWithValues<29> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_93_WARMELTmiMACRO_LAMBDA_cl14)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_93_WARMELTmiMACRO_LAMBDA_cl14(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<29> (fil,lin, sizeof(MeltFrame_meltrout_93_WARMELTmiMACRO_LAMBDA_cl14)) {};
        MeltFrame_meltrout_93_WARMELTmiMACRO_LAMBDA_cl14(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<29> (fil,lin, sizeof(MeltFrame_meltrout_93_WARMELTmiMACRO_LAMBDA_cl14), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_93_WARMELTmiMACRO_LAMBDA_cl14


    /** end of frame for meltrout_93_WARMELTmiMACRO_LAMBDA_cl14 fromline 1660**/

    /* end of frame for routine meltrout_93_WARMELTmiMACRO_LAMBDA_cl14 fromline 1719 */

    /* classy proc frame meltrout_93_WARMELTmiMACRO_LAMBDA_cl14 */
    MeltFrame_meltrout_93_WARMELTmiMACRO_LAMBDA_cl14
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_93_WARMELTmiMACRO_LAMBDA_cl14 fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl14", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5676:/ getarg");
    /*_.BLOC__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.BLIST__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.BLIST__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5677:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5677:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L1*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5677:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5677;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_with_cloned_symb/processlist bloc=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.BLOC__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  " blist=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.BLIST__V3*/ meltfptr[2];
                            /*_.MELT_DEBUG_FUN__V5*/
                            meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V4*/
                        meltfptr[3] = /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5677:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/
                        meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V5*/
                        meltfptr[4] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V4*/ meltfptr[3] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5677:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V4*/
            meltfptr[3] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5678:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_LIST_OR_NULL__L3*/
            meltfnum[1] =
                ((/*_.BLIST__V3*/ meltfptr[2]) == NULL
                 || (melt_unsafe_magic_discr((melt_ptr_t)(/*_.BLIST__V3*/ meltfptr[2])) == MELTOBMAG_LIST));;
            MELT_LOCATION("warmelt-macro.melt:5678:/ cond");
            /*cond*/
            if (/*_#IS_LIST_OR_NULL__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[3] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5678:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check blist";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5678;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.BLIST__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                            meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V7*/
                        meltfptr[3] = /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5678:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                        meltfptr[7] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V6*/
            meltfptr[4] = /*_._IFELSE___V7*/ meltfptr[3];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5678:/ clear");
            /*clear*/ /*_#IS_LIST_OR_NULL__L3*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V7*/
            meltfptr[3] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V6*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*citerblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        {
            /* start foreach_pair_component_in_list meltcit1__EACHLIST */
            for (/*_.CURPAIR__V9*/ meltfptr[7] = melt_list_first( (melt_ptr_t)/*_.BLIST__V3*/ meltfptr[2]);
                                   melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V9*/ meltfptr[7]) == MELTOBMAG_PAIR;
                                   /*_.CURPAIR__V9*/ meltfptr[7] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V9*/ meltfptr[7]))
                {
                    /*_.CURSYMB__V10*/ meltfptr[3] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V9*/ meltfptr[7]);



                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_NOT_A__L4*/
                    meltfnum[0] =
                        !melt_is_instance_of((melt_ptr_t)(/*_.CURSYMB__V10*/ meltfptr[3]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[2])));;
                    MELT_LOCATION("warmelt-macro.melt:5682:/ cond");
                    /*cond*/
                    if (/*_#IS_NOT_A__L4*/ meltfnum[0]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:5683:/ locexp");
                                    /* error_plain */
                                    melt_error_str((melt_ptr_t)(/*_.BLOC__V2*/ meltfptr[1]), ( "non-symbol in symbol list for (WITH_CLONED_SYMB <symbol-list> <body\
...>)"), (melt_ptr_t)0);
                                }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:5684:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] =  /*reallynil*/ NULL ;;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:5684:/ locexp");
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
                                /*finalret*/
                                goto meltlabend_rout ;
                                MELT_LOCATION("warmelt-macro.melt:5682:/ quasiblock");


                                /*_.PROGN___V13*/
                                meltfptr[12] = /*_.RETURN___V12*/ meltfptr[11];;
                                /*^compute*/
                                /*_._IF___V11*/
                                meltfptr[10] = /*_.PROGN___V13*/ meltfptr[12];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:5682:/ clear");
                                /*clear*/ /*_.RETURN___V12*/
                                meltfptr[11] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V13*/
                                meltfptr[12] = 0 ;
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
                    /*_#IS_A__L5*/
                    meltfnum[1] =
                        melt_is_instance_of((melt_ptr_t)(/*_.CURSYMB__V10*/ meltfptr[3]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[3])));;
                    MELT_LOCATION("warmelt-macro.melt:5685:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L5*/ meltfnum[1]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:5687:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CURSYMB__V10*/ meltfptr[3]),
                                                                  (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[4])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CURSYMB__V10*/ meltfptr[3]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                            /*_.NAMED_NAME__V14*/
                                            meltfptr[11] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.NAMED_NAME__V14*/ meltfptr[11] =  /*reallynil*/ NULL ;;
                                    }
                                ;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:5686:/ locexp");
                                    melt_warning_str(0, (melt_ptr_t)(/*_.BLOC__V2*/ meltfptr[1]), ( "keyword in WITH_CLONED_SYMB is discouraged"), (melt_ptr_t)(/*_.NAMED_NAME__V14*/ meltfptr[11]));
                                }
                                ;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:5685:/ clear");
                                /*clear*/ /*_.NAMED_NAME__V14*/
                                meltfptr[11] = 0 ;
                            }
                            ;
                        } /*noelse*/
                    ;

                    MELT_CHECK_SIGNAL();
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5688:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CURSYMB__V10*/ meltfptr[3];
                        /*_.FIND_ENV__V15*/
                        meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!FIND_ENV*/ meltfrout->tabval[5])), (melt_ptr_t)((/*~NEWENV*/ meltfclos->tabval[0])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*^cond*/
                    /*cond*/
                    if (/*_.FIND_ENV__V15*/ meltfptr[12]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:5690:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CURSYMB__V10*/ meltfptr[3]),
                                                                  (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[4])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CURSYMB__V10*/ meltfptr[3]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                            /*_.NAMED_NAME__V16*/
                                            meltfptr[11] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.NAMED_NAME__V16*/ meltfptr[11] =  /*reallynil*/ NULL ;;
                                    }
                                ;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:5689:/ locexp");
                                    melt_warning_str(0, (melt_ptr_t)(/*_.BLOC__V2*/ meltfptr[1]), ( "symbol in WITH_CLONED_SYMB hides previous definition"), (melt_ptr_t)(/*_.NAMED_NAME__V16*/ meltfptr[11]));
                                }
                                ;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:5688:/ clear");
                                /*clear*/ /*_.NAMED_NAME__V16*/
                                meltfptr[11] = 0 ;
                            }
                            ;
                        } /*noelse*/
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5691:/ quasiblock");



                    MELT_CHECK_SIGNAL();
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5694:/ quasiblock");


                    /*^rawallocobj*/
                    /*rawallocobj*/
                    {
                        melt_ptr_t newobj = 0;
                        melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_QUOTE*/ meltfrout->tabval[8])), (3), "CLASS_SOURCE_QUOTE");
                        /*_.INST__V18*/
                        meltfptr[17] =
                            newobj;
                    };
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V18*/ meltfptr[17])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V18*/ meltfptr[17]), (1), (/*_.BLOC__V2*/ meltfptr[1]), "LOCA_LOCATION");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SQUOTED", melt_magic_discr((melt_ptr_t)(/*_.INST__V18*/ meltfptr[17])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V18*/ meltfptr[17]), (2), (/*_.CURSYMB__V10*/ meltfptr[3]), "SQUOTED");
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object (/*_.INST__V18*/ meltfptr[17], "newly made instance");
                    ;
                    /*_.INST___V17*/
                    meltfptr[11] = /*_.INST__V18*/ meltfptr[17];;
                    MELT_LOCATION("warmelt-macro.melt:5694:/ blockmultialloc");
                    /*multiallocblock*/
                    {
                        struct meltletrec_1_st
                        {
                            struct MELT_MULTIPLE_STRUCT(1) rtup_0__TUPLREC__x2;
                            long meltletrec_1_endgap;
                        } *meltletrec_1_ptr = 0;
                        meltletrec_1_ptr = (struct meltletrec_1_st *) meltgc_allocate (sizeof (struct meltletrec_1_st), 0);
                        /*^blockmultialloc.initfill*/
                        /*inimult rtup_0__TUPLREC__x2*/
                        /*_.TUPLREC___V20*/
                        meltfptr[19] = (melt_ptr_t) &meltletrec_1_ptr->rtup_0__TUPLREC__x2;
                        meltletrec_1_ptr->rtup_0__TUPLREC__x2.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_MULTIPLE))));
                        meltletrec_1_ptr->rtup_0__TUPLREC__x2.nbval = 1;


                        /*^putuple*/
                        /*putupl#2*/
                        melt_assertmsg("putupl [:5694] #2 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V20*/ meltfptr[19]))== MELTOBMAG_MULTIPLE);
                        melt_assertmsg("putupl [:5694] #2 checkoff", (0>=0 && 0< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V20*/ meltfptr[19]))));
                        ((meltmultiple_ptr_t)(/*_.TUPLREC___V20*/ meltfptr[19]))->tabval[0] = (melt_ptr_t)(/*_.INST___V17*/ meltfptr[11]);
                        ;
                        /*^touch*/
                        meltgc_touch(/*_.TUPLREC___V20*/ meltfptr[19]);
                        ;
                        /*_.TUPLE___V19*/
                        meltfptr[18] = /*_.TUPLREC___V20*/ meltfptr[19];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5694:/ clear");
                        /*clear*/ /*_.TUPLREC___V20*/
                        meltfptr[19] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.TUPLREC___V20*/
                        meltfptr[19] = 0 ;
                    } /*end multiallocblock*/
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5691:/ quasiblock");


                    /*^rawallocobj*/
                    /*rawallocobj*/
                    {
                        melt_ptr_t newobj = 0;
                        melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_APPLY*/ meltfrout->tabval[6])), (4), "CLASS_SOURCE_APPLY");
                        /*_.INST__V22*/
                        meltfptr[21] =
                            newobj;
                    };
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V22*/ meltfptr[21])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V22*/ meltfptr[21]), (1), (/*_.BLOC__V2*/ meltfptr[1]), "LOCA_LOCATION");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SAPP_FUN", melt_magic_discr((melt_ptr_t)(/*_.INST__V22*/ meltfptr[21])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V22*/ meltfptr[21]), (3), ((/*!konst_7_CLONE_SYMBOL*/ meltfrout->tabval[7])), "SAPP_FUN");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SARGOP_ARGS", melt_magic_discr((melt_ptr_t)(/*_.INST__V22*/ meltfptr[21])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V22*/ meltfptr[21]), (2), (/*_.TUPLE___V19*/ meltfptr[18]), "SARGOP_ARGS");
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object (/*_.INST__V22*/ meltfptr[21], "newly made instance");
                    ;
                    /*_.APPCLONESYMB__V21*/
                    meltfptr[19] = /*_.INST__V22*/ meltfptr[21];;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5698:/ quasiblock");


                    /*^rawallocobj*/
                    /*rawallocobj*/
                    {
                        melt_ptr_t newobj = 0;
                        melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_LET_BINDING*/ meltfrout->tabval[9])), (5), "CLASS_SOURCE_LET_BINDING");
                        /*_.INST__V24*/
                        meltfptr[23] =
                            newobj;
                    };
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V24*/ meltfptr[23])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V24*/ meltfptr[23]), (1), (/*_.BLOC__V2*/ meltfptr[1]), "LOCA_LOCATION");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SLETB_TYPE", melt_magic_discr((melt_ptr_t)(/*_.INST__V24*/ meltfptr[23])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V24*/ meltfptr[23]), (3), ((/*!CTYPE_VALUE*/ meltfrout->tabval[10])), "SLETB_TYPE");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SLETB_BINDER", melt_magic_discr((melt_ptr_t)(/*_.INST__V24*/ meltfptr[23])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V24*/ meltfptr[23]), (2), (/*_.CURSYMB__V10*/ meltfptr[3]), "SLETB_BINDER");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SLETB_EXPR", melt_magic_discr((melt_ptr_t)(/*_.INST__V24*/ meltfptr[23])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V24*/ meltfptr[23]), (4), (/*_.APPCLONESYMB__V21*/ meltfptr[19]), "SLETB_EXPR");
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object (/*_.INST__V24*/ meltfptr[23], "newly made instance");
                    ;
                    /*_.BINDSYMB__V23*/
                    meltfptr[22] = /*_.INST__V24*/ meltfptr[23];;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5705:/ quasiblock");


                    /*^rawallocobj*/
                    /*rawallocobj*/
                    {
                        melt_ptr_t newobj = 0;
                        melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_LET_BINDING*/ meltfrout->tabval[11])), (4), "CLASS_LET_BINDING");
                        /*_.INST__V26*/
                        meltfptr[25] =
                            newobj;
                    };
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @BINDER", melt_magic_discr((melt_ptr_t)(/*_.INST__V26*/ meltfptr[25])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V26*/ meltfptr[25]), (0), (/*_.CURSYMB__V10*/ meltfptr[3]), "BINDER");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @LETBIND_TYPE", melt_magic_discr((melt_ptr_t)(/*_.INST__V26*/ meltfptr[25])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V26*/ meltfptr[25]), (1), ((/*!CTYPE_VALUE*/ meltfrout->tabval[10])), "LETBIND_TYPE");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @LETBIND_EXPR", melt_magic_discr((melt_ptr_t)(/*_.INST__V26*/ meltfptr[25])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V26*/ meltfptr[25]), (2), (/*_.APPCLONESYMB__V21*/ meltfptr[19]), "LETBIND_EXPR");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @LETBIND_LOC", melt_magic_discr((melt_ptr_t)(/*_.INST__V26*/ meltfptr[25])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V26*/ meltfptr[25]), (3), (/*_.BLOC__V2*/ meltfptr[1]), "LETBIND_LOC");
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object (/*_.INST__V26*/ meltfptr[25], "newly made instance");
                    ;
                    /*_.BIND__V25*/
                    meltfptr[24] = /*_.INST__V26*/ meltfptr[25];;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5711:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.BIND__V25*/ meltfptr[24];
                        /*_.PUT_ENV__V27*/
                        meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!PUT_ENV*/ meltfrout->tabval[12])), (melt_ptr_t)((/*~NEWENV*/ meltfclos->tabval[0])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-macro.melt:5712:/ locexp");
                        meltgc_append_list((melt_ptr_t)((/*~BINDLIST*/ meltfclos->tabval[1])), (melt_ptr_t)(/*_.BINDSYMB__V23*/ meltfptr[22]));
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:5713:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L6*/
                        meltfnum[5] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-macro.melt:5713:/ cond");
                        /*cond*/
                        if (/*_#MELT_NEED_DBG__L6*/ meltfnum[5]) /*then*/
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
                                    MELT_LOCATION("warmelt-macro.melt:5713:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[6];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 5713;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "mexpand_with_cloned_symb/processlist end bindsymb=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.BINDSYMB__V23*/ meltfptr[22];
                                        /*_.MELT_DEBUG_FUN__V29*/
                                        meltfptr[28] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V28*/
                                    meltfptr[27] = /*_.MELT_DEBUG_FUN__V29*/ meltfptr[28];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:5713:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L7*/
                                    meltfnum[6] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V29*/
                                    meltfptr[28] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V28*/ meltfptr[27] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-macro.melt:5713:/ locexp");
                            /*void*/
                            (void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L6*/
                        meltfnum[5] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V28*/
                        meltfptr[27] = 0 ;
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

                    MELT_LOCATION("warmelt-macro.melt:5691:/ clear");
                    /*clear*/ /*_.INST___V17*/
                    meltfptr[11] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.TUPLE___V19*/
                    meltfptr[18] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.APPCLONESYMB__V21*/
                    meltfptr[19] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.BINDSYMB__V23*/
                    meltfptr[22] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.BIND__V25*/
                    meltfptr[24] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PUT_ENV__V27*/
                    meltfptr[26] = 0 ;
                } /* end foreach_pair_component_in_list meltcit1__EACHLIST */
            /*_.CURPAIR__V9*/ meltfptr[7] = NULL;
            /*_.CURSYMB__V10*/
            meltfptr[3] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-macro.melt:5679:/ clear");
            /*clear*/ /*_.CURPAIR__V9*/
            meltfptr[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_.CURSYMB__V10*/
            meltfptr[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_#IS_NOT_A__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V11*/
            meltfptr[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_#IS_A__L5*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_.FIND_ENV__V15*/
            meltfptr[12] = 0 ;
        } /*endciterblock FOREACH_PAIR_COMPONENT_IN_LIST*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-macro.melt:5676:/ clear");
        /*clear*/ /*_.IFCPP___V6*/
        meltfptr[4] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl14", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_93_WARMELTmiMACRO_LAMBDA_cl14_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_93_WARMELTmiMACRO_LAMBDA_cl14*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF fromline 1715 */

    /** start of frame for meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF// fromline 1531
        : public Melt_CallFrameWithValues<56>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<56> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF), clos) {};
        MeltFrame_meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF() //the constructor fromline 1605
            : Melt_CallFrameWithValues<56> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<56> (fil,lin, sizeof(MeltFrame_meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF)) {};
        MeltFrame_meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<56> (fil,lin, sizeof(MeltFrame_meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF


    /** end of frame for meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF fromline 1660**/

    /* end of frame for routine meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF fromline 1719 */

    /* classy proc frame meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF */
    MeltFrame_meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_CPPIF", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5762:/ getarg");
    /*_.SEXPR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V5*/
    meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5763:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:5763:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5763:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5763;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                            meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V7*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5763:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                        meltfptr[7] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V6*/
            meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5763:/ clear");
            /*clear*/ /*_#IS_A__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V7*/
            meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V6*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5764:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:5764:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5764:/ cond.else");

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
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5764;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                            meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V10*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5764:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                        meltfptr[10] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V9*/
            meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5764:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V10*/
            meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V9*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L3*/
        meltfnum[0] =
            ((/*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:5765:/ cond");
        /*cond*/
        if (/*_#NULL__L3*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V13*/ meltfptr[6] = (/*!MACROEXPAND_1*/ meltfrout->tabval[3]);;
                    /*_._IF___V12*/
                    meltfptr[10] = /*_.SETQ___V13*/ meltfptr[6];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5765:/ clear");
                    /*clear*/ /*_.SETQ___V13*/
                    meltfptr[6] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V12*/ meltfptr[10] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5766:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L4*/
            meltfnum[3] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:5766:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L4*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5766:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5766;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                            /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                            meltfptr[15] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V15*/
                        meltfptr[14] = /*_.MELT_ASSERT_FAILURE_FUN__V16*/ meltfptr[15];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5766:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V16*/
                        meltfptr[15] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V14*/
            meltfptr[6] = /*_._IFELSE___V15*/ meltfptr[14];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5766:/ clear");
            /*clear*/ /*_#IS_OBJECT__L4*/
            meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[14] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5767:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L5*/
            meltfnum[3] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5767:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L5*/ meltfnum[3]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5767:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L6*/ meltfnum[5];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5767;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_cppif sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V18*/
                            meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V17*/
                        meltfptr[15] = /*_.MELT_DEBUG_FUN__V18*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5767:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L6*/
                        meltfnum[5] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V18*/
                        meltfptr[14] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V17*/ meltfptr[15] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5767:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L5*/
            meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V17*/
            meltfptr[15] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5768:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:5769:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V20*/
            meltfptr[15] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5770:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V21*/
            meltfptr[20] = slot;
        };
        ;
        /*_.LIST_FIRST__V22*/
        meltfptr[21] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V20*/ meltfptr[15])));;
        /*^compute*/
        /*_.CURPAIR__V23*/
        meltfptr[22] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V22*/ meltfptr[21])));;
        /*^compute*/
        /*_.CURIF__V24*/
        meltfptr[23] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_PAIR__L7*/
        meltfnum[5] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])) == MELTOBMAG_PAIR);;
        /*^compute*/
        /*_#NOT__L8*/
        meltfnum[3] =
            (!(/*_#IS_PAIR__L7*/ meltfnum[5]));;
        MELT_LOCATION("warmelt-macro.melt:5774:/ cond");
        /*cond*/
        if (/*_#NOT__L8*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5775:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20]), ( "missing condition in CPPIF"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*_.PAIR_TAIL__V25*/
        meltfptr[24] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;
        MELT_LOCATION("warmelt-macro.melt:5776:/ compute");
        /*_.CURPAIR__V23*/
        meltfptr[22] = /*_.SETQ___V26*/ meltfptr[25] = /*_.PAIR_TAIL__V25*/ meltfptr[24];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_PAIR__L9*/
        meltfnum[8] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])) == MELTOBMAG_PAIR);;
        /*^compute*/
        /*_#NOT__L10*/
        meltfnum[9] =
            (!(/*_#IS_PAIR__L9*/ meltfnum[8]));;
        MELT_LOCATION("warmelt-macro.melt:5777:/ cond");
        /*cond*/
        if (/*_#NOT__L10*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5778:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20]), ( "missing then in CPPIF"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5779:/ quasiblock");



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
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.XCOND__V28*/
            meltfptr[27] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURIF__V24*/ meltfptr[23]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5780:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L11*/
            meltfnum[10] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5780:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L11*/ meltfnum[10]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5780:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5780;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_cppif xcond";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XCOND__V28*/ meltfptr[27];
                            /*_.MELT_DEBUG_FUN__V30*/
                            meltfptr[29] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V29*/
                        meltfptr[28] = /*_.MELT_DEBUG_FUN__V30*/ meltfptr[29];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5780:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L12*/
                        meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V30*/
                        meltfptr[29] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V29*/ meltfptr[28] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5780:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L11*/
            meltfnum[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V29*/
            meltfptr[28] = 0 ;
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
        /*_#IS_STRING__L13*/
        meltfnum[11] =
            (melt_magic_discr((melt_ptr_t)(/*_.XCOND__V28*/ meltfptr[27])) == MELTOBMAG_STRING);;
        MELT_LOCATION("warmelt-macro.melt:5782:/ cond");
        /*cond*/
        if (/*_#IS_STRING__L13*/ meltfnum[11]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V31*/ meltfptr[29] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-macro.melt:5782:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_A__L14*/
                    meltfnum[10] =
                        melt_is_instance_of((melt_ptr_t)(/*_.XCOND__V28*/ meltfptr[27]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[5])));;
                    MELT_LOCATION("warmelt-macro.melt:5783:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L14*/ meltfnum[10]) /*then*/
                        {
                            /*^cond.then*/
                            /*_._IFELSE___V32*/ meltfptr[28] = (/*nil*/NULL);;
                        }
                    else
                        {
                            MELT_LOCATION("warmelt-macro.melt:5783:/ cond.else");

                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:5785:/ locexp");
                                    /* error_plain */
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20]), ( "invalid cpp-condition in CPPIF - string or symbol expected"), (melt_ptr_t)0);
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-macro.melt:5786:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] = (/*nil*/NULL);;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:5786:/ locexp");
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
                                /*finalret*/
                                goto meltlabend_rout ;
                                MELT_LOCATION("warmelt-macro.melt:5784:/ quasiblock");


                                /*_.PROGN___V34*/
                                meltfptr[33] = /*_.RETURN___V33*/ meltfptr[32];;
                                /*^compute*/
                                /*_._IFELSE___V32*/
                                meltfptr[28] = /*_.PROGN___V34*/ meltfptr[33];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:5783:/ clear");
                                /*clear*/ /*_.RETURN___V33*/
                                meltfptr[32] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V34*/
                                meltfptr[33] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V31*/
                    meltfptr[29] = /*_._IFELSE___V32*/ meltfptr[28];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5782:/ clear");
                    /*clear*/ /*_#IS_A__L14*/
                    meltfnum[10] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V32*/
                    meltfptr[28] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:5787:/ quasiblock");


        /*_.CURTHEN__V36*/
        meltfptr[33] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;
        /*^compute*/
        /*_.PAIR_TAIL__V37*/
        meltfptr[28] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;
        MELT_LOCATION("warmelt-macro.melt:5788:/ compute");
        /*_.CURPAIR__V23*/
        meltfptr[22] = /*_.SETQ___V38*/ meltfptr[37] = /*_.PAIR_TAIL__V37*/ meltfptr[28];;
        MELT_LOCATION("warmelt-macro.melt:5789:/ quasiblock");



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
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.XTHEN__V40*/
            meltfptr[39] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURTHEN__V36*/ meltfptr[33]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.XELSE__V41*/
        meltfptr[40] = (/*nil*/NULL);;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5792:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L15*/
            meltfnum[10] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5792:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L15*/ meltfnum[10]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L16*/ meltfnum[15] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:5792:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L16*/ meltfnum[15];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5792;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_cppif xthen";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XTHEN__V40*/ meltfptr[39];
                            /*_.MELT_DEBUG_FUN__V43*/
                            meltfptr[42] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V42*/
                        meltfptr[41] = /*_.MELT_DEBUG_FUN__V43*/ meltfptr[42];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5792:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L16*/
                        meltfnum[15] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V43*/
                        meltfptr[42] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V42*/ meltfptr[41] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5792:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L15*/
            meltfnum[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V42*/
            meltfptr[41] = 0 ;
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
        /*_#IS_PAIR__L17*/
        meltfnum[15] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])) == MELTOBMAG_PAIR);;
        MELT_LOCATION("warmelt-macro.melt:5793:/ cond");
        /*cond*/
        if (/*_#IS_PAIR__L17*/ meltfnum[15]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:5794:/ quasiblock");


                    /*_.CURELSE__V44*/
                    meltfptr[42] =
                        (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5795:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[3];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                        /*^apply.arg*/
                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
                        /*^apply.arg*/
                        argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                        /*_.GOTXELSE__V45*/
                        meltfptr[41] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURELSE__V44*/ meltfptr[42]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:5796:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L18*/
                        meltfnum[10] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-macro.melt:5796:/ cond");
                        /*cond*/
                        if (/*_#MELT_NEED_DBG__L18*/ meltfnum[10]) /*then*/
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
                                    MELT_LOCATION("warmelt-macro.melt:5796:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L19*/ meltfnum[18];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 5796;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "mexpand_cppif gotxelse";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.GOTXELSE__V45*/ meltfptr[41];
                                        /*_.MELT_DEBUG_FUN__V47*/
                                        meltfptr[46] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V46*/
                                    meltfptr[45] = /*_.MELT_DEBUG_FUN__V47*/ meltfptr[46];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:5796:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L19*/
                                    meltfnum[18] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V47*/
                                    meltfptr[46] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V46*/ meltfptr[45] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-macro.melt:5796:/ locexp");
                            /*void*/
                            (void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L18*/
                        meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V46*/
                        meltfptr[45] = 0 ;
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
                    /*_.PAIR_TAIL__V48*/
                    meltfptr[46] =
                        (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])));;
                    MELT_LOCATION("warmelt-macro.melt:5797:/ compute");
                    /*_.CURPAIR__V23*/
                    meltfptr[22] = /*_.SETQ___V49*/ meltfptr[45] = /*_.PAIR_TAIL__V48*/ meltfptr[46];;
                    MELT_LOCATION("warmelt-macro.melt:5798:/ compute");
                    /*_.XELSE__V41*/
                    meltfptr[40] = /*_.SETQ___V50*/ meltfptr[49] = /*_.GOTXELSE__V45*/ meltfptr[41];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_PAIR__L20*/
                    meltfnum[18] =
                        (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22])) == MELTOBMAG_PAIR);;
                    MELT_LOCATION("warmelt-macro.melt:5799:/ cond");
                    /*cond*/
                    if (/*_#IS_PAIR__L20*/ meltfnum[18]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:5800:/ locexp");
                                    /* error_plain */
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20]), ( "CPPIF with more than three sons"), (melt_ptr_t)0);
                                }
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;

                    MELT_LOCATION("warmelt-macro.melt:5794:/ clear");
                    /*clear*/ /*_.CURELSE__V44*/
                    meltfptr[42] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.GOTXELSE__V45*/
                    meltfptr[41] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PAIR_TAIL__V48*/
                    meltfptr[46] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V49*/
                    meltfptr[45] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V50*/
                    meltfptr[49] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#IS_PAIR__L20*/
                    meltfnum[18] = 0 ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5801:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^quasiblock*/


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_CPPIF*/ meltfrout->tabval[6])), (5), "CLASS_SOURCE_CPPIF");
            /*_.INST__V53*/
            meltfptr[46] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V53*/ meltfptr[46])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V53*/ meltfptr[46]), (1), (/*_.LOC__V21*/ meltfptr[20]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIFP_COND", melt_magic_discr((melt_ptr_t)(/*_.INST__V53*/ meltfptr[46])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V53*/ meltfptr[46]), (2), (/*_.XCOND__V28*/ meltfptr[27]), "SIFP_COND");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIFP_THEN", melt_magic_discr((melt_ptr_t)(/*_.INST__V53*/ meltfptr[46])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V53*/ meltfptr[46]), (3), (/*_.XTHEN__V40*/ meltfptr[39]), "SIFP_THEN");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIFP_ELSE", melt_magic_discr((melt_ptr_t)(/*_.INST__V53*/ meltfptr[46])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V53*/ meltfptr[46]), (4), (/*_.XELSE__V41*/ meltfptr[40]), "SIFP_ELSE");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V53*/ meltfptr[46], "newly made instance");
        ;
        /*_.RESP__V52*/
        meltfptr[41] = /*_.INST__V53*/ meltfptr[46];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5808:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L21*/
            meltfnum[10] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5808:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L21*/ meltfnum[10]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5808:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L22*/ meltfnum[18];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5808;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_cppif return resp";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RESP__V52*/ meltfptr[41];
                            /*_.MELT_DEBUG_FUN__V55*/
                            meltfptr[49] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V54*/
                        meltfptr[45] = /*_.MELT_DEBUG_FUN__V55*/ meltfptr[49];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5808:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L22*/
                        meltfnum[18] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V55*/
                        meltfptr[49] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V54*/ meltfptr[45] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5808:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L21*/
            meltfnum[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V54*/
            meltfptr[45] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5809:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.RESP__V52*/ meltfptr[41];;

        {
            MELT_LOCATION("warmelt-macro.melt:5809:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*_.LET___V51*/
        meltfptr[42] = /*_.RETURN___V56*/ meltfptr[49];;

        MELT_LOCATION("warmelt-macro.melt:5801:/ clear");
        /*clear*/ /*_.RESP__V52*/
        meltfptr[41] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V56*/
        meltfptr[49] = 0 ;
        /*_.LET___V39*/
        meltfptr[38] = /*_.LET___V51*/ meltfptr[42];;

        MELT_LOCATION("warmelt-macro.melt:5789:/ clear");
        /*clear*/ /*_.XTHEN__V40*/
        meltfptr[39] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XELSE__V41*/
        meltfptr[40] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L17*/
        meltfnum[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V51*/
        meltfptr[42] = 0 ;
        /*_.LET___V35*/
        meltfptr[32] = /*_.LET___V39*/ meltfptr[38];;

        MELT_LOCATION("warmelt-macro.melt:5787:/ clear");
        /*clear*/ /*_.CURTHEN__V36*/
        meltfptr[33] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V37*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V38*/
        meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V39*/
        meltfptr[38] = 0 ;
        /*_.LET___V27*/
        meltfptr[26] = /*_.LET___V35*/ meltfptr[32];;

        MELT_LOCATION("warmelt-macro.melt:5779:/ clear");
        /*clear*/ /*_.XCOND__V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_STRING__L13*/
        meltfnum[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V31*/
        meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V35*/
        meltfptr[32] = 0 ;
        /*_.LET___V19*/
        meltfptr[14] = /*_.LET___V27*/ meltfptr[26];;

        MELT_LOCATION("warmelt-macro.melt:5768:/ clear");
        /*clear*/ /*_.CONT__V20*/
        meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURIF__V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L7*/
        meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L8*/
        meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L9*/
        meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L10*/
        meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V27*/
        meltfptr[26] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5762:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V19*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-macro.melt:5762:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V6*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V9*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L3*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V12*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V19*/
        meltfptr[14] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_CPPIF", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_94_WARMELTmiMACRO_MEXPAND_CPPIF*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_95_WARMELTmiMACRO_MEXPAND_COND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_95_WARMELTmiMACRO_MEXPAND_COND_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_95_WARMELTmiMACRO_MEXPAND_COND fromline 1715 */

    /** start of frame for meltrout_95_WARMELTmiMACRO_MEXPAND_COND of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_95_WARMELTmiMACRO_MEXPAND_COND// fromline 1531
        : public Melt_CallFrameWithValues<86>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_95_WARMELTmiMACRO_MEXPAND_COND(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<86> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_95_WARMELTmiMACRO_MEXPAND_COND), clos) {};
        MeltFrame_meltrout_95_WARMELTmiMACRO_MEXPAND_COND() //the constructor fromline 1605
            : Melt_CallFrameWithValues<86> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_95_WARMELTmiMACRO_MEXPAND_COND)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_95_WARMELTmiMACRO_MEXPAND_COND(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<86> (fil,lin, sizeof(MeltFrame_meltrout_95_WARMELTmiMACRO_MEXPAND_COND)) {};
        MeltFrame_meltrout_95_WARMELTmiMACRO_MEXPAND_COND(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<86> (fil,lin, sizeof(MeltFrame_meltrout_95_WARMELTmiMACRO_MEXPAND_COND), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_95_WARMELTmiMACRO_MEXPAND_COND


    /** end of frame for meltrout_95_WARMELTmiMACRO_MEXPAND_COND fromline 1660**/

    /* end of frame for routine meltrout_95_WARMELTmiMACRO_MEXPAND_COND fromline 1719 */

    /* classy proc frame meltrout_95_WARMELTmiMACRO_MEXPAND_COND */
    MeltFrame_meltrout_95_WARMELTmiMACRO_MEXPAND_COND
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_95_WARMELTmiMACRO_MEXPAND_COND fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_COND", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5818:/ getarg");
    /*_.SEXPR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V5*/
    meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5819:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:5819:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5819:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5819;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                            meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V7*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5819:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                        meltfptr[7] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V6*/
            meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5819:/ clear");
            /*clear*/ /*_#IS_A__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V7*/
            meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V6*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5820:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:5820:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5820:/ cond.else");

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
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5820;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                            meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V10*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5820:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                        meltfptr[10] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V9*/
            meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5820:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V10*/
            meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V9*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5821:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L3*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5821:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L3*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L4*/ meltfnum[3] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:5821:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L4*/ meltfnum[3];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5821;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_cond sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V13*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V12*/
                        meltfptr[10] = /*_.MELT_DEBUG_FUN__V13*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5821:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L4*/
                        meltfnum[3] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V13*/
                        meltfptr[6] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V12*/ meltfptr[10] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5821:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L3*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V12*/
            meltfptr[10] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5822:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:5823:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V15*/
            meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5824:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V16*/
            meltfptr[15] = slot;
        };
        ;
        /*_.LIST_FIRST__V17*/
        meltfptr[16] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V15*/ meltfptr[10])));;
        /*^compute*/
        /*_.PAIR_TAIL__V18*/
        meltfptr[17] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V17*/ meltfptr[16])));;
        MELT_LOCATION("warmelt-macro.melt:5828:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V20*/
        meltfptr[19] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_7*/ meltfrout->tabval[7])), (1));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V20*/ meltfptr[19])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V20*/ meltfptr[19])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V20*/ meltfptr[19])->tabval[0] = (melt_ptr_t)(/*_.LOC__V16*/ meltfptr[15]);
        ;
        /*_.LAMBDA___V19*/
        meltfptr[18] = /*_.LAMBDA___V20*/ meltfptr[19];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5825:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[5]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.LAMBDA___V19*/ meltfptr[18];
            /*_.CEXPTUPLE__V21*/
            meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!PAIRLIST_TO_MULTIPLE*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.PAIR_TAIL__V18*/ meltfptr[17]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_#NBCOND__L5*/
        meltfnum[3] =
            (melt_multiple_length((melt_ptr_t)(/*_.CEXPTUPLE__V21*/ meltfptr[20])));;
        MELT_LOCATION("warmelt-macro.melt:5834:/ quasiblock");


        /*_.LX__V23*/
        meltfptr[22] =
            (melt_multiple_nth((melt_ptr_t)(/*_.CEXPTUPLE__V21*/ meltfptr[20]), (-1)));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5835:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L6*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5835:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L6*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5835:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[6];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5835;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_cond lastcexp lx";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.LX__V23*/ meltfptr[22];
                            /*_.MELT_DEBUG_FUN__V25*/
                            meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V24*/
                        meltfptr[23] = /*_.MELT_DEBUG_FUN__V25*/ meltfptr[24];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5835:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L7*/
                        meltfnum[6] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V25*/
                        meltfptr[24] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V24*/ meltfptr[23] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5835:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L6*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V24*/
            meltfptr[23] = 0 ;
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
        /*_.LET___V22*/
        meltfptr[21] = /*_.LX__V23*/ meltfptr[22];;

        MELT_LOCATION("warmelt-macro.melt:5834:/ clear");
        /*clear*/ /*_.LX__V23*/
        meltfptr[22] = 0 ;
        /*_.LASTCEXP__V26*/
        meltfptr[24] = /*_.LET___V22*/ meltfptr[21];;
        /*^compute*/
        /*_#IX__L8*/
        meltfnum[6] =
            ((/*_#NBCOND__L5*/ meltfnum[3]) - (1));;
        /*^compute*/
        /*_.RES__V27*/
        meltfptr[23] = (/*nil*/NULL);;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5840:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L9*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5840:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L9*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5840:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L10*/ meltfnum[9];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5840;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_cond cexptuple";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CEXPTUPLE__V21*/ meltfptr[20];
                            /*_.MELT_DEBUG_FUN__V29*/
                            meltfptr[28] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V28*/
                        meltfptr[22] = /*_.MELT_DEBUG_FUN__V29*/ meltfptr[28];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5840:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L10*/
                        meltfnum[9] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V29*/
                        meltfptr[28] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V28*/ meltfptr[22] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5840:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L9*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V28*/
            meltfptr[22] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5841:/ loop");
        /*loop*/
        {
meltlabloop_CONDLOOP_1:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;

                MELT_CHECK_SIGNAL();
                ;
                /*_#ltI__L11*/
                meltfnum[9] =
                    ((/*_#IX__L8*/ meltfnum[6]) < (0));;
                MELT_LOCATION("warmelt-macro.melt:5842:/ cond");
                /*cond*/
                if (/*_#ltI__L11*/ meltfnum[9]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^quasiblock*/


                            /*^compute*/
                            /*_.CONDLOOP__V31*/ meltfptr[22] =  /*reallynil*/ NULL ;;

                            /*^exit*/
                            /*exit*/
                            {
                                goto meltlabexit_CONDLOOP_1;
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    } /*noelse*/
                ;
                MELT_LOCATION("warmelt-macro.melt:5843:/ quasiblock");


                /*_.CURCOND__V32*/
                meltfptr[31] =
                    (melt_multiple_nth((melt_ptr_t)(/*_.CEXPTUPLE__V21*/ meltfptr[20]), (/*_#IX__L8*/ meltfnum[6])));;

#if MELT_HAVE_DEBUG
                MELT_LOCATION("warmelt-macro.melt:5844:/ cppif.then");
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
                    /*_#MELT_NEED_DBG__L12*/
                    meltfnum[0] =
                        /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                        (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                        0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                        ;;
                    MELT_LOCATION("warmelt-macro.melt:5844:/ cond");
                    /*cond*/
                    if (/*_#MELT_NEED_DBG__L12*/ meltfnum[0]) /*then*/
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
                                MELT_LOCATION("warmelt-macro.melt:5844:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[5];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L13*/ meltfnum[12];
                                    /*^apply.arg*/
                                    argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                    /*^apply.arg*/
                                    argtab[2].meltbp_long = 5844;
                                    /*^apply.arg*/
                                    argtab[3].meltbp_cstring =  "mexpand_cond curcond";
                                    /*^apply.arg*/
                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CURCOND__V32*/ meltfptr[31];
                                    /*_.MELT_DEBUG_FUN__V34*/
                                    meltfptr[33] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                /*_._IF___V33*/
                                meltfptr[32] = /*_.MELT_DEBUG_FUN__V34*/ meltfptr[33];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:5844:/ clear");
                                /*clear*/ /*_#MELT_CALLCOUNT__L13*/
                                meltfnum[12] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.MELT_DEBUG_FUN__V34*/
                                meltfptr[33] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IF___V33*/ meltfptr[32] =  /*reallynil*/ NULL ;;
                        }
                    ;

                    {
                        MELT_LOCATION("warmelt-macro.melt:5844:/ locexp");
                        /*void*/
                        (void)0;
                    }
                    ;
                    /*^quasiblock*/


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_#MELT_NEED_DBG__L12*/
                    meltfnum[0] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IF___V33*/
                    meltfptr[32] = 0 ;
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
                /*_#NULL__L14*/
                meltfnum[12] =
                    ((/*_.RES__V27*/ meltfptr[23]) == NULL);;
                MELT_LOCATION("warmelt-macro.melt:5846:/ cond");
                /*cond*/
                if (/*_#NULL__L14*/ meltfnum[12]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            MELT_CHECK_SIGNAL();
                            ;
                            /*_#IS_A__L16*/
                            meltfnum[15] =
                                melt_is_instance_of((melt_ptr_t)(/*_.CURCOND__V32*/ meltfptr[31]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
                            MELT_LOCATION("warmelt-macro.melt:5846:/ cond");
                            /*cond*/
                            if (/*_#IS_A__L16*/ meltfnum[15]) /*then*/
                                {
                                    /*^cond.then*/
                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        MELT_LOCATION("warmelt-macro.melt:5849:/ getslot");
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CURCOND__V32*/ meltfptr[31]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
                                            /*_.SEXP_CONTENTS__V35*/
                                            meltfptr[33] = slot;
                                        };
                                        ;
                                        /*_.LIST_FIRST__V36*/
                                        meltfptr[32] =
                                            (melt_list_first((melt_ptr_t)(/*_.SEXP_CONTENTS__V35*/ meltfptr[33])));;
                                        /*^compute*/
                                        /*_.PAIR_HEAD__V37*/
                                        meltfptr[36] =
                                            (melt_pair_head((melt_ptr_t)(/*_.LIST_FIRST__V36*/ meltfptr[32])));;
                                        /*^compute*/
                                        /*_#eqeq__L18*/
                                        meltfnum[17] =
                                            (((/*!konst_8_ELSE*/ meltfrout->tabval[8])) == (/*_.PAIR_HEAD__V37*/ meltfptr[36]));;
                                        /*^compute*/
                                        /*_#_IF___L17*/
                                        meltfnum[16] = /*_#eqeq__L18*/ meltfnum[17];;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-macro.melt:5846:/ clear");
                                        /*clear*/ /*_.SEXP_CONTENTS__V35*/
                                        meltfptr[33] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.LIST_FIRST__V36*/
                                        meltfptr[32] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.PAIR_HEAD__V37*/
                                        meltfptr[36] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_#eqeq__L18*/
                                        meltfnum[17] = 0 ;
                                    }
                                    ;
                                }
                            else    /*^cond.else*/
                                {

                                    /*_#_IF___L17*/ meltfnum[16] = 0;;
                                }
                            ;
                            /*^compute*/
                            /*_#_IF___L15*/
                            meltfnum[0] = /*_#_IF___L17*/ meltfnum[16];;
                            /*epilog*/

                            MELT_LOCATION("warmelt-macro.melt:5846:/ clear");
                            /*clear*/ /*_#IS_A__L16*/
                            meltfnum[15] = 0 ;
                            /*^clear*/
                            /*clear*/ /*_#_IF___L17*/
                            meltfnum[16] = 0 ;
                        }
                        ;
                    }
                else    /*^cond.else*/
                    {

                        /*_#_IF___L15*/ meltfnum[0] = 0;;
                    }
                ;
                MELT_LOCATION("warmelt-macro.melt:5846:/ cond");
                /*cond*/
                if (/*_#_IF___L15*/ meltfnum[0]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            MELT_LOCATION("warmelt-macro.melt:5850:/ quasiblock");


                            MELT_LOCATION("warmelt-macro.melt:5851:/ getslot");
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.CURCOND__V32*/ meltfptr[31]) /*=obj*/;
                                melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
                                /*_.SEXP_CONTENTS__V38*/
                                meltfptr[33] = slot;
                            };
                            ;
                            /*_.LIST_FIRST__V39*/
                            meltfptr[32] =
                                (melt_list_first((melt_ptr_t)(/*_.SEXP_CONTENTS__V38*/ meltfptr[33])));;
                            /*^compute*/
                            /*_.RESTPAIRS__V40*/
                            meltfptr[36] =
                                (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V39*/ meltfptr[32])));;

                            MELT_CHECK_SIGNAL();
                            ;
                            MELT_LOCATION("warmelt-macro.melt:5853:/ cond");
                            /*cond*/
                            if (/*_.RESTPAIRS__V40*/ meltfptr[36]) /*then*/
                                {
                                    /*^cond.then*/
                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        MELT_LOCATION("warmelt-macro.melt:5855:/ getslot");
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CURCOND__V32*/ meltfptr[31]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
                                            /*_.LOCA_LOCATION__V42*/
                                            meltfptr[41] = slot;
                                        };
                                        ;

                                        MELT_CHECK_SIGNAL();
                                        ;
                                        MELT_LOCATION("warmelt-macro.melt:5854:/ apply");
                                        /*apply*/
                                        {
                                            union meltparam_un argtab[4];
                                            memset(&argtab, 0, sizeof(argtab));
                                            /*^apply.arg*/
                                            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LOCA_LOCATION__V42*/ meltfptr[41];
                                            /*^apply.arg*/
                                            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                                            /*^apply.arg*/
                                            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
                                            /*^apply.arg*/
                                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                                            /*_.PAIRLIST_TO_PROGN__V43*/
                                            meltfptr[42] =  melt_apply ((meltclosure_ptr_t)((/*!PAIRLIST_TO_PROGN*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.RESTPAIRS__V40*/ meltfptr[36]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                        }
                                        ;
                                        /*^compute*/
                                        /*_.RES__V27*/
                                        meltfptr[23] = /*_.SETQ___V44*/ meltfptr[43] = /*_.PAIRLIST_TO_PROGN__V43*/ meltfptr[42];;
                                        /*_._IF___V41*/
                                        meltfptr[40] = /*_.SETQ___V44*/ meltfptr[43];;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-macro.melt:5853:/ clear");
                                        /*clear*/ /*_.LOCA_LOCATION__V42*/
                                        meltfptr[41] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.PAIRLIST_TO_PROGN__V43*/
                                        meltfptr[42] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.SETQ___V44*/
                                        meltfptr[43] = 0 ;
                                    }
                                    ;
                                }
                            else    /*^cond.else*/
                                {

                                    /*_._IF___V41*/ meltfptr[40] =  /*reallynil*/ NULL ;;
                                }
                            ;

#if MELT_HAVE_DEBUG
                            MELT_LOCATION("warmelt-macro.melt:5858:/ cppif.then");
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
                                /*_#MELT_NEED_DBG__L19*/
                                meltfnum[17] =
                                    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                    (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                    0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                    ;;
                                MELT_LOCATION("warmelt-macro.melt:5858:/ cond");
                                /*cond*/
                                if (/*_#MELT_NEED_DBG__L19*/ meltfnum[17]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            /*_#MELT_CALLCOUNT__L20*/ meltfnum[15] =
                                                /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                                meltcallcount  /* melt_callcount debugging */
#else
                                                0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                                ;;

                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-macro.melt:5858:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[5];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L20*/ meltfnum[15];
                                                /*^apply.arg*/
                                                argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                                /*^apply.arg*/
                                                argtab[2].meltbp_long = 5858;
                                                /*^apply.arg*/
                                                argtab[3].meltbp_cstring =  "mexpand_cond res for :else";
                                                /*^apply.arg*/
                                                argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RES__V27*/ meltfptr[23];
                                                /*_.MELT_DEBUG_FUN__V46*/
                                                meltfptr[42] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                            /*_._IF___V45*/
                                            meltfptr[41] = /*_.MELT_DEBUG_FUN__V46*/ meltfptr[42];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-macro.melt:5858:/ clear");
                                            /*clear*/ /*_#MELT_CALLCOUNT__L20*/
                                            meltfnum[15] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.MELT_DEBUG_FUN__V46*/
                                            meltfptr[42] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_._IF___V45*/ meltfptr[41] =  /*reallynil*/ NULL ;;
                                    }
                                ;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:5858:/ locexp");
                                    /*void*/
                                    (void)0;
                                }
                                ;
                                /*^quasiblock*/


                                /*epilog*/

                                /*^clear*/
                                /*clear*/ /*_#MELT_NEED_DBG__L19*/
                                meltfnum[17] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IF___V45*/
                                meltfptr[41] = 0 ;
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

                            MELT_LOCATION("warmelt-macro.melt:5850:/ clear");
                            /*clear*/ /*_.SEXP_CONTENTS__V38*/
                            meltfptr[33] = 0 ;
                            /*^clear*/
                            /*clear*/ /*_.LIST_FIRST__V39*/
                            meltfptr[32] = 0 ;
                            /*^clear*/
                            /*clear*/ /*_.RESTPAIRS__V40*/
                            meltfptr[36] = 0 ;
                            /*^clear*/
                            /*clear*/ /*_._IF___V41*/
                            meltfptr[40] = 0 ;
                            /*epilog*/
                        }
                        ;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-macro.melt:5846:/ cond.else");

                        /*^block*/
                        /*anyblock*/
                        {


                            MELT_CHECK_SIGNAL();
                            ;
                            /*_#IS_A__L21*/
                            meltfnum[16] =
                                melt_is_instance_of((melt_ptr_t)(/*_.CURCOND__V32*/ meltfptr[31]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
                            MELT_LOCATION("warmelt-macro.melt:5860:/ cond");
                            /*cond*/
                            if (/*_#IS_A__L21*/ meltfnum[16]) /*then*/
                                {
                                    /*^cond.then*/
                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        MELT_LOCATION("warmelt-macro.melt:5861:/ quasiblock");


                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CURCOND__V32*/ meltfptr[31]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
                                            /*_.CURCONDCONT__V47*/
                                            meltfptr[43] = slot;
                                        };
                                        ;
                                        MELT_LOCATION("warmelt-macro.melt:5862:/ getslot");
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CURCOND__V32*/ meltfptr[31]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
                                            /*_.CURCONDLOC__V48*/
                                            meltfptr[42] = slot;
                                        };
                                        ;
                                        /*_.LIST_FIRST__V49*/
                                        meltfptr[41] =
                                            (melt_list_first((melt_ptr_t)(/*_.CURCONDCONT__V47*/ meltfptr[43])));;
                                        /*^compute*/
                                        /*_.CURCONDTEST__V50*/
                                        meltfptr[33] =
                                            (melt_pair_head((melt_ptr_t)(/*_.LIST_FIRST__V49*/ meltfptr[41])));;
                                        /*^compute*/
                                        /*_.LIST_FIRST__V51*/
                                        meltfptr[32] =
                                            (melt_list_first((melt_ptr_t)(/*_.CURCONDCONT__V47*/ meltfptr[43])));;
                                        /*^compute*/
                                        /*_.CURCONDRESTPAIRS__V52*/
                                        meltfptr[36] =
                                            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V51*/ meltfptr[32])));;

                                        MELT_CHECK_SIGNAL();
                                        ;
                                        /*^compute*/
                                        /*_#NULL__L22*/
                                        meltfnum[15] =
                                            ((/*_.CURCONDRESTPAIRS__V52*/ meltfptr[36]) == NULL);;
                                        MELT_LOCATION("warmelt-macro.melt:5866:/ cond");
                                        /*cond*/
                                        if (/*_#NULL__L22*/ meltfnum[15]) /*then*/
                                            {
                                                /*^cond.then*/
                                                /*^block*/
                                                /*anyblock*/
                                                {


                                                    MELT_CHECK_SIGNAL();
                                                    ;

                                                    MELT_CHECK_SIGNAL();
                                                    ;
                                                    MELT_LOCATION("warmelt-macro.melt:5872:/ apply");
                                                    /*apply*/
                                                    {
                                                        union meltparam_un argtab[3];
                                                        memset(&argtab, 0, sizeof(argtab));
                                                        /*^apply.arg*/
                                                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                                                        /*^apply.arg*/
                                                        argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
                                                        /*^apply.arg*/
                                                        argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                                                        /*_.MEXPANDER__V53*/
                                                        meltfptr[40] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURCONDTEST__V50*/ meltfptr[33]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                    }
                                                    ;
                                                    MELT_LOCATION("warmelt-macro.melt:5871:/ blockmultialloc");
                                                    /*multiallocblock*/
                                                    {
                                                        struct meltletrec_1_st
                                                        {
                                                            struct MELT_MULTIPLE_STRUCT(2) rtup_0__TUPLREC__x3;
                                                            long meltletrec_1_endgap;
                                                        } *meltletrec_1_ptr = 0;
                                                        meltletrec_1_ptr = (struct meltletrec_1_st *) meltgc_allocate (sizeof (struct meltletrec_1_st), 0);
                                                        /*^blockmultialloc.initfill*/
                                                        /*inimult rtup_0__TUPLREC__x3*/
                                                        /*_.TUPLREC___V55*/
                                                        meltfptr[54] = (melt_ptr_t) &meltletrec_1_ptr->rtup_0__TUPLREC__x3;
                                                        meltletrec_1_ptr->rtup_0__TUPLREC__x3.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_MULTIPLE))));
                                                        meltletrec_1_ptr->rtup_0__TUPLREC__x3.nbval = 2;


                                                        /*^putuple*/
                                                        /*putupl#3*/
                                                        melt_assertmsg("putupl [:5871] #3 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V55*/ meltfptr[54]))== MELTOBMAG_MULTIPLE);
                                                        melt_assertmsg("putupl [:5871] #3 checkoff", (0>=0 && 0< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V55*/ meltfptr[54]))));
                                                        ((meltmultiple_ptr_t)(/*_.TUPLREC___V55*/ meltfptr[54]))->tabval[0] = (melt_ptr_t)(/*_.MEXPANDER__V53*/ meltfptr[40]);
                                                        ;
                                                        /*^putuple*/
                                                        /*putupl#4*/
                                                        melt_assertmsg("putupl [:5871] #4 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V55*/ meltfptr[54]))== MELTOBMAG_MULTIPLE);
                                                        melt_assertmsg("putupl [:5871] #4 checkoff", (1>=0 && 1< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V55*/ meltfptr[54]))));
                                                        ((meltmultiple_ptr_t)(/*_.TUPLREC___V55*/ meltfptr[54]))->tabval[1] = (melt_ptr_t)(/*_.RES__V27*/ meltfptr[23]);
                                                        ;
                                                        /*^touch*/
                                                        meltgc_touch(/*_.TUPLREC___V55*/ meltfptr[54]);
                                                        ;
                                                        /*_.TUPLE___V54*/
                                                        meltfptr[53] = /*_.TUPLREC___V55*/ meltfptr[54];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-macro.melt:5871:/ clear");
                                                        /*clear*/ /*_.TUPLREC___V55*/
                                                        meltfptr[54] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.TUPLREC___V55*/
                                                        meltfptr[54] = 0 ;
                                                    } /*end multiallocblock*/
                                                    ;
                                                    MELT_LOCATION("warmelt-macro.melt:5868:/ quasiblock");


                                                    /*^rawallocobj*/
                                                    /*rawallocobj*/
                                                    {
                                                        melt_ptr_t newobj = 0;
                                                        melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_OR*/ meltfrout->tabval[10])), (3), "CLASS_SOURCE_OR");
                                                        /*_.INST__V57*/
                                                        meltfptr[56] =
                                                            newobj;
                                                    };
                                                    ;
                                                    /*^putslot*/
                                                    /*putslot*/
                                                    melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V57*/ meltfptr[56])) == MELTOBMAG_OBJECT);
                                                    melt_putfield_object((/*_.INST__V57*/ meltfptr[56]), (1), (/*_.CURCONDLOC__V48*/ meltfptr[42]), "LOCA_LOCATION");
                                                    ;
                                                    /*^putslot*/
                                                    /*putslot*/
                                                    melt_assertmsg("checkobj putslot  _ @SOR_DISJ", melt_magic_discr((melt_ptr_t)(/*_.INST__V57*/ meltfptr[56])) == MELTOBMAG_OBJECT);
                                                    melt_putfield_object((/*_.INST__V57*/ meltfptr[56]), (2), (/*_.TUPLE___V54*/ meltfptr[53]), "SOR_DISJ");
                                                    ;
                                                    /*^touchobj*/

                                                    melt_dbgtrace_written_object (/*_.INST__V57*/ meltfptr[56], "newly made instance");
                                                    ;
                                                    /*_.INST___V56*/
                                                    meltfptr[54] = /*_.INST__V57*/ meltfptr[56];;
                                                    MELT_LOCATION("warmelt-macro.melt:5867:/ compute");
                                                    /*_.RES__V27*/
                                                    meltfptr[23] = /*_.SETQ___V58*/ meltfptr[57] = /*_.INST___V56*/ meltfptr[54];;

#if MELT_HAVE_DEBUG
                                                    MELT_LOCATION("warmelt-macro.melt:5874:/ cppif.then");
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
                                                        /*_#MELT_NEED_DBG__L23*/
                                                        meltfnum[17] =
                                                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                                            ;;
                                                        MELT_LOCATION("warmelt-macro.melt:5874:/ cond");
                                                        /*cond*/
                                                        if (/*_#MELT_NEED_DBG__L23*/ meltfnum[17]) /*then*/
                                                            {
                                                                /*^cond.then*/
                                                                /*^block*/
                                                                /*anyblock*/
                                                                {

                                                                    /*_#MELT_CALLCOUNT__L24*/ meltfnum[23] =
                                                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                                                        meltcallcount  /* melt_callcount debugging */
#else
                                                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                                                        ;;

                                                                    MELT_CHECK_SIGNAL();
                                                                    ;
                                                                    MELT_LOCATION("warmelt-macro.melt:5874:/ apply");
                                                                    /*apply*/
                                                                    {
                                                                        union meltparam_un argtab[5];
                                                                        memset(&argtab, 0, sizeof(argtab));
                                                                        /*^apply.arg*/
                                                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L24*/ meltfnum[23];
                                                                        /*^apply.arg*/
                                                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                                                        /*^apply.arg*/
                                                                        argtab[2].meltbp_long = 5874;
                                                                        /*^apply.arg*/
                                                                        argtab[3].meltbp_cstring =  "mexpand_cond res for monoexp cond";
                                                                        /*^apply.arg*/
                                                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RES__V27*/ meltfptr[23];
                                                                        /*_.MELT_DEBUG_FUN__V60*/
                                                                        meltfptr[59] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                    }
                                                                    ;
                                                                    /*_._IF___V59*/
                                                                    meltfptr[58] = /*_.MELT_DEBUG_FUN__V60*/ meltfptr[59];;
                                                                    /*epilog*/

                                                                    MELT_LOCATION("warmelt-macro.melt:5874:/ clear");
                                                                    /*clear*/ /*_#MELT_CALLCOUNT__L24*/
                                                                    meltfnum[23] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_.MELT_DEBUG_FUN__V60*/
                                                                    meltfptr[59] = 0 ;
                                                                }
                                                                ;
                                                            }
                                                        else    /*^cond.else*/
                                                            {

                                                                /*_._IF___V59*/ meltfptr[58] =  /*reallynil*/ NULL ;;
                                                            }
                                                        ;

                                                        {
                                                            MELT_LOCATION("warmelt-macro.melt:5874:/ locexp");
                                                            /*void*/
                                                            (void)0;
                                                        }
                                                        ;
                                                        /*^quasiblock*/


                                                        /*epilog*/

                                                        /*^clear*/
                                                        /*clear*/ /*_#MELT_NEED_DBG__L23*/
                                                        meltfnum[17] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_._IF___V59*/
                                                        meltfptr[58] = 0 ;
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
                                                    MELT_LOCATION("warmelt-macro.melt:5866:/ quasiblock");


                                                    /*epilog*/

                                                    /*^clear*/
                                                    /*clear*/ /*_.MEXPANDER__V53*/
                                                    meltfptr[40] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_.TUPLE___V54*/
                                                    meltfptr[53] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_.INST___V56*/
                                                    meltfptr[54] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_.SETQ___V58*/
                                                    meltfptr[57] = 0 ;
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
                                                    /*_.PAIR_TAIL__V61*/
                                                    meltfptr[59] =
                                                        (melt_pair_tail((melt_ptr_t)(/*_.CURCONDRESTPAIRS__V52*/ meltfptr[36])));;
                                                    /*^compute*/
                                                    /*_#NULL__L25*/
                                                    meltfnum[23] =
                                                        ((/*_.PAIR_TAIL__V61*/ meltfptr[59]) == NULL);;
                                                    MELT_LOCATION("warmelt-macro.melt:5876:/ cond");
                                                    /*cond*/
                                                    if (/*_#NULL__L25*/ meltfnum[23]) /*then*/
                                                        {
                                                            /*^cond.then*/
                                                            /*^block*/
                                                            /*anyblock*/
                                                            {


                                                                MELT_CHECK_SIGNAL();
                                                                ;

                                                                MELT_CHECK_SIGNAL();
                                                                ;
                                                                MELT_LOCATION("warmelt-macro.melt:5880:/ apply");
                                                                /*apply*/
                                                                {
                                                                    union meltparam_un argtab[3];
                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                    /*^apply.arg*/
                                                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                                                                    /*^apply.arg*/
                                                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
                                                                    /*^apply.arg*/
                                                                    argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                                                                    /*_.MEXPANDER__V62*/
                                                                    meltfptr[58] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURCONDTEST__V50*/ meltfptr[33]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                }
                                                                ;
                                                                /*_.PAIR_HEAD__V63*/
                                                                meltfptr[40] =
                                                                    (melt_pair_head((melt_ptr_t)(/*_.CURCONDRESTPAIRS__V52*/ meltfptr[36])));;

                                                                MELT_CHECK_SIGNAL();
                                                                ;
                                                                MELT_LOCATION("warmelt-macro.melt:5881:/ apply");
                                                                /*apply*/
                                                                {
                                                                    union meltparam_un argtab[3];
                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                    /*^apply.arg*/
                                                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                                                                    /*^apply.arg*/
                                                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
                                                                    /*^apply.arg*/
                                                                    argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                                                                    /*_.MEXPANDER__V64*/
                                                                    meltfptr[53] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.PAIR_HEAD__V63*/ meltfptr[40]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                }
                                                                ;
                                                                MELT_LOCATION("warmelt-macro.melt:5878:/ quasiblock");


                                                                /*^rawallocobj*/
                                                                /*rawallocobj*/
                                                                {
                                                                    melt_ptr_t newobj = 0;
                                                                    melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_IFELSE*/ meltfrout->tabval[11])), (5), "CLASS_SOURCE_IFELSE");
                                                                    /*_.INST__V66*/
                                                                    meltfptr[57] =
                                                                        newobj;
                                                                };
                                                                ;
                                                                /*^putslot*/
                                                                /*putslot*/
                                                                melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V66*/ meltfptr[57])) == MELTOBMAG_OBJECT);
                                                                melt_putfield_object((/*_.INST__V66*/ meltfptr[57]), (1), (/*_.CURCONDLOC__V48*/ meltfptr[42]), "LOCA_LOCATION");
                                                                ;
                                                                /*^putslot*/
                                                                /*putslot*/
                                                                melt_assertmsg("checkobj putslot  _ @SIF_TEST", melt_magic_discr((melt_ptr_t)(/*_.INST__V66*/ meltfptr[57])) == MELTOBMAG_OBJECT);
                                                                melt_putfield_object((/*_.INST__V66*/ meltfptr[57]), (2), (/*_.MEXPANDER__V62*/ meltfptr[58]), "SIF_TEST");
                                                                ;
                                                                /*^putslot*/
                                                                /*putslot*/
                                                                melt_assertmsg("checkobj putslot  _ @SIF_THEN", melt_magic_discr((melt_ptr_t)(/*_.INST__V66*/ meltfptr[57])) == MELTOBMAG_OBJECT);
                                                                melt_putfield_object((/*_.INST__V66*/ meltfptr[57]), (3), (/*_.MEXPANDER__V64*/ meltfptr[53]), "SIF_THEN");
                                                                ;
                                                                /*^putslot*/
                                                                /*putslot*/
                                                                melt_assertmsg("checkobj putslot  _ @SIF_ELSE", melt_magic_discr((melt_ptr_t)(/*_.INST__V66*/ meltfptr[57])) == MELTOBMAG_OBJECT);
                                                                melt_putfield_object((/*_.INST__V66*/ meltfptr[57]), (4), (/*_.RES__V27*/ meltfptr[23]), "SIF_ELSE");
                                                                ;
                                                                /*^touchobj*/

                                                                melt_dbgtrace_written_object (/*_.INST__V66*/ meltfptr[57], "newly made instance");
                                                                ;
                                                                /*_.INST___V65*/
                                                                meltfptr[54] = /*_.INST__V66*/ meltfptr[57];;
                                                                MELT_LOCATION("warmelt-macro.melt:5877:/ compute");
                                                                /*_.RES__V27*/
                                                                meltfptr[23] = /*_.SETQ___V67*/ meltfptr[66] = /*_.INST___V65*/ meltfptr[54];;

#if MELT_HAVE_DEBUG
                                                                MELT_LOCATION("warmelt-macro.melt:5884:/ cppif.then");
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
                                                                    /*_#MELT_NEED_DBG__L26*/
                                                                    meltfnum[17] =
                                                                        /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                                                        (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                                                        0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                                                        ;;
                                                                    MELT_LOCATION("warmelt-macro.melt:5884:/ cond");
                                                                    /*cond*/
                                                                    if (/*_#MELT_NEED_DBG__L26*/ meltfnum[17]) /*then*/
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
                                                                                MELT_LOCATION("warmelt-macro.melt:5884:/ apply");
                                                                                /*apply*/
                                                                                {
                                                                                    union meltparam_un argtab[5];
                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                    /*^apply.arg*/
                                                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L27*/ meltfnum[26];
                                                                                    /*^apply.arg*/
                                                                                    argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                                                                    /*^apply.arg*/
                                                                                    argtab[2].meltbp_long = 5884;
                                                                                    /*^apply.arg*/
                                                                                    argtab[3].meltbp_cstring =  "mexpand_cond res for biexp cond";
                                                                                    /*^apply.arg*/
                                                                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RES__V27*/ meltfptr[23];
                                                                                    /*_.MELT_DEBUG_FUN__V69*/
                                                                                    meltfptr[68] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                }
                                                                                ;
                                                                                /*_._IF___V68*/
                                                                                meltfptr[67] = /*_.MELT_DEBUG_FUN__V69*/ meltfptr[68];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-macro.melt:5884:/ clear");
                                                                                /*clear*/ /*_#MELT_CALLCOUNT__L27*/
                                                                                meltfnum[26] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.MELT_DEBUG_FUN__V69*/
                                                                                meltfptr[68] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    else    /*^cond.else*/
                                                                        {

                                                                            /*_._IF___V68*/ meltfptr[67] =  /*reallynil*/ NULL ;;
                                                                        }
                                                                    ;

                                                                    {
                                                                        MELT_LOCATION("warmelt-macro.melt:5884:/ locexp");
                                                                        /*void*/
                                                                        (void)0;
                                                                    }
                                                                    ;
                                                                    /*^quasiblock*/


                                                                    /*epilog*/

                                                                    /*^clear*/
                                                                    /*clear*/ /*_#MELT_NEED_DBG__L26*/
                                                                    meltfnum[17] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_._IF___V68*/
                                                                    meltfptr[67] = 0 ;
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
                                                                MELT_LOCATION("warmelt-macro.melt:5876:/ quasiblock");


                                                                /*epilog*/

                                                                /*^clear*/
                                                                /*clear*/ /*_.MEXPANDER__V62*/
                                                                meltfptr[58] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.PAIR_HEAD__V63*/
                                                                meltfptr[40] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.MEXPANDER__V64*/
                                                                meltfptr[53] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.INST___V65*/
                                                                meltfptr[54] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.SETQ___V67*/
                                                                meltfptr[66] = 0 ;
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
                                                                MELT_LOCATION("warmelt-macro.melt:5890:/ apply");
                                                                /*apply*/
                                                                {
                                                                    union meltparam_un argtab[3];
                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                    /*^apply.arg*/
                                                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                                                                    /*^apply.arg*/
                                                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
                                                                    /*^apply.arg*/
                                                                    argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                                                                    /*_.MEXPANDER__V70*/
                                                                    meltfptr[68] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURCONDTEST__V50*/ meltfptr[33]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                }
                                                                ;

                                                                MELT_CHECK_SIGNAL();
                                                                ;
                                                                MELT_LOCATION("warmelt-macro.melt:5891:/ apply");
                                                                /*apply*/
                                                                {
                                                                    union meltparam_un argtab[4];
                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                    /*^apply.arg*/
                                                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CURCONDLOC__V48*/ meltfptr[42];
                                                                    /*^apply.arg*/
                                                                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                                                                    /*^apply.arg*/
                                                                    argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
                                                                    /*^apply.arg*/
                                                                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                                                                    /*_.PAIRLIST_TO_PROGN__V71*/
                                                                    meltfptr[67] =  melt_apply ((meltclosure_ptr_t)((/*!PAIRLIST_TO_PROGN*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.CURCONDRESTPAIRS__V52*/ meltfptr[36]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                }
                                                                ;
                                                                MELT_LOCATION("warmelt-macro.melt:5888:/ quasiblock");


                                                                /*^rawallocobj*/
                                                                /*rawallocobj*/
                                                                {
                                                                    melt_ptr_t newobj = 0;
                                                                    melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_IFELSE*/ meltfrout->tabval[11])), (5), "CLASS_SOURCE_IFELSE");
                                                                    /*_.INST__V73*/
                                                                    meltfptr[40] =
                                                                        newobj;
                                                                };
                                                                ;
                                                                /*^putslot*/
                                                                /*putslot*/
                                                                melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V73*/ meltfptr[40])) == MELTOBMAG_OBJECT);
                                                                melt_putfield_object((/*_.INST__V73*/ meltfptr[40]), (1), (/*_.CURCONDLOC__V48*/ meltfptr[42]), "LOCA_LOCATION");
                                                                ;
                                                                /*^putslot*/
                                                                /*putslot*/
                                                                melt_assertmsg("checkobj putslot  _ @SIF_TEST", melt_magic_discr((melt_ptr_t)(/*_.INST__V73*/ meltfptr[40])) == MELTOBMAG_OBJECT);
                                                                melt_putfield_object((/*_.INST__V73*/ meltfptr[40]), (2), (/*_.MEXPANDER__V70*/ meltfptr[68]), "SIF_TEST");
                                                                ;
                                                                /*^putslot*/
                                                                /*putslot*/
                                                                melt_assertmsg("checkobj putslot  _ @SIF_THEN", melt_magic_discr((melt_ptr_t)(/*_.INST__V73*/ meltfptr[40])) == MELTOBMAG_OBJECT);
                                                                melt_putfield_object((/*_.INST__V73*/ meltfptr[40]), (3), (/*_.PAIRLIST_TO_PROGN__V71*/ meltfptr[67]), "SIF_THEN");
                                                                ;
                                                                /*^putslot*/
                                                                /*putslot*/
                                                                melt_assertmsg("checkobj putslot  _ @SIF_ELSE", melt_magic_discr((melt_ptr_t)(/*_.INST__V73*/ meltfptr[40])) == MELTOBMAG_OBJECT);
                                                                melt_putfield_object((/*_.INST__V73*/ meltfptr[40]), (4), (/*_.RES__V27*/ meltfptr[23]), "SIF_ELSE");
                                                                ;
                                                                /*^touchobj*/

                                                                melt_dbgtrace_written_object (/*_.INST__V73*/ meltfptr[40], "newly made instance");
                                                                ;
                                                                /*_.INST___V72*/
                                                                meltfptr[58] = /*_.INST__V73*/ meltfptr[40];;
                                                                MELT_LOCATION("warmelt-macro.melt:5887:/ compute");
                                                                /*_.RES__V27*/
                                                                meltfptr[23] = /*_.SETQ___V74*/ meltfptr[53] = /*_.INST___V72*/ meltfptr[58];;

#if MELT_HAVE_DEBUG
                                                                MELT_LOCATION("warmelt-macro.melt:5893:/ cppif.then");
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
                                                                    /*_#MELT_NEED_DBG__L28*/
                                                                    meltfnum[26] =
                                                                        /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                                                        (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                                                        0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                                                        ;;
                                                                    MELT_LOCATION("warmelt-macro.melt:5893:/ cond");
                                                                    /*cond*/
                                                                    if (/*_#MELT_NEED_DBG__L28*/ meltfnum[26]) /*then*/
                                                                        {
                                                                            /*^cond.then*/
                                                                            /*^block*/
                                                                            /*anyblock*/
                                                                            {

                                                                                /*_#MELT_CALLCOUNT__L29*/ meltfnum[17] =
                                                                                    /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                                                                    meltcallcount  /* melt_callcount debugging */
#else
                                                                                    0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                                                                    ;;

                                                                                MELT_CHECK_SIGNAL();
                                                                                ;
                                                                                MELT_LOCATION("warmelt-macro.melt:5893:/ apply");
                                                                                /*apply*/
                                                                                {
                                                                                    union meltparam_un argtab[5];
                                                                                    memset(&argtab, 0, sizeof(argtab));
                                                                                    /*^apply.arg*/
                                                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L29*/ meltfnum[17];
                                                                                    /*^apply.arg*/
                                                                                    argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                                                                    /*^apply.arg*/
                                                                                    argtab[2].meltbp_long = 5893;
                                                                                    /*^apply.arg*/
                                                                                    argtab[3].meltbp_cstring =  "mexpand_cond res for manyexp cond";
                                                                                    /*^apply.arg*/
                                                                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RES__V27*/ meltfptr[23];
                                                                                    /*_.MELT_DEBUG_FUN__V76*/
                                                                                    meltfptr[66] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                                                }
                                                                                ;
                                                                                /*_._IF___V75*/
                                                                                meltfptr[54] = /*_.MELT_DEBUG_FUN__V76*/ meltfptr[66];;
                                                                                /*epilog*/

                                                                                MELT_LOCATION("warmelt-macro.melt:5893:/ clear");
                                                                                /*clear*/ /*_#MELT_CALLCOUNT__L29*/
                                                                                meltfnum[17] = 0 ;
                                                                                /*^clear*/
                                                                                /*clear*/ /*_.MELT_DEBUG_FUN__V76*/
                                                                                meltfptr[66] = 0 ;
                                                                            }
                                                                            ;
                                                                        }
                                                                    else    /*^cond.else*/
                                                                        {

                                                                            /*_._IF___V75*/ meltfptr[54] =  /*reallynil*/ NULL ;;
                                                                        }
                                                                    ;

                                                                    {
                                                                        MELT_LOCATION("warmelt-macro.melt:5893:/ locexp");
                                                                        /*void*/
                                                                        (void)0;
                                                                    }
                                                                    ;
                                                                    /*^quasiblock*/


                                                                    /*epilog*/

                                                                    /*^clear*/
                                                                    /*clear*/ /*_#MELT_NEED_DBG__L28*/
                                                                    meltfnum[26] = 0 ;
                                                                    /*^clear*/
                                                                    /*clear*/ /*_._IF___V75*/
                                                                    meltfptr[54] = 0 ;
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
                                                                MELT_LOCATION("warmelt-macro.melt:5886:/ quasiblock");


                                                                /*epilog*/

                                                                MELT_LOCATION("warmelt-macro.melt:5876:/ clear");
                                                                /*clear*/ /*_.MEXPANDER__V70*/
                                                                meltfptr[68] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.PAIRLIST_TO_PROGN__V71*/
                                                                meltfptr[67] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.INST___V72*/
                                                                meltfptr[58] = 0 ;
                                                                /*^clear*/
                                                                /*clear*/ /*_.SETQ___V74*/
                                                                meltfptr[53] = 0 ;
                                                            }
                                                            ;
                                                        }
                                                    ;
                                                    /*epilog*/

                                                    MELT_LOCATION("warmelt-macro.melt:5866:/ clear");
                                                    /*clear*/ /*_.PAIR_TAIL__V61*/
                                                    meltfptr[59] = 0 ;
                                                    /*^clear*/
                                                    /*clear*/ /*_#NULL__L25*/
                                                    meltfnum[23] = 0 ;
                                                }
                                                ;
                                            }
                                        ;

                                        MELT_LOCATION("warmelt-macro.melt:5861:/ clear");
                                        /*clear*/ /*_.CURCONDCONT__V47*/
                                        meltfptr[43] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.CURCONDLOC__V48*/
                                        meltfptr[42] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.LIST_FIRST__V49*/
                                        meltfptr[41] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.CURCONDTEST__V50*/
                                        meltfptr[33] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.LIST_FIRST__V51*/
                                        meltfptr[32] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.CURCONDRESTPAIRS__V52*/
                                        meltfptr[36] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_#NULL__L22*/
                                        meltfnum[15] = 0 ;
                                        /*epilog*/
                                    }
                                    ;
                                }
                            else
                                {
                                    MELT_LOCATION("warmelt-macro.melt:5860:/ cond.else");

                                    /*^block*/
                                    /*anyblock*/
                                    {


                                        MELT_CHECK_SIGNAL();
                                        ;
                                        MELT_LOCATION("warmelt-macro.melt:5899:/ blockmultialloc");
                                        /*multiallocblock*/
                                        {
                                            struct meltletrec_2_st
                                            {
                                                struct MELT_MULTIPLE_STRUCT(2) rtup_0__TUPLREC__x4;
                                                long meltletrec_2_endgap;
                                            } *meltletrec_2_ptr = 0;
                                            meltletrec_2_ptr = (struct meltletrec_2_st *) meltgc_allocate (sizeof (struct meltletrec_2_st), 0);
                                            /*^blockmultialloc.initfill*/
                                            /*inimult rtup_0__TUPLREC__x4*/
                                            /*_.TUPLREC___V78*/
                                            meltfptr[54] = (melt_ptr_t) &meltletrec_2_ptr->rtup_0__TUPLREC__x4;
                                            meltletrec_2_ptr->rtup_0__TUPLREC__x4.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_MULTIPLE))));
                                            meltletrec_2_ptr->rtup_0__TUPLREC__x4.nbval = 2;


                                            /*^putuple*/
                                            /*putupl#5*/
                                            melt_assertmsg("putupl [:5899] #5 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V78*/ meltfptr[54]))== MELTOBMAG_MULTIPLE);
                                            melt_assertmsg("putupl [:5899] #5 checkoff", (0>=0 && 0< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V78*/ meltfptr[54]))));
                                            ((meltmultiple_ptr_t)(/*_.TUPLREC___V78*/ meltfptr[54]))->tabval[0] = (melt_ptr_t)(/*_.CURCOND__V32*/ meltfptr[31]);
                                            ;
                                            /*^putuple*/
                                            /*putupl#6*/
                                            melt_assertmsg("putupl [:5899] #6 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V78*/ meltfptr[54]))== MELTOBMAG_MULTIPLE);
                                            melt_assertmsg("putupl [:5899] #6 checkoff", (1>=0 && 1< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V78*/ meltfptr[54]))));
                                            ((meltmultiple_ptr_t)(/*_.TUPLREC___V78*/ meltfptr[54]))->tabval[1] = (melt_ptr_t)(/*_.RES__V27*/ meltfptr[23]);
                                            ;
                                            /*^touch*/
                                            meltgc_touch(/*_.TUPLREC___V78*/ meltfptr[54]);
                                            ;
                                            /*_.TUPLE___V77*/
                                            meltfptr[66] = /*_.TUPLREC___V78*/ meltfptr[54];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-macro.melt:5899:/ clear");
                                            /*clear*/ /*_.TUPLREC___V78*/
                                            meltfptr[54] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.TUPLREC___V78*/
                                            meltfptr[54] = 0 ;
                                        } /*end multiallocblock*/
                                        ;
                                        MELT_LOCATION("warmelt-macro.melt:5897:/ quasiblock");


                                        /*^rawallocobj*/
                                        /*rawallocobj*/
                                        {
                                            melt_ptr_t newobj = 0;
                                            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_OR*/ meltfrout->tabval[10])), (3), "CLASS_SOURCE_OR");
                                            /*_.INST__V80*/
                                            meltfptr[67] =
                                                newobj;
                                        };
                                        ;
                                        /*^putslot*/
                                        /*putslot*/
                                        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V80*/ meltfptr[67])) == MELTOBMAG_OBJECT);
                                        melt_putfield_object((/*_.INST__V80*/ meltfptr[67]), (1), (/*_.LOC__V16*/ meltfptr[15]), "LOCA_LOCATION");
                                        ;
                                        /*^putslot*/
                                        /*putslot*/
                                        melt_assertmsg("checkobj putslot  _ @SOR_DISJ", melt_magic_discr((melt_ptr_t)(/*_.INST__V80*/ meltfptr[67])) == MELTOBMAG_OBJECT);
                                        melt_putfield_object((/*_.INST__V80*/ meltfptr[67]), (2), (/*_.TUPLE___V77*/ meltfptr[66]), "SOR_DISJ");
                                        ;
                                        /*^touchobj*/

                                        melt_dbgtrace_written_object (/*_.INST__V80*/ meltfptr[67], "newly made instance");
                                        ;
                                        /*_.INST___V79*/
                                        meltfptr[68] = /*_.INST__V80*/ meltfptr[67];;
                                        MELT_LOCATION("warmelt-macro.melt:5897:/ compute");
                                        /*_.RES__V27*/
                                        meltfptr[23] = /*_.SETQ___V81*/ meltfptr[58] = /*_.INST___V79*/ meltfptr[68];;

#if MELT_HAVE_DEBUG
                                        MELT_LOCATION("warmelt-macro.melt:5900:/ cppif.then");
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
                                            /*_#MELT_NEED_DBG__L30*/
                                            meltfnum[17] =
                                                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                                ;;
                                            MELT_LOCATION("warmelt-macro.melt:5900:/ cond");
                                            /*cond*/
                                            if (/*_#MELT_NEED_DBG__L30*/ meltfnum[17]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        /*_#MELT_CALLCOUNT__L31*/ meltfnum[26] =
                                                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                                            meltcallcount  /* melt_callcount debugging */
#else
                                                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                                            ;;

                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-macro.melt:5900:/ apply");
                                                        /*apply*/
                                                        {
                                                            union meltparam_un argtab[5];
                                                            memset(&argtab, 0, sizeof(argtab));
                                                            /*^apply.arg*/
                                                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L31*/ meltfnum[26];
                                                            /*^apply.arg*/
                                                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                                            /*^apply.arg*/
                                                            argtab[2].meltbp_long = 5900;
                                                            /*^apply.arg*/
                                                            argtab[3].meltbp_cstring =  "mexpand_cond res for nonsexp cond";
                                                            /*^apply.arg*/
                                                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RES__V27*/ meltfptr[23];
                                                            /*_.MELT_DEBUG_FUN__V83*/
                                                            meltfptr[59] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                        }
                                                        ;
                                                        /*_._IF___V82*/
                                                        meltfptr[53] = /*_.MELT_DEBUG_FUN__V83*/ meltfptr[59];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-macro.melt:5900:/ clear");
                                                        /*clear*/ /*_#MELT_CALLCOUNT__L31*/
                                                        meltfnum[26] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.MELT_DEBUG_FUN__V83*/
                                                        meltfptr[59] = 0 ;
                                                    }
                                                    ;
                                                }
                                            else    /*^cond.else*/
                                                {

                                                    /*_._IF___V82*/ meltfptr[53] =  /*reallynil*/ NULL ;;
                                                }
                                            ;

                                            {
                                                MELT_LOCATION("warmelt-macro.melt:5900:/ locexp");
                                                /*void*/
                                                (void)0;
                                            }
                                            ;
                                            /*^quasiblock*/


                                            /*epilog*/

                                            /*^clear*/
                                            /*clear*/ /*_#MELT_NEED_DBG__L30*/
                                            meltfnum[17] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_._IF___V82*/
                                            meltfptr[53] = 0 ;
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
                                        MELT_LOCATION("warmelt-macro.melt:5896:/ quasiblock");


                                        /*epilog*/

                                        MELT_LOCATION("warmelt-macro.melt:5860:/ clear");
                                        /*clear*/ /*_.TUPLE___V77*/
                                        meltfptr[66] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.INST___V79*/
                                        meltfptr[68] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.SETQ___V81*/
                                        meltfptr[58] = 0 ;
                                    }
                                    ;
                                }
                            ;
                            /*epilog*/

                            MELT_LOCATION("warmelt-macro.melt:5846:/ clear");
                            /*clear*/ /*_#IS_A__L21*/
                            meltfnum[16] = 0 ;
                        }
                        ;
                    }
                ;

                MELT_LOCATION("warmelt-macro.melt:5843:/ clear");
                /*clear*/ /*_.CURCOND__V32*/
                meltfptr[31] = 0 ;
                /*^clear*/
                /*clear*/ /*_#NULL__L14*/
                meltfnum[12] = 0 ;
                /*^clear*/
                /*clear*/ /*_#_IF___L15*/
                meltfnum[0] = 0 ;
                /*_#miI__L32*/
                meltfnum[23] =
                    ((/*_#IX__L8*/ meltfnum[6]) - (1));;
                MELT_LOCATION("warmelt-macro.melt:5904:/ compute");
                /*_#IX__L8*/
                meltfnum[6] = /*_#SETQ___L33*/ meltfnum[15] = /*_#miI__L32*/ meltfnum[23];;

                MELT_CHECK_SIGNAL();
                ;
                /*epilog*/

                MELT_LOCATION("warmelt-macro.melt:5841:/ clear");
                /*clear*/ /*_#ltI__L11*/
                meltfnum[9] = 0 ;
                /*^clear*/
                /*clear*/ /*_#miI__L32*/
                meltfnum[23] = 0 ;
                /*^clear*/
                /*clear*/ /*_#SETQ___L33*/
                meltfnum[15] = 0 ;
            }
            ;
            ;
            goto meltlabloop_CONDLOOP_1;
meltlabexit_CONDLOOP_1:;/*^loopepilog*/
            /*loopepilog*/
            /*_.FOREVER___V30*/
            meltfptr[28] = /*_.CONDLOOP__V31*/ meltfptr[22];;
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5906:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L34*/
            meltfnum[26] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5906:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L34*/ meltfnum[26]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L35*/ meltfnum[17] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:5906:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L35*/ meltfnum[17];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5906;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_cond final res";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RES__V27*/ meltfptr[23];
                            /*_.MELT_DEBUG_FUN__V85*/
                            meltfptr[42] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V84*/
                        meltfptr[43] = /*_.MELT_DEBUG_FUN__V85*/ meltfptr[42];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5906:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L35*/
                        meltfnum[17] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V85*/
                        meltfptr[42] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V84*/ meltfptr[43] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5906:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L34*/
            meltfnum[26] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V84*/
            meltfptr[43] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5907:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.RES__V27*/ meltfptr[23];;

        {
            MELT_LOCATION("warmelt-macro.melt:5907:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*_.LET___V14*/
        meltfptr[6] = /*_.RETURN___V86*/ meltfptr[41];;

        MELT_LOCATION("warmelt-macro.melt:5822:/ clear");
        /*clear*/ /*_.CONT__V15*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V16*/
        meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V17*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LAMBDA___V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CEXPTUPLE__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NBCOND__L5*/
        meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LASTCEXP__V26*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IX__L8*/
        meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RES__V27*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V30*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V86*/
        meltfptr[41] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5818:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V14*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-macro.melt:5818:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V6*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V9*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V14*/
        meltfptr[6] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_COND", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_95_WARMELTmiMACRO_MEXPAND_COND_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_95_WARMELTmiMACRO_MEXPAND_COND*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_96_WARMELTmiMACRO_LAMBDA_cl15(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_96_WARMELTmiMACRO_LAMBDA_cl15_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_96_WARMELTmiMACRO_LAMBDA_cl15 fromline 1715 */

    /** start of frame for meltrout_96_WARMELTmiMACRO_LAMBDA_cl15 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_96_WARMELTmiMACRO_LAMBDA_cl15// fromline 1531
        : public Melt_CallFrameWithValues<2>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_96_WARMELTmiMACRO_LAMBDA_cl15(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_96_WARMELTmiMACRO_LAMBDA_cl15), clos) {};
        MeltFrame_meltrout_96_WARMELTmiMACRO_LAMBDA_cl15() //the constructor fromline 1605
            : Melt_CallFrameWithValues<2> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_96_WARMELTmiMACRO_LAMBDA_cl15)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_96_WARMELTmiMACRO_LAMBDA_cl15(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_96_WARMELTmiMACRO_LAMBDA_cl15)) {};
        MeltFrame_meltrout_96_WARMELTmiMACRO_LAMBDA_cl15(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<2> (fil,lin, sizeof(MeltFrame_meltrout_96_WARMELTmiMACRO_LAMBDA_cl15), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_96_WARMELTmiMACRO_LAMBDA_cl15


    /** end of frame for meltrout_96_WARMELTmiMACRO_LAMBDA_cl15 fromline 1660**/

    /* end of frame for routine meltrout_96_WARMELTmiMACRO_LAMBDA_cl15 fromline 1719 */

    /* classy proc frame meltrout_96_WARMELTmiMACRO_LAMBDA_cl15 */
    MeltFrame_meltrout_96_WARMELTmiMACRO_LAMBDA_cl15
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_96_WARMELTmiMACRO_LAMBDA_cl15 fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl15", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5828:/ getarg");
    /*_.C__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
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
        /*_#IS_NOT_A__L1*/
        meltfnum[0] =
            !melt_is_instance_of((melt_ptr_t)(/*_.C__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
        MELT_LOCATION("warmelt-macro.melt:5829:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L1*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5830:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)((/*~LOC*/ meltfclos->tabval[0])), ( "COND with non-sexpr"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5828:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.C__V2*/ meltfptr[1];;

        {
            MELT_LOCATION("warmelt-macro.melt:5828:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L1*/
        meltfnum[0] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl15", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_96_WARMELTmiMACRO_LAMBDA_cl15_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_96_WARMELTmiMACRO_LAMBDA_cl15*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_97_WARMELTmiMACRO_MEXPAND_AND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_97_WARMELTmiMACRO_MEXPAND_AND_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_97_WARMELTmiMACRO_MEXPAND_AND fromline 1715 */

    /** start of frame for meltrout_97_WARMELTmiMACRO_MEXPAND_AND of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_97_WARMELTmiMACRO_MEXPAND_AND// fromline 1531
        : public Melt_CallFrameWithValues<43>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_97_WARMELTmiMACRO_MEXPAND_AND(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<43> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_97_WARMELTmiMACRO_MEXPAND_AND), clos) {};
        MeltFrame_meltrout_97_WARMELTmiMACRO_MEXPAND_AND() //the constructor fromline 1605
            : Melt_CallFrameWithValues<43> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_97_WARMELTmiMACRO_MEXPAND_AND)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_97_WARMELTmiMACRO_MEXPAND_AND(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<43> (fil,lin, sizeof(MeltFrame_meltrout_97_WARMELTmiMACRO_MEXPAND_AND)) {};
        MeltFrame_meltrout_97_WARMELTmiMACRO_MEXPAND_AND(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<43> (fil,lin, sizeof(MeltFrame_meltrout_97_WARMELTmiMACRO_MEXPAND_AND), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_97_WARMELTmiMACRO_MEXPAND_AND


    /** end of frame for meltrout_97_WARMELTmiMACRO_MEXPAND_AND fromline 1660**/

    /* end of frame for routine meltrout_97_WARMELTmiMACRO_MEXPAND_AND fromline 1719 */

    /* classy proc frame meltrout_97_WARMELTmiMACRO_MEXPAND_AND */
    MeltFrame_meltrout_97_WARMELTmiMACRO_MEXPAND_AND
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_97_WARMELTmiMACRO_MEXPAND_AND fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_AND", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5923:/ getarg");
    /*_.SEXPR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V5*/
    meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5924:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5924:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L1*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5924:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5924;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_and sexpr:";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5924:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/
                        meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V7*/
                        meltfptr[6] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5924:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V6*/
            meltfptr[5] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5925:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:5925:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5925:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5925;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5925:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5925:/ clear");
            /*clear*/ /*_#IS_A__L3*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5926:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:5926:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5926:/ cond.else");

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
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5926;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5926:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5926:/ clear");
            /*clear*/ /*_#IS_A__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V12*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V11*/ meltfptr[9] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L5*/
        meltfnum[1] =
            ((/*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:5927:/ cond");
        /*cond*/
        if (/*_#NULL__L5*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V15*/ meltfptr[5] = (/*!MACROEXPAND_1*/ meltfrout->tabval[4]);;
                    /*_._IF___V14*/
                    meltfptr[12] = /*_.SETQ___V15*/ meltfptr[5];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5927:/ clear");
                    /*clear*/ /*_.SETQ___V15*/
                    meltfptr[5] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V14*/ meltfptr[12] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5928:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L6*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:5928:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V17*/ meltfptr[16] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5928:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5928;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                            /*_.MELT_ASSERT_FAILURE_FUN__V18*/
                            meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V17*/
                        meltfptr[16] = /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5928:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V18*/
                        meltfptr[17] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V16*/
            meltfptr[5] = /*_._IFELSE___V17*/ meltfptr[16];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5928:/ clear");
            /*clear*/ /*_#IS_OBJECT__L6*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V17*/
            meltfptr[16] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V16*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5929:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:5930:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V20*/
            meltfptr[16] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5931:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V21*/
            meltfptr[20] = slot;
        };
        ;
        /*_.LIST_FIRST__V22*/
        meltfptr[21] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V20*/ meltfptr[16])));;
        /*^compute*/
        /*_.CURPAIR__V23*/
        meltfptr[22] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V22*/ meltfptr[21])));;
        MELT_LOCATION("warmelt-macro.melt:5936:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V25*/
        meltfptr[24] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_7*/ meltfrout->tabval[7])), (3));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V25*/ meltfptr[24])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V25*/ meltfptr[24])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V25*/ meltfptr[24])->tabval[0] = (melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V25*/ meltfptr[24])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 1>= 0 && 1< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V25*/ meltfptr[24])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V25*/ meltfptr[24])->tabval[1] = (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V25*/ meltfptr[24])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 2>= 0 && 2< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V25*/ meltfptr[24])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V25*/ meltfptr[24])->tabval[2] = (melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4]);
        ;
        /*_.LAMBDA___V24*/
        meltfptr[23] = /*_.LAMBDA___V25*/ meltfptr[24];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5933:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[6]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.LAMBDA___V24*/ meltfptr[23];
            /*_.CXTUP__V26*/
            meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!PAIRLIST_TO_MULTIPLE*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.CURPAIR__V23*/ meltfptr[22]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_#NBCOMP__L7*/
        meltfnum[0] =
            (melt_multiple_length((melt_ptr_t)(/*_.CXTUP__V26*/ meltfptr[25])));;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5941:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L8*/
            meltfnum[7] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5941:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L8*/ meltfnum[7]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5941:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[8];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5941;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_and cxtup";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CXTUP__V26*/ meltfptr[25];
                            /*_.MELT_DEBUG_FUN__V28*/
                            meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V27*/
                        meltfptr[26] = /*_.MELT_DEBUG_FUN__V28*/ meltfptr[27];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5941:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L9*/
                        meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V28*/
                        meltfptr[27] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V27*/ meltfptr[26] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5941:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L8*/
            meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V27*/
            meltfptr[26] = 0 ;
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
        /*_#ltI__L10*/
        meltfnum[8] =
            ((/*_#NBCOMP__L7*/ meltfnum[0]) < (1));;
        MELT_LOCATION("warmelt-macro.melt:5942:/ cond");
        /*cond*/
        if (/*_#ltI__L10*/ meltfnum[8]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5944:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20]), ( "AND without sons"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5945:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:5945:/ locexp");
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
                    /*finalret*/
                    goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-macro.melt:5943:/ quasiblock");


                    /*_.PROGN___V31*/
                    meltfptr[30] = /*_.RETURN___V30*/ meltfptr[26];;
                    /*^compute*/
                    /*_._IFELSE___V29*/
                    meltfptr[27] = /*_.PROGN___V31*/ meltfptr[30];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5942:/ clear");
                    /*clear*/ /*_.RETURN___V30*/
                    meltfptr[26] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V31*/
                    meltfptr[30] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:5946:/ quasiblock");


                    /*_#miI__L11*/
                    meltfnum[7] =
                        ((/*_#NBCOMP__L7*/ meltfnum[0]) - (1));;
                    /*^compute*/
                    /*_.RES__V33*/
                    meltfptr[30] =
                        (melt_multiple_nth((melt_ptr_t)(/*_.CXTUP__V26*/ meltfptr[25]), (/*_#miI__L11*/ meltfnum[7])));;
                    /*^compute*/
                    /*_#IX__L12*/
                    meltfnum[11] =
                        ((/*_#NBCOMP__L7*/ meltfnum[0]) - (2));;
                    MELT_LOCATION("warmelt-macro.melt:5948:/ loop");
                    /*loop*/
                    {
meltlabloop_REVLOOP_1:;/*^loopbody*/

                        /*^block*/
                        /*anyblock*/
                        {


                            MELT_CHECK_SIGNAL();
                            ;

                            MELT_CHECK_SIGNAL();
                            ;
                            /*_#ltI__L13*/
                            meltfnum[12] =
                                ((/*_#IX__L12*/ meltfnum[11]) < (0));;
                            MELT_LOCATION("warmelt-macro.melt:5949:/ cond");
                            /*cond*/
                            if (/*_#ltI__L13*/ meltfnum[12]) /*then*/
                                {
                                    /*^cond.then*/
                                    /*^block*/
                                    /*anyblock*/
                                    {

                                        /*^quasiblock*/


                                        /*^compute*/
                                        /*_.REVLOOP__V35*/ meltfptr[34] =  /*reallynil*/ NULL ;;

                                        /*^exit*/
                                        /*exit*/
                                        {
                                            goto meltlabexit_REVLOOP_1;
                                        }
                                        ;
                                        /*epilog*/
                                    }
                                    ;
                                } /*noelse*/
                            ;
                            MELT_LOCATION("warmelt-macro.melt:5950:/ quasiblock");


                            /*_.CURC__V37*/
                            meltfptr[36] =
                                (melt_multiple_nth((melt_ptr_t)(/*_.CXTUP__V26*/ meltfptr[25]), (/*_#IX__L12*/ meltfnum[11])));;

                            MELT_CHECK_SIGNAL();
                            ;
                            MELT_LOCATION("warmelt-macro.melt:5952:/ quasiblock");


                            /*^rawallocobj*/
                            /*rawallocobj*/
                            {
                                melt_ptr_t newobj = 0;
                                melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_IF*/ meltfrout->tabval[8])), (4), "CLASS_SOURCE_IF");
                                /*_.INST__V39*/
                                meltfptr[38] =
                                    newobj;
                            };
                            ;
                            /*^putslot*/
                            /*putslot*/
                            melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V39*/ meltfptr[38])) == MELTOBMAG_OBJECT);
                            melt_putfield_object((/*_.INST__V39*/ meltfptr[38]), (1), (/*_.LOC__V21*/ meltfptr[20]), "LOCA_LOCATION");
                            ;
                            /*^putslot*/
                            /*putslot*/
                            melt_assertmsg("checkobj putslot  _ @SIF_TEST", melt_magic_discr((melt_ptr_t)(/*_.INST__V39*/ meltfptr[38])) == MELTOBMAG_OBJECT);
                            melt_putfield_object((/*_.INST__V39*/ meltfptr[38]), (2), (/*_.CURC__V37*/ meltfptr[36]), "SIF_TEST");
                            ;
                            /*^putslot*/
                            /*putslot*/
                            melt_assertmsg("checkobj putslot  _ @SIF_THEN", melt_magic_discr((melt_ptr_t)(/*_.INST__V39*/ meltfptr[38])) == MELTOBMAG_OBJECT);
                            melt_putfield_object((/*_.INST__V39*/ meltfptr[38]), (3), (/*_.RES__V33*/ meltfptr[30]), "SIF_THEN");
                            ;
                            /*^touchobj*/

                            melt_dbgtrace_written_object (/*_.INST__V39*/ meltfptr[38], "newly made instance");
                            ;
                            /*_.INST___V38*/
                            meltfptr[37] = /*_.INST__V39*/ meltfptr[38];;
                            MELT_LOCATION("warmelt-macro.melt:5951:/ compute");
                            /*_.RES__V33*/
                            meltfptr[30] = /*_.SETQ___V40*/ meltfptr[39] = /*_.INST___V38*/ meltfptr[37];;
                            /*_.LET___V36*/
                            meltfptr[35] = /*_.SETQ___V40*/ meltfptr[39];;

                            MELT_LOCATION("warmelt-macro.melt:5950:/ clear");
                            /*clear*/ /*_.CURC__V37*/
                            meltfptr[36] = 0 ;
                            /*^clear*/
                            /*clear*/ /*_.INST___V38*/
                            meltfptr[37] = 0 ;
                            /*^clear*/
                            /*clear*/ /*_.SETQ___V40*/
                            meltfptr[39] = 0 ;
                            /*_#miI__L14*/
                            meltfnum[13] =
                                ((/*_#IX__L12*/ meltfnum[11]) - (1));;
                            MELT_LOCATION("warmelt-macro.melt:5957:/ compute");
                            /*_#IX__L12*/
                            meltfnum[11] = /*_#SETQ___L15*/ meltfnum[14] = /*_#miI__L14*/ meltfnum[13];;

                            MELT_CHECK_SIGNAL();
                            ;
                            /*epilog*/

                            MELT_LOCATION("warmelt-macro.melt:5948:/ clear");
                            /*clear*/ /*_#ltI__L13*/
                            meltfnum[12] = 0 ;
                            /*^clear*/
                            /*clear*/ /*_.LET___V36*/
                            meltfptr[35] = 0 ;
                            /*^clear*/
                            /*clear*/ /*_#miI__L14*/
                            meltfnum[13] = 0 ;
                            /*^clear*/
                            /*clear*/ /*_#SETQ___L15*/
                            meltfnum[14] = 0 ;
                        }
                        ;
                        ;
                        goto meltlabloop_REVLOOP_1;
meltlabexit_REVLOOP_1:;/*^loopepilog*/
                        /*loopepilog*/
                        /*_.FOREVER___V34*/
                        meltfptr[33] = /*_.REVLOOP__V35*/ meltfptr[34];;
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:5959:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L16*/
                        meltfnum[12] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-macro.melt:5959:/ cond");
                        /*cond*/
                        if (/*_#MELT_NEED_DBG__L16*/ meltfnum[12]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L17*/ meltfnum[13] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-macro.melt:5959:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L17*/ meltfnum[13];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 5959;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "mexpand_and res:";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RES__V33*/ meltfptr[30];
                                        /*_.MELT_DEBUG_FUN__V42*/
                                        meltfptr[37] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V41*/
                                    meltfptr[36] = /*_.MELT_DEBUG_FUN__V42*/ meltfptr[37];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:5959:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L17*/
                                    meltfnum[13] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V42*/
                                    meltfptr[37] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V41*/ meltfptr[36] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-macro.melt:5959:/ locexp");
                            /*void*/
                            (void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L16*/
                        meltfnum[12] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V41*/
                        meltfptr[36] = 0 ;
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
                    MELT_LOCATION("warmelt-macro.melt:5960:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = /*_.RES__V33*/ meltfptr[30];;

                    {
                        MELT_LOCATION("warmelt-macro.melt:5960:/ locexp");
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
                    /*finalret*/
                    goto meltlabend_rout ;
                    /*_.LET___V32*/
                    meltfptr[26] = /*_.RETURN___V43*/ meltfptr[39];;

                    MELT_LOCATION("warmelt-macro.melt:5946:/ clear");
                    /*clear*/ /*_#miI__L11*/
                    meltfnum[7] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RES__V33*/
                    meltfptr[30] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#IX__L12*/
                    meltfnum[11] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.FOREVER___V34*/
                    meltfptr[33] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V43*/
                    meltfptr[39] = 0 ;
                    /*_._IFELSE___V29*/
                    meltfptr[27] = /*_.LET___V32*/ meltfptr[26];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5942:/ clear");
                    /*clear*/ /*_.LET___V32*/
                    meltfptr[26] = 0 ;
                }
                ;
            }
        ;
        /*_.LET___V19*/
        meltfptr[17] = /*_._IFELSE___V29*/ meltfptr[27];;

        MELT_LOCATION("warmelt-macro.melt:5929:/ clear");
        /*clear*/ /*_.CONT__V20*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LAMBDA___V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CXTUP__V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NBCOMP__L7*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#ltI__L10*/
        meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V29*/
        meltfptr[27] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5923:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V19*/ meltfptr[17];;

        {
            MELT_LOCATION("warmelt-macro.melt:5923:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V8*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V11*/
        meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L5*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V16*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V19*/
        meltfptr[17] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_AND", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_97_WARMELTmiMACRO_MEXPAND_AND_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_97_WARMELTmiMACRO_MEXPAND_AND*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_98_WARMELTmiMACRO_LAMBDA_cl16(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_98_WARMELTmiMACRO_LAMBDA_cl16_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_98_WARMELTmiMACRO_LAMBDA_cl16 fromline 1715 */

    /** start of frame for meltrout_98_WARMELTmiMACRO_LAMBDA_cl16 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_98_WARMELTmiMACRO_LAMBDA_cl16// fromline 1531
        : public Melt_CallFrameWithValues<3>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_98_WARMELTmiMACRO_LAMBDA_cl16(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_98_WARMELTmiMACRO_LAMBDA_cl16), clos) {};
        MeltFrame_meltrout_98_WARMELTmiMACRO_LAMBDA_cl16() //the constructor fromline 1605
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_98_WARMELTmiMACRO_LAMBDA_cl16)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_98_WARMELTmiMACRO_LAMBDA_cl16(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_98_WARMELTmiMACRO_LAMBDA_cl16)) {};
        MeltFrame_meltrout_98_WARMELTmiMACRO_LAMBDA_cl16(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_98_WARMELTmiMACRO_LAMBDA_cl16), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_98_WARMELTmiMACRO_LAMBDA_cl16


    /** end of frame for meltrout_98_WARMELTmiMACRO_LAMBDA_cl16 fromline 1660**/

    /* end of frame for routine meltrout_98_WARMELTmiMACRO_LAMBDA_cl16 fromline 1719 */

    /* classy proc frame meltrout_98_WARMELTmiMACRO_LAMBDA_cl16 */
    MeltFrame_meltrout_98_WARMELTmiMACRO_LAMBDA_cl16
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_98_WARMELTmiMACRO_LAMBDA_cl16 fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl16", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5936:/ getarg");
    /*_.C__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
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
        MELT_LOCATION("warmelt-macro.melt:5937:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*~ENV*/ meltfclos->tabval[1]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*~MEXPANDER*/ meltfclos->tabval[0]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*~MODCTX*/ meltfclos->tabval[2]);
            /*_.MEXPANDER__V3*/
            meltfptr[2] =  melt_apply ((meltclosure_ptr_t)((/*~MEXPANDER*/ meltfclos->tabval[0])), (melt_ptr_t)(/*_.C__V2*/ meltfptr[1]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:5936:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.MEXPANDER__V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-macro.melt:5936:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.MEXPANDER__V3*/
        meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl16", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_98_WARMELTmiMACRO_LAMBDA_cl16_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_98_WARMELTmiMACRO_LAMBDA_cl16*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_99_WARMELTmiMACRO_PATEXPAND_AS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_99_WARMELTmiMACRO_PATEXPAND_AS_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_99_WARMELTmiMACRO_PATEXPAND_AS fromline 1715 */

    /** start of frame for meltrout_99_WARMELTmiMACRO_PATEXPAND_AS of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_99_WARMELTmiMACRO_PATEXPAND_AS// fromline 1531
        : public Melt_CallFrameWithValues<43>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[13];
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
        MeltFrame_meltrout_99_WARMELTmiMACRO_PATEXPAND_AS(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<43> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_99_WARMELTmiMACRO_PATEXPAND_AS), clos) {};
        MeltFrame_meltrout_99_WARMELTmiMACRO_PATEXPAND_AS() //the constructor fromline 1605
            : Melt_CallFrameWithValues<43> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_99_WARMELTmiMACRO_PATEXPAND_AS)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_99_WARMELTmiMACRO_PATEXPAND_AS(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<43> (fil,lin, sizeof(MeltFrame_meltrout_99_WARMELTmiMACRO_PATEXPAND_AS)) {};
        MeltFrame_meltrout_99_WARMELTmiMACRO_PATEXPAND_AS(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<43> (fil,lin, sizeof(MeltFrame_meltrout_99_WARMELTmiMACRO_PATEXPAND_AS), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_99_WARMELTmiMACRO_PATEXPAND_AS


    /** end of frame for meltrout_99_WARMELTmiMACRO_PATEXPAND_AS fromline 1660**/

    /* end of frame for routine meltrout_99_WARMELTmiMACRO_PATEXPAND_AS fromline 1719 */

    /* classy proc frame meltrout_99_WARMELTmiMACRO_PATEXPAND_AS */
    MeltFrame_meltrout_99_WARMELTmiMACRO_PATEXPAND_AS
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_99_WARMELTmiMACRO_PATEXPAND_AS fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("PATEXPAND_AS", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5964:/ getarg");
    /*_.SEXPR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.PCTX__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.PCTX__V4*/ meltfptr[3])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5965:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:5965:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5965:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5965;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5965:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/
                        meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/
            meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5965:/ clear");
            /*clear*/ /*_#IS_A__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5966:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:5966:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5966:/ cond.else");

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
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5966;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5966:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5966:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5967:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.PCTX__V4*/ meltfptr[3]), (melt_ptr_t)((/*!CLASS_PATTERN_EXPANSION_CONTEXT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:5967:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5967:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check pctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5967;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.PCTX__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5967:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5967:/ clear");
            /*clear*/ /*_#IS_A__L3*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V12*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V11*/ meltfptr[9] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5968:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L4*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5968:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L5*/ meltfnum[4] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:5968:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L5*/ meltfnum[4];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5968;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "patexpand_as sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V15*/
                            meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V14*/
                        meltfptr[12] = /*_.MELT_DEBUG_FUN__V15*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5968:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L5*/
                        meltfnum[4] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V15*/
                        meltfptr[5] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V14*/ meltfptr[12] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5968:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V14*/
            meltfptr[12] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5969:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:5970:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V17*/
            meltfptr[12] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5971:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V18*/
            meltfptr[17] = slot;
        };
        ;
        /*_.LIST_FIRST__V19*/
        meltfptr[18] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V17*/ meltfptr[12])));;
        /*^compute*/
        /*_.CURPAIR__V20*/
        meltfptr[19] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V19*/ meltfptr[18])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5973:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.PCTX__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.LOC__V18*/ meltfptr[17];
            /*_.ARGSP__V21*/
            meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!PATTERNEXPAND_PAIRLIST_AS_TUPLE*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#MULTIPLE_LENGTH__L6*/
        meltfnum[4] =
            (melt_multiple_length((melt_ptr_t)(/*_.ARGSP__V21*/ meltfptr[20])));;
        /*^compute*/
        /*_#exeqI__L7*/
        meltfnum[0] =
            ((/*_#MULTIPLE_LENGTH__L6*/ meltfnum[4]) != (2));;
        MELT_LOCATION("warmelt-macro.melt:5975:/ cond");
        /*cond*/
        if (/*_#exeqI__L7*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5977:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "AS pattern expects two arguments: ?patvar subpattern"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5978:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:5978:/ locexp");
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
                    /*finalret*/
                    goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-macro.melt:5976:/ quasiblock");


                    /*_.PROGN___V24*/
                    meltfptr[23] = /*_.RETURN___V23*/ meltfptr[22];;
                    /*^compute*/
                    /*_._IF___V22*/
                    meltfptr[21] = /*_.PROGN___V24*/ meltfptr[23];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5975:/ clear");
                    /*clear*/ /*_.RETURN___V23*/
                    meltfptr[22] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V24*/
                    meltfptr[23] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V22*/ meltfptr[21] =  /*reallynil*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:5979:/ quasiblock");


        /*_.ARGVAR__V26*/
        meltfptr[23] =
            (melt_multiple_nth((melt_ptr_t)(/*_.ARGSP__V21*/ meltfptr[20]), (0)));;
        /*^compute*/
        /*_.ARGSUB__V27*/
        meltfptr[26] =
            (melt_multiple_nth((melt_ptr_t)(/*_.ARGSP__V21*/ meltfptr[20]), (1)));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_A__L8*/
        meltfnum[7] =
            melt_is_instance_of((melt_ptr_t)(/*_.ARGVAR__V26*/ meltfptr[23]), (melt_ptr_t)((/*!CLASS_SOURCE_PATTERN_JOKER_VARIABLE*/ meltfrout->tabval[6])));;
        MELT_LOCATION("warmelt-macro.melt:5982:/ cond");
        /*cond*/
        if (/*_#IS_A__L8*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5984:/ locexp");
                        melt_warning_str(0, (melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "AS pattern with useless joker"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5985:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = /*_.ARGSUB__V27*/ meltfptr[26];;

                    {
                        MELT_LOCATION("warmelt-macro.melt:5985:/ locexp");
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
                    /*finalret*/
                    goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-macro.melt:5983:/ quasiblock");


                    /*_.PROGN___V30*/
                    meltfptr[29] = /*_.RETURN___V29*/ meltfptr[28];;
                    /*^compute*/
                    /*_._IF___V28*/
                    meltfptr[27] = /*_.PROGN___V30*/ meltfptr[29];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5982:/ clear");
                    /*clear*/ /*_.RETURN___V29*/
                    meltfptr[28] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V30*/
                    meltfptr[29] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V28*/ meltfptr[27] =  /*reallynil*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L9*/
        meltfnum[8] =
            !melt_is_instance_of((melt_ptr_t)(/*_.ARGVAR__V26*/ meltfptr[23]), (melt_ptr_t)((/*!CLASS_SOURCE_PATTERN_VARIABLE*/ meltfrout->tabval[7])));;
        MELT_LOCATION("warmelt-macro.melt:5986:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L9*/ meltfnum[8]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5988:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "AS pattern needs a pattern variable as first argument"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5989:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*reallynil*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:5989:/ locexp");
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
                    /*finalret*/
                    goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-macro.melt:5987:/ quasiblock");


                    /*_.PROGN___V33*/
                    meltfptr[32] = /*_.RETURN___V32*/ meltfptr[29];;
                    /*^compute*/
                    /*_._IF___V31*/
                    meltfptr[28] = /*_.PROGN___V33*/ meltfptr[32];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5986:/ clear");
                    /*clear*/ /*_.RETURN___V32*/
                    meltfptr[29] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V33*/
                    meltfptr[32] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V31*/ meltfptr[28] =  /*reallynil*/ NULL ;;
            }
        ;

        {
            MELT_LOCATION("warmelt-macro.melt:5990:/ locexp");
            melt_warning_str(0, (melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "deprecated AS pattern - use AND pattern instead"), (melt_ptr_t)0);
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:5991:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:5992:/ cond");
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.ARGSUB__V27*/ meltfptr[26]),
                                          (melt_ptr_t)((/*!CLASS_SOURCE_PATTERN*/ meltfrout->tabval[8])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.ARGSUB__V27*/ meltfptr[26]) /*=obj*/;
                    melt_object_get_field(slot,obj, 2, "PAT_WEIGHT");
                    /*_.PAT_WEIGHT__V35*/
                    meltfptr[32] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.PAT_WEIGHT__V35*/ meltfptr[32] =  /*reallynil*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_#SUBW__L10*/
        meltfnum[9] =
            (melt_get_int((melt_ptr_t)(/*_.PAT_WEIGHT__V35*/ meltfptr[32])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#plI__L11*/
        meltfnum[10] =
            ((2) + (/*_#SUBW__L10*/ meltfnum[9]));;
        /*^compute*/
        /*_.MAKE_INTEGERBOX__V36*/
        meltfptr[35] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_CONSTANT_INTEGER*/ meltfrout->tabval[10])), (/*_#plI__L11*/ meltfnum[10])));;
        MELT_LOCATION("warmelt-macro.melt:5996:/ blockmultialloc");
        /*multiallocblock*/
        {
            struct meltletrec_1_st
            {
                struct MELT_MULTIPLE_STRUCT(2) rtup_0__TUPLREC__x5;
                long meltletrec_1_endgap;
            } *meltletrec_1_ptr = 0;
            meltletrec_1_ptr = (struct meltletrec_1_st *) meltgc_allocate (sizeof (struct meltletrec_1_st), 0);
            /*^blockmultialloc.initfill*/
            /*inimult rtup_0__TUPLREC__x5*/
            /*_.TUPLREC___V38*/
            meltfptr[37] = (melt_ptr_t) &meltletrec_1_ptr->rtup_0__TUPLREC__x5;
            meltletrec_1_ptr->rtup_0__TUPLREC__x5.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_MULTIPLE))));
            meltletrec_1_ptr->rtup_0__TUPLREC__x5.nbval = 2;


            /*^putuple*/
            /*putupl#7*/
            melt_assertmsg("putupl [:5996] #7 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V38*/ meltfptr[37]))== MELTOBMAG_MULTIPLE);
            melt_assertmsg("putupl [:5996] #7 checkoff", (0>=0 && 0< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V38*/ meltfptr[37]))));
            ((meltmultiple_ptr_t)(/*_.TUPLREC___V38*/ meltfptr[37]))->tabval[0] = (melt_ptr_t)(/*_.ARGVAR__V26*/ meltfptr[23]);
            ;
            /*^putuple*/
            /*putupl#8*/
            melt_assertmsg("putupl [:5996] #8 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V38*/ meltfptr[37]))== MELTOBMAG_MULTIPLE);
            melt_assertmsg("putupl [:5996] #8 checkoff", (1>=0 && 1< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V38*/ meltfptr[37]))));
            ((meltmultiple_ptr_t)(/*_.TUPLREC___V38*/ meltfptr[37]))->tabval[1] = (melt_ptr_t)(/*_.ARGSUB__V27*/ meltfptr[26]);
            ;
            /*^touch*/
            meltgc_touch(/*_.TUPLREC___V38*/ meltfptr[37]);
            ;
            /*_.TUPLE___V37*/
            meltfptr[36] = /*_.TUPLREC___V38*/ meltfptr[37];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5996:/ clear");
            /*clear*/ /*_.TUPLREC___V38*/
            meltfptr[37] = 0 ;
            /*^clear*/
            /*clear*/ /*_.TUPLREC___V38*/
            meltfptr[37] = 0 ;
        } /*end multiallocblock*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5993:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_PATTERN_AND*/ meltfrout->tabval[9])), (4), "CLASS_SOURCE_PATTERN_AND");
            /*_.INST__V40*/
            meltfptr[39] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V40*/ meltfptr[39])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V40*/ meltfptr[39]), (1), (/*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @PAT_WEIGHT", melt_magic_discr((melt_ptr_t)(/*_.INST__V40*/ meltfptr[39])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V40*/ meltfptr[39]), (2), (/*_.MAKE_INTEGERBOX__V36*/ meltfptr[35]), "PAT_WEIGHT");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @ANDPAT_CONJ", melt_magic_discr((melt_ptr_t)(/*_.INST__V40*/ meltfptr[39])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V40*/ meltfptr[39]), (3), (/*_.TUPLE___V37*/ meltfptr[36]), "ANDPAT_CONJ");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V40*/ meltfptr[39], "newly made instance");
        ;
        /*_.RES__V39*/
        meltfptr[37] = /*_.INST__V40*/ meltfptr[39];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5998:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L12*/
            meltfnum[11] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5998:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L12*/ meltfnum[11]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:5998:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L13*/ meltfnum[12];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5998;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "patexpand_as returns res";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RES__V39*/ meltfptr[37];
                            /*_.MELT_DEBUG_FUN__V42*/
                            meltfptr[41] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V41*/
                        meltfptr[40] = /*_.MELT_DEBUG_FUN__V42*/ meltfptr[41];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5998:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L13*/
                        meltfnum[12] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V42*/
                        meltfptr[41] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V41*/ meltfptr[40] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5998:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L12*/
            meltfnum[11] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V41*/
            meltfptr[40] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5999:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.RES__V39*/ meltfptr[37];;

        {
            MELT_LOCATION("warmelt-macro.melt:5999:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*_.LET___V34*/
        meltfptr[29] = /*_.RETURN___V43*/ meltfptr[41];;

        MELT_LOCATION("warmelt-macro.melt:5991:/ clear");
        /*clear*/ /*_.PAT_WEIGHT__V35*/
        meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_#SUBW__L10*/
        meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L11*/
        meltfnum[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_INTEGERBOX__V36*/
        meltfptr[35] = 0 ;
        /*^clear*/
        /*clear*/ /*_.TUPLE___V37*/
        meltfptr[36] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RES__V39*/
        meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V43*/
        meltfptr[41] = 0 ;
        /*_.LET___V25*/
        meltfptr[22] = /*_.LET___V34*/ meltfptr[29];;

        MELT_LOCATION("warmelt-macro.melt:5979:/ clear");
        /*clear*/ /*_.ARGVAR__V26*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ARGSUB__V27*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L8*/
        meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L9*/
        meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V31*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V34*/
        meltfptr[29] = 0 ;
        /*_.LET___V16*/
        meltfptr[5] = /*_.LET___V25*/ meltfptr[22];;

        MELT_LOCATION("warmelt-macro.melt:5969:/ clear");
        /*clear*/ /*_.CONT__V17*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V20*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ARGSP__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_#MULTIPLE_LENGTH__L6*/
        meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#exeqI__L7*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V25*/
        meltfptr[22] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5964:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V16*/ meltfptr[5];;

        {
            MELT_LOCATION("warmelt-macro.melt:5964:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V5*/
        meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V8*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V11*/
        meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V16*/
        meltfptr[5] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("PATEXPAND_AS", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_99_WARMELTmiMACRO_PATEXPAND_AS_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_99_WARMELTmiMACRO_PATEXPAND_AS*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_100_WARMELTmiMACRO_MEXPAND_AS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_100_WARMELTmiMACRO_MEXPAND_AS_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_100_WARMELTmiMACRO_MEXPAND_AS fromline 1715 */

    /** start of frame for meltrout_100_WARMELTmiMACRO_MEXPAND_AS of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_100_WARMELTmiMACRO_MEXPAND_AS// fromline 1531
        : public Melt_CallFrameWithValues<16>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_100_WARMELTmiMACRO_MEXPAND_AS(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<16> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_100_WARMELTmiMACRO_MEXPAND_AS), clos) {};
        MeltFrame_meltrout_100_WARMELTmiMACRO_MEXPAND_AS() //the constructor fromline 1605
            : Melt_CallFrameWithValues<16> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_100_WARMELTmiMACRO_MEXPAND_AS)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_100_WARMELTmiMACRO_MEXPAND_AS(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<16> (fil,lin, sizeof(MeltFrame_meltrout_100_WARMELTmiMACRO_MEXPAND_AS)) {};
        MeltFrame_meltrout_100_WARMELTmiMACRO_MEXPAND_AS(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<16> (fil,lin, sizeof(MeltFrame_meltrout_100_WARMELTmiMACRO_MEXPAND_AS), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_100_WARMELTmiMACRO_MEXPAND_AS


    /** end of frame for meltrout_100_WARMELTmiMACRO_MEXPAND_AS fromline 1660**/

    /* end of frame for routine meltrout_100_WARMELTmiMACRO_MEXPAND_AS fromline 1719 */

    /* classy proc frame meltrout_100_WARMELTmiMACRO_MEXPAND_AS */
    MeltFrame_meltrout_100_WARMELTmiMACRO_MEXPAND_AS
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_100_WARMELTmiMACRO_MEXPAND_AS fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_AS", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:6002:/ getarg");
    /*_.SEXPR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V5*/
    meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:6003:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:6003:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:6003:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6003;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                            meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V7*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6003:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V8*/
                        meltfptr[7] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V6*/
            meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:6003:/ clear");
            /*clear*/ /*_#IS_A__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V7*/
            meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V6*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:6004:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:6004:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:6004:/ cond.else");

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
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6004;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                            meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V10*/
                        meltfptr[6] = /*_.MELT_ASSERT_FAILURE_FUN__V11*/ meltfptr[10];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6004:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V11*/
                        meltfptr[10] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V9*/
            meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:6004:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V10*/
            meltfptr[6] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V9*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:6005:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L3*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:6005:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L3*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L4*/ meltfnum[3] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:6005:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L4*/ meltfnum[3];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6005;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_as sexpr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V13*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V12*/
                        meltfptr[10] = /*_.MELT_DEBUG_FUN__V13*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6005:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L4*/
                        meltfnum[3] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V13*/
                        meltfptr[6] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V12*/ meltfptr[10] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:6005:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L3*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V12*/
            meltfptr[10] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:6006:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:6007:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V15*/
            meltfptr[10] = slot;
        };
        ;

        {
            MELT_LOCATION("warmelt-macro.melt:6009:/ locexp");
            /* error_plain */
            melt_error_str((melt_ptr_t)(/*_.LOC__V15*/ meltfptr[10]), ( "AS cannot be macro-expanded in expression context"), (melt_ptr_t)0);
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:6010:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] =  /*reallynil*/ NULL ;;

        {
            MELT_LOCATION("warmelt-macro.melt:6010:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*_.LET___V14*/
        meltfptr[6] = /*_.RETURN___V16*/ meltfptr[15];;

        MELT_LOCATION("warmelt-macro.melt:6006:/ clear");
        /*clear*/ /*_.LOC__V15*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V16*/
        meltfptr[15] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:6002:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V14*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-macro.melt:6002:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V6*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V9*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V14*/
        meltfptr[6] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_AS", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_100_WARMELTmiMACRO_MEXPAND_AS_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_100_WARMELTmiMACRO_MEXPAND_AS*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN fromline 1715 */

    /** start of frame for meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN// fromline 1531
        : public Melt_CallFrameWithValues<22>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<22> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN), clos) {};
        MeltFrame_meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN() //the constructor fromline 1605
            : Melt_CallFrameWithValues<22> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<22> (fil,lin, sizeof(MeltFrame_meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN)) {};
        MeltFrame_meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<22> (fil,lin, sizeof(MeltFrame_meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN


    /** end of frame for meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN fromline 1660**/

    /* end of frame for routine meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN fromline 1719 */

    /* classy proc frame meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN */
    MeltFrame_meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("PATEXPAND_WHEN", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:6016:/ getarg");
    /*_.SEXPR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.PCTX__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.PCTX__V4*/ meltfptr[3])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:6017:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:6017:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L1*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:6017:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6017;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "patexpand_when sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V6*/
                            meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/
                        meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6017:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/
                        meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V6*/
                        meltfptr[5] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V5*/ meltfptr[4] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:6017:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V5*/
            meltfptr[4] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:6018:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:6018:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:6018:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6018;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/
                            meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/
                        meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6018:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/
                        meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/
            meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:6018:/ clear");
            /*clear*/ /*_#IS_A__L3*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/
            meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:6019:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:6019:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:6019:/ cond.else");

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
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6019;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V12*/
                            meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V11*/
                        meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6019:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V12*/
                        meltfptr[11] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V10*/
            meltfptr[8] = /*_._IFELSE___V11*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:6019:/ clear");
            /*clear*/ /*_#IS_A__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V11*/
            meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V10*/ meltfptr[8] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:6020:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L5*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.PCTX__V4*/ meltfptr[3]), (melt_ptr_t)((/*!CLASS_PATTERN_EXPANSION_CONTEXT*/ meltfrout->tabval[4])));;
            MELT_LOCATION("warmelt-macro.melt:6020:/ cond");
            /*cond*/
            if (/*_#IS_A__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V14*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:6020:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check pctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6020;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.PCTX__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V15*/
                            meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V14*/
                        meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6020:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V15*/
                        meltfptr[14] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V13*/
            meltfptr[11] = /*_._IFELSE___V14*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:6020:/ clear");
            /*clear*/ /*_#IS_A__L5*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V14*/
            meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V13*/ meltfptr[11] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:6021:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:6022:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V17*/
            meltfptr[4] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:6023:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V18*/
            meltfptr[17] = slot;
        };
        ;

        {
            MELT_LOCATION("warmelt-macro.melt:6026:/ locexp");
            /* error_plain */
            melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "WHEN is not yet implemented in patterns"), (melt_ptr_t)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:6027:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*^cond*/
            /*cond*/
            if ((/*nil*/NULL)) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V20*/ meltfptr[19] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:6027:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "@$@ unimplemented patexpand_when";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6027;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V21*/
                            meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V20*/
                        meltfptr[19] = /*_.MELT_ASSERT_FAILURE_FUN__V21*/ meltfptr[20];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6027:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V21*/
                        meltfptr[20] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V19*/
            meltfptr[18] = /*_._IFELSE___V20*/ meltfptr[19];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:6027:/ clear");
            /*clear*/ /*_._IFELSE___V20*/
            meltfptr[19] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V19*/ meltfptr[18] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:6028:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] =  /*reallynil*/ NULL ;;

        {
            MELT_LOCATION("warmelt-macro.melt:6028:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*_.LET___V16*/
        meltfptr[14] = /*_.RETURN___V22*/ meltfptr[20];;

        MELT_LOCATION("warmelt-macro.melt:6021:/ clear");
        /*clear*/ /*_.CONT__V17*/
        meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V22*/
        meltfptr[20] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:6016:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-macro.melt:6016:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V7*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V10*/
        meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V13*/
        meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V16*/
        meltfptr[14] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("PATEXPAND_WHEN", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_101_WARMELTmiMACRO_PATEXPAND_WHEN*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_102_WARMELTmiMACRO_PATEXPAND_AND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_102_WARMELTmiMACRO_PATEXPAND_AND_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_102_WARMELTmiMACRO_PATEXPAND_AND fromline 1715 */

    /** start of frame for meltrout_102_WARMELTmiMACRO_PATEXPAND_AND of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_102_WARMELTmiMACRO_PATEXPAND_AND// fromline 1531
        : public Melt_CallFrameWithValues<28>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_102_WARMELTmiMACRO_PATEXPAND_AND(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<28> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_102_WARMELTmiMACRO_PATEXPAND_AND), clos) {};
        MeltFrame_meltrout_102_WARMELTmiMACRO_PATEXPAND_AND() //the constructor fromline 1605
            : Melt_CallFrameWithValues<28> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_102_WARMELTmiMACRO_PATEXPAND_AND)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_102_WARMELTmiMACRO_PATEXPAND_AND(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<28> (fil,lin, sizeof(MeltFrame_meltrout_102_WARMELTmiMACRO_PATEXPAND_AND)) {};
        MeltFrame_meltrout_102_WARMELTmiMACRO_PATEXPAND_AND(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<28> (fil,lin, sizeof(MeltFrame_meltrout_102_WARMELTmiMACRO_PATEXPAND_AND), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_102_WARMELTmiMACRO_PATEXPAND_AND


    /** end of frame for meltrout_102_WARMELTmiMACRO_PATEXPAND_AND fromline 1660**/

    /* end of frame for routine meltrout_102_WARMELTmiMACRO_PATEXPAND_AND fromline 1719 */

    /* classy proc frame meltrout_102_WARMELTmiMACRO_PATEXPAND_AND */
    MeltFrame_meltrout_102_WARMELTmiMACRO_PATEXPAND_AND
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_102_WARMELTmiMACRO_PATEXPAND_AND fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("PATEXPAND_AND", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:6036:/ getarg");
    /*_.SEXPR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.PCTX__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.PCTX__V4*/ meltfptr[3])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:6037:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:6037:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:6037:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6037;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V6*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6037:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V7*/
                        meltfptr[6] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V5*/
            meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:6037:/ clear");
            /*clear*/ /*_#IS_A__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V6*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V5*/ meltfptr[4] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:6038:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:6038:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:6038:/ cond.else");

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
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6038;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6038:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:6038:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:6039:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.PCTX__V4*/ meltfptr[3]), (melt_ptr_t)((/*!CLASS_PATTERN_EXPANSION_CONTEXT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:6039:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:6039:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check pctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6039;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.PCTX__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6039:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:6039:/ clear");
            /*clear*/ /*_#IS_A__L3*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V12*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V11*/ meltfptr[9] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:6040:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L4*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:6040:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L5*/ meltfnum[4] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:6040:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L5*/ meltfnum[4];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6040;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "patexpand_and sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V15*/
                            meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V14*/
                        meltfptr[12] = /*_.MELT_DEBUG_FUN__V15*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6040:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L5*/
                        meltfnum[4] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V15*/
                        meltfptr[5] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V14*/ meltfptr[12] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:6040:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V14*/
            meltfptr[12] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:6041:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:6042:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V17*/
            meltfptr[12] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:6043:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V18*/
            meltfptr[17] = slot;
        };
        ;
        /*_.LIST_FIRST__V19*/
        meltfptr[18] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V17*/ meltfptr[12])));;
        /*^compute*/
        /*_.CURPAIR__V20*/
        meltfptr[19] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V19*/ meltfptr[18])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:6045:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.PCTX__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.LOC__V18*/ meltfptr[17];
            /*_.ARGSP__V21*/
            meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!PATTERNEXPAND_PAIRLIST_AS_TUPLE*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:6046:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_PATTERN_AND*/ meltfrout->tabval[6])), (4), "CLASS_SOURCE_PATTERN_AND");
            /*_.INST__V23*/
            meltfptr[22] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V23*/ meltfptr[22])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V23*/ meltfptr[22]), (1), (/*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @ANDPAT_CONJ", melt_magic_discr((melt_ptr_t)(/*_.INST__V23*/ meltfptr[22])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V23*/ meltfptr[22]), (3), (/*_.ARGSP__V21*/ meltfptr[20]), "ANDPAT_CONJ");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V23*/ meltfptr[22], "newly made instance");
        ;
        /*_.RES__V22*/
        meltfptr[21] = /*_.INST__V23*/ meltfptr[22];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:6050:/ quasiblock");


        /*^multiapply*/
        /*multiapply 1args, 3x.res*/
        {

            union meltparam_un restab[3];
            memset(&restab, 0, sizeof(restab));
            /*^multiapply.xres*/
            restab[0].meltbp_longptr =  & /*_#IMAX__L6*/ meltfnum[4];
            /*^multiapply.xres*/
            restab[1].meltbp_longptr =  & /*_#IMIN__L7*/ meltfnum[0];
            /*^multiapply.xres*/
            restab[2].meltbp_longptr =  & /*_#ISUM__L8*/ meltfnum[7];
            /*^multiapply.appl*/
            /*_.SUBPATW__V24*/
            meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!PATTERN_WEIGHT_TUPLE*/ meltfrout->tabval[7])), (melt_ptr_t)(/*_.ARGSP__V21*/ meltfptr[20]), (""), (union meltparam_un*)0, (MELTBPARSTR_LONG MELTBPARSTR_LONG MELTBPARSTR_LONG ""), restab);
        }
        ;
        /*^quasiblock*/



        MELT_CHECK_SIGNAL();
        ;
        /*_#plI__L9*/
        meltfnum[8] =
            ((1) + (/*_#IMAX__L6*/ meltfnum[4]));;
        /*^compute*/
        /*_#plI__L10*/
        meltfnum[9] =
            ((/*_#plI__L9*/ meltfnum[8]) + (/*_#ISUM__L8*/ meltfnum[7]));;
        /*^compute*/
        /*_.MAKE_INTEGERBOX__V25*/
        meltfptr[24] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_CONSTANT_INTEGER*/ meltfrout->tabval[8])), (/*_#plI__L10*/ meltfnum[9])));;
        MELT_LOCATION("warmelt-macro.melt:6053:/ quasiblock");


        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @PAT_WEIGHT", melt_magic_discr((melt_ptr_t)(/*_.RES__V22*/ meltfptr[21])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.RES__V22*/ meltfptr[21]), (2), (/*_.MAKE_INTEGERBOX__V25*/ meltfptr[24]), "PAT_WEIGHT");
        ;
        /*^touch*/
        meltgc_touch(/*_.RES__V22*/ meltfptr[21]);
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.RES__V22*/ meltfptr[21], "put-fields");
        ;


        MELT_LOCATION("warmelt-macro.melt:6050:/ clear");
        /*clear*/ /*_#plI__L9*/
        meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_#plI__L10*/
        meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_INTEGERBOX__V25*/
        meltfptr[24] = 0 ;

        /*^clear*/
        /*clear*/ /*_#IMAX__L6*/
        meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IMIN__L7*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#ISUM__L8*/
        meltfnum[7] = 0 ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:6056:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L11*/
            meltfnum[8] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:6056:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L11*/ meltfnum[8]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L12*/ meltfnum[9] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:6056:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[9];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6056;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "patexpand_and res";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RES__V22*/ meltfptr[21];
                            /*_.MELT_DEBUG_FUN__V27*/
                            meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[4])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V26*/
                        meltfptr[24] = /*_.MELT_DEBUG_FUN__V27*/ meltfptr[23];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6056:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L12*/
                        meltfnum[9] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V27*/
                        meltfptr[23] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V26*/ meltfptr[24] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:6056:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L11*/
            meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V26*/
            meltfptr[24] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:6057:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.RES__V22*/ meltfptr[21];;

        {
            MELT_LOCATION("warmelt-macro.melt:6057:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*_.LET___V16*/
        meltfptr[5] = /*_.RETURN___V28*/ meltfptr[23];;

        MELT_LOCATION("warmelt-macro.melt:6041:/ clear");
        /*clear*/ /*_.CONT__V17*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V20*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ARGSP__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RES__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V28*/
        meltfptr[23] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:6036:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V16*/ meltfptr[5];;

        {
            MELT_LOCATION("warmelt-macro.melt:6036:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V5*/
        meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V8*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V11*/
        meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V16*/
        meltfptr[5] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("PATEXPAND_AND", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_102_WARMELTmiMACRO_PATEXPAND_AND_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_102_WARMELTmiMACRO_PATEXPAND_AND*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_103_WARMELTmiMACRO_MEXPAND_OR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_103_WARMELTmiMACRO_MEXPAND_OR_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_103_WARMELTmiMACRO_MEXPAND_OR fromline 1715 */

    /** start of frame for meltrout_103_WARMELTmiMACRO_MEXPAND_OR of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_103_WARMELTmiMACRO_MEXPAND_OR// fromline 1531
        : public Melt_CallFrameWithValues<35>
    {
    public: /* fromline 1535*/
        long mcfr_varnum[10];
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
        MeltFrame_meltrout_103_WARMELTmiMACRO_MEXPAND_OR(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<35> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_103_WARMELTmiMACRO_MEXPAND_OR), clos) {};
        MeltFrame_meltrout_103_WARMELTmiMACRO_MEXPAND_OR() //the constructor fromline 1605
            : Melt_CallFrameWithValues<35> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_103_WARMELTmiMACRO_MEXPAND_OR)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_103_WARMELTmiMACRO_MEXPAND_OR(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<35> (fil,lin, sizeof(MeltFrame_meltrout_103_WARMELTmiMACRO_MEXPAND_OR)) {};
        MeltFrame_meltrout_103_WARMELTmiMACRO_MEXPAND_OR(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<35> (fil,lin, sizeof(MeltFrame_meltrout_103_WARMELTmiMACRO_MEXPAND_OR), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_103_WARMELTmiMACRO_MEXPAND_OR


    /** end of frame for meltrout_103_WARMELTmiMACRO_MEXPAND_OR fromline 1660**/

    /* end of frame for routine meltrout_103_WARMELTmiMACRO_MEXPAND_OR fromline 1719 */

    /* classy proc frame meltrout_103_WARMELTmiMACRO_MEXPAND_OR */
    MeltFrame_meltrout_103_WARMELTmiMACRO_MEXPAND_OR
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_103_WARMELTmiMACRO_MEXPAND_OR fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_OR", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:6072:/ getarg");
    /*_.SEXPR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MEXPANDER__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.MODCTX__V5*/
    meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:6073:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:6073:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L1*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:6073:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6073;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_or sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6073:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/
                        meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V7*/
                        meltfptr[6] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V6*/ meltfptr[5] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:6073:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V6*/
            meltfptr[5] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:6074:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:6074:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:6074:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6074;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V9*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6074:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:6074:/ clear");
            /*clear*/ /*_#IS_A__L3*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:6075:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:6075:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:6075:/ cond.else");

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
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6075;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                            meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V12*/
                        meltfptr[5] = /*_.MELT_ASSERT_FAILURE_FUN__V13*/ meltfptr[12];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6075:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V13*/
                        meltfptr[12] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:6075:/ clear");
            /*clear*/ /*_#IS_A__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V12*/
            meltfptr[5] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V11*/ meltfptr[9] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L5*/
        meltfnum[1] =
            ((/*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:6076:/ cond");
        /*cond*/
        if (/*_#NULL__L5*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V15*/ meltfptr[5] = (/*!MACROEXPAND_1*/ meltfrout->tabval[4]);;
                    /*_._IF___V14*/
                    meltfptr[12] = /*_.SETQ___V15*/ meltfptr[5];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:6076:/ clear");
                    /*clear*/ /*_.SETQ___V15*/
                    meltfptr[5] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V14*/ meltfptr[12] =  /*reallynil*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:6077:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L6*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:6077:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V17*/ meltfptr[16] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:6077:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check modctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6077;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                            /*_.MELT_ASSERT_FAILURE_FUN__V18*/
                            meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V17*/
                        meltfptr[16] = /*_.MELT_ASSERT_FAILURE_FUN__V18*/ meltfptr[17];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6077:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V18*/
                        meltfptr[17] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V16*/
            meltfptr[5] = /*_._IFELSE___V17*/ meltfptr[16];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:6077:/ clear");
            /*clear*/ /*_#IS_OBJECT__L6*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V17*/
            meltfptr[16] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V16*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:6078:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:6079:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V20*/
            meltfptr[16] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:6080:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V21*/
            meltfptr[20] = slot;
        };
        ;
        /*_.LIST_FIRST__V22*/
        meltfptr[21] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V20*/ meltfptr[16])));;
        /*^compute*/
        /*_.PAIR_TAIL__V23*/
        meltfptr[22] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V22*/ meltfptr[21])));;
        MELT_LOCATION("warmelt-macro.melt:6084:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V25*/
        meltfptr[24] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_7*/ meltfrout->tabval[7])), (3));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V25*/ meltfptr[24])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V25*/ meltfptr[24])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V25*/ meltfptr[24])->tabval[0] = (melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V25*/ meltfptr[24])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 1>= 0 && 1< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V25*/ meltfptr[24])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V25*/ meltfptr[24])->tabval[1] = (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V25*/ meltfptr[24])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 2>= 0 && 2< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V25*/ meltfptr[24])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V25*/ meltfptr[24])->tabval[2] = (melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4]);
        ;
        /*_.LAMBDA___V24*/
        meltfptr[23] = /*_.LAMBDA___V25*/ meltfptr[24];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:6081:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[6]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.LAMBDA___V24*/ meltfptr[23];
            /*_.CXTUP__V26*/
            meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!PAIRLIST_TO_MULTIPLE*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.PAIR_TAIL__V23*/ meltfptr[22]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_#NBCOMP__L7*/
        meltfnum[0] =
            (melt_multiple_length((melt_ptr_t)(/*_.CXTUP__V26*/ meltfptr[25])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#ltI__L8*/
        meltfnum[7] =
            ((/*_#NBCOMP__L7*/ meltfnum[0]) < (1));;
        MELT_LOCATION("warmelt-macro.melt:6089:/ cond");
        /*cond*/
        if (/*_#ltI__L8*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:6091:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V21*/ meltfptr[20]), ( "OR without sons"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:6092:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:6092:/ locexp");
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
                    /*finalret*/
                    goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-macro.melt:6090:/ quasiblock");


                    /*_.PROGN___V29*/
                    meltfptr[28] = /*_.RETURN___V28*/ meltfptr[27];;
                    /*^compute*/
                    /*_._IFELSE___V27*/
                    meltfptr[26] = /*_.PROGN___V29*/ meltfptr[28];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:6089:/ clear");
                    /*clear*/ /*_.RETURN___V28*/
                    meltfptr[27] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V29*/
                    meltfptr[28] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:6093:/ quasiblock");



                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:6094:/ quasiblock");


                    /*^rawallocobj*/
                    /*rawallocobj*/
                    {
                        melt_ptr_t newobj = 0;
                        melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_OR*/ meltfrout->tabval[8])), (3), "CLASS_SOURCE_OR");
                        /*_.INST__V32*/
                        meltfptr[31] =
                            newobj;
                    };
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V32*/ meltfptr[31])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V32*/ meltfptr[31]), (1), (/*_.LOC__V21*/ meltfptr[20]), "LOCA_LOCATION");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SOR_DISJ", melt_magic_discr((melt_ptr_t)(/*_.INST__V32*/ meltfptr[31])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V32*/ meltfptr[31]), (2), (/*_.CXTUP__V26*/ meltfptr[25]), "SOR_DISJ");
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object (/*_.INST__V32*/ meltfptr[31], "newly made instance");
                    ;
                    /*_.RES__V31*/
                    meltfptr[28] = /*_.INST__V32*/ meltfptr[31];;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:6097:/ cppif.then");
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
                        /*_#MELT_NEED_DBG__L9*/
                        meltfnum[8] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-macro.melt:6097:/ cond");
                        /*cond*/
                        if (/*_#MELT_NEED_DBG__L9*/ meltfnum[8]) /*then*/
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
                                    MELT_LOCATION("warmelt-macro.melt:6097:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L10*/ meltfnum[9];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 6097;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "mexpand_or res";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RES__V31*/ meltfptr[28];
                                        /*_.MELT_DEBUG_FUN__V34*/
                                        meltfptr[33] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V33*/
                                    meltfptr[32] = /*_.MELT_DEBUG_FUN__V34*/ meltfptr[33];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:6097:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L10*/
                                    meltfnum[9] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V34*/
                                    meltfptr[33] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V33*/ meltfptr[32] =  /*reallynil*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-macro.melt:6097:/ locexp");
                            /*void*/
                            (void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L9*/
                        meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V33*/
                        meltfptr[32] = 0 ;
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
                    MELT_LOCATION("warmelt-macro.melt:6098:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = /*_.RES__V31*/ meltfptr[28];;

                    {
                        MELT_LOCATION("warmelt-macro.melt:6098:/ locexp");
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
                    /*finalret*/
                    goto meltlabend_rout ;
                    /*_.LET___V30*/
                    meltfptr[27] = /*_.RETURN___V35*/ meltfptr[33];;

                    MELT_LOCATION("warmelt-macro.melt:6093:/ clear");
                    /*clear*/ /*_.RES__V31*/
                    meltfptr[28] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V35*/
                    meltfptr[33] = 0 ;
                    /*_._IFELSE___V27*/
                    meltfptr[26] = /*_.LET___V30*/ meltfptr[27];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:6089:/ clear");
                    /*clear*/ /*_.LET___V30*/
                    meltfptr[27] = 0 ;
                }
                ;
            }
        ;
        /*_.LET___V19*/
        meltfptr[17] = /*_._IFELSE___V27*/ meltfptr[26];;

        MELT_LOCATION("warmelt-macro.melt:6078:/ clear");
        /*clear*/ /*_.CONT__V20*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LAMBDA___V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CXTUP__V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NBCOMP__L7*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#ltI__L8*/
        meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V27*/
        meltfptr[26] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:6072:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V19*/ meltfptr[17];;

        {
            MELT_LOCATION("warmelt-macro.melt:6072:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V8*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V11*/
        meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L5*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V16*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V19*/
        meltfptr[17] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_OR", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_103_WARMELTmiMACRO_MEXPAND_OR_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_103_WARMELTmiMACRO_MEXPAND_OR*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_104_WARMELTmiMACRO_LAMBDA_cl17(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_104_WARMELTmiMACRO_LAMBDA_cl17_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_104_WARMELTmiMACRO_LAMBDA_cl17 fromline 1715 */

    /** start of frame for meltrout_104_WARMELTmiMACRO_LAMBDA_cl17 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_104_WARMELTmiMACRO_LAMBDA_cl17// fromline 1531
        : public Melt_CallFrameWithValues<3>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_104_WARMELTmiMACRO_LAMBDA_cl17(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_104_WARMELTmiMACRO_LAMBDA_cl17), clos) {};
        MeltFrame_meltrout_104_WARMELTmiMACRO_LAMBDA_cl17() //the constructor fromline 1605
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_104_WARMELTmiMACRO_LAMBDA_cl17)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_104_WARMELTmiMACRO_LAMBDA_cl17(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_104_WARMELTmiMACRO_LAMBDA_cl17)) {};
        MeltFrame_meltrout_104_WARMELTmiMACRO_LAMBDA_cl17(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_104_WARMELTmiMACRO_LAMBDA_cl17), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_104_WARMELTmiMACRO_LAMBDA_cl17


    /** end of frame for meltrout_104_WARMELTmiMACRO_LAMBDA_cl17 fromline 1660**/

    /* end of frame for routine meltrout_104_WARMELTmiMACRO_LAMBDA_cl17 fromline 1719 */

    /* classy proc frame meltrout_104_WARMELTmiMACRO_LAMBDA_cl17 */
    MeltFrame_meltrout_104_WARMELTmiMACRO_LAMBDA_cl17
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_104_WARMELTmiMACRO_LAMBDA_cl17 fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl17", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:6084:/ getarg");
    /*_.C__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
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
        MELT_LOCATION("warmelt-macro.melt:6085:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*~ENV*/ meltfclos->tabval[1]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*~MEXPANDER*/ meltfclos->tabval[0]);
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*~MODCTX*/ meltfclos->tabval[2]);
            /*_.MEXPANDER__V3*/
            meltfptr[2] =  melt_apply ((meltclosure_ptr_t)((/*~MEXPANDER*/ meltfclos->tabval[0])), (melt_ptr_t)(/*_.C__V2*/ meltfptr[1]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:6084:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.MEXPANDER__V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-macro.melt:6084:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.MEXPANDER__V3*/
        meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl17", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_104_WARMELTmiMACRO_LAMBDA_cl17_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_104_WARMELTmiMACRO_LAMBDA_cl17*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_105_WARMELTmiMACRO_PATEXPAND_OR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_105_WARMELTmiMACRO_PATEXPAND_OR_melt =  melt_blocklevel_signals;

#if MELT_HAVE_DEBUG
    static long melt_call_counter__;
    long melt_thiscallcounter__ ATTRIBUTE_UNUSED = ++ melt_call_counter__;
#undef meltcallcount
#define meltcallcount melt_thiscallcounter__
#else
#undef meltcallcount
#define meltcallcount 0L
#endif
//// emitted by output_curframe_declstruct_init using OUTPUT_CURFRAME_DECLSTRUCT @warmelt-outobj.melt:1501

    /* start of frame for routine meltrout_105_WARMELTmiMACRO_PATEXPAND_OR fromline 1715 */

    /** start of frame for meltrout_105_WARMELTmiMACRO_PATEXPAND_OR of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_105_WARMELTmiMACRO_PATEXPAND_OR// fromline 1531
        : public Melt_CallFrameWithValues<29>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_105_WARMELTmiMACRO_PATEXPAND_OR(meltclosure_ptr_t clos) //the closure constructor fromline 1593
            : Melt_CallFrameWithValues<29> (
#if ENABLE_CHECKING /*fromline 1597*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1601*/
                sizeof(MeltFrame_meltrout_105_WARMELTmiMACRO_PATEXPAND_OR), clos) {};
        MeltFrame_meltrout_105_WARMELTmiMACRO_PATEXPAND_OR() //the constructor fromline 1605
            : Melt_CallFrameWithValues<29> (
#if ENABLE_CHECKING /*fromline 1609*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1613*/
                sizeof(MeltFrame_meltrout_105_WARMELTmiMACRO_PATEXPAND_OR)) {};
#if ENABLE_CHECKING /*fromline 1617*/
        MeltFrame_meltrout_105_WARMELTmiMACRO_PATEXPAND_OR(const char*fil, int lin) //the constructor fromline 1619
            : Melt_CallFrameWithValues<29> (fil,lin, sizeof(MeltFrame_meltrout_105_WARMELTmiMACRO_PATEXPAND_OR)) {};
        MeltFrame_meltrout_105_WARMELTmiMACRO_PATEXPAND_OR(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1624
            : Melt_CallFrameWithValues<29> (fil,lin, sizeof(MeltFrame_meltrout_105_WARMELTmiMACRO_PATEXPAND_OR), clos) {};
#endif /* ENABLE_CHECKING fromline 1629*/

    }; // end  class MeltFrame_meltrout_105_WARMELTmiMACRO_PATEXPAND_OR


    /** end of frame for meltrout_105_WARMELTmiMACRO_PATEXPAND_OR fromline 1660**/

    /* end of frame for routine meltrout_105_WARMELTmiMACRO_PATEXPAND_OR fromline 1719 */

    /* classy proc frame meltrout_105_WARMELTmiMACRO_PATEXPAND_OR */
    MeltFrame_meltrout_105_WARMELTmiMACRO_PATEXPAND_OR
    meltfram__ /*declfrastruct fromline 1743*/
    /*classyprocarg meltrout_105_WARMELTmiMACRO_PATEXPAND_OR fromline 1748*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1752*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1756*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("PATEXPAND_OR", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:6102:/ getarg");
    /*_.SEXPR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.PCTX__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.PCTX__V4*/ meltfptr[3])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:6103:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:6103:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L1*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:6103:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6103;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "patexpand_or sexpr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V6*/
                            meltfptr[5] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V5*/
                        meltfptr[4] = /*_.MELT_DEBUG_FUN__V6*/ meltfptr[5];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6103:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/
                        meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V6*/
                        meltfptr[5] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V5*/ meltfptr[4] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:6103:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V5*/
            meltfptr[4] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:6104:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:6104:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:6104:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check sexpr";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6104;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_ASSERT_FAILURE_FUN__V9*/
                            meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V8*/
                        meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6104:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V9*/
                        meltfptr[8] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V7*/
            meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:6104:/ clear");
            /*clear*/ /*_#IS_A__L3*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/
            meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:6105:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:6105:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:6105:/ cond.else");

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
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6105;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                            /*_.MELT_ASSERT_FAILURE_FUN__V12*/
                            meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V11*/
                        meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V12*/ meltfptr[11];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6105:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V12*/
                        meltfptr[11] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V10*/
            meltfptr[8] = /*_._IFELSE___V11*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:6105:/ clear");
            /*clear*/ /*_#IS_A__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V11*/
            meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V10*/ meltfptr[8] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:6106:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L5*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.PCTX__V4*/ meltfptr[3]), (melt_ptr_t)((/*!CLASS_PATTERN_EXPANSION_CONTEXT*/ meltfrout->tabval[4])));;
            MELT_LOCATION("warmelt-macro.melt:6106:/ cond");
            /*cond*/
            if (/*_#IS_A__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V14*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:6106:/ cond.else");

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
                            argtab[0].meltbp_cstring =  "check pctx";
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6106;
                            /*^apply.arg*/
                            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.PCTX__V4*/ meltfptr[3];
                            /*_.MELT_ASSERT_FAILURE_FUN__V15*/
                            meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_ASSERT_FAILURE_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_CSTRING MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IFELSE___V14*/
                        meltfptr[4] = /*_.MELT_ASSERT_FAILURE_FUN__V15*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6106:/ clear");
                        /*clear*/ /*_.MELT_ASSERT_FAILURE_FUN__V15*/
                        meltfptr[14] = 0 ;
                    }
                    ;
                }
            ;
            /*_.IFCPP___V13*/
            meltfptr[11] = /*_._IFELSE___V14*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:6106:/ clear");
            /*clear*/ /*_#IS_A__L5*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V14*/
            meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V13*/ meltfptr[11] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:6107:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:6108:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V17*/
            meltfptr[4] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:6109:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V18*/
            meltfptr[17] = slot;
        };
        ;
        /*_.LIST_FIRST__V19*/
        meltfptr[18] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V17*/ meltfptr[4])));;
        /*^compute*/
        /*_.CURPAIR__V20*/
        meltfptr[19] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V19*/ meltfptr[18])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:6111:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.PCTX__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.LOC__V18*/ meltfptr[17];
            /*_.ARGSP__V21*/
            meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!PATTERNEXPAND_PAIRLIST_AS_TUPLE*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:6112:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_PATTERN_OR*/ meltfrout->tabval[6])), (4), "CLASS_SOURCE_PATTERN_OR");
            /*_.INST__V23*/
            meltfptr[22] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V23*/ meltfptr[22])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V23*/ meltfptr[22]), (1), (/*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @ORPAT_DISJ", melt_magic_discr((melt_ptr_t)(/*_.INST__V23*/ meltfptr[22])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V23*/ meltfptr[22]), (3), (/*_.ARGSP__V21*/ meltfptr[20]), "ORPAT_DISJ");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V23*/ meltfptr[22], "newly made instance");
        ;
        /*_.RES__V22*/
        meltfptr[21] = /*_.INST__V23*/ meltfptr[22];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:6116:/ quasiblock");


        /*^multiapply*/
        /*multiapply 1args, 3x.res*/
        {

            union meltparam_un restab[3];
            memset(&restab, 0, sizeof(restab));
            /*^multiapply.xres*/
            restab[0].meltbp_longptr =  & /*_#IMAX__L6*/ meltfnum[0];
            /*^multiapply.xres*/
            restab[1].meltbp_longptr =  & /*_#IMIN__L7*/ meltfnum[1];
            /*^multiapply.xres*/
            restab[2].meltbp_longptr =  & /*_#ISUM__L8*/ meltfnum[7];
            /*^multiapply.appl*/
            /*_.SUBPATW__V25*/
            meltfptr[24] =  melt_apply ((meltclosure_ptr_t)((/*!PATTERN_WEIGHT_TUPLE*/ meltfrout->tabval[7])), (melt_ptr_t)(/*_.ARGSP__V21*/ meltfptr[20]), (""), (union meltparam_un*)0, (MELTBPARSTR_LONG MELTBPARSTR_LONG MELTBPARSTR_LONG ""), restab);
        }
        ;
        /*^quasiblock*/



        MELT_CHECK_SIGNAL();
        ;
        /*_#plI__L9*/
        meltfnum[8] =
            ((1) + (/*_#IMIN__L7*/ meltfnum[1]));;
        /*^compute*/
        /*_.MAKE_INTEGERBOX__V26*/
        meltfptr[25] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_CONSTANT_INTEGER*/ meltfrout->tabval[8])), (/*_#plI__L9*/ meltfnum[8])));;
        MELT_LOCATION("warmelt-macro.melt:6119:/ quasiblock");


        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @PAT_WEIGHT", melt_magic_discr((melt_ptr_t)(/*_.RES__V22*/ meltfptr[21])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.RES__V22*/ meltfptr[21]), (2), (/*_.MAKE_INTEGERBOX__V26*/ meltfptr[25]), "PAT_WEIGHT");
        ;
        /*^touch*/
        meltgc_touch(/*_.RES__V22*/ meltfptr[21]);
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.RES__V22*/ meltfptr[21], "put-fields");
        ;


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:6122:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L10*/
            meltfnum[9] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:6122:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L10*/ meltfnum[9]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:6122:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[10];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 6122;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "patexpand_or res";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RES__V22*/ meltfptr[21];
                            /*_.MELT_DEBUG_FUN__V28*/
                            meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V27*/
                        meltfptr[26] = /*_.MELT_DEBUG_FUN__V28*/ meltfptr[27];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:6122:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L11*/
                        meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V28*/
                        meltfptr[27] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V27*/ meltfptr[26] =  /*reallynil*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:6122:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L10*/
            meltfnum[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V27*/
            meltfptr[26] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:6123:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.RES__V22*/ meltfptr[21];;

        {
            MELT_LOCATION("warmelt-macro.melt:6123:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;

        MELT_CHECK_SIGNAL();
        ;
        /*_.MULTI___V24*/
        meltfptr[23] = /*_.RETURN___V29*/ meltfptr[27];;

        MELT_LOCATION("warmelt-macro.melt:6116:/ clear");
        /*clear*/ /*_#plI__L9*/
        meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_INTEGERBOX__V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V29*/
        meltfptr[27] = 0 ;

        /*^clear*/
        /*clear*/ /*_#IMAX__L6*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IMIN__L7*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_#ISUM__L8*/
        meltfnum[7] = 0 ;
        /*_.LET___V16*/
        meltfptr[14] = /*_.MULTI___V24*/ meltfptr[23];;

        MELT_LOCATION("warmelt-macro.melt:6107:/ clear");
        /*clear*/ /*_.CONT__V17*/
        meltfptr[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V20*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.ARGSP__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RES__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MULTI___V24*/
        meltfptr[23] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:6102:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-macro.melt:6102:/ locexp");
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
        /*finalret*/
        goto meltlabend_rout ;
        /*epilog*/

        /*^clear*/
        /*clear*/ /*_.IFCPP___V7*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V10*/
        meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V13*/
        meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V16*/
        meltfptr[14] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("PATEXPAND_OR", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_105_WARMELTmiMACRO_PATEXPAND_OR_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_105_WARMELTmiMACRO_PATEXPAND_OR*/



/**** end of warmelt-macro+04.cc ****/
