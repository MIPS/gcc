/* GCC MELT GENERATED C++ FILE warmelt-macro+03.cc - DO NOT EDIT - see http://gcc-melt.org/ */
/* secondary MELT generated C++ file of rank #3 */
#include "melt-run.h"


/* used hash from melt-run.h when compiling this file: */
MELT_EXTERN const char meltrun_used_md5_melt_f3[] = MELT_RUN_HASHMD5 /* from melt-run.h */;


/**** warmelt-macro+03.cc declarations ****/
/**** MELT GENERATED DECLARATIONS for warmelt-macro ** DO NOT EDIT ; see gcc-melt.org ****/

/****++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
***
    Copyright 2008 - 2013 Free Software Foundation, Inc.
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



class Melt_InitialClassyFrame_WARMELTmiMACRO_h163418781; // forward declaration fromline 6518
typedef Melt_InitialClassyFrame_WARMELTmiMACRO_h163418781 Melt_InitialFrame;
/*** 1 MELT module variables declarations ****/
MELT_EXTERN void melt_forwarding_module_data (void);
MELT_EXTERN void melt_marking_module_data (void);

MELT_EXTERN melt_ptr_t MELT_MODULE_VISIBILITY melt_WARMELTmiMACRO_module_var_ptr_tab[16];
MELT_EXTERN bool MELT_MODULE_VISIBILITY melt_WARMELTmiMACRO_module_var_flags[1];

static inline melt_ptr_t
melt_module_var_fetch (int ix)
{
    return (ix > 0  &&  ix <= 1)?melt_WARMELTmiMACRO_module_var_ptr_tab[ix]:NULL;
}

static inline void
melt_module_var_put (int ix, melt_ptr_t val)
{
    if (ix > 0  &&  ix <= 1)
        {
            melt_WARMELTmiMACRO_module_var_ptr_tab[ix]= val;
            melt_WARMELTmiMACRO_module_var_flags[ix/16] = true;
        }
}

/* MELT module variables indexes */
enum
{
    meltmodatix_none,
    meltmodvarix_MELT_TRANSRUNMACRO_CLOS_symb = 1,
    meltmodatix_last
}; /* end MELT module variables indexes */

/*** 5 MELT called hook declarations ***/

/*declare MELT called hook #0 HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_FRESH_ENVIRONMENT_REFERENCE_MAKER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_PREVENV, const char* meltinp1_MODULNAME);

/*declare MELT called hook #1 HOOK_MACRO_EXPORTER **/
MELT_EXTERN void melthook_HOOK_MACRO_EXPORTER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_SYM, melt_ptr_t meltinp1_VAL, melt_ptr_t meltinp2_CONTENV);

/*declare MELT called hook #2 HOOK_PATMACRO_EXPORTER **/
MELT_EXTERN void melthook_HOOK_PATMACRO_EXPORTER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_SYM, melt_ptr_t meltinp1_MACVAL, melt_ptr_t meltinp2_PATVAL, melt_ptr_t meltinp3_CONTENV);

/*declare MELT called hook #3 HOOK_SYMBOL_IMPORTER **/
MELT_EXTERN melt_ptr_t melthook_HOOK_SYMBOL_IMPORTER (melt_ptr_t melthookdatap, const char* meltinp0_SYMNAMESTR, const char* meltinp1_MODULENAMESTR, melt_ptr_t meltinp2_PARENV);

/*declare MELT called hook #4 HOOK_VALUE_EXPORTER **/
MELT_EXTERN void melthook_HOOK_VALUE_EXPORTER (melt_ptr_t melthookdatap, melt_ptr_t meltinp0_SYM, melt_ptr_t meltinp1_VAL, melt_ptr_t meltinp2_CONTENV);

/*** end of 5 MELT called hook declarations ***/

/*** no extra MELT c-headers ***/




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_1_WARMELTmiMACRO_S_EXPR_WEIGHT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_2_WARMELTmiMACRO_LAMBDA_cl1(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_3_WARMELTmiMACRO_EXPAND_RESTLIST_AS_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_4_WARMELTmiMACRO_EXPAND_RESTLIST_AS_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_5_WARMELTmiMACRO_EXPAND_PAIRLIST_AS_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_6_WARMELTmiMACRO_EXPAND_PAIRLIST_AS_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_7_WARMELTmiMACRO_REGISTER_GENERATOR_DEVICE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_8_WARMELTmiMACRO_EXPAND_APPLY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_9_WARMELTmiMACRO_EXPAND_MSEND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_10_WARMELTmiMACRO_EXPAND_FIELDEXPR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_11_WARMELTmiMACRO_EXPAND_CITERATION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_12_WARMELTmiMACRO_EXPAND_CMATCHEXPR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_13_WARMELTmiMACRO_EXPAND_FUNMATCHEXPR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_14_WARMELTmiMACRO_EXPAND_KEYWORDFUN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_15_WARMELTmiMACRO_MACROEXPAND_1(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_16_WARMELTmiMACRO_LAMBDA_cl2(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_17_WARMELTmiMACRO_EXPAND_PRIMITIVE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_18_WARMELTmiMACRO_EXPAND_HOOK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_19_WARMELTmiMACRO_PATTERNEXPAND_PAIRLIST_AS_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_20_WARMELTmiMACRO_LAMBDA_cl3(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_21_WARMELTmiMACRO_PATMACEXPAND_FOR_MATCHER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_22_WARMELTmiMACRO_LAMBDA_cl4(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_23_WARMELTmiMACRO_LAMBDA_cl5(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_24_WARMELTmiMACRO_PATTERN_WEIGHT_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_25_WARMELTmiMACRO_LAMBDA_cl6(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_26_WARMELTmiMACRO_PATTERNEXPAND_EXPR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_27_WARMELTmiMACRO_PATTERNEXPAND_1(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_28_WARMELTmiMACRO_MACROEXPAND_TOPLEVEL_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_29_WARMELTmiMACRO_LAMBDA_ARG_BINDINGS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_30_WARMELTmiMACRO_INSTALL_INITIAL_MACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_31_WARMELTmiMACRO_INSTALL_INITIAL_PATMACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_32_WARMELTmiMACRO_WARN_IF_REDEFINED(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_33_WARMELTmiMACRO_FLATTEN_FOR_C_CODE_EXPANSION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_34_WARMELTmiMACRO_PARSE_PAIRLIST_C_CODE_EXPANSION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_35_WARMELTmiMACRO_CHECK_C_EXPANSION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_36_WARMELTmiMACRO_MELT_REGISTER_TRANSLATOR_RUNNER_MACROEXPANSIONS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_37_WARMELTmiMACRO_MELT_INVOKE_TRANSLATOR_RUNNER_MACROEXPANSIONS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_38_WARMELTmiMACRO_MEXPAND_DEFPRIMITIVE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_39_WARMELTmiMACRO_MEXPAND_DEFCITERATOR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_40_WARMELTmiMACRO_MEXPAND_DEFCMATCHER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_41_WARMELTmiMACRO_LAMBDA_cl7(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_42_WARMELTmiMACRO_MEXPAND_DEFUNMATCHER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_43_WARMELTmiMACRO_LAMBDA_cl8(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_44_WARMELTmiMACRO_MEXPAND_DEFUN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_45_WARMELTmiMACRO_MELT_DELAYED_MACRO_EXPANDER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_46_WARMELTmiMACRO_MEXPAND_DEFMACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_47_WARMELTmiMACRO_LAMBDA_cl9(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_MACRO_INSTALLER**/

/** declaration of hook melthook_HOOK_MACRO_INSTALLER */
MELT_EXTERN
void melthook_HOOK_MACRO_INSTALLER(melt_ptr_t melthookdatap,
                                   melt_ptr_t meltinp0_MBIND,
                                   melt_ptr_t meltinp1_MEXPCLOS)
;



/** end of declaration for hook melthook_HOOK_MACRO_INSTALLER**/






melt_ptr_t MELT_MODULE_VISIBILITY meltrout_49_WARMELTmiMACRO_MEXPAND_DEFVAR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_50_WARMELTmiMACRO_MEXPAND_DEFHOOK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_51_WARMELTmiMACRO_MEXPAND_DEFINE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_52_WARMELTmiMACRO_LAMBDA_cl10(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_53_WARMELTmiMACRO_SCAN_DEFCLASS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_54_WARMELTmiMACRO_MEXPAND_DEFCLASS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_55_WARMELTmiMACRO_LAMBDA_cl11(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_56_WARMELTmiMACRO_LAMBDA_cl12(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_57_WARMELTmiMACRO_PARSE_FIELD_ASSIGNMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_58_WARMELTmiMACRO_LAMBDA_cl13(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_59_WARMELTmiMACRO_MEXPAND_DEFINSTANCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_60_WARMELTmiMACRO_MEXPAND_DEFSELECTOR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_61_WARMELTmiMACRO_MEXPAND_INSTANCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_64_WARMELTmiMACRO_LAMBDA_cl14(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_66_WARMELTmiMACRO_LAMBDA_cl15(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_68_WARMELTmiMACRO_LAMBDA_cl16(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_71_WARMELTmiMACRO_LAMBDA_cl17(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_79_WARMELTmiMACRO_LAMBDA_cl18(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_81_WARMELTmiMACRO_MEXPAND_IF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_87_WARMELTmiMACRO_MEXPAND_COND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_88_WARMELTmiMACRO_LAMBDA_cl19(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_89_WARMELTmiMACRO_MEXPAND_AND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_90_WARMELTmiMACRO_LAMBDA_cl20(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_91_WARMELTmiMACRO_PATEXPAND_AS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_92_WARMELTmiMACRO_MEXPAND_AS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_93_WARMELTmiMACRO_PATEXPAND_WHEN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_94_WARMELTmiMACRO_PATEXPAND_AND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_95_WARMELTmiMACRO_MEXPAND_OR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_96_WARMELTmiMACRO_LAMBDA_cl21(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_97_WARMELTmiMACRO_PATEXPAND_OR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_98_WARMELTmiMACRO_MEXPAND_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_99_WARMELTmiMACRO_PATEXPAND_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_100_WARMELTmiMACRO_MEXPANDOBSOLETE_CONTAINER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_101_WARMELTmiMACRO_PATEXPANDOBSOLETE_CONTAINER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_102_WARMELTmiMACRO_MEXPAND_DEREF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_103_WARMELTmiMACRO_MEXPANDOBSOLETE_CONTENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_104_WARMELTmiMACRO_MEXPAND_SET_REF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_105_WARMELTmiMACRO_MEXPAND_PLUS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_106_WARMELTmiMACRO_MEXPAND_MINUS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_107_WARMELTmiMACRO_MEXPAND_TIMES(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_108_WARMELTmiMACRO_MEXPAND_DIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_109_WARMELTmiMACRO_MEXPAND_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_110_WARMELTmiMACRO_PATEXPAND_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_111_WARMELTmiMACRO_MEXPAND_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_112_WARMELTmiMACRO_PATEXPAND_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_113_WARMELTmiMACRO_MEXPAND_MATCH(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_114_WARMELTmiMACRO_LAMBDA_cl22(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_115_WARMELTmiMACRO_MEXPAND_MATCHALT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_116_WARMELTmiMACRO_LAMBDA_cl23(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_117_WARMELTmiMACRO_MEXPAND_LETBINDING(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_118_WARMELTmiMACRO_MEXPAND_LET(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_119_WARMELTmiMACRO_LAMBDA_cl24(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_120_WARMELTmiMACRO_LAMBDA_cl25(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_121_WARMELTmiMACRO_LAMBDA_cl26(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_122_WARMELTmiMACRO_YES_RECURSIVELY_CONSTRUCTIBLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_123_WARMELTmiMACRO_MEXPAND_LETREC(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_124_WARMELTmiMACRO_LAMBDA_cl27(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_125_WARMELTmiMACRO_LAMBDA_cl28(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_126_WARMELTmiMACRO_MEXPAND_LAMBDA(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_127_WARMELTmiMACRO_LAMBDA_cl29(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_128_WARMELTmiMACRO_MEXPAND_VARIADIC(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_129_WARMELTmiMACRO_LAMBDA_cl30(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_130_WARMELTmiMACRO_LAMBDA_cl31(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_131_WARMELTmiMACRO_LAMBDA_cl32(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_132_WARMELTmiMACRO_LAMBDA_cl33(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_133_WARMELTmiMACRO_MEXPAND_MULTICALL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_134_WARMELTmiMACRO_LAMBDA_cl34(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_135_WARMELTmiMACRO_LAMBDA_cl35(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_136_WARMELTmiMACRO_MEXPAND_QUOTE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_137_WARMELTmiMACRO_MEXPAND_BOX(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_138_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_139_WARMELTmiMACRO_MEXPAND_UNBOX(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_140_WARMELTmiMACRO_MEXPAND_COMMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_141_WARMELTmiMACRO_MEXPAND_CHEADER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_142_WARMELTmiMACRO_MEXPAND_CIMPLEMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_143_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_144_WARMELTmiMACRO_PAIRLIST_TO_RETURN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_145_WARMELTmiMACRO_LAMBDA_cl36(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_146_WARMELTmiMACRO_MEXPAND_PROGN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_147_WARMELTmiMACRO_MEXPAND_RETURN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_148_WARMELTmiMACRO_MEXPAND_FOREVER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_149_WARMELTmiMACRO_LAMBDA_cl37(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_150_WARMELTmiMACRO_MEXPAND_EXIT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_151_WARMELTmiMACRO_LAMBDA_cl38(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_152_WARMELTmiMACRO_MEXPAND_AGAIN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_153_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_154_WARMELTmiMACRO_MEXPAND_ASSERT_MSG(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_155_WARMELTmiMACRO_MEXPAND_THIS_LINE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_156_WARMELTmiMACRO_MEXPAND_THIS_FILE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_157_WARMELTmiMACRO_MEXPAND_DEBUG(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_158_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_159_WARMELTmiMACRO_LAMBDA_cl39(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_160_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_161_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_162_WARMELTmiMACRO_MEXPAND_EXPORT_CLASS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_163_WARMELTmiMACRO_LAMBDA_cl40(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_SYNONYM(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_165_WARMELTmiMACRO_MEXPAND_CURRENT_MODULE_ENVIRONMENT_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_166_WARMELTmiMACRO_MEXPAND_PARENT_MODULE_ENVIRONMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_167_WARMELTmiMACRO_MEXPAND_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_168_WARMELTmiMACRO_MEXPAND_FETCH_PREDEFINED(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_169_WARMELTmiMACRO_MEXPAND_STORE_PREDEFINED(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);


MELT_EXTERN void* melt_start_this_module (void*); /*mandatory start of module*/


/*declare opaque initial frame: */

typedef Melt_InitialClassyFrame_WARMELTmiMACRO_h163418781 /*opaqueinitialclassy*/ meltinitial_frame_t;


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


/**** warmelt-macro+03.cc implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD fromline 1716 */

    /** start of frame for meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD// fromline 1531
        : public Melt_CallFrameWithValues<42>
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
        MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<42> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD), clos) {};
        MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD() //the constructor fromline 1606
            : Melt_CallFrameWithValues<42> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<42> (fil,lin, sizeof(MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD)) {};
        MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<42> (fil,lin, sizeof(MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD


    /** end of frame for meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD fromline 1661**/

    /* end of frame for routine meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD fromline 1720 */

    /* classy proc frame meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD */
    MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_LOAD", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:4243:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4244:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4244:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4244:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4244;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_load start sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4244:/ clear");
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

                    /*_._IF___V6*/ meltfptr[5] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4244:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4245:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4245:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4245:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4245)?(4245):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V9*/
                        meltfptr[5] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4245:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4246:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:4246:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[10] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4246:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4246)?(4246):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V11*/
                        meltfptr[10] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V10*/
            meltfptr[5] = /*_._IFELSE___V11*/ meltfptr[10];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4246:/ clear");
            /*clear*/ /*_#IS_A__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V11*/
            meltfptr[10] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V10*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4247:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L5*/
            meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:4247:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V13*/ meltfptr[12] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4247:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check modctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4247)?(4247):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V13*/
                        meltfptr[12] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V12*/
            meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[12];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4247:/ clear");
            /*clear*/ /*_#IS_OBJECT__L5*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V13*/
            meltfptr[12] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V12*/ meltfptr[10] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L6*/
        meltfnum[0] =
            ((/*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:4248:/ cond");
        /*cond*/
        if (/*_#NULL__L6*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V15*/ meltfptr[14] = (/*!MACROEXPAND_1*/ meltfrout->tabval[3]);;
                    /*_._IF___V14*/
                    meltfptr[12] = /*_.SETQ___V15*/ meltfptr[14];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4248:/ clear");
                    /*clear*/ /*_.SETQ___V15*/
                    meltfptr[14] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V14*/ meltfptr[12] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:4249:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V17*/
            meltfptr[16] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4250:/ getslot");
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
        /*_.FILNAM__V21*/
        meltfptr[20] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V22*/
        meltfptr[21] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
        MELT_LOCATION("warmelt-macro.melt:4254:/ cond");
        /*cond*/
        if (/*_.PAIR_TAIL__V22*/ meltfptr[21]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4255:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "LOAD expects one argument"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_A__L7*/
        meltfnum[1] =
            melt_is_instance_of((melt_ptr_t)(/*_.FILNAM__V21*/ meltfptr[20]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
        MELT_LOCATION("warmelt-macro.melt:4256:/ cond");
        /*cond*/
        if (/*_#IS_A__L7*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4257:/ apply");
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
                        /*_.MEXPANDER__V24*/
                        meltfptr[23] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.FILNAM__V21*/ meltfptr[20]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*^compute*/
                    /*_.FILNAM__V21*/
                    meltfptr[20] = /*_.SETQ___V25*/ meltfptr[24] = /*_.MEXPANDER__V24*/ meltfptr[23];;
                    /*_._IF___V23*/
                    meltfptr[22] = /*_.SETQ___V25*/ meltfptr[24];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4256:/ clear");
                    /*clear*/ /*_.MEXPANDER__V24*/
                    meltfptr[23] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V25*/
                    meltfptr[24] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V23*/ meltfptr[22] =  /*fromline 1341*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_A__L8*/
        meltfnum[7] =
            melt_is_instance_of((melt_ptr_t)(/*_.FILNAM__V21*/ meltfptr[20]), (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[4])));;
        MELT_LOCATION("warmelt-macro.melt:4258:/ cond");
        /*cond*/
        if (/*_#IS_A__L8*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:4259:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.FILNAM__V21*/ meltfptr[20]) /*=obj*/;
                        melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                        /*_.NAMED_NAME__V27*/
                        meltfptr[24] = slot;
                    };
                    ;
                    /*^compute*/
                    /*_.FILNAM__V21*/
                    meltfptr[20] = /*_.SETQ___V28*/ meltfptr[27] = /*_.NAMED_NAME__V27*/ meltfptr[24];;
                    /*_._IF___V26*/
                    meltfptr[23] = /*_.SETQ___V28*/ meltfptr[27];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4258:/ clear");
                    /*clear*/ /*_.NAMED_NAME__V27*/
                    meltfptr[24] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V28*/
                    meltfptr[27] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V26*/ meltfptr[23] =  /*fromline 1341*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_STRING__L9*/
        meltfnum[8] =
            (melt_magic_discr((melt_ptr_t)(/*_.FILNAM__V21*/ meltfptr[20])) == MELTOBMAG_STRING);;
        MELT_LOCATION("warmelt-macro.melt:4261:/ cond");
        /*cond*/
        if (/*_#IS_STRING__L9*/ meltfnum[8]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4262:/ locexp");
                        melt_inform_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "MELT loading file "), (melt_ptr_t)(/*_.FILNAM__V21*/ meltfptr[20]));
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4263:/ quasiblock");


                    /*_.CUREAD__V31*/
                    meltfptr[30] =
                        (meltgc_read_file (melt_string_str((melt_ptr_t)(/*_.FILNAM__V21*/ meltfptr[20])), (char*)0));;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:4264:/ cppif.then");
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
                        MELT_LOCATION("warmelt-macro.melt:4264:/ cond");
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
                                    MELT_LOCATION("warmelt-macro.melt:4264:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[10];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 4264;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "mexpand_load curead";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CUREAD__V31*/ meltfptr[30];
                                        /*_.MELT_DEBUG_FUN__V33*/
                                        meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V32*/
                                    meltfptr[31] = /*_.MELT_DEBUG_FUN__V33*/ meltfptr[32];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:4264:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L11*/
                                    meltfnum[10] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V33*/
                                    meltfptr[32] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V32*/ meltfptr[31] =  /*fromline 1341*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-macro.melt:4264:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:4265:/ quasiblock");



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
                        /*_.EXPLIST__V35*/
                        meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!MACROEXPAND_TOPLEVEL_LIST*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.CUREAD__V31*/ meltfptr[30]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4266:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[7]);
                        /*_.EXPTUPLE__V36*/
                        meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.EXPLIST__V35*/ meltfptr[31]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:4268:/ cppif.then");
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
                        meltfnum[10] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-macro.melt:4268:/ cond");
                        /*cond*/
                        if (/*_#MELT_NEED_DBG__L12*/ meltfnum[10]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L13*/ meltfnum[9] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-macro.melt:4268:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L13*/ meltfnum[9];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 4268;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "mexpand_load return exptuple";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.EXPTUPLE__V36*/ meltfptr[35];
                                        /*_.MELT_DEBUG_FUN__V38*/
                                        meltfptr[37] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V37*/
                                    meltfptr[36] = /*_.MELT_DEBUG_FUN__V38*/ meltfptr[37];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:4268:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L13*/
                                    meltfnum[9] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V38*/
                                    meltfptr[37] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V37*/ meltfptr[36] =  /*fromline 1341*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-macro.melt:4268:/ locexp");
                            /*void*/
                            (void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L12*/
                        meltfnum[10] = 0 ;
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
                    MELT_LOCATION("warmelt-macro.melt:4269:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = /*_.EXPTUPLE__V36*/ meltfptr[35];;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4269:/ locexp");
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
                    meltfptr[32] = /*_.RETURN___V39*/ meltfptr[37];;

                    MELT_LOCATION("warmelt-macro.melt:4265:/ clear");
                    /*clear*/ /*_.EXPLIST__V35*/
                    meltfptr[31] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.EXPTUPLE__V36*/
                    meltfptr[35] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V39*/
                    meltfptr[37] = 0 ;
                    /*_.LET___V30*/
                    meltfptr[27] = /*_.LET___V34*/ meltfptr[32];;

                    MELT_LOCATION("warmelt-macro.melt:4263:/ clear");
                    /*clear*/ /*_.CUREAD__V31*/
                    meltfptr[30] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.LET___V34*/
                    meltfptr[32] = 0 ;
                    MELT_LOCATION("warmelt-macro.melt:4261:/ quasiblock");


                    /*_.PROGN___V40*/
                    meltfptr[36] = /*_.LET___V30*/ meltfptr[27];;
                    /*^compute*/
                    /*_._IFELSE___V29*/
                    meltfptr[24] = /*_.PROGN___V40*/ meltfptr[36];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4261:/ clear");
                    /*clear*/ /*_.LET___V30*/
                    meltfptr[27] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V40*/
                    meltfptr[36] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:4274:/ cppif.then");
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
                        meltfnum[9] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-macro.melt:4274:/ cond");
                        /*cond*/
                        if (/*_#MELT_NEED_DBG__L14*/ meltfnum[9]) /*then*/
                            {
                                /*^cond.then*/
                                /*^block*/
                                /*anyblock*/
                                {

                                    /*_#MELT_CALLCOUNT__L15*/ meltfnum[10] =
                                        /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                        meltcallcount  /* melt_callcount debugging */
#else
                                        0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                        ;;

                                    MELT_CHECK_SIGNAL();
                                    ;
                                    MELT_LOCATION("warmelt-macro.melt:4274:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L15*/ meltfnum[10];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 4274;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "mexpand_load strange filnam";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.FILNAM__V21*/ meltfptr[20];
                                        /*_.MELT_DEBUG_FUN__V42*/
                                        meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V41*/
                                    meltfptr[31] = /*_.MELT_DEBUG_FUN__V42*/ meltfptr[35];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:4274:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L15*/
                                    meltfnum[10] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V42*/
                                    meltfptr[35] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V41*/ meltfptr[31] =  /*fromline 1341*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-macro.melt:4274:/ locexp");
                            /*void*/
                            (void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L14*/
                        meltfnum[9] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V41*/
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

                    {
                        MELT_LOCATION("warmelt-macro.melt:4275:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "LOAD expects a string filename"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4273:/ quasiblock");


                    /*epilog*/
                }
                ;
            }
        ;
        /*_.LET___V16*/
        meltfptr[14] = /*_._IFELSE___V29*/ meltfptr[24];;

        MELT_LOCATION("warmelt-macro.melt:4249:/ clear");
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
        /*clear*/ /*_.FILNAM__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L7*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L8*/
        meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V26*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_STRING__L9*/
        meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V29*/
        meltfptr[24] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4243:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-macro.melt:4243:/ locexp");
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
        /*clear*/ /*_.IFCPP___V10*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V12*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L6*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V16*/
        meltfptr[14] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_LOAD", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_62_WARMELTmiMACRO_MEXPAND_LOAD*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN fromline 1716 */

    /** start of frame for meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN// fromline 1531
        : public Melt_CallFrameWithValues<41>
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
        MeltFrame_meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<41> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN), clos) {};
        MeltFrame_meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN() //the constructor fromline 1606
            : Melt_CallFrameWithValues<41> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<41> (fil,lin, sizeof(MeltFrame_meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN)) {};
        MeltFrame_meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<41> (fil,lin, sizeof(MeltFrame_meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN


    /** end of frame for meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN fromline 1661**/

    /* end of frame for routine meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN fromline 1720 */

    /* classy proc frame meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN */
    MeltFrame_meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("PARSE_FIELD_PATTERN", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:4284:/ getarg");
    /*_.FKEYW__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.CLA__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.CLA__V3*/ meltfptr[2])) != NULL);


    /*getarg#2*/
    /*^getarg*/
    if (meltxargdescr_[1] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.FLPSEXP__V4*/
    meltfptr[3] = (meltxargtab_[1].meltbp_aptr) ? (*(meltxargtab_[1].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.FLPSEXP__V4*/ meltfptr[3])) != NULL);


    /*getarg#3*/
    /*^getarg*/
    if (meltxargdescr_[2] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.ENV__V5*/
    meltfptr[4] = (meltxargtab_[2].meltbp_aptr) ? (*(meltxargtab_[2].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.ENV__V5*/ meltfptr[4])) != NULL);


    /*getarg#4*/
    /*^getarg*/
    if (meltxargdescr_[3] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.PCTX__V6*/
    meltfptr[5] = (meltxargtab_[3].meltbp_aptr) ? (*(meltxargtab_[3].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.PCTX__V6*/ meltfptr[5])) != NULL);


    /*getarg#5*/
    /*^getarg*/
    if (meltxargdescr_[4] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.PSLOC__V7*/
    meltfptr[6] = (meltxargtab_[4].meltbp_aptr) ? (*(meltxargtab_[4].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.PSLOC__V7*/ meltfptr[6])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4285:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4285:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4285:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4285;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "parse_field_pattern keyw";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.FKEYW__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V9*/
                            meltfptr[8] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V8*/
                        meltfptr[7] = /*_.MELT_DEBUG_FUN__V9*/ meltfptr[8];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4285:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L2*/
                        meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V9*/
                        meltfptr[8] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V8*/ meltfptr[7] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4285:/ locexp");
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
            /*clear*/ /*_._IF___V8*/
            meltfptr[7] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:4286:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.FKEYW__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4286:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[7] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4286:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check fkeyw"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4286)?(4286):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V11*/
                        meltfptr[7] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V10*/
            meltfptr[8] = /*_._IFELSE___V11*/ meltfptr[7];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4286:/ clear");
            /*clear*/ /*_#IS_A__L3*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V11*/
            meltfptr[7] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V10*/ meltfptr[8] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4287:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V5*/ meltfptr[4]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:4287:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V13*/ meltfptr[12] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4287:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4287)?(4287):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V13*/
                        meltfptr[12] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V12*/
            meltfptr[7] = /*_._IFELSE___V13*/ meltfptr[12];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4287:/ clear");
            /*clear*/ /*_#IS_A__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V13*/
            meltfptr[12] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V12*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4288:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L5*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.PCTX__V6*/ meltfptr[5]), (melt_ptr_t)((/*!CLASS_PATTERN_EXPANSION_CONTEXT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:4288:/ cond");
            /*cond*/
            if (/*_#IS_A__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4288:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check pctc"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4288)?(4288):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V15*/
                        meltfptr[14] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V14*/
            meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4288:/ clear");
            /*clear*/ /*_#IS_A__L5*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[14] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4289:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L6*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.CLA__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_CLASS*/ meltfrout->tabval[4])));;
            MELT_LOCATION("warmelt-macro.melt:4289:/ cond");
            /*cond*/
            if (/*_#IS_A__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V17*/ meltfptr[16] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4289:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check cla"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4289)?(4289):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V17*/
                        meltfptr[16] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V16*/
            meltfptr[14] = /*_._IFELSE___V17*/ meltfptr[16];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4289:/ clear");
            /*clear*/ /*_#IS_A__L6*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V17*/
            meltfptr[16] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V16*/ meltfptr[14] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:4290:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:4291:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.CLA__V3*/ meltfptr[2]) /*=obj*/;
            melt_object_get_field(slot,obj, 6, "CLASS_FIELDS");
            /*_.CLAFIELDS__V19*/
            meltfptr[18] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4292:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[5])), (1), "CLASS_REFERENCE");
            /*_.INST__V21*/
            meltfptr[20] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @REFERENCED_VALUE", melt_magic_discr((melt_ptr_t)(/*_.INST__V21*/ meltfptr[20])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V21*/ meltfptr[20]), (0), ((/*nil*/NULL)), "REFERENCED_VALUE");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V21*/ meltfptr[20], "newly made instance");
        ;
        /*_.FLDCONT__V20*/
        meltfptr[19] = /*_.INST__V21*/ meltfptr[20];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4293:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V5*/ meltfptr[4];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.PCTX__V6*/ meltfptr[5];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.PSLOC__V7*/ meltfptr[6];
            /*_.FLPAT__V22*/
            meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!PATTERNEXPAND_1*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.FLPSEXP__V4*/ meltfptr[3]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4295:/ cppif.then");
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
            meltfnum[1] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:4295:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L7*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L8*/ meltfnum[0] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:4295:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4295;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "parse_field_pattern clafields";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CLAFIELDS__V19*/ meltfptr[18];
                            /*_.MELT_DEBUG_FUN__V24*/
                            meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V23*/
                        meltfptr[22] = /*_.MELT_DEBUG_FUN__V24*/ meltfptr[23];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4295:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L8*/
                        meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V24*/
                        meltfptr[23] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V23*/ meltfptr[22] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4295:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L7*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V23*/
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
        MELT_LOCATION("warmelt-macro.melt:4299:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V26*/
        meltfptr[22] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_10*/ meltfrout->tabval[10])), (2));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V26*/ meltfptr[22])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V26*/ meltfptr[22])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V26*/ meltfptr[22])->tabval[0] = (melt_ptr_t)(/*_.FKEYW__V2*/ meltfptr[1]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V26*/ meltfptr[22])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 1>= 0 && 1< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V26*/ meltfptr[22])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V26*/ meltfptr[22])->tabval[1] = (melt_ptr_t)(/*_.FLDCONT__V20*/ meltfptr[19]);
        ;
        /*_.LAMBDA___V25*/
        meltfptr[23] = /*_.LAMBDA___V26*/ meltfptr[22];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4297:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LAMBDA___V25*/ meltfptr[23];
            /*_.MULTIPLE_ITERATE_TEST__V27*/
            meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!MULTIPLE_ITERATE_TEST*/ meltfrout->tabval[7])), (melt_ptr_t)(/*_.CLAFIELDS__V19*/ meltfptr[18]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:4309:/ quasiblock");


        /*^cond*/
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.FLDCONT__V20*/ meltfptr[19]),
                                          (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[5])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.FLDCONT__V20*/ meltfptr[19]) /*=obj*/;
                    melt_object_get_field(slot,obj, 0, "REFERENCED_VALUE");
                    /*_.FLD__V29*/
                    meltfptr[28] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.FLD__V29*/ meltfptr[28] =  /*fromline 1341*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L9*/
        meltfnum[0] =
            !melt_is_instance_of((melt_ptr_t)(/*_.FLD__V29*/ meltfptr[28]), (melt_ptr_t)((/*!CLASS_FIELD*/ meltfrout->tabval[11])));;
        MELT_LOCATION("warmelt-macro.melt:4311:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L9*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:4313:/ cppif.then");
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
                        meltfnum[1] =
                            /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                            (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                            0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                            ;;
                        MELT_LOCATION("warmelt-macro.melt:4313:/ cond");
                        /*cond*/
                        if (/*_#MELT_NEED_DBG__L10*/ meltfnum[1]) /*then*/
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
                                    MELT_LOCATION("warmelt-macro.melt:4313:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[10];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 4313;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "parse_field_pattern bad fld";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.FLD__V29*/ meltfptr[28];
                                        /*_.MELT_DEBUG_FUN__V32*/
                                        meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V31*/
                                    meltfptr[30] = /*_.MELT_DEBUG_FUN__V32*/ meltfptr[31];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:4313:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L11*/
                                    meltfnum[10] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V32*/
                                    meltfptr[31] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V31*/ meltfptr[30] =  /*fromline 1341*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-macro.melt:4313:/ locexp");
                            /*void*/
                            (void)0;
                        }
                        ;
                        /*^quasiblock*/


                        /*epilog*/

                        /*^clear*/
                        /*clear*/ /*_#MELT_NEED_DBG__L10*/
                        meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_._IF___V31*/
                        meltfptr[30] = 0 ;
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
                    MELT_LOCATION("warmelt-macro.melt:4315:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.FKEYW__V2*/ meltfptr[1]) /*=obj*/;
                        melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                        /*_.NAMED_NAME__V33*/
                        meltfptr[31] = slot;
                    };
                    ;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4314:/ locexp");
                        melt_error_str((melt_ptr_t)(/*_.PSLOC__V7*/ meltfptr[6]), ( "invalid :field in pattern"), (melt_ptr_t)(/*_.NAMED_NAME__V33*/ meltfptr[31]));
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4316:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4316:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:4312:/ quasiblock");


                    /*_.PROGN___V35*/
                    meltfptr[34] = /*_.RETURN___V34*/ meltfptr[30];;
                    /*^compute*/
                    /*_._IF___V30*/
                    meltfptr[29] = /*_.PROGN___V35*/ meltfptr[34];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4311:/ clear");
                    /*clear*/ /*_.NAMED_NAME__V33*/
                    meltfptr[31] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V34*/
                    meltfptr[30] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V35*/
                    meltfptr[34] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V30*/ meltfptr[29] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:4317:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4318:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_FIELD_PATTERN*/ meltfrout->tabval[12])), (4), "CLASS_SOURCE_FIELD_PATTERN");
            /*_.INST__V38*/
            meltfptr[34] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V38*/ meltfptr[34])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V38*/ meltfptr[34]), (1), (/*_.PSLOC__V7*/ meltfptr[6]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SPAF_FIELD", melt_magic_discr((melt_ptr_t)(/*_.INST__V38*/ meltfptr[34])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V38*/ meltfptr[34]), (2), (/*_.FLD__V29*/ meltfptr[28]), "SPAF_FIELD");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SPAF_PATTERN", melt_magic_discr((melt_ptr_t)(/*_.INST__V38*/ meltfptr[34])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V38*/ meltfptr[34]), (3), (/*_.FLPAT__V22*/ meltfptr[21]), "SPAF_PATTERN");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V38*/ meltfptr[34], "newly made instance");
        ;
        /*_.PATF__V37*/
        meltfptr[30] = /*_.INST__V38*/ meltfptr[34];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4324:/ cppif.then");
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
            meltfnum[10] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:4324:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L12*/ meltfnum[10]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L13*/ meltfnum[1] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:4324:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L13*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4324;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "parse_field_pattern return patf";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.PATF__V37*/ meltfptr[30];
                            /*_.MELT_DEBUG_FUN__V40*/
                            meltfptr[39] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V39*/
                        meltfptr[38] = /*_.MELT_DEBUG_FUN__V40*/ meltfptr[39];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4324:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L13*/
                        meltfnum[1] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V40*/
                        meltfptr[39] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V39*/ meltfptr[38] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4324:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L12*/
            meltfnum[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V39*/
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

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4325:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.PATF__V37*/ meltfptr[30];;

        {
            MELT_LOCATION("warmelt-macro.melt:4325:/ locexp");
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
        /*_.LET___V36*/
        meltfptr[31] = /*_.RETURN___V41*/ meltfptr[39];;

        MELT_LOCATION("warmelt-macro.melt:4317:/ clear");
        /*clear*/ /*_.PATF__V37*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V41*/
        meltfptr[39] = 0 ;
        /*_.LET___V28*/
        meltfptr[27] = /*_.LET___V36*/ meltfptr[31];;

        MELT_LOCATION("warmelt-macro.melt:4309:/ clear");
        /*clear*/ /*_.FLD__V29*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L9*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V30*/
        meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V36*/
        meltfptr[31] = 0 ;
        /*_.LET___V18*/
        meltfptr[16] = /*_.LET___V28*/ meltfptr[27];;

        MELT_LOCATION("warmelt-macro.melt:4290:/ clear");
        /*clear*/ /*_.CLAFIELDS__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FLDCONT__V20*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FLPAT__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LAMBDA___V25*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MULTIPLE_ITERATE_TEST__V27*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V28*/
        meltfptr[27] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4284:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V18*/ meltfptr[16];;

        {
            MELT_LOCATION("warmelt-macro.melt:4284:/ locexp");
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
        /*clear*/ /*_.IFCPP___V10*/
        meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V12*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V16*/
        meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V18*/
        meltfptr[16] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("PARSE_FIELD_PATTERN", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_63_WARMELTmiMACRO_PARSE_FIELD_PATTERN*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_64_WARMELTmiMACRO_LAMBDA_cl14(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_64_WARMELTmiMACRO_LAMBDA_cl14_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_64_WARMELTmiMACRO_LAMBDA_cl14 fromline 1716 */

    /** start of frame for meltrout_64_WARMELTmiMACRO_LAMBDA_cl14 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_64_WARMELTmiMACRO_LAMBDA_cl14// fromline 1531
        : public Melt_CallFrameWithValues<10>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_64_WARMELTmiMACRO_LAMBDA_cl14(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<10> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_64_WARMELTmiMACRO_LAMBDA_cl14), clos) {};
        MeltFrame_meltrout_64_WARMELTmiMACRO_LAMBDA_cl14() //the constructor fromline 1606
            : Melt_CallFrameWithValues<10> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_64_WARMELTmiMACRO_LAMBDA_cl14)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_64_WARMELTmiMACRO_LAMBDA_cl14(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_64_WARMELTmiMACRO_LAMBDA_cl14)) {};
        MeltFrame_meltrout_64_WARMELTmiMACRO_LAMBDA_cl14(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_64_WARMELTmiMACRO_LAMBDA_cl14), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_64_WARMELTmiMACRO_LAMBDA_cl14


    /** end of frame for meltrout_64_WARMELTmiMACRO_LAMBDA_cl14 fromline 1661**/

    /* end of frame for routine meltrout_64_WARMELTmiMACRO_LAMBDA_cl14 fromline 1720 */

    /* classy proc frame meltrout_64_WARMELTmiMACRO_LAMBDA_cl14 */
    MeltFrame_meltrout_64_WARMELTmiMACRO_LAMBDA_cl14
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_64_WARMELTmiMACRO_LAMBDA_cl14 fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
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
    MELT_LOCATION("warmelt-macro.melt:4299:/ getarg");
    /*_.CFLD__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#IX__L1*/
    meltfnum[0] = meltxargtab_[0].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4300:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.CFLD__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_FIELD*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4300:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V4*/ meltfptr[3] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4300:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check fld"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4300)?(4300):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V4*/
                        meltfptr[3] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V3*/
            meltfptr[2] = /*_._IFELSE___V4*/ meltfptr[3];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4300:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[1] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V4*/
            meltfptr[3] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V3*/ meltfptr[2] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4301:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.CFLD__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
            /*_.NAMED_NAME__V5*/
            meltfptr[3] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4302:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)((/*~FKEYW*/ meltfclos->tabval[0])) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
            /*_.NAMED_NAME__V6*/
            meltfptr[5] = slot;
        };
        ;
        /*_#eqeqS__L3*/
        meltfnum[1] =
            melt_string_same((melt_ptr_t)(/*_.NAMED_NAME__V5*/ meltfptr[3]), (melt_ptr_t)(/*_.NAMED_NAME__V6*/ meltfptr[5]));;
        MELT_LOCATION("warmelt-macro.melt:4301:/ cond");
        /*cond*/
        if (/*_#eqeqS__L3*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4304:/ cond");
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)((/*~FLDCONT*/ meltfclos->tabval[1])),
                                                      (melt_ptr_t)((/*!CLASS_REFERENCE*/ meltfrout->tabval[1])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                /*^putslot*/
                                /*putslot*/
                                melt_assertmsg("checkobj putslot  _ @REFERENCED_VALUE", melt_magic_discr((melt_ptr_t)((/*~FLDCONT*/ meltfclos->tabval[1]))) == MELTOBMAG_OBJECT);
                                melt_putfield_object(((/*~FLDCONT*/ meltfclos->tabval[1])), (0), (/*_.CFLD__V2*/ meltfptr[1]), "REFERENCED_VALUE");
                                ;
                                /*^touch*/
                                meltgc_touch((/*~FLDCONT*/ meltfclos->tabval[1]));
                                ;
                                /*^touchobj*/

                                melt_dbgtrace_written_object ((/*~FLDCONT*/ meltfclos->tabval[1]), "put-fields");
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4305:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;
                    MELT_LOCATION("warmelt-macro.melt:4305:/ putxtraresult");
                    if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                    if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                    if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                    ;
                    /*^finalreturn*/
                    ;
                    /*finalret*/
                    goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-macro.melt:4303:/ quasiblock");


                    /*_.PROGN___V9*/
                    meltfptr[8] = /*_.RETURN___V8*/ meltfptr[7];;
                    /*^compute*/
                    /*_._IFELSE___V7*/
                    meltfptr[6] = /*_.PROGN___V9*/ meltfptr[8];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4301:/ clear");
                    /*clear*/ /*_.RETURN___V8*/
                    meltfptr[7] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V9*/
                    meltfptr[8] = 0 ;
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
                    MELT_LOCATION("warmelt-macro.melt:4307:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = /*_.CFLD__V2*/ meltfptr[1];;
                    MELT_LOCATION("warmelt-macro.melt:4307:/ putxtraresult");
                    if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                    if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                    if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                    ;
                    /*^finalreturn*/
                    ;
                    /*finalret*/
                    goto meltlabend_rout ;
                    /*_._IFELSE___V7*/
                    meltfptr[6] = /*_.RETURN___V10*/ meltfptr[7];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4301:/ clear");
                    /*clear*/ /*_.RETURN___V10*/
                    meltfptr[7] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4299:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_._IFELSE___V7*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-macro.melt:4299:/ locexp");
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
        /*clear*/ /*_.IFCPP___V3*/
        meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NAMED_NAME__V5*/
        meltfptr[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NAMED_NAME__V6*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_#eqeqS__L3*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V7*/
        meltfptr[6] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl14", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_64_WARMELTmiMACRO_LAMBDA_cl14_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_64_WARMELTmiMACRO_LAMBDA_cl14*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE fromline 1716 */

    /** start of frame for meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE// fromline 1531
        : public Melt_CallFrameWithValues<67>
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
        MeltFrame_meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<67> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE), clos) {};
        MeltFrame_meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE() //the constructor fromline 1606
            : Melt_CallFrameWithValues<67> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<67> (fil,lin, sizeof(MeltFrame_meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE)) {};
        MeltFrame_meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<67> (fil,lin, sizeof(MeltFrame_meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE


    /** end of frame for meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE fromline 1661**/

    /* end of frame for routine meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE fromline 1720 */

    /* classy proc frame meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE */
    MeltFrame_meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("PATEXPAND_INSTANCE", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:4328:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4329:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4329:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4329:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4329)?(4329):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V6*/
                        meltfptr[5] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V5*/
            meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4329:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4330:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4330:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[7] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4330:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4330)?(4330):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V8*/
                        meltfptr[7] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V7*/
            meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[7];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4330:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/
            meltfptr[7] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4331:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.PCTX__V4*/ meltfptr[3]), (melt_ptr_t)((/*!CLASS_PATTERN_EXPANSION_CONTEXT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:4331:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[9] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4331:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check pctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4331)?(4331):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V10*/
                        meltfptr[9] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V9*/
            meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[9];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4331:/ clear");
            /*clear*/ /*_#IS_A__L3*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V10*/
            meltfptr[9] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V9*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4332:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4332:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4332:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L5*/ meltfnum[4];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4332;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "patexpand_instance sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V12*/
                            meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V11*/
                        meltfptr[9] = /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4332:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L5*/
                        meltfnum[4] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V12*/
                        meltfptr[11] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V11*/ meltfptr[9] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4332:/ locexp");
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
            /*clear*/ /*_._IF___V11*/
            meltfptr[9] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:4333:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:4334:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V14*/
            meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4335:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V15*/
            meltfptr[14] = slot;
        };
        ;
        /*_.FIELDS__V16*/
        meltfptr[15] = (/*nil*/NULL);;
        /*^compute*/
        /*_.FIELDNAMS__V17*/
        meltfptr[16] = (/*nil*/NULL);;
        /*^compute*/
        /*_.LIST_FIRST__V18*/
        meltfptr[17] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V14*/ meltfptr[9])));;
        /*^compute*/
        /*_.CURPAIR__V19*/
        meltfptr[18] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V18*/ meltfptr[17])));;
        /*^compute*/
        /*_.CLANAME__V20*/
        meltfptr[19] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V19*/ meltfptr[18])));;
        /*^compute*/
        /*_.CLA__V21*/
        meltfptr[20] = (/*nil*/NULL);;
        /*^compute*/
        /*_.CLABIND__V22*/
        meltfptr[21] = (/*nil*/NULL);;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L6*/
        meltfnum[4] =
            !melt_is_instance_of((melt_ptr_t)(/*_.CLANAME__V20*/ meltfptr[19]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[4])));;
        MELT_LOCATION("warmelt-macro.melt:4343:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L6*/ meltfnum[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4344:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V15*/ meltfptr[14]), ( "missing class symbol for INSTANCE pattern"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:4345:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CLANAME__V20*/ meltfptr[19];
            /*_.BND__V24*/
            meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!FIND_ENV*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.FLDLIST__V25*/
        meltfptr[24] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[6]))));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_A__L7*/
        meltfnum[0] =
            melt_is_instance_of((melt_ptr_t)(/*_.BND__V24*/ meltfptr[23]), (melt_ptr_t)((/*!CLASS_CLASS_BINDING*/ meltfrout->tabval[7])));;
        MELT_LOCATION("warmelt-macro.melt:4349:/ cond");
        /*cond*/
        if (/*_#IS_A__L7*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:4350:/ compute");
                    /*_.CLABIND__V22*/
                    meltfptr[21] = /*_.SETQ___V27*/ meltfptr[26] = /*_.BND__V24*/ meltfptr[23];;
                    MELT_LOCATION("warmelt-macro.melt:4351:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.BND__V24*/ meltfptr[23]) /*=obj*/;
                        melt_object_get_field(slot,obj, 3, "CBIND_CLASS");
                        /*_.CBIND_CLASS__V28*/
                        meltfptr[27] = slot;
                    };
                    ;
                    /*^compute*/
                    /*_.CLA__V21*/
                    meltfptr[20] = /*_.SETQ___V29*/ meltfptr[28] = /*_.CBIND_CLASS__V28*/ meltfptr[27];;
                    MELT_LOCATION("warmelt-macro.melt:4349:/ quasiblock");


                    /*_.PROGN___V30*/
                    meltfptr[29] = /*_.SETQ___V29*/ meltfptr[28];;
                    /*^compute*/
                    /*_._IFELSE___V26*/
                    meltfptr[25] = /*_.PROGN___V30*/ meltfptr[29];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4349:/ clear");
                    /*clear*/ /*_.SETQ___V27*/
                    meltfptr[26] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.CBIND_CLASS__V28*/
                    meltfptr[27] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V29*/
                    meltfptr[28] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V30*/
                    meltfptr[29] = 0 ;
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
                    /*_#IS_A__L8*/
                    meltfnum[7] =
                        melt_is_instance_of((melt_ptr_t)(/*_.BND__V24*/ meltfptr[23]), (melt_ptr_t)((/*!CLASS_VALUE_BINDING*/ meltfrout->tabval[8])));;
                    MELT_LOCATION("warmelt-macro.melt:4353:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L8*/ meltfnum[7]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:4354:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.BND__V24*/ meltfptr[23]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 1, "VBIND_VALUE");
                                    /*_.VBIND_VALUE__V31*/
                                    meltfptr[26] = slot;
                                };
                                ;
                                /*_#IS_A__L10*/
                                meltfnum[9] =
                                    melt_is_instance_of((melt_ptr_t)(/*_.VBIND_VALUE__V31*/ meltfptr[26]), (melt_ptr_t)((/*!CLASS_CLASS*/ meltfrout->tabval[9])));;
                                /*^compute*/
                                /*_#_IF___L9*/
                                meltfnum[8] = /*_#IS_A__L10*/ meltfnum[9];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:4353:/ clear");
                                /*clear*/ /*_.VBIND_VALUE__V31*/
                                meltfptr[26] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#IS_A__L10*/
                                meltfnum[9] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_#_IF___L9*/ meltfnum[8] = 0;;
                        }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4353:/ cond");
                    /*cond*/
                    if (/*_#_IF___L9*/ meltfnum[8]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:4355:/ compute");
                                /*_.CLABIND__V22*/
                                meltfptr[21] = /*_.SETQ___V33*/ meltfptr[28] = /*_.BND__V24*/ meltfptr[23];;
                                MELT_LOCATION("warmelt-macro.melt:4356:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.BND__V24*/ meltfptr[23]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 1, "VBIND_VALUE");
                                    /*_.VBIND_VALUE__V34*/
                                    meltfptr[29] = slot;
                                };
                                ;
                                /*^compute*/
                                /*_.CLA__V21*/
                                meltfptr[20] = /*_.SETQ___V35*/ meltfptr[26] = /*_.VBIND_VALUE__V34*/ meltfptr[29];;
                                MELT_LOCATION("warmelt-macro.melt:4353:/ quasiblock");


                                /*_.PROGN___V36*/
                                meltfptr[35] = /*_.SETQ___V35*/ meltfptr[26];;
                                /*^compute*/
                                /*_._IFELSE___V32*/
                                meltfptr[27] = /*_.PROGN___V36*/ meltfptr[35];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:4353:/ clear");
                                /*clear*/ /*_.SETQ___V33*/
                                meltfptr[28] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.VBIND_VALUE__V34*/
                                meltfptr[29] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.SETQ___V35*/
                                meltfptr[26] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V36*/
                                meltfptr[35] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:4360:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.CLANAME__V20*/ meltfptr[19]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                    /*_.NAMED_NAME__V37*/
                                    meltfptr[28] = slot;
                                };
                                ;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:4359:/ locexp");
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V15*/ meltfptr[14]), ( "invalid class name for INSTANCE"), (melt_ptr_t)(/*_.NAMED_NAME__V37*/ meltfptr[28]));
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-macro.melt:4361:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] = (/*nil*/NULL);;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:4361:/ locexp");
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
                                MELT_LOCATION("warmelt-macro.melt:4358:/ quasiblock");


                                /*_.PROGN___V39*/
                                meltfptr[26] = /*_.RETURN___V38*/ meltfptr[29];;
                                /*^compute*/
                                /*_._IFELSE___V32*/
                                meltfptr[27] = /*_.PROGN___V39*/ meltfptr[26];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:4353:/ clear");
                                /*clear*/ /*_.NAMED_NAME__V37*/
                                meltfptr[28] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.RETURN___V38*/
                                meltfptr[29] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V39*/
                                meltfptr[26] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V26*/
                    meltfptr[25] = /*_._IFELSE___V32*/ meltfptr[27];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4349:/ clear");
                    /*clear*/ /*_#IS_A__L8*/
                    meltfnum[7] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#_IF___L9*/
                    meltfnum[8] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V32*/
                    meltfptr[27] = 0 ;
                }
                ;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4363:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L11*/
            meltfnum[9] =
                melt_is_instance_of((melt_ptr_t)(/*_.CLA__V21*/ meltfptr[20]), (melt_ptr_t)((/*!CLASS_CLASS*/ meltfrout->tabval[9])));;
            MELT_LOCATION("warmelt-macro.melt:4363:/ cond");
            /*cond*/
            if (/*_#IS_A__L11*/ meltfnum[9]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V41*/ meltfptr[28] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4363:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check cla"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4363)?(4363):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V41*/
                        meltfptr[28] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V40*/
            meltfptr[35] = /*_._IFELSE___V41*/ meltfptr[28];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4363:/ clear");
            /*clear*/ /*_#IS_A__L11*/
            meltfnum[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V41*/
            meltfptr[28] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V40*/ meltfptr[35] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V42*/
        meltfptr[29] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V19*/ meltfptr[18])));;
        MELT_LOCATION("warmelt-macro.melt:4364:/ compute");
        /*_.CURPAIR__V19*/
        meltfptr[18] = /*_.SETQ___V43*/ meltfptr[26] = /*_.PAIR_TAIL__V42*/ meltfptr[29];;
        MELT_LOCATION("warmelt-macro.melt:4366:/ loop");
        /*loop*/
        {
meltlabloop_INSLOOP_4:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;

                MELT_CHECK_SIGNAL();
                ;
                /*_#IS_PAIR__L12*/
                meltfnum[7] =
                    (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V19*/ meltfptr[18])) == MELTOBMAG_PAIR);;
                /*^compute*/
                /*_#NOT__L13*/
                meltfnum[8] =
                    (!(/*_#IS_PAIR__L12*/ meltfnum[7]));;
                MELT_LOCATION("warmelt-macro.melt:4368:/ cond");
                /*cond*/
                if (/*_#NOT__L13*/ meltfnum[8]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^quasiblock*/


                            /*^compute*/
                            /*_.INSLOOP__V45*/ meltfptr[28] =  /*fromline 1341*/ NULL ;;

                            /*^exit*/
                            /*exit*/
                            {
                                goto meltlabexit_INSLOOP_4;
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    } /*noelse*/
                ;
                MELT_LOCATION("warmelt-macro.melt:4369:/ quasiblock");


                /*_.CURFKW__V46*/
                meltfptr[45] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V19*/ meltfptr[18])));;

                MELT_CHECK_SIGNAL();
                ;
                /*^compute*/
                /*_#IS_NOT_A__L14*/
                meltfnum[9] =
                    !melt_is_instance_of((melt_ptr_t)(/*_.CURFKW__V46*/ meltfptr[45]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[10])));;
                MELT_LOCATION("warmelt-macro.melt:4370:/ cond");
                /*cond*/
                if (/*_#IS_NOT_A__L14*/ meltfnum[9]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:4371:/ locexp");
                                /* error_plain */
                                melt_error_str((melt_ptr_t)(/*_.LOC__V15*/ meltfptr[14]), ( "expecting keyword in INSTANCE"), (melt_ptr_t)0);
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    } /*noelse*/
                ;
                /*_.PAIR_TAIL__V47*/
                meltfptr[46] =
                    (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V19*/ meltfptr[18])));;
                MELT_LOCATION("warmelt-macro.melt:4372:/ compute");
                /*_.CURPAIR__V19*/
                meltfptr[18] = /*_.SETQ___V48*/ meltfptr[47] = /*_.PAIR_TAIL__V47*/ meltfptr[46];;
                MELT_LOCATION("warmelt-macro.melt:4373:/ quasiblock");


                /*_.CUREXP__V49*/
                meltfptr[48] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V19*/ meltfptr[18])));;
                MELT_LOCATION("warmelt-macro.melt:4374:/ quasiblock");



                MELT_CHECK_SIGNAL();
                ;
                /*^apply*/
                /*apply*/
                {
                    union meltparam_un argtab[5];
                    memset(&argtab, 0, sizeof(argtab));
                    /*^apply.arg*/
                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CLA__V21*/ meltfptr[20];
                    /*^apply.arg*/
                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.CUREXP__V49*/ meltfptr[48];
                    /*^apply.arg*/
                    argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                    /*^apply.arg*/
                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.PCTX__V4*/ meltfptr[3];
                    /*^apply.arg*/
                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.LOC__V15*/ meltfptr[14];
                    /*_.FLDP__V50*/
                    meltfptr[49] =  melt_apply ((meltclosure_ptr_t)((/*!PARSE_FIELD_PATTERN*/ meltfrout->tabval[11])), (melt_ptr_t)(/*_.CURFKW__V46*/ meltfptr[45]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                }
                ;

#if MELT_HAVE_DEBUG
                MELT_LOCATION("warmelt-macro.melt:4375:/ cppif.then");
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
                    meltfnum[14] =
                        /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                        (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                        0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                        ;;
                    MELT_LOCATION("warmelt-macro.melt:4375:/ cond");
                    /*cond*/
                    if (/*_#MELT_NEED_DBG__L15*/ meltfnum[14]) /*then*/
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
                                MELT_LOCATION("warmelt-macro.melt:4375:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[5];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L16*/ meltfnum[15];
                                    /*^apply.arg*/
                                    argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                    /*^apply.arg*/
                                    argtab[2].meltbp_long = 4375;
                                    /*^apply.arg*/
                                    argtab[3].meltbp_cstring =  "patexpand_instance fldp";
                                    /*^apply.arg*/
                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.FLDP__V50*/ meltfptr[49];
                                    /*_.MELT_DEBUG_FUN__V52*/
                                    meltfptr[51] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                /*_._IF___V51*/
                                meltfptr[50] = /*_.MELT_DEBUG_FUN__V52*/ meltfptr[51];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:4375:/ clear");
                                /*clear*/ /*_#MELT_CALLCOUNT__L16*/
                                meltfnum[15] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.MELT_DEBUG_FUN__V52*/
                                meltfptr[51] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IF___V51*/ meltfptr[50] =  /*fromline 1341*/ NULL ;;
                        }
                    ;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4375:/ locexp");
                        /*void*/
                        (void)0;
                    }
                    ;
                    /*^quasiblock*/


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_#MELT_NEED_DBG__L15*/
                    meltfnum[14] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IF___V51*/
                    meltfptr[50] = 0 ;
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
                MELT_LOCATION("warmelt-macro.melt:4376:/ cond");
                /*cond*/
                if (/*_.FLDP__V50*/ meltfptr[49]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:4377:/ locexp");
                                meltgc_append_list((melt_ptr_t)(/*_.FLDLIST__V25*/ meltfptr[24]), (melt_ptr_t)(/*_.FLDP__V50*/ meltfptr[49]));
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-macro.melt:4376:/ cond.else");

                        /*^block*/
                        /*anyblock*/
                        {

                            MELT_LOCATION("warmelt-macro.melt:4379:/ getslot");
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.CURFKW__V46*/ meltfptr[45]) /*=obj*/;
                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                /*_.NAMED_NAME__V53*/
                                meltfptr[51] = slot;
                            };
                            ;

                            {
                                MELT_LOCATION("warmelt-macro.melt:4378:/ locexp");
                                melt_error_str((melt_ptr_t)(/*_.LOC__V15*/ meltfptr[14]), ( "bad field name in INSTANCE"), (melt_ptr_t)(/*_.NAMED_NAME__V53*/ meltfptr[51]));
                            }
                            ;
                            /*epilog*/

                            MELT_LOCATION("warmelt-macro.melt:4376:/ clear");
                            /*clear*/ /*_.NAMED_NAME__V53*/
                            meltfptr[51] = 0 ;
                        }
                        ;
                    }
                ;

                MELT_LOCATION("warmelt-macro.melt:4374:/ clear");
                /*clear*/ /*_.FLDP__V50*/
                meltfptr[49] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:4373:/ clear");
                /*clear*/ /*_.CUREXP__V49*/
                meltfptr[48] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:4369:/ clear");
                /*clear*/ /*_.CURFKW__V46*/
                meltfptr[45] = 0 ;
                /*^clear*/
                /*clear*/ /*_#IS_NOT_A__L14*/
                meltfnum[9] = 0 ;
                /*^clear*/
                /*clear*/ /*_.PAIR_TAIL__V47*/
                meltfptr[46] = 0 ;
                /*^clear*/
                /*clear*/ /*_.SETQ___V48*/
                meltfptr[47] = 0 ;
                /*_.PAIR_TAIL__V54*/
                meltfptr[50] =
                    (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V19*/ meltfptr[18])));;
                MELT_LOCATION("warmelt-macro.melt:4381:/ compute");
                /*_.CURPAIR__V19*/
                meltfptr[18] = /*_.SETQ___V55*/ meltfptr[51] = /*_.PAIR_TAIL__V54*/ meltfptr[50];;

                MELT_CHECK_SIGNAL();
                ;
                /*epilog*/

                MELT_LOCATION("warmelt-macro.melt:4366:/ clear");
                /*clear*/ /*_#IS_PAIR__L12*/
                meltfnum[7] = 0 ;
                /*^clear*/
                /*clear*/ /*_#NOT__L13*/
                meltfnum[8] = 0 ;
                /*^clear*/
                /*clear*/ /*_.PAIR_TAIL__V54*/
                meltfptr[50] = 0 ;
                /*^clear*/
                /*clear*/ /*_.SETQ___V55*/
                meltfptr[51] = 0 ;
            }
            ;
            ;
            goto meltlabloop_INSLOOP_4;
meltlabexit_INSLOOP_4:;/*^loopepilog*/
            /*loopepilog*/
            /*_.FOREVER___V44*/
            meltfptr[27] = /*_.INSLOOP__V45*/ meltfptr[28];;
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:4383:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4384:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[13]);
            /*_.SPATFIELDS__V57*/
            meltfptr[48] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[12])), (melt_ptr_t)(/*_.FLDLIST__V25*/ meltfptr[24]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:4386:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V59*/
        meltfptr[46] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_16*/ meltfrout->tabval[16])), (0));
        ;
        /*_.LAMBDA___V58*/
        meltfptr[45] = /*_.LAMBDA___V59*/ meltfptr[46];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4385:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LAMBDA___V58*/ meltfptr[45];
            /*_.SUBPATS__V60*/
            meltfptr[47] =  melt_apply ((meltclosure_ptr_t)((/*!MULTIPLE_MAP*/ meltfrout->tabval[14])), (melt_ptr_t)(/*_.SPATFIELDS__V57*/ meltfptr[48]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4388:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_PATTERN_INSTANCE*/ meltfrout->tabval[17])), (5), "CLASS_SOURCE_PATTERN_INSTANCE");
            /*_.INST__V62*/
            meltfptr[51] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V62*/ meltfptr[51])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V62*/ meltfptr[51]), (1), (/*_.LOC__V15*/ meltfptr[14]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @PAT_WEIGHT", melt_magic_discr((melt_ptr_t)(/*_.INST__V62*/ meltfptr[51])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V62*/ meltfptr[51]), (2), ((/*nil*/NULL)), "PAT_WEIGHT");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SPAT_CLASS", melt_magic_discr((melt_ptr_t)(/*_.INST__V62*/ meltfptr[51])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V62*/ meltfptr[51]), (3), (/*_.CLA__V21*/ meltfptr[20]), "SPAT_CLASS");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SPAT_FIELDS", melt_magic_discr((melt_ptr_t)(/*_.INST__V62*/ meltfptr[51])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V62*/ meltfptr[51]), (4), (/*_.SPATFIELDS__V57*/ meltfptr[48]), "SPAT_FIELDS");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V62*/ meltfptr[51], "newly made instance");
        ;
        /*_.SPATI__V61*/
        meltfptr[50] = /*_.INST__V62*/ meltfptr[51];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4394:/ quasiblock");


        /*^multiapply*/
        /*multiapply 1args, 3x.res*/
        {

            union meltparam_un restab[3];
            memset(&restab, 0, sizeof(restab));
            /*^multiapply.xres*/
            restab[0].meltbp_longptr =  & /*_#IMAX__L17*/ meltfnum[15];
            /*^multiapply.xres*/
            restab[1].meltbp_longptr =  & /*_#IMIN__L18*/ meltfnum[14];
            /*^multiapply.xres*/
            restab[2].meltbp_longptr =  & /*_#ISUM__L19*/ meltfnum[9];
            /*^multiapply.appl*/
            /*_.SUBPATW__V63*/
            meltfptr[62] =  melt_apply ((meltclosure_ptr_t)((/*!PATTERN_WEIGHT_TUPLE*/ meltfrout->tabval[18])), (melt_ptr_t)(/*_.SUBPATS__V60*/ meltfptr[47]), (""), (union meltparam_un*)0, (MELTBPARSTR_LONG MELTBPARSTR_LONG MELTBPARSTR_LONG ""), restab);
        }
        ;
        /*^quasiblock*/



        MELT_CHECK_SIGNAL();
        ;
        /*_#plI__L20*/
        meltfnum[7] =
            ((1) + (/*_#ISUM__L19*/ meltfnum[9]));;
        /*^compute*/
        /*_.MAKE_INTEGERBOX__V64*/
        meltfptr[63] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_CONSTANT_INTEGER*/ meltfrout->tabval[19])), (/*_#plI__L20*/ meltfnum[7])));;
        MELT_LOCATION("warmelt-macro.melt:4397:/ quasiblock");


        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @PAT_WEIGHT", melt_magic_discr((melt_ptr_t)(/*_.SPATI__V61*/ meltfptr[50])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.SPATI__V61*/ meltfptr[50]), (2), (/*_.MAKE_INTEGERBOX__V64*/ meltfptr[63]), "PAT_WEIGHT");
        ;
        /*^touch*/
        meltgc_touch(/*_.SPATI__V61*/ meltfptr[50]);
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.SPATI__V61*/ meltfptr[50], "put-fields");
        ;


        MELT_LOCATION("warmelt-macro.melt:4394:/ clear");
        /*clear*/ /*_#plI__L20*/
        meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_INTEGERBOX__V64*/
        meltfptr[63] = 0 ;

        /*^clear*/
        /*clear*/ /*_#IMAX__L17*/
        meltfnum[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IMIN__L18*/
        meltfnum[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_#ISUM__L19*/
        meltfnum[9] = 0 ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4402:/ cppif.then");
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
            meltfnum[8] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:4402:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L21*/ meltfnum[8]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L22*/ meltfnum[7] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:4402:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L22*/ meltfnum[7];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4402;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "patexpand_instance return spati";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SPATI__V61*/ meltfptr[50];
                            /*_.MELT_DEBUG_FUN__V66*/
                            meltfptr[62] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V65*/
                        meltfptr[63] = /*_.MELT_DEBUG_FUN__V66*/ meltfptr[62];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4402:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L22*/
                        meltfnum[7] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V66*/
                        meltfptr[62] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V65*/ meltfptr[63] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4402:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L21*/
            meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V65*/
            meltfptr[63] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:4403:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.SPATI__V61*/ meltfptr[50];;

        {
            MELT_LOCATION("warmelt-macro.melt:4403:/ locexp");
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
        /*_.LET___V56*/
        meltfptr[49] = /*_.RETURN___V67*/ meltfptr[62];;

        MELT_LOCATION("warmelt-macro.melt:4383:/ clear");
        /*clear*/ /*_.SPATFIELDS__V57*/
        meltfptr[48] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LAMBDA___V58*/
        meltfptr[45] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SUBPATS__V60*/
        meltfptr[47] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SPATI__V61*/
        meltfptr[50] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V67*/
        meltfptr[62] = 0 ;
        /*_.LET___V23*/
        meltfptr[22] = /*_.LET___V56*/ meltfptr[49];;

        MELT_LOCATION("warmelt-macro.melt:4345:/ clear");
        /*clear*/ /*_.BND__V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FLDLIST__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L7*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V40*/
        meltfptr[35] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V42*/
        meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V43*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V44*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V56*/
        meltfptr[49] = 0 ;
        /*_.LET___V13*/
        meltfptr[11] = /*_.LET___V23*/ meltfptr[22];;

        MELT_LOCATION("warmelt-macro.melt:4333:/ clear");
        /*clear*/ /*_.CONT__V14*/
        meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V15*/
        meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FIELDS__V16*/
        meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FIELDNAMS__V17*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CLANAME__V20*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CLA__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CLABIND__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L6*/
        meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V23*/
        meltfptr[22] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4328:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V13*/ meltfptr[11];;

        {
            MELT_LOCATION("warmelt-macro.melt:4328:/ locexp");
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
        /*clear*/ /*_.IFCPP___V7*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V9*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V13*/
        meltfptr[11] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("PATEXPAND_INSTANCE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_65_WARMELTmiMACRO_PATEXPAND_INSTANCE*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_66_WARMELTmiMACRO_LAMBDA_cl15(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_66_WARMELTmiMACRO_LAMBDA_cl15_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_66_WARMELTmiMACRO_LAMBDA_cl15 fromline 1716 */

    /** start of frame for meltrout_66_WARMELTmiMACRO_LAMBDA_cl15 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_66_WARMELTmiMACRO_LAMBDA_cl15// fromline 1531
        : public Melt_CallFrameWithValues<3>
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
        MeltFrame_meltrout_66_WARMELTmiMACRO_LAMBDA_cl15(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_66_WARMELTmiMACRO_LAMBDA_cl15), clos) {};
        MeltFrame_meltrout_66_WARMELTmiMACRO_LAMBDA_cl15() //the constructor fromline 1606
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_66_WARMELTmiMACRO_LAMBDA_cl15)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_66_WARMELTmiMACRO_LAMBDA_cl15(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_66_WARMELTmiMACRO_LAMBDA_cl15)) {};
        MeltFrame_meltrout_66_WARMELTmiMACRO_LAMBDA_cl15(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_66_WARMELTmiMACRO_LAMBDA_cl15), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_66_WARMELTmiMACRO_LAMBDA_cl15


    /** end of frame for meltrout_66_WARMELTmiMACRO_LAMBDA_cl15 fromline 1661**/

    /* end of frame for routine meltrout_66_WARMELTmiMACRO_LAMBDA_cl15 fromline 1720 */

    /* classy proc frame meltrout_66_WARMELTmiMACRO_LAMBDA_cl15 */
    MeltFrame_meltrout_66_WARMELTmiMACRO_LAMBDA_cl15
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_66_WARMELTmiMACRO_LAMBDA_cl15 fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
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
    MELT_LOCATION("warmelt-macro.melt:4386:/ getarg");
    /*_.CURPATFLD__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#CURIX__L1*/
    meltfnum[0] = meltxargtab_[0].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-macro.melt:4387:/ cond");
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CURPATFLD__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_SOURCE_FIELD_PATTERN*/ meltfrout->tabval[0])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.CURPATFLD__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 3, "SPAF_PATTERN");
                    /*_.SPAF_PATTERN__V3*/
                    meltfptr[2] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.SPAF_PATTERN__V3*/ meltfptr[2] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:4386:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.SPAF_PATTERN__V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-macro.melt:4386:/ locexp");
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
        /*clear*/ /*_.SPAF_PATTERN__V3*/
        meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl15", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_66_WARMELTmiMACRO_LAMBDA_cl15_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_66_WARMELTmiMACRO_LAMBDA_cl15*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT fromline 1716 */

    /** start of frame for meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT// fromline 1531
        : public Melt_CallFrameWithValues<68>
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
        MeltFrame_meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<68> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT), clos) {};
        MeltFrame_meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT() //the constructor fromline 1606
            : Melt_CallFrameWithValues<68> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<68> (fil,lin, sizeof(MeltFrame_meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT)) {};
        MeltFrame_meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<68> (fil,lin, sizeof(MeltFrame_meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT


    /** end of frame for meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT fromline 1661**/

    /* end of frame for routine meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT fromline 1720 */

    /* classy proc frame meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT */
    MeltFrame_meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("PATEXPAND_OBJECT", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:4423:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4424:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4424:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4424:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4424)?(4424):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V6*/
                        meltfptr[5] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V5*/
            meltfptr[4] = /*_._IFELSE___V6*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4424:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4425:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4425:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[7] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4425:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4425)?(4425):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V8*/
                        meltfptr[7] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V7*/
            meltfptr[5] = /*_._IFELSE___V8*/ meltfptr[7];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4425:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/
            meltfptr[7] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4426:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.PCTX__V4*/ meltfptr[3]), (melt_ptr_t)((/*!CLASS_PATTERN_EXPANSION_CONTEXT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:4426:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[9] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4426:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check pctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4426)?(4426):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V10*/
                        meltfptr[9] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V9*/
            meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[9];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4426:/ clear");
            /*clear*/ /*_#IS_A__L3*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V10*/
            meltfptr[9] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V9*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4427:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4427:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4427:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L5*/ meltfnum[4];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4427;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "patexpand_object sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V12*/
                            meltfptr[11] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V11*/
                        meltfptr[9] = /*_.MELT_DEBUG_FUN__V12*/ meltfptr[11];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4427:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L5*/
                        meltfnum[4] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V12*/
                        meltfptr[11] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V11*/ meltfptr[9] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4427:/ locexp");
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
            /*clear*/ /*_._IF___V11*/
            meltfptr[9] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:4428:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:4429:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V14*/
            meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4430:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V15*/
            meltfptr[14] = slot;
        };
        ;
        /*_.FIELDS__V16*/
        meltfptr[15] = (/*nil*/NULL);;
        /*^compute*/
        /*_.FIELDNAMS__V17*/
        meltfptr[16] = (/*nil*/NULL);;
        /*^compute*/
        /*_.LIST_FIRST__V18*/
        meltfptr[17] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V14*/ meltfptr[9])));;
        /*^compute*/
        /*_.CURPAIR__V19*/
        meltfptr[18] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V18*/ meltfptr[17])));;
        /*^compute*/
        /*_.CLANAME__V20*/
        meltfptr[19] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V19*/ meltfptr[18])));;
        /*^compute*/
        /*_.CLA__V21*/
        meltfptr[20] = (/*nil*/NULL);;
        /*^compute*/
        /*_.CLABIND__V22*/
        meltfptr[21] = (/*nil*/NULL);;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L6*/
        meltfnum[4] =
            !melt_is_instance_of((melt_ptr_t)(/*_.CLANAME__V20*/ meltfptr[19]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[4])));;
        MELT_LOCATION("warmelt-macro.melt:4438:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L6*/ meltfnum[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4439:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V15*/ meltfptr[14]), ( "missing class symbol for  OBJECT pattern"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:4440:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CLANAME__V20*/ meltfptr[19];
            /*_.BND__V24*/
            meltfptr[23] =  melt_apply ((meltclosure_ptr_t)((/*!FIND_ENV*/ meltfrout->tabval[5])), (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.FLDLIST__V25*/
        meltfptr[24] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[6]))));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_A__L7*/
        meltfnum[0] =
            melt_is_instance_of((melt_ptr_t)(/*_.BND__V24*/ meltfptr[23]), (melt_ptr_t)((/*!CLASS_CLASS_BINDING*/ meltfrout->tabval[7])));;
        MELT_LOCATION("warmelt-macro.melt:4444:/ cond");
        /*cond*/
        if (/*_#IS_A__L7*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:4445:/ compute");
                    /*_.CLABIND__V22*/
                    meltfptr[21] = /*_.SETQ___V27*/ meltfptr[26] = /*_.BND__V24*/ meltfptr[23];;
                    MELT_LOCATION("warmelt-macro.melt:4446:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.BND__V24*/ meltfptr[23]) /*=obj*/;
                        melt_object_get_field(slot,obj, 3, "CBIND_CLASS");
                        /*_.CBIND_CLASS__V28*/
                        meltfptr[27] = slot;
                    };
                    ;
                    /*^compute*/
                    /*_.CLA__V21*/
                    meltfptr[20] = /*_.SETQ___V29*/ meltfptr[28] = /*_.CBIND_CLASS__V28*/ meltfptr[27];;
                    MELT_LOCATION("warmelt-macro.melt:4444:/ quasiblock");


                    /*_.PROGN___V30*/
                    meltfptr[29] = /*_.SETQ___V29*/ meltfptr[28];;
                    /*^compute*/
                    /*_._IFELSE___V26*/
                    meltfptr[25] = /*_.PROGN___V30*/ meltfptr[29];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4444:/ clear");
                    /*clear*/ /*_.SETQ___V27*/
                    meltfptr[26] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.CBIND_CLASS__V28*/
                    meltfptr[27] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V29*/
                    meltfptr[28] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V30*/
                    meltfptr[29] = 0 ;
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
                    /*_#IS_A__L8*/
                    meltfnum[7] =
                        melt_is_instance_of((melt_ptr_t)(/*_.BND__V24*/ meltfptr[23]), (melt_ptr_t)((/*!CLASS_VALUE_BINDING*/ meltfrout->tabval[8])));;
                    MELT_LOCATION("warmelt-macro.melt:4448:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L8*/ meltfnum[7]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:4449:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.BND__V24*/ meltfptr[23]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 1, "VBIND_VALUE");
                                    /*_.VBIND_VALUE__V31*/
                                    meltfptr[26] = slot;
                                };
                                ;
                                /*_#IS_A__L10*/
                                meltfnum[9] =
                                    melt_is_instance_of((melt_ptr_t)(/*_.VBIND_VALUE__V31*/ meltfptr[26]), (melt_ptr_t)((/*!CLASS_CLASS*/ meltfrout->tabval[9])));;
                                /*^compute*/
                                /*_#_IF___L9*/
                                meltfnum[8] = /*_#IS_A__L10*/ meltfnum[9];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:4448:/ clear");
                                /*clear*/ /*_.VBIND_VALUE__V31*/
                                meltfptr[26] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#IS_A__L10*/
                                meltfnum[9] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_#_IF___L9*/ meltfnum[8] = 0;;
                        }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4448:/ cond");
                    /*cond*/
                    if (/*_#_IF___L9*/ meltfnum[8]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:4450:/ compute");
                                /*_.CLABIND__V22*/
                                meltfptr[21] = /*_.SETQ___V33*/ meltfptr[28] = /*_.BND__V24*/ meltfptr[23];;
                                MELT_LOCATION("warmelt-macro.melt:4451:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.BND__V24*/ meltfptr[23]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 1, "VBIND_VALUE");
                                    /*_.VBIND_VALUE__V34*/
                                    meltfptr[29] = slot;
                                };
                                ;
                                /*^compute*/
                                /*_.CLA__V21*/
                                meltfptr[20] = /*_.SETQ___V35*/ meltfptr[26] = /*_.VBIND_VALUE__V34*/ meltfptr[29];;
                                MELT_LOCATION("warmelt-macro.melt:4448:/ quasiblock");


                                /*_.PROGN___V36*/
                                meltfptr[35] = /*_.SETQ___V35*/ meltfptr[26];;
                                /*^compute*/
                                /*_._IFELSE___V32*/
                                meltfptr[27] = /*_.PROGN___V36*/ meltfptr[35];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:4448:/ clear");
                                /*clear*/ /*_.SETQ___V33*/
                                meltfptr[28] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.VBIND_VALUE__V34*/
                                meltfptr[29] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.SETQ___V35*/
                                meltfptr[26] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V36*/
                                meltfptr[35] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:4455:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.CLANAME__V20*/ meltfptr[19]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                    /*_.NAMED_NAME__V37*/
                                    meltfptr[28] = slot;
                                };
                                ;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:4454:/ locexp");
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V15*/ meltfptr[14]), ( "invalid class name for OBJECT pattern"), (melt_ptr_t)(/*_.NAMED_NAME__V37*/ meltfptr[28]));
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-macro.melt:4456:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] = (/*nil*/NULL);;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:4456:/ locexp");
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
                                MELT_LOCATION("warmelt-macro.melt:4453:/ quasiblock");


                                /*_.PROGN___V39*/
                                meltfptr[26] = /*_.RETURN___V38*/ meltfptr[29];;
                                /*^compute*/
                                /*_._IFELSE___V32*/
                                meltfptr[27] = /*_.PROGN___V39*/ meltfptr[26];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:4448:/ clear");
                                /*clear*/ /*_.NAMED_NAME__V37*/
                                meltfptr[28] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.RETURN___V38*/
                                meltfptr[29] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V39*/
                                meltfptr[26] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V26*/
                    meltfptr[25] = /*_._IFELSE___V32*/ meltfptr[27];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4444:/ clear");
                    /*clear*/ /*_#IS_A__L8*/
                    meltfnum[7] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#_IF___L9*/
                    meltfnum[8] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V32*/
                    meltfptr[27] = 0 ;
                }
                ;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4458:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L11*/
            meltfnum[9] =
                melt_is_instance_of((melt_ptr_t)(/*_.CLA__V21*/ meltfptr[20]), (melt_ptr_t)((/*!CLASS_CLASS*/ meltfrout->tabval[9])));;
            MELT_LOCATION("warmelt-macro.melt:4458:/ cond");
            /*cond*/
            if (/*_#IS_A__L11*/ meltfnum[9]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V41*/ meltfptr[28] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4458:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check cla"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4458)?(4458):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V41*/
                        meltfptr[28] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V40*/
            meltfptr[35] = /*_._IFELSE___V41*/ meltfptr[28];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4458:/ clear");
            /*clear*/ /*_#IS_A__L11*/
            meltfnum[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V41*/
            meltfptr[28] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V40*/ meltfptr[35] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V42*/
        meltfptr[29] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V19*/ meltfptr[18])));;
        MELT_LOCATION("warmelt-macro.melt:4459:/ compute");
        /*_.CURPAIR__V19*/
        meltfptr[18] = /*_.SETQ___V43*/ meltfptr[26] = /*_.PAIR_TAIL__V42*/ meltfptr[29];;
        MELT_LOCATION("warmelt-macro.melt:4461:/ loop");
        /*loop*/
        {
meltlabloop_INSLOOP_5:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;

                MELT_CHECK_SIGNAL();
                ;
                /*_#IS_PAIR__L12*/
                meltfnum[7] =
                    (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V19*/ meltfptr[18])) == MELTOBMAG_PAIR);;
                /*^compute*/
                /*_#NOT__L13*/
                meltfnum[8] =
                    (!(/*_#IS_PAIR__L12*/ meltfnum[7]));;
                MELT_LOCATION("warmelt-macro.melt:4463:/ cond");
                /*cond*/
                if (/*_#NOT__L13*/ meltfnum[8]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^quasiblock*/


                            /*^compute*/
                            /*_.INSLOOP__V45*/ meltfptr[28] =  /*fromline 1341*/ NULL ;;

                            /*^exit*/
                            /*exit*/
                            {
                                goto meltlabexit_INSLOOP_5;
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    } /*noelse*/
                ;
                MELT_LOCATION("warmelt-macro.melt:4464:/ quasiblock");


                /*_.CURFKW__V46*/
                meltfptr[45] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V19*/ meltfptr[18])));;

                MELT_CHECK_SIGNAL();
                ;
                /*^compute*/
                /*_#IS_NOT_A__L14*/
                meltfnum[9] =
                    !melt_is_instance_of((melt_ptr_t)(/*_.CURFKW__V46*/ meltfptr[45]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[10])));;
                MELT_LOCATION("warmelt-macro.melt:4465:/ cond");
                /*cond*/
                if (/*_#IS_NOT_A__L14*/ meltfnum[9]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:4466:/ locexp");
                                /* error_plain */
                                melt_error_str((melt_ptr_t)(/*_.LOC__V15*/ meltfptr[14]), ( "expecting keyword in OBJECT pattern"), (melt_ptr_t)0);
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    } /*noelse*/
                ;
                /*_.PAIR_TAIL__V47*/
                meltfptr[46] =
                    (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V19*/ meltfptr[18])));;
                MELT_LOCATION("warmelt-macro.melt:4467:/ compute");
                /*_.CURPAIR__V19*/
                meltfptr[18] = /*_.SETQ___V48*/ meltfptr[47] = /*_.PAIR_TAIL__V47*/ meltfptr[46];;
                MELT_LOCATION("warmelt-macro.melt:4468:/ quasiblock");


                /*_.CUREXP__V49*/
                meltfptr[48] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V19*/ meltfptr[18])));;
                MELT_LOCATION("warmelt-macro.melt:4469:/ quasiblock");



                MELT_CHECK_SIGNAL();
                ;
                /*^apply*/
                /*apply*/
                {
                    union meltparam_un argtab[5];
                    memset(&argtab, 0, sizeof(argtab));
                    /*^apply.arg*/
                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CLA__V21*/ meltfptr[20];
                    /*^apply.arg*/
                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.CUREXP__V49*/ meltfptr[48];
                    /*^apply.arg*/
                    argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                    /*^apply.arg*/
                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.PCTX__V4*/ meltfptr[3];
                    /*^apply.arg*/
                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.LOC__V15*/ meltfptr[14];
                    /*_.FLDP__V50*/
                    meltfptr[49] =  melt_apply ((meltclosure_ptr_t)((/*!PARSE_FIELD_PATTERN*/ meltfrout->tabval[11])), (melt_ptr_t)(/*_.CURFKW__V46*/ meltfptr[45]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                }
                ;

#if MELT_HAVE_DEBUG
                MELT_LOCATION("warmelt-macro.melt:4470:/ cppif.then");
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
                    meltfnum[14] =
                        /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                        (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                        0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                        ;;
                    MELT_LOCATION("warmelt-macro.melt:4470:/ cond");
                    /*cond*/
                    if (/*_#MELT_NEED_DBG__L15*/ meltfnum[14]) /*then*/
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
                                MELT_LOCATION("warmelt-macro.melt:4470:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[5];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L16*/ meltfnum[15];
                                    /*^apply.arg*/
                                    argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                    /*^apply.arg*/
                                    argtab[2].meltbp_long = 4470;
                                    /*^apply.arg*/
                                    argtab[3].meltbp_cstring =  "patexpand_object fldp";
                                    /*^apply.arg*/
                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.FLDP__V50*/ meltfptr[49];
                                    /*_.MELT_DEBUG_FUN__V52*/
                                    meltfptr[51] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                /*_._IF___V51*/
                                meltfptr[50] = /*_.MELT_DEBUG_FUN__V52*/ meltfptr[51];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:4470:/ clear");
                                /*clear*/ /*_#MELT_CALLCOUNT__L16*/
                                meltfnum[15] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.MELT_DEBUG_FUN__V52*/
                                meltfptr[51] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IF___V51*/ meltfptr[50] =  /*fromline 1341*/ NULL ;;
                        }
                    ;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4470:/ locexp");
                        /*void*/
                        (void)0;
                    }
                    ;
                    /*^quasiblock*/


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_#MELT_NEED_DBG__L15*/
                    meltfnum[14] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IF___V51*/
                    meltfptr[50] = 0 ;
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
                MELT_LOCATION("warmelt-macro.melt:4471:/ cond");
                /*cond*/
                if (/*_.FLDP__V50*/ meltfptr[49]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:4472:/ locexp");
                                meltgc_append_list((melt_ptr_t)(/*_.FLDLIST__V25*/ meltfptr[24]), (melt_ptr_t)(/*_.FLDP__V50*/ meltfptr[49]));
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-macro.melt:4471:/ cond.else");

                        /*^block*/
                        /*anyblock*/
                        {

                            MELT_LOCATION("warmelt-macro.melt:4474:/ getslot");
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.CURFKW__V46*/ meltfptr[45]) /*=obj*/;
                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                /*_.NAMED_NAME__V53*/
                                meltfptr[51] = slot;
                            };
                            ;

                            {
                                MELT_LOCATION("warmelt-macro.melt:4473:/ locexp");
                                melt_error_str((melt_ptr_t)(/*_.LOC__V15*/ meltfptr[14]), ( "bad field name in OBJECT pattern"), (melt_ptr_t)(/*_.NAMED_NAME__V53*/ meltfptr[51]));
                            }
                            ;
                            /*epilog*/

                            MELT_LOCATION("warmelt-macro.melt:4471:/ clear");
                            /*clear*/ /*_.NAMED_NAME__V53*/
                            meltfptr[51] = 0 ;
                        }
                        ;
                    }
                ;

                MELT_LOCATION("warmelt-macro.melt:4469:/ clear");
                /*clear*/ /*_.FLDP__V50*/
                meltfptr[49] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:4468:/ clear");
                /*clear*/ /*_.CUREXP__V49*/
                meltfptr[48] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:4464:/ clear");
                /*clear*/ /*_.CURFKW__V46*/
                meltfptr[45] = 0 ;
                /*^clear*/
                /*clear*/ /*_#IS_NOT_A__L14*/
                meltfnum[9] = 0 ;
                /*^clear*/
                /*clear*/ /*_.PAIR_TAIL__V47*/
                meltfptr[46] = 0 ;
                /*^clear*/
                /*clear*/ /*_.SETQ___V48*/
                meltfptr[47] = 0 ;
                /*_.PAIR_TAIL__V54*/
                meltfptr[50] =
                    (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V19*/ meltfptr[18])));;
                MELT_LOCATION("warmelt-macro.melt:4476:/ compute");
                /*_.CURPAIR__V19*/
                meltfptr[18] = /*_.SETQ___V55*/ meltfptr[51] = /*_.PAIR_TAIL__V54*/ meltfptr[50];;

                MELT_CHECK_SIGNAL();
                ;
                /*epilog*/

                MELT_LOCATION("warmelt-macro.melt:4461:/ clear");
                /*clear*/ /*_#IS_PAIR__L12*/
                meltfnum[7] = 0 ;
                /*^clear*/
                /*clear*/ /*_#NOT__L13*/
                meltfnum[8] = 0 ;
                /*^clear*/
                /*clear*/ /*_.PAIR_TAIL__V54*/
                meltfptr[50] = 0 ;
                /*^clear*/
                /*clear*/ /*_.SETQ___V55*/
                meltfptr[51] = 0 ;
            }
            ;
            ;
            goto meltlabloop_INSLOOP_5;
meltlabexit_INSLOOP_5:;/*^loopepilog*/
            /*loopepilog*/
            /*_.FOREVER___V44*/
            meltfptr[27] = /*_.INSLOOP__V45*/ meltfptr[28];;
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:4478:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4479:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[13]);
            /*_.PATFIELDS__V57*/
            meltfptr[48] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[12])), (melt_ptr_t)(/*_.FLDLIST__V25*/ meltfptr[24]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:4481:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V59*/
        meltfptr[46] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_16*/ meltfrout->tabval[16])), (0));
        ;
        /*_.LAMBDA___V58*/
        meltfptr[45] = /*_.LAMBDA___V59*/ meltfptr[46];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4480:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LAMBDA___V58*/ meltfptr[45];
            /*_.SUBPATS__V60*/
            meltfptr[47] =  melt_apply ((meltclosure_ptr_t)((/*!MULTIPLE_MAP*/ meltfrout->tabval[14])), (melt_ptr_t)(/*_.PATFIELDS__V57*/ meltfptr[48]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4483:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_PATTERN_OBJECT*/ meltfrout->tabval[17])), (5), "CLASS_SOURCE_PATTERN_OBJECT");
            /*_.INST__V62*/
            meltfptr[51] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V62*/ meltfptr[51])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V62*/ meltfptr[51]), (1), (/*_.LOC__V15*/ meltfptr[14]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SPAT_CLASS", melt_magic_discr((melt_ptr_t)(/*_.INST__V62*/ meltfptr[51])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V62*/ meltfptr[51]), (3), (/*_.CLA__V21*/ meltfptr[20]), "SPAT_CLASS");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SPAT_FIELDS", melt_magic_discr((melt_ptr_t)(/*_.INST__V62*/ meltfptr[51])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V62*/ meltfptr[51]), (4), (/*_.PATFIELDS__V57*/ meltfptr[48]), "SPAT_FIELDS");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V62*/ meltfptr[51], "newly made instance");
        ;
        /*_.SPATI__V61*/
        meltfptr[50] = /*_.INST__V62*/ meltfptr[51];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4488:/ quasiblock");


        /*^multiapply*/
        /*multiapply 1args, 3x.res*/
        {

            union meltparam_un restab[3];
            memset(&restab, 0, sizeof(restab));
            /*^multiapply.xres*/
            restab[0].meltbp_longptr =  & /*_#IMAX__L17*/ meltfnum[15];
            /*^multiapply.xres*/
            restab[1].meltbp_longptr =  & /*_#IMIN__L18*/ meltfnum[14];
            /*^multiapply.xres*/
            restab[2].meltbp_longptr =  & /*_#ISUM__L19*/ meltfnum[9];
            /*^multiapply.appl*/
            /*_.SUBPATW__V64*/
            meltfptr[63] =  melt_apply ((meltclosure_ptr_t)((/*!PATTERN_WEIGHT_TUPLE*/ meltfrout->tabval[18])), (melt_ptr_t)(/*_.SUBPATS__V60*/ meltfptr[47]), (""), (union meltparam_un*)0, (MELTBPARSTR_LONG MELTBPARSTR_LONG MELTBPARSTR_LONG ""), restab);
        }
        ;
        /*^quasiblock*/



        MELT_CHECK_SIGNAL();
        ;
        /*_#plI__L20*/
        meltfnum[7] =
            ((1) + (/*_#ISUM__L19*/ meltfnum[9]));;
        /*^compute*/
        /*_.MAKE_INTEGERBOX__V65*/
        meltfptr[64] =
            (meltgc_new_int((meltobject_ptr_t)((/*!DISCR_CONSTANT_INTEGER*/ meltfrout->tabval[19])), (/*_#plI__L20*/ meltfnum[7])));;
        MELT_LOCATION("warmelt-macro.melt:4491:/ quasiblock");


        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @PAT_WEIGHT", melt_magic_discr((melt_ptr_t)(/*_.SPATI__V61*/ meltfptr[50])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.SPATI__V61*/ meltfptr[50]), (2), (/*_.MAKE_INTEGERBOX__V65*/ meltfptr[64]), "PAT_WEIGHT");
        ;
        /*^touch*/
        meltgc_touch(/*_.SPATI__V61*/ meltfptr[50]);
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.SPATI__V61*/ meltfptr[50], "put-fields");
        ;


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4495:/ cppif.then");
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
            meltfnum[8] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:4495:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L21*/ meltfnum[8]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L22*/ meltfnum[21] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:4495:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L22*/ meltfnum[21];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4495;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "patexpand_object return spati";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SPATI__V61*/ meltfptr[50];
                            /*_.MELT_DEBUG_FUN__V67*/
                            meltfptr[66] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V66*/
                        meltfptr[65] = /*_.MELT_DEBUG_FUN__V67*/ meltfptr[66];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4495:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L22*/
                        meltfnum[21] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V67*/
                        meltfptr[66] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V66*/ meltfptr[65] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4495:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L21*/
            meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V66*/
            meltfptr[65] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:4496:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.SPATI__V61*/ meltfptr[50];;

        {
            MELT_LOCATION("warmelt-macro.melt:4496:/ locexp");
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
        /*_.MULTI___V63*/
        meltfptr[62] = /*_.RETURN___V68*/ meltfptr[66];;

        MELT_LOCATION("warmelt-macro.melt:4488:/ clear");
        /*clear*/ /*_#plI__L20*/
        meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MAKE_INTEGERBOX__V65*/
        meltfptr[64] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V68*/
        meltfptr[66] = 0 ;

        /*^clear*/
        /*clear*/ /*_#IMAX__L17*/
        meltfnum[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IMIN__L18*/
        meltfnum[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_#ISUM__L19*/
        meltfnum[9] = 0 ;
        /*_.LET___V56*/
        meltfptr[49] = /*_.MULTI___V63*/ meltfptr[62];;

        MELT_LOCATION("warmelt-macro.melt:4478:/ clear");
        /*clear*/ /*_.PATFIELDS__V57*/
        meltfptr[48] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LAMBDA___V58*/
        meltfptr[45] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SUBPATS__V60*/
        meltfptr[47] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SPATI__V61*/
        meltfptr[50] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MULTI___V63*/
        meltfptr[62] = 0 ;
        /*_.LET___V23*/
        meltfptr[22] = /*_.LET___V56*/ meltfptr[49];;

        MELT_LOCATION("warmelt-macro.melt:4440:/ clear");
        /*clear*/ /*_.BND__V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FLDLIST__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L7*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V40*/
        meltfptr[35] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V42*/
        meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V43*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V44*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V56*/
        meltfptr[49] = 0 ;
        /*_.LET___V13*/
        meltfptr[11] = /*_.LET___V23*/ meltfptr[22];;

        MELT_LOCATION("warmelt-macro.melt:4428:/ clear");
        /*clear*/ /*_.CONT__V14*/
        meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V15*/
        meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FIELDS__V16*/
        meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FIELDNAMS__V17*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CLANAME__V20*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CLA__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CLABIND__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L6*/
        meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V23*/
        meltfptr[22] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4423:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V13*/ meltfptr[11];;

        {
            MELT_LOCATION("warmelt-macro.melt:4423:/ locexp");
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
        /*clear*/ /*_.IFCPP___V7*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V9*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V13*/
        meltfptr[11] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("PATEXPAND_OBJECT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_67_WARMELTmiMACRO_PATEXPAND_OBJECT*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_68_WARMELTmiMACRO_LAMBDA_cl16(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_68_WARMELTmiMACRO_LAMBDA_cl16_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_68_WARMELTmiMACRO_LAMBDA_cl16 fromline 1716 */

    /** start of frame for meltrout_68_WARMELTmiMACRO_LAMBDA_cl16 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_68_WARMELTmiMACRO_LAMBDA_cl16// fromline 1531
        : public Melt_CallFrameWithValues<3>
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
        MeltFrame_meltrout_68_WARMELTmiMACRO_LAMBDA_cl16(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_68_WARMELTmiMACRO_LAMBDA_cl16), clos) {};
        MeltFrame_meltrout_68_WARMELTmiMACRO_LAMBDA_cl16() //the constructor fromline 1606
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_68_WARMELTmiMACRO_LAMBDA_cl16)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_68_WARMELTmiMACRO_LAMBDA_cl16(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_68_WARMELTmiMACRO_LAMBDA_cl16)) {};
        MeltFrame_meltrout_68_WARMELTmiMACRO_LAMBDA_cl16(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_68_WARMELTmiMACRO_LAMBDA_cl16), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_68_WARMELTmiMACRO_LAMBDA_cl16


    /** end of frame for meltrout_68_WARMELTmiMACRO_LAMBDA_cl16 fromline 1661**/

    /* end of frame for routine meltrout_68_WARMELTmiMACRO_LAMBDA_cl16 fromline 1720 */

    /* classy proc frame meltrout_68_WARMELTmiMACRO_LAMBDA_cl16 */
    MeltFrame_meltrout_68_WARMELTmiMACRO_LAMBDA_cl16
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_68_WARMELTmiMACRO_LAMBDA_cl16 fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
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
    MELT_LOCATION("warmelt-macro.melt:4481:/ getarg");
    /*_.CURPATFLD__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_LONG) goto meltlab_endgetargs;
    /*_#IX__L1*/
    meltfnum[0] = meltxargtab_[0].meltbp_long;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {

        MELT_LOCATION("warmelt-macro.melt:4482:/ cond");
        /*cond*/
        if (
            /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CURPATFLD__V2*/ meltfptr[1]),
                                          (melt_ptr_t)((/*!CLASS_SOURCE_FIELD_PATTERN*/ meltfrout->tabval[0])))
        ) /*then*/
            {
                /*^cond.then*/
                /*^getslot*/
                {
                    melt_ptr_t slot=NULL, obj=NULL;
                    obj = (melt_ptr_t)(/*_.CURPATFLD__V2*/ meltfptr[1]) /*=obj*/;
                    melt_object_get_field(slot,obj, 3, "SPAF_PATTERN");
                    /*_.SPAF_PATTERN__V3*/
                    meltfptr[2] = slot;
                };
                ;
            }
        else    /*^cond.else*/
            {

                /*_.SPAF_PATTERN__V3*/ meltfptr[2] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:4481:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.SPAF_PATTERN__V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-macro.melt:4481:/ locexp");
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
        /*clear*/ /*_.SPAF_PATTERN__V3*/
        meltfptr[2] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl16", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_68_WARMELTmiMACRO_LAMBDA_cl16_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_68_WARMELTmiMACRO_LAMBDA_cl16*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT fromline 1716 */

    /** start of frame for meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT// fromline 1531
        : public Melt_CallFrameWithValues<14>
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
        MeltFrame_meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<14> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT), clos) {};
        MeltFrame_meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT() //the constructor fromline 1606
            : Melt_CallFrameWithValues<14> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<14> (fil,lin, sizeof(MeltFrame_meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT)) {};
        MeltFrame_meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<14> (fil,lin, sizeof(MeltFrame_meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT


    /** end of frame for meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT fromline 1661**/

    /* end of frame for routine meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT fromline 1720 */

    /* classy proc frame meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT */
    MeltFrame_meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_OBJECT", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:4500:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4501:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4501:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4501:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4501)?(4501):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V7*/
                        meltfptr[6] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V6*/
            meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4501:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4502:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4502:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[8] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4502:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4502)?(4502):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V9*/
                        meltfptr[8] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[8];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4502:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/
            meltfptr[8] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4503:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4503:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4503:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L4*/ meltfnum[3];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4503;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_object sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V11*/
                            meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V10*/
                        meltfptr[8] = /*_.MELT_DEBUG_FUN__V11*/ meltfptr[10];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4503:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L4*/
                        meltfnum[3] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V11*/
                        meltfptr[10] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V10*/ meltfptr[8] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4503:/ locexp");
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
            /*clear*/ /*_._IF___V10*/
            meltfptr[8] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:4504:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:4505:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V13*/
            meltfptr[8] = slot;
        };
        ;

        {
            MELT_LOCATION("warmelt-macro.melt:4507:/ locexp");
            /* error_plain */
            melt_error_str((melt_ptr_t)(/*_.LOC__V13*/ meltfptr[8]), ( "OBJECT cannot be macro-expanded in expression context - use INSTANCE\
 instead"), (melt_ptr_t)0);
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:4508:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] =  /*fromline 1341*/ NULL ;;

        {
            MELT_LOCATION("warmelt-macro.melt:4508:/ locexp");
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
        /*_.LET___V12*/
        meltfptr[10] = /*_.RETURN___V14*/ meltfptr[13];;

        MELT_LOCATION("warmelt-macro.melt:4504:/ clear");
        /*clear*/ /*_.LOC__V13*/
        meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V14*/
        meltfptr[13] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4500:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V12*/ meltfptr[10];;

        {
            MELT_LOCATION("warmelt-macro.melt:4500:/ locexp");
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
        /*clear*/ /*_.IFCPP___V8*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V12*/
        meltfptr[10] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_OBJECT", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_69_WARMELTmiMACRO_MEXPAND_OBJECT*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS fromline 1716 */

    /** start of frame for meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS// fromline 1531
        : public Melt_CallFrameWithValues<28>
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
        MeltFrame_meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<28> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS), clos) {};
        MeltFrame_meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS() //the constructor fromline 1606
            : Melt_CallFrameWithValues<28> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<28> (fil,lin, sizeof(MeltFrame_meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS)) {};
        MeltFrame_meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<28> (fil,lin, sizeof(MeltFrame_meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS


    /** end of frame for meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS fromline 1661**/

    /* end of frame for routine meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS fromline 1720 */

    /* classy proc frame meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS */
    MeltFrame_meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("EXPAND_MACROSTRING_PAIRS", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:4525:/ getarg");
    /*_.PAIRS__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.SLOC__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.SLOC__V3*/ meltfptr[2])) != NULL);


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
        MELT_LOCATION("warmelt-macro.melt:4526:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4526:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4526:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4526;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "expand_macrostring_pairs pairs=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.PAIRS__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V8*/
                            meltfptr[7] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V7*/
                        meltfptr[6] = /*_.MELT_DEBUG_FUN__V8*/ meltfptr[7];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4526:/ clear");
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

                    /*_._IF___V7*/ meltfptr[6] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4526:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4527:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_PAIR__L3*/
            meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.PAIRS__V2*/ meltfptr[1])) == MELTOBMAG_PAIR);;
            MELT_LOCATION("warmelt-macro.melt:4527:/ cond");
            /*cond*/
            if (/*_#IS_PAIR__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4527:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check pairs"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4527)?(4527):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V10*/
                        meltfptr[6] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V9*/
            meltfptr[7] = /*_._IFELSE___V10*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4527:/ clear");
            /*clear*/ /*_#IS_PAIR__L3*/
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
        MELT_LOCATION("warmelt-macro.melt:4528:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V4*/ meltfptr[3]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4528:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[11] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4528:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4528)?(4528):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V12*/
                        meltfptr[11] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V11*/
            meltfptr[6] = /*_._IFELSE___V12*/ meltfptr[11];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4528:/ clear");
            /*clear*/ /*_#IS_A__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V12*/
            meltfptr[11] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V11*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:4529:/ quasiblock");


        /*_.CHKLIST__V14*/
        meltfptr[13] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
        MELT_LOCATION("warmelt-macro.melt:4531:/ blockmultialloc");
        /*multiallocblock*/
        {
            struct meltletrec_1_st
            {
                struct MELT_CLOSURE_STRUCT(5) rclo_0__MEXP_MACROSTRING;
                long meltletrec_1_endgap;
            } *meltletrec_1_ptr = 0;
            meltletrec_1_ptr = (struct meltletrec_1_st *) meltgc_allocate (sizeof (struct meltletrec_1_st), 0);
            /*^blockmultialloc.initfill*/
            /*iniclos rclo_0__MEXP_MACROSTRING*/
            /*_.MEXP_MACROSTRING__V16*/
            meltfptr[15] = (melt_ptr_t) &meltletrec_1_ptr->rclo_0__MEXP_MACROSTRING;
            meltletrec_1_ptr->rclo_0__MEXP_MACROSTRING.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE))));
            meltletrec_1_ptr->rclo_0__MEXP_MACROSTRING.nbval = 5;
            meltletrec_1_ptr->rclo_0__MEXP_MACROSTRING.rout = (meltroutine_ptr_t) ((/*!konst_8*/ meltfrout->tabval[8]));



            MELT_LOCATION("warmelt-macro.melt:4533:/ putclosurout");
            /*putclosurout#2*/
            melt_assertmsg("putclosrout#2 checkclo", melt_magic_discr((melt_ptr_t)(/*_.MEXP_MACROSTRING__V16*/ meltfptr[15])) == MELTOBMAG_CLOSURE);
            melt_assertmsg("putclosrout#2 checkrout", melt_magic_discr((melt_ptr_t)((/*!konst_8*/ meltfrout->tabval[8]))) == MELTOBMAG_ROUTINE);
            ((meltclosure_ptr_t)/*_.MEXP_MACROSTRING__V16*/ meltfptr[15])->rout = (meltroutine_ptr_t) ((/*!konst_8*/ meltfrout->tabval[8]));
            ;
            /*^putclosedv*/
            /*putclosv*/
            melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.MEXP_MACROSTRING__V16*/ meltfptr[15])) == MELTOBMAG_CLOSURE);
            melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.MEXP_MACROSTRING__V16*/ meltfptr[15])));
            ((meltclosure_ptr_t)/*_.MEXP_MACROSTRING__V16*/ meltfptr[15])->tabval[0] = (melt_ptr_t)(/*_.CHKLIST__V14*/ meltfptr[13]);
            ;
            /*^putclosedv*/
            /*putclosv*/
            melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.MEXP_MACROSTRING__V16*/ meltfptr[15])) == MELTOBMAG_CLOSURE);
            melt_assertmsg("putclosv checkoff", 1>= 0 && 1< melt_closure_size((melt_ptr_t) (/*_.MEXP_MACROSTRING__V16*/ meltfptr[15])));
            ((meltclosure_ptr_t)/*_.MEXP_MACROSTRING__V16*/ meltfptr[15])->tabval[1] = (melt_ptr_t)(/*_.MEXP_MACROSTRING__V16*/ meltfptr[15]);
            ;
            /*^putclosedv*/
            /*putclosv*/
            melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.MEXP_MACROSTRING__V16*/ meltfptr[15])) == MELTOBMAG_CLOSURE);
            melt_assertmsg("putclosv checkoff", 2>= 0 && 2< melt_closure_size((melt_ptr_t) (/*_.MEXP_MACROSTRING__V16*/ meltfptr[15])));
            ((meltclosure_ptr_t)/*_.MEXP_MACROSTRING__V16*/ meltfptr[15])->tabval[2] = (melt_ptr_t)(/*_.MEXPANDER__V5*/ meltfptr[4]);
            ;
            /*^putclosedv*/
            /*putclosv*/
            melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.MEXP_MACROSTRING__V16*/ meltfptr[15])) == MELTOBMAG_CLOSURE);
            melt_assertmsg("putclosv checkoff", 3>= 0 && 3< melt_closure_size((melt_ptr_t) (/*_.MEXP_MACROSTRING__V16*/ meltfptr[15])));
            ((meltclosure_ptr_t)/*_.MEXP_MACROSTRING__V16*/ meltfptr[15])->tabval[3] = (melt_ptr_t)(/*_.ENV__V4*/ meltfptr[3]);
            ;
            /*^putclosedv*/
            /*putclosv*/
            melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.MEXP_MACROSTRING__V16*/ meltfptr[15])) == MELTOBMAG_CLOSURE);
            melt_assertmsg("putclosv checkoff", 4>= 0 && 4< melt_closure_size((melt_ptr_t) (/*_.MEXP_MACROSTRING__V16*/ meltfptr[15])));
            ((meltclosure_ptr_t)/*_.MEXP_MACROSTRING__V16*/ meltfptr[15])->tabval[4] = (melt_ptr_t)(/*_.MODCTX__V6*/ meltfptr[5]);
            ;
            /*^touch*/
            meltgc_touch(/*_.MEXP_MACROSTRING__V16*/ meltfptr[15]);
            ;
            /*citerblock FOREACH_PAIR*/
            {
                /* start foreach_pair meltcit1__EACHPAIR */
                for (/*_.THISPAIR__V17*/ meltfptr[16] = /*_.PAIRS__V2*/ meltfptr[1];
                                         melt_magic_discr((melt_ptr_t) /*_.THISPAIR__V17*/ meltfptr[16]) == MELTOBMAG_PAIR;
                                         /*_.THISPAIR__V17*/ meltfptr[16] = melt_pair_tail((melt_ptr_t) /*_.THISPAIR__V17*/ meltfptr[16]))
                    {
                        /*_.THISCOMP__V18*/ meltfptr[17] = melt_pair_head((melt_ptr_t) /*_.THISPAIR__V17*/ meltfptr[16]);




#if MELT_HAVE_DEBUG
                        MELT_LOCATION("warmelt-macro.melt:4563:/ cppif.then");
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
                            meltfnum[1] =
                                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                ;;
                            MELT_LOCATION("warmelt-macro.melt:4563:/ cond");
                            /*cond*/
                            if (/*_#MELT_NEED_DBG__L5*/ meltfnum[1]) /*then*/
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
                                        MELT_LOCATION("warmelt-macro.melt:4563:/ apply");
                                        /*apply*/
                                        {
                                            union meltparam_un argtab[5];
                                            memset(&argtab, 0, sizeof(argtab));
                                            /*^apply.arg*/
                                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L6*/ meltfnum[0];
                                            /*^apply.arg*/
                                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                            /*^apply.arg*/
                                            argtab[2].meltbp_long = 4563;
                                            /*^apply.arg*/
                                            argtab[3].meltbp_cstring =  "expand_macrostring_pairs thiscomp=";
                                            /*^apply.arg*/
                                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.THISCOMP__V18*/ meltfptr[17];
                                            /*_.MELT_DEBUG_FUN__V20*/
                                            meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                        }
                                        ;
                                        /*_._IF___V19*/
                                        meltfptr[18] = /*_.MELT_DEBUG_FUN__V20*/ meltfptr[19];;
                                        /*epilog*/

                                        MELT_LOCATION("warmelt-macro.melt:4563:/ clear");
                                        /*clear*/ /*_#MELT_CALLCOUNT__L6*/
                                        meltfnum[0] = 0 ;
                                        /*^clear*/
                                        /*clear*/ /*_.MELT_DEBUG_FUN__V20*/
                                        meltfptr[19] = 0 ;
                                    }
                                    ;
                                }
                            else    /*^cond.else*/
                                {

                                    /*_._IF___V19*/ meltfptr[18] =  /*fromline 1341*/ NULL ;;
                                }
                            ;

                            {
                                MELT_LOCATION("warmelt-macro.melt:4563:/ locexp");
                                /*void*/
                                (void)0;
                            }
                            ;
                            /*^quasiblock*/


                            /*epilog*/

                            /*^clear*/
                            /*clear*/ /*_#MELT_NEED_DBG__L5*/
                            meltfnum[1] = 0 ;
                            /*^clear*/
                            /*clear*/ /*_._IF___V19*/
                            meltfptr[18] = 0 ;
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
                        MELT_LOCATION("warmelt-macro.melt:4564:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[1];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.SLOC__V3*/ meltfptr[2];
                            /*_.MEXP_MACROSTRING__V21*/
                            meltfptr[19] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXP_MACROSTRING__V16*/ meltfptr[15]), (melt_ptr_t)(/*_.THISCOMP__V18*/ meltfptr[17]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                    } /* ending  foreach_pair meltcit1__EACHPAIR */
                /*_.THISPAIR__V17*/ meltfptr[16] = NULL;
                /*_.THISCOMP__V18*/
                meltfptr[17] = NULL;


                /*citerepilog*/

                MELT_LOCATION("warmelt-macro.melt:4560:/ clear");
                /*clear*/ /*_.THISPAIR__V17*/
                meltfptr[16] = 0 ;
                /*^clear*/
                /*clear*/ /*_.THISCOMP__V18*/
                meltfptr[17] = 0 ;
                /*^clear*/
                /*clear*/ /*_.MEXP_MACROSTRING__V21*/
                meltfptr[19] = 0 ;
            } /*endciterblock FOREACH_PAIR*/
            ;

#if MELT_HAVE_DEBUG
            MELT_LOCATION("warmelt-macro.melt:4566:/ cppif.then");
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
                MELT_LOCATION("warmelt-macro.melt:4566:/ cond");
                /*cond*/
                if (/*_#MELT_NEED_DBG__L7*/ meltfnum[0]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*_#MELT_CALLCOUNT__L8*/ meltfnum[1] =
                                /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                meltcallcount  /* melt_callcount debugging */
#else
                                0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                ;;

                            MELT_CHECK_SIGNAL();
                            ;
                            MELT_LOCATION("warmelt-macro.melt:4566:/ apply");
                            /*apply*/
                            {
                                union meltparam_un argtab[5];
                                memset(&argtab, 0, sizeof(argtab));
                                /*^apply.arg*/
                                argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[1];
                                /*^apply.arg*/
                                argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                /*^apply.arg*/
                                argtab[2].meltbp_long = 4566;
                                /*^apply.arg*/
                                argtab[3].meltbp_cstring =  "expand_macrostring_pairs final chklist=";
                                /*^apply.arg*/
                                argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CHKLIST__V14*/ meltfptr[13];
                                /*_.MELT_DEBUG_FUN__V23*/
                                meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                            }
                            ;
                            /*_._IF___V22*/
                            meltfptr[18] = /*_.MELT_DEBUG_FUN__V23*/ meltfptr[22];;
                            /*epilog*/

                            MELT_LOCATION("warmelt-macro.melt:4566:/ clear");
                            /*clear*/ /*_#MELT_CALLCOUNT__L8*/
                            meltfnum[1] = 0 ;
                            /*^clear*/
                            /*clear*/ /*_.MELT_DEBUG_FUN__V23*/
                            meltfptr[22] = 0 ;
                        }
                        ;
                    }
                else    /*^cond.else*/
                    {

                        /*_._IF___V22*/ meltfptr[18] =  /*fromline 1341*/ NULL ;;
                    }
                ;

                {
                    MELT_LOCATION("warmelt-macro.melt:4566:/ locexp");
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
                /*clear*/ /*_._IF___V22*/
                meltfptr[18] = 0 ;
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
            MELT_LOCATION("warmelt-macro.melt:4567:/ quasiblock");



            MELT_CHECK_SIGNAL();
            ;
            /*^apply*/
            /*apply*/
            {
                union meltparam_un argtab[1];
                memset(&argtab, 0, sizeof(argtab));
                /*^apply.arg*/
                argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[10]);
                /*_.TUP__V25*/
                meltfptr[18] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[9])), (melt_ptr_t)(/*_.CHKLIST__V14*/ meltfptr[13]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
            }
            ;

#if MELT_HAVE_DEBUG
            MELT_LOCATION("warmelt-macro.melt:4569:/ cppif.then");
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
                meltfnum[1] =
                    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                    (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                    0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                    ;;
                MELT_LOCATION("warmelt-macro.melt:4569:/ cond");
                /*cond*/
                if (/*_#MELT_NEED_DBG__L9*/ meltfnum[1]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*_#MELT_CALLCOUNT__L10*/ meltfnum[0] =
                                /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                meltcallcount  /* melt_callcount debugging */
#else
                                0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                ;;

                            MELT_CHECK_SIGNAL();
                            ;
                            MELT_LOCATION("warmelt-macro.melt:4569:/ apply");
                            /*apply*/
                            {
                                union meltparam_un argtab[5];
                                memset(&argtab, 0, sizeof(argtab));
                                /*^apply.arg*/
                                argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L10*/ meltfnum[0];
                                /*^apply.arg*/
                                argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                /*^apply.arg*/
                                argtab[2].meltbp_long = 4569;
                                /*^apply.arg*/
                                argtab[3].meltbp_cstring =  "expand_macrostring_pairs return tup=";
                                /*^apply.arg*/
                                argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.TUP__V25*/ meltfptr[18];
                                /*_.MELT_DEBUG_FUN__V27*/
                                meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                            }
                            ;
                            /*_._IF___V26*/
                            meltfptr[25] = /*_.MELT_DEBUG_FUN__V27*/ meltfptr[26];;
                            /*epilog*/

                            MELT_LOCATION("warmelt-macro.melt:4569:/ clear");
                            /*clear*/ /*_#MELT_CALLCOUNT__L10*/
                            meltfnum[0] = 0 ;
                            /*^clear*/
                            /*clear*/ /*_.MELT_DEBUG_FUN__V27*/
                            meltfptr[26] = 0 ;
                        }
                        ;
                    }
                else    /*^cond.else*/
                    {

                        /*_._IF___V26*/ meltfptr[25] =  /*fromline 1341*/ NULL ;;
                    }
                ;

                {
                    MELT_LOCATION("warmelt-macro.melt:4569:/ locexp");
                    /*void*/
                    (void)0;
                }
                ;
                /*^quasiblock*/


                /*epilog*/

                /*^clear*/
                /*clear*/ /*_#MELT_NEED_DBG__L9*/
                meltfnum[1] = 0 ;
                /*^clear*/
                /*clear*/ /*_._IF___V26*/
                meltfptr[25] = 0 ;
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
            MELT_LOCATION("warmelt-macro.melt:4570:/ quasiblock");


            /*_._RETVAL___V1*/
            meltfptr[0] = /*_.TUP__V25*/ meltfptr[18];;

            {
                MELT_LOCATION("warmelt-macro.melt:4570:/ locexp");
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
            /*_.LET___V24*/
            meltfptr[22] = /*_.RETURN___V28*/ meltfptr[26];;

            MELT_LOCATION("warmelt-macro.melt:4567:/ clear");
            /*clear*/ /*_.TUP__V25*/
            meltfptr[18] = 0 ;
            /*^clear*/
            /*clear*/ /*_.RETURN___V28*/
            meltfptr[26] = 0 ;
            /*_.LETREC___V15*/
            meltfptr[14] = /*_.LET___V24*/ meltfptr[22];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4531:/ clear");
            /*clear*/ /*_.MEXP_MACROSTRING__V16*/
            meltfptr[15] = 0 ;
            /*^clear*/
            /*clear*/ /*_.MEXP_MACROSTRING__V16*/
            meltfptr[15] = 0 ;
            /*^clear*/
            /*clear*/ /*_.LET___V24*/
            meltfptr[22] = 0 ;
        } /*end multiallocblock*/
        ;
        /*_.LET___V13*/
        meltfptr[11] = /*_.LETREC___V15*/ meltfptr[14];;

        MELT_LOCATION("warmelt-macro.melt:4529:/ clear");
        /*clear*/ /*_.CHKLIST__V14*/
        meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LETREC___V15*/
        meltfptr[14] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4525:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V13*/ meltfptr[11];;

        {
            MELT_LOCATION("warmelt-macro.melt:4525:/ locexp");
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
        /*clear*/ /*_.IFCPP___V11*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V13*/
        meltfptr[11] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("EXPAND_MACROSTRING_PAIRS", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_70_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_71_WARMELTmiMACRO_LAMBDA_cl17(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_71_WARMELTmiMACRO_LAMBDA_cl17_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_71_WARMELTmiMACRO_LAMBDA_cl17 fromline 1716 */

    /** start of frame for meltrout_71_WARMELTmiMACRO_LAMBDA_cl17 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_71_WARMELTmiMACRO_LAMBDA_cl17// fromline 1531
        : public Melt_CallFrameWithValues<20>
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
        MeltFrame_meltrout_71_WARMELTmiMACRO_LAMBDA_cl17(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<20> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_71_WARMELTmiMACRO_LAMBDA_cl17), clos) {};
        MeltFrame_meltrout_71_WARMELTmiMACRO_LAMBDA_cl17() //the constructor fromline 1606
            : Melt_CallFrameWithValues<20> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_71_WARMELTmiMACRO_LAMBDA_cl17)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_71_WARMELTmiMACRO_LAMBDA_cl17(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<20> (fil,lin, sizeof(MeltFrame_meltrout_71_WARMELTmiMACRO_LAMBDA_cl17)) {};
        MeltFrame_meltrout_71_WARMELTmiMACRO_LAMBDA_cl17(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<20> (fil,lin, sizeof(MeltFrame_meltrout_71_WARMELTmiMACRO_LAMBDA_cl17), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_71_WARMELTmiMACRO_LAMBDA_cl17


    /** end of frame for meltrout_71_WARMELTmiMACRO_LAMBDA_cl17 fromline 1661**/

    /* end of frame for routine meltrout_71_WARMELTmiMACRO_LAMBDA_cl17 fromline 1720 */

    /* classy proc frame meltrout_71_WARMELTmiMACRO_LAMBDA_cl17 */
    MeltFrame_meltrout_71_WARMELTmiMACRO_LAMBDA_cl17
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_71_WARMELTmiMACRO_LAMBDA_cl17 fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
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
    MELT_LOCATION("warmelt-macro.melt:4533:/ getarg");
    /*_.COMP__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;

    /*getarg#1*/
    /*^getarg*/
    if (meltxargdescr_[0] != MELTBPAR_PTR) goto meltlab_endgetargs;
    /*_.LOC__V3*/
    meltfptr[2] = (meltxargtab_[0].meltbp_aptr) ? (*(meltxargtab_[0].meltbp_aptr)) : NULL;
    gcc_assert(melt_discr((melt_ptr_t)(/*_.LOC__V3*/ meltfptr[2])) != NULL);

    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4534:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4534:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4534:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4534;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "expand_macrostring_pairs/mexp_macrostring comp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.COMP__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n loc=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &/*_.LOC__V3*/ meltfptr[2];
                            /*_.MELT_DEBUG_FUN__V5*/
                            meltfptr[4] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V4*/
                        meltfptr[3] = /*_.MELT_DEBUG_FUN__V5*/ meltfptr[4];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4534:/ clear");
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

                    /*_._IF___V4*/ meltfptr[3] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4534:/ locexp");
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

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_STRING__L3*/
        meltfnum[1] =
            (melt_magic_discr((melt_ptr_t)(/*_.COMP__V2*/ meltfptr[1])) == MELTOBMAG_STRING);;
        MELT_LOCATION("warmelt-macro.melt:4536:/ cond");
        /*cond*/
        if (/*_#IS_STRING__L3*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*_.MAKE_STRING__V6*/ meltfptr[4] =
                        (meltgc_new_stringdup((meltobject_ptr_t)((/*!DISCR_VERBATIM_STRING*/ meltfrout->tabval[1])),
                                              melt_string_str((melt_ptr_t)(/*_.COMP__V2*/ meltfptr[1]))));;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4537:/ locexp");
                        meltgc_append_list((melt_ptr_t)((/*~CHKLIST*/ meltfclos->tabval[0])), (melt_ptr_t)(/*_.MAKE_STRING__V6*/ meltfptr[4]));
                    }
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4536:/ clear");
                    /*clear*/ /*_.MAKE_STRING__V6*/
                    meltfptr[4] = 0 ;
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
                    /*_#IS_A__L4*/
                    meltfnum[0] =
                        melt_is_instance_of((melt_ptr_t)(/*_.COMP__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR_MACROSTRING*/ meltfrout->tabval[2])));;
                    MELT_LOCATION("warmelt-macro.melt:4538:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:4539:/ quasiblock");


                                MELT_LOCATION("warmelt-macro.melt:4540:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.COMP__V2*/ meltfptr[1]),
                                                                  (melt_ptr_t)((/*!CLASS_LOCATED*/ meltfrout->tabval[3])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.COMP__V2*/ meltfptr[1]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
                                            /*_.CLOC__V7*/
                                            meltfptr[3] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.CLOC__V7*/ meltfptr[3] =  /*fromline 1341*/ NULL ;;
                                    }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:4541:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.COMP__V2*/ meltfptr[1]),
                                                                  (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[4])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.COMP__V2*/ meltfptr[1]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
                                            /*_.COMPCONT__V8*/
                                            meltfptr[4] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.COMPCONT__V8*/ meltfptr[4] =  /*fromline 1341*/ NULL ;;
                                    }
                                ;

#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-macro.melt:4543:/ cppif.then");
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
                                    meltfnum[4] =
                                        /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                        (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                        0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                        ;;
                                    MELT_LOCATION("warmelt-macro.melt:4543:/ cond");
                                    /*cond*/
                                    if (/*_#MELT_NEED_DBG__L5*/ meltfnum[4]) /*then*/
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
                                                MELT_LOCATION("warmelt-macro.melt:4543:/ apply");
                                                /*apply*/
                                                {
                                                    union meltparam_un argtab[5];
                                                    memset(&argtab, 0, sizeof(argtab));
                                                    /*^apply.arg*/
                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L6*/ meltfnum[5];
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 4543;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_cstring =  "expand_macrostring_pairs/mexp_macrostring compcont=";
                                                    /*^apply.arg*/
                                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.COMPCONT__V8*/ meltfptr[4];
                                                    /*_.MELT_DEBUG_FUN__V10*/
                                                    meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IF___V9*/
                                                meltfptr[8] = /*_.MELT_DEBUG_FUN__V10*/ meltfptr[9];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-macro.melt:4543:/ clear");
                                                /*clear*/ /*_#MELT_CALLCOUNT__L6*/
                                                meltfnum[5] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_.MELT_DEBUG_FUN__V10*/
                                                meltfptr[9] = 0 ;
                                            }
                                            ;
                                        }
                                    else    /*^cond.else*/
                                        {

                                            /*_._IF___V9*/ meltfptr[8] =  /*fromline 1341*/ NULL ;;
                                        }
                                    ;

                                    {
                                        MELT_LOCATION("warmelt-macro.melt:4543:/ locexp");
                                        /*void*/
                                        (void)0;
                                    }
                                    ;
                                    /*^quasiblock*/


                                    /*epilog*/

                                    /*^clear*/
                                    /*clear*/ /*_#MELT_NEED_DBG__L5*/
                                    meltfnum[4] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IF___V9*/
                                    meltfptr[8] = 0 ;
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
                                /*citerblock FOREACH_IN_LIST*/
                                {
                                    /* start foreach_in_list meltcit1__EACHLIST */
                                    for (/*_.COMPAIR__V11*/ meltfptr[9] = melt_list_first( (melt_ptr_t)/*_.COMPCONT__V8*/ meltfptr[4]);
                                                            melt_magic_discr((melt_ptr_t) /*_.COMPAIR__V11*/ meltfptr[9]) == MELTOBMAG_PAIR;
                                                            /*_.COMPAIR__V11*/ meltfptr[9] = melt_pair_tail((melt_ptr_t) /*_.COMPAIR__V11*/ meltfptr[9]))
                                        {
                                            /*_.SUBCOMP__V12*/ meltfptr[8] = melt_pair_head((melt_ptr_t) /*_.COMPAIR__V11*/ meltfptr[9]);



                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-macro.melt:4547:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[1];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CLOC__V7*/ meltfptr[3];
                                                /*_.MEXP_MACROSTRING__V13*/
                                                meltfptr[12] =  melt_apply ((meltclosure_ptr_t)((/*~MEXP_MACROSTRING*/ meltfclos->tabval[1])), (melt_ptr_t)(/*_.SUBCOMP__V12*/ meltfptr[8]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;
                                        } /* end foreach_in_list meltcit1__EACHLIST */
                                    /*_.COMPAIR__V11*/ meltfptr[9] = NULL;
                                    /*_.SUBCOMP__V12*/
                                    meltfptr[8] = NULL;


                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-macro.melt:4544:/ clear");
                                    /*clear*/ /*_.COMPAIR__V11*/
                                    meltfptr[9] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.SUBCOMP__V12*/
                                    meltfptr[8] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MEXP_MACROSTRING__V13*/
                                    meltfptr[12] = 0 ;
                                } /*endciterblock FOREACH_IN_LIST*/
                                ;

#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-macro.melt:4548:/ cppif.then");
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
                                    meltfnum[5] =
                                        /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                        (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                        0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                        ;;
                                    MELT_LOCATION("warmelt-macro.melt:4548:/ cond");
                                    /*cond*/
                                    if (/*_#MELT_NEED_DBG__L7*/ meltfnum[5]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*^block*/
                                            /*anyblock*/
                                            {

                                                /*_#MELT_CALLCOUNT__L8*/ meltfnum[4] =
                                                    /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                                    meltcallcount  /* melt_callcount debugging */
#else
                                                    0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                                    ;;

                                                MELT_CHECK_SIGNAL();
                                                ;
                                                MELT_LOCATION("warmelt-macro.melt:4548:/ apply");
                                                /*apply*/
                                                {
                                                    union meltparam_un argtab[5];
                                                    memset(&argtab, 0, sizeof(argtab));
                                                    /*^apply.arg*/
                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[4];
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 4548;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_cstring =  "expand_macrostring_pairs/mexp_macrostring updated chklist=";
                                                    /*^apply.arg*/
                                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &(/*~CHKLIST*/ meltfclos->tabval[0]);
                                                    /*_.MELT_DEBUG_FUN__V15*/
                                                    meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IF___V14*/
                                                meltfptr[13] = /*_.MELT_DEBUG_FUN__V15*/ meltfptr[14];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-macro.melt:4548:/ clear");
                                                /*clear*/ /*_#MELT_CALLCOUNT__L8*/
                                                meltfnum[4] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_.MELT_DEBUG_FUN__V15*/
                                                meltfptr[14] = 0 ;
                                            }
                                            ;
                                        }
                                    else    /*^cond.else*/
                                        {

                                            /*_._IF___V14*/ meltfptr[13] =  /*fromline 1341*/ NULL ;;
                                        }
                                    ;

                                    {
                                        MELT_LOCATION("warmelt-macro.melt:4548:/ locexp");
                                        /*void*/
                                        (void)0;
                                    }
                                    ;
                                    /*^quasiblock*/


                                    /*epilog*/

                                    /*^clear*/
                                    /*clear*/ /*_#MELT_NEED_DBG__L7*/
                                    meltfnum[5] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IF___V14*/
                                    meltfptr[13] = 0 ;
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

                                MELT_LOCATION("warmelt-macro.melt:4539:/ clear");
                                /*clear*/ /*_.CLOC__V7*/
                                meltfptr[3] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.COMPCONT__V8*/
                                meltfptr[4] = 0 ;
                                /*epilog*/
                            }
                            ;
                        }
                    else
                        {
                            MELT_LOCATION("warmelt-macro.melt:4538:/ cond.else");

                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                /*_#IS_A__L9*/
                                meltfnum[4] =
                                    melt_is_instance_of((melt_ptr_t)(/*_.COMP__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[4])));;
                                MELT_LOCATION("warmelt-macro.melt:4550:/ cond");
                                /*cond*/
                                if (/*_#IS_A__L9*/ meltfnum[4]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-macro.melt:4551:/ quasiblock");



                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*^apply*/
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[3];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*~ENV*/ meltfclos->tabval[3]);
                                                /*^apply.arg*/
                                                argtab[1].meltbp_aptr = (melt_ptr_t*) &(/*~MEXPANDER*/ meltfclos->tabval[2]);
                                                /*^apply.arg*/
                                                argtab[2].meltbp_aptr = (melt_ptr_t*) &(/*~MODCTX*/ meltfclos->tabval[4]);
                                                /*_.MCOMP__V16*/
                                                meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*~MEXPANDER*/ meltfclos->tabval[2])), (melt_ptr_t)(/*_.COMP__V2*/ meltfptr[1]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;

#if MELT_HAVE_DEBUG
                                            MELT_LOCATION("warmelt-macro.melt:4553:/ cppif.then");
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
                                                meltfnum[5] =
                                                    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                                    (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                                    0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                                    ;;
                                                MELT_LOCATION("warmelt-macro.melt:4553:/ cond");
                                                /*cond*/
                                                if (/*_#MELT_NEED_DBG__L10*/ meltfnum[5]) /*then*/
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
                                                            MELT_LOCATION("warmelt-macro.melt:4553:/ apply");
                                                            /*apply*/
                                                            {
                                                                union meltparam_un argtab[5];
                                                                memset(&argtab, 0, sizeof(argtab));
                                                                /*^apply.arg*/
                                                                argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[10];
                                                                /*^apply.arg*/
                                                                argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                                                /*^apply.arg*/
                                                                argtab[2].meltbp_long = 4553;
                                                                /*^apply.arg*/
                                                                argtab[3].meltbp_cstring =  "expand_macrostring_pairs//mexp_macrostring mcomp=";
                                                                /*^apply.arg*/
                                                                argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MCOMP__V16*/ meltfptr[14];
                                                                /*_.MELT_DEBUG_FUN__V18*/
                                                                meltfptr[3] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                            }
                                                            ;
                                                            /*_._IF___V17*/
                                                            meltfptr[13] = /*_.MELT_DEBUG_FUN__V18*/ meltfptr[3];;
                                                            /*epilog*/

                                                            MELT_LOCATION("warmelt-macro.melt:4553:/ clear");
                                                            /*clear*/ /*_#MELT_CALLCOUNT__L11*/
                                                            meltfnum[10] = 0 ;
                                                            /*^clear*/
                                                            /*clear*/ /*_.MELT_DEBUG_FUN__V18*/
                                                            meltfptr[3] = 0 ;
                                                        }
                                                        ;
                                                    }
                                                else    /*^cond.else*/
                                                    {

                                                        /*_._IF___V17*/ meltfptr[13] =  /*fromline 1341*/ NULL ;;
                                                    }
                                                ;

                                                {
                                                    MELT_LOCATION("warmelt-macro.melt:4553:/ locexp");
                                                    /*void*/
                                                    (void)0;
                                                }
                                                ;
                                                /*^quasiblock*/


                                                /*epilog*/

                                                /*^clear*/
                                                /*clear*/ /*_#MELT_NEED_DBG__L10*/
                                                meltfnum[5] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_._IF___V17*/
                                                meltfptr[13] = 0 ;
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
                                                MELT_LOCATION("warmelt-macro.melt:4554:/ locexp");
                                                meltgc_append_list((melt_ptr_t)((/*~CHKLIST*/ meltfclos->tabval[0])), (melt_ptr_t)(/*_.MCOMP__V16*/ meltfptr[14]));
                                            }
                                            ;

                                            MELT_LOCATION("warmelt-macro.melt:4551:/ clear");
                                            /*clear*/ /*_.MCOMP__V16*/
                                            meltfptr[14] = 0 ;
                                            /*epilog*/
                                        }
                                        ;
                                    }
                                else
                                    {
                                        MELT_LOCATION("warmelt-macro.melt:4550:/ cond.else");

                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            {
                                                MELT_LOCATION("warmelt-macro.melt:4556:/ locexp");
                                                meltgc_append_list((melt_ptr_t)((/*~CHKLIST*/ meltfclos->tabval[0])), (melt_ptr_t)(/*_.COMP__V2*/ meltfptr[1]));
                                            }
                                            ;
                                            MELT_LOCATION("warmelt-macro.melt:4555:/ quasiblock");


                                            /*epilog*/
                                        }
                                        ;
                                    }
                                ;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:4538:/ clear");
                                /*clear*/ /*_#IS_A__L9*/
                                meltfnum[4] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4536:/ clear");
                    /*clear*/ /*_#IS_A__L4*/
                    meltfnum[0] = 0 ;
                }
                ;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4557:/ cppif.then");
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
            meltfnum[10] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:4557:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L12*/ meltfnum[10]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L13*/ meltfnum[5] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:4557:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L13*/ meltfnum[5];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4557;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "expand_macrostring_pairs/mexp_macrostring comp=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.COMP__V2*/ meltfptr[1];
                            /*^apply.arg*/
                            argtab[5].meltbp_cstring =  "\n updated chklist=";
                            /*^apply.arg*/
                            argtab[6].meltbp_aptr = (melt_ptr_t*) &(/*~CHKLIST*/ meltfclos->tabval[0]);
                            /*_.MELT_DEBUG_FUN__V20*/
                            meltfptr[3] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V19*/
                        meltfptr[4] = /*_.MELT_DEBUG_FUN__V20*/ meltfptr[3];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4557:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L13*/
                        meltfnum[5] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V20*/
                        meltfptr[3] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V19*/ meltfptr[4] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4557:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L12*/
            meltfnum[10] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V19*/
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

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-macro.melt:4533:/ clear");
        /*clear*/ /*_#IS_STRING__L3*/
        meltfnum[1] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl17", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_71_WARMELTmiMACRO_LAMBDA_cl17_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_71_WARMELTmiMACRO_LAMBDA_cl17*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK fromline 1716 */

    /** start of frame for meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK// fromline 1531
        : public Melt_CallFrameWithValues<32>
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
        MeltFrame_meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<32> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK), clos) {};
        MeltFrame_meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK() //the constructor fromline 1606
            : Melt_CallFrameWithValues<32> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<32> (fil,lin, sizeof(MeltFrame_meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK)) {};
        MeltFrame_meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<32> (fil,lin, sizeof(MeltFrame_meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK


    /** end of frame for meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK fromline 1661**/

    /* end of frame for routine meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK fromline 1720 */

    /* classy proc frame meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK */
    MeltFrame_meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_CODE_CHUNK", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:4575:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4576:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4576:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4576:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4576;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_code_chunk sexpr=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4576:/ clear");
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

                    /*_._IF___V6*/ meltfptr[5] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4576:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4577:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4577:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4577:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4577)?(4577):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V9*/
                        meltfptr[5] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4577:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4578:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:4578:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[10] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4578:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4578)?(4578):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V11*/
                        meltfptr[10] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V10*/
            meltfptr[5] = /*_._IFELSE___V11*/ meltfptr[10];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4578:/ clear");
            /*clear*/ /*_#IS_A__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V11*/
            meltfptr[10] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V10*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:4579:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V13*/
            meltfptr[12] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4580:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.SLOC__V14*/
            meltfptr[13] = slot;
        };
        ;
        /*_.LIST_FIRST__V15*/
        meltfptr[14] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V13*/ meltfptr[12])));;
        /*^compute*/
        /*_.CURPAIR__V16*/
        meltfptr[15] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V15*/ meltfptr[14])));;
        /*^compute*/
        /*_.CURCHK__V17*/
        meltfptr[16] = (/*nil*/NULL);;
        /*^compute*/
        /*_.GSYM__V18*/
        meltfptr[17] = (/*nil*/NULL);;
        /*^compute*/
        /*_.CUREL__V19*/
        meltfptr[18] = (/*nil*/NULL);;
        /*^compute*/
        /*_.PAIR_HEAD__V20*/
        meltfptr[19] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;
        MELT_LOCATION("warmelt-macro.melt:4586:/ compute");
        /*_.CUREL__V19*/
        meltfptr[18] = /*_.SETQ___V21*/ meltfptr[20] = /*_.PAIR_HEAD__V20*/ meltfptr[19];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_A__L5*/
        meltfnum[1] =
            melt_is_instance_of((melt_ptr_t)(/*_.CUREL__V19*/ meltfptr[18]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[3])));;
        MELT_LOCATION("warmelt-macro.melt:4587:/ cond");
        /*cond*/
        if (/*_#IS_A__L5*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:4588:/ compute");
                    /*_.GSYM__V18*/
                    meltfptr[17] = /*_.SETQ___V23*/ meltfptr[22] = /*_.CUREL__V19*/ meltfptr[18];;
                    /*_._IFELSE___V22*/
                    meltfptr[21] = /*_.SETQ___V23*/ meltfptr[22];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4587:/ clear");
                    /*clear*/ /*_.SETQ___V23*/
                    meltfptr[22] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-macro.melt:4589:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.SLOC__V14*/ meltfptr[13]), ( "missing symbol in (CODE_CHUNK <state-symbol> <chunk>...)"), (melt_ptr_t)0);
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V22*/
                    meltfptr[21] = 0 ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V24*/
        meltfptr[22] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;
        MELT_LOCATION("warmelt-macro.melt:4590:/ compute");
        /*_.CURPAIR__V16*/
        meltfptr[15] = /*_.SETQ___V25*/ meltfptr[24] = /*_.PAIR_TAIL__V24*/ meltfptr[22];;
        MELT_LOCATION("warmelt-macro.melt:4591:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4592:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.SLOC__V14*/ meltfptr[13];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.TUP__V27*/
            meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!EXPAND_MACROSTRING_PAIRS*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4593:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_CODECHUNK*/ meltfrout->tabval[5])), (4), "CLASS_SOURCE_CODECHUNK");
            /*_.INST__V29*/
            meltfptr[28] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V29*/ meltfptr[28])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V29*/ meltfptr[28]), (1), (/*_.SLOC__V14*/ meltfptr[13]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SCH_GENSYM", melt_magic_discr((melt_ptr_t)(/*_.INST__V29*/ meltfptr[28])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V29*/ meltfptr[28]), (2), (/*_.GSYM__V18*/ meltfptr[17]), "SCH_GENSYM");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SCH_CHUNKS", melt_magic_discr((melt_ptr_t)(/*_.INST__V29*/ meltfptr[28])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V29*/ meltfptr[28]), (3), (/*_.TUP__V27*/ meltfptr[26]), "SCH_CHUNKS");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V29*/ meltfptr[28], "newly made instance");
        ;
        /*_.MRES__V28*/
        meltfptr[27] = /*_.INST__V29*/ meltfptr[28];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4599:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4599:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4599:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[6];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4599;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_code_chunk return mres=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MRES__V28*/ meltfptr[27];
                            /*_.MELT_DEBUG_FUN__V31*/
                            meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V30*/
                        meltfptr[29] = /*_.MELT_DEBUG_FUN__V31*/ meltfptr[30];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4599:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L7*/
                        meltfnum[6] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V31*/
                        meltfptr[30] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V30*/ meltfptr[29] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4599:/ locexp");
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
            /*clear*/ /*_._IF___V30*/
            meltfptr[29] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:4600:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.MRES__V28*/ meltfptr[27];;

        {
            MELT_LOCATION("warmelt-macro.melt:4600:/ locexp");
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
        /*_.LET___V26*/
        meltfptr[25] = /*_.RETURN___V32*/ meltfptr[30];;

        MELT_LOCATION("warmelt-macro.melt:4591:/ clear");
        /*clear*/ /*_.TUP__V27*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MRES__V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V32*/
        meltfptr[30] = 0 ;
        /*_.LET___V12*/
        meltfptr[10] = /*_.LET___V26*/ meltfptr[25];;

        MELT_LOCATION("warmelt-macro.melt:4579:/ clear");
        /*clear*/ /*_.CONT__V13*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SLOC__V14*/
        meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V15*/
        meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V16*/
        meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURCHK__V17*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.GSYM__V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CUREL__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_HEAD__V20*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L5*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V22*/
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
        MELT_LOCATION("warmelt-macro.melt:4575:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V12*/ meltfptr[10];;

        {
            MELT_LOCATION("warmelt-macro.melt:4575:/ locexp");
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
        /*clear*/ /*_.IFCPP___V10*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V12*/
        meltfptr[10] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_CODE_CHUNK", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_72_WARMELTmiMACRO_MEXPAND_CODE_CHUNK*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK fromline 1716 */

    /** start of frame for meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK// fromline 1531
        : public Melt_CallFrameWithValues<58>
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
        MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<58> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK), clos) {};
        MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK() //the constructor fromline 1606
            : Melt_CallFrameWithValues<58> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<58> (fil,lin, sizeof(MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK)) {};
        MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<58> (fil,lin, sizeof(MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK


    /** end of frame for meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK fromline 1661**/

    /* end of frame for routine meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK fromline 1720 */

    /* classy proc frame meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK */
    MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
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
    MELT_LOCATION("warmelt-macro.melt:4614:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4615:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4615:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4615:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4615;
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

                        MELT_LOCATION("warmelt-macro.melt:4615:/ clear");
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

                    /*_._IF___V6*/ meltfptr[5] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4615:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4616:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4616:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4616:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4616)?(4616):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V9*/
                        meltfptr[5] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4616:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4617:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:4617:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[10] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4617:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4617)?(4617):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V11*/
                        meltfptr[10] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V10*/
            meltfptr[5] = /*_._IFELSE___V11*/ meltfptr[10];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4617:/ clear");
            /*clear*/ /*_#IS_A__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V11*/
            meltfptr[10] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V10*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:4618:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V13*/
            meltfptr[12] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4619:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.SLOC__V14*/
            meltfptr[13] = slot;
        };
        ;
        /*_.LIST_FIRST__V15*/
        meltfptr[14] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V13*/ meltfptr[12])));;
        /*^compute*/
        /*_.CURPAIR__V16*/
        meltfptr[15] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V15*/ meltfptr[14])));;
        /*^compute*/
        /*_.CURCHK__V17*/
        meltfptr[16] = (/*nil*/NULL);;
        /*^compute*/
        /*_.GSYM__V18*/
        meltfptr[17] = (/*nil*/NULL);;
        /*^compute*/
        /*_.KCTY__V19*/
        meltfptr[18] = (/*nil*/NULL);;
        /*^compute*/
        /*_.CUREL__V20*/
        meltfptr[19] = (/*nil*/NULL);;
        /*^compute*/
        /*_.PAIR_HEAD__V21*/
        meltfptr[20] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;
        MELT_LOCATION("warmelt-macro.melt:4626:/ compute");
        /*_.CUREL__V20*/
        meltfptr[19] = /*_.SETQ___V22*/ meltfptr[21] = /*_.PAIR_HEAD__V21*/ meltfptr[20];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_A__L5*/
        meltfnum[1] =
            melt_is_instance_of((melt_ptr_t)(/*_.CUREL__V20*/ meltfptr[19]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[3])));;
        MELT_LOCATION("warmelt-macro.melt:4627:/ cond");
        /*cond*/
        if (/*_#IS_A__L5*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:4628:/ compute");
                    /*_.GSYM__V18*/
                    meltfptr[17] = /*_.SETQ___V24*/ meltfptr[23] = /*_.CUREL__V20*/ meltfptr[19];;
                    /*_._IFELSE___V23*/
                    meltfptr[22] = /*_.SETQ___V24*/ meltfptr[23];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4627:/ clear");
                    /*clear*/ /*_.SETQ___V24*/
                    meltfptr[23] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-macro.melt:4629:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.SLOC__V14*/ meltfptr[13]), ( "missing symbol in (EXPR_CHUNK <state-symbol> <ctype> <chunk>...)"), (melt_ptr_t)0);
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V23*/
                    meltfptr[22] = 0 ;
                    /*epilog*/
                }
                ;
            }
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V25*/
        meltfptr[23] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;
        MELT_LOCATION("warmelt-macro.melt:4630:/ compute");
        /*_.CURPAIR__V16*/
        meltfptr[15] = /*_.SETQ___V26*/ meltfptr[25] = /*_.PAIR_TAIL__V25*/ meltfptr[23];;
        /*_.PAIR_HEAD__V27*/
        meltfptr[26] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;
        MELT_LOCATION("warmelt-macro.melt:4631:/ compute");
        /*_.KCTY__V19*/
        meltfptr[18] = /*_.SETQ___V28*/ meltfptr[27] = /*_.PAIR_HEAD__V27*/ meltfptr[26];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_NOT_A__L6*/
        meltfnum[0] =
            !melt_is_instance_of((melt_ptr_t)(/*_.KCTY__V19*/ meltfptr[18]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[4])));;
        MELT_LOCATION("warmelt-macro.melt:4632:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L6*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4633:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.SLOC__V14*/ meltfptr[13]), ( "missing type keyword in (EXPR_CHUNK <state-symbol> <ctype> <chunk>...)"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4634:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4634:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:4632:/ quasiblock");


                    /*_.PROGN___V31*/
                    meltfptr[30] = /*_.RETURN___V30*/ meltfptr[29];;
                    /*^compute*/
                    /*_._IF___V29*/
                    meltfptr[28] = /*_.PROGN___V31*/ meltfptr[30];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4632:/ clear");
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

                /*_._IF___V29*/ meltfptr[28] =  /*fromline 1341*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V32*/
        meltfptr[29] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;
        MELT_LOCATION("warmelt-macro.melt:4635:/ compute");
        /*_.CURPAIR__V16*/
        meltfptr[15] = /*_.SETQ___V33*/ meltfptr[30] = /*_.PAIR_TAIL__V32*/ meltfptr[29];;
        MELT_LOCATION("warmelt-macro.melt:4636:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.KCTY__V19*/ meltfptr[18]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SYMB_DATA");
            /*_.CTY__V35*/
            meltfptr[34] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4637:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.KCTY__V19*/ meltfptr[18]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
            /*_.TYPKNAM__V36*/
            meltfptr[35] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_NOT_A__L7*/
        meltfnum[6] =
            !melt_is_instance_of((melt_ptr_t)(/*_.CTY__V35*/ meltfptr[34]), (melt_ptr_t)((/*!CLASS_CTYPE*/ meltfrout->tabval[5])));;
        MELT_LOCATION("warmelt-macro.melt:4639:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L7*/ meltfnum[6]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:4640:/ cppif.then");
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
                        MELT_LOCATION("warmelt-macro.melt:4640:/ cond");
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
                                    MELT_LOCATION("warmelt-macro.melt:4640:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[8];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 4640;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "mexpand_expr_chunk bad kcty=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.KCTY__V19*/ meltfptr[18];
                                        /*_.MELT_DEBUG_FUN__V39*/
                                        meltfptr[38] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V38*/
                                    meltfptr[37] = /*_.MELT_DEBUG_FUN__V39*/ meltfptr[38];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:4640:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L9*/
                                    meltfnum[8] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V39*/
                                    meltfptr[38] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V38*/ meltfptr[37] =  /*fromline 1341*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-macro.melt:4640:/ locexp");
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
                        /*clear*/ /*_._IF___V38*/
                        meltfptr[37] = 0 ;
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
                        MELT_LOCATION("warmelt-macro.melt:4641:/ locexp");
                        melt_error_str((melt_ptr_t)(/*_.SLOC__V14*/ meltfptr[13]), ( "bad type keyword for EXPR_CHUNK"), (melt_ptr_t)(/*_.TYPKNAM__V36*/ meltfptr[35]));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4642:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4642:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:4639:/ quasiblock");


                    /*_.PROGN___V41*/
                    meltfptr[37] = /*_.RETURN___V40*/ meltfptr[38];;
                    /*^compute*/
                    /*_._IF___V37*/
                    meltfptr[36] = /*_.PROGN___V41*/ meltfptr[37];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4639:/ clear");
                    /*clear*/ /*_.RETURN___V40*/
                    meltfptr[38] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V41*/
                    meltfptr[37] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V37*/ meltfptr[36] =  /*fromline 1341*/ NULL ;;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4644:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.CTY__V35*/ meltfptr[34]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "CTYPE_KEYWORD");
            /*_.CTYPE_KEYWORD__V42*/
            meltfptr[38] = slot;
        };
        ;
        /*_#eqeq__L10*/
        meltfnum[8] =
            ((/*_.CTYPE_KEYWORD__V42*/ meltfptr[38]) == (/*_.KCTY__V19*/ meltfptr[18]));;
        MELT_LOCATION("warmelt-macro.melt:4644:/ cond");
        /*cond*/
        if (/*_#eqeq__L10*/ meltfnum[8]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V43*/ meltfptr[37] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-macro.melt:4644:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4647:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.CTY__V35*/ meltfptr[34]) /*=obj*/;
                        melt_object_get_field(slot,obj, 10, "CTYPE_ALTKEYWORD");
                        /*_.CTYPE_ALTKEYWORD__V44*/
                        meltfptr[43] = slot;
                    };
                    ;
                    /*_#eqeq__L11*/
                    meltfnum[7] =
                        ((/*_.CTYPE_ALTKEYWORD__V44*/ meltfptr[43]) == (/*_.KCTY__V19*/ meltfptr[18]));;
                    MELT_LOCATION("warmelt-macro.melt:4647:/ cond");
                    /*cond*/
                    if (/*_#eqeq__L11*/ meltfnum[7]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:4648:/ locexp");
                                    melt_warning_str(0, (melt_ptr_t)(/*_.SLOC__V14*/ meltfptr[13]), ( "using obsolete ctype keyword in EXPR_CHUNK"), (melt_ptr_t)(/*_.TYPKNAM__V36*/ meltfptr[35]));
                                }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:4649:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CTY__V35*/ meltfptr[34]),
                                                                  (melt_ptr_t)((/*!CLASS_QUASI_CTYPE*/ meltfrout->tabval[6])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CTY__V35*/ meltfptr[34]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 2, "CTYPE_KEYWORD");
                                            /*_.CTYPE_KEYWORD__V46*/
                                            meltfptr[45] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.CTYPE_KEYWORD__V46*/ meltfptr[45] =  /*fromline 1341*/ NULL ;;
                                    }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:4649:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CTYPE_KEYWORD__V46*/ meltfptr[45]),
                                                                  (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[7])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CTYPE_KEYWORD__V46*/ meltfptr[45]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                            /*_.NAMED_NAME__V47*/
                                            meltfptr[46] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.NAMED_NAME__V47*/ meltfptr[46] =  /*fromline 1341*/ NULL ;;
                                    }
                                ;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:4649:/ locexp");
                                    melt_inform_str((melt_ptr_t)(/*_.SLOC__V14*/ meltfptr[13]), ( "prefered ctype is"), (melt_ptr_t)(/*_.NAMED_NAME__V47*/ meltfptr[46]));
                                }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:4647:/ quasiblock");


                                /*epilog*/

                                /*^clear*/
                                /*clear*/ /*_.CTYPE_KEYWORD__V46*/
                                meltfptr[45] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.NAMED_NAME__V47*/
                                meltfptr[46] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {


#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-macro.melt:4652:/ cppif.then");
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
                                    MELT_LOCATION("warmelt-macro.melt:4652:/ cond");
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
                                                MELT_LOCATION("warmelt-macro.melt:4652:/ apply");
                                                /*apply*/
                                                {
                                                    union meltparam_un argtab[5];
                                                    memset(&argtab, 0, sizeof(argtab));
                                                    /*^apply.arg*/
                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L13*/ meltfnum[12];
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 4652;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_cstring =  "mexpand_expr_chunk strange kcty=";
                                                    /*^apply.arg*/
                                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.KCTY__V19*/ meltfptr[18];
                                                    /*_.MELT_DEBUG_FUN__V49*/
                                                    meltfptr[46] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IF___V48*/
                                                meltfptr[45] = /*_.MELT_DEBUG_FUN__V49*/ meltfptr[46];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-macro.melt:4652:/ clear");
                                                /*clear*/ /*_#MELT_CALLCOUNT__L13*/
                                                meltfnum[12] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_.MELT_DEBUG_FUN__V49*/
                                                meltfptr[46] = 0 ;
                                            }
                                            ;
                                        }
                                    else    /*^cond.else*/
                                        {

                                            /*_._IF___V48*/ meltfptr[45] =  /*fromline 1341*/ NULL ;;
                                        }
                                    ;

                                    {
                                        MELT_LOCATION("warmelt-macro.melt:4652:/ locexp");
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
                                    /*clear*/ /*_._IF___V48*/
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

                                {
                                    MELT_LOCATION("warmelt-macro.melt:4653:/ locexp");
                                    melt_error_str((melt_ptr_t)(/*_.SLOC__V14*/ meltfptr[13]), ( "invalid type keyword for EXPR_CHUNK"), (melt_ptr_t)(/*_.TYPKNAM__V36*/ meltfptr[35]));
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-macro.melt:4655:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] = (/*nil*/NULL);;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:4655:/ locexp");
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
                                MELT_LOCATION("warmelt-macro.melt:4651:/ quasiblock");


                                /*_.PROGN___V51*/
                                meltfptr[45] = /*_.RETURN___V50*/ meltfptr[46];;
                                /*^compute*/
                                /*_._IFELSE___V45*/
                                meltfptr[44] = /*_.PROGN___V51*/ meltfptr[45];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:4647:/ clear");
                                /*clear*/ /*_.RETURN___V50*/
                                meltfptr[46] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V51*/
                                meltfptr[45] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V43*/
                    meltfptr[37] = /*_._IFELSE___V45*/ meltfptr[44];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4644:/ clear");
                    /*clear*/ /*_.CTYPE_ALTKEYWORD__V44*/
                    meltfptr[43] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#eqeq__L11*/
                    meltfnum[7] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V45*/
                    meltfptr[44] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:4657:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4658:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.SLOC__V14*/ meltfptr[13];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.TUP__V53*/
            meltfptr[45] =  melt_apply ((meltclosure_ptr_t)((/*!EXPAND_MACROSTRING_PAIRS*/ meltfrout->tabval[8])), (melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4659:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_EXPRCHUNK*/ meltfrout->tabval[9])), (5), "CLASS_SOURCE_EXPRCHUNK");
            /*_.INST__V55*/
            meltfptr[44] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V55*/ meltfptr[44])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V55*/ meltfptr[44]), (1), (/*_.SLOC__V14*/ meltfptr[13]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SCH_GENSYM", melt_magic_discr((melt_ptr_t)(/*_.INST__V55*/ meltfptr[44])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V55*/ meltfptr[44]), (2), (/*_.GSYM__V18*/ meltfptr[17]), "SCH_GENSYM");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SCH_CHUNKS", melt_magic_discr((melt_ptr_t)(/*_.INST__V55*/ meltfptr[44])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V55*/ meltfptr[44]), (3), (/*_.TUP__V53*/ meltfptr[45]), "SCH_CHUNKS");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SXCH_CTYPE", melt_magic_discr((melt_ptr_t)(/*_.INST__V55*/ meltfptr[44])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V55*/ meltfptr[44]), (4), (/*_.CTY__V35*/ meltfptr[34]), "SXCH_CTYPE");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V55*/ meltfptr[44], "newly made instance");
        ;
        /*_.MRES__V54*/
        meltfptr[43] = /*_.INST__V55*/ meltfptr[44];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4666:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4666:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4666:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L15*/ meltfnum[11];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4666;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_expr_chunk return mres=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MRES__V54*/ meltfptr[43];
                            /*_.MELT_DEBUG_FUN__V57*/
                            meltfptr[56] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V56*/
                        meltfptr[55] = /*_.MELT_DEBUG_FUN__V57*/ meltfptr[56];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4666:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L15*/
                        meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V57*/
                        meltfptr[56] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V56*/ meltfptr[55] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4666:/ locexp");
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
            /*clear*/ /*_._IF___V56*/
            meltfptr[55] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:4667:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.MRES__V54*/ meltfptr[43];;

        {
            MELT_LOCATION("warmelt-macro.melt:4667:/ locexp");
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
        /*_.LET___V52*/
        meltfptr[46] = /*_.RETURN___V58*/ meltfptr[56];;

        MELT_LOCATION("warmelt-macro.melt:4657:/ clear");
        /*clear*/ /*_.TUP__V53*/
        meltfptr[45] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MRES__V54*/
        meltfptr[43] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V58*/
        meltfptr[56] = 0 ;
        /*_.LET___V34*/
        meltfptr[33] = /*_.LET___V52*/ meltfptr[46];;

        MELT_LOCATION("warmelt-macro.melt:4636:/ clear");
        /*clear*/ /*_.CTY__V35*/
        meltfptr[34] = 0 ;
        /*^clear*/
        /*clear*/ /*_.TYPKNAM__V36*/
        meltfptr[35] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L7*/
        meltfnum[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V37*/
        meltfptr[36] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CTYPE_KEYWORD__V42*/
        meltfptr[38] = 0 ;
        /*^clear*/
        /*clear*/ /*_#eqeq__L10*/
        meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V43*/
        meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V52*/
        meltfptr[46] = 0 ;
        /*_.LET___V12*/
        meltfptr[10] = /*_.LET___V34*/ meltfptr[33];;

        MELT_LOCATION("warmelt-macro.melt:4618:/ clear");
        /*clear*/ /*_.CONT__V13*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SLOC__V14*/
        meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V15*/
        meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V16*/
        meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURCHK__V17*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.GSYM__V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.KCTY__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CUREL__V20*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_HEAD__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L5*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V25*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_HEAD__V27*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L6*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V29*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V32*/
        meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V33*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V34*/
        meltfptr[33] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4614:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V12*/ meltfptr[10];;

        {
            MELT_LOCATION("warmelt-macro.melt:4614:/ locexp");
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
        /*clear*/ /*_.IFCPP___V10*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V12*/
        meltfptr[10] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_EXPR_CHUNK", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_73_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS fromline 1716 */

    /** start of frame for meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS// fromline 1531
        : public Melt_CallFrameWithValues<42>
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
        MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<42> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS), clos) {};
        MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS() //the constructor fromline 1606
            : Melt_CallFrameWithValues<42> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<42> (fil,lin, sizeof(MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS)) {};
        MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<42> (fil,lin, sizeof(MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS


    /** end of frame for meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS fromline 1661**/

    /* end of frame for routine meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS fromline 1720 */

    /* classy proc frame meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS */
    MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
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
    MELT_LOCATION("warmelt-macro.melt:4683:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4684:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4684:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4684:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4684)?(4684):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V7*/
                        meltfptr[6] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V6*/
            meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4684:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4685:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4685:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[8] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4685:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4685)?(4685):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V9*/
                        meltfptr[8] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[8];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4685:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/
            meltfptr[8] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L3*/
        meltfnum[0] =
            ((/*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:4686:/ cond");
        /*cond*/
        if (/*_#NULL__L3*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V11*/ meltfptr[10] = (/*!MACROEXPAND_1*/ meltfrout->tabval[2]);;
                    /*_._IF___V10*/
                    meltfptr[8] = /*_.SETQ___V11*/ meltfptr[10];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4686:/ clear");
                    /*clear*/ /*_.SETQ___V11*/
                    meltfptr[10] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V10*/ meltfptr[8] =  /*fromline 1341*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4687:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_CLOSURE__L4*/
            meltfnum[3] =
                (melt_magic_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) == MELTOBMAG_CLOSURE);;
            MELT_LOCATION("warmelt-macro.melt:4687:/ cond");
            /*cond*/
            if (/*_#IS_CLOSURE__L4*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V13*/ meltfptr[12] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4687:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check mexpander"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4687)?(4687):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V13*/
                        meltfptr[12] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V12*/
            meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[12];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4687:/ clear");
            /*clear*/ /*_#IS_CLOSURE__L4*/
            meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V13*/
            meltfptr[12] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V12*/ meltfptr[10] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4688:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L5*/
            meltfnum[3] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:4688:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L5*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4688:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check modctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4688)?(4688):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V15*/
                        meltfptr[14] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V14*/
            meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4688:/ clear");
            /*clear*/ /*_#IS_OBJECT__L5*/
            meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[14] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:4689:/ quasiblock");


        /*_.FIELDS__V17*/
        meltfptr[16] = (/*nil*/NULL);;
        /*^compute*/
        /*_.FIELDNAMS__V18*/
        meltfptr[17] = (/*nil*/NULL);;
        MELT_LOCATION("warmelt-macro.melt:4691:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V19*/
            meltfptr[18] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4692:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V20*/
            meltfptr[19] = slot;
        };
        ;
        /*_.LIST_FIRST__V21*/
        meltfptr[20] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V19*/ meltfptr[18])));;
        /*^compute*/
        /*_.CURPAIR__V22*/
        meltfptr[21] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V21*/ meltfptr[20])));;
        /*^compute*/
        /*_.OBJSRC__V23*/
        meltfptr[22] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V22*/ meltfptr[21])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_A__L6*/
        meltfnum[3] =
            melt_is_instance_of((melt_ptr_t)(/*_.OBJSRC__V23*/ meltfptr[22]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
        MELT_LOCATION("warmelt-macro.melt:4696:/ cond");
        /*cond*/
        if (/*_#IS_A__L6*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4697:/ apply");
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
                        /*_.MEXPANDER__V25*/
                        meltfptr[24] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.OBJSRC__V23*/ meltfptr[22]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_.OBJEXP__V24*/
                    meltfptr[23] = /*_.MEXPANDER__V25*/ meltfptr[24];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4696:/ clear");
                    /*clear*/ /*_.MEXPANDER__V25*/
                    meltfptr[24] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OBJEXP__V24*/ meltfptr[23] = /*_.OBJSRC__V23*/ meltfptr[22];;
            }
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V26*/
        meltfptr[24] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V22*/ meltfptr[21])));;
        MELT_LOCATION("warmelt-macro.melt:4700:/ compute");
        /*_.CURPAIR__V22*/
        meltfptr[21] = /*_.SETQ___V27*/ meltfptr[26] = /*_.PAIR_TAIL__V26*/ meltfptr[24];;
        MELT_LOCATION("warmelt-macro.melt:4701:/ quasiblock");


        /*_.FLDLIST__V29*/
        meltfptr[28] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[3]))));;
        MELT_LOCATION("warmelt-macro.melt:4703:/ loop");
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
                    (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V22*/ meltfptr[21])) == MELTOBMAG_PAIR);;
                /*^compute*/
                /*_#NOT__L8*/
                meltfnum[7] =
                    (!(/*_#IS_PAIR__L7*/ meltfnum[6]));;
                MELT_LOCATION("warmelt-macro.melt:4704:/ cond");
                /*cond*/
                if (/*_#NOT__L8*/ meltfnum[7]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^quasiblock*/


                            /*^compute*/
                            /*_.INSLOOP__V31*/ meltfptr[30] =  /*fromline 1341*/ NULL ;;

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
                MELT_LOCATION("warmelt-macro.melt:4705:/ quasiblock");


                /*_.CURFKW__V32*/
                meltfptr[31] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V22*/ meltfptr[21])));;

                MELT_CHECK_SIGNAL();
                ;
                /*^compute*/
                /*_#IS_NOT_A__L9*/
                meltfnum[8] =
                    !melt_is_instance_of((melt_ptr_t)(/*_.CURFKW__V32*/ meltfptr[31]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[4])));;
                MELT_LOCATION("warmelt-macro.melt:4706:/ cond");
                /*cond*/
                if (/*_#IS_NOT_A__L9*/ meltfnum[8]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:4707:/ locexp");
                                /* error_plain */
                                melt_error_str((melt_ptr_t)(/*_.LOC__V20*/ meltfptr[19]), ( "expecting heyword in UNSAFE_PUT_FIELDS"), (melt_ptr_t)0);
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    } /*noelse*/
                ;
                /*_.PAIR_TAIL__V33*/
                meltfptr[32] =
                    (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V22*/ meltfptr[21])));;
                MELT_LOCATION("warmelt-macro.melt:4708:/ compute");
                /*_.CURPAIR__V22*/
                meltfptr[21] = /*_.SETQ___V34*/ meltfptr[33] = /*_.PAIR_TAIL__V33*/ meltfptr[32];;
                MELT_LOCATION("warmelt-macro.melt:4709:/ quasiblock");


                /*_.CUREXP__V35*/
                meltfptr[34] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V22*/ meltfptr[21])));;
                MELT_LOCATION("warmelt-macro.melt:4710:/ quasiblock");



                MELT_CHECK_SIGNAL();
                ;
                /*^apply*/
                /*apply*/
                {
                    union meltparam_un argtab[6];
                    memset(&argtab, 0, sizeof(argtab));
                    /*^apply.arg*/
                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LOC__V20*/ meltfptr[19];
                    /*^apply.arg*/
                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.CURFKW__V32*/ meltfptr[31];
                    /*^apply.arg*/
                    argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.CUREXP__V35*/ meltfptr[34];
                    /*^apply.arg*/
                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                    /*^apply.arg*/
                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
                    /*^apply.arg*/
                    argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                    /*_.FLDA__V36*/
                    meltfptr[35] =  melt_apply ((meltclosure_ptr_t)((/*!PARSE_FIELD_ASSIGNMENT*/ meltfrout->tabval[5])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                }
                ;

                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-macro.melt:4711:/ cond");
                /*cond*/
                if (/*_.FLDA__V36*/ meltfptr[35]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:4712:/ locexp");
                                meltgc_append_list((melt_ptr_t)(/*_.FLDLIST__V29*/ meltfptr[28]), (melt_ptr_t)(/*_.FLDA__V36*/ meltfptr[35]));
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    } /*noelse*/
                ;

                MELT_LOCATION("warmelt-macro.melt:4710:/ clear");
                /*clear*/ /*_.FLDA__V36*/
                meltfptr[35] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:4709:/ clear");
                /*clear*/ /*_.CUREXP__V35*/
                meltfptr[34] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:4705:/ clear");
                /*clear*/ /*_.CURFKW__V32*/
                meltfptr[31] = 0 ;
                /*^clear*/
                /*clear*/ /*_#IS_NOT_A__L9*/
                meltfnum[8] = 0 ;
                /*^clear*/
                /*clear*/ /*_.PAIR_TAIL__V33*/
                meltfptr[32] = 0 ;
                /*^clear*/
                /*clear*/ /*_.SETQ___V34*/
                meltfptr[33] = 0 ;
                /*_.PAIR_TAIL__V37*/
                meltfptr[35] =
                    (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V22*/ meltfptr[21])));;
                MELT_LOCATION("warmelt-macro.melt:4714:/ compute");
                /*_.CURPAIR__V22*/
                meltfptr[21] = /*_.SETQ___V38*/ meltfptr[34] = /*_.PAIR_TAIL__V37*/ meltfptr[35];;

                MELT_CHECK_SIGNAL();
                ;
                /*epilog*/

                MELT_LOCATION("warmelt-macro.melt:4703:/ clear");
                /*clear*/ /*_#IS_PAIR__L7*/
                meltfnum[6] = 0 ;
                /*^clear*/
                /*clear*/ /*_#NOT__L8*/
                meltfnum[7] = 0 ;
                /*^clear*/
                /*clear*/ /*_.PAIR_TAIL__V37*/
                meltfptr[35] = 0 ;
                /*^clear*/
                /*clear*/ /*_.SETQ___V38*/
                meltfptr[34] = 0 ;
            }
            ;
            ;
            goto meltlabloop_INSLOOP_6;
meltlabexit_INSLOOP_6:;/*^loopepilog*/
            /*loopepilog*/
            /*_.FOREVER___V30*/
            meltfptr[29] = /*_.INSLOOP__V31*/ meltfptr[30];;
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:4716:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[7]);
            /*_.FASTUP__V40*/
            meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.FLDLIST__V29*/ meltfptr[28]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4717:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_UNSAFE_PUT_FIELDS*/ meltfrout->tabval[8])), (4), "CLASS_SOURCE_UNSAFE_PUT_FIELDS");
            /*_.INST__V42*/
            meltfptr[35] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[35])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[35]), (1), (/*_.LOC__V20*/ meltfptr[19]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SUPUT_OBJ", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[35])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[35]), (2), (/*_.OBJEXP__V24*/ meltfptr[23]), "SUPUT_OBJ");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SUPUT_FIELDS", melt_magic_discr((melt_ptr_t)(/*_.INST__V42*/ meltfptr[35])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V42*/ meltfptr[35]), (3), (/*_.FASTUP__V40*/ meltfptr[32]), "SUPUT_FIELDS");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V42*/ meltfptr[35], "newly made instance");
        ;
        /*_.INST___V41*/
        meltfptr[33] = /*_.INST__V42*/ meltfptr[35];;
        /*^compute*/
        /*_.LET___V39*/
        meltfptr[31] = /*_.INST___V41*/ meltfptr[33];;

        MELT_LOCATION("warmelt-macro.melt:4716:/ clear");
        /*clear*/ /*_.FASTUP__V40*/
        meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_.INST___V41*/
        meltfptr[33] = 0 ;
        /*_.LET___V28*/
        meltfptr[27] = /*_.LET___V39*/ meltfptr[31];;

        MELT_LOCATION("warmelt-macro.melt:4701:/ clear");
        /*clear*/ /*_.FLDLIST__V29*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V30*/
        meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V39*/
        meltfptr[31] = 0 ;
        /*_.LET___V16*/
        meltfptr[14] = /*_.LET___V28*/ meltfptr[27];;

        MELT_LOCATION("warmelt-macro.melt:4689:/ clear");
        /*clear*/ /*_.FIELDS__V17*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FIELDNAMS__V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CONT__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V20*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OBJSRC__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L6*/
        meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OBJEXP__V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V26*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V27*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V28*/
        meltfptr[27] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4683:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-macro.melt:4683:/ locexp");
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
        /*clear*/ /*_.IFCPP___V8*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L3*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V10*/
        meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V12*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V16*/
        meltfptr[14] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_UNSAFE_PUT_FIELDS", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_74_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS fromline 1716 */

    /** start of frame for meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS// fromline 1531
        : public Melt_CallFrameWithValues<38>
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
        MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<38> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS), clos) {};
        MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS() //the constructor fromline 1606
            : Melt_CallFrameWithValues<38> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<38> (fil,lin, sizeof(MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS)) {};
        MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<38> (fil,lin, sizeof(MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS


    /** end of frame for meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS fromline 1661**/

    /* end of frame for routine meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS fromline 1720 */

    /* classy proc frame meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS */
    MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
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
    MELT_LOCATION("warmelt-macro.melt:4731:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4732:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4732:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4732:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4732)?(4732):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V7*/
                        meltfptr[6] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V6*/
            meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4732:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4733:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4733:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[8] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4733:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4733)?(4733):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V9*/
                        meltfptr[8] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[8];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4733:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/
            meltfptr[8] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L3*/
        meltfnum[0] =
            ((/*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:4734:/ cond");
        /*cond*/
        if (/*_#NULL__L3*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V11*/ meltfptr[10] = (/*!MACROEXPAND_1*/ meltfrout->tabval[2]);;
                    /*_._IF___V10*/
                    meltfptr[8] = /*_.SETQ___V11*/ meltfptr[10];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4734:/ clear");
                    /*clear*/ /*_.SETQ___V11*/
                    meltfptr[10] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V10*/ meltfptr[8] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:4735:/ quasiblock");


        /*_.FIELDS__V13*/
        meltfptr[12] = (/*nil*/NULL);;
        /*^compute*/
        /*_.FIELDNAMS__V14*/
        meltfptr[13] = (/*nil*/NULL);;
        MELT_LOCATION("warmelt-macro.melt:4737:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V15*/
            meltfptr[14] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4738:/ getslot");
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
            (melt_list_first((melt_ptr_t)(/*_.CONT__V15*/ meltfptr[14])));;
        /*^compute*/
        /*_.CURPAIR__V18*/
        meltfptr[17] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V17*/ meltfptr[16])));;
        /*^compute*/
        /*_.OBJSRC__V19*/
        meltfptr[18] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_A__L4*/
        meltfnum[3] =
            melt_is_instance_of((melt_ptr_t)(/*_.OBJSRC__V19*/ meltfptr[18]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
        MELT_LOCATION("warmelt-macro.melt:4742:/ cond");
        /*cond*/
        if (/*_#IS_A__L4*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4743:/ apply");
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
                        /*_.MEXPANDER__V21*/
                        meltfptr[20] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.OBJSRC__V19*/ meltfptr[18]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*_.OBJEXP__V20*/
                    meltfptr[19] = /*_.MEXPANDER__V21*/ meltfptr[20];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4742:/ clear");
                    /*clear*/ /*_.MEXPANDER__V21*/
                    meltfptr[20] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_.OBJEXP__V20*/ meltfptr[19] = /*_.OBJSRC__V19*/ meltfptr[18];;
            }
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V22*/
        meltfptr[20] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])));;
        MELT_LOCATION("warmelt-macro.melt:4746:/ compute");
        /*_.CURPAIR__V18*/
        meltfptr[17] = /*_.SETQ___V23*/ meltfptr[22] = /*_.PAIR_TAIL__V22*/ meltfptr[20];;
        MELT_LOCATION("warmelt-macro.melt:4747:/ quasiblock");


        /*_.FLDLIST__V25*/
        meltfptr[24] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[3]))));;
        MELT_LOCATION("warmelt-macro.melt:4749:/ loop");
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
                    (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])) == MELTOBMAG_PAIR);;
                /*^compute*/
                /*_#NOT__L6*/
                meltfnum[5] =
                    (!(/*_#IS_PAIR__L5*/ meltfnum[4]));;
                MELT_LOCATION("warmelt-macro.melt:4750:/ cond");
                /*cond*/
                if (/*_#NOT__L6*/ meltfnum[5]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^quasiblock*/


                            /*^compute*/
                            /*_.INSLOOP__V27*/ meltfptr[26] =  /*fromline 1341*/ NULL ;;

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
                MELT_LOCATION("warmelt-macro.melt:4751:/ quasiblock");


                /*_.CURFKW__V28*/
                meltfptr[27] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])));;

                MELT_CHECK_SIGNAL();
                ;
                /*^compute*/
                /*_#IS_NOT_A__L7*/
                meltfnum[6] =
                    !melt_is_instance_of((melt_ptr_t)(/*_.CURFKW__V28*/ meltfptr[27]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[4])));;
                MELT_LOCATION("warmelt-macro.melt:4752:/ cond");
                /*cond*/
                if (/*_#IS_NOT_A__L7*/ meltfnum[6]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:4753:/ locexp");
                                /* error_plain */
                                melt_error_str((melt_ptr_t)(/*_.LOC__V16*/ meltfptr[15]), ( "expecting heyword in PUT_FIELDS"), (melt_ptr_t)0);
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    } /*noelse*/
                ;
                /*_.PAIR_TAIL__V29*/
                meltfptr[28] =
                    (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])));;
                MELT_LOCATION("warmelt-macro.melt:4754:/ compute");
                /*_.CURPAIR__V18*/
                meltfptr[17] = /*_.SETQ___V30*/ meltfptr[29] = /*_.PAIR_TAIL__V29*/ meltfptr[28];;
                MELT_LOCATION("warmelt-macro.melt:4755:/ quasiblock");


                /*_.CUREXP__V31*/
                meltfptr[30] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])));;
                MELT_LOCATION("warmelt-macro.melt:4756:/ quasiblock");



                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-macro.melt:4757:/ apply");
                /*apply*/
                {
                    union meltparam_un argtab[6];
                    memset(&argtab, 0, sizeof(argtab));
                    /*^apply.arg*/
                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LOC__V16*/ meltfptr[15];
                    /*^apply.arg*/
                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.CURFKW__V28*/ meltfptr[27];
                    /*^apply.arg*/
                    argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.CUREXP__V31*/ meltfptr[30];
                    /*^apply.arg*/
                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                    /*^apply.arg*/
                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
                    /*^apply.arg*/
                    argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                    /*_.FLDA__V32*/
                    meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!PARSE_FIELD_ASSIGNMENT*/ meltfrout->tabval[5])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                }
                ;

                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-macro.melt:4758:/ cond");
                /*cond*/
                if (/*_.FLDA__V32*/ meltfptr[31]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:4759:/ locexp");
                                meltgc_append_list((melt_ptr_t)(/*_.FLDLIST__V25*/ meltfptr[24]), (melt_ptr_t)(/*_.FLDA__V32*/ meltfptr[31]));
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    } /*noelse*/
                ;

                MELT_LOCATION("warmelt-macro.melt:4756:/ clear");
                /*clear*/ /*_.FLDA__V32*/
                meltfptr[31] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:4755:/ clear");
                /*clear*/ /*_.CUREXP__V31*/
                meltfptr[30] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:4751:/ clear");
                /*clear*/ /*_.CURFKW__V28*/
                meltfptr[27] = 0 ;
                /*^clear*/
                /*clear*/ /*_#IS_NOT_A__L7*/
                meltfnum[6] = 0 ;
                /*^clear*/
                /*clear*/ /*_.PAIR_TAIL__V29*/
                meltfptr[28] = 0 ;
                /*^clear*/
                /*clear*/ /*_.SETQ___V30*/
                meltfptr[29] = 0 ;
                /*_.PAIR_TAIL__V33*/
                meltfptr[31] =
                    (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])));;
                MELT_LOCATION("warmelt-macro.melt:4761:/ compute");
                /*_.CURPAIR__V18*/
                meltfptr[17] = /*_.SETQ___V34*/ meltfptr[30] = /*_.PAIR_TAIL__V33*/ meltfptr[31];;

                MELT_CHECK_SIGNAL();
                ;
                /*epilog*/

                MELT_LOCATION("warmelt-macro.melt:4749:/ clear");
                /*clear*/ /*_#IS_PAIR__L5*/
                meltfnum[4] = 0 ;
                /*^clear*/
                /*clear*/ /*_#NOT__L6*/
                meltfnum[5] = 0 ;
                /*^clear*/
                /*clear*/ /*_.PAIR_TAIL__V33*/
                meltfptr[31] = 0 ;
                /*^clear*/
                /*clear*/ /*_.SETQ___V34*/
                meltfptr[30] = 0 ;
            }
            ;
            ;
            goto meltlabloop_INSLOOP_7;
meltlabexit_INSLOOP_7:;/*^loopepilog*/
            /*loopepilog*/
            /*_.FOREVER___V26*/
            meltfptr[25] = /*_.INSLOOP__V27*/ meltfptr[26];;
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:4763:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[7]);
            /*_.FASTUP__V36*/
            meltfptr[28] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[6])), (melt_ptr_t)(/*_.FLDLIST__V25*/ meltfptr[24]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4764:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_PUT_FIELDS*/ meltfrout->tabval[8])), (4), "CLASS_SOURCE_PUT_FIELDS");
            /*_.INST__V38*/
            meltfptr[31] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V38*/ meltfptr[31])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V38*/ meltfptr[31]), (1), (/*_.LOC__V16*/ meltfptr[15]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SUPUT_OBJ", melt_magic_discr((melt_ptr_t)(/*_.INST__V38*/ meltfptr[31])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V38*/ meltfptr[31]), (2), (/*_.OBJEXP__V20*/ meltfptr[19]), "SUPUT_OBJ");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SUPUT_FIELDS", melt_magic_discr((melt_ptr_t)(/*_.INST__V38*/ meltfptr[31])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V38*/ meltfptr[31]), (3), (/*_.FASTUP__V36*/ meltfptr[28]), "SUPUT_FIELDS");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V38*/ meltfptr[31], "newly made instance");
        ;
        /*_.INST___V37*/
        meltfptr[29] = /*_.INST__V38*/ meltfptr[31];;
        /*^compute*/
        /*_.LET___V35*/
        meltfptr[27] = /*_.INST___V37*/ meltfptr[29];;

        MELT_LOCATION("warmelt-macro.melt:4763:/ clear");
        /*clear*/ /*_.FASTUP__V36*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.INST___V37*/
        meltfptr[29] = 0 ;
        /*_.LET___V24*/
        meltfptr[23] = /*_.LET___V35*/ meltfptr[27];;

        MELT_LOCATION("warmelt-macro.melt:4747:/ clear");
        /*clear*/ /*_.FLDLIST__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V35*/
        meltfptr[27] = 0 ;
        /*_.LET___V12*/
        meltfptr[10] = /*_.LET___V24*/ meltfptr[23];;

        MELT_LOCATION("warmelt-macro.melt:4735:/ clear");
        /*clear*/ /*_.FIELDS__V13*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FIELDNAMS__V14*/
        meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CONT__V15*/
        meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V16*/
        meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V17*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OBJSRC__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L4*/
        meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.OBJEXP__V20*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V22*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V24*/
        meltfptr[23] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4731:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V12*/ meltfptr[10];;

        {
            MELT_LOCATION("warmelt-macro.melt:4731:/ locexp");
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
        /*clear*/ /*_.IFCPP___V8*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L3*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V10*/
        meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V12*/
        meltfptr[10] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_PUT_FIELDS", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_75_WARMELTmiMACRO_MEXPAND_PUT_FIELDS*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD fromline 1716 */

    /** start of frame for meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD// fromline 1531
        : public Melt_CallFrameWithValues<34>
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
        MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<34> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD), clos) {};
        MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD() //the constructor fromline 1606
            : Melt_CallFrameWithValues<34> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<34> (fil,lin, sizeof(MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD)) {};
        MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<34> (fil,lin, sizeof(MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD


    /** end of frame for meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD fromline 1661**/

    /* end of frame for routine meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD fromline 1720 */

    /* classy proc frame meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD */
    MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
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
    MELT_LOCATION("warmelt-macro.melt:4778:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4779:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4779:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4779:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4779)?(4779):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V7*/
                        meltfptr[6] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V6*/
            meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4779:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4780:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4780:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[8] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4780:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4780)?(4780):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V9*/
                        meltfptr[8] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[8];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4780:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/
            meltfptr[8] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:4781:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V11*/
            meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4782:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V12*/
            meltfptr[11] = slot;
        };
        ;
        /*_.LIST_FIRST__V13*/
        meltfptr[12] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V11*/ meltfptr[10])));;
        /*^compute*/
        /*_.CURPAIR__V14*/
        meltfptr[13] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V13*/ meltfptr[12])));;
        /*^compute*/
        /*_.CURFKW__V15*/
        meltfptr[14] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V14*/ meltfptr[13])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L3*/
        meltfnum[0] =
            !melt_is_instance_of((melt_ptr_t)(/*_.CURFKW__V15*/ meltfptr[14]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[2])));;
        MELT_LOCATION("warmelt-macro.melt:4786:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L3*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4788:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V12*/ meltfptr[11]), ( "field keyword expected in UNSAFE_GET_FIELD"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4789:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4789:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:4787:/ quasiblock");


                    /*_.PROGN___V18*/
                    meltfptr[17] = /*_.RETURN___V17*/ meltfptr[16];;
                    /*^compute*/
                    /*_._IF___V16*/
                    meltfptr[15] = /*_.PROGN___V18*/ meltfptr[17];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4786:/ clear");
                    /*clear*/ /*_.RETURN___V17*/
                    meltfptr[16] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V18*/
                    meltfptr[17] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V16*/ meltfptr[15] =  /*fromline 1341*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V19*/
        meltfptr[16] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V14*/ meltfptr[13])));;
        MELT_LOCATION("warmelt-macro.melt:4790:/ compute");
        /*_.CURPAIR__V14*/
        meltfptr[13] = /*_.SETQ___V20*/ meltfptr[17] = /*_.PAIR_TAIL__V19*/ meltfptr[16];;
        MELT_LOCATION("warmelt-macro.melt:4791:/ quasiblock");


        /*_.CUREXP__V22*/
        meltfptr[21] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V14*/ meltfptr[13])));;
        /*^compute*/
        /*_.PAIR_TAIL__V23*/
        meltfptr[22] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V14*/ meltfptr[13])));;
        MELT_LOCATION("warmelt-macro.melt:4792:/ compute");
        /*_.CURPAIR__V14*/
        meltfptr[13] = /*_.SETQ___V24*/ meltfptr[23] = /*_.PAIR_TAIL__V23*/ meltfptr[22];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4793:/ cond");
        /*cond*/
        if (/*_.CURPAIR__V14*/ meltfptr[13]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        /*^locexp*/
                        /* error_plain */ melt_error_str((melt_ptr_t)(/*_.LOC__V12*/ meltfptr[11]), ( "UNSAFE_GET_FIELD with more than two sons"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:4796:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[6];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LOC__V12*/ meltfptr[11];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.CURFKW__V15*/ meltfptr[14];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.CUREXP__V22*/ meltfptr[21];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.FLDA__V26*/
            meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!PARSE_FIELD_ASSIGNMENT*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_A__L4*/
        meltfnum[3] =
            melt_is_instance_of((melt_ptr_t)(/*_.FLDA__V26*/ meltfptr[25]), (melt_ptr_t)((/*!CLASS_SOURCE_FIELDASSIGN*/ meltfrout->tabval[4])));;
        /*^compute*/
        /*_#NOT__L5*/
        meltfnum[4] =
            (!(/*_#IS_A__L4*/ meltfnum[3]));;
        MELT_LOCATION("warmelt-macro.melt:4797:/ cond");
        /*cond*/
        if (/*_#NOT__L5*/ meltfnum[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4799:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V12*/ meltfptr[11]), ( "bad field and expression in UNSAFE_GET_FIELD"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4800:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4800:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:4798:/ quasiblock");


                    /*_.PROGN___V29*/
                    meltfptr[28] = /*_.RETURN___V28*/ meltfptr[27];;
                    /*^compute*/
                    /*_._IF___V27*/
                    meltfptr[26] = /*_.PROGN___V29*/ meltfptr[28];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4797:/ clear");
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

                /*_._IF___V27*/ meltfptr[26] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:4801:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.FLDA__V26*/ meltfptr[25]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SFLA_FIELD");
            /*_.FLD__V31*/
            meltfptr[28] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4802:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.FLDA__V26*/ meltfptr[25]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "SFLA_EXPR");
            /*_.EXP__V32*/
            meltfptr[31] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#NULL__L6*/
        meltfnum[5] =
            ((/*_.EXP__V32*/ meltfptr[31]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:4803:/ cond");
        /*cond*/
        if (/*_#NULL__L6*/ meltfnum[5]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4804:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V12*/ meltfptr[11]), ( "missing object expression for (UNSAFE_GET_FIELD :field objexpr)"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4805:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_UNSAFE_GET_FIELD*/ meltfrout->tabval[5])), (4), "CLASS_SOURCE_UNSAFE_GET_FIELD");
            /*_.INST__V34*/
            meltfptr[33] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V34*/ meltfptr[33])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V34*/ meltfptr[33]), (1), (/*_.LOC__V12*/ meltfptr[11]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SUGET_OBJ", melt_magic_discr((melt_ptr_t)(/*_.INST__V34*/ meltfptr[33])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V34*/ meltfptr[33]), (2), (/*_.EXP__V32*/ meltfptr[31]), "SUGET_OBJ");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SUGET_FIELD", melt_magic_discr((melt_ptr_t)(/*_.INST__V34*/ meltfptr[33])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V34*/ meltfptr[33]), (3), (/*_.FLD__V31*/ meltfptr[28]), "SUGET_FIELD");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V34*/ meltfptr[33], "newly made instance");
        ;
        /*_.INST___V33*/
        meltfptr[32] = /*_.INST__V34*/ meltfptr[33];;
        /*^compute*/
        /*_.LET___V30*/
        meltfptr[27] = /*_.INST___V33*/ meltfptr[32];;

        MELT_LOCATION("warmelt-macro.melt:4801:/ clear");
        /*clear*/ /*_.FLD__V31*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.EXP__V32*/
        meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L6*/
        meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.INST___V33*/
        meltfptr[32] = 0 ;
        /*_.LET___V25*/
        meltfptr[24] = /*_.LET___V30*/ meltfptr[27];;

        MELT_LOCATION("warmelt-macro.melt:4796:/ clear");
        /*clear*/ /*_.FLDA__V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L4*/
        meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L5*/
        meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V27*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V30*/
        meltfptr[27] = 0 ;
        /*_.LET___V21*/
        meltfptr[20] = /*_.LET___V25*/ meltfptr[24];;

        MELT_LOCATION("warmelt-macro.melt:4791:/ clear");
        /*clear*/ /*_.CUREXP__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V25*/
        meltfptr[24] = 0 ;
        /*_.LET___V10*/
        meltfptr[8] = /*_.LET___V21*/ meltfptr[20];;

        MELT_LOCATION("warmelt-macro.melt:4781:/ clear");
        /*clear*/ /*_.CONT__V11*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V12*/
        meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V13*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V14*/
        meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURFKW__V15*/
        meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L3*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V16*/
        meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V19*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V20*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V21*/
        meltfptr[20] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4778:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V10*/ meltfptr[8];;

        {
            MELT_LOCATION("warmelt-macro.melt:4778:/ locexp");
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
        /*clear*/ /*_.IFCPP___V8*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V10*/
        meltfptr[8] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_UNSAFE_GET_FIELD", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_76_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD fromline 1716 */

    /** start of frame for meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD// fromline 1531
        : public Melt_CallFrameWithValues<34>
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
        MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<34> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD), clos) {};
        MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD() //the constructor fromline 1606
            : Melt_CallFrameWithValues<34> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<34> (fil,lin, sizeof(MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD)) {};
        MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<34> (fil,lin, sizeof(MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD


    /** end of frame for meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD fromline 1661**/

    /* end of frame for routine meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD fromline 1720 */

    /* classy proc frame meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD */
    MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
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
    MELT_LOCATION("warmelt-macro.melt:4819:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4820:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4820:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4820:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4820)?(4820):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V7*/
                        meltfptr[6] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V6*/
            meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4820:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4821:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4821:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[8] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4821:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4821)?(4821):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V9*/
                        meltfptr[8] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[8];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4821:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/
            meltfptr[8] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:4822:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V11*/
            meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4823:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V12*/
            meltfptr[11] = slot;
        };
        ;
        /*_.LIST_FIRST__V13*/
        meltfptr[12] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V11*/ meltfptr[10])));;
        /*^compute*/
        /*_.CURPAIR__V14*/
        meltfptr[13] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V13*/ meltfptr[12])));;
        /*^compute*/
        /*_.CURFKW__V15*/
        meltfptr[14] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V14*/ meltfptr[13])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L3*/
        meltfnum[0] =
            !melt_is_instance_of((melt_ptr_t)(/*_.CURFKW__V15*/ meltfptr[14]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[2])));;
        MELT_LOCATION("warmelt-macro.melt:4827:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L3*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4829:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V12*/ meltfptr[11]), ( "field keyword expected in GET_FIELD"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4830:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4830:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:4828:/ quasiblock");


                    /*_.PROGN___V18*/
                    meltfptr[17] = /*_.RETURN___V17*/ meltfptr[16];;
                    /*^compute*/
                    /*_._IF___V16*/
                    meltfptr[15] = /*_.PROGN___V18*/ meltfptr[17];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4827:/ clear");
                    /*clear*/ /*_.RETURN___V17*/
                    meltfptr[16] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V18*/
                    meltfptr[17] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V16*/ meltfptr[15] =  /*fromline 1341*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V19*/
        meltfptr[16] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V14*/ meltfptr[13])));;
        MELT_LOCATION("warmelt-macro.melt:4831:/ compute");
        /*_.CURPAIR__V14*/
        meltfptr[13] = /*_.SETQ___V20*/ meltfptr[17] = /*_.PAIR_TAIL__V19*/ meltfptr[16];;
        MELT_LOCATION("warmelt-macro.melt:4832:/ quasiblock");


        /*_.CUREXP__V22*/
        meltfptr[21] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V14*/ meltfptr[13])));;
        /*^compute*/
        /*_.PAIR_TAIL__V23*/
        meltfptr[22] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V14*/ meltfptr[13])));;
        MELT_LOCATION("warmelt-macro.melt:4833:/ compute");
        /*_.CURPAIR__V14*/
        meltfptr[13] = /*_.SETQ___V24*/ meltfptr[23] = /*_.PAIR_TAIL__V23*/ meltfptr[22];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4834:/ cond");
        /*cond*/
        if (/*_.CURPAIR__V14*/ meltfptr[13]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        /*^locexp*/
                        /* error_plain */ melt_error_str((melt_ptr_t)(/*_.LOC__V12*/ meltfptr[11]), ( "UNSAFE_GET_FIELD with more than two sons"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:4837:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[6];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LOC__V12*/ meltfptr[11];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.CURFKW__V15*/ meltfptr[14];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.CUREXP__V22*/ meltfptr[21];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.FLDA__V26*/
            meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!PARSE_FIELD_ASSIGNMENT*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_A__L4*/
        meltfnum[3] =
            melt_is_instance_of((melt_ptr_t)(/*_.FLDA__V26*/ meltfptr[25]), (melt_ptr_t)((/*!CLASS_SOURCE_FIELDASSIGN*/ meltfrout->tabval[4])));;
        /*^compute*/
        /*_#NOT__L5*/
        meltfnum[4] =
            (!(/*_#IS_A__L4*/ meltfnum[3]));;
        MELT_LOCATION("warmelt-macro.melt:4838:/ cond");
        /*cond*/
        if (/*_#NOT__L5*/ meltfnum[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4840:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V12*/ meltfptr[11]), ( "bad field and expression in GET_FIELD"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4841:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4841:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:4839:/ quasiblock");


                    /*_.PROGN___V29*/
                    meltfptr[28] = /*_.RETURN___V28*/ meltfptr[27];;
                    /*^compute*/
                    /*_._IF___V27*/
                    meltfptr[26] = /*_.PROGN___V29*/ meltfptr[28];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4838:/ clear");
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

                /*_._IF___V27*/ meltfptr[26] =  /*fromline 1341*/ NULL ;;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:4842:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.FLDA__V26*/ meltfptr[25]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SFLA_FIELD");
            /*_.FLD__V31*/
            meltfptr[28] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4843:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.FLDA__V26*/ meltfptr[25]) /*=obj*/;
            melt_object_get_field(slot,obj, 3, "SFLA_EXPR");
            /*_.EXP__V32*/
            meltfptr[31] = slot;
        };
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*_#NULL__L6*/
        meltfnum[5] =
            ((/*_.EXP__V32*/ meltfptr[31]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:4844:/ cond");
        /*cond*/
        if (/*_#NULL__L6*/ meltfnum[5]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4845:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V12*/ meltfptr[11]), ( "missing object expression for (GET_FIELD :field objexpr)"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4846:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_GET_FIELD*/ meltfrout->tabval[5])), (4), "CLASS_SOURCE_GET_FIELD");
            /*_.INST__V34*/
            meltfptr[33] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V34*/ meltfptr[33])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V34*/ meltfptr[33]), (1), (/*_.LOC__V12*/ meltfptr[11]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SUGET_OBJ", melt_magic_discr((melt_ptr_t)(/*_.INST__V34*/ meltfptr[33])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V34*/ meltfptr[33]), (2), (/*_.EXP__V32*/ meltfptr[31]), "SUGET_OBJ");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SUGET_FIELD", melt_magic_discr((melt_ptr_t)(/*_.INST__V34*/ meltfptr[33])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V34*/ meltfptr[33]), (3), (/*_.FLD__V31*/ meltfptr[28]), "SUGET_FIELD");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V34*/ meltfptr[33], "newly made instance");
        ;
        /*_.INST___V33*/
        meltfptr[32] = /*_.INST__V34*/ meltfptr[33];;
        /*^compute*/
        /*_.LET___V30*/
        meltfptr[27] = /*_.INST___V33*/ meltfptr[32];;

        MELT_LOCATION("warmelt-macro.melt:4842:/ clear");
        /*clear*/ /*_.FLD__V31*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.EXP__V32*/
        meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L6*/
        meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.INST___V33*/
        meltfptr[32] = 0 ;
        /*_.LET___V25*/
        meltfptr[24] = /*_.LET___V30*/ meltfptr[27];;

        MELT_LOCATION("warmelt-macro.melt:4837:/ clear");
        /*clear*/ /*_.FLDA__V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L4*/
        meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L5*/
        meltfnum[4] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V27*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V30*/
        meltfptr[27] = 0 ;
        /*_.LET___V21*/
        meltfptr[20] = /*_.LET___V25*/ meltfptr[24];;

        MELT_LOCATION("warmelt-macro.melt:4832:/ clear");
        /*clear*/ /*_.CUREXP__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V25*/
        meltfptr[24] = 0 ;
        /*_.LET___V10*/
        meltfptr[8] = /*_.LET___V21*/ meltfptr[20];;

        MELT_LOCATION("warmelt-macro.melt:4822:/ clear");
        /*clear*/ /*_.CONT__V11*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V12*/
        meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V13*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V14*/
        meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURFKW__V15*/
        meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L3*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V16*/
        meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V19*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V20*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V21*/
        meltfptr[20] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4819:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V10*/ meltfptr[8];;

        {
            MELT_LOCATION("warmelt-macro.melt:4819:/ locexp");
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
        /*clear*/ /*_.IFCPP___V8*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V10*/
        meltfptr[8] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_GET_FIELD", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_77_WARMELTmiMACRO_MEXPAND_GET_FIELD*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN fromline 1716 */

    /** start of frame for meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN// fromline 1531
        : public Melt_CallFrameWithValues<23>
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
        MeltFrame_meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<23> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN), clos) {};
        MeltFrame_meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN() //the constructor fromline 1606
            : Melt_CallFrameWithValues<23> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<23> (fil,lin, sizeof(MeltFrame_meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN)) {};
        MeltFrame_meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<23> (fil,lin, sizeof(MeltFrame_meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN


    /** end of frame for meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN fromline 1661**/

    /* end of frame for routine meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN fromline 1720 */

    /* classy proc frame meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN */
    MeltFrame_meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
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
    MELT_LOCATION("warmelt-macro.melt:4860:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4861:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V4*/ meltfptr[3]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4861:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[7] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4861:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4861)?(4861):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V8*/
                        meltfptr[7] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V7*/
            meltfptr[6] = /*_._IFELSE___V8*/ meltfptr[7];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4861:/ clear");
            /*clear*/ /*_#IS_A__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V8*/
            meltfptr[7] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V7*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4862:/ cppif.then");
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
            /*_#MELT_NEED_DBG__L2*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:4862:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L2*/ meltfnum[0]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:4862:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4862;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "pairlist_to_progn pair";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.PAIR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V10*/
                            meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V9*/
                        meltfptr[7] = /*_.MELT_DEBUG_FUN__V10*/ meltfptr[9];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4862:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L3*/
                        meltfnum[2] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V10*/
                        meltfptr[9] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V9*/ meltfptr[7] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4862:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V9*/
            meltfptr[7] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:4863:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_PAIR__L4*/
            meltfnum[2] =
                (melt_magic_discr((melt_ptr_t)(/*_.PAIR__V2*/ meltfptr[1])) == MELTOBMAG_PAIR);;
            MELT_LOCATION("warmelt-macro.melt:4863:/ cond");
            /*cond*/
            if (/*_#IS_PAIR__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[7] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4863:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check_pair"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4863)?(4863):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V12*/
                        meltfptr[7] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V11*/
            meltfptr[9] = /*_._IFELSE___V12*/ meltfptr[7];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4863:/ clear");
            /*clear*/ /*_#IS_PAIR__L4*/
            meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V12*/
            meltfptr[7] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V11*/ meltfptr[9] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4864:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L5*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V6*/ meltfptr[5])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:4864:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L5*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V14*/ meltfptr[13] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4864:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check modctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4864)?(4864):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V14*/
                        meltfptr[13] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V13*/
            meltfptr[7] = /*_._IFELSE___V14*/ meltfptr[13];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4864:/ clear");
            /*clear*/ /*_#IS_OBJECT__L5*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V14*/
            meltfptr[13] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V13*/ meltfptr[7] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:4865:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:4868:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V17*/
        meltfptr[16] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_4*/ meltfrout->tabval[4])), (3));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V17*/ meltfptr[16])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V17*/ meltfptr[16])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V17*/ meltfptr[16])->tabval[0] = (melt_ptr_t)(/*_.MEXPANDER__V5*/ meltfptr[4]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V17*/ meltfptr[16])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 1>= 0 && 1< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V17*/ meltfptr[16])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V17*/ meltfptr[16])->tabval[1] = (melt_ptr_t)(/*_.ENV__V4*/ meltfptr[3]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V17*/ meltfptr[16])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 2>= 0 && 2< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V17*/ meltfptr[16])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V17*/ meltfptr[16])->tabval[2] = (melt_ptr_t)(/*_.MODCTX__V6*/ meltfptr[5]);
        ;
        /*_.LAMBDA___V16*/
        meltfptr[15] = /*_.LAMBDA___V17*/ meltfptr[16];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4865:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[2];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[3]);
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.LAMBDA___V16*/ meltfptr[15];
            /*_.BODYTUP__V18*/
            meltfptr[17] =  melt_apply ((meltclosure_ptr_t)((/*!PAIRLIST_TO_MULTIPLE*/ meltfrout->tabval[2])), (melt_ptr_t)(/*_.PAIR__V2*/ meltfptr[1]), (MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4870:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_PROGN*/ meltfrout->tabval[5])), (3), "CLASS_SOURCE_PROGN");
            /*_.INST__V20*/
            meltfptr[19] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V20*/ meltfptr[19])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V20*/ meltfptr[19]), (1), (/*_.LOC__V3*/ meltfptr[2]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SPROGN_BODY", melt_magic_discr((melt_ptr_t)(/*_.INST__V20*/ meltfptr[19])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V20*/ meltfptr[19]), (2), (/*_.BODYTUP__V18*/ meltfptr[17]), "SPROGN_BODY");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V20*/ meltfptr[19], "newly made instance");
        ;
        /*_.SPROGN__V19*/
        meltfptr[18] = /*_.INST__V20*/ meltfptr[19];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4874:/ cppif.then");
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
            meltfnum[2] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:4874:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L6*/ meltfnum[2]) /*then*/
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
                        MELT_LOCATION("warmelt-macro.melt:4874:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4874;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "pairlist_to_progn sprogn";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SPROGN__V19*/ meltfptr[18];
                            /*_.MELT_DEBUG_FUN__V22*/
                            meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[1])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V21*/
                        meltfptr[20] = /*_.MELT_DEBUG_FUN__V22*/ meltfptr[21];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4874:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L7*/
                        meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V22*/
                        meltfptr[21] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V21*/ meltfptr[20] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4874:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L6*/
            meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V21*/
            meltfptr[20] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:4875:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.SPROGN__V19*/ meltfptr[18];;

        {
            MELT_LOCATION("warmelt-macro.melt:4875:/ locexp");
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
        /*_.LET___V15*/
        meltfptr[13] = /*_.RETURN___V23*/ meltfptr[21];;

        MELT_LOCATION("warmelt-macro.melt:4865:/ clear");
        /*clear*/ /*_.LAMBDA___V16*/
        meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BODYTUP__V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SPROGN__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V23*/
        meltfptr[21] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4860:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V15*/ meltfptr[13];;

        {
            MELT_LOCATION("warmelt-macro.melt:4860:/ locexp");
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
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V11*/
        meltfptr[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V13*/
        meltfptr[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V15*/
        meltfptr[13] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("PAIRLIST_TO_PROGN", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_78_WARMELTmiMACRO_PAIRLIST_TO_PROGN*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_79_WARMELTmiMACRO_LAMBDA_cl18(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_79_WARMELTmiMACRO_LAMBDA_cl18_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_79_WARMELTmiMACRO_LAMBDA_cl18 fromline 1716 */

    /** start of frame for meltrout_79_WARMELTmiMACRO_LAMBDA_cl18 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_79_WARMELTmiMACRO_LAMBDA_cl18// fromline 1531
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
        MeltFrame_meltrout_79_WARMELTmiMACRO_LAMBDA_cl18(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_79_WARMELTmiMACRO_LAMBDA_cl18), clos) {};
        MeltFrame_meltrout_79_WARMELTmiMACRO_LAMBDA_cl18() //the constructor fromline 1606
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_79_WARMELTmiMACRO_LAMBDA_cl18)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_79_WARMELTmiMACRO_LAMBDA_cl18(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_79_WARMELTmiMACRO_LAMBDA_cl18)) {};
        MeltFrame_meltrout_79_WARMELTmiMACRO_LAMBDA_cl18(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_79_WARMELTmiMACRO_LAMBDA_cl18), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_79_WARMELTmiMACRO_LAMBDA_cl18


    /** end of frame for meltrout_79_WARMELTmiMACRO_LAMBDA_cl18 fromline 1661**/

    /* end of frame for routine meltrout_79_WARMELTmiMACRO_LAMBDA_cl18 fromline 1720 */

    /* classy proc frame meltrout_79_WARMELTmiMACRO_LAMBDA_cl18 */
    MeltFrame_meltrout_79_WARMELTmiMACRO_LAMBDA_cl18
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_79_WARMELTmiMACRO_LAMBDA_cl18 fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl18", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:4868:/ getarg");
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
            MELT_LOCATION("warmelt-macro.melt:4868:/ locexp");
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
    melt_trace_end("LAMBDA_cl18", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_79_WARMELTmiMACRO_LAMBDA_cl18_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_79_WARMELTmiMACRO_LAMBDA_cl18*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ fromline 1716 */

    /** start of frame for meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ// fromline 1531
        : public Melt_CallFrameWithValues<31>
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
        MeltFrame_meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<31> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ), clos) {};
        MeltFrame_meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ() //the constructor fromline 1606
            : Melt_CallFrameWithValues<31> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<31> (fil,lin, sizeof(MeltFrame_meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ)) {};
        MeltFrame_meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<31> (fil,lin, sizeof(MeltFrame_meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ


    /** end of frame for meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ fromline 1661**/

    /* end of frame for routine meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ fromline 1720 */

    /* classy proc frame meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ */
    MeltFrame_meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
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
    MELT_LOCATION("warmelt-macro.melt:4879:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4880:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4880:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4880:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4880)?(4880):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V7*/
                        meltfptr[6] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V6*/
            meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4880:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4881:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4881:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[8] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4881:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4881)?(4881):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V9*/
                        meltfptr[8] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[8];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4881:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/
            meltfptr[8] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L3*/
        meltfnum[0] =
            ((/*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:4882:/ cond");
        /*cond*/
        if (/*_#NULL__L3*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V11*/ meltfptr[10] = (/*!MACROEXPAND_1*/ meltfrout->tabval[2]);;
                    /*_._IF___V10*/
                    meltfptr[8] = /*_.SETQ___V11*/ meltfptr[10];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4882:/ clear");
                    /*clear*/ /*_.SETQ___V11*/
                    meltfptr[10] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V10*/ meltfptr[8] =  /*fromline 1341*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4883:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L4*/
            meltfnum[3] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:4883:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L4*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V13*/ meltfptr[12] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4883:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check modctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4883)?(4883):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V13*/
                        meltfptr[12] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V12*/
            meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[12];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4883:/ clear");
            /*clear*/ /*_#IS_OBJECT__L4*/
            meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V13*/
            meltfptr[12] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V12*/ meltfptr[10] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:4884:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V15*/
            meltfptr[14] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4885:/ getslot");
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
            (melt_list_first((melt_ptr_t)(/*_.CONT__V15*/ meltfptr[14])));;
        /*^compute*/
        /*_.CURPAIR__V18*/
        meltfptr[17] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V17*/ meltfptr[16])));;
        /*^compute*/
        /*_.CURSYM__V19*/
        meltfptr[18] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_NOT_A__L5*/
        meltfnum[3] =
            !melt_is_instance_of((melt_ptr_t)(/*_.CURSYM__V19*/ meltfptr[18]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[3])));;
        MELT_LOCATION("warmelt-macro.melt:4889:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L5*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4891:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V16*/ meltfptr[15]), ( "var symbol name expected in SETQ"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4892:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4892:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:4890:/ quasiblock");


                    /*_.PROGN___V22*/
                    meltfptr[21] = /*_.RETURN___V21*/ meltfptr[20];;
                    /*^compute*/
                    /*_._IF___V20*/
                    meltfptr[19] = /*_.PROGN___V22*/ meltfptr[21];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4889:/ clear");
                    /*clear*/ /*_.RETURN___V21*/
                    meltfptr[20] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PROGN___V22*/
                    meltfptr[21] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V20*/ meltfptr[19] =  /*fromline 1341*/ NULL ;;
            }
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V23*/
        meltfptr[20] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])));;
        MELT_LOCATION("warmelt-macro.melt:4893:/ compute");
        /*_.CURPAIR__V18*/
        meltfptr[17] = /*_.SETQ___V24*/ meltfptr[21] = /*_.PAIR_TAIL__V23*/ meltfptr[20];;
        MELT_LOCATION("warmelt-macro.melt:4894:/ quasiblock");


        /*_.CUREXP__V26*/
        meltfptr[25] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])));;
        /*^compute*/
        /*_.PAIR_TAIL__V27*/
        meltfptr[26] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])));;
        MELT_LOCATION("warmelt-macro.melt:4895:/ compute");
        /*_.CURPAIR__V18*/
        meltfptr[17] = /*_.SETQ___V28*/ meltfptr[27] = /*_.PAIR_TAIL__V27*/ meltfptr[26];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4896:/ cond");
        /*cond*/
        if (/*_.CURPAIR__V18*/ meltfptr[17]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        /*^locexp*/
                        /* error_plain */ melt_error_str((melt_ptr_t)(/*_.LOC__V16*/ meltfptr[15]), ( "SETQ with more than two sons"), (melt_ptr_t)0);
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
        MELT_LOCATION("warmelt-macro.melt:4900:/ apply");
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
            meltfptr[28] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CUREXP__V26*/ meltfptr[25]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:4897:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_SETQ*/ meltfrout->tabval[4])), (4), "CLASS_SOURCE_SETQ");
            /*_.INST__V31*/
            meltfptr[30] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V31*/ meltfptr[30])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V31*/ meltfptr[30]), (1), (/*_.LOC__V16*/ meltfptr[15]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SSTQ_VAR", melt_magic_discr((melt_ptr_t)(/*_.INST__V31*/ meltfptr[30])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V31*/ meltfptr[30]), (2), (/*_.CURSYM__V19*/ meltfptr[18]), "SSTQ_VAR");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SSTQ_EXPR", melt_magic_discr((melt_ptr_t)(/*_.INST__V31*/ meltfptr[30])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V31*/ meltfptr[30]), (3), (/*_.MEXPANDER__V29*/ meltfptr[28]), "SSTQ_EXPR");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V31*/ meltfptr[30], "newly made instance");
        ;
        /*_.INST___V30*/
        meltfptr[29] = /*_.INST__V31*/ meltfptr[30];;
        /*^compute*/
        /*_.LET___V25*/
        meltfptr[24] = /*_.INST___V30*/ meltfptr[29];;

        MELT_LOCATION("warmelt-macro.melt:4894:/ clear");
        /*clear*/ /*_.CUREXP__V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V27*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MEXPANDER__V29*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.INST___V30*/
        meltfptr[29] = 0 ;
        /*_.LET___V14*/
        meltfptr[12] = /*_.LET___V25*/ meltfptr[24];;

        MELT_LOCATION("warmelt-macro.melt:4884:/ clear");
        /*clear*/ /*_.CONT__V15*/
        meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LOC__V16*/
        meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V17*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURSYM__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L5*/
        meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V20*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V23*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V24*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V25*/
        meltfptr[24] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4879:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V14*/ meltfptr[12];;

        {
            MELT_LOCATION("warmelt-macro.melt:4879:/ locexp");
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
        /*clear*/ /*_.IFCPP___V8*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L3*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V10*/
        meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V12*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V14*/
        meltfptr[12] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_SETQ", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_80_WARMELTmiMACRO_MEXPAND_SETQ*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_81_WARMELTmiMACRO_MEXPAND_IF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                      const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_81_WARMELTmiMACRO_MEXPAND_IF_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_81_WARMELTmiMACRO_MEXPAND_IF fromline 1716 */

    /** start of frame for meltrout_81_WARMELTmiMACRO_MEXPAND_IF of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_IF// fromline 1531
        : public Melt_CallFrameWithValues<55>
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
        MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_IF(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<55> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_IF), clos) {};
        MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_IF() //the constructor fromline 1606
            : Melt_CallFrameWithValues<55> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_IF)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_IF(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<55> (fil,lin, sizeof(MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_IF)) {};
        MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_IF(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<55> (fil,lin, sizeof(MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_IF), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_IF


    /** end of frame for meltrout_81_WARMELTmiMACRO_MEXPAND_IF fromline 1661**/

    /* end of frame for routine meltrout_81_WARMELTmiMACRO_MEXPAND_IF fromline 1720 */

    /* classy proc frame meltrout_81_WARMELTmiMACRO_MEXPAND_IF */
    MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_IF
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_81_WARMELTmiMACRO_MEXPAND_IF fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
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
    MELT_LOCATION("warmelt-macro.melt:4910:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4911:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4911:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4911:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4911;
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

                        MELT_LOCATION("warmelt-macro.melt:4911:/ clear");
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

                    /*_._IF___V6*/ meltfptr[5] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4911:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4912:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4912:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4912:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4912)?(4912):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V9*/
                        meltfptr[5] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4912:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4913:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:4913:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[10] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4913:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4913)?(4913):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V11*/
                        meltfptr[10] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V10*/
            meltfptr[5] = /*_._IFELSE___V11*/ meltfptr[10];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4913:/ clear");
            /*clear*/ /*_#IS_A__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V11*/
            meltfptr[10] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V10*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L5*/
        meltfnum[1] =
            ((/*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:4914:/ cond");
        /*cond*/
        if (/*_#NULL__L5*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V13*/ meltfptr[12] = (/*!MACROEXPAND_1*/ meltfrout->tabval[3]);;
                    /*_._IF___V12*/
                    meltfptr[10] = /*_.SETQ___V13*/ meltfptr[12];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4914:/ clear");
                    /*clear*/ /*_.SETQ___V13*/
                    meltfptr[12] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V12*/ meltfptr[10] =  /*fromline 1341*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4915:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L6*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:4915:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4915:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check modctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4915)?(4915):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V15*/
                        meltfptr[14] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V14*/
            meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4915:/ clear");
            /*clear*/ /*_#IS_OBJECT__L6*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[14] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:4916:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:4917:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V17*/
            meltfptr[16] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4918:/ getslot");
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
        /*_.CURIF__V21*/
        meltfptr[20] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_PAIR__L7*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])) == MELTOBMAG_PAIR);;
        /*^compute*/
        /*_#NOT__L8*/
        meltfnum[7] =
            (!(/*_#IS_PAIR__L7*/ meltfnum[0]));;
        MELT_LOCATION("warmelt-macro.melt:4921:/ cond");
        /*cond*/
        if (/*_#NOT__L8*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4922:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "missing condition in IF"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*_.PAIR_TAIL__V22*/
        meltfptr[21] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
        MELT_LOCATION("warmelt-macro.melt:4923:/ compute");
        /*_.CURPAIR__V20*/
        meltfptr[19] = /*_.SETQ___V23*/ meltfptr[22] = /*_.PAIR_TAIL__V22*/ meltfptr[21];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_PAIR__L9*/
        meltfnum[8] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])) == MELTOBMAG_PAIR);;
        /*^compute*/
        /*_#NOT__L10*/
        meltfnum[9] =
            (!(/*_#IS_PAIR__L9*/ meltfnum[8]));;
        MELT_LOCATION("warmelt-macro.melt:4924:/ cond");
        /*cond*/
        if (/*_#NOT__L10*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4925:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "missing then in IF"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:4926:/ quasiblock");



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
            /*_.XCOND__V25*/
            meltfptr[24] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURIF__V21*/ meltfptr[20]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4927:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4927:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4927:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4927;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_if xcond";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XCOND__V25*/ meltfptr[24];
                            /*_.MELT_DEBUG_FUN__V27*/
                            meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V26*/
                        meltfptr[25] = /*_.MELT_DEBUG_FUN__V27*/ meltfptr[26];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4927:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L12*/
                        meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V27*/
                        meltfptr[26] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V26*/ meltfptr[25] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4927:/ locexp");
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
            /*clear*/ /*_._IF___V26*/
            meltfptr[25] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:4928:/ quasiblock");


        /*_.CURTHEN__V29*/
        meltfptr[25] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
        /*^compute*/
        /*_.PAIR_TAIL__V30*/
        meltfptr[29] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
        MELT_LOCATION("warmelt-macro.melt:4929:/ compute");
        /*_.CURPAIR__V20*/
        meltfptr[19] = /*_.SETQ___V31*/ meltfptr[30] = /*_.PAIR_TAIL__V30*/ meltfptr[29];;
        MELT_LOCATION("warmelt-macro.melt:4930:/ quasiblock");



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
            /*_.XTHEN__V33*/
            meltfptr[32] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURTHEN__V29*/ meltfptr[25]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4932:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4932:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4932:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L14*/ meltfnum[10];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4932;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_if xthen";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XTHEN__V33*/ meltfptr[32];
                            /*_.MELT_DEBUG_FUN__V35*/
                            meltfptr[34] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V34*/
                        meltfptr[33] = /*_.MELT_DEBUG_FUN__V35*/ meltfptr[34];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4932:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L14*/
                        meltfnum[10] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V35*/
                        meltfptr[34] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V34*/ meltfptr[33] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4932:/ locexp");
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
            /*clear*/ /*_._IF___V34*/
            meltfptr[33] = 0 ;
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
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])) == MELTOBMAG_PAIR);;
        MELT_LOCATION("warmelt-macro.melt:4933:/ cond");
        /*cond*/
        if (/*_#IS_PAIR__L15*/ meltfnum[10]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:4934:/ quasiblock");


                    /*_.CURELSE__V38*/
                    meltfptr[37] =
                        (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4935:/ apply");
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
                        /*_.XELSE__V39*/
                        meltfptr[38] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURELSE__V38*/ meltfptr[37]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:4936:/ cppif.then");
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
                        MELT_LOCATION("warmelt-macro.melt:4936:/ cond");
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
                                    MELT_LOCATION("warmelt-macro.melt:4936:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L17*/ meltfnum[16];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 4936;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "mexpand_if xelse";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XELSE__V39*/ meltfptr[38];
                                        /*_.MELT_DEBUG_FUN__V41*/
                                        meltfptr[40] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V40*/
                                    meltfptr[39] = /*_.MELT_DEBUG_FUN__V41*/ meltfptr[40];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:4936:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L17*/
                                    meltfnum[16] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V41*/
                                    meltfptr[40] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V40*/ meltfptr[39] =  /*fromline 1341*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-macro.melt:4936:/ locexp");
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
                    /*_.PAIR_TAIL__V42*/
                    meltfptr[40] =
                        (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
                    MELT_LOCATION("warmelt-macro.melt:4937:/ compute");
                    /*_.CURPAIR__V20*/
                    meltfptr[19] = /*_.SETQ___V43*/ meltfptr[39] = /*_.PAIR_TAIL__V42*/ meltfptr[40];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_PAIR__L18*/
                    meltfnum[16] =
                        (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])) == MELTOBMAG_PAIR);;
                    MELT_LOCATION("warmelt-macro.melt:4938:/ cond");
                    /*cond*/
                    if (/*_#IS_PAIR__L18*/ meltfnum[16]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:4939:/ locexp");
                                    /* error_plain */
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "IF with more than three sons"), (melt_ptr_t)0);
                                }
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4940:/ quasiblock");



                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4941:/ quasiblock");


                    /*^rawallocobj*/
                    /*rawallocobj*/
                    {
                        melt_ptr_t newobj = 0;
                        melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_IFELSE*/ meltfrout->tabval[4])), (5), "CLASS_SOURCE_IFELSE");
                        /*_.INST__V46*/
                        meltfptr[45] =
                            newobj;
                    };
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V46*/ meltfptr[45])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V46*/ meltfptr[45]), (1), (/*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SIF_TEST", melt_magic_discr((melt_ptr_t)(/*_.INST__V46*/ meltfptr[45])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V46*/ meltfptr[45]), (2), (/*_.XCOND__V25*/ meltfptr[24]), "SIF_TEST");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SIF_THEN", melt_magic_discr((melt_ptr_t)(/*_.INST__V46*/ meltfptr[45])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V46*/ meltfptr[45]), (3), (/*_.XTHEN__V33*/ meltfptr[32]), "SIF_THEN");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SIF_ELSE", melt_magic_discr((melt_ptr_t)(/*_.INST__V46*/ meltfptr[45])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V46*/ meltfptr[45]), (4), (/*_.XELSE__V39*/ meltfptr[38]), "SIF_ELSE");
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object (/*_.INST__V46*/ meltfptr[45], "newly made instance");
                    ;
                    /*_.RESE__V45*/
                    meltfptr[44] = /*_.INST__V46*/ meltfptr[45];;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:4947:/ cppif.then");
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
                        MELT_LOCATION("warmelt-macro.melt:4947:/ cond");
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
                                    MELT_LOCATION("warmelt-macro.melt:4947:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L20*/ meltfnum[19];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 4947;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "mexpand_if with else return rese";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RESE__V45*/ meltfptr[44];
                                        /*_.MELT_DEBUG_FUN__V48*/
                                        meltfptr[47] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V47*/
                                    meltfptr[46] = /*_.MELT_DEBUG_FUN__V48*/ meltfptr[47];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:4947:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L20*/
                                    meltfnum[19] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V48*/
                                    meltfptr[47] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V47*/ meltfptr[46] =  /*fromline 1341*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-macro.melt:4947:/ locexp");
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
                        /*clear*/ /*_._IF___V47*/
                        meltfptr[46] = 0 ;
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
                    MELT_LOCATION("warmelt-macro.melt:4948:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = /*_.RESE__V45*/ meltfptr[44];;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4948:/ locexp");
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
                    /*_.LET___V44*/
                    meltfptr[43] = /*_.RETURN___V49*/ meltfptr[47];;

                    MELT_LOCATION("warmelt-macro.melt:4940:/ clear");
                    /*clear*/ /*_.RESE__V45*/
                    meltfptr[44] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V49*/
                    meltfptr[47] = 0 ;
                    /*_.LET___V37*/
                    meltfptr[33] = /*_.LET___V44*/ meltfptr[43];;

                    MELT_LOCATION("warmelt-macro.melt:4934:/ clear");
                    /*clear*/ /*_.CURELSE__V38*/
                    meltfptr[37] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.XELSE__V39*/
                    meltfptr[38] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PAIR_TAIL__V42*/
                    meltfptr[40] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V43*/
                    meltfptr[39] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#IS_PAIR__L18*/
                    meltfnum[16] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.LET___V44*/
                    meltfptr[43] = 0 ;
                    /*_._IFELSE___V36*/
                    meltfptr[34] = /*_.LET___V37*/ meltfptr[33];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4933:/ clear");
                    /*clear*/ /*_.LET___V37*/
                    meltfptr[33] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:4949:/ quasiblock");



                    MELT_CHECK_SIGNAL();
                    ;
                    /*^quasiblock*/


                    /*^rawallocobj*/
                    /*rawallocobj*/
                    {
                        melt_ptr_t newobj = 0;
                        melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_IF*/ meltfrout->tabval[5])), (4), "CLASS_SOURCE_IF");
                        /*_.INST__V52*/
                        meltfptr[47] =
                            newobj;
                    };
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V52*/ meltfptr[47])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V52*/ meltfptr[47]), (1), (/*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SIF_TEST", melt_magic_discr((melt_ptr_t)(/*_.INST__V52*/ meltfptr[47])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V52*/ meltfptr[47]), (2), (/*_.XCOND__V25*/ meltfptr[24]), "SIF_TEST");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SIF_THEN", melt_magic_discr((melt_ptr_t)(/*_.INST__V52*/ meltfptr[47])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V52*/ meltfptr[47]), (3), (/*_.XTHEN__V33*/ meltfptr[32]), "SIF_THEN");
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object (/*_.INST__V52*/ meltfptr[47], "newly made instance");
                    ;
                    /*_.RESP__V51*/
                    meltfptr[44] = /*_.INST__V52*/ meltfptr[47];;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:4953:/ cppif.then");
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
                        MELT_LOCATION("warmelt-macro.melt:4953:/ cond");
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
                                    MELT_LOCATION("warmelt-macro.melt:4953:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L22*/ meltfnum[11];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 4953;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "mexpand_if plain return resp";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RESP__V51*/ meltfptr[44];
                                        /*_.MELT_DEBUG_FUN__V54*/
                                        meltfptr[38] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V53*/
                                    meltfptr[37] = /*_.MELT_DEBUG_FUN__V54*/ meltfptr[38];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:4953:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L22*/
                                    meltfnum[11] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V54*/
                                    meltfptr[38] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V53*/ meltfptr[37] =  /*fromline 1341*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-macro.melt:4953:/ locexp");
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
                        /*clear*/ /*_._IF___V53*/
                        meltfptr[37] = 0 ;
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
                    MELT_LOCATION("warmelt-macro.melt:4954:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = /*_.RESP__V51*/ meltfptr[44];;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4954:/ locexp");
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
                    /*_.LET___V50*/
                    meltfptr[46] = /*_.RETURN___V55*/ meltfptr[40];;

                    MELT_LOCATION("warmelt-macro.melt:4949:/ clear");
                    /*clear*/ /*_.RESP__V51*/
                    meltfptr[44] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V55*/
                    meltfptr[40] = 0 ;
                    /*_._IFELSE___V36*/
                    meltfptr[34] = /*_.LET___V50*/ meltfptr[46];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4933:/ clear");
                    /*clear*/ /*_.LET___V50*/
                    meltfptr[46] = 0 ;
                }
                ;
            }
        ;
        /*_.LET___V32*/
        meltfptr[31] = /*_._IFELSE___V36*/ meltfptr[34];;

        MELT_LOCATION("warmelt-macro.melt:4930:/ clear");
        /*clear*/ /*_.XTHEN__V33*/
        meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L15*/
        meltfnum[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V36*/
        meltfptr[34] = 0 ;
        /*_.LET___V28*/
        meltfptr[26] = /*_.LET___V32*/ meltfptr[31];;

        MELT_LOCATION("warmelt-macro.melt:4928:/ clear");
        /*clear*/ /*_.CURTHEN__V29*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V30*/
        meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V31*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V32*/
        meltfptr[31] = 0 ;
        /*_.LET___V24*/
        meltfptr[23] = /*_.LET___V28*/ meltfptr[26];;

        MELT_LOCATION("warmelt-macro.melt:4926:/ clear");
        /*clear*/ /*_.XCOND__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V28*/
        meltfptr[26] = 0 ;
        /*_.LET___V16*/
        meltfptr[14] = /*_.LET___V24*/ meltfptr[23];;

        MELT_LOCATION("warmelt-macro.melt:4916:/ clear");
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
        /*clear*/ /*_.CURIF__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L7*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L8*/
        meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L9*/
        meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L10*/
        meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V24*/
        meltfptr[23] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4910:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-macro.melt:4910:/ locexp");
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
        /*clear*/ /*_.IFCPP___V10*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L5*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V12*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V16*/
        meltfptr[14] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_IF", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_81_WARMELTmiMACRO_MEXPAND_IF_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_81_WARMELTmiMACRO_MEXPAND_IF*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN fromline 1716 */

    /** start of frame for meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN// fromline 1531
        : public Melt_CallFrameWithValues<31>
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
        MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<31> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN), clos) {};
        MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN() //the constructor fromline 1606
            : Melt_CallFrameWithValues<31> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<31> (fil,lin, sizeof(MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN)) {};
        MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<31> (fil,lin, sizeof(MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN


    /** end of frame for meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN fromline 1661**/

    /* end of frame for routine meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN fromline 1720 */

    /* classy proc frame meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN */
    MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
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
    MELT_LOCATION("warmelt-macro.melt:4964:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4965:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4965:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4965:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4965;
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

                        MELT_LOCATION("warmelt-macro.melt:4965:/ clear");
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

                    /*_._IF___V6*/ meltfptr[5] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4965:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4966:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4966:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4966:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4966)?(4966):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V9*/
                        meltfptr[5] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4966:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4967:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:4967:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[10] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4967:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4967)?(4967):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V11*/
                        meltfptr[10] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V10*/
            meltfptr[5] = /*_._IFELSE___V11*/ meltfptr[10];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4967:/ clear");
            /*clear*/ /*_#IS_A__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V11*/
            meltfptr[10] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V10*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L5*/
        meltfnum[1] =
            ((/*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:4968:/ cond");
        /*cond*/
        if (/*_#NULL__L5*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V13*/ meltfptr[12] = (/*!MACROEXPAND_1*/ meltfrout->tabval[3]);;
                    /*_._IF___V12*/
                    meltfptr[10] = /*_.SETQ___V13*/ meltfptr[12];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4968:/ clear");
                    /*clear*/ /*_.SETQ___V13*/
                    meltfptr[12] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V12*/ meltfptr[10] =  /*fromline 1341*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4969:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L6*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:4969:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4969:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check modctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4969)?(4969):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V15*/
                        meltfptr[14] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V14*/
            meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4969:/ clear");
            /*clear*/ /*_#IS_OBJECT__L6*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[14] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:4970:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:4971:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V17*/
            meltfptr[16] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4972:/ getslot");
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
        /*_.CURIF__V21*/
        meltfptr[20] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_PAIR__L7*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])) == MELTOBMAG_PAIR);;
        /*^compute*/
        /*_#NOT__L8*/
        meltfnum[7] =
            (!(/*_#IS_PAIR__L7*/ meltfnum[0]));;
        MELT_LOCATION("warmelt-macro.melt:4976:/ cond");
        /*cond*/
        if (/*_#NOT__L8*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4977:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "missing condition in WHEN"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*_.PAIR_TAIL__V22*/
        meltfptr[21] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
        MELT_LOCATION("warmelt-macro.melt:4978:/ compute");
        /*_.CURPAIR__V20*/
        meltfptr[19] = /*_.SETQ___V23*/ meltfptr[22] = /*_.PAIR_TAIL__V22*/ meltfptr[21];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_PAIR__L9*/
        meltfnum[8] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])) == MELTOBMAG_PAIR);;
        /*^compute*/
        /*_#NOT__L10*/
        meltfnum[9] =
            (!(/*_#IS_PAIR__L9*/ meltfnum[8]));;
        MELT_LOCATION("warmelt-macro.melt:4979:/ cond");
        /*cond*/
        if (/*_#NOT__L10*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4980:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "missing body in WHEN"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:4981:/ quasiblock");



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
            /*_.XCOND__V25*/
            meltfptr[24] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURIF__V21*/ meltfptr[20]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4982:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LOC__V18*/ meltfptr[17];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.XPROGN__V26*/
            meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!PAIRLIST_TO_PROGN*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4983:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_IF*/ meltfrout->tabval[5])), (4), "CLASS_SOURCE_IF");
            /*_.INST__V28*/
            meltfptr[27] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V28*/ meltfptr[27])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V28*/ meltfptr[27]), (1), (/*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIF_TEST", melt_magic_discr((melt_ptr_t)(/*_.INST__V28*/ meltfptr[27])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V28*/ meltfptr[27]), (2), (/*_.XCOND__V25*/ meltfptr[24]), "SIF_TEST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIF_THEN", melt_magic_discr((melt_ptr_t)(/*_.INST__V28*/ meltfptr[27])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V28*/ meltfptr[27]), (3), (/*_.XPROGN__V26*/ meltfptr[25]), "SIF_THEN");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V28*/ meltfptr[27], "newly made instance");
        ;
        /*_.XWHEN__V27*/
        meltfptr[26] = /*_.INST__V28*/ meltfptr[27];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4988:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4988:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4988:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4988;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_when return xwhen=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XWHEN__V27*/ meltfptr[26];
                            /*_.MELT_DEBUG_FUN__V30*/
                            meltfptr[29] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V29*/
                        meltfptr[28] = /*_.MELT_DEBUG_FUN__V30*/ meltfptr[29];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4988:/ clear");
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

                    /*_._IF___V29*/ meltfptr[28] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:4988:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4989:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.XWHEN__V27*/ meltfptr[26];;

        {
            MELT_LOCATION("warmelt-macro.melt:4989:/ locexp");
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
        /*_.LET___V24*/
        meltfptr[23] = /*_.RETURN___V31*/ meltfptr[29];;

        MELT_LOCATION("warmelt-macro.melt:4981:/ clear");
        /*clear*/ /*_.XCOND__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XPROGN__V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XWHEN__V27*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V31*/
        meltfptr[29] = 0 ;
        /*_.LET___V16*/
        meltfptr[14] = /*_.LET___V24*/ meltfptr[23];;

        MELT_LOCATION("warmelt-macro.melt:4970:/ clear");
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
        /*clear*/ /*_.CURIF__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L7*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L8*/
        meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L9*/
        meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L10*/
        meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V24*/
        meltfptr[23] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4964:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-macro.melt:4964:/ locexp");
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
        /*clear*/ /*_.IFCPP___V10*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L5*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V12*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V16*/
        meltfptr[14] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_WHEN", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_82_WARMELTmiMACRO_MEXPAND_WHEN*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS fromline 1716 */

    /** start of frame for meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS// fromline 1531
        : public Melt_CallFrameWithValues<31>
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
        MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<31> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS), clos) {};
        MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS() //the constructor fromline 1606
            : Melt_CallFrameWithValues<31> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<31> (fil,lin, sizeof(MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS)) {};
        MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<31> (fil,lin, sizeof(MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS


    /** end of frame for meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS fromline 1661**/

    /* end of frame for routine meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS fromline 1720 */

    /* classy proc frame meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS */
    MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
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
    MELT_LOCATION("warmelt-macro.melt:5000:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:5001:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:5001:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:5001:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5001;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_unless sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V7*/
                            meltfptr[6] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V6*/
                        meltfptr[5] = /*_.MELT_DEBUG_FUN__V7*/ meltfptr[6];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5001:/ clear");
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

                    /*_._IF___V6*/ meltfptr[5] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5001:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:5002:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:5002:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5002:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5002)?(5002):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V9*/
                        meltfptr[5] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[5];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5002:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5003:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:5003:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[10] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5003:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5003)?(5003):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V11*/
                        meltfptr[10] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V10*/
            meltfptr[5] = /*_._IFELSE___V11*/ meltfptr[10];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5003:/ clear");
            /*clear*/ /*_#IS_A__L4*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V11*/
            meltfptr[10] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V10*/ meltfptr[5] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L5*/
        meltfnum[1] =
            ((/*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:5004:/ cond");
        /*cond*/
        if (/*_#NULL__L5*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V13*/ meltfptr[12] = (/*!MACROEXPAND_1*/ meltfrout->tabval[3]);;
                    /*_._IF___V12*/
                    meltfptr[10] = /*_.SETQ___V13*/ meltfptr[12];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5004:/ clear");
                    /*clear*/ /*_.SETQ___V13*/
                    meltfptr[12] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V12*/ meltfptr[10] =  /*fromline 1341*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5005:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L6*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:5005:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5005:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check modctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5005)?(5005):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V15*/
                        meltfptr[14] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V14*/
            meltfptr[12] = /*_._IFELSE___V15*/ meltfptr[14];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5005:/ clear");
            /*clear*/ /*_#IS_OBJECT__L6*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V15*/
            meltfptr[14] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V14*/ meltfptr[12] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5006:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:5007:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V17*/
            meltfptr[16] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5008:/ getslot");
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
        /*_.CURIF__V21*/
        meltfptr[20] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_PAIR__L7*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])) == MELTOBMAG_PAIR);;
        /*^compute*/
        /*_#NOT__L8*/
        meltfnum[7] =
            (!(/*_#IS_PAIR__L7*/ meltfnum[0]));;
        MELT_LOCATION("warmelt-macro.melt:5012:/ cond");
        /*cond*/
        if (/*_#NOT__L8*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5013:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "missing condition in UNLESS"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*_.PAIR_TAIL__V22*/
        meltfptr[21] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
        MELT_LOCATION("warmelt-macro.melt:5014:/ compute");
        /*_.CURPAIR__V20*/
        meltfptr[19] = /*_.SETQ___V23*/ meltfptr[22] = /*_.PAIR_TAIL__V22*/ meltfptr[21];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_PAIR__L9*/
        meltfnum[8] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])) == MELTOBMAG_PAIR);;
        /*^compute*/
        /*_#NOT__L10*/
        meltfnum[9] =
            (!(/*_#IS_PAIR__L9*/ meltfnum[8]));;
        MELT_LOCATION("warmelt-macro.melt:5015:/ cond");
        /*cond*/
        if (/*_#NOT__L10*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5016:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "missing body in UNLESS"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5017:/ quasiblock");



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
            /*_.XCOND__V25*/
            meltfptr[24] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURIF__V21*/ meltfptr[20]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5018:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[4];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LOC__V18*/ meltfptr[17];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.XPROGN__V26*/
            meltfptr[25] =  melt_apply ((meltclosure_ptr_t)((/*!PAIRLIST_TO_PROGN*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5019:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_IFELSE*/ meltfrout->tabval[5])), (5), "CLASS_SOURCE_IFELSE");
            /*_.INST__V28*/
            meltfptr[27] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V28*/ meltfptr[27])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V28*/ meltfptr[27]), (1), (/*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIF_TEST", melt_magic_discr((melt_ptr_t)(/*_.INST__V28*/ meltfptr[27])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V28*/ meltfptr[27]), (2), (/*_.XCOND__V25*/ meltfptr[24]), "SIF_TEST");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIF_THEN", melt_magic_discr((melt_ptr_t)(/*_.INST__V28*/ meltfptr[27])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V28*/ meltfptr[27]), (3), ((/*nil*/NULL)), "SIF_THEN");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIF_ELSE", melt_magic_discr((melt_ptr_t)(/*_.INST__V28*/ meltfptr[27])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V28*/ meltfptr[27]), (4), (/*_.XPROGN__V26*/ meltfptr[25]), "SIF_ELSE");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V28*/ meltfptr[27], "newly made instance");
        ;
        /*_.XUNLESS__V27*/
        meltfptr[26] = /*_.INST__V28*/ meltfptr[27];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5025:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:5025:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:5025:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5025;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_unless return xunless=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XUNLESS__V27*/ meltfptr[26];
                            /*_.MELT_DEBUG_FUN__V30*/
                            meltfptr[29] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V29*/
                        meltfptr[28] = /*_.MELT_DEBUG_FUN__V30*/ meltfptr[29];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5025:/ clear");
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

                    /*_._IF___V29*/ meltfptr[28] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5025:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:5026:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.XUNLESS__V27*/ meltfptr[26];;

        {
            MELT_LOCATION("warmelt-macro.melt:5026:/ locexp");
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
        /*_.LET___V24*/
        meltfptr[23] = /*_.RETURN___V31*/ meltfptr[29];;

        MELT_LOCATION("warmelt-macro.melt:5017:/ clear");
        /*clear*/ /*_.XCOND__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XPROGN__V26*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XUNLESS__V27*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V31*/
        meltfptr[29] = 0 ;
        /*_.LET___V16*/
        meltfptr[14] = /*_.LET___V24*/ meltfptr[23];;

        MELT_LOCATION("warmelt-macro.melt:5006:/ clear");
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
        /*clear*/ /*_.CURIF__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L7*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L8*/
        meltfnum[7] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L9*/
        meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L10*/
        meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V24*/
        meltfptr[23] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5000:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-macro.melt:5000:/ locexp");
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
        /*clear*/ /*_.IFCPP___V10*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L5*/
        meltfnum[1] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V12*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V14*/
        meltfptr[12] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V16*/
        meltfptr[14] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_UNLESS", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_83_WARMELTmiMACRO_MEXPAND_UNLESS*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF fromline 1716 */

    /** start of frame for meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF// fromline 1531
        : public Melt_CallFrameWithValues<53>
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
        MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<53> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF), clos) {};
        MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF() //the constructor fromline 1606
            : Melt_CallFrameWithValues<53> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<53> (fil,lin, sizeof(MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF)) {};
        MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<53> (fil,lin, sizeof(MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF


    /** end of frame for meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF fromline 1661**/

    /* end of frame for routine meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF fromline 1720 */

    /* classy proc frame meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF */
    MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
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
    MELT_LOCATION("warmelt-macro.melt:5036:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:5037:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:5037:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5037:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5037)?(5037):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V7*/
                        meltfptr[6] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V6*/
            meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5037:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5038:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:5038:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[8] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5038:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5038)?(5038):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V9*/
                        meltfptr[8] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[8];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5038:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/
            meltfptr[8] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#NULL__L3*/
        meltfnum[0] =
            ((/*_.MEXPANDER__V4*/ meltfptr[3]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:5039:/ cond");
        /*cond*/
        if (/*_#NULL__L3*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    /*^compute*/
                    /*_.MEXPANDER__V4*/ meltfptr[3] = /*_.SETQ___V11*/ meltfptr[10] = (/*!MACROEXPAND_1*/ meltfrout->tabval[2]);;
                    /*_._IF___V10*/
                    meltfptr[8] = /*_.SETQ___V11*/ meltfptr[10];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5039:/ clear");
                    /*clear*/ /*_.SETQ___V11*/
                    meltfptr[10] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V10*/ meltfptr[8] =  /*fromline 1341*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5040:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L4*/
            meltfnum[3] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:5040:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L4*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V13*/ meltfptr[12] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5040:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check modctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5040)?(5040):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V13*/
                        meltfptr[12] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V12*/
            meltfptr[10] = /*_._IFELSE___V13*/ meltfptr[12];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5040:/ clear");
            /*clear*/ /*_#IS_OBJECT__L4*/
            meltfnum[3] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V13*/
            meltfptr[12] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V12*/ meltfptr[10] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5041:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:5041:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:5041:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L6*/ meltfnum[5];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5041;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_cppif sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V15*/
                            meltfptr[14] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V14*/
                        meltfptr[12] = /*_.MELT_DEBUG_FUN__V15*/ meltfptr[14];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5041:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L6*/
                        meltfnum[5] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V15*/
                        meltfptr[14] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V14*/ meltfptr[12] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5041:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:5042:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:5043:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V17*/
            meltfptr[12] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5044:/ getslot");
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
        /*^compute*/
        /*_.CURIF__V21*/
        meltfptr[20] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_PAIR__L7*/
        meltfnum[5] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])) == MELTOBMAG_PAIR);;
        /*^compute*/
        /*_#NOT__L8*/
        meltfnum[3] =
            (!(/*_#IS_PAIR__L7*/ meltfnum[5]));;
        MELT_LOCATION("warmelt-macro.melt:5048:/ cond");
        /*cond*/
        if (/*_#NOT__L8*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5049:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "missing condition in CPPIF"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        /*_.PAIR_TAIL__V22*/
        meltfptr[21] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
        MELT_LOCATION("warmelt-macro.melt:5050:/ compute");
        /*_.CURPAIR__V20*/
        meltfptr[19] = /*_.SETQ___V23*/ meltfptr[22] = /*_.PAIR_TAIL__V22*/ meltfptr[21];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_PAIR__L9*/
        meltfnum[8] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])) == MELTOBMAG_PAIR);;
        /*^compute*/
        /*_#NOT__L10*/
        meltfnum[9] =
            (!(/*_#IS_PAIR__L9*/ meltfnum[8]));;
        MELT_LOCATION("warmelt-macro.melt:5051:/ cond");
        /*cond*/
        if (/*_#NOT__L10*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5052:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "missing then in CPPIF"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5053:/ quasiblock");



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
            /*_.XCOND__V25*/
            meltfptr[24] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURIF__V21*/ meltfptr[20]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5054:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:5054:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:5054:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5054;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_cppif xcond";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XCOND__V25*/ meltfptr[24];
                            /*_.MELT_DEBUG_FUN__V27*/
                            meltfptr[26] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V26*/
                        meltfptr[25] = /*_.MELT_DEBUG_FUN__V27*/ meltfptr[26];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5054:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L12*/
                        meltfnum[11] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V27*/
                        meltfptr[26] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V26*/ meltfptr[25] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5054:/ locexp");
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
            /*clear*/ /*_._IF___V26*/
            meltfptr[25] = 0 ;
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
            (melt_magic_discr((melt_ptr_t)(/*_.XCOND__V25*/ meltfptr[24])) == MELTOBMAG_STRING);;
        MELT_LOCATION("warmelt-macro.melt:5056:/ cond");
        /*cond*/
        if (/*_#IS_STRING__L13*/ meltfnum[11]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V28*/ meltfptr[26] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-macro.melt:5056:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_A__L14*/
                    meltfnum[10] =
                        melt_is_instance_of((melt_ptr_t)(/*_.XCOND__V25*/ meltfptr[24]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[4])));;
                    MELT_LOCATION("warmelt-macro.melt:5057:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L14*/ meltfnum[10]) /*then*/
                        {
                            /*^cond.then*/
                            /*_._IFELSE___V29*/ meltfptr[25] = (/*nil*/NULL);;
                        }
                    else
                        {
                            MELT_LOCATION("warmelt-macro.melt:5057:/ cond.else");

                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:5059:/ locexp");
                                    /* error_plain */
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "invalid cpp-condition in CPPIF - string or symbol expected"), (melt_ptr_t)0);
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-macro.melt:5060:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] = (/*nil*/NULL);;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:5060:/ locexp");
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
                                MELT_LOCATION("warmelt-macro.melt:5058:/ quasiblock");


                                /*_.PROGN___V31*/
                                meltfptr[30] = /*_.RETURN___V30*/ meltfptr[29];;
                                /*^compute*/
                                /*_._IFELSE___V29*/
                                meltfptr[25] = /*_.PROGN___V31*/ meltfptr[30];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:5057:/ clear");
                                /*clear*/ /*_.RETURN___V30*/
                                meltfptr[29] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V31*/
                                meltfptr[30] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V28*/
                    meltfptr[26] = /*_._IFELSE___V29*/ meltfptr[25];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5056:/ clear");
                    /*clear*/ /*_#IS_A__L14*/
                    meltfnum[10] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V29*/
                    meltfptr[25] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:5061:/ quasiblock");


        /*_.CURTHEN__V33*/
        meltfptr[30] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
        /*^compute*/
        /*_.PAIR_TAIL__V34*/
        meltfptr[25] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
        MELT_LOCATION("warmelt-macro.melt:5062:/ compute");
        /*_.CURPAIR__V20*/
        meltfptr[19] = /*_.SETQ___V35*/ meltfptr[34] = /*_.PAIR_TAIL__V34*/ meltfptr[25];;
        MELT_LOCATION("warmelt-macro.melt:5063:/ quasiblock");



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
            /*_.XTHEN__V37*/
            meltfptr[36] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURTHEN__V33*/ meltfptr[30]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.XELSE__V38*/
        meltfptr[37] = (/*nil*/NULL);;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5066:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:5066:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:5066:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L16*/ meltfnum[15];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5066;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_cppif xthen";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XTHEN__V37*/ meltfptr[36];
                            /*_.MELT_DEBUG_FUN__V40*/
                            meltfptr[39] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V39*/
                        meltfptr[38] = /*_.MELT_DEBUG_FUN__V40*/ meltfptr[39];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5066:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L16*/
                        meltfnum[15] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V40*/
                        meltfptr[39] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V39*/ meltfptr[38] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5066:/ locexp");
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
            /*clear*/ /*_._IF___V39*/
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

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_PAIR__L17*/
        meltfnum[15] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])) == MELTOBMAG_PAIR);;
        MELT_LOCATION("warmelt-macro.melt:5067:/ cond");
        /*cond*/
        if (/*_#IS_PAIR__L17*/ meltfnum[15]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:5068:/ quasiblock");


                    /*_.CURELSE__V41*/
                    meltfptr[39] =
                        (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5069:/ apply");
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
                        /*_.GOTXELSE__V42*/
                        meltfptr[38] =  melt_apply ((meltclosure_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3]), (melt_ptr_t)(/*_.CURELSE__V41*/ meltfptr[39]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:5070:/ cppif.then");
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
                        MELT_LOCATION("warmelt-macro.melt:5070:/ cond");
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
                                    MELT_LOCATION("warmelt-macro.melt:5070:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L19*/ meltfnum[18];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 5070;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "mexpand_cppif gotxelse";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.GOTXELSE__V42*/ meltfptr[38];
                                        /*_.MELT_DEBUG_FUN__V44*/
                                        meltfptr[43] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V43*/
                                    meltfptr[42] = /*_.MELT_DEBUG_FUN__V44*/ meltfptr[43];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:5070:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L19*/
                                    meltfnum[18] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V44*/
                                    meltfptr[43] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V43*/ meltfptr[42] =  /*fromline 1341*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-macro.melt:5070:/ locexp");
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
                        (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
                    MELT_LOCATION("warmelt-macro.melt:5071:/ compute");
                    /*_.CURPAIR__V20*/
                    meltfptr[19] = /*_.SETQ___V46*/ meltfptr[42] = /*_.PAIR_TAIL__V45*/ meltfptr[43];;
                    MELT_LOCATION("warmelt-macro.melt:5072:/ compute");
                    /*_.XELSE__V38*/
                    meltfptr[37] = /*_.SETQ___V47*/ meltfptr[46] = /*_.GOTXELSE__V42*/ meltfptr[38];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_PAIR__L20*/
                    meltfnum[18] =
                        (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])) == MELTOBMAG_PAIR);;
                    MELT_LOCATION("warmelt-macro.melt:5073:/ cond");
                    /*cond*/
                    if (/*_#IS_PAIR__L20*/ meltfnum[18]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:5074:/ locexp");
                                    /* error_plain */
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "CPPIF with more than three sons"), (melt_ptr_t)0);
                                }
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;

                    MELT_LOCATION("warmelt-macro.melt:5068:/ clear");
                    /*clear*/ /*_.CURELSE__V41*/
                    meltfptr[39] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.GOTXELSE__V42*/
                    meltfptr[38] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PAIR_TAIL__V45*/
                    meltfptr[43] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V46*/
                    meltfptr[42] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V47*/
                    meltfptr[46] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#IS_PAIR__L20*/
                    meltfnum[18] = 0 ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5075:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^quasiblock*/


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_CPPIF*/ meltfrout->tabval[5])), (5), "CLASS_SOURCE_CPPIF");
            /*_.INST__V50*/
            meltfptr[43] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V50*/ meltfptr[43])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V50*/ meltfptr[43]), (1), (/*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIFP_COND", melt_magic_discr((melt_ptr_t)(/*_.INST__V50*/ meltfptr[43])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V50*/ meltfptr[43]), (2), (/*_.XCOND__V25*/ meltfptr[24]), "SIFP_COND");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIFP_THEN", melt_magic_discr((melt_ptr_t)(/*_.INST__V50*/ meltfptr[43])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V50*/ meltfptr[43]), (3), (/*_.XTHEN__V37*/ meltfptr[36]), "SIFP_THEN");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SIFP_ELSE", melt_magic_discr((melt_ptr_t)(/*_.INST__V50*/ meltfptr[43])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V50*/ meltfptr[43]), (4), (/*_.XELSE__V38*/ meltfptr[37]), "SIFP_ELSE");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V50*/ meltfptr[43], "newly made instance");
        ;
        /*_.RESP__V49*/
        meltfptr[38] = /*_.INST__V50*/ meltfptr[43];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5082:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:5082:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:5082:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L22*/ meltfnum[18];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5082;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_cppif return resp";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.RESP__V49*/ meltfptr[38];
                            /*_.MELT_DEBUG_FUN__V52*/
                            meltfptr[46] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[3])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V51*/
                        meltfptr[42] = /*_.MELT_DEBUG_FUN__V52*/ meltfptr[46];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5082:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L22*/
                        meltfnum[18] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V52*/
                        meltfptr[46] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V51*/ meltfptr[42] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5082:/ locexp");
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
            /*clear*/ /*_._IF___V51*/
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

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5083:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.RESP__V49*/ meltfptr[38];;

        {
            MELT_LOCATION("warmelt-macro.melt:5083:/ locexp");
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
        /*_.LET___V48*/
        meltfptr[39] = /*_.RETURN___V53*/ meltfptr[46];;

        MELT_LOCATION("warmelt-macro.melt:5075:/ clear");
        /*clear*/ /*_.RESP__V49*/
        meltfptr[38] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V53*/
        meltfptr[46] = 0 ;
        /*_.LET___V36*/
        meltfptr[35] = /*_.LET___V48*/ meltfptr[39];;

        MELT_LOCATION("warmelt-macro.melt:5063:/ clear");
        /*clear*/ /*_.XTHEN__V37*/
        meltfptr[36] = 0 ;
        /*^clear*/
        /*clear*/ /*_.XELSE__V38*/
        meltfptr[37] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L17*/
        meltfnum[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V48*/
        meltfptr[39] = 0 ;
        /*_.LET___V32*/
        meltfptr[29] = /*_.LET___V36*/ meltfptr[35];;

        MELT_LOCATION("warmelt-macro.melt:5061:/ clear");
        /*clear*/ /*_.CURTHEN__V33*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V34*/
        meltfptr[25] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V35*/
        meltfptr[34] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V36*/
        meltfptr[35] = 0 ;
        /*_.LET___V24*/
        meltfptr[23] = /*_.LET___V32*/ meltfptr[29];;

        MELT_LOCATION("warmelt-macro.melt:5053:/ clear");
        /*clear*/ /*_.XCOND__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_STRING__L13*/
        meltfnum[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V28*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V32*/
        meltfptr[29] = 0 ;
        /*_.LET___V16*/
        meltfptr[14] = /*_.LET___V24*/ meltfptr[23];;

        MELT_LOCATION("warmelt-macro.melt:5042:/ clear");
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
        /*clear*/ /*_.CURIF__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L7*/
        meltfnum[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L8*/
        meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_PAIR__L9*/
        meltfnum[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NOT__L10*/
        meltfnum[9] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V24*/
        meltfptr[23] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5036:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-macro.melt:5036:/ locexp");
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
        /*clear*/ /*_.IFCPP___V8*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L3*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V10*/
        meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V12*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V16*/
        meltfptr[14] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_CPPIF", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_84_WARMELTmiMACRO_MEXPAND_CPPIF*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION fromline 1716 */

    /** start of frame for meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION// fromline 1531
        : public Melt_CallFrameWithValues<7>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<7> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION), clos) {};
        MeltFrame_meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION() //the constructor fromline 1606
            : Melt_CallFrameWithValues<7> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<7> (fil,lin, sizeof(MeltFrame_meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION)) {};
        MeltFrame_meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<7> (fil,lin, sizeof(MeltFrame_meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION


    /** end of frame for meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION fromline 1661**/

    /* end of frame for routine meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION fromline 1720 */

    /* classy proc frame meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION */
    MeltFrame_meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("FILTERGCCVERSION", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5093:/ getarg");
    /*_.VERSIONSTR__V2*/
    meltfptr[1] = (melt_ptr_t) meltfirstargp_;
    ;
    goto meltlab_endgetargs;
meltlab_endgetargs:
    ;
    /*body*/
    /*^block*/
    /*anyblock*/
    {


#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5094:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_STRING__L1*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.VERSIONSTR__V2*/ meltfptr[1])) == MELTOBMAG_STRING);;
            MELT_LOCATION("warmelt-macro.melt:5094:/ cond");
            /*cond*/
            if (/*_#IS_STRING__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V4*/ meltfptr[3] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5094:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check versionstr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5094)?(5094):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V4*/
                        meltfptr[3] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V3*/
            meltfptr[2] = /*_._IFELSE___V4*/ meltfptr[3];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5094:/ clear");
            /*clear*/ /*_#IS_STRING__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V4*/
            meltfptr[3] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V3*/ meltfptr[2] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5095:/ quasiblock");


        /*_#VERSIONLEN__L2*/
        meltfnum[0] =
            melt_string_length((melt_ptr_t)(/*_.VERSIONSTR__V2*/ meltfptr[1]));;
        /*^compute*/
        /*_.RES__V6*/
        meltfptr[5] = (/*nil*/NULL);;

        {
            MELT_LOCATION("warmelt-macro.melt:5098:/ locexp");
            if (/*_#VERSIONLEN__L2*/ meltfnum[0]>0 /*FILTERGCC__1*/
                                     && !strncmp (melt_string_str((melt_ptr_t)/*_.VERSIONSTR__V2*/ meltfptr[1]),
                                                  melt_gccversionstr,
                                                  /*_#VERSIONLEN__L2*/ meltfnum[0]))
                /*_.RES__V6*/ meltfptr[5] = /*_.VERSIONSTR__V2*/ meltfptr[1];
            ;
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5106:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.RES__V6*/ meltfptr[5];;

        {
            MELT_LOCATION("warmelt-macro.melt:5106:/ locexp");
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
        /*_.LET___V5*/
        meltfptr[3] = /*_.RETURN___V7*/ meltfptr[6];;

        MELT_LOCATION("warmelt-macro.melt:5095:/ clear");
        /*clear*/ /*_#VERSIONLEN__L2*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RES__V6*/
        meltfptr[5] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V7*/
        meltfptr[6] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5093:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V5*/ meltfptr[3];;

        {
            MELT_LOCATION("warmelt-macro.melt:5093:/ locexp");
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
        /*clear*/ /*_.IFCPP___V3*/
        meltfptr[2] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V5*/
        meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("FILTERGCCVERSION", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_85_WARMELTmiMACRO_FILTERGCCVERSION*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF fromline 1716 */

    /** start of frame for meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF// fromline 1531
        : public Melt_CallFrameWithValues<58>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<58> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF), clos) {};
        MeltFrame_meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF() //the constructor fromline 1606
            : Melt_CallFrameWithValues<58> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<58> (fil,lin, sizeof(MeltFrame_meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF)) {};
        MeltFrame_meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<58> (fil,lin, sizeof(MeltFrame_meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF


    /** end of frame for meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF fromline 1661**/

    /* end of frame for routine meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF fromline 1720 */

    /* classy proc frame meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF */
    MeltFrame_meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_GCCIF", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5108:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:5109:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:5109:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5109:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5109)?(5109):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V7*/
                        meltfptr[6] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V6*/
            meltfptr[5] = /*_._IFELSE___V7*/ meltfptr[6];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5109:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5110:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:5110:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[8] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5110:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5110)?(5110):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V9*/
                        meltfptr[8] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V8*/
            meltfptr[6] = /*_._IFELSE___V9*/ meltfptr[8];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5110:/ clear");
            /*clear*/ /*_#IS_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V9*/
            meltfptr[8] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V8*/ meltfptr[6] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5111:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:5111:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:5111:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L4*/ meltfnum[3];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5111;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_gccif sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V11*/
                            meltfptr[10] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V10*/
                        meltfptr[8] = /*_.MELT_DEBUG_FUN__V11*/ meltfptr[10];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5111:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L4*/
                        meltfnum[3] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V11*/
                        meltfptr[10] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V10*/ meltfptr[8] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5111:/ locexp");
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
            /*clear*/ /*_._IF___V10*/
            meltfptr[8] = 0 ;
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
        MELT_LOCATION("warmelt-macro.melt:5112:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:5113:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V13*/
            meltfptr[8] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5114:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.SLOC__V14*/
            meltfptr[13] = slot;
        };
        ;
        /*_.LIST_FIRST__V15*/
        meltfptr[14] =
            (melt_list_first((melt_ptr_t)(/*_.CONT__V13*/ meltfptr[8])));;
        /*^compute*/
        /*_.CURPAIR__V16*/
        meltfptr[15] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V15*/ meltfptr[14])));;
        /*^compute*/
        /*_.CURIF__V17*/
        meltfptr[16] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;
        /*^compute*/
        /*_.RESTPAIR__V18*/
        meltfptr[17] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_A__L5*/
        meltfnum[3] =
            melt_is_instance_of((melt_ptr_t)(/*_.CURIF__V17*/ meltfptr[16]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
        MELT_LOCATION("warmelt-macro.melt:5119:/ cond");
        /*cond*/
        if (/*_#IS_A__L5*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:5120:/ quasiblock");


                    /*^cond*/
                    /*cond*/
                    if (
                        /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CURIF__V17*/ meltfptr[16]),
                                                      (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])))
                    ) /*then*/
                        {
                            /*^cond.then*/
                            /*^getslot*/
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.CURIF__V17*/ meltfptr[16]) /*=obj*/;
                                melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
                                /*_.XCURIF__V21*/
                                meltfptr[20] = slot;
                            };
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_.XCURIF__V21*/ meltfptr[20] =  /*fromline 1341*/ NULL ;;
                        }
                    ;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:5122:/ cppif.then");
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
                        MELT_LOCATION("warmelt-macro.melt:5122:/ cond");
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
                                    MELT_LOCATION("warmelt-macro.melt:5122:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[6];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 5122;
                                        /*^apply.arg*/
                                        argtab[3].meltbp_cstring =  "mexpand_gccif xcurif=";
                                        /*^apply.arg*/
                                        argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.XCURIF__V21*/ meltfptr[20];
                                        /*_.MELT_DEBUG_FUN__V23*/
                                        meltfptr[22] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                    }
                                    ;
                                    /*_._IF___V22*/
                                    meltfptr[21] = /*_.MELT_DEBUG_FUN__V23*/ meltfptr[22];;
                                    /*epilog*/

                                    MELT_LOCATION("warmelt-macro.melt:5122:/ clear");
                                    /*clear*/ /*_#MELT_CALLCOUNT__L7*/
                                    meltfnum[6] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.MELT_DEBUG_FUN__V23*/
                                    meltfptr[22] = 0 ;
                                }
                                ;
                            }
                        else    /*^cond.else*/
                            {

                                /*_._IF___V22*/ meltfptr[21] =  /*fromline 1341*/ NULL ;;
                            }
                        ;

                        {
                            MELT_LOCATION("warmelt-macro.melt:5122:/ locexp");
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
                        /*clear*/ /*_._IF___V22*/
                        meltfptr[21] = 0 ;
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
                    MELT_LOCATION("warmelt-macro.melt:5123:/ compute");
                    /*_.CURIF__V17*/
                    meltfptr[16] = /*_.SETQ___V24*/ meltfptr[22] = /*_.XCURIF__V21*/ meltfptr[20];;
                    /*_.LET___V20*/
                    meltfptr[19] = /*_.SETQ___V24*/ meltfptr[22];;

                    MELT_LOCATION("warmelt-macro.melt:5120:/ clear");
                    /*clear*/ /*_.XCURIF__V21*/
                    meltfptr[20] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V24*/
                    meltfptr[22] = 0 ;
                    /*_._IF___V19*/
                    meltfptr[18] = /*_.LET___V20*/ meltfptr[19];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5119:/ clear");
                    /*clear*/ /*_.LET___V20*/
                    meltfptr[19] = 0 ;
                }
                ;
            }
        else    /*^cond.else*/
            {

                /*_._IF___V19*/ meltfptr[18] =  /*fromline 1341*/ NULL ;;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5125:/ cppif.then");
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
            meltfnum[6] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5125:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L8*/ meltfnum[6]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L9*/ meltfnum[0] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:5125:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5125;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_gccif curif";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CURIF__V17*/ meltfptr[16];
                            /*_.MELT_DEBUG_FUN__V26*/
                            meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V25*/
                        meltfptr[21] = /*_.MELT_DEBUG_FUN__V26*/ meltfptr[20];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5125:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L9*/
                        meltfnum[0] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V26*/
                        meltfptr[20] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V25*/ meltfptr[21] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5125:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L8*/
            meltfnum[6] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V25*/
            meltfptr[21] = 0 ;
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
        /*_#IS_STRING__L10*/
        meltfnum[0] =
            (melt_magic_discr((melt_ptr_t)(/*_.CURIF__V17*/ meltfptr[16])) == MELTOBMAG_STRING);;
        MELT_LOCATION("warmelt-macro.melt:5127:/ cond");
        /*cond*/
        if (/*_#IS_STRING__L10*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5128:/ apply");
                    /*apply*/
                    {
                        /*_.FILTERGCCVERSION__V28*/ meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!FILTERGCCVERSION*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.CURIF__V17*/ meltfptr[16]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                    }
                    ;
                    /*^cond*/
                    /*cond*/
                    if (/*_.FILTERGCCVERSION__V28*/ meltfptr[19]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:5129:/ quasiblock");



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
                                    /*_.EXPREST__V31*/
                                    meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!EXPAND_PAIRLIST_AS_TUPLE*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.RESTPAIR__V18*/ meltfptr[17]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-macro.melt:5131:/ cppif.then");
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
                                    meltfnum[6] =
                                        /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                        (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                        0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                        ;;
                                    MELT_LOCATION("warmelt-macro.melt:5131:/ cond");
                                    /*cond*/
                                    if (/*_#MELT_NEED_DBG__L11*/ meltfnum[6]) /*then*/
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
                                                MELT_LOCATION("warmelt-macro.melt:5131:/ apply");
                                                /*apply*/
                                                {
                                                    union meltparam_un argtab[5];
                                                    memset(&argtab, 0, sizeof(argtab));
                                                    /*^apply.arg*/
                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 5131;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_cstring =  "mexpand_gccif return exprest";
                                                    /*^apply.arg*/
                                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.EXPREST__V31*/ meltfptr[30];
                                                    /*_.MELT_DEBUG_FUN__V33*/
                                                    meltfptr[32] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IF___V32*/
                                                meltfptr[31] = /*_.MELT_DEBUG_FUN__V33*/ meltfptr[32];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-macro.melt:5131:/ clear");
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

                                            /*_._IF___V32*/ meltfptr[31] =  /*fromline 1341*/ NULL ;;
                                        }
                                    ;

                                    {
                                        MELT_LOCATION("warmelt-macro.melt:5131:/ locexp");
                                        /*void*/
                                        (void)0;
                                    }
                                    ;
                                    /*^quasiblock*/


                                    /*epilog*/

                                    /*^clear*/
                                    /*clear*/ /*_#MELT_NEED_DBG__L11*/
                                    meltfnum[6] = 0 ;
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
                                MELT_LOCATION("warmelt-macro.melt:5132:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] = /*_.EXPREST__V31*/ meltfptr[30];;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:5132:/ locexp");
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
                                meltfptr[21] = /*_.RETURN___V34*/ meltfptr[32];;

                                MELT_LOCATION("warmelt-macro.melt:5129:/ clear");
                                /*clear*/ /*_.EXPREST__V31*/
                                meltfptr[30] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.RETURN___V34*/
                                meltfptr[32] = 0 ;
                                /*_._IFELSE___V29*/
                                meltfptr[20] = /*_.LET___V30*/ meltfptr[21];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:5128:/ clear");
                                /*clear*/ /*_.LET___V30*/
                                meltfptr[21] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {


#if MELT_HAVE_DEBUG
                                MELT_LOCATION("warmelt-macro.melt:5134:/ cppif.then");
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
                                    MELT_LOCATION("warmelt-macro.melt:5134:/ cond");
                                    /*cond*/
                                    if (/*_#MELT_NEED_DBG__L13*/ meltfnum[11]) /*then*/
                                        {
                                            /*^cond.then*/
                                            /*^block*/
                                            /*anyblock*/
                                            {

                                                /*_#MELT_CALLCOUNT__L14*/ meltfnum[6] =
                                                    /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                                    meltcallcount  /* melt_callcount debugging */
#else
                                                    0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                                    ;;

                                                MELT_CHECK_SIGNAL();
                                                ;
                                                MELT_LOCATION("warmelt-macro.melt:5134:/ apply");
                                                /*apply*/
                                                {
                                                    union meltparam_un argtab[5];
                                                    memset(&argtab, 0, sizeof(argtab));
                                                    /*^apply.arg*/
                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L14*/ meltfnum[6];
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 5134;
                                                    /*^apply.arg*/
                                                    argtab[3].meltbp_cstring =  "mexpand_gccif sexpr gcc version mismatched";
                                                    /*^apply.arg*/
                                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                                                    /*_.MELT_DEBUG_FUN__V36*/
                                                    meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                }
                                                ;
                                                /*_._IF___V35*/
                                                meltfptr[31] = /*_.MELT_DEBUG_FUN__V36*/ meltfptr[30];;
                                                /*epilog*/

                                                MELT_LOCATION("warmelt-macro.melt:5134:/ clear");
                                                /*clear*/ /*_#MELT_CALLCOUNT__L14*/
                                                meltfnum[6] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_.MELT_DEBUG_FUN__V36*/
                                                meltfptr[30] = 0 ;
                                            }
                                            ;
                                        }
                                    else    /*^cond.else*/
                                        {

                                            /*_._IF___V35*/ meltfptr[31] =  /*fromline 1341*/ NULL ;;
                                        }
                                    ;

                                    {
                                        MELT_LOCATION("warmelt-macro.melt:5134:/ locexp");
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
                                    /*clear*/ /*_._IF___V35*/
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
                                MELT_LOCATION("warmelt-macro.melt:5135:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] =  /*fromline 1341*/ NULL ;;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:5135:/ locexp");
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
                                MELT_LOCATION("warmelt-macro.melt:5133:/ quasiblock");


                                /*_.PROGN___V38*/
                                meltfptr[21] = /*_.RETURN___V37*/ meltfptr[32];;
                                /*^compute*/
                                /*_._IFELSE___V29*/
                                meltfptr[20] = /*_.PROGN___V38*/ meltfptr[21];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:5128:/ clear");
                                /*clear*/ /*_.RETURN___V37*/
                                meltfptr[32] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V38*/
                                meltfptr[21] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V27*/
                    meltfptr[22] = /*_._IFELSE___V29*/ meltfptr[20];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5127:/ clear");
                    /*clear*/ /*_.FILTERGCCVERSION__V28*/
                    meltfptr[19] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V29*/
                    meltfptr[20] = 0 ;
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
                    /*_#IS_LIST__L15*/
                    meltfnum[6] =
                        (melt_magic_discr((melt_ptr_t)(/*_.CURIF__V17*/ meltfptr[16])) == MELTOBMAG_LIST);;
                    MELT_LOCATION("warmelt-macro.melt:5136:/ cond");
                    /*cond*/
                    if (/*_#IS_LIST__L15*/ meltfnum[6]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:5137:/ quasiblock");


                                /*_.OK__V41*/
                                meltfptr[32] = (/*nil*/NULL);;
                                /*citerblock FOREACH_IN_LIST*/
                                {
                                    /* start foreach_in_list meltcit1__EACHLIST */
                                    for (/*_.CURPAIR__V42*/ meltfptr[21] = melt_list_first( (melt_ptr_t)/*_.CURIF__V17*/ meltfptr[16]);
                                                            melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V42*/ meltfptr[21]) == MELTOBMAG_PAIR;
                                                            /*_.CURPAIR__V42*/ meltfptr[21] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V42*/ meltfptr[21]))
                                        {
                                            /*_.CURSTR__V43*/ meltfptr[19] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V42*/ meltfptr[21]);



                                            MELT_CHECK_SIGNAL();
                                            ;
                                            /*_#IS_STRING__L16*/
                                            meltfnum[11] =
                                                (melt_magic_discr((melt_ptr_t)(/*_.CURSTR__V43*/ meltfptr[19])) == MELTOBMAG_STRING);;
                                            /*^compute*/
                                            /*_#NOT__L17*/
                                            meltfnum[16] =
                                                (!(/*_#IS_STRING__L16*/ meltfnum[11]));;
                                            MELT_LOCATION("warmelt-macro.melt:5142:/ cond");
                                            /*cond*/
                                            if (/*_#NOT__L17*/ meltfnum[16]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        {
                                                            MELT_LOCATION("warmelt-macro.melt:5143:/ locexp");
                                                            /* error_plain */
                                                            melt_error_str((melt_ptr_t)(/*_.SLOC__V14*/ meltfptr[13]), ( "GCCIF condition not a list of strings"), (melt_ptr_t)0);
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
                                            MELT_LOCATION("warmelt-macro.melt:5144:/ apply");
                                            /*apply*/
                                            {
                                                /*_.FILTERGCCVERSION__V44*/ meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!FILTERGCCVERSION*/ meltfrout->tabval[3])), (melt_ptr_t)(/*_.CURSTR__V43*/ meltfptr[19]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
                                            }
                                            ;
                                            /*^cond*/
                                            /*cond*/
                                            if (/*_.FILTERGCCVERSION__V44*/ meltfptr[20]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {

                                                        MELT_LOCATION("warmelt-macro.melt:5145:/ compute");
                                                        /*_.OK__V41*/
                                                        meltfptr[32] = /*_.SETQ___V46*/ meltfptr[45] = (/*!konst_5_TRUE*/ meltfrout->tabval[5]);;
                                                        /*_._IF___V45*/
                                                        meltfptr[44] = /*_.SETQ___V46*/ meltfptr[45];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-macro.melt:5144:/ clear");
                                                        /*clear*/ /*_.SETQ___V46*/
                                                        meltfptr[45] = 0 ;
                                                    }
                                                    ;
                                                }
                                            else    /*^cond.else*/
                                                {

                                                    /*_._IF___V45*/ meltfptr[44] =  /*fromline 1341*/ NULL ;;
                                                }
                                            ;
                                        } /* end foreach_in_list meltcit1__EACHLIST */
                                    /*_.CURPAIR__V42*/ meltfptr[21] = NULL;
                                    /*_.CURSTR__V43*/
                                    meltfptr[19] = NULL;


                                    /*citerepilog*/

                                    MELT_LOCATION("warmelt-macro.melt:5139:/ clear");
                                    /*clear*/ /*_.CURPAIR__V42*/
                                    meltfptr[21] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.CURSTR__V43*/
                                    meltfptr[19] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#IS_STRING__L16*/
                                    meltfnum[11] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_#NOT__L17*/
                                    meltfnum[16] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_.FILTERGCCVERSION__V44*/
                                    meltfptr[20] = 0 ;
                                    /*^clear*/
                                    /*clear*/ /*_._IF___V45*/
                                    meltfptr[44] = 0 ;
                                } /*endciterblock FOREACH_IN_LIST*/
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-macro.melt:5147:/ cond");
                                /*cond*/
                                if (/*_.OK__V41*/ meltfptr[32]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-macro.melt:5148:/ quasiblock");



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
                                                /*_.EXPREST__V49*/
                                                meltfptr[48] =  melt_apply ((meltclosure_ptr_t)((/*!EXPAND_PAIRLIST_AS_TUPLE*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.RESTPAIR__V18*/ meltfptr[17]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;

#if MELT_HAVE_DEBUG
                                            MELT_LOCATION("warmelt-macro.melt:5150:/ cppif.then");
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
                                                meltfnum[17] =
                                                    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                                    (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                                    0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                                    ;;
                                                MELT_LOCATION("warmelt-macro.melt:5150:/ cond");
                                                /*cond*/
                                                if (/*_#MELT_NEED_DBG__L18*/ meltfnum[17]) /*then*/
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
                                                            MELT_LOCATION("warmelt-macro.melt:5150:/ apply");
                                                            /*apply*/
                                                            {
                                                                union meltparam_un argtab[5];
                                                                memset(&argtab, 0, sizeof(argtab));
                                                                /*^apply.arg*/
                                                                argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L19*/ meltfnum[18];
                                                                /*^apply.arg*/
                                                                argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                                                /*^apply.arg*/
                                                                argtab[2].meltbp_long = 5150;
                                                                /*^apply.arg*/
                                                                argtab[3].meltbp_cstring =  "mexpand_gccif return exprest multicond";
                                                                /*^apply.arg*/
                                                                argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.EXPREST__V49*/ meltfptr[48];
                                                                /*_.MELT_DEBUG_FUN__V51*/
                                                                meltfptr[50] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                            }
                                                            ;
                                                            /*_._IF___V50*/
                                                            meltfptr[49] = /*_.MELT_DEBUG_FUN__V51*/ meltfptr[50];;
                                                            /*epilog*/

                                                            MELT_LOCATION("warmelt-macro.melt:5150:/ clear");
                                                            /*clear*/ /*_#MELT_CALLCOUNT__L19*/
                                                            meltfnum[18] = 0 ;
                                                            /*^clear*/
                                                            /*clear*/ /*_.MELT_DEBUG_FUN__V51*/
                                                            meltfptr[50] = 0 ;
                                                        }
                                                        ;
                                                    }
                                                else    /*^cond.else*/
                                                    {

                                                        /*_._IF___V50*/ meltfptr[49] =  /*fromline 1341*/ NULL ;;
                                                    }
                                                ;

                                                {
                                                    MELT_LOCATION("warmelt-macro.melt:5150:/ locexp");
                                                    /*void*/
                                                    (void)0;
                                                }
                                                ;
                                                /*^quasiblock*/


                                                /*epilog*/

                                                /*^clear*/
                                                /*clear*/ /*_#MELT_NEED_DBG__L18*/
                                                meltfnum[17] = 0 ;
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
                                            MELT_LOCATION("warmelt-macro.melt:5151:/ quasiblock");


                                            /*_._RETVAL___V1*/
                                            meltfptr[0] = /*_.EXPREST__V49*/ meltfptr[48];;

                                            {
                                                MELT_LOCATION("warmelt-macro.melt:5151:/ locexp");
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
                                            /*_.LET___V48*/
                                            meltfptr[47] = /*_.RETURN___V52*/ meltfptr[50];;

                                            MELT_LOCATION("warmelt-macro.melt:5148:/ clear");
                                            /*clear*/ /*_.EXPREST__V49*/
                                            meltfptr[48] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.RETURN___V52*/
                                            meltfptr[50] = 0 ;
                                            /*_._IFELSE___V47*/
                                            meltfptr[45] = /*_.LET___V48*/ meltfptr[47];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-macro.melt:5147:/ clear");
                                            /*clear*/ /*_.LET___V48*/
                                            meltfptr[47] = 0 ;
                                        }
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*^block*/
                                        /*anyblock*/
                                        {


#if MELT_HAVE_DEBUG
                                            MELT_LOCATION("warmelt-macro.melt:5153:/ cppif.then");
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
                                                /*_#MELT_NEED_DBG__L20*/
                                                meltfnum[18] =
                                                    /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                                                    (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                                                    0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                                                    ;;
                                                MELT_LOCATION("warmelt-macro.melt:5153:/ cond");
                                                /*cond*/
                                                if (/*_#MELT_NEED_DBG__L20*/ meltfnum[18]) /*then*/
                                                    {
                                                        /*^cond.then*/
                                                        /*^block*/
                                                        /*anyblock*/
                                                        {

                                                            /*_#MELT_CALLCOUNT__L21*/ meltfnum[17] =
                                                                /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                                                                meltcallcount  /* melt_callcount debugging */
#else
                                                                0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                                                                ;;

                                                            MELT_CHECK_SIGNAL();
                                                            ;
                                                            MELT_LOCATION("warmelt-macro.melt:5153:/ apply");
                                                            /*apply*/
                                                            {
                                                                union meltparam_un argtab[5];
                                                                memset(&argtab, 0, sizeof(argtab));
                                                                /*^apply.arg*/
                                                                argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L21*/ meltfnum[17];
                                                                /*^apply.arg*/
                                                                argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                                                /*^apply.arg*/
                                                                argtab[2].meltbp_long = 5153;
                                                                /*^apply.arg*/
                                                                argtab[3].meltbp_cstring =  "mexpand_gccif sexpr gcc version multicond mismatched";
                                                                /*^apply.arg*/
                                                                argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                                                                /*_.MELT_DEBUG_FUN__V54*/
                                                                meltfptr[48] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                            }
                                                            ;
                                                            /*_._IF___V53*/
                                                            meltfptr[49] = /*_.MELT_DEBUG_FUN__V54*/ meltfptr[48];;
                                                            /*epilog*/

                                                            MELT_LOCATION("warmelt-macro.melt:5153:/ clear");
                                                            /*clear*/ /*_#MELT_CALLCOUNT__L21*/
                                                            meltfnum[17] = 0 ;
                                                            /*^clear*/
                                                            /*clear*/ /*_.MELT_DEBUG_FUN__V54*/
                                                            meltfptr[48] = 0 ;
                                                        }
                                                        ;
                                                    }
                                                else    /*^cond.else*/
                                                    {

                                                        /*_._IF___V53*/ meltfptr[49] =  /*fromline 1341*/ NULL ;;
                                                    }
                                                ;

                                                {
                                                    MELT_LOCATION("warmelt-macro.melt:5153:/ locexp");
                                                    /*void*/
                                                    (void)0;
                                                }
                                                ;
                                                /*^quasiblock*/


                                                /*epilog*/

                                                /*^clear*/
                                                /*clear*/ /*_#MELT_NEED_DBG__L20*/
                                                meltfnum[18] = 0 ;
                                                /*^clear*/
                                                /*clear*/ /*_._IF___V53*/
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
                                            MELT_LOCATION("warmelt-macro.melt:5154:/ quasiblock");


                                            /*_._RETVAL___V1*/
                                            meltfptr[0] =  /*fromline 1341*/ NULL ;;

                                            {
                                                MELT_LOCATION("warmelt-macro.melt:5154:/ locexp");
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
                                            MELT_LOCATION("warmelt-macro.melt:5152:/ quasiblock");


                                            /*_.PROGN___V56*/
                                            meltfptr[47] = /*_.RETURN___V55*/ meltfptr[50];;
                                            /*^compute*/
                                            /*_._IFELSE___V47*/
                                            meltfptr[45] = /*_.PROGN___V56*/ meltfptr[47];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-macro.melt:5147:/ clear");
                                            /*clear*/ /*_.RETURN___V55*/
                                            meltfptr[50] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_.PROGN___V56*/
                                            meltfptr[47] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_.LET___V40*/
                                meltfptr[31] = /*_._IFELSE___V47*/ meltfptr[45];;

                                MELT_LOCATION("warmelt-macro.melt:5137:/ clear");
                                /*clear*/ /*_.OK__V41*/
                                meltfptr[32] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V47*/
                                meltfptr[45] = 0 ;
                                /*_._IFELSE___V39*/
                                meltfptr[30] = /*_.LET___V40*/ meltfptr[31];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:5136:/ clear");
                                /*clear*/ /*_.LET___V40*/
                                meltfptr[31] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:5158:/ locexp");
                                    /* error_plain */
                                    melt_error_str((melt_ptr_t)(/*_.SLOC__V14*/ meltfptr[13]), ( "GCC-IF bad condition, should be a string or a list of strings"), (melt_ptr_t)0);
                                }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:5159:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] =  /*fromline 1341*/ NULL ;;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:5159:/ locexp");
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
                                MELT_LOCATION("warmelt-macro.melt:5157:/ quasiblock");


                                /*_.PROGN___V58*/
                                meltfptr[49] = /*_.RETURN___V57*/ meltfptr[48];;
                                /*^compute*/
                                /*_._IFELSE___V39*/
                                meltfptr[30] = /*_.PROGN___V58*/ meltfptr[49];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:5136:/ clear");
                                /*clear*/ /*_.RETURN___V57*/
                                meltfptr[48] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V58*/
                                meltfptr[49] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V27*/
                    meltfptr[22] = /*_._IFELSE___V39*/ meltfptr[30];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5127:/ clear");
                    /*clear*/ /*_#IS_LIST__L15*/
                    meltfnum[6] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V39*/
                    meltfptr[30] = 0 ;
                }
                ;
            }
        ;
        /*_.LET___V12*/
        meltfptr[10] = /*_._IFELSE___V27*/ meltfptr[22];;

        MELT_LOCATION("warmelt-macro.melt:5112:/ clear");
        /*clear*/ /*_.CONT__V13*/
        meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SLOC__V14*/
        meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_FIRST__V15*/
        meltfptr[14] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURPAIR__V16*/
        meltfptr[15] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CURIF__V17*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RESTPAIR__V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L5*/
        meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IF___V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_STRING__L10*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V27*/
        meltfptr[22] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5108:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V12*/ meltfptr[10];;

        {
            MELT_LOCATION("warmelt-macro.melt:5108:/ locexp");
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
        /*clear*/ /*_.IFCPP___V8*/
        meltfptr[6] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V12*/
        meltfptr[10] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("MEXPAND_GCCIF", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_86_WARMELTmiMACRO_MEXPAND_GCCIF*/



/**** end of warmelt-macro+03.cc ****/
