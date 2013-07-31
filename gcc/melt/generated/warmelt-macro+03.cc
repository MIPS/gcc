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



class Melt_InitialClassyFrame_WARMELTmiMACRO_h677217180; // forward declaration fromline 6518
typedef Melt_InitialClassyFrame_WARMELTmiMACRO_h677217180 Melt_InitialFrame;
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




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_1_WARMELTmiMACRO_MELT_MAKE_SEXPR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_2_WARMELTmiMACRO_S_EXPR_WEIGHT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_3_WARMELTmiMACRO_LAMBDA_cl1(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_4_WARMELTmiMACRO_EXPAND_RESTLIST_AS_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_5_WARMELTmiMACRO_EXPAND_RESTLIST_AS_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_6_WARMELTmiMACRO_EXPAND_PAIRLIST_AS_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_7_WARMELTmiMACRO_EXPAND_PAIRLIST_AS_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_8_WARMELTmiMACRO_REGISTER_GENERATOR_DEVICE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_9_WARMELTmiMACRO_EXPAND_APPLY(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_10_WARMELTmiMACRO_EXPAND_MSEND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_11_WARMELTmiMACRO_EXPAND_FIELDEXPR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_12_WARMELTmiMACRO_EXPAND_CITERATION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_13_WARMELTmiMACRO_EXPAND_CMATCHEXPR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_14_WARMELTmiMACRO_EXPAND_FUNMATCHEXPR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_15_WARMELTmiMACRO_EXPAND_KEYWORDFUN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_16_WARMELTmiMACRO_MACROEXPAND_1(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_17_WARMELTmiMACRO_LAMBDA_cl2(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_18_WARMELTmiMACRO_EXPAND_PRIMITIVE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_19_WARMELTmiMACRO_EXPAND_HOOK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_20_WARMELTmiMACRO_PATTERNEXPAND_PAIRLIST_AS_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_21_WARMELTmiMACRO_LAMBDA_cl3(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_22_WARMELTmiMACRO_PATMACEXPAND_FOR_MATCHER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_23_WARMELTmiMACRO_LAMBDA_cl4(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_24_WARMELTmiMACRO_LAMBDA_cl5(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_25_WARMELTmiMACRO_PATTERN_WEIGHT_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_26_WARMELTmiMACRO_LAMBDA_cl6(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_27_WARMELTmiMACRO_PATTERNEXPAND_EXPR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_28_WARMELTmiMACRO_PATTERNEXPAND_1(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_29_WARMELTmiMACRO_MACROEXPAND_TOPLEVEL_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_30_WARMELTmiMACRO_LAMBDA_ARG_BINDINGS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_31_WARMELTmiMACRO_INSTALL_INITIAL_MACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_32_WARMELTmiMACRO_INSTALL_INITIAL_PATMACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_33_WARMELTmiMACRO_WARN_IF_REDEFINED(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_34_WARMELTmiMACRO_FLATTEN_FOR_C_CODE_EXPANSION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_35_WARMELTmiMACRO_PARSE_PAIRLIST_C_CODE_EXPANSION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_36_WARMELTmiMACRO_CHECK_C_EXPANSION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_37_WARMELTmiMACRO_MELT_REGISTER_TRANSLATOR_RUNNER_MACROEXPANSIONS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_38_WARMELTmiMACRO_MELT_INVOKE_TRANSLATOR_RUNNER_MACROEXPANSIONS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_39_WARMELTmiMACRO_MEXPAND_DEFPRIMITIVE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_40_WARMELTmiMACRO_MEXPAND_DEFCITERATOR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_41_WARMELTmiMACRO_MEXPAND_DEFCMATCHER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_42_WARMELTmiMACRO_LAMBDA_cl7(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_43_WARMELTmiMACRO_MEXPAND_DEFUNMATCHER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_44_WARMELTmiMACRO_LAMBDA_cl8(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_45_WARMELTmiMACRO_MEXPAND_DEFUN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_46_WARMELTmiMACRO_MELT_DELAYED_MACRO_EXPANDER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_47_WARMELTmiMACRO_MEXPAND_DEFMACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_48_WARMELTmiMACRO_LAMBDA_cl9(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);

/** start of declaration for hook melthook_HOOK_MACRO_INSTALLER**/

/** declaration of hook melthook_HOOK_MACRO_INSTALLER */
MELT_EXTERN
void melthook_HOOK_MACRO_INSTALLER(melt_ptr_t melthookdatap,
                                   melt_ptr_t meltinp0_MBIND,
                                   melt_ptr_t meltinp1_MEXPCLOS)
;



/** end of declaration for hook melthook_HOOK_MACRO_INSTALLER**/






melt_ptr_t MELT_MODULE_VISIBILITY meltrout_50_WARMELTmiMACRO_MEXPAND_DEFVAR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_51_WARMELTmiMACRO_MEXPAND_DEFHOOK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_52_WARMELTmiMACRO_MEXPAND_DEFINE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_53_WARMELTmiMACRO_LAMBDA_cl10(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_54_WARMELTmiMACRO_SCAN_DEFCLASS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_55_WARMELTmiMACRO_MEXPAND_DEFCLASS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_56_WARMELTmiMACRO_LAMBDA_cl11(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_57_WARMELTmiMACRO_LAMBDA_cl12(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_58_WARMELTmiMACRO_PARSE_FIELD_ASSIGNMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_59_WARMELTmiMACRO_LAMBDA_cl13(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_60_WARMELTmiMACRO_MEXPAND_DEFINSTANCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_61_WARMELTmiMACRO_MEXPAND_DEFSELECTOR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_65_WARMELTmiMACRO_LAMBDA_cl14(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_67_WARMELTmiMACRO_LAMBDA_cl15(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_69_WARMELTmiMACRO_LAMBDA_cl16(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_72_WARMELTmiMACRO_LAMBDA_cl17(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_80_WARMELTmiMACRO_LAMBDA_cl18(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_82_WARMELTmiMACRO_MEXPAND_IF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_86_WARMELTmiMACRO_LAMBDA_cl19(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_88_WARMELTmiMACRO_FILTERGCCVERSION(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_89_WARMELTmiMACRO_MEXPAND_GCCIF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_90_WARMELTmiMACRO_MEXPAND_COND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_91_WARMELTmiMACRO_LAMBDA_cl20(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_92_WARMELTmiMACRO_MEXPAND_AND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_93_WARMELTmiMACRO_LAMBDA_cl21(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_94_WARMELTmiMACRO_PATEXPAND_AS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_95_WARMELTmiMACRO_MEXPAND_AS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_96_WARMELTmiMACRO_PATEXPAND_WHEN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_97_WARMELTmiMACRO_PATEXPAND_AND(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_98_WARMELTmiMACRO_MEXPAND_OR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_99_WARMELTmiMACRO_LAMBDA_cl22(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_100_WARMELTmiMACRO_PATEXPAND_OR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_101_WARMELTmiMACRO_MEXPAND_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_102_WARMELTmiMACRO_PATEXPAND_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_103_WARMELTmiMACRO_MEXPANDOBSOLETE_CONTAINER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_104_WARMELTmiMACRO_PATEXPANDOBSOLETE_CONTAINER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_105_WARMELTmiMACRO_MEXPAND_DEREF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_106_WARMELTmiMACRO_MEXPANDOBSOLETE_CONTENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_107_WARMELTmiMACRO_MEXPAND_SET_REF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_108_WARMELTmiMACRO_MEXPAND_PLUS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_109_WARMELTmiMACRO_MEXPAND_MINUS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_110_WARMELTmiMACRO_MEXPAND_TIMES(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_111_WARMELTmiMACRO_MEXPAND_DIV(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_112_WARMELTmiMACRO_MEXPAND_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_113_WARMELTmiMACRO_PATEXPAND_TUPLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_114_WARMELTmiMACRO_MEXPAND_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_115_WARMELTmiMACRO_PATEXPAND_LIST(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_116_WARMELTmiMACRO_MEXPAND_MATCH(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_117_WARMELTmiMACRO_LAMBDA_cl23(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_118_WARMELTmiMACRO_MEXPAND_MATCHALT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_119_WARMELTmiMACRO_LAMBDA_cl24(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_120_WARMELTmiMACRO_MEXPAND_LETBINDING(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_121_WARMELTmiMACRO_MEXPAND_LET(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_122_WARMELTmiMACRO_LAMBDA_cl25(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_123_WARMELTmiMACRO_LAMBDA_cl26(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_124_WARMELTmiMACRO_YES_RECURSIVELY_CONSTRUCTIBLE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_125_WARMELTmiMACRO_MEXPAND_LETREC(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_126_WARMELTmiMACRO_LAMBDA_cl27(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_127_WARMELTmiMACRO_LAMBDA_cl28(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_128_WARMELTmiMACRO_MEXPAND_LAMBDA(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_129_WARMELTmiMACRO_LAMBDA_cl29(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_130_WARMELTmiMACRO_MEXPAND_VARIADIC(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_131_WARMELTmiMACRO_LAMBDA_cl30(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_132_WARMELTmiMACRO_LAMBDA_cl31(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_133_WARMELTmiMACRO_LAMBDA_cl32(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_134_WARMELTmiMACRO_LAMBDA_cl33(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_135_WARMELTmiMACRO_MEXPAND_MULTICALL(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_136_WARMELTmiMACRO_LAMBDA_cl34(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_137_WARMELTmiMACRO_LAMBDA_cl35(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_138_WARMELTmiMACRO_EXPAND_QUOTED_SEXPR(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_139_WARMELTmiMACRO_MEXPAND_QUOTE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_140_WARMELTmiMACRO_ANTIQUOTER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_141_WARMELTmiMACRO_MEXPAND_BACKQUOTE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_142_WARMELTmiMACRO_MEXPAND_COMMA(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_143_WARMELTmiMACRO_MEXPAND_BOX(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_144_WARMELTmiMACRO_MEXPAND_CONSTANT_BOX(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_145_WARMELTmiMACRO_MEXPAND_UNBOX(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_146_WARMELTmiMACRO_MEXPAND_COMMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_147_WARMELTmiMACRO_MEXPAND_CHEADER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_148_WARMELTmiMACRO_MEXPAND_CIMPLEMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_149_WARMELTmiMACRO_MEXPAND_USE_PACKAGE_FROM_PKG_CONFIG(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_150_WARMELTmiMACRO_PAIRLIST_TO_RETURN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_151_WARMELTmiMACRO_LAMBDA_cl36(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_152_WARMELTmiMACRO_MEXPAND_PROGN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_153_WARMELTmiMACRO_MEXPAND_RETURN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_154_WARMELTmiMACRO_MEXPAND_FOREVER(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_155_WARMELTmiMACRO_LAMBDA_cl37(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_156_WARMELTmiMACRO_MEXPAND_EXIT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_157_WARMELTmiMACRO_LAMBDA_cl38(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_158_WARMELTmiMACRO_MEXPAND_AGAIN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_159_WARMELTmiMACRO_MEXPAND_COMPILE_WARNING(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_160_WARMELTmiMACRO_MEXPAND_ASSERT_MSG(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_161_WARMELTmiMACRO_MEXPAND_THIS_LINE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_162_WARMELTmiMACRO_MEXPAND_THIS_FILE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_163_WARMELTmiMACRO_MEXPAND_DEBUG(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_164_WARMELTmiMACRO_MEXPAND_EXPORT_VALUES(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_165_WARMELTmiMACRO_LAMBDA_cl39(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_166_WARMELTmiMACRO_MEXPAND_EXPORT_MACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_167_WARMELTmiMACRO_MEXPAND_EXPORT_PATMACRO(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_168_WARMELTmiMACRO_MEXPAND_EXPORT_CLASS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_169_WARMELTmiMACRO_LAMBDA_cl40(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_170_WARMELTmiMACRO_MEXPAND_EXPORT_SYNONYM(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_171_WARMELTmiMACRO_MEXPAND_CURRENT_MODULE_ENVIRONMENT_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_172_WARMELTmiMACRO_MEXPAND_PARENT_MODULE_ENVIRONMENT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_173_WARMELTmiMACRO_MEXPAND_UPDATE_CURRENT_MODULE_ENVIRONMENT_REFERENCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_174_WARMELTmiMACRO_MEXPAND_FETCH_PREDEFINED(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);




melt_ptr_t MELT_MODULE_VISIBILITY meltrout_175_WARMELTmiMACRO_MEXPAND_STORE_PREDEFINED(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_, const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_);


MELT_EXTERN void* melt_start_this_module (void*); /*mandatory start of module*/


/*declare opaque initial frame: */

typedef Melt_InitialClassyFrame_WARMELTmiMACRO_h677217180 /*opaqueinitialclassy*/ meltinitial_frame_t;


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


/**** warmelt-macro+03.cc implementations ****/




melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE fromline 1716 */

    /** start of frame for meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE// fromline 1531
        : public Melt_CallFrameWithValues<60>
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
        MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<60> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE), clos) {};
        MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE() //the constructor fromline 1606
            : Melt_CallFrameWithValues<60> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<60> (fil,lin, sizeof(MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE)) {};
        MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<60> (fil,lin, sizeof(MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE


    /** end of frame for meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE fromline 1661**/

    /* end of frame for routine meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE fromline 1720 */

    /* classy proc frame meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE */
    MeltFrame_meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("MEXPAND_INSTANCE", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:4252:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4253:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4253:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4253:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4253)?(4253):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4253:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4254:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4254:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[8] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4254:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4254)?(4254):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4254:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4255:/ quasiblock");


        /*_.FIELDS__V11*/
        meltfptr[10] = (/*nil*/NULL);;
        /*^compute*/
        /*_.FIELDNAMS__V12*/
        meltfptr[11] = (/*nil*/NULL);;
        MELT_LOCATION("warmelt-macro.melt:4257:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V13*/
            meltfptr[12] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4258:/ getslot");
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
            (melt_list_first((melt_ptr_t)(/*_.CONT__V13*/ meltfptr[12])));;
        /*^compute*/
        /*_.CURPAIR__V16*/
        meltfptr[15] =
            (melt_pair_tail((melt_ptr_t)(/*_.LIST_FIRST__V15*/ meltfptr[14])));;
        /*^compute*/
        /*_.CLANAME__V17*/
        meltfptr[16] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;
        /*^compute*/
        /*_.CLA__V18*/
        meltfptr[17] = (/*nil*/NULL);;
        /*^compute*/
        /*_.CLABIND__V19*/
        meltfptr[18] = (/*nil*/NULL);;

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
            /*_#MELT_NEED_DBG__L3*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:4264:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4264:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L4*/ meltfnum[3];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4264;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_instance start sexpr";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.SEXPR__V2*/ meltfptr[1];
                            /*_.MELT_DEBUG_FUN__V21*/
                            meltfptr[20] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V20*/
                        meltfptr[19] = /*_.MELT_DEBUG_FUN__V21*/ meltfptr[20];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4264:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L4*/
                        meltfnum[3] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V21*/
                        meltfptr[20] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V20*/ meltfptr[19] =  /*fromline 1341*/ NULL ;;
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
            /*clear*/ /*_#MELT_NEED_DBG__L3*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V20*/
            meltfptr[19] = 0 ;
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
        /*_#IS_NOT_A__L5*/
        meltfnum[3] =
            !melt_is_instance_of((melt_ptr_t)(/*_.CLANAME__V17*/ meltfptr[16]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[3])));;
        MELT_LOCATION("warmelt-macro.melt:4265:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L5*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4266:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V14*/ meltfptr[13]), ( "missing class symbol for INSTANCE"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:4267:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CLANAME__V17*/ meltfptr[16];
            /*_.BND__V23*/
            meltfptr[19] =  melt_apply ((meltclosure_ptr_t)((/*!FIND_ENV*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        /*_.FLDLIST__V24*/
        meltfptr[23] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[5]))));;

        MELT_CHECK_SIGNAL();
        ;
        /*^compute*/
        /*_#IS_A__L6*/
        meltfnum[0] =
            melt_is_instance_of((melt_ptr_t)(/*_.BND__V23*/ meltfptr[19]), (melt_ptr_t)((/*!CLASS_CLASS_BINDING*/ meltfrout->tabval[6])));;
        MELT_LOCATION("warmelt-macro.melt:4271:/ cond");
        /*cond*/
        if (/*_#IS_A__L6*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:4272:/ compute");
                    /*_.CLABIND__V19*/
                    meltfptr[18] = /*_.SETQ___V26*/ meltfptr[25] = /*_.BND__V23*/ meltfptr[19];;
                    MELT_LOCATION("warmelt-macro.melt:4273:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.BND__V23*/ meltfptr[19]) /*=obj*/;
                        melt_object_get_field(slot,obj, 3, "CBIND_CLASS");
                        /*_.CBIND_CLASS__V27*/
                        meltfptr[26] = slot;
                    };
                    ;
                    /*^compute*/
                    /*_.CLA__V18*/
                    meltfptr[17] = /*_.SETQ___V28*/ meltfptr[27] = /*_.CBIND_CLASS__V27*/ meltfptr[26];;
                    MELT_LOCATION("warmelt-macro.melt:4271:/ quasiblock");


                    /*_.PROGN___V29*/
                    meltfptr[28] = /*_.SETQ___V28*/ meltfptr[27];;
                    /*^compute*/
                    /*_._IFELSE___V25*/
                    meltfptr[24] = /*_.PROGN___V29*/ meltfptr[28];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4271:/ clear");
                    /*clear*/ /*_.SETQ___V26*/
                    meltfptr[25] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.CBIND_CLASS__V27*/
                    meltfptr[26] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.SETQ___V28*/
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


                    MELT_CHECK_SIGNAL();
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_A__L7*/
                    meltfnum[6] =
                        melt_is_instance_of((melt_ptr_t)(/*_.BND__V23*/ meltfptr[19]), (melt_ptr_t)((/*!CLASS_VALUE_BINDING*/ meltfrout->tabval[7])));;
                    MELT_LOCATION("warmelt-macro.melt:4275:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L7*/ meltfnum[6]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:4276:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.BND__V23*/ meltfptr[19]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 1, "VBIND_VALUE");
                                    /*_.VBIND_VALUE__V30*/
                                    meltfptr[25] = slot;
                                };
                                ;
                                /*_#IS_A__L9*/
                                meltfnum[8] =
                                    melt_is_instance_of((melt_ptr_t)(/*_.VBIND_VALUE__V30*/ meltfptr[25]), (melt_ptr_t)((/*!CLASS_CLASS*/ meltfrout->tabval[8])));;
                                /*^compute*/
                                /*_#_IF___L8*/
                                meltfnum[7] = /*_#IS_A__L9*/ meltfnum[8];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:4275:/ clear");
                                /*clear*/ /*_.VBIND_VALUE__V30*/
                                meltfptr[25] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_#IS_A__L9*/
                                meltfnum[8] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_#_IF___L8*/ meltfnum[7] = 0;;
                        }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4275:/ cond");
                    /*cond*/
                    if (/*_#_IF___L8*/ meltfnum[7]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:4277:/ compute");
                                /*_.CLABIND__V19*/
                                meltfptr[18] = /*_.SETQ___V32*/ meltfptr[27] = /*_.BND__V23*/ meltfptr[19];;
                                MELT_LOCATION("warmelt-macro.melt:4278:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.BND__V23*/ meltfptr[19]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 1, "VBIND_VALUE");
                                    /*_.VBIND_VALUE__V33*/
                                    meltfptr[28] = slot;
                                };
                                ;
                                /*^compute*/
                                /*_.CLA__V18*/
                                meltfptr[17] = /*_.SETQ___V34*/ meltfptr[25] = /*_.VBIND_VALUE__V33*/ meltfptr[28];;
                                MELT_LOCATION("warmelt-macro.melt:4275:/ quasiblock");


                                /*_.PROGN___V35*/
                                meltfptr[34] = /*_.SETQ___V34*/ meltfptr[25];;
                                /*^compute*/
                                /*_._IFELSE___V31*/
                                meltfptr[26] = /*_.PROGN___V35*/ meltfptr[34];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:4275:/ clear");
                                /*clear*/ /*_.SETQ___V32*/
                                meltfptr[27] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.VBIND_VALUE__V33*/
                                meltfptr[28] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.SETQ___V34*/
                                meltfptr[25] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V35*/
                                meltfptr[34] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:4282:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.CLANAME__V17*/ meltfptr[16]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                    /*_.NAMED_NAME__V36*/
                                    meltfptr[27] = slot;
                                };
                                ;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:4281:/ locexp");
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V14*/ meltfptr[13]), ( "invalid class name for INSTANCE"), (melt_ptr_t)(/*_.NAMED_NAME__V36*/ meltfptr[27]));
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-macro.melt:4283:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] = (/*nil*/NULL);;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:4283:/ locexp");
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
                                MELT_LOCATION("warmelt-macro.melt:4280:/ quasiblock");


                                /*_.PROGN___V38*/
                                meltfptr[25] = /*_.RETURN___V37*/ meltfptr[28];;
                                /*^compute*/
                                /*_._IFELSE___V31*/
                                meltfptr[26] = /*_.PROGN___V38*/ meltfptr[25];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:4275:/ clear");
                                /*clear*/ /*_.NAMED_NAME__V36*/
                                meltfptr[27] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.RETURN___V37*/
                                meltfptr[28] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V38*/
                                meltfptr[25] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V25*/
                    meltfptr[24] = /*_._IFELSE___V31*/ meltfptr[26];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4271:/ clear");
                    /*clear*/ /*_#IS_A__L7*/
                    meltfnum[6] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_#_IF___L8*/
                    meltfnum[7] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V31*/
                    meltfptr[26] = 0 ;
                }
                ;
            }
        ;

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
            /*_#MELT_NEED_DBG__L10*/
            meltfnum[8] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:4285:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L10*/ meltfnum[8]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L11*/ meltfnum[6] =
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
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[6];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4285;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_instance cla";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.CLA__V18*/ meltfptr[17];
                            /*_.MELT_DEBUG_FUN__V40*/
                            meltfptr[27] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V39*/
                        meltfptr[34] = /*_.MELT_DEBUG_FUN__V40*/ meltfptr[27];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:4285:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L11*/
                        meltfnum[6] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V40*/
                        meltfptr[27] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V39*/ meltfptr[34] =  /*fromline 1341*/ NULL ;;
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
            /*clear*/ /*_#MELT_NEED_DBG__L10*/
            meltfnum[8] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V39*/
            meltfptr[34] = 0 ;
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
            /*_#IS_A__L12*/
            meltfnum[7] =
                melt_is_instance_of((melt_ptr_t)(/*_.CLA__V18*/ meltfptr[17]), (melt_ptr_t)((/*!CLASS_CLASS*/ meltfrout->tabval[8])));;
            MELT_LOCATION("warmelt-macro.melt:4286:/ cond");
            /*cond*/
            if (/*_#IS_A__L12*/ meltfnum[7]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V42*/ meltfptr[25] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4286:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check cla"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4286)?(4286):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V42*/
                        meltfptr[25] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V41*/
            meltfptr[28] = /*_._IFELSE___V42*/ meltfptr[25];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4286:/ clear");
            /*clear*/ /*_#IS_A__L12*/
            meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V42*/
            meltfptr[25] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V41*/ meltfptr[28] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*^compute*/
        /*_.PAIR_TAIL__V43*/
        meltfptr[26] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;
        MELT_LOCATION("warmelt-macro.melt:4287:/ compute");
        /*_.CURPAIR__V16*/
        meltfptr[15] = /*_.SETQ___V44*/ meltfptr[27] = /*_.PAIR_TAIL__V43*/ meltfptr[26];;
        MELT_LOCATION("warmelt-macro.melt:4288:/ loop");
        /*loop*/
        {
meltlabloop_INSLOOP_3:;/*^loopbody*/

            /*^block*/
            /*anyblock*/
            {


                MELT_CHECK_SIGNAL();
                ;

                MELT_CHECK_SIGNAL();
                ;
                /*_#IS_PAIR__L13*/
                meltfnum[6] =
                    (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])) == MELTOBMAG_PAIR);;
                /*^compute*/
                /*_#NOT__L14*/
                meltfnum[8] =
                    (!(/*_#IS_PAIR__L13*/ meltfnum[6]));;
                MELT_LOCATION("warmelt-macro.melt:4289:/ cond");
                /*cond*/
                if (/*_#NOT__L14*/ meltfnum[8]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {

                            /*^quasiblock*/


                            /*^compute*/
                            /*_.INSLOOP__V46*/ meltfptr[25] =  /*fromline 1341*/ NULL ;;

                            /*^exit*/
                            /*exit*/
                            {
                                goto meltlabexit_INSLOOP_3;
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    } /*noelse*/
                ;
                MELT_LOCATION("warmelt-macro.melt:4290:/ quasiblock");


                /*_.CURFKW__V47*/
                meltfptr[46] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;

                MELT_CHECK_SIGNAL();
                ;
                /*^compute*/
                /*_#IS_NOT_A__L15*/
                meltfnum[7] =
                    !melt_is_instance_of((melt_ptr_t)(/*_.CURFKW__V47*/ meltfptr[46]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[9])));;
                MELT_LOCATION("warmelt-macro.melt:4291:/ cond");
                /*cond*/
                if (/*_#IS_NOT_A__L15*/ meltfnum[7]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:4292:/ locexp");
                                /* error_plain */
                                melt_error_str((melt_ptr_t)(/*_.LOC__V14*/ meltfptr[13]), ( "expecting keyword in INSTANCE"), (melt_ptr_t)0);
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    } /*noelse*/
                ;
                /*_.PAIR_TAIL__V48*/
                meltfptr[47] =
                    (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;
                MELT_LOCATION("warmelt-macro.melt:4293:/ compute");
                /*_.CURPAIR__V16*/
                meltfptr[15] = /*_.SETQ___V49*/ meltfptr[48] = /*_.PAIR_TAIL__V48*/ meltfptr[47];;
                MELT_LOCATION("warmelt-macro.melt:4294:/ quasiblock");


                /*_.CUREXP__V50*/
                meltfptr[49] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;
                MELT_LOCATION("warmelt-macro.melt:4295:/ quasiblock");



                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-macro.melt:4296:/ apply");
                /*apply*/
                {
                    union meltparam_un argtab[6];
                    memset(&argtab, 0, sizeof(argtab));
                    /*^apply.arg*/
                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.LOC__V14*/ meltfptr[13];
                    /*^apply.arg*/
                    argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.CURFKW__V47*/ meltfptr[46];
                    /*^apply.arg*/
                    argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.CUREXP__V50*/ meltfptr[49];
                    /*^apply.arg*/
                    argtab[3].meltbp_aptr = (melt_ptr_t*) &/*_.ENV__V3*/ meltfptr[2];
                    /*^apply.arg*/
                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
                    /*^apply.arg*/
                    argtab[5].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
                    /*_.FLDA__V51*/
                    meltfptr[50] =  melt_apply ((meltclosure_ptr_t)((/*!PARSE_FIELD_ASSIGNMENT*/ meltfrout->tabval[10])), (melt_ptr_t)(/*_.CLA__V18*/ meltfptr[17]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                }
                ;

#if MELT_HAVE_DEBUG
                MELT_LOCATION("warmelt-macro.melt:4297:/ cppif.then");
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
                    meltfnum[15] =
                        /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                        (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                        0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                        ;;
                    MELT_LOCATION("warmelt-macro.melt:4297:/ cond");
                    /*cond*/
                    if (/*_#MELT_NEED_DBG__L16*/ meltfnum[15]) /*then*/
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
                                MELT_LOCATION("warmelt-macro.melt:4297:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[5];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L17*/ meltfnum[16];
                                    /*^apply.arg*/
                                    argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                    /*^apply.arg*/
                                    argtab[2].meltbp_long = 4297;
                                    /*^apply.arg*/
                                    argtab[3].meltbp_cstring =  "mexpand_instance flda";
                                    /*^apply.arg*/
                                    argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.FLDA__V51*/ meltfptr[50];
                                    /*_.MELT_DEBUG_FUN__V53*/
                                    meltfptr[52] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[2])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;
                                /*_._IF___V52*/
                                meltfptr[51] = /*_.MELT_DEBUG_FUN__V53*/ meltfptr[52];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:4297:/ clear");
                                /*clear*/ /*_#MELT_CALLCOUNT__L17*/
                                meltfnum[16] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.MELT_DEBUG_FUN__V53*/
                                meltfptr[52] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IF___V52*/ meltfptr[51] =  /*fromline 1341*/ NULL ;;
                        }
                    ;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4297:/ locexp");
                        /*void*/
                        (void)0;
                    }
                    ;
                    /*^quasiblock*/


                    /*epilog*/

                    /*^clear*/
                    /*clear*/ /*_#MELT_NEED_DBG__L16*/
                    meltfnum[15] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IF___V52*/
                    meltfptr[51] = 0 ;
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
                MELT_LOCATION("warmelt-macro.melt:4298:/ cond");
                /*cond*/
                if (/*_.FLDA__V51*/ meltfptr[50]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:4299:/ locexp");
                                meltgc_append_list((melt_ptr_t)(/*_.FLDLIST__V24*/ meltfptr[23]), (melt_ptr_t)(/*_.FLDA__V51*/ meltfptr[50]));
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-macro.melt:4298:/ cond.else");

                        /*^block*/
                        /*anyblock*/
                        {

                            MELT_LOCATION("warmelt-macro.melt:4301:/ getslot");
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.CURFKW__V47*/ meltfptr[46]) /*=obj*/;
                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                /*_.NAMED_NAME__V54*/
                                meltfptr[52] = slot;
                            };
                            ;

                            {
                                MELT_LOCATION("warmelt-macro.melt:4300:/ locexp");
                                melt_error_str((melt_ptr_t)(/*_.LOC__V14*/ meltfptr[13]), ( "bad field name in INSTANCE"), (melt_ptr_t)(/*_.NAMED_NAME__V54*/ meltfptr[52]));
                            }
                            ;
                            /*epilog*/

                            MELT_LOCATION("warmelt-macro.melt:4298:/ clear");
                            /*clear*/ /*_.NAMED_NAME__V54*/
                            meltfptr[52] = 0 ;
                        }
                        ;
                    }
                ;

                MELT_LOCATION("warmelt-macro.melt:4295:/ clear");
                /*clear*/ /*_.FLDA__V51*/
                meltfptr[50] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:4294:/ clear");
                /*clear*/ /*_.CUREXP__V50*/
                meltfptr[49] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:4290:/ clear");
                /*clear*/ /*_.CURFKW__V47*/
                meltfptr[46] = 0 ;
                /*^clear*/
                /*clear*/ /*_#IS_NOT_A__L15*/
                meltfnum[7] = 0 ;
                /*^clear*/
                /*clear*/ /*_.PAIR_TAIL__V48*/
                meltfptr[47] = 0 ;
                /*^clear*/
                /*clear*/ /*_.SETQ___V49*/
                meltfptr[48] = 0 ;
                /*_.PAIR_TAIL__V55*/
                meltfptr[51] =
                    (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;
                MELT_LOCATION("warmelt-macro.melt:4303:/ compute");
                /*_.CURPAIR__V16*/
                meltfptr[15] = /*_.SETQ___V56*/ meltfptr[52] = /*_.PAIR_TAIL__V55*/ meltfptr[51];;

                MELT_CHECK_SIGNAL();
                ;
                /*epilog*/

                MELT_LOCATION("warmelt-macro.melt:4288:/ clear");
                /*clear*/ /*_#IS_PAIR__L13*/
                meltfnum[6] = 0 ;
                /*^clear*/
                /*clear*/ /*_#NOT__L14*/
                meltfnum[8] = 0 ;
                /*^clear*/
                /*clear*/ /*_.PAIR_TAIL__V55*/
                meltfptr[51] = 0 ;
                /*^clear*/
                /*clear*/ /*_.SETQ___V56*/
                meltfptr[52] = 0 ;
            }
            ;
            ;
            goto meltlabloop_INSLOOP_3;
meltlabexit_INSLOOP_3:;/*^loopepilog*/
            /*loopepilog*/
            /*_.FOREVER___V45*/
            meltfptr[34] = /*_.INSLOOP__V46*/ meltfptr[25];;
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:4305:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[12]);
            /*_.FASTUP__V58*/
            meltfptr[49] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[11])), (melt_ptr_t)(/*_.FLDLIST__V24*/ meltfptr[23]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4306:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_INSTANCE*/ meltfrout->tabval[13])), (5), "CLASS_SOURCE_INSTANCE");
            /*_.INST__V60*/
            meltfptr[47] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V60*/ meltfptr[47])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V60*/ meltfptr[47]), (1), (/*_.LOC__V14*/ meltfptr[13]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SMINS_CLASS", melt_magic_discr((melt_ptr_t)(/*_.INST__V60*/ meltfptr[47])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V60*/ meltfptr[47]), (2), (/*_.CLA__V18*/ meltfptr[17]), "SMINS_CLASS");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SMINS_CLABIND", melt_magic_discr((melt_ptr_t)(/*_.INST__V60*/ meltfptr[47])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V60*/ meltfptr[47]), (3), (/*_.CLABIND__V19*/ meltfptr[18]), "SMINS_CLABIND");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SMINS_FIELDS", melt_magic_discr((melt_ptr_t)(/*_.INST__V60*/ meltfptr[47])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V60*/ meltfptr[47]), (4), (/*_.FASTUP__V58*/ meltfptr[49]), "SMINS_FIELDS");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V60*/ meltfptr[47], "newly made instance");
        ;
        /*_.INST___V59*/
        meltfptr[46] = /*_.INST__V60*/ meltfptr[47];;
        /*^compute*/
        /*_.LET___V57*/
        meltfptr[50] = /*_.INST___V59*/ meltfptr[46];;

        MELT_LOCATION("warmelt-macro.melt:4305:/ clear");
        /*clear*/ /*_.FASTUP__V58*/
        meltfptr[49] = 0 ;
        /*^clear*/
        /*clear*/ /*_.INST___V59*/
        meltfptr[46] = 0 ;
        /*_.LET___V22*/
        meltfptr[20] = /*_.LET___V57*/ meltfptr[50];;

        MELT_LOCATION("warmelt-macro.melt:4267:/ clear");
        /*clear*/ /*_.BND__V23*/
        meltfptr[19] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FLDLIST__V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_A__L6*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.IFCPP___V41*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V43*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V44*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FOREVER___V45*/
        meltfptr[34] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V57*/
        meltfptr[50] = 0 ;
        /*_.LET___V10*/
        meltfptr[8] = /*_.LET___V22*/ meltfptr[20];;

        MELT_LOCATION("warmelt-macro.melt:4255:/ clear");
        /*clear*/ /*_.FIELDS__V11*/
        meltfptr[10] = 0 ;
        /*^clear*/
        /*clear*/ /*_.FIELDNAMS__V12*/
        meltfptr[11] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CONT__V13*/
        meltfptr[12] = 0 ;
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
        /*clear*/ /*_.CLANAME__V17*/
        meltfptr[16] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CLA__V18*/
        meltfptr[17] = 0 ;
        /*^clear*/
        /*clear*/ /*_.CLABIND__V19*/
        meltfptr[18] = 0 ;
        /*^clear*/
        /*clear*/ /*_#IS_NOT_A__L5*/
        meltfnum[3] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V22*/
        meltfptr[20] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4252:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V10*/ meltfptr[8];;

        {
            MELT_LOCATION("warmelt-macro.melt:4252:/ locexp");
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
    melt_trace_end("MEXPAND_INSTANCE", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_62_WARMELTmiMACRO_MEXPAND_INSTANCE*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD fromline 1716 */

    /** start of frame for meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD// fromline 1531
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
        MeltFrame_meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<42> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD), clos) {};
        MeltFrame_meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD() //the constructor fromline 1606
            : Melt_CallFrameWithValues<42> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<42> (fil,lin, sizeof(MeltFrame_meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD)) {};
        MeltFrame_meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<42> (fil,lin, sizeof(MeltFrame_meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD


    /** end of frame for meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD fromline 1661**/

    /* end of frame for routine meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD fromline 1720 */

    /* classy proc frame meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD */
    MeltFrame_meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:4320:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4321:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4321:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4321:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4321;
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

                        MELT_LOCATION("warmelt-macro.melt:4321:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4321:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4322:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4322:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4322:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4322)?(4322):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4322:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4323:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:4323:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[10] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4323:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4323)?(4323):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4323:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4324:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L5*/
            meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:4324:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V13*/ meltfptr[12] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4324:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check modctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4324)?(4324):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4324:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4325:/ cond");
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

                    MELT_LOCATION("warmelt-macro.melt:4325:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4326:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V17*/
            meltfptr[16] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4327:/ getslot");
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
        MELT_LOCATION("warmelt-macro.melt:4331:/ cond");
        /*cond*/
        if (/*_.PAIR_TAIL__V22*/ meltfptr[21]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4332:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4333:/ cond");
        /*cond*/
        if (/*_#IS_A__L7*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4334:/ apply");
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

                    MELT_LOCATION("warmelt-macro.melt:4333:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4335:/ cond");
        /*cond*/
        if (/*_#IS_A__L8*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:4336:/ getslot");
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

                    MELT_LOCATION("warmelt-macro.melt:4335:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4338:/ cond");
        /*cond*/
        if (/*_#IS_STRING__L9*/ meltfnum[8]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4339:/ locexp");
                        melt_inform_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "MELT loading file "), (melt_ptr_t)(/*_.FILNAM__V21*/ meltfptr[20]));
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4340:/ quasiblock");


                    /*_.CUREAD__V31*/
                    meltfptr[30] =
                        (meltgc_read_file (melt_string_str((melt_ptr_t)(/*_.FILNAM__V21*/ meltfptr[20])), (char*)0));;

#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:4341:/ cppif.then");
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
                        MELT_LOCATION("warmelt-macro.melt:4341:/ cond");
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
                                    MELT_LOCATION("warmelt-macro.melt:4341:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[10];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 4341;
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

                                    MELT_LOCATION("warmelt-macro.melt:4341:/ clear");
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
                            MELT_LOCATION("warmelt-macro.melt:4341:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:4342:/ quasiblock");



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
                    MELT_LOCATION("warmelt-macro.melt:4343:/ apply");
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
                    MELT_LOCATION("warmelt-macro.melt:4345:/ cppif.then");
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
                        MELT_LOCATION("warmelt-macro.melt:4345:/ cond");
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
                                    MELT_LOCATION("warmelt-macro.melt:4345:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L13*/ meltfnum[9];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 4345;
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

                                    MELT_LOCATION("warmelt-macro.melt:4345:/ clear");
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
                            MELT_LOCATION("warmelt-macro.melt:4345:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:4346:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = /*_.EXPTUPLE__V36*/ meltfptr[35];;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4346:/ locexp");
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

                    MELT_LOCATION("warmelt-macro.melt:4342:/ clear");
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

                    MELT_LOCATION("warmelt-macro.melt:4340:/ clear");
                    /*clear*/ /*_.CUREAD__V31*/
                    meltfptr[30] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.LET___V34*/
                    meltfptr[32] = 0 ;
                    MELT_LOCATION("warmelt-macro.melt:4338:/ quasiblock");


                    /*_.PROGN___V40*/
                    meltfptr[36] = /*_.LET___V30*/ meltfptr[27];;
                    /*^compute*/
                    /*_._IFELSE___V29*/
                    meltfptr[24] = /*_.PROGN___V40*/ meltfptr[36];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4338:/ clear");
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
                    MELT_LOCATION("warmelt-macro.melt:4351:/ cppif.then");
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
                        MELT_LOCATION("warmelt-macro.melt:4351:/ cond");
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
                                    MELT_LOCATION("warmelt-macro.melt:4351:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L15*/ meltfnum[10];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 4351;
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

                                    MELT_LOCATION("warmelt-macro.melt:4351:/ clear");
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
                            MELT_LOCATION("warmelt-macro.melt:4351:/ locexp");
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
                        MELT_LOCATION("warmelt-macro.melt:4352:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "LOAD expects a string filename"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4350:/ quasiblock");


                    /*epilog*/
                }
                ;
            }
        ;
        /*_.LET___V16*/
        meltfptr[14] = /*_._IFELSE___V29*/ meltfptr[24];;

        MELT_LOCATION("warmelt-macro.melt:4326:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4320:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-macro.melt:4320:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_63_WARMELTmiMACRO_MEXPAND_LOAD*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN fromline 1716 */

    /** start of frame for meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN// fromline 1531
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
        MeltFrame_meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<41> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN), clos) {};
        MeltFrame_meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN() //the constructor fromline 1606
            : Melt_CallFrameWithValues<41> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<41> (fil,lin, sizeof(MeltFrame_meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN)) {};
        MeltFrame_meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<41> (fil,lin, sizeof(MeltFrame_meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN


    /** end of frame for meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN fromline 1661**/

    /* end of frame for routine meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN fromline 1720 */

    /* classy proc frame meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN */
    MeltFrame_meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:4361:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4362:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4362:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4362:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4362;
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

                        MELT_LOCATION("warmelt-macro.melt:4362:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4362:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4363:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.FKEYW__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4363:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[7] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4363:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check fkeyw"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4363)?(4363):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4363:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4364:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V5*/ meltfptr[4]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:4364:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V13*/ meltfptr[12] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4364:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4364)?(4364):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4364:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4365:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L5*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.PCTX__V6*/ meltfptr[5]), (melt_ptr_t)((/*!CLASS_PATTERN_EXPANSION_CONTEXT*/ meltfrout->tabval[3])));;
            MELT_LOCATION("warmelt-macro.melt:4365:/ cond");
            /*cond*/
            if (/*_#IS_A__L5*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4365:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check pctc"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4365)?(4365):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4365:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4366:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L6*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.CLA__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_CLASS*/ meltfrout->tabval[4])));;
            MELT_LOCATION("warmelt-macro.melt:4366:/ cond");
            /*cond*/
            if (/*_#IS_A__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V17*/ meltfptr[16] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4366:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check cla"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4366)?(4366):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4366:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4367:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:4368:/ getslot");
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
        MELT_LOCATION("warmelt-macro.melt:4369:/ quasiblock");


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
        MELT_LOCATION("warmelt-macro.melt:4370:/ apply");
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
        MELT_LOCATION("warmelt-macro.melt:4372:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4372:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4372:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4372;
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

                        MELT_LOCATION("warmelt-macro.melt:4372:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4372:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4376:/ quasiblock");


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
        MELT_LOCATION("warmelt-macro.melt:4374:/ apply");
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
        MELT_LOCATION("warmelt-macro.melt:4386:/ quasiblock");


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
        MELT_LOCATION("warmelt-macro.melt:4388:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L9*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:4390:/ cppif.then");
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
                        MELT_LOCATION("warmelt-macro.melt:4390:/ cond");
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
                                    MELT_LOCATION("warmelt-macro.melt:4390:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[10];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 4390;
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

                                    MELT_LOCATION("warmelt-macro.melt:4390:/ clear");
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
                            MELT_LOCATION("warmelt-macro.melt:4390:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:4392:/ getslot");
                    {
                        melt_ptr_t slot=NULL, obj=NULL;
                        obj = (melt_ptr_t)(/*_.FKEYW__V2*/ meltfptr[1]) /*=obj*/;
                        melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                        /*_.NAMED_NAME__V33*/
                        meltfptr[31] = slot;
                    };
                    ;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4391:/ locexp");
                        melt_error_str((melt_ptr_t)(/*_.PSLOC__V7*/ meltfptr[6]), ( "invalid :field in pattern"), (melt_ptr_t)(/*_.NAMED_NAME__V33*/ meltfptr[31]));
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4393:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4393:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:4389:/ quasiblock");


                    /*_.PROGN___V35*/
                    meltfptr[34] = /*_.RETURN___V34*/ meltfptr[30];;
                    /*^compute*/
                    /*_._IF___V30*/
                    meltfptr[29] = /*_.PROGN___V35*/ meltfptr[34];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4388:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4394:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4395:/ quasiblock");


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
        MELT_LOCATION("warmelt-macro.melt:4401:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4401:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4401:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L13*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4401;
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

                        MELT_LOCATION("warmelt-macro.melt:4401:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4401:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4402:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.PATF__V37*/ meltfptr[30];;

        {
            MELT_LOCATION("warmelt-macro.melt:4402:/ locexp");
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

        MELT_LOCATION("warmelt-macro.melt:4394:/ clear");
        /*clear*/ /*_.PATF__V37*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V41*/
        meltfptr[39] = 0 ;
        /*_.LET___V28*/
        meltfptr[27] = /*_.LET___V36*/ meltfptr[31];;

        MELT_LOCATION("warmelt-macro.melt:4386:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:4367:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4361:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V18*/ meltfptr[16];;

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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_64_WARMELTmiMACRO_PARSE_FIELD_PATTERN*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_65_WARMELTmiMACRO_LAMBDA_cl14(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_65_WARMELTmiMACRO_LAMBDA_cl14_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_65_WARMELTmiMACRO_LAMBDA_cl14 fromline 1716 */

    /** start of frame for meltrout_65_WARMELTmiMACRO_LAMBDA_cl14 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_65_WARMELTmiMACRO_LAMBDA_cl14// fromline 1531
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
        MeltFrame_meltrout_65_WARMELTmiMACRO_LAMBDA_cl14(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<10> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_65_WARMELTmiMACRO_LAMBDA_cl14), clos) {};
        MeltFrame_meltrout_65_WARMELTmiMACRO_LAMBDA_cl14() //the constructor fromline 1606
            : Melt_CallFrameWithValues<10> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_65_WARMELTmiMACRO_LAMBDA_cl14)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_65_WARMELTmiMACRO_LAMBDA_cl14(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_65_WARMELTmiMACRO_LAMBDA_cl14)) {};
        MeltFrame_meltrout_65_WARMELTmiMACRO_LAMBDA_cl14(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<10> (fil,lin, sizeof(MeltFrame_meltrout_65_WARMELTmiMACRO_LAMBDA_cl14), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_65_WARMELTmiMACRO_LAMBDA_cl14


    /** end of frame for meltrout_65_WARMELTmiMACRO_LAMBDA_cl14 fromline 1661**/

    /* end of frame for routine meltrout_65_WARMELTmiMACRO_LAMBDA_cl14 fromline 1720 */

    /* classy proc frame meltrout_65_WARMELTmiMACRO_LAMBDA_cl14 */
    MeltFrame_meltrout_65_WARMELTmiMACRO_LAMBDA_cl14
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_65_WARMELTmiMACRO_LAMBDA_cl14 fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:4376:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4377:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.CFLD__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_FIELD*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4377:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V4*/ meltfptr[3] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4377:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check fld"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4377)?(4377):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4377:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4378:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.CFLD__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
            /*_.NAMED_NAME__V5*/
            meltfptr[3] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4379:/ getslot");
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
        MELT_LOCATION("warmelt-macro.melt:4378:/ cond");
        /*cond*/
        if (/*_#eqeqS__L3*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4381:/ cond");
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
                    MELT_LOCATION("warmelt-macro.melt:4382:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;
                    MELT_LOCATION("warmelt-macro.melt:4382:/ putxtraresult");
                    if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                    if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                    if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                    ;
                    /*^finalreturn*/
                    ;
                    /*finalret*/
                    goto meltlabend_rout ;
                    MELT_LOCATION("warmelt-macro.melt:4380:/ quasiblock");


                    /*_.PROGN___V9*/
                    meltfptr[8] = /*_.RETURN___V8*/ meltfptr[7];;
                    /*^compute*/
                    /*_._IFELSE___V7*/
                    meltfptr[6] = /*_.PROGN___V9*/ meltfptr[8];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4378:/ clear");
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
                    MELT_LOCATION("warmelt-macro.melt:4384:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = /*_.CFLD__V2*/ meltfptr[1];;
                    MELT_LOCATION("warmelt-macro.melt:4384:/ putxtraresult");
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

                    MELT_LOCATION("warmelt-macro.melt:4378:/ clear");
                    /*clear*/ /*_.RETURN___V10*/
                    meltfptr[7] = 0 ;
                }
                ;
            }
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4376:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_._IFELSE___V7*/ meltfptr[6];;

        {
            MELT_LOCATION("warmelt-macro.melt:4376:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_65_WARMELTmiMACRO_LAMBDA_cl14_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_65_WARMELTmiMACRO_LAMBDA_cl14*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE fromline 1716 */

    /** start of frame for meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE// fromline 1531
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
        MeltFrame_meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<67> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE), clos) {};
        MeltFrame_meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE() //the constructor fromline 1606
            : Melt_CallFrameWithValues<67> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<67> (fil,lin, sizeof(MeltFrame_meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE)) {};
        MeltFrame_meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<67> (fil,lin, sizeof(MeltFrame_meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE


    /** end of frame for meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE fromline 1661**/

    /* end of frame for routine meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE fromline 1720 */

    /* classy proc frame meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE */
    MeltFrame_meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:4405:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4406:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4406:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4406:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4406)?(4406):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4406:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4407:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4407:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[7] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4407:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4407)?(4407):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4407:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4408:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.PCTX__V4*/ meltfptr[3]), (melt_ptr_t)((/*!CLASS_PATTERN_EXPANSION_CONTEXT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:4408:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[9] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4408:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check pctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4408)?(4408):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4408:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4409:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4409:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4409:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L5*/ meltfnum[4];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4409;
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

                        MELT_LOCATION("warmelt-macro.melt:4409:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4409:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4410:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:4411:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V14*/
            meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4412:/ getslot");
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
        MELT_LOCATION("warmelt-macro.melt:4420:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L6*/ meltfnum[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4421:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V15*/ meltfptr[14]), ( "missing class symbol for INSTANCE pattern"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:4422:/ quasiblock");



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
        MELT_LOCATION("warmelt-macro.melt:4426:/ cond");
        /*cond*/
        if (/*_#IS_A__L7*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:4427:/ compute");
                    /*_.CLABIND__V22*/
                    meltfptr[21] = /*_.SETQ___V27*/ meltfptr[26] = /*_.BND__V24*/ meltfptr[23];;
                    MELT_LOCATION("warmelt-macro.melt:4428:/ getslot");
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
                    MELT_LOCATION("warmelt-macro.melt:4426:/ quasiblock");


                    /*_.PROGN___V30*/
                    meltfptr[29] = /*_.SETQ___V29*/ meltfptr[28];;
                    /*^compute*/
                    /*_._IFELSE___V26*/
                    meltfptr[25] = /*_.PROGN___V30*/ meltfptr[29];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4426:/ clear");
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
                    MELT_LOCATION("warmelt-macro.melt:4430:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L8*/ meltfnum[7]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:4431:/ getslot");
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

                                MELT_LOCATION("warmelt-macro.melt:4430:/ clear");
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
                    MELT_LOCATION("warmelt-macro.melt:4430:/ cond");
                    /*cond*/
                    if (/*_#_IF___L9*/ meltfnum[8]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:4432:/ compute");
                                /*_.CLABIND__V22*/
                                meltfptr[21] = /*_.SETQ___V33*/ meltfptr[28] = /*_.BND__V24*/ meltfptr[23];;
                                MELT_LOCATION("warmelt-macro.melt:4433:/ getslot");
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
                                MELT_LOCATION("warmelt-macro.melt:4430:/ quasiblock");


                                /*_.PROGN___V36*/
                                meltfptr[35] = /*_.SETQ___V35*/ meltfptr[26];;
                                /*^compute*/
                                /*_._IFELSE___V32*/
                                meltfptr[27] = /*_.PROGN___V36*/ meltfptr[35];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:4430:/ clear");
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

                                MELT_LOCATION("warmelt-macro.melt:4437:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.CLANAME__V20*/ meltfptr[19]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                    /*_.NAMED_NAME__V37*/
                                    meltfptr[28] = slot;
                                };
                                ;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:4436:/ locexp");
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V15*/ meltfptr[14]), ( "invalid class name for INSTANCE"), (melt_ptr_t)(/*_.NAMED_NAME__V37*/ meltfptr[28]));
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-macro.melt:4438:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] = (/*nil*/NULL);;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:4438:/ locexp");
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
                                MELT_LOCATION("warmelt-macro.melt:4435:/ quasiblock");


                                /*_.PROGN___V39*/
                                meltfptr[26] = /*_.RETURN___V38*/ meltfptr[29];;
                                /*^compute*/
                                /*_._IFELSE___V32*/
                                meltfptr[27] = /*_.PROGN___V39*/ meltfptr[26];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:4430:/ clear");
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

                    MELT_LOCATION("warmelt-macro.melt:4426:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4440:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L11*/
            meltfnum[9] =
                melt_is_instance_of((melt_ptr_t)(/*_.CLA__V21*/ meltfptr[20]), (melt_ptr_t)((/*!CLASS_CLASS*/ meltfrout->tabval[9])));;
            MELT_LOCATION("warmelt-macro.melt:4440:/ cond");
            /*cond*/
            if (/*_#IS_A__L11*/ meltfnum[9]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V41*/ meltfptr[28] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4440:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check cla"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4440)?(4440):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4440:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4441:/ compute");
        /*_.CURPAIR__V19*/
        meltfptr[18] = /*_.SETQ___V43*/ meltfptr[26] = /*_.PAIR_TAIL__V42*/ meltfptr[29];;
        MELT_LOCATION("warmelt-macro.melt:4443:/ loop");
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
                MELT_LOCATION("warmelt-macro.melt:4445:/ cond");
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
                MELT_LOCATION("warmelt-macro.melt:4446:/ quasiblock");


                /*_.CURFKW__V46*/
                meltfptr[45] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V19*/ meltfptr[18])));;

                MELT_CHECK_SIGNAL();
                ;
                /*^compute*/
                /*_#IS_NOT_A__L14*/
                meltfnum[9] =
                    !melt_is_instance_of((melt_ptr_t)(/*_.CURFKW__V46*/ meltfptr[45]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[10])));;
                MELT_LOCATION("warmelt-macro.melt:4447:/ cond");
                /*cond*/
                if (/*_#IS_NOT_A__L14*/ meltfnum[9]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:4448:/ locexp");
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
                MELT_LOCATION("warmelt-macro.melt:4449:/ compute");
                /*_.CURPAIR__V19*/
                meltfptr[18] = /*_.SETQ___V48*/ meltfptr[47] = /*_.PAIR_TAIL__V47*/ meltfptr[46];;
                MELT_LOCATION("warmelt-macro.melt:4450:/ quasiblock");


                /*_.CUREXP__V49*/
                meltfptr[48] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V19*/ meltfptr[18])));;
                MELT_LOCATION("warmelt-macro.melt:4451:/ quasiblock");



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
                MELT_LOCATION("warmelt-macro.melt:4452:/ cppif.then");
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
                    MELT_LOCATION("warmelt-macro.melt:4452:/ cond");
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
                                MELT_LOCATION("warmelt-macro.melt:4452:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[5];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L16*/ meltfnum[15];
                                    /*^apply.arg*/
                                    argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                    /*^apply.arg*/
                                    argtab[2].meltbp_long = 4452;
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

                                MELT_LOCATION("warmelt-macro.melt:4452:/ clear");
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
                        MELT_LOCATION("warmelt-macro.melt:4452:/ locexp");
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
                MELT_LOCATION("warmelt-macro.melt:4453:/ cond");
                /*cond*/
                if (/*_.FLDP__V50*/ meltfptr[49]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:4454:/ locexp");
                                meltgc_append_list((melt_ptr_t)(/*_.FLDLIST__V25*/ meltfptr[24]), (melt_ptr_t)(/*_.FLDP__V50*/ meltfptr[49]));
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-macro.melt:4453:/ cond.else");

                        /*^block*/
                        /*anyblock*/
                        {

                            MELT_LOCATION("warmelt-macro.melt:4456:/ getslot");
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.CURFKW__V46*/ meltfptr[45]) /*=obj*/;
                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                /*_.NAMED_NAME__V53*/
                                meltfptr[51] = slot;
                            };
                            ;

                            {
                                MELT_LOCATION("warmelt-macro.melt:4455:/ locexp");
                                melt_error_str((melt_ptr_t)(/*_.LOC__V15*/ meltfptr[14]), ( "bad field name in INSTANCE"), (melt_ptr_t)(/*_.NAMED_NAME__V53*/ meltfptr[51]));
                            }
                            ;
                            /*epilog*/

                            MELT_LOCATION("warmelt-macro.melt:4453:/ clear");
                            /*clear*/ /*_.NAMED_NAME__V53*/
                            meltfptr[51] = 0 ;
                        }
                        ;
                    }
                ;

                MELT_LOCATION("warmelt-macro.melt:4451:/ clear");
                /*clear*/ /*_.FLDP__V50*/
                meltfptr[49] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:4450:/ clear");
                /*clear*/ /*_.CUREXP__V49*/
                meltfptr[48] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:4446:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4458:/ compute");
                /*_.CURPAIR__V19*/
                meltfptr[18] = /*_.SETQ___V55*/ meltfptr[51] = /*_.PAIR_TAIL__V54*/ meltfptr[50];;

                MELT_CHECK_SIGNAL();
                ;
                /*epilog*/

                MELT_LOCATION("warmelt-macro.melt:4443:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4460:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4461:/ apply");
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
        MELT_LOCATION("warmelt-macro.melt:4463:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V59*/
        meltfptr[46] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_16*/ meltfrout->tabval[16])), (0));
        ;
        /*_.LAMBDA___V58*/
        meltfptr[45] = /*_.LAMBDA___V59*/ meltfptr[46];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4462:/ apply");
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
        MELT_LOCATION("warmelt-macro.melt:4465:/ quasiblock");


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
        MELT_LOCATION("warmelt-macro.melt:4471:/ quasiblock");


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
        MELT_LOCATION("warmelt-macro.melt:4474:/ quasiblock");


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


        MELT_LOCATION("warmelt-macro.melt:4471:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4479:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4479:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4479:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L22*/ meltfnum[7];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4479;
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

                        MELT_LOCATION("warmelt-macro.melt:4479:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4479:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4480:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.SPATI__V61*/ meltfptr[50];;

        {
            MELT_LOCATION("warmelt-macro.melt:4480:/ locexp");
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

        MELT_LOCATION("warmelt-macro.melt:4460:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:4422:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:4410:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4405:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V13*/ meltfptr[11];;

        {
            MELT_LOCATION("warmelt-macro.melt:4405:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_66_WARMELTmiMACRO_PATEXPAND_INSTANCE*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_67_WARMELTmiMACRO_LAMBDA_cl15(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_67_WARMELTmiMACRO_LAMBDA_cl15_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_67_WARMELTmiMACRO_LAMBDA_cl15 fromline 1716 */

    /** start of frame for meltrout_67_WARMELTmiMACRO_LAMBDA_cl15 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_67_WARMELTmiMACRO_LAMBDA_cl15// fromline 1531
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
        MeltFrame_meltrout_67_WARMELTmiMACRO_LAMBDA_cl15(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_67_WARMELTmiMACRO_LAMBDA_cl15), clos) {};
        MeltFrame_meltrout_67_WARMELTmiMACRO_LAMBDA_cl15() //the constructor fromline 1606
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_67_WARMELTmiMACRO_LAMBDA_cl15)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_67_WARMELTmiMACRO_LAMBDA_cl15(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_67_WARMELTmiMACRO_LAMBDA_cl15)) {};
        MeltFrame_meltrout_67_WARMELTmiMACRO_LAMBDA_cl15(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_67_WARMELTmiMACRO_LAMBDA_cl15), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_67_WARMELTmiMACRO_LAMBDA_cl15


    /** end of frame for meltrout_67_WARMELTmiMACRO_LAMBDA_cl15 fromline 1661**/

    /* end of frame for routine meltrout_67_WARMELTmiMACRO_LAMBDA_cl15 fromline 1720 */

    /* classy proc frame meltrout_67_WARMELTmiMACRO_LAMBDA_cl15 */
    MeltFrame_meltrout_67_WARMELTmiMACRO_LAMBDA_cl15
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_67_WARMELTmiMACRO_LAMBDA_cl15 fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:4463:/ getarg");
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

        MELT_LOCATION("warmelt-macro.melt:4464:/ cond");
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
        MELT_LOCATION("warmelt-macro.melt:4463:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.SPAF_PATTERN__V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-macro.melt:4463:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_67_WARMELTmiMACRO_LAMBDA_cl15_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_67_WARMELTmiMACRO_LAMBDA_cl15*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT fromline 1716 */

    /** start of frame for meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT// fromline 1531
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
        MeltFrame_meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<68> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT), clos) {};
        MeltFrame_meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT() //the constructor fromline 1606
            : Melt_CallFrameWithValues<68> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<68> (fil,lin, sizeof(MeltFrame_meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT)) {};
        MeltFrame_meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<68> (fil,lin, sizeof(MeltFrame_meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT


    /** end of frame for meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT fromline 1661**/

    /* end of frame for routine meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT fromline 1720 */

    /* classy proc frame meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT */
    MeltFrame_meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:4499:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4500:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4500:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V6*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4500:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4500)?(4500):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4500:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4501:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4501:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[7] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4501:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4501)?(4501):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4501:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4502:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.PCTX__V4*/ meltfptr[3]), (melt_ptr_t)((/*!CLASS_PATTERN_EXPANSION_CONTEXT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:4502:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[9] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4502:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check pctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4502)?(4502):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4502:/ clear");
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
            /*_#MELT_NEED_DBG__L4*/
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
                        MELT_LOCATION("warmelt-macro.melt:4503:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L5*/ meltfnum[4];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4503;
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

                        MELT_LOCATION("warmelt-macro.melt:4503:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4503:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4504:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:4505:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V14*/
            meltfptr[9] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4506:/ getslot");
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
        MELT_LOCATION("warmelt-macro.melt:4514:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L6*/ meltfnum[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4515:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V15*/ meltfptr[14]), ( "missing class symbol for  OBJECT pattern"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:4516:/ quasiblock");



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
        MELT_LOCATION("warmelt-macro.melt:4520:/ cond");
        /*cond*/
        if (/*_#IS_A__L7*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:4521:/ compute");
                    /*_.CLABIND__V22*/
                    meltfptr[21] = /*_.SETQ___V27*/ meltfptr[26] = /*_.BND__V24*/ meltfptr[23];;
                    MELT_LOCATION("warmelt-macro.melt:4522:/ getslot");
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
                    MELT_LOCATION("warmelt-macro.melt:4520:/ quasiblock");


                    /*_.PROGN___V30*/
                    meltfptr[29] = /*_.SETQ___V29*/ meltfptr[28];;
                    /*^compute*/
                    /*_._IFELSE___V26*/
                    meltfptr[25] = /*_.PROGN___V30*/ meltfptr[29];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4520:/ clear");
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
                    MELT_LOCATION("warmelt-macro.melt:4524:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L8*/ meltfnum[7]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:4525:/ getslot");
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

                                MELT_LOCATION("warmelt-macro.melt:4524:/ clear");
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
                    MELT_LOCATION("warmelt-macro.melt:4524:/ cond");
                    /*cond*/
                    if (/*_#_IF___L9*/ meltfnum[8]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:4526:/ compute");
                                /*_.CLABIND__V22*/
                                meltfptr[21] = /*_.SETQ___V33*/ meltfptr[28] = /*_.BND__V24*/ meltfptr[23];;
                                MELT_LOCATION("warmelt-macro.melt:4527:/ getslot");
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
                                MELT_LOCATION("warmelt-macro.melt:4524:/ quasiblock");


                                /*_.PROGN___V36*/
                                meltfptr[35] = /*_.SETQ___V35*/ meltfptr[26];;
                                /*^compute*/
                                /*_._IFELSE___V32*/
                                meltfptr[27] = /*_.PROGN___V36*/ meltfptr[35];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:4524:/ clear");
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

                                MELT_LOCATION("warmelt-macro.melt:4531:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.CLANAME__V20*/ meltfptr[19]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                    /*_.NAMED_NAME__V37*/
                                    meltfptr[28] = slot;
                                };
                                ;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:4530:/ locexp");
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V15*/ meltfptr[14]), ( "invalid class name for OBJECT pattern"), (melt_ptr_t)(/*_.NAMED_NAME__V37*/ meltfptr[28]));
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-macro.melt:4532:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] = (/*nil*/NULL);;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:4532:/ locexp");
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
                                MELT_LOCATION("warmelt-macro.melt:4529:/ quasiblock");


                                /*_.PROGN___V39*/
                                meltfptr[26] = /*_.RETURN___V38*/ meltfptr[29];;
                                /*^compute*/
                                /*_._IFELSE___V32*/
                                meltfptr[27] = /*_.PROGN___V39*/ meltfptr[26];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:4524:/ clear");
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

                    MELT_LOCATION("warmelt-macro.melt:4520:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4534:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L11*/
            meltfnum[9] =
                melt_is_instance_of((melt_ptr_t)(/*_.CLA__V21*/ meltfptr[20]), (melt_ptr_t)((/*!CLASS_CLASS*/ meltfrout->tabval[9])));;
            MELT_LOCATION("warmelt-macro.melt:4534:/ cond");
            /*cond*/
            if (/*_#IS_A__L11*/ meltfnum[9]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V41*/ meltfptr[28] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4534:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check cla"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4534)?(4534):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4534:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4535:/ compute");
        /*_.CURPAIR__V19*/
        meltfptr[18] = /*_.SETQ___V43*/ meltfptr[26] = /*_.PAIR_TAIL__V42*/ meltfptr[29];;
        MELT_LOCATION("warmelt-macro.melt:4537:/ loop");
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
                MELT_LOCATION("warmelt-macro.melt:4539:/ cond");
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
                MELT_LOCATION("warmelt-macro.melt:4540:/ quasiblock");


                /*_.CURFKW__V46*/
                meltfptr[45] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V19*/ meltfptr[18])));;

                MELT_CHECK_SIGNAL();
                ;
                /*^compute*/
                /*_#IS_NOT_A__L14*/
                meltfnum[9] =
                    !melt_is_instance_of((melt_ptr_t)(/*_.CURFKW__V46*/ meltfptr[45]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[10])));;
                MELT_LOCATION("warmelt-macro.melt:4541:/ cond");
                /*cond*/
                if (/*_#IS_NOT_A__L14*/ meltfnum[9]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:4542:/ locexp");
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
                MELT_LOCATION("warmelt-macro.melt:4543:/ compute");
                /*_.CURPAIR__V19*/
                meltfptr[18] = /*_.SETQ___V48*/ meltfptr[47] = /*_.PAIR_TAIL__V47*/ meltfptr[46];;
                MELT_LOCATION("warmelt-macro.melt:4544:/ quasiblock");


                /*_.CUREXP__V49*/
                meltfptr[48] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V19*/ meltfptr[18])));;
                MELT_LOCATION("warmelt-macro.melt:4545:/ quasiblock");



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
                MELT_LOCATION("warmelt-macro.melt:4546:/ cppif.then");
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
                    MELT_LOCATION("warmelt-macro.melt:4546:/ cond");
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
                                MELT_LOCATION("warmelt-macro.melt:4546:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[5];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L16*/ meltfnum[15];
                                    /*^apply.arg*/
                                    argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                    /*^apply.arg*/
                                    argtab[2].meltbp_long = 4546;
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

                                MELT_LOCATION("warmelt-macro.melt:4546:/ clear");
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
                        MELT_LOCATION("warmelt-macro.melt:4546:/ locexp");
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
                MELT_LOCATION("warmelt-macro.melt:4547:/ cond");
                /*cond*/
                if (/*_.FLDP__V50*/ meltfptr[49]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:4548:/ locexp");
                                meltgc_append_list((melt_ptr_t)(/*_.FLDLIST__V25*/ meltfptr[24]), (melt_ptr_t)(/*_.FLDP__V50*/ meltfptr[49]));
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    }
                else
                    {
                        MELT_LOCATION("warmelt-macro.melt:4547:/ cond.else");

                        /*^block*/
                        /*anyblock*/
                        {

                            MELT_LOCATION("warmelt-macro.melt:4550:/ getslot");
                            {
                                melt_ptr_t slot=NULL, obj=NULL;
                                obj = (melt_ptr_t)(/*_.CURFKW__V46*/ meltfptr[45]) /*=obj*/;
                                melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                /*_.NAMED_NAME__V53*/
                                meltfptr[51] = slot;
                            };
                            ;

                            {
                                MELT_LOCATION("warmelt-macro.melt:4549:/ locexp");
                                melt_error_str((melt_ptr_t)(/*_.LOC__V15*/ meltfptr[14]), ( "bad field name in OBJECT pattern"), (melt_ptr_t)(/*_.NAMED_NAME__V53*/ meltfptr[51]));
                            }
                            ;
                            /*epilog*/

                            MELT_LOCATION("warmelt-macro.melt:4547:/ clear");
                            /*clear*/ /*_.NAMED_NAME__V53*/
                            meltfptr[51] = 0 ;
                        }
                        ;
                    }
                ;

                MELT_LOCATION("warmelt-macro.melt:4545:/ clear");
                /*clear*/ /*_.FLDP__V50*/
                meltfptr[49] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:4544:/ clear");
                /*clear*/ /*_.CUREXP__V49*/
                meltfptr[48] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:4540:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4552:/ compute");
                /*_.CURPAIR__V19*/
                meltfptr[18] = /*_.SETQ___V55*/ meltfptr[51] = /*_.PAIR_TAIL__V54*/ meltfptr[50];;

                MELT_CHECK_SIGNAL();
                ;
                /*epilog*/

                MELT_LOCATION("warmelt-macro.melt:4537:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4554:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4555:/ apply");
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
        MELT_LOCATION("warmelt-macro.melt:4557:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V59*/
        meltfptr[46] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_16*/ meltfrout->tabval[16])), (0));
        ;
        /*_.LAMBDA___V58*/
        meltfptr[45] = /*_.LAMBDA___V59*/ meltfptr[46];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4556:/ apply");
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
        MELT_LOCATION("warmelt-macro.melt:4559:/ quasiblock");


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
        MELT_LOCATION("warmelt-macro.melt:4564:/ quasiblock");


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
        MELT_LOCATION("warmelt-macro.melt:4567:/ quasiblock");


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
        MELT_LOCATION("warmelt-macro.melt:4571:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4571:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4571:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L22*/ meltfnum[21];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4571;
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

                        MELT_LOCATION("warmelt-macro.melt:4571:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4571:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4572:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.SPATI__V61*/ meltfptr[50];;

        {
            MELT_LOCATION("warmelt-macro.melt:4572:/ locexp");
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

        MELT_LOCATION("warmelt-macro.melt:4564:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:4554:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:4516:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:4504:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4499:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V13*/ meltfptr[11];;

        {
            MELT_LOCATION("warmelt-macro.melt:4499:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_68_WARMELTmiMACRO_PATEXPAND_OBJECT*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_69_WARMELTmiMACRO_LAMBDA_cl16(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_69_WARMELTmiMACRO_LAMBDA_cl16_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_69_WARMELTmiMACRO_LAMBDA_cl16 fromline 1716 */

    /** start of frame for meltrout_69_WARMELTmiMACRO_LAMBDA_cl16 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_69_WARMELTmiMACRO_LAMBDA_cl16// fromline 1531
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
        MeltFrame_meltrout_69_WARMELTmiMACRO_LAMBDA_cl16(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_69_WARMELTmiMACRO_LAMBDA_cl16), clos) {};
        MeltFrame_meltrout_69_WARMELTmiMACRO_LAMBDA_cl16() //the constructor fromline 1606
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_69_WARMELTmiMACRO_LAMBDA_cl16)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_69_WARMELTmiMACRO_LAMBDA_cl16(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_69_WARMELTmiMACRO_LAMBDA_cl16)) {};
        MeltFrame_meltrout_69_WARMELTmiMACRO_LAMBDA_cl16(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_69_WARMELTmiMACRO_LAMBDA_cl16), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_69_WARMELTmiMACRO_LAMBDA_cl16


    /** end of frame for meltrout_69_WARMELTmiMACRO_LAMBDA_cl16 fromline 1661**/

    /* end of frame for routine meltrout_69_WARMELTmiMACRO_LAMBDA_cl16 fromline 1720 */

    /* classy proc frame meltrout_69_WARMELTmiMACRO_LAMBDA_cl16 */
    MeltFrame_meltrout_69_WARMELTmiMACRO_LAMBDA_cl16
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_69_WARMELTmiMACRO_LAMBDA_cl16 fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:4557:/ getarg");
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

        MELT_LOCATION("warmelt-macro.melt:4558:/ cond");
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
        MELT_LOCATION("warmelt-macro.melt:4557:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.SPAF_PATTERN__V3*/ meltfptr[2];;

        {
            MELT_LOCATION("warmelt-macro.melt:4557:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_69_WARMELTmiMACRO_LAMBDA_cl16_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_69_WARMELTmiMACRO_LAMBDA_cl16*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT fromline 1716 */

    /** start of frame for meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT// fromline 1531
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
        MeltFrame_meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<14> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT), clos) {};
        MeltFrame_meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT() //the constructor fromline 1606
            : Melt_CallFrameWithValues<14> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<14> (fil,lin, sizeof(MeltFrame_meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT)) {};
        MeltFrame_meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<14> (fil,lin, sizeof(MeltFrame_meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT


    /** end of frame for meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT fromline 1661**/

    /* end of frame for routine meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT fromline 1720 */

    /* classy proc frame meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT */
    MeltFrame_meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:4576:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4577:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4577:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
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

            MELT_LOCATION("warmelt-macro.melt:4577:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4578:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4578:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[8] = (/*nil*/NULL);;
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

            MELT_LOCATION("warmelt-macro.melt:4578:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4579:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4579:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4579:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L4*/ meltfnum[3];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4579;
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

                        MELT_LOCATION("warmelt-macro.melt:4579:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4579:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4580:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:4581:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
            /*_.LOC__V13*/
            meltfptr[8] = slot;
        };
        ;

        {
            MELT_LOCATION("warmelt-macro.melt:4583:/ locexp");
            /* error_plain */
            melt_error_str((melt_ptr_t)(/*_.LOC__V13*/ meltfptr[8]), ( "OBJECT cannot be macro-expanded in expression context - use INSTANCE\
 instead"), (melt_ptr_t)0);
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:4584:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] =  /*fromline 1341*/ NULL ;;

        {
            MELT_LOCATION("warmelt-macro.melt:4584:/ locexp");
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

        MELT_LOCATION("warmelt-macro.melt:4580:/ clear");
        /*clear*/ /*_.LOC__V13*/
        meltfptr[8] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V14*/
        meltfptr[13] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4576:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V12*/ meltfptr[10];;

        {
            MELT_LOCATION("warmelt-macro.melt:4576:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_70_WARMELTmiMACRO_MEXPAND_OBJECT*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS fromline 1716 */

    /** start of frame for meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS// fromline 1531
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
        MeltFrame_meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<28> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS), clos) {};
        MeltFrame_meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS() //the constructor fromline 1606
            : Melt_CallFrameWithValues<28> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<28> (fil,lin, sizeof(MeltFrame_meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS)) {};
        MeltFrame_meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<28> (fil,lin, sizeof(MeltFrame_meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS


    /** end of frame for meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS fromline 1661**/

    /* end of frame for routine meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS fromline 1720 */

    /* classy proc frame meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS */
    MeltFrame_meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:4601:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4602:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4602:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4602:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4602;
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

                        MELT_LOCATION("warmelt-macro.melt:4602:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4602:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4603:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_PAIR__L3*/
            meltfnum[1] =
                (melt_magic_discr((melt_ptr_t)(/*_.PAIRS__V2*/ meltfptr[1])) == MELTOBMAG_PAIR);;
            MELT_LOCATION("warmelt-macro.melt:4603:/ cond");
            /*cond*/
            if (/*_#IS_PAIR__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V10*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4603:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check pairs"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4603)?(4603):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4603:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4604:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V4*/ meltfptr[3]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4604:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[11] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4604:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4604)?(4604):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4604:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4605:/ quasiblock");


        /*_.CHKLIST__V14*/
        meltfptr[13] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[2]))));;
        MELT_LOCATION("warmelt-macro.melt:4607:/ blockmultialloc");
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



            MELT_LOCATION("warmelt-macro.melt:4609:/ putclosurout");
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
                        MELT_LOCATION("warmelt-macro.melt:4639:/ cppif.then");
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
                            MELT_LOCATION("warmelt-macro.melt:4639:/ cond");
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
                                        MELT_LOCATION("warmelt-macro.melt:4639:/ apply");
                                        /*apply*/
                                        {
                                            union meltparam_un argtab[5];
                                            memset(&argtab, 0, sizeof(argtab));
                                            /*^apply.arg*/
                                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L6*/ meltfnum[0];
                                            /*^apply.arg*/
                                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                            /*^apply.arg*/
                                            argtab[2].meltbp_long = 4639;
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

                                        MELT_LOCATION("warmelt-macro.melt:4639:/ clear");
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
                                MELT_LOCATION("warmelt-macro.melt:4639:/ locexp");
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
                        MELT_LOCATION("warmelt-macro.melt:4640:/ apply");
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

                MELT_LOCATION("warmelt-macro.melt:4636:/ clear");
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
            MELT_LOCATION("warmelt-macro.melt:4642:/ cppif.then");
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
                MELT_LOCATION("warmelt-macro.melt:4642:/ cond");
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
                            MELT_LOCATION("warmelt-macro.melt:4642:/ apply");
                            /*apply*/
                            {
                                union meltparam_un argtab[5];
                                memset(&argtab, 0, sizeof(argtab));
                                /*^apply.arg*/
                                argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[1];
                                /*^apply.arg*/
                                argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                /*^apply.arg*/
                                argtab[2].meltbp_long = 4642;
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

                            MELT_LOCATION("warmelt-macro.melt:4642:/ clear");
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
                    MELT_LOCATION("warmelt-macro.melt:4642:/ locexp");
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
            MELT_LOCATION("warmelt-macro.melt:4643:/ quasiblock");



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
            MELT_LOCATION("warmelt-macro.melt:4645:/ cppif.then");
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
                MELT_LOCATION("warmelt-macro.melt:4645:/ cond");
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
                            MELT_LOCATION("warmelt-macro.melt:4645:/ apply");
                            /*apply*/
                            {
                                union meltparam_un argtab[5];
                                memset(&argtab, 0, sizeof(argtab));
                                /*^apply.arg*/
                                argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L10*/ meltfnum[0];
                                /*^apply.arg*/
                                argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                /*^apply.arg*/
                                argtab[2].meltbp_long = 4645;
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

                            MELT_LOCATION("warmelt-macro.melt:4645:/ clear");
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
                    MELT_LOCATION("warmelt-macro.melt:4645:/ locexp");
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
            MELT_LOCATION("warmelt-macro.melt:4646:/ quasiblock");


            /*_._RETVAL___V1*/
            meltfptr[0] = /*_.TUP__V25*/ meltfptr[18];;

            {
                MELT_LOCATION("warmelt-macro.melt:4646:/ locexp");
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

            MELT_LOCATION("warmelt-macro.melt:4643:/ clear");
            /*clear*/ /*_.TUP__V25*/
            meltfptr[18] = 0 ;
            /*^clear*/
            /*clear*/ /*_.RETURN___V28*/
            meltfptr[26] = 0 ;
            /*_.LETREC___V15*/
            meltfptr[14] = /*_.LET___V24*/ meltfptr[22];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:4607:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:4605:/ clear");
        /*clear*/ /*_.CHKLIST__V14*/
        meltfptr[13] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LETREC___V15*/
        meltfptr[14] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4601:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V13*/ meltfptr[11];;

        {
            MELT_LOCATION("warmelt-macro.melt:4601:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_71_WARMELTmiMACRO_EXPAND_MACROSTRING_PAIRS*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_72_WARMELTmiMACRO_LAMBDA_cl17(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_72_WARMELTmiMACRO_LAMBDA_cl17_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_72_WARMELTmiMACRO_LAMBDA_cl17 fromline 1716 */

    /** start of frame for meltrout_72_WARMELTmiMACRO_LAMBDA_cl17 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_72_WARMELTmiMACRO_LAMBDA_cl17// fromline 1531
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
        MeltFrame_meltrout_72_WARMELTmiMACRO_LAMBDA_cl17(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<20> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_72_WARMELTmiMACRO_LAMBDA_cl17), clos) {};
        MeltFrame_meltrout_72_WARMELTmiMACRO_LAMBDA_cl17() //the constructor fromline 1606
            : Melt_CallFrameWithValues<20> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_72_WARMELTmiMACRO_LAMBDA_cl17)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_72_WARMELTmiMACRO_LAMBDA_cl17(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<20> (fil,lin, sizeof(MeltFrame_meltrout_72_WARMELTmiMACRO_LAMBDA_cl17)) {};
        MeltFrame_meltrout_72_WARMELTmiMACRO_LAMBDA_cl17(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<20> (fil,lin, sizeof(MeltFrame_meltrout_72_WARMELTmiMACRO_LAMBDA_cl17), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_72_WARMELTmiMACRO_LAMBDA_cl17


    /** end of frame for meltrout_72_WARMELTmiMACRO_LAMBDA_cl17 fromline 1661**/

    /* end of frame for routine meltrout_72_WARMELTmiMACRO_LAMBDA_cl17 fromline 1720 */

    /* classy proc frame meltrout_72_WARMELTmiMACRO_LAMBDA_cl17 */
    MeltFrame_meltrout_72_WARMELTmiMACRO_LAMBDA_cl17
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_72_WARMELTmiMACRO_LAMBDA_cl17 fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:4609:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4610:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4610:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4610:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4610;
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

                        MELT_LOCATION("warmelt-macro.melt:4610:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4610:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4612:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4613:/ locexp");
                        meltgc_append_list((melt_ptr_t)((/*~CHKLIST*/ meltfclos->tabval[0])), (melt_ptr_t)(/*_.MAKE_STRING__V6*/ meltfptr[4]));
                    }
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4612:/ clear");
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
                    MELT_LOCATION("warmelt-macro.melt:4614:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:4615:/ quasiblock");


                                MELT_LOCATION("warmelt-macro.melt:4616:/ cond");
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
                                MELT_LOCATION("warmelt-macro.melt:4617:/ cond");
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
                                MELT_LOCATION("warmelt-macro.melt:4619:/ cppif.then");
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
                                    MELT_LOCATION("warmelt-macro.melt:4619:/ cond");
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
                                                MELT_LOCATION("warmelt-macro.melt:4619:/ apply");
                                                /*apply*/
                                                {
                                                    union meltparam_un argtab[5];
                                                    memset(&argtab, 0, sizeof(argtab));
                                                    /*^apply.arg*/
                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L6*/ meltfnum[5];
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 4619;
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

                                                MELT_LOCATION("warmelt-macro.melt:4619:/ clear");
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
                                        MELT_LOCATION("warmelt-macro.melt:4619:/ locexp");
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
                                            MELT_LOCATION("warmelt-macro.melt:4623:/ apply");
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

                                    MELT_LOCATION("warmelt-macro.melt:4620:/ clear");
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
                                MELT_LOCATION("warmelt-macro.melt:4624:/ cppif.then");
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
                                    MELT_LOCATION("warmelt-macro.melt:4624:/ cond");
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
                                                MELT_LOCATION("warmelt-macro.melt:4624:/ apply");
                                                /*apply*/
                                                {
                                                    union meltparam_un argtab[5];
                                                    memset(&argtab, 0, sizeof(argtab));
                                                    /*^apply.arg*/
                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L8*/ meltfnum[4];
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 4624;
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

                                                MELT_LOCATION("warmelt-macro.melt:4624:/ clear");
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
                                        MELT_LOCATION("warmelt-macro.melt:4624:/ locexp");
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

                                MELT_LOCATION("warmelt-macro.melt:4615:/ clear");
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
                            MELT_LOCATION("warmelt-macro.melt:4614:/ cond.else");

                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                /*_#IS_A__L9*/
                                meltfnum[4] =
                                    melt_is_instance_of((melt_ptr_t)(/*_.COMP__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[4])));;
                                MELT_LOCATION("warmelt-macro.melt:4626:/ cond");
                                /*cond*/
                                if (/*_#IS_A__L9*/ meltfnum[4]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {

                                            MELT_LOCATION("warmelt-macro.melt:4627:/ quasiblock");



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
                                            MELT_LOCATION("warmelt-macro.melt:4629:/ cppif.then");
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
                                                MELT_LOCATION("warmelt-macro.melt:4629:/ cond");
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
                                                            MELT_LOCATION("warmelt-macro.melt:4629:/ apply");
                                                            /*apply*/
                                                            {
                                                                union meltparam_un argtab[5];
                                                                memset(&argtab, 0, sizeof(argtab));
                                                                /*^apply.arg*/
                                                                argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L11*/ meltfnum[10];
                                                                /*^apply.arg*/
                                                                argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                                                /*^apply.arg*/
                                                                argtab[2].meltbp_long = 4629;
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

                                                            MELT_LOCATION("warmelt-macro.melt:4629:/ clear");
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
                                                    MELT_LOCATION("warmelt-macro.melt:4629:/ locexp");
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
                                                MELT_LOCATION("warmelt-macro.melt:4630:/ locexp");
                                                meltgc_append_list((melt_ptr_t)((/*~CHKLIST*/ meltfclos->tabval[0])), (melt_ptr_t)(/*_.MCOMP__V16*/ meltfptr[14]));
                                            }
                                            ;

                                            MELT_LOCATION("warmelt-macro.melt:4627:/ clear");
                                            /*clear*/ /*_.MCOMP__V16*/
                                            meltfptr[14] = 0 ;
                                            /*epilog*/
                                        }
                                        ;
                                    }
                                else
                                    {
                                        MELT_LOCATION("warmelt-macro.melt:4626:/ cond.else");

                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            {
                                                MELT_LOCATION("warmelt-macro.melt:4632:/ locexp");
                                                meltgc_append_list((melt_ptr_t)((/*~CHKLIST*/ meltfclos->tabval[0])), (melt_ptr_t)(/*_.COMP__V2*/ meltfptr[1]));
                                            }
                                            ;
                                            MELT_LOCATION("warmelt-macro.melt:4631:/ quasiblock");


                                            /*epilog*/
                                        }
                                        ;
                                    }
                                ;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:4614:/ clear");
                                /*clear*/ /*_#IS_A__L9*/
                                meltfnum[4] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4612:/ clear");
                    /*clear*/ /*_#IS_A__L4*/
                    meltfnum[0] = 0 ;
                }
                ;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:4633:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4633:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4633:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[7];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L13*/ meltfnum[5];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4633;
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

                        MELT_LOCATION("warmelt-macro.melt:4633:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4633:/ locexp");
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

        MELT_LOCATION("warmelt-macro.melt:4609:/ clear");
        /*clear*/ /*_#IS_STRING__L3*/
        meltfnum[1] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl17", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_72_WARMELTmiMACRO_LAMBDA_cl17_melt;
    return (melt_ptr_t)(/*noretval*/ NULL);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_72_WARMELTmiMACRO_LAMBDA_cl17*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK fromline 1716 */

    /** start of frame for meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK// fromline 1531
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
        MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<32> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK), clos) {};
        MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK() //the constructor fromline 1606
            : Melt_CallFrameWithValues<32> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<32> (fil,lin, sizeof(MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK)) {};
        MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<32> (fil,lin, sizeof(MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK


    /** end of frame for meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK fromline 1661**/

    /* end of frame for routine meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK fromline 1720 */

    /* classy proc frame meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK */
    MeltFrame_meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:4651:/ getarg");
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
            /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:4652:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4652:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4652;
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

                        MELT_LOCATION("warmelt-macro.melt:4652:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4652:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4653:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4653:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4653:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4653)?(4653):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4653:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4654:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:4654:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[10] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4654:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4654)?(4654):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4654:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4655:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V13*/
            meltfptr[12] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4656:/ getslot");
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
        MELT_LOCATION("warmelt-macro.melt:4662:/ compute");
        /*_.CUREL__V19*/
        meltfptr[18] = /*_.SETQ___V21*/ meltfptr[20] = /*_.PAIR_HEAD__V20*/ meltfptr[19];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_A__L5*/
        meltfnum[1] =
            melt_is_instance_of((melt_ptr_t)(/*_.CUREL__V19*/ meltfptr[18]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[3])));;
        MELT_LOCATION("warmelt-macro.melt:4663:/ cond");
        /*cond*/
        if (/*_#IS_A__L5*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:4664:/ compute");
                    /*_.GSYM__V18*/
                    meltfptr[17] = /*_.SETQ___V23*/ meltfptr[22] = /*_.CUREL__V19*/ meltfptr[18];;
                    /*_._IFELSE___V22*/
                    meltfptr[21] = /*_.SETQ___V23*/ meltfptr[22];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4663:/ clear");
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
                        MELT_LOCATION("warmelt-macro.melt:4665:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4666:/ compute");
        /*_.CURPAIR__V16*/
        meltfptr[15] = /*_.SETQ___V25*/ meltfptr[24] = /*_.PAIR_TAIL__V24*/ meltfptr[22];;
        MELT_LOCATION("warmelt-macro.melt:4667:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4668:/ apply");
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
        MELT_LOCATION("warmelt-macro.melt:4669:/ quasiblock");


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
        MELT_LOCATION("warmelt-macro.melt:4675:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4675:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4675:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[6];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4675;
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

                        MELT_LOCATION("warmelt-macro.melt:4675:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4675:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4676:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.MRES__V28*/ meltfptr[27];;

        {
            MELT_LOCATION("warmelt-macro.melt:4676:/ locexp");
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

        MELT_LOCATION("warmelt-macro.melt:4667:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:4655:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4651:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V12*/ meltfptr[10];;

        {
            MELT_LOCATION("warmelt-macro.melt:4651:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_73_WARMELTmiMACRO_MEXPAND_CODE_CHUNK*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK fromline 1716 */

    /** start of frame for meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK// fromline 1531
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
        MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<58> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK), clos) {};
        MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK() //the constructor fromline 1606
            : Melt_CallFrameWithValues<58> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<58> (fil,lin, sizeof(MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK)) {};
        MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<58> (fil,lin, sizeof(MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK


    /** end of frame for meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK fromline 1661**/

    /* end of frame for routine meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK fromline 1720 */

    /* classy proc frame meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK */
    MeltFrame_meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:4690:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4691:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4691:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4691:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4691;
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

                        MELT_LOCATION("warmelt-macro.melt:4691:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4691:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4692:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4692:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4692:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4692)?(4692):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4692:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4693:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:4693:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[10] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4693:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4693)?(4693):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4693:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4694:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V13*/
            meltfptr[12] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4695:/ getslot");
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
        MELT_LOCATION("warmelt-macro.melt:4702:/ compute");
        /*_.CUREL__V20*/
        meltfptr[19] = /*_.SETQ___V22*/ meltfptr[21] = /*_.PAIR_HEAD__V21*/ meltfptr[20];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_A__L5*/
        meltfnum[1] =
            melt_is_instance_of((melt_ptr_t)(/*_.CUREL__V20*/ meltfptr[19]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[3])));;
        MELT_LOCATION("warmelt-macro.melt:4703:/ cond");
        /*cond*/
        if (/*_#IS_A__L5*/ meltfnum[1]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:4704:/ compute");
                    /*_.GSYM__V18*/
                    meltfptr[17] = /*_.SETQ___V24*/ meltfptr[23] = /*_.CUREL__V20*/ meltfptr[19];;
                    /*_._IFELSE___V23*/
                    meltfptr[22] = /*_.SETQ___V24*/ meltfptr[23];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4703:/ clear");
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
                        MELT_LOCATION("warmelt-macro.melt:4705:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4706:/ compute");
        /*_.CURPAIR__V16*/
        meltfptr[15] = /*_.SETQ___V26*/ meltfptr[25] = /*_.PAIR_TAIL__V25*/ meltfptr[23];;
        /*_.PAIR_HEAD__V27*/
        meltfptr[26] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V16*/ meltfptr[15])));;
        MELT_LOCATION("warmelt-macro.melt:4707:/ compute");
        /*_.KCTY__V19*/
        meltfptr[18] = /*_.SETQ___V28*/ meltfptr[27] = /*_.PAIR_HEAD__V27*/ meltfptr[26];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#IS_NOT_A__L6*/
        meltfnum[0] =
            !melt_is_instance_of((melt_ptr_t)(/*_.KCTY__V19*/ meltfptr[18]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[4])));;
        MELT_LOCATION("warmelt-macro.melt:4708:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L6*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4709:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.SLOC__V14*/ meltfptr[13]), ( "missing type keyword in (EXPR_CHUNK <state-symbol> <ctype> <chunk>...)"), (melt_ptr_t)0);
                    }
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4710:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] =  /*fromline 1341*/ NULL ;;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4710:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:4708:/ quasiblock");


                    /*_.PROGN___V31*/
                    meltfptr[30] = /*_.RETURN___V30*/ meltfptr[29];;
                    /*^compute*/
                    /*_._IF___V29*/
                    meltfptr[28] = /*_.PROGN___V31*/ meltfptr[30];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4708:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4711:/ compute");
        /*_.CURPAIR__V16*/
        meltfptr[15] = /*_.SETQ___V33*/ meltfptr[30] = /*_.PAIR_TAIL__V32*/ meltfptr[29];;
        MELT_LOCATION("warmelt-macro.melt:4712:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.KCTY__V19*/ meltfptr[18]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SYMB_DATA");
            /*_.CTY__V35*/
            meltfptr[34] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4713:/ getslot");
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
        MELT_LOCATION("warmelt-macro.melt:4715:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L7*/ meltfnum[6]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


#if MELT_HAVE_DEBUG
                    MELT_LOCATION("warmelt-macro.melt:4716:/ cppif.then");
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
                        MELT_LOCATION("warmelt-macro.melt:4716:/ cond");
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
                                    MELT_LOCATION("warmelt-macro.melt:4716:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L9*/ meltfnum[8];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 4716;
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

                                    MELT_LOCATION("warmelt-macro.melt:4716:/ clear");
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
                            MELT_LOCATION("warmelt-macro.melt:4716:/ locexp");
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
                        MELT_LOCATION("warmelt-macro.melt:4717:/ locexp");
                        melt_error_str((melt_ptr_t)(/*_.SLOC__V14*/ meltfptr[13]), ( "bad type keyword for EXPR_CHUNK"), (melt_ptr_t)(/*_.TYPKNAM__V36*/ meltfptr[35]));
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4718:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4718:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:4715:/ quasiblock");


                    /*_.PROGN___V41*/
                    meltfptr[37] = /*_.RETURN___V40*/ meltfptr[38];;
                    /*^compute*/
                    /*_._IF___V37*/
                    meltfptr[36] = /*_.PROGN___V41*/ meltfptr[37];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4715:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4720:/ getslot");
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
        MELT_LOCATION("warmelt-macro.melt:4720:/ cond");
        /*cond*/
        if (/*_#eqeq__L10*/ meltfnum[8]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V43*/ meltfptr[37] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-macro.melt:4720:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4723:/ getslot");
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
                    MELT_LOCATION("warmelt-macro.melt:4723:/ cond");
                    /*cond*/
                    if (/*_#eqeq__L11*/ meltfnum[7]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:4724:/ locexp");
                                    melt_warning_str(0, (melt_ptr_t)(/*_.SLOC__V14*/ meltfptr[13]), ( "using obsolete ctype keyword in EXPR_CHUNK"), (melt_ptr_t)(/*_.TYPKNAM__V36*/ meltfptr[35]));
                                }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:4725:/ cond");
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
                                MELT_LOCATION("warmelt-macro.melt:4725:/ cond");
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
                                    MELT_LOCATION("warmelt-macro.melt:4725:/ locexp");
                                    melt_inform_str((melt_ptr_t)(/*_.SLOC__V14*/ meltfptr[13]), ( "prefered ctype is"), (melt_ptr_t)(/*_.NAMED_NAME__V47*/ meltfptr[46]));
                                }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:4723:/ quasiblock");


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
                                MELT_LOCATION("warmelt-macro.melt:4728:/ cppif.then");
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
                                    MELT_LOCATION("warmelt-macro.melt:4728:/ cond");
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
                                                MELT_LOCATION("warmelt-macro.melt:4728:/ apply");
                                                /*apply*/
                                                {
                                                    union meltparam_un argtab[5];
                                                    memset(&argtab, 0, sizeof(argtab));
                                                    /*^apply.arg*/
                                                    argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L13*/ meltfnum[12];
                                                    /*^apply.arg*/
                                                    argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                                    /*^apply.arg*/
                                                    argtab[2].meltbp_long = 4728;
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

                                                MELT_LOCATION("warmelt-macro.melt:4728:/ clear");
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
                                        MELT_LOCATION("warmelt-macro.melt:4728:/ locexp");
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
                                    MELT_LOCATION("warmelt-macro.melt:4729:/ locexp");
                                    melt_error_str((melt_ptr_t)(/*_.SLOC__V14*/ meltfptr[13]), ( "invalid type keyword for EXPR_CHUNK"), (melt_ptr_t)(/*_.TYPKNAM__V36*/ meltfptr[35]));
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-macro.melt:4731:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] = (/*nil*/NULL);;

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
                                MELT_LOCATION("warmelt-macro.melt:4727:/ quasiblock");


                                /*_.PROGN___V51*/
                                meltfptr[45] = /*_.RETURN___V50*/ meltfptr[46];;
                                /*^compute*/
                                /*_._IFELSE___V45*/
                                meltfptr[44] = /*_.PROGN___V51*/ meltfptr[45];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:4723:/ clear");
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

                    MELT_LOCATION("warmelt-macro.melt:4720:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4733:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4734:/ apply");
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
        MELT_LOCATION("warmelt-macro.melt:4735:/ quasiblock");


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
        MELT_LOCATION("warmelt-macro.melt:4742:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4742:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4742:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L15*/ meltfnum[11];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4742;
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

                        MELT_LOCATION("warmelt-macro.melt:4742:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4742:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4743:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.MRES__V54*/ meltfptr[43];;

        {
            MELT_LOCATION("warmelt-macro.melt:4743:/ locexp");
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

        MELT_LOCATION("warmelt-macro.melt:4733:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:4712:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:4694:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4690:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V12*/ meltfptr[10];;

        {
            MELT_LOCATION("warmelt-macro.melt:4690:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_74_WARMELTmiMACRO_MEXPAND_EXPR_CHUNK*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS fromline 1716 */

    /** start of frame for meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS// fromline 1531
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
        MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<42> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS), clos) {};
        MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS() //the constructor fromline 1606
            : Melt_CallFrameWithValues<42> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<42> (fil,lin, sizeof(MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS)) {};
        MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<42> (fil,lin, sizeof(MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS


    /** end of frame for meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS fromline 1661**/

    /* end of frame for routine meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS fromline 1720 */

    /* classy proc frame meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS */
    MeltFrame_meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:4759:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4760:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4760:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4760:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4760)?(4760):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4760:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4761:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4761:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[8] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4761:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4761)?(4761):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4761:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4762:/ cond");
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

                    MELT_LOCATION("warmelt-macro.melt:4762:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4763:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_CLOSURE__L4*/
            meltfnum[3] =
                (melt_magic_discr((melt_ptr_t)(/*_.MEXPANDER__V4*/ meltfptr[3])) == MELTOBMAG_CLOSURE);;
            MELT_LOCATION("warmelt-macro.melt:4763:/ cond");
            /*cond*/
            if (/*_#IS_CLOSURE__L4*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V13*/ meltfptr[12] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4763:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check mexpander"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4763)?(4763):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4763:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4764:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L5*/
            meltfnum[3] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:4764:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L5*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4764:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check modctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4764)?(4764):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4764:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4765:/ quasiblock");


        /*_.FIELDS__V17*/
        meltfptr[16] = (/*nil*/NULL);;
        /*^compute*/
        /*_.FIELDNAMS__V18*/
        meltfptr[17] = (/*nil*/NULL);;
        MELT_LOCATION("warmelt-macro.melt:4767:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V19*/
            meltfptr[18] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4768:/ getslot");
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
        MELT_LOCATION("warmelt-macro.melt:4772:/ cond");
        /*cond*/
        if (/*_#IS_A__L6*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4773:/ apply");
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

                    MELT_LOCATION("warmelt-macro.melt:4772:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4776:/ compute");
        /*_.CURPAIR__V22*/
        meltfptr[21] = /*_.SETQ___V27*/ meltfptr[26] = /*_.PAIR_TAIL__V26*/ meltfptr[24];;
        MELT_LOCATION("warmelt-macro.melt:4777:/ quasiblock");


        /*_.FLDLIST__V29*/
        meltfptr[28] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[3]))));;
        MELT_LOCATION("warmelt-macro.melt:4779:/ loop");
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
                MELT_LOCATION("warmelt-macro.melt:4780:/ cond");
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
                MELT_LOCATION("warmelt-macro.melt:4781:/ quasiblock");


                /*_.CURFKW__V32*/
                meltfptr[31] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V22*/ meltfptr[21])));;

                MELT_CHECK_SIGNAL();
                ;
                /*^compute*/
                /*_#IS_NOT_A__L9*/
                meltfnum[8] =
                    !melt_is_instance_of((melt_ptr_t)(/*_.CURFKW__V32*/ meltfptr[31]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[4])));;
                MELT_LOCATION("warmelt-macro.melt:4782:/ cond");
                /*cond*/
                if (/*_#IS_NOT_A__L9*/ meltfnum[8]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:4783:/ locexp");
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
                MELT_LOCATION("warmelt-macro.melt:4784:/ compute");
                /*_.CURPAIR__V22*/
                meltfptr[21] = /*_.SETQ___V34*/ meltfptr[33] = /*_.PAIR_TAIL__V33*/ meltfptr[32];;
                MELT_LOCATION("warmelt-macro.melt:4785:/ quasiblock");


                /*_.CUREXP__V35*/
                meltfptr[34] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V22*/ meltfptr[21])));;
                MELT_LOCATION("warmelt-macro.melt:4786:/ quasiblock");



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
                MELT_LOCATION("warmelt-macro.melt:4787:/ cond");
                /*cond*/
                if (/*_.FLDA__V36*/ meltfptr[35]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:4788:/ locexp");
                                meltgc_append_list((melt_ptr_t)(/*_.FLDLIST__V29*/ meltfptr[28]), (melt_ptr_t)(/*_.FLDA__V36*/ meltfptr[35]));
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    } /*noelse*/
                ;

                MELT_LOCATION("warmelt-macro.melt:4786:/ clear");
                /*clear*/ /*_.FLDA__V36*/
                meltfptr[35] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:4785:/ clear");
                /*clear*/ /*_.CUREXP__V35*/
                meltfptr[34] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:4781:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4790:/ compute");
                /*_.CURPAIR__V22*/
                meltfptr[21] = /*_.SETQ___V38*/ meltfptr[34] = /*_.PAIR_TAIL__V37*/ meltfptr[35];;

                MELT_CHECK_SIGNAL();
                ;
                /*epilog*/

                MELT_LOCATION("warmelt-macro.melt:4779:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4792:/ quasiblock");



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
        MELT_LOCATION("warmelt-macro.melt:4793:/ quasiblock");


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

        MELT_LOCATION("warmelt-macro.melt:4792:/ clear");
        /*clear*/ /*_.FASTUP__V40*/
        meltfptr[32] = 0 ;
        /*^clear*/
        /*clear*/ /*_.INST___V41*/
        meltfptr[33] = 0 ;
        /*_.LET___V28*/
        meltfptr[27] = /*_.LET___V39*/ meltfptr[31];;

        MELT_LOCATION("warmelt-macro.melt:4777:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:4765:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4759:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-macro.melt:4759:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_75_WARMELTmiMACRO_MEXPAND_UNSAFE_PUT_FIELDS*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS fromline 1716 */

    /** start of frame for meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS// fromline 1531
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
        MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<38> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS), clos) {};
        MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS() //the constructor fromline 1606
            : Melt_CallFrameWithValues<38> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<38> (fil,lin, sizeof(MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS)) {};
        MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<38> (fil,lin, sizeof(MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS


    /** end of frame for meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS fromline 1661**/

    /* end of frame for routine meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS fromline 1720 */

    /* classy proc frame meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS */
    MeltFrame_meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:4807:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4808:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4808:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4808:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4808)?(4808):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4808:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4809:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4809:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[8] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4809:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4809)?(4809):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4809:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4810:/ cond");
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

                    MELT_LOCATION("warmelt-macro.melt:4810:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4811:/ quasiblock");


        /*_.FIELDS__V13*/
        meltfptr[12] = (/*nil*/NULL);;
        /*^compute*/
        /*_.FIELDNAMS__V14*/
        meltfptr[13] = (/*nil*/NULL);;
        MELT_LOCATION("warmelt-macro.melt:4813:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V15*/
            meltfptr[14] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4814:/ getslot");
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
        MELT_LOCATION("warmelt-macro.melt:4818:/ cond");
        /*cond*/
        if (/*_#IS_A__L4*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4819:/ apply");
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

                    MELT_LOCATION("warmelt-macro.melt:4818:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4822:/ compute");
        /*_.CURPAIR__V18*/
        meltfptr[17] = /*_.SETQ___V23*/ meltfptr[22] = /*_.PAIR_TAIL__V22*/ meltfptr[20];;
        MELT_LOCATION("warmelt-macro.melt:4823:/ quasiblock");


        /*_.FLDLIST__V25*/
        meltfptr[24] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[3]))));;
        MELT_LOCATION("warmelt-macro.melt:4825:/ loop");
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
                MELT_LOCATION("warmelt-macro.melt:4826:/ cond");
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
                MELT_LOCATION("warmelt-macro.melt:4827:/ quasiblock");


                /*_.CURFKW__V28*/
                meltfptr[27] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])));;

                MELT_CHECK_SIGNAL();
                ;
                /*^compute*/
                /*_#IS_NOT_A__L7*/
                meltfnum[6] =
                    !melt_is_instance_of((melt_ptr_t)(/*_.CURFKW__V28*/ meltfptr[27]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[4])));;
                MELT_LOCATION("warmelt-macro.melt:4828:/ cond");
                /*cond*/
                if (/*_#IS_NOT_A__L7*/ meltfnum[6]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:4829:/ locexp");
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
                MELT_LOCATION("warmelt-macro.melt:4830:/ compute");
                /*_.CURPAIR__V18*/
                meltfptr[17] = /*_.SETQ___V30*/ meltfptr[29] = /*_.PAIR_TAIL__V29*/ meltfptr[28];;
                MELT_LOCATION("warmelt-macro.melt:4831:/ quasiblock");


                /*_.CUREXP__V31*/
                meltfptr[30] =
                    (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])));;
                MELT_LOCATION("warmelt-macro.melt:4832:/ quasiblock");



                MELT_CHECK_SIGNAL();
                ;
                MELT_LOCATION("warmelt-macro.melt:4833:/ apply");
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
                MELT_LOCATION("warmelt-macro.melt:4834:/ cond");
                /*cond*/
                if (/*_.FLDA__V32*/ meltfptr[31]) /*then*/
                    {
                        /*^cond.then*/
                        /*^block*/
                        /*anyblock*/
                        {


                            {
                                MELT_LOCATION("warmelt-macro.melt:4835:/ locexp");
                                meltgc_append_list((melt_ptr_t)(/*_.FLDLIST__V25*/ meltfptr[24]), (melt_ptr_t)(/*_.FLDA__V32*/ meltfptr[31]));
                            }
                            ;
                            /*epilog*/
                        }
                        ;
                    } /*noelse*/
                ;

                MELT_LOCATION("warmelt-macro.melt:4832:/ clear");
                /*clear*/ /*_.FLDA__V32*/
                meltfptr[31] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:4831:/ clear");
                /*clear*/ /*_.CUREXP__V31*/
                meltfptr[30] = 0 ;

                MELT_LOCATION("warmelt-macro.melt:4827:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4837:/ compute");
                /*_.CURPAIR__V18*/
                meltfptr[17] = /*_.SETQ___V34*/ meltfptr[30] = /*_.PAIR_TAIL__V33*/ meltfptr[31];;

                MELT_CHECK_SIGNAL();
                ;
                /*epilog*/

                MELT_LOCATION("warmelt-macro.melt:4825:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4839:/ quasiblock");



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
        MELT_LOCATION("warmelt-macro.melt:4840:/ quasiblock");


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

        MELT_LOCATION("warmelt-macro.melt:4839:/ clear");
        /*clear*/ /*_.FASTUP__V36*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.INST___V37*/
        meltfptr[29] = 0 ;
        /*_.LET___V24*/
        meltfptr[23] = /*_.LET___V35*/ meltfptr[27];;

        MELT_LOCATION("warmelt-macro.melt:4823:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:4811:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4807:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V12*/ meltfptr[10];;

        {
            MELT_LOCATION("warmelt-macro.melt:4807:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_76_WARMELTmiMACRO_MEXPAND_PUT_FIELDS*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD fromline 1716 */

    /** start of frame for meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD// fromline 1531
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
        MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<34> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD), clos) {};
        MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD() //the constructor fromline 1606
            : Melt_CallFrameWithValues<34> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<34> (fil,lin, sizeof(MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD)) {};
        MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<34> (fil,lin, sizeof(MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD


    /** end of frame for meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD fromline 1661**/

    /* end of frame for routine meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD fromline 1720 */

    /* classy proc frame meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD */
    MeltFrame_meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:4854:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4855:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4855:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4855:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4855)?(4855):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4855:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4856:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4856:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[8] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4856:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4856)?(4856):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4856:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4857:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V11*/
            meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4858:/ getslot");
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
        MELT_LOCATION("warmelt-macro.melt:4862:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L3*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4864:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V12*/ meltfptr[11]), ( "field keyword expected in UNSAFE_GET_FIELD"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4865:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4865:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:4863:/ quasiblock");


                    /*_.PROGN___V18*/
                    meltfptr[17] = /*_.RETURN___V17*/ meltfptr[16];;
                    /*^compute*/
                    /*_._IF___V16*/
                    meltfptr[15] = /*_.PROGN___V18*/ meltfptr[17];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4862:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4866:/ compute");
        /*_.CURPAIR__V14*/
        meltfptr[13] = /*_.SETQ___V20*/ meltfptr[17] = /*_.PAIR_TAIL__V19*/ meltfptr[16];;
        MELT_LOCATION("warmelt-macro.melt:4867:/ quasiblock");


        /*_.CUREXP__V22*/
        meltfptr[21] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V14*/ meltfptr[13])));;
        /*^compute*/
        /*_.PAIR_TAIL__V23*/
        meltfptr[22] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V14*/ meltfptr[13])));;
        MELT_LOCATION("warmelt-macro.melt:4868:/ compute");
        /*_.CURPAIR__V14*/
        meltfptr[13] = /*_.SETQ___V24*/ meltfptr[23] = /*_.PAIR_TAIL__V23*/ meltfptr[22];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4869:/ cond");
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
        MELT_LOCATION("warmelt-macro.melt:4872:/ quasiblock");



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
        MELT_LOCATION("warmelt-macro.melt:4873:/ cond");
        /*cond*/
        if (/*_#NOT__L5*/ meltfnum[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4875:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V12*/ meltfptr[11]), ( "bad field and expression in UNSAFE_GET_FIELD"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4876:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4876:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:4874:/ quasiblock");


                    /*_.PROGN___V29*/
                    meltfptr[28] = /*_.RETURN___V28*/ meltfptr[27];;
                    /*^compute*/
                    /*_._IF___V27*/
                    meltfptr[26] = /*_.PROGN___V29*/ meltfptr[28];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4873:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4877:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.FLDA__V26*/ meltfptr[25]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SFLA_FIELD");
            /*_.FLD__V31*/
            meltfptr[28] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4878:/ getslot");
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
        MELT_LOCATION("warmelt-macro.melt:4879:/ cond");
        /*cond*/
        if (/*_#NULL__L6*/ meltfnum[5]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4880:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4881:/ quasiblock");


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

        MELT_LOCATION("warmelt-macro.melt:4877:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:4872:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:4867:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:4857:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4854:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V10*/ meltfptr[8];;

        {
            MELT_LOCATION("warmelt-macro.melt:4854:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_77_WARMELTmiMACRO_MEXPAND_UNSAFE_GET_FIELD*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD fromline 1716 */

    /** start of frame for meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD// fromline 1531
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
        MeltFrame_meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<34> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD), clos) {};
        MeltFrame_meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD() //the constructor fromline 1606
            : Melt_CallFrameWithValues<34> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<34> (fil,lin, sizeof(MeltFrame_meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD)) {};
        MeltFrame_meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<34> (fil,lin, sizeof(MeltFrame_meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD


    /** end of frame for meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD fromline 1661**/

    /* end of frame for routine meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD fromline 1720 */

    /* classy proc frame meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD */
    MeltFrame_meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:4895:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4896:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4896:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4896:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4896)?(4896):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4896:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4897:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4897:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[8] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4897:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4897)?(4897):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4897:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4898:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V11*/
            meltfptr[10] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4899:/ getslot");
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
        MELT_LOCATION("warmelt-macro.melt:4903:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L3*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4905:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V12*/ meltfptr[11]), ( "field keyword expected in GET_FIELD"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4906:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4906:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:4904:/ quasiblock");


                    /*_.PROGN___V18*/
                    meltfptr[17] = /*_.RETURN___V17*/ meltfptr[16];;
                    /*^compute*/
                    /*_._IF___V16*/
                    meltfptr[15] = /*_.PROGN___V18*/ meltfptr[17];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4903:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4907:/ compute");
        /*_.CURPAIR__V14*/
        meltfptr[13] = /*_.SETQ___V20*/ meltfptr[17] = /*_.PAIR_TAIL__V19*/ meltfptr[16];;
        MELT_LOCATION("warmelt-macro.melt:4908:/ quasiblock");


        /*_.CUREXP__V22*/
        meltfptr[21] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V14*/ meltfptr[13])));;
        /*^compute*/
        /*_.PAIR_TAIL__V23*/
        meltfptr[22] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V14*/ meltfptr[13])));;
        MELT_LOCATION("warmelt-macro.melt:4909:/ compute");
        /*_.CURPAIR__V14*/
        meltfptr[13] = /*_.SETQ___V24*/ meltfptr[23] = /*_.PAIR_TAIL__V23*/ meltfptr[22];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4910:/ cond");
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
        MELT_LOCATION("warmelt-macro.melt:4913:/ quasiblock");



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
        MELT_LOCATION("warmelt-macro.melt:4914:/ cond");
        /*cond*/
        if (/*_#NOT__L5*/ meltfnum[4]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4916:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V12*/ meltfptr[11]), ( "bad field and expression in GET_FIELD"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4917:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4917:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:4915:/ quasiblock");


                    /*_.PROGN___V29*/
                    meltfptr[28] = /*_.RETURN___V28*/ meltfptr[27];;
                    /*^compute*/
                    /*_._IF___V27*/
                    meltfptr[26] = /*_.PROGN___V29*/ meltfptr[28];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4914:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4918:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.FLDA__V26*/ meltfptr[25]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SFLA_FIELD");
            /*_.FLD__V31*/
            meltfptr[28] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4919:/ getslot");
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
        MELT_LOCATION("warmelt-macro.melt:4920:/ cond");
        /*cond*/
        if (/*_#NULL__L6*/ meltfnum[5]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4921:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4922:/ quasiblock");


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

        MELT_LOCATION("warmelt-macro.melt:4918:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:4913:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:4908:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:4898:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4895:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V10*/ meltfptr[8];;

        {
            MELT_LOCATION("warmelt-macro.melt:4895:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_78_WARMELTmiMACRO_MEXPAND_GET_FIELD*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN fromline 1716 */

    /** start of frame for meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN// fromline 1531
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
        MeltFrame_meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<23> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN), clos) {};
        MeltFrame_meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN() //the constructor fromline 1606
            : Melt_CallFrameWithValues<23> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<23> (fil,lin, sizeof(MeltFrame_meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN)) {};
        MeltFrame_meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<23> (fil,lin, sizeof(MeltFrame_meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN


    /** end of frame for meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN fromline 1661**/

    /* end of frame for routine meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN fromline 1720 */

    /* classy proc frame meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN */
    MeltFrame_meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:4936:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4937:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V4*/ meltfptr[3]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4937:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V8*/ meltfptr[7] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4937:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4937)?(4937):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4937:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4938:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4938:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4938:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L3*/ meltfnum[2];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4938;
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

                        MELT_LOCATION("warmelt-macro.melt:4938:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4938:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4939:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_PAIR__L4*/
            meltfnum[2] =
                (melt_magic_discr((melt_ptr_t)(/*_.PAIR__V2*/ meltfptr[1])) == MELTOBMAG_PAIR);;
            MELT_LOCATION("warmelt-macro.melt:4939:/ cond");
            /*cond*/
            if (/*_#IS_PAIR__L4*/ meltfnum[2]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V12*/ meltfptr[7] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4939:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check_pair"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4939)?(4939):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4939:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4940:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L5*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V6*/ meltfptr[5])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:4940:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L5*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V14*/ meltfptr[13] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4940:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check modctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4940)?(4940):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4940:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4941:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:4944:/ quasiblock");


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
        MELT_LOCATION("warmelt-macro.melt:4941:/ apply");
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
        MELT_LOCATION("warmelt-macro.melt:4946:/ quasiblock");


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
        MELT_LOCATION("warmelt-macro.melt:4950:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4950:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4950:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L7*/ meltfnum[0];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4950;
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

                        MELT_LOCATION("warmelt-macro.melt:4950:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4950:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4951:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.SPROGN__V19*/ meltfptr[18];;

        {
            MELT_LOCATION("warmelt-macro.melt:4951:/ locexp");
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

        MELT_LOCATION("warmelt-macro.melt:4941:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4936:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V15*/ meltfptr[13];;

        {
            MELT_LOCATION("warmelt-macro.melt:4936:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_79_WARMELTmiMACRO_PAIRLIST_TO_PROGN*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_80_WARMELTmiMACRO_LAMBDA_cl18(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_80_WARMELTmiMACRO_LAMBDA_cl18_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_80_WARMELTmiMACRO_LAMBDA_cl18 fromline 1716 */

    /** start of frame for meltrout_80_WARMELTmiMACRO_LAMBDA_cl18 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_80_WARMELTmiMACRO_LAMBDA_cl18// fromline 1531
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
        MeltFrame_meltrout_80_WARMELTmiMACRO_LAMBDA_cl18(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_80_WARMELTmiMACRO_LAMBDA_cl18), clos) {};
        MeltFrame_meltrout_80_WARMELTmiMACRO_LAMBDA_cl18() //the constructor fromline 1606
            : Melt_CallFrameWithValues<3> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_80_WARMELTmiMACRO_LAMBDA_cl18)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_80_WARMELTmiMACRO_LAMBDA_cl18(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_80_WARMELTmiMACRO_LAMBDA_cl18)) {};
        MeltFrame_meltrout_80_WARMELTmiMACRO_LAMBDA_cl18(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<3> (fil,lin, sizeof(MeltFrame_meltrout_80_WARMELTmiMACRO_LAMBDA_cl18), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_80_WARMELTmiMACRO_LAMBDA_cl18


    /** end of frame for meltrout_80_WARMELTmiMACRO_LAMBDA_cl18 fromline 1661**/

    /* end of frame for routine meltrout_80_WARMELTmiMACRO_LAMBDA_cl18 fromline 1720 */

    /* classy proc frame meltrout_80_WARMELTmiMACRO_LAMBDA_cl18 */
    MeltFrame_meltrout_80_WARMELTmiMACRO_LAMBDA_cl18
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_80_WARMELTmiMACRO_LAMBDA_cl18 fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:4944:/ getarg");
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
            MELT_LOCATION("warmelt-macro.melt:4944:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_80_WARMELTmiMACRO_LAMBDA_cl18_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_80_WARMELTmiMACRO_LAMBDA_cl18*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ fromline 1716 */

    /** start of frame for meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ// fromline 1531
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
        MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<31> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ), clos) {};
        MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ() //the constructor fromline 1606
            : Melt_CallFrameWithValues<31> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<31> (fil,lin, sizeof(MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ)) {};
        MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<31> (fil,lin, sizeof(MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ


    /** end of frame for meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ fromline 1661**/

    /* end of frame for routine meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ fromline 1720 */

    /* classy proc frame meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ */
    MeltFrame_meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:4955:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4956:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:4956:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4956:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4956)?(4956):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4956:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4957:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4957:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[8] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4957:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4957)?(4957):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4957:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4958:/ cond");
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

                    MELT_LOCATION("warmelt-macro.melt:4958:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4959:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L4*/
            meltfnum[3] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:4959:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L4*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V13*/ meltfptr[12] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4959:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check modctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4959)?(4959):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4959:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4960:/ quasiblock");


        /*^getslot*/
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V15*/
            meltfptr[14] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4961:/ getslot");
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
        MELT_LOCATION("warmelt-macro.melt:4965:/ cond");
        /*cond*/
        if (/*_#IS_NOT_A__L5*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4967:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V16*/ meltfptr[15]), ( "var symbol name expected in SETQ"), (melt_ptr_t)0);
                    }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:4968:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = (/*nil*/NULL);;

                    {
                        MELT_LOCATION("warmelt-macro.melt:4968:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:4966:/ quasiblock");


                    /*_.PROGN___V22*/
                    meltfptr[21] = /*_.RETURN___V21*/ meltfptr[20];;
                    /*^compute*/
                    /*_._IF___V20*/
                    meltfptr[19] = /*_.PROGN___V22*/ meltfptr[21];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:4965:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4969:/ compute");
        /*_.CURPAIR__V18*/
        meltfptr[17] = /*_.SETQ___V24*/ meltfptr[21] = /*_.PAIR_TAIL__V23*/ meltfptr[20];;
        MELT_LOCATION("warmelt-macro.melt:4970:/ quasiblock");


        /*_.CUREXP__V26*/
        meltfptr[25] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])));;
        /*^compute*/
        /*_.PAIR_TAIL__V27*/
        meltfptr[26] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V18*/ meltfptr[17])));;
        MELT_LOCATION("warmelt-macro.melt:4971:/ compute");
        /*_.CURPAIR__V18*/
        meltfptr[17] = /*_.SETQ___V28*/ meltfptr[27] = /*_.PAIR_TAIL__V27*/ meltfptr[26];;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:4972:/ cond");
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
        MELT_LOCATION("warmelt-macro.melt:4976:/ apply");
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
        MELT_LOCATION("warmelt-macro.melt:4973:/ quasiblock");


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

        MELT_LOCATION("warmelt-macro.melt:4970:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:4960:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4955:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V14*/ meltfptr[12];;

        {
            MELT_LOCATION("warmelt-macro.melt:4955:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_81_WARMELTmiMACRO_MEXPAND_SETQ*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_82_WARMELTmiMACRO_MEXPAND_IF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                      const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_82_WARMELTmiMACRO_MEXPAND_IF_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_82_WARMELTmiMACRO_MEXPAND_IF fromline 1716 */

    /** start of frame for meltrout_82_WARMELTmiMACRO_MEXPAND_IF of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_IF// fromline 1531
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
        MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_IF(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<55> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_IF), clos) {};
        MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_IF() //the constructor fromline 1606
            : Melt_CallFrameWithValues<55> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_IF)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_IF(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<55> (fil,lin, sizeof(MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_IF)) {};
        MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_IF(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<55> (fil,lin, sizeof(MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_IF), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_IF


    /** end of frame for meltrout_82_WARMELTmiMACRO_MEXPAND_IF fromline 1661**/

    /* end of frame for routine meltrout_82_WARMELTmiMACRO_MEXPAND_IF fromline 1720 */

    /* classy proc frame meltrout_82_WARMELTmiMACRO_MEXPAND_IF */
    MeltFrame_meltrout_82_WARMELTmiMACRO_MEXPAND_IF
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_82_WARMELTmiMACRO_MEXPAND_IF fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:4986:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:4987:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:4987:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:4987:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 4987;
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

                        MELT_LOCATION("warmelt-macro.melt:4987:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:4987:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4988:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:4988:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4988:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4988)?(4988):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4988:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4989:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:4989:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[10] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4989:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4989)?(4989):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4989:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4990:/ cond");
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

                    MELT_LOCATION("warmelt-macro.melt:4990:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4991:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L6*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:4991:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:4991:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check modctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (4991)?(4991):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:4991:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4992:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:4993:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V17*/
            meltfptr[16] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:4994:/ getslot");
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
        MELT_LOCATION("warmelt-macro.melt:4997:/ cond");
        /*cond*/
        if (/*_#NOT__L8*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:4998:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:4999:/ compute");
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
        MELT_LOCATION("warmelt-macro.melt:5000:/ cond");
        /*cond*/
        if (/*_#NOT__L10*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5001:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "missing then in IF"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5002:/ quasiblock");



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
        MELT_LOCATION("warmelt-macro.melt:5003:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:5003:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:5003:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5003;
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

                        MELT_LOCATION("warmelt-macro.melt:5003:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:5003:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:5004:/ quasiblock");


        /*_.CURTHEN__V29*/
        meltfptr[25] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
        /*^compute*/
        /*_.PAIR_TAIL__V30*/
        meltfptr[29] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
        MELT_LOCATION("warmelt-macro.melt:5005:/ compute");
        /*_.CURPAIR__V20*/
        meltfptr[19] = /*_.SETQ___V31*/ meltfptr[30] = /*_.PAIR_TAIL__V30*/ meltfptr[29];;
        MELT_LOCATION("warmelt-macro.melt:5006:/ quasiblock");



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
        MELT_LOCATION("warmelt-macro.melt:5008:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:5008:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:5008:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L14*/ meltfnum[10];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5008;
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

                        MELT_LOCATION("warmelt-macro.melt:5008:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:5008:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:5009:/ cond");
        /*cond*/
        if (/*_#IS_PAIR__L15*/ meltfnum[10]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:5010:/ quasiblock");


                    /*_.CURELSE__V38*/
                    meltfptr[37] =
                        (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5011:/ apply");
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
                    MELT_LOCATION("warmelt-macro.melt:5012:/ cppif.then");
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
                        MELT_LOCATION("warmelt-macro.melt:5012:/ cond");
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
                                    MELT_LOCATION("warmelt-macro.melt:5012:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L17*/ meltfnum[16];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 5012;
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

                                    MELT_LOCATION("warmelt-macro.melt:5012:/ clear");
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
                            MELT_LOCATION("warmelt-macro.melt:5012:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:5013:/ compute");
                    /*_.CURPAIR__V20*/
                    meltfptr[19] = /*_.SETQ___V43*/ meltfptr[39] = /*_.PAIR_TAIL__V42*/ meltfptr[40];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_PAIR__L18*/
                    meltfnum[16] =
                        (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])) == MELTOBMAG_PAIR);;
                    MELT_LOCATION("warmelt-macro.melt:5014:/ cond");
                    /*cond*/
                    if (/*_#IS_PAIR__L18*/ meltfnum[16]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:5015:/ locexp");
                                    /* error_plain */
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "IF with more than three sons"), (melt_ptr_t)0);
                                }
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5016:/ quasiblock");



                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5017:/ quasiblock");


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
                    MELT_LOCATION("warmelt-macro.melt:5023:/ cppif.then");
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
                        MELT_LOCATION("warmelt-macro.melt:5023:/ cond");
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
                                    MELT_LOCATION("warmelt-macro.melt:5023:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L20*/ meltfnum[19];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 5023;
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

                                    MELT_LOCATION("warmelt-macro.melt:5023:/ clear");
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
                            MELT_LOCATION("warmelt-macro.melt:5023:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:5024:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = /*_.RESE__V45*/ meltfptr[44];;

                    {
                        MELT_LOCATION("warmelt-macro.melt:5024:/ locexp");
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

                    MELT_LOCATION("warmelt-macro.melt:5016:/ clear");
                    /*clear*/ /*_.RESE__V45*/
                    meltfptr[44] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V49*/
                    meltfptr[47] = 0 ;
                    /*_.LET___V37*/
                    meltfptr[33] = /*_.LET___V44*/ meltfptr[43];;

                    MELT_LOCATION("warmelt-macro.melt:5010:/ clear");
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

                    MELT_LOCATION("warmelt-macro.melt:5009:/ clear");
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

                    MELT_LOCATION("warmelt-macro.melt:5025:/ quasiblock");



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
                    MELT_LOCATION("warmelt-macro.melt:5029:/ cppif.then");
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
                        MELT_LOCATION("warmelt-macro.melt:5029:/ cond");
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
                                    MELT_LOCATION("warmelt-macro.melt:5029:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L22*/ meltfnum[11];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 5029;
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

                                    MELT_LOCATION("warmelt-macro.melt:5029:/ clear");
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
                            MELT_LOCATION("warmelt-macro.melt:5029:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:5030:/ quasiblock");


                    /*_._RETVAL___V1*/
                    meltfptr[0] = /*_.RESP__V51*/ meltfptr[44];;

                    {
                        MELT_LOCATION("warmelt-macro.melt:5030:/ locexp");
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

                    MELT_LOCATION("warmelt-macro.melt:5025:/ clear");
                    /*clear*/ /*_.RESP__V51*/
                    meltfptr[44] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.RETURN___V55*/
                    meltfptr[40] = 0 ;
                    /*_._IFELSE___V36*/
                    meltfptr[34] = /*_.LET___V50*/ meltfptr[46];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5009:/ clear");
                    /*clear*/ /*_.LET___V50*/
                    meltfptr[46] = 0 ;
                }
                ;
            }
        ;
        /*_.LET___V32*/
        meltfptr[31] = /*_._IFELSE___V36*/ meltfptr[34];;

        MELT_LOCATION("warmelt-macro.melt:5006:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:5004:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:5002:/ clear");
        /*clear*/ /*_.XCOND__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V28*/
        meltfptr[26] = 0 ;
        /*_.LET___V16*/
        meltfptr[14] = /*_.LET___V24*/ meltfptr[23];;

        MELT_LOCATION("warmelt-macro.melt:4992:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:4986:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-macro.melt:4986:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_82_WARMELTmiMACRO_MEXPAND_IF_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_82_WARMELTmiMACRO_MEXPAND_IF*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN fromline 1716 */

    /** start of frame for meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN// fromline 1531
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
        MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<31> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN), clos) {};
        MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN() //the constructor fromline 1606
            : Melt_CallFrameWithValues<31> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<31> (fil,lin, sizeof(MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN)) {};
        MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<31> (fil,lin, sizeof(MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN


    /** end of frame for meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN fromline 1661**/

    /* end of frame for routine meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN fromline 1720 */

    /* classy proc frame meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN */
    MeltFrame_meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:5040:/ getarg");
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
            /*_#MELT_NEED_DBG__L1*/
            meltfnum[0] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5041:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:5041:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5041;
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

                        MELT_LOCATION("warmelt-macro.melt:5041:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:5041:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:5042:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:5042:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5042:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5042)?(5042):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:5042:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5043:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:5043:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[10] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5043:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5043)?(5043):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:5043:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5044:/ cond");
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

                    MELT_LOCATION("warmelt-macro.melt:5044:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5045:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L6*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:5045:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5045:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check modctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5045)?(5045):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:5045:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5046:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:5047:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V17*/
            meltfptr[16] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5048:/ getslot");
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
        MELT_LOCATION("warmelt-macro.melt:5052:/ cond");
        /*cond*/
        if (/*_#NOT__L8*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5053:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:5054:/ compute");
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
        MELT_LOCATION("warmelt-macro.melt:5055:/ cond");
        /*cond*/
        if (/*_#NOT__L10*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5056:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "missing body in WHEN"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5057:/ quasiblock");



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
        MELT_LOCATION("warmelt-macro.melt:5058:/ apply");
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
        MELT_LOCATION("warmelt-macro.melt:5059:/ quasiblock");


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
        MELT_LOCATION("warmelt-macro.melt:5064:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:5064:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:5064:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5064;
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

                        MELT_LOCATION("warmelt-macro.melt:5064:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:5064:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:5065:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.XWHEN__V27*/ meltfptr[26];;

        {
            MELT_LOCATION("warmelt-macro.melt:5065:/ locexp");
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

        MELT_LOCATION("warmelt-macro.melt:5057:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:5046:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5040:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-macro.melt:5040:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_83_WARMELTmiMACRO_MEXPAND_WHEN*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS fromline 1716 */

    /** start of frame for meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS// fromline 1531
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
        MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<31> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS), clos) {};
        MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS() //the constructor fromline 1606
            : Melt_CallFrameWithValues<31> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<31> (fil,lin, sizeof(MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS)) {};
        MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<31> (fil,lin, sizeof(MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS


    /** end of frame for meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS fromline 1661**/

    /* end of frame for routine meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS fromline 1720 */

    /* classy proc frame meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS */
    MeltFrame_meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:5076:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:5077:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:5077:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:5077:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5077;
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

                        MELT_LOCATION("warmelt-macro.melt:5077:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:5077:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:5078:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:5078:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5078:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5078)?(5078):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:5078:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5079:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:5079:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[10] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5079:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5079)?(5079):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:5079:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5080:/ cond");
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

                    MELT_LOCATION("warmelt-macro.melt:5080:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5081:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L6*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:5081:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5081:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check modctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5081)?(5081):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:5081:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5082:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:5083:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V17*/
            meltfptr[16] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5084:/ getslot");
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
        MELT_LOCATION("warmelt-macro.melt:5088:/ cond");
        /*cond*/
        if (/*_#NOT__L8*/ meltfnum[7]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5089:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:5090:/ compute");
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
        MELT_LOCATION("warmelt-macro.melt:5091:/ cond");
        /*cond*/
        if (/*_#NOT__L10*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5092:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "missing body in UNLESS"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5093:/ quasiblock");



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
        MELT_LOCATION("warmelt-macro.melt:5094:/ apply");
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
        MELT_LOCATION("warmelt-macro.melt:5095:/ quasiblock");


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
        MELT_LOCATION("warmelt-macro.melt:5101:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:5101:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:5101:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5101;
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

                        MELT_LOCATION("warmelt-macro.melt:5101:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:5101:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:5102:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.XUNLESS__V27*/ meltfptr[26];;

        {
            MELT_LOCATION("warmelt-macro.melt:5102:/ locexp");
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

        MELT_LOCATION("warmelt-macro.melt:5093:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:5082:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5076:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-macro.melt:5076:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_84_WARMELTmiMACRO_MEXPAND_UNLESS*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB fromline 1716 */

    /** start of frame for meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB// fromline 1531
        : public Melt_CallFrameWithValues<51>
    {
    public: /* fromline 1535*/
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
        MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<51> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB), clos) {};
        MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB() //the constructor fromline 1606
            : Melt_CallFrameWithValues<51> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<51> (fil,lin, sizeof(MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB)) {};
        MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<51> (fil,lin, sizeof(MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB


    /** end of frame for meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB fromline 1661**/

    /* end of frame for routine meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB fromline 1720 */

    /* classy proc frame meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB */
    MeltFrame_meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
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
    MELT_LOCATION("warmelt-macro.melt:5115:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:5116:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:5116:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:5116:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L2*/ meltfnum[1];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5116;
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

                        MELT_LOCATION("warmelt-macro.melt:5116:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:5116:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:5117:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L3*/
            meltfnum[1] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:5117:/ cond");
            /*cond*/
            if (/*_#IS_A__L3*/ meltfnum[1]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[5] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5117:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5117)?(5117):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:5117:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5118:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L4*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[2])));;
            MELT_LOCATION("warmelt-macro.melt:5118:/ cond");
            /*cond*/
            if (/*_#IS_A__L4*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V11*/ meltfptr[10] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5118:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5118)?(5118):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:5118:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5119:/ cond");
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

                    MELT_LOCATION("warmelt-macro.melt:5119:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5120:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L6*/
            meltfnum[0] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:5120:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L6*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V15*/ meltfptr[14] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5120:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check modctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5120)?(5120):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:5120:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5121:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:5122:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V17*/
            meltfptr[16] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5123:/ getslot");
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
        /*_.SYMBS__V21*/
        meltfptr[20] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5126:/ apply");
        /*apply*/
        {
            /*_.NEWENV__V22*/ meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!FRESH_ENV*/ meltfrout->tabval[4])), (melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (""), (union meltparam_un*)0, "", (union meltparam_un*)0);
        }
        ;
        /*_.BINDLIST__V23*/
        meltfptr[22] =
            (meltgc_new_list((meltobject_ptr_t)((/*!DISCR_LIST*/ meltfrout->tabval[5]))));;
        /*^compute*/
        /*_.BODYL__V24*/
        meltfptr[23] = (/*nil*/NULL);;
        MELT_LOCATION("warmelt-macro.melt:5130:/ quasiblock");


        /*^newclosure*/
        /*newclosure*/ /*_.LAMBDA___V26*/
        meltfptr[25] =
            (melt_ptr_t) meltgc_new_closure((meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_CLOSURE)))), (meltroutine_ptr_t)((/*!konst_15*/ meltfrout->tabval[15])), (2));
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V26*/ meltfptr[25])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 0>= 0 && 0< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V26*/ meltfptr[25])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V26*/ meltfptr[25])->tabval[0] = (melt_ptr_t)(/*_.NEWENV__V22*/ meltfptr[21]);
        ;
        /*^putclosedv*/
        /*putclosv*/
        melt_assertmsg("putclosv checkclo", melt_magic_discr((melt_ptr_t)(/*_.LAMBDA___V26*/ meltfptr[25])) == MELTOBMAG_CLOSURE);
        melt_assertmsg("putclosv checkoff", 1>= 0 && 1< melt_closure_size((melt_ptr_t) (/*_.LAMBDA___V26*/ meltfptr[25])));
        ((meltclosure_ptr_t)/*_.LAMBDA___V26*/ meltfptr[25])->tabval[1] = (melt_ptr_t)(/*_.BINDLIST__V23*/ meltfptr[22]);
        ;
        /*_.PROCESSLIST__V25*/
        meltfptr[24] = /*_.LAMBDA___V26*/ meltfptr[25];;
        /*^compute*/
        /*_.PAIR_TAIL__V27*/
        meltfptr[26] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
        MELT_LOCATION("warmelt-macro.melt:5162:/ compute");
        /*_.CURPAIR__V20*/
        meltfptr[19] = /*_.SETQ___V28*/ meltfptr[27] = /*_.PAIR_TAIL__V27*/ meltfptr[26];;

        MELT_CHECK_SIGNAL();
        ;
        /*_#NULL__L7*/
        meltfnum[0] =
            ((/*_.SYMBS__V21*/ meltfptr[20]) == NULL);;
        MELT_LOCATION("warmelt-macro.melt:5164:/ cond");
        /*cond*/
        if (/*_#NULL__L7*/ meltfnum[0]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {




                    {
                        MELT_LOCATION("warmelt-macro.melt:5165:/ locexp");
                        /*void*/
                        (void)0;
                    }
                    ;
                    /*clear*/ /*_._IFELSE___V29*/
                    meltfptr[28] = 0 ;
                    /*epilog*/
                }
                ;
            }
        else
            {
                MELT_LOCATION("warmelt-macro.melt:5164:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_A__L8*/
                    meltfnum[7] =
                        melt_is_instance_of((melt_ptr_t)(/*_.SYMBS__V21*/ meltfptr[20]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[1])));;
                    MELT_LOCATION("warmelt-macro.melt:5167:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L8*/ meltfnum[7]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:5168:/ quasiblock");


                                /*^getslot*/
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.SYMBS__V21*/ meltfptr[20]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 1, "LOCA_LOCATION");
                                    /*_.LOCA_LOCATION__V31*/
                                    meltfptr[30] = slot;
                                };
                                ;
                                /*^cond*/
                                /*cond*/
                                if (/*_.LOCA_LOCATION__V31*/ meltfptr[30]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*_.SYLOC__V32*/ meltfptr[31] = /*_.LOCA_LOCATION__V31*/ meltfptr[30];;
                                    }
                                else
                                    {
                                        MELT_LOCATION("warmelt-macro.melt:5168:/ cond.else");

                                        /*_.SYLOC__V32*/
                                        meltfptr[31] = /*_.LOC__V18*/ meltfptr[17];;
                                    }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:5169:/ getslot");
                                {
                                    melt_ptr_t slot=NULL, obj=NULL;
                                    obj = (melt_ptr_t)(/*_.SYMBS__V21*/ meltfptr[20]) /*=obj*/;
                                    melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
                                    /*_.SYLIST__V33*/
                                    meltfptr[32] = slot;
                                };
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-macro.melt:5171:/ apply");
                                /*apply*/
                                {
                                    union meltparam_un argtab[1];
                                    memset(&argtab, 0, sizeof(argtab));
                                    /*^apply.arg*/
                                    argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.SYLIST__V33*/ meltfptr[32];
                                    /*_.PROCESSLIST__V34*/
                                    meltfptr[33] =  melt_apply ((meltclosure_ptr_t)(/*_.PROCESSLIST__V25*/ meltfptr[24]), (melt_ptr_t)(/*_.SYLOC__V32*/ meltfptr[31]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                }
                                ;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:5172:/ locexp");
                                    /*void*/
                                    (void)0;
                                }
                                ;
                                /*_._IFELSE___V30*/
                                meltfptr[29] = /*_.PROCESSLIST__V34*/ meltfptr[33];;

                                MELT_LOCATION("warmelt-macro.melt:5168:/ clear");
                                /*clear*/ /*_.LOCA_LOCATION__V31*/
                                meltfptr[30] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.SYLOC__V32*/
                                meltfptr[31] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.SYLIST__V33*/
                                meltfptr[32] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROCESSLIST__V34*/
                                meltfptr[33] = 0 ;
                                /*epilog*/
                            }
                            ;
                        }
                    else
                        {
                            MELT_LOCATION("warmelt-macro.melt:5167:/ cond.else");

                            /*^block*/
                            /*anyblock*/
                            {


                                MELT_CHECK_SIGNAL();
                                ;
                                /*_#IS_LIST__L9*/
                                meltfnum[8] =
                                    (melt_magic_discr((melt_ptr_t)(/*_.SYMBS__V21*/ meltfptr[20])) == MELTOBMAG_LIST);;
                                MELT_LOCATION("warmelt-macro.melt:5174:/ cond");
                                /*cond*/
                                if (/*_#IS_LIST__L9*/ meltfnum[8]) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^block*/
                                        /*anyblock*/
                                        {


                                            MELT_CHECK_SIGNAL();
                                            ;
                                            MELT_LOCATION("warmelt-macro.melt:5175:/ apply");
                                            /*apply*/
                                            {
                                                union meltparam_un argtab[1];
                                                memset(&argtab, 0, sizeof(argtab));
                                                /*^apply.arg*/
                                                argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.SYMBS__V21*/ meltfptr[20];
                                                /*_.PROCESSLIST__V36*/
                                                meltfptr[31] =  melt_apply ((meltclosure_ptr_t)(/*_.PROCESSLIST__V25*/ meltfptr[24]), (melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                            }
                                            ;

                                            {
                                                MELT_LOCATION("warmelt-macro.melt:5176:/ locexp");
                                                /*void*/
                                                (void)0;
                                            }
                                            ;
                                            MELT_LOCATION("warmelt-macro.melt:5174:/ quasiblock");


                                            /*epilog*/

                                            /*^clear*/
                                            /*clear*/ /*_.PROCESSLIST__V36*/
                                            meltfptr[31] = 0 ;
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
                                            /*_#IS_MULTIPLE__L10*/
                                            meltfnum[9] =
                                                (melt_magic_discr((melt_ptr_t)(/*_.SYMBS__V21*/ meltfptr[20])) == MELTOBMAG_MULTIPLE);;
                                            MELT_LOCATION("warmelt-macro.melt:5177:/ cond");
                                            /*cond*/
                                            if (/*_#IS_MULTIPLE__L10*/ meltfnum[9]) /*then*/
                                                {
                                                    /*^cond.then*/
                                                    /*^block*/
                                                    /*anyblock*/
                                                    {


                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-macro.melt:5178:/ apply");
                                                        /*apply*/
                                                        {
                                                            union meltparam_un argtab[1];
                                                            memset(&argtab, 0, sizeof(argtab));
                                                            /*^apply.arg*/
                                                            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_LIST*/ meltfrout->tabval[5]);
                                                            /*_.MULTIPLE_TO_LIST__V38*/
                                                            meltfptr[33] =  melt_apply ((meltclosure_ptr_t)((/*!MULTIPLE_TO_LIST*/ meltfrout->tabval[16])), (melt_ptr_t)(/*_.SYMBS__V21*/ meltfptr[20]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
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
                                                            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.MULTIPLE_TO_LIST__V38*/ meltfptr[33];
                                                            /*_.PROCESSLIST__V39*/
                                                            meltfptr[31] =  melt_apply ((meltclosure_ptr_t)(/*_.PROCESSLIST__V25*/ meltfptr[24]), (melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                                                        }
                                                        ;

                                                        {
                                                            MELT_LOCATION("warmelt-macro.melt:5179:/ locexp");
                                                            /*void*/
                                                            (void)0;
                                                        }
                                                        ;
                                                        MELT_LOCATION("warmelt-macro.melt:5177:/ quasiblock");


                                                        /*epilog*/

                                                        /*^clear*/
                                                        /*clear*/ /*_.MULTIPLE_TO_LIST__V38*/
                                                        meltfptr[33] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.PROCESSLIST__V39*/
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
                                                            MELT_LOCATION("warmelt-macro.melt:5181:/ locexp");
                                                            /* error_plain */
                                                            melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "WITH_CLONED_SYMB wants a list of symbols as first argument"), (melt_ptr_t)0);
                                                        }
                                                        ;

                                                        MELT_CHECK_SIGNAL();
                                                        ;
                                                        MELT_LOCATION("warmelt-macro.melt:5182:/ quasiblock");


                                                        /*_._RETVAL___V1*/
                                                        meltfptr[0] = (/*nil*/NULL);;
                                                        MELT_LOCATION("warmelt-macro.melt:5182:/ putxtraresult");
                                                        if (!meltxrestab_ || !meltxresdescr_) goto meltlabend_rout;
                                                        if (meltxresdescr_[0] != MELTBPAR_PTR) goto meltlabend_rout;
                                                        if (meltxrestab_[0].meltbp_aptr) *(meltxrestab_[0].meltbp_aptr) = (melt_ptr_t) ((/*nil*/NULL));
                                                        ;
                                                        /*^finalreturn*/
                                                        ;
                                                        /*finalret*/
                                                        goto meltlabend_rout ;
                                                        MELT_LOCATION("warmelt-macro.melt:5180:/ quasiblock");


                                                        /*_.PROGN___V41*/
                                                        meltfptr[31] = /*_.RETURN___V40*/ meltfptr[33];;
                                                        /*^compute*/
                                                        /*_._IFELSE___V37*/
                                                        meltfptr[32] = /*_.PROGN___V41*/ meltfptr[31];;
                                                        /*epilog*/

                                                        MELT_LOCATION("warmelt-macro.melt:5177:/ clear");
                                                        /*clear*/ /*_.RETURN___V40*/
                                                        meltfptr[33] = 0 ;
                                                        /*^clear*/
                                                        /*clear*/ /*_.PROGN___V41*/
                                                        meltfptr[31] = 0 ;
                                                    }
                                                    ;
                                                }
                                            ;
                                            /*_._IFELSE___V35*/
                                            meltfptr[30] = /*_._IFELSE___V37*/ meltfptr[32];;
                                            /*epilog*/

                                            MELT_LOCATION("warmelt-macro.melt:5174:/ clear");
                                            /*clear*/ /*_#IS_MULTIPLE__L10*/
                                            meltfnum[9] = 0 ;
                                            /*^clear*/
                                            /*clear*/ /*_._IFELSE___V37*/
                                            meltfptr[32] = 0 ;
                                        }
                                        ;
                                    }
                                ;
                                /*_._IFELSE___V30*/
                                meltfptr[29] = /*_._IFELSE___V35*/ meltfptr[30];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:5167:/ clear");
                                /*clear*/ /*_#IS_LIST__L9*/
                                meltfnum[8] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_._IFELSE___V35*/
                                meltfptr[30] = 0 ;
                            }
                            ;
                        }
                    ;
                    /*_._IFELSE___V29*/
                    meltfptr[28] = /*_._IFELSE___V30*/ meltfptr[29];;
                    /*epilog*/

                    MELT_LOCATION("warmelt-macro.melt:5164:/ clear");
                    /*clear*/ /*_#IS_A__L8*/
                    meltfnum[7] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V30*/
                    meltfptr[29] = 0 ;
                }
                ;
            }
        ;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5183:/ cppif.then");
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
            meltfnum[9] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5183:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L11*/ meltfnum[9]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L12*/ meltfnum[8] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:5183:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[8];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5183;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_with_cloned_symb bindlist=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.BINDLIST__V23*/ meltfptr[22];
                            /*_.MELT_DEBUG_FUN__V43*/
                            meltfptr[31] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V42*/
                        meltfptr[33] = /*_.MELT_DEBUG_FUN__V43*/ meltfptr[31];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5183:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L12*/
                        meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V43*/
                        meltfptr[31] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V42*/ meltfptr[33] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5183:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L11*/
            meltfnum[9] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V42*/
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
        MELT_LOCATION("warmelt-macro.melt:5185:/ quasiblock");



        MELT_CHECK_SIGNAL();
        ;
        /*^apply*/
        /*apply*/
        {
            union meltparam_un argtab[3];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.NEWENV__V22*/ meltfptr[21];
            /*^apply.arg*/
            argtab[1].meltbp_aptr = (melt_ptr_t*) &/*_.MEXPANDER__V4*/ meltfptr[3];
            /*^apply.arg*/
            argtab[2].meltbp_aptr = (melt_ptr_t*) &/*_.MODCTX__V5*/ meltfptr[4];
            /*_.MBODY__V45*/
            meltfptr[30] =  melt_apply ((meltclosure_ptr_t)((/*!EXPAND_PAIRLIST_AS_TUPLE*/ meltfrout->tabval[17])), (melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19]), (MELTBPARSTR_PTR MELTBPARSTR_PTR MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;

        MELT_CHECK_SIGNAL();
        ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5188:/ apply");
        /*apply*/
        {
            union meltparam_un argtab[1];
            memset(&argtab, 0, sizeof(argtab));
            /*^apply.arg*/
            argtab[0].meltbp_aptr = (melt_ptr_t*) &(/*!DISCR_MULTIPLE*/ meltfrout->tabval[20]);
            /*_.LIST_TO_MULTIPLE__V46*/
            meltfptr[29] =  melt_apply ((meltclosure_ptr_t)((/*!LIST_TO_MULTIPLE*/ meltfrout->tabval[19])), (melt_ptr_t)(/*_.BINDLIST__V23*/ meltfptr[22]), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
        }
        ;
        MELT_LOCATION("warmelt-macro.melt:5186:/ quasiblock");


        /*^rawallocobj*/
        /*rawallocobj*/
        {
            melt_ptr_t newobj = 0;
            melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_LET*/ meltfrout->tabval[18])), (4), "CLASS_SOURCE_LET");
            /*_.INST__V48*/
            meltfptr[33] =
                newobj;
        };
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V48*/ meltfptr[33])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V48*/ meltfptr[33]), (1), (/*_.LOC__V18*/ meltfptr[17]), "LOCA_LOCATION");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SLET_BINDINGS", melt_magic_discr((melt_ptr_t)(/*_.INST__V48*/ meltfptr[33])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V48*/ meltfptr[33]), (2), (/*_.LIST_TO_MULTIPLE__V46*/ meltfptr[29]), "SLET_BINDINGS");
        ;
        /*^putslot*/
        /*putslot*/
        melt_assertmsg("checkobj putslot  _ @SLET_BODY", melt_magic_discr((melt_ptr_t)(/*_.INST__V48*/ meltfptr[33])) == MELTOBMAG_OBJECT);
        melt_putfield_object((/*_.INST__V48*/ meltfptr[33]), (3), (/*_.MBODY__V45*/ meltfptr[30]), "SLET_BODY");
        ;
        /*^touchobj*/

        melt_dbgtrace_written_object (/*_.INST__V48*/ meltfptr[33], "newly made instance");
        ;
        /*_.MLET__V47*/
        meltfptr[31] = /*_.INST__V48*/ meltfptr[33];;

#if MELT_HAVE_DEBUG
        MELT_LOCATION("warmelt-macro.melt:5192:/ cppif.then");
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
            meltfnum[7] =
                /*MELT_NEED_DBG*/
#if MELT_HAVE_DEBUG
                (/*melt_need_dbg*/ melt_need_debug ((int) 0))
#else
                0 /* no melt_need_dbg */
#endif /*MELT_HAVE_DEBUG*/
                ;;
            MELT_LOCATION("warmelt-macro.melt:5192:/ cond");
            /*cond*/
            if (/*_#MELT_NEED_DBG__L13*/ meltfnum[7]) /*then*/
                {
                    /*^cond.then*/
                    /*^block*/
                    /*anyblock*/
                    {

                        /*_#MELT_CALLCOUNT__L14*/ meltfnum[8] =
                            /* for melt_callcount */
#if MELT_HAVE_DEBUG && defined (meltcallcount)
                            meltcallcount  /* melt_callcount debugging */
#else
                            0L  /* melt_callcount without debug */
#endif /* MELT_HAVE_DEBUG melt_callcount */
                            ;;

                        MELT_CHECK_SIGNAL();
                        ;
                        MELT_LOCATION("warmelt-macro.melt:5192:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L14*/ meltfnum[8];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5192;
                            /*^apply.arg*/
                            argtab[3].meltbp_cstring =  "mexpand_with_cloned_symb result mbody=";
                            /*^apply.arg*/
                            argtab[4].meltbp_aptr = (melt_ptr_t*) &/*_.MBODY__V45*/ meltfptr[30];
                            /*_.MELT_DEBUG_FUN__V50*/
                            meltfptr[49] =  melt_apply ((meltclosure_ptr_t)((/*!MELT_DEBUG_FUN*/ meltfrout->tabval[0])), (melt_ptr_t)((/*nil*/NULL)), (MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_LONG MELTBPARSTR_CSTRING MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                        }
                        ;
                        /*_._IF___V49*/
                        meltfptr[48] = /*_.MELT_DEBUG_FUN__V50*/ meltfptr[49];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5192:/ clear");
                        /*clear*/ /*_#MELT_CALLCOUNT__L14*/
                        meltfnum[8] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.MELT_DEBUG_FUN__V50*/
                        meltfptr[49] = 0 ;
                    }
                    ;
                }
            else    /*^cond.else*/
                {

                    /*_._IF___V49*/ meltfptr[48] =  /*fromline 1341*/ NULL ;;
                }
            ;

            {
                MELT_LOCATION("warmelt-macro.melt:5192:/ locexp");
                /*void*/
                (void)0;
            }
            ;
            /*^quasiblock*/


            /*epilog*/

            /*^clear*/
            /*clear*/ /*_#MELT_NEED_DBG__L13*/
            meltfnum[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V49*/
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
        MELT_LOCATION("warmelt-macro.melt:5193:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.MBODY__V45*/ meltfptr[30];;

        {
            MELT_LOCATION("warmelt-macro.melt:5193:/ locexp");
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
        meltfptr[32] = /*_.RETURN___V51*/ meltfptr[49];;

        MELT_LOCATION("warmelt-macro.melt:5185:/ clear");
        /*clear*/ /*_.MBODY__V45*/
        meltfptr[30] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LIST_TO_MULTIPLE__V46*/
        meltfptr[29] = 0 ;
        /*^clear*/
        /*clear*/ /*_.MLET__V47*/
        meltfptr[31] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V51*/
        meltfptr[49] = 0 ;
        /*_.LET___V16*/
        meltfptr[14] = /*_.LET___V44*/ meltfptr[32];;

        MELT_LOCATION("warmelt-macro.melt:5121:/ clear");
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
        /*clear*/ /*_.SYMBS__V21*/
        meltfptr[20] = 0 ;
        /*^clear*/
        /*clear*/ /*_.NEWENV__V22*/
        meltfptr[21] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BINDLIST__V23*/
        meltfptr[22] = 0 ;
        /*^clear*/
        /*clear*/ /*_.BODYL__V24*/
        meltfptr[23] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PROCESSLIST__V25*/
        meltfptr[24] = 0 ;
        /*^clear*/
        /*clear*/ /*_.PAIR_TAIL__V27*/
        meltfptr[26] = 0 ;
        /*^clear*/
        /*clear*/ /*_.SETQ___V28*/
        meltfptr[27] = 0 ;
        /*^clear*/
        /*clear*/ /*_#NULL__L7*/
        meltfnum[0] = 0 ;
        /*^clear*/
        /*clear*/ /*_._IFELSE___V29*/
        meltfptr[28] = 0 ;
        /*^clear*/
        /*clear*/ /*_.LET___V44*/
        meltfptr[32] = 0 ;

        MELT_CHECK_SIGNAL();
        ;
        MELT_LOCATION("warmelt-macro.melt:5115:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-macro.melt:5115:/ locexp");
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
    melt_trace_end("MEXPAND_WITH_CLONED_SYMB", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_85_WARMELTmiMACRO_MEXPAND_WITH_CLONED_SYMB*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_86_WARMELTmiMACRO_LAMBDA_cl19(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
                                       const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_86_WARMELTmiMACRO_LAMBDA_cl19_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_86_WARMELTmiMACRO_LAMBDA_cl19 fromline 1716 */

    /** start of frame for meltrout_86_WARMELTmiMACRO_LAMBDA_cl19 of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_86_WARMELTmiMACRO_LAMBDA_cl19// fromline 1531
        : public Melt_CallFrameWithValues<22>
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
        MeltFrame_meltrout_86_WARMELTmiMACRO_LAMBDA_cl19(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<22> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_86_WARMELTmiMACRO_LAMBDA_cl19), clos) {};
        MeltFrame_meltrout_86_WARMELTmiMACRO_LAMBDA_cl19() //the constructor fromline 1606
            : Melt_CallFrameWithValues<22> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_86_WARMELTmiMACRO_LAMBDA_cl19)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_86_WARMELTmiMACRO_LAMBDA_cl19(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<22> (fil,lin, sizeof(MeltFrame_meltrout_86_WARMELTmiMACRO_LAMBDA_cl19)) {};
        MeltFrame_meltrout_86_WARMELTmiMACRO_LAMBDA_cl19(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<22> (fil,lin, sizeof(MeltFrame_meltrout_86_WARMELTmiMACRO_LAMBDA_cl19), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_86_WARMELTmiMACRO_LAMBDA_cl19


    /** end of frame for meltrout_86_WARMELTmiMACRO_LAMBDA_cl19 fromline 1661**/

    /* end of frame for routine meltrout_86_WARMELTmiMACRO_LAMBDA_cl19 fromline 1720 */

    /* classy proc frame meltrout_86_WARMELTmiMACRO_LAMBDA_cl19 */
    MeltFrame_meltrout_86_WARMELTmiMACRO_LAMBDA_cl19
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_86_WARMELTmiMACRO_LAMBDA_cl19 fromline 1749*/
#if ENABLE_CHECKING
    (__FILE__, __LINE__, meltclosp_) /* fromline 1753*/
#else /*ENABLE_CHECKING */
    (meltclosp_) /* fromline 1757*/
#endif /*ENABLE_CHECKING */

    ;
#define meltframe meltfram__

    melt_trace_start("LAMBDA_cl19", meltcallcount);
    /*use arguments*/
    (void) meltclosp_;
    (void)meltfirstargp_;
    (void)meltxargdescr_;
    (void)meltxargtab_;
    (void)meltxresdescr_;
    (void)meltxrestab_;
    /*getargs*/

    /*getarg#0*/
    MELT_LOCATION("warmelt-macro.melt:5130:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:5131:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_LIST_OR_NULL__L1*/
            meltfnum[0] =
                ((/*_.BLIST__V3*/ meltfptr[2]) == NULL
                 || (melt_unsafe_magic_discr((melt_ptr_t)(/*_.BLIST__V3*/ meltfptr[2])) == MELTOBMAG_LIST));;
            MELT_LOCATION("warmelt-macro.melt:5131:/ cond");
            /*cond*/
            if (/*_#IS_LIST_OR_NULL__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V5*/ meltfptr[4] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5131:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check blist"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5131)?(5131):__LINE__, __FUNCTION__);
                            ;
                        }
                        ;
                        /*clear*/ /*_._IFELSE___V5*/
                        meltfptr[4] = 0 ;
                        /*epilog*/
                    }
                    ;
                }
            ;
            /*^compute*/
            /*_.IFCPP___V4*/
            meltfptr[3] = /*_._IFELSE___V5*/ meltfptr[4];;
            /*epilog*/

            MELT_LOCATION("warmelt-macro.melt:5131:/ clear");
            /*clear*/ /*_#IS_LIST_OR_NULL__L1*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IFELSE___V5*/
            meltfptr[4] = 0 ;
        }

#else /*MELT_HAVE_DEBUG*/
        /*^cppif.else*/
        /*_.IFCPP___V4*/ meltfptr[3] = (/*nil*/NULL);
#endif /*MELT_HAVE_DEBUG*/
        ;
        /*citerblock FOREACH_IN_LIST*/
        {
            /* start foreach_in_list meltcit1__EACHLIST */
            for (/*_.CURPAIR__V6*/ meltfptr[4] = melt_list_first( (melt_ptr_t)/*_.BLIST__V3*/ meltfptr[2]);
                                   melt_magic_discr((melt_ptr_t) /*_.CURPAIR__V6*/ meltfptr[4]) == MELTOBMAG_PAIR;
                                   /*_.CURPAIR__V6*/ meltfptr[4] = melt_pair_tail((melt_ptr_t) /*_.CURPAIR__V6*/ meltfptr[4]))
                {
                    /*_.CURSYMB__V7*/ meltfptr[6] = melt_pair_head((melt_ptr_t) /*_.CURPAIR__V6*/ meltfptr[4]);



                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_NOT_A__L2*/
                    meltfnum[0] =
                        !melt_is_instance_of((melt_ptr_t)(/*_.CURSYMB__V7*/ meltfptr[6]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[0])));;
                    MELT_LOCATION("warmelt-macro.melt:5135:/ cond");
                    /*cond*/
                    if (/*_#IS_NOT_A__L2*/ meltfnum[0]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:5136:/ locexp");
                                    /* error_plain */
                                    melt_error_str((melt_ptr_t)(/*_.BLOC__V2*/ meltfptr[1]), ( "non-symbol in symbol list for (WITH_CLONED_SYMB <symbol-list> <body\
...>)"), (melt_ptr_t)0);
                                }
                                ;
                                MELT_LOCATION("warmelt-macro.melt:5137:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] =  /*fromline 1341*/ NULL ;;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:5137:/ locexp");
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
                                MELT_LOCATION("warmelt-macro.melt:5135:/ quasiblock");


                                /*_.PROGN___V10*/
                                meltfptr[9] = /*_.RETURN___V9*/ meltfptr[8];;
                                /*^compute*/
                                /*_._IF___V8*/
                                meltfptr[7] = /*_.PROGN___V10*/ meltfptr[9];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:5135:/ clear");
                                /*clear*/ /*_.RETURN___V9*/
                                meltfptr[8] = 0 ;
                                /*^clear*/
                                /*clear*/ /*_.PROGN___V10*/
                                meltfptr[9] = 0 ;
                            }
                            ;
                        }
                    else    /*^cond.else*/
                        {

                            /*_._IF___V8*/ meltfptr[7] =  /*fromline 1341*/ NULL ;;
                        }
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*^compute*/
                    /*_#IS_A__L3*/
                    meltfnum[2] =
                        melt_is_instance_of((melt_ptr_t)(/*_.CURSYMB__V7*/ meltfptr[6]), (melt_ptr_t)((/*!CLASS_KEYWORD*/ meltfrout->tabval[1])));;
                    MELT_LOCATION("warmelt-macro.melt:5138:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L3*/ meltfnum[2]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:5140:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CURSYMB__V7*/ meltfptr[6]),
                                                                  (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[2])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CURSYMB__V7*/ meltfptr[6]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                            /*_.NAMED_NAME__V11*/
                                            meltfptr[8] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.NAMED_NAME__V11*/ meltfptr[8] =  /*fromline 1341*/ NULL ;;
                                    }
                                ;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:5139:/ locexp");
                                    melt_warning_str(0, (melt_ptr_t)(/*_.BLOC__V2*/ meltfptr[1]), ( "keyword in WITH_CLONED_SYMB is discouraged"), (melt_ptr_t)(/*_.NAMED_NAME__V11*/ meltfptr[8]));
                                }
                                ;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:5138:/ clear");
                                /*clear*/ /*_.NAMED_NAME__V11*/
                                meltfptr[8] = 0 ;
                            }
                            ;
                        } /*noelse*/
                    ;

                    MELT_CHECK_SIGNAL();
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5141:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.CURSYMB__V7*/ meltfptr[6];
                        /*_.FIND_ENV__V12*/
                        meltfptr[9] =  melt_apply ((meltclosure_ptr_t)((/*!FIND_ENV*/ meltfrout->tabval[3])), (melt_ptr_t)((/*~NEWENV*/ meltfclos->tabval[0])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;
                    /*^cond*/
                    /*cond*/
                    if (/*_.FIND_ENV__V12*/ meltfptr[9]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {

                                MELT_LOCATION("warmelt-macro.melt:5143:/ cond");
                                /*cond*/
                                if (
                                    /*ifisa*/ melt_is_instance_of((melt_ptr_t)(/*_.CURSYMB__V7*/ meltfptr[6]),
                                                                  (melt_ptr_t)((/*!CLASS_NAMED*/ meltfrout->tabval[2])))
                                ) /*then*/
                                    {
                                        /*^cond.then*/
                                        /*^getslot*/
                                        {
                                            melt_ptr_t slot=NULL, obj=NULL;
                                            obj = (melt_ptr_t)(/*_.CURSYMB__V7*/ meltfptr[6]) /*=obj*/;
                                            melt_object_get_field(slot,obj, 1, "NAMED_NAME");
                                            /*_.NAMED_NAME__V13*/
                                            meltfptr[8] = slot;
                                        };
                                        ;
                                    }
                                else    /*^cond.else*/
                                    {

                                        /*_.NAMED_NAME__V13*/ meltfptr[8] =  /*fromline 1341*/ NULL ;;
                                    }
                                ;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:5142:/ locexp");
                                    melt_warning_str(0, (melt_ptr_t)(/*_.BLOC__V2*/ meltfptr[1]), ( "symbol in WITH_CLONED_SYMB hides previous definition"), (melt_ptr_t)(/*_.NAMED_NAME__V13*/ meltfptr[8]));
                                }
                                ;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:5141:/ clear");
                                /*clear*/ /*_.NAMED_NAME__V13*/
                                meltfptr[8] = 0 ;
                            }
                            ;
                        } /*noelse*/
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5144:/ quasiblock");



                    MELT_CHECK_SIGNAL();
                    ;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5147:/ quasiblock");


                    /*^rawallocobj*/
                    /*rawallocobj*/
                    {
                        melt_ptr_t newobj = 0;
                        melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_QUOTE*/ meltfrout->tabval[6])), (3), "CLASS_SOURCE_QUOTE");
                        /*_.INST__V15*/
                        meltfptr[14] =
                            newobj;
                    };
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V15*/ meltfptr[14])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V15*/ meltfptr[14]), (1), (/*_.BLOC__V2*/ meltfptr[1]), "LOCA_LOCATION");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SQUOTED", melt_magic_discr((melt_ptr_t)(/*_.INST__V15*/ meltfptr[14])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V15*/ meltfptr[14]), (2), (/*_.CURSYMB__V7*/ meltfptr[6]), "SQUOTED");
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object (/*_.INST__V15*/ meltfptr[14], "newly made instance");
                    ;
                    /*_.INST___V14*/
                    meltfptr[8] = /*_.INST__V15*/ meltfptr[14];;
                    MELT_LOCATION("warmelt-macro.melt:5147:/ blockmultialloc");
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
                        /*_.TUPLREC___V17*/
                        meltfptr[16] = (melt_ptr_t) &meltletrec_1_ptr->rtup_0__TUPLREC__x2;
                        meltletrec_1_ptr->rtup_0__TUPLREC__x2.discr = (meltobject_ptr_t)(((melt_ptr_t)(MELT_PREDEF(DISCR_MULTIPLE))));
                        meltletrec_1_ptr->rtup_0__TUPLREC__x2.nbval = 1;


                        /*^putuple*/
                        /*putupl#2*/
                        melt_assertmsg("putupl [:5147] #2 checktup", melt_magic_discr((melt_ptr_t)(/*_.TUPLREC___V17*/ meltfptr[16]))== MELTOBMAG_MULTIPLE);
                        melt_assertmsg("putupl [:5147] #2 checkoff", (0>=0 && 0< melt_multiple_length((melt_ptr_t)(/*_.TUPLREC___V17*/ meltfptr[16]))));
                        ((meltmultiple_ptr_t)(/*_.TUPLREC___V17*/ meltfptr[16]))->tabval[0] = (melt_ptr_t)(/*_.INST___V14*/ meltfptr[8]);
                        ;
                        /*^touch*/
                        meltgc_touch(/*_.TUPLREC___V17*/ meltfptr[16]);
                        ;
                        /*_.TUPLE___V16*/
                        meltfptr[15] = /*_.TUPLREC___V17*/ meltfptr[16];;
                        /*epilog*/

                        MELT_LOCATION("warmelt-macro.melt:5147:/ clear");
                        /*clear*/ /*_.TUPLREC___V17*/
                        meltfptr[16] = 0 ;
                        /*^clear*/
                        /*clear*/ /*_.TUPLREC___V17*/
                        meltfptr[16] = 0 ;
                    } /*end multiallocblock*/
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5144:/ quasiblock");


                    /*^rawallocobj*/
                    /*rawallocobj*/
                    {
                        melt_ptr_t newobj = 0;
                        melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_APPLY*/ meltfrout->tabval[4])), (4), "CLASS_SOURCE_APPLY");
                        /*_.INST__V19*/
                        meltfptr[18] =
                            newobj;
                    };
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V19*/ meltfptr[18])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V19*/ meltfptr[18]), (1), (/*_.BLOC__V2*/ meltfptr[1]), "LOCA_LOCATION");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SAPP_FUN", melt_magic_discr((melt_ptr_t)(/*_.INST__V19*/ meltfptr[18])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V19*/ meltfptr[18]), (3), ((/*!konst_5_CLONE_SYMBOL*/ meltfrout->tabval[5])), "SAPP_FUN");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SARGOP_ARGS", melt_magic_discr((melt_ptr_t)(/*_.INST__V19*/ meltfptr[18])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V19*/ meltfptr[18]), (2), (/*_.TUPLE___V16*/ meltfptr[15]), "SARGOP_ARGS");
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object (/*_.INST__V19*/ meltfptr[18], "newly made instance");
                    ;
                    /*_.APPCLONESYMB__V18*/
                    meltfptr[16] = /*_.INST__V19*/ meltfptr[18];;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5150:/ quasiblock");


                    /*^rawallocobj*/
                    /*rawallocobj*/
                    {
                        melt_ptr_t newobj = 0;
                        melt_raw_object_create(newobj,(melt_ptr_t)((/*!CLASS_SOURCE_LET_BINDING*/ meltfrout->tabval[7])), (5), "CLASS_SOURCE_LET_BINDING");
                        /*_.INST__V21*/
                        meltfptr[20] =
                            newobj;
                    };
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @LOCA_LOCATION", melt_magic_discr((melt_ptr_t)(/*_.INST__V21*/ meltfptr[20])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V21*/ meltfptr[20]), (1), (/*_.BLOC__V2*/ meltfptr[1]), "LOCA_LOCATION");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SLETB_TYPE", melt_magic_discr((melt_ptr_t)(/*_.INST__V21*/ meltfptr[20])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V21*/ meltfptr[20]), (3), ((/*!CTYPE_VALUE*/ meltfrout->tabval[8])), "SLETB_TYPE");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SLETB_BINDER", melt_magic_discr((melt_ptr_t)(/*_.INST__V21*/ meltfptr[20])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V21*/ meltfptr[20]), (2), (/*_.CURSYMB__V7*/ meltfptr[6]), "SLETB_BINDER");
                    ;
                    /*^putslot*/
                    /*putslot*/
                    melt_assertmsg("checkobj putslot  _ @SLETB_EXPR", melt_magic_discr((melt_ptr_t)(/*_.INST__V21*/ meltfptr[20])) == MELTOBMAG_OBJECT);
                    melt_putfield_object((/*_.INST__V21*/ meltfptr[20]), (4), (/*_.APPCLONESYMB__V18*/ meltfptr[16]), "SLETB_EXPR");
                    ;
                    /*^touchobj*/

                    melt_dbgtrace_written_object (/*_.INST__V21*/ meltfptr[20], "newly made instance");
                    ;
                    /*_.BINDSYMB__V20*/
                    meltfptr[19] = /*_.INST__V21*/ meltfptr[20];;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5157:/ apply");
                    /*apply*/
                    {
                        union meltparam_un argtab[1];
                        memset(&argtab, 0, sizeof(argtab));
                        /*^apply.arg*/
                        argtab[0].meltbp_aptr = (melt_ptr_t*) &/*_.BINDSYMB__V20*/ meltfptr[19];
                        /*_.PUT_ENV__V22*/
                        meltfptr[21] =  melt_apply ((meltclosure_ptr_t)((/*!PUT_ENV*/ meltfrout->tabval[9])), (melt_ptr_t)((/*~NEWENV*/ meltfclos->tabval[0])), (MELTBPARSTR_PTR ""), argtab, "", (union meltparam_un*)0);
                    }
                    ;

                    {
                        MELT_LOCATION("warmelt-macro.melt:5158:/ locexp");
                        meltgc_append_list((melt_ptr_t)((/*~BINDLIST*/ meltfclos->tabval[1])), (melt_ptr_t)(/*_.BINDSYMB__V20*/ meltfptr[19]));
                    }
                    ;

                    MELT_LOCATION("warmelt-macro.melt:5144:/ clear");
                    /*clear*/ /*_.INST___V14*/
                    meltfptr[8] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.TUPLE___V16*/
                    meltfptr[15] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.APPCLONESYMB__V18*/
                    meltfptr[16] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.BINDSYMB__V20*/
                    meltfptr[19] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_.PUT_ENV__V22*/
                    meltfptr[21] = 0 ;
                } /* end foreach_in_list meltcit1__EACHLIST */
            /*_.CURPAIR__V6*/ meltfptr[4] = NULL;
            /*_.CURSYMB__V7*/
            meltfptr[6] = NULL;


            /*citerepilog*/

            MELT_LOCATION("warmelt-macro.melt:5132:/ clear");
            /*clear*/ /*_.CURPAIR__V6*/
            meltfptr[4] = 0 ;
            /*^clear*/
            /*clear*/ /*_.CURSYMB__V7*/
            meltfptr[6] = 0 ;
            /*^clear*/
            /*clear*/ /*_#IS_NOT_A__L2*/
            meltfnum[0] = 0 ;
            /*^clear*/
            /*clear*/ /*_._IF___V8*/
            meltfptr[7] = 0 ;
            /*^clear*/
            /*clear*/ /*_#IS_A__L3*/
            meltfnum[2] = 0 ;
            /*^clear*/
            /*clear*/ /*_.FIND_ENV__V12*/
            meltfptr[9] = 0 ;
        } /*endciterblock FOREACH_IN_LIST*/
        ;

        MELT_CHECK_SIGNAL();
        ;
        /*epilog*/

        MELT_LOCATION("warmelt-macro.melt:5130:/ clear");
        /*clear*/ /*_.IFCPP___V4*/
        meltfptr[3] = 0 ;
    }

    ;
    goto meltlabend_rout;
meltlabend_rout:
    melt_trace_end("LAMBDA_cl19", meltcallcount);
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_86_WARMELTmiMACRO_LAMBDA_cl19_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_86_WARMELTmiMACRO_LAMBDA_cl19*/





melt_ptr_t MELT_MODULE_VISIBILITY
meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF(meltclosure_ptr_t meltclosp_, melt_ptr_t meltfirstargp_, const melt_argdescr_cell_t meltxargdescr_[], union meltparam_un *meltxargtab_,
        const melt_argdescr_cell_t meltxresdescr_[], union meltparam_un *meltxrestab_)
{
    long current_blocklevel_signals_meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF_melt =  melt_blocklevel_signals;

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

    /* start of frame for routine meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF fromline 1716 */

    /** start of frame for meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF of CLASS_PROCROUTINEOBJ from 1518**/

    /*curframdeclclassy*/ class MeltFrame_meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF// fromline 1531
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
        MeltFrame_meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF(meltclosure_ptr_t clos) //the closure constructor fromline 1594
            : Melt_CallFrameWithValues<53> (
#if ENABLE_CHECKING /*fromline 1598*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1602*/
                sizeof(MeltFrame_meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF), clos) {};
        MeltFrame_meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF() //the constructor fromline 1606
            : Melt_CallFrameWithValues<53> (
#if ENABLE_CHECKING /*fromline 1610*/
                __FILE__, __LINE__,
#endif /* ENABLE_CHECKING fromline 1614*/
                sizeof(MeltFrame_meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF)) {};
#if ENABLE_CHECKING /*fromline 1618*/
        MeltFrame_meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF(const char*fil, int lin) //the constructor fromline 1620
            : Melt_CallFrameWithValues<53> (fil,lin, sizeof(MeltFrame_meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF)) {};
        MeltFrame_meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF(const char*fil, int lin, meltclosure_ptr_t clos) //the closure constructor fromline 1625
            : Melt_CallFrameWithValues<53> (fil,lin, sizeof(MeltFrame_meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF), clos) {};
#endif /* ENABLE_CHECKING fromline 1630*/

    }; // end  class MeltFrame_meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF


    /** end of frame for meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF fromline 1661**/

    /* end of frame for routine meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF fromline 1720 */

    /* classy proc frame meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF */
    MeltFrame_meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF
    meltfram__ /*declfrastruct fromline 1744*/
    /*classyprocarg meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF fromline 1749*/
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
    MELT_LOCATION("warmelt-macro.melt:5205:/ getarg");
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
        MELT_LOCATION("warmelt-macro.melt:5206:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L1*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]), (melt_ptr_t)((/*!CLASS_SEXPR*/ meltfrout->tabval[0])));;
            MELT_LOCATION("warmelt-macro.melt:5206:/ cond");
            /*cond*/
            if (/*_#IS_A__L1*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V7*/ meltfptr[6] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5206:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check sexpr"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5206)?(5206):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:5206:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5207:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_A__L2*/
            meltfnum[0] =
                melt_is_instance_of((melt_ptr_t)(/*_.ENV__V3*/ meltfptr[2]), (melt_ptr_t)((/*!CLASS_ENVIRONMENT*/ meltfrout->tabval[1])));;
            MELT_LOCATION("warmelt-macro.melt:5207:/ cond");
            /*cond*/
            if (/*_#IS_A__L2*/ meltfnum[0]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V9*/ meltfptr[8] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5207:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check env"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5207)?(5207):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:5207:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5208:/ cond");
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

                    MELT_LOCATION("warmelt-macro.melt:5208:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5209:/ cppif.then");
        /*^block*/
        /*anyblock*/
        {


            MELT_CHECK_SIGNAL();
            ;
            /*_#IS_OBJECT__L4*/
            meltfnum[3] =
                (melt_magic_discr((melt_ptr_t)(/*_.MODCTX__V5*/ meltfptr[4])) == MELTOBMAG_OBJECT);;
            MELT_LOCATION("warmelt-macro.melt:5209:/ cond");
            /*cond*/
            if (/*_#IS_OBJECT__L4*/ meltfnum[3]) /*then*/
                {
                    /*^cond.then*/
                    /*_._IFELSE___V13*/ meltfptr[12] = (/*nil*/NULL);;
                }
            else
                {
                    MELT_LOCATION("warmelt-macro.melt:5209:/ cond.else");

                    /*^block*/
                    /*anyblock*/
                    {




                        {
                            /*^locexp*/
                            melt_assert_failed(( "check modctx"),( "warmelt-macro.melt")?( "warmelt-macro.melt"):__FILE__,
                                               (5209)?(5209):__LINE__, __FUNCTION__);
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

            MELT_LOCATION("warmelt-macro.melt:5209:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5210:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:5210:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:5210:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L6*/ meltfnum[5];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5210;
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

                        MELT_LOCATION("warmelt-macro.melt:5210:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:5210:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:5211:/ quasiblock");


        MELT_LOCATION("warmelt-macro.melt:5212:/ getslot");
        {
            melt_ptr_t slot=NULL, obj=NULL;
            obj = (melt_ptr_t)(/*_.SEXPR__V2*/ meltfptr[1]) /*=obj*/;
            melt_object_get_field(slot,obj, 2, "SEXP_CONTENTS");
            /*_.CONT__V17*/
            meltfptr[12] = slot;
        };
        ;
        MELT_LOCATION("warmelt-macro.melt:5213:/ getslot");
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
        MELT_LOCATION("warmelt-macro.melt:5217:/ cond");
        /*cond*/
        if (/*_#NOT__L8*/ meltfnum[3]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5218:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:5219:/ compute");
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
        MELT_LOCATION("warmelt-macro.melt:5220:/ cond");
        /*cond*/
        if (/*_#NOT__L10*/ meltfnum[9]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {


                    {
                        MELT_LOCATION("warmelt-macro.melt:5221:/ locexp");
                        /* error_plain */
                        melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "missing then in CPPIF"), (melt_ptr_t)0);
                    }
                    ;
                    /*epilog*/
                }
                ;
            } /*noelse*/
        ;
        MELT_LOCATION("warmelt-macro.melt:5222:/ quasiblock");



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
        MELT_LOCATION("warmelt-macro.melt:5223:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:5223:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:5223:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L12*/ meltfnum[11];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5223;
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

                        MELT_LOCATION("warmelt-macro.melt:5223:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:5223:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:5225:/ cond");
        /*cond*/
        if (/*_#IS_STRING__L13*/ meltfnum[11]) /*then*/
            {
                /*^cond.then*/
                /*_._IFELSE___V28*/ meltfptr[26] = (/*nil*/NULL);;
            }
        else
            {
                MELT_LOCATION("warmelt-macro.melt:5225:/ cond.else");

                /*^block*/
                /*anyblock*/
                {


                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_A__L14*/
                    meltfnum[10] =
                        melt_is_instance_of((melt_ptr_t)(/*_.XCOND__V25*/ meltfptr[24]), (melt_ptr_t)((/*!CLASS_SYMBOL*/ meltfrout->tabval[4])));;
                    MELT_LOCATION("warmelt-macro.melt:5226:/ cond");
                    /*cond*/
                    if (/*_#IS_A__L14*/ meltfnum[10]) /*then*/
                        {
                            /*^cond.then*/
                            /*_._IFELSE___V29*/ meltfptr[25] = (/*nil*/NULL);;
                        }
                    else
                        {
                            MELT_LOCATION("warmelt-macro.melt:5226:/ cond.else");

                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:5228:/ locexp");
                                    /* error_plain */
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "invalid cpp-condition in CPPIF - string or symbol expected"), (melt_ptr_t)0);
                                }
                                ;

                                MELT_CHECK_SIGNAL();
                                ;
                                MELT_LOCATION("warmelt-macro.melt:5229:/ quasiblock");


                                /*_._RETVAL___V1*/
                                meltfptr[0] = (/*nil*/NULL);;

                                {
                                    MELT_LOCATION("warmelt-macro.melt:5229:/ locexp");
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
                                MELT_LOCATION("warmelt-macro.melt:5227:/ quasiblock");


                                /*_.PROGN___V31*/
                                meltfptr[30] = /*_.RETURN___V30*/ meltfptr[29];;
                                /*^compute*/
                                /*_._IFELSE___V29*/
                                meltfptr[25] = /*_.PROGN___V31*/ meltfptr[30];;
                                /*epilog*/

                                MELT_LOCATION("warmelt-macro.melt:5226:/ clear");
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

                    MELT_LOCATION("warmelt-macro.melt:5225:/ clear");
                    /*clear*/ /*_#IS_A__L14*/
                    meltfnum[10] = 0 ;
                    /*^clear*/
                    /*clear*/ /*_._IFELSE___V29*/
                    meltfptr[25] = 0 ;
                }
                ;
            }
        ;
        MELT_LOCATION("warmelt-macro.melt:5230:/ quasiblock");


        /*_.CURTHEN__V33*/
        meltfptr[30] =
            (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
        /*^compute*/
        /*_.PAIR_TAIL__V34*/
        meltfptr[25] =
            (melt_pair_tail((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;
        MELT_LOCATION("warmelt-macro.melt:5231:/ compute");
        /*_.CURPAIR__V20*/
        meltfptr[19] = /*_.SETQ___V35*/ meltfptr[34] = /*_.PAIR_TAIL__V34*/ meltfptr[25];;
        MELT_LOCATION("warmelt-macro.melt:5232:/ quasiblock");



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
        MELT_LOCATION("warmelt-macro.melt:5235:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:5235:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:5235:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L16*/ meltfnum[15];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5235;
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

                        MELT_LOCATION("warmelt-macro.melt:5235:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:5235:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:5236:/ cond");
        /*cond*/
        if (/*_#IS_PAIR__L17*/ meltfnum[15]) /*then*/
            {
                /*^cond.then*/
                /*^block*/
                /*anyblock*/
                {

                    MELT_LOCATION("warmelt-macro.melt:5237:/ quasiblock");


                    /*_.CURELSE__V41*/
                    meltfptr[39] =
                        (melt_pair_head((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])));;

                    MELT_CHECK_SIGNAL();
                    ;
                    MELT_LOCATION("warmelt-macro.melt:5238:/ apply");
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
                    MELT_LOCATION("warmelt-macro.melt:5239:/ cppif.then");
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
                        MELT_LOCATION("warmelt-macro.melt:5239:/ cond");
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
                                    MELT_LOCATION("warmelt-macro.melt:5239:/ apply");
                                    /*apply*/
                                    {
                                        union meltparam_un argtab[5];
                                        memset(&argtab, 0, sizeof(argtab));
                                        /*^apply.arg*/
                                        argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L19*/ meltfnum[18];
                                        /*^apply.arg*/
                                        argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                                        /*^apply.arg*/
                                        argtab[2].meltbp_long = 5239;
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

                                    MELT_LOCATION("warmelt-macro.melt:5239:/ clear");
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
                            MELT_LOCATION("warmelt-macro.melt:5239:/ locexp");
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
                    MELT_LOCATION("warmelt-macro.melt:5240:/ compute");
                    /*_.CURPAIR__V20*/
                    meltfptr[19] = /*_.SETQ___V46*/ meltfptr[42] = /*_.PAIR_TAIL__V45*/ meltfptr[43];;
                    MELT_LOCATION("warmelt-macro.melt:5241:/ compute");
                    /*_.XELSE__V38*/
                    meltfptr[37] = /*_.SETQ___V47*/ meltfptr[46] = /*_.GOTXELSE__V42*/ meltfptr[38];;

                    MELT_CHECK_SIGNAL();
                    ;
                    /*_#IS_PAIR__L20*/
                    meltfnum[18] =
                        (melt_magic_discr((melt_ptr_t)(/*_.CURPAIR__V20*/ meltfptr[19])) == MELTOBMAG_PAIR);;
                    MELT_LOCATION("warmelt-macro.melt:5242:/ cond");
                    /*cond*/
                    if (/*_#IS_PAIR__L20*/ meltfnum[18]) /*then*/
                        {
                            /*^cond.then*/
                            /*^block*/
                            /*anyblock*/
                            {


                                {
                                    MELT_LOCATION("warmelt-macro.melt:5243:/ locexp");
                                    /* error_plain */
                                    melt_error_str((melt_ptr_t)(/*_.LOC__V18*/ meltfptr[17]), ( "CPPIF with more than three sons"), (melt_ptr_t)0);
                                }
                                ;
                                /*epilog*/
                            }
                            ;
                        } /*noelse*/
                    ;

                    MELT_LOCATION("warmelt-macro.melt:5237:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5244:/ quasiblock");



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
        MELT_LOCATION("warmelt-macro.melt:5251:/ cppif.then");
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
            MELT_LOCATION("warmelt-macro.melt:5251:/ cond");
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
                        MELT_LOCATION("warmelt-macro.melt:5251:/ apply");
                        /*apply*/
                        {
                            union meltparam_un argtab[5];
                            memset(&argtab, 0, sizeof(argtab));
                            /*^apply.arg*/
                            argtab[0].meltbp_long = /*_#MELT_CALLCOUNT__L22*/ meltfnum[18];
                            /*^apply.arg*/
                            argtab[1].meltbp_cstring =  "warmelt-macro.melt";
                            /*^apply.arg*/
                            argtab[2].meltbp_long = 5251;
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

                        MELT_LOCATION("warmelt-macro.melt:5251:/ clear");
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
                MELT_LOCATION("warmelt-macro.melt:5251:/ locexp");
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
        MELT_LOCATION("warmelt-macro.melt:5252:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.RESP__V49*/ meltfptr[38];;

        {
            MELT_LOCATION("warmelt-macro.melt:5252:/ locexp");
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

        MELT_LOCATION("warmelt-macro.melt:5244:/ clear");
        /*clear*/ /*_.RESP__V49*/
        meltfptr[38] = 0 ;
        /*^clear*/
        /*clear*/ /*_.RETURN___V53*/
        meltfptr[46] = 0 ;
        /*_.LET___V36*/
        meltfptr[35] = /*_.LET___V48*/ meltfptr[39];;

        MELT_LOCATION("warmelt-macro.melt:5232:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:5230:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:5222:/ clear");
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

        MELT_LOCATION("warmelt-macro.melt:5211:/ clear");
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
        MELT_LOCATION("warmelt-macro.melt:5205:/ quasiblock");


        /*_._RETVAL___V1*/
        meltfptr[0] = /*_.LET___V16*/ meltfptr[14];;

        {
            MELT_LOCATION("warmelt-macro.melt:5205:/ locexp");
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
    melt_blocklevel_signals = current_blocklevel_signals_meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF_melt;
    return (melt_ptr_t)(/*_._RETVAL___V1*/ meltfptr[0]);
#undef meltcallcount
#undef meltfram__
#undef MELTFRAM_NBVARNUM
#undef MELTFRAM_NBVARPTR
} /*end meltrout_87_WARMELTmiMACRO_MEXPAND_CPPIF*/



/**** end of warmelt-macro+03.cc ****/
